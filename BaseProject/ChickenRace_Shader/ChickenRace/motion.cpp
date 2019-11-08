//=============================================================================
//
// ���[�V�����̏��� [motion.cpp]
// Author : �������� Sato_Asumi
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "motion.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"
#include "camerabace.h"
#include "scene.h"
#include "model.h"
#include "LoadText.h"
#include "sound.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define NUTORAL_MOTION	(0)		//�j���[�g�������[�V����
#define KICK_MOTION		(5)		//�L�b�N���[�V�����i���o�[
#define KICK_KEY		(5)		//�ΏۃL�[�ԍ�

//=============================================================================
// ��������
//=============================================================================
CMotion * CMotion::Create(void)
{
	//�C���X�^���X�̐���
	CMotion * pMotion;
	pMotion = new CMotion;

	//����������
	pMotion->Init();

	return pMotion;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMotion::CMotion()
{
	m_pModel = NULL;								//���f�����
	m_nMaxModel = 0;								//���f���̍ő吔
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�ړ�
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//����
	m_nMaxMotion = 0;								//���[�V�����̍ő吔
	m_pMotionInfo = NULL;							//���[�V�������
	m_nNumMotion = 0;								//�j���[�g�������[�V����
	m_nKey = 0;										//���݂̃L�[
	m_nCountMotion = 0;								//���[�V�����J�E���^�[
	m_nNextNumMotion = -1;							//���̃��[�V�����ԍ�
	m_bBlend = false;								//���̃��[�V�����̏�Ԑݒ�
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMotion::~CMotion(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CMotion::Init(void)
{
	//�ϐ��̏�����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);;		//�ړ�
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);;			//����

	//�j���[�g�������[�V�����̍ŏ��̌�����ݒ肷��
	for (int nCntModel = 0; nCntModel < m_nMaxModel; nCntModel++)
	{
		m_pModel[nCntModel]->SetRot(D3DXVECTOR3(m_pMotionInfo[0].m_pKeyInfo[0].aKey[nCntModel].fRotX, m_pMotionInfo[0].m_pKeyInfo[0].aKey[nCntModel].fRotY, m_pMotionInfo[0].m_pKeyInfo[0].aKey[nCntModel].fRotZ));
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CMotion::Uninit(void)
{
	//���f���f�[�^�̔j��
	if (m_pModel != NULL)
	{
		m_pModel = NULL;
	}

	//���[�V�����f�[�^�̔j��
	if (m_pMotionInfo != NULL)
	{
		m_pMotionInfo = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
int CMotion::Update(void)
{
	//�ϐ��錾
	KEY * pKey, *pNextKey;		//���݂̃L�[�Ǝ��̃L�[
	D3DXVECTOR3 ModelRot;		//���f���̌���

	//�J�E���^�[��i�߂�
	m_nCountMotion++;

	//�L�b�N���[�V�����Ȃ特���o��
	if ((m_nNumMotion == KICK_MOTION) && 
		(m_nKey == KICK_KEY) && 
		m_nCountMotion == 1)
	{
		CSound * pSound = CManager::GetSound();
		//pSound->PlaySoundA(CSound::SOUND_LABEL_SE_KICK);
	}

	//�t���[������
	if (m_pMotionInfo[m_nNumMotion].m_pKeyInfo[m_nKey].nFrame < m_nCountMotion)
	{
		m_nCountMotion = 0;	//�J�E���^�[�����Z�b�g����

		if (m_bBlend == true)
		{//���[�V�����u�����h���L����������
			m_nKey = 0;										//�L�[������������
			m_nNumMotion = m_nNextNumMotion;				//���݂̃��[�V���������̃��[�V�����ɓ���ւ���
			m_bBlend = false;								//���[�V�����u�����h�𖳌��ɂ���
		}
		else
		{//���[�V�����u�����h��������������
			m_nKey = (m_nKey + 1) % m_pMotionInfo[m_nNumMotion].nNumKey;			//�L�[��i�߂�
		}
	}

	//���[�V�����̃L�[�ݒ�E���������߂�
	for (int nCntModel = 0; nCntModel < m_nMaxModel; nCntModel++)
	{
		pKey = &m_pMotionInfo[m_nNumMotion].m_pKeyInfo[m_nKey].aKey[nCntModel];		//���݂̃L�[��ݒ肷��
		pNextKey = SetKey(nCntModel);	//���̃L�[��ݒ肷��
		SetMotionDifference(pNextKey, pKey, nCntModel);
	}

	return m_nNumMotion;
}

//=============================================================================
// �`�揈��
//=============================================================================
void CMotion::Draw(void){}

//=============================================================================
// �L�[�ݒ�
//=============================================================================
CMotion::KEY * CMotion::SetKey(int nCntModel)
{
	//�ϐ��錾
	KEY * pNextKey;

	if (m_nKey != m_pMotionInfo[m_nNumMotion].nNumKey - 1)
	{//�Ō�̃L�[����Ȃ�������

		if (m_bBlend == false)
		{//���݂̃��[�V�����̎��̃L�[��ݒ肷��
			pNextKey = &m_pMotionInfo[m_nNumMotion].m_pKeyInfo[m_nKey + 1].aKey[nCntModel];
		}
		else
		{//���̃��[�V�����̃L�[�ɐݒ肷��
			pNextKey = &m_pMotionInfo[m_nNextNumMotion].m_pKeyInfo[0].aKey[nCntModel];	//�ŏ��̃L�[�ɐݒ肷��
		}
	}
	else
	{//�Ō�̃L�[��������
		if (m_pMotionInfo[m_nNumMotion].m_bLoop == true)
		{
			pNextKey = &m_pMotionInfo[m_nNumMotion].m_pKeyInfo[0].aKey[nCntModel];	//�ŏ��̃L�[�ɐݒ肷��
		}
		else
		{
			m_nNextNumMotion = NUTORAL_MOTION;	//�j���[�g�������[�V�����ɂ���
			pNextKey = &m_pMotionInfo[m_nNextNumMotion].m_pKeyInfo[0].aKey[nCntModel];	//�ŏ��̃L�[�ɐݒ肷��
			m_bBlend = true;
		}
	}

	return pNextKey;
}

//=============================================================================
// ���[�V�����̍��������߂�
//=============================================================================
void CMotion::SetMotionDifference(KEY * pNextKey, KEY * pKey, int nCntModel)
{
	//�ϐ��錾
	float fRateMotion;			//���Βl
	KEY * pDiffKey = new KEY;	//����

	fRateMotion = (float)m_nCountMotion / (float)m_pMotionInfo[m_nNumMotion].m_pKeyInfo[m_nKey].nFrame;	//���Βl�����߂�

	pDiffKey->fPosX = pNextKey->fPosX - pKey->fPosX;		//���݂̃L�[�Ǝ��̃L�[�̍��������߂�
	pDiffKey->fPosY = pNextKey->fPosY - pKey->fPosY;
	pDiffKey->fPosZ = pNextKey->fPosZ - pKey->fPosZ;
	pDiffKey->fRotX = pNextKey->fRotX - pKey->fRotX;
	pDiffKey->fRotY = pNextKey->fRotY - pKey->fRotY;
	pDiffKey->fRotZ = pNextKey->fRotZ - pKey->fRotZ;

	//�ω��ʂ����߂�
	D3DXVECTOR3 ModelPos;
	D3DXVECTOR3 ModelRot;
	ModelPos.x = pKey->fPosX + (pDiffKey->fPosX * fRateMotion);
	ModelPos.y = pKey->fPosY + (pDiffKey->fPosY * fRateMotion);
	ModelPos.z = pKey->fPosZ + (pDiffKey->fPosZ * fRateMotion);
	ModelRot.x = pKey->fRotX + (pDiffKey->fRotX * fRateMotion);
	ModelRot.y = pKey->fRotY + (pDiffKey->fRotY * fRateMotion);
	ModelRot.z = pKey->fRotZ + (pDiffKey->fRotZ * fRateMotion);

	//�v�Z���ʂ����f���ɓn��
	m_pModel[nCntModel]->SetRot(ModelRot);
	if (!m_bBlend) { m_pModel[nCntModel]->AddPos(ModelPos); }

	//���_�̈ʒu��ύX����
	if (nCntModel == 0)
	{
		m_move.x = ModelPos.x;
		m_move.y = ModelPos.y;
		m_move.z = ModelPos.z;
	}

	//�L�[����j������
	delete pDiffKey;
	pDiffKey = NULL;
}

//=============================================================================
// ���[�V�����̐ݒ�
//=============================================================================
void CMotion::SetNumMotion(int NumMotion)
{
	m_nNumMotion = NumMotion;
	m_nKey = 0;
}