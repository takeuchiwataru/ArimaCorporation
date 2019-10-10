//=============================================================================
//
// ���U���g�v���C���[���� [resultplayer.h]
// Author : Shun Yokomichi
//
//=============================================================================
#ifndef _RESULTPLAYER_H_
#define _RESULTPLAYER_H_

#include "main.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "motion.h"
#include "scene.h"
#include "taillamp.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define RESULT_MAX_PARTS	(7)		//�ǂݍ��ރp�[�c��
#define RESULT_MAX_LIFE		(15)	//���C�t
#define RESULT_MAX_TIRE		(4)		//�^�C���̍ő吔

//=============================================================================
// �O���錾
//=============================================================================
class CModel;
class CLoadTextPlayer;
class CTire;

//=============================================================================
// ���U���g�v���C���[�N���X
//=============================================================================
class CResultPlayer : public CScene
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
		STATE_SPEED_NONE = 0,
		STATE_SPEED_ACCEL,
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

	//�e�L�X�g���
	typedef struct
	{
		float fAccel;			//�����l
		float fBraks;			//�����l
		float fAddRot;			//�������]�l
		float fWindy;			//����
		float fDistance;		//�ړ�����
		int	  nCountTime;		//���Ԃ̌v�Z
		D3DXVECTOR3 FirstPos;	//�����ʒu
	}PLAYER_INFO;

	//�F�̏�Ԃ̎��
	CResultPlayer();
	~CResultPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CResultPlayer * Create(const D3DXVECTOR3 pos);
	static void LoadModel(void);
	static void UnloadModel(void);

	static D3DXVECTOR3 GetPos(void) { return m_pos; };
	static void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };

	D3DXVECTOR3 GetOldPos(void) { return m_OldPos; };

	D3DXVECTOR3 GetMove(void) { return m_move; };
	void SetMove(D3DXVECTOR3 move) { m_move = move; };

	D3DXVECTOR3 GetRot(void) { return m_rot; };
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };

	PLAYER_INFO GetPlayerInfo(void) { return m_PlayerInfo; };
	PLAYER_INFO * GetPlayerInfoPoint(void) { return &m_PlayerInfo; };
	STATE_SPEED GetStateSpeed(void) { return m_StateSpeed; };
	STATE GetState(void) { return m_state; };

private:
	//�ϐ��錾
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void RemakeAngle(float * pAngle);
	void UpdateMove(void);
	void CreateTire(void);
	void SetState(STATE state);
	void SetStateSpeed(STATE_SPEED state);
	void SetTailLampType(CTailLamp::TYPE type);
	void DirectingCount(void);

	static CModel *		m_pModel;		//�p�[�c���f���̃|�C���^
	static int				m_nMaxModel;	//�ǂݍ��ރ��f���̍ő吔
	static int				m_nMaxParts;	//�ǂݍ��ރp�[�c�̍ő吔

											//�����o�ϐ�
	static int	m_nMaxMotion;							//���[�V�����̍ő吔
	static CMotion::MOTION_INFO * m_pMotionInfo;		//���[�V�������
	static LPDIRECT3DTEXTURE9     m_pTexture;			//�e�N�X�`��
	static D3DXVECTOR3			  m_pos;				//���S���W
	D3DXVECTOR3					  m_move;				//�ړ�
	D3DXVECTOR3					  m_OldPos;				//�O��̍��W
	D3DXVECTOR3					  m_rot;				//����
	D3DXVECTOR3					  m_vtxMaxModel;		//���f���̒��_�ő�l
	D3DXVECTOR3					  m_vtxMinModel;		//���f���̒��_�ŏ��l
	D3DXMATRIX					  m_mtxWorld;			//���[���h�}�g���b�N�X
	CMotion *					  m_pMotion;			//���[�V�����̃|�C���^
	STATE						  m_state;				//��Ԑݒ�
	STATE_SPEED					  m_StateSpeed;			//�X�s�[�h�̏��
	PLAYER_INFO					  m_PlayerInfo;			//�v���C���[�̏��
	CLoadTextPlayer *			  m_pText;				//�v���C���[�̏��ǂݍ���
	CTire *						  m_pTire[RESULT_MAX_TIRE];	//�^�C���̃|�C���^
	CTailLamp *					  m_apTailLamp[2];		// �e�[�������v
	float						  m_fMaxSpeed;			//�X�s�[�h�̍ő�l
};
#endif
