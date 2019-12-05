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
#define MOVE_CAMERA		(90.0f)							//�J�����̈ړ���
#define VECU_CAMERA		(30.0f)							//�J�����̈ړ���
#define MOVE_ANGLE		(0.06f)								//�J�����p�x�̈ړ���
#define HIGHT_V			(40.0f)							//���_�̍���
#define HIGHT_R			(25.0f)								//�����_�̍���

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
	m_fCameraAngle = 90;
	m_cameraType = CAMERA_NONE;
	m_pPlayer = NULL;
	m_fDistance = MOVE_CAMERA;
	m_fBackTime = 0.0f;
	m_fCntTime = 0.0f;
	m_fPlusDis = 0.0f;
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
	case CAMERA_CHARSELECT:
		UpdateCharSelect();	//�L�����I�����
		break;
	case CAMERA_CHARUP:
		UpdateCharUp();		//�L�����A�b�v���
		break;
	case CAMERA_COURSE:
		UpdateCourse();		//�R�[�X���
		break;
	case CAMERA_PLAYER:
		UpdatePlayer();		//�v���C���[���
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
// �Q�[���J�����i�L�����I���j�X�V����
//=============================================================================
void CGameCamera::UpdateCharSelect(void)
{
	m_posR = D3DXVECTOR3(3000.0f, -90.0f, 140.0f);
	m_posV = D3DXVECTOR3(2600.0f, -10.0f, 141.0f);
}

//=============================================================================
// �Q�[���J�����i�L�����A�b�v�j�X�V����
//=============================================================================
void CGameCamera::UpdateCharUp(void)
{
	if (m_pPlayer != NULL)
	{// NULL�ȊO
		int nCounter = CGame::GetGameCounter();
		float frot = ((float)nCounter * 0.008f);

		D3DXVECTOR3 pos = m_pPlayer->Getpos();
		m_posR = pos + D3DXVECTOR3(0.0f, 30.0f, 0.0f);
		m_posV = m_posR + D3DXVECTOR3(sinf(-(D3DX_PI * 0.5f) + frot) * 30.0f, 0.0f, cosf(-(D3DX_PI * 0.5f) + frot) * 30.0f);
	}
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
			m_posR = D3DXVECTOR3(-11000.0f, -1800.0f, -200.0f);
			m_posV = D3DXVECTOR3(-9000.0f, -1200.0f, 700.0f);
		}
		else
		{
			m_posR = D3DXVECTOR3(-11000.0f - (2.0f * nCount), -1800.0f + (2.0f * nCount), -200.0f + (1.0f * nCount));
			m_posV = D3DXVECTOR3(-9000.0f, -1200.0f + (4.0f * nCount), 700.0f);
		}
	}
	else if (nGameCounter < CUORSE_VIEW_TIPE_1)
	{// 2���
		nCount = nGameCounter - CUORSE_VIEW_TIPE_0;

		if (nCount == 0)
		{// �����l
			m_posR = D3DXVECTOR3(-2000, 0.0f, -4200.0f);
			m_posV = D3DXVECTOR3(-350, 800.0f, -3500.0f);
		}
		else
		{
			m_posR = D3DXVECTOR3(-2000 + (-3.0f * nCount), 0.0f, -4200.0f);
			m_posV = D3DXVECTOR3(-350, 800.0f, -3500.0f);
		}
	}
	else
	{// 3���
		nCount = nGameCounter - CUORSE_VIEW_TIPE_1;

		if (nCount == 0)
		{// �����l
			m_posR = D3DXVECTOR3(-450.0f, -30.0f, 0.0f);
			m_posV = D3DXVECTOR3(100.0f, 80.0f, -1.0f);
		}
		else
		{
			m_posV = D3DXVECTOR3(100.0f + (3.0f * nCount), 80.0f, -1.0f);
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
	m_fPlusDis *= 0.95f;

	if (m_pPlayer != NULL)
	{
		D3DXVECTOR3 PlayerPos = m_pPlayer->GetPos();
		D3DXVECTOR3 PlayerRot = m_pPlayer->GetRot();
		D3DXVECTOR3 PlayerMove = m_pPlayer->GetMove();
		D3DXVECTOR3 VecUPos;
		D3DXVECTOR3 WKPosR;
		float		fDistance = 0.0f;
		float		fRotX = 0.0f;
		float		fRotY = m_pPlayer->GetRot().y;
		m_vecU.z = m_pPlayer->Getrot().z * -1.0f;
		fRotY += D3DX_PI * 0.5f;

		//�v���C���[�ɒǏ]�悤�ɂ���
		m_fRotDest = PlayerRot.y - m_rot.y;
		RemakeAngle(&m_fRotDest);

		m_rot.y += m_fRotDest * MOVE_ANGLE;
		RemakeAngle(&m_rot.y);

		float fTilt = m_pPlayer->GetfTiltV() * 0.25f - 0.09f;
		float fWK = -m_pPlayer->GetfTiltV();
		float fMove = MOVE_CAMERA * (fWK * 1.5f + 1.0f);
		if (m_pPlayer->GetbJump()) { fMove += MOVE_CAMERA * 0.5f; }
		fWK += 1.0f;

		CRoad_Pointer::BentRotX(m_pPlayer->Getpos(), m_pPlayer->GetpEnmPoint(), fRotX, fDistance);
		if (m_pPlayer->GetbJump()) { fRotX -= D3DX_PI * 0.15f; }
		RemakeAngle(&fRotX);

		//���_�X�V
		fMove -= m_vecU.z * m_vecU.z * MOVE_CAMERA * 0.75f;
		if (fMove < MOVE_CAMERA * 0.25f) { fMove = MOVE_CAMERA * 0.25f; }
		VecUPos = D3DXVECTOR3(sinf(fRotY), 0.0f, cosf(fRotY)) * (m_vecU.z * VECU_CAMERA);

		fMove *= (1.0f + m_fPlusDis);
		m_rot.x += (fRotX - m_rot.x) * 0.05f;
		m_fDistance += (fMove - m_fDistance) * 0.05f;

		PlayerPos += VecUPos;
		m_posV = D3DXVECTOR3(
			(sinf(m_rot.y) * -m_fDistance) * cosf(m_rot.x),	//X��
			sinf(m_rot.x) * -m_fDistance + HIGHT_V,		//Y��
			(cosf(m_rot.y) * -m_fDistance) * cosf(m_rot.x)) + PlayerPos;	//Z��
																			//�����_�̍X�V
		WKPosR = D3DXVECTOR3(0.0f, HIGHT_R, 0.0f) + PlayerPos;	//����ꏊ + �����ړ���
		WKPosR += D3DXVECTOR3(sinf(m_rot.y), 0.0f, cosf(m_rot.y)) * (5.0f * ((-fWK + 2.0f) * 0.5f) + 30.0f);
		WKPosR.y += (-5.0f * ((fWK - 1.0f) * 1.0f) + 15.0f);
		m_posR += (WKPosR - m_posR) * 0.2f;

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