//=============================================================================
//
// �a�̏��� [feed.cpp]
// Author : ���R���
//
//=============================================================================
#include "scene.h"
#include "feed.h"
#include "manager.h"
#include "object.h"
#include "meshfield.h"
#include "player.h"
#include "fade.h"
#include "shadow.h"
#include "tutorial.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FEED_NAME_000	"data\\MODEL\\Item\\cone.x"			// �ǂݍ��ރ��f���t�@�C��
#define FEED_NAME_001	"data\\MODEL\\Item\\Apple.x"			// �ǂݍ��ރ��f���t�@�C��
#define FEED_NAME_002	"data\\MODEL\\Item\\Banana.x"			// �ǂݍ��ރ��f���t�@�C��

#define MODEL_SPEED				(5.0f)
#define PLAYER_DEPTH			(50)		// �v���C���[�̕������p
#define PLAYER_HEIGHT			(100.0f)	// �v���C���[�̔w�̍���
#define OBJCT_ANGLE_REVISION	(0.2f)		// �p�x�␳
#define EFFECT_HIGHT			(250.0f)	// �G�~�b�^�[�̍���
#define FOUNTAIN_UP				(20.0f)		// �����̏㏸������l

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
LPD3DXMESH CFeed::m_pMeshModel[FEEDTYPE_MAX] = {};						//���b�V�����ւ̃|�C���^
LPD3DXBUFFER CFeed::m_pBuffMatModel[FEEDTYPE_MAX] = {};					//�}�e���A���̏��ւ̃|�C���^
DWORD CFeed::m_nNumMatModel[FEEDTYPE_MAX] = {};							//�}�e���A���̏��
LPDIRECT3DTEXTURE9 CFeed::m_pMeshTextures = NULL;
D3DXVECTOR3 CFeed::m_VtxMaxModel[FEEDTYPE_MAX] = {};
D3DXVECTOR3 CFeed::m_VtxMinModel[FEEDTYPE_MAX] = {};

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CFeed::CFeed() : CModel3D(FEED_PRIOTITY, CScene::OBJTYPE_FEED)
{
	m_scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �傫��
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CFeed::~CFeed() {}

//===============================================================================
//�@����
//===============================================================================
CFeed * CFeed::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, FEEDTYPE feedType)
{
	CFeed *pFeed = NULL;

	// NULL�`�F�b�N
	if (pFeed == NULL)
	{// �������̓��I�m��

		pFeed = new CFeed;

		if (pFeed != NULL)
		{
			// ��ނ̐ݒ�
			pFeed->BindModel(m_pMeshModel[feedType], m_pBuffMatModel[feedType], m_nNumMatModel[feedType], m_pMeshTextures,
				m_VtxMaxModel[feedType], m_VtxMinModel[feedType]);
			// �T�C�Y����
			pFeed->m_scale = scale;
			// �T�C�Y��e�N���X�ɑ��
			pFeed->SetScale(scale);
			// �a�̎�ނ���
			pFeed->m_feedType = feedType;
			// �I�u�W�F�N�g�N���X�̐���
			pFeed->Init();
			// �ʒu����
			pFeed->SetPosition(pos);
			// ��]�𔽉f
			pFeed->SetRot(rot);
		}
	}

	return pFeed;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CFeed::Init(void)
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
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CFeed::Uninit(void)
{
	//�I�u�W�F�N�g�r���{�[�h
	m_pObjBill = NULL;

	//3D���f����Uninit
	CModel3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CFeed::Update(void)
{
	CModel3D::Update();

	//�����̎擾
	float fLength = CModel3D::GetLength();

	if (CModel3D::GetDelete() == true) { Uninit(); }
}
//=============================================================================
// �`�揈��
//=============================================================================
void CFeed::Draw(void)
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
HRESULT CFeed::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL *pMat;

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(FEED_NAME_000, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[0], NULL, &m_nNumMatModel[0], &m_pMeshModel[0]);
	D3DXLoadMeshFromX(FEED_NAME_001, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[1], NULL, &m_nNumMatModel[1], &m_pMeshModel[1]);
	D3DXLoadMeshFromX(FEED_NAME_002, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[2], NULL, &m_nNumMatModel[2], &m_pMeshModel[2]);

	for (int nCount = 0; nCount < FEEDTYPE_MAX; nCount++)
	{
		//�}�e���A����񂩂�e�N�X�`���̎擾
		pMat = (D3DXMATERIAL*)m_pBuffMatModel[nCount]->GetBufferPointer();
	}

	int nNumVtx;		//���_��
	DWORD sizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

						//���f���̍ő�l�E�ŏ��l���擾����
	for (int nCntModel = 0; nCntModel < FEEDTYPE_MAX; nCntModel++)
	{
		m_VtxMaxModel[nCntModel] = D3DXVECTOR3(-10000, -10000, -10000);	//�ő�l
		m_VtxMinModel[nCntModel] = D3DXVECTOR3(10000, 10000, 10000);	//�ŏ��l

																		//���_�����擾
		nNumVtx = m_pMeshModel[nCntModel]->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(m_pMeshModel[nCntModel]->GetFVF());

		//���_�o�b�t�@�̃��b�N
		m_pMeshModel[nCntModel]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

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
		m_pMeshModel[nCntModel]->UnlockVertexBuffer();
	}


	//�g���Ă���e�N�X�`��
	//D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_1, &m_pMeshTextures[0]);

	return S_OK;
}

//===============================================================================
// �e�N�X�`���̔j��
//===============================================================================
void CFeed::UnLoad(void)
{
	for (int nCount = 0; nCount < FEEDTYPE_MAX; nCount++)
	{
		// ���b�V���̊J��
		if (m_pMeshModel[nCount] != NULL)
		{
			m_pMeshModel[nCount]->Release();
			m_pMeshModel[nCount] = NULL;
		}
		// �}�e���A���̊J��
		if (m_pBuffMatModel[nCount] != NULL)
		{
			m_pBuffMatModel[nCount]->Release();
			m_pBuffMatModel[nCount] = NULL;
		}
	}

	//�e�N�X�`��
	if (m_pMeshTextures != NULL)
	{
		m_pMeshTextures->Release();
		m_pMeshTextures = NULL;
	}
}


//===============================================================================
// �����蔻��
//===============================================================================
bool CFeed::CollisionFeed(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld)
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

	if (pPos->x >= ModelMin.x - PLAYER_DEPTH && pPos->x <= ModelMax.x + PLAYER_DEPTH)
	{// Z�͈͓̔��ɂ���
		if (pPos->z >= ModelMin.z - PLAYER_DEPTH && pPos->z <= ModelMax.z + PLAYER_DEPTH)
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

	return bHit;
}