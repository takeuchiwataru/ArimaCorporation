//=============================================================================
//
// ���f����p�̏��� [model.cpp]
// Author : �L�n���u
//
//=============================================================================
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "toonshader.h"
#include "gamecamera.h"
#include "model3d.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
D3DMATERIAL9	   *CModel::m_pMeshMaterials[PARTS_MAX] = {};					// ���b�V���}�e���A���̏��
LPDIRECT3DTEXTURE9 *CModel::m_pShaderMeshTextures[PARTS_MAX] = {};				// �V�F�[�_�[�p�̃��b�V���e�N�X�`��
LPDIRECT3DTEXTURE9 *CModel::m_pMeshTextures[MAX_MODEL_TEXTURE] = {};			// �V�F�[�_�[�p�̃��b�V���e�N�X�`��
LPD3DXMESH			CModel::m_pMesh[PARTS_MAX] = {};							//���b�V�����ւ̃|�C���^
LPD3DXBUFFER		CModel::m_pBuffMat[PARTS_MAX] = {};							//�}�e���A���̏��ւ̃|�C���^
DWORD				CModel::m_nNumMat[PARTS_MAX] = {};							//�}�e���A���̏��
D3DXVECTOR3			CModel::m_VtxMax[PARTS_MAX];								//�ő�l
D3DXVECTOR3			CModel::m_VtxMin[PARTS_MAX];								//�ŏ��l
CModel::PARTS_TYPE	*CModel::m_partstype[TYPE_MAX] = {};
int					CModel::m_nModelMax[TYPE_MAX];								// ���f���̎��
LPDIRECT3DTEXTURE9	CModel::m_pTexAll[TEX_MAX] = {};

//=============================================================================
// ��������
//=============================================================================
CModel * CModel::Create(const D3DXVECTOR3 pos, char FileName[40], D3DXVECTOR3 Scale)
{
	//�C���X�^���X�̐���
	CModel * pModel;
	pModel = new CModel;

	//����������
	pModel->Init();

	//�ݒ菈��
	pModel->SetPos(pos);
	pModel->SetFirstPos(pos);
	//pModel->SetScale(Scale);

	return pModel;
}

//===============================================================================
//�@�R���X�g���N�^
//===============================================================================
void CModel::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	int nNumVtx;				//���_��
	DWORD sizeFVF;				//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;				//���_�o�b�t�@�ւ̃|�C���^

	//===================================================
	//    �@�@�@�@�@�}�e���A���ƃe�N�X�`���̏��
	//===================================================
	for (int nCount = 0; nCount < PARTS_MAX; nCount++)
	{
		m_VtxMax[nCount] = D3DXVECTOR3(-10000, -10000, -10000);			//�ő�l
		m_VtxMin[nCount] = D3DXVECTOR3(10000, 10000, 10000);			//�ŏ��l

		//���_�����擾
		nNumVtx = m_pMesh[nCount]->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(m_pMesh[nCount]->GetFVF());

		//���_�o�b�t�@�̃��b�N
		m_pMesh[nCount]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		//���_���W�̑��

			//�ő�l
			if (vtx.x > m_VtxMax[nCount].x)
			{
				m_VtxMax[nCount].x = vtx.x;
			}
			if (vtx.y > m_VtxMax[nCount].y)
			{
				m_VtxMax[nCount].y = vtx.y;
			}
			if (vtx.z > m_VtxMax[nCount].z)
			{
				m_VtxMax[nCount].z = vtx.z;
			}
			//�ŏ��l
			if (vtx.x < m_VtxMin[nCount].x)
			{
				m_VtxMin[nCount].x = vtx.x;
			}
			if (vtx.y < m_VtxMin[nCount].y)
			{
				m_VtxMin[nCount].y = vtx.y;
			}
			if (vtx.z < m_VtxMin[nCount].z)
			{
				m_VtxMin[nCount].z = vtx.z;
			}

			//�T�C�Y���̃|�C���^��i�߂�
			pVtxBuff += sizeFVF;
		}

		//���_�o�b�t�@�̃A�����b�N
		m_pMesh[nCount]->UnlockVertexBuffer();

		D3DXMATERIAL *d3dxMaterials = (D3DXMATERIAL*)m_pBuffMat[nCount]->GetBufferPointer();
		m_pMeshMaterials[nCount] = new D3DMATERIAL9[(int)m_nNumMat[nCount]];
		m_pShaderMeshTextures[nCount] = new LPDIRECT3DTEXTURE9[(int)m_nNumMat[nCount]];

		for (DWORD MatCount = 0; MatCount < (int)m_nNumMat[nCount]; MatCount++)
		{
			m_pMeshMaterials[nCount][MatCount] = d3dxMaterials[MatCount].MatD3D;
			m_pMeshMaterials[nCount][MatCount].Ambient = m_pMeshMaterials[nCount][MatCount].Diffuse;
			m_pShaderMeshTextures[nCount][MatCount] = NULL;

			if (d3dxMaterials[MatCount].pTextureFilename != NULL &&
				lstrlen(d3dxMaterials[MatCount].pTextureFilename) > 0)
			{
				D3DXCreateTextureFromFile(pDevice,
					d3dxMaterials[MatCount].pTextureFilename,
					&m_pShaderMeshTextures[nCount][MatCount]);
			}
		}
	}

	//�g���Ă���e�N�X�`��
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\costumetex\\happyboy.jpg", &m_pTexAll[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\costumetex\\hat.jpg", &m_pTexAll[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\costumetex\\featherbone.jpg", &m_pTexAll[2]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\costumetex\\pirate.jpg", &m_pTexAll[3]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\costumetex\\kabuto.jpg", &m_pTexAll[4]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\costumetex\\helmet.jpg", &m_pTexAll[5]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\costumetex\\beret.jpg", &m_pTexAll[6]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\costumetex\\clown.jpg", &m_pTexAll[7]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\modeltex\\�j���g��00.jpg", &m_pTexAll[8]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\modeltex\\chick01.jpg", &m_pTexAll[9]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\modeltex\\chick02.jpg", &m_pTexAll[10]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\modeltex\\chick00.jpg", &m_pTexAll[11]);

}

//===============================================================================
//�@�R���X�g���N�^
//===============================================================================
CModel::CModel()
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
	m_FirstPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�����ʒu
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	//�g��k����
	m_pTextures = NULL;
	m_bTexMat = false;
	m_pParent = NULL;
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CModel::~CModel() { }

//=============================================================================
// ����������
//=============================================================================
HRESULT CModel::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//�ʒu
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//�����B����
	m_Rot.y = 0.0f;
	m_pTextures = NULL;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CModel::Uninit(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void CModel::Update(void) {}

//=============================================================================
// �`�揈��
//=============================================================================
void CModel::Draw(float fAlpha)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxParent, mtxScale;	//�v�Z
	D3DMATERIAL9 matDef;								//���݂̃}�e���A����ۑ�
	D3DXMATERIAL *pMat;									//�}�e���A���f�[�^�ւ̃|�C���^
	LPD3DXEFFECT Shader = NULL;							//�V�F�[�_�[
	CCamera *pCamera = NULL;							//�J�����̃|�C���g
	LPDIRECT3DTEXTURE9 ShaderTex = NULL;
	LPDIRECT3DTEXTURE9 LineTex = NULL;

	CModel3D *pModel3D = NULL;

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
	CToonShader *&pToonShader = CModel3D::GetToonShader();

	if (pToonShader != NULL)
	{
		Shader = pToonShader->GetShader();
	}

	//�V�F�[�_�[�̒��g������ꍇ
	if (Shader != NULL)
	{
		if (pToonShader != NULL)
		{
			ShaderTex = pToonShader->GetTexture();
			LineTex = pToonShader->GetLineTexture();
		}

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_mtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		if (m_pParent != NULL)
		{//�e�̏ꍇ
			mtxParent = m_pParent->GetMtxWorld();
		}
		else
		{//�e�ł͂Ȃ��ꍇ
			pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
		}

		//�e�̃}�g���b�N�X�Ɗ|�����킹��
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

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
		Shader->SetMatrix("matWorld", &m_mtxWorld);

		//�e�N�X�`���̊��蓖��
		Shader->SetTexture("ShadeTexture", ShaderTex);
		Shader->SetTexture("LineTexture", LineTex);

		Shader->SetVector("EyePos", (D3DXVECTOR4*)&ViewPosV);

		//�V�F�[�_�J�n
		Shader->Begin(NULL, 0);

		// ���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		//for (int nCount = 0; nCount < PARTS_MAX; nCount++)
		//{
			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)m_pBuffMat[m_Type]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)m_nNumMat[m_Type]; nCntMat++)
			{
				//�V�F�[�_�̃f�[�^�Ƀ��f���̃e�N�X�`��������
				m_pShaderMeshTextures[m_Type][nCntMat] = m_pTextures;

				//�}�e���A���̐ݒ�
				pMat[nCntMat].MatD3D.Diffuse.a = fAlpha;
				m_pMeshMaterials[m_Type][nCntMat].Diffuse.a = fAlpha;

				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// �e�N�X�`����NULL�ɂ���
				pDevice->SetTexture(0, m_pShaderMeshTextures[m_Type][nCntMat]);

				//===================================================
				//    �@�@�@�@�V�F�[�_�[�̊��蓖�č��
				//===================================================
				//�p�X���w�肵�ĊJ�n
				Shader->BeginPass(0);

				//�V�F�[�_�[�e�N�X�`���̗L���ɉ����ăV�F�[�_�[������
				if (m_pShaderMeshTextures[m_Type][nCntMat] != NULL)
				{
					//�e�N�X�`��������ꍇ
					m_bTexMat = true;
					//�V�F�[�_�[��bool�̏���n��
					Shader->SetBool("TexMat", m_bTexMat);
				}
				else if (m_pShaderMeshTextures[m_Type][nCntMat] == NULL)
				{
					//�e�N�X�`�����Ȃ��ꍇ
					m_bTexMat = false;
					//�V�F�[�_�[��bool�̏���n��
					Shader->SetBool("TexMat", m_bTexMat);
				}

				//�I�u�W�F�N�g�̃e�N�X�`��
				Shader->SetTexture("DecalTexture", m_pShaderMeshTextures[m_Type][nCntMat]);
				//�v���p�e�B�[��,NameID,�ݒ肷��z��̒l
				Shader->SetFloatArray("Diffuse", (FLOAT*)&m_pMeshMaterials[m_Type][nCntMat].Diffuse, 4);

				//�ύX����ɂȂ�f�B���N�g�� �X�g�A�ɕۑ�
				Shader->CommitChanges();

				//�I�u�W�F�N�g(�p�[�c)�̕`��
				m_pMesh[m_Type]->DrawSubset(nCntMat);

				//�p�X�̏I��
				Shader->EndPass();
			}

		//}
		// �}�e���A�����f�t�H���g�ɖ߂�
		pDevice->SetMaterial(&matDef);

		//�V�F�[�_�̏I��
		Shader->End();
	}
}
//=============================================================================
//�@X�t�@�C���̓ǂݍ���
//=============================================================================
void CModel::SetParts(void)
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{
		if (m_partstype[nCount] != NULL) { return; }

		switch (nCount)
		{
		case TYPE_CHICKEN:
			m_nModelMax[nCount] = 12;
			m_partstype[nCount] = new PARTS_TYPE[m_nModelMax[nCount]];

			m_partstype[nCount][0] = PARTS_CHICKEN_BODY;
			m_partstype[nCount][1] = PARTS_CHICKEN_HEAD;
			m_partstype[nCount][2] = PARTS_CHICKEN_ASS;
			m_partstype[nCount][3] = PARTS_CHICKEN_WINGSR_0;
			m_partstype[nCount][4] = PARTS_CHICKEN_WINGSR_1;
			m_partstype[nCount][5] = PARTS_CHICKEN_WINGSL_0;
			m_partstype[nCount][6] = PARTS_CHICKEN_WINGSL_1;
			m_partstype[nCount][7] = PARTS_CHICKEN_LEGR;
			m_partstype[nCount][8] = PARTS_CHICKEN_FOOTR;
			m_partstype[nCount][9] = PARTS_CHICKEN_LEGL;
			m_partstype[nCount][10] = PARTS_CHICKEN_FOOTL;
			m_partstype[nCount][11] = PARTS_MAX;

			break;
		case TYPE_CHICK:

			m_nModelMax[nCount] = 6;
			m_partstype[nCount] = new PARTS_TYPE[m_nModelMax[nCount]];

			m_partstype[nCount][0] = PARTS_CHICK_BODY;
			m_partstype[nCount][1] = PARTS_CHICK_HEAD;
			m_partstype[nCount][2] = PARTS_CHICK_LEGR;
			m_partstype[nCount][3] = PARTS_CHICK_FOOTR;
			m_partstype[nCount][4] = PARTS_CHICK_LEGL;
			m_partstype[nCount][5] = PARTS_CHICK_FOOTL;
			break;
		case TYPE_WOOD:
			m_nModelMax[nCount] = 9;
			m_partstype[nCount] = new PARTS_TYPE[m_nModelMax[nCount]];

			m_partstype[nCount][0] = PARTS_WOOD_LEG;
			m_partstype[nCount][1] = PARTS_WOOD_BODY_R;
			m_partstype[nCount][2] = PARTS_WOOD_LONG_R;
			m_partstype[nCount][3] = PARTS_WOOD_SHORT_R;
			m_partstype[nCount][4] = PARTS_WOOD_LEAF_R;
			m_partstype[nCount][5] = PARTS_WOOD_BODY_L;
			m_partstype[nCount][6] = PARTS_WOOD_LONG_L;
			m_partstype[nCount][7] = PARTS_WOOD_SHORT_L;
			m_partstype[nCount][8] = PARTS_WOOD_LEAF_L;

			break;
		}
	}
}
//=============================================================================
//�@X�t�@�C���̓ǂݍ���
//=============================================================================
void CModel::ParentModel(CModel **&apModel, TYPE type, int nSelectParts)
{
	if (apModel != NULL) { return; }

	apModel = new CModel*[m_nModelMax[type]];
	for (int nCount = 0; nCount < m_nModelMax[type]; nCount++)
	{// �����{�̃|�C���^��������
		apModel[nCount] = NULL;

		//NULL�`�F�b�N
		if (apModel[nCount] == NULL)
		{//���I�m��
			apModel[nCount] = new CModel;
			//NULL�`�F�b�N
			if (apModel[nCount] != NULL)
			{
				//���f���̐���
				apModel[nCount]->m_Type = m_partstype[type][nCount];

				if (apModel[nCount]->m_Type == PARTS_MAX)
				{//�L�������Ƃɐݒ�
					apModel[nCount]->m_Type = (CModel::PARTS_TYPE)(PARTS_CHICKEN_11 + nSelectParts);
				}

				apModel[nCount]->Init();
			}
		}
	}
}
//=============================================================================
//�@X�t�@�C���̓ǂݍ���
//=============================================================================
void CModel::PartsTypeUnLoad(void)
{
	for (int nCount = 0; nCount < TEX_MAX; nCount++)
	{
		if (m_pTexAll[nCount] != NULL)
		{
			m_pTexAll[nCount]->Release();
			m_pTexAll[nCount] = NULL;
		}
	}

	for (int nCount = 0; nCount < PARTS_MAX; nCount++)
	{
		// ���b�V���̊J��
		if (m_pMesh[nCount] != NULL)
		{
			m_pMesh[nCount]->Release();
			m_pMesh[nCount] = NULL;
		}

		// �}�e���A���̊J��
		if (m_pBuffMat[nCount] != NULL)
		{
			m_pBuffMat[nCount]->Release();
			m_pBuffMat[nCount] = NULL;
		}
	}

	for (int nCount = 0; nCount < PARTS_MAX; nCount++)
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

	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{
		delete[] m_partstype[nCount];
		m_partstype[nCount] = NULL;
	}
}