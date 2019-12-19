//=============================================================================
//
// �Q�[���̏��� [game.h]
// Author : �L�n ���u
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_MAP_OBJECT		(3500)
#define MAX_MAP_CAR			(300)
#define MAX_MAP_MESH		(1400)
#define MAX_MAP_WALL		(550)
#define MAX_MAP_FEED		(100)
#define MAX_MAP_ROUTE		(300)
#define MAX_MAP_POINT		(300)
#define MAX_POINT			(10)
#define MAX_SCORE_UI		(6)

#define MAX_MEMBER			(8)
#define MAX_PLAYER			(4)
#define MAX_CHARCTER		(8)

#define CUORSE_VIEW_TIME	(360)
#define CUORSE_VIEW_TIPE_0	(120)
#define CUORSE_VIEW_TIPE_1	(240)

#define START_SET_TIME		(360)
#define START_COUNT_TIME	(180)

#define MAX_TIME			(35999)

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CPlayer;
class CNumber;
class CPause;
class CObject;
class CCarBase;
class CModel;
class CMeshField;
class CPolygon3D;
class CBillboord;
class CWall;
class CShadow;
class CLogo;
class CLoadTextMotion;
class CGameCamera;

class CGameCharSelect;
class CGamePlay;
class CResultUI;

//=====================
// ��{�N���X
//=====================
class CGame
{
public:
	typedef enum
	{
		GAMEMODE_NONE = 0,
		GAMEMODE_CHARSELECT,				// �L�����I��
		GAMEMODE_COURSESELECT,				// �R�[�X�I��
		GAMEMODE_COURSE_VIEW,				// �R�[�X����
		GAMEMODE_PLAY,						// �v���C
		GAMEMODE_MAX						// �ő吔
	}GAMEMODE;

	typedef enum
	{
		GAMESTATE_NONE = 0,					//�������Ă��Ȃ����
		GAMESTATE_NORMAL,					//�ʏ���
		GAMESTATE_CLEAR,					//�Q�[���N���A
		GAMESTATE_OVER,						//�Q�[���I�[�o�[
		GAMESTATE_END,						//�Q�[���I��
		GAMESTATE_MAX						//�ő吔
	}GAMESTATE;

	CGame();								//�R���X�g���N�^
	~CGame();								//�f�X�g���N�^
	HRESULT Init();							//����������
	void Uninit(void);						//�I������
	void Update(void);						//�X�V����
	static void Draw(void);					//�`�揈��
	static void SetGameState(GAMESTATE state) { m_gameState = state; }
	static void SetPause(bool bPause);
	static bool GetPause(void) { return m_bPause; }
	static bool GetHelp(void) { return m_bHelp; }
	static GAMESTATE GetGameState(void) { return m_gameState; }
	static CPlayer **GetPlayer(void) { return m_pPlayer; }
	static CLoadTextMotion * GetPlayerMotion(void) { return m_pPlayerMotion; }	//�v���C���[�̃��[�V�����̎擾
	static CGameCamera * GetCourseCamera(void) { return m_pCuorseCamera; }
	static CGameCamera * GetGameCamera(int nNum)
	{
		if (0 <= nNum && nNum < MAX_PLAYER)
			return m_pGameCamera[nNum];
		else
			return NULL;
	}			//�Q�[���J�����̎擾

	static int GetGameCounter(void) { return m_nGameCounter; }					//�Q�[���̃J�E���^�[
	static void SetGameModeNext(GAMEMODE gameModeNext) { m_gameModeNext = gameModeNext; };
	void SetGameMode(GAMEMODE gameMode);
	static GAMEMODE GetGameMode(void) { return m_gameMode; }
	void SetStage(void);
	void SetPlayer(bool bCreate, int nMode);
	void Ranking(void);

	//---------------------------------
	//�t�@�C����ǂݍ���
	//---------------------------------
	void TextLoad(int nLoadNumber);
	void MeshTextLoad(int nLoadNumber);
	void WallTextLoad(int nLoadNumber);
	void FeedTextLoad(void);

	char *ReadLine(FILE *pFile, char *pDst);	//1�s�ǂݍ���
	char *GetLineTop(char *pStr);				//�s�̐擪���擾
	int  PopString(char *pStr, char *pDest);	//�s�̍Ō��؂�̂�

												//�E�H�[�N�X���[�p
	static bool GetDrawUI(void) { return m_bDrawUI; };

	// �v���C���[�ő吔
	static void SetMaxPlayer(int nMaxPlayer) { if (0 <= m_nMaxPlayer && m_nMaxPlayer < 4) m_nMaxPlayer = nMaxPlayer; }
	static int GetMaxPlayer(void) { return m_nMaxPlayer; }

	// �L�����I��ԍ�
	static void SetCharSelectNum(int *nCharSelectNum)
	{
		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
			m_nCharSelectNum[nCntPlayer] = nCharSelectNum[nCntPlayer];
	}
	static int *GetCharSelectNum(void) { return m_nCharSelectNum; }

	// �R���g���[���[�ԍ�
	static void SetControllerNum(int *nControllerNum)
	{
		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
			m_nControllerNum[nCntPlayer] = nControllerNum[nCntPlayer];
	}
	static int *GetControllerNum(void) { return m_nControllerNum; }
	static int GetControllerNum(int nNum) { return m_nControllerNum[nNum]; }

	// �����L���O
	static int GetRanking(int nPlayerNum) { return m_nRanking[nPlayerNum]; }
	static int *GetRanking(void) { return m_nRanking; }

	static int GetRankingSort(int nPlayerNum) { return m_nRankingSort[nPlayerNum]; }
	static int *GetRankingSort(void) { return m_nRankingSort; }

	// �S�[��
	static bool *GetGoul(void) { return m_bGoul; }

	static int GetCameraNumber(void) { return m_nCameraNumber; }
	
	static int *GetTimer(void) { return m_nTime; }

private:
	//�e�L�X�g�̒l���Ǘ�����ꏊ
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
		int					m_nTexType;						// �e�N�X�`���̎��
		int					m_nWidthDivide;					// ���̕�����������
		int					m_nDepthDivide;					// ���̕�����������
		float				m_fWidthLength;					// ���̒���
		float				m_fDepthLength;					// ���̒���
		float				m_fTexXUV;						// XUV���W
		float				m_fTexYUV;						// YUV���W
		float				m_fVtxHeight_No0;				// ���_�ԍ�0�𑀍�
		float				m_fVtxHeight_No1;				// ���_�ԍ�1�𑀍�
		float				m_fVtxHeight_No2;				// ���_�ԍ�2�𑀍�
		float				m_fVtxHeight_No3;				// ���_�ԍ�3�𑀍�
		float				m_fVtxSide_No0;					// ���_�ԍ�0�𑀍�
		float				m_fVtxSide_No1;					// ���_�ԍ�1�𑀍�
		float				m_fVtxSide_No2;					// ���_�ԍ�2�𑀍�
		float				m_fVtxSide_No3;					// ���_�ԍ�3�𑀍�
		D3DXVECTOR3			m_pos;							// �ʒu
	}Mesh;

	//�e�L�X�g�̒l���Ǘ�����ꏊ
	typedef struct
	{
		D3DXVECTOR3			m_pos;				// �ʒu
		D3DXVECTOR3			m_rot;				// ��]
		float				m_fWidthDivide;		// ����
		float				m_fHightDivide;		// �c��
		int					m_nTexType;			// �e�N�X�`���̎��
	}Wall;

	//�e�L�X�g�̒l���Ǘ�����ꏊ
	typedef struct
	{
		D3DXVECTOR3			m_pos;				// �ʒu
		int					m_nZone;			// �]�[��
		int					m_nType;			// �H�ו��̎��
	}Feed;

	static GAMEMODE m_gameMode;					//�Q�[�����[�h
	static GAMEMODE m_gameModeNext;				//���̃Q�[�����[�h

	static CGameCharSelect *m_pGameCharSelect;	// �Q�[���i�L�����I���j
	static CGamePlay*m_pGamePlay;				// �Q�[���i�v���C�j

	static GAMESTATE m_gameState;				//�Q�[�����
	GAMESTATE m_NowGameState;					//���݂̃Q�[����
	int m_nCounterGameState;					//��ԃJ�E���^
	static CPlayer *m_pPlayer[MAX_MEMBER];		//�v���C���[�̃|�C���^
	static CPause *m_pPause;					//�|�[�Y�̃|�C���^
	static CGameCamera * m_pCuorseCamera;		//�R�[�X�̃|�C���^
	static CGameCamera * m_pGameCamera[MAX_PLAYER];//�J�����̃|�C���^
	static bool m_bPause;						//���݃|�[�Y���|�[�Y����Ȃ���
	static bool m_bHelp;						//���݃w���v���w���v����Ȃ���
	static int	m_nCntSetStage;					// �X�e�[�W�Z�b�g�J�E���^
	static CLoadTextMotion * m_pPlayerMotion;	//�v���C���[�̃��[�V�����ǂݍ���
	static int m_nGameCounter;					//�Q�[���̃J�E���^�[

	int m_nSetObjectNum;						//�I�u�W�F�N�g��u������
	int m_nSetMeshFieldNum;						//���b�V���t�B�[���h��u������
	int	m_nSetWallNum;							//�ǂ̒u������
	int	m_nSetFeedNum;							//�H�ו��̒u������

	Map m_Map[MAX_MAP_OBJECT];					//�ݒu����I�u�W�F�N�g�̍\����
	Mesh m_Mesh[MAX_MAP_MESH];					//�ݒu���郁�b�V���t�B�[���h�̍\����
	Wall m_aWall[MAX_MAP_WALL];					//�ݒu����ǂ̍\����
	Feed m_aFeed[MAX_MAP_FEED];					//�ݒu����H�ו��̍\����

	static int m_nMaxPlayer;					// �v���C���[��
	static int m_nCharSelectNum[MAX_MEMBER];	// �L�����I��ԍ�

	static int m_nControllerNum[MAX_PLAYER];	// �R���g���[���[�ԍ�

	static int m_nRanking[MAX_MEMBER];			// �����L���O
	static int m_nRankingSort[MAX_MEMBER];		// �����L���O
	static bool m_bGoul[MAX_PLAYER];			// �S�[������

												//�E�H�[�N�X���[�p
	static bool m_bDrawUI;

	static int m_nCameraNumber;					// ���ݎg�p���Ă���J�����ԍ�

	static int m_nTime[MAX_MEMBER];				// �^�C��

	CResultUI			*m_pResultUI;			// ���U���gUI

};
#endif