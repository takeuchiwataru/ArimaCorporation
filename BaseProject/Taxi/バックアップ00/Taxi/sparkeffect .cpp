//=============================================================================
//
// �ΉԂ̃G�t�F�N�g���� [sparkeffect.cpp]
// Author : �������� Sato_Asumi
//
//=============================================================================
#include "sparkeffect .h"
#include "player.h"
#include "game.h"
#include "particlebillboad.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define SIZE			(D3DXVECTOR2(4.0f, 1.0f))				// �T�C�Y
#define LIFE			(20)									// ���C�t
#define CUT_COLA		(0.1f)									// �F�̕ω���
#define FILENAME_TEX	("data\\TEXTURE\\effect000.jpg")		// �ǂݍ��ރe�N�X�`���t�@�C����
#define COL_YELLOW		(D3DXCOLOR(1.0f, 1.0f, 0.25f, 1.0f))	// ���F�̐ݒ�
#define COL_RED			(D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f))		// �ԐF�̐ݒ�
#define GRAVITY			(-0.3f)									// �d��

//=============================================================================
// �O���錾
//=============================================================================
LPDIRECT3DTEXTURE9 CSparkEffect::m_pTexture = NULL;

//=============================================================================
// ��������
//=============================================================================
CSparkEffect * CSparkEffect::Create(D3DXVECTOR3 Pos)
{
	CSparkEffect * pSmokeEffect = new CSparkEffect;

	pSmokeEffect->SetPos(Pos);				//�ʒu�̐ݒ�
	pSmokeEffect->Init();					//����������

	return pSmokeEffect;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CSparkEffect::CSparkEffect(){}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSparkEffect::~CSparkEffect(){}

//=============================================================================
// �e�N�X�`���ǂݍ��ݏ���
//=============================================================================
void CSparkEffect::LoadTex(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXCreateTextureFromFile(pDevice, FILENAME_TEX, &m_pTexture);
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CSparkEffect::Unload(void)
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
HRESULT CSparkEffect::Init(void)
{
	//�ϐ��錾
	float fAngle = (float)(rand() % 314) * 0.01f;
	float fHight = (float)((rand() % 3) + 2);
	float fSpeed = (float)((rand() % 13) + 1) * 0.1f;

	//�ϐ��̏�����
	CParticleBillboad::SetSize(SIZE);			//�T�C�Y�̐ݒ�
	CParticleBillboad::Init();					//����������
	CParticleBillboad::BindTexture(m_pTexture);	//�e�N�X�`���̐ݒ�
	CParticleBillboad::SetColor(COL_YELLOW);	//�F�̐ݒ�
	m_nLife = LIFE;
	m_fCol_A = 1.0f;

	//�ړ��ʂ����߂�
	m_move.x = sinf(fAngle) * fSpeed;
	m_move.z = cosf(fAngle) * fSpeed;
	m_move.y = fHight;

	//�F�̕ω��ʂ����߂�
	m_fCol_G = COL_YELLOW.g;
	m_fCol_B = COL_YELLOW.b;
	m_fChangeCol_G = (m_fCol_G - COL_RED.g) / (float)m_nLife;
	m_fChangeCol_B = (m_fCol_B - COL_RED.b) / (float)m_nLife;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CSparkEffect::Uninit(void)
{
	CParticleBillboad::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CSparkEffect::Update(void)
{
	CParticleBillboad::Update();

	//�ʒu�̎擾
	D3DXVECTOR3 pos = CParticleBillboad::GetPos();

	//�ʒu�̍X�V
	pos += m_move;
	CParticleBillboad::SetPos(pos);

	//�d�͂̉��Z
	m_move.y += GRAVITY;

	//�\�����Ԃ̌��Z
	m_nLife--;

	if (m_nLife < 0)
	{//���C�t��0�ȉ��œ����ɂ��Ă���
		m_fCol_A -= CUT_COLA;
	}

	//�F�̐ݒ�
	m_fCol_G -= m_fChangeCol_G;
	m_fCol_B -= m_fChangeCol_B;
	CParticleBillboad::SetColor(D3DXCOLOR(COL_RED.r, m_fCol_G, m_fCol_B, m_fCol_A));

	//�����ɂȂ�����j��
	if (m_fCol_A < 0.0f) { Uninit(); }
}

//=============================================================================
// �`�揈��
//=============================================================================
void CSparkEffect::Draw(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// Z�e�X�g�����𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	CParticleBillboad::Draw();

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// Z�e�X�g�������f�t�H���g�ɂ���
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}