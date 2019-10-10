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
#define MAX_MAP_OBJECT	(3500)
#define MAX_MAP_CAR		(300)
#define MAX_MAP_MESH	(1400)
#define MAX_MAP_WALL	(550)
#define MAX_MAP_ROUTE	(300)
#define MAX_MAP_POINT	(300)
#define MAX_POINT		(10)
#define MAX_SCORE_UI	(6)

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
class CBillBoord;
class CWall;
class CShadow;
class CLogo;
class CLoadTextMotion;
class CSatisfaction;
class CGameCamera;
class CShaderMgr;
class CEffectMgr;
class CPoliceLamp;
class CGround;
class CLoadTextEffect;

//=====================
// ��{�N���X
//=====================
class CGame
{
public:
	typedef enum
	{
		GAMESTATE_NONE = 0,					//�������Ă��Ȃ����
		GAMESTATE_NORMAL,					//�ʏ���
		GAMESTATE_CLEAR,					//�Q�[���N���A
		GAMESTATE_OVER,						//�Q�[���I�[�o�[
		GAMESTATE_END,						//�Q�[���I��
		GAMESTATE_MAX						//�ő吔
	}GAMESTATE;

	typedef enum
	{//�e�X�e�[�W
		STAGE_NONE = 0,						//�������ĂȂ����
		STAGENUM_1,							//�X�e�[�W1
		STAGENUM_2,							//�X�e�[�W2
		STAGENUM_3,							//�X�e�[�W3
		STAGENUM_4,							//�X�e�[�W4
		STAGENUM_5,							//�X�e�[�W5
		STAGENUM_6,							//�X�e�[�W6
		STAGENUM_7,							//�X�e�[�W7
	}STAGENUM;

	//�e�L�X�g�̒l���Ǘ�����ꏊ
	typedef struct
	{
		int					nRouteCount;							// ���[�g�̃J�E���^�[
		int					nPointCount[MAX_MAP_ROUTE];				// �|�C���g�̃J�E���^�[
		int					nIndexPoint[MAX_MAP_ROUTE][MAX_POINT];	// �Ԃ������ԍ� [���[�g�ԍ�]�@[�|�C���g�̔ԍ�]�@

	}Route;

	//�e�L�X�g�̒l���Ǘ�����ꏊ
	typedef struct
	{
		int					nCount;					// �J�E���^�[
		int					nNum[MAX_MAP_POINT];	// �ԍ�
		D3DXVECTOR3			pos[MAX_MAP_POINT];		// �ʒu
	}Point;



	CGame();								//�R���X�g���N�^
	~CGame();								//�f�X�g���N�^
	HRESULT Init();							//����������
	void Uninit(void);						//�I������
	void Update(void);						//�X�V����
	void Draw(void);						//�`�揈��
	static void SetGameState(GAMESTATE state) { m_gameState = state; }
	static void SetPause(bool bPause);
	static bool GetPause(void) { return m_bPause; }
	static bool GetHelp(void) { return m_bHelp; }
	static GAMESTATE GetGameState(void) { return m_gameState; }
	static CPlayer *&GetPlayer(void) { return m_pPlayer; }
	static CPolygon3D *GetPolygon3D(void) { return m_pPolygon3D; }
	static CModel *GetModel(void) { return m_pModel; }
	static CMeshField *GetMeshField(void) { return m_pMeshField; }
	static CObject *Get3DObject(void) { return m_pObject; }
	static CCarBase *GetCar(void) { return m_pCar; }
	static CBillBoord *GetBillboord(void) { return m_pBillBoord; }
	static CLoadTextMotion * GetPlayerMotion(void) { return m_pPlayerMotion; }	//�v���C���[�̃��[�V�����̎擾
	static CLoadTextMotion * GetBoyMotion(void) { return m_pBoyMotion; }		//���q����̃��[�V�����̎擾
	static CLoadTextMotion * GetGirlMotion(void) { return m_pGirlMotion; }		//�������[�V�����̎擾
	static CSatisfaction * GetSatisfaction(void) { return m_pSatisfaction; }	//�����x�̎擾
	static CGameCamera * GetGameCamera(void) { return m_pGameCamera; }			//�Q�[���J�����̎擾
	static CShaderMgr * GetShaderMgr(void) { return m_pShaderMgr; }				//�V�F�[�_�[�}�l�[�W���[�̎擾
	static CGround * GetRiver(void) { return m_pGround; }						//��̎擾
	static CLoadTextEffect * GetLoadEffect(void) { return m_pLoadTextEffect; }	//�ǂݍ��񂾃G�t�F�N�g�̎擾
	static int GetGameCounter(void) { return m_nGameCounter; }					//�Q�[���̃J�E���^�[
	static CLogo * GetScoreUI(int nNum) { return m_pScoreUI[nNum]; }						//�X�R�A�֌W��UI�̎擾

	static Route GetCarRoute(void) { return m_Route; }	//	�Ԃ̃��[�g�̎擾
	static Point GetCarPoint(void) { return m_Point; }	//	�Ԃ̃|�C���g�̎擾
														//�X�e�[�W
	static STAGENUM GetNumState(void) { return m_StageState; }
	void SetNumState(STAGENUM stagenum);
	void SetStageState(STAGENUM stage);
	void SetStage(void);

	//---------------------------------
	//�t�@�C����ǂݍ���
	//---------------------------------
	void TextLoad(int nLoadNumber);
	void MeshTextLoad(int nLoadNumber);
	void WallTextLoad(int nLoadNumber);
	void NpcTextLoad(int nLoadNumber);

	void PointTextLoad(int nLoadNumber);	//	�Ԃ̓����|�C���g�̓ǂݍ���
	void RouteLoad(int nLoadNumber);		//	�o�H�̃p�^�[���̓ǂݍ���

	static void SetStopRoute(int nStopRoute) { m_nStopRoute = nStopRoute; }
	static int GetStopRoute(void) { return m_nStopRoute; }


	char *ReadLine(FILE *pFile, char *pDst);	//1�s�ǂݍ���
	char *GetLineTop(char *pStr);				//�s�̐擪���擾
	int  PopString(char *pStr, char *pDest);	//�s�̍Ō��؂�̂�

	//�E�H�[�N�X���[�p
	static bool GetDrawUI(void) { return m_bDrawUI; };

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
		int					m_nType;			// ���
		int					m_nTexType;			// �e�N�X�`���̎��
		int					m_nCollision;		// �����蔻���ONOFF
		D3DXVECTOR3			m_pos;				// �ʒu
		D3DXVECTOR3			m_rot;				// ��]
		D3DXVECTOR3			m_scale;			// �T�C�Y
		int					m_nRoute;			// ���[�g�̐ݒ�
		bool				bLoop;				// ���[�v���邩�ǂ���
	}Npc;

	static GAMESTATE m_gameState;				//�Q�[�����
	GAMESTATE m_NowGameState;					//���݂̃Q�[����
	int m_nCounterGameState;					//��ԃJ�E���^
	static CPlayer *m_pPlayer;					//�v���C���[�̃|�C���^
	static CNumber *m_pNumber;					//�����̃|�C���^
	static CPause *m_pPause;					//�|�[�Y�̃|�C���^
	static CPolygon3D *m_pPolygon3D;			//�RD�|���S���̃|�C���^
	static CModel *m_pModel;					//���f���̃|�C���^
	static CMeshField *m_pMeshField;			//���b�V���t�B�[���h�̃|�C���^
	static CObject *m_pObject;					//�I�u�W�F�N�g�̃|�C���^
	static CCarBase *m_pCar;					//NPC�̃|�C���^
	static CBillBoord *m_pBillBoord;			//�r���{�[�h�̃|�C���^
	static CWall *m_pWall;						//�ǂ̃|�C���^
	static CShadow *m_pShadow;					//�e�̃|�C���^
	static CGameCamera * m_pGameCamera;			//�J�����̃|�C���^
	static bool m_bPause;						//���݃|�[�Y���|�[�Y����Ȃ���
	static bool m_bHelp;						//���݃w���v���w���v����Ȃ���
	static int	m_nCntSetStage;					// �X�e�[�W�Z�b�g�J�E���^
	static STAGENUM m_StageState;				// �X�e�[�W���
	static STAGENUM m_ConteniueStage;			//�R���e�j���[�������̃G���A����X�^�[�g
	static CLoadTextMotion * m_pPlayerMotion;	//�v���C���[�̃��[�V�����ǂݍ���
	static CLoadTextMotion * m_pBoyMotion;		//�j�����[�V�����ǂݍ���
	static CLoadTextMotion * m_pGirlMotion;		//�������[�V�����ǂݍ���
	static CSatisfaction * m_pSatisfaction;		//�����x�ւ̃|�C���^
	static Route m_Route;						//�Ԃ̃}�b�v�̃��[�g�p
	static Point m_Point;						//�ݒu����point�̍\����
	static CShaderMgr * m_pShaderMgr;			//�V�F�[�_�[�}�l�[�W���[
	static CPoliceLamp *m_pPoliceLamp;			//�Ԃ̃����v�_��
	static CGround *m_pGround;					//��ւ̃|�C���^�ϐ�
	static CLoadTextEffect * m_pLoadTextEffect;	//�G�t�F�N�g�̓ǂݍ���
	static int m_nGameCounter;					//�Q�[���̃J�E���^�[
	static CLogo *m_pScoreUI[MAX_SCORE_UI];		//���S�ւ̃|�C���^ �X�R�AUI�Ɋ֌W���Ă��镨

	int m_nSetObjectNum;						//�I�u�W�F�N�g��u������
	int m_nSetMeshFieldNum;						//���b�V���t�B�[���h��u������
	int	m_nSetWallNum;							//�ǂ̒u������
	int	m_nSetNpcNum;							//NPC�̒u������
	int	m_nSetPointNum;							//Point�̒u������
	int	m_nEnemyCount;							//�G�̐�
	Map m_Map[MAX_MAP_OBJECT];					//�ݒu����I�u�W�F�N�g�̍\����
	Mesh m_Mesh[MAX_MAP_MESH];					//�ݒu���郁�b�V���t�B�[���h�̍\����
	Wall m_aWall[MAX_MAP_WALL];					//�ݒu����ǂ̍\����
	Npc m_aNpc[MAX_MAP_CAR];					//�ݒu����NPC�̍\����

	static int m_nStopRoute;					//���[�g�ŎԂ��~�܂�

	//�E�H�[�N�X���[�p
	static bool m_bDrawUI;
};
#endif