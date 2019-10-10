//=============================================================================
//
// �G�t�F�N�g�̏��� [effect.cpp]
// Author : �L�n���u
//
//=============================================================================
#include "scene.h"
#include "effect.h"
#include "manager.h"
#include "renderer.h"
#include "scene3D.h"
#include "game.h"
#include "player.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	EFFECT_TEXTURE_NAME_1	"data\\TEXTURE\\effect000.jpg"			//�ǂݍ��ރe�N�X�`���t�@�C����
#define	EFFECT_TEXTURE_NAME_2	"data\\TEXTURE\\kemuri.jpg"				//�ǂݍ��ރe�N�X�`���t�@�C����
#define	EFFECT_TEXTURE_NAME_3	"data\\TEXTURE\\smoke.png"					//�ǂݍ��ރe�N�X�`���t�@�C����

#define TEXTURE_SIZE			(50)									//�e�N�X�`���T�C�Y

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
int CEffect::m_nCntEffect = 0;
//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
LPDIRECT3DTEXTURE9	CEffect::m_pTexture[MAX_BILLBOORD_TEX] = {};

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CEffect::CEffect() : CScene3D(EFFECT_PRIORTY, CScene::OBJTYPE_EFFECT)
{
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_EffectType = EFFECTTYPE_NONE;
	m_fRadius = 0.0f;
	m_nLife = 0;
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CEffect::~CEffect()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CEffect::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	CScene3D::Init();

	//����̓r���{�[�h���G�t�F�N�g���̔���
	CScene3D::SetTypeNumber(CScene3D::TYPE_EFFECT);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEffect::Uninit(void)
{
	CScene3D::Uninit();
}
//=============================================================================
// �X�V����
//=============================================================================
void CEffect::Update(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();

	CScene3D::Update();

	//�ʒu�̒l��e������炤
	D3DXVECTOR3 pos = CScene3D::GetPosition();
	//�ړ��ʂ̒l��e������炤
	D3DXVECTOR3 move = CScene3D::GetMove();

	//�J���[�̐錾�Ə�����
	D3DXCOLOR Color[4];

	//���_���̐ݒ�
	CScene3D::SetVtxEffect(m_fRadius);

	//4�F�̂����̂ǂꂩ
	int nNumber = rand() % 4 + 1;

	switch (m_EffectType)
	{
	case EFFECTTYPE_SPLASHES:	//�����Ԃ�
		if (nNumber == 1) { Color[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
		if (nNumber == 2) { Color[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
		if (nNumber == 3) { Color[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
		if (nNumber == 4) { Color[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }

		//�G�t�F�N�g�̃J���[
		CScene3D::SetColor(Color[0]);

		//�ړ��l���
		pos += move;

		//��������
		m_nLife--;

		if (m_nLife <= 0)
		{//���a��0�ȉ��̏ꍇ
			CScene3D::Uninit();
		}
		break;
	case EFFECTTYPE_ERUPTION:	//����
		if (nNumber == 1) { Color[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
		if (nNumber == 2) { Color[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
		if (nNumber == 3) { Color[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
		if (nNumber == 4) { Color[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }

		//�G�t�F�N�g�̃J���[
		CScene3D::SetColor(Color[0]);

		//�ړ��l���
		pos += move;

		//��������
		m_nLife--;

		//�o�E���h�̏���
		if (pos.y <= 0)
		{
			move.y = (m_posOld.y - pos.y) * 2;
		}
		if (m_nLife <= 0)
		{//���a��0�ȉ��̏ꍇ
			CScene3D::Uninit();
		}
		break;
	case EFFECTTYPE_SMOKE:		//���G�t�F�N�g
		if (nNumber == 1) { Color[0] = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f); }
		if (nNumber == 2) { Color[1] = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f); }
		if (nNumber == 3) { Color[2] = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f); }
		if (nNumber == 4) { Color[3] = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f); }

		//�G�t�F�N�g�̃J���[
		CScene3D::SetColor(Color[0]);

		//�ړ��l���
		pos.y += move.y;
		//��������
		m_nLife--;

		//���a����
		if (m_nLife > 0)
		{
			m_fRadius -= 0.05f;
		}
		else
		{
			m_fRadius -= 0.5f;
		}
		if (m_fRadius <= 0)
		{//���a��0�ȉ��̏ꍇ
			CScene3D::Uninit();
		}
		break;
	case EFFECTTYPE_FIRE:		//�΃G�t�F�N�g
		if (nNumber == 1) { Color[0] = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f); }
		if (nNumber == 2) { Color[1] = D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f); }
		if (nNumber == 3) { Color[2] = D3DXCOLOR(1.0f, 0.3f, 0.0f, 1.0f); }
		if (nNumber == 4) { Color[3] = D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f); }

		//�G�t�F�N�g�̃J���[
		CScene3D::SetColor(Color[0]);

		//�ړ��l���
		pos.y += 0.2f;
		//��������
		m_nLife--;

		//���a����
		if (m_nLife > 0)
		{
			m_fRadius -= 1.0f;
		}
		else
		{
			m_fRadius -= 1.0f;
		}

		if (m_fRadius <= 0)
		{//���a��0�ȉ��̏ꍇ
			CScene3D::Uninit();
		}
		break;
	case EFFECTTYPE_POWDER:		//�΂̕��G�t�F�N�g
		if (nNumber == 1) { Color[0] = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f); }
		if (nNumber == 2) { Color[1] = D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f); }
		if (nNumber == 3) { Color[2] = D3DXCOLOR(1.0f, 0.3f, 0.0f, 1.0f); }
		if (nNumber == 4) { Color[3] = D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f); }

		//�G�t�F�N�g�̃J���[
		CScene3D::SetColor(Color[0]);

		//�ړ��l���
		pos.y += 0.2f;
		//��������
		m_nLife--;

		//���a����
		if (m_nLife > 0)
		{
			m_fRadius -= 0.05f;
		}
		else
		{
			m_fRadius -= 0.05f;
		}
		if (m_fRadius <= 0)
		{//���a��0�ȉ��̏ꍇ
			CScene3D::Uninit();
		}
		break;
	case EFFECTTYPE_SNOW:		//��G�t�F�N�g
		if (nNumber == 1) { Color[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
		if (nNumber == 2) { Color[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
		if (nNumber == 3) { Color[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
		if (nNumber == 4) { Color[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }

		//�G�t�F�N�g�̃J���[
		CScene3D::SetColor(Color[0]);

		//move�ɏd��
		//move.y = -1.0f;
		//�ړ��l���
		pos += move;

		if (pos.y <= 0)
		{//�ʒu�̂�����0�ȉ��̏ꍇ
			CScene3D::Uninit();
		}
		break;
	case EFFECTTYPE_RAIN:		//�J�G�t�F�N�g
		if (nNumber == 1) { Color[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
		if (nNumber == 2) { Color[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
		if (nNumber == 3) { Color[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
		if (nNumber == 4) { Color[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }

		//�G�t�F�N�g�̃J���[
		CScene3D::SetColor(Color[0]);

		//move�ɏd��
		//move.y = -3.0f;
		//�ړ��l���
		pos += move;

		if (pos.y <= 0)
		{//���a��0�ȉ��̏ꍇ
			CScene3D::Uninit();
		}
		break;
	}

	//�ʒu���E���C�t�E���a��0����Ȃ��ꍇ�̂݃Z�b�g����
	if (pos.y >= 0.1 || m_fRadius >= 0.1)
	{//���a��0�ȉ��̏ꍇ
		//�G�t�F�N�g�̈ʒu
		CScene3D::SetPosition(pos);
		//�G�t�F�N�g�̈ړ���
		//CScene3D::SetMove(move);
	}

}
//=============================================================================
// �`�揈��
//=============================================================================
void CEffect::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�ʒu�̒l��e������炤
	D3DXVECTOR3 EffectPos = CScene3D::GetPosition();
	//�v���C���[�̈ʒu���
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();
	//�Q�[���̏��
	CManager::MODE pMode = CManager::GetMode();

	//�t�H�O���g�p���Ȃ�
	//pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

	//�A���t�@�e�X�g
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//�A���t�@�e�X�g����
	//�f�t�H���g�̏ꍇFALSE�ɂȂ��Ă��邩��TRUE�ɂ���
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	if (m_EffectType == CEffect::EFFECTTYPE_SMOKE)
	{
		//���Z�����̐ݒ�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	else
	{
		//���u�����f�B���O�����Z�����ɐݒ�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	//�߂��̃G�t�F�N�g�ȊO������
	if (EffectPos.x + 1000.0f >= PlayerPos.x && EffectPos.x - 1000.0f <= PlayerPos.x && pMode == CManager::MODE_GAME)
	{
		CScene3D::Draw();
	}
	else if(pMode != CManager::MODE_GAME)
	{
		CScene3D::Draw();
	}

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�f�t�H���g�̏ꍇFALSE�ɂȂ��Ă��邩��TRUE�ɂ���
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	//�t�H�O���g�p����
	//pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
}
//===============================================================================
//�@�N���G�C�g
//===============================================================================
CEffect * CEffect::Create(EFFECTTYPE Type,D3DXVECTOR3 pos, int nRadius, int nLife,int nTexType)
{
	CEffect *pEffect = NULL;

	//NULL�`�F�b�N
	if (pEffect == NULL)
	{//�������̓��I�m��

		pEffect = new CEffect;

		if (pEffect != NULL)
		{
			//�I�u�W�F�N�g�N���X�̐���
			pEffect->Init();
			//�G�t�F�N�g�̎��
			pEffect->m_EffectType = Type;
			//���a�̐ݒ�
			pEffect->m_fRadius = (float)nRadius;
			//���C�t�̐ݒ�
			pEffect->m_nLife = nLife;
			//�e�N�X�`���̊��蓖��
			pEffect->BindTexture(m_pTexture[nTexType]);
			//�ʒu�̊��蓖��
			pEffect->SetPosition(pos);
			//�p�[�e�B�N���̐ݒ�
			pEffect->SetParticleEffect(Type, nRadius);
		}
	}

	return pEffect;
}
//=========================================================================================================================
//�p�[�e�B�N���G�t�F�N�g�ݒ菈��
//=========================================================================================================================
void CEffect::SetParticleEffect(EFFECTTYPE Type, int nRadius)
{
	//�^�C�v�̐ݒ�
	m_EffectType = Type;

	//�ʒu�̒l��e������炤
	//D3DXVECTOR3 pos = CScene3D::GetPosition();
	//�ړ��ʂ̒l��e������炤
	D3DXVECTOR3 move = CScene3D::GetMove();

	if (m_EffectType == EFFECTTYPE_ERUPTION || m_EffectType == EFFECTTYPE_SMOKE || m_EffectType == EFFECTTYPE_FIRE || m_EffectType == EFFECTTYPE_POWDER)
	{
		//������ɂł�G�t�F�N�g�̊p�x
		float fLap = 1.0f / (10 / 2);

		//�ړ������_����
		move.x = sinf((-D3DX_PI * (1.0f * (fLap * (m_nCntEffect + 1)))))* 1.5f;
		move.y = cosf((rand() % 10) / 10.0f) * ((rand() % 4) + 1);
		move.z = cosf((-D3DX_PI * (1.0f * (fLap * (m_nCntEffect + 1)))))* 1.5f;
	}
	else if (m_EffectType == EFFECTTYPE_SPLASHES)
	{
		//������ɂł�G�t�F�N�g�̊p�x
		float fLap = 1.0f / (15 / 2);

		//�ړ������_����
		move.x = sinf((-D3DX_PI * (1.0f * (fLap * (m_nCntEffect + 1)))))* 2.0f;
		//move.y = cosf((rand() % 1) / 1.0f) * ((rand() % 4) + 1);
		move.y += 1.0f;
		move.z = cosf((-D3DX_PI * (1.0f * (fLap * (m_nCntEffect + 1)))))* 2.0f;
	}

	m_nCntEffect = (1 + m_nCntEffect)% 100;

	//�G�t�F�N�g�̈ړ���
	CScene3D::SetMove(move);
}
//===============================================================================
// �e�N�X�`���̓ǂݍ���
//===============================================================================
HRESULT CEffect::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, EFFECT_TEXTURE_NAME_1, &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, EFFECT_TEXTURE_NAME_2, &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, EFFECT_TEXTURE_NAME_3, &m_pTexture[2]);

	return S_OK;
}
//===============================================================================
// �e�N�X�`���̔j��
//===============================================================================
void CEffect::UnLoad(void)
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
