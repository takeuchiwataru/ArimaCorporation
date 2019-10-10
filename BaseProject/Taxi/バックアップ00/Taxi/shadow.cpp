//=============================================================================
//
// �e�̏��� [shadow.cpp]
// Author : �L�n���u
//
//=============================================================================
#include "scene.h"
#include "shadow.h"
#include "manager.h"
#include "renderer.h"
#include "scene3D.h"
#include "mesh.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	SHADOW_TEXTURE_NAME_1	"data\\TEXTURE\\effect000.jpg"			//�ǂݍ��ރe�N�X�`���t�@�C����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
LPDIRECT3DTEXTURE9	CShadow::m_pTexture[MAX_BILLBOORD_TEX] = {};
float CShadow::m_fMeshHeight = 0.0f;

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CShadow::CShadow() : CScene3D(SHADOW_PRIOTITY, CScene::OBJTYPE_SHADOW)
{
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CShadow::~CShadow(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CShadow::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	CScene3D::Init();

	//����̓r���{�[�h���G�t�F�N�g���̔���
	CScene3D::SetTypeNumber(CScene3D::TYPE_SHADOW);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CShadow::Uninit(void)
{
	CScene3D::Uninit();
}
//=============================================================================
// �X�V����
//=============================================================================
void CShadow::Update(void){}

//=============================================================================
// �`�揈��
//=============================================================================
void CShadow::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�A���t�@�e�X�g
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//�A���t�@�e�X�g����
	//�f�t�H���g�̏ꍇFALSE�ɂȂ��Ă��邩��TRUE�ɂ���
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	//���Z�����̐ݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CScene3D::Draw();

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�f�t�H���g�̏ꍇFALSE�ɂȂ��Ă��邩��TRUE�ɂ���
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}
//===============================================================================
//�@�N���G�C�g
//===============================================================================
CShadow * CShadow::Create(D3DXVECTOR3 pos, float fWidth, float fDepth,int nTexType)
{
	CShadow *pShadow = NULL;

	//NULL�`�F�b�N
	if (pShadow == NULL)
	{//�������̓��I�m��

		pShadow = new CShadow;

		if (pShadow != NULL)
		{
			//�I�u�W�F�N�g�N���X�̐���
			pShadow->Init();
			//�e�N�X�`���̊��蓖��
			pShadow->BindTexture(m_pTexture[nTexType]);
			//�ʒu����T�C�Y���o��
			pShadow->SetGroundPosSize(pos, D3DXVECTOR2(fWidth, fDepth));
			//�����̐ݒ�
			pShadow->m_fWidth = fWidth;
			//���s�̐ݒ�
			pShadow->m_fDepth = fDepth;
			//�ʒu�̊��蓖��
			pShadow->SetPosition(pos);
			//�ʒu�̐ݒ�
			pShadow->m_pos = pos;
		}
	}

	return pShadow;
}

//=============================================================================
// �e�̈ʒu��ݒ�
//=============================================================================
void CShadow::SetPositionShadow(D3DXVECTOR3 pos)
{
	//�ʒu�𓮂���
	m_pos = pos;
	CScene3D::SetPosition(m_pos);
}
//===============================================================================
// �e�N�X�`���̓ǂݍ���
//===============================================================================
HRESULT CShadow::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, SHADOW_TEXTURE_NAME_1, &m_pTexture[0]);

	return S_OK;
}

//===============================================================================
// �e�N�X�`���̔j��
//===============================================================================
void CShadow::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_BILLBOORD_TEX; nCount++)
	{
		//�e�N�X�`���[�̔j��
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}
