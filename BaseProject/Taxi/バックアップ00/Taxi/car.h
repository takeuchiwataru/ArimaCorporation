//=============================================================================
//
// NPC�Ԃ̏��� [object.h]
// Author : YUTARO�@ABE
//
//=============================================================================
#ifndef _CAR_H_
#define _CAR_H_

#include "model3D.h"
#include "taillamp.h"
#include "policelamp.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXT_CARNAME	"data\\TEXT\\�X�e�[�W6\\car.txt"			//�ǂݍ��ރe�L�X�g�t�@�C��
#define MAX_CAR						(10)			// �Ԃ̃��f����
#define MAX_CAR_TEXTURE				(1)				// �Ԃ̃e�N�X�`����
#define CAR_PRIOTITY				(2)				// �Ԃ̗D�揇��
#define INDXPOINT_MAX				(10)			// �Ԃ̃|�C���g�̍ő吔

//*****************************************************************************
//	�O���錾
//*****************************************************************************

//=====================
//  CModel3D�̔h���N���X
//=====================
class CCarBase : public CModel3D
{
public://�N�ł�������

	typedef enum
	{//�Ԃ̎��
		MODEL_NOMAL_CAR_000 = 0,	//	�ʏ�̎�[0]
		MODEL_NOMAL_CAR_001,		//	�ʏ�̎�[1]
		MODEL_NOMAL_CAR_002,		//	�ʏ�̎�[2]

		MODEL_SPORTS_CAR_000,		//	�X�|�[�c�J�[[0]
		MODEL_SPORTS_CAR_001,		//	�X�|�[�c�J�[[1]
		MODEL_SPORTS_CAR_002,		//	�X�|�[�c�J�[[2]

		MODEL_BUS_CAR_000,			//	�o�X[0]
		MODEL_BUS_CAR_001,			//	�o�X[1]
		MODEL_BUS_CAR_002,			//	�o�X[2]

		MODEL_POLICE_CAR,			//	�x�@

		CAR_MODEL_MAX
	}CAR_MODEL;

	const static int m_MaxCarModel = CAR_MODEL_MAX;			// �Ԃ̐�

	CCarBase();
	~CCarBase();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MoveMent(D3DXVECTOR3 VecA, float fAddRot, int bPoliceMoveValeu);
	D3DXVECTOR3 AddRod(D3DXVECTOR3 rot, D3DXVECTOR3 rotDest, float fAddRot);
	static CCarBase *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nTexType, int nObjectType, int nCollision, int route, bool bLoop);
	//void CollisitionPriority2(void);

	bool GetPointFar(void) { return m_bPointFar; }					//	�|�C���g�Ɖ������ǂ���
	int GetType(void) { return m_nType; }							//	�Ԃ̎�ނ̎擾
	int GetRoute(void) { return m_nRoute; }							//	���[�g�̎擾
	void SetRoute(int nRoute) { m_nRoute = nRoute; }				//	���[�g�̐ݒ�
	void SetPointFar(bool bPointFar) { m_bPointFar = bPointFar; }	//	�|�C���g�Ɖ������ǂ���
	bool GetLoop(void) { return m_bLoop; }							//	�ړ��Ń��[�v���邩�ǂ����̎擾
	void SetLoop(bool bLoop) { m_bLoop = bLoop; }					//	�ړ��Ń��[�v���邩�ǂ����ݒ�

	D3DXVECTOR3 GetInitPos(void) { return m_InitPos; }				//	�����ʒu�̎擾
	void SetInitPos(D3DXVECTOR3 InitPos) { m_InitPos = InitPos; }	//	�����ʒu�̐ݒ�

	bool GetOherHit(void) { return m_bOtherCarHit; }				//	�G���m�̓����蔻��擾
	void SetOherHit(bool bOtherCarHit) { m_bOtherCarHit = bOtherCarHit; }	//	�G���m�̓����蔻��ݒ�

private://	�l�ł̂ݎg��
	static LPD3DXMESH	m_pMesh[MAX_CAR];							//	���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER m_pBuffMat[MAX_CAR];						//	�}�e���A���̏��ւ̃|�C���^
	static DWORD		m_nNumMat[MAX_CAR];							//	�}�e���A���̏��
	static D3DXVECTOR3  m_LoadVtxMax[MAX_CAR];
	static D3DXVECTOR3  m_LoadVtxMin[MAX_CAR];
	static LPDIRECT3DTEXTURE9 m_pMeshTextures[MAX_CAR_TEXTURE];
	D3DXVECTOR3			m_Spin;										//  ��]
	int					m_nCount;									//  �J�E���^�[
	D3DXVECTOR3			m_ModelMove;								//  �ړ��̎d��
	int					m_nSelectNum;								//  �I�u�W�F�N�g�̕ύX
	int					m_nSetObjectNum;							//  �I�u�W�F�N�g�̐�
	D3DXVECTOR3			m_rotDest;									//  �ړI�̉�]
	float				m_fAngle;									//  ����
	int					m_nRoute;									//  �Ԃ̌o�H
	bool				m_bLoop;									//  �Ԃ̃��[�v
																	//  �t�@�C��
	int					m_nType;									//  ���
	int					m_nTexType;									//  �e�N�X�`���̎��
	int					m_nCollision;								//  �����蔻���ONOFF
	bool				m_bPointFar;								//	�|�C���g�Ɖ������ꍇ
	D3DXVECTOR3			m_InitPos;									//	�����ʒu
	bool 				m_bOtherCarHit;								//	�ԓ��m�������Ă��邩�ǂ���
};

//=====================
//	NPC�̎�
//=====================
class CCarNpc : public CCarBase
{
public://�N�ł�������
	CCarNpc();
	~CCarNpc();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetCarPointVec(void);						//	�Ԃ̃|�C���g�Ƃ̃x�N�g���̎擾
	void GoPointNormalCurve(D3DXVECTOR3 pos, int nRoute);	//	���̃|�C���g�Ɍ�����[�ʏ�̋Ȃ���]
	void CollisitionPriority2(void);

private://�l�ł̂ݎg��
	D3DXVECTOR3			m_posOld;							//  �ߋ��̈ʒu
	D3DXVECTOR3			m_VecA;								//  �������x�N�g���̒l
	int					m_nRouteCounter;					//  ���[�g�̃J�E���g
	int					m_nNextPoint;						//  ���̃|�C���g
	int					m_nIndexPoint[INDXPOINT_MAX];		//  �|�C���g�̔ԍ�
	int					m_nRoute;							//  �Ԃ̌o�H
	int					m_nNumPoint;						//  �������|�C���g�̔ԍ��̉��Z
	float				m_fLengthPlayer;					//  �v���C���[�Ƃ̋���
	float				m_fLengthPoint;						//	�|�C���g�Ƃ̋���
															//	�X�v���C�g�Ȑ��p�ϐ�
	int					m_nCurveSwitch;						//	�J�[�u�̐؂�ւ�
	D3DXVECTOR3			m_CurvePosDest;						//	�J�[�u���鎞�̖ړI�̈ʒu
	D3DXVECTOR3			m_LengthValue[2];					//	�v�Z�p�̋������i�[����l
	D3DXVECTOR3			m_posSave;							//	�ʒu�̕ۑ�
	bool				m_bCurve;							//	�J�[�u�����ǂ���
	int					m_nRotState;						//	�|��Ă�����
	static	int			m_nStopRoute;						//	�~�߂�Ԃ̃��[�g

};

//=====================
//	�x�@
//=====================
class CCarPolice : public CCarBase
{
public://�N�ł�������
	CCarPolice();
	~CCarPolice();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetPoliceVec(void);							//	�x�@�̃x�N�g���̎擾
	void RangeCheckOut(D3DXVECTOR3 pos, int nRoute);		//	�͈͊O�m�F
	void RangeCheckIn(D3DXVECTOR3 pos, float fLengthPlayer, int nRoute);	//	�͈͓��m�F
	void AskPoint(D3DXVECTOR3 pos, int nRoute);				//	���̃|�C���g�����߂�
	void GoPoint(D3DXVECTOR3 pos, int nRoute);				//	���̃|�C���g�Ɍ�����

private://�l�ł̂ݎg��
	D3DXVECTOR3			m_VecA;								//  �������x�N�g���̒l
	int					m_nRouteCounter;					//  ���[�g�̃J�E���g
	int					m_nIndexPoint[INDXPOINT_MAX];		//  �|�C���g�̔ԍ�
	int					m_nNumPoint;						//  �������|�C���g�̔ԍ��̉��Z
	float				m_fLengthPlayer;					//  �v���C���[�Ƃ̋���
															//  �x�@
	bool			    m_bPursuer;							//	�ǂ��Ă��邩�ǂ���
	float				m_fLengthPoint;						//	�|�C���g�Ƃ̋���
	float				m_fLengthPointB;					//	�v�Z�p
	int					m_nNumSavePoint;					//	�߂��|�C���g�̔ԍ��ۑ�
	float				m_fLengthNextPoint;					//	�߂��|�C���g�Ƃ��̎��̃|�C���g�̋���
	float				m_fLenPoliceNextPit;				//	�x�@�Ǝ��̃|�C���g�̋���
	D3DXVECTOR3			m_CurvePosDest;						//	�J�[�u���鎞�̖ړI�̈ʒu
	D3DXVECTOR3			m_LengthValue[2];					//	�v�Z�p�̋������i�[����l
	D3DXVECTOR3			m_posSave;							//	�ʒu�̕ۑ�
	int					m_nCurveSwitch;						//	�J�[�u�̐؂�ւ�
	int					m_nMoveState;						//	�����̏��
	float				m_fLengthFar;						//	�Ԃƃ|�C���g�̋���
	bool				m_bRestart;							//	�Ԃ�߂����ǂ���
	CPoliceLamp			*m_pPoliceLamp;						//	�x�@�̃����v
	int					m_nSirenCut;						//	�T�C�����J�E���^�[

};

#endif