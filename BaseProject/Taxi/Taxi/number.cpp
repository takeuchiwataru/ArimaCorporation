//=============================================================================
//
// �������� [number.cpp]
// Author : �L�n�@���u
//
//=============================================================================
#include "number.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"
#include "texture.h"
//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================

//=============================================================================
//	�}�N����`
//=============================================================================
#define NUMBER_SIZE_X		(30.0f)									// �����̉���
#define NUMBER_SIZE_Y		(50.0f)									// �����̏c��

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CNumber::CNumber()
{
	m_pVtxBuff = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CNumber::~CNumber(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CNumber::Init(D3DXVECTOR3 pos, int nType)
{
	// �f�o�C�X�擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//�Q�[���̏��
	CManager::MODE pMode = CManager::GetMode();

	// �ԍ��̐ݒ�
	m_nType = nType;

	// ���_�o�b�t�@����
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_���ւ̃|�C���^
	VERTEX_2D * pVtx;

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if ((pMode == CManager::MODE_GAME) || (pMode == CManager::MODE_TUTORIAL))
	{
		pVtx[0].pos = D3DXVECTOR3(pos.x - NUMBER_SIZE_X, pos.y - NUMBER_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x + NUMBER_SIZE_X, pos.y - NUMBER_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x - NUMBER_SIZE_X, pos.y + NUMBER_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x + NUMBER_SIZE_X, pos.y + NUMBER_SIZE_Y, 0.0f);
	}
	else if (pMode == CManager::MODE_RANKING)
	{
		pVtx[0].pos = D3DXVECTOR3(pos.x - 15.0f, pos.y - 25.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x + 15.0f, pos.y - 25.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x - 15.0f, pos.y + 25.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x + 15.0f, pos.y + 25.0f, 0.0f);
	}

	for (int nCntrhw = 0; nCntrhw < 4; nCntrhw++)
	{
		pVtx[nCntrhw].rhw = 1.0f;
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
void CNumber::Uninit(void)
{
	// ���_�o�b�t�@�j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	delete this;
}

//=============================================================================
// �X�V����
//=============================================================================
void CNumber::Update(void){}

//=============================================================================
// �`�揈��
//=============================================================================
void CNumber::Draw(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 	pDevice = CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���ݒ�
	pDevice->SetTexture(0, *CTexture::GetTexture(CTexture::TYPE_TIME_NUMBER + m_nType));

	// �|���S���`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// �i���o�[�̐ݒ�
//=============================================================================
void CNumber::SetNumber(int nNumber)
{
	VERTEX_2D *pVtx;				//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N�����_�f�[�^�̃|�C���^���擾
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2((nNumber % 10) * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((nNumber % 10) * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((nNumber % 10) * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((nNumber % 10) * 0.1f + 0.1f, 1.0f);

		//���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// �T�C�Y�ݒ�
//=============================================================================
void CNumber::SetSize(D3DXVECTOR2 size, D3DXVECTOR2 pos)
{
	VERTEX_2D *pVtx;				//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N�����_�f�[�^�̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�T�C�Y�ݒ�
	pVtx[0].pos = D3DXVECTOR3(pos.x - size.x , pos.y - size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + size.x, pos.y - size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - size.x, pos.y + size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + size.x, pos.y + size.y, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �F�̐ݒ�
//=============================================================================
void CNumber::SetCol(D3DXCOLOR col)
{
	VERTEX_2D *pVtx;				//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N�����_�f�[�^�̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �F�̐ݒ�
	for (int nCntCol = 0; nCntCol < 4; nCntCol++)
	{
		pVtx[nCntCol].col = col;
	}

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

}