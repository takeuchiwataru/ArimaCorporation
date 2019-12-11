//==============================================================================
//
// ���U���g�J�������� [resultcamera.cpp]
// Auther : �L�n ���u
//
//==============================================================================
#include "resultcamera.h"
#include "game.h"
#include "result.h"

//===============================================================================
//�@�}�N����`
//===============================================================================

//===============================================================================
//�@�R���X�g���N�^
//===============================================================================
CResultCamera::CResultCamera() {}

//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CResultCamera::~CResultCamera() {}

//===============================================================================
//�@������
//===============================================================================
HRESULT CResultCamera::Init(void)
{
	CCamera::Init();
	// �ڕW�ʒu
	//m_posR = D3DXVECTOR3(-24400.0f, -3200.0f, 1200.0f);		//�����_�̏����l
	//m_posV = D3DXVECTOR3(-24100.0f, -3200.0f, 1450.0f);		//���_�̏����l

	// �����ƕ\����
	m_posR = D3DXVECTOR3(-24736.0f, -3150.0f, 1109.0f);
	m_posV = D3DXVECTOR3(-24588.0, -3150.0f, 1213.0f);

	// �p�x�v�Z
	//float fAngle = atan2f(m_posV.x - m_posR.x, m_posV.z - m_posR.z);

	// �ʒu�����i�p�x�v�Z�ς݁j
	m_posV = m_posR + D3DXVECTOR3(sinf(0.95f) * 500.0f, 0.0f, cosf(0.95f) * 500.0f);

	m_fRotDest = 0;
	return S_OK;
}

//===============================================================================
//�@�I������
//===============================================================================
void CResultCamera::Uninit(void)
{
	CCamera::Uninit();
}

//===============================================================================
//�@�X�V����
//===============================================================================
void CResultCamera::Updata(void)
{
	int nCounter = CResult::GetCounter();

	if (nCounter < RESULT_CAMERA_WAIT_0)
	{// wait

	}
	else if (nCounter < RESULT_CAMERA_DOWN)
	{// �g���t�B�[����̕\�����
		m_posR.y -= 1.2f;
		m_posV.y -= 1.2f;
	}
	else if (nCounter < RESULT_CAMERA_WAIT_1)
	{// wait

	}
	else if (nCounter < RESULT_CAMERA_ROLL)
	{// 
		int nFrame = nCounter - RESULT_CAMERA_WAIT_1;

		m_posR = m_posV + D3DXVECTOR3(sinf(0.95f + D3DX_PI - (0.01f * nFrame)) * 500.0f, 0.0f, cosf(0.95f + D3DX_PI - (0.01f * nFrame)) * 500.0f);
		m_posR.y -= nFrame;

		m_posV = m_posV + D3DXVECTOR3(sinf(0.95f + (-D3DX_PI * 0.5f)) * 1.0f, 0.0f, cosf(0.95f + (-D3DX_PI * 0.5f)) * 1.0f);
	}
	else if (nCounter < RESULT_CAMERA_WAIT_2)
	{// wait

	}
	else if (nCounter == RESULT_RANK_APP)
	{// �J����������
	 /*int nFrame = nCounter - RESULT_CAMERA_WAIT_2;

	 // �ڕW�ʒu��
	 m_posR.x += (-24400.0f - m_posR.x)	* (1.0f / (float)((RESULT_RANK_APP - RESULT_CAMERA_WAIT_2) - nFrame));
	 m_posR.y += (-3200.0f - m_posR.y)	* (1.0f / (float)((RESULT_RANK_APP - RESULT_CAMERA_WAIT_2) - nFrame));
	 m_posR.z += (1200.0f - m_posR.z)	* (1.0f / (float)((RESULT_RANK_APP - RESULT_CAMERA_WAIT_2) - nFrame));
	 m_posV.x += (-24100.0f - m_posV.x)	* (1.0f / (float)((RESULT_RANK_APP - RESULT_CAMERA_WAIT_2) - nFrame));
	 m_posV.y += (-3200.0f - m_posV.y)	* (1.0f / (float)((RESULT_RANK_APP - RESULT_CAMERA_WAIT_2) - nFrame));
	 m_posV.z += (1450.0f - m_posV.z)	* (1.0f / (float)((RESULT_RANK_APP - RESULT_CAMERA_WAIT_2) - nFrame));*/

		m_posR = D3DXVECTOR3(-24400.0f, -3200.0f, 1200.0f);
		m_posV = D3DXVECTOR3(-24100.0f, -3200.0f, 1450.0f);
	}
}
//===============================================================================
//�@�J�����̐ݒ�
//===============================================================================
void CResultCamera::SetCamera(void)
{
	CCamera::SetCamera();
}