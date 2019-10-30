//=============================================================================
//
// �v���C���[�̏��� [player.cpp]
// Author : �������� Sato_Asumi
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"
#include "camerabace.h"
#include "scene.h"
#include "model.h"
#include "motion.h"
#include "loadText.h"
#include "wall.h"
#include "incline.h"
#include "sound.h"
#include "tutorial.h"
#include "gamecamera.h"
#include "object.h"
#include "feed.h"
#include "egg.h"
#include "billboord.h"
#include "chick.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define VECTOR_ZERO		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))				//�x�N�g���̏�����
#define FAILE_NAME		("data\\TEXT\\Player\\PlayerState.txt")		//�ǂݍ��ރt�@�C����
#define FILE_TEXTURE	("data\\TEXTURE\\modeltex\\carup.jpg")		//�e�N�X�`���̓ǂݍ���
#define LEFT_FRONT_POS	(D3DXVECTOR3(-26.0f, 3.0f, 45.0f))			//���O�^�C���ʒu
#define LEFT_BACK_POS	(D3DXVECTOR3(-26.0f, 3.0f, -40.0f))			//�����^�C���ʒu
#define RIGHT_FRONT_POS	(D3DXVECTOR3(26.0f, 3.0f, 45.0f))			//�E�O�^�C���ʒu
#define RIGHT_BACK_POS	(D3DXVECTOR3(26.0f, 3.0f, -40.0f))			//�E���^�C���ʒu
#define ACCLE_TIME		(60.0f * 4)									//���������鎞��
#define AVERAGE			(2.0f)										//���ς���l
#define ROLLOVER_STOP	(0.6f)										//���]�h�~�p�x
#define GRAVITY_FRONT	(0.5f)										//�O���̏d��
#define GRAVITY_BACK	(0.2f)										//����̏d��
#define DECELERATION	(0.5f)										//�����̊���
#define START_ENGINE	(200)										//�G���W�����̍Đ�����
#define START_GEARCHANGE (300)										//�X�^�[�g���̃M�A�ؑ֎���
#define EGG_SCALE		(10.0f)										//���̑傫��
#define EGG_RANGE		(50.0f)										// ���ƃv���C���[�̋���
#define EGG_POS			(7)											// �����m�̊Ԋu�̍L���i���₷�ƍL���Ȃ�j
#define SPEEDUP_TIME	(60)										// �������Ă��鎞��
#define SPEEDDOWN		(0.95f)										// ����������l
#define CHICK_SCALE		(D3DXVECTOR3(2.0f, 2.0f, 2.0f))				//�Ђ悱�̑傫��
#define THROW			(13.0f)										// ���𓊂����

// �v���C���[���
#define PLAYER_ACCEL	(1.0f)										//�����l�i�O�i�j
#define PLAYER_BRAKS	(-0.25f)									//�����l�i��i�j
#define PLAYER_DOWN		(0.055f)									//�����x
#define PLAYER_ADDROT	(0.015f)										//��]��

//�ԑ̂̊p�x
#define SHAKE_X			(0.007f)									//X���̗h��
#define INPULS_X		(0.01f)										//X���ɉ�����p�x
#define SHAKE_Z			(0.02f)										//Z���̗h��
#define SHAKE_DRIFT		(0.005f)										//�h���t�g���̊p�x���Z
#define SHAKE_BRAK		(0.01f)										//�u���[�L���̊p�x

//�^�C���̔ԍ�
#define LEFT_FRONT		(0)											//���O�^�C��
#define LEFT_BACK		(1)											//�����^�C��
#define RIGHT_FRONT		(2)											//�E�O�^�C��
#define RIGHT_BACK		(3)											//�E���^�C��

#define RIVER_SOUND_RANGE		(1000.0f)							// ��̉�����������͈�
#define FOUNTAIN_SOUND_RANGE	(1000.0f)							// �����̉�����������͈�

// �R���{���̃X�R�A���Z
#define JUMP_ADD_SCORE	(100)										// �W�����v���̃X�R�A���Z��
#define JUMP_FREAM_TIME	(25)										// �W�����v���̃X�R�A���Z����
#define DRIFT_COMBO_TIME (25)										// �h���t�g���̃X�R�A���Z����
#define THROUGH_COMBO_TIME	(1)									// ���ꂷ�ꎞ�̃X�R�A���Z����
#define DRIFT_SPACE_SMALL	(10)
#define DRIFT_SPACE_BIG		(250)
//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CModel * CPlayer::m_pModel = NULL;		//���f���̃p�[�c�|�C���^
int	CPlayer::m_nMaxModel = 0;
int CPlayer::m_nMaxParts = 0;
int CPlayer::m_nMaxMotion = 0;
CMotion::MOTION_INFO * CPlayer::m_pMotionInfo = NULL;
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;

//=============================================================================
// ��������
//=============================================================================
CPlayer * CPlayer::Create(const D3DXVECTOR3 pos, int nPlayerNum, int nControllerNum)
{
	//�C���X�^���X�̐���
	CPlayer * pPlayer;
	pPlayer = new CPlayer;

	//����������
	pPlayer->Init();
	pPlayer->m_nPlayerNum = nPlayerNum;
	pPlayer->m_nControllerNum = nControllerNum;

	//�ݒ菈��
	pPlayer->Set(pos, VECTOR_ZERO);
	return pPlayer;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPlayer::CPlayer() : CScene(3, OBJTYPE_PLAYER)
{
	m_pPlayerNum = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer() {}

//=============================================================================
// X�t�@�C���̓ǂݍ���
//=============================================================================
void CPlayer::LoadModel(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice,
		FILE_TEXTURE,
		&m_pTexture);

	//���[�V�������̎擾
	CLoadTextMotion * pLoadTextMotion = NULL;
	CManager::MODE mode = CManager::GetMode();

	if (mode == CManager::MODE_GAME)
	{//�Q�[��
		pLoadTextMotion = CGame::GetPlayerMotion();
	}
	else if (mode == CManager::MODE_TUTORIAL)
	{//�`���[�g���A��
		pLoadTextMotion = CTutorial::GetPlayerMotion();
	}

	m_pMotionInfo = pLoadTextMotion->GetMotionInfo();					//���[�V�������̎擾
	m_nMaxMotion = pLoadTextMotion->GetMaxMotion();						//���[�V�����̍ő吔�̎擾

																		//���f�������擾
	CLoadTextMotion::MODEL_INFO ModelInfo = pLoadTextMotion->GetModelInfo();
	m_nMaxModel = ModelInfo.nMaxModel;	//���f���̍ő吔�̎擾
	m_nMaxParts = ModelInfo.nMaxParts;	//���f���̃p�[�c�ő吔�̎擾

										//���f���̐���
	for (int nCntParts = 0; nCntParts < ModelInfo.nMaxParts; nCntParts++)
	{
		m_pModel = CModel::Create(ModelInfo.pOfSetPos[nCntParts], &ModelInfo.paFileName[nCntParts][0]);

		//�e�N�X�`���̊�����
		//if (m_pModel != NULL) { m_pModel->BindTexture(m_pTexture); }
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
void CPlayer::UnloadModel(void)
{
	if (m_pModel != NULL)
	{
		delete m_pModel;
		m_pModel = NULL;
	}

	//���[�V�����f�[�^�̔j��
	if (m_pMotionInfo != NULL)
	{
		m_pMotionInfo = NULL;
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
HRESULT CPlayer::Init(void)
{
	//�ϐ��̏�����
	m_pos = VECTOR_ZERO;				//���S���W
	m_OldPos = VECTOR_ZERO;				//�O��̍��W
	m_move = VECTOR_ZERO;				//�ړ�
	m_pos = VECTOR_ZERO;
	m_rot = VECTOR_ZERO;				//����
	m_nCntFrame = -1;				//����
	m_OldDiffuse = VECTOR_ZERO;				//�O��̍���
	m_MoveMotion = VECTOR_ZERO;				//���[�V�����̈ړ�
	m_vtxMaxModel = VECTOR_ZERO;			//���f���̒��_�ő�l
	m_vtxMinModel = VECTOR_ZERO;			//���f���̒��_�ŏ��l
	m_pMotion = NULL;						//���[�V�����|�C���^
	m_pText = NULL;							//�v���C���[�̃e�L�X�g�|�C���^
	m_MoveState = STATE_DRIVE;					//��Ԑݒ�
	m_StateSpeed = STATE_SPEED_STOP;		//�X�s�[�h�̏�Ԑݒ�
	m_StateHandle = HANDLE_MAX;				//�n���h���̏��
	m_PlayerInfo.nCountTime = 0;			//�J�E���^�[
	m_PlayerInfo.fAccel = PLAYER_ACCEL;		//�����l�i�O�i�j
	m_PlayerInfo.fBraks = PLAYER_BRAKS;		//�����l�i��i�j
	m_PlayerInfo.fDown = PLAYER_DOWN;		//�����l
	m_PlayerInfo.fAddRot = PLAYER_ADDROT;	//�������]�l
	m_PlayerInfo.fDistance = 0.0f;			//����
	m_PlayerInfo.FirstPos = VECTOR_ZERO;	//�����ʒu
	m_bJump = false;						//�W�����v���
	m_bControl = true;						//�R���g���[���[�t���O
	m_nCountJumpTime = 0;					//�W�����v��Ԃ̎��Ԃ����Z
	m_fvtxMaxY = 0.0f;						//���f�����_�̍ő�l�iY�j
	m_fMass = 200.0f;						// ����
	m_fSpeed = 0.0f;						// ����
	m_nCountSpeed = 0;
	m_nCountTime = 0;						// ���ԃJ�E���^�[
	m_bCrcleCarIn = false;
	m_pLoadEffect = NULL;					// �G�t�F�N�g�c�[���|�C���^
	m_nCntCombo = 0;
	m_nCntShake = 0;
	m_bShake = true;
	m_nCntFlag = 0;
	m_bDirive = true;
	m_nNumEgg = 0;
	m_nNumChick = 0;
	m_bJumpSave = false;
	m_nCntDamage = 0;
	m_bDamage = false;
	m_State = PLAYERSTATE_NORMAL;

	m_nPlayerNum = 0;				// �v���C���[�ԍ�
	m_nControllerNum = 0;			// �R���g���[���[�ԍ�

									// �v���C���[�ԍ��i�Ǐ]�j
	if (m_pPlayerNum == NULL)
		m_pPlayerNum = CBillBoord::Create(m_pos + D3DXVECTOR3(0.0f, 80.0f, 0.0f), D3DXVECTOR2(25.0f, 25.0f), 0);

	for (int nCntEggPos = 0; nCntEggPos < MAX_FRAME; nCntEggPos++)
	{
		m_OldEggRot[nCntEggPos] = VECTOR_ZERO;
		m_abJump[nCntEggPos] = false;
	}

	//���̃|�C���^��������
	for (int nCntEgg = 0; nCntEgg < MAX_EGG; nCntEgg++)
	{
		m_pEgg[nCntEgg] = NULL;
		m_pChick[nCntEgg] = NULL;
	}

	if (m_pMotion == NULL)	//���[�V�����̐���
	{
		m_pMotion = CMotion::Create();

		if (m_pMotion != NULL)
		{
			//���f���f�[�^�̐ݒ�
			m_pMotion->SetModel(&m_pModel);			//���[�V�����f�[�^�Ƀ��f������n��
			m_pMotion->SetMaxModel(m_nMaxParts);	//���f���p�[�c�̍ő吔��n��

													//���[�V�����f�[�^�̐ݒ�
			m_pMotion->SetMotion(m_pMotionInfo);	//���[�V�������̎擾
			m_pMotion->SetMaxMotion(m_nMaxMotion);	//���[�V�����̍ő吔�̎擾

													//����������
			m_pMotion->Init();
		}
	}

	//���f���̍ŏ��l�E�ő�l�̎擾
	m_vtxMaxModel = m_pModel->GetVtxMax();
	m_vtxMinModel = m_pModel->GetVtxMin();
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPlayer::Uninit(void)
{
	if (m_pPlayerNum != NULL)
	{
		m_pPlayerNum->Uninit();
		m_pPlayerNum = NULL;
	}

	//���[�V�����̔j��
	if (m_pMotion != NULL)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = NULL;
	}

	//�^�C�����f���̔j��
	for (int nCntEgg = 0; nCntEgg < MAX_EGG; nCntEgg++)
	{
		if (m_pEgg[nCntEgg] != NULL)
		{
			m_pEgg[nCntEgg] = NULL;
		}

		if (m_pChick[nCntEgg] != NULL)
		{
			m_pChick[nCntEgg] = NULL;
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
void CPlayer::Update(void)
{
	//�ϐ��錾
	D3DXVECTOR3 TirePos[MAX_TIRE];
	CManager::MODE mode = CManager::GetMode();
	CGameCamera * pGameCamera = NULL;
	m_bShake = true;

	if (m_bControl)
	{//�R���g���[���L�[
		ControlKey();
	}
	else
	{//�n���h���������Ă��Ȃ���Ԃɂ���
		SetStateHandle(HANDLE_MAX);
		SetStateSpeed(STATE_SPEED_DOWN);
	}

	//�^�C���A�b�v��ԂȂ�ȍ~�͍X�V���Ȃ�
	//if (CTime::GetTime() == 0 && CManager::MODE_GAME == CManager::GetMode()) { return; }

	UpdateMove();			// �ړ�����

	UpdateField();

	CollisionFeed();		// �a�̓����蔻��

	CollisionEgg();			// ���Ƃ̓����蔻��

	ChaseEgg();				// �������Ă��鏈��

	CollisionCharacter();	// �L�����N�^�[���m�̓����蔻��
	CollisionObject();		// �I�u�W�F�N�g�Ƃ̓����蔻��

	if (m_pPlayerNum != NULL)
	{
		m_pPlayerNum->SetPosSize(m_pos + D3DXVECTOR3(0.0f, 80.0f, 0.0f), D3DXVECTOR2(25.0f, 25.0f));
		m_pPlayerNum->SetTexture(m_nPlayerNum, 5, 1, 1);
	}

	DebugProc();		// �f�o�b�N�\��
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPlayer::Draw(void)
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
}

//=============================================================================
// �R���g���[���L�[
//=============================================================================
void CPlayer::ControlKey(void)
{
	CSound *pSound = CManager::GetSound();
	CInputKeyBoard * pInputKeyboard = CManager::GetInput();		//�L�[�{�[�h�̎擾
	CInputXPad * pXpad = CManager::GetXInput();					//�W���C�p�b�g�̎擾

																//�O�i��ނ̐ݒ�
	if (m_bDirive)
	{
		if ((pInputKeyboard->GetKeyboardPress(DIK_L) == true) ||
			(pXpad->GetPress(CInputXPad::XPADOTHER_TRIGGER_RIGHT, m_nControllerNum) == true) ||
			(pXpad->GetPress(XINPUT_GAMEPAD_RIGHT_SHOULDER, m_nControllerNum) == true))
		{
			SetState(STATE_DRIVE);		//�O�i��Ԃɐݒ�
		}
		if ((pInputKeyboard->GetKeyboardPress(DIK_K) == true) ||
			(pXpad->GetPress(CInputXPad::XPADOTHER_TRIGGER_LEFT, m_nControllerNum) == true) ||
			(pXpad->GetPress(XINPUT_GAMEPAD_LEFT_SHOULDER, m_nControllerNum) == true))
		{
			SetState(STATE_REVERSE);	//��ޏ�Ԃɐݒ�
		}
	}

	//�����̐ݒ�
	if (m_StateSpeed != STATE_SPEED_STOP)
	{
		if ((pInputKeyboard->GetKeyboardPress(DIK_A) == true) || pXpad->GetPress(CInputXPad::XPADOTHER_STICK_L_LEFT, m_nControllerNum) == true || pXpad->GetPress(XINPUT_GAMEPAD_DPAD_LEFT, m_nControllerNum) == true)
		{ //���n���h�����
			SetStateHandle(HANDLE_LEFT);
		}
		else if ((pInputKeyboard->GetKeyboardPress(DIK_D) == true) || pXpad->GetPress(CInputXPad::XPADOTHER_STICK_L_RIGHT, m_nControllerNum) == true || pXpad->GetPress(XINPUT_GAMEPAD_DPAD_RIGHT, m_nControllerNum) == true)
		{//�E�n���h�����
			SetStateHandle(HANDLE_RIGHT);
		}
		else
		{//�n���h����G���Ă��Ȃ����
			SetStateHandle(HANDLE_MAX);
		}
	}
	else
	{
		SetStateHandle(HANDLE_MAX);
	}

	//�A�N�Z��
	if ((pInputKeyboard->GetKeyboardPress(DIK_K) == true) ||
		(pXpad->GetPress(CInputXPad::XPADOTHER_TRIGGER_LEFT, m_nControllerNum) == true) ||
		(pXpad->GetPress(XINPUT_GAMEPAD_LEFT_SHOULDER, m_nControllerNum) == true))
	{//�������
		SetStateSpeed(STATE_SPEED_BRAKS);
	}
	else if
		((pInputKeyboard->GetKeyboardPress(DIK_L) == true) ||
		(pXpad->GetPress(CInputXPad::XPADOTHER_TRIGGER_RIGHT, m_nControllerNum) == true) ||
			(pXpad->GetPress(XINPUT_GAMEPAD_RIGHT_SHOULDER, m_nControllerNum) == true))
	{ //�A�N�Z�������
		SetStateSpeed(STATE_SPEED_ACCEL);
	}
	else
	{
		//��~����
		D3DXVECTOR3 fDiffuse = m_pos - m_OldPos;

		if (fDiffuse.x < 0.10f && fDiffuse.x > -0.10f)
		{
			if (fDiffuse.z < 0.10f && fDiffuse.z > -0.10f)
			{
				SetStateSpeed(STATE_SPEED_STOP);

				if (m_State == PLAYERSTATE_SPEEDUP)
				{// �X�s�[�h�A�C�e�����g�����Ƃ�
				 //�i�s�����̐ݒ�
					m_move.x += sinf(m_rot.y) * (m_fSpeed);
					m_move.z += cosf(m_rot.y) * (m_fSpeed);
				}
			}
			else
			{
				SetStateSpeed(STATE_SPEED_DOWN);
			}
		}
		else
		{
			SetStateSpeed(STATE_SPEED_DOWN);
		}
	}

	//if (m_StateSpeed == STATE_SPEED_ACCEL)
	//{
	//	//���鉹
	//	pSound->SetVolume(CSound::SOUND_LABEL_SE_RAN, 3.0f);
	//	pSound->PlaySound(CSound::SOUND_LABEL_SE_RAN);
	//}

	// �W�����v
	if (m_bJump == false)
	{// �W�����v���Ă��Ȃ�
		m_bJumpSave = false;

		if (pInputKeyboard->GetKeyboardTrigger(DIK_W) == true || pXpad->GetTrigger(XINPUT_GAMEPAD_A, m_nControllerNum) == true)
		{// �W�����v�L�[
			m_bJumpSave = true;
			m_bJump = true;
			m_move.y += 7.5f;
		}
	}
	else
	{
		m_bJumpSave = false;
	}

	if (pInputKeyboard->GetKeyboardTrigger(DIK_SPACE) == true || pXpad->GetTrigger(XINPUT_GAMEPAD_B, m_nControllerNum) == true)
	{// �e����
		BulletEgg();
	}

	ChickAppear();

	/*if ((pInputKeyboard->GetKeyboardPress(DIK_N) == true) || (pInputJoypad->GetPress(CXInput::XIJS_BUTTON_0) == true))
	{
	pSound->SetVolume(CSound::SOUND_LABEL_SE_KLAXON, 1.5f);
	pSound->PlaySound(CSound::SOUND_LABEL_SE_KLAXON);
	}*/
}

//=============================================================================
// �ړ�����
//=============================================================================
void CPlayer::UpdateMove(void)
{
	CSound *pSound = CManager::GetSound();

	m_OldPos = m_pos;	//�O��̈ʒu��ۑ�����

	RemakeAngle(&m_rot.y);

	if (m_State == PLAYERSTATE_SPEEDUP)
	{// �X�s�[�h�A�C�e�����g�����Ƃ�
		m_nCountSpeed++;

		if (m_nCountSpeed > SPEEDUP_TIME)
		{
			m_State = PLAYERSTATE_NORMAL;
			m_nCountSpeed = 0;
		}
	}

	//��Ԃ��Ƃ̍X�V����
	switch (m_StateSpeed)
	{
	case STATE_SPEED_ACCEL:	//�A�N�Z�����

							//�W�����v��ԂȂ�
		if (m_bJump == true) { break; }

		if (m_State == PLAYERSTATE_NORMAL)
		{
			m_fSpeed = m_PlayerInfo.fAccel * (m_PlayerInfo.nCountTime < 90 ? (m_PlayerInfo.nCountTime / 90) : 1.0f);
		}

		//�i�s�����̐ݒ�
		m_move.x += sinf(m_rot.y) * (m_fSpeed);
		m_move.z += cosf(m_rot.y) * (m_fSpeed);
		break;

	case STATE_SPEED_BRAKS: //�u���[�L���

							//�W�����v��ԂȂ�
		if (m_bJump == true) { break; }

		m_fSpeed = m_PlayerInfo.fBraks * (m_PlayerInfo.nCountTime < 90 ? (m_PlayerInfo.nCountTime / 90) : 1.0f);

		//�i�s�����̐ݒ�
		m_move.x += sinf(m_rot.y) * m_fSpeed;
		m_move.z += cosf(m_rot.y) * m_fSpeed;

		//�h��𖳌��ɂ���
		m_bShake = false;
		break;
	case STATE_SPEED_DOWN: //�_�E�����
		m_PlayerInfo.nCountTime = 0;

		if (m_State == PLAYERSTATE_SPEEDUP)
		{// �X�s�[�h�A�C�e�����g�����Ƃ�
		 //�i�s�����̐ݒ�
			m_move.x += sinf(m_rot.y) * (m_fSpeed);
			m_move.z += cosf(m_rot.y) * (m_fSpeed);
		}

		break;
	}

	switch (m_State)
	{// �e��H������Ƃ�
	case PLAYERSTATE_NORMAL:
		break;

	case PLAYERSTATE_SPEEDDOWN:
		//�i�s�����̐ݒ�
		m_move.x *= SPEEDDOWN;
		m_move.z *= SPEEDDOWN;

		//�X�s�[�h�_�E���̉�
		pSound->SetVolume(CSound::SOUND_LABEL_SE_SPEEDDOWN, 2.0f);
		pSound->PlaySound(CSound::SOUND_LABEL_SE_SPEEDDOWN);

		break;

	case PLAYERSTATE_DAMAGE:

		m_fSpeed = 0.0f;

		//�i�s�����̐ݒ�
		m_move.x = sinf(m_rot.y) * (m_fSpeed);
		m_move.z = cosf(m_rot.y) * (m_fSpeed);

		m_PlayerInfo.nCountTime = 0;
		break;
	}

	if (m_bDamage == true)
	{
		m_nCntDamage++;

		int nDamageTime = 0;	// ��Ԃ��ς�鎞�Ԃ̒���

		if (m_State == PLAYERSTATE_DAMAGE)
		{// �U����H������Ƃ�
			nDamageTime = DAMAGE_TIME;
		}
		else if (m_State == PLAYERSTATE_SPEEDDOWN)
		{// �X�s�[�h�_�E����H������Ƃ�
			nDamageTime = SPEEDDOWN_TIME;
		}

		if (m_nCntDamage > nDamageTime)
		{
			m_State = PLAYERSTATE_NORMAL;
			m_nCntDamage = 0;

			m_bDamage = false;
		}
	}

	m_PlayerInfo.nCountTime++;

	//CDebugProc::Print("�A�N�Z�� : %1f\n", m_PlayerInfo.fAccel);
	//CDebugProc::Print("�X�s�[�h : %1f  %1f  %1f\n", m_move.x, m_move.y, m_move.z);

	//�n���h���̏�ԍX�V
	if (m_StateHandle == HANDLE_LEFT)
	{
		if (m_StateSpeed != STATE_SPEED_STOP)
		{
			m_rot.y -= m_PlayerInfo.fAddRot * (m_PlayerInfo.nCountTime < 45 ? (m_PlayerInfo.nCountTime / 45) : 1.0f);
		}
	}
	else if (m_StateHandle == HANDLE_RIGHT)
	{
		if (m_StateSpeed != STATE_SPEED_STOP)
		{
			m_rot.y += m_PlayerInfo.fAddRot * (m_PlayerInfo.nCountTime < 45 ? (m_PlayerInfo.nCountTime / 45) : 1.0f);
		}
	}

	// �d��
	//if (m_bShake == true)
	{
		m_move.y -= cosf(0) * 0.4f;
	}

	//���S���W�X�V
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;
	m_pos.z += m_move.z;

	//����
	if (!m_bJump)
	{
		m_move.x += (0.0f - m_move.x) * m_PlayerInfo.fDown;
		m_move.z += (0.0f - m_move.z) * m_PlayerInfo.fDown;
	}

	//���̕\��
	/*if ((fDiffuse.x < 1.5f) && (fDiffuse.x > -1.5f))
	{
	if ((fDiffuse.z < 1.5f) && (fDiffuse.z > -1.5f))
	{
	m_bSmoke = false;
	m_bGrassEffect = false;		//���G�t�F�N�g��\��
	}
	else
	{
	m_bSmoke = true;
	}
	}
	else
	{
	m_bSmoke = true;
	}

	//���q������悹�邽�߂̒�~����
	if ((fDiffuse.x < 1.0f) && (fDiffuse.x > -1.0f))
	{
	if ((fDiffuse.z < 1.0f) && (fDiffuse.z > -1.0f))
	{
	m_bCustomrStop = true;
	}
	else
	{
	m_bCustomrStop = false;
	}
	}
	else
	{
	m_bCustomrStop = false;
	}*/

	/*if ((m_StateSpeed == STATE_SPEED_BRAKS))
	{
	//�p�x��ς���
	m_rot.x += SHAKE_BRAK;
	}*/
}

//=============================================================================
// �h��̍X�V����
//=============================================================================
void CPlayer::UpdateShake(void)
{
	if (!m_bShake) { return; }

	m_nCntShake++;	//�J�E���^�[�̉��Z

	if ((m_nCntShake % 8) == 0)
	{//�ԑ̂�h�炷
		m_rot.x += SHAKE_X;

		if (((m_nCntShake % 24) == 0) && (m_MoveState != STATE_SPEED_STOP))
		{
			int nRand = rand() % 5;

			if (nRand == 0)
			{
				m_rot.x += INPULS_X;
			}
		}
	}
}

//=============================================================================
// �v���C���[�̍�������
//=============================================================================
void CPlayer::UpdateField(void)
{
	CScene *pScene = CScene::GetTop(MESH_PRIOTITY);

	//NULL�`�F�b�N
	while (pScene != NULL)
	{
		//Update��Uninit����Ă��܂��ꍇ�@Next��������\�������邩��Next�Ƀf�[�^���c���Ă���
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetObjType() == CScene::OBJTYPE_GROUND)
		{//�^�C�v���n�ʂ�������
			CMeshField *pField = (CMeshField*)pScene;

			if (pField->OnField(m_pos, m_fvtxMaxY))
			{// �X�΂̌v�Z
				float fHight = pField->GetHeightMesh(m_pos);

				//if (m_pos.y <= fHight)
				if (m_bJump == false || (m_bJump == true && m_pos.y < fHight))
				{
					m_pos.y = fHight;					//�n�ʂ̍������擾
					m_move.y = 0.0f;					//�ړ��ʂ�����������

														//�W�����v�����ǂ���
														//bool bJumpFlag = pPlayer->GetJump();
														//if (bJumpFlag)
														//{
														//	pPlayer->GetPlayerInfoPoint()->nCountTime -= (int)std::round(pPlayer->GetPlayerInfoPoint()->nCountTime * 0.05f);
														//}

														//�W�����v�̏�Ԑݒ�
					m_bJump = false;
					//m_pIncline->SetJumpFlag(false);

					break;
				}
			}
		}
		//Next�Ɏ���Scene������
		pScene = pSceneNext;
	}
}

//=============================================================================
// ��Ԃ̐ݒ�
//=============================================================================
void CPlayer::SetState(CPlayer::STATE state)
{
	//�l�̔��]
	if (m_MoveState != state)
	{
		//m_PlayerInfo.fAccel *= -0.5f;
		//m_PlayerInfo.fBraks *= -1;
		//m_fMaxSpeed *= -1;

		CSound *pSound = CManager::GetSound();

		if (STATE_REVERSE == state)
		{// �o�b�N��
		 //pSound->StopSound(CSound::SOUND_LABEL_SE_ACCEL);
		 //pSound->PlaySoundA(CSound::SOUND_LABEL_SE_BACK);
		}
		else if (STATE_DRIVE == state)
		{
			//pSound->StopSound(CSound::SOUND_LABEL_SE_BACK);
			//pSound->SetVolume(CSound::SOUND_LABEL_SE_ACCEL, 0.7f);
			//pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ACCEL);
		}
	}

	//��Ԃ̐ݒ�
	m_MoveState = state;
}

//=============================================================================
// �X�s�[�h�̏�Ԑݒ�
//=============================================================================
void CPlayer::SetStateSpeed(CPlayer::STATE_SPEED state)
{
	//���l�̐ݒ�
	if (m_StateSpeed != state)
	{
		m_PlayerInfo.nCountTime = 0;

		CSound *pSound = CManager::GetSound();

		/*if (STATE_SPEED_ACCEL == state && STATE_DRIVE == m_MoveState)
		{// �A�N�Z����
		pSound->SetVolume(CSound::SOUND_LABEL_SE_ACCEL, 0.7f);
		pSound->PlaySound(CSound::SOUND_LABEL_SE_ACCEL);
		}
		else if (STATE_SPEED_BRAKS == state)
		{// �u���[�L��
		if (m_move.x >= 0.1f || m_move.x <= -0.1f || m_move.z >= 0.1f || m_move.z <= -0.1f)
		{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_BRAKING);
		pSound->StopSound(CSound::SOUND_LABEL_SE_ACCEL);
		}
		}*/
	}

	//��Ԃ̐ݒ�
	m_StateSpeed = state;
}

//=============================================================================
// �ݒ菈��
//=============================================================================
void CPlayer::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	m_pos = pos;
}

//=============================================================================
// �I�u�W�F�N�g�Ƃ̓����蔻��
//=============================================================================
void CPlayer::CollisionObject(void)
{
	CSound *pSound = CManager::GetSound();
	CScene *pScene;

	//������΍��Ȃ��I�u�W�F�N�g�ԍ��̓o�^
	/*int anUpdateType[UPDATE_TYPE_NUM + HIGHT_OBJ_NUM + 2] = { TYPE_TREE00, TYPE_TREE01, TYPE_BILL00, TYPE_BILL01,
	TYPE_BILL02, TYPE_TVBILL, TYPE_FLOWER, TYPE_TANUKI,
	TYPE_OCLOCK, TYPE_REDBILL, TYPE_TREE02,  TYPE_CORN2,
	TYPE_STATION, TYPE_ESTA, TYPE_DAIMAL, TYPE_APIA,
	TYPE_TOWER, TYPE_FOUNTAIN, TYPE_FERRISWGEEL,
	TYPE_STREETLIGHT, TYPE_TRAFFICLIGHT00, TYPE_TRAFFICLIGHT01, TYPE_ROAD , TYPE_TAPIOCA, TYPE_HOSPITAL };*/

	int anUpdateType[UPDATE_TYPE_NUM + HIGHT_OBJ_NUM + 2] = { 0 };	// ��
	bool bType = false;	//�^�C�v�̃t���O

	for (int nCntPriority = 2; nCntPriority <= OBJECT_PRIOTITY; nCntPriority++)
	{
		// �v���C�I���e�B�[�`�F�b�N
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �v���C�I���e�B�[���̃��X�g�\�����Ō�܂Ō���
			CScene *pSceneNext = pScene->GetNext();		// ���̃I�u�W�F�N�g��ۑ�

			if (pScene->GetObjType() == OBJTYPE_OBJECT)
			{// �^�C�v����Q����������
				CObject *pObject = (CObject*)pScene;	// �I�u�W�F�N�g�N���X�̃|�C���^�ϐ��ɂ���
				int nType = pObject->GetType();			// ��Q���̎�ނ��擾
														//�����蔻��p�̃I�u�W�F�N�g�ɂ̂ݓ�����
				if (nType == 2) { pObject->CollisionObject(&m_pos, &m_OldPos, &m_move); }
			}

			// Next�Ɏ���Scene������
			pScene = pSceneNext;
		}
	}

	//�f�o�b�N�\��
	/*if (m_bCrcleCarIn == true)
	{
	CDebugProc::Print("�����Ă���\n");
	}
	else if (m_bCrcleCarIn == false)
	{
	CDebugProc::Print("�����Ă��Ȃ�\n");
	}*/
}

//=============================================================================
// �ǂƂ̓����蔻��
//=============================================================================
void CPlayer::CollisitionWall(void)
{
	CScene *pScene;
	//�v���C�I���e�B�[�`�F�b�N
	pScene = CScene::GetTop(WALL_PRIOTITY);

	//NULL�`�F�b�N
	while (pScene != NULL)
	{
		//Update��Uninit����Ă��܂��ꍇ�@Next��������\�������邩��Next�Ƀf�[�^���c���Ă���
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetDeath() == false)
		{
			//�^�C�v���ǂ�������
			if (pScene->GetObjType() == OBJTYPE_WALL)
			{
				bool bTouch = ((CWall*)pScene)->CollisionWall(&m_pos, &m_OldPos, &m_move, 20.0f);

				if (bTouch)
				{ //�������Ă���Ȃ猸������
					m_PlayerInfo.fAccel = m_PlayerInfo.fAccel * DECELERATION;
					m_PlayerInfo.nCountTime = 0;
					break;
				}
			}
		}

		//Next�Ɏ���Scene������
		pScene = pSceneNext;
	}
}

//=============================================================================
// �ԑ̂̊p�x�����߂�
//=============================================================================
void CPlayer::CarCalculate(D3DXVECTOR3 * TirePos)
{
	if (m_bJump) { return; }
	//�ϐ��錾
	float fHight = 0.0f;
	float fTireHight = 0.0f;
	bool bDecisionAngle = false;

	//�ԑ̂̊p�x�����߂�
	if (!bDecisionAngle) { return; }

	//----------------------
	// X���̊p�x�����߂�
	//----------------------
	float fHight00 = TirePos[LEFT_FRONT].y - TirePos[LEFT_BACK].y;			//���E�̍��������߂�
	float fHight01 = TirePos[RIGHT_FRONT].y - TirePos[RIGHT_BACK].y;

	float fLength00 = GetLength(TirePos[LEFT_FRONT], TirePos[LEFT_BACK]);		//�Q���_�̒��������߂�
	float fLength01 = GetLength(TirePos[RIGHT_FRONT], TirePos[RIGHT_BACK]);

	float fAngleLeft = fHight00 / fLength00;		//�p�x�����߂�
	float fAngleRight = fHight01 / fLength01;
	RemakeAngle(&fAngleLeft);						//�p�x�C��
	RemakeAngle(&fAngleRight);

	//���ϒl�����߂�
	float fAngleX = (fAngleLeft + fAngleRight) / AVERAGE;
	RemakeAngle(&fAngleX);

	//----------------------
	// Z���̊p�x�����߂�
	//----------------------
	fHight00 = TirePos[LEFT_FRONT].y - TirePos[RIGHT_FRONT].y;			//�O��̍��������߂�
	fHight01 = TirePos[LEFT_BACK].y - TirePos[RIGHT_BACK].y;

	fLength00 = GetLength(TirePos[LEFT_FRONT], TirePos[RIGHT_FRONT]);		//�Q���_�̒��������߂�
	fLength01 = GetLength(TirePos[LEFT_BACK], TirePos[RIGHT_BACK]);

	float fAngleFront = fHight00 / fLength00;		//�p�x�����߂�
	float fAngleBack = fHight01 / fLength01;
	RemakeAngle(&fAngleFront);						//�p�x�C��
	RemakeAngle(&fAngleBack);

	//���ϒl�����߂�
	float fAngleZ = (fAngleFront + fAngleBack) / AVERAGE;
	RemakeAngle(&fAngleZ);

	//-------------------------
	// �����̒l�C��(���]�h�~)
	//-------------------------
	fAngleX *= -1;
	fAngleZ *= -1;
	RemakeCarRot(&fAngleX);
	RemakeCarRot(&fAngleZ);

	//������ݒ肷��
	m_rot.x = fAngleX;
	m_rot.z = fAngleZ;

	//CDebugProc::Print("LEFT : %.2f, RIGHT : %.2f\n", fAngleLeft, fAngleRight);
	//CDebugProc::Print("FRONT : %.2f, BACK : %.2f\n", fAngleFront, fAngleBack);
	//CDebugProc::Print("ANGLE : X %.2f, Z %.2f\n", fAngleX, fAngleZ);
}

//=============================================================================
// �W�����v��Ԃ̍X�V����
//=============================================================================
void CPlayer::UpdateStateJump(void)
{
	if (m_bJump)
	{//�W�����v��Ԃ̎��Ԃ����Z
		m_nCountJumpTime++;

		/*//X���̊p�x����
		if (m_rot.x > 0.5f)
		{
		m_rot.x = 0.5f;
		}
		else if (m_rot.x < -0.5f)
		{
		m_rot.x = -0.5f;
		}

		//Z���̊p�x����
		if (m_rot.z > 0.5f)
		{
		m_rot.z = 0.5f;
		}
		else if (m_rot.z < -0.5f)
		{
		m_rot.z = -0.5f;
		}*/
		return;
	}
	m_nCountJumpTime = 0;
}

//=============================================================================
// �ӂ̒��������߂�
//=============================================================================
float CPlayer::GetLength(D3DXVECTOR3 StartPos, D3DXVECTOR3 EndPos)
{
	D3DXVECTOR3 Distance = StartPos - EndPos;
	float fLength = sqrtf((Distance.x * Distance.x) + (Distance.z * Distance.z));

	return fLength;
}

//=============================================================================
// �Ԃ̊p�x�C��
//=============================================================================
void CPlayer::RemakeCarRot(float * pAngle)
{
	if (*pAngle > ROLLOVER_STOP)
	{
		*pAngle = ROLLOVER_STOP;
	}
	else if (*pAngle < -ROLLOVER_STOP)
	{
		*pAngle = -ROLLOVER_STOP;
	}
}

//=============================================================================
// �p�x�C��
//=============================================================================
void CPlayer::RemakeAngle(float * pAngle)
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
// �f�o�b�N�\��
//=============================================================================
void CPlayer::DebugProc(void)
{
	//��Ԃ̕\��
	/*if (m_MoveState == STATE_DRIVE)
	{
	CDebugProc::Print("��� : STATE_DRIVE\n");
	}
	else
	{
	CDebugProc::Print("��� : STATE_REVERSE\n");
	}

	//���s��Ԃ̕W��
	if (m_StateSpeed == STATE_SPEED_STOP)
	{
	CDebugProc::Print("��~���\n");
	}
	*/
	//�ʒu�\��
	CDebugProc::Print("�ʒu : X %.2f, Y %.2f, Z %.2f\n", m_pos.x, m_pos.y, m_pos.z);

	CDebugProc::Print("�W�����v�F%s\n", m_bJump ? "�Z" : "�~");

	//CDebugProc::Print("�J�E���g : %f\n", m_PlayerInfo.nCountTime);

}

//=============================================================================
// ���̍Đ�
//=============================================================================
void CPlayer::PlaySoundObj(int nType, CSound * pSound)
{
	/*switch (nType)
	{*/
	/*case TYPE_PHONEBOX: pSound->PlaySoundA(CSound::SOUND_LABEL_SE_STEAL); break;
	case TYPE_CARDBORD: pSound->PlaySoundA(CSound::SOUND_LABEL_SE_SMALLBOX); break;
	case TYPE_CORN:
	pSound->SetVolume(CSound::SOUND_LABEL_SE_SMALLBOX, 0.5f);
	pSound->PlaySoundA(CSound::SOUND_LABEL_SE_SMALLBOX);
	break;
	case TYPE_LEAF:

	break;
	case TYPE_FENCE:*/
	//if ((m_nCountSound % 5) == 0) { m_nCountSound = 0; }

	//if (m_nCountSound == 0)
	//{//�t�F���X00
	//	pSound->PlaySoundA(CSound::SOUND_LABEL_SE_FANCE00);
	//}
	//else if (m_nCountSound == 1)
	//{//�t�F���X01
	//	pSound->PlaySoundA(CSound::SOUND_LABEL_SE_FANCE00);
	//}
	//else if (m_nCountSound == 2)
	//{//�t�F���X02
	//	pSound->PlaySoundA(CSound::SOUND_LABEL_SE_FANCE00);
	//}
	//else if (m_nCountSound == 3)
	//{//�t�F���X02
	//	pSound->PlaySoundA(CSound::SOUND_LABEL_SE_FANCE00);
	//}
	//else if (m_nCountSound == 4)
	//{//�t�F���X02
	//	pSound->PlaySoundA(CSound::SOUND_LABEL_SE_FANCE00);
	//}

	//m_nCountSound++;	//�J�E���^�[�̉��Z
	//break;

	//case TYPE_BENCH:
	//if ((m_nCountWood % 2) == 0) { m_nCountWood = 0; }	//�J�E���^�[�����Z�b�g����

	////���̍Đ�
	//if (m_nCountWood == 0)
	//{
	//	pSound->SetVolume(CSound::SOUND_LABEL_SE_WOOD00, 2.0f);
	//	pSound->PlaySoundA(CSound::SOUND_LABEL_SE_WOOD00);
	//}
	//else if (m_nCountWood)
	//{
	//	pSound->PlaySoundA(CSound::SOUND_LABEL_SE_WOOD01);
	//}

	////�J�E���^�[�̉��Z
	//m_nCountWood++;
	//break;

	//case TYPE_SIGNBOARD: pSound->PlaySoundA(CSound::SOUND_LABEL_SE_WOOD01); break;
	//}
}

//=============================================================================
// �a�Ƃ̓����蔻��
//=============================================================================
void CPlayer::CollisionFeed(void)
{
	if (m_nNumEgg + m_nNumChick >= MAX_EGG) { return; }

	CSound *pSound = CManager::GetSound();
	CScene *pScene;

	for (int nCntPriority = 2; nCntPriority <= FEED_PRIOTITY; nCntPriority++)
	{
		// �v���C�I���e�B�[�`�F�b�N
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �v���C�I���e�B�[���̃��X�g�\�����Ō�܂Ō���
			CScene *pSceneNext = pScene->GetNext();		// ���̃I�u�W�F�N�g��ۑ�

			if (pScene->GetObjType() == OBJTYPE_FEED)
			{// �^�C�v����Q����������
				CFeed *pFeed = (CFeed*)pScene;	// �I�u�W�F�N�g�N���X�̃|�C���^�ϐ��ɂ���

				if (pFeed->GetDeath() != true)
				{
					if (pFeed->CollisionFeed(&m_pos, &m_OldPos))
					{// �Փ˂���
						EggAppear(pFeed);	// ���o��
						pFeed->Uninit();	// �a�폜
						m_nNumEgg++;
					}
				}
			}

			// Next�Ɏ���Scene������
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// ���̏o������
//=============================================================================
void CPlayer::EggAppear(CFeed *pFeed)
{
	if (m_pEgg[m_nNumEgg] == NULL)
	{
		if (pFeed->GetFeedType() == CFeed::FEEDTYPE_ATTACK)
		{// �U���̗�����
			m_pEgg[m_nNumEgg] = CEgg::Create(m_pos,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(EGG_SCALE, EGG_SCALE, EGG_SCALE),
				CEgg::EGGTYPE_ATTACK,
				CEgg::BULLETTYPE_PLAYER,
				m_nPlayerNum);
		}
		else if (pFeed->GetFeedType() == CFeed::FEEDTYPE_ANNOY)
		{// �W�Q�̗�����
			m_pEgg[m_nNumEgg] = CEgg::Create(m_pos,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(EGG_SCALE, EGG_SCALE, EGG_SCALE),
				CEgg::EGGTYPE_ANNOY,
				CEgg::BULLETTYPE_PLAYER,
				m_nPlayerNum);
		}
		else if (pFeed->GetFeedType() == CFeed::FEEDTYPE_SPEED)
		{// �����̗�����
			m_pEgg[m_nNumEgg] = CEgg::Create(m_pos,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(EGG_SCALE, EGG_SCALE, EGG_SCALE),
				CEgg::EGGTYPE_SPEED,
				CEgg::BULLETTYPE_PLAYER,
				m_nPlayerNum);
		}
	}

	m_pEgg[m_nNumEgg]->SetState(CEgg::EGGSTATE_CHASE);
}


//=============================================================================
// �������Ă��鏈��
//=============================================================================
void CPlayer::ChaseEgg(void)
{
	m_nCntFrame++;

	if (m_nCntFrame >= MAX_FRAME)
	{
		m_nCntFrame = 0;
	}

	// �O�̃v���C���[�̊p�x��ۑ�
	m_OldEggRot[m_nCntFrame] = m_rot;
	m_abJump[m_nCntFrame] = m_bJumpSave;

	if (m_nNumEgg + m_nNumChick <= 0) { return; }

	if (m_nNumEgg >= 1)
	{// ������̎�
		if (m_pEgg[0]->GetState() == CEgg::EGGSTATE_CHASE)
		{
			// �O�̌�������
			int nData = m_nCntFrame - EGG_POS * (1 + m_nNumChick);

			if (nData < 0)
			{
				nData += MAX_FRAME;
			}

			// ���̈ʒu�ݒ�
			m_pEgg[0]->SetPosition(D3DXVECTOR3((sinf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * (1 + m_nNumChick)) + m_pos.x,
				m_pEgg[0]->SetHeight(),
				(cosf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * (1 + m_nNumChick)) + m_pos.z));

			// ���̊p�x�ݒ�
			m_pEgg[0]->SetRot(m_OldEggRot[nData]);

			if (m_abJump[nData] == true)
			{
				m_pEgg[0]->Jump(EGGJUMP);
			}
		}
	}
	if (m_nNumEgg >= 2)
	{// ������̎�
	 // ���
		int nData = m_nCntFrame - EGG_POS * 2;

		if (nData < 0)
		{
			nData += MAX_FRAME;
		}

		m_pEgg[1]->SetPosition(D3DXVECTOR3((sinf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * (2 + m_nNumChick)) + m_pos.x,
			m_pEgg[1]->SetHeight(),
			(cosf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * (2 + m_nNumChick)) + m_pos.z));

		m_pEgg[1]->SetRot(m_OldEggRot[nData]);

		if (m_abJump[nData] == true)
		{
			m_pEgg[1]->Jump(EGGJUMP);
		}
	}
	if (m_nNumEgg >= 3)
	{
		// �O��
		int nData = m_nCntFrame - EGG_POS * 3;

		if (nData < 0)
		{
			nData += MAX_FRAME;
		}

		m_pEgg[2]->SetPosition(D3DXVECTOR3((sinf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * (3 + m_nNumChick)) + m_pos.x,
			m_pEgg[2]->SetHeight(),
			(cosf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * (3 + m_nNumChick)) + m_pos.z));

		m_pEgg[2]->SetRot(m_OldEggRot[nData]);

		if (m_abJump[nData] == true)
		{
			m_pEgg[2]->Jump(EGGJUMP);
		}
	}

	if (m_nNumChick >= 1)
	{// ������̎�
		if (m_pChick[0]->GetState() == CChick::STATE_CHASE)
		{
			// �O�̌�������
			int nData = m_nCntFrame - EGG_POS;

			if (nData < 0)
			{
				nData += MAX_FRAME;
			}

			// ���̈ʒu�ݒ�
			m_pChick[0]->SetPosition(D3DXVECTOR3((sinf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE) + m_pos.x,
				m_pChick[0]->SetHeight(),
				(cosf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE) + m_pos.z));

			// ���̊p�x�ݒ�
			m_pChick[0]->SetRot(m_OldEggRot[nData]);

			if (m_abJump[nData] == true)
			{
				m_pChick[0]->Jump();
			}
		}
	}
	if (m_nNumChick >= 2)
	{// �Ђ悱����C�̎�
	 // ��C��
		int nData = m_nCntFrame - EGG_POS * 2;

		if (nData < 0)
		{
			nData += MAX_FRAME;
		}

		m_pChick[1]->SetPosition(D3DXVECTOR3((sinf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * 2) + m_pos.x,
			m_pChick[1]->SetHeight(),
			(cosf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * 2) + m_pos.z));

		m_pChick[1]->SetRot(m_OldEggRot[nData]);

		if (m_abJump[nData] == true)
		{
			m_pChick[1]->Jump();
		}
	}
	if (m_nNumChick >= 3)
	{
		// ��C��
		int nData = m_nCntFrame - EGG_POS * 3;

		if (nData < 0)
		{
			nData += MAX_FRAME;
		}

		m_pChick[2]->SetPosition(D3DXVECTOR3((sinf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * 3) + m_pos.x,
			m_pChick[2]->SetHeight(),
			(cosf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * 3) + m_pos.z));

		m_pChick[2]->SetRot(m_OldEggRot[nData]);

		if (m_abJump[nData] == true)
		{
			m_pChick[2]->Jump();
		}
	}
}

//=============================================================================
// �����ˏ���
//=============================================================================
void CPlayer::BulletEgg(void)
{
	if (m_nNumEgg + m_nNumChick > 0)
	{// �����Ђ悱�������Ă���Ƃ�
		if (m_pChick[0] != NULL && m_pChick[0]->GetState() == CChick::STATE_CHASE)
		{
			m_pChick[0]->SetState(CChick::STATE_BULLET);	// ��Ԃ�e�ɂ���
			m_pChick[0]->SetRank(CGame::GetRanking(m_nPlayerNum));

			m_nNumChick--;	// �����������炷

			if (m_pChick[0]->GetType() == CChick::TYPE_SPEED)
			{
				m_State = PLAYERSTATE_SPEEDUP;
				m_fSpeed += SPEED;

				m_pChick[0]->Uninit();
				m_pChick[0] = NULL;
			}

			// ������ւ�
			m_pChick[0] = m_pChick[1];
			m_pChick[1] = m_pChick[2];
			m_pChick[2] = NULL;
		}
		else if (m_pEgg[0] != NULL && m_pEgg[0]->GetState() == CEgg::EGGSTATE_CHASE)
		{// ��ڂ̗��ɏ�񂪓����Ă��āA�v���C���[�ɂ��Ă��鎞
			m_pEgg[0]->SetState(CEgg::EGGSTATE_BULLET);	// ��Ԃ�e�ɂ���
			m_pEgg[0]->SetRank(CGame::GetRanking(m_nPlayerNum));

			m_nNumEgg--;	// �����������炷

			switch (m_pEgg[0]->GetType())
			{
				// �U��
			case CEgg::EGGTYPE_ATTACK:
				m_pEgg[0]->Jump(THROW);
				break;

				// ����
			case CEgg::EGGTYPE_SPEED:
				m_State = PLAYERSTATE_SPEEDUP;
				m_fSpeed += SPEED;

				m_pEgg[0]->Uninit();
				m_pEgg[0] = NULL;
				break;
			}

			// ������ւ�
			m_pEgg[0] = m_pEgg[1];
			m_pEgg[1] = m_pEgg[2];
			m_pEgg[2] = NULL;
		}
	}
}

//=============================================================================
// ���Ƃ̓����蔻��
//=============================================================================
void CPlayer::CollisionEgg(void)
{
	CSound *pSound = CManager::GetSound();
	CScene *pScene;

	for (int nCntPriority = 2; nCntPriority <= EGG_PRIOTITY; nCntPriority++)
	{
		// �v���C�I���e�B�[�`�F�b�N
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �v���C�I���e�B�[���̃��X�g�\�����Ō�܂Ō���
			CScene *pSceneNext = pScene->GetNext();		// ���̃I�u�W�F�N�g��ۑ�

			if (pScene->GetObjType() == OBJTYPE_EGG)
			{// �^�C�v����Q����������
				CEgg *pEgg = (CEgg*)pScene;	// �I�u�W�F�N�g�N���X�̃|�C���^�ϐ��ɂ���

				if (pEgg->GetState() == CEgg::EGGSTATE_BULLET)
				{
					if (pEgg->GetRank() != CGame::GetRanking(m_nPlayerNum))
					{
						if (pEgg->CollisionEgg(&m_pos, &m_OldPos) == true)
						{// �Փ˂���
							switch (pEgg->GetType())
							{
								// �U��
							case CEgg::EGGTYPE_ATTACK:
								// �_���[�W��Ԃɂ���
								if (m_bDamage == false)
								{
									m_bDamage = true;
									m_State = PLAYERSTATE_DAMAGE;
								}
								pEgg->Uninit();	// ���폜
								break;

								// ����
							case CEgg::EGGTYPE_ANNOY:
								if (m_bDamage == false)
								{
									m_bDamage = true;
									m_State = PLAYERSTATE_SPEEDDOWN;
								}
								pEgg->Uninit();	// ���폜
								break;
							}
						}
					}
				}
			}
			if (pScene->GetObjType() == OBJTYPE_CHICK)
			{
				CChick *pChick = (CChick*)pScene;	// �I�u�W�F�N�g�N���X�̃|�C���^�ϐ��ɂ���

				if (pChick->GetState() == CChick::STATE_BULLET)
				{
					if (pChick->GetRank() != CGame::GetRanking(m_nPlayerNum))
					{
						if (pChick->CollisionChick(&m_pos, &m_OldPos) == true)
						{// �Փ˂���
							switch (pChick->GetType())
							{
								// �U��
							case CChick::TYPE_ATTACK:
								// �_���[�W��Ԃɂ���
								if (m_bDamage == false)
								{
									m_bDamage = true;
									m_State = PLAYERSTATE_DAMAGE;
								}
								pChick->Uninit();	// �Ђ悱�폜
								break;

								// ����
							case CChick::TYPE_ANNOY:
								if (m_bDamage == false)
								{
									m_bDamage = true;
									m_State = PLAYERSTATE_SPEEDDOWN;
								}
								pChick->Uninit();	// �Ђ悱�폜
								break;
							}
						}
					}
				}
			}

			// Next�Ɏ���Scene������
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// �Ђ悱�̏o������
//=============================================================================
void CPlayer::ChickAppear(void)
{
	if (m_pEgg[0] != NULL)
	{
		if (m_pEgg[0]->GetHatchingTimer() > HATCHING_TIME)
		{// �z�����鎞�Ԃ��o��
		 // �^�C�}�[��0�ɂ��ǂ�
			m_pEgg[0]->SetHatchingTimer(0);

			if (m_pEgg[0] != NULL)
			{
				switch (m_pEgg[0]->GetType())
				{
					// �U��
				case CEgg::EGGTYPE_ATTACK:
					m_pChick[m_nNumChick] = CChick::Create(m_pos,
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						CHICK_SCALE,
						CChick::TYPE_ATTACK,
						CChick::BULLETTYPE_PLAYER,
						m_nPlayerNum);
					break;

					// ����
				case CEgg::EGGTYPE_ANNOY:
					m_pChick[m_nNumChick] = CChick::Create(m_pos,
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						CHICK_SCALE,
						CChick::TYPE_ANNOY,
						CChick::BULLETTYPE_PLAYER,
						m_nPlayerNum);
					break;

					// ����
				case CEgg::EGGTYPE_SPEED:
					m_pChick[m_nNumChick] = CChick::Create(m_pos,
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						CHICK_SCALE,
						CChick::TYPE_SPEED,
						CChick::BULLETTYPE_PLAYER,
						m_nPlayerNum);
					break;
				}

				m_pEgg[0]->Uninit();
				m_pEgg[0] = m_pEgg[1];
				m_pEgg[1] = m_pEgg[2];
				m_pEgg[2] = NULL;
				m_nNumEgg--;

				// �Ђ悱�̏�Ԃ�ݒ�
				m_pChick[m_nNumChick]->SetState(CChick::STATE_CHASE);

				m_nNumChick++;
			}
		}
	}
}

//=============================================================================
// �L�������m�̏Փ˔��菈��
//=============================================================================
void CPlayer::CollisionCharacter(void)
{
	CPlayer **pPlayer = CGame::GetPlayer();

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// �v���C���[�J�E���g
		if (pPlayer[nCntPlayer] != NULL)
		{// NULL�ȊO
			if (pPlayer[nCntPlayer] != this)
			{// �����ȊO
				// �����v�Z
				D3DXVECTOR3 pos = pPlayer[nCntPlayer]->GetPos();
				float fLenght = sqrtf(powf(pos.x - m_pos.x, 2.0f) + powf(pos.z - m_pos.z, 2.0f));

				if (fLenght < 80.0f)
				{// �͈͓�
					// �p�x�v�Z
					float fAngle = atan2f(m_pos.x - pos.x, m_pos.z - pos.z);

					// �ʒu����
					m_pos = pos + D3DXVECTOR3(sinf(fAngle) * 80.0f, 0.0f, cosf(fAngle) * 80.0f);
				}
			}
		}
	}
}