//=============================================================================
//
// �p�[�e�B�N���̏��� [particle.cpp]
// Author : ���R���
//
//=============================================================================
#include "particle.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define PARTICLE_TEXTURE_0		"data\\TEXTURE\\game\\ParticleEffect\\Star.png"		//�ǂݍ��ރe�N�X�`���t�@�C��
#define PARTICLE_TEXTURE_1		"data\\TEXTURE\\game\\ParticleEffect\\.png"			//�ǂݍ��ރe�N�X�`���t�@�C��

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9	CParticle::m_apTexture[MAX_EFFECT_TEX] = {};

//=============================================================================
// �p�[�e�B�N���̐�������
//=============================================================================
CParticle *CParticle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, D3DXVECTOR2 size, int nLife, PARTICLE_TYPE type)
{
	CParticle *pParticle = NULL;

	if (pParticle == NULL)
	{
		// �p�[�e�B�N���N���X�̐���
		pParticle = new CParticle;

		if (pParticle != NULL)
		{
			pParticle->Init();							// ����������
			pParticle->m_move = move;					// �ړ��ʂ̐ݒ�
			pParticle->SetPosSize(pos, size);			// ���̐ݒ�
			pParticle->m_Size = size;
			pParticle->m_nLife = nLife;					// ���C�t�̐ݒ�
			pParticle->BindTexture(m_apTexture[type]);	// �e�N�X�`���̐ݒ�
			pParticle->SetColor(col);					// �F�̐ݒ�
		}
	}
	return pParticle;
}

//=============================================================================
// �p�[�e�B�N���N���X�̃R���X�g���N�^
//=============================================================================
CParticle::CParticle()
{
	// �l���N���A
	m_nLife = 0;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// �p�[�e�B�N���N���X�̃f�X�g���N�^
//=============================================================================
CParticle::~CParticle()
{
}

//=============================================================================
// �p�[�e�B�N���̏���������
//=============================================================================
HRESULT CParticle::Init(void)
{
	// �l���N���A
	m_nLife = 0;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 2D�I�u�W�F�N�g����������
	CBillBoord::Init();

	// �I�u�W�F�N�g�̎�ނ̐ݒ�
	SetObjType(CScene::OBJTYPE_PARTICLE);

	return S_OK;
}

//=============================================================================
// �p�[�e�B�N���̏I������
//=============================================================================
void CParticle::Uninit(void)
{
	// 2D�I�u�W�F�N�g�I������
	CBillBoord::Uninit();
}

//=============================================================================
// �p�[�e�B�N���̍X�V����
//=============================================================================
void CParticle::Update(void)
{
	// �ʒu���擾
	D3DXVECTOR3 pos;
	pos = CBillBoord::GetPosition();

	//// ���_�o�b�t�@���擾
	//LPDIRECT3DVERTEXBUFFER9 VtxBuff;
	//VtxBuff = CBillBoord::GetVtxBuff();

	//VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	//// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	//VtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	
	// ��莞�Ԍo��
	m_nLife--;

	m_Size.x -= 0.1f;
	m_Size.y -= 0.1f;

	if (m_Size.x <= 0 && m_Size.y <= 0)
	{
		m_Size.x = 0.0f;
		m_Size.y = 0.0f;
	}

	pos -= m_move;

	switch (m_ParticleType)
	{
		// �ʏ���
	case TYPE_NORMAL:
		break;
		// �������
	case TYPE_DOWN:
		// �d�͉��Z
		m_move.y += 0.5f;

		//CPlayer **pPlayer = NULL;
		//pPlayer = CGame::GetPlayer();

		//pos.x = pPlayer[m_nPlayer]->GetPos().x;
		//pos.z = pPlayer[m_nPlayer]->GetPos().z;

		break;
	}

	// ���_�o�b�t�@���A�����b�N
	//VtxBuff->Unlock();

	// �ʒu�̐ݒ�
	CBillBoord::SetPosition(pos);

	// ���̐ݒ�
	CBillBoord::SetPosSize(pos, m_Size);

	if (m_nLife <= 0)
	{
		// ����
		Uninit();
	}
}

//=============================================================================
// �p�[�e�B�N���̕`�揈��
//=============================================================================
void CParticle::Draw(void)
{
	// �����_���[���擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �[�x�o�b�t�@��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// 2D�I�u�W�F�N�g�`�揈��
	CBillBoord::Draw();

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �[�x�o�b�t�@�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

//===============================================================================
// �e�N�X�`���̓ǂݍ���
//===============================================================================
HRESULT CParticle::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	for (int nCntTex = 0; nCntTex < MAX_EFFECT_TEX; nCntTex++)
	{
		// �e�N�X�`���̐���
		m_apTexture[nCntTex] = NULL;
	}

	D3DXCreateTextureFromFile(pDevice, PARTICLE_TEXTURE_0, &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, PARTICLE_TEXTURE_1, &m_apTexture[1]);

	return S_OK;
}

//===============================================================================
// �e�N�X�`���̔j��
//===============================================================================
void CParticle::UnLoad(void)
{
	for (int nCntTex = 0; nCntTex < MAX_EFFECT_TEX; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}