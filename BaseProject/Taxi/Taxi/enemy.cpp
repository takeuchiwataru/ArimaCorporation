//=============================================================================
//
// �G�̏����i���j [enemy.cpp]
// Author : ���R���
//
//=============================================================================
#include "scene.h"
#include "enemy.h"
#include "manager.h"
#include "object.h"
#include "meshfield.h"
#include "player.h"
#include "fade.h"
#include "shadow.h"
#include "tutorial.h"
#include "egg.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FEED_NAME_000	"data\\MODEL\\cone.x"			// �ǂݍ��ރ��f���t�@�C��
#define FEED_NAME_001	"data\\MODEL\\Apple.x"			// �ǂݍ��ރ��f���t�@�C��
#define FEED_NAME_002	"data\\MODEL\\Banana.x"			// �ǂݍ��ރ��f���t�@�C��

#define MODEL_SPEED				(5.0f)
#define PLAYER_DEPTH			(50)		// �v���C���[�̕������p
#define PLAYER_HEIGHT			(100.0f)	// �v���C���[�̔w�̍���
#define OBJCT_ANGLE_REVISION	(0.2f)		// �p�x�␳
#define EFFECT_HIGHT			(250.0f)	// �G�~�b�^�[�̍���
#define FOUNTAIN_UP				(20.0f)		// �����̏㏸������l

#define DAMAGE_TIME				(60)		// �_���[�W��H����Ă��鎞��

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
LPD3DXMESH CEnemy::m_pMeshModel = NULL;						//���b�V�����ւ̃|�C���^
LPD3DXBUFFER CEnemy::m_pBuffMatModel = NULL;					//�}�e���A���̏��ւ̃|�C���^
DWORD CEnemy::m_nNumMatModel = {};							//�}�e���A���̏��
LPDIRECT3DTEXTURE9 CEnemy::m_pMeshTextures = NULL;
D3DXVECTOR3 CEnemy::m_VtxMaxModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 CEnemy::m_VtxMinModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CEnemy::CEnemy() : CModel3D(ENEMY_PRIOTITY, CScene::OBJTYPE_ENEMY)
{
	m_scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �傫��
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntMove = 0;
	m_nCntDamage = 0;
	m_fSpeed = 0.0f;
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CEnemy::~CEnemy() {}

//===============================================================================
//�@����
//===============================================================================
CEnemy * CEnemy::Create(D3DXVECTOR3 pos)
{
	CEnemy *pEnemy = NULL;

	// NULL�`�F�b�N
	if (pEnemy == NULL)
	{// �������̓��I�m��

		pEnemy = new CEnemy;

		if (pEnemy != NULL)
		{
			// ��ނ̐ݒ�
			pEnemy->BindModel(m_pMeshModel, m_pBuffMatModel, m_nNumMatModel, m_pMeshTextures,
				m_VtxMaxModel, m_VtxMinModel);
			// �T�C�Y����
			pEnemy->m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			// �T�C�Y��e�N���X�ɑ��
			pEnemy->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			// �I�u�W�F�N�g�N���X�̐���
			pEnemy->Init();
			// �ʒu����
			pEnemy->SetPosition(pos);
		}
	}

	return pEnemy;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CEnemy::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//3D���f����Init
	CModel3D::Init();

	// �ʒu�̏�����
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�ʒu�̑��
	CModel3D::SetPosition(m_pos);

	// �e����̑��
	CModel3D::SetScale(m_scale);

	//�ϐ��̏�����
	m_pObjBill = NULL;
	m_State = ENEMYSTATE::ENEMYSTATE_NORMAL;
	m_bSpeedDown = false;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntMove = 0;
	m_nCntDamage = 0;
	m_fSpeed = 0.0f;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEnemy::Uninit(void)
{
	//�I�u�W�F�N�g�r���{�[�h
	m_pObjBill = NULL;

	//3D���f����Uninit
	CModel3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEnemy::Update(void)
{
	m_pos = CModel3D::GetPosition();
	m_posOld = m_pos;

	// ���Ƃ̓����蔻��
	CollisionEgg();

	switch (m_State)
	{
	case ENEMYSTATE_NORMAL:
		m_fSpeed = 3.0f;
		break;

	case ENEMYSTATE_SPEEDDOWN:
		m_fSpeed = 1.0f;
		break;

	case ENEMYSTATE_SPEEDUP:
		m_fSpeed = 5.0f;
		break;

	case ENEMYSTATE_DAMAGE:

		m_fSpeed = 0.0f;

		m_nCntDamage++;

		if (m_nCntDamage > DAMAGE_TIME)
		{
			m_State = ENEMYSTATE_NORMAL;
			m_nCntDamage = 0;
		}
		break;
	}

	m_nCntMove++;

	m_move.x = m_fSpeed;

	if (m_nCntMove >= 120)
	{
		m_move.x *= -1.0f;
	}

	if (m_nCntMove >= 240)
	{
		m_nCntMove = 0;
	}

	m_pos.x += m_move.x;

	CModel3D::SetMove(m_move);
	CModel3D::SetPosition(m_pos);
	
	//�����̎擾
	float fLength = CModel3D::GetLength();

	if (CModel3D::GetDelete() == true) { Uninit(); }
}
//=============================================================================
// �`�揈��
//=============================================================================
void CEnemy::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�ʒu�̏�����
	D3DXVECTOR3 Modelpos = CModel3D::GetPosition();
	//�Q�[���̏��
	CManager::MODE pMode = CManager::GetMode();

	//���_�@���̎������K��
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	//�`�揈��
	CModel3D::Draw();

	//���_�@���̎������K��
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}

//===============================================================================
// X�t�@�C���̓ǂݍ���
//===============================================================================
HRESULT CEnemy::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL *pMat;

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data\\MODEL\\box.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel, NULL, &m_nNumMatModel, &m_pMeshModel);

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


	//�g���Ă���e�N�X�`��
	//D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_1, &m_pMeshTextures[0]);

	return S_OK;
}

//===============================================================================
// �e�N�X�`���̔j��
//===============================================================================
void CEnemy::UnLoad(void)
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

//=============================================================================
// ���Ƃ̓����蔻��
//=============================================================================
void CEnemy::CollisionEgg(void)
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

				if (pEgg->GetState() == CEgg::EGGSTATE_BULLET || pEgg->GetState() == CEgg::EGGSTATE_CHASE)
				{
					if (pEgg->CollisionEgg(&m_pos, &m_posOld))
					{// �Փ˂���
						switch (pEgg->GetType())
						{
						// �U��
						case CEgg::EGGTYPE_ATTACK:
							if (m_State != ENEMYSTATE_DAMAGE)
							{
								// �_���[�W��Ԃɂ���
								m_State = ENEMYSTATE_DAMAGE;
							}
							pEgg->Uninit();	// ���폜
							break;

						// ����
						case CEgg::EGGTYPE_ANNOY:
							m_State = ENEMYSTATE_SPEEDDOWN;
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