//=============================================================================
//
// �r���{�[�h�������� [number.cpp]
// Author : �L�n�@���u
//
//=============================================================================
#include "billnumber.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"

//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9	CBillNumber::m_pTexture = NULL;

//=============================================================================
//	�}�N����`
//=============================================================================
#define BILLNUMBER_TEX			"data/TEXTURE/number000.png"	// �����e�N�X�`��
#define BILLNUMBER_SIZE_X		(5.0f)							// �����̉���
#define BILLNUMBER_SIZE_Y		(15.0f)							// �����̏c��

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBillNumber::CBillNumber()
{
	m_pVtxBuff = NULL;
	m_nNumber = 0;
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBillNumber::~CBillNumber()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBillNumber::Init(D3DXVECTOR3 pos)
{
	// �f�o�C�X�擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	
	//�Q�[���̏��
	CManager::MODE pMode = CManager::GetMode();

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

	m_pos = pos;

	//���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (pMode == CManager::MODE_GAME)
	{
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - BILLNUMBER_SIZE_X, m_pos.y + BILLNUMBER_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + BILLNUMBER_SIZE_X, m_pos.y + BILLNUMBER_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - BILLNUMBER_SIZE_X, m_pos.y - BILLNUMBER_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + BILLNUMBER_SIZE_X, m_pos.y - BILLNUMBER_SIZE_Y, 0.0f);
	}

	for (int nCntrhw = 0; nCntrhw < 4; nCntrhw++)
	{
		pVtx[nCntrhw].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	}

	for (int nCntCol = 0; nCntCol < 4; nCntCol++)
	{
		pVtx[nCntCol].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBillNumber::Uninit(void)
{
	// ���_�o�b�t�@�j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CBillNumber::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CBillNumber::Draw(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 	pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxView, mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�r���[�}�g���b�N�X���擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//�t�s���ݒ�
	m_mtxWorld._11 = mtxView._11;
	m_mtxWorld._12 = mtxView._21;
	m_mtxWorld._13 = mtxView._31;
	m_mtxWorld._21 = mtxView._12;
	m_mtxWorld._22 = mtxView._22;
	m_mtxWorld._23 = mtxView._32;
	m_mtxWorld._31 = mtxView._13;
	m_mtxWorld._32 = mtxView._23;
	m_mtxWorld._33 = mtxView._33;

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// �i���o�[�̐ݒ�
//=============================================================================
void CBillNumber::SetBillNumber(int nNumber)
{
	VERTEX_3D *pVtx;				//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N�����_�f�[�^�̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//����ւ���
	m_nNumber = nNumber;

	////�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2((m_nNumber % 10) * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((m_nNumber % 10) * 0.1f + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2((m_nNumber % 10) * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((m_nNumber % 10) * 0.1f + 0.1f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//	�ǂݍ��ݏ���
//=============================================================================
HRESULT CBillNumber::Load(void)
{
	// �f�o�C�X�擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// �e�N�X�`������
	D3DXCreateTextureFromFile(pDevice, BILLNUMBER_TEX, &m_pTexture);

	return S_OK;
}

//=============================================================================
//	�J������
//=============================================================================
void CBillNumber::Unload(void)
{
	//�e�N�X�`���j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
//===============================================================================
//  �J���[�̐ݒ菈��
//===============================================================================
void CBillNumber::SetColor(D3DXCOLOR *Color)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = Color[0];
	pVtx[1].col = Color[1];
	pVtx[2].col = Color[2];
	pVtx[3].col = Color[3];

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}