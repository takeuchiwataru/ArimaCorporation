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
#include "enemy.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CHICK_NAME	"data\\MODEL\\Weapon\\chick.x"			// �ǂݍ��ރ��f���t�@�C��

#define MODEL_SPEED				(5.0f)
#define PLAYER_DEPTH			(50)		// �v���C���[�̕������p
#define PLAYER_HEIGHT			(100.0f)	// �v���C���[�̔w�̍���
#define OBJCT_ANGLE_REVISION	(0.2f)		// �p�x�␳
#define EFFECT_HIGHT			(250.0f)	// �G�~�b�^�[�̍���
#define FOUNTAIN_UP				(20.0f)		// �����̏㏸������l
#define DISTIME					(100)		// ������܂ł̎���
#define CHICK_SPEED				(30.0f)		// �Ђ悱�����ł��X�s�[�h
#define ANNOY_RANGE				(200.0f)	// ����������͈�
#define CHICK_JUMP				(6.5f)		// �W�����v��
#define CHICK_FALL_TIME			(30)		// �Ђ悱�������Ă���^�C�~���O�̊Ԋu
#define CHICK_FALL_NUM			(5)			// �����Ă���Ђ悱�̐�
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
LPD3DXMESH CChick::m_pMeshModel = NULL;						//���b�V�����ւ̃|�C���^
LPD3DXBUFFER CChick::m_pBuffMatModel = NULL;					//�}�e���A���̏��ւ̃|�C���^
DWORD CChick::m_nNumMatModel = NULL;							//�}�e���A���̏��
LPDIRECT3DTEXTURE9 CChick::m_pMeshTextures = NULL;
D3DXVECTOR3 CChick::m_VtxMaxModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 CChick::m_VtxMinModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
int CChick::m_nChickTimer = 0;

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CChick::CChick() : CModel3D(EGG_PRIOTITY, CScene::OBJTYPE_CHICK)
{
	m_scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �傫��
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fDestAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fDiffAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fHeight = 0.0f;
	m_nRank = 0;
	m_nNumPlayer = 0;
	m_nDisTimer = 0;
	m_DestRank = 0;
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
			// ��ނ̐ݒ�
			pChick->BindModel(m_pMeshModel, m_pBuffMatModel, m_nNumMatModel, m_pMeshTextures,
				m_VtxMaxModel, m_VtxMinModel);
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

	// �ʒu�̏�����
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�ʒu�̑��
	CModel3D::SetPosition(pos);

	// �e����̑��
	CModel3D::SetScale(m_scale);

	//�ϐ��̏�����
	m_pObjBill = NULL;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fHeight = 0.0f;
	m_fDestAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fDiffAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bJump = false;
	m_bDis = true;
	m_state = STATE_NORMAL;
	m_nRank = 0;
	m_nNumPlayer = 0;
	m_nDisTimer = 0;
	m_DestRank = -1;
	m_nChickTimer = 0;
	m_bAttackS = false;

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
	m_pos = CModel3D::GetPosition();
	// �Ђ悱�̓���
	Move();

	CModel3D::SetPosition(m_pos);
	CModel3D::SetRot(m_rot);

	//�����̎擾
	float fLength = CModel3D::GetLength();

	if (CModel3D::GetDelete() == true) { Uninit(); }

	CDebugProc::Print("%.1f : %.1f : %.1f\n", m_move.x, m_move.y, m_move.z);
	CDebugProc::Print("%.1f\n", m_rot.y);
}
//=============================================================================
// �`�揈��
//=============================================================================
void CChick::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�ʒu�̏�����
	D3DXVECTOR3 Modelpos = CModel3D::GetPosition();
	//�Q�[���̏��
	CManager::MODE pMode = CManager::GetMode();

	//���_�@���̎������K��
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	// �F�ύX
	if (m_type == TYPE_ATTACK)
	{
		CModel3D::Setcol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else if (m_type == TYPE_ANNOY)
	{
		CModel3D::Setcol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	}
	else if (m_type == TYPE_SPEED)
	{
		CModel3D::Setcol(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	}
	else if (m_type == TYPE_ATTACK_S)
	{
		CModel3D::Setcol(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
	}
	else if (m_type == TYPE_ANNOY_S)
	{
		CModel3D::Setcol(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
	}
	else if (m_type == TYPE_SPEED_S)
	{
		CModel3D::Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	//�`�揈��
	CModel3D::Draw();

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
	D3DXMATERIAL *pMat;

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(CHICK_NAME, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel, NULL, &m_nNumMatModel, &m_pMeshModel);

	//�}�e���A����񂩂�e�N�X�`���̎擾
	pMat = (D3DXMATERIAL*)m_pBuffMatModel->GetBufferPointer();

	int nNumVtx;		//���_��
	DWORD sizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

						//���f���̍ő�l�E�ŏ��l���擾����
	m_VtxMaxModel = D3DXVECTOR3(-10000, -10000, -10000);	//�ő�l
	m_VtxMinModel = D3DXVECTOR3(10000, 10000, 10000);	//�ŏ��l

														//���_�����擾
	nNumVtx = m_pMeshModel->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(m_pMeshModel->GetFVF());

	//���_�o�b�t�@�̃��b�N
	m_pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		//���_���W�̑��

														//�ő�l
		if (vtx.x > m_VtxMaxModel.x)
		{
			m_VtxMaxModel.x = vtx.x;
		}
		if (vtx.y > m_VtxMaxModel.y)
		{
			m_VtxMaxModel.y = vtx.y;
		}
		if (vtx.z > m_VtxMaxModel.z)
		{
			m_VtxMaxModel.z = vtx.z;
		}
		//�ŏ��l
		if (vtx.x < m_VtxMinModel.x)
		{
			m_VtxMinModel.x = vtx.x;
		}
		if (vtx.y < m_VtxMinModel.y)
		{
			m_VtxMinModel.y = vtx.y;
		}
		if (vtx.z < m_VtxMinModel.z)
		{
			m_VtxMinModel.z = vtx.z;
		}

		//�T�C�Y���̃|�C���^��i�߂�
		pVtxBuff += sizeFVF;
	}

	//���_�o�b�t�@�̃A�����b�N
	m_pMeshModel->UnlockVertexBuffer();

	return S_OK;
}

//===============================================================================
// �e�N�X�`���̔j��
//===============================================================================
void CChick::UnLoad(void)
{
	// ���b�V���̊J��
	if (m_pMeshModel != NULL)
	{
		m_pMeshModel->Release();
		m_pMeshModel = NULL;
	}
	// �}�e���A���̊J��
	if (m_pBuffMatModel != NULL)
	{
		m_pBuffMatModel->Release();
		m_pBuffMatModel = NULL;
	}

	//�e�N�X�`��
	if (m_pMeshTextures != NULL)
	{
		m_pMeshTextures->Release();
		m_pMeshTextures = NULL;
	}
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
		m_move.y -= cosf(0) * 0.4f;
		m_fHeight += m_move.y;

		// ������ݒ�
		SetHeight();

		m_pos.y = m_fHeight;

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


	CDebugProc::Print("%.1f\n", m_pos.y);
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

			m_pos = D3DXVECTOR3(pPlayer[m_nNumPlayer]->GetPos().x, pPlayer[m_nNumPlayer]->GetPos().y + 100.0f, pPlayer[m_nNumPlayer]->GetPos().z);

			break;

			// ����������(����)
		case TYPE_ANNOY_S:
			/*pPlayer = CGame::GetPlayer();

			m_pos = D3DXVECTOR3(pPlayer[m_nNumPlayer]->GetPos().x, pPlayer[m_nNumPlayer]->GetPos().y + 100.0f, pPlayer[m_nNumPlayer]->GetPos().z);*/

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
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();

	//�������Ă��邩�������ĂȂ���
	bool bHit = false;

	// �e����̎擾
	D3DXVECTOR3 ModelPos = CModel3D::GetPosition();		// �ʒu
	D3DXVECTOR3 VtxMax = CModel3D::GetVtxMax();			// ���f���̍ő�l
	D3DXVECTOR3 VtxMin = CModel3D::GetVtxMin();			// ���f���̍ŏ��l
	D3DXVECTOR3 rot = CModel3D::GetRot();

	D3DXVECTOR3 ModelMax = CModel3D::GetPosition() + CModel3D::GetVtxMax();	// �ʒu���݂̍ő�l
	D3DXVECTOR3 ModelMin = CModel3D::GetPosition() + CModel3D::GetVtxMin();	// �ʒu���݂̍ŏ��l

	float fDepth = PLAYER_DEPTH;

	if (m_type == TYPE_ANNOY)
	{
		fDepth = ANNOY_RANGE;
	}

#if(1)
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

	// �ʒu�̑��
	CModel3D::SetPosition(ModelPos);

#endif

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

	return fHeight;
}

//=============================================================================
// �W�����v
//=============================================================================
void CChick::Jump(void)
{
	// �W�����v
	if (m_bJump == false)
	{// �W�����v���Ă��Ȃ�
		m_bJump = true;
		m_move.y += CHICK_JUMP;
	}

	CModel3D::SetMove(m_move);
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
		m_move.x = sinf(m_rot.y) * CHICK_SPEED;
		m_move.z = cosf(m_rot.y) * CHICK_SPEED;
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
			m_move.y = 5.0f;
			m_move.z = pPlayer[m_nNumPlayer]->GetMove().z;

			if (m_pos.y > pPlayer[m_nNumPlayer]->GetPos().y + 300.0f)
			{// 覐΂ɂȂ�^�C�~���O
				int fx = rand() % FALL_CHICK_RANGE;
				int fz = rand() % FALL_CHICK_RANGE;

				m_pos = D3DXVECTOR3(pPlayer[m_DestRank]->GetPos().x + ((FALL_CHICK_RANGE / 2) - fx),
					pPlayer[m_DestRank]->GetPos().y + 300.0f,
					pPlayer[m_DestRank]->GetPos().z + ((FALL_CHICK_RANGE / 2) - fz));
				m_bAttackS = true;
			}
		}
		else
		{
			m_nChickTimer++;

			if (m_nChickTimer > CHICK_FALL_TIME)
			{
				if (pPlayer[m_nNumPlayer]->GetCntChick() < CHICK_FALL_NUM)
				{// 5�C�܂ŏo��
				 // ������Ђ悱�o��
					pPlayer[m_nNumPlayer]->FallChicks(D3DXVECTOR3(
						(sinf(pPlayer[m_DestRank]->GetRot().y + D3DX_PI) * 50.0f) + pPlayer[m_DestRank]->GetPos().x,
						pPlayer[m_DestRank]->GetPos().y,
						(cosf(pPlayer[m_DestRank]->GetRot().y + D3DX_PI) * 50.0f) + pPlayer[m_DestRank]->GetPos().z));
				}

				m_nChickTimer = 0;
			}

			// �ړ��ʂ�ݒ�
			m_move.y = -CHICK_FALL_SPEED;

			if (m_pos.y < -200.0f)
			{// ������x���ɍs���������
				Uninit();
			}
		}
	}

	CDebugProc::Print("%d\n", m_nChickTimer);

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