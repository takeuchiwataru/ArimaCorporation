//==============================================================================
//
// �^�C�g���J�������� [titlecamera.cpp]
// Auther : �L�n ���u
//
//==============================================================================
#include "titlecamera.h"
#include "manager.h"
#include "title.h"
#include "game.h"
#include "player.h"
#include "object.h"

//===============================================================================
//�@�}�N����`
//===============================================================================
#define MOVE_CAMERA		(90.0f)							//�J�����̈ړ���
#define VECU_CAMERA		(30.0f)							//�J�����̈ړ���
#define MOVE_ANGLE		(0.06f)								//�J�����p�x�̈ړ���
#define HIGHT_V			(40.0f)								//���_�̍���
#define HIGHT_R			(25.0f)								//�����_�̍���

//===============================================================================
//�@�R���X�g���N�^
//===============================================================================
CTitleCamera::CTitleCamera() {}

//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CTitleCamera::~CTitleCamera() {}

//===============================================================================
//�@������
//===============================================================================
HRESULT CTitleCamera::Init(void)
{
	CCamera::Init();
	m_posR = D3DXVECTOR3(-450.0f, -30.0f, 0.0f);	//���_�̏����l
	m_posV = D3DXVECTOR3(270.0f, 80.0f, -1.0f);		//�����_�̏����l
	m_fRotDest = 0;
	m_nTypeCounter = 0;

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
void CTitleCamera::Uninit(void)
{
	CCamera::Uninit();
}

//===============================================================================
//�@�X�V����
//===============================================================================
void CTitleCamera::Updata(void)
{
	switch (m_cameraType)
	{
	case CAMERA_NORMAL:
		UpdateNormal();	//�L�����I�����
		break;
	case CAMERA_CHARSELECT:
		UpdateCharSelect();	//�L�����I�����
		break;
	case CAMERA_CHARUP:
		UpdateCharUp();		//�L�����A�b�v���
		break;
	case CAMERA_COURSE:
		UpdateCourse();		//�R�[�X���
		break;
	case CAMERA_GOUL:
		UpdateGoul();		//�S�[�����
		break;
	case CAMERA_PLAYER:
		UpdatePlayer();		//�v���C���[���
		break;
	}

	m_nTypeCounter++;
}
//===============================================================================
//�@�J�����̐ݒ�
//===============================================================================
void CTitleCamera::SetCamera(void)
{
	CCamera::SetCamera();
}

//=============================================================================
// ���Z�b�g
//=============================================================================
void CTitleCamera::SetTypeReset(void)
{
	switch (m_cameraType)
	{
	case CAMERA_GOUL:
		UpdateGoul(true);	//�S�[�����
		break;
	case CAMERA_PLAYER:
		UpdatePlayer(true);		//�v���C���[���
		break;
	}
	m_vecU.z = 0.0f;
}

//=============================================================================
// �^�C�g���J�����i�m�[�}���j�X�V����
//=============================================================================
void CTitleCamera::UpdateNormal(void)
{
	m_posR = D3DXVECTOR3(-450.0f, -30.0f, 0.0f);	//���_�̏����l
	m_posV = D3DXVECTOR3(270.0f, 80.0f, -1.0f);		//�����_�̏����l
}

//=============================================================================
// �^�C�g���J�����i�L�����I���j�X�V����
//=============================================================================
void CTitleCamera::UpdateCharSelect(void)
{
	m_posR = D3DXVECTOR3(3000.0f, -90.0f, 140.0f);
	m_posV = D3DXVECTOR3(2600.0f, -10.0f, 141.0f);
}

//=============================================================================
// �^�C�g���J�����i�L�����A�b�v�j�X�V����
//=============================================================================
void CTitleCamera::UpdateCharUp(void)
{
	if (m_pPlayer != NULL)
	{// NULL�ȊO
		float frot = ((float)m_nTypeCounter * 0.008f);

		D3DXVECTOR3 pos = m_pPlayer->Getpos();
		m_posR = pos + D3DXVECTOR3(0.0f, 30.0f, 0.0f);
		m_posV = m_posR + D3DXVECTOR3(sinf(-(D3DX_PI * 0.5f) + frot) * 30.0f, 0.0f, cosf(-(D3DX_PI * 0.5f) + frot) * 30.0f);
	}
}

//=============================================================================
// �^�C�g���J�����i�R�[�X�j�X�V����
//=============================================================================
void CTitleCamera::UpdateCourse(void)
{
	int nCount = m_nTypeCounter;

	if (m_nTypeCounter < CUORSE_VIEW_TIPE_0)
	{// 1���
		nCount = m_nTypeCounter;

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
	else if (m_nTypeCounter < CUORSE_VIEW_TIPE_1)
	{// 2���
		nCount = m_nTypeCounter - CUORSE_VIEW_TIPE_0;

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
	else if (m_nTypeCounter < CUORSE_VIEW_TIME - 60)
	{// 3���
		nCount = m_nTypeCounter - CUORSE_VIEW_TIPE_1;

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

	//if (m_nTypeCounter == CUORSE_VIEW_TIME - 120)
	//	SetType(CAMERA_NORMAL);
}

//=============================================================================
// �^�C�g���J�����i�S�[���j�X�V����
//=============================================================================
void CTitleCamera::UpdateGoul(bool bSet)
{
	m_vecU.z = 0.0f;

	D3DXVECTOR3 pos = m_pPlayer->GetPos();
	float frot = m_pPlayer->Getrot().y;

	m_posR = pos;

	// �ʒu�ݒ�
	m_posR += (pos - m_posR) * 0.08f;
	m_posV = m_posR + D3DXVECTOR3(sinf(frot) * 200.0f, 300.0f, cosf(frot) * 200.0f);
}

//=============================================================================
// �^�C�g���J�����i�v���C���[�j�X�V����
//=============================================================================
void CTitleCamera::UpdatePlayer(bool bSet)
{
	//�v���C���[�̏����擾����
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
		float		fSet = 0.2f;
		m_vecU.z = m_pPlayer->Getrot().z * -1.0f;
		fRotY += D3DX_PI * 0.5f;

		//�v���C���[�ɒǏ]�悤�ɂ���
		m_fRotDest = PlayerRot.y - m_rot.y;
		RemakeAngle(&m_fRotDest);

		m_rot.y += m_fRotDest * MOVE_ANGLE;

		if (bSet == true)
		{
			m_rot.y = PlayerRot.y;
			fSet = 1.0f;
		}

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

		if (bSet == true)
		{
			m_rot.x = fRotX;
		}

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
		m_posR += (WKPosR - m_posR) * fSet;
	}
}

//=============================================================================
// �`�悷����̂��Đݒ肷��
//=============================================================================
void CTitleCamera::DrawReset(void)
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
				pObject->SetDraw(Clipping(pObject->GetposR(), CModel3D::GetVtxMin(pObject->GetModelType()), CModel3D::GetVtxMax(pObject->GetModelType())));
			}
		}

		//Next�Ɏ���Scene������
		pScene = pSceneNext;
	}
}
