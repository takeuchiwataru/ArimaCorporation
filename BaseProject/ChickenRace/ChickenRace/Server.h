//=============================================================================
//
// サーバー処理 [Server.h]
// Author : 杉本涼
//
//=============================================================================
#ifndef _SERVER_H_
#define _SERVER_H_

#include "main.h"
//*****************************************************************************
// マクロの定義
//*****************************************************************************
#define MAX_CLIENT	(4)
#define MAX_STRING	(256)
#define PLUS_STRING	(32)
#define LOAD_IP		"data/TEXT/IPaddr/Connect_IPaddr.txt"
//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CServer
{
public:
	CServer() {}; ~CServer() {};
	typedef enum
	{
		FLAG_CONNECT,		//接続！
		FLAG_INPUT,			//インプットデータ
		FLAG_END,			//接続終了
		FLAG_MAX
	}FLAG;

	HRESULT			Init(void);		//初期化
	void			Update(void);	//更新
	void			Uninit(void);	//終了
	void			Stop(void);	//受付終了
	static int		Rand(void);
	static	int		&GetnMaxClient(void)		{ return m_nMaxClient;	}
	static	int		&GetnRand(void)				{ return m_nRand; }
	static	CServer	*&GetpmyServer(void)		{ return m_pmyServer;	}
	sockaddr_in		&GetaddrClient(void)		{ return m_addrClient;	}
	SOCKET			&GetsockServer(void)		{ return m_sockServer;	}
	SOCKET			&GetsockClient(int nNumber) { return m_sockClient[nNumber]; }
	bool			&GetbConnect(int nNumber)	{ return m_bConnect[nNumber];	}
	bool			&GetbEnd(void)				{ return m_bEnd; }
	bool			&GetbReady(int nNumber) { return m_bReady[nNumber]; }
	HANDLE			&GethThSend(int nNumber) { return m_hThSend[nNumber]; }

	char			*GetaInformation(void)		{ return &m_aInformation[0]; }

	static void		ResetMaxClient(void)		{ m_nMaxClient = 0; }
private://*****************************************************************************
		//変数宣言//***********************************************************************
	static CServer		*m_pmyServer;					//サーバーのポインタ
	static int			m_nMaxClient;				//クライアントの数
	static int			m_nRand;				//ランダムな数
	struct sockaddr_in	m_addrServer;				//ソケットアドレス
	struct sockaddr_in	m_addrClient;				//クライアントアドレス
	SOCKET				m_sockServer;				//ソケットサーバー用
	SOCKET				m_sockClient[MAX_CLIENT];	//ソケットクライアント
	HANDLE				m_hThConnect;				//接続用スレッドのハンドル
	HANDLE				m_hThUpdate;				//更新用スレッドのハンドル
	HANDLE				m_hThSend[MAX_CLIENT];		//送信用スレッドのハンドル
	bool				m_bConnect[MAX_CLIENT];		//繋がっているかどうか
	bool				m_bReady[MAX_CLIENT];		//リセット後接続されたかの確認
	bool				m_bTry[MAX_CLIENT];			//受け取り状態かどうか
	int					m_nLengthaddrClient;		//ソケットアドレス（クライアント）のサイズ
	bool				m_bEnd;						//接続終了フラグ

	char				m_aStr[MAX_STRING];	//クライアントごとの情報
	char				m_aInformation[MAX_STRING * MAX_CLIENT + PLUS_STRING];	//送り付ける情報

};
//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CClient
{
public:
	CClient() {}; ~CClient() {};

	HRESULT Init(void);
	void	Update(void);
	void	Takes(void);
	void	Uninit(void);
	char	*IPLoad(char *aStr);
	void	Assign(void);
	void	Reflect(void);
	void	EndSend(void);

	SOCKET	&GetSockClient(void)	{ return m_sockClient; }
	bool	&GetbRecv(void)			{ return m_bRecv; };
	bool	&GetbEnd(void)			{ return m_bEnd; };
	char	*GetaStr(void)			{ return &m_aStr[0]; };
	char	*GetaStr0(void) { return &m_aStr0[0]; };

	static CClient *&GetpmyClient(void) { return m_pmyClient; };
	static int GetnID(void);

private://*****************************************************************************
		//変数宣言//***********************************************************************
	static CClient		*m_pmyClient;						//クライアントのポインタ
	struct sockaddr_in	m_addrServer;						//ソケットアドレス
	SOCKET				m_sockClient;						//ソケットクライアント
	HANDLE				m_hThRecv;							//受け取り用スレッドのハンドル
	char				m_aAssign[MAX_STRING];				//送信するデータ
	char				m_aStr[MAX_STRING * MAX_CLIENT + PLUS_STRING];	//受信したデータ
	char				m_aStr0[MAX_STRING * MAX_CLIENT + PLUS_STRING];	//受信したデータ
	int					m_nLengthData;						//ソケットアドレス（クライアント）のサイズ
	int					m_nID;								//プレイヤー番号
	bool				m_bRecv;							//受け取りの有無
	bool				m_bEnd;							//終了の有無

};
#endif
