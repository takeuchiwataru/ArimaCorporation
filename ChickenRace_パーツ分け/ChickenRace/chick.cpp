//=============================================================================
//
// �Ђ悱�̏��� [chick.cpp]
// Author : ���R���
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "scene.h"
#include "chick.h"
#include "manager.h"
#include "object.h"
#include "meshfield.h"
#include "player.h"
#include "fade.h"
#include "shadow.h"
#include "tutorial.h"
#include "ColMesh.h"
#include "egg.h"
#include "particle.h"
#include "model.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FILE_TEXTURE			("data\\TEXTURE\\modeltex\\�j���g��.jpg")	//�e�N�X�`���̓ǂݍ���

#define MODEL_SPEED				(5.0f)
#define OBJCT_ANGLE_REVISION	(0.2f)		// �p�x�␳
#define EFFECT_HIGHT			(250.0f)	// �G�~�b�^�[�̍���
#define FOUNTAIN_UP				(20.0f)		// �����̏㏸������l
#define DISTIME					(100)		// ������܂ł̎���
#define CHICK_SPEED				(20.0f)		// �Ђ悱�����ł��X�s�[�h
#define ANNOY_RANGE				(120.0f)	// ����������͈�
#define ATTACK_RANGE			(200.0f)	// �͈͍U���͈̔�
#define CHICK_JUMP				(3.5f)		// �W�����v��
#define CHICK_FALL_TIME			(30)		// �Ђ悱�������Ă���^�C�~���O�̊Ԋu
#define CHICK_FALL_SPEED		(12.0f)		// �����Ă���Ђ悱�̑���
#define CHICK_PARTICLE			(30)		// �p�[�e�B�N���̐�

//�X�V�͈�
#define FOUNTAIN_LENGTH			(15000)		//�����̍X�V�͈�
#define LEAF_LENGTH				(10500)		//�t�̍X�V����
#define WOOD_LENGTH				(8000)		//�؂��r���{�[�h�ɓ���ւ��鋗��
#define FLOWER_LENGTH			(5500)		//�Ԓd�̕`��͈�

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
D3DXVECTOR3				CChick::m_VtxMaxModel[MAX_CHICK] = {};
D3DXVECTOR3				CChick::m_VtxMinModel[MAX_CHICK] = {};

CChick::KEY_INFO		*CChick::m_pKeyInfo[MAX_CHICK_MOTION] = {};		//�L�[���ւ̃|�C���^
int						CChick::m_nNumParts = 0;						//�p�[�c��
int						CChick::m_aIndexParent[MAX_CHICK_PARTS] = {};	//�e�̃C���f�b�N�X
CChick::KEY				CChick::m_aKayOffset[MAX_CHICK_PARTS] = {};		//�I�t�Z�b�g���
CChick::MOTION_INFO		CChick::m_aMotionInfo[MAX_CHICK_MOTION] = {};	//���[�V�������

//--------------------------------------------
//�O���[�o���ϐ�
//--------------------------------------------
int g_nChickNumModel;
char g_aChickFileNameModel[MAX_CHICK_PARTS][256];

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CChick::CChick() : CScene(EGG_PRIOTITY, CScene::OBJTYPE_CHICK)
{
	//�l�̏�����
	m_apModel = NULL;

	m_scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �傫��
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fDestAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fDiffAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_FNor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fHeight = 0.0f;
	m_fLength = 0.0f;
	m_nRank = 0;
	m_nNumPlayer = 0;
	m_nDisTimer = 0;
	m_DestRank = 0;
	m_nMap = 0;
	m_nExplosion = 0;
	m_nKey = 0;
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CChick::~CChick() {}

//===============================================================================
//�@����
//===============================================================================
CChick * CChick::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type, BULLETTYPE bulletType, STATE state, int nNumPlayer)
{
	CChick *pChick = NULL;

	// NULL�`�F�b�N
	if (pChick == NULL)
	{// �������̓��I�m��

		pChick = new CChick;

		if (pChick != NULL)
		{
			// �T�C�Y����
			pChick->m_scale = scale;
			// ���̎�ނ���
			pChick->m_type = type;
			// �e�̎�ނ���
			pChick->m_bulletType = bulletType;
			// �I�u�W�F�N�g�N���X�̐���
			pChick->Init();
			//�ʒu�̑��
			pChick->m_pos = pos;
			//�����̑��
			pChick->m_fHeight = pos.y;
			// ��]�𔽉f
			pChick->SetRot(rot);
			// ���ʂ̗���
			pChick->m_nNumPlayer = nNumPlayer;
			// ��Ԃ̐ݒ�
			pChick->m_state = state;
		}
	}

	return pChick;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CChick::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�ϐ��̏�����
	m_pObjBill = NULL;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fHeight = 0.0f;
	m_fDestAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fDiffAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_FNor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = 3.0f;
	m_bJump = false;
	m_bDis = true;
	m_state = STATE_NORMAL;
	m_nRank = 0;
	m_nNumPlayer = 0;
	m_nDisTimer = 0;
	m_DestRank = -1;
	m_nMap = 0;
	m_bAttackS = false;
	m_bExplosion = false;
	m_nExplosion = 0;
	m_nAnimnow = CHICK_ANIM_NEUTRAL;	//�j���[�g�������
	m_nCountFlame = 0;

	CModel::ParentModel(m_apModel, CModel::TYPE_CHICK);
	int &nMaxModel = CModel::GetnModelMax(CModel::TYPE_CHICK);
	for (int nCountIndex = 0; nCountIndex < nMaxModel; nCountIndex++)
	{// �����{�̃|�C���^��������
		if (m_aIndexParent[nCountIndex] == -1)
		{
			//���f���̐e���w��
			m_apModel[nCountIndex]->SetParent(NULL);
		}
		else
		{
			//���f���̐e���w��
			m_apModel[nCountIndex]->SetParent(m_apModel[m_aIndexParent[nCountIndex]]);
		}

		m_apModel[nCountIndex]->SetPos(D3DXVECTOR3(m_pos.x + m_aKayOffset[nCountIndex].fposX,
			m_pos.y + m_aKayOffset[nCountIndex].fposY,
			m_pos.z + m_aKayOffset[nCountIndex].fposZ));
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CChick::Uninit(void)
{
	int &nMaxModel = CModel::GetnModelMax(CModel::TYPE_CHICK);
	if (m_apModel != NULL)
	{
		for (int nCount = 0; nCount < nMaxModel; nCount++)
		{
			if (m_apModel[nCount] != NULL)
			{
				//3D���f����Uninit
				m_apModel[nCount]->Uninit();
				delete m_apModel[nCount];
				m_apModel[nCount] = NULL;
			}
		}
		delete[] m_apModel;
		m_apModel = NULL;
	}

	//���S�t���O�𗧂Ă�
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CChick::Update(void)
{
	//m_pos = CModel3D::GetPosition();
	m_posOld = m_pos;	//�O��̈ʒu��ۑ�����

	// �Ђ悱�̓���
	if (Move()) { return; }

	m_nMotionType = m_nAnimnow;

	//���[�V�����X�V
	UpdateMotion();

	//CDebugProc::Print("m_rot x : %.1f y : %.1f z : %.1f\n", m_rot.x, m_rot.y, m_rot.z);
}
//=============================================================================
// �`�揈��
//=============================================================================
void CChick::Draw(void)
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

	int &nMaxParts = CModel::GetnModelMax(CModel::TYPE_CHICK);
	for (int nCount = 0; nCount < nMaxParts; nCount++)
	{//���f���̕`��
		if (m_apModel[nCount] != NULL)
		{
			//�`�悷��
			m_apModel[nCount]->Draw(1.0f);
		}
	}
}

//===============================================================================
// X�t�@�C���̓ǂݍ���
//===============================================================================
void CChick::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	int			nNumVtx = 0;	//���_��
	//���f���̃I�t�Z�b�g�Ɠǂݍ���
	FileLoad();

	//���f���̍ő�l�E�ŏ��l���擾����
	for (int nCntModel = 0; nCntModel < MAX_CHICK; nCntModel++)
	{
		m_VtxMaxModel[nCntModel] = D3DXVECTOR3(-10000, -10000, -10000);	//�ő�l
		m_VtxMinModel[nCntModel] = D3DXVECTOR3(10000, 10000, 10000);	//�ŏ��l

		CModel3D::MODEL_TYPE type;

		switch (nCntModel)
		{
		case TYPE_ATTACK:	type = CModel3D::MODEL_TYPE_CHICK;	break;
		case TYPE_ANNOY:	type = CModel3D::MODEL_TYPE_CHICK;	break;
		case TYPE_SPEED:	type = CModel3D::MODEL_TYPE_CHICK;	break;
		case TYPE_ATTACK_S:	type = CModel3D::MODEL_TYPE_CHICK;	break;
		case TYPE_ANNOY_S:	type = CModel3D::MODEL_TYPE_CHICK;	break;
		case TYPE_SPEED_S:	type = CModel3D::MODEL_TYPE_CHICK;	break;
		}

		//LPD3DXMESH &Mesh = CModel3D::MeshLoad(type);
	}
}

//===============================================================================
// �e�N�X�`���̔j��
//===============================================================================
void CChick::UnLoad(void)
{

}

//===============================================================================
// �Ђ悱�̓���
//===============================================================================
bool CChick::Move(void)
{
	// �g�����Ƃ�
	if (Item()) { return true; }

	if ((m_type != TYPE_ANNOY && m_type != TYPE_ATTACK_S && m_type != TYPE_ANNOY_S) || m_state != STATE_BULLET)
	{
		m_move.y -= cosf(0) * 0.1f;
	}

	m_pos.x += m_move.x;
	m_pos.y += m_move.y;
	m_pos.z += m_move.z;

	if (m_bDis == false)
	{
		m_nDisTimer++;

		if (m_nDisTimer > DISTIME)
		{// ����
			m_nDisTimer = 0;
			Uninit();
			return true;
		}
	}

	if ((m_type != TYPE_ATTACK_S || m_type != TYPE_ANNOY_S) && m_state == STATE_CHASE)
	{
		//�}�b�v�Ƃ̓����蔻��
		CPlayer **pPlayer = NULL;
		pPlayer = CGame::GetPlayer();

		m_fHeight = CCOL_MESH_MANAGER::GetHeight(m_pos, pPlayer[m_nNumPlayer]->GetnMap());

		if (m_pos.y < m_fHeight)
		{
			m_move.y = 0.0f;
			m_pos.y = m_fHeight/* + 10.0f*/;
			//�W�����v�̏�Ԑݒ�
			m_bJump = false;
		}
	}
	else if ((m_type == TYPE_ATTACK || m_type == TYPE_ATTACK_S) && m_state == STATE_BULLET)
	{
		//�}�b�v�Ƃ̓����蔻��
		CPlayer **pPlayer = NULL;
		pPlayer = CGame::GetPlayer();

		m_fHeight = CCOL_MESH_MANAGER::GetHeight(m_pos, pPlayer[m_nNumPlayer]->GetnMap());

		if (m_pos.y < m_fHeight)
		{
			m_move.y = 0.0f;
			m_pos.y = m_fHeight;
			//�W�����v�̏�Ԑݒ�
			m_bJump = false;

			if (m_type == TYPE_ATTACK_S)
			{
				if (m_bExplosion == false)
				{
					m_bExplosion = true;
					D3DXVECTOR2 fSize;

					for (int nCntParticle = 0; nCntParticle < CHICK_PARTICLE; nCntParticle++)
					{// �p�[�e�B�N������
						fSize.x = 5.0f + (float)(rand() % 5);
						fSize.y = 5.0f + (float)(rand() % 5);

						CParticle::Create(m_pos,
							D3DXVECTOR3(sinf((rand() % 628) / 100.0f) * ((rand() % 5 + 1)), cosf((rand() % 628) / 100.0f) * ((rand() % 5 + 1)), cosf((rand() % 628) / 100.0f) * ((rand() % 5 + 1))),
							D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
							fSize,
							20,
							CParticle::TEXTURE_STAR,
							CParticle::TYPE_NORMAL);
					}
				}
			}
		}
	}

	return false;
	//CDebugProc::Print("%.1f\n", m_move.y);
}

//===============================================================================
// �Ђ悱���g�p�����Ƃ��̓���
//===============================================================================
bool CChick::Item(void)
{
	if (m_state == STATE_BULLET)
	{// �e�̏�Ԃ̎�
		float fHeight = 0.0f;
		CPlayer **pPlayer = NULL;

		switch (m_type)
		{
			// �U��
		case TYPE_ATTACK:
			break;

			// �U��
		case TYPE_ATTACK_S:
			if (m_bExplosion == true)
			{// ������
				m_nExplosion++;

				if (m_nExplosion > EXPLOSION_TIME)
				{// �폜
					m_nExplosion = 0;
					Uninit();
					return true;
				}
			}
			break;

			// ����������
		case TYPE_ANNOY:
			pPlayer = CGame::GetPlayer();

			m_pos = D3DXVECTOR3(pPlayer[m_nNumPlayer]->GetPos().x, pPlayer[m_nNumPlayer]->GetPos().y + 60.0f, pPlayer[m_nNumPlayer]->GetPos().z);
			m_rot = D3DXVECTOR3(pPlayer[m_nNumPlayer]->GetRot().x, pPlayer[m_nNumPlayer]->GetRot().y, pPlayer[m_nNumPlayer]->GetRot().z);

			break;
		}
		// ���ł�����
		Bullet();
	}
	return false;
}

//===============================================================================
// �����蔻��
//===============================================================================
bool CChick::CollisionChick(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld)
{
	//�������Ă��邩�������ĂȂ���
	bool bHit = false;
	
	if (m_state == STATE_BULLET)
	{
		// �e����̎擾
		float fDepth = PLAYER_DEPTH;

		if (m_type == TYPE_ANNOY)
		{
			fDepth = ANNOY_RANGE;
		}
		if (m_bExplosion == true)
		{
			fDepth = 80.0f;
		}

		//�����v�Z
		float fDistance = sqrtf(powf(pPos->x - m_pos.x, 2) + powf(pPos->z - m_pos.z, 2));

		if (fDistance < 20.0f + fDepth)
		{//�����`�F�b�N
			bHit = true;
		}

		if (bHit == true && m_type == TYPE_ATTACK)
		{
			m_bDis = false;
		}
	}
	return bHit;
}

//=============================================================================
// ��������
//=============================================================================
float CChick::SetHeight(void)
{
	float fHeight = 0.0f;

	CScene *pScene = CScene::GetTop(MESH_PRIOTITY);

	//NULL�`�F�b�N
	while (pScene != NULL)
	{
		//Update��Uninit����Ă��܂��ꍇ�@Next��������\�������邩��Next�Ƀf�[�^���c���Ă���
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetObjType() == CScene::OBJTYPE_GROUND)
		{//�^�C�v���n�ʂ�������
			CMeshField *pField = (CMeshField*)pScene;

			if (pField->OnField(m_pos, 0.0f))
			{// �X�΂̌v�Z
				fHeight = pField->GetHeightMesh(m_pos);

				if (m_bJump == false || (m_bJump == true && m_fHeight < fHeight))
				{
					m_fHeight = fHeight + 15.0f;				//�n�ʂ̍������擾
					m_move.y = 0.0f;					//�ړ��ʂ�����������

														//�W�����v�̏�Ԑݒ�
					m_bJump = false;

					break;
				}
			}
		}
		//Next�Ɏ���Scene������
		pScene = pSceneNext;
	}

	return m_fHeight;
}

//=============================================================================
// �W�����v
//=============================================================================
void CChick::Jump(float fJump)
{
	// �W�����v
	if (m_bJump == false)
	{// �W�����v���Ă��Ȃ�
		m_bJump = true;
		m_move.y += fJump;
	}
}

//=============================================================================
// ���ł�
//=============================================================================
void CChick::Bullet(void)
{
	if (m_bulletType == BULLETTYPE_PLAYER)
	{
		switch (m_type)
		{
			// �U���Ђ悱
		case TYPE_ATTACK:
			// 1�O�̃v���C���[�ɔ��ł���
			Attack();
			break;

			// �����U���Ђ悱
		case TYPE_ATTACK_S:
			// 1�ʂɑS�̍U��
			AttackS();
			break;

			// ���������Ђ悱
		case TYPE_ANNOY_S:
			// 1�ʂɑS�̍U��
			AnnoyS();
			break;
		}
	}

}

//=============================================================================
// �p�x�̒���
//=============================================================================
float CChick::AdjustAngle(float rot)
{
	if (rot > D3DX_PI)
	{
		rot -= D3DX_PI * 2.0f;
	}
	if (rot < -D3DX_PI)
	{
		rot += D3DX_PI * 2.0f;
	}

	return rot;
}

//=============================================================================
// �U���Ђ悱
//=============================================================================
void CChick::Attack(void)
{
	CPlayer **pPlayer = CGame::GetPlayer();

	if (m_DestRank >= 0)
	{
		// �ړI�̊p�x
		m_fDestAngle.y = atan2f(pPlayer[m_DestRank]->GetPos().x - m_pos.x, pPlayer[m_DestRank]->GetPos().z - m_pos.z);

		// ����
		m_fDiffAngle.y = m_fDestAngle.y - m_rot.y;

		m_fDiffAngle.y = AdjustAngle(m_fDiffAngle.y);

		m_rot.y += m_fDiffAngle.y * 0.5f;

		m_rot.y = AdjustAngle(m_rot.y);

		//���f���̈ړ�	���f���̈ړ�����p�x(�J�����̌��� + �p�x) * �ړ���
		m_move.x = sinf(m_rot.y) * CHICK_SPEED;
		m_move.z = cosf(m_rot.y) * CHICK_SPEED;

		CDebugProc::Print("m_DestRank : %d\n", m_DestRank);
		CDebugProc::Print("m_DestRank : %.1f  %.1f\n", m_move.x, m_move.z);
	}
	else
	{
		//���f���̈ړ�	���f���̈ړ�����p�x(�J�����̌��� + �p�x) * �ړ���
		m_move.x = sinf(m_rot.y) * -CHICK_SPEED * 0.6f;
		m_move.z = cosf(m_rot.y) * -CHICK_SPEED * 0.6f;
	}

}

//=============================================================================
// �U���Ђ悱(����)
//=============================================================================
void CChick::AttackS(void)
{
	CPlayer **pPlayer = CGame::GetPlayer();

	if (m_nRank != 0)
	{
		if (m_bAttackS == false)
		{// �オ���Ă���
			m_move.x = pPlayer[m_nNumPlayer]->GetMove().x;
			m_move.y = 4.0f;
			m_move.z = pPlayer[m_nNumPlayer]->GetMove().z;

			if (m_pos.y > pPlayer[m_nNumPlayer]->GetPos().y + 300.0f)
			{// 覐΂ɂȂ�^�C�~���O
				int fx = rand() % FALL_CHICK_RANGE;
				int fz = rand() % FALL_CHICK_RANGE;

				m_pos = D3DXVECTOR3(pPlayer[m_DestRank]->GetPos().x + ((FALL_CHICK_RANGE / 2) - fx),
					pPlayer[m_DestRank]->GetPos().y + 100.0f,
					pPlayer[m_DestRank]->GetPos().z + ((FALL_CHICK_RANGE / 2) - fz));

				if (pPlayer[m_nNumPlayer]->GetCntChick() < CHICK_FALL_NUM)
				{// 5�C�܂ŏo��
				 // ������Ђ悱�o��
					pPlayer[m_nNumPlayer]->FallChicks(D3DXVECTOR3(
						(sinf(pPlayer[m_DestRank]->GetRot().y + D3DX_PI)) + pPlayer[m_DestRank]->GetPos().x,
						pPlayer[m_DestRank]->GetPos().y,
						(cosf(pPlayer[m_DestRank]->GetRot().y + D3DX_PI)) + pPlayer[m_DestRank]->GetPos().z));
				}

				m_bAttackS = true;
				pPlayer[m_nNumPlayer]->SetCntChick(0);
			}
		}
		else
		{
			// �ړ��ʂ�ݒ�
			m_move.y = -CHICK_FALL_SPEED;
		}
	}
}

//=============================================================================
// �����Ђ悱(����)
//=============================================================================
void CChick::AnnoyS(void)
{
	CPlayer **pPlayer = CGame::GetPlayer();

	if (m_bAttackS == false)
	{
		m_move.x = pPlayer[m_nNumPlayer]->GetMove().x;
		m_move.y = 3.0f;
		m_move.z = pPlayer[m_nNumPlayer]->GetMove().z;

		pPlayer[m_nNumPlayer]->AnnoyChicks();
		m_bAttackS = true;
	}
}
//=============================================================================
// �v���C���[�̃��[�V����
//=============================================================================
void CChick::UpdateMotion(void)
{
	//���[�V����
	KEY *pKey, *pNextKey;
	float fRateMotion;
	float fDiffMotion;
	D3DXVECTOR3 rotmotion;
	D3DXVECTOR3 posmotion;

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
			m_apModel[nCntParts]->SetPos(D3DXVECTOR3(m_aKayOffset[nCntParts].fposX + posmotion.x,
				m_aKayOffset[nCntParts].fposY + posmotion.y,
				m_aKayOffset[nCntParts].fposZ + posmotion.z));
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
void CChick::FileLoad(void)
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

#if 1
	//�t�@�C�����J�� �ǂݍ���
	pFile = fopen(FILE_NAME_CHICK, "r");
	//NULL�`�F�b�N
	if (pFile != NULL)
	{
		for (int nCntMotion = 0; nCntMotion < MAX_CHICK_MOTION; )
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
				g_nChickNumModel = atoi(pStrcur);

				for (int nCntModel = 0; nCntModel < g_nChickNumModel; nCntModel++)
				{
					//������̐擪��ݒ�
					pStrcur = ReadLine(pFile, &aLine[0]);
					//����������߂�
					strcpy(aStr, pStrcur);
					if (memcmp(pStrcur, "MODEL_FILENAME = ", strlen("MODEL_FILENAME = ")) == 0)
					{
						int nNumber = nCntModel + CModel::PARTS_CHICK_BODY;
						LPD3DXBUFFER &m_pBuffMat = CModel::GetpBuffMat(nNumber);
						DWORD &m_nNumMat = CModel::GetnNumMat(nNumber);
						LPD3DXMESH &m_pMesh = CModel::GetpMesh(nNumber);

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
						strcpy(&g_aChickFileNameModel[nCntModel][0], aStr);

						// X�t�@�C���̓ǂݍ���
						D3DXLoadMeshFromX(&g_aChickFileNameModel[nCntModel][0],
							D3DXMESH_SYSTEMMEM,
							pDevice,
							NULL,
							&m_pBuffMat,
							NULL,
							&m_nNumMat,
							&m_pMesh);

						//CModel::SetModelType(CModel::TYPE_CHICK_BODY + nCntModel);
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
char *CChick::ReadLine(FILE *pFile, char *pDst)
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
char * CChick::GetLineTop(char * pStr)
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
int CChick::PopString(char * pStr, char * pDest)
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