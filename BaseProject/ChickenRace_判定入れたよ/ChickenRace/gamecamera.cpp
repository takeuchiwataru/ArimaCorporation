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
#include "fade.h"

//===============================================================================
//�@�}�N����`
//===============================================================================
#define MOVE_CAMERA		(500.0f)							//�J�����̈ړ���
#define MOVE_ANGLE		(0.06f)								//�J�����p�x�̈ړ���
#define HIGHT_V			(130.0f)							//���_�̍���
#define HIGHT_R			(60.0f)								//�����_�̍���

//===============================================================================
//�@�R���X�g���N�^
//===============================================================================
CGameCamera::CGameCamera() {}

//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CGameCamera::~CGameCamera() {}

//===============================================================================
//�@����������
//===============================================================================
HRESULT CGameCamera::Init(void)
{
	CCamera::Init();

	//�ϐ��̏�����
	m_posV = D3DXVECTOR3(0.0f, 500.0f, 0.0f);	//���_�̏����l
	m_posR = VECTOR_ZERO;						//�����_�̏����l

	m_cameraType = CAMERA_NONE;
	m_pPlayer = NULL;
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
	switch (m_cameraType)
	{
	case CAMERA_COURSE:
		UpdateCourse(); //�R�[�X���
		break;
	case CAMERA_PLAYER:
		UpdatePlayer(); //�v���C���[���
		break;
	}
}

//===============================================================================
//�@�J�����̐ݒ�
//===============================================================================
void CGameCamera::SetCamera(void)
{
	CCamera::SetCamera();
}

//=============================================================================
// �Q�[���J�����i�R�[�X�j�X�V����
//=============================================================================
void CGameCamera::UpdateCourse(void)
{
	int nGameCounter = CGame::GetGameCounter();
	int nCount = nGameCounter;

	if (nGameCounter < CUORSE_VIEW_TIPE_0)
	{// 1���
		nCount = nGameCounter;

		if (nCount == 0)
		{// �����l
			m_posR = D3DXVECTOR3(0.0f, 500.0f, 500.0f);
			m_posV = D3DXVECTOR3(0.0f, 1000.0f, -1200.0f);
		}
		else
		{
			m_posR = D3DXVECTOR3(0.0f, 500.0f, 500.0f + (5.0f * nCount));
		}
	}
	else if (nGameCounter < CUORSE_VIEW_TIPE_1)
	{// 2���
		nCount = nGameCounter - CUORSE_VIEW_TIPE_0;

		if (nCount == 0)
		{// �����l
			m_posR = D3DXVECTOR3(0.0f + 1000.0f, 300.0f, 100.0f);
			m_posV = D3DXVECTOR3(0.0f + 1000.0f + 1500.0f, 550.0f, -1.0f);
		}
		else
		{
			m_posR = D3DXVECTOR3(0.0f + 1000.0f, 300.0f, 100.0f + (4.0f * nCount));
			m_posV = D3DXVECTOR3(0.0f + 1000.0f + 1500.0f, 550.0f, -1.0f + (2.0f * nCount));
		}
	}
	else
	{// 3���
		nCount = nGameCounter - CUORSE_VIEW_TIPE_1;

		if (nCount == 0)
		{// �����l
			m_posR = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
			m_posV = D3DXVECTOR3(0.0f, 200.0f, -800.0f);
		}
		else
		{
			m_posV = D3DXVECTOR3(0.0f, 200.0f, -800.0f + (-2.0f * nCount));
		}
	}

	if (nGameCounter == CUORSE_VIEW_TIME)
		CFade::Create(CGame::GAMEMODE_PLAY);
}

//=============================================================================
// �Q�[���J�����i�v���C���[�j�X�V����
//=============================================================================
void CGameCamera::UpdatePlayer(void)
{
	//�v���C���[�̏����擾����
	CManager::MODE mode = CManager::GetMode();

	if (m_pPlayer != NULL)
	{
		D3DXVECTOR3 PlayerPos = m_pPlayer->GetPos();
		D3DXVECTOR3 PlayerRot = m_pPlayer->GetRot();
		D3DXVECTOR3 PlayerMove = m_pPlayer->GetMove();

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