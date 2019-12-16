//============================================================================
//
// ���C������ [title.h]
// Author : �L�n ���u
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_MAP_OBJECT			(3500)
#define MAX_MAP_FEED			(100)
#define MAX_MEMBER				(8)

#define RACEINIT_TITLEMODE		(0)
#define RACEMOVE_TITLEMODE		(600 + RACEINIT_TITLEMODE)
#define CAMERA_P_TITLEMODE_1	(360 + RACEMOVE_TITLEMODE)
#define CAMERA_UP_TITLEMODE		(720 + CAMERA_P_TITLEMODE_1)
#define CAMERA_P_TITLEMODE_2	(720 + CAMERA_UP_TITLEMODE)
#define RANKING_TITLEMODE		(600)
#define HOME_TITLEMODE			(720 + CAMERA_P_TITLEMODE_2 + RANKING_TITLEMODE)
#define CHARUP_TITLEMODE		(300 + HOME_TITLEMODE)
#define RESET_TITLEMODE			(300 + CHARUP_TITLEMODE)

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CTitleCamera;

class CTitleMenu;
class CPlayer;
class CResultUI;

//=====================
// ��{�N���X
//=====================
class CTitle
{
public:
	CTitle();								// �R���X�g���N�^
	~CTitle();								// �f�X�g���N�^
	HRESULT Init();							// ����������
	void	Uninit(void);					// �I������
	void	Update(void);					// �X�V����
	static void	Draw(void);					// �`�揈��

	static CPlayer **GetPlayer(void) { return m_pPlayer; }

	static void SetCounter(int nCounter) { m_nTitleCounter = nCounter; }
	static int GetCounter(void) { return m_nTitleCounter; }

	// ���j���[
	static void SetMenu(bool bMenu) { m_bMenu = bMenu; }
	static bool GetMenu(void) { return m_bMenu; }
	// �I�����C��
	static void SetOnline(bool bOnline) { m_bOnline = bOnline; }
	static bool GetOnline(void) { return m_bOnline; }
	// �z�X�g
	static void SetHost(bool bHost) { m_bHost = bHost; }
	static bool GetHost(void) { return m_bHost; }

	static CTitleCamera * GetTitleCamera(void) { return m_pTitleCamera; }

	// �����L���O
	static int GetRanking(int nPlayerNum) { return m_nRanking[nPlayerNum]; }
	static int *GetRanking(void) { return m_nRanking; }

	void SetStage(void);
	void SetTitleMode(void);
	void Ranking(void);

private:
	typedef struct
	{
		int					m_nType;			// ���
		int					m_nTexType;			// �e�N�X�`���̎��
		int					m_nCollision;		// �����蔻���ONOFF
		D3DXVECTOR3			m_pos;				// �ʒu
		D3DXVECTOR3			m_rot;				// ��]
		D3DXVECTOR3			m_scale;			// �T�C�Y
	}Map;

	//�e�L�X�g�̒l���Ǘ�����ꏊ
	typedef struct
	{
		D3DXVECTOR3			m_pos;				// �ʒu
		int					m_nZone;			// �]�[��
		int					m_nType;			// �H�ו��̎��
	}Feed;

	//---------------------------------
	//�t�@�C����ǂݍ���
	//---------------------------------
	void TextLoad(int nLoadNumber);
	void FeedTextLoad(void);

	char *ReadLine(FILE *pFile, char *pDst);	//1�s�ǂݍ���
	char *GetLineTop(char *pStr);				//�s�̐擪���擾
	int  PopString(char *pStr, char *pDest);	//�s�̍Ō��؂�̂�

	int m_nSetObjectNum;						//�I�u�W�F�N�g��u������
	int	m_nSetFeedNum;							//�H�ו��̒u������

	Map m_Map[MAX_MAP_OBJECT];					//�ݒu����I�u�W�F�N�g�̍\����
	Feed m_aFeed[MAX_MAP_FEED];					//�ݒu����H�ו��̍\����

	int					m_nFrame;			// �G�߂̃J�E���^�[
	int					m_nCntTex;			// �e�N�X�`���̃J�E���^�[
	int					m_nCntCreate;		// ��������
	int					m_nLeafposX;		// �����t�� �������W
	int					m_nLeafsize;		// �����t�� �T�C�Y
	static int			m_nTitleCounter;	// �^�C�g���̃J�E���^�[
	bool				m_bOnOff;

	static CTitleCamera	*m_pTitleCamera;	//�^�C�g���J�����̃|�C���^

	static bool			m_bMenu;			// ���j���[
	static bool			m_bOnline;			// �I�����C��
	static bool			m_bHost;			// �z�X�g

	CTitleMenu			*m_pTitleMenu;		// �^�C�g�����j���[

	static CPlayer *m_pPlayer[MAX_MEMBER];		//�v���C���[�̃|�C���^

	static int m_nRanking[MAX_MEMBER];			// �����L���O

	CResultUI			*m_pResultUI;			// ���U���gUI

};
#endif