//=============================================================================
//
// ��̃G�t�F�N�g���� [.cpp]
// Author : �������� Sato_Asumi
//
//=============================================================================
#include "grasseffect.h"
#include "player.h"
#include "game.h"
#include "particlebillboad.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define SIZE			(D3DXVECTOR2(10.0f, 10.0f))	// �T�C�Y
#define LIFE			(10)						// ���C�t
#define ADD_SIZE		(1.0f)						// �T�C�Y�̉��Z
#define CUT_COLA		(0.1f)						// �F�̕ω���
#define FILENAME_TEX	("data\\TEXTURE\\game\\Particle\\Grass.png")
#define GRAVITY			(-0.15f)					//�d��

//=============================================================================
// �O���錾
//=============================================================================
LPDIRECT3DTEXTURE9 CGrassEffect::m_pTexture = NULL;

//=============================================================================
// ��������
//=============================================================================
CGrassEffect * CGrassEffect::Create(D3DXVECTOR3 Pos, int nNumPattern)
{
	CGrassEffect * pGrassEffect = new CGrassEffect;

	pGrassEffect->SetPos(Pos);				//�ʒu�̐ݒ�
	pGrassEffect->SetNumPattern(nNumPattern);	//UV�p�^�[���̔ԍ���ݒ�
	pGrassEffect->Init();					//����������

	return pGrassEffect;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CGrassEffect::CGrassEffect(){}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGrassEffect::~CGrassEffect(){}

//=============================================================================
// �e�N�X�`���ǂݍ��ݏ���
//=============================================================================
void CGrassEffect::LoadTex(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXCreateTextureFromFile(pDevice, FILENAME_TEX, &m_pTexture);
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CGrassEffect::Unload(void)
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
HRESULT CGrassEffect::Init(void)
{
	//�ϐ��錾
	float fAngle = (float)(rand() % 10) * 0.1f;
	float fHight = (float)((rand() % 4) + 2);
	float fSpeed = (float)((rand() % 15) + 3) * 0.1f;

	int nRandSize = (rand() % 6) + 1;
	CParticleBillboad::SetSize(D3DXVECTOR2((float)nRandSize, (float)nRandSize));			//�T�C�Y�̐ݒ�
	CParticleBillboad::Init();					//����������
	CParticleBillboad::BindTexture(m_pTexture);	//�e�N�X�`���̐ݒ�
	CParticleBillboad::SetUV(D3DXVECTOR2((0.25f * m_nNumPattern), 0.0f), D3DXVECTOR2(((0.25f * m_nNumPattern) + 0.25f), 1.0f));		//UV�ݒ�
	m_nLife = LIFE;
	m_fCol_A = 1.0f;

	//�ړ��ʂ����߂�
	m_move.x = sinf(D3DX_PI * fAngle) * fSpeed;
	m_move.z = cosf(D3DX_PI * fAngle) * fSpeed;
	m_move.y = fHight;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CGrassEffect::Uninit(void)
{
	CParticleBillboad::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CGrassEffect::Update(void)
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
		CParticleBillboad::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fCol_A));

		//�����ɂȂ�����j��
		if (m_fCol_A < 0.0f) { Uninit(); }
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CGrassEffect::Draw(void)
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