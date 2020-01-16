//=============================================================================
//
// ���̏��� [egg.cpp]
// Author : ���R���
//
//=============================================================================
#include "scene.h"
#include "egg.h"
#include "manager.h"
#include "object.h"
#include "meshfield.h"
#include "player.h"
#include "fade.h"
#include "shadow.h"
#include "tutorial.h"
#include "ColMesh.h"
#include "particle.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_SPEED				(5.0f)
#define OBJCT_ANGLE_REVISION	(0.2f)		// �p�x�␳
#define EFFECT_HIGHT			(250.0f)	// �G�~�b�^�[�̍���
#define FOUNTAIN_UP				(20.0f)		// �����̏㏸������l
#define EGG_SPEED				(15.0f)		// �������ł��X�s�[�h
#define EXPLOSION_RANGE			(80)		// �����͈̔�
#define EGG_PARTICLE			(30)		// �p�[�e�B�N���̐�
#define EGG_DIS_TIME			(30)		// ������܂ł̎���
#define EGG_GRAVITY				(0.2f)		// �d��

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
D3DXVECTOR3 CEgg::m_VtxMaxModel[MAX_EGG] = {};
D3DXVECTOR3 CEgg::m_VtxMinModel[MAX_EGG] = {};

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CEgg::CEgg() : CModel3D(EGG_PRIOTITY, CScene::OBJTYPE_EGG)
{
	m_scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �傫��
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_FNor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fDestAngle = 0.0f;
	m_fDiffAngle = 0.0f;
	m_fHeight = 0.0f;
	m_fLength = 0.0f;
	m_nRank = 0;
	m_nNumPlayer = 0;
	m_nHatchingTimer = 0;
	m_nExplosion = 0;
	m_nMap = 0;
	m_nDisTimer = 0;
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CEgg::~CEgg() {}

//===============================================================================
//�@����
//===============================================================================
CEgg * CEgg::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, EGGTYPE eggType, BULLETTYPE bulletType, int nNumPlayer)
{
	CEgg *pEgg = NULL;

	// NULL�`�F�b�N
	if (pEgg == NULL)
	{// �������̓��I�m��

		pEgg = new CEgg;

		if (pEgg != NULL)
		{
			// ���f���̐ݒ�
			pEgg->SetModelType(MODEL_TYPE_EGG);
			// �e�N�X�`���̐ݒ�
			pEgg->SetTextureType(eggType + TEXTURE_TYPE_EGG_K);
			// �T�C�Y����
			pEgg->m_scale = scale;
			// �T�C�Y��e�N���X�ɑ��
			pEgg->SetScale(scale);
			// ���̎�ނ���
			pEgg->m_eggType = eggType;
			// �e�̎�ނ���
			pEgg->m_bulletType = bulletType;
			// �I�u�W�F�N�g�N���X�̐���
			pEgg->Init();
			// �ʒu����
			pEgg->SetPosition(pos);
			pEgg->m_pos = pos;
			pEgg->m_fHeight = pos.y;
			// ��]�𔽉f
			pEgg->SetRot(rot);
			// ���ʂ̗���
			pEgg->m_nNumPlayer = nNumPlayer;
		}
	}

	return pEgg;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CEgg::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//3D���f����Init
	CModel3D::Init();

	// �e����̑��
	CModel3D::SetScale(m_scale);

	//�ϐ��̏�����
	m_pObjBill = NULL;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_FNor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fHeight = 0.0f;
	m_fDestAngle = 0.0f;
	m_fDiffAngle = 0.0f;
	m_fLength = 3.0f;
	m_bJump = false;
	m_eggState = EGGSTATE_NORMAL;
	m_nRank = 0;
	m_nNumPlayer = 0;
	m_nHatchingTimer = 0;
	m_nMap = 0;
	m_nExplosion = 0;
	m_nDisTimer = 0;
	m_bThrow = false;
	m_bExplosion = false;
	m_bDis = true;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEgg::Uninit(void)
{
	//�I�u�W�F�N�g�r���{�[�h
	m_pObjBill = NULL;

	//3D���f����Uninit
	CModel3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEgg::Update(void)
{
	m_nHatchingTimer++;

	m_posOld = m_pos;	//�O��̈ʒu��ۑ�����

	if (m_bDis == false)
	{
		m_nDisTimer++;

		if (m_nDisTimer > 60 * EGG_DIS_TIME)
		{// ����
			m_nDisTimer = 0;
			Uninit();
		}
	}

	Item();

	Move();

	CModel3D::SetMove(m_move);
	CModel3D::SetPosition(D3DXVECTOR3(m_pos.x, m_pos.y + 10.0f, m_pos.z));
	CModel3D::SetRot(m_rot);
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEgg::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�ʒu�̏�����
	D3DXVECTOR3 Modelpos = CModel3D::GetPosition();
	//�Q�[���̏��
	CManager::MODE pMode = CManager::GetMode();

	//���_�@���̎������K��
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	if (m_bExplosion == false)
	{
		//�`�揈��
		CModel3D::Draw();
	}

	//���_�@���̎������K��
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}

//===============================================================================
// X�t�@�C���̓ǂݍ���
//===============================================================================
HRESULT CEgg::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	int			nNumVtx = 0;						//���_��
	DWORD		sizeFVF;							//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE		*pVtxBuff;							//���_�o�b�t�@�ւ̃|�C���^

													//���f���̍ő�l�E�ŏ��l���擾����
	for (int nCntModel = 0; nCntModel < MAX_EGG; nCntModel++)
	{
		m_VtxMaxModel[nCntModel] = D3DXVECTOR3(-10000, -10000, -10000);	//�ő�l
		m_VtxMinModel[nCntModel] = D3DXVECTOR3(10000, 10000, 10000);	//�ŏ��l

		CModel3D::MODEL_TYPE type;
		switch (nCntModel)
		{
		case EGGTYPE_ATTACK:type = MODEL_TYPE_EGG;	break;
		case EGGTYPE_ANNOY:	type = MODEL_TYPE_EGG;	break;
		case EGGTYPE_SPEED:	type = MODEL_TYPE_EGG;	break;
		}

		LPD3DXMESH &Mesh = MeshLoad(type);

		if (Mesh != NULL)
		{
			//���_�����擾
			nNumVtx = Mesh->GetNumVertices();

			//���_�t�H�[�}�b�g�̃T�C�Y���擾
			sizeFVF = D3DXGetFVFVertexSize(Mesh->GetFVF());

			//���_�o�b�t�@�̃��b�N
			Mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		//���_���W�̑��

																//�ő�l
				if (vtx.x > m_VtxMaxModel[nCntModel].x)
				{
					m_VtxMaxModel[nCntModel].x = vtx.x;
				}
				if (vtx.y > m_VtxMaxModel[nCntModel].y)
				{
					m_VtxMaxModel[nCntModel].y = vtx.y;
				}
				if (vtx.z > m_VtxMaxModel[nCntModel].z)
				{
					m_VtxMaxModel[nCntModel].z = vtx.z;
				}
				//�ŏ��l
				if (vtx.x < m_VtxMinModel[nCntModel].x)
				{
					m_VtxMinModel[nCntModel].x = vtx.x;
				}
				if (vtx.y < m_VtxMinModel[nCntModel].y)
				{
					m_VtxMinModel[nCntModel].y = vtx.y;
				}
				if (vtx.z < m_VtxMinModel[nCntModel].z)
				{
					m_VtxMinModel[nCntModel].z = vtx.z;
				}

				//�T�C�Y���̃|�C���^��i�߂�
				pVtxBuff += sizeFVF;
			}

			//���_�o�b�t�@�̃A�����b�N
			Mesh->UnlockVertexBuffer();
		}
	}

	return S_OK;
}

//===============================================================================
// �e�N�X�`���̔j��
//===============================================================================
void CEgg::UnLoad(void)
{

}

//=============================================================================
// �g�����Ƃ��̏���
//=============================================================================
void CEgg::Item(void)
{
	if (m_eggState == EGGSTATE_BULLET)
	{
		switch (m_eggType)
		{
			// �U��
		case EGGTYPE_ATTACK:
			if (m_bExplosion == true)
			{// ������
				m_nExplosion++;

				if (m_nExplosion > EXPLOSION_TIME)
				{// �폜
					m_nExplosion = 0;
					Uninit();
				}
			}
			break;

			// ����
		case EGGTYPE_SPEED:
			CPlayer **pPlayer = NULL;
			switch (CManager::GetMode())
			{
			case CManager::MODE_TITLE:
				pPlayer = CTitle::GetPlayer();
				break;
			case CManager::MODE_GAME:
				pPlayer = CGame::GetPlayer();
				break;
			}

			m_pos = D3DXVECTOR3(pPlayer[m_nNumPlayer]->GetPos().x, pPlayer[m_nNumPlayer]->GetPos().y + 5.0f, pPlayer[m_nNumPlayer]->GetPos().z);

			m_rot.z = D3DX_PI * 0.5f;
			m_rot.x += 0.5f;
			m_rot.x = AdjustAngle(m_rot.x);

			m_rot.y = pPlayer[m_nNumPlayer]->GetRot().y;

			if (pPlayer[m_nNumPlayer]->GetPlayerState() == CPlayer::PLAYERSTATE_NORMAL)
			{// ������Ԃ���Ȃ��Ȃ�����폜
				Uninit();
			}
			break;
		}

		Bullet();
	}
}

//===============================================================================
// ����
//===============================================================================
void CEgg::Move(void)
{
	float fGravity = EGG_GRAVITY;

	if (m_eggType == EGGTYPE_ATTACK && m_eggState == EGGSTATE_BULLET)
	{
		fGravity = EGG_GRAVITY + 0.1f;
	}
	else if (m_eggType == EGGTYPE_ANNOY && m_eggState == EGGSTATE_BULLET)
	{
		fGravity = 0.0f;
	}

	m_move.y -= cosf(0) * fGravity;

	m_pos.x += m_move.x;
	m_pos.z += m_move.z;
	m_pos.y += m_move.y;

	if (m_eggState == EGGSTATE_CHASE || m_eggType == EGGTYPE_ANNOY || m_eggType == EGGTYPE_ATTACK || m_eggType == EGGTYPE_SPEED)
	{
		//�}�b�v�Ƃ̓����蔻��
		CPlayer **pPlayer = NULL;
		switch (CManager::GetMode())
		{
		case CManager::MODE_TITLE:
			pPlayer = CTitle::GetPlayer();
			break;
		case CManager::MODE_GAME:
			pPlayer = CGame::GetPlayer();
			break;
		}

		m_fHeight = CCOL_MESH_MANAGER::GetHeight(m_pos, pPlayer[m_nNumPlayer]->GetnMap());

		if (m_pos.y < m_fHeight)
		{
			//�W�����v�̏�Ԑݒ�
			m_bJump = false;

			m_move.y = 0.0f;
			m_pos.y = m_fHeight;

			if (m_eggType == EGGTYPE_ATTACK && m_eggState == EGGSTATE_BULLET)
			{// ��������
				if (m_bExplosion == false)
				{
					m_bExplosion = true;
					D3DXVECTOR2 fSize;

					for (int nCntParticle = 0; nCntParticle < EGG_PARTICLE; nCntParticle++)
					{
						fSize.x = 5.0f + (float)(CServer::Rand() % 5);
						fSize.y = 5.0f + (float)(CServer::Rand() % 5);

						CParticle::Create(m_pos,
							D3DXVECTOR3(sinf((CServer::Rand() % 628) / 100.0f) * ((CServer::Rand() % 5 + 1)), cosf((CServer::Rand() % 628) / 100.0f) * ((CServer::Rand() % 5 + 1)), cosf((CServer::Rand() % 628) / 100.0f) * ((CServer::Rand() % 5 + 1))),
							D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
							fSize,
							20,
							CParticle::TEXTURE_STAR,
							CParticle::TYPE_NORMAL,
							m_nNumPlayer);
					}
					CCylinder::Create(m_pos, m_rot, CCylinder::TYPE_EATK);
				}
			}
		}
	}
}

//===============================================================================
// �����蔻��
//===============================================================================
bool CEgg::CollisionEgg(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld)
{
	//�������Ă��邩�������ĂȂ���
	bool bHit = false;

	if (m_eggState == EGGSTATE_BULLET)
	{
		// �e����̎擾
		D3DXVECTOR3 ModelPos = CModel3D::GetPosition();		// �ʒu
		D3DXVECTOR3 VtxMax = m_VtxMaxModel[m_eggType];		// ���f���̍ő�l
		D3DXVECTOR3 VtxMin = m_VtxMinModel[m_eggType];		// ���f���̍ŏ��l
		D3DXVECTOR3 rot = CModel3D::GetRot();

		D3DXVECTOR3 ModelMax = ModelPos + VtxMax;			// �ʒu���݂̍ő�l
		D3DXVECTOR3 ModelMin = ModelPos + VtxMin;			// �ʒu���݂̍ŏ��l

		float fDepth = PLAYER_DEPTH;

		if (m_eggType == EGGTYPE_ATTACK)
		{
			if (m_bExplosion == true)
			{
				fDepth = EXPLOSION_RANGE;
			}
		}

		if (pPos->x >= ModelMin.x - fDepth && pPos->x <= ModelMax.x + fDepth)
		{// Z�͈͓̔��ɂ���w
			if (pPos->z >= ModelMin.z - fDepth && pPos->z <= ModelMax.z + fDepth)
			{// X�͈͓̔��ɂ���
				if (pPosOld->y >= ModelMax.y && pPos->y <= ModelMax.y)
				{// �I�u�W�F�N�g�̏ォ�瓖����ꍇ
					bHit = true;
				}
				else if (pPosOld->y + PLAYER_HEIGHT <= ModelMin.y && pPos->y + PLAYER_HEIGHT >= ModelMin.y)
				{// �I�u�W�F�N�g�̉����瓖����ꍇ
					bHit = true;
				}

				if (!(pPos->y >= ModelMax.y) && !(pPos->y + PLAYER_HEIGHT <= ModelMin.y))
				{// �I�u�W�F�N�g���Ƃ̓����蔻��
					bHit = true;
				}
			}
		}

		// �ʒu�̑��
		CModel3D::SetPosition(ModelPos);
	}
	return bHit;
}

//=============================================================================
// �W�����v
//=============================================================================
void CEgg::Jump(float fJump)
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
void CEgg::Bullet(void)
{
	if (m_bulletType == BULLETTYPE_PLAYER)
	{
		if (m_eggType == EGGTYPE_ATTACK)
		{//�^�C�v���G��������
		 //���f���̈ړ�	���f���̈ړ�����p�x(�J�����̌��� + �p�x) * �ړ���
			float fEggSpeed = EGG_SPEED;

			if (m_bExplosion == true)
			{
				fEggSpeed = 0.0f;
			}

			m_move.x = sinf(m_rot.y) * fEggSpeed;
			m_move.z = cosf(m_rot.y) * fEggSpeed;

			m_rot.x = D3DX_PI * 0.5f;

			if (m_pos.y < -3000.0f)
			{
				Uninit();
			}

			CModel3D::SetRot(m_rot);
		}
	}
}

//=============================================================================
// �p�x�̒���
//=============================================================================
float CEgg::AdjustAngle(float rot)
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