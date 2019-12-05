//=============================================================================
//
// �I�u�W�F�N�g�̏��� [model3D.cpp]
// Author : �L�n���u
//
//=============================================================================
#include "scene.h"
#include "model3D.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include <math.h>
#include "billboord.h"
#include "gamecamera.h"
#include "player.h"
#include "resultcamera.h"
#include "tutorial.h"
#include "toonshader.h"
#include "camerabace.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPD3DXMESH			CModel3D::m_pMeshModel[MODEL_TYPE_MAX] = {};			// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		CModel3D::m_pBuffMatModel[MODEL_TYPE_MAX] = {};			// �}�e���A���̏��ւ̃|�C���^
DWORD				CModel3D::m_nNumMatModel[MODEL_TYPE_MAX] = {};			// �}�e���A���̏��
LPDIRECT3DTEXTURE9  CModel3D::m_pMeshTextures[TEXTURE_TYPE_MAX] = {};		// �e�N�X�`���̏��
CToonShader		   *CModel3D::m_pToonShader = NULL;							// �V�F�[�_�[�̃|�C���^
D3DMATERIAL9	   *CModel3D::m_pMeshMaterials[MODEL_TYPE_MAX] = {};		// ���b�V���}�e���A���̏��
LPDIRECT3DTEXTURE9 *CModel3D::m_pShaderMeshTextures[MODEL_TYPE_MAX] = {};	// �V�F�[�_�[�p�̃��b�V���e�N�X�`��

//*****************************************************************************
//					 ���f���̓ǂݍ��݃t�@�C����
//*****************************************************************************
const char *CModel3D::m_apModelFile[MODEL_TYPE_MAX] =
{
	//=============================
	//�@         �}�b�v
	//=============================
	"data\\MODEL\\Object\\tree4.x",
	"data\\MODEL\\Object\\��.x",
	"data\\MODEL\\Collision\\box.x",
	"data\\MODEL\\Map\\Map_First.x",
	"data\\MODEL\\Map\\Map_Second.x",
	"data\\MODEL\\Map\\Map_Mountain.x",
	"data\\MODEL\\Map\\Map_Sky_1.x",
	"data\\MODEL\\Object\\�j���g������.x",
	"data\\MODEL\\Object\\L_�Ŕ�.x",
	"data\\MODEL\\Object\\R_�Ŕ�.x",
	"data\\MODEL\\Object\\LR_�Ŕ�.x",
	"data\\MODEL\\Object\\��.x",
	"data\\MODEL\\Object\\�Α�.x",
	"data\\MODEL\\Object\\�\����.x",
	"data\\MODEL\\Object\\bigkanban.x",
	"data\\MODEL\\Object\\��.x",
	"data\\MODEL\\Object\\�S�[��.x",

	//=============================
	//�@          �a
	//=============================
	"data\\MODEL\\Item\\��.x",
	"data\\MODEL\\Item\\�u�h�E.x",
	"data\\MODEL\\Item\\�g�E�����R�V.x",

	//=============================
	//�@          ��
	//=============================
	"data\\MODEL\\Weapon\\egg.x",

	//=============================
	//�@          �q���R
	//=============================
	"data\\MODEL\\Weapon\\chick.x",

	//=============================
	//�@          �G�t�F�N�g
	//=============================
	"data\\MODEL\\Effect\\Dirt.x"
};
//*****************************************************************************
//						 �e�N�X�`���̓ǂݍ��݃t�@�C����
//*****************************************************************************
const char *CModel3D::m_apTextureFile[TEXTURE_TYPE_MAX] =
{
	//=============================
	//�@         �}�b�v
	//=============================
	"data\\TEXTURE\\modeltex\\tree000.jpg",
	"data\\TEXTURE\\modeltex\\��.jpg",
	"data\\TEXTURE\\modeltex\\bender.jpg",
	"data\\TEXTURE\\modeltex\\meat.jpg",
	"data\\TEXTURE\\modeltex\\meat.jpg",
	"data\\TEXTURE\\modeltex\\Mountain.jpg",
	"data\\TEXTURE\\modeltex\\Map_Sky.jpg",
	"data\\TEXTURE\\modeltex\\�j���g������.png",
	"data\\TEXTURE\\modeltex\\�Ŕ�.jpg",
	"data\\TEXTURE\\modeltex\\�Ŕ�.jpg",
	"data\\TEXTURE\\modeltex\\�Ŕ�.jpg",
	"data\\TEXTURE\\modeltex\\��.jpg",
	"data\\TEXTURE\\modeltex\\�Α���.jpg",
	"data\\TEXTURE\\modeltex\\�\����.jpg",
	"data\\TEXTURE\\modeltex\\bigkanban.jpg",
	"data\\TEXTURE\\modeltex\\��.jpg",
	"data\\TEXTURE\\modeltex\\Snap000.jpg",

	//=============================
	//�@          �a
	//=============================
	"data\\TEXTURE\\modeltex\\��.jpg",
	"data\\TEXTURE\\modeltex\\�u�h�E.jpg",
	"data\\TEXTURE\\modeltex\\�g�E�����R�V.jpg",

	//============================
	//�@          ��
	//=============================
	"data\\TEXTURE\\modeltex\\egg02.jpg",
	"data\\TEXTURE\\modeltex\\egg00.jpg",
	"data\\TEXTURE\\modeltex\\egg01.jpg",

	//=============================
	//�@          �q���R
	//=============================
	"data\\TEXTURE\\modeltex\\egg02.jpg",
	"data\\TEXTURE\\modeltex\\egg00.jpg",
	"data\\TEXTURE\\modeltex\\egg01.jpg",
	"data\\TEXTURE\\modeltex\\egg02.jpg",
	"data\\TEXTURE\\modeltex\\egg00.jpg",
	"data\\TEXTURE\\modeltex\\egg01.jpg",

	//=============================
	//�@          �G�t�F�N�g
	//=============================
	"data\\TEXTURE\\modeltex\\ModelEffect.jpg"
};
//===============================================================================
//�@�I�[�o�[���[�h�R���X�g���N�^
//===============================================================================
CModel3D::CModel3D(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_mtxWorldObject;							//���[���h�}�g���b�N�X
	m_VtxMinModel, m_VtxMaxModel;				//���f���̍ŏ��l�E�ő�l
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	//�傫��
	m_VecAxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fValueRot = 0.0f;
	m_bcolChange = false;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �J���[
	m_bTexMat = false;
	m_MapView = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_modeltype = MODEL_TYPE_MAX;
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CModel3D::~CModel3D() {}

//===============================================================================
//�@�X�ǂݍ��ݏ���
//===============================================================================
LPD3DXMESH	&CModel3D::MeshLoad(MODEL_TYPE modeltype)
{
	if (modeltype >= MODEL_TYPE_MAX) { return m_pMeshModel[0]; }
	if (m_pMeshMaterials[modeltype] == NULL)
	{
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		//���f���̓ǂݍ���
		D3DXLoadMeshFromX(m_apModelFile[modeltype], D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[modeltype], NULL, &m_nNumMatModel[modeltype], &m_pMeshModel[modeltype]);
		D3DXMATERIAL *d3dxMaterials = (D3DXMATERIAL*)m_pBuffMatModel[modeltype]->GetBufferPointer();

		//�}�e���A�����ƃV�F�[�_�̃e�N�X�`���𓮓I�m��
		m_pMeshMaterials[modeltype] = new D3DMATERIAL9[(int)m_nNumMatModel[modeltype]];
		m_pShaderMeshTextures[modeltype] = new LPDIRECT3DTEXTURE9[(int)m_nNumMatModel[modeltype]];

		for (DWORD MatCount = 0; MatCount < (int)m_nNumMatModel[modeltype]; MatCount++)
		{
			m_pMeshMaterials[modeltype][MatCount] = d3dxMaterials[MatCount].MatD3D;
			m_pMeshMaterials[modeltype][MatCount].Ambient = m_pMeshMaterials[modeltype][MatCount].Diffuse;
			m_pShaderMeshTextures[modeltype][MatCount] = NULL;

			if (d3dxMaterials[MatCount].pTextureFilename != NULL &&
				lstrlen(d3dxMaterials[MatCount].pTextureFilename) > 0)
			{
				D3DXCreateTextureFromFile(pDevice,
					d3dxMaterials[MatCount].pTextureFilename,
					&m_pShaderMeshTextures[modeltype][MatCount]);
			}
		}
	}

	//�V�F�[�_�[�̓ǂݍ��ރt�@�C��
	if (m_pToonShader == NULL)
	{
		m_pToonShader = new CToonShader;
		m_pToonShader->Init("data\\SHADER\\ToonShader.fx");
	}

	return m_pMeshModel[modeltype];
}
//===============================================================================
//�@�J���̏���
//===============================================================================
void CModel3D::UnLoad(void)
{
	for (int nCount = 0; nCount < MODEL_TYPE_MAX; nCount++)
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

	for (int nCount = 0; nCount < TEXTURE_TYPE_MAX; nCount++)
	{
		//�e�N�X�`���̔j��
		if (m_pMeshTextures[nCount] != NULL)
		{
			m_pMeshTextures[nCount]->Release();
			m_pMeshTextures[nCount] = NULL;
		}
	}
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CModel3D::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//�ʒu
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//�����B����

	m_VtxMaxModel = D3DXVECTOR3(m_VtxMaxModel.x * m_Scale.x, m_VtxMaxModel.y * m_Scale.y, m_VtxMaxModel.z * m_Scale.z);
	m_VtxMinModel = D3DXVECTOR3(m_VtxMinModel.x * m_Scale.x, m_VtxMinModel.y * m_Scale.y, m_VtxMinModel.z * m_Scale.z);

	m_Rot.y = 0.0f;

	//��ނ�����
	m_modeltype = (MODEL_TYPE)m_nModelType;

	MeshLoad(m_modeltype);

	if (m_pMeshTextures[m_nTextureType] == NULL)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, m_apTextureFile[m_nTextureType], &m_pMeshTextures[m_nTextureType]);
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CModel3D::Uninit(void)
{
	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CModel3D::Update(void)
{
	//���݂̃��[�h���擾
	CManager::MODE mode = CManager::GetMode();

	//�Q�[�����[�h�Ȃ�N���b�s���O�̏���
	if (CManager::MODE_GAME == mode)
	{
		//SetDraw(CGame::GetGameCamera(0)->Clipping(m_VtxMinModel + m_Pos, m_VtxMaxModel + m_Pos));
	}

	if ((CManager::MODE_GAME == mode) || (CManager::MODE_TUTORIAL == mode))
	{
		// �ʒu�̕ۑ�
		D3DXVECTOR3 posOld = m_Pos;

		// �d��
		m_Move.y -= cosf(0) * 0.5f;

		// �ړ��ʂ̊���
		m_Move.x += (0 - m_Move.x) * 0.04f;
		m_Move.z += (0 - m_Move.z) * 0.04f;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CModel3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX		mtxRot, mtxTrans, mtxSpin, mtxScale;	//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9	matDef;									//���݂̃}�e���A����ۑ�
	D3DXMATERIAL	*pMat;									//�}�e���A���f�[�^�ւ̃|�C���^
	LPD3DXEFFECT	Shader = NULL;							//�V�F�[�_�[
	CCamera			*pCamera = NULL;						//�J�����̃|�C���g

	//�J�����̃|�C���^�ɏ�����
	if (pCamera == NULL)
	{
		switch (CManager::GetMode())
		{// ���[�h
		case CManager::MODE_TITLE:
			pCamera = (CCamera*)CTitle::GetTitleCamera();
			break;
		case CManager::MODE_GAME:
			switch (CGame::GetGameMode())
			{// �Q�[�����[�h
			case CGame::GAMEMODE_CHARSELECT:
				if (CGame::GetCameraNumber() == -1)
					pCamera = (CCamera*)CGame::GetCourseCamera();
				else
					pCamera = (CCamera*)CGame::GetGameCamera(CGame::GetCameraNumber());
				break;
			case CGame::GAMEMODE_COURSESELECT:
			case CGame::GAMEMODE_COURSE_VIEW:
				pCamera = (CCamera*)CGame::GetCourseCamera();
				break;
			case CGame::GAMEMODE_PLAY:
				if (CGame::GetCameraNumber() == -1)
					pCamera = (CCamera*)CGame::GetCourseCamera();
				else
					pCamera = (CCamera*)CGame::GetGameCamera(CGame::GetCameraNumber());
				break;
			}
			break;
		case CManager::MODE_RESULT:
			pCamera = (CCamera*)CResult::GetResultCamera();
			break;
		}
	}

	//�V�F�[�_�[�ɏ�����
	if (m_pToonShader != NULL)
	{
		Shader = m_pToonShader->GetShader();
	}

	//�V�F�[�_�[�̒��g������ꍇ
	if (Shader != NULL)
	{
		LPDIRECT3DTEXTURE9 ShaderTex = m_pToonShader->GetTexture();
		LPDIRECT3DTEXTURE9 LineTex = m_pToonShader->GetLineTexture();

		LPDIRECT3DTEXTURE9 ShaderMapTex = m_pToonShader->GetMapTexture();
		LPDIRECT3DTEXTURE9 LineMapTex = m_pToonShader->GetLineMapTexture();

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_mtxWorldObject);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
		D3DXMatrixMultiply(&m_mtxWorldObject, &m_mtxWorldObject, &mtxRot);

		// �N�H�[�^�j�I��
		D3DXQuaternionRotationAxis(&m_quat, &m_VecAxis, m_fValueRot);

		// �N�H�[�^�j�I�������]�s��
		D3DXMatrixRotationQuaternion(&mtxSpin, &m_quat);

		// ���[�J���ŏo���l��ۑ�
		m_mtxRot = mtxSpin;

		// �|�����킹��
		D3DXMatrixMultiply(&m_mtxWorldObject, &m_mtxWorldObject, &m_mtxRot);

		//// �g��k���s��̍쐬
		//D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
		//D3DXMatrixMultiply(&m_mtxWorldObject, &m_mtxWorldObject, &mtxScale);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
		D3DXMatrixMultiply(&m_mtxWorldObject, &m_mtxWorldObject, &mtxTrans);

		//�g�又��
		m_mtxWorldObject._44 = (1.0f / m_Scale.x);
		m_mtxWorldObject._41 *= m_mtxWorldObject._44;
		m_mtxWorldObject._42 *= m_mtxWorldObject._44;
		m_mtxWorldObject._43 *= m_mtxWorldObject._44;

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorldObject);

		//===================================================
		//    �@�@�@�@�@�@�V�F�[�_�[�̏��ݒ�
		//===================================================
		//�e�N�j�b�N�̐ݒ�
		Shader->SetTechnique("ToonShading");

		//�}�g���b�N�X��n��
		D3DXMATRIX mtxView = pCamera->GetViewMatrix();
		D3DXMATRIX mtxProjection = pCamera->GetProjectionMatrix();
		D3DXVECTOR3 ViewPosV = pCamera->GetCameraPosV();

		//�V�F�[�_�[�Ƀ}�g���b�N�X��n��
		Shader->SetMatrix("matProj", &mtxProjection);
		Shader->SetMatrix("matView", &mtxView);
		Shader->SetMatrix("matWorld", &m_mtxWorldObject);

		if (m_nModelType == 3 || m_nModelType == 4 || m_nModelType == 5)
		{//�}�b�v
			Shader->SetTexture("ShadeTexture", ShaderMapTex);
			Shader->SetTexture("LineTexture", LineMapTex);
		}
		else if (m_nModelType != 3 && m_nModelType != 4 && m_nModelType != 5)
		{//����ȊO
			Shader->SetTexture("ShadeTexture", ShaderTex);
			Shader->SetTexture("LineTexture", LineTex);
		}

		Shader->SetVector("EyePos", (D3DXVECTOR4*)&ViewPosV);

		//�V�F�[�_�J�n
		Shader->Begin(NULL, 0);

		if (m_pBuffMatModel[m_modeltype] != NULL)
		{
			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)m_pBuffMatModel[m_modeltype]->GetBufferPointer();
			D3DCOLORVALUE col;

			for (int nCntMat = 0; nCntMat < (int)m_nNumMatModel[m_modeltype]; nCntMat++)
			{
				//�V�F�[�_�̃f�[�^�Ƀ��f���̃e�N�X�`��������
				m_pShaderMeshTextures[m_modeltype][nCntMat] = m_pMeshTextures[m_nTextureType];

				if (m_bcolChange == true)
				{// �J���[�ύX(�����x)
					col = m_pMeshMaterials[m_modeltype][nCntMat].Diffuse;

					m_pMeshMaterials[m_modeltype][nCntMat].Diffuse.a = m_col.a;
					m_pMeshMaterials[m_modeltype][nCntMat].Diffuse.r = m_col.r;
					m_pMeshMaterials[m_modeltype][nCntMat].Diffuse.g = m_col.g;
					m_pMeshMaterials[m_modeltype][nCntMat].Diffuse.b = m_col.b;
				}

				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`��������ꍇ
				pDevice->SetTexture(0, m_pShaderMeshTextures[m_modeltype][nCntMat]);

				// �J���[�ύX
				//if (m_bcolChange == true)
				//{
				//	pMat[nCntMat].MatD3D.Diffuse.a = fcol_a;
				//	pMat[nCntMat].MatD3D.Diffuse.r = fcol_r;
				//	pMat[nCntMat].MatD3D.Diffuse.g = fcol_g;
				//	pMat[nCntMat].MatD3D.Diffuse.b = fcol_b;
				//}

				//===================================================
				//    �@�@�@�@�V�F�[�_�[�̊��蓖�č��
				//===================================================
				//if (m_nModelType != 3 && m_nModelType != 4)
				{
					//�p�X���w�肵�ĊJ�n
					Shader->BeginPass(0);

					//�V�F�[�_�[�e�N�X�`���̗L���ɉ����ăV�F�[�_�[������
					if (m_pShaderMeshTextures[m_modeltype][nCntMat] != NULL)
					{
						//�e�N�X�`��������ꍇ
						m_bTexMat = true;
						//�V�F�[�_�[��bool�̏���n��
						Shader->SetBool("TexMat", m_bTexMat);
					}
					else if (m_pShaderMeshTextures[m_modeltype][nCntMat] == NULL)
					{
						//�e�N�X�`�����Ȃ��ꍇ
						m_bTexMat = false;
						//�V�F�[�_�[��bool�̏���n��11111
						Shader->SetBool("TexMat", m_bTexMat);
					}

					//�I�u�W�F�N�g�̃e�N�X�`��
					Shader->SetTexture("DecalTexture", m_pShaderMeshTextures[m_modeltype][nCntMat]);

					//�v���p�e�B�[��,NameID,�ݒ肷��z��̒l
					Shader->SetFloatArray("Diffuse", (FLOAT*)&m_pMeshMaterials[m_modeltype][nCntMat].Diffuse, 4);

					//�ύX����ɂȂ�f�B���N�g�� �X�g�A�ɕۑ�
					Shader->CommitChanges();
				}

				//�I�u�W�F�N�g(�p�[�c)�̕`��
				m_pMeshModel[m_modeltype]->DrawSubset(nCntMat);

				//�p�X�̏I��
				Shader->EndPass();
			}

			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef);
			m_mtxWorldObject._44 = 1.0f;

			m_bcolChange = false;	// �ύX�I��

		}
		//�V�F�[�_�̏I��
		Shader->End();
	}
}
//===============================================================================
//�@�V�F�[�_�̏��̍폜
//===============================================================================
void CModel3D::ModelShaderDeleter(void)
{
	for (int nCount = 0; nCount < MODEL_TYPE_MAX; nCount++)
	{
		//�V�F�[�_�[�c�[��������
		if (m_pMeshMaterials[nCount] != NULL)
		{
			delete[] m_pMeshMaterials[nCount];
			m_pMeshMaterials[nCount] = NULL;
		}
		if (m_pShaderMeshTextures[nCount] != NULL)
		{
			delete[] m_pShaderMeshTextures[nCount];
			m_pShaderMeshTextures[nCount] = NULL;
		}
	}

	//�V�F�[�_�[�̃|�C���^�̔j��
	if (m_pToonShader != NULL)
	{
		m_pToonShader->Uninit();
		delete m_pToonShader;
		m_pToonShader = NULL;
	}
}
//===============================================================================
//�@�N���G�C�g
//===============================================================================
CModel3D * CModel3D::Create(void)
{
	CModel3D *pModel3D = NULL;

	//NULL�`�F�b�N
	if (pModel3D == NULL)
	{//�������̓��I�m��

		pModel3D = new CModel3D;

		if (pModel3D != NULL)
		{
			//�I�u�W�F�N�g�N���X�̐���
			pModel3D->Init();
		}
	}

	return pModel3D;
}

//===============================================================================
// �O�ς̓����蔻��
//===============================================================================
bool CModel3D::Collision(D3DXVECTOR3 pos, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin, D3DXVECTOR3 move)
{
	bool bHit = false;
	bool bHitX = false, bHitY = false, bHitZ = false;
	D3DXVECTOR3 vtxMaxObject = D3DXVECTOR3(m_mtxWorldObject._41, m_mtxWorldObject._42, m_mtxWorldObject._43);	// ���[���h���W�̎擾
	D3DXVECTOR3 vtxMinObject = D3DXVECTOR3(m_mtxWorldObject._41, m_mtxWorldObject._42, m_mtxWorldObject._43);	// ���[���h���W�̎擾

																												// ���_���W�̍ő�l�����߂�
	vtxMaxObject.x += m_VtxMaxModel.x * m_Scale.x;
	vtxMaxObject.y += m_VtxMaxModel.y * m_Scale.y;
	vtxMaxObject.z += m_VtxMaxModel.z * m_Scale.z;

	// ���_���W�̍ŏ��l�����߂�
	vtxMinObject.x += m_VtxMinModel.x * m_Scale.x;
	vtxMinObject.y += m_VtxMinModel.y * m_Scale.y;
	vtxMinObject.z += m_VtxMinModel.z * m_Scale.z;

	D3DXVECTOR3 vecA, vecB, vecC;

	/********************************/
	/*			X�̊O�όv�Z			*/
	/********************************/
	// �ŏ��l���̔���
	vecA = D3DXVECTOR3(vtxMinObject.x, 0.0f, vtxMaxObject.z) - D3DXVECTOR3(vtxMinObject.x, 0.0f, vtxMinObject.z);
	vecB = (pos + vtxMax) - D3DXVECTOR3(vtxMinObject.x, 0.0f, vtxMinObject.z);
	vecC = (pos + vtxMin) - D3DXVECTOR3(vtxMinObject.x, 0.0f, vtxMinObject.z);

	if ((vecA.z * vecB.x) - (vecA.x * vecB.z) > 0 || (vecA.z * vecC.x) - (vecA.x * vecC.z) > 0)
	{// �͈͓�
	 // �ő吔���̔���
		vecA = D3DXVECTOR3(vtxMaxObject.x, 0.0f, vtxMinObject.z) - D3DXVECTOR3(vtxMaxObject.x, 0.0f, vtxMaxObject.z);
		vecB = (pos + vtxMin) - D3DXVECTOR3(vtxMaxObject.x, 0.0f, vtxMaxObject.z);
		vecC = (pos + vtxMax) - D3DXVECTOR3(vtxMaxObject.x, 0.0f, vtxMaxObject.z);

		if ((vecA.z * vecB.x) - (vecA.x * vecB.z) > 0 || (vecA.z * vecC.x) - (vecA.x * vecC.z) > 0)
		{// �͈͓�
			bHitX = true;
		}
		else { return false; }
	}
	else { return false; }

	/********************************/
	/*			Y�̊O�όv�Z			*/
	/********************************/
	// �ŏ��l���̔���
	vecA = D3DXVECTOR3(vtxMinObject.x, vtxMinObject.y, 0.0f) - D3DXVECTOR3(vtxMaxObject.x, vtxMinObject.y, 0.0f);
	vecB = (pos + vtxMax) - D3DXVECTOR3(vtxMaxObject.x, vtxMinObject.y, 0.0f);
	vecC = (pos + vtxMin) - D3DXVECTOR3(vtxMaxObject.x, vtxMinObject.y, 0.0f);

	if ((vecA.y * vecB.x) - (vecA.x * vecB.y) > 0 || (vecA.y * vecC.x) - (vecA.x * vecC.y) > 0)
	{// �͈͓�
	 // �ő吔���̔���
		vecA = D3DXVECTOR3(vtxMaxObject.x, vtxMaxObject.y, 0.0f) - D3DXVECTOR3(vtxMinObject.x, vtxMaxObject.y, 0.0f);
		vecB = (pos + vtxMin) - D3DXVECTOR3(vtxMinObject.x, vtxMaxObject.y, 0.0f);
		vecC = (pos + vtxMax) - D3DXVECTOR3(vtxMinObject.x, vtxMaxObject.y, 0.0f);

		if ((vecA.y * vecB.x) - (vecA.x * vecB.y) > 0 || (vecA.y * vecC.x) - (vecA.x * vecC.y) > 0)
		{// �͈͓�
			bHitY = true;
		}
		else { return false; }
	}
	else { return false; }

	/********************************/
	/*			Z�̊O�όv�Z			*/
	/********************************/
	// �ŏ��l���̔���
	vecA = D3DXVECTOR3(vtxMinObject.x, 0.0f, vtxMinObject.z) - D3DXVECTOR3(vtxMaxObject.x, 0.0f, vtxMinObject.z);
	vecB = (pos + vtxMax) - D3DXVECTOR3(vtxMaxObject.x, 0.0f, vtxMinObject.z);
	vecC = (pos + vtxMin) - D3DXVECTOR3(vtxMaxObject.x, 0.0f, vtxMinObject.z);

	if ((vecA.z * vecB.x) - (vecA.x * vecB.z) > 0 || (vecA.z * vecC.x) - (vecA.x * vecC.z) > 0)
	{// �͈͓�
	 // �ő吔���̔���
		vecA = D3DXVECTOR3(vtxMaxObject.x, 0.0f, vtxMaxObject.z) - D3DXVECTOR3(vtxMinObject.x, 0.0f, vtxMaxObject.z);
		vecB = (pos + vtxMin) - D3DXVECTOR3(vtxMinObject.x, 0.0f, vtxMaxObject.z);
		vecC = (pos + vtxMax) - D3DXVECTOR3(vtxMinObject.x, 0.0f, vtxMaxObject.z);

		if ((vecA.z * vecB.x) - (vecA.x * vecB.z) > 0 || (vecA.z * vecC.x) - (vecA.x * vecC.z) > 0)
		{// �͈͓�
			bHitZ = true;
		}
		else { return false; }
	}
	else { return false; }

	if (bHitX && bHitY && bHitZ)
	{// �͈͓��ɓ����Ă���
		bHit = true;

		D3DXVECTOR3 middle = (vtxMinObject + vtxMaxObject) * 0.5f;		// ���S�̍��W
		D3DXVECTOR3 vec0(0.0f, 0.0f, 0.0f);

		if (pos.x < middle.x)
		{// �����X���W��������菬����
			if (pos.z < middle.z)
			{// �����Z���W��������菬����
				vec0 = D3DXVECTOR3(vtxMinObject.x, 0.0f, vtxMaxObject.z) - D3DXVECTOR3(vtxMaxObject.x, 0.0f, vtxMinObject.z);
			}
			else if (pos.z > middle.z)
			{// �����Z���W���������傫��
				vec0 = D3DXVECTOR3(vtxMaxObject.x, 0.0f, vtxMaxObject.z) - D3DXVECTOR3(vtxMinObject.x, 0.0f, vtxMinObject.z);
			}
		}
		else if (pos.x > middle.x)
		{// �����X���W���������傫��
			if (pos.z < middle.z)
			{// �����Z���W��������菬����
				vec0 = D3DXVECTOR3(vtxMinObject.x, 0.0f, vtxMinObject.z) - D3DXVECTOR3(vtxMaxObject.x, 0.0f, vtxMaxObject.z);
			}
			else if (pos.z > middle.z)
			{// �����Z���W���������傫��
				vec0 = D3DXVECTOR3(vtxMaxObject.x, 0.0f, vtxMinObject.z) - D3DXVECTOR3(vtxMinObject.x, 0.0f, vtxMaxObject.z);
			}
		}
	}

	return bHit;
}