//=============================================================================
//
// ���b�V���̏��� [mesh.cpp]
// Author : �L�n���u
//
//=============================================================================
#include "meshfield.h"
#include "markmeshfield.h"
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
CMeshField::CMeshField() : CMesh(MESHFIELD_PRIOTITY, CScene::OBJTYPE_MESHFILED)
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

	//�v���C���[�̈ʒu���
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();
	//�Q�[���̏��
	CManager::MODE pMode = CManager::GetMode();

	CMesh::Draw();
}
//===============================================================================
//�@�N���G�C�g
//===============================================================================
CMeshField * CMeshField::Create(D3DXVECTOR3 pos,int nMeshX, int nMeshZ, float fMeshXUV, float fMeshYUV, float fMeshWidth, float fMeshDepth, float fVtx0, float fVtxMeshX, float fVtxMeshZ, float fVtxMeshXMeshZ, int nTexType,int nMeshType)
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
//===============================================================================
// �t�@�C���ɃZ�[�u
//===============================================================================
void CMeshField::TextSave(void)
{
	int nMarkMeshField = CMarkMeshField::GetMeshFieldNum();

	CScene *pScene;

	//�v���C�I���e�B�[�`�F�b�N
	pScene = CScene::GetTop(MESHFIELD_PRIOTITY);

	//�t�@�C���̃|�C���^
	FILE *pFile;

	//�t�@�C���ǂݎ��ݒ�
	pFile = fopen(MESHFILE_NAME, "wb");

	//�v���C���[�̃e�L�X�g�f�[�^�̓ǂݍ���
	if (pFile != NULL)
	{
		//������
		fprintf(pFile, "#====================================================\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#�@�}�b�v�̃��b�V���t�B�[���h�z�u�̃G�f�B�^ [meshfield.txt]\n");
		fprintf(pFile, "#�@����� : �L�n�@���u\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#====================================================\n\n");
		//�u�����I�u�W�F�N�g�̐�
		fprintf(pFile, "#----------------------------------------------------\n");
		fprintf(pFile, "#�@���b�V���t�B�[���h�̐ݒu��\n");
		fprintf(pFile, "#----------------------------------------------------\n");
		fprintf(pFile, "MESHFIELD_SETNUM = ");
		fprintf(pFile, "%d\n\n", nMarkMeshField);
		//������
		fprintf(pFile, "#----------------------------------------------------\n");
		fprintf(pFile, "#�@���b�V���t�B�[���h�̐ݒu���\n");
		fprintf(pFile, "#----------------------------------------------------\n");

		//NULL�`�F�b�N
		while (pScene != NULL)
		{
			//Update��Uninit����Ă��܂��ꍇ�@Next��������\�������邩��Next�Ƀf�[�^���c���Ă���
			CScene *pSceneNext = pScene->GetNext();

			if (pScene->GetDeath() == false)
			{
				//�^�C�v���I�u�W�F�N�g��������
				if (pScene->GetObjType() == OBJTYPE_MESHFILED)
				{
					//�J�n�̂��߂̐錾
					fprintf(pFile, "MESHFIELD_START\n");
					//�e�N�X�`���̎�ނ�����
					fprintf(pFile, "	TEXTURETYPE = ");
					fprintf(pFile, "%d\n", ((CMeshField*)pScene)->m_nTexType); 
					//���̕�����������
					fprintf(pFile, "	X_DIVIDE = ");
					fprintf(pFile, "%d\n", ((CMeshField*)pScene)->m_nWidthDivide); 
					//�c�̕�����������
					fprintf(pFile, "	Z_DIVIDE = ");
					fprintf(pFile, "%d\n", ((CMeshField*)pScene)->m_nDepthDivide);
					//X�̃e�N�X�`�����W������
					fprintf(pFile, "	X_TEXUV = ");
					fprintf(pFile, "%.1f\n", ((CMeshField*)pScene)->m_fTextXUV);
					//Y�̃e�N�X�`�����W������
					fprintf(pFile, "	Y_TEXUV = ");
					fprintf(pFile, "%.1f\n", ((CMeshField*)pScene)->m_fTextYUV);
					//���̒���������
					fprintf(pFile, "	X_LENGTH = ");
					fprintf(pFile, "%.2f\n", ((CMeshField*)pScene)->m_fWidthLength);
					//�c�̒���������
					fprintf(pFile, "	Z_LENGTH = ");
					fprintf(pFile, "%.2f\n", ((CMeshField*)pScene)->m_fDepthLength);
					//0�Ԃ̒��_�̍���������
					fprintf(pFile, "	VTX0_HEIGHT = ");
					fprintf(pFile, "%.2f\n", ((CMeshField*)pScene)->m_fVtxHeight_No0);
					//1�Ԃ̒��_�̍���������
					fprintf(pFile, "	VTX1_HEIGHT = ");
					fprintf(pFile, "%.2f\n", ((CMeshField*)pScene)->m_fVtxHeight_No1);
					//2�Ԃ̒��_�̍���������
					fprintf(pFile, "	VTX2_HEIGHT = ");
					fprintf(pFile, "%.2f\n", ((CMeshField*)pScene)->m_fVtxHeight_No2);
					//3�Ԃ̒��_�̍���������
					fprintf(pFile, "	VTX3_HEIGHT = ");
					fprintf(pFile, "%.2f\n", ((CMeshField*)pScene)->m_fVtxHeight_No3);
					//�ʒu������
					fprintf(pFile, "	POS = ");
					fprintf(pFile, "%.2f ", ((CMeshField*)pScene)->m_pos.x);
					fprintf(pFile, "%.2f ", ((CMeshField*)pScene)->m_pos.y);
					fprintf(pFile, "%.2f\n", ((CMeshField*)pScene)->m_pos.z);
					//�J�n�̂��߂̐錾
					fprintf(pFile, "MESHFIELD_END\n\n");
				}
			}

			//Next�Ɏ���Scene������
			pScene = pSceneNext;
		}

		//�t�@�C�������
		fclose(pFile);
	}
}

