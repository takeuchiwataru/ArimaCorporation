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
#include "ColMesh.h"

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
	m_posVDest = VECTOR_ZERO;				//���_�̏����l
	m_posRDest = VECTOR_ZERO;				//�����_�̏����l
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//�@���̌���
	m_rot = VECTOR_ZERO;					//����
	m_fCameraAngle = 0.0f;					//�p�x
	m_fRotDest = 0.0f;						//�ړI�̊p�x
	m_bCollCamera = false;					// �J��������
}

//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CCamera::~CCamera() {}

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
	m_fCameraAngle = 25.0f;						//�p�x

	m_viewport.X = 0;								// X�J�n�ʒu
	m_viewport.Y = 0;								// Y�J�n�ʒu
	m_viewport.Width = SCREEN_WIDTH;						// X�J�n�ʒu����̑傫��
	m_viewport.Height = SCREEN_HEIGHT;					// Y�J�n�ʒu����̑傫��
	m_viewport.MinZ = 0.0f;								// �ŏ�
	m_viewport.MaxZ = 1.0f;								// �ő�

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CCamera::Uninit(void) {}

//=============================================================================
// �X�V����
//=============================================================================
void CCamera::Update(void) {}

//=============================================================================
//�@�J�����̐���
//=============================================================================
void CCamera::SetCamera()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �r���[�|�[�g�ݒ�
	pDevice->SetViewport(&m_viewport);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH
	(
		&m_mtxProjection,
		D3DXToRadian(m_fCameraAngle),
		(float)m_viewport.Width / (float)m_viewport.Height,
		10.0f,
		60000.0f
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
bool CCamera::Clipping(D3DXVECTOR3 &pos, D3DXVECTOR3 VtxMin, D3DXVECTOR3 VtxMax)
{
	////�ϐ��錾
	//D3DXVECTOR3 Reference[MAX_POS];
	//D3DXVECTOR3 Pos[VECTOL] = { VECTOR_ZERO, VECTOR_ZERO };
	//D3DXVECTOR3 VecA;
	//D3DXVECTOR3 VecB;
	//bool		bDraw = false;
	//float		fAngle = 0.0f;
	float		fLength = 0.0f;

	////�S���_�̐ݒ�
	//Reference[0] = D3DXVECTOR3(VtxMin.x, 0.0f, VtxMax.z);
	//Reference[1] = D3DXVECTOR3(VtxMax.x, 0.0f, VtxMax.z);
	//Reference[2] = D3DXVECTOR3(VtxMin.x, 0.0f, VtxMin.z);
	//Reference[3] = D3DXVECTOR3(VtxMax.x, 0.0f, VtxMin.z);

	////�p�x�ݒ�
	//fAngle = (D3DX_PI * 0.5f) + (m_rot.y - D3DX_PI);
	//RemakeAngle(&fAngle);

	////�E���̉�p�̈ʒu�����߂�
	//Pos[0].x = sinf(fAngle) * -1500.0f + m_posV.x;
	//Pos[0].z = cosf(fAngle) * -1500.0f + m_posV.z;

	////�p�x�ݒ�
	//fAngle = (D3DX_PI * -0.5f) + (m_rot.y - D3DX_PI);
	//RemakeAngle(&fAngle);

	//�͈̓`�F�b�N
	fLength = sqrtf(powf(pos.x - VtxMax.x, 2) + powf(pos.z - VtxMax.z, 2));
	fLength += sqrtf(powf(pos.x - VtxMin.x, 2) + powf(pos.z - VtxMin.z, 2));

	if (fLength > IGNOR_SIZE) { return false; }

	float fValue = sqrtf(powf(m_posR.x - pos.x, 2) + powf(m_posR.z - pos.z, 2));
	if (fValue - fLength > OVER_DIS)
	{//�����`�F�b�N
		return true;
	}

	//for (int nCntPoint = 0; nCntPoint < MAX_POS; nCntPoint++)
	//{
	//	for (int nCntVec = 0; nCntVec < VECTOL; nCntVec++)
	//	{
	//		VecA = Pos[nCntVec] - m_posV;							//�J�����̈ʒu�����p�̏I�_�̃x�N�g�������߂�
	//		VecB = Reference[nCntPoint] - Pos[nCntVec];				//��p�̏I�_�ƈʒu�̃x�N�g�������߂�			
	//		float fCross = (VecA.z * VecB.x) - (VecA.x * VecB.z);	//�O�ς����߂�

	//																//�x�N�g����菬�������ǂ���
	//		if (fCross > 0)
	//		{
	//			return true;
	//		}
	//	}
	//}

	return false;
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

//=============================================================================
// �r���[�|�[�g����									(public)	*** CCamera ***
//=============================================================================
void CCamera::SetViewPort(DWORD X, DWORD Y, DWORD Width, DWORD Height)
{
	m_viewport.X = X;		// �ʒuX
	m_viewport.Y = Y;		// �ʒuY
	m_viewport.Width = Width;	// �T�C�YX
	m_viewport.Height = Height;	// �T�C�YY
	m_viewport.MinZ = 0.0f;		// �ŏ�
	m_viewport.MaxZ = 1.0f;		// �ő�
}