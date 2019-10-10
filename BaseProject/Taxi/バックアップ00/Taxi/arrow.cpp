//=============================================================================
//
// ���̏��� [arrow.cpp]
// Author : shun yokomichi
//
//=============================================================================
#include "scene.h"
#include "model3D.h"
#include "manager.h"
#include "arrow.h"
#include "logo.h"
#include "scene3D.h"
#include "gamecamera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_NAME_1	"data\\MODEL\\���\\Arrow.x"			//�ǂݍ��ރe�N�X�`���t�@�C��
#define AREA_RADIUS		(m_fRadius + 6000.0f)					//���a����̑傫��
#define MAX_SCALE		(2.0f)									//�X�P�[���̍ő�l
#define MIN_SCALE		(1.0f)									//�X�P�[���̍ŏ��l
#define ADD_SCALE		(0.05f)									//�X�P�[���̉��Z�l
#define COL_CAHANGE		(0.01f)									//�F�̕ω���
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPD3DXMESH CArrow::m_pMeshModel = NULL;							//���b�V�����ւ̃|�C���^
LPD3DXBUFFER CArrow::m_pBuffMatModel = NULL;					//�}�e���A���̏��ւ̃|�C���^
DWORD CArrow::m_nNumMatModel = NULL;							//�}�e���A���̏��
LPDIRECT3DTEXTURE9      CArrow::m_pTextureMT = NULL;
D3DXVECTOR3 CArrow::m_LoadVtxMax = {};
D3DXVECTOR3 CArrow::m_LoadVtxMin = {};


//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CArrow::CArrow() : CModel3D(5, CScene::OBJTYPE_3DMODEL)
{
	m_pLogo = NULL;
	m_pCommand = NULL;
	m_scale = D3DXVECTOR3(0.1f, 0.1f, 0.1f);
	m_nSizeScale = MIN_SCALE;
	m_pScaleType = SCZLE_SMALL;

}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CArrow::~CArrow(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CArrow::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	LPDIRECT3DSURFACE9 pRenderOrg;
	LPDIRECT3DSURFACE9 pBuffOrg;

	//3D���f����Init
	CModel3D::Init();

	// �ʒu�̏�����
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 rot = D3DXVECTOR3(-0.1f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	m_colChange = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

	//�ʒu�̑��
	CModel3D::SetPosition(pos);
	CModel3D::SetRot(rot);

	// Texture�̍쐬
	pDevice->CreateTexture(SCREEN_WIDTH, SCREEN_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTextureMT, NULL);

	// �����_���[�̎擾
	m_pTextureMT->GetSurfaceLevel(0, &m_pRenderMT);

	// �o�b�t�@�̐���
	pDevice->CreateDepthStencilSurface(SCREEN_WIDTH, SCREEN_HEIGHT, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pBuffMT, NULL);

	pDevice->GetRenderTarget(0, &pRenderOrg);		// ���݂̃����_���[�^�[�Q�b�g��ۑ�
	pDevice->GetDepthStencilSurface(&pBuffOrg);		// ���݂̃o�b�t�@��ۑ�
	pDevice->SetRenderTarget(0, m_pRenderMT);		// �����_���[�̐ݒ�
	pDevice->SetDepthStencilSurface(m_pBuffMT);		// �o�b�t�@�̐ݒ�
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// ���ɖ߂�
	pDevice->SetRenderTarget(0, pRenderOrg);		// �����_���[
	pDevice->SetDepthStencilSurface(pBuffOrg);		// �o�b�t�@

	// �r���[�|�[�g�̏�����
	m_viewportMT.X = 0;						// X�̐ݒ�
	m_viewportMT.Y = 0;						// Y�̐ݒ�
	m_viewportMT.Width = SCREEN_WIDTH;		// ���̐ݒ�
	m_viewportMT.Height = SCREEN_HEIGHT;	// �����̐ݒ�
	m_viewportMT.MinZ = 0.0f;				// ��O
	m_viewportMT.MaxZ = 1.0f;				// ��

	m_pLogo = CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 270.0f, 0.0f), D3DXVECTOR2(400.0f, 400.0f), 1, 0, CLogo::TYPE_MULTIRENDERING);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CArrow::Uninit(void)
{
	// ���S�̔j��
	if (m_pLogo != NULL) { m_pLogo->Uninit(); m_pLogo = NULL; }
	if (m_pCommand != NULL) { m_pCommand->Uninit(); m_pCommand = NULL; }
	//3D���f����Uninit
	CModel3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CArrow::Update(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputJoypad *pCInputJoyPad = CManager::GetInputJoyPad();

	//3D���f����Update
	CModel3D::Update();

	//�ʒu�̏�����
	D3DXVECTOR3 pos = CModel3D::GetPosition();

	// �ړ��ʂ̏�����
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�X�P�[��
	D3DXVECTOR3 scale = CModel3D::GetScale();

	//�����̑��
	D3DXVECTOR3 &rot = CModel3D::GetRot();

	scale = AreaDetermine(pos, scale);
	rot = SetRot(pos, rot);

	// �e����̑��
	CModel3D::SetScale(scale);
	CModel3D::SetMove(move);
	CModel3D::SetRot(rot);
	CModel3D::Setcol(m_col);
}
//=============================================================================
// �`�揈��
//=============================================================================
void CArrow::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	LPDIRECT3DSURFACE9 pRenderOrg;
	LPDIRECT3DSURFACE9 pBuffOrg;
	D3DXMATRIX pmtxProjectionOrg, pmtxViewOrg;

	pDevice->GetRenderTarget(0, &pRenderOrg);						// ���݂̃����_���[�^�[�Q�b�g��ۑ�
	pDevice->GetDepthStencilSurface(&pBuffOrg);						// ���݂̃o�b�t�@��ۑ�
	pDevice->GetViewport(&m_viewportMT);

	pDevice->GetTransform(D3DTS_VIEW, &pmtxViewOrg);				// �r���[�}�g���b�N�X�̎擾
	pDevice->GetTransform(D3DTS_PROJECTION, &pmtxProjectionOrg);	// �v���W�F�N�V�����}�g���b�N�X�̎擾

	pDevice->SetRenderTarget(0, m_pRenderMT);		// �����_���[�̐ݒ�
	pDevice->SetDepthStencilSurface(m_pBuffMT);		// �o�b�t�@�̐ݒ�

	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// �r���[�|�[�g�̐ݒ�
	pDevice->SetViewport(&m_viewportMT);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&pmtxViewOrg);

	// �r���[�}�g���b�N�X���쐬
	D3DXMatrixLookAtLH(&pmtxViewOrg, &D3DXVECTOR3(0.0f, 90.0f, -100.0f), &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&pmtxProjectionOrg);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&pmtxProjectionOrg,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH /
		(float)SCREEN_HEIGHT,
		10.0f,
		200.0f);

	//���_�@���̎������K��
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	CModel3D::Draw();	// �`�揈��

	//���_�@���̎������K��
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);

	// ���ɖ߂�
	pDevice->SetRenderTarget(0, pRenderOrg);		// �����_���[
	pDevice->SetDepthStencilSurface(pBuffOrg);		// �o�b�t�@

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &pmtxViewOrg);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &pmtxProjectionOrg);

	if (m_pCommand != NULL) { m_pCommand->Draw(); }

}
//=============================================================================
// �G���A�̔���
//=============================================================================
D3DXVECTOR3 CArrow::AreaDetermine(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	// ���݂̋���
	float fArea = ((pos.x - m_goalpos.x) * (pos.x - m_goalpos.x)) + ((pos.z - m_goalpos.z) * (pos.z - m_goalpos.z));

	if (fArea <= AREA_RADIUS * AREA_RADIUS)
	{	// �G���A�͈͓̔��Ȃ�
		if (fArea <= m_fRadius * m_fRadius)
		{
			// ���S�̔p�� & �S�[���ʒu�̖��
			if (m_pLogo != NULL)
			{
				m_pLogo->Uninit();
				m_pLogo = NULL;
				if (m_pCommand == NULL) { m_pCommand = CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 100.0f, 0.0f), D3DXVECTOR2(200.0f, 30.0f), CTexture::TYPE_COMMAND_000, 0, CLogo::TYPE_LOGO);}
				//CModel3D::SetPosition(D3DXVECTOR3(m_goalpos.x, m_goalpos.y + 100.0f, m_goalpos.z));
				scale = D3DXVECTOR3(MIN_SCALE, MIN_SCALE, MIN_SCALE);
				m_pScaleType = SCZLE_NOMAL;
			}
		}
		else
		{
			m_colChange.r += COL_CAHANGE;
			m_colChange.g -= COL_CAHANGE;

			if (m_colChange.r >= 1.0f) { m_colChange.r = 1.0f; }
			if (m_colChange.g <= 0.0f) { m_colChange.g = 0.0f; }

			// ���S�̐���
			if (m_pCommand != NULL) { m_pCommand->Uninit(); m_pCommand = NULL; }
			if (m_pLogo == NULL)
			{
				m_pLogo = CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 270.0f, 0.0f), D3DXVECTOR2(400.0f, 400.0f), 1, 0, CLogo::TYPE_MULTIRENDERING);
				m_pScaleType = SCZLE_SMALL;
			}

			if (m_pScaleType == SCZLE_SMALL)
			{	// �X�P�[���T�C�Y���������ꍇ
				m_nSizeScale += ADD_SCALE;
				scale = D3DXVECTOR3(m_nSizeScale, m_nSizeScale, m_nSizeScale);
				if (m_nSizeScale >= MAX_SCALE) { m_pScaleType = SCZLE_BIG; }		// ��ނ���
			}
			else if (m_pScaleType == SCZLE_BIG)
			{	// �X�P�[���T�C�Y���傫���ꍇ
				m_nSizeScale -= ADD_SCALE;
				scale = D3DXVECTOR3(m_nSizeScale, m_nSizeScale, m_nSizeScale);
				if (m_nSizeScale <= MIN_SCALE) { m_pScaleType = SCZLE_SMALL; }		// ��ނ���
			}
			m_col = D3DXCOLOR(m_colChange.r, m_colChange.g, 0.0f, 1.0f);			// �F�̑��
		}
	}
	else { scale = D3DXVECTOR3(MIN_SCALE, MIN_SCALE, MIN_SCALE);  	m_col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);}

	return scale;
}

//=============================================================================
// �����ݒ�
//=============================================================================
D3DXVECTOR3 CArrow::SetRot(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	if (m_pScaleType != SCZLE_NOMAL)
	{	// ���S�̏ꍇ(�������S�[���Ɍ�����)
		rot.x = -0.1f;
		rot.y = atan2f(m_goalpos.x - pos.x, m_goalpos.z - pos.z) + D3DX_PI;
	}
	else { rot.x = D3DX_PI * -0.5f; }

	// ���ݕ�������
	if (rot.x > D3DX_PI) { rot.x -= D3DX_PI * 2.0f; }	// X
	if (rot.x < -D3DX_PI) { rot.x += D3DX_PI * 2.0f; }	// X
	if (rot.y > D3DX_PI) { rot.y -= D3DX_PI * 2.0f; }	// Y
	if (rot.y < -D3DX_PI) { rot.y += D3DX_PI * 2.0f; }	// Y
	if (rot.z > D3DX_PI) { rot.z -= D3DX_PI * 2.0f; }	// Z
	if (rot.z < -D3DX_PI) { rot.z += D3DX_PI * 2.0f; }	// Z

	return rot;
}

//===============================================================================
//�@�N���G�C�g
//===============================================================================
CArrow * CArrow::Create(int nArrowType, D3DXVECTOR3 pos, D3DXVECTOR3 GoalPos, float fRadius)
{
	CArrow *pArrow = NULL;

	// NULL�`�F�b�N
	if (pArrow == NULL)
	{// �������̓��I�m��

		pArrow = new CArrow;

		if (pArrow != NULL)
		{
			// ��ނ̐ݒ�
			pArrow->BindModel(m_pMeshModel, m_pBuffMatModel, m_nNumMatModel, NULL, m_LoadVtxMax, m_LoadVtxMin);

			// �I�u�W�F�N�g���Ƃ̐ݒ�p�^�C�v
			pArrow->m_nArrow = nArrowType;

			// �I�u�W�F�N�g�N���X�̐���
			pArrow->Init();

			// �ʒu����
			pArrow->SetPosition(pos);
			pArrow->m_goalpos = GoalPos;

			// ���a�̑��
			pArrow->m_fRadius = fRadius;
			pArrow->Setcol(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		}
	}

	return pArrow;
}//===============================================================================
// X�t�@�C���̓ǂݍ���
//===============================================================================
HRESULT CArrow::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL *pMat;

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(MODEL_NAME_1, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel, NULL, &m_nNumMatModel, &m_pMeshModel);
	//�}�e���A����񂩂�e�N�X�`���̎擾
	pMat = (D3DXMATERIAL*)m_pBuffMatModel->GetBufferPointer();

	int nNumVtx;		//���_��
	DWORD sizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

	//���f���̍ő�l�E�ŏ��l���擾����
	m_LoadVtxMax = D3DXVECTOR3(-10000, -10000, -10000);	//�ő�l
	m_LoadVtxMin = D3DXVECTOR3(10000, 10000, 10000);	//�ŏ��l

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
		if (vtx.x > m_LoadVtxMax.x)
		{
			m_LoadVtxMax.x = vtx.x;
		}
		if (vtx.y > m_LoadVtxMax.y)
		{
			m_LoadVtxMax.y = vtx.y;
		}
		if (vtx.z > m_LoadVtxMax.z)
		{
			m_LoadVtxMax.z = vtx.z;
		}
		//�ŏ��l
		if (vtx.x < m_LoadVtxMin.x)
		{
			m_LoadVtxMin.x = vtx.x;
		}
		if (vtx.y < m_LoadVtxMin.y)
		{
			m_LoadVtxMin.y = vtx.y;
		}
		if (vtx.z < m_LoadVtxMin.z)
		{
			m_LoadVtxMin.z = vtx.z;
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
void CArrow::UnLoad(void)
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
}