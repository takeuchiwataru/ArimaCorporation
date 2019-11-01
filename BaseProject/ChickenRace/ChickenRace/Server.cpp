//=============================================================================
//
// サーバー処理 [Server.cpp]
// Author : Ryo Sugimoto
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS	//Fire Wall突破
#include <stdio.h>				//インクルドファイル
#include <stdlib.h>				//ランド用
#include <time.h>				//同じくランド用
#include <string.h>				//ストリング使用のため
#include <math.h>
#include"main.h"
#include"Server.h"
#include"manager.h"
#include <windows.h>
#include <process.h>

//==================================================================================================//
//    * 静的変数宣言 *
//==================================================================================================//
CServer		*CServer::m_pmyServer = NULL;					//自身のポインタ
CClient		*CClient::m_pmyClient = NULL;					//クライアントのポインタ
int			CServer::m_nMaxClient = 0;						//クライアントの数
int			CServer::m_nRand = 0;							//クライアント含め共有のランダムな数

//==================================================================================================//
//    * プロトタイプ宣言 *
//==================================================================================================//
unsigned __stdcall ServerUpdate(void *);
unsigned __stdcall Connect(void *);
unsigned __stdcall Send0(void *);
unsigned __stdcall Send1(void *);
unsigned __stdcall Send2(void *);
unsigned __stdcall Send3(void *);
unsigned __stdcall ClientRecv(void *);

//==================================================================================================//
//    * サーバーの初期化処理 *
//==================================================================================================//
HRESULT CServer::Init(void)
{
	m_pmyServer = this;

	WSADATA WsaData;
	WSAStartup(MAKEWORD(2, 0), &WsaData); //初期化

	//サーバー側のソケットの作成
	m_sockServer = socket(AF_INET, SOCK_STREAM, 0);

	//ソケットの設定
	m_addrServer.sin_family = AF_INET;				//接続の形式
	m_addrServer.sin_port = htons(12345);				//番号の設定
	m_addrServer.sin_addr.S_un.S_addr = INADDR_ANY;	//よくわかんない

	//ソケットに名称を付与
	bind(m_sockServer, (struct sockaddr *)&m_addrServer, sizeof(m_addrServer));

	//クライアントからの接続要求を持てる状態にする
	listen(m_sockServer, 5);

	m_bEnd = false;
	for (int nCount = 0; nCount < MAX_CLIENT; nCount++)
	{//繋がっていない状態にする
		m_bConnect[nCount] = false;
		m_bReady[nCount] = false;
	}
	strcpy(m_aStr, "-1");
	strcpy(m_aInformation, "HELLO_ONLINE");

	//接続をスレッドに分ける
	DWORD thID;
	m_hThConnect	= (HANDLE)_beginthreadex(NULL, 0, Connect, NULL, 0, (unsigned int*)&thID);
	m_hThUpdate		= (HANDLE)_beginthreadex(NULL, 0, ServerUpdate, NULL, 0, (unsigned int*)&thID);

	return S_OK;
}
//==================================================================================================//
//    * サーバーの更新処理 *
//==================================================================================================//
void	CServer::Update(void)
{
	//INVALID_SOCET	//接続失敗のマクロ
	int		nMaxClient = m_nMaxClient;

	if (nMaxClient <= 0) { return; }

	strcpy(&m_aInformation[0], "");
	wsprintf(&m_aInformation[0], "%d %d %d ", FLAG_INPUT, nMaxClient, 9);
	for (int nCount = 0; nCount < nMaxClient; nCount++)
	{//受信
	 //接続されていないなら無視
		if (!m_bConnect[nCount]) { continue; }
		//書き込み
		strcpy(m_aStr, "");
		recv(m_sockClient[nCount], m_aStr, sizeof(m_aStr), 0);	//データ受信とそのバイト数を返す
		if (m_bEnd) { break; }
		else if (strcmp(m_aStr, "-1") == 0)
		{
			m_bConnect[nCount] = false;
		}
		else
		{//通常通り受け取ったなら 合成
			int nLength = strlen(m_aStr);
			char aBox;
			for (int nCnt = 0; nCnt < MAX_STRING; nCnt++)
			{
				aBox = m_aStr[nCnt];
				if (aBox == 'A')
				{
					m_aStr[nCnt] = '\0';
					break;
				}
			}
			strcat(&m_aInformation[0], &m_aStr[0]);
			nCount = nCount;
		}
	}
	if (m_bEnd)
	{//終了しているなら文字列を変更
		wsprintf(&m_aInformation[0], "%d %d ", FLAG_END, nMaxClient);
	}
	//CManager::GetDLog()->Printf(CDebugLog::MODE_LOG, "%s\n", m_aInformation);

	for (int nCount = 0; nCount < nMaxClient; nCount++)
	{//インプットを全員に配布
		//接続されていないなら無視
		if (!m_bConnect[nCount]) { continue; }
		m_bReady[nCount] = true;
		//m_bMessage[nCount] = true;
	}

	if (m_bEnd) { return; }
	for (int nCnt = 0; nCnt < nMaxClient; nCnt++)
	{
		if (m_bReady[nCnt]) 
		{ nCnt--; }
	}
}
//==================================================================================================//
//    * サーバーの終了処理 *
//==================================================================================================//
void	CServer::Uninit(void)
{
	m_bEnd = true;
	CClient *pClient;
	DWORD thID;
	int		nCntLoop = 0;

	while (1) 
	{//更新の稼働確認
		GetExitCodeThread(m_hThUpdate, &thID);
		if (thID == STILL_ACTIVE)
		{//稼働中なら
			CClient *&pClient = CClient::GetpmyClient();
			if (pClient != NULL && nCntLoop == 0)
			{ pClient->EndSend(); }
			nCntLoop++;
		}
		else { break; }
	}
	while (1)
	{//接続の稼働確認
		GetExitCodeThread(m_hThConnect, &thID);
		if (thID == STILL_ACTIVE)
		{//稼働中なら
			if (m_nMaxClient < MAX_CLIENT && nCntLoop % 1000 == 0)
			{//クライアントが足りていないなら
				pClient = new CClient;
				pClient->Init();
				pClient->Uninit();
			}
			nCntLoop++;
		}
		else { break; }
	}
	for (int nCount = 0; nCount < m_nMaxClient; nCount++)
	{
		while (1)
		{//接続の稼働確認
			GetExitCodeThread(m_hThSend[nCount], &thID);
			if (thID == STILL_ACTIVE)
			{//稼働中なら
			}
			else { break; }
		}
	}
	//スレッドの解放	稼働確認も後で作ろうね！
	if (m_hThConnect != NULL) { CloseHandle(m_hThConnect); }
	if (m_hThUpdate != NULL) { CloseHandle(m_hThUpdate); }

	//ソケット（クライアント）の解放
	for (int nCount = 0; nCount < MAX_CLIENT; nCount++)
	{
		closesocket(m_sockClient[nCount]);
	}
	//ソケット（サーバー）の解放
	closesocket(m_sockServer);

	m_nMaxClient = 0;
	m_pmyServer = NULL;
	delete this;
}
//==================================================================================================//
//    * サーバーの受付終了処理 *
//==================================================================================================//
void	CServer::Stop(void)	
{
	CClient *pClient;
	int		nCntLoop = 0;
	DWORD thID;

	while (1)
	{//接続の稼働確認
		GetExitCodeThread(m_hThConnect, &thID);
		if (thID == STILL_ACTIVE)
		{//稼働中なら
			if (m_nMaxClient < MAX_CLIENT && nCntLoop % 1000 == 0)
			{//クライアントが足りていないなら
				pClient = new CClient;
				pClient->Init();
				pClient->Uninit();
			}
			nCntLoop++;
		}
		else { break; }
	}
}
//==================================================================================================//
//    * サーバーの更新処理 *
//==================================================================================================//
int	CServer::Rand(void)
{
	if (m_pmyServer != NULL)
	{
		m_nRand++;
		return m_nRand;
	}
	else
	{
		return rand();
	}
}
//==================================================================================================//
//    * サーバーの更新処理 *
//==================================================================================================//
unsigned __stdcall ServerUpdate(void *lpx)
{
	CServer *&pServer = CServer::GetpmyServer();
	while (1)
	{
		pServer->Update();
		if (pServer->GetbEnd()) 
		{ break; }
	}

	return 0;
}

//==================================================================================================//
//    * サーバーの接続処理 *
//==================================================================================================//
unsigned __stdcall Connect(void *lpx)
{
	CServer *&pServer = CServer::GetpmyServer();
	sockaddr_in &addrClient = pServer->GetaddrClient();
	SOCKET		&sockServer = pServer->GetsockServer();
	int &nMaxClient = pServer->GetnMaxClient();
	int nOldClient = nMaxClient;
	char aStr[1064];
	int nLength;

	//クライアントからの接続要求を受け付ける
	nLength = sizeof(addrClient);

	for (int nCount = nMaxClient; nCount < MAX_CLIENT; nCount++)
	{//クライアント分接続
		if (pServer->GetbConnect(nCount)) { continue; }	//接続されているなら無視
		SOCKET		&sockClient = pServer->GetsockClient(nCount);
		sockClient = accept(sockServer, (struct sockaddr *)&addrClient, &nLength);

		//プレイヤー番号をお知らせ
		wsprintf(aStr, "%d %d %d", CServer::FLAG_CONNECT, nMaxClient + 1, nCount);
		send(sockClient, aStr, strlen(aStr), 0);
		if (pServer->GetbEnd()) { return 0; }

		//接続後本体にお知らせ
		DWORD thID;
		switch (nCount)
		{
		case 0:	pServer->GethThSend(nCount) = (HANDLE)_beginthreadex(NULL, 0, Send0, NULL, 0, (unsigned int*)&thID);	break;
		case 1:	pServer->GethThSend(nCount) = (HANDLE)_beginthreadex(NULL, 0, Send1, NULL, 0, (unsigned int*)&thID);	break;
		case 2:	pServer->GethThSend(nCount) = (HANDLE)_beginthreadex(NULL, 0, Send2, NULL, 0, (unsigned int*)&thID);	break;
		case 3:	pServer->GethThSend(nCount) = (HANDLE)_beginthreadex(NULL, 0, Send3, NULL, 0, (unsigned int*)&thID);	break;
		}
		pServer->GetbConnect(nCount) = true;
		nMaxClient++; 

		//do
		//{
		//	nMaxClient = pServer->GetnMaxClient();
		//} while (nOldClient == nMaxClient);
		//nOldClient = nMaxClient;
	}
	return 0;
}
//==================================================================================================//
//    * サーバーのメッセージ送信処理　0番 *
//==================================================================================================//
unsigned __stdcall Send0(void *)
{
	int		nNumber = 0;
	CServer	*&pServer	= CServer::GetpmyServer();
	while (1)
	{
		if (pServer->GetbReady(nNumber))
		{
			SOCKET	&sockClient = pServer->GetsockClient(nNumber);
			char	*aInformaion = pServer->GetaInformation();

			//クライアントへデータ（メッセージを送信）
			send(sockClient, aInformaion, strlen(aInformaion), 0);
			pServer->GetbReady(nNumber) = false;
		}
		if (pServer->GetbEnd()) { break; }
	}
	return 0;
}
//==================================================================================================//
//    * サーバーのメッセージ送信処理　1番 *
//==================================================================================================//
unsigned __stdcall Send1(void *)
{
	int		nNumber = 1;
	CServer	*&pServer = CServer::GetpmyServer();
	while (1)
	{
		if (pServer->GetbReady(nNumber))
		{
			SOCKET	&sockClient = pServer->GetsockClient(nNumber);
			char	*aInformaion = pServer->GetaInformation();

			//クライアントへデータ（メッセージを送信）
			send(sockClient, aInformaion, strlen(aInformaion), 0);
			pServer->GetbReady(nNumber) = false;
		}
		if (pServer->GetbEnd()) { break; }
	}
	return 0;
}
//==================================================================================================//
//    * サーバーのメッセージ送信処理　2番 *
//==================================================================================================//
unsigned __stdcall Send2(void *)
{
	int		nNumber = 2;
	CServer	*&pServer = CServer::GetpmyServer();
	while (1)
	{
		if (pServer->GetbReady(nNumber))
		{
			SOCKET	&sockClient = pServer->GetsockClient(nNumber);
			char	*aInformaion = pServer->GetaInformation();

			//クライアントへデータ（メッセージを送信）
			send(sockClient, aInformaion, strlen(aInformaion), 0);
			pServer->GetbReady(nNumber) = false;
		}
		if (pServer->GetbEnd()) { break; }
	}
	return 0;
}
//==================================================================================================//
//    * サーバーのメッセージ送信処理　3番 *
//==================================================================================================//
unsigned __stdcall Send3(void *)
{
	int		nNumber = 3;
	CServer	*&pServer = CServer::GetpmyServer();
	while (1)
	{
		if (pServer->GetbReady(nNumber))
		{
			SOCKET	&sockClient = pServer->GetsockClient(nNumber);
			char	*aInformaion = pServer->GetaInformation();

			//クライアントへデータ（メッセージを送信）
			send(sockClient, aInformaion, strlen(aInformaion), 0);
			pServer->GetbReady(nNumber) = false;
		}
		if (pServer->GetbEnd()) { break; }
	}
	return 0;
}

//==================================================================================================//
//    * クライアントの初期化処理 *
//==================================================================================================//
HRESULT CClient::Init(void)
{
	WSADATA WsaData;
	char	aStr[32];
	WSAStartup(MAKEWORD(2, 0), &WsaData); //初期化

	//サーバー側のソケットの作成
	m_sockClient = socket(AF_INET, SOCK_STREAM, 0);

	//ソケットの設定
	m_addrServer.sin_family = AF_INET;				//接続の形式
	m_addrServer.sin_port = htons(12345);			//番号の設定
	m_addrServer.sin_addr.S_un.S_addr = inet_addr(IPLoad(aStr));	//IPアドレス

	//サーバーに接続
	int nLoop;
	do
	{
		nLoop = connect(m_sockClient, (struct sockaddr *)&m_addrServer, sizeof(m_addrServer));

	} while (nLoop == -1);
	//サーバーからデータ（メッセージの受信）
	memset(m_aStr, 0, sizeof(m_aStr));
	m_nLengthData = recv(m_sockClient, m_aStr, sizeof(m_aStr), 0);	//データ受信とそのバイト数を返す

	Reflect();

	//送信用文字列を構築
	CClient::Assign();
	send(m_sockClient, m_aAssign, strlen(m_aAssign), 0);

	//スレッドで分ける
	m_bEnd = false;
	m_bRecv = false;
	//m_hThRecv = (HANDLE)_beginthreadex(NULL, 0, ClientRecv, NULL, 0, (unsigned int*)&thID);

	strcpy(m_aStr, "NULL\n");
	GetpmyClient() = this;
	return S_OK;
}
//==================================================================================================//
//    * クライアントの更新処理 *
//==================================================================================================//
void	CClient::Update(void)
{
	//インプット更新
	//受信
	Takes();

	////送信
	//send(m_sockClient, m_aAssign, strlen(&m_aAssign[0]), 0);

	//受信インプット反映
	Reflect();

	//更新
}
//==================================================================================================//
//    * クライアントの初期化処理 *
//==================================================================================================//
void	CClient::Takes(void)
{
	//表示
	//CManager::GetDLog()->Printf(CDebugLog::MODE_LOG, &m_aStr[0]);
	
	//送信用文字列を構築
	CClient::Assign();

	//サーバーからデータ（メッセージの受信）
	memset(m_aStr, 0, sizeof(m_aStr));
	m_nLengthData = recv(m_sockClient, m_aStr, sizeof(m_aStr), 0);	//データ受信とそのバイト数を返す

	//while (!m_bRecv) {}

	//送信
	send(m_sockClient, m_aAssign, strlen(&m_aAssign[0]), 0);
	m_bRecv = false;
}
//==================================================================================================//
//    * クライアントの初期化処理 *
//==================================================================================================//
void	CClient::Uninit(void)
{
	DWORD thID;

	m_bEnd = true;
	closesocket(m_sockClient);	//ソケット（クライアント）の解放
	WSACleanup();

	while (1)
	{//更新の稼働確認
		GetExitCodeThread(m_hThRecv, &thID);
		if (thID == STILL_ACTIVE)
		{//稼働中ならサーバーからの終了を待つ
		}
		else { break; }
	}
	GetpmyClient() = NULL;

	delete this;
}
//==================================================================================================//
//    * クライアントのIPアドレス取得処理 *
//==================================================================================================//
char	*CClient::IPLoad(char *aStr)
{
	FILE *pFile;
	char ReadText[256];		// 読み込んだ文字列を入れておく
	char HeadText[256];		// 比較用
	char DustBox[32];		// 使用しないものを入れておく

	// ファイルオープン
	pFile = fopen(LOAD_IP, "r");

	if (pFile != NULL)
	{// ファイルが開けたら
		while (1)
		{
			do
			{//Comment以外を読み込むまで回転
				fgets(ReadText, sizeof(ReadText), pFile);
				sscanf(ReadText, "%s", &HeadText);
			} while (strcmp(HeadText, "\n") == 0 || strcmp(HeadText, "#") == 0);

			if		(strcmp(HeadText, "LOAD_END") == 0) { break; }
			else if (strcmp(HeadText, "IPaddr") == 0)	{ sscanf(ReadText, "%s %c %s", &DustBox, &DustBox, aStr); }
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{
		strcpy(aStr, "127.0.1.1");
	}
	return aStr;
}
//==================================================================================================//
//    * クライアントの送信データ作成処理 *
//==================================================================================================//
void	CClient::Assign(void)
{
	//wsprintf(m_aAssign, "%d %d ", CServer::GetnMaxClient(), m_nID);
	wsprintf(m_aAssign, "%d ", m_nID);
	//インプットの代わりにデータを代入
	//CManager::GetInputKey()->GetAllAssign(m_aAssign);
	CManager::GetInputJoyPad0(0)->GetAllAssign(m_aAssign);
}
//==================================================================================================//
//    * クライアントの反映処理 *
//==================================================================================================//
void	CClient::Reflect(void)
{
	//CManager::GetDLog()->Printf(CDebugLog::MODE_LOG, "\naStr0: %s", m_aStr);

	CServer::FLAG	flag;
	int				nClient;
	char			WKBox[32];
	sscanf(&m_aStr[0], "%d", &flag);
	sscanf(&m_aStr[2], "%d", &nClient);
	if (CServer::GetpmyServer() == NULL) { CServer::GetnMaxClient() = nClient; }
	//strcpy(m_aStr, &m_aStr[4]);

	switch (flag)
	{//FLAGによって読み取り変更
	case CServer::FLAG_END:	Uninit();	break;
	case CServer::FLAG_INPUT:
		sscanf(&m_aStr[4], "%d", &CServer::GetnRand());
		wsprintf(&WKBox[0], "%d", CServer::GetnRand());
		strcpy(&m_aStr[0], &m_aStr[5 + strlen(&WKBox[0])]);
		//CManager::GetInputKey()->GetAllReflect(m_aStr, nClient);
		CInputJoyPad_0::GetReflect(m_aStr, nClient);
		break;
	case CServer::FLAG_CONNECT:
		sscanf(&m_aStr[4], "%d ", &m_nID);
		break;
	}
}
//==================================================================================================//
//    * 終了時のメッセージ送信処理 *
//==================================================================================================//
void	CClient::EndSend(void)
{
	//char aStr[4] = "0";
	send(m_sockClient, m_aAssign, strlen(m_aAssign), 0);
}
//==================================================================================================//
//    * ID獲得処理 *
//==================================================================================================//
int CClient::GetnID(void)
{
	if (m_pmyClient != NULL) { return m_pmyClient->m_nID; }
	return 0;
}
//==================================================================================================//
//    * 受け取り処理 *
//==================================================================================================//
unsigned __stdcall ClientRecv(void *)
{
	CClient *&pClient = CClient::GetpmyClient();

	while (1)
	{
		if (pClient->GetbEnd()) { break; }
		if (!pClient->GetbRecv())
		{
			//pClient->THRecv();
			//サーバーからデータ（メッセージの受信）
			char aStr[MAX_STRING * MAX_CLIENT + PLUS_STRING];

			memset(aStr, 0, sizeof(aStr));
			recv(pClient->GetSockClient(), aStr, sizeof(aStr), 0);	//データ受信とそのバイト数を返す
			strcpy(pClient->GetaStr(), aStr);
			pClient->GetbRecv() = true;
		}
	}


	return 0;
}