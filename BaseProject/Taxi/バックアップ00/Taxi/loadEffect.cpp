//=============================================================================
//
// �G�t�F�N�g���� [Effect.cpp]
// Author : �������� Sato_Asumi
//
//=============================================================================
#include "loadEffect.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "particlebillboad.h"
#include "input.h"
#include "LoadText.h"
#include "game.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_TIME		(10)									//�C���^�[�o���^�C��
#define LIFE			(50)									//���C�t�̐ݒ�
#define FIRST_POS		(D3DXVECTOR3(0.0f, 10.0f, 0.0f))		//�����ʒu
#define FIRST_SIZE		(D3DXVECTOR3(20.0f, 20.0f, 0.0f))		//�����T�C�Y
#define BOUND_HIGHT		(4.0f)									//�����̐ݒ�
#define MAX_EXPLOTION	(2)										//�����G�t�F�N�g�ő吔
#define MAX_STAR		(13)									//���̃G�t�F�N�g�̍ő吔
#define MAX_CIRCLE		(10)									//�~�G�t�F�N�g�̍ő吔
#define EFFECT_NUM		(4)										//�G�t�F�N�g�̔ԍ�
#define EFFECT_NUM2		(3)										//�G�t�F�N�g�̔ԍ�
#define VECTOR_ZERO		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))			

//=============================================================================
// ��������
//=============================================================================
CLoadEffect * CLoadEffect::Create(int nNumEffect, D3DXVECTOR3 pos, int nPriority)
{
	CLoadEffect * pLoadEffect;
	pLoadEffect = new CLoadEffect;

	//����������
	if (pLoadEffect != NULL)
	{
		pLoadEffect->Init();
		pLoadEffect->SetNumEffect(nNumEffect);	//�G�t�F�N�g�i���o�[�̐ݒ�
		pLoadEffect->SetPos(pos);				//�ʒu�̐ݒ�
		pLoadEffect->SetPriority(nPriority);	//�D�揇�ʂ̐ݒ�
	}

	return pLoadEffect;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLoadEffect::CLoadEffect()
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLoadEffect::~CLoadEffect()
{
}

//=============================================================================
// ����������
//=============================================================================
void CLoadEffect::Init(void)
{
	CEffectTool::Init();

	//����������
	m_pos = VECTOR_ZERO;			//�ʒu
	m_size = VECTOR_ZERO;			//�T�C�Y
	m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//�F
	m_bRemake = false;								//�Đݒ肷�邩�ǂ���
}

//=============================================================================
// �I������
//=============================================================================
void CLoadEffect::Uninit(void)
{
	CEffectTool::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CLoadEffect::Update(void)
{
	CEffectTool::Update();

	//�p�[�e�B�N���̐�������
	for (int nCntParticle = 0; nCntParticle < MAX_EFFECT; nCntParticle++)
	{
		//�J�E���^�[�̉��Z
		m_pEffect[m_nNumEffect].EffectInfo[nCntParticle].nCountTime++;

		if ((m_pEffect[m_nNumEffect].EffectInfo[nCntParticle].bCreateEffect == true) &&
			(m_pEffect[m_nNumEffect].EffectInfo[nCntParticle].bLoop == true))				//�������邩�ǂ���
		{
			if (m_pEffect[m_nNumEffect].EffectInfo[nCntParticle].nCountTime > m_pEffect[m_nNumEffect].EffectInfo[nCntParticle].nInterbalTime)
			{
				m_pEffect[m_nNumEffect].EffectInfo[nCntParticle].nCountTime = 0;				//�J�E���^�[�̏�����

				if (m_bRemake == true)
				{
					CreateEffect(nCntParticle, m_nNumEffect, m_pos, m_size, m_color, m_nPriority);	//�G�t�F�N�g�̐���
				}
				else if ((m_bRemake == false) && (m_nNumEffect == EFFECT_NUM))
				{
					CreateEffect(nCntParticle, m_nNumEffect, m_pos, m_pEffect[m_nNumEffect].EffectInfo[nCntParticle].size, m_color, m_nPriority);	//�G�t�F�N�g�̐���
				}
				else
				{
					CreateEffect(nCntParticle,												//�G�t�F�N�g�̐���
						m_nNumEffect,
						m_pos,
						m_pEffect[m_nNumEffect].EffectInfo[nCntParticle].size,
						m_pEffect[m_nNumEffect].EffectInfo[nCntParticle].Color,
						m_nPriority);
				}
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CLoadEffect::Draw(void)
{
	CEffectTool::Draw();

	/* �O�Ղɐݒ肷��}�g���b�N�X���v�Z */
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX		  mtxRot, mtxTrans;			// �v�Z�p�}�g���b�N�X

												// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
}