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
#define TEXTURE_MESHFIELD_0		"data\\TEXTURE\\mesh\\field_000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_MESHFIELD_1		"data\\TEXTURE\\mesh\\field_001.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_MESHFIELD_2		"data\\TEXTURE\\mesh\\field_002.png"	// �ǂݍ��ރe�N�X�`���t�@�C����

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

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CMeshField::CMeshField()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nWidthDivide = 5;
	m_nDepthDivide = 5;
	m_fTextXUV = 1.0f;
	m_fTextYUV = 1.0f;
	m_fWidthLength = 500.0f;
	m_fDepthLength = 500.0f;
	m_fVtxHeight_No0 = 0.0f;
	m_fVtxHeight_No1 = 0.0f;
	m_fVtxHeight_No2 = 0.0f;
	m_fVtxHeight_No3 = 0.0f;
	m_fVtxSide_No0 = 0.0f;
	m_fVtxSide_No1 = 0.0f;
	m_fVtxSide_No2 = 0.0f;
	m_fVtxSide_No3 = 0.0f;
	m_nTexType = 0;
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CMeshField::~CMeshField() { }

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
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�Q�[���̏��
	CManager::MODE pMode = CManager::GetMode();

	CMesh::Draw();
}
//===============================================================================
//�@�N���G�C�g
//===============================================================================
CMeshField * CMeshField::Create(D3DXVECTOR3 pos, int nMeshX, int nMeshZ, float fMeshXUV, float fMeshYUV, float fMeshWidth, float fMeshDepth, float fVtx0, float fVtxMeshX, float fVtxMeshZ, float fVtxMeshXMeshZ, float fVtxSide0, float fVtxSide1, float fVtxSide2, float fVtxSide3, int nTexType, int nMeshType)
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
			//���b�V���̒��_�̕�
			pMeshField->SetVtxSide(fVtxSide0, fVtxSide1, fVtxSide2, fVtxSide3);
			//�I�u�W�F�N�g�N���X�̐���
			pMeshField->Init();
			//�e�N�X�`���̐ݒ�
			pMeshField->BindTexture(m_pTexture[nTexType]);

			//�t�@�C���������ݗp�ϐ�
			pMeshField->m_nWidthDivide = nMeshX;
			pMeshField->m_nDepthDivide = nMeshZ;
			pMeshField->m_fTextXUV = fMeshXUV;
			pMeshField->m_fTextYUV = fMeshYUV;
			pMeshField->m_fWidthLength = fMeshWidth;
			pMeshField->m_fDepthLength = fMeshDepth;
			pMeshField->m_fVtxHeight_No0 = fVtx0;
			pMeshField->m_fVtxHeight_No1 = fVtxMeshX;
			pMeshField->m_fVtxHeight_No2 = fVtxMeshZ;
			pMeshField->m_fVtxHeight_No3 = fVtxMeshXMeshZ;
			pMeshField->m_fVtxSide_No0 = fVtxSide0;
			pMeshField->m_fVtxSide_No1 = fVtxSide1;
			pMeshField->m_fVtxSide_No2 = fVtxSide2;
			pMeshField->m_fVtxSide_No3 = fVtxSide3;
			pMeshField->m_nTexType = nTexType;
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
