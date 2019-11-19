//=============================================================================
//
// �v���C���[���� [player.h]
// Author : �������� Sato_Asumi
//
//=============================================================================
#ifndef _MOTIONPLAYER_H_
#define _MOTIONPLAYER_H_

#include "main.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "motion.h"
#include "scene.h"
#include "RoadPointer.h"
#include "ColMesh.h"
#include "game.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_PARTS	(11)											//�ǂݍ��ރp�[�c��
#define MAX_LIFE	(15)										//���C�t
#define MAX_TIRE	(4)											//�^�C���̍ő吔
#define MAX_EGG		(3)											//���̍ő吔
#define MAX_FRAME	(60)

#define DAMAGE_TIME	(60)										// �_���[�W��H����Ă��鎞��
#define SPEEDDOWN_TIME	(300)									// �������Ă��鎞��
#define FALL_CHICK_RANGE		(400)							// �Ђ悱���~��͈�
#define CHICK_FALL_NUM			(5)								// �����Ă���Ђ悱�̐�
#define MAX_EGG		(3)											//���̍ő吔

#define FILE_NAME_PRISONER		"data\\TEXT\\Player\\Player.txt"//�ǂݍ���txt�t�@�C���̖��O
#define MAX_PLAYERANIM			(8)								//�A�j���[�V������
#define MAX_PLAYERKEY			(8)								//�L�[�t���[����
#define MAX_MOTION				(10)							//���[�V�����̍ő吔


//=============================================================================
// �O���錾
//=============================================================================
class CModel;
class CLoadTextPlayer;
class CLoadEffect;
class CCombo;
class CSound;
class CFeed;
class CEgg;
class CChick;
class CBillBoord;

//=============================================================================
// �v���C���[�N���X
//=============================================================================
class CPlayer : public CScene
{
public:
	//�L�[�v�f
	typedef struct
	{
		float fposX;
		float fposY;
		float fposZ;
		float frotX;
		float frotY;
		float frotZ;
	}KEY;

	//�L�[���
	typedef struct
	{
		int nFrame;
		KEY aKey[MAX_PARTS];
	}KEY_INFO;

	//���[�V�������
	typedef struct
	{
		bool bLoop;
		int nNumKey;
		KEY_INFO aKayInfo[MAX_MOTION];
	}MOTION_INFO;

	//���[�V�������
	typedef enum
	{
		PLAYERANIM_NEUTRAL = 0,		//�j���[�g�������[�V����
		PLAYERANIM_RUN,				//����
		PLALYER_MAX					//���[�V�����̍ő吔
	}PlayerAnim;

	typedef enum
	{
		PLAYERTYPE_PLAYER = 0,
		PLAYERTYPE_ENEMY,
		PLAYERTYPE_MAX
	}PLAYERTYPE;

	//���s���
	typedef enum
	{
		STATE_SPEED_ACCEL = 0,
		STATE_SPEED_BRAKS,
		STATE_SPEED_DRIFT,
		STATE_SPEED_DOWN,
		STATE_SPEED_STOP,
	}STATE_SPEED;

	//�n���h���̏��
	typedef enum
	{
		HANDLE_LEFT = 0,
		HANDLE_RIGHT,
		HANDLE_MAX,
	}STATE_HANDLE;

	typedef enum
	{// �v���C���[�̏��
		PLAYERSTATE_NORMAL = 0,	// �ʏ�
		PLAYERSTATE_SPEEDUP,		// ����
		PLAYERSTATE_SPEEDUP_S,		// ��������
		PLAYERSTATE_SPEEDDOWN,	// ����
		PLAYERSTATE_SPEEDDOWN_S,	// �S������
		PLAYERSTATE_DAMAGE,		// �U���H�炤
		PLAYERSTATE_MAX,			//�ő吔
	} PLAYERSTATE;

	typedef enum
	{
		BULLET_EGG_ATTACK = 0,		// ���i�U���j
		BULLET_EGG_ANNOY,			// ���i�W�Q�j
		BULLET_EGG_SPEED,			// ���i���x�j
		BULLET_CHICK_ATTACK,		// �Ђ悱�i�U���j
		BULLET_CHICK_ANNOY,			// �Ђ悱�i�W�Q�j
		BULLET_CHICK_SPEED,			// �Ђ悱�i���x�j
		BULLET_MAX,
	}BULLET;

	//�e�L�X�g���
	typedef struct
	{
		float fAccel;			//�����l�i�O�i�j
		float fBraks;			//�����l�i��i�j
		float fDown;			//�����l
		float fAddRot;			//�������]�l
		float fDistance;		//�ړ�����
		float nCountTime;		//���Ԃ̌v�Z
		D3DXVECTOR3 FirstPos;	//�����ʒu
	}PLAYER_INFO;

	//�F�̏�Ԃ̎��
	CPlayer();
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CPlayer * Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, int nPlayerNum = 0, int nControllerNum = 0, PLAYERTYPE playerType = PLAYERTYPE_PLAYER);
	static void LoadModel(void);
	static void UnloadModel(void);

	PLAYERTYPE GetPlayerType(void) { return m_PlayerType; }

	D3DXVECTOR3 GetPos(void) { return m_pos; };
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };

	D3DXVECTOR3 GetOldPos(void) { return m_OldPos; };

	D3DXVECTOR3 GetMove(void) { return m_move; };
	void SetMove(D3DXVECTOR3 move) { m_move = move; };

	D3DXVECTOR3 GetRot(void) { return m_rot; };
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };

	PLAYER_INFO GetPlayerInfo(void) { return m_PlayerInfo; };
	PLAYER_INFO * GetPlayerInfoPoint(void) { return &m_PlayerInfo; };
	STATE_SPEED GetStateSpeed(void) { return m_StateSpeed; };
	STATE_HANDLE GetStateHandle(void) { return m_StateHandle; };
	PLAYERSTATE GetPlayerState(void) { return m_State; }

	void CollisitionWall(void);

	void SetControl(bool bControl) { m_bControl = bControl; };
	bool GetControl(void) { return m_bControl; };
	int GetCntChick(void) { return m_nCntChick; }
	void SetCntChick(int nCntChick) { m_nCntChick = nCntChick; }
	CChick **GetAnnoyChick(void) { return m_pAnnoyChick; }
	void FallChicks(D3DXVECTOR3 pos);
	void AnnoyChicks(void);

	void SetJump(bool bJump) { m_bJump = bJump; }
	bool GetJump(void) { return m_bJump; };
	void SetCrcleCarIn(bool bCrcleCarIn) { m_bCrcleCarIn = bCrcleCarIn; }
	bool GetCrcleCarIn(void) { return m_bCrcleCarIn; }
	bool GetShake(void) { return m_bShake; }
	bool GetDrive(void) { return m_bDirive; }
	void SetDrive(bool bDrive) { m_bDirive = bDrive; }

	CCOL_MESH::EFFECT &GgetFEffect(void) { return m_FEffect; }
	CRoad_Pointer	*&GetpPoint(void) { return m_pPoint; }
	CRoad_Pointer	*&GetpEnmPoint(void) { return m_pEnmPoint; }
	D3DXVECTOR3		&GetFNor(void) { return m_FNor; };
	D3DXVECTOR3		&Getpos(void) { return m_pos; };
	D3DXVECTOR3		&Getposold(void) { return m_OldPos; };
	D3DXVECTOR3		&Getmove(void) { return m_move; };
	bool			&GetbJump(void) { return m_bJump; };
	float			&GetfLength(void) { return m_fLength; };
	float			&GetfTilt(void) { return m_fCTilt; };
	float			&GetfRotOld(void) { return m_fRotOld; };
	int				&GetnMap(void) { return m_nMap; };
	int				&GetnPlayerNum(void) { return m_nPlayerNum; }
	bool			&GetbDivided(void) { return m_bDivided; };

	int GetItemNum(void) { return m_nNumItem; }
	int GetItemType(int nNum) { return m_bulletType[nNum]; }

	bool GetGoal(void) { return m_bGoal; }

	//���[�V�����̍X�V�֐�
	void UpdateMotion(void);

	//�t�@�C���ǂݍ��݊֐�
	void FileLoad(void);								//�t�@�C���ǂݍ���
	char *ReadLine(FILE *pFile, char *pDst);			//1�s�ǂݍ���
	char *GetLineTop(char *pStr);						//�s�̐擪���擾
	int  PopString(char *pStr, char *pDest);			//�s�̍Ō��؂�̂�

private:
	static LPD3DXMESH	m_pMesh[MAX_PARTS];				//���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER	m_pBuffMat[MAX_PARTS];			//�}�e���A���̏��ւ̃|�C���^
	static DWORD		m_nNumMat[MAX_PARTS];			//�}�e���A���̏��

	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void RemakeAngle(float * pAngle);
	void RemakeCarRot(float * pAngle);
	float GetLength(D3DXVECTOR3 StartPos, D3DXVECTOR3 EndPos);
	void UpdateMove(void);
	void ControlKey(void);
	void DebugProc(void);

	void UpdateAI(void);
	void UseItem(void);
	bool UseATK(int &nRank);
	bool UseDEF(int &nRank);
	bool UseSPD(int &nRank);
	float GetDistance(int nRank);
	void UpdateKiller(void);
	void UpdateFEffect(void);
	void WarpNext(void);

	void UpdateField(void);
	void SetStateSpeed(STATE_SPEED state);
	void SetStateHandle(STATE_HANDLE state)
	{
		if (m_StateHandle != state) { m_StateHandle = state; if (m_StateSpeed == STATE_SPEED_DRIFT) m_nDriftCounter = 0; }
	};
	void CollisionObject(void);
	void CollisionFeed(void);
	void CollisionEgg(void);
	void CollisionChick(void);
	void CarCalculate(D3DXVECTOR3 * TirePos);
	void UpdateStateJump(void);
	void PlaySoundObj(int nType, CSound * pSound);
	void EggAppear(CFeed *pFeed);
	void ChickAppear(void);
	void ChaseEgg(void);
	void BulletEgg(void);
	void CollisionCharacter(void);
	void ChaseAnnoyS(void);


	CModel						*m_apModel[MAX_PARTS];			//�p�[�c���f���̃|�C���^
	PlayerAnim					m_nAnimnow;						//���݂̃A�j���[�V����

	//static CModel *		m_pModel;			//�p�[�c���f���̃|�C���^
	//static int				m_nMaxModel;	//�ǂݍ��ރ��f���̍ő吔
	//static int				m_nMaxParts;	//�ǂݍ��ރp�[�c�̍ő吔

	//�����o�ϐ�
	static int					  m_nMaxMotion;					// ���[�V�����̍ő吔
	static CMotion::MOTION_INFO * m_pMotionInfo;				// ���[�V�������
	static LPDIRECT3DTEXTURE9     m_pTexture;					// �e�N�X�`��
	PLAYERTYPE					  m_PlayerType;					// �v���C���[�^�C�v
	D3DXVECTOR3				      m_pos;						// ���S���W
	D3DXVECTOR3					  m_move;						// �ړ�
	D3DXVECTOR3					  m_MoveMotion;					// ���[�V�����̈ړ���
	D3DXVECTOR3					  m_OldPos;						// �O��̍��W
	D3DXVECTOR3					  m_OldDiffuse;					// �O��̍���
	D3DXVECTOR3					  m_rot;						// ����
	D3DXVECTOR3					  m_OldEggRot[MAX_FRAME];		// �O��̈ʒu
	D3DXVECTOR3					  m_vtxMaxModel;				// ���f���̒��_�ő�l
	D3DXVECTOR3					  m_vtxMinModel;				// ���f���̒��_�ŏ��l
	D3DXMATRIX					  m_mtxWorld;					// ���[���h�}�g���b�N�X
	CMotion *					  m_pMotion;					// ���[�V�����̃|�C���^
	STATE_SPEED					  m_StateSpeed;					// �X�s�[�h�̏��
	STATE_HANDLE				  m_StateHandle;				// �n���h���̏��
	PLAYER_INFO					  m_PlayerInfo;					// �v���C���[�̏��
	CLoadTextPlayer *			  m_pText;						// �v���C���[�̏��ǂݍ���
	CEgg						  *m_pEgg[MAX_EGG];				//���̃|�C���^
	CChick						  *m_pChick[MAX_EGG];			//�Ђ悱�̃|�C���^
	static CChick						  *m_pAnnoyChick[MAX_MEMBER];	//�Ђ悱�̃|�C���^
	bool						  m_bJump;						//  �W�����v�t���O
	bool						  m_bControl;					// �R���g���[���[�̎g�p���
	int							  m_nCountJumpTime;				// �W�����v��Ԃ̎��Ԃ��J�E���g����
	float						  m_fvtxMaxY;					// ���f�����_�̍ő�l�iY�j
	float						  m_fMaxSpeed;					// �X�s�[�h�̍ő�l
	float						  m_fMass;						// ����
	float						  m_fSpeed;						// ����
	int							  m_nCountTime;					// ���Ԃ̉��Z
	int							  m_nCountSpeed;				// ���Ԃ̉��Z
	bool						  m_bCrcleCarIn;				// �Ԃ��͈͓��ɓ����Ă��邩�ǂ���
	CLoadEffect *				  m_pLoadEffect;				// ���[�h�G�t�F�N�g�̃|�C���^�ϐ�
	int							  m_nCntCombo;
	int							  m_nCntShake;					//�h��̎��Ԃ��J�E���g����
	bool						  m_bShake;						//�h��̃I���I�t
	bool						  m_bDirive;					//�O�i�A��ނ̑���t���O
	int							  m_nCntFlag;
	int							  m_nNumEgg;
	int							  m_nNumChick;
	int							  m_nCntFrame;					// ���̂��Ă��鏈���Ɏg��
	int							  m_nPlayerRank;				// ���ʕۑ��p
	int							  m_nCntChick;
	int							  m_nDestRank;					// �_������
	bool						  m_abJump[MAX_FRAME];
	bool						  m_bJumpSave;
	bool						  m_bAnnoyS;
	int							  m_nNumItem;
	int							  m_nAnnoySTimer;
	BULLET						  m_bulletType[MAX_EGG];

	int							  m_nDriftCounter;		// �h���t�g�J�E���g

	bool						  m_bDamage;
	PLAYERSTATE					  m_State;
	int							  m_nCntDamage;
	int							  m_nPlayerNum;			// �v���C���[�ԍ�
	int							  m_nControllerNum;		// �R���g���[���[�ԍ�

	CCOL_MESH::EFFECT			  m_FEffect;				//�n�ʌ���
	CRoad_Pointer				  *m_pPoint;			//���̃|�C���g
	CRoad_Pointer				  *m_pEnmPoint;			//�G�����鎟�̃|�C���g
	D3DXVECTOR3					  m_FNor;				//�n�ʂ̖@��
	float						  m_fPosY;				//�ʉ��Z�ʒuY
	bool						  m_bDivided;			//�����ꓹ���ǂ���
	int							  m_nMap;				//��������}�b�v
	float						  m_fLength;			//����
	float						  m_fRoad;				//IN_OUT�́�
	float						  m_fTilt;				//��
	float						  m_fCTilt;				//�J�����p��
	float						  m_fRotOld;			//�O��RotY

	// ���[�V�����֐�	�V�K
	KEY_INFO						*m_pKeyInfo[MAX_MOTION];	//�L�[���ւ̃|�C���^
	int								m_nKey;						//���݂̃L�[�i���o�[
	int								m_nCountFlame;				//�t���[����
	int								m_nNumParts;				//�p�[�c��
	int								m_aIndexParent[MAX_PARTS];	//�e�̃C���f�b�N�X
	KEY								m_aKayOffset[MAX_PARTS];	//�I�t�Z�b�g���
	MOTION_INFO						m_aMotionInfo[MAX_MOTION];	//���[�V�������
	int								m_nMotionType;				//���[�V�����̃^�C�v(int�^)
	bool							m_bMotionEnd;				//���[�V�����I��
	D3DXVECTOR3						m_OffSetPos[MAX_PARTS];		//�p�[�c���Ƃ̍ŏ��̈ʒu

	CBillBoord					  *m_pPlayerNum;		// �v���C���[�ԍ��i�Ǐ]�j
	bool						  m_bGoal;				// �S�[��

	CBillBoord					  *m_pPlayerpos;		// �v���C���[
	float						  m_fAddRot;			// ���Z�p�x
};
#endif
