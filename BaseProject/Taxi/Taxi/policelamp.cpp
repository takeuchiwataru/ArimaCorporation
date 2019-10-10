//=============================================================================
//
// �x�@�̃����v���� [policelanmp.cpp]
// Author : �����@�C���N
//
//=============================================================================
#include "policelamp.h"
#include "manager.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define POLICELAMP_TEX	"data\\TEXTURE\\effect000.jpg"			// �e�N�X�`���̖��O

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 CPoliceLamp::m_pTexture[MAX_POLICELAMP_TEX] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPoliceLamp::CPoliceLamp() : CScene3D(POLICELAMP_PRIOTITY, CScene::OBJTYPE_3DPOLYGON)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPoliceLamp::~CPoliceLamp()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPoliceLamp::Init()
{
	//����������
	CScene3D::Init();

	//��ނ̐ݒ�
	SetTypeNumber(TYPE_NONE);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPoliceLamp::Uninit(void)
{
	CScene3D::Uninit();
}
//=============================================================================
// �X�V����
//=============================================================================
void CPoliceLamp::Update(void)
{
	CScene3D::Update();
}
//=============================================================================
// �`�揈��
//=============================================================================
void CPoliceLamp::Draw(void)
{

	// �f�o�C�X�擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//�A���t�@�e�X�g
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CScene3D::Draw();

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�A���t�@�e�X�g
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, D3DZB_FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}
//=============================================================================
// ��������
//=============================================================================
CPoliceLamp * CPoliceLamp::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	CPoliceLamp *pPoliceLamp = NULL;
	//NULL�`�F�b�N
	if (pPoliceLamp == NULL)
	{//�������̓��I�m��

		pPoliceLamp = new CPoliceLamp;

		if (pPoliceLamp != NULL)
		{
			//����������
			pPoliceLamp->Init();
			//�ʒu�̐ݒ�
			pPoliceLamp->SetPosSize(pos, size);
			//�e�N�X�`���̐ݒ�
			pPoliceLamp->BindTexture(m_pTexture[0]);
		}
	}

	return pPoliceLamp;
}

//=============================================================================
// �ǂݍ��ݏ���
//=============================================================================
HRESULT CPoliceLamp::Load(void)
{
	// �f�o�C�X�擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// �e�N�X�`������
	D3DXCreateTextureFromFile(pDevice, POLICELAMP_TEX, &m_pTexture[0]);

	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CPoliceLamp::UnLoad(void)
{
	// �e�N�X�`���j��
	if (m_pTexture[0]!= NULL)
	{
		m_pTexture[0]->Release();
		m_pTexture[0] = NULL;
	}
}