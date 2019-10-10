//=============================================================================
//
// ���b�V���̏��� [mesh.cpp]
// Author : �L�n���u
//
//=============================================================================
#include "meshfield.h"
#include "mesh.h"
#include "manager.h"
#include "scene.h"
#include "model.h"
#include "scene3D.h"
#include "game.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MESHFIELD_0		"data\\TEXTURE\\mesh\\stairs.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_MESHFIELD_1		"data\\TEXTURE\\mesh\\flooa000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_MESHFIELD_2		"data\\TEXTURE\\mesh\\field03.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_MESHFIELD_3		"data\\TEXTURE\\mesh\\green2.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_MESHFIELD_4		"data\\TEXTURE\\mesh\\flooa001.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_MESHFIELD_5		"data\\TEXTURE\\mesh\\flooa002.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_MESHFIELD_6		"data\\TEXTURE\\mesh\\���H�c.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_MESHFIELD_7		"data\\TEXTURE\\mesh\\���H��.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_MESHFIELD_8		"data\\TEXTURE\\mesh\\������.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_MESHFIELD_9		"data\\TEXTURE\\mesh\\���s���c.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_MESHFIELD_10	"data\\TEXTURE\\mesh\\���s����.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_MESHFIELD_11	"data\\TEXTURE\\mesh\\�����_.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_MESHFIELD_12	"data\\TEXTURE\\mesh\\flooa003.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_MESHFIELD_13	"data\\TEXTURE\\mesh\\water.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_MESHFIELD_14	"data\\TEXTURE\\mesh\\col.jpg"		// �ǂݍ��ރe�N�X�`���t�@�C����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CMeshField::m_pTexture[MAX_MESH_TEXTURE] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 CMeshField::m_pVtxBuff = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 CMeshField::m_pIdxBuff = NULL;				// �C���f�b�N�X�ւ̃|�C���^
//int CMeshField::m_nMeshType = 0;
//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CMeshField::CMeshField()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CMeshField::~CMeshField()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CMeshField::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	CMesh::Init();

	CScene::SetObjType(CScene::OBJTYPE_GROUND);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CMeshField::Uninit(void)
{
	CMesh::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CMeshField::Update(void)
{
	CMesh::Update();
}
//=============================================================================
// �`�揈��
//=============================================================================
void CMeshField::Draw(void)
{
	CMesh::Draw();
}
//===============================================================================
//�@�N���G�C�g
//===============================================================================
CMeshField * CMeshField::Create(D3DXVECTOR3 pos,int nMeshX, int nMeshZ, float fMeshXUV, float fMeshYUV, float fMeshWidth, float fMeshDepth,float fVtx0, float fVtxMeshX, float fVtxMeshZ, float fVtxMeshXMeshZ, int nTexType,int nMeshType)
{
	CMeshField *pMeshField = NULL;

	//NULL�`�F�b�N
	if (pMeshField == NULL)
	{//�������̓��I�m��

		pMeshField = new CMeshField;

		if (pMeshField != NULL)
		{
			//�ʒu��ϐ��ɑ��
			pMeshField->m_pos = pos;
			//�ʒu�̑��
			pMeshField->SetPosition(pos);
			//���b�V���̎��
			pMeshField->SetMeshType(nMeshType);
			//���b�V���̏���e�N���X�ɓn��
			pMeshField->SetMeshInfo(nMeshX, nMeshZ, fMeshXUV, fMeshYUV, fMeshDepth, fMeshWidth);
			//���b�V���̒��_�̍���
			pMeshField->SetVtx(fVtx0, fVtxMeshX, fVtxMeshZ, fVtxMeshXMeshZ);
			//�I�u�W�F�N�g�N���X�̐���
			pMeshField->Init();
			//�e�N�X�`���̐ݒ�
			pMeshField->BindTexture(m_pTexture[nTexType]);

			pMeshField->m_nMeshType = nTexType;
		}
	}

	return pMeshField;
}
//===============================================================================
// �e�N�X�`���̓ǂݍ���
//===============================================================================
HRESULT CMeshField::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD_0, &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD_1, &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD_2, &m_pTexture[2]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD_3, &m_pTexture[3]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD_4, &m_pTexture[4]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD_5, &m_pTexture[5]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD_6, &m_pTexture[6]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD_7, &m_pTexture[7]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD_8, &m_pTexture[8]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD_9, &m_pTexture[9]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD_10, &m_pTexture[10]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD_11, &m_pTexture[11]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD_12, &m_pTexture[12]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD_13, &m_pTexture[14]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD_14, &m_pTexture[15]);

	return S_OK;
}
//===============================================================================
// �e�N�X�`���̔j��
//===============================================================================
void CMeshField::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_MESH_TEXTURE; nCount++)
	{
		//�e�N�X�`���[�̔j��
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}
//=============================================================================
//	�X�e�[�W�ړ����ɏ��������邽��
//=============================================================================
void CMeshField::DeleteMeshField(void)
{
	CScene *pScene;

	//�v���C�I���e�B�[�`�F�b�N
	pScene = CScene::GetTop(MESH_PRIOTITY);

	//NULL�`�F�b�N
	while (pScene != NULL)
	{
		//Update��Uninit����Ă��܂��ꍇ�@Next��������\�������邩��Next�Ƀf�[�^���c���Ă���
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetDeath() == false)
		{//�^�C�v���ǂ�������
			if (pScene->GetObjType() == OBJTYPE_MESH)
			{
				pScene->Uninit();
			}
		}
		//Next�Ɏ���Scene������
		pScene = pSceneNext;
	}
}

