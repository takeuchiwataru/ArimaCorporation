//=============================================================================
//
// �n�ʂRD�̏��� [polygon3D.cpp]
// Author : �L�n���u
//
//=============================================================================
#include "scene.h"
#include "polygon3D.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_NAME_1		"data\\TEXTURE\\A.png"				//�ǂݍ��ރe�N�X�`���t�@�C��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
D3DXVECTOR3 CPolygon3D::m_nor[4] = {};
//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CPolygon3D::CPolygon3D()
{	
	m_pVtxBuff = NULL;
	m_pTexture = NULL;
	m_Pos = D3DXVECTOR3(0.0f,0.0f,0.0f);	//�ʒu
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����
	m_bUse = false;							//���
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CPolygon3D::~CPolygon3D()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CPolygon3D::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �|���S���̈ʒu��ݒ�
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//����
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//����
	m_bUse = false;									//�g�p���Ă��邩�ǂ���

	// ���_���̍쐬
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL
	);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_Pos.x - 300.0f, 0.0f, m_Pos.z - 300.0f);
	pVtx[1].pos = D3DXVECTOR3(m_Pos.x - 300.0f, 200.0f, m_Pos.z + 300.0f);
	pVtx[2].pos = D3DXVECTOR3(m_Pos.x + 300.0f, 200.0f, m_Pos.z - 300.0f);
	pVtx[3].pos = D3DXVECTOR3(m_Pos.x + 300.0f, 0.0f, m_Pos.z + 300.0f);
	//�@���x�N�g��
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//���_�J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
	//�e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@�̃A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPolygon3D::Uninit(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();

	//�e�N�X�`���[�̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPolygon3D::Update(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();

	//�O�ςƂ̓v���C���[���E�ɂ��邩���ɂ��邩�@���O�ςŏo���Ă���B
	D3DXVECTOR3 vec0;
	D3DXVECTOR3 vec1;
	D3DXVECTOR3	nor0, nor1;					//�@��

	//============================
	//		�@�������߂�
	//============================

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//
	// ���_0�Ԗ�
	//
	//�x�N�g���v�Z��
	vec0 = pVtx[1].pos - pVtx[0].pos;
	//�ړI��������
	vec1 = pVtx[2].pos - pVtx[0].pos;

	//�O�όv�Z
	D3DXVec3Cross(&nor0, &vec0, &vec1);
	//���K��
	D3DXVec3Normalize(&nor0, &nor0);

	//
	// ���_3�Ԗ�
	//
	//�x�N�g���v�Z��
	vec0 = pVtx[2].pos - pVtx[3].pos;//vec0 ����@pos  //vec0�@����� nor ���� y������
	//�ړI��������
	vec1 = pVtx[1].pos - pVtx[3].pos;

	//�O�όv�Z
	D3DXVec3Cross(&nor1, &vec0, &vec1);
	//���K��
	D3DXVec3Normalize(&nor1, &nor1);

	//���_�ݒ�
	m_nor[0] = pVtx[0].nor = nor0;
	m_nor[1] = pVtx[1].nor = ((nor0 + nor1) / 2);
	m_nor[2] = pVtx[2].nor = ((nor0 + nor1) / 2);
	m_nor[3] = pVtx[3].nor = nor1;

	//���_�o�b�t�@�̃A�����b�N����
	m_pVtxBuff->Unlock();

	//�f�o�b�N�p
	CDebugProc::Print("nor[%.1f][%.1f][%.1f]\n", nor0.x, nor0.y, nor0.z);
	CDebugProc::Print("nor[%.1f][%.1f][%.1f]\n", nor1.x, nor1.y, nor1.z);

	//============================
	//     �f�o�b�N�p����
	//============================
	if (pCInputKeyBoard->GetKeyboardTrigger(DIK_2) == true)
	{
		Uninit();
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void CPolygon3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxView;		//�v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorldPolygon);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);

	D3DXMatrixMultiply(&m_mtxWorldPolygon, &m_mtxWorldPolygon, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);

	D3DXMatrixMultiply(&m_mtxWorldPolygon, &m_mtxWorldPolygon, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorldPolygon);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	pDevice->SetTexture(0, NULL);
}
//===============================================================================
//�@�N���G�C�g
//===============================================================================
CPolygon3D * CPolygon3D::Create(void)
{
	CPolygon3D *pScene3D = NULL;

	//NULL�`�F�b�N
	if (pScene3D == NULL)
	{//�������̓��I�m��

		pScene3D = new CPolygon3D;

		if (pScene3D != NULL)
		{
			//�I�u�W�F�N�g�N���X�̐���
			pScene3D->Init();
		}
	}

	return pScene3D;
}
//===============================================================================
//�@�v���C���[��pos.y����ς𗘗p���ďo��
//===============================================================================
float CPolygon3D::GetHeight(D3DXVECTOR3 pos)
{
	//�O�ςƂ̓v���C���[���E�ɂ��邩���ɂ��邩�@���O�ςŏo���Ă���B
	D3DXVECTOR3 vec0;
	D3DXVECTOR3 vec1;
	D3DXVECTOR3 vec2;
	int			nNumber;

	float		fVec;

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//============================
	//		�O�p�`�P����
	//============================
	//�x�N�g���v�Z�� �ړI - ����
	vec0 = pVtx[1].pos - pVtx[2].pos;
	//�x�N�g���v�Z�� �����̈ʒu�@- ����
	vec1 = D3DXVECTOR3(pos) - pVtx[2].pos;
	//�E�ɂ��邩���ɂ��邩 -���@+�E
	//�O�ϗp�v�Z��
	fVec = (vec0.x * vec1.z) - (vec0.z * vec1.x);

	if (fVec >= 0)
	{
		CDebugProc::Print("���P�͈� : TURE\n");
		vec0 = (pos - pVtx[0].pos);
		nNumber = 0;
	}
	else if (fVec <= 0)
	{
		CDebugProc::Print("���Q�͈� : TURE\n");
		vec0 = (pos - pVtx[3].pos);
		nNumber = 3;
	}

	//
	//����
	//
	//���ς̏o����
	((vec0.x * pVtx[nNumber].nor.x) + (vec0.y * pVtx[nNumber].nor.y) + (vec0.z * pVtx[nNumber].nor.z));

	vec0.y = (-(vec0.x * pVtx[nNumber].nor.x) - (vec0.z * pVtx[nNumber].nor.z)) / pVtx[nNumber].nor.y;

	pos.y = vec0.y;

	//�f�o�b�N�p
	//CDebugProc::Print("����vec0[%.1f][%.1f][%.1f]\n", vec0.x, vec0.y, vec0.z);

	//CDebugProc::Print("pos.y = %.1f\n", pos.y);

	//���_�o�b�t�@�̃A�����b�N����
	m_pVtxBuff->Unlock();

	return pos.y;
}
//===============================================================================
//�@polygon3D�̖@����������
//===============================================================================
void CPolygon3D::GetNor(D3DXVECTOR3 *nor)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	nor[0] = pVtx[0].nor;
	nor[1] = pVtx[1].nor;
	nor[2] = pVtx[2].nor;
	nor[3] = pVtx[3].nor;

	//���_�o�b�t�@�̃A�����b�N����
	m_pVtxBuff->Unlock();
}
