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

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_PARTS	(7)		//�ǂݍ��ރp�[�c��
#define MAX_LIFE	(15)	//���C�t
#define MAX_TIRE	(4)		//�^�C���̍ő吔
#define MAX_EGG		(3)		//���̍ő吔
#define MAX_FRAME	(60)

#define DAMAGE_TIME	(60)		// �_���[�W��H����Ă��鎞��
#define SPEEDUP_TIME	(60)	// �������Ă��鎞��
#define SPEEDDOWN_TIME	(300)	// �������Ă��鎞��
#define MAX_EGG		(3)		//���̍ő吔

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
	//�M�A�̏��
	typedef enum
	{
		STATE_DRIVE = 0,
		STATE_REVERSE,
		STATE_MAX,
	}STATE;

	//���s���
	typedef enum
	{
		STATE_SPEED_ACCEL = 0,
		STATE_SPEED_BRAKS,
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
		PLAYERSTATE_SPEEDDOWN,	// ����
		PLAYERSTATE_DAMAGE,		// �U���H�炤
		PLAYERSTATE_MAX,			//�ő吔
	} PLAYERSTATE;

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
	static CPlayer * Create(const D3DXVECTOR3 pos, int nPlayerNum = 0, int nControllerNum = 0);
	static void LoadModel(void);
	static void UnloadModel(void);

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
	STATE GetState(void) { return m_MoveState; };

	void CollisitionWall(void);
	void CollisionEgg(void);

	void SetControl(bool bControl) { m_bControl = bControl; };
	bool GetControl(void) { return m_bControl; };

	void SetJump(bool bJump) { m_bJump = bJump; }
	bool GetJump(void) { return m_bJump; };
	void SetCrcleCarIn(bool bCrcleCarIn) { m_bCrcleCarIn = bCrcleCarIn; }
	bool GetCrcleCarIn(void) { return m_bCrcleCarIn; }
	bool GetShake(void) { return m_bShake; }
	bool GetDrive(void) { return m_bDirive; }
	void SetDrive(bool bDrive) { m_bDirive = bDrive; }

private:
	//�ϐ��錾
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void RemakeAngle(float * pAngle);
	void RemakeCarRot(float * pAngle);
	float GetLength(D3DXVECTOR3 StartPos, D3DXVECTOR3 EndPos);
	void UpdateMove(void);
	void ControlKey(void);
	void DebugProc(void);
	void UpdateShake(void);
	void UpdateField(void);
	void SetState(STATE state);
	void SetStateSpeed(STATE_SPEED state);
	void SetStateHandle(STATE_HANDLE state) { m_StateHandle = state; };
	void CollisionObject(void);
	void CollisionFeed(void);
	void CarCalculate(D3DXVECTOR3 * TirePos);
	void UpdateStateJump(void);
	void PlaySoundObj(int nType, CSound * pSound);
	void EggAppear(CFeed *pFeed);
	void ChickAppear(void);
	void ChaseEgg(void);
	void BulletEgg(void);

	void CollisionCharacter(void);

	static CModel *		m_pModel;			//�p�[�c���f���̃|�C���^
	static int				m_nMaxModel;	//�ǂݍ��ރ��f���̍ő吔
	static int				m_nMaxParts;	//�ǂݍ��ރp�[�c�̍ő吔

											//�����o�ϐ�
	static int					  m_nMaxMotion;			// ���[�V�����̍ő吔
	static CMotion::MOTION_INFO * m_pMotionInfo;		// ���[�V�������
	static LPDIRECT3DTEXTURE9     m_pTexture;			// �e�N�X�`��
	D3DXVECTOR3				      m_pos;				// ���S���W
	D3DXVECTOR3					  m_move;				// �ړ�
	D3DXVECTOR3					  m_MoveMotion;			// ���[�V�����̈ړ���
	D3DXVECTOR3					  m_OldPos;				// �O��̍��W
	D3DXVECTOR3					  m_OldDiffuse;			// �O��̍���
	D3DXVECTOR3					  m_rot;				// ����
	D3DXVECTOR3					  m_OldEggRot[MAX_FRAME];	// �O��̈ʒu
	D3DXVECTOR3					  m_vtxMaxModel;		// ���f���̒��_�ő�l
	D3DXVECTOR3					  m_vtxMinModel;		// ���f���̒��_�ŏ��l
	D3DXMATRIX					  m_mtxWorld;			// ���[���h�}�g���b�N�X
	CMotion *					  m_pMotion;			// ���[�V�����̃|�C���^
	STATE						  m_MoveState;			// ��Ԑݒ�
	STATE_SPEED					  m_StateSpeed;			// �X�s�[�h�̏��
	STATE_HANDLE				  m_StateHandle;		// �n���h���̏��
	PLAYER_INFO					  m_PlayerInfo;			// �v���C���[�̏��
	CLoadTextPlayer *			  m_pText;				// �v���C���[�̏��ǂݍ���
	CEgg						  *m_pEgg[MAX_EGG];		//���̃|�C���^
	CChick						  *m_pChick[MAX_EGG];	//�Ђ悱�̃|�C���^
	bool						  m_bJump;				//  �W�����v�t���O
	bool						  m_bControl;			// �R���g���[���[�̎g�p���
	int							  m_nCountJumpTime;		// �W�����v��Ԃ̎��Ԃ��J�E���g����
	float						  m_fvtxMaxY;			// ���f�����_�̍ő�l�iY�j
	float						  m_fMaxSpeed;			// �X�s�[�h�̍ő�l
	float						  m_fMass;				// ����
	float						  m_fSpeed;				// ����
	int							  m_nCountTime;			// ���Ԃ̉��Z
	int							  m_nCountSpeed;		// ���Ԃ̉��Z
	bool						  m_bCrcleCarIn;		// �Ԃ��͈͓��ɓ����Ă��邩�ǂ���
	CLoadEffect *				  m_pLoadEffect;		// ���[�h�G�t�F�N�g�̃|�C���^�ϐ�
	int							  m_nCntCombo;
	int							  m_nCntShake;			//�h��̎��Ԃ��J�E���g����
	bool						  m_bShake;				//�h��̃I���I�t
	bool						  m_bDirive;			//�O�i�A��ނ̑���t���O
	int							  m_nCntFlag;
	int							  m_nNumEgg;
	int							  m_nNumChick;
	int							  m_nCntFrame;			// ���̂��Ă��鏈���Ɏg��
	bool						  m_abJump[MAX_FRAME];
	bool						  m_bJumpSave;

	bool						  m_bDamage;
	PLAYERSTATE					  m_State;
	int							  m_nCntDamage;
	int							  m_nPlayerNum;			// �v���C���[�ԍ�
	int							  m_nControllerNum;		// �R���g���[���[�ԍ�

	CBillBoord					  *m_pPlayerNum;		// �v���C���[�ԍ��i�Ǐ]�j
};
#endif
