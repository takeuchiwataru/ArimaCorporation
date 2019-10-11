//=============================================================================
//
// �Q�[���J�������� [gamecamera.cpp]
// Author : ��������
//
//=============================================================================
#include "gamecamera.h"
#include "game.h"
#include "player.h"
#include "scene.h"
#include "object.h"
#include "manager.h"
#include "tutorial.h"
//===============================================================================
//�@�}�N����`
//===============================================================================
#define MOVE_CAMERA		(290.0f)							//�J�����̈ړ���
#define MOVE_ANGLE		(0.06f)								//�J�����p�x�̈ړ���
#define WINDY			(0.4f)								//����
#define HIGHT_V			(100.0f)							//���_�̍���
#define HIGHT_R			(60.0f)								//�����_�̍���
#define PUTON_DISTANCE	(D3DXVECTOR3(150.0f, 0.0f, 150.0f))	//��ԏ�Ԃ̃J��������
#define ADD_ANGLE		(0.04f)								//�p�x�̉��Z
#define POSV_DISTANCE	(200.0f)							//���_�̋���
#define POSV_MOVE		(0.1f)								//�ړ���

//===============================================================================
//�@�R���X�g���N�^
//===============================================================================
CGameCamera::CGameCamera(){}

//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CGameCamera::~CGameCamera(){}

//===============================================================================
//�@����������
//===============================================================================
HRESULT CGameCamera::Init(void)
{
	CCamera::Init();

	//�ϐ��̏�����
	m_posV = D3DXVECTOR3(0.0f, 500.0f, 0.0f);	//���_�̏����l
	m_posR = VECTOR_ZERO;						//�����_�̏����l
	m_DestPosV = VECTOR_ZERO;					//�ړI�̈ʒu
	m_Move = VECTOR_ZERO;						//�ړ���
	m_fRotDest = 0;
	m_bSet = true;								//�J�����̐ݒ�
	m_bSetAngle = false;						//�p�x��ݒ肵�����ǂ���
	m_fOldRotY = 0.0f;							//�O��̌���
	m_fAngle = 0.0f;							//�p�x
	m_fHoldAngle = 0.0f;						//�ۊǂ���p�x
	m_fAddDirecting = 0.0f;
	return S_OK;
}

//===============================================================================
//�@�I������
//===============================================================================
void CGameCamera::Uninit(void)
{
	CCamera::Uninit();
}

//===============================================================================
//�@�X�V����
//===============================================================================
void CGameCamera::Update(void)
{
	UpdateNormal(); //�ʏ���
}

//===============================================================================
//�@�J�����̐ݒ�
//===============================================================================
void CGameCamera::SetCamera(void)
{
	CCamera::SetCamera();
}

//=============================================================================
// �Q�[���J�����ʏ�X�V����
//=============================================================================
void CGameCamera::UpdateNormal(void)
{
	//�v���C���[�̏����擾����
	CPlayer * pPlayer = NULL;
	CManager::MODE mode = CManager::GetMode();

	if (mode == CManager::MODE_GAME)
	{//�Q�[��
		pPlayer = CGame::GetPlayer();
	}
	else if (mode == CManager::MODE_TUTORIAL)
	{//�`���[�g���A��
		pPlayer = CTutorial::GetPlayer();
	}

	D3DXVECTOR3 PlayerPos = pPlayer->GetPos();
	D3DXVECTOR3 PlayerRot = pPlayer->GetRot();
	D3DXVECTOR3 PlayerMove = pPlayer->GetMove();

	//�����_�̍X�V
	m_posR.x = PlayerPos.x;
	m_posR.z = PlayerPos.z;
	m_posR.y = PlayerPos.y + HIGHT_R;

	//�v���C���[�ɒǏ]�悤�ɂ���
	m_fRotDest = PlayerRot.y - m_rot.y;
	RemakeAngle(&m_fRotDest);

	m_rot.y += m_fRotDest * MOVE_ANGLE;
	RemakeAngle(&m_rot.y);

	float fRot = m_rot.y - D3DX_PI;
	RemakeAngle(&fRot);

	//���_�X�V
	m_posV.x = PlayerPos.x - sinf(fRot) * -MOVE_CAMERA;
	m_posV.z = PlayerPos.z - cosf(fRot) * -MOVE_CAMERA;
	m_posV.y = PlayerPos.y + HIGHT_V;
}

//=============================================================================
// �`�悷����̂��Đݒ肷��
//=============================================================================
void CGameCamera::DrawReset(void)
{
	//�I�u�W�F�N�g�̕`����Č����E�Đݒ肷��
	CScene *pScene;

	//�v���C�I���e�B�[�`�F�b�N
	pScene = CScene::GetTop(OBJECT_PRIOTITY);

	//NULL�`�F�b�N
	while (pScene != NULL)
	{
		//Update��Uninit����Ă��܂��ꍇ�@Next��������\�������邩��Next�Ƀf�[�^���c���Ă���
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetObjType() == CScene::OBJTYPE_OBJECT)
		{//�^�C�v����Q��������
			CObject *pObject = (CObject*)pScene;
			
			if (pObject->GetDraw() == false)
			{//������x�`�攻�������
				pObject->SetDraw(Clipping(pObject->GetVtxMin(), pObject->GetVtxMax()));
			}
		}

		//Next�Ɏ���Scene������
		pScene = pSceneNext;
	}
}