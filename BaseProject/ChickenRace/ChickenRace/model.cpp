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

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

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
	pModel->SetScale(Scale);

	return pModel;
}

//===============================================================================
//�@�R���X�g���N�^
//===============================================================================
CModel::CModel()
{
	m_pMesh = NULL;								//���b�V�����ւ̃|�C���^
	m_pBuffMat = NULL;							//�}�e���A���̏��ւ̃|�C���^
	m_nNumMat = 0;								//�}�e���A���̏��
	m_VtxMin, m_VtxMax;							//���f���̍ŏ��l�E�ő�l
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
	m_FirstPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�����ʒu
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	//�g��k����
	m_pTextures = NULL;
	m_bTexMat = false;
	m_pMeshMaterials = NULL;
	m_pShaderMeshTextures = NULL;
	m_pToonShader = NULL;

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

	int nNumVtx;				//���_��
	DWORD sizeFVF;				//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;				//���_�o�b�t�@�ւ̃|�C���^

	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//�ʒu
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//�����B����
	m_VtxMax = D3DXVECTOR3(-10000, -10000, -10000);			//�ő�l
	m_VtxMin = D3DXVECTOR3(10000, 10000, 10000);			//�ŏ��l

	//���_�����擾
	nNumVtx = m_pMesh->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	//���_�o�b�t�@�̃��b�N
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		//���_���W�̑��

														//�ő�l
		if (vtx.x > m_VtxMax.x)
		{
			m_VtxMax.x = vtx.x;
		}
		if (vtx.y > m_VtxMax.y)
		{
			m_VtxMax.y = vtx.y;
		}
		if (vtx.z > m_VtxMax.z)
		{
			m_VtxMax.z = vtx.z;
		}
		//�ŏ��l
		if (vtx.x < m_VtxMin.x)
		{
			m_VtxMin.x = vtx.x;
		}
		if (vtx.y < m_VtxMin.y)
		{
			m_VtxMin.y = vtx.y;
		}
		if (vtx.z < m_VtxMin.z)
		{
			m_VtxMin.z = vtx.z;
		}

		//�T�C�Y���̃|�C���^��i�߂�
		pVtxBuff += sizeFVF;
	}

	//���_�o�b�t�@�̃A�����b�N
	m_pMesh->UnlockVertexBuffer();

	m_Rot.y = 0.0f;

	m_pToonShader = NULL;			// �V�F�[�_�[�̃|�C���^�̏�����
	m_pMeshMaterials = NULL;
	m_pShaderMeshTextures = NULL;

	//===================================================
	//    �@�@�@�@�@�}�e���A���ƃe�N�X�`���̏��
	//===================================================
	D3DXMATERIAL *d3dxMaterials = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
	m_pMeshMaterials = new D3DMATERIAL9[(int)m_nNumMat];
	m_pShaderMeshTextures = new LPDIRECT3DTEXTURE9[(int)m_nNumMat];

	for (DWORD MatCount = 0; MatCount < (int)m_nNumMat; MatCount++)
	{
		m_pMeshMaterials[MatCount] = d3dxMaterials[MatCount].MatD3D;
		m_pMeshMaterials[MatCount].Ambient = m_pMeshMaterials[MatCount].Diffuse;
		m_pShaderMeshTextures[MatCount] = NULL;

		if (d3dxMaterials[MatCount].pTextureFilename != NULL &&
			lstrlen(d3dxMaterials[MatCount].pTextureFilename) > 0)
		{
			D3DXCreateTextureFromFile(pDevice,
				d3dxMaterials[MatCount].pTextureFilename,
				&m_pShaderMeshTextures[MatCount]);
		}
	}

	//�V�F�[�_�[�̓ǂݍ��ރt�@�C��
	if (m_pToonShader == NULL)
	{
		m_pToonShader = new CToonShader;
		m_pToonShader->Init("data\\SHADER\\ToonShader.fx");
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CModel::Uninit(void)
{
	//�V�F�[�_�[�c�[��������
	if (m_pMeshMaterials != NULL)
	{
		delete[] m_pMeshMaterials;
		m_pMeshMaterials = NULL;
	}
	if (m_pShaderMeshTextures != NULL)
	{
		delete[] m_pShaderMeshTextures;
		m_pShaderMeshTextures = NULL;
	}

	//�V�F�[�_�[�̃|�C���^�̔j��
	if (m_pToonShader != NULL)
	{
		m_pToonShader->Uninit();
		delete m_pToonShader;
		m_pToonShader = NULL;
	}

	// ���b�V���̊J��
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	// �}�e���A���̊J��
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	//�e�N�X�`���̔j��
	if (m_pTextures != NULL)
	{
		//m_pTextures->Release();
		m_pTextures = NULL;
	}
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
		if (m_pToonShader != NULL)
		{
			ShaderTex = m_pToonShader->GetTexture();
			LineTex = m_pToonShader->GetLineTexture();
		}

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_mtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		//�g��k���s��̍쐬
		D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

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

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
		{
			//�V�F�[�_�̃f�[�^�Ƀ��f���̃e�N�X�`��������
			m_pShaderMeshTextures[nCntMat] = m_pTextures;

			//�}�e���A���̐ݒ�
			pMat[nCntMat].MatD3D.Diffuse.a = fAlpha;
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// �e�N�X�`����NULL�ɂ���
			pDevice->SetTexture(0, m_pShaderMeshTextures[nCntMat]);

			//===================================================
			//    �@�@�@�@�V�F�[�_�[�̊��蓖�č��
			//===================================================
			//�p�X���w�肵�ĊJ�n
			Shader->BeginPass(0);

			//�V�F�[�_�[�e�N�X�`���̗L���ɉ����ăV�F�[�_�[������
			if (m_pShaderMeshTextures[nCntMat] != NULL)
			{
				//�e�N�X�`��������ꍇ
				m_bTexMat = true;
				//�V�F�[�_�[��bool�̏���n��
				Shader->SetBool("TexMat", m_bTexMat);
			}
			else if (m_pShaderMeshTextures[nCntMat] == NULL)
			{
				//�e�N�X�`�����Ȃ��ꍇ
				m_bTexMat = false;
				//�V�F�[�_�[��bool�̏���n��
				Shader->SetBool("TexMat", m_bTexMat);
			}

			//�I�u�W�F�N�g�̃e�N�X�`��
			Shader->SetTexture("DecalTexture", m_pShaderMeshTextures[nCntMat]);
			//�v���p�e�B�[��,NameID,�ݒ肷��z��̒l
			Shader->SetFloatArray("Diffuse", (FLOAT*)&m_pMeshMaterials[nCntMat].Diffuse, 4);

			//�ύX����ɂȂ�f�B���N�g�� �X�g�A�ɕۑ�
			Shader->CommitChanges();

			//�I�u�W�F�N�g(�p�[�c)�̕`��
			m_pMesh->DrawSubset(nCntMat);

			//�p�X�̏I��
			Shader->EndPass();
		}

		// �}�e���A�����f�t�H���g�ɖ߂�
		pDevice->SetMaterial(&matDef);

		//�V�F�[�_�̏I��
		Shader->End();
	}
}

//=============================================================================
//�@�}�e���A���J���[�̐ݒ�
//=============================================================================
void CModel::SetColor(D3DXCOLOR col)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DMATERIAL9 matDef;	// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;		// �}�e���A���f�[�^�ւ̃|�C���^

							// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// �F��t����
	pMat->MatD3D.Diffuse = col;

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
//�@X�t�@�C���̓ǂݍ���
//=============================================================================
void CModel::CreateXFile(char FileName[40])
{
	//�����_�����O�N���X���擾
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(&FileName[0],
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);
}