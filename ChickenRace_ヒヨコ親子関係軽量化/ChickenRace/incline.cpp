//=============================================================================
//
// �X�Ώ��� [incline.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "incline.h"
#include "manager.h"
#include "renderer.h"
#include "debugProc.h"
#include "meshField.h"

//==================================
// �}�N����`
//==================================

//==================================
// �ÓI�����o�ϐ��錾
//==================================

//==================================
// ��������
//==================================
CIncline* CIncline::Create(void)
{
	CIncline *pIncline;

	pIncline = new CIncline;	// ���������m��

	if (pIncline != NULL)
	{// �������m�ې���
		pIncline->Init();
	}

	return pIncline;
}

//=========================================
// �R���X�g���N�^
//=========================================
CIncline::CIncline()
{
	m_pField = NULL;
	m_fJumpHeight = 0.0f;
	m_fSpeed = 0.0f;
	m_bJump = false;
	m_fClimbSpeed = 0.0f;
	m_fAngle = 0.0f;
	m_norField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=========================================
// �f�X�g���N�^
//=========================================
CIncline::~CIncline()
{
}

//=========================================
// ����������
//=========================================
HRESULT CIncline::Init(void)
{
	return S_OK;
}

//=========================================
// �I������
//=========================================
void CIncline::Uninit(void)
{
	// ������j��
	delete this;
}

//=========================================
// �X�V����
//=========================================
void CIncline::Update(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSpeed)
{
	m_fSpeed = fSpeed;
	m_fJumpHeight = 0.0f;

	if (NULL != m_pField)
	{// �n�ʂ̃|�C���^��NULL�łȂ��Ƃ�
		const float fAngleOld = m_fAngle;					// �����̌����Ă�������ɑ΂��Ă̌X�΂̊p�x
		const float fSpeedOld = m_fSpeed;

		m_fAngle = m_pField->GetInclineAngle(pos, rot.y);	// �����̌����Ă�������ɑ΂��Ă̌X�΂̊p�x�̎擾

		if (m_fAngle > 0.0f)
		{// �����Ƃ��ɑ��x���グ��
			m_fSpeed += fSpeed * m_fAngle * m_fClimbSpeed;
		}
		else if (m_fAngle < 0.0f)
		{// ���Ƃ��ɑ��x��������
			m_fSpeed -= fSpeed * fabsf(m_fAngle) * m_fClimbSpeed;
			if (1.0f > m_fSpeed) { m_fSpeed = 1.0f; }
		}

		if (m_fAngle > fAngleOld && fAngleOld <= 0.0f)
		{// �W�����v����Ƃ�
			m_fJumpHeight = (fabsf(fAngleOld) + 1.0f) * m_fSpeed * 5.0f;
			if (m_fJumpHeight > 0.0f)
			{// ��ɔ�񂾎��W�����v��Ԃɂ���
				m_bJump = true;
				m_fSpeed = fSpeedOld;
			}
		}
	}
}

//=========================================
// �X�΂ɉ������p�x�̎擾
//=========================================
D3DXMATRIX CIncline::GetRot(D3DXVECTOR3 pos, D3DXMATRIX mtxRot)
{
	D3DXMATRIX mtxRot1, mtxRot2;
	if (NULL != m_pField)
	{// �n�ʂ̃|�C���^��NULL�łȂ��Ƃ�
		if (!m_bJump) { m_norField = m_pField->GetNor(pos); }
		if (isnan(m_norField.x) || isnan(m_norField.z) || isnan(m_norField.y))
		{
			m_norField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		D3DXMatrixRotationYawPitchRoll(&mtxRot1, 0.0f, m_norField.z, -m_norField.x);

		D3DXMatrixMultiply(&mtxRot2, &mtxRot, &mtxRot1);
	}

	return mtxRot2;
}