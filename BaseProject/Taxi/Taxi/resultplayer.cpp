//=============================================================================
//
// ���U���g�v���C���[�̏��� [resultplayer.cpp]
// Author : shun yokomichi
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "resultplayer.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"
#include "camerabace.h"
#include "scene.h"
#include "model.h"
#include "motion.h"
#include "loadText.h"
#include "tire.h"
#include "wall.h"
#include "incline.h"
#include "taillamp.h"
#include "sound.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define VECTOR_ZERO		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))				//�x�N�g���̏�����
#define END_POS			(D3DXVECTOR3(530.0f, 0.0f, -120.0f))		// �~�܂�ʒu
#define FAILE_NAME		("data\\TEXT\\Player\\PlayerState.txt")		//�ǂݍ��ރt�@�C����
#define FILE_TEXTURE	("data\\TEXTURE\\modeltex\\carup.jpg")		//�e�N�X�`���̓ǂݍ���
#define LEFT_FRONT_POS	(D3DXVECTOR3(-26.0f, 3.0f, 50.0f))			//���O�^�C���ʒu
#define LEFT_BACK_POS	(D3DXVECTOR3(-26.0f, 3.0f, -50.0f))			//�����^�C���ʒu
#define RIGHT_FRONT_POS	(D3DXVECTOR3(26.0f, 3.0f, 50.0f))			//�E�O�^�C���ʒu
#define RIGHT_BACK_POS	(D3DXVECTOR3(26.0f, 3.0f, -50.0f))			//�E���^�C���ʒu
#define GRAVITY_FRONT	(0.5f)										//�O���̏d��
#define GRAVITY_BACK	(0.2f)										//����̏d��

//�^�C���̔ԍ�
#define LEFT_FRONT		(0)											//���O�^�C��
#define LEFT_BACK		(1)											//�����^�C��
#define RIGHT_FRONT		(2)											//�E�O�^�C��
#define RIGHT_BACK		(3)											//�E���^�C��

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CModel * CResultPlayer::m_pModel = NULL;	//���f���̃p�[�c�|�C���^
int	CResultPlayer::m_nMaxModel = 0;
int CResultPlayer::m_nMaxParts = 0;
int CResultPlayer::m_nMaxMotion = 0;
D3DXVECTOR3 CResultPlayer::m_pos = VECTOR_ZERO;
CMotion::MOTION_INFO * CResultPlayer::m_pMotionInfo = NULL;
LPDIRECT3DTEXTURE9 CResultPlayer::m_pTexture = NULL;

//=============================================================================
// ��������
//=============================================================================
CResultPlayer * CResultPlayer::Create(const D3DXVECTOR3 pos)
{
	//�C���X�^���X�̐���
	CResultPlayer * pPlayer;
	pPlayer = new CResultPlayer;

	//����������
	pPlayer->Init();

	//�ݒ菈��
	pPlayer->Set(pos, VECTOR_ZERO);
	return pPlayer;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CResultPlayer::CResultPlayer() : CScene(3, OBJTYPE_PLAYER) {}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CResultPlayer::~CResultPlayer() {}

//=============================================================================
// X�t�@�C���̓ǂݍ���
//=============================================================================
void CResultPlayer::LoadModel(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice,
		FILE_TEXTURE,
		&m_pTexture);

	//���[�V�������̎擾
	CLoadTextMotion * pLoadTextMotion = CResult::GetPlayerMotion();

	//���f�������擾
	CLoadTextMotion::MODEL_INFO ModelInfo = pLoadTextMotion->GetModelInfo();
	m_nMaxModel = ModelInfo.nMaxModel;	//���f���̍ő吔�̎擾
	m_nMaxParts = ModelInfo.nMaxParts;	//���f���̃p�[�c�ő吔�̎擾

	//���f���̐���
	for (int nCntParts = 0; nCntParts < ModelInfo.nMaxParts; nCntParts++)
	{
		m_pModel = CModel::Create(ModelInfo.pOfSetPos[nCntParts], &ModelInfo.paFileName[nCntParts][0]);

		//�e�N�X�`���̊�����
		if (m_pModel != NULL) { m_pModel->BindTexture(m_pTexture); }
	}

	//���f���̐e�ݒ�
	for (int nCntParts = 0; nCntParts < ModelInfo.nMaxParts; nCntParts++)
	{
		if (nCntParts == 0)
		{
			m_pModel->SetParent(NULL);
			break;
		}
	}
}

//=============================================================================
// X�t�@�C���̔j��
//=============================================================================
void CResultPlayer::UnloadModel(void)
{
	if (m_pModel != NULL)
	{
		delete m_pModel;
		m_pModel = NULL;
	}

	//�e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CResultPlayer::Init(void)
{
	//�ϐ��̏�����
	m_pos = VECTOR_ZERO;					//���S���W
	m_OldPos = VECTOR_ZERO;					//�O��̍��W
	m_move = VECTOR_ZERO;					//�ړ�
	m_rot = VECTOR_ZERO;					//����
	m_vtxMaxModel = VECTOR_ZERO;			//���f���̒��_�ő�l
	m_vtxMinModel = VECTOR_ZERO;			//���f���̒��_�ŏ��l
	m_pMotion = NULL;						//���[�V�����|�C���^
	m_pText = NULL;							//�v���C���[�̃e�L�X�g�|�C���^
	m_state = STATE_DRIVE;					//��Ԑݒ�
	m_StateSpeed = STATE_SPEED_STOP;		//�X�s�[�h�̏�Ԑݒ�
	m_PlayerInfo.nCountTime = 0;			//�J�E���^�[
	m_PlayerInfo.fAccel = 3.5f;				//�����l
	m_PlayerInfo.fBraks = 0.1f;				//�����l
	m_PlayerInfo.fAddRot = 0.02f;			//�������]�l
	m_PlayerInfo.fDistance = 0.0f;			//����
	m_PlayerInfo.fWindy = 0.05f;			//����
	m_PlayerInfo.FirstPos = VECTOR_ZERO;	//�����ʒu

	//�^�C���̃|�C���^��������
	for (int nCntTire = 0; nCntTire < MAX_TIRE; nCntTire++)
	{
		m_pTire[nCntTire] = NULL;
	}

	CreateTire();			//�^�C���̐���

	for (int nCntTailLamp = 0; nCntTailLamp < 2; nCntTailLamp++)
	{// �e�[�������v�̐���
		m_apTailLamp[nCntTailLamp] = CTailLamp::Create(D3DXVECTOR3(22.0f - (nCntTailLamp * 44.0f), 35.0f, -85.0f), D3DXVECTOR2(10.0f, 10.0f), &m_mtxWorld);
	}

	//���f���̍ŏ��l�E�ő�l�̎擾
	m_vtxMaxModel = m_pModel->GetVtxMax();
	m_vtxMinModel = m_pModel->GetVtxMin();
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CResultPlayer::Uninit(void)
{
	//�^�C�����f���̔j��
	for (int nCntTire = 0; nCntTire < MAX_TIRE; nCntTire++)
	{
		if (m_pTire[nCntTire] != NULL)
		{
			m_pTire[nCntTire]->Uninit();
			delete m_pTire[nCntTire];
			m_pTire[nCntTire] = NULL;
		}
	}

	//�e�L�X�g�̔j��
	if (m_pText != NULL)
	{
		m_pText->Uninit();
		delete m_pText;
		m_pText = NULL;
	}

	//���S�t���O�𗧂Ă�
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CResultPlayer::Update(void)
{
	//�ϐ��錾
	D3DXVECTOR3 TirePos[MAX_TIRE];

	//�^�C���̍X�V����
	for (int nCntTire = 0; nCntTire < MAX_TIRE; nCntTire++)
	{
		if (m_pTire[nCntTire] == NULL) { continue; }

		if ((nCntTire % 2) == 0)
		{
			m_pTire[nCntTire]->Update(nCntTire, GRAVITY_FRONT);
		}
		else
		{
			m_pTire[nCntTire]->Update(nCntTire, GRAVITY_BACK);
		}

		if (m_StateSpeed == STATE_SPEED_STOP)
		{
			m_pTire[nCntTire]->SetRot(VECTOR_ZERO);
		}
		else
		{
			m_pTire[nCntTire]->SetRot(D3DXVECTOR3(m_pTire[nCntTire]->GetRot().x, VECTOR_ZERO.y, VECTOR_ZERO.z));
		}

		TirePos[nCntTire] = m_pTire[nCntTire]->GetWorldPos();
	}

	DirectingCount();
	UpdateMove();			// �ړ�����
}

//=============================================================================
// �`�揈��
//=============================================================================
void CResultPlayer::Draw(void)
{
	//�����_�����O�N���X���擾
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX		  mtxRot, mtxTrans;			// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���f���̕`��
	if (m_pModel != NULL)
	{
		//�`�悷��
		m_pModel->Draw(1.0f);
	}

	//�^�C���̕`��
	for (int nCntTire = 0; nCntTire < MAX_TIRE; nCntTire++)
	{
		if (m_pTire[nCntTire] != NULL) { m_pTire[nCntTire]->Draw(1.0); }
	}
}

//=============================================================================
// ���U���g���o���̃J�E���g
//=============================================================================
void CResultPlayer::DirectingCount(void)
{
	if (m_pos.z <= END_POS.z - 100.0f)
	{
		SetStateSpeed(STATE_SPEED_ACCEL);
		m_move.z += cosf(atan2f(END_POS.x - m_pos.x, END_POS.z - m_pos.z)) * 0.8f;
	}
	else
	{
		m_move.z -= cosf(atan2f(END_POS.x - m_pos.x, END_POS.z - m_pos.z)) * 0.8f;
		if (m_move.z <= 0.0f) { m_move.z = 0.0f; SetStateSpeed(STATE_SPEED_STOP); }
	}
}
//=============================================================================
// �ړ�����
//=============================================================================
void CResultPlayer::UpdateMove(void)
{
	m_OldPos = m_pos;	//�O��̈ʒu��ۑ�����

	RemakeAngle(&m_rot.y);

	// �d��
	m_move.y -= cosf(0) * 0.4f;

	//���S���W�X�V
	m_pos.x += m_move.x;
	//m_pos.y += m_move.y;
	m_pos.z += m_move.z;

	//����
	m_PlayerInfo.fAccel += (0.0f - m_PlayerInfo.fAccel) * m_PlayerInfo.fWindy;
	m_move.x += (0.0f - m_move.x) * m_PlayerInfo.fWindy;
	m_move.z += (0.0f - m_move.z) * m_PlayerInfo.fWindy;

	//��~����
	D3DXVECTOR3 fDiffuse = m_pos - m_OldPos;

	if (fDiffuse.x < 0.10f && fDiffuse.x > -0.10f)
	{
		if (fDiffuse.z < 0.10f && fDiffuse.z > -0.10f)
		{
			SetStateSpeed(STATE_SPEED_STOP);
		}
	}
}

//=============================================================================
// ��Ԃ̐ݒ�
//=============================================================================
void CResultPlayer::SetState(CResultPlayer::STATE state)
{
	//�l�̔��]
	if (m_state != state)
	{
		m_PlayerInfo.fAccel *= -0.5f;
		m_PlayerInfo.fBraks *= -1;
		m_fMaxSpeed *= -1;

		CSound *pSound = CManager::GetSound();

		if (STATE_REVERSE == state)
		{// �o�b�N��
			pSound->StopSound(CSound::SOUND_LABEL_SE_ACCEL);
			pSound->PlaySoundA(CSound::SOUND_LABEL_SE_BACK);

			if (STATE_SPEED_STOP != m_StateSpeed)
			{// �e�[�������v���o�b�N�p�ɓ_��
				SetTailLampType(CTailLamp::TYPE_BACK);
			}
		}
		else if (STATE_DRIVE == state)
		{
			pSound->StopSound(CSound::SOUND_LABEL_SE_BACK);
			pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ACCEL);

			//�e�[�������v������
			SetTailLampType(CTailLamp::TYPE_NONE);
		}
	}

	//��Ԃ̐ݒ�
	m_state = state;
}

//=============================================================================
// �X�s�[�h�̏�Ԑݒ�
//=============================================================================
void CResultPlayer::SetStateSpeed(CResultPlayer::STATE_SPEED state)
{
	//���l�̐ݒ�
	if (m_StateSpeed != state)
	{
		m_PlayerInfo.nCountTime = 0;

		CSound *pSound = CManager::GetSound();

		if (STATE_SPEED_ACCEL == state && STATE_DRIVE == m_state)
		{// �A�N�Z����
			pSound->PlaySound(CSound::SOUND_LABEL_SE_ACCEL);
		}
		else if (STATE_SPEED_BRAKS == state)
		{// �u���[�L��
			pSound->PlaySound(CSound::SOUND_LABEL_SE_BRAKING);
			pSound->StopSound(CSound::SOUND_LABEL_SE_ACCEL);
		}
	}

	//��Ԃ̐ݒ�
	m_StateSpeed = state;
}

//=============================================================================
// �ݒ菈��
//=============================================================================
void CResultPlayer::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	m_pos = pos;
}

//=============================================================================
// �^�C���̐���
//=============================================================================
void CResultPlayer::CreateTire(void)
{
	//�^�C���̈ʒu��ݒ�
	D3DXVECTOR3 TirePos[MAX_TIRE];

	TirePos[LEFT_FRONT] = LEFT_FRONT_POS;
	TirePos[LEFT_BACK] = LEFT_BACK_POS;
	TirePos[RIGHT_FRONT] = RIGHT_FRONT_POS;
	TirePos[RIGHT_BACK] = RIGHT_BACK_POS;

	//�^�C���̐���
	for (int nCntTire = 0; nCntTire < MAX_TIRE; nCntTire++)
	{
		m_pTire[nCntTire] = NULL;
		m_pTire[nCntTire] = CTire::Create(TirePos[nCntTire]);

		if (m_pTire[nCntTire] != NULL) { m_pTire[nCntTire]->SetParent(m_pModel); }	//�e�q�֌W�̐ݒ�
	}
}
//=============================================================================
// �p�x�C��
//=============================================================================
void CResultPlayer::RemakeAngle(float * pAngle)
{
	//�ړI�̊p�x�C��
	if (*pAngle < -D3DX_PI)
	{
		*pAngle += D3DX_PI * 2.0f;
	}
	if (*pAngle > D3DX_PI)
	{
		*pAngle -= D3DX_PI * 2.0f;
	}
}
//=============================================================================
// �e�[�������v�̎�ނ̐ݒ�
//=============================================================================
void CResultPlayer::SetTailLampType(CTailLamp::TYPE type)
{
	for (int nCntTailLamp = 0; nCntTailLamp < 2; nCntTailLamp++)
	{// �e�[�������v�̐������[�v
		if (NULL != m_apTailLamp[nCntTailLamp])
		{// �e�[�������v�𓧖��ɂ���
			m_apTailLamp[nCntTailLamp]->SetType(type);
		}
	}
}