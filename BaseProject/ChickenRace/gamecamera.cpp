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
#define MOVE_CAMERA		(250.0f)							//�J�����̈ړ���
#define MOVE_ANGLE		(0.06f)								//�J�����p�x�̈ړ���
#define HIGHT_V			(30.0f)							//���_�̍���
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

	m_cameraType = CAMERA_NONE;
	m_pPlayer = NULL;
	m_fDistance = MOVE_CAMERA;
	m_fBackTime = 0.0f;
	m_fCntTime = 0.0f;
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
	if (m_fBackTime > 0.0f) { m_fBackTime--; }

	if (m_pPlayer != NULL)
	{
		D3DXVECTOR3 PlayerPos = m_pPlayer->GetPos();
		D3DXVECTOR3 PlayerRot = m_pPlayer->GetRot();
		D3DXVECTOR3 PlayerMove = m_pPlayer->GetMove();
		D3DXVECTOR3 WKPosR;

		//�����_�̍X�V
		WKPosR = D3DXVECTOR3(0.0f, HIGHT_R, 0.0f) + PlayerPos;	//����ꏊ + �����ړ���

		//�v���C���[�ɒǏ]�悤�ɂ���
		m_fRotDest = PlayerRot.y - m_rot.y;
		RemakeAngle(&m_fRotDest);

		m_rot.y += m_fRotDest * MOVE_ANGLE;
		RemakeAngle(&m_rot.y);

		float fTilt = m_pPlayer->GetfTilt() * 0.25f - 0.075f;
		float fWK = m_pPlayer->GetfTilt();
		float fMove = MOVE_CAMERA * (fWK * 1.0f + 1.0f);
		fWK += 1.0f;
		if (fTilt < -0.15f) { fTilt = -0.15f; m_fBackTime = 300.0f; }
		if (fTilt > 0.1f) { fTilt = 0.1f; }
		if (fTilt > 0.0f) { m_fBackTime = 0.0f; }

		float fRot = (D3DX_PI * fTilt);

		if (m_fBackTime > 0) { fMove *= 2.0f; }
		m_rot.x += (fRot - m_rot.x) * 0.03f;

		if (fMove - m_fDistance < 0.0f && m_fBackTime == 0)
		{//�J���������Ȃ瑁��
			m_fDistance += (fMove - m_fDistance) * 0.1f;
		}
		else
		{//�����̂͂������
			m_fDistance += (fMove - m_fDistance) * 0.025f;
		}
		//���_�X�V
		m_posV = D3DXVECTOR3(
			(sinf(m_rot.y) * -m_fDistance) * cosf(m_rot.x),	//X��
			sinf(m_rot.x) * -m_fDistance + HIGHT_V,		//Y��
			(cosf(m_rot.y) * -m_fDistance) * cosf(m_rot.x)) + PlayerPos;	//Z��
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