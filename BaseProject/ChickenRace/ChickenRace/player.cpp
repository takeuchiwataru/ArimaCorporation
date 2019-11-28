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
#include "ModelEffect.h"
#include "DispEffect.h"
#include "particle.h"
#include "mesh.h"
#include "meshfield.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define VECTOR_ZERO			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))				//�x�N�g���̏�����
#define FAILE_NAME			("data\\TEXT\\Player\\PlayerState.txt")		//�ǂݍ��ރt�@�C����
#define FILE_TEXTURE		("data\\TEXTURE\\modeltex\\�j���g��.jpg")	//�e�N�X�`���̓ǂݍ���
#define ROLLOVER_STOP		(0.6f)										//���]�h�~�p�x
#define DECELERATION		(0.5f)										//�����̊���
#define EGG_RANGE			(25.0f)										// ���ƃv���C���[�̋���
#define EGG_POS				(7)											// �����m�̊Ԋu�̍L���i���₷�ƍL���Ȃ�j
#define SPEEDUP_TIME		(60)										// �������Ă��鎞��
#define CHICK_SCALE			(D3DXVECTOR3(1.0f, 1.0f, 1.0f))				//�Ђ悱�̑傫��
#define THROW				(11.0f)										// ���𓊂����
#define EGG_RAND			(2.0f)										// ���ɏ��Ƃ��̃W�����v��
#define EGG_HEIGHT			(40.0f)										// ���ɏ�����悤�Ɍ����鍂��
#define SPEED_CHICK			(0.5f)										// ��������ʂЂ悱
#define SPEED_EGG			(0.2f)										// ��������ʗ�
#define SPEED_COUNT_ANNOY	(50)										// �������
#define SPEED_COUNT_DAMAGE	(0)											// �_���[�W���
#define EGGJUMP				(2.0f)										// ���̃W�����v��
#define ANNOY_PARTICLE		(5)											// �����G�t�F�N�g�̏o��Ԋu
#define COL_PARTICLE		(30)										// ����Ђ悱�����������Ƃ��ɏo��G�t�F�N�g�̗�
#define COL_PARTICLE_S		(10)										// 覐΂Ђ悱�����������Ƃ��ɏo��G�t�F�N�g�̗�

// �v���C���[���
#define PLAYER_ACCEL	(0.5f)										//�����l�i�O�i�j
#define PLAYER_BRAKS	(-0.2f)										//�����l�i��i�j
#define PLAYER_DOWN		(0.08f)										//�����x
#define PLAYER_ADDROT	(0.005f)									//��]��
#define PLAYER_DOWNROT	(0.2f)										//��]��

#define PLAYER_JUMP		(2.0f)										//��]��
#define PLAYER_GRAVITY	(0.09f)										//��]��

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
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

	if (playerType == PLAYERTYPE_PLAYER)
	{//��ʃG�t�F�N�g�̐���
		pPlayer->m_pDispEffect = CDispEffect::Create(pPlayer);
	}
	pPlayer->m_fRoad = nPlayerNum * (140.0f / 8.0f) - 70.0f;

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
// �t�@�C���̓ǂݍ���
//=============================================================================
void CPlayer::Load(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice,
		FILE_TEXTURE,
		&m_pTexture);
}

//=============================================================================
// �t�@�C���̔j��
//=============================================================================
void CPlayer::Unload(void)
{
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
	m_pos = VECTOR_ZERO;					//���S���W
	m_OldPos = VECTOR_ZERO;					//�O��̍��W
	m_move = VECTOR_ZERO;					//�ړ�
	m_pos = VECTOR_ZERO;					//�ʒu
	m_rot = VECTOR_ZERO;					//����
	m_nCntFrame = -1;						//����
	m_pMotion = NULL;						//���[�V�����|�C���^
	m_StateSpeed = STATE_SPEED_STOP;		//�X�s�[�h�̏�Ԑݒ�
	m_StateHandle = HANDLE_MAX;				//�n���h���̏��
	m_PlayerInfo.fCountTime = 0;			//�J�E���^�[
	m_PlayerInfo.fAccel = PLAYER_ACCEL;		//�����l�i�O�i�j
	m_PlayerInfo.fBraks = PLAYER_BRAKS;		//�����l�i��i�j
	m_PlayerInfo.fDown = PLAYER_DOWN;		//�����l
	m_PlayerInfo.fAddRot = PLAYER_ADDROT;	//�������]�l
	m_PlayerInfo.fDistance = 0.0f;			//����
	m_PlayerInfo.FirstPos = VECTOR_ZERO;	//�����ʒu
	m_bJump = true;							//�W�����v���
	m_bControl = false;						//�R���g���[���[�t���O
	m_fvtxMaxY = 0.0f;						//���f�����_�̍ő�l�iY�j
	m_fSpeed = 0.0f;						// ����
	m_nCountSpeed = 0;
	m_nCountTime = 0;						// ���ԃJ�E���^�[
	m_bCrcleCarIn = false;
	m_bShake = true;
	m_nNumEgg = 0;
	m_nNumChick = 0;
	m_nNumItem = 0;
	m_bJumpSave = false;
	m_bJumpFlag = false;
	m_nCntDamage = 0;
	m_bDamage = false;
	m_State = PLAYERSTATE_NORMAL;
	m_nPlayerRank = 0;
	m_nCntChick = 0;
	m_nDestRank = 0;
	m_nAnnoySTimer = 0;
	m_nCntParticle = 0;
	m_bAnnoyS = false;

	m_nDriftCounter = 0;		// �h���t�g�J�E���g

	m_pPoint = CRoad_Manager::GetManager()->GetTop(0);
	m_pEnmPoint = CRoad_Manager::GetManager()->GetTop(1);

	m_nPlayerNum = 0;					// �v���C���[�ԍ�
	m_nControllerNum = 0;				// �R���g���[���[�ԍ�

	m_nStartFrame = (CServer::Rand() % 50);
	m_nStartCounter = 0;

	m_pDispEffect = NULL;
	m_FEffect = CCOL_MESH::EFFECT_NORMAL;
	m_bDrop = false;				// ������
	m_nDropCounter = 0;					// �������J�E���g
	m_fLength = 20.0f;
	m_bDivided = false;
	m_nMap = 0;
	m_nNumRoad = 0;

	m_FNor = INIT_VECTOR;
	m_fTilt = 0.0f;
	m_fCTiltV = 0.0f;
	m_fCTiltW = 0.0f;
	m_fPosY = 0.0f;
	m_fRotOld = 0.0f;
	m_bJumpOld = m_bJump;

	m_nAnimnow = PLAYERANIM_NEUTRAL;	//�j���[�g�������
	m_nCountFlame = 0;
	m_nKey = 0;

	m_bGoal = false;					// �S�[��
	m_fAddRot = 0.0f;					// ���Z�p�x

										// �v���C���[�ԍ��i�Ǐ]�j
	if (m_pPlayerNum == NULL)
		m_pPlayerNum = CBillBoord::Create(m_pos + D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXVECTOR2(10.0f, 10.0f), 1);

	if (m_pPlayerpos == NULL)
	{
		m_pPlayerpos = CBillBoord::Create(m_pos + D3DXVECTOR3(0.0f, 500.0f, 0.0f), D3DXVECTOR2(300.0f, 300.0f), 0, true);
		//m_pPlayerpos->BindTexture(m_pTexture);
	}

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

	for (int nCntChick = 0; nCntChick < MAX_MEMBER; nCntChick++)
	{// �����{�̃|�C���^��������
		m_pAnnoyChick[nCntChick] = NULL;
	}

	FileLoad();

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

	for (int nCntChick = 0; nCntChick < MAX_MEMBER; nCntChick++)
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

	if (m_pDispEffect != NULL)
	{
		m_pDispEffect->Uninit();
		m_pDispEffect = NULL;
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
	m_fRotOld = m_rot.y;
	m_bJumpOld = m_bJump;

	if (m_bControl)
	{//�R���g���[���L�[
		if (m_bGoal == false)
		{
			if (m_PlayerType == PLAYERTYPE_PLAYER)
			{
				if (m_State != PLAYERSTATE_SPEEDUP_S)
				{
					ControlKey();
				}
				else
				{
					UpdateKiller();
				}
			}
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

	CollisionFeed();		// �a�̓����蔻��

	CollisionEgg();			// ���Ƃ̓����蔻��

	CollisionChick();		// �Ђ悱�Ƃ̓����蔻��

	ChaseEgg();				// �������Ă��鏈��

	//CollisionCharacter();	// �L�����N�^�[���m�̓����蔻��

	ChickAppear();			// 

	ChaseAnnoyS();			// ���������Ђ悱��������

							//�}�b�v�Ƃ̓����蔻��
	bool bGoal = false;

	CRoad_Pointer::RankPoint(this, bGoal);
	bool bLand = CCOL_MESH_MANAGER::Collision(this);
	UpdateFEffect();

	if (!bLand && m_bJump)
	{//���n�����Ȃ�
	 //���n���[�V����
	}
	m_bJump = bLand;

	if (bGoal == true)
	{
		m_bGoal = true;
		m_move *= 0.0f;
		SetStateHandle(HANDLE_MAX);
		SetStateSpeed(STATE_SPEED_DOWN);
		m_nAnimnow = PLAYERANIM_NEUTRAL;
		return;
	}

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
	EffectUp();
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
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y + m_fPosY, m_pos.z);
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
	float fRot = CRoad_Pointer::NextRot(m_pos, m_pEnmPoint, m_fRoad, m_nMap, m_nNumRoad);

	fRot = fRot - m_rot.y;
	RemakeAngle(&fRot);

	m_rot.y += fRot * 0.05f;
	RemakeAngle(&m_rot.y);

	if (m_nStartCounter == m_nStartFrame)
		SetStateSpeed(STATE_SPEED_ACCEL);
	else
		m_nStartCounter++;

	UseItem();
}
//=============================================================================
// �A�C�e���̎g�p�̍X�V����
//=============================================================================
void CPlayer::UseItem(void)
{
	bool bUse = false;

	//�A�C�e�����Ȃ��Ȃ�
	if (m_nNumItem <= 0) { return; }

	int nRank = CRoad_Manager::GetManager()->GetRank(m_nPlayerNum);
	BULLET Type = (BULLET)(m_bulletType[0] % BULLET_CHICK_ATTACK);
	switch (Type)
	{//�A�C�e�����Ƃɔ��f
	case BULLET_EGG_ATTACK:	bUse = UseATK(nRank);	break;
	case BULLET_EGG_ANNOY:	bUse = UseDEF(nRank);	break;
	case BULLET_EGG_SPEED:	bUse = UseSPD(nRank);	break;
	}

	if (bUse)
	{//�A�C�e���̎g�p
		BulletEgg();
	}
}
//=============================================================================
// �W�Q�A�C�e���̍U������
//=============================================================================
bool CPlayer::UseATK(int &nRank)
{
	if (nRank > 6 && m_nNumItem > 2) { return true; }
	float fWk;

	switch (m_bulletType[0])
	{
	case BULLET_CHICK_ATTACK:	//�߂�������
		if (nRank - 1 > 0)
		{//���̏��ʂ̂��
			fWk = GetDistance(nRank - 1);
			if (fWk < 500.0f && fWk > 100.0f) { return true; }
		}
		break;
	case BULLET_EGG_ATTACK:		//�g��Ȃ�
		break;
	default:
		return true;
		break;
	}
	return false;
}
//=============================================================================
// �W�Q�A�C�e���̎g�p����
//=============================================================================
bool CPlayer::UseDEF(int &nRank)
{
	if (m_nNumItem > 2 || nRank <= 0) { return true; }
	float fWk = m_rot.y - m_fRotOld;

	switch (m_bulletType[0])
	{
	case BULLET_CHICK_ANNOY:	//�߂�������
		if (nRank - 1 > 0)
		{//���̏��ʂ̂��
			if (GetDistance(nRank - 1) < 200.0f) { return true; }
		}
		if (nRank + 1 < MAX_RACER - 1)
		{//�O�̏��ʂ̂��
			if (GetDistance(nRank + 1) < 200.0f) { return true; }
		}
		break;
	case BULLET_EGG_ANNOY:		//�Ȃ����Ă�����
		if (fWk != 0.0f && m_nNumItem > 1) { return true; }
		break;
	default:
		return true;
		break;
	}
	return false;
}
//=============================================================================
// �����A�C�e���̎g�p����
//=============================================================================
bool CPlayer::UseSPD(int &nRank)
{
	if (m_nNumItem > 2 || nRank <= 0) { return true; }

	switch (m_bulletType[0])
	{
	case BULLET_CHICK_SPEED:	//�߂�������
		return true;
		break;
	case BULLET_EGG_SPEED:		//�Ȃ����Ă�����
		break;
	default:
		return true;
		break;
	}
	return false;
}
//=============================================================================
// �w�菇�ʂƂ̋����̎擾����
//=============================================================================
float CPlayer::GetDistance(int nRank)
{
	CPlayer **pPlayer = NULL;
	pPlayer = CGame::GetPlayer();
	D3DXVECTOR3 pos = CGame::GetPlayer()[nRank]->Getpos();

	return sqrtf(powf(pos.x - m_pos.x, 2) + powf(pos.z - m_pos.z, 2));
}
//=============================================================================
// �L���[�̍X�V����
//=============================================================================
void CPlayer::UpdateKiller(void)
{
	m_fRoad = 0.0f;
	float fRot = CRoad_Pointer::NextRot(m_pos, m_pEnmPoint, m_fRoad, m_nMap, 1);

	fRot = fRot - m_rot.y;
	RemakeAngle(&fRot);

	m_rot.y += fRot * 0.2f;
	RemakeAngle(&m_rot.y);
	SetStateSpeed(STATE_SPEED_ACCEL);
}
//=============================================================================
// �n�ʌ��ʂ̍X�V����
//=============================================================================
void CPlayer::UpdateFEffect(void)
{
	CDispEffect::EFFECT Effect = CDispEffect::EFFECT_MAX;

	if (m_FEffect == CCOL_MESH::EFFECT_SWAMP)
	{//�����܂�
		Effect = CDispEffect::EFFECT_SWAMP;
		m_fPosY += (-10.0f - m_fPosY) * 0.1f;
	}
	else { m_fPosY *= 0.9f; }

	if (m_FEffect == CCOL_MESH::EFFECT_GRASS)
	{//�_�[�g
	 //Effect = CDispEffect::EFFECT_SWAMP;
	}

	if (m_FEffect == CCOL_MESH::EFFECT_DROP)
	{//����
		if (m_bDrop == false)
			m_bDrop = true;					// ������
	}

	if (m_bDrop == true)
	{// ������
		m_bControl = false;					// �R���g���[�����Ȃ�
		SetStateHandle(HANDLE_MAX);
		SetStateSpeed(STATE_SPEED_DOWN);

		if (m_nDropCounter == MAX_FALL_FADE + MAX_FALL_WAIT)
		{// ���ǂ�
			WarpNext();
		}
		else if (m_nDropCounter == ((MAX_FALL_FADE * 2.0f) + MAX_FALL_WAIT) - 1)
		{// �t�F�[�h�I��
			m_bControl = true;				// �R���g���[������
			m_nDropCounter = 0;				// ������
			m_bDrop = false;				// �����Ă��Ȃ�

			return;
		}

		m_nDropCounter++;
	}

	if (m_bJump) { Effect = CDispEffect::EFFECT_MAX; }
	if (m_pDispEffect != NULL) { m_pDispEffect->SetEffect(Effect); }
}
//=============================================================================
// ���������̃��[�v����
//=============================================================================
void CPlayer::WarpNext(void)
{
	CRoad_Pointer *pNext = m_pPoint->GetNext(0, m_nMap, 0);
	if (pNext == NULL) { return; }

	m_rot.y = pNext->GetfRotY();
	m_pos = pNext->Getpos() + D3DXVECTOR3(sinf(m_rot.y), 0.0f, cosf(m_rot.y)) * 5.0f;
	m_OldPos = m_pos;
	m_move *= 0.0f;
	m_PlayerInfo.fCountTime = 0;
	m_FEffect = CCOL_MESH::EFFECT_NORMAL;
}
//=============================================================================
// �����g�p����
//=============================================================================
void CPlayer::UseBoost(void)
{
	CGameCamera *pCamera = CGame::GetGameCamera(m_nPlayerNum);
	if (pCamera != NULL) { pCamera->UseBoost(); }
	if (m_pDispEffect != NULL) { m_pDispEffect->SetEffect(CDispEffect::EFFECT_BOOST); }
}
//=============================================================================
// �G�t�F�N�g�̍X�V����
//=============================================================================
void CPlayer::EffectUp(void)
{
	//���A���ՍX�V
	if (!m_bJump && m_nMotionType == PLAYERANIM_RUN)
	{//�n�ʂɂ��� && �������[�V����
		if (m_nCountFlame == 0)
		{//�L�[���ς�����Ȃ�
			D3DXVECTOR3 pos = m_pos + D3DXVECTOR3(sinf(m_rot.y + D3DX_PI * 0.5f), 0.0f, cosf(m_rot.y + D3DX_PI * 0.5f)) * (m_nKey == 0 ? -6.0f : 6.0f);
			//��
			CModelEffect::Create(&pos, m_move, CModelEffect::TYPE_SMOKE);
			CModelEffect::Create(&pos, m_move, CModelEffect::TYPE_SMOKE);
			CModelEffect::Create(&pos, m_move, CModelEffect::TYPE_SMOKE);

			if (m_nKey % 2 == 0)
			{//�����t�����Ȃ瑫��
				C3DPolygon::Create(C3DPolygon::TYPE_FootSteps, pos
					, D3DXVECTOR3(-m_fCTiltV * D3DX_PI * 0.5f, m_rot.y, m_fCTiltW * D3DX_PI * 0.25f))->SetTexture(m_nKey / 2, 2, 1, 1);
			}
		}
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

	if (m_bGoal == true)
	{
		m_bGoal = true;
		m_move *= 0.0f;
		SetStateHandle(HANDLE_MAX);
		SetStateSpeed(STATE_SPEED_DOWN);
		m_nAnimnow = PLAYERANIM_NEUTRAL;
		return;
	}

	//CDebugProc::Print("�ʒu : X %.2f, Y %.2f, Z %.2f\n", m_pos.x, m_pos.y, m_pos.z);

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
		if (m_PlayerInfo.fCountTime < 90)
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
						SetStateSpeed(STATE_SPEED_ACCEL);
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

		if ((bOnline == false &&
			(pInputKeyboard->GetKeyboardTrigger(DIK_W) == true ||
				pInputKeyboard->GetKeyboardTrigger(DIK_UP) == true)) ||
			pXpad->GetTrigger(INPUT_A) == true)
		{// �W�����v�L�[
			m_bJump = true;
			m_move.y += PLAYER_JUMP;
		}
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
		m_fCTiltV = (m_fTilt * -1.0f) * 0.5f;
		m_fTilt = m_fCTiltV + 0.1f;

		Vec2 = D3DXVECTOR3(sinf(m_rot.y + D3DX_PI * 0.5f), 0.0f, cosf(m_rot.y + D3DX_PI * 0.5f));
		m_fCTiltW = (m_FNor.x * Vec2.x) + (m_FNor.y * Vec2.y) + (m_FNor.z * Vec2.z) /
			(sqrtf(powf(m_FNor.x, 2) + powf(m_FNor.y, 2) + powf(m_FNor.z, 2)) * sqrtf(powf(Vec2.x, 2) + powf(Vec2.y, 2) + powf(Vec2.z, 2)));
		acosf(m_fCTiltW);
		m_fCTiltW = (m_fTilt * -1.0f) * 0.5f;
	}
	else { m_fTilt = 0.0f; }
	if (m_fTilt > 0.05f) { m_fTilt = 0.05f; }

	RemakeAngle(&m_rot.y);

	//bool bGoal = false;
	//float fRot = CRoad_Pointer::NextPoint(m_pos, m_pPoint, m_nPlayerNum, m_bDivided, bGoal, m_nMap);
	//m_rot.y += (fRot - m_rot.y);
	//if (bGoal) { m_move *= 0.0f; }

	if (m_State == PLAYERSTATE_SPEEDUP || m_State == PLAYERSTATE_SPEEDUP_S)
	{// �X�s�[�h�A�C�e�����g�����Ƃ�
		m_nCountSpeed++;

		float fTime = SPEEDUP_TIME;

		if (m_State == PLAYERSTATE_SPEEDUP_S)
		{
			fTime = SPEEDUP_TIME * 2;
		}

		if (m_nCountSpeed > fTime)
		{
			m_State = PLAYERSTATE_NORMAL;
			m_nCountSpeed = 0;
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

		if (m_State == PLAYERSTATE_NORMAL || m_State == PLAYERSTATE_SPEEDDOWN || m_State == PLAYERSTATE_SPEEDDOWN_S)
		{
			m_fSpeed = fAccel * (m_PlayerInfo.fCountTime < 90 ? (m_PlayerInfo.fCountTime / 90) : 1.0f) * (1.0f - m_fTilt * 1.5f);
		}

		//�i�s�����̐ݒ�
		m_move.x += sinf(m_rot.y) * (m_fSpeed);
		m_move.z += cosf(m_rot.y) * (m_fSpeed);

		if (m_PlayerInfo.fCountTime < 90)
			m_PlayerInfo.fCountTime++;
		else
			m_PlayerInfo.fCountTime = 90.0f;

		break;
	case STATE_SPEED_BRAKS: //�u���[�L���

							//�W�����v��ԂȂ�
		if (m_bJump == true) { break; }

		m_fSpeed = fBraks * (m_PlayerInfo.fCountTime < 90 ? (m_PlayerInfo.fCountTime / 90) : 1.0f);

		//�i�s�����̐ݒ�
		m_move.x += sinf(m_rot.y) * m_fSpeed;
		m_move.z += cosf(m_rot.y) * m_fSpeed;

		//�h��𖳌��ɂ���
		m_bShake = false;

		if (m_PlayerInfo.fCountTime < 90)
			m_PlayerInfo.fCountTime++;
		else
			m_PlayerInfo.fCountTime = 90.0f;

		break;
	case STATE_SPEED_DRIFT:	//�h���t�g���

		fAccel *= 0.345f;
		fAddRot = m_PlayerInfo.fAddRot;
		fDown *= 0.35f;

		//���郂�[�V����
		m_nAnimnow = PLAYERANIM_RUN;

		//�W�����v��ԂȂ�
		if (m_bJump == true) { break; }

		if (m_State == PLAYERSTATE_NORMAL || m_State == PLAYERSTATE_SPEEDDOWN || m_State == PLAYERSTATE_SPEEDDOWN_S)
		{
			m_fSpeed = fAccel * (m_PlayerInfo.fCountTime < 90 ? (m_PlayerInfo.fCountTime / 90) : 1.0f) * (1.0f - m_fTilt * 1.5f);
		}

		//�i�s�����̐ݒ�
		m_move.x += sinf(m_rot.y) * (m_fSpeed);
		m_move.z += cosf(m_rot.y) * (m_fSpeed);

		if (m_PlayerInfo.fCountTime < 90)
			m_PlayerInfo.fCountTime++;
		else
			m_PlayerInfo.fCountTime = 90.0f;

		break;
	case STATE_SPEED_DOWN: //�_�E�����
		if (m_bJump == true) { break; }
		
		//CDebugProc::Print("DWON***\n");

		m_fSpeed += (0.0f - m_fSpeed) * 0.5f;// ((1.0f - (m_PlayerInfo.fCountTime < 90 ? (m_PlayerInfo.fCountTime / 90) : 1.0f)) * (1.0f - m_fTilt * 1.5f));

		//�i�s�����̐ݒ�
		m_move.x += sinf(m_rot.y) * (m_fSpeed);
		m_move.z += cosf(m_rot.y) * (m_fSpeed);

		if (0 < m_PlayerInfo.fCountTime)
			m_PlayerInfo.fCountTime -= 0.3f;
		else
			m_PlayerInfo.fCountTime = 0.0f;

		break;
	default:
		//���郂�[�V����
		m_nAnimnow = PLAYERANIM_NEUTRAL;

		if (0 < m_PlayerInfo.fCountTime)
			m_PlayerInfo.fCountTime--;
		else
			m_PlayerInfo.fCountTime = 0.0f;

		break;
	}

	switch (m_State)
	{// �e��H������Ƃ�
	case PLAYERSTATE_NORMAL:
		break;

	case PLAYERSTATE_SPEEDDOWN:
		//�i�s�����̐ݒ�
		m_PlayerInfo.fCountTime = SPEED_COUNT_ANNOY;	// ����

		if (m_nCntParticle > ANNOY_PARTICLE)
		{
			D3DXVECTOR2 fPos;
			fPos.x = 30.0f - (float)(rand() % 60);
			fPos.y = 30.0f - (float)(rand() % 60);

			CParticle::Create(D3DXVECTOR3(m_pos.x + fPos.x, m_pos.y + 30.0f, m_pos.z + fPos.y),
				D3DXVECTOR3(-m_move.x, 1.0f, -m_move.z),
				D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f),
				D3DXVECTOR2(0.5f, 5.0f),
				20,
				CParticle::TEXTURE_POLYGON,
				CParticle::TYPE_DOWN);

			m_nCntParticle = 0;
		}

		//�X�s�[�h�_�E���̉�
		pSound->SetVolume(CSound::SOUND_LABEL_SE_SPEEDDOWN, 2.0f);
		pSound->PlaySound(CSound::SOUND_LABEL_SE_SPEEDDOWN);

		m_nCntParticle++;

		break;

	case PLAYERSTATE_SPEEDDOWN_S:
		//�i�s�����̐ݒ�
		m_PlayerInfo.fCountTime = SPEED_COUNT_ANNOY;	// ����

		if (m_nCntParticle > ANNOY_PARTICLE)
		{
			D3DXVECTOR2 fPos;
			fPos.x = 20.0f - (float)(rand() % 40);
			fPos.y = 20.0f - (float)(rand() % 40);

			CParticle::Create(D3DXVECTOR3(m_pos.x + fPos.x, m_pos.y + 30.0f, m_pos.z + fPos.y),
				D3DXVECTOR3(-m_move.x, 1.0f, -m_move.z),
				D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f),
				D3DXVECTOR2(0.5f, 5.0f),
				20,
				CParticle::TEXTURE_POLYGON,
				CParticle::TYPE_DOWN);

			m_nCntParticle = 0;
		}

		//�X�s�[�h�_�E���̉�
		pSound->SetVolume(CSound::SOUND_LABEL_SE_SPEEDDOWN, 2.0f);
		pSound->PlaySound(CSound::SOUND_LABEL_SE_SPEEDDOWN);

		m_nCntParticle++;
		break;

	case PLAYERSTATE_DAMAGE:
		//�i�s�����̐ݒ�
		m_PlayerInfo.fCountTime = SPEED_COUNT_DAMAGE;	// ����
		m_fSpeed = 0.0f;
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
		else if (m_State == PLAYERSTATE_SPEEDDOWN_S)
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

	//if (m_PlayerType == PLAYERTYPE_PLAYER)
	{
		/*CDebugProc::Print("m_bDamage : %d\n", m_bDamage);
		CDebugProc::Print("m_State : %s\n",
		(m_State == PLAYERSTATE_NORMAL ? "PLAYERSTATE_NORMAL" :
		(m_State == PLAYERSTATE_SPEEDUP ? "PLAYERSTATE_SPEEDUP" :
		(m_State == PLAYERSTATE_SPEEDUP_S ? "PLAYERSTATE_SPEEDUP_S" :
		(m_State == PLAYERSTATE_SPEEDDOWN ? "PLAYERSTATE_SPEEDDOWN" :
		(m_State == PLAYERSTATE_SPEEDDOWN_S ? "PLAYERSTATE_SPEEDDOWN_S" :
		(m_State == PLAYERSTATE_DAMAGE ? "PLAYERSTATE_DAMAGE" : "")))))));*/
		//CDebugProc::Print("m_nCntDamage : %d\n", m_nCntDamage);

		CDebugProc::Print("m_StateSpeed : %s\n",
			(m_StateSpeed == STATE_SPEED_ACCEL ? "STATE_SPEED_ACCEL" :
			(m_StateSpeed == STATE_SPEED_BRAKS ? "STATE_SPEED_BRAKS" :
			(m_StateSpeed == STATE_SPEED_DRIFT ? "STATE_SPEED_DRIFT" :
			(m_StateSpeed == STATE_SPEED_DOWN ? "STATE_SPEED_DOWN" :
			(m_StateSpeed == STATE_SPEED_STOP ? "STATE_SPEED_STOP" : ""))))));
		//CDebugProc::Print("m_rot x : %.1f y : %.1f z : %.1f\n", m_rot.x, m_rot.y, m_rot.z);

		CDebugProc::Print("fCountTime : %f\n", m_PlayerInfo.fCountTime);
		CDebugProc::Print("m_fSpeed : %f\n", m_fSpeed);
		CDebugProc::Print("fDown : %f\n", fDown);
	}

	//CDebugProc::Print("�A�N�Z�� : %1f\n", m_PlayerInfo.fAccel);
	CDebugProc::Print("�X�s�[�h : %1f  %1f  %1f\n", m_move.x, m_move.y, m_move.z);

	//�n���h���̏�ԍX�V
	if (m_StateHandle == HANDLE_LEFT)
	{
		if (m_StateSpeed != STATE_SPEED_STOP)
		{
			m_fAddRot -= fAddRot * (m_PlayerInfo.fCountTime < 45 ? (m_PlayerInfo.fCountTime / 45) : 1.0f);
		}
	}
	else if (m_StateHandle == HANDLE_RIGHT)
	{
		if (m_StateSpeed != STATE_SPEED_STOP)
		{
			m_fAddRot += fAddRot * (m_PlayerInfo.fCountTime < 45 ? (m_PlayerInfo.fCountTime / 45) : 1.0f);
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

	EggJump();

	m_fAddRot += (0.0f - m_fAddRot) * PLAYER_DOWNROT;

	//CDebugProc::Print("addrot : %f\n", m_fAddRot);
	//CDebugProc::Print("fSpeed : %f\n", m_fSpeed);
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
		/*else
		{
		m_PlayerInfo.fCountTime = 0;
		}*/

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
					m_PlayerInfo.fCountTime = 0;
					break;
				}
			}
		}

		//Next�Ɏ���Scene������
		pScene = pSceneNext;
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
// �a�Ƃ̓����蔻��
//=============================================================================
void CPlayer::EggAppear(CFeed *pFeed)
{
	CSound *pSound = CManager::GetSound();

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

			//�����Y�܂�鉹
			pSound->PlaySound(CSound::SOUND_LABEL_SE_EGGLAY);
			pSound->SetVolume(CSound::SOUND_LABEL_SE_EGGLAY, 7.0f);
			pSound->SetFrequency(CSound::SOUND_LABEL_SE_EGGLAY, 0.7f);
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

			//�����Y�܂�鉹
			pSound->PlaySound(CSound::SOUND_LABEL_SE_EGGLAY);
			pSound->SetVolume(CSound::SOUND_LABEL_SE_EGGLAY, 7.0f);
			pSound->SetFrequency(CSound::SOUND_LABEL_SE_EGGLAY, 0.7f);
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

			//�����Y�܂�鉹
			pSound->PlaySound(CSound::SOUND_LABEL_SE_EGGLAY);
			pSound->SetVolume(CSound::SOUND_LABEL_SE_EGGLAY, 7.0f);
			pSound->SetFrequency(CSound::SOUND_LABEL_SE_EGGLAY, 0.7f);
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

	//CDebugProc::Print("%d : %d\n", m_nNumEgg, m_nNumChick);

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
				m_pChick[0]->GetPos().y,
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
			m_pChick[1]->GetPos().y,
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
			m_pChick[2]->GetPos().y,
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
	CSound *pSound = CManager::GetSound();

	//if (m_State != PLAYERSTATE_DAMAGE && m_State != PLAYERSTATE_SPEEDUP && m_State != PLAYERSTATE_SPEEDUP_S)
	//{// �_���[�W��H����Ă���Ƃ��͎g���Ȃ�
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
				UseBoost();
				m_State = PLAYERSTATE_SPEEDUP;
				m_fSpeed += SPEED_CHICK;

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

				// ����
			case CChick::TYPE_SPEED_S:
				UseBoost();
				m_State = PLAYERSTATE_SPEEDUP_S;
				m_fSpeed += SPEED_CHICK;

				m_pChick[0]->Uninit();
				m_pChick[0] = NULL;
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
				UseBoost();
				m_State = PLAYERSTATE_SPEEDUP;
				m_fSpeed += SPEED_EGG;
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

		pSound->PlaySound(CSound::SOUND_LABEL_SE_THROW);
		pSound->SetVolume(CSound::SOUND_LABEL_SE_THROW, 3.0f);
		pSound->SetFrequency(CSound::SOUND_LABEL_SE_THROW, 0.5f);
	}
	//}
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
							if (m_State != PLAYERSTATE_DAMAGE)
							{
								m_bDamage = true;
								m_nCntDamage = 0;
								m_State = PLAYERSTATE_DAMAGE;

								D3DXVECTOR2 fSize;

								for (int nCntParticle = 0; nCntParticle < COL_PARTICLE; nCntParticle++)
								{
									fSize.x = 5.0f + (float)(rand() % 5);
									fSize.y = 5.0f + (float)(rand() % 5);

									CParticle::Create(m_pos,
										D3DXVECTOR3(sinf((rand() % 628) / 100.0f) * ((rand() % 5 + 1)), cosf((rand() % 628) / 100.0f) * ((rand() % 5 + 1)), cosf((rand() % 628) / 100.0f) * ((rand() % 5 + 1))),
										D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
										fSize,
										20,
										CParticle::TEXTURE_STAR,
										CParticle::TYPE_NORMAL);
								}
							}
							pEgg->Uninit();	// ���폜
							break;

							// ����
						case CEgg::EGGTYPE_ANNOY:
							if (m_bDamage == false)
							{
								m_bDamage = true;
								m_nCntDamage = 0;
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
							if (m_State != PLAYERSTATE_DAMAGE)
							{
								m_bDamage = true;
								m_nCntDamage = 0;
								m_State = PLAYERSTATE_DAMAGE;

								D3DXVECTOR2 fSize;

								for (int nCntParticle = 0; nCntParticle < COL_PARTICLE; nCntParticle++)
								{
									fSize.x = 5.0f + (float)(rand() % 5);
									fSize.y = 5.0f + (float)(rand() % 5);

									CParticle::Create(m_pos,
										D3DXVECTOR3(sinf((rand() % 628) / 100.0f) * ((rand() % 5 + 1)), cosf((rand() % 628) / 100.0f) * ((rand() % 5 + 1)), cosf((rand() % 628) / 100.0f) * ((rand() % 5 + 1))),
										D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
										fSize,
										20,
										CParticle::TEXTURE_STAR,
										CParticle::TYPE_NORMAL);
								}
							}
							pChick->Uninit();	// �Ђ悱�폜
							break;

							// �����U��
						case CChick::TYPE_ATTACK_S:
							// �_���[�W��Ԃɂ���
							if (m_State != PLAYERSTATE_DAMAGE && pChick->GetAttackS() == true)
							{
								m_bDamage = true;
								m_nCntDamage = 0;
								m_State = PLAYERSTATE_DAMAGE;

								D3DXVECTOR2 fSize;

								for (int nCntParticle = 0; nCntParticle < COL_PARTICLE_S; nCntParticle++)
								{
									fSize.x = 5.0f + (float)(rand() % 5);
									fSize.y = 5.0f + (float)(rand() % 5);

									CParticle::Create(m_pos,
										D3DXVECTOR3(sinf((rand() % 628) / 100.0f) * ((rand() % 5 + 1)), cosf((rand() % 628) / 100.0f) * ((rand() % 5 + 1)), cosf((rand() % 628) / 100.0f) * ((rand() % 5 + 1))),
										D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
										fSize,
										20,
										CParticle::TEXTURE_STAR,
										CParticle::TYPE_NORMAL);
								}
							}
							break;

							// ����
						case CChick::TYPE_ANNOY:
							if (m_bDamage == false)
							{
								m_bDamage = true;
								m_nCntDamage = 0;
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

			int nRank = rand() % 101;
			int nGameTime = (CGame::GetGameCounter() - START_SET_TIME) / 60;

			CChick::TYPE type = CChick::TYPE_MAX;

			if (nGameTime < 60)
			{
				// �^�C�v�ݒ�
				type = SetChickType(type, false);
			}
			else if (nGameTime >= 60)
			{
				if (CGame::GetRanking(m_nPlayerNum) < 3)
				{// 4�ʂ���̏ꍇ
				 // �^�C�v�ݒ�
					type = SetChickType(type, false);
				}
				else if (CGame::GetRanking(m_nPlayerNum) >= 3)
				{// 4�ʂ�艺�̏ꍇ
					for (int nCntRank = 0; nCntRank < (MAX_MEMBER / 2) + 1; nCntRank++)
					{
						if (CGame::GetRanking(m_nPlayerNum) == nCntRank + 3)
						{
							if (nRank <= 10 * ((2 + nGameTime) + nCntRank))
							{// �����ق�
							 // �^�C�v�ݒ�
								type = SetChickType(type, true);
							}
							else if (nRank > 10 * ((2 + nGameTime) + nCntRank))
							{// ���ʂ̂ق�
							 // �^�C�v�ݒ�
								type = SetChickType(type, false);
							}

							break;
						}
					}
				}
			}

			if (m_pEgg[0]->GetType() == CEgg::EGGTYPE_ATTACK)
			{
				m_bulletType[m_nNumChick] = BULLET_CHICK_ATTACK;
			}
			else if (m_pEgg[0]->GetType() == CEgg::EGGTYPE_ANNOY)
			{
				m_bulletType[m_nNumChick] = BULLET_CHICK_ANNOY;
			}
			else if (m_pEgg[0]->GetType() == CEgg::EGGTYPE_SPEED)
			{
				m_bulletType[m_nNumChick] = BULLET_CHICK_SPEED;
			}

			if (type != CChick::TYPE_MAX)
			{
				m_pChick[m_nNumChick] = CChick::Create(m_pos,
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					CHICK_SCALE,
					type,
					CChick::BULLETTYPE_PLAYER,
					CChick::STATE_CHASE,
					m_nPlayerNum);
				//m_pChick[m_nNumChick] = CChick::Create(m_pos,
				//	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				//	CHICK_SCALE,
				//	CChick::TYPE_ANNOY_S,
				//	CChick::BULLETTYPE_PLAYER,
				//	CChick::STATE_CHASE,
				//	m_nPlayerNum);
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

//=============================================================================
// �Ђ悱�̎�ސݒ�
//=============================================================================
CChick::TYPE CPlayer::SetChickType(CChick::TYPE type, bool bStrong)
{
	if (m_pEgg[0]->GetType() == CEgg::EGGTYPE_ATTACK)
	{// �U����
		if (bStrong == false)
		{// ����
			type = CChick::TYPE_ATTACK;
		}
		else
		{// ����
			type = CChick::TYPE_ATTACK_S;
		}
	}
	else if (m_pEgg[0]->GetType() == CEgg::EGGTYPE_ANNOY)
	{// �W�Q��
		if (bStrong == false)
		{// ����
			type = CChick::TYPE_ANNOY;
		}
		else
		{// ����
			type = CChick::TYPE_ANNOY_S;
		}
	}
	else if (m_pEgg[0]->GetType() == CEgg::EGGTYPE_SPEED)
	{// ������
		if (bStrong == false)
		{// ����
			type = CChick::TYPE_SPEED;
		}
		else
		{// ����
			type = CChick::TYPE_SPEED_S;
		}
	}

	return type;
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
		CChick::Create(D3DXVECTOR3(pos.x + ((FALL_CHICK_RANGE / 2) - fx), pos.y + (nCntChick * 100.0f), pos.z + ((FALL_CHICK_RANGE / 2) - fz)),
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

				if (pChick->GetState() == CChick::STATE_BULLET && pChick->GetType() == CChick::TYPE_ATTACK_S && pChick->GetAttackS() == false && pChick->GetDis() == true)
				{
					pChick->SetAttackS(true);
					pChick->SetDis(false);
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
						D3DXVECTOR3(pPlayer[nCntPlayer]->GetPos().x, pPlayer[nCntPlayer]->GetPos().y + 50.0f, pPlayer[nCntPlayer]->GetPos().z),
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

					pPlayer[nCntPlayer]->SetPlayerState(PLAYERSTATE_SPEEDDOWN_S);

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
		m_pAnnoyChick[m_nPlayerNum]->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y + 60.0f, m_pos.z));	// ������Ɉʒu�X�V
		m_pAnnoyChick[m_nPlayerNum]->SetRot(D3DXVECTOR3(m_rot.x, m_rot.y + -D3DX_PI, m_rot.z));		// ��������ʂɌ�����

																									// �H����Ă��鎞�Ԃ��J�E���g
		m_nAnnoySTimer++;

		if (m_nAnnoySTimer > SPEEDDOWN_TIME)
		{// ��莞�Ԃ�������
			m_nAnnoySTimer = 0;
			m_pAnnoyChick[m_nPlayerNum]->Uninit();
			m_pAnnoyChick[m_nPlayerNum] = NULL;
		}

		if (m_bDamage == false)
		{
			m_bDamage = true;
			m_nCntDamage = 0;
			m_State = PLAYERSTATE_SPEEDDOWN_S;
		}
	}
}

//=============================================================================
// �����W�����v������
//=============================================================================
void CPlayer::EggJump(void)
{
	if (m_bControl == true)
	{
		if (m_bJumpFlag == false)
		{
			if (m_bJump == true)
			{
				m_bJumpFlag = true;
				m_bJumpSave = true;
			}
			else
			{
				m_bJumpSave = false;
			}
		}
		else
		{
			m_bJumpSave = false;

			if (m_bJump == false)
			{
				m_bJumpFlag = false;
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
// CPU�̃R�[�X���ύX
//=============================================================================
void CPlayer::ChangeRoad(void)
{
	int nRank = CRoad_Manager::GetManager()->GetRank(m_nPlayerNum);
	if (nRank < 3) { m_nNumRoad = 1; }
	else
	{
		if (nRank < 6) { m_nNumRoad = CServer::Rand() % 2; }
		else { m_nNumRoad = 0; }
	}

	float fWKRoad = m_fRoad;
	float fPlus = (float)(CServer::Rand() % 4) * 7.5f;
	bool bPlus;
	if (m_nMap == 0) { bPlus = (m_nNumRoad == 0 ? false : true); }
	else { bPlus = (m_nNumRoad == 0 ? true : false); }

	if (bPlus)
	{
		if (m_fRoad + fPlus <= 70.0f) { m_fRoad += fPlus; }
		else { m_fRoad -= fPlus; }
	}
	else
	{
		if (m_fRoad - fPlus >= -70.0f) { m_fRoad -= fPlus; }
		else { m_fRoad += fPlus; }
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
	/*CDebugProc::Print(" Num�L�[  : (%d)\n", m_nKey);
	CDebugProc::Print(" m_nCountFlame  : (%d)\n", m_nCountFlame);*/

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