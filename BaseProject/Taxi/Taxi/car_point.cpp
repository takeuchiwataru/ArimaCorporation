//=============================================================================
//
// �Ԃ̓����|�C���g�̏��� [car_point.cpp]
// Author : YUTARO�@ABE
//
//=============================================================================
#include "scene.h"
#include "model3D.h"
#include "manager.h"
#include "meshfield.h"
#include "player.h"
#include "fade.h"
#include "car_point.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_NAME_1	"data\\MODEL\\point\\point.x"					//�ǂݍ��ރ��f���t�@�C��

#define TEXTURE_NAME_1	"data\\TEXTURE\\modeltex\\carup.jpg"		//�ǂݍ��ރe�N�X�`���t�@�C��

#define MODEL_SPEED (5.0f)
#define CAR_DEPTH	(10)			// NPC�Ԃ̕������p
#define CAR_HEIGHT	(100.0f)		// NPC�Ԃ̔w�̍���

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPD3DXMESH CCarPoint::m_pMeshModel[MAX_CARPOINT] = {};						//���b�V�����ւ̃|�C���^
LPD3DXBUFFER CCarPoint::m_pBuffMatModel[MAX_CARPOINT] = {};					//�}�e���A���̏��ւ̃|�C���^
DWORD CCarPoint::m_nNumMatModel[MAX_CARPOINT] = {};							//�}�e���A���̏��
LPDIRECT3DTEXTURE9 CCarPoint::m_pMeshTextures[MAX_CARPOINT_TEXTURE] = {};
D3DXVECTOR3 CCarPoint::m_LoadVtxMaxModel[MAX_CARPOINT] = {};
D3DXVECTOR3 CCarPoint::m_LoadVtxMinModel[MAX_CARPOINT] = {};


//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CCarPoint::CCarPoint() : CModel3D(CARPOINT_PRIOTITY, CScene::OBJTYPE_CARPOINT)
{
	m_nObjNumber = 0;
	m_bLook = false;
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CCarPoint::~CCarPoint()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CCarPoint::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//3D���f����Init
	CModel3D::Init();

	// �ʒu�̏�����
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 scale = D3DXVECTOR3(3.0f, 5.0f, 3.0f);

	//�ʒu�̑��
	CModel3D::SetPosition(pos);
	CModel3D::SetScale(scale);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CCarPoint::Uninit(void)
{
	//3D���f����Uninit
	CModel3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CCarPoint::Update(void)
{
	//3D���f����Update
	CModel3D::Update();
	//�ʒu�̏�����
	D3DXVECTOR3 pos = CModel3D::GetPosition();
	// �ړ��ʂ̏�����
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//�����̑��
	D3DXVECTOR3 &rot = CModel3D::GetRot();
	//�傫���̏�����
	D3DXVECTOR3 scale = D3DXVECTOR3(3.0f, 5.0f, 3.0f);


	// �e����̑��
	CModel3D::SetScale(scale);
	CModel3D::SetPosition(pos);
	CModel3D::SetMove(move);
}
//=============================================================================
// �`�揈��
//=============================================================================
void CCarPoint::Draw(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();

	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���_�@���̎������K��
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	if (pCInputKeyBoard->GetKeyboardTrigger(DIK_RETURN) == true)
	{
		m_bLook = m_bLook ? false : true;
	}
	if (m_bLook == true)
	{
		//3D���f����Draw
		CModel3D::Draw();
	}

	//���_�@���̎������K��
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}
//===============================================================================
//�@�N���G�C�g
//===============================================================================
CCarPoint * CCarPoint::Create(int nNum ,D3DXVECTOR3 pos)
{
	CCarPoint *pCarPoint = NULL;

	// NULL�`�F�b�N
	if (pCarPoint == NULL)
	{// �������̓��I�m��
		pCarPoint = new CCarPoint;
		if (pCarPoint != NULL)
		{
			// ��ނ̐ݒ�
			pCarPoint->BindModel(m_pMeshModel[0], m_pBuffMatModel[0], m_nNumMatModel[0], m_pMeshTextures[0],
				m_LoadVtxMaxModel[0], m_LoadVtxMinModel[0]);
			// �I�u�W�F�N�g�N���X�̐���
			pCarPoint->Init();;
			// �ʒu����
			pCarPoint->SetPosition(pos);
			//	�|�C���g�̔ԍ�
			pCarPoint->m_nObjNumber = nNum;
		}
	}

	//	�l��Ԃ�
	return pCarPoint;
}
//===============================================================================
// X�t�@�C���̓ǂݍ���
//===============================================================================
HRESULT CCarPoint::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL *pMat;

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(MODEL_NAME_1, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[0], NULL, &m_nNumMatModel[0], &m_pMeshModel[0]);

	for (int nCount = 0; nCount < MAX_CARPOINT; nCount++)
	{
		//�}�e���A����񂩂�e�N�X�`���̎擾
		pMat = (D3DXMATERIAL*)m_pBuffMatModel[nCount]->GetBufferPointer();
	}

	int nNumVtx;		//���_��
	DWORD sizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

						//���f���̍ő�l�E�ŏ��l���擾����
	for (int nCntModel = 0; nCntModel < MAX_CARPOINT; nCntModel++)
	{
		m_LoadVtxMaxModel[nCntModel] = D3DXVECTOR3(-10000, -10000, -10000);	//�ő�l
		m_LoadVtxMinModel[nCntModel] = D3DXVECTOR3(10000, 10000, 10000);	//�ŏ��l

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
			if (vtx.x > m_LoadVtxMaxModel[nCntModel].x)
			{
				m_LoadVtxMaxModel[nCntModel].x = vtx.x;
			}
			if (vtx.y > m_LoadVtxMaxModel[nCntModel].y)
			{
				m_LoadVtxMaxModel[nCntModel].y = vtx.y;
			}
			if (vtx.z > m_LoadVtxMaxModel[nCntModel].z)
			{
				m_LoadVtxMaxModel[nCntModel].z = vtx.z;
			}
			//�ŏ��l
			if (vtx.x < m_LoadVtxMinModel[nCntModel].x)
			{
				m_LoadVtxMinModel[nCntModel].x = vtx.x;
			}
			if (vtx.y < m_LoadVtxMinModel[nCntModel].y)
			{
				m_LoadVtxMinModel[nCntModel].y = vtx.y;
			}
			if (vtx.z < m_LoadVtxMinModel[nCntModel].z)
			{
				m_LoadVtxMinModel[nCntModel].z = vtx.z;
			}

			//�T�C�Y���̃|�C���^��i�߂�
			pVtxBuff += sizeFVF;
		}

		//���_�o�b�t�@�̃A�����b�N
		m_pMeshModel[nCntModel]->UnlockVertexBuffer();
	}

	//�g���Ă���e�N�X�`��
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_1, &m_pMeshTextures[0]);

	return S_OK;
}
//===============================================================================
// �e�N�X�`���̔j��
//===============================================================================
void CCarPoint::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_CARPOINT; nCount++)
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
	for (int nCntTex = 0; nCntTex < MAX_CARPOINT_TEXTURE; nCntTex++)
	{
		if (m_pMeshTextures[nCntTex] != NULL)
		{
			m_pMeshTextures[nCntTex]->Release();
			m_pMeshTextures[nCntTex] = NULL;
		}
	}
}