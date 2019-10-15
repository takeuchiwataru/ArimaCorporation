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
#include "tire.h"
#include "wall.h"
#include "incline.h"
#include "sound.h"
#include "tutorial.h"
#include "gamecamera.h"
#include "object.h"
#include "feed.h"

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
CModel * CPlayer::m_pModel = NULL;	//���f���̃p�[�c�|�C���^
int	CPlayer::m_nMaxModel = 0;
int CPlayer::m_nMaxParts = 0;
int CPlayer::m_nMaxMotion = 0;
D3DXVECTOR3 CPlayer::m_pos = VECTOR_ZERO;
CMotion::MOTION_INFO * CPlayer::m_pMotionInfo = NULL;
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;

//=============================================================================
// ��������
//=============================================================================
CPlayer * CPlayer::Create(const D3DXVECTOR3 pos)
{
	//�C���X�^���X�̐���
	CPlayer * pPlayer;
	pPlayer = new CPlayer;

	//����������
	pPlayer->Init();

	//�ݒ菈��
	pPlayer->Set(pos, VECTOR_ZERO);
	return pPlayer;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPlayer::CPlayer() : CScene(3, OBJTYPE_PLAYER) {}

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
	m_rot = VECTOR_ZERO;				//����
	m_OldDiffuse = VECTOR_ZERO;				//�O��̍���
	m_MoveMotion = VECTOR_ZERO;				//���[�V�����̈ړ�
	m_vtxMaxModel = VECTOR_ZERO;			//���f���̒��_�ő�l
	m_vtxMinModel = VECTOR_ZERO;			//���f���̒��_�ŏ��l
	m_pMotion = NULL;						//���[�V�����|�C���^
	m_pText = NULL;							//�v���C���[�̃e�L�X�g�|�C���^
	m_state = STATE_DRIVE;					//��Ԑݒ�
	m_StateSpeed = STATE_SPEED_STOP;		//�X�s�[�h�̏�Ԑݒ�
	m_StateHandle = HANDLE_MAX;				//�n���h���̏��
	m_PlayerInfo.nCountTime = 0;			//�J�E���^�[
	m_PlayerInfo.fAccel = 1.0f;				//�����l
	m_PlayerInfo.fBraks = 0.00f;			//�����l
	m_PlayerInfo.fAddRot = 0.00f;			//�������]�l
	m_PlayerInfo.fDistance = 0.0f;			//����
	m_PlayerInfo.fWindy = 0.0f;				//����
	m_PlayerInfo.FirstPos = VECTOR_ZERO;	//�����ʒu
	m_bJump = false;						//�W�����v���
	m_bControl = true;						//�R���g���[���[�t���O
	m_nCountJumpTime = 0;					//�W�����v��Ԃ̎��Ԃ����Z
	m_fvtxMaxY = 0.0f;						//���f�����_�̍ő�l�iY�j
	m_fMass = 200.0f;						// ����
	m_nCountTime = 0;						// ���ԃJ�E���^�[
	m_bCrcleCarIn = false;
	m_pLoadEffect = NULL;					// �G�t�F�N�g�c�[���|�C���^
	m_nCntCombo = 0;
	m_nCntShake = 0;
	m_bShake = true;
	m_nCntFlag = 0;
	m_bDirive = true;

	//�^�C���̃|�C���^��������
	for (int nCntTire = 0; nCntTire < MAX_TIRE; nCntTire++)
	{
		m_pTire[nCntTire] = NULL;
	}

	CreateTire();			//�^�C���̐���

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
	//���[�V�����̔j��
	if (m_pMotion != NULL)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = NULL;
	}

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
	}

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

		TirePos[nCntTire] = m_pTire[nCntTire]->GetWorldPos();
	}

	CarCalculate(TirePos);	// �ԑ̂̍����E�p�x�ݒ�

	//�^�C���A�b�v��ԂȂ�ȍ~�͍X�V���Ȃ�
	//if (CTime::GetTime() == 0 && CManager::MODE_GAME == CManager::GetMode()) { return; }

	UpdateMove();			// �ړ�����
	/*UpdateStateJump();		// �W�����v��Ԃ̍X�V����
	RiverInfluence();		// ��ɂ��e��

	CollisionObject();		// �I�u�W�F�N�g�Ƃ̓����蔻��
	UpdateShake();			//�Ԃ̗h��̏���
	//CollisitionWall();	// �ǂƂ̓����蔻��
	UpdateSmoke(TirePos);	//���̍X�V����
	UpdateGrass(TirePos);	//���G�t�F�N�g�̍X�V����
	*/
	/*m_nCntCombo++;

	// �R���{��
	if (m_pCombo != NULL && m_pCombo->GetFream() == 0)
	{	// NULL�`�F�b�N & �t���[����0�̏ꍇ
		m_nCntCombo = 0;
		m_pCombo->Uninit();
		m_pCombo = NULL;
	}*/

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

	//�^�C���̕`��
	for (int nCntTire = 0; nCntTire < MAX_TIRE; nCntTire++)
	{
		if (m_pTire[nCntTire] != NULL) { m_pTire[nCntTire]->Draw(1.0); }
	}
}

//=============================================================================
// �R���g���[���L�[
//=============================================================================
void CPlayer::ControlKey(void)
{
	CInputKeyBoard * pInputKeyboard = CManager::GetInput();		//�L�[�{�[�h�̎擾
	CXInput * pInputJoypad = CManager::GetXInput();				//�W���C�p�b�g�̎擾

	//�O�i��ނ̐ݒ�
	if (m_bDirive)
	{
		if ((pInputKeyboard->GetKeyboardPress(DIK_L) == true) ||
			(pInputJoypad->GetPress(CXInput::XIJS_BUTTON_15) == true) ||
			(pInputJoypad->GetPress(CXInput::XIJS_BUTTON_9) == true))
		{
			SetState(STATE_DRIVE);		//�O�i��Ԃɐݒ�
		}
		if ((pInputKeyboard->GetKeyboardPress(DIK_K) == true) ||
			(pInputJoypad->GetPress(CXInput::XIJS_BUTTON_14) == true) ||
			(pInputJoypad->GetPress(CXInput::XIJS_BUTTON_8) == true))
		{
			SetState(STATE_REVERSE);	//��ޏ�Ԃɐݒ�
		}
	}

	//�����̐ݒ�
	if (m_StateSpeed != STATE_SPEED_STOP)
	{
		if ((pInputKeyboard->GetKeyboardPress(DIK_A) == true) || pInputJoypad->GetPress(CXInput::XIJS_BUTTON_18))
		{ //���n���h�����
			SetStateHandle(HANDLE_LEFT);
		}
		else if ((pInputKeyboard->GetKeyboardPress(DIK_D) == true) || pInputJoypad->GetPress(CXInput::XIJS_BUTTON_19))
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
		(pInputJoypad->GetPress(CXInput::XIJS_BUTTON_14) == true) ||
		(pInputJoypad->GetPress(CXInput::XIJS_BUTTON_8) == true) ||
		(pInputKeyboard->GetKeyboardPress(DIK_L) == true) ||
		(pInputJoypad->GetPress(CXInput::XIJS_BUTTON_15) == true) ||
		(pInputJoypad->GetPress(CXInput::XIJS_BUTTON_9) == true))
	{ //�A�N�Z�������
		SetStateSpeed(STATE_SPEED_ACCEL);
	}
	else
	{//�������
		SetStateSpeed(STATE_SPEED_DOWN);
	}

	CSound *pSound = CManager::GetSound();

	if ((pInputKeyboard->GetKeyboardPress(DIK_N) == true) || (pInputJoypad->GetPress(CXInput::XIJS_BUTTON_0) == true))
	{
		pSound->SetVolume(CSound::SOUND_LABEL_SE_KLAXON, 1.5f);
		pSound->PlaySound(CSound::SOUND_LABEL_SE_KLAXON);
	}
}

//=============================================================================
// �ړ�����
//=============================================================================
void CPlayer::UpdateMove(void)
{
	m_OldPos = m_pos;	//�O��̈ʒu��ۑ�����

	RemakeAngle(&m_rot.y);


	//��Ԃ��Ƃ̍X�V����
	switch (m_StateSpeed)
	{
	case STATE_SPEED_ACCEL:	//�A�N�Z�����

		//�W�����v��ԂȂ�
		if (m_bJump == true) { break; }

			CDebugProc::Print("������������\n");
			//���x���グ��
			m_PlayerInfo.nCountTime++;
			if (m_move.x < 3.0f && m_move.z < 3.0f &&m_move.x > -3.0f && m_move.z > -3.0f)
			{
				m_PlayerInfo.nCountTime = m_PlayerInfo.nCountTime*1.03f;
				CDebugProc::Print("����UP��� [1�i�K]\n");
			}
			else if (m_move.x < 6.0f && m_move.z < 6.0f &&m_move.x > -6.0f && m_move.z > -6.0f)
			{
				m_PlayerInfo.nCountTime = m_PlayerInfo.nCountTime*1.04f;
				CDebugProc::Print("����UP��� [2�i�K]\n");
			}
			else if (m_move.x < 9.0f && m_move.z < 9.0f &&m_move.x > -9.0f && m_move.z > -9.0f)
			{
				m_PlayerInfo.nCountTime = m_PlayerInfo.nCountTime*1.065f;
				CDebugProc::Print("����UP��� [3�i�K]\n");
			}

			//�����l�����߂�
			m_PlayerInfo.fAccel = (m_fMaxSpeed / ACCLE_TIME) * m_PlayerInfo.nCountTime;

			//����������
			if (m_state == STATE_DRIVE)
			{
				if (m_PlayerInfo.fAccel > m_fMaxSpeed) { m_PlayerInfo.fAccel = m_fMaxSpeed; }
			}
			else if (m_state == STATE_REVERSE)
			{
				if (m_PlayerInfo.fAccel < m_fMaxSpeed) { m_PlayerInfo.fAccel = m_fMaxSpeed; }
			}


		//�i�s�����̐ݒ�
		m_move.x += sinf(m_rot.y) * (m_PlayerInfo.fAccel);
		m_move.z += cosf(m_rot.y) * (m_PlayerInfo.fAccel);
		break;

	case STATE_SPEED_BRAKS: //�u���[�L���

		//�W�����v��ԂȂ�
		if (m_bJump == true) { break; }

		//���x��������
		m_PlayerInfo.fAccel -= m_PlayerInfo.fBraks;

		if (m_state == STATE_DRIVE)
		{
			if (m_PlayerInfo.fAccel < 0.0f) { m_PlayerInfo.fAccel = 0.0f; }
		}
		else
		{
			if (m_PlayerInfo.fAccel > 0.0f) { m_PlayerInfo.fAccel = 0.0f; }
		}

		//�i�s�����̐ݒ�
		m_move.x += sinf(m_rot.y) * m_PlayerInfo.fAccel;
		m_move.z += cosf(m_rot.y) * m_PlayerInfo.fAccel;

		//�h��𖳌��ɂ���
		m_bShake = false;
		break;

	case STATE_SPEED_DOWN:  //�������
		m_PlayerInfo.nCountTime = 0;
		break;
	}

	CDebugProc::Print("�A�N�Z�� : %1f\n", m_PlayerInfo.fAccel);
	CDebugProc::Print("�X�s�[�h : %1f  %1f  %1f\n", m_move.x , m_move.y , m_move.z);

	//�n���h���̏�ԍX�V
	if (m_StateHandle == HANDLE_LEFT)
	{
		m_rot.y -= m_PlayerInfo.fAddRot;
		m_rot.z -= SHAKE_Z;
	}
	else if (m_StateHandle == HANDLE_RIGHT)
	{
		m_rot.y += m_PlayerInfo.fAddRot;
		m_rot.z += SHAKE_Z;
	}

	// �d��
	if (m_bShake == true)
	{
		m_move.y -= cosf(0) * 0.4f;
	}

	//���S���W�X�V
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;
	m_pos.z += m_move.z;

	//����
	m_PlayerInfo.fAccel += (0.0f - m_PlayerInfo.fAccel) * m_PlayerInfo.fWindy;
	if (!m_bJump)
	{
		m_move.x += (0.0f - m_move.x) * m_PlayerInfo.fWindy;
		m_move.z += (0.0f - m_move.z) * m_PlayerInfo.fWindy;
	}

	//��~����
	D3DXVECTOR3 fDiffuse = m_pos - m_OldPos;

	if (fDiffuse.x < 0.10f && fDiffuse.x > -0.10f)
	{
		if (fDiffuse.z < 0.10f && fDiffuse.z > -0.10f)
		{
			SetStateSpeed(STATE_SPEED_STOP);
		}
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

	if ((m_StateSpeed == STATE_SPEED_BRAKS))
	{
		//�p�x��ς���
		m_rot.x += SHAKE_BRAK;
	}
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

		if (((m_nCntShake % 24) == 0) && (m_state != STATE_SPEED_STOP))
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
// ��Ԃ̐ݒ�
//=============================================================================
void CPlayer::SetState(CPlayer::STATE state)
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
		}
		else if (STATE_DRIVE == state)
		{
			pSound->StopSound(CSound::SOUND_LABEL_SE_BACK);
			pSound->SetVolume(CSound::SOUND_LABEL_SE_ACCEL, 0.7f);
			pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ACCEL);
		}
	}

	//��Ԃ̐ݒ�
	m_state = state;
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

		if (STATE_SPEED_ACCEL == state && STATE_DRIVE == m_state)
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
		}
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
// �^�C���̐���
//=============================================================================
void CPlayer::CreateTire(void)
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
// �e�L�X�g�̓ǂݍ���
//=============================================================================
void CPlayer::LoadText(void)
{
	if (m_pText == NULL) { m_pText = CLoadTextPlayer::Create(FAILE_NAME); }	//�v���C���[�̏���ǂݍ���
	m_PlayerInfo = m_pText->GetPlayerInfo();								//�ǂݍ��񂾏����擾����

	//�v���C���[����ݒ肷��
	m_pos = m_PlayerInfo.FirstPos;
	m_fMaxSpeed = m_PlayerInfo.fAccel;	//�A�N�Z���ő�l�̎擾
	m_PlayerInfo.fAccel = 0.0f;
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
				int nType = pObject->GetType();		// ��Q���̎�ނ��擾
				if (nType != 19)
				{// �ǈȊO
					if (pObject->Collision(m_pos, m_pModel[0].GetVtxMax(), m_pModel[0].GetVtxMin(), m_move))
					{// �Փ˂���
						for (int nCntObj = 0; nCntObj < UPDATE_TYPE_NUM + HIGHT_OBJ_NUM + 2; nCntObj++) {//�^�C�v����
							if (nType == anUpdateType[nCntObj])
							{
								bType = true;
								break;
							}
						}

						if (bType == false)
						{// �w�肵����Q���������琁����΂�
							if (!m_bJump) { m_move = pObject->BlowOff(m_pos, m_move, m_fMass); }		// �I�u�W�F�N�g�𐁂���΂�

							//���̍Đ�����
							//PlaySoundObj(nType, pSound);
						}
						if (nType == 30 || nType == 33)
						{// ���� || ���ΐ�
							m_move = pObject->Fountain(m_pos, m_move);
							m_bJump = true;
							m_pos.y += m_move.y;
						}
					}
				}
				else if (19 == nType)
				{// ��
					if (pObject->CollisionObject(&m_pos, &m_OldPos, &m_move))
					{
					}
				}
			}

			// Next�Ɏ���Scene������
			pScene = pSceneNext;
		}
	}

	//�f�o�b�N�\��
	if (m_bCrcleCarIn == true)
	{
		CDebugProc::Print("�����Ă���\n");
	}
	else if (m_bCrcleCarIn == false)
	{
		CDebugProc::Print("�����Ă��Ȃ�\n");
	}
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

	//�^�C���̍����̍��v�l�����߂�
	for (int nCntTire = 0; nCntTire < MAX_TIRE; nCntTire++)
	{
		fTireHight += TirePos[nCntTire].y - m_pTire[0]->TIRE_HIGHT;
	}

	//�ԑ̂̍��������߂�
	for (int nCntTire = 0; nCntTire < MAX_TIRE; nCntTire++)
	{
		if (true == m_pTire[nCntTire]->GetLand())
		{
			fHight = fTireHight / (float)MAX_TIRE;
			m_pos.y = fHight;
			bDecisionAngle = true;
			break;
		}
	}

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

		//X���̊p�x����
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
		}
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
	if (m_state == STATE_DRIVE)
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

	//�ʒu�\��
	CDebugProc::Print("�ʒu : X %.2f, Y %.2f, Z %.2f\n", m_pos.x, m_pos.y, m_pos.z);

	//�^�C���̈ʒu
	for (int nCntTire = 0; nCntTire < MAX_TIRE; nCntTire++)
	{
		CDebugProc::Print("�^�C��[ %d ] : X %.2f, Y %.2f, Z %.2f\n", nCntTire, m_pTire[nCntTire]->GetPos().x, m_pTire[nCntTire]->GetPos().y, m_pTire[nCntTire]->GetPos().z);
	}

	CDebugProc::Print("�W�����v�F%s\n", m_bJump ? "�Z" : "�~");
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

				//if (pFeed->Collision(m_pos, m_pModel[0].GetVtxMax(), m_pModel[0].GetVtxMin(), m_move))
				//{// �Փ˂���



				//}
			}

			// Next�Ɏ���Scene������
			pScene = pSceneNext;
		}
	}
}