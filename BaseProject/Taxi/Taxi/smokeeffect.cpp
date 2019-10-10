//=============================================================================
//
// ��̃G�t�F�N�g���� [.cpp]
// Author : �������� Sato_Asumi
//
//=============================================================================
#include "smokeeffect.h"
#include "player.h"
#include "game.h"
#include "particlebillboad.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define SIZE			(D3DXVECTOR2(10.0f, 10.0f))	// �T�C�Y
#define LIFE			(15)						// ���C�t
#define ADD_SIZE		(1.0f)						// �T�C�Y�̉��Z
#define CUT_COLA		(0.1f)						// �F�̕ω���
#define FILENAME_TEX	("data\\TEXTURE\\game\\Particle\\smoke.png")
#define COLOR			(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f))

//=============================================================================
// �O���錾
//=============================================================================
LPDIRECT3DTEXTURE9 CSmokeEffect::m_pTexture = NULL;

//=============================================================================
// ��������
//=============================================================================
CSmokeEffect * CSmokeEffect::Create(D3DXVECTOR3 Pos)
{
	CSmokeEffect * pSmokeEffect = new CSmokeEffect;

	pSmokeEffect->SetPos(Pos);				//�ʒu�̐ݒ�
	pSmokeEffect->Init();					//����������

	return pSmokeEffect;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CSmokeEffect::CSmokeEffect(){}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSmokeEffect::~CSmokeEffect(){}

//=============================================================================
// �e�N�X�`���ǂݍ��ݏ���
//=============================================================================
void CSmokeEffect::LoadTex(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXCreateTextureFromFile(pDevice, FILENAME_TEX, &m_pTexture);
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CSmokeEffect::Unload(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CSmokeEffect::Init(void)
{
	int nRandSize = (rand() % 10) + 5;
	CParticleBillboad::SetSize(D3DXVECTOR2((float)nRandSize, (float)nRandSize));			//�T�C�Y�̐ݒ�
	CParticleBillboad::Init();					//����������
	CParticleBillboad::BindTexture(m_pTexture);	//�e�N�X�`���̐ݒ�
	CParticleBillboad::SetColor(COLOR);			//�F�̐ݒ�
	m_nLife = LIFE;
	m_fCol_A = 1.0f;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CSmokeEffect::Uninit(void)
{
	CParticleBillboad::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CSmokeEffect::Update(void)
{
	CParticleBillboad::Update();

	//�T�C�Y���擾�E�X�V
	D3DXVECTOR2 size = CParticleBillboad::GetSize();
	size.x += ADD_SIZE;
	size.y += ADD_SIZE;

	//�T�C�Y���Đݒ�
	CParticleBillboad::RemakeSize(size);

	//�\�����Ԃ̌��Z
	m_nLife--;

	if (m_nLife < 0)
	{//���C�t��0�ȉ��œ����ɂ��Ă���
		m_fCol_A -= CUT_COLA;
		CParticleBillboad::SetColor(D3DXCOLOR(COLOR.r, COLOR.g, COLOR.b, m_fCol_A));

		//�����ɂȂ�����j��
		if (m_fCol_A < 0.0f) { Uninit(); }
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CSmokeEffect::Draw(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// Z�e�X�g�����𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	CParticleBillboad::Draw();

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Z�e�X�g�������f�t�H���g�ɂ���
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}