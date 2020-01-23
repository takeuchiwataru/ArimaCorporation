//=============================================================================
//
// mesh���� [mesh.cpp]
// Author : Ryo Sugimoto
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "Orbit.h"
#include "renderer.h"
#include "manager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BULOCK_X					(20.0f)											//�u���b�N�̉��s
#define BULOCK_Y					(400.0f)											//�u���b�N�̉��s
#define BULOCK_Z					(20.0f)											//�u���b�N�̉��s
#define VERTEX						((VERTICAL + 1) * (CORSS + 1))						//�w�肵���c�Ɖ�+�P�̒l
#define IDX							((VERTICAL * CORSS) * 2 + (VERTICAL - 1) * 4 + 2)	//�C���f�b�N�X�̎�
#define POLYGON						((VERTICAL * CORSS) * 2 + (VERTICAL - 1) * 4)		//�|���S�����̎�
#define WALL_CURVE					(3.0f)		//�ǂ̋Ȃ���
#define FIELD_SIZE					(100.0f)		//��}�X�̑傫��
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//==================================================================================================//
//    * �����֐� *
//==================================================================================================//
CEfcOrbit	*CEfcOrbit::Create(void)
{
	CEfcOrbit *pOrbit = NULL;	pOrbit = new CEfcOrbit;
	if (pOrbit != NULL) { pOrbit->Init(); }
	return pOrbit;
}
//=============================================================================
// ���b�V���I�[�r�b�g�ݒ菈��
//=============================================================================
CEfcOrbit	*CEfcOrbit::Set(D3DXMATRIX &mtxWorld, D3DXVECTOR3 length0, D3DXVECTOR3 length1, bool &bDelete, D3DXCOLOR col, int nUV, int nHeight, int nReset)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾

																		//���
	m_pmtxWorld = &mtxWorld;	m_bDelete = &bDelete;
	m_col = col;
	//m_pTex = Load(tex);
	m_nUV = nUV;				m_nHeight = nHeight;
	m_fEnd = 0.0f;

	m_nCross = 10;										//���̒���
	m_nNumPolygon = (1 * m_nCross) * 2 + (1 - 1) * 4;	//�ʂ̐�
	m_fPlusCol = 1.0f / m_nCross;
	m_fUV = 0.0f;
	m_nReset = nReset;
	m_nInitReset = nReset;
	m_DrawType = C2D::DRAW_TYPE_ADD;

	for (int nCount = 0; nCount < 2; nCount++)
	{//�}�g���b�N�X�v�Z
		if (nCount == 1) { length0 = length1; }
		// ���g�̈ʒu���v�Z
		D3DXMatrixTranslation(&m_mtxWorld[nCount], length0.x, length0.y, length0.z);
	}

	//���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * ((1 + 1) * (m_nCross + 1)), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	//���b�V���ݒ�
	//CMesh::Set(CMesh::TYPE_ORBIT, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 100, true);
	SetObjType(OBJTYPE_ORBIT);
	ReSet();

	return this;
}
//=============================================================================
//����������
//=============================================================================
HRESULT	CEfcOrbit::Init(void)
{
	m_Type = TYPE_FADE;
	m_pTex = NULL;
	m_bUpdate = false;
	return S_OK;
}
//=============================================================================
//�I������
//=============================================================================
void	CEfcOrbit::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	CScene::Release();
}
//=============================================================================
//�X�V����
//=============================================================================
void	CEfcOrbit::Update(void)
{
	if (m_nReset > 1) { m_nReset--; }
	else
	{
		SwapVtx();		//���_�̓���ւ�
		m_bUpdate = true;

		//�ŐV�̒��_����
		if (m_pmtxWorld != NULL) { SetVtx(false); }
		if (m_bDelete == NULL)
		{
			m_fEnd += m_fPlusCol;
			if (m_Type == TYPE_FADE)
			{
				m_fPlusCol *= 0.95f;
				if (m_fPlusCol < 0.0001f) { Uninit(); return; }
			}
			else
			{
				if (m_fEnd >= 1.0f) { Uninit(); return; }
			}
		}
	}
}
//=============================================================================
//�`�揈��
//=============================================================================
void	CEfcOrbit::Draw(void)
{
	if (!m_bUpdate) { return; }

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxView;	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX	mtxWorld;					//���[���h�}�g���b�N�X

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);    // ���ʂ��J�����O

	if (C2D::DrawPrepare(GetDrawType(), pDevice)) { return; }

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTex);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, m_nNumPolygon);
	pDevice->SetTexture(0, NULL);

	C2D::DrawPrepare(C2D::DRAW_TYPE_NORMAL, pDevice);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);    // ���ʂ��J�����O
}
//=============================================================================
//�ŐV�̒��_���v�Z���đ��
//=============================================================================
void	CEfcOrbit::SetVtx(bool bAll)
{
	if (m_pmtxWorld == NULL) { return; }
	if (m_nReset == 1) { m_nReset--; SetVtx(true); }
	else if (m_nReset > 0) { m_nReset--; return; }
	D3DXMATRIX mtxWK;

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	//���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾

	for (int nCount = 0; nCount < 2; nCount++)
	{
		// �v�Z�p�}�g���b�N�X�ɑ��
		mtxWK = m_mtxWorld[nCount];

		// �e�̊p�x��p���Ĉړ��𔽉f
		D3DXMatrixMultiply(&mtxWK, &mtxWK, m_pmtxWorld);

		pVtx[nCount].pos = D3DXVECTOR3(mtxWK._41, mtxWK._42, mtxWK._43);
		pVtx[nCount].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, (bAll ? 0.0f : m_col.a));
		pVtx[nCount].tex = D3DXVECTOR2(m_fUV * 0.1f, (float)m_nUV / (float)m_nHeight + nCount * (1.0f / (float)m_nHeight));
		pVtx[nCount].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		m_fUV += 0.1f;
	}

	if (bAll)
	{//�S�Ă̒��_�ɑ��
		for (int nCount = 1; nCount < m_nCross + 1; nCount++)
		{//���̕���������]
			pVtx[(nCount) * 2] = pVtx[0];
			pVtx[(nCount) * 2 + 1] = pVtx[1];
		}
	}

	m_pVtxBuff->Unlock();	//���_�o�b�t�@�̃A�����b�N����
}
//=============================================================================
//���_�̓���ւ�
//=============================================================================
void	CEfcOrbit::SwapVtx(void)
{
	D3DXCOLOR				col;

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	//���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾

	for (int nCount = m_nCross - 1; nCount >= 0; nCount--)
	{//���̕���������]

		switch (m_Type)
		{
		case TYPE_NONE://�����x����
			col = pVtx[(nCount * 2)].col;		pVtx[(nCount * 2)].col = D3DXCOLOR(col.r, col.g, col.b, col.a - m_fPlusCol);
			col = pVtx[(nCount * 2) + 1].col;	pVtx[(nCount * 2) + 1].col = D3DXCOLOR(col.r, col.g, col.b, col.a - m_fPlusCol);
			break;
		case TYPE_FADE:
			if (nCount <= 3)
			{
				col = pVtx[(nCount * 2)].col;		pVtx[(nCount * 2)].col = D3DXCOLOR(col.r, col.g, col.b, col.a + m_fPlusCol * 1.5f);
				col = pVtx[(nCount * 2) + 1].col;	pVtx[(nCount * 2) + 1].col = D3DXCOLOR(col.r, col.g, col.b, col.a + m_fPlusCol * 1.5f);
			}
			else
			{
				col = pVtx[(nCount * 2)].col;		pVtx[(nCount * 2)].col = D3DXCOLOR(col.r, col.g, col.b, col.a - m_fPlusCol);
				col = pVtx[(nCount * 2) + 1].col;	pVtx[(nCount * 2) + 1].col = D3DXCOLOR(col.r, col.g, col.b, col.a - m_fPlusCol);
			}
			break;
		}


		//����ւ�
		pVtx[(nCount * 2) + 2] = pVtx[(nCount * 2)];
		pVtx[(nCount * 2) + 3] = pVtx[(nCount * 2) + 1];
	}

	m_pVtxBuff->Unlock();	//���_�o�b�t�@�̃A�����b�N����
}

//=============================================================================
//���_�̃��Z�b�g
//=============================================================================
void	CEfcOrbit::ReSetAll(void)
{
	CScene *pScene;

	pScene = CScene::GetTop(ORBIT_PRIORITY);
	while (pScene != NULL)
	{//�I���܂�
		if (pScene->GetObjType() == OBJTYPE_ORBIT)
		{//���b�V������
			pScene->Uninit();
			//if (((CEfcOrbit*)pScene)->m_nInitReset == 0) { pScene->Uninit(); }
			//else { ((CEfcOrbit*)pScene)->ReSet(); }
		}
		pScene = pScene->GetNext();
	}
}
//=============================================================================
//�폜�m�F�Ɣj��
//=============================================================================
void	CEfcOrbit::Delete(bool bEnd)
{
	CScene *pScene;

	pScene = CScene::GetTop(ORBIT_PRIORITY);
	while (pScene != NULL)
	{//�I���܂�
		if (!pScene->GetDeath())
		{//�폜���Ă��Ȃ��Ȃ�
			if (pScene->GetObjType() == OBJTYPE_ORBIT)
			{//���b�V������
				if (((CEfcOrbit*)pScene)->m_bDelete != NULL)
				{//�I���Ȃ�j��
					if (((CEfcOrbit*)pScene)->m_bDelete[0])
					{
						if (bEnd) { ((CEfcOrbit*)pScene)->m_pmtxWorld = NULL; }
						((CEfcOrbit*)pScene)->m_bDelete = NULL;
					}
				}
			}
		}
		pScene = pScene->GetNext();
	}
}
//=============================================================================
//�폜�m�F�Ɣj��
//=============================================================================
void	CEfcOrbit::DeleteAll(void)
{
	CScene *pScene;

	pScene = CScene::GetTop(ORBIT_PRIORITY);
	while (pScene != NULL)
	{//�I���܂�
		if (!pScene->GetDeath())
		{//�폜���Ă��Ȃ��Ȃ�
			if (pScene->GetObjType() == OBJTYPE_ORBIT)
			{//���b�V������
				((CEfcOrbit*)pScene)->m_pmtxWorld = NULL;
				((CEfcOrbit*)pScene)->m_bDelete = NULL;
			}
		}
		pScene = pScene->GetNext();
	}
}
//=============================================================================
//���_�̃��Z�b�g
//=============================================================================
void	CEfcOrbit::ReSet(void)
{
	m_fUV = 0.0f;
	m_nReset = m_nInitReset;
	SetVtx(true);
}
