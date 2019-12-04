//=============================================================================
//
// �T�[�o�[���� [Server.h]
// Author : ���{��
//
//=============================================================================
#ifndef _SERVER_H_
#define _SERVER_H_

#include "main.h"
//*****************************************************************************
// �}�N���̒�`
//*****************************************************************************
#define MAX_CLIENT	(4)
#define MAX_STRING	(256)
#define PLUS_STRING	(32)
#define LOAD_IP		"data/TEXT/IPaddr/Connect_IPaddr.txt"
//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CServer
{
public:
	CServer() {}; ~CServer() {};
	typedef enum
	{
		FLAG_CONNECT,		//�ڑ��I
		FLAG_INPUT,			//�C���v�b�g�f�[�^
		FLAG_END,			//�ڑ��I��
		FLAG_MAX
	}FLAG;

	HRESULT			Init(void);		//������
	void			Update(void);	//�X�V
	void			Uninit(void);	//�I��
	void			Stop(void);	//��t�I��
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
		//�ϐ��錾//***********************************************************************
	static CServer		*m_pmyServer;					//�T�[�o�[�̃|�C���^
	static int			m_nMaxClient;				//�N���C�A���g�̐�
	static int			m_nRand;				//�����_���Ȑ�
	struct sockaddr_in	m_addrServer;				//�\�P�b�g�A�h���X
	struct sockaddr_in	m_addrClient;				//�N���C�A���g�A�h���X
	SOCKET				m_sockServer;				//�\�P�b�g�T�[�o�[�p
	SOCKET				m_sockClient[MAX_CLIENT];	//�\�P�b�g�N���C�A���g
	HANDLE				m_hThConnect;				//�ڑ��p�X���b�h�̃n���h��
	HANDLE				m_hThUpdate;				//�X�V�p�X���b�h�̃n���h��
	HANDLE				m_hThSend[MAX_CLIENT];		//���M�p�X���b�h�̃n���h��
	bool				m_bConnect[MAX_CLIENT];		//�q�����Ă��邩�ǂ���
	bool				m_bReady[MAX_CLIENT];		//���Z�b�g��ڑ����ꂽ���̊m�F
	bool				m_bTry[MAX_CLIENT];			//�󂯎���Ԃ��ǂ���
	int					m_nLengthaddrClient;		//�\�P�b�g�A�h���X�i�N���C�A���g�j�̃T�C�Y
	bool				m_bEnd;						//�ڑ��I���t���O

	char				m_aStr[MAX_STRING];	//�N���C�A���g���Ƃ̏��
	char				m_aInformation[MAX_STRING * MAX_CLIENT + PLUS_STRING];	//����t������

};
//*****************************************************************************
// �N���X�̒�`
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
		//�ϐ��錾//***********************************************************************
	static CClient		*m_pmyClient;						//�N���C�A���g�̃|�C���^
	struct sockaddr_in	m_addrServer;						//�\�P�b�g�A�h���X
	SOCKET				m_sockClient;						//�\�P�b�g�N���C�A���g
	HANDLE				m_hThRecv;							//�󂯎��p�X���b�h�̃n���h��
	char				m_aAssign[MAX_STRING];				//���M����f�[�^
	char				m_aStr[MAX_STRING * MAX_CLIENT + PLUS_STRING];	//��M�����f�[�^
	char				m_aStr0[MAX_STRING * MAX_CLIENT + PLUS_STRING];	//��M�����f�[�^
	int					m_nLengthData;						//�\�P�b�g�A�h���X�i�N���C�A���g�j�̃T�C�Y
	int					m_nID;								//�v���C���[�ԍ�
	bool				m_bRecv;							//�󂯎��̗L��
	bool				m_bEnd;							//�I���̗L��

};
#endif
