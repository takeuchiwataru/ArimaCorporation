//=============================================================================
//
// �v���C���[�̏��� [player.cpp]
// Author : ���r�S
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
#include "ColMesh.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define VECTOR_ZERO		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))				//�x�N�g���̏�����
#define FAILE_NAME		("data\\TEXT\\Player\\PlayerState.txt")		//�ǂݍ��ރt�@�C����
#define FILE_TEXTURE	("data\\TEXTURE\\modeltex\\�j���g��.jpg")	//�e�N�X�`���̓ǂݍ���
#define AVERAGE			(2.0f)										//���ς���l
#define ROLLOVER_STOP	(0.6f)										//���]�h�~�p�x
#define GRAVITY_BACK	(0.2f)										//����̏d��
#define DECELERATION	(0.5f)										//�����̊���
#define EGG_RANGE		(50.0f)										// ���ƃv���C���[�̋���
#define EGG_POS			(7)											// �����m�̊Ԋu�̍L���i���₷�ƍL���Ȃ�j
#define SPEEDUP_TIME	(60)										// �������Ă��鎞��
#define SPEEDDOWN		(0.95f)										// ����������l
#define CHICK_SCALE		(D3DXVECTOR3(1.0f, 1.0f, 1.0f))				//�Ђ悱�̑傫��
#define THROW			(13.0f)										// ���𓊂����
#define EGG_RAND		(2.0f)										// ���ɏ��Ƃ��̃W�����v��
#define EGG_HEIGHT		(40.0f)										// ���ɏ�����悤�Ɍ����鍂��

// �v���C���[���
#define PLAYER_ACCEL	(0.5f)										//�����l�i�O�i�j
#define PLAYER_BRAKS	(-0.2f)										//�����l�i��i�j
#define PLAYER_DOWN		(0.08f)										//�����x
#define PLAYER_ADDROT	(0.005f)									//��]��
#define PLAYER_DOWNROT	(0.2f)										//��]��

#define PLAYER_JUMP		(2.0f)										//��]��
#define PLAYER_GRAVITY	(0.09f)										//��]��

//�ԑ̂̊p�x
#define SHAKE_X			(0.007f)									//X���̗h��
#define INPULS_X		(0.01f)										//X���ɉ�����p�x
#define SHAKE_Z			(0.02f)										//Z���̗h��
#define SHAKE_DRIFT		(0.005f)									//�h���t�g���̊p�x���Z
#define SHAKE_BRAK		(0.01f)										//�u���[�L���̊p�x

#define RIVER_SOUND_RANGE		(1000.0f)							// ��̉�����������͈�
#define FOUNTAIN_SOUND_RANGE	(1000.0f)							// �����̉�����������͈�

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
//CModel * CPlayer::m_pModel = NULL;		//���f���̃p�[�c�|�C���^
//int	CPlayer::m_nMaxModel = 0;
//int CPlayer::m_nMaxParts = 0;
//int CPlayer::m_nMaxMotion = 0;
//CMotion::MOTION_INFO * CPlayer::m_pMotionInfo = NULL;
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;
CChick *CPlayer::m_pAnnoyChick[MAX_MEMBER] = {};

LPD3DXMESH	CPlayer::m_pMesh[MAX_PARTS] = {};					//���b�V�����̏�����
LPD3DXBUFFER CPlayer::m_pBuffMat[MAX_PARTS] = {};				//�}�e���A���̏��̏�����
DWORD CPlayer::m_nNumMat[MAX_PARTS] = {};						//�}�e���A���̏�񐔂̏�����

//--------------------------------------------
//�O���[�o���ϐ�
//--------------------------------------------
int g_nNumModel;
char g_aFileNameModel[MAX_PARTS][256];

//=============================================================================
// ��������
//=============================================================================
CPlayer * CPlayer::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, int nPlayerNum, int nControllerNum, PLAYERTYPE playerType)
{
	//�C���X�^���X�̐���
	CPlayer * pPlayer;
	pPlayer = new CPlayer;

	//����������
	pPlayer->Init();
	pPlayer->m_nPlayerNum = nPlayerNum;
	pPlayer->m_nControllerNum = nControllerNum;
	pPlayer->m_PlayerType = playerType;

	//�ݒ菈��
	pPlayer->Set(pos, VECTOR_ZERO);
	pPlayer->SetRot(rot);

	return pPlayer;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPlayer::CPlayer() : CScene(3, OBJTYPE_PLAYER)
{
	//�l�̏�����
	for (int nCount = 0; nCount < MAX_PARTS; nCount++)
	{
		m_apModel[nCount] = NULL;
	}

	m_pPlayerNum = NULL;
	m_pPlayerpos = NULL;
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
	//CLoadTextMotion * pLoadTextMotion = NULL;
	CManager::MODE mode = CManager::GetMode();

	//pLoadTextMotion = CTutorial::GetPlayerMotion();

	//m_pMotionInfo = pLoadTextMotion->GetMotionInfo();					//���[�V�������̎擾
	//m_nMaxMotion = pLoadTextMotion->GetMaxMotion();						//���[�V�����̍ő吔�̎擾

	//																	//���f�������擾
	//CLoadTextMotion::MODEL_INFO ModelInfo = pLoadTextMotion->GetModelInfo();
	//m_nMaxModel = ModelInfo.nMaxModel;	//���f���̍ő吔�̎擾
	//m_nMaxParts = ModelInfo.nMaxParts;	//���f���̃p�[�c�ő吔�̎擾

	//									//���f���̐���
	//for (int nCntParts = 0; nCntParts < ModelInfo.nMaxParts; nCntParts++)
	//{
	//	m_pModel = CModel::Create(ModelInfo.pOfSetPos[nCntParts], &ModelInfo.paFileName[nCntParts][0]);

	//	//�e�N�X�`���̊�����
	//	if (m_pModel != NULL) { m_pModel->BindTexture(m_pTexture); }
	//}

	////���f���̐e�ݒ�
	//for (int nCntParts = 0; nCntParts < ModelInfo.nMaxParts; nCntParts++)
	//{
	//	if (nCntParts == 0)
	//	{
	//		m_pModel->SetParent(NULL);
	//		break;
	//	}
	//}
}

//=============================================================================
// X�t�@�C���̔j��
//=============================================================================
void CPlayer::UnloadModel(void)
{
	////���[�V�����f�[�^�̔j��
	//if (m_pMotionInfo != NULL)
	//{
	//	m_pMotionInfo = NULL;
	//}

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
	m_StateSpeed = STATE_SPEED_STOP;		//�X�s�[�h�̏�Ԑݒ�
	m_StateHandle = HANDLE_MAX;				//�n���h���̏��
	m_PlayerInfo.nCountTime = 0;			//�J�E���^�[
	m_PlayerInfo.fAccel = PLAYER_ACCEL;		//�����l�i�O�i�j
	m_PlayerInfo.fBraks = PLAYER_BRAKS;		//�����l�i��i�j
	m_PlayerInfo.fDown = PLAYER_DOWN;		//�����l
	m_PlayerInfo.fAddRot = PLAYER_ADDROT;	//�������]�l
	m_PlayerInfo.fDistance = 0.0f;			//����
	m_PlayerInfo.FirstPos = VECTOR_ZERO;	//�����ʒu
	m_bJump = true;							//�W�����v���
	m_bControl = false;						//�R���g���[���[�t���O
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
	m_nNumItem = 0;
	m_bJumpSave = false;
	m_nCntDamage = 0;
	m_bDamage = false;
	m_State = PLAYERSTATE_NORMAL;
	m_nPlayerRank = 0;
	m_nCntChick = 0;
	m_nDestRank = 0;
	m_nAnnoySTimer = 0;
	m_bAnnoyS = false;

	m_nDriftCounter = 0;		// �h���t�g�J�E���g

	m_pPoint = CRoad_Manager::GetManager()->GetTop(0);
	m_fLength = 5.0f;
	m_bDivided = false;
	m_nMap = 0;
	m_fRoad = 0.0f;
	m_FNor = INIT_VECTOR;
	m_fTilt = 0.0f;
	m_fCTilt = 0.0f;

	m_nPlayerNum = 0;					// �v���C���[�ԍ�
	m_nControllerNum = 0;				// �R���g���[���[�ԍ�

	m_nAnimnow = PLAYERANIM_NEUTRAL;	//�j���[�g�������
	m_nCountFlame = 0;
	m_nKey = 0;

	m_bGoal = false;					// �S�[��
	m_fAddRot = 0.0f;					// ���Z�p�x

	// �v���C���[�ԍ��i�Ǐ]�j
	if (m_pPlayerNum == NULL)
		m_pPlayerNum = CBillBoord::Create(m_pos + D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXVECTOR2(10.0f, 10.0f), 1);

	if (m_pPlayerpos == NULL)
		m_pPlayerpos = CBillBoord::Create(m_pos + D3DXVECTOR3(0.0f, 500.0f, 0.0f), D3DXVECTOR2(300.0f, 300.0f), 0, true);

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

		m_bulletType[nCntEgg] = BULLET_EGG_ATTACK;
	}

	for (int nCntChick = 0; nCntChick < MAX_PLAYER; nCntChick++)
	{// �����{�̃|�C���^��������
		m_pAnnoyChick[nCntChick] = NULL;
	}

	FileLoad();

	//�e�N�X�`���̊�����
	for (int nCount = 0; nCount < MAX_PARTS; nCount++)
	{
		if (m_apModel[nCount] != NULL) { m_apModel[nCount]->BindTexture(m_pTexture); }
	}

	//if (m_pMotion == NULL)	//���[�V�����̐���
	//{
	//	m_pMotion = CMotion::Create();

	//	if (m_pMotion != NULL)
	//	{
	//		//���f���f�[�^�̐ݒ�
	//		m_pMotion->SetModel(&m_pModel);			//���[�V�����f�[�^�Ƀ��f������n��
	//		m_pMotion->SetMaxModel(m_nMaxParts);	//���f���p�[�c�̍ő吔��n��

	//												//���[�V�����f�[�^�̐ݒ�
	//		m_pMotion->SetMotion(m_pMotionInfo);	//���[�V�������̎擾
	//		m_pMotion->SetMaxMotion(m_nMaxMotion);	//���[�V�����̍ő吔�̎擾

	//												//����������
	//		m_pMotion->Init();
	//	}
	//}

	//���f���̍ŏ��l�E�ő�l�̎擾
	//m_vtxMaxModel = m_pModel->GetVtxMax();
	//m_vtxMinModel = m_pModel->GetVtxMin();
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

	if (m_pPlayerpos != NULL)
	{
		m_pPlayerpos->Uninit();
		m_pPlayerpos = NULL;
	}

	////���[�V�����̔j��
	//if (m_pMotion != NULL)
	//{
	//	m_pMotion->Uninit();
	//	delete m_pMotion;
	//	m_pMotion = NULL;
	//}

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

	for (int nCntChick = 0; nCntChick < MAX_PLAYER; nCntChick++)
	{
		if (m_pAnnoyChick[nCntChick] != NULL)
		{
			m_pChick[nCntChick] = NULL;
		}
	}

	for (int nCount = 0; nCount < MAX_PARTS; nCount++)
	{
		if (m_apModel[nCount] != NULL)
		{
			//3D���f����Uninit
			m_apModel[nCount]->Uninit();
			delete m_apModel[nCount];
			m_apModel[nCount] = NULL;
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
		if (m_bGoal == false)
		{
			if (m_PlayerType == PLAYERTYPE_PLAYER)
				ControlKey();
			else
				UpdateAI();
		}
		else
		{

		}
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

	/*CollisionFeed();		// �a�̓����蔻��

	CollisionEgg();			// ���Ƃ̓����蔻��

	CollisionChick();		// �Ђ悱�Ƃ̓����蔻��


	ChaseEgg();				// �������Ă��鏈��

	CollisionCharacter();	// �L�����N�^�[���m�̓����蔻��
	//CollisionObject();		// �I�u�W�F�N�g�Ƃ̓����蔻��

	ChickAppear();

	// ���������Ђ悱��������
	ChaseAnnoyS();*/

							//�}�b�v�Ƃ̓����蔻��
	if (!CCOL_MESH_MANAGER::Collision(m_pos, m_OldPos, m_move, m_fLength, m_FNor, m_bJump, m_nMap, true)) { m_bJump = false; }

	if (m_pPlayerNum != NULL)
	{
		m_pPlayerNum->SetPosSize(m_pos + D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXVECTOR2(10.0f, 10.0f));

		if (m_PlayerType == CPlayer::PLAYERTYPE_PLAYER)
			m_pPlayerNum->SetTexture(m_nPlayerNum, 5, 1, 1);
		else
			m_pPlayerNum->SetTexture(4, 5, 1, 1);
	}

	if (m_pPlayerpos != NULL)
	{
		m_pPlayerpos->SetPosSize(m_pos + D3DXVECTOR3(0.0f, 500.0f, 0.0f), D3DXVECTOR2(300.0f, 300.0f));

		if (m_PlayerType == PLAYERTYPE_PLAYER)
			m_pPlayerpos->SetColor(D3DXCOLOR((m_nPlayerNum % 2 == 0 ? 1.0f : 0.0f), (m_nPlayerNum / 2 == 1 ? 1.0f : 0.0f), (m_nPlayerNum == 1 ? 1.0f : 0.0f), 1.0f));
		else
			m_pPlayerpos->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	m_nMotionType = m_nAnimnow;

	//���[�V�����X�V
	UpdateMotion();

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

	for (int nCount = 0; nCount < MAX_PARTS; nCount++)
	{//���f���̕`��
		if (m_apModel[nCount] != NULL)
		{
			//�`�悷��
			m_apModel[nCount]->Draw(1.0f);
		}
	}
}
//=============================================================================
// AI�̍X�V����
//=============================================================================
void CPlayer::UpdateAI(void)
{
	CGame::GetGameCamera(m_nPlayerNum);
	m_fRoad = m_nPlayerNum * (140.0f / 8.0f) - 70.0f;
	bool bGoal = false;
	float fRot = CRoad_Pointer::NextPoint(m_pos, m_pPoint, m_nPlayerNum, m_fRoad, m_bDivided, bGoal, m_nMap);
	fRot = fRot - m_rot.y;
	RemakeAngle(&fRot);

	m_rot.y += fRot * 0.05f;
	RemakeAngle(&m_rot.y);
	SetStateSpeed(STATE_SPEED_ACCEL);

	if (bGoal == true)
	{
		m_nAnimnow = PLAYERANIM_NEUTRAL;
		m_bGoal = true;
		m_move *= 0.0f;
		SetStateHandle(HANDLE_MAX);
		SetStateSpeed(STATE_SPEED_DOWN);
		return;
	}
}
//=============================================================================
// �R���g���[���L�[
//=============================================================================
void CPlayer::ControlKey(void)
{
	// �擾
	bool bOnline = CTitle::GetOnline();

	CSound *pSound = CManager::GetSound();

	CInputKeyBoard * pInputKeyboard = CManager::GetInput();		//�L�[�{�[�h�̎擾
	CInputJoyPad_0 * pXpad = CManager::GetInputJoyPad0(m_nControllerNum);		//�W���C�p�b�g�̎擾

	bool bGoal = false;
	CRoad_Pointer::NextPoint(m_pos, m_pPoint, m_nPlayerNum, m_fRoad, m_bDivided, bGoal, m_nMap);

	if (bGoal == true)
	{
		m_bGoal = true;
		m_move *= 0.0f;
		SetStateHandle(HANDLE_MAX);
		SetStateSpeed(STATE_SPEED_DOWN);
		m_nAnimnow = PLAYERANIM_NEUTRAL;
		return;
	}

	CDebugProc::Print("�ʒu : X %.2f, Y %.2f, Z %.2f\n", m_pos.x, m_pos.y, m_pos.z);

	//�����̐ݒ�
	if (m_StateSpeed != STATE_SPEED_STOP)
	{
		if ((bOnline == false &&
			(pInputKeyboard->GetKeyboardPress(DIK_A) == true ||
				pInputKeyboard->GetKeyboardPress(DIK_LEFT) == true)) ||
			pXpad->GetPress(INPUT_LS_L) == true ||
			pXpad->GetPress(INPUT_LEFT) == true)
		{ //���n���h�����
			SetStateHandle(HANDLE_LEFT);
		}
		else if ((bOnline == false &&
			(pInputKeyboard->GetKeyboardPress(DIK_D) == true ||
				pInputKeyboard->GetKeyboardPress(DIK_RIGHT) == true)) ||
			pXpad->GetPress(INPUT_LS_R) == true ||
			pXpad->GetPress(INPUT_RIGHT) == true)
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

	// ����
	if (
		((bOnline == false && pInputKeyboard->GetKeyboardPress(DIK_K) == true) ||
		 (pXpad->GetPress(INPUT_L1) == true) ||
		 (pXpad->GetPress(INPUT_L2) == true)) &&
		((bOnline == false && pInputKeyboard->GetKeyboardPress(DIK_L) == true) ||
		 (pXpad->GetPress(INPUT_R1) == true) ||
		 (pXpad->GetPress(INPUT_R2) == true))		
		)
	{// �h���t�g
		if (m_PlayerInfo.nCountTime < 90)
			SetStateSpeed(STATE_SPEED_ACCEL);
		else
			SetStateSpeed(STATE_SPEED_DRIFT);
	}
	else
	{
		//�A�N�Z��
		if ((bOnline == false && pInputKeyboard->GetKeyboardPress(DIK_K) == true) ||
			(pXpad->GetPress(INPUT_L1) == true) ||
			(pXpad->GetPress(INPUT_L2) == true))
		{//�������
			SetStateSpeed(STATE_SPEED_BRAKS);
		}
		else if
			((bOnline == false && pInputKeyboard->GetKeyboardPress(DIK_L) == true) ||
			(pXpad->GetPress(INPUT_R1) == true) ||
				(pXpad->GetPress(INPUT_R2) == true))
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

		if ((bOnline == false &&
			(pInputKeyboard->GetKeyboardTrigger(DIK_W) == true ||
				pInputKeyboard->GetKeyboardTrigger(DIK_UP) == true)) ||
			pXpad->GetTrigger(INPUT_A) == true)
		{// �W�����v�L�[
			m_bJumpSave = true;
			m_bJump = true;
			m_move.y += PLAYER_JUMP;
		}
	}
	else
	{
		m_bJumpSave = false;
	}

	if (pInputKeyboard->GetKeyboardTrigger(DIK_SPACE) == true ||
		pXpad->GetTrigger(INPUT_B) == true)
	{// �e����
		BulletEgg();
	}

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

	if (m_bJump == false)
	{//�X�Ώ���
		D3DXVECTOR3 Vec2;
		Vec2 = D3DXVECTOR3(sinf(m_rot.y), 0.0f, cosf(m_rot.y));
		m_fTilt = (m_FNor.x * Vec2.x) + (m_FNor.y * Vec2.y) + (m_FNor.z * Vec2.z) /
			(sqrtf(powf(m_FNor.x, 2) + powf(m_FNor.y, 2) + powf(m_FNor.z, 2)) * sqrtf(powf(Vec2.x, 2) + powf(Vec2.y, 2) + powf(Vec2.z, 2)));
		acosf(m_fTilt);
		m_fTilt = (m_fTilt * -1.0f) * 1.0f + 0.1f;
	}
	else { m_fTilt = 0.0f; }
	if (m_fTilt > 0.05f) { m_fTilt = 0.05f; }
	m_fCTilt = m_fTilt;

	RemakeAngle(&m_rot.y);

	//bool bGoal = false;
	//float fRot = CRoad_Pointer::NextPoint(m_pos, m_pPoint, m_nPlayerNum, m_bDivided, bGoal, m_nMap);
	//m_rot.y += (fRot - m_rot.y);
	//if (bGoal) { m_move *= 0.0f; }


	if (m_State == PLAYERSTATE_SPEEDUP)
	{// �X�s�[�h�A�C�e�����g�����Ƃ�
		m_nCountSpeed++;

		if (m_nCountSpeed > SPEEDUP_TIME)
		{
			m_State = PLAYERSTATE_NORMAL;
			m_nCountSpeed = 0;

			// �W�����v
			m_bJumpSave = true;
			m_bJump = true;
			m_move.y += EGG_RAND;
		}
	}

	float fAccel = m_PlayerInfo.fAccel;
	float fBraks = m_PlayerInfo.fBraks;
	float fAddRot = m_PlayerInfo.fAddRot;
	float fDown = m_PlayerInfo.fDown;

	fAddRot *= 0.65f;

	//��Ԃ��Ƃ̍X�V����
	switch (m_StateSpeed)
	{
	case STATE_SPEED_ACCEL:	//�A�N�Z�����

		//���郂�[�V����
		m_nAnimnow = PLAYERANIM_RUN;

		//�W�����v��ԂȂ�
		if (m_bJump == true) { break; }

		if (m_State == PLAYERSTATE_NORMAL)
		{
			m_fSpeed = fAccel * (m_PlayerInfo.nCountTime < 90 ? (m_PlayerInfo.nCountTime / 90) : 1.0f) * (1.0f - m_fTilt * 1.5f);
		}

		//�i�s�����̐ݒ�
		m_move.x += sinf(m_rot.y) * (m_fSpeed);
		m_move.z += cosf(m_rot.y) * (m_fSpeed);
		break;
	case STATE_SPEED_BRAKS: //�u���[�L���

		//�W�����v��ԂȂ�
		if (m_bJump == true) { break; }

		m_fSpeed = fBraks * (m_PlayerInfo.nCountTime < 90 ? (m_PlayerInfo.nCountTime / 90) : 1.0f);

		//�i�s�����̐ݒ�
		m_move.x += sinf(m_rot.y) * m_fSpeed;
		m_move.z += cosf(m_rot.y) * m_fSpeed;

		//�h��𖳌��ɂ���
		m_bShake = false;
		break;
	case STATE_SPEED_DRIFT:	//�h���t�g���

		fAccel *= 0.345f;
		fAddRot = m_PlayerInfo.fAddRot;
		fDown *= 0.35f;

		//���郂�[�V����
		m_nAnimnow = PLAYERANIM_RUN;

		//�W�����v��ԂȂ�
		if (m_bJump == true) { break; }

		if (m_State == PLAYERSTATE_NORMAL)
		{
			m_fSpeed = fAccel * (m_PlayerInfo.nCountTime < 90 ? (m_PlayerInfo.nCountTime / 90) : 1.0f) * (1.0f - m_fTilt * 1.5f);
		}

		//�i�s�����̐ݒ�
		m_move.x += sinf(m_rot.y) * (m_fSpeed);
		m_move.z += cosf(m_rot.y) * (m_fSpeed);

		break;
	case STATE_SPEED_DOWN: //�_�E�����
		m_PlayerInfo.nCountTime = 0;
		//CDebugProc::Print("DWON***\n");

		if (m_State == PLAYERSTATE_SPEEDUP)
		{// �X�s�[�h�A�C�e�����g�����Ƃ�
		 //�i�s�����̐ݒ�
			m_move.x += sinf(m_rot.y) * (m_fSpeed);
			m_move.z += cosf(m_rot.y) * (m_fSpeed);
		}

		break;
	default:
		//���郂�[�V����
		m_nAnimnow = PLAYERANIM_NEUTRAL;
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
			m_fAddRot -= fAddRot * (m_PlayerInfo.nCountTime < 45 ? (m_PlayerInfo.nCountTime / 45) : 1.0f);
		}
	}
	else if (m_StateHandle == HANDLE_RIGHT)
	{
		if (m_StateSpeed != STATE_SPEED_STOP)
		{
			m_fAddRot += fAddRot * (m_PlayerInfo.nCountTime < 45 ? (m_PlayerInfo.nCountTime / 45) : 1.0f);
		}
	}

	// �d��
	//if (m_bShake == true)
	{
		m_move.y -= cosf(0) * PLAYER_GRAVITY;
	}

	//���S���W�X�V
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;
	m_pos.z += m_move.z;

	m_rot.y += m_fAddRot;

	//����
	if (!m_bJump)
	{
		m_move.x += (0.0f - m_move.x) * fDown;
		m_move.z += (0.0f - m_move.z) * fDown;
	}

	m_fAddRot += (0.0f - m_fAddRot) * PLAYER_DOWNROT;

	CDebugProc::Print("addrot : %f\n", m_fAddRot);
	CDebugProc::Print("fSpeed : %f\n", m_fSpeed);
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
					float fEggHeight = 0.0f;

					if (m_State == PLAYERSTATE_SPEEDUP)
					{// ���ɏ�����悤�Ɍ����鍂��
						fEggHeight = EGG_HEIGHT;
					}

					m_pos.y = fHight + fEggHeight;		//�n�ʂ̍������擾
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
// �X�s�[�h�̏�Ԑݒ�
//=============================================================================
void CPlayer::SetStateSpeed(CPlayer::STATE_SPEED state)
{
	//���l�̐ݒ�
	if (m_StateSpeed != state)
	{
		if ((m_StateSpeed == STATE_SPEED_ACCEL && state == STATE_SPEED_DRIFT) ||
			(state == STATE_SPEED_ACCEL && m_StateSpeed == STATE_SPEED_DRIFT))
		{
			m_nDriftCounter = 0;
		}
		else
		{
			m_PlayerInfo.nCountTime = 0;
		}

		//CDebugProc::Print("CHANGE***\n");

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
	//CDebugProc::Print("�ʒu : X %.2f, Y %.2f, Z %.2f\n", m_pos.x, m_pos.y, m_pos.z);
	//CDebugProc::Print("�ړ� : X %.2f, Y %.2f, Z %.2f\n", m_move.x, m_move.y, m_move.z);
	//
	//CDebugProc::Print("�W�����v�F%s\n", m_bJump ? "�Z" : "�~");

	//CDebugProc::Print("�J�E���g : %f\n", m_PlayerInfo.nCountTime);

	//	CDebugProc::Print("�A�C�e���F%d\n", m_nNumItem);
	//	for (int nCount = 0; nCount < MAX_EGG; nCount++)
	//		CDebugProc::Print("�A�C�e����ށF%d\n", m_bulletType[nCount]);

	//CDebugProc::Print("�J�E���g�F%f\n", m_PlayerInfo.nCountTime);
	//
	//switch (m_StateSpeed)
	//{
	//case STATE_SPEED_ACCEL:
	//	CDebugProc::Print("STATE_SPEED_ACCEL\n");
	//	break;
	//case STATE_SPEED_BRAKS:
	//	CDebugProc::Print("STATE_SPEED_BRAKS\n");
	//	break;
	//case STATE_SPEED_DOWN:
	//	CDebugProc::Print("STATE_SPEED_DOWN\n");
	//	break;
	//case STATE_SPEED_STOP:
	//	CDebugProc::Print("STATE_SPEED_STOP\n");
	//	break;
	//}
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
	if (m_nNumItem >= MAX_EGG) { return; }

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
					if (pFeed->CollisionFeed(&m_pos, &m_OldPos) == true)
					{// �Փ˂���
						EggAppear(pFeed);	// ���o��
						pFeed->Uninit();	// �a�폜
						m_nNumEgg++;
						m_nNumItem++;

						break;
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

			m_bulletType[m_nNumChick + m_nNumEgg] = BULLET_EGG_ATTACK;
		}
		else if (pFeed->GetFeedType() == CFeed::FEEDTYPE_ANNOY)
		{// �W�Q�̗�����
			m_pEgg[m_nNumEgg] = CEgg::Create(m_pos,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(EGG_SCALE, EGG_SCALE, EGG_SCALE),
				CEgg::EGGTYPE_ANNOY,
				CEgg::BULLETTYPE_PLAYER,
				m_nPlayerNum);

			m_bulletType[m_nNumChick + m_nNumEgg] = BULLET_EGG_ANNOY;
		}
		else if (pFeed->GetFeedType() == CFeed::FEEDTYPE_SPEED)
		{// �����̗�����
			m_pEgg[m_nNumEgg] = CEgg::Create(m_pos,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(EGG_SCALE, EGG_SCALE, EGG_SCALE),
				CEgg::EGGTYPE_SPEED,
				CEgg::BULLETTYPE_PLAYER,
				m_nPlayerNum);

			m_bulletType[m_nNumChick + m_nNumEgg] = BULLET_EGG_SPEED;
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
			m_pEgg[0]->SetPos(D3DXVECTOR3((sinf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * (1 + m_nNumChick)) + m_pos.x,
				m_pEgg[0]->GetPos().y,
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

		m_pEgg[1]->SetPos(D3DXVECTOR3((sinf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * (2 + m_nNumChick)) + m_pos.x,
			m_pEgg[1]->GetPos().y,
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

		m_pEgg[2]->SetPos(D3DXVECTOR3((sinf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * (3 + m_nNumChick)) + m_pos.x,
			m_pEgg[2]->GetPos().y,
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
			m_pChick[0]->SetPos(D3DXVECTOR3((sinf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE) + m_pos.x,
				m_pChick[0]->SetHeight(),
				(cosf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE) + m_pos.z));

			// ���̊p�x�ݒ�
			m_pChick[0]->SetRot(m_OldEggRot[nData]);

			if (m_abJump[nData] == true)
			{
				m_pChick[0]->Jump(EGGJUMP);
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

		m_pChick[1]->SetPos(D3DXVECTOR3((sinf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * 2) + m_pos.x,
			m_pChick[1]->SetHeight(),
			(cosf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * 2) + m_pos.z));

		m_pChick[1]->SetRot(m_OldEggRot[nData]);

		if (m_abJump[nData] == true)
		{
			m_pChick[1]->Jump(EGGJUMP);
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

		m_pChick[2]->SetPos(D3DXVECTOR3((sinf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * 3) + m_pos.x,
			m_pChick[2]->SetHeight(),
			(cosf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * 3) + m_pos.z));

		m_pChick[2]->SetRot(m_OldEggRot[nData]);

		if (m_abJump[nData] == true)
		{
			m_pChick[2]->Jump(EGGJUMP);
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
			m_nPlayerRank = CGame::GetRanking(m_nPlayerNum);
			int nRank = CGame::GetRanking(m_nPlayerNum) - 1;

			m_nNumChick--;	// �����������炷

			m_nNumItem--;

			switch (m_pChick[0]->GetType())
			{
				// �U��
			case CChick::TYPE_ATTACK:
				if (nRank >= 0)
				{
					for (int nCntChar = 0; nCntChar < MAX_MEMBER; nCntChar++)
					{// �ЂƂO�̏��ʂ̂��������
						int nData = CGame::GetRanking(nCntChar);

						if (nRank == nData)
						{
							m_nDestRank = nCntChar;
							m_pChick[0]->SetDestRank(nCntChar);
							break;
						}
					}
				}
				else
				{
					m_pChick[0]->SetDis(false);
				}

				m_pChick[0]->SetRot(m_rot);
				break;

				// ����
			case CChick::TYPE_SPEED:
				m_State = PLAYERSTATE_SPEEDUP;
				m_fSpeed += SPEED;

				m_pChick[0]->Uninit();
				m_pChick[0] = NULL;
				break;

				// ����
			case CChick::TYPE_ANNOY:
				m_pChick[0]->SetDis(false);
				break;

				// �����U��
			case CChick::TYPE_ATTACK_S:
				for (int nCntChar = 0; nCntChar < MAX_MEMBER; nCntChar++)
				{// 1�ʂ̂��������
					int nDestRank = CGame::GetRanking(nCntChar);

					if (nDestRank == 0)
					{
						m_nDestRank = nCntChar;
						m_pChick[0]->SetDestRank(nCntChar);
						break;
					}
				}
				m_pChick[0]->SetRot(m_rot);
				break;

				// ����
			case CChick::TYPE_ANNOY_S:
				m_pChick[0]->SetDis(false);
				break;
			}

			// ������ւ�
			m_pChick[0] = m_pChick[1];
			m_pChick[1] = m_pChick[2];
			m_pChick[2] = NULL;

			m_bulletType[0] = m_bulletType[1];
			m_bulletType[1] = m_bulletType[2];
			m_bulletType[2] = BULLET_EGG_ATTACK;
		}
		else if (m_pEgg[0] != NULL && m_pEgg[0]->GetState() == CEgg::EGGSTATE_CHASE)
		{// ��ڂ̗��ɏ�񂪓����Ă��āA�v���C���[�ɂ��Ă��鎞
			m_pEgg[0]->SetState(CEgg::EGGSTATE_BULLET);	// ��Ԃ�e�ɂ���
			m_pEgg[0]->SetRank(CGame::GetRanking(m_nPlayerNum));
			m_nPlayerRank = CGame::GetRanking(m_nPlayerNum);

			m_nNumEgg--;	// �����������炷

			m_nNumItem--;

			switch (m_pEgg[0]->GetType())
			{
				// �U��
			case CEgg::EGGTYPE_ATTACK:
				m_pEgg[0]->Jump(THROW);
				m_pEgg[0]->SetRot(m_rot);
				m_pEgg[0]->SetThrow(TRUE);
				break;

				// ����
			case CEgg::EGGTYPE_SPEED:
				m_State = PLAYERSTATE_SPEEDUP;
				m_fSpeed += SPEED;

				// �W�����v
				m_bJumpSave = true;
				m_bJump = true;
				m_move.y += EGG_RAND;
				break;
			}

			// ������ւ�
			m_pEgg[0] = m_pEgg[1];
			m_pEgg[1] = m_pEgg[2];
			m_pEgg[2] = NULL;

			m_bulletType[0] = m_bulletType[1];
			m_bulletType[1] = m_bulletType[2];
			m_bulletType[2] = BULLET_EGG_ATTACK;
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

				if (pEgg->GetNumPlayer() != m_nPlayerNum)
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
			// Next�Ɏ���Scene������
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// �Ђ悱�Ƃ̓����蔻��
//=============================================================================
void CPlayer::CollisionChick(void)
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

			if (pScene->GetObjType() == OBJTYPE_CHICK)
			{
				CChick *pChick = (CChick*)pScene;	// �I�u�W�F�N�g�N���X�̃|�C���^�ϐ��ɂ���

				if (pChick->GetNumPlayer() != m_nPlayerNum && pChick->GetType() != CChick::TYPE_ANNOY_S)
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

							// �����U��
						case CChick::TYPE_ATTACK_S:
							// �_���[�W��Ԃɂ���
							if (m_bDamage == false && pChick->GetAttackS() == true)
							{
								m_bDamage = true;
								m_State = PLAYERSTATE_DAMAGE;
							}
							break;

							// ����
						case CChick::TYPE_ANNOY:
							if (m_bDamage == false)
							{
								m_bDamage = true;
								m_State = PLAYERSTATE_SPEEDDOWN;
							}
							break;
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
					if (CGame::GetRanking(m_nPlayerNum) < 3)
					{
						m_pChick[m_nNumChick] = CChick::Create(m_pos,
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							CHICK_SCALE,
							CChick::TYPE_ATTACK,
							CChick::BULLETTYPE_PLAYER,
							CChick::STATE_CHASE,
							m_nPlayerNum);
					}
					else if (CGame::GetRanking(m_nPlayerNum) >= 3)
					{
						m_pChick[m_nNumChick] = CChick::Create(m_pos,
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							CHICK_SCALE,
							CChick::TYPE_ATTACK_S,
							CChick::BULLETTYPE_PLAYER,
							CChick::STATE_CHASE,
							m_nPlayerNum);
					}

					m_bulletType[m_nNumChick] = BULLET_CHICK_ATTACK;

					break;

					// ����
				case CEgg::EGGTYPE_ANNOY:
					if (CGame::GetRanking(m_nPlayerNum) < 3)
					{
						m_pChick[m_nNumChick] = CChick::Create(m_pos,
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							CHICK_SCALE,
							CChick::TYPE_ANNOY,
							CChick::BULLETTYPE_PLAYER,
							CChick::STATE_CHASE,
							m_nPlayerNum);
					}
					else if (CGame::GetRanking(m_nPlayerNum) >= 3)
					{
						m_pChick[m_nNumChick] = CChick::Create(m_pos,
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							CHICK_SCALE,
							CChick::TYPE_ANNOY_S,
							CChick::BULLETTYPE_PLAYER,
							CChick::STATE_CHASE,
							m_nPlayerNum);
					}

					m_bulletType[m_nNumChick] = BULLET_CHICK_ANNOY;

					break;

					// ����
				case CEgg::EGGTYPE_SPEED:
					/*if (CGame::GetRanking(m_nPlayerNum) < 3)
					{*/
					m_pChick[m_nNumChick] = CChick::Create(m_pos,
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						CHICK_SCALE,
						CChick::TYPE_SPEED,
						CChick::BULLETTYPE_PLAYER,
						CChick::STATE_CHASE,
						m_nPlayerNum);
					/*}*/
					/*else if (CGame::GetRanking(m_nPlayerNum) >= 3)
					{
					m_pChick[m_nNumChick] = CChick::Create(m_pos,
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					CHICK_SCALE,
					CChick::TYPE_SPEED_S,
					CChick::BULLETTYPE_PLAYER,
					CChick::STATE_CHASE,
					m_nPlayerNum);
					}*/

					m_bulletType[m_nNumChick] = BULLET_CHICK_SPEED;

					break;
				}

				m_pEgg[0]->Uninit();
				m_pEgg[0] = m_pEgg[1];
				m_pEgg[1] = m_pEgg[2];
				m_pEgg[2] = NULL;
				m_nNumEgg--;

				m_nNumChick++;
			}
		}
	}
}

//=============================================================================
// �~�炷�Ђ悱�̏o������
//=============================================================================
void CPlayer::FallChicks(D3DXVECTOR3 pos)
{
	CScene *pScene;

	for (int nCntChick = 0; nCntChick < CHICK_FALL_NUM; nCntChick++)
	{
		int fx = rand() % FALL_CHICK_RANGE;
		int fz = rand() % FALL_CHICK_RANGE;

		// �Ђ悱�o��
		CChick::Create(D3DXVECTOR3(pos.x + ((FALL_CHICK_RANGE / 2) - fx), pos.y + 50.0f + (nCntChick * 100.0f), pos.z + ((FALL_CHICK_RANGE / 2) - fz)),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			CHICK_SCALE,
			CChick::TYPE_ATTACK_S,
			CChick::BULLETTYPE_PLAYER,
			CChick::STATE_BULLET,
			m_nPlayerNum);

		m_nCntChick++;
	}

	for (int nCntPriority = 2; nCntPriority <= EGG_PRIOTITY; nCntPriority++)
	{
		// �v���C�I���e�B�[�`�F�b�N
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �v���C�I���e�B�[���̃��X�g�\�����Ō�܂Ō���
			CScene *pSceneNext = pScene->GetNext();		// ���̃I�u�W�F�N�g��ۑ�

			if (pScene->GetObjType() == OBJTYPE_CHICK)
			{// �^�C�v����Q����������
				CChick *pChick = (CChick*)pScene;	// �I�u�W�F�N�g�N���X�̃|�C���^�ϐ��ɂ���

				if (pChick->GetState() == CChick::STATE_BULLET && pChick->GetType() == CChick::TYPE_ATTACK_S && pChick->GetAttackS() == false)
				{
					pChick->SetAttackS(true);
					pChick->SetRank(CGame::GetRanking(m_nPlayerNum));
					pChick->SetDestRank(m_nDestRank);
				}
			}
			// Next�Ɏ���Scene������
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// ���������Ђ悱�̏o������
//=============================================================================
void CPlayer::AnnoyChicks(void)
{
	CPlayer **pPlayer = CGame::GetPlayer();

	for (int nCntPlayer = 0; nCntPlayer < MAX_MEMBER; nCntPlayer++)
	{
		if (pPlayer[nCntPlayer] != NULL)
		{
			if (nCntPlayer != m_nPlayerNum)
			{
				if (m_pAnnoyChick[nCntPlayer] == NULL)
				{
					m_pAnnoyChick[nCntPlayer] = CChick::Create(
						D3DXVECTOR3(pPlayer[nCntPlayer]->GetPos().x, pPlayer[nCntPlayer]->GetPos().y + 100.0f, pPlayer[nCntPlayer]->GetPos().z),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						CHICK_SCALE,
						CChick::TYPE_ANNOY_S,
						CChick::BULLETTYPE_PLAYER,
						CChick::STATE_BULLET,
						m_nPlayerNum);

					if (m_pAnnoyChick[nCntPlayer]->GetState() == CChick::STATE_BULLET
						&& m_pAnnoyChick[nCntPlayer]->GetType() == CChick::TYPE_ANNOY_S
						&& m_pAnnoyChick[nCntPlayer]->GetAttackS() == false
						&& m_pAnnoyChick[nCntPlayer]->GetDis() == true)
					{
						m_pAnnoyChick[nCntPlayer]->SetAttackS(true);
						m_pAnnoyChick[nCntPlayer]->SetRank(CGame::GetRanking(m_nPlayerNum));
					}

					m_bAnnoyS = true;
				}
			}
		}
	}
}

//=============================================================================
// ���������Ђ悱������������
//=============================================================================
void CPlayer::ChaseAnnoyS(void)
{
	if (m_pAnnoyChick[m_nPlayerNum] != NULL)
	{
		// �ʒu�X�V
		m_pAnnoyChick[m_nPlayerNum]->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y + 60.0f, m_pos.z));

		// �H����Ă��鎞�Ԃ��J�E���g
		m_nAnnoySTimer++;

		if (m_nAnnoySTimer > 100)
		{// ��莞�Ԃ�������
			m_nAnnoySTimer = 0;
			m_pAnnoyChick[m_nPlayerNum]->Uninit();
			m_pAnnoyChick[m_nPlayerNum] = NULL;
		}

		if (m_bDamage == false)
		{
			m_bDamage = true;
			m_State = PLAYERSTATE_SPEEDDOWN;
		}
	}
}

//=============================================================================
// �L�������m�̏Փ˔��菈��
//=============================================================================
void CPlayer::CollisionCharacter(void)
{
	CPlayer **pPlayer = CGame::GetPlayer();

	for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
	{// �v���C���[�J�E���g
		if (pPlayer[nCntMember] != NULL)
		{// NULL�ȊO
			if (pPlayer[nCntMember] != this)
			{// �����ȊO
			 // �����v�Z
				D3DXVECTOR3 pos = pPlayer[nCntMember]->GetPos();
				float fLenght = sqrtf(powf(pos.x - m_pos.x, 2.0f) + powf(pos.z - m_pos.z, 2.0f));

				if (fLenght < 10.0f)
				{// �͈͓�
				 // �p�x�v�Z
					float fAngle = atan2f(m_pos.x - pos.x, m_pos.z - pos.z);

					// �ʒu����
					m_pos = pos + D3DXVECTOR3(sinf(fAngle) * 10.0f, 0.0f, cosf(fAngle) * 10.0f);
				}
			}
		}
	}
}
//=============================================================================
// �v���C���[�̃��[�V����
//=============================================================================
void CPlayer::UpdateMotion(void)
{
	//���[�V����
	KEY *pKey, *pNextKey;
	float fRateMotion;
	float fDiffMotion;
	D3DXVECTOR3 rotmotion;
	D3DXVECTOR3 posmotion;
	//D3DXVECTOR3 posParent;


	//�L�[���ő吔������Ȃ��悤��
	if (m_aMotionInfo[m_nMotionType].nNumKey <= m_nKey)
	{
		m_nKey = 0;
	}

	//���[�V�����X�V
	for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
	{
		if (m_apModel[nCntParts] != NULL)
		{
			//���݂̃L�[���擾
			pKey = &m_pKeyInfo[m_nMotionType][m_nKey].aKey[nCntParts];
			//���̃L�[���擾
			pNextKey = &m_pKeyInfo[m_nMotionType][(m_nKey + 1) % m_aMotionInfo[m_nMotionType].nNumKey].aKey[nCntParts];

			//���݂̃L�[���玟�̃L�[�ւ̍Đ��t���[�����ɂ����郂�[�V�����J�E���^�[�̑��Βl���Z�o
			fRateMotion = (float)m_nCountFlame / (float)m_pKeyInfo[m_nMotionType][m_nKey].nFrame;

			//ROT
			//���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍������Z�o
			fDiffMotion = pNextKey->frotX - pKey->frotX;
			//���Βl���������g���Ċe�v�f�̒l���Z�o
			rotmotion.x = pKey->frotX + (fDiffMotion * fRateMotion);

			//POS
			//���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍������Z�o
			fDiffMotion = pNextKey->fposX - pKey->fposX;
			//���Βl���������g���Ċe�v�f�̒l���Z�o
			posmotion.x = pKey->fposX + (fDiffMotion * fRateMotion);


			//���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍������Z�o
			fDiffMotion = pNextKey->frotY - pKey->frotY;
			//���Βl���������g���Ċe�v�f�̒l���Z�o
			rotmotion.y = pKey->frotY + (fDiffMotion * fRateMotion);
			//POS
			//���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍������Z�o
			fDiffMotion = pNextKey->fposY - pKey->fposY;
			//���Βl���������g���Ċe�v�f�̒l���Z�o
			posmotion.y = pKey->fposY + (fDiffMotion * fRateMotion);


			//���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍������Z�o
			fDiffMotion = pNextKey->frotZ - pKey->frotZ;
			//���Βl���������g���Ċe�v�f�̒l���Z�o
			rotmotion.z = pKey->frotZ + (fDiffMotion * fRateMotion);
			//POS
			//���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍������Z�o
			fDiffMotion = pNextKey->fposZ - pKey->fposZ;
			//���Βl���������g���Ċe�v�f�̒l���Z�o
			posmotion.z = pKey->fposZ + (fDiffMotion * fRateMotion);


			//�p�[�c�𓮂���
			m_apModel[nCntParts]->SetRot(rotmotion);

			//POS
			m_apModel[nCntParts]->SetPos(D3DXVECTOR3(m_OffSetPos[nCntParts].x + posmotion.x,
				m_OffSetPos[nCntParts].y + posmotion.y,
				m_OffSetPos[nCntParts].z + posmotion.z));

		}
	}

	//���[�v�̔���
	switch (m_aMotionInfo[m_nMotionType].bLoop)
	{
	case true:
		//���[�v����
		//�t���[����i�߂�
		m_nCountFlame++;
		//�L�[�̍X�V
		if (m_nCountFlame >= m_pKeyInfo[m_nMotionType][m_nKey].nFrame)
		{
			if (m_aMotionInfo[m_nMotionType].nNumKey - 1 == m_nKey)
			{
				m_nKey = 0;
			}
			else
			{
				m_nKey += 1;
			}
			m_nCountFlame = 0;
		}

		break;
	case false:
		//���[�v���Ȃ�
		if (m_aMotionInfo[m_nMotionType].nNumKey - 1 > m_nKey)
		{//�t���[����i�߂�
			m_nCountFlame++;
		}
		else if (m_aMotionInfo[m_nMotionType].nNumKey - 1 == m_nKey)
		{
			//if (m_nAnimnow == PLAYERANIM_ATTACK)
			//{//�U�����[�V����
			//	m_nAttackDelay++;
			//	if (m_nAttackDelay > 20)
			//	{
			//		m_bAttack = false;
			//		m_nAttackDelay = 0;
			//	}
			//}
			m_bMotionEnd = true;
		}
		//�L�[�̍X�V
		if (m_nCountFlame >= m_pKeyInfo[m_nMotionType][m_nKey].nFrame)
		{
			if (m_aMotionInfo[m_nMotionType].nNumKey > m_nKey)
			{
				m_nKey += 1;
			}
			m_nCountFlame = 0;
		}
		break;
	}

#ifdef  _DEBUG
	CDebugProc::Print(" Num�L�[  : (%d)\n", m_nKey);
	CDebugProc::Print(" m_nCountFlame  : (%d)\n", m_nCountFlame);

#endif

}

//=============================================================================
// �t�@�C���ǂݍ���
//=============================================================================
void CPlayer::FileLoad(void)
{
	//�f�o�C�X���擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//�t�@�C���ǂݍ��ݗp�ϐ�
	FILE *pFile;		//�t�@�C���|�C���^
	char *pStrcur;		//���݂̐擪�̕�����
	char aLine[256];	//������
	char aStr[256];		//�ꎞ�ۑ�������
	int nIndex = 0;		//���݂̃C���f�b�N�X
	int nWord = 0;		//�|�b�v�ŕԂ��ꂽ�l��ێ�

	D3DXVECTOR3 ParentPos;	//�e�̈ʒu�����擾
#if 1
							//�t�@�C�����J�� �ǂݍ���
	pFile = fopen(FILE_NAME_PRISONER, "r");
	//NULL�`�F�b�N
	if (pFile != NULL)
	{
		for (int nCntMotion = 0; nCntMotion < MAX_MOTION; )
		{
			//������̐擪��ݒ�
			pStrcur = ReadLine(pFile, &aLine[0]);
			//����������o��
			strcpy(aStr, pStrcur);

			//������̃f�[�^ ��r���镶���� ��r���镶����
			if (memcmp(pStrcur, "NUM_MODEL = ", strlen("NUM_MODEL = ")) == 0)
			{
				//���o��
				pStrcur += strlen("NUM_MODEL = ");
				//������̐擪��ݒ�
				strcpy(aStr, pStrcur);
				//�����񔲂��o��
				g_nNumModel = atoi(pStrcur);

				for (int nCntModel = 0; nCntModel < g_nNumModel; nCntModel++)
				{
					//������̐擪��ݒ�
					pStrcur = ReadLine(pFile, &aLine[0]);
					//����������߂�
					strcpy(aStr, pStrcur);
					if (memcmp(pStrcur, "MODEL_FILENAME = ", strlen("MODEL_FILENAME = ")) == 0)
					{
						//���o��
						pStrcur += strlen("MODEL_FILENAME = ");

						//�������ݒ�
						strcpy(aStr, pStrcur);

						//�K�v�ȕ�����̍Ō�̕����܂ł̕������𐔂���
						int nNullNum = PopString(pStrcur, &aStr[0]);

						//����������߂�
						strcpy(aStr, pStrcur);

						//�Ō�̕�����NULL����������
						aStr[nNullNum - 1] = '\0';

						//�Ώۂ̕����񂩂甲���o��
						strcpy(&g_aFileNameModel[nCntModel][0], aStr);

						// X�t�@�C���̓ǂݍ���
						D3DXLoadMeshFromX(&g_aFileNameModel[nCntModel][0],
							D3DXMESH_SYSTEMMEM,
							pDevice,
							NULL,
							&m_pBuffMat[nCntModel],
							NULL,
							&m_nNumMat[nCntModel],
							&m_pMesh[nCntModel]);
					}
				}
				//������̐擪��ݒ�
				pStrcur = ReadLine(pFile, &aLine[0]);
				//����������o��
				strcpy(aStr, pStrcur);
			}


			//������̃f�[�^ ��r���镶���� ��r���镶����
			if (memcmp(pStrcur, "CHARACTERSET", 12) == 0)
			{
				while (1)
				{
					//������̐擪��ݒ�
					pStrcur = ReadLine(pFile, &aLine[0]);
					//����������o��
					strcpy(aStr, pStrcur);
					if (memcmp(pStrcur, "NUM_PARTS = ", strlen("NUM_PARTS = ")) == 0)
					{
						//���o��
						pStrcur += strlen("NUM_PARTS = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_nNumParts = atoi(pStrcur);
					}
					if (memcmp(pStrcur, "PARTSSET", strlen("PARTSSET")) == 0)
					{
						while (1)
						{
							//������̐擪��ݒ�
							pStrcur = ReadLine(pFile, &aLine[0]);

							//INDEX��ǂݍ���
							if (memcmp(pStrcur, "INDEX = ", strlen("INDEX = ")) == 0)
							{
								//���o��
								pStrcur += strlen("INDEX = ");
								//������̐擪��ݒ�
								strcpy(aStr, pStrcur);
								//�����񔲂��o��
								nIndex = atoi(pStrcur);
							}
							//PARENT��ǂݍ���
							if (memcmp(pStrcur, "PARENT = ", strlen("PARENT = ")) == 0)
							{
								//���o��
								pStrcur += strlen("PARENT = ");
								//������̐擪��ݒ�
								strcpy(aStr, pStrcur);
								//�����񔲂��o��
								m_aIndexParent[nIndex] = atoi(pStrcur);
							}
							//POS��ǂݍ���
							if (memcmp(pStrcur, "POS = ", strlen("POS = ")) == 0)
							{
								//���o��
								pStrcur += strlen("POS = ");
								//������̐擪��ݒ�
								strcpy(aStr, pStrcur);

								//��������Ԃ��Ă��炤
								nWord = PopString(pStrcur, &aStr[0]);
								//������ϊ�
								m_aKayOffset[nIndex].fposX = (float)atof(pStrcur);
								//���������i�߂�
								pStrcur += nWord;

								//��������Ԃ��Ă��炤
								nWord = PopString(pStrcur, &aStr[0]);
								//������ϊ�
								m_aKayOffset[nIndex].fposY = (float)atof(pStrcur);
								//���������i�߂�
								pStrcur += nWord;

								//��������Ԃ��Ă��炤
								nWord = PopString(pStrcur, &aStr[0]);
								//������ϊ�
								m_aKayOffset[nIndex].fposZ = (float)atof(pStrcur);

							}
							//ROT��ǂݍ���
							if (memcmp(pStrcur, "ROT = ", strlen("ROT = ")) == 0)
							{
								//���o��
								pStrcur += strlen("ROT = ");
								//������̐擪��ݒ�
								strcpy(aStr, pStrcur);

								//��������Ԃ��Ă��炤
								nWord = PopString(pStrcur, &aStr[0]);
								//������ϊ�
								m_aKayOffset[nIndex].frotX = (float)atof(pStrcur);

								//���������i�߂�
								pStrcur += nWord;
								//��������Ԃ��Ă��炤
								nWord = PopString(pStrcur, &aStr[0]);
								//������ϊ�
								m_aKayOffset[nIndex].frotY = (float)atof(pStrcur);

								//���������i�߂�
								pStrcur += nWord;
								//��������Ԃ��Ă��炤
								nWord = PopString(pStrcur, &aStr[0]);
								//������ϊ�
								m_aKayOffset[nIndex].frotZ = (float)atof(pStrcur);
							}
							//�p�[�c�Z�b�g�I��
							else if (memcmp(pStrcur, "END_PARTSSET", strlen("END_PARTSSET")) == 0)
							{
								//NULL�`�F�b�N
								if (m_apModel[nIndex] == NULL)
								{//���I�m��
									m_apModel[nIndex] = new CModel;
									//NULL�`�F�b�N
									if (m_apModel[nIndex] != NULL)
									{
										//���f���̐���
										m_apModel[nIndex]->BindModel(m_pMesh[nIndex], m_pBuffMat[nIndex], m_nNumMat[nIndex]);
										m_apModel[nIndex]->Init();
									}
								}

								//���f���𐶐�	�I�t�Z�b�g�ݒ�
								/*m_apModel[nIndex] = CModel::Create(
								D3DXVECTOR3(m_pos.x + m_aKayOffset[nIndex].fposX,
								m_pos.y + m_aKayOffset[nIndex].fposY,
								m_pos.z + m_aKayOffset[nIndex].fposZ), m_rot);*/

								m_apModel[nIndex]->SetPos(D3DXVECTOR3(m_pos.x + m_aKayOffset[nIndex].fposX,
									m_pos.y + m_aKayOffset[nIndex].fposY,
									m_pos.z + m_aKayOffset[nIndex].fposZ));

								//pos����
								ParentPos = m_apModel[nIndex]->GetPos();
								m_OffSetPos[nIndex] = m_apModel[nIndex]->GetPos();

								//���f�������蓖��
								m_apModel[nIndex]->BindModel(m_pMesh[nIndex], m_pBuffMat[nIndex], m_nNumMat[nIndex]);

								if (m_aIndexParent[nIndex] == -1)
								{
									//���f���̐e���w��
									m_apModel[nIndex]->SetParent(NULL);
									ParentPos = m_apModel[nIndex]->GetPos();
								}
								else
								{
									//���f���̐e���w��
									m_apModel[nIndex]->SetParent(m_apModel[m_aIndexParent[nIndex]]);
								}

								break;
							}
						}
					}
					//�L�����N�^�[�Z�b�g�I��
					else if (memcmp(pStrcur, "END_CHARACTERSET", strlen("END_CHARACTERSET")) == 0)
					{
						break;
					}
				}
				//������̐擪��ݒ�
				pStrcur = ReadLine(pFile, &aLine[0]);
				//����������o��
				strcpy(aStr, pStrcur);
			}

			//���[�V�����ǂݍ���
			if (memcmp(pStrcur, "MOTIONSET", strlen("MOTIONSET")) == 0)
			{
				//���o��
				pStrcur += strlen("MOTIONSET");

				while (1)
				{
					//������̐擪��ݒ�
					pStrcur = ReadLine(pFile, &aLine[0]);
					//����������o��
					strcpy(aStr, pStrcur);

					if (memcmp(pStrcur, "LOOP = ", strlen("LOOP = ")) == 0)
					{
						//���o��
						pStrcur += strlen("LOOP = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);

						switch (atoi(pStrcur))
						{
						case 0:
							//�����񔲂��o��
							m_aMotionInfo[nCntMotion].bLoop = false;
							break;
						case 1:
							//�����񔲂��o��
							m_aMotionInfo[nCntMotion].bLoop = true;
							break;
						}
						//������̐擪��ݒ�
						pStrcur = ReadLine(pFile, &aLine[0]);
					}
					if (memcmp(pStrcur, "NUM_KEY = ", strlen("NUM_KEY = ")) == 0)
					{
						//���o��
						pStrcur += strlen("NUM_KEY = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_aMotionInfo[nCntMotion].nNumKey = atoi(pStrcur);

						//������̐擪��ݒ�
						pStrcur = ReadLine(pFile, &aLine[0]);
						//����������o��
						strcpy(aStr, pStrcur);
					}

					//�L�[�̐ݒ�
					for (int nCntKey = 0; nCntKey < m_aMotionInfo[nCntMotion].nNumKey;)
					{
						if (memcmp(pStrcur, "KEYSET", strlen("KEYSET")) == 0)
						{
							//���o��
							pStrcur += strlen("KEYSET");
							//������̐擪��ݒ�
							strcpy(aStr, pStrcur);
							//������̐擪��ݒ�
							pStrcur = ReadLine(pFile, &aLine[0]);

							if (memcmp(pStrcur, "FRAME = ", strlen("FRAME = ")) == 0)
							{
								//���o��
								pStrcur += strlen("FRAME = ");

								m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].nFrame = atoi(pStrcur);

								//������̐擪��ݒ�
								strcpy(aStr, pStrcur);
								//������̐擪��ݒ�
								pStrcur = ReadLine(pFile, &aLine[0]);
							}

							//�p�[�c����
							for (int nCntParts = 0; nCntParts < m_nNumParts;)
							{
								if (memcmp(pStrcur, "KEY", strlen("KEY")) == 0)
								{
									//������̐擪��ݒ�
									pStrcur = ReadLine(pFile, &aLine[0]);

									if (memcmp(pStrcur, "POS = ", strlen("POS = ")) == 0)
									{
										//���o��
										pStrcur += strlen("POS = ");
										//������̐擪��ݒ�
										strcpy(aStr, pStrcur);

										//��������Ԃ��Ă��炤
										nWord = PopString(pStrcur, &aStr[0]);
										//POS.X���
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].fposX = (float)atof(pStrcur);
										//���������i�߂�
										pStrcur += nWord;

										//��������Ԃ��Ă��炤
										nWord = PopString(pStrcur, &aStr[0]);
										//POS.Y���
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].fposY = (float)atof(pStrcur);
										//���������i�߂�
										pStrcur += nWord;

										//��������Ԃ��Ă��炤
										nWord = PopString(pStrcur, &aStr[0]);
										//POS.Z���
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].fposZ = (float)atof(pStrcur);
										//������̐擪��ݒ�
										pStrcur = ReadLine(pFile, &aLine[0]);
									}
									//ROT��ǂݍ���
									if (memcmp(pStrcur, "ROT = ", strlen("ROT = ")) == 0)
									{
										//���o��
										pStrcur += strlen("ROT = ");
										//������̐擪��ݒ�
										strcpy(aStr, pStrcur);

										//��������Ԃ��Ă��炤
										nWord = PopString(pStrcur, &aStr[0]);
										//RotX
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].frotX = (float)atof(pStrcur);
										//���������i�߂�
										pStrcur += nWord;

										//��������Ԃ��Ă��炤
										nWord = PopString(pStrcur, &aStr[0]);
										//RotY
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].frotY = (float)atof(pStrcur);
										//���������i�߂�
										pStrcur += nWord;

										//��������Ԃ��Ă��炤
										nWord = PopString(pStrcur, &aStr[0]);
										//RotZ
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].frotZ = (float)atof(pStrcur);

										//������̐擪��ݒ�
										pStrcur = ReadLine(pFile, &aLine[0]);
									}
									if (memcmp(pStrcur, "END_KEY", strlen("END_KEY")) == 0)
									{
										//���o��
										pStrcur += strlen("END_KEY");
										//������̐擪��ݒ�
										strcpy(aStr, pStrcur);
										//������̐擪��ݒ�
										pStrcur = ReadLine(pFile, &aLine[0]);
										//�p�[�c�̃J�E���g��i�߂�
										nCntParts++;
									}
								}
								else
								{
									//������̐擪��ݒ�
									pStrcur = ReadLine(pFile, &aLine[0]);
								}
							}
							if (memcmp(pStrcur, "END_KEYSET", strlen("END_KEYSET")) == 0)
							{
								//������̐擪��ݒ�
								pStrcur = ReadLine(pFile, &aLine[0]);
								//�J�E���g��i�߂�
								nCntKey++;
							}
						}
						else
						{
							//������̐擪��ݒ�
							pStrcur = ReadLine(pFile, &aLine[0]);
						}

					}
					if (memcmp(pStrcur, "END_MOTIONSET", strlen("END_MOTIONSET")) == 0)
					{
						//���[�V�����̏����Z�b�g
						m_pKeyInfo[nCntMotion] = &m_aMotionInfo[nCntMotion].aKayInfo[0];
						nCntMotion++;
						break;
					}
				}
			}
			//�X�N���v�g�̏I���
			if (memcmp(pStrcur, "END_SCRIPT	", strlen("END_SCRIPT")) == 0)
			{
				break;
			}
		}
	}
	else
	{	//�t�@�C�����J���Ȃ�����
		printf("�t�@�C�����J���܂���ł���\n");
	}
	//�t�@�C�������
	fclose(pFile);
#endif

}

//=============================================================================
//�@�t�@�C���ǂݍ��ݖ�������r��
//=============================================================================
char *CPlayer::ReadLine(FILE *pFile, char *pDst)
{
	while (1)
	{
		//�P�s���ǂݍ���
		fgets(&pDst[0], 256, pFile);

		//������̃f�[�^ ��r���镶���� ��r���镶����
		if (memcmp(pDst, "#", strlen("#")) == 0)
		{
			pDst += strlen("\n");
		}
		//������̃f�[�^ ��r���镶���� ��r���镶����
		else if (memcmp(pDst, "\t", strlen("\t")) == 0)
		{
			pDst += strlen("\t");
			while (1)
			{
				if (memcmp(pDst, "\t", strlen("\t")) == 0)
				{
					pDst += strlen("\t");
				}
				else
				{
					break;
				}
			}
			break;
		}
		//������̃f�[�^ ��r���镶���� ��r���镶����
		else if (memcmp(pDst, " ", strlen(" ")) == 0)
		{
			pDst += strlen(" ");
			while (1)
			{
				if (memcmp(pDst, " ", strlen(" ")) == 0)
				{
					pDst += strlen(" ");
				}
				else
				{
					break;
				}
				break;
			}
		}
		//������̃f�[�^ ��r���镶���� ��r���镶����
		else if (memcmp(pDst, "\n", strlen("\n")) == 0)
		{
			pDst += strlen("\n");
		}
		else
		{
			break;
		}
	}
	return pDst;
}

//=============================================================================
//�@�t�@�C���ǂݍ��ݐ擪��r��
//=============================================================================
char * CPlayer::GetLineTop(char * pStr)
{
	while (1)
	{
		//������̃f�[�^ ��r���镶���� ��r���镶����
		if (memcmp(pStr, " ", strlen(" ")) == 0)
		{
			pStr += strlen(" ");
			while (1)
			{
				if (memcmp(pStr, " ", strlen(" ")) == 0)
				{
					pStr += strlen(" ");
				}
				else
				{
					break;
				}
				break;
			}
		}
		//������̃f�[�^ ��r���镶���� ��r���镶����
		else if (memcmp(pStr, "\t", strlen("\t")) == 0)
		{
			pStr += strlen("\t");
			while (1)
			{
				if (memcmp(pStr, "\t", strlen("\t")) == 0)
				{
					pStr += strlen("\t");
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			break;
		}
	}
	return pStr;
}

//=============================================================================
//�@��������Ԃ�
//=============================================================================
int CPlayer::PopString(char * pStr, char * pDest)
{
	int nWord = 0;

	while (1)
	{	//���o��
		pStr += 1;
		nWord += 1;
		if (memcmp(pStr, " ", strlen(" ")) == 0)
		{	//���o��
			pStr = "\0";
			nWord += 1;
			break;
		}
		if (memcmp(pStr, "\t", strlen("\t")) == 0)
		{	//���o��
			pStr = "\0";
			nWord += (int)strlen("\t");
			break;
		}
		//������̃f�[�^ ��r���镶���� ��r���镶����
		else if (memcmp(pStr, "\n", strlen("\n")) == 0)
		{
			//���o��
			nWord += (int)strlen("\n");
			break;
		}
	}
	strcpy(pDest, pStr);
	//������̐���Ԃ�
	return nWord;
}