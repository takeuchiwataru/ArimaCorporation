//=============================================================================
//
// �e�[�������v���� [taillamp.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "taillamp.h"
#include "manager.h"
#include "renderer.h"

//==================================
// �}�N����`
//==================================
#define TAILLAMP_TEXTURE	"data\\TEXTURE\\effect000.jpg"			// �ǂݍ��ރe�N�X�`���t�@�C����
#define TAILLAMP_INTERVAL	(60)			// �_�ł̊Ԋu

//==================================
// �ÓI�����o�ϐ��錾
//==================================

//==================================
// ��������
//==================================
CTailLamp* CTailLamp::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXMATRIX *pMtxParent)
{
	CTailLamp *pTailLamp;

	pTailLamp = new CTailLamp;	// ���������m��

	if (pTailLamp != NULL)
	{// �������m�ې���
		pTailLamp->m_pos = pos;
		pTailLamp->m_size = size;
		pTailLamp->m_pMtxParent = pMtxParent;
		pTailLamp->Init();
	}

	return pTailLamp;
}

//=========================================
// �R���X�g���N�^
//=========================================
CTailLamp::CTailLamp()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_type = TYPE_NONE;
	m_nCntFlash = 0;
}

//=========================================
// �f�X�g���N�^
//=========================================
CTailLamp::~CTailLamp()
{
}

//=========================================
// ����������
//=========================================
HRESULT CTailLamp::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_col = D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f);

	// �e�N�X�`������
	D3DXCreateTextureFromFile(pDevice, TAILLAMP_TEXTURE, &m_pTexture);

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_3D,
								D3DPOOL_MANAGED,
								&m_pVtxBuff,
								NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_��
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, m_size.y, 0.0f) * 0.5f;
	pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, 0.0f) * 0.5f;
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, 0.0f) * 0.5f;
	pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y, 0.0f) * 0.5f;

	//�@���x�N�g��
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//���_�J���[
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//�e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@�̃A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CTailLamp::Uninit(void)
{
	if (NULL != m_pVtxBuff)
	{// ���_���̔j��
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// ������j��
	CScene::Release();
}

//=========================================
// �X�V����
//=========================================
void CTailLamp::Update(void)
{
	if (TYPE_BACK == m_type)
	{// �o�b�N�̂Ƃ�
		m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		if (0 == m_nCntFlash)
		{// ���点��
			SetColor(D3DXCOLOR(m_col.r, m_col.g, m_col.b, 1.0f));
		}
		else if (TAILLAMP_INTERVAL == m_nCntFlash)
		{// ���点�Ȃ�
			SetColor(D3DXCOLOR(m_col.r, m_col.g, m_col.b, 0.0f));
		}

		m_nCntFlash = (m_nCntFlash + 1) % (TAILLAMP_INTERVAL * 2);
	}
	else if (TYPE_BREAKING == m_type)
	{// �u���[�L�̂Ƃ�
		m_col = D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f);
		m_nCntFlash = 0;
		SetColor(D3DXCOLOR(m_col.r, m_col.g, m_col.b, 1.0f));
	}
	else if (TYPE_HAZARD_LAMP == m_type)
	{// �n�U�[�h�����v�̂Ƃ�
		m_col = D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f);
		if (0 == m_nCntFlash)
		{// ���点��
			SetColor(D3DXCOLOR(m_col.r, m_col.g, m_col.b, 1.0f));
		}
		else if (TAILLAMP_INTERVAL / 2 == m_nCntFlash)
		{// ���点�Ȃ�
			SetColor(D3DXCOLOR(m_col.r, m_col.g, m_col.b, 0.0f));
		}

		m_nCntFlash = (m_nCntFlash + 1) % TAILLAMP_INTERVAL;
	}
	else if (TYPE_NONE == m_type)
	{// �e�[�������v���g��Ȃ��Ƃ�
		m_nCntFlash = 0;
		SetColor(D3DXCOLOR(m_col.r, m_col.g, m_col.b, 0.0f));
	}
}

//=========================================
// �`�揈��
//=========================================
void CTailLamp::Draw(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X

	//�A���t�@�e�X�g
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// �e�̃}�g���b�N�X�Ɗ|�����킹��
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, m_pMtxParent);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�A���t�@�e�X�g
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, D3DZB_FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

//=========================================
// �F�̐ݒ�
//=========================================
void CTailLamp::SetColor(D3DXCOLOR col)
{
	m_col = col;

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//���_�o�b�t�@�̃A�����b�N����
	m_pVtxBuff->Unlock();
}

//=========================================
// ��ނ̐ݒ�
//=========================================
void CTailLamp::SetType(TYPE type)
{
	// �J�E���g���O�ɖ߂�
	m_nCntFlash = 0;

	// �^�C�v������
	m_type = type;
}