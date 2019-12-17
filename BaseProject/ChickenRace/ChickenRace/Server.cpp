//=============================================================================
//
// �T�[�o�[���� [Server.cpp]
// Author : Ryo Sugimoto
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS	//Fire Wall�˔j
#include <stdio.h>				//�C���N���h�t�@�C��
#include <stdlib.h>				//�����h�p
#include <time.h>				//�����������h�p
#include <string.h>				//�X�g�����O�g�p�̂���
#include <math.h>
#include"main.h"
#include"Server.h"
#include"manager.h"
#include <windows.h>
#include <process.h>

//==================================================================================================//
//    * �ÓI�ϐ��錾 *
//==================================================================================================//
CServer		*CServer::m_pmyServer = NULL;					//���g�̃|�C���^
CClient		*CClient::m_pmyClient = NULL;					//�N���C�A���g�̃|�C���^
int			CServer::m_nMaxClient = 0;						//�N���C�A���g�̐�
int			CServer::m_nRand = 0;							//�N���C�A���g�܂ߋ��L�̃����_���Ȑ�

//==================================================================================================//
//    * �v���g�^�C�v�錾 *
//==================================================================================================//
unsigned __stdcall ServerUpdate(void *);
unsigned __stdcall Connect(void *);
unsigned __stdcall Send0(void *);
unsigned __stdcall Send1(void *);
unsigned __stdcall Send2(void *);
unsigned __stdcall Send3(void *);
unsigned __stdcall ClientRecv(void *);

//==================================================================================================//
//    * �T�[�o�[�̏��������� *
//==================================================================================================//
HRESULT CServer::Init(void)
{
	m_pmyServer = this;

	WSADATA WsaData;
	WSAStartup(MAKEWORD(2, 0), &WsaData); //������

	//�T�[�o�[���̃\�P�b�g�̍쐬
	m_sockServer = socket(AF_INET, SOCK_STREAM, 0);

	//�\�P�b�g�̐ݒ�
	m_addrServer.sin_family = AF_INET;				//�ڑ��̌`��
	m_addrServer.sin_port = htons(12345);			//�ԍ��̐ݒ�
	m_addrServer.sin_addr.S_un.S_addr = INADDR_ANY;	//�悭�킩��Ȃ�

	//�\�P�b�g�ɖ��̂�t�^
	bind(m_sockServer, (struct sockaddr *)&m_addrServer, sizeof(m_addrServer));

	//�N���C�A���g����̐ڑ��v�������Ă��Ԃɂ���
	listen(m_sockServer, 5);

	m_bEnd = false;
	for (int nCount = 0; nCount < MAX_CLIENT; nCount++)
	{//�q�����Ă��Ȃ���Ԃɂ���
		m_bConnect[nCount] = false;
		m_bReady[nCount] = false;
	}
	strcpy(m_aStr, "-1");
	strcpy(m_aInformation, "HELLO_ONLINE");

	//�ڑ����X���b�h�ɕ�����
	DWORD thID;
	m_hThConnect	= (HANDLE)_beginthreadex(NULL, 0, Connect, NULL, 0, (unsigned int*)&thID);
	m_hThUpdate		= (HANDLE)_beginthreadex(NULL, 0, ServerUpdate, NULL, 0, (unsigned int*)&thID);

	return S_OK;
}
//==================================================================================================//
//    * �T�[�o�[�̍X�V���� *
//==================================================================================================//
void	CServer::Update(void)
{
	//INVALID_SOCET	//�ڑ����s�̃}�N��
	int		nMaxClient = m_nMaxClient;

	if (nMaxClient <= 0) { return; }

	strcpy(&m_aInformation[0], "");
	wsprintf(&m_aInformation[0], "%d %d %d ", FLAG_INPUT, nMaxClient, rand());
	for (int nCount = 0; nCount < nMaxClient; nCount++)
	{//��M
	 //�ڑ�����Ă��Ȃ��Ȃ疳��
		if (!m_bConnect[nCount]) { continue; }
		//��������
		strcpy(m_aStr, "");
		recv(m_sockClient[nCount], m_aStr, sizeof(m_aStr), 0);	//�f�[�^��M�Ƃ��̃o�C�g����Ԃ�
		if (m_bEnd) { break; }
		else if (strcmp(m_aStr, "-1") == 0)
		{
			m_bConnect[nCount] = false;
		}
		else
		{//�ʏ�ʂ�󂯎�����Ȃ� ����
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
	{//�I�����Ă���Ȃ當�����ύX
		wsprintf(&m_aInformation[0], "%d %d ", FLAG_END, nMaxClient);
	}
	//CManager::GetDLog()->Printf(CDebugLog::MODE_LOG, "%s\n", m_aInformation);

	for (int nCount = 0; nCount < nMaxClient; nCount++)
	{//�C���v�b�g��S���ɔz�z
		//�ڑ�����Ă��Ȃ��Ȃ疳��
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
//    * �T�[�o�[�̏I������ *
//==================================================================================================//
void	CServer::Uninit(void)
{
	m_bEnd = true;
	CClient *pClient;
	DWORD thID;
	int		nCntLoop = 0;

	while (1) 
	{//�X�V�̉ғ��m�F
		GetExitCodeThread(m_hThUpdate, &thID);
		if (thID == STILL_ACTIVE)
		{//�ғ����Ȃ�
			CClient *&pClient = CClient::GetpmyClient();
			if (pClient != NULL && nCntLoop == 0)
			{ pClient->EndSend(); }
			nCntLoop++;
		}
		else { break; }
	}
	while (1)
	{//�ڑ��̉ғ��m�F
		GetExitCodeThread(m_hThConnect, &thID);
		if (thID == STILL_ACTIVE)
		{//�ғ����Ȃ�
			if (m_nMaxClient < MAX_CLIENT && nCntLoop % 1000 == 0)
			{//�N���C�A���g������Ă��Ȃ��Ȃ�
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
		{//�ڑ��̉ғ��m�F
			GetExitCodeThread(m_hThSend[nCount], &thID);
			if (thID == STILL_ACTIVE)
			{//�ғ����Ȃ�
			}
			else { break; }
		}
	}
	//�X���b�h�̉��	�ғ��m�F����ō�낤�ˁI
	if (m_hThConnect != NULL) { CloseHandle(m_hThConnect); }
	if (m_hThUpdate != NULL) { CloseHandle(m_hThUpdate); }

	//�\�P�b�g�i�N���C�A���g�j�̉��
	for (int nCount = 0; nCount < MAX_CLIENT; nCount++)
	{
		closesocket(m_sockClient[nCount]);
	}
	//�\�P�b�g�i�T�[�o�[�j�̉��
	closesocket(m_sockServer);

	m_nMaxClient = 0;
	m_pmyServer = NULL;
	delete this;
}
//==================================================================================================//
//    * �T�[�o�[�̎�t�I������ *
//==================================================================================================//
void	CServer::Stop(void)	
{
	CClient *pClient;
	int		nCntLoop = 0;
	DWORD thID;

	while (1)
	{//�ڑ��̉ғ��m�F
		GetExitCodeThread(m_hThConnect, &thID);
		if (thID == STILL_ACTIVE)
		{//�ғ����Ȃ�
			if (m_nMaxClient < MAX_CLIENT && nCntLoop % 1000 == 0)
			{//�N���C�A���g������Ă��Ȃ��Ȃ�
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
//    * �T�[�o�[�̍X�V���� *
//==================================================================================================//
int	CServer::Rand(void)
{
	if (CClient::GetpmyClient() != NULL)
	{
		m_nRand += 1234;
		return m_nRand;
	}
	else
	{
		return rand();
	}
}
//==================================================================================================//
//    * �T�[�o�[�̍X�V���� *
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
//    * �T�[�o�[�̐ڑ����� *
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

	//�N���C�A���g����̐ڑ��v�����󂯕t����
	nLength = sizeof(addrClient);

	for (int nCount = nMaxClient; nCount < MAX_CLIENT; nCount++)
	{//�N���C�A���g���ڑ�
		if (pServer->GetbConnect(nCount)) { continue; }	//�ڑ�����Ă���Ȃ疳��
		SOCKET		&sockClient = pServer->GetsockClient(nCount);
		sockClient = accept(sockServer, (struct sockaddr *)&addrClient, &nLength);

		//�v���C���[�ԍ������m�点
		wsprintf(aStr, "%d %d %d", CServer::FLAG_CONNECT, nMaxClient + 1, nCount);
		send(sockClient, aStr, strlen(aStr), 0);
		if (pServer->GetbEnd()) { return 0; }

		//�ڑ���{�̂ɂ��m�点
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
//    * �T�[�o�[�̃��b�Z�[�W���M�����@0�� *
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

			//�N���C�A���g�փf�[�^�i���b�Z�[�W�𑗐M�j
			send(sockClient, aInformaion, strlen(aInformaion), 0);
			pServer->GetbReady(nNumber) = false;
		}
		if (pServer->GetbEnd()) { break; }
	}
	return 0;
}
//==================================================================================================//
//    * �T�[�o�[�̃��b�Z�[�W���M�����@1�� *
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

			//�N���C�A���g�փf�[�^�i���b�Z�[�W�𑗐M�j
			send(sockClient, aInformaion, strlen(aInformaion), 0);
			pServer->GetbReady(nNumber) = false;
		}
		if (pServer->GetbEnd()) { break; }
	}
	return 0;
}
//==================================================================================================//
//    * �T�[�o�[�̃��b�Z�[�W���M�����@2�� *
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

			//�N���C�A���g�փf�[�^�i���b�Z�[�W�𑗐M�j
			send(sockClient, aInformaion, strlen(aInformaion), 0);
			pServer->GetbReady(nNumber) = false;
		}
		if (pServer->GetbEnd()) { break; }
	}
	return 0;
}
//==================================================================================================//
//    * �T�[�o�[�̃��b�Z�[�W���M�����@3�� *
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

			//�N���C�A���g�փf�[�^�i���b�Z�[�W�𑗐M�j
			send(sockClient, aInformaion, strlen(aInformaion), 0);
			pServer->GetbReady(nNumber) = false;
		}
		if (pServer->GetbEnd()) { break; }
	}
	return 0;
}

//==================================================================================================//
//    * �N���C�A���g�̏��������� *
//==================================================================================================//
HRESULT CClient::Init(void)
{
	WSADATA WsaData;
	char	aStr[32];
	WSAStartup(MAKEWORD(2, 0), &WsaData); //������

	//�T�[�o�[���̃\�P�b�g�̍쐬
	m_sockClient = socket(AF_INET, SOCK_STREAM, 0);

	//�\�P�b�g�̐ݒ�
	m_addrServer.sin_family = AF_INET;				//�ڑ��̌`��
	m_addrServer.sin_port = htons(12345);			//�ԍ��̐ݒ�
	m_addrServer.sin_addr.S_un.S_addr = inet_addr(IPLoad(aStr));	//IP�A�h���X

	//�T�[�o�[�ɐڑ�
	int nLoop;
	//do
	//{
		nLoop = connect(m_sockClient, (struct sockaddr *)&m_addrServer, sizeof(m_addrServer));

	//} while (nLoop == -1);

	if (nLoop == -1)
		return E_FAIL;

	//�T�[�o�[����f�[�^�i���b�Z�[�W�̎�M�j
	memset(m_aStr, 0, sizeof(m_aStr));
	m_nLengthData = recv(m_sockClient, m_aStr, sizeof(m_aStr), 0);	//�f�[�^��M�Ƃ��̃o�C�g����Ԃ�

	Reflect();

	//���M�p��������\�z
	CClient::Assign();
	send(m_sockClient, m_aAssign, strlen(m_aAssign), 0);

	//�X���b�h�ŕ�����
	m_bEnd = false;
	m_bRecv = false;
	//m_hThRecv = (HANDLE)_beginthreadex(NULL, 0, ClientRecv, NULL, 0, (unsigned int*)&thID);

	strcpy(m_aStr, "NULL\n");
	GetpmyClient() = this;
	return S_OK;
}
//==================================================================================================//
//    * �N���C�A���g�̍X�V���� *
//==================================================================================================//
void	CClient::Update(void)
{
	//�C���v�b�g�X�V
	//��M
	Takes();

	////���M
	//send(m_sockClient, m_aAssign, strlen(&m_aAssign[0]), 0);

	//��M�C���v�b�g���f
	Reflect();

	//�X�V
}
//==================================================================================================//
//    * �N���C�A���g�̏��������� *
//==================================================================================================//
void	CClient::Takes(void)
{
	//�\��
	//CManager::GetDLog()->Printf(CDebugLog::MODE_LOG, &m_aStr[0]);
	
	//���M�p��������\�z
	CClient::Assign();

	//�T�[�o�[����f�[�^�i���b�Z�[�W�̎�M�j
	memset(m_aStr, 0, sizeof(m_aStr));
	m_nLengthData = recv(m_sockClient, m_aStr, sizeof(m_aStr), 0);	//�f�[�^��M�Ƃ��̃o�C�g����Ԃ�

	//while (!m_bRecv) {}

	//���M
	send(m_sockClient, m_aAssign, strlen(&m_aAssign[0]), 0);
	m_bRecv = false;
}
//==================================================================================================//
//    * �N���C�A���g�̏��������� *
//==================================================================================================//
void	CClient::Uninit(void)
{
	DWORD thID;

	m_bEnd = true;
	closesocket(m_sockClient);	//�\�P�b�g�i�N���C�A���g�j�̉��
	WSACleanup();

	while (1)
	{//�X�V�̉ғ��m�F
		GetExitCodeThread(m_hThRecv, &thID);
		if (thID == STILL_ACTIVE)
		{//�ғ����Ȃ�T�[�o�[����̏I����҂�
		}
		else { break; }
	}
	GetpmyClient() = NULL;

	delete this;
}
//==================================================================================================//
//    * �N���C�A���g��IP�A�h���X�擾���� *
//==================================================================================================//
char	*CClient::IPLoad(char *aStr)
{
	FILE *pFile;
	char ReadText[256];		// �ǂݍ��񂾕���������Ă���
	char HeadText[256];		// ��r�p
	char DustBox[32];		// �g�p���Ȃ����̂����Ă���

	// �t�@�C���I�[�v��
	pFile = fopen(LOAD_IP, "r");

	if (pFile != NULL)
	{// �t�@�C�����J������
		while (1)
		{
			do
			{//Comment�ȊO��ǂݍ��ނ܂ŉ�]
				fgets(ReadText, sizeof(ReadText), pFile);
				sscanf(ReadText, "%s", &HeadText);
			} while (strcmp(HeadText, "\n") == 0 || strcmp(HeadText, "#") == 0);

			if		(strcmp(HeadText, "LOAD_END") == 0) { break; }
			else if (strcmp(HeadText, "IPaddr") == 0)	{ sscanf(ReadText, "%s %c %s", &DustBox, &DustBox, aStr); }
		}

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{
		strcpy(aStr, "127.0.1.1");
	}
	return aStr;
}
//==================================================================================================//
//    * �N���C�A���g�̑��M�f�[�^�쐬���� *
//==================================================================================================//
void	CClient::Assign(void)
{
	//wsprintf(m_aAssign, "%d %d ", CServer::GetnMaxClient(), m_nID);
	wsprintf(m_aAssign, "%d ", m_nID);
	//�C���v�b�g�̑���Ƀf�[�^����
	//CManager::GetInputKey()->GetAllAssign(m_aAssign);
	CManager::GetInputJoyPad0(0)->GetAllAssign(m_aAssign);
}
//==================================================================================================//
//    * �N���C�A���g�̔��f���� *
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
	{//FLAG�ɂ���ēǂݎ��ύX
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
//    * �I�����̃��b�Z�[�W���M���� *
//==================================================================================================//
void	CClient::EndSend(void)
{
	//char aStr[4] = "0";
	send(m_sockClient, m_aAssign, strlen(m_aAssign), 0);
}
//==================================================================================================//
//    * ID�l������ *
//==================================================================================================//
int CClient::GetnID(void)
{
	if (m_pmyClient != NULL) { return m_pmyClient->m_nID; }
	return 0;
}
//==================================================================================================//
//    * �󂯎�菈�� *
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
			//�T�[�o�[����f�[�^�i���b�Z�[�W�̎�M�j
			char aStr[MAX_STRING * MAX_CLIENT + PLUS_STRING];

			memset(aStr, 0, sizeof(aStr));
			recv(pClient->GetSockClient(), aStr, sizeof(aStr), 0);	//�f�[�^��M�Ƃ��̃o�C�g����Ԃ�
			strcpy(pClient->GetaStr(), aStr);
			pClient->GetbRecv() = true;
		}
	}


	return 0;
}