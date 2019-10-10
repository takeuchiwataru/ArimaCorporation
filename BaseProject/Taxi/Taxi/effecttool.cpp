//=============================================================================
//
// �G�t�F�N�g�c�[������ [Effecttool.cpp]
// Author : �������� Sato_Asumi
//
//=============================================================================
#include "effecttool.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "particlebillboad.h"
#include "input.h"
#include "LoadText.h"
#include "game.h"
#include "tutorial.h"
//=============================================================================
// �}�N����`
//=============================================================================
#define FIRST_POS		(D3DXVECTOR3(0.0f, 10.0f, 0.0f))		//�����ʒu

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CEffectTool::Effect * CEffectTool::m_pEffect = NULL;
int CEffectTool::m_nMaxEffect = 0;

//=============================================================================
// ��������
//=============================================================================
CEffectTool * CEffectTool::Create(void)
{//�C���X�^���X�̐���

	CEffectTool * pEffect;
	pEffect = new CEffectTool;

	//����������
	if (pEffect != NULL)
	{
		pEffect->Init();
	}

	return pEffect;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEffectTool::CEffectTool(){}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEffectTool::~CEffectTool(){}

//=============================================================================
// �G�t�F�N�g�̓ǂݍ���
//=============================================================================
void CEffectTool::LoadEffect(void)
{
	//�G�t�F�N�g���̐����E������
	if (m_pEffect == NULL)
	{
		//�ǂݍ��񂾏����擾����
		CLoadTextEffect * pLoadTextEffect = {};
		if (CManager::GetMode() == CManager::MODE_GAME)
		{
			pLoadTextEffect = CGame::GetLoadEffect();
		}
		else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
		{
			pLoadTextEffect = CTutorial::GetLoadEffect();
		}
		m_nMaxEffect = pLoadTextEffect->GetMaxEffect();							//�G�t�F�N�g�̍ő吔���擾����
		Effect * pEffect = pLoadTextEffect->GetEffect();						//�G�t�F�N�g���|�C���^�̎擾

																				//�������̓��I�m��
		m_pEffect = new Effect[m_nMaxEffect];

		if (m_pEffect != NULL)
		{
			for (int nCntEffect = 0; nCntEffect < m_nMaxEffect; nCntEffect++)
			{
				//�G�t�F�N�g���̏�����
				if (m_pEffect[nCntEffect].EffectInfo != NULL)
				{
					m_pEffect[nCntEffect].EffectInfo = NULL;
				}

				//�G�t�F�N�g���̃������𓮓I�m��
				if (m_pEffect[nCntEffect].EffectInfo == NULL)
				{
					m_pEffect[nCntEffect].EffectInfo = new Effect_Info[MAX_EFFECT];
				}

				//�ǂݍ��񂾏��̎擾
				if (m_pEffect[nCntEffect].EffectInfo != NULL)
				{
					for (int nCntParticle = 0; nCntParticle < MAX_EFFECT; nCntParticle++)
					{
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].bCreateEffect = pEffect[nCntEffect].EffectInfo[nCntParticle].bCreateEffect;		//�G�t�F�N�g���g�p�Ă��邩�ǂ���
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].bLoop = pEffect[nCntEffect].EffectInfo[nCntParticle].bLoop;						//���[�v���邩�ǂ���
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].pos = FIRST_POS;																	//�ʒu
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].size = pEffect[nCntEffect].EffectInfo[nCntParticle].size;						//�T�C�Y
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].nTexNum = pEffect[nCntEffect].EffectInfo[nCntParticle].nTexNum;					//�e�N�X�`���ԍ�
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].DrawType = pEffect[nCntEffect].EffectInfo[nCntParticle].DrawType;				//�`����
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].Type = pEffect[nCntEffect].EffectInfo[nCntParticle].Type;						//�X�V���
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].Color = pEffect[nCntEffect].EffectInfo[nCntParticle].Color;						//�F
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].bZbuffur = pEffect[nCntEffect].EffectInfo[nCntParticle].bZbuffur;				//Z�o�b�t�@�̏��
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].nLife = pEffect[nCntEffect].EffectInfo[nCntParticle].nLife;						//�\������
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].fChangeCol_a = pEffect[nCntEffect].EffectInfo[nCntParticle].fChangeCol_a;		//�����x�̕ω���
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].bUseGravity = pEffect[nCntEffect].EffectInfo[nCntParticle].bUseGravity;			//�d�͂��g�p���邩�ǂ���
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].nInterbalTime = pEffect[nCntEffect].EffectInfo[nCntParticle].nInterbalTime;		//�������Ԃ̊Ԋu
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].nNumCreate = pEffect[nCntEffect].EffectInfo[nCntParticle].nNumCreate;			//��x�̐�����
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].fSpeed = pEffect[nCntEffect].EffectInfo[nCntParticle].fSpeed;					//�X�s�[�h
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].nMinHight = pEffect[nCntEffect].EffectInfo[nCntParticle].nMinHight;				//�����̍ŏ��l
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].nMaxHight = pEffect[nCntEffect].EffectInfo[nCntParticle].nMaxHight;				//�����̍ő�l
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].fGravity = pEffect[nCntEffect].EffectInfo[nCntParticle].fGravity;				//�d�͂̏d��
						m_pEffect[nCntEffect].EffectInfo[nCntParticle].nCountTime = 0;																	//���Ԃ̃J�E���^�[
					}
				}
			}
		}
	}
}

//=============================================================================
// �G�t�F�N�g�̔j��
//=============================================================================
void CEffectTool::UnloadEffect(void)
{
	//�G�t�F�N�g���̊J��
	for (int nCntEffect = 0; nCntEffect < m_nMaxEffect; nCntEffect++)
	{
		if (m_pEffect[nCntEffect].EffectInfo != NULL)
		{
			delete[] m_pEffect[nCntEffect].EffectInfo;
			m_pEffect[nCntEffect].EffectInfo = NULL;
		}
	}
	if (m_pEffect != NULL)
	{
		delete[] m_pEffect;
		m_pEffect = NULL;
	}
}

//=============================================================================
// ����������
//=============================================================================
void CEffectTool::Init(void)
{
	m_bBuffZ = true;								//Z�o�b�t�@�̗L�����
}

//=============================================================================
// �I������
//=============================================================================
void CEffectTool::Uninit(void){}

//=============================================================================
// �X�V����
//=============================================================================
void CEffectTool::Update(void){}

//=============================================================================
// �`�揈��
//=============================================================================
void CEffectTool::Draw(void){}

//=============================================================================
// �G�t�F�N�g�̐���
//=============================================================================
void CEffectTool::CreateEffect(int nCntParticle, int nNumEffect, D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nPriority)
{
	//�ϐ��錾
	bool bBuffZ;

	for (int nCnt = 0; nCnt < m_pEffect[nNumEffect].EffectInfo[nCntParticle].nNumCreate; nCnt++)
	{
		//Z�o�b�t�@�̗L���ݒ�
		if (m_bBuffZ != m_pEffect[nNumEffect].EffectInfo[nCntParticle].bZbuffur)
		{
			bBuffZ = m_bBuffZ;
		}
		else
		{
			bBuffZ = m_pEffect[nNumEffect].EffectInfo[nCntParticle].bZbuffur;
		}

		//�p�[�e�B�N���̐���
		CParticleBillboad * pParticle = CParticleBillboad::Create(pos,														//�ʒu���
												  size,														//�T�C�Y
												  m_pEffect[nNumEffect].EffectInfo[nCntParticle].nTexNum,	//�e�N�X�`���ԍ�
												  m_pEffect[nNumEffect].EffectInfo[nCntParticle].DrawType,	//�`����
												  m_pEffect[nNumEffect].EffectInfo[nCntParticle].Type,		//�����^�C�v
												  col,														//�F
												  m_pEffect[nNumEffect].EffectInfo[nCntParticle].nLife,		//�\������
												  m_pEffect[nNumEffect].EffectInfo[nCntParticle].fSpeed,	//�X�s�[�h
												  m_pEffect[nNumEffect].EffectInfo[nCntParticle].nMinHight,	//�����̍ŏ��l
												  m_pEffect[nNumEffect].EffectInfo[nCntParticle].nMaxHight,	//�����̍ő�l
												  bBuffZ,													//Z�o�b�t�@�̏��
												  nPriority);												//�D�揇��

		if (pParticle != NULL)
		{
			if (m_pEffect[nNumEffect].EffectInfo->Type == CParticleBillboad::TYPE_SPEW || m_pEffect[nNumEffect].EffectInfo->Type == CParticleBillboad::TYPE_CICLE)
			{
				pParticle->SetChangeCol_a(m_pEffect[nNumEffect].EffectInfo[nCntParticle].fChangeCol_a);				//�����x�̕ω��ʂ̐ݒ�
				pParticle->SetUseGravity(m_pEffect[nNumEffect].EffectInfo[nCntParticle].bUseGravity);				//�d�͂̏�Ԃ�ݒ肷��
				pParticle->SetGravity(m_pEffect[nNumEffect].EffectInfo[nCntParticle].fGravity);						//�d�͂̐ݒ�
			}
		}
	}
}