//=============================================================================
//
// �J�����x�[�X���� [camerabace.cpp]
// Author : ��������
//
//=============================================================================
#include "camerabace.h"
#include "manager.h"
#include "input.h"
#include "scene.h"
#include "player.h"
#include "game.h"
#include "title.h"

//*****************************************************************************
// �}�N����`
//****************************************************************************
#define VECTOL			(2)									//�x�N�g��
#define MAX_POS			(4)									//�Q�ƈʒu�̍ő吔

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CCamera::CCamera()
{
	m_posV = VECTOR_ZERO;					//���_�̏����l
	m_posR = VECTOR_ZERO;					//�����_�̏����l
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//�@���̌���
	m_rot = VECTOR_ZERO;					//����
}

//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CCamera::~CCamera(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCamera::Init(void)
{
	m_posV = VECTOR_ZERO;						//���_�̏����l
	m_posR = VECTOR_ZERO;						//�����_�̏����l
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//�@���̌���
	m_rot = VECTOR_ZERO;						//����
	m_fRotDest = 0;
	m_fCameraAngle = 45.0f;						//�p�x

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CCamera::Uninit(void){}

//=============================================================================
// �X�V����
//=============================================================================
void CCamera::Update(void){}

//=============================================================================
//�@�J�����̐���
//=============================================================================
void CCamera::SetCamera()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH
	(
		&m_mtxProjection,
		D3DXToRadian(m_fCameraAngle),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		30000
	);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	// �r���[�}�g���b�N�X���쐬
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//=============================================================================
// �N���b�s���O����
//=============================================================================
bool CCamera::Clipping(D3DXVECTOR3 VtxMin, D3DXVECTOR3 VtxMax)
{
	//�ϐ��錾
	D3DXVECTOR3 Reference[MAX_POS];
	D3DXVECTOR3 Pos[VECTOL] = {VECTOR_ZERO, VECTOR_ZERO };
	D3DXVECTOR3 VecA;
	D3DXVECTOR3 VecB;
	bool		bDraw = false;
	float		fAngle = 0.0f;

	//�S���_�̐ݒ�
	Reference[0] = D3DXVECTOR3(VtxMin.x, 0.0f, VtxMax.z);
	Reference[1] = D3DXVECTOR3(VtxMax.x, 0.0f, VtxMax.z);
	Reference[2] = D3DXVECTOR3(VtxMin.x, 0.0f, VtxMin.z);
	Reference[3] = D3DXVECTOR3(VtxMax.x, 0.0f, VtxMin.z);

	//�p�x�ݒ�
	fAngle = (D3DX_PI * 0.5f) + (m_rot.y - D3DX_PI);
	RemakeAngle(&fAngle);

	//�E���̉�p�̈ʒu�����߂�
	Pos[0].x = sinf(fAngle) * -1500.0f + m_posV.x;
	Pos[0].z = cosf(fAngle) * -1500.0f + m_posV.z;

	//�p�x�ݒ�
	fAngle = (D3DX_PI * -0.5f) + (m_rot.y - D3DX_PI);
	RemakeAngle(&fAngle);

	//�����̉�p�̈ʒu�����߂�
	Pos[1].x = sinf(fAngle) * 1500.0f + m_posV.x;
	Pos[1].z = cosf(fAngle) * 1500.0f + m_posV.z;

	//�͈̓`�F�b�N
	for (int nCntPoint = 0; nCntPoint < MAX_POS; nCntPoint++)
	{
		for (int nCntVec = 0; nCntVec < VECTOL; nCntVec++)
		{
			VecA = Pos[nCntVec] - m_posV;							//�J�����̈ʒu�����p�̏I�_�̃x�N�g�������߂�
			VecB = Reference[nCntPoint] - Pos[nCntVec];				//��p�̏I�_�ƈʒu�̃x�N�g�������߂�			
			float fCross = (VecA.z * VecB.x) - (VecA.x * VecB.z);	//�O�ς����߂�

			//�x�N�g����菬�������ǂ���
			if (fCross <= 0)
			{
				bDraw = true;		//�`���Ԃɂ���
				return bDraw;
			}
			else
			{
				bDraw = false;		//�`�悵�Ȃ���Ԃɂ���
			}
		}
	}

	return bDraw;
}

//=============================================================================
// ���S�_�ł̃N���b�s���O����
//=============================================================================
bool CCamera::ClippingPos(D3DXVECTOR3 ObjPos)
{
	//�ϐ��錾
	D3DXVECTOR3 Pos[2];
	D3DXVECTOR3 VecA;
	D3DXVECTOR3 VecB;
	bool		bDraw = false;
	float		fAngle = 0.0f;

	//�p�x�ݒ�
	fAngle = (D3DX_PI * 0.5f) + (m_rot.y - D3DX_PI);
	RemakeAngle(&fAngle);

	//�E���̉�p�̈ʒu�����߂�
	Pos[0].x = sinf(fAngle) * -1500.0f + m_posV.x;
	Pos[0].z = cosf(fAngle) * -1500.0f + m_posV.z;

	//�p�x�ݒ�
	fAngle = (D3DX_PI * -0.5f) + (m_rot.y - D3DX_PI);
	RemakeAngle(&fAngle);

	//�����̉�p�̈ʒu�����߂�
	Pos[1].x = sinf(fAngle) * 1500.0f + m_posV.x;
	Pos[1].z = cosf(fAngle) * 1500.0f + m_posV.z;

	//�͈̓`�F�b�N
	for (int nCntVec = 0; nCntVec < VECTOL; nCntVec++)
	{
		VecA = Pos[nCntVec] - m_posV;							//�J�����̈ʒu�����p�̏I�_�̃x�N�g�������߂�
		VecB = ObjPos - Pos[nCntVec];							//��p�̏I�_�ƈʒu�̃x�N�g�������߂�			
		float fCross = (VecA.z * VecB.x) - (VecA.x * VecB.z);	//�O�ς����߂�

		//�x�N�g����菬�������ǂ���
		if (fCross <= 0)
		{
			bDraw = true;		//�`�悵�Ȃ���Ԃɂ���
			break;
		}
		else
		{
			bDraw = false;		//�`�悷���Ԃɂ���
		}
	}

	return bDraw;
}

//=============================================================================
// �p�x�C��
//=============================================================================
void CCamera::RemakeAngle(float * pAngle)
{
	//�ړI�̊p�x�C��
	if (*pAngle < -D3DX_PI)
	{
		*pAngle += D3DX_PI * 2.0f;
	}
	if (*pAngle > D3DX_PI)
	{
		*pAngle -= D3DX_PI * 2.0f;
	}
}