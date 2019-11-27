//=============================================================================
//
// ���b�V���V�����_�[���� [meshCylinder.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "meshCylinder.h"
#include "manager.h"
#include "meshfield.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//===============================================================================
// ��������
//===============================================================================
CMeshCylinder * CMeshCylinder::Create(D3DXVECTOR3 pos, int nLap, int nStage, float fRadius, float fHeight, D3DXCOLOR col)
{
	CMeshCylinder *pMeshCylinder = NULL;

	pMeshCylinder = new CMeshCylinder;	//�������̓��I�m��

	if (pMeshCylinder != NULL)
	{// �������m�ې���
		pMeshCylinder->m_pos = pos;									// �ʒu�̐ݒ�
		pMeshCylinder->m_col = col;									// �F�̐ݒ�
		pMeshCylinder->MakeVertex(nLap, nStage, fRadius, fHeight);	// ���_�̍쐬
		pMeshCylinder->Init();										// ����������
	}

	return pMeshCylinder;
}

//===============================================================================
// �f�t�H���g�R���X�g���N�^
//===============================================================================
CMeshCylinder::CMeshCylinder():CScene(4, CScene::OBJTYPE_CYLINDER)
{
	m_nNumVtx = 0;
	m_nNumPolygon = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nLap = 0;
	m_nStage = 0;
	m_fRadius = 0.0f;
	m_fHeight = 0.0f;
	m_bAlong = false;
	m_fFloatHeight = 0.0f;
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//===============================================================================
// �f�X�g���N�^
//===============================================================================
CMeshCylinder::~CMeshCylinder(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CMeshCylinder::Init(void)
{
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CMeshCylinder::Uninit(void)
{
	if (NULL != m_pVtxBuff)
	{// ���_�o�b�t�@�̔j��
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	if (NULL != m_pIdxBuff)
	{// �C���f�b�N�X�o�b�t�@�̔j��
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// ������j��
	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CMeshCylinder::Update(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void CMeshCylinder::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���[���h�}�g���b�N�X�̕ۑ��p
	D3DXMATRIX mtxRot, mtxTrans;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	// ���C�g����
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �J�����O�Ȃ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVtx, 0, m_nNumPolygon);

	// ���ʂ��J�����O
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// ���C�g�L��
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=========================================================================================================================
// ���_���̍쐬
//=========================================================================================================================
void CMeshCylinder::MakeVertex(int nLap, int nStage, float fRadius, float fHeight)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	int nCntLap;
	int nCntStage;
	int nCntIdx;

	// �e��l�̐ݒ�
	m_nLap = nLap;
	m_nStage = nStage;
	m_fRadius = fRadius;
	m_fHeight = fHeight;

	m_nNumVtx = (m_nLap + 1) * (m_nStage + 1);						// ���_��
	m_nNumPolygon = m_nLap * m_nStage * 2 + (4 * (m_nStage - 1));	// �|���S����

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// �C���f�b�N�X�o�b�t�@�𐶐�
	pDevice->CreateIndexBuffer(sizeof(WORD) * (m_nNumPolygon + 2),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,			// 16�r�b�g�̃f�[�^���m��
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fAngle = 1.0f / (m_nLap / 2);	// ����̊p�x

	for (nCntStage = 0; nCntStage <= m_nStage; nCntStage++)
	{
		for (nCntLap = 0; nCntLap <= m_nLap; nCntLap++)
		{
			// ���_�̍��W
			pVtx[0].pos.x = sinf(D3DX_PI * (0.0f + (fAngle * nCntLap))) * m_fRadius;
			if (m_bAlong)
			{// �n�ʂɉ��킹��
				pVtx[0].pos.y = GetHeightAlongField(pVtx[0].pos + m_pos) + m_fFloatHeight + (m_fHeight - ((nCntStage * m_fHeight) / m_nStage));
			}
			else
			{// �n�ʂɓY�킹�Ȃ�
				pVtx[0].pos.y = m_fHeight - ((nCntStage * m_fHeight) / m_nStage) * 1.0f;
			}
			pVtx[0].pos.z = cosf(D3DX_PI * (0.0f + (fAngle * nCntLap))) * m_fRadius;

			// �@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// ���_�J���[
			pVtx[0].col = m_col;

			//�e�N�X�`�����W
			pVtx[0].tex = D3DXVECTOR2(nCntLap * 1.0f, nCntStage * 1.0f);

			pVtx += 1;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	WORD *pIdx;		// �C���f�b�N�X�f�[�^�ւ̃|�C���^

	// �C���f�b�N�X�o�b�t�@�����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (nCntIdx = 0; nCntIdx <= m_nLap; nCntIdx++)
	{// ���_�̏��Ԃ�ݒ�
		pIdx[0] = (m_nLap + 1) + nCntIdx;
		pIdx[1] = 0 + nCntIdx;

		pIdx += 2;
	}

	// ���_�o�b�t�@���A�����b�N
	m_pIdxBuff->Unlock();
}

//=========================================================================================================================
// ���_������蒼��
//=========================================================================================================================
void CMeshCylinder::ReMakeVertex(void)
{
	if (NULL != m_pVtxBuff)
	{// ���_�o�b�t�@�̔j��
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	if (NULL != m_pIdxBuff)
	{// �C���f�b�N�X�o�b�t�@�̔j��
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �J�E���^
	int nCntLap;
	int nCntStage;
	int nCntIdx;

	m_nNumVtx = (m_nLap + 1) * (m_nStage + 1);						// ���_��
	m_nNumPolygon = m_nLap * m_nStage * 2 + (4 * (m_nStage - 1));	// �|���S����

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// �C���f�b�N�X�o�b�t�@�𐶐�
	pDevice->CreateIndexBuffer(sizeof(WORD) * (m_nNumPolygon + 2),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,			// 16�r�b�g�̃f�[�^���m��
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	const float fAngle = 1.0f / (m_nLap / 2);	// ����̊p�x

	for (nCntStage = 0; nCntStage <= m_nStage; nCntStage++)
	{
		for (nCntLap = 0; nCntLap <= m_nLap; nCntLap++)
		{
			// ���_�̍��W
			pVtx[0].pos.x = sinf(D3DX_PI * (0.0f + (fAngle * nCntLap))) * m_fRadius;
			if (m_bAlong)
			{// �n�ʂɉ��킹��
				pVtx[0].pos.y = GetHeightAlongField(pVtx[0].pos) + m_fFloatHeight + (m_fHeight - ((nCntStage * m_fHeight) / m_nStage));
			}
			else
			{// �n�ʂɓY�킹�Ȃ�
				pVtx[0].pos.y = m_fHeight - ((nCntStage * m_fHeight) / m_nStage) * 1.0f;
			}

			pVtx[0].pos.z = cosf(D3DX_PI * (0.0f + (fAngle * nCntLap))) * m_fRadius;

			// �@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// ���_�J���[
			pVtx[0].col = m_col;

			//�e�N�X�`�����W
			pVtx[0].tex = D3DXVECTOR2(nCntLap * 1.0f, nCntStage * 1.0f);

			pVtx += 1;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	WORD *pIdx;		// �C���f�b�N�X�f�[�^�ւ̃|�C���^

	// �C���f�b�N�X�o�b�t�@�����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (nCntStage = 0, nCntIdx = 0; nCntStage < m_nStage; nCntStage++)
	{
		for (nCntLap = 0; nCntLap <= m_nLap; nCntLap++, nCntIdx++)
		{
			pIdx[0] = (m_nLap + 1) + nCntIdx;
			pIdx[1] = 0 + nCntIdx;

			pIdx += 2;

			if (nCntStage < m_nStage - 1 && nCntLap == m_nLap)
			{// �܂�Ԃ���
				pIdx[0] = 0 + nCntIdx;
				pIdx[1] = (m_nLap + 1) + nCntIdx + 1;

				pIdx += 2;
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N
	m_pIdxBuff->Unlock();
}

//=========================================================================================================================
// �n�ʂɉ��킹�邩�ǂ���
//=========================================================================================================================
void CMeshCylinder::AlongField(bool bAlong, float fFloatHeight)
{
	if (bAlong != m_bAlong)
	{// �t���O���ύX���ꂽ�Ƃ�
		m_bAlong = bAlong;
		m_fFloatHeight = fFloatHeight;

		VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

		// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
		{
			pVtx[nCntVtx].pos.y = 0.0f;
		}

		for (int nCntStage = 0; nCntStage <= m_nStage; nCntStage++)
		{
			for (int nCntLap = 0; nCntLap <= m_nLap; nCntLap++)
			{
				if (m_bAlong)
				{// �n�ʂɉ��킹��
					pVtx[0].pos.y = GetHeightAlongField(pVtx[0].pos) + m_fFloatHeight + (m_fHeight - ((nCntStage * m_fHeight) / m_nStage));
				}
				else
				{// �n�ʂɉ��킹�Ȃ�
					pVtx[0].pos.y = m_fHeight - ((nCntStage * m_fHeight) / m_nStage) * 1.0f;
				}

				pVtx += 1;	// �|�C���^��i�߂�
			}
		}

		// ���_�o�b�t�@���A�����b�N
		m_pVtxBuff->Unlock();
	}
}

//=========================================================================================================================
// �n�ʂɉ����������̎擾
//=========================================================================================================================
float CMeshCylinder::GetHeightAlongField(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 pos1 = pos;

	//�v���C�I���e�B�[�`�F�b�N
	CScene *pScene = CScene::GetTop(MESH_PRIOTITY);

	//NULL�`�F�b�N
	while (pScene != NULL)
	{
		//Update��Uninit����Ă��܂��ꍇ�@Next��������\�������邩��Next�Ƀf�[�^���c���Ă���
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetObjType() == OBJTYPE_MESH)
		{//�^�C�v���n�ʂ�������
			CMeshField *pField = (CMeshField*)pScene;
			if (pField->OnField(pos1 + D3DXVECTOR3(0.0f, 50.0f, 0.0f), 50.0f))
			{// �X�΂̌v�Z
				pField->GetHeight(pos1);
				break;
			}
		}
		//Next�Ɏ���Scene������
		pScene = pSceneNext;
	}

	return pos1.y;
}

//=========================================================================================================================
// �F�̐ݒ�
//=========================================================================================================================
void CMeshCylinder::SetColor(D3DXCOLOR col)
{
	m_col = col;

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
	{// ���_�������[�v
		pVtx[0].col = m_col;	// ���_�J���[
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}