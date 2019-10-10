//=============================================================================
//
// �G�t�F�N�g�}�l�[�W���[���� [effectmgr.cpp]
// Author : �������� Sato_Asumi
//
//=============================================================================
#include "effectmgr.h"
#include "manager.h"
#include "renderer.h"
#include "woodeffect.h"
#include "3Dparticle.h"
#include "game.h"
#include "player.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define FILENAME_TEX00	("data/TEXTURE/game/Particle/flower.png")
#define LEAF_TEX		(0)

//�t�B�[���h�͈̔�
#define FILD_MIN		(D3DXVECTOR2(-19400.0f, -8200.0f))
#define FILD_MAX		(D3DXVECTOR2(14600.0f, 10000.0f))

//�e���r���͈̔�
#define TV_MIN			(D3DXVECTOR2(1100.0f, 5720.0f))
#define TV_MAX			(D3DXVECTOR2(2220.0f, 6810.0f))

//�K���H�͈̔�
#define TANUKI00_MIN	(D3DXVECTOR2(6100.0f, 3100.0f))
#define TANUKI00_MAX	(D3DXVECTOR2(10210.0f, 7070.0f))
#define TANUKI01_MIN	(D3DXVECTOR2(6100.0f, -1970.0f))
#define TANUKI01_MAX	(D3DXVECTOR2(10210.0f, 1990.0f))
#define TANUKI02_MIN	(D3DXVECTOR2(6100.0f, -7100.0f))
#define TANUKI02_MAX	(D3DXVECTOR2(10210.0f, -3140.0f))

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CEffectMgr::m_pTexture = NULL;		//�e�N�X�`��

//=============================================================================
// ��������
//=============================================================================
CEffectMgr * CEffectMgr::Create(D3DXVECTOR3 pos, TYPE type)
{
	CEffectMgr * pEffectMgr = new CEffectMgr;

	pEffectMgr->SetType(type);
	pEffectMgr->Init();
	pEffectMgr->SetPos(pos);

	return pEffectMgr;
}

//=============================================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================================
void CEffectMgr::LoadTex(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXCreateTextureFromFile(pDevice, FILENAME_TEX00, &m_pTexture);
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CEffectMgr::UnloadTex(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEffectMgr::CEffectMgr(){}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEffectMgr::~CEffectMgr(){}

//=============================================================================
// ����������
//=============================================================================
void CEffectMgr::Init(void)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCounter = 0;
}

//=============================================================================
// �I������
//=============================================================================
void CEffectMgr::Uninit(void){}

//=============================================================================
// �X�V����
//=============================================================================
void CEffectMgr::Update(void)
{
	//�ϐ��錾
	CWoodEffect * pWoodEffect = NULL;

	m_nCounter++;

	if ((m_nCounter % 20) == 0)
	{

		//�^�C�v���Ƃ̐���
		switch (m_Type)
		{
		case TYPE_SUMMER:	//�t����
			pWoodEffect = CWoodEffect::Create(m_pos);
			pWoodEffect->BindTexture(m_pTexture);
			pWoodEffect->SetDivision(D3DXVECTOR2(0.25f, 1.0f));
			pWoodEffect->SetUV(m_Type, 0);
			break;
		}
	}
}