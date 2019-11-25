//=============================================================================
//
// �Ђ悱�̏��� [chick.cpp]
// Author : ���R���
//
//=============================================================================
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

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define MODEL_SPEED				(5.0f)
#define OBJCT_ANGLE_REVISION	(0.2f)		// �p�x�␳
#define EFFECT_HIGHT			(250.0f)	// �G�~�b�^�[�̍���
#define FOUNTAIN_UP				(20.0f)		// �����̏㏸������l
#define DISTIME					(100)		// ������܂ł̎���
#define CHICK_SPEED				(30.0f)		// �Ђ悱�����ł��X�s�[�h
#define ANNOY_RANGE				(200.0f)	// ����������͈�
#define ATTACK_RANGE			(200.0f)	// �͈͍U���͈̔�
#define CHICK_JUMP				(3.5f)		// �W�����v��
#define CHICK_FALL_TIME			(30)		// �Ђ悱�������Ă���^�C�~���O�̊Ԋu
#define CHICK_FALL_SPEED		(10.0f)		// �����Ă���Ђ悱�̑���

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
D3DXVECTOR3 CChick::m_VtxMaxModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 CChick::m_VtxMinModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CChick::CChick() : CModel3D(EGG_PRIOTITY, CScene::OBJTYPE_CHICK)
{
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
			pChick->SetModelType(MODEL_TYPE_CHICK);
			// �e�N�X�`���̐ݒ�
			pChick->SetTextureType(type + TEXTURE_TYPE_CHICK_K);
			// �T�C�Y����
			pChick->m_scale = scale;
			// �T�C�Y��e�N���X�ɑ��
			pChick->SetScale(scale);
			// ���̎�ނ���
			pChick->m_type = type;
			// �e�̎�ނ���
			pChick->m_bulletType = bulletType;
			// �I�u�W�F�N�g�N���X�̐���
			pChick->Init();
			// �ʒu����
			pChick->SetPosition(pos);
			pChick->m_pos = pos;
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

	//3D���f����Init
	CModel3D::Init();

	// �e����̑��
	CModel3D::SetScale(m_scale);

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

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CChick::Uninit(void)
{
	//�I�u�W�F�N�g�r���{�[�h
	m_pObjBill = NULL;

	//3D���f����Uninit
	CModel3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CChick::Update(void)
{
	//m_pos = CModel3D::GetPosition();
	m_posOld = m_pos;	//�O��̈ʒu��ۑ�����

	// �Ђ悱�̓���
	Move();

	CModel3D::SetPosition(D3DXVECTOR3(m_pos.x, m_pos.y + 5.0f, m_pos.z));
	CModel3D::SetRot(m_rot);
}
//=============================================================================
// �`�揈��
//=============================================================================
void CChick::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���_�@���̎������K��
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	if (m_bExplosion == false)
	{
		//�`�揈��
		//CModel3D::Draw();
	}

	//���_�@���̎������K��
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}

//===============================================================================
// X�t�@�C���̓ǂݍ���
//===============================================================================
HRESULT CChick::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�}�e���A���f�[�^�ւ̃|�C���^
	//D3DXMATERIAL *pMat;

	////�}�e���A����񂩂�e�N�X�`���̎擾
	//pMat = (D3DXMATERIAL*)m_pBuffMatModel->GetBufferPointer();

	//int nNumVtx;		//���_��
	//DWORD sizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
	//BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

	////���f���̍ő�l�E�ŏ��l���擾����
	//m_VtxMaxModel = D3DXVECTOR3(-10000, -10000, -10000);	//�ő�l
	//m_VtxMinModel = D3DXVECTOR3(10000, 10000, 10000);	//�ŏ��l

	////���_�����擾
	//nNumVtx = m_pMeshModel->GetNumVertices();

	////���_�t�H�[�}�b�g�̃T�C�Y���擾
	//sizeFVF = D3DXGetFVFVertexSize(m_pMeshModel->GetFVF());

	////���_�o�b�t�@�̃��b�N
	//m_pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	//for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	//{
	//	D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		//���_���W�̑��

	//	//�ő�l
	//	if (vtx.x > m_VtxMaxModel.x)
	//	{
	//		m_VtxMaxModel.x = vtx.x;
	//	}
	//	if (vtx.y > m_VtxMaxModel.y)
	//	{
	//		m_VtxMaxModel.y = vtx.y;
	//	}
	//	if (vtx.z > m_VtxMaxModel.z)
	//	{
	//		m_VtxMaxModel.z = vtx.z;
	//	}
	//	//�ŏ��l
	//	if (vtx.x < m_VtxMinModel.x)
	//	{
	//		m_VtxMinModel.x = vtx.x;
	//	}
	//	if (vtx.y < m_VtxMinModel.y)
	//	{
	//		m_VtxMinModel.y = vtx.y;
	//	}
	//	if (vtx.z < m_VtxMinModel.z)
	//	{
	//		m_VtxMinModel.z = vtx.z;
	//	}

	//	//�T�C�Y���̃|�C���^��i�߂�
	//	pVtxBuff += sizeFVF;
	//}

	////���_�o�b�t�@�̃A�����b�N
	//m_pMeshModel->UnlockVertexBuffer();

	return S_OK;
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
void CChick::Move(void)
{
	// �g�����Ƃ�
	Item();

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
	else if (m_type == TYPE_ATTACK && m_state == STATE_BULLET)
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

	//CDebugProc::Print("%.1f\n", m_move.y);
}

//===============================================================================
// �Ђ悱���g�p�����Ƃ��̓���
//===============================================================================
void CChick::Item(void)
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

			// ����������
		case TYPE_ANNOY:
			pPlayer = CGame::GetPlayer();

			m_pos = D3DXVECTOR3(pPlayer[m_nNumPlayer]->GetPos().x, pPlayer[m_nNumPlayer]->GetPos().y + 60.0f, pPlayer[m_nNumPlayer]->GetPos().z);

			break;
		}
		// ���ł�����
		Bullet();
	}
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
		D3DXVECTOR3 ModelPos = CModel3D::GetPosition();		// �ʒu
		D3DXVECTOR3 VtxMax = CModel3D::GetVtxMax();			// ���f���̍ő�l
		D3DXVECTOR3 VtxMin = CModel3D::GetVtxMin();			// ���f���̍ŏ��l
		D3DXVECTOR3 rot = CModel3D::GetRot();

		D3DXVECTOR3 ModelMax = CModel3D::GetPosition() + CModel3D::GetVtxMax();	// �ʒu���݂̍ő�l
		D3DXVECTOR3 ModelMin = CModel3D::GetPosition() + CModel3D::GetVtxMin();	// �ʒu���݂̍ŏ��l

		float fDepth = PLAYER_DEPTH - 10.0f;

		if (m_type == TYPE_ANNOY)
		{
			fDepth = ANNOY_RANGE;
		}
		if (m_bExplosion == true)
		{
			fDepth = ATTACK_RANGE;
		}

		if (pPos->x >= ModelMin.x - fDepth && pPos->x <= ModelMax.x + fDepth)
		{// Z�͈͓̔��ɂ���
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

		if (bHit == true && m_type == TYPE_ATTACK)
		{
			m_bExplosion = true;
			m_bDis = false;
		}

		// �ʒu�̑��
		CModel3D::SetPosition(ModelPos);
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
				fHeight = pField->GetHeightMesh(CModel3D::GetPosition());

				if (m_bJump == false || (m_bJump == true && m_fHeight < fHeight))
				{
					m_fHeight = fHeight + 15.0f;				//�n�ʂ̍������擾
					m_move.y = 0.0f;					//�ړ��ʂ�����������

														//�W�����v�̏�Ԑݒ�
					m_bJump = false;

					CModel3D::SetMove(m_move);
					CModel3D::SetPosition(D3DXVECTOR3(m_pos.x, m_fHeight, m_pos.z));

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
void CChick::AdjustAngle(float rot)
{
	if (rot > D3DX_PI)
	{
		rot -= D3DX_PI * 2.0f;
	}
	if (rot < -D3DX_PI)
	{
		rot += D3DX_PI * 2.0f;
	}
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

		AdjustAngle(m_fDiffAngle.y);

		m_rot.y += m_fDiffAngle.y * 0.5f;

		AdjustAngle(m_rot.y);

		//���f���̈ړ�	���f���̈ړ�����p�x(�J�����̌��� + �p�x) * �ړ���
		m_move.x = sinf(m_rot.y) * CHICK_SPEED;
		m_move.z = cosf(m_rot.y) * CHICK_SPEED;
	}
	else
	{
		//���f���̈ړ�	���f���̈ړ�����p�x(�J�����̌��� + �p�x) * �ړ���
		m_move.x = sinf(m_rot.y) * -CHICK_SPEED * 0.6f;
		m_move.z = cosf(m_rot.y) * -CHICK_SPEED * 0.6f;
	}

	CModel3D::SetRot(m_rot);
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

	CModel3D::SetRot(m_rot);
}

//=============================================================================
// �����Ђ悱(����)
//=============================================================================
void CChick::AnnoyS(void)
{
	CPlayer **pPlayer = CGame::GetPlayer();

	if (m_bAttackS == false)
	{
		pPlayer[m_nNumPlayer]->AnnoyChicks();
		m_bAttackS = true;
	}
}