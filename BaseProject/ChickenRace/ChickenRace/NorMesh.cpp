//=============================================================================
//
// mesh���� [mesh.cpp]
// Author : Ryo Sugimoto
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "NorMesh.h"
#include "renderer.h"
#include "manager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//==================================================================================================//
//    * ���b�V���̐����֐� *
//==================================================================================================//
bool	CNorMesh::Create(CNorMesh *&pNorMesh)
{
	pNorMesh = NULL;
	pNorMesh = new CNorMesh;
	if (pNorMesh != NULL) { pNorMesh->Init(); return true; }
	//MessageBox(0, "�����ł��Ȃ��I", "", MB_OK);

	return false;
}
//==================================================================================================//
//    * ���b�V���̃Z�b�g�֐� *
//==================================================================================================//
void CNorMesh::Set(TYPE type, D3DXVECTOR3 pos, int nVertical, int nCorss, bool bMesh)	//�Q�[�W�Z�b�g����
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾

	m_Type = type;
	m_pos = pos;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nVertical = nVertical;
	m_nCross = nCorss;
	m_bMesh = bMesh;
	m_nNumVertex = (nVertical + 1) * (nCorss + 1);		//���_��
	m_nNumIdx = (nVertical * nCorss) * 2 + (nVertical - 1) * 4 + 2;				//�C���f�b�N�X��
	m_nNumPolygon = (nVertical * nCorss) * 2 + (nVertical - 1) * 4;		//�|���S����

																		//���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIdx, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIdxBuff, NULL);
	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^
	WORD * pIdx;		//�C���f�b�N�X�f�[�^�ւ̃|�C���^
	float fUV_X = 0.25f, fUV_Y = 0.2f;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	//���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
												//���_�A�@���A�J���[�A�e�N�X�`���̒��_�ݒ�
	for (int nCntVertical = 0; nCntVertical < nVertical + 1; nCntVertical++)
	{//�c��
		for (int nCntCorss = 0; nCntCorss < nCorss + 1; nCntCorss++)
		{//����
		 //���_���W
			switch (type)
			{
			case TYPE_FIELD:
				pVtx[0].pos = D3DXVECTOR3(-100.0f * 2 + 100.0f * nCntCorss, 0.0f, 100.0f * 2 - 100.0f * nCntVertical);
				break;
			case TYPE_WALL:
				pVtx[0].pos = D3DXVECTOR3(-100.0f * nCntCorss, 100.0f * nCntVertical, 100.0f * nCntCorss);
				break;
			case TYPE_CEILING:
				pVtx[0].pos = D3DXVECTOR3(-100.0f * 2 + 100.0f * nCntCorss, 100.0f - rand() % 10 * 10.0f, 100.0f * 2 + 100.0f * nCntVertical);
				if (nCntCorss == 0 || nCntCorss == nCorss || nCntVertical == 0 || nCntVertical == nVertical)
				{
					pVtx[0].pos.y = 0.0f;
				}
				break;
			case TYPE_CYLINDER:
				fUV_Y = 0.5f;
				pVtx[0].pos = D3DXVECTOR3(
					sinf((float)(nCntCorss + 1) / (float)(nCorss + 2)) * 10.0f
					, nCntVertical * 10.0f
					, cosf((float)(nCntCorss + 1) / (float)(nCorss + 2)) * 10.0f);
				break;
			}


			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[0].tex = D3DXVECTOR2(nCntCorss * fUV_X, nCntVertical * fUV_Y);
			pVtx += 1;
		}
	}

	m_pVtxBuff->Unlock();	//���_�o�b�t�@�̃A�����b�N����

							//�C���f�b�N�X�o�b�t�@�̃��b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// �C���f�b�N�X�̐ݒ�
	for (int nCount_yz = 0; nCount_yz < nVertical; nCount_yz++)
	{// yz���J�E���g
		for (int nCount_x = 0; nCount_x < nCorss + 1; nCount_x++)
		{// x���J�E���g
		 // �C���f�b�N�X���̐ݒ�
			pIdx[0] = (WORD)(((nCount_yz + 1) * (nCorss + 1)) + nCount_x);
			pIdx[1] = (WORD)(((nCount_yz) * (nCorss + 1)) + nCount_x);

			pIdx += 2;			// �C���f�b�N�X�o�b�t�@��i�߂�

			if (nVertical > 1 && nCount_yz != nVertical - 1 && nCount_x == nCorss)
			{// ���[�ɓ��B������
			 // �C���f�b�N�X���̐ݒ�i�k�ރ|���S���j
				pIdx[0] = (WORD)(((nCount_yz) * (nCorss + 1)) + nCount_x);
				pIdx[1] = (WORD)(((nCount_yz + 2) * (nCorss + 1)));

				pIdx += 2;		// �C���f�b�N�X�o�b�t�@��i�߂�
			}
		}
	}
	//�C���f�b�N�X�̃o�b�t�@�̃A�����b�N
	m_pIdxBuff->Unlock();

	SetMtx();
}
//==================================================================================================//
//    * �@���̃Z�b�g���� *
//==================================================================================================//
void	CNorMesh::SetNor(void)
{
	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^
	D3DXVECTOR3 vec0, vec1;
	D3DXVECTOR3 nor[2];
	int		nCntHit = 0;
	int nCntVertical, nCntCorss;
	VERTEX_3D *pVtxLU, *pVtxLD, *pVtxRU, *pVtxRD;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	//���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	for (nCntVertical = 0; nCntVertical < m_nVertical + 1; nCntVertical++)
	{//�c��
		for (nCntCorss = 0; nCntCorss < m_nCross + 1; nCntCorss++)
		{//����
			pVtx[nCntVertical * (m_nCross + 1) + nCntCorss].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}

	for (int nCntVertical = 0; nCntVertical < m_nVertical; nCntVertical++)
	{//�c��
		for (int nCntCorss = 0; nCntCorss < m_nCross; nCntCorss++)
		{//����
		 //���_�擾
			pVtxLU = &pVtx[(nCntVertical) * (m_nCross + 1) + nCntCorss];
			pVtxLD = &pVtx[(nCntVertical + 1) * (m_nCross + 1) + nCntCorss];
			pVtxRU = &pVtx[(nCntVertical) * (m_nCross + 1) + nCntCorss + 1];
			pVtxRD = &pVtx[(nCntVertical + 1) * (m_nCross + 1) + nCntCorss + 1];
			//�@�����Z
			PlusNor(pVtxLD, pVtxLU, pVtxRD);
			PlusNor(pVtxRD, pVtxRD, pVtxLU);
		}
	}

	for (nCntVertical = 0; nCntVertical < m_nVertical + 1; nCntVertical++)
	{//�c��
		for (nCntCorss = 0; nCntCorss < m_nCross + 1; nCntCorss++)
		{//����
			if (nCntVertical == 0)
			{//��[
				if (nCntCorss == 0 || nCntCorss == m_nCross) { pVtx[nCntVertical * (m_nCross + 1) + nCntCorss].nor /= 2.0f - (nCntCorss / m_nCross); }
				else { pVtx[nCntVertical * (m_nCross + 1) + nCntCorss].nor /= 3.0f; }
			}
			else if (nCntVertical == m_nVertical)
			{//���[
				if (nCntCorss == 0 || nCntCorss == m_nCross) { pVtx[nCntVertical * (m_nCross + 1) + nCntCorss].nor /= 1.0f + (nCntCorss / m_nCross); }
				else { pVtx[nCntVertical * (m_nCross + 1) + nCntCorss].nor /= 3.0f; }
			}
			else
			{//����
				if (nCntCorss == 0 || nCntCorss == m_nCross) { pVtx[nCntVertical * (m_nCross + 1) + nCntCorss].nor /= 3.0f; }
				else { pVtx[nCntVertical * (m_nCross + 1) + nCntCorss].nor /= 6.0f; }
			}
		}
	}
	m_pVtxBuff->Unlock();	//���_�o�b�t�@�̃A�����b�N����
}
//==================================================================================================//
//    * �}�g���b�N�X�v�Z���� *
//==================================================================================================//
void	CNorMesh::SetMtx(void)
{
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X

												// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
}
//==================================================================================================//
//    * �@���̉��Z���� *
//==================================================================================================//
void CNorMesh::PlusNor(VERTEX_3D *pVtx, VERTEX_3D *pVtxL, VERTEX_3D *pVtxR)	//�@���̃Z�b�g����
{
	D3DXVECTOR3 vec0, vec1;
	D3DXVECTOR3 nor;

	//�@���v�Z
	vec0 = pVtxL->pos - pVtx->pos;
	vec1 = pVtxR->pos - pVtx->pos;
	D3DXVec3Cross(&nor, (m_bMesh ? &vec0 : &vec1), (m_bMesh ? &vec1 : &vec0));	//�O�όv�Z
	D3DXVec3Normalize(&nor, &nor);												//���K���ɂ���

																				//���Z
	pVtx->nor += nor;
	pVtxL->nor += nor;
	pVtxR->nor += nor;
}
//=============================================================================
// ���b�V���̏���������
//=============================================================================
HRESULT  CNorMesh::Init(void)
{
	m_pTex = NULL;
	m_pVtxBuff = NULL;
	m_pIdxBuff = NULL;
	m_bMesh = true;
	return S_OK;
}
//=============================================================================
// ���b�V���̏I������
//=============================================================================
void  CNorMesh::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	//�C���f�b�N�X�o�b�t�@�̉��
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	delete this;
}
//=============================================================================
// ���b�V���̕`�揈��
//=============================================================================
void  CNorMesh::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@�f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	if (m_pTex != NULL) { pDevice->SetTexture(0, m_pTex); }
	else { pDevice->SetTexture(0, NULL); }

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVertex, 0, m_nNumPolygon);

	pDevice->SetTexture(0, NULL);
}
