//=============================================================================
//
// ���b�V���̏��� [mesh.cpp]
// Author : �L�n���u
//
//=============================================================================
#include "scene.h"
#include "mesh.h"
#include "manager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXT_MESHNAME			"data\\TEXT\\mesh.txt"					// �ǂݍ��ރe�L�X�g�t�@�C��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CMesh::CMesh() : CScene(MESH_PRIOTITY, CScene::OBJTYPE_MESH)
{
	m_pTexture = NULL;								// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;								// ���_�o�b�t�@�ւ̃|�C���^
	m_pIdxBuff = NULL;								// �C���f�b�N�X�ւ̃|�C���^
	m_fMeshTexX = 1.0f;
	m_fMeshTexY = 1.0f;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ����
	m_nType = 0;									// ���
	m_fVtx0 = 0.0f;
	m_fVtxMeshX = 0.0f;
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CMesh::~CMesh()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CMesh::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�ϐ��錾
	int nCntMeshZ;
	int nCntMeshX;
	int nCntVertical;
	int nCntCorss;
	int nCntIdx;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * ((m_nMeshX + 1) * (m_nMeshZ + 1)),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// �C���f�b�N�X�o�b�t�@�𐶐�
	pDevice->CreateIndexBuffer(sizeof(WORD) * ((m_nMeshX * m_nMeshZ) * 2 + (4 * (m_nMeshZ - 1)) + 2),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,			// 16�r�b�g�̃f�[�^���m��
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	switch (m_nType)
	{
		//=======================
		//	���b�V���t�B�[���h
		//=======================
	case 0:
		//���_�A�@���A�J���[�A�e�N�X�`���̒��_�ݒ�
		for (nCntMeshZ = 0; nCntMeshZ <= m_nMeshZ; nCntMeshZ++)
		{//�c��
			for (nCntMeshX = 0; nCntMeshX <= m_nMeshX; nCntMeshX++)
			{//����
				// ���_�̍��W
				pVtx[0].pos = D3DXVECTOR3(((nCntMeshX * m_fMeshWidth) / m_nMeshX), 0.0f/*rand() % 100 * 1.0f*/, ((nCntMeshZ * -m_fMeshDepth) / m_nMeshZ));

				// �@���̐ݒ�
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// ���_�J���[
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				//�e�N�X�`�����W
				pVtx[0].tex = D3DXVECTOR2(nCntMeshX * m_fMeshTexX, nCntMeshZ * m_fMeshTexY);

				//���_��ύX
				if (nCntMeshZ == 0 && nCntMeshX == 0)
				{//����
					pVtx[0].pos.y = m_fVtx0;
				}
				else if (nCntMeshX > 0 && nCntMeshZ == 0 && nCntMeshX == m_nMeshX)
				{//�E��
					pVtx[0].pos.y = m_fVtxMeshX;
				}
				//���_��ύX
				else if (nCntMeshZ == nCntMeshZ && nCntMeshX == 0)
				{//����
					pVtx[0].pos.y = m_fVtxMeshZ;
				}
				else if (nCntMeshX > 0 && nCntMeshZ > 0 && nCntMeshZ == nCntMeshZ && nCntMeshX == m_nMeshX)
				{//�E��
					pVtx[0].pos.y = m_fVtxMeshXMeshZ;
				}

				pVtx += 1;
			}
		}

		break;
		//=======================
		//	���b�V���V�����_�[
		//=======================
	case 1:
		//���_�A�@���A�J���[�A�e�N�X�`���̒��_�ݒ�
		for (nCntVertical = 0; nCntVertical <= m_nMeshZ; nCntVertical++)
		{//�c��
			for (nCntCorss = 0; nCntCorss < m_nMeshX + 1; nCntCorss++)
			{//����
				//pVtx[���_�ԍ�].x,,z = sinf((3.14�̉~���@�����-������ƁA�J�����O�������t�ɂȂ�@* (�J�E���g�̉񐔁@* �w�肵�������v�Z���Ċp�x����)))* ���a;
				pVtx[0].pos.x = sinf((-D3DX_PI * (nCntCorss * (1.0f / (m_nMeshX / 2)))))* 2000.0f;
				//pVtx[���_�ԍ�].y = �c�̃J�E���g�񐔁@* �c�̕�
				pVtx[0].pos.y = nCntVertical * 700.0f;
				pVtx[0].pos.z = cosf((-D3DX_PI * (nCntCorss * (1.0f / (m_nMeshX / 2)))))* 2000.0f;

				//�@��
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				//���_�J���[
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				//�e�N�X�`��
				pVtx[0].tex = D3DXVECTOR2(nCntCorss * 1.0f, nCntVertical * 1.0f);

				//���炵
				pVtx += 1;
			}
		}

		break;
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	//���b�V���t�B�[���h�̂ݖ@���̌v�Z������
	if(m_nType != 1)
	{//�@���̌v�Z
		CMesh::Nor();
	}

	WORD *pIdx;		// �C���f�b�N�X�f�[�^�ւ̃|�C���^

	// �C���f�b�N�X�o�b�t�@�����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	switch (m_nType)
	{
		//=======================
		//	���b�V���t�B�[���h
		//=======================
	case 0:
		//�c�̃u���b�N
		for (nCntMeshZ = 0, nCntIdx = 0; nCntMeshZ < m_nMeshZ; nCntMeshZ++)
		{
			//���̃u���b�N
			for (nCntMeshX = 0; nCntMeshX <= m_nMeshX; nCntMeshX++, nCntIdx++)
			{
				//pIdx[�ԍ�] = (�w�肵���� + �P�i���̂P�͂O����X�^�[�g�������ɂP����Ȃ�����j + �C���f�b�N�X�̉�)
				pIdx[0] = (m_nMeshX + 1) + nCntIdx;
				pIdx[1] = 0 + nCntIdx;

				pIdx += 2;

				if (nCntMeshZ < m_nMeshZ - 1 && nCntMeshX == m_nMeshX)
				{// �܂�Ԃ���
					pIdx[0] = 0 + nCntIdx;
					pIdx[1] = (m_nMeshX + 1) + nCntIdx + 1;

					pIdx += 2;
				}
			}
		}
		break;
		//=======================
		//	���b�V���V�����_�[
		//=======================
	case 1:
		//�c�̃u���b�N
		for (nCntVertical = 0, nCntIdx = 0; nCntVertical < m_nMeshZ; nCntVertical++)
		{
			//���̃u���b�N
			for (nCntCorss = 0; nCntCorss <= m_nMeshX; nCntCorss++, nCntIdx++)
			{//�����J�E���g
				//pIdx[�ԍ�] = (�w�肵���� + �P�i���̂P�͂O����X�^�[�g�������ɂP����Ȃ�����j + �C���f�b�N�X�̉�)
				pIdx[0] = (m_nMeshX + 1) + nCntIdx;
				pIdx[1] = 0 + nCntIdx;

				//�Q���_���炵
				pIdx += 2;

				if (nCntVertical < m_nMeshZ - 1 && nCntCorss == m_nMeshX)
				{//�w�肵���c-1�ɍs�����Ƃ��Ɖ����w�肵�����܂ōs�����Ƃ�
					pIdx[0] = 0 + nCntIdx;
					pIdx[1] = (m_nMeshX + 1) + nCntIdx + 1;

					//�Q���_�����炷
					pIdx += 2;
				}
			}
		}
		break;
	}

	// ���_�o�b�t�@���A�����b�N
	m_pIdxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CMesh::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �C���f�b�N�X�o�b�t�@�̊J��
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CMesh::Update(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();

	if (pCInputKeyBoard->GetKeyboardPress(DIK_N) == true)
	{//���̏�
		if (m_nType != 1)
		{//�@���̌v�Z
			CMesh::Nor();
		}
	}

	//CDebugProc::Print("m_fVtx0 : %.1f\n", m_fVtx0);
	//CDebugProc::Print("m_fVtxMeshX : %.1f\n", m_fVtxMeshX);
}
//=============================================================================
// �`�揈��
//=============================================================================
void CMesh::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX	mtxRot, mtxTrans;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
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
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, ((m_nMeshX + 1) * (m_nMeshZ + 1)), 0, ((m_nMeshX * m_nMeshZ * 2) + (4 * (m_nMeshZ - 1))));

}
//===============================================================================
//�@�N���G�C�g
//===============================================================================
CMesh * CMesh::Create()
{
	CMesh *pMesh = NULL;

	//NULL�`�F�b�N
	if (pMesh == NULL)
	{//�������̓��I�m��

		pMesh = new CMesh;

		if (pMesh != NULL)
		{
			//�I�u�W�F�N�g�N���X�̐���
			pMesh->Init();
		}
	}

	return pMesh;
}
//===============================================================================
// ���L�e�N�X�`���̊��蓖��
//===============================================================================
void CMesh::BindTexture(LPDIRECT3DTEXTURE9 Texture)
{
	m_pTexture = Texture;
}
//===============================================================================
//�@�ʒu�̑��
//===============================================================================
void CMesh::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}
//===============================================================================
//�@���b�V���̏��
//===============================================================================
void CMesh::SetMeshInfo(int nMeshX, int nMeshZ, float fMeshTexX, float fMeshTexY, float fMeshDepth, float fMeshWidth)
{
	m_nMeshX = nMeshX;			//������X
	m_nMeshZ = nMeshZ;			//������Z
	m_fMeshTexX = fMeshTexX;	//�e�N�X�`��X���W
	m_fMeshTexY = fMeshTexY;	//�e�N�X�`��Y���W
	m_fMeshDepth = fMeshDepth;	//���s
	m_fMeshWidth = fMeshWidth;	//����
}
//===============================================================================
//�@�@��
//===============================================================================
void CMesh::Nor(void)
{
	//�ϐ��錾
	D3DXVECTOR3 vec0;
	D3DXVECTOR3 vec1;
	D3DXVECTOR3 nor;
	D3DXVECTOR3 VtxNor[2];

	int nCntMeshZ;
	int nCntMeshX;

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//pVtx�̏�����
	//�c�̃u���b�N
	for (nCntMeshZ = 0; nCntMeshZ < m_nMeshZ; nCntMeshZ++)
	{
		//���̃u���b�N
		for (nCntMeshX = 0; nCntMeshX < m_nMeshX; nCntMeshX++)
		{
			pVtx[nCntMeshX + nCntMeshZ * (m_nMeshX + 1)].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		}
	}

	//�c�̃u���b�N
	for (nCntMeshZ = 0; nCntMeshZ < m_nMeshZ; nCntMeshZ++)
	{
		//���̃u���b�N
		for (nCntMeshX = 0; nCntMeshX < m_nMeshX; nCntMeshX++)
		{
			//���݂̏�������Ă���u���b�N�̒��_�̏o����
			int	nMeshLU = nCntMeshX + nCntMeshZ * (m_nMeshX + 1);					//��@0�Ԗڂ̒��_
			int	nMeshRU = (nCntMeshX + 1) + nCntMeshZ * (m_nMeshX + 1);				//��@1�Ԗڂ̒��_
			int nMeshLD = nCntMeshX + (nCntMeshZ + 1) * (m_nMeshX + 1);				//��@3�Ԗڂ̒��_
			int nMeshRD = (nCntMeshX + 1) + (nCntMeshZ + 1) * (m_nMeshX + 1);		//��@4�Ԗڂ̒��_

			//======================
			//  1�|���S����nor
			//======================
			//������nor
			vec0 = pVtx[nMeshLU].pos - pVtx[nMeshLD].pos;
			vec1 = pVtx[nMeshRD].pos - pVtx[nMeshLD].pos;
			//�O�όv�Z
			D3DXVec3Cross(&VtxNor[0], &vec0, &vec1);
			D3DXVec3Normalize(&VtxNor[0], &VtxNor[0]);

			//======================
			//  �Q�|���S����nor
			//======================
			//�E���nor
			vec0 = pVtx[nMeshRD].pos - pVtx[nMeshRU].pos;
			vec1 = pVtx[nMeshLU].pos - pVtx[nMeshRU].pos;
			//�O�όv�Z
			D3DXVec3Cross(&VtxNor[1], &vec0, &vec1);
			D3DXVec3Normalize(&VtxNor[1], &VtxNor[1]);

			pVtx[nMeshLD].nor += VtxNor[0];
			pVtx[nMeshLU].nor += VtxNor[0] + VtxNor[1];
			pVtx[nMeshRD].nor += VtxNor[0] + VtxNor[1];
			pVtx[nMeshRU].nor += VtxNor[1];
		}
	}

	//�c�̃u���b�N
	for (nCntMeshZ = 0; nCntMeshZ <= m_nMeshZ; nCntMeshZ++)
	{
		//���̃u���b�N
		for (nCntMeshX = 0; nCntMeshX <= m_nMeshX; nCntMeshX++)
		{
			if (nCntMeshZ == 0)
			{//�c�̗�
				if (nCntMeshX > 0 && nCntMeshX < m_nMeshX)
				{
					pVtx[nCntMeshX + nCntMeshZ * (m_nMeshX + 1)].nor /= 3.0f;
				}
				else if(nCntMeshX == 0)
				{
					pVtx[nCntMeshX + nCntMeshZ * (m_nMeshX + 1)].nor /= 2.0f;
				}
			}
			else if (nCntMeshZ == m_nMeshZ)
			{//��
				if (nCntMeshX > 0 && nCntMeshX < m_nMeshX)
				{
					pVtx[nCntMeshX + nCntMeshZ * (m_nMeshX + 1)].nor /= 3.0f;
				}
				else if (nCntMeshX == m_nMeshX)
				{
					pVtx[nCntMeshX + nCntMeshZ * (m_nMeshX + 1)].nor /= 2.0f;
				}
			}
			else
			{//���̗�
				if (nCntMeshX > 0 && nCntMeshX < m_nMeshX)
				{
					pVtx[nCntMeshX + nCntMeshZ * (m_nMeshX + 1)].nor /= 6.0f;
				}
				else
				{
					pVtx[nCntMeshX + nCntMeshZ * (m_nMeshX + 1)].nor /= 3.0f;
				}
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
//===============================================================================
//�@�v���C���[��pos.y����ς𗘗p���ďo��
//===============================================================================
bool CMesh::GetHeight(D3DXVECTOR3 &pos)
{
	//�ϐ��錾
	D3DXVECTOR3 vec0;
	D3DXVECTOR3 vec1;
	D3DXVECTOR3 nor;
	D3DXVECTOR3 VtxPos[3];
	float		fHeight;

	//�v���C���[�ƃ��b�V���t�B�[���h�̈ʒu�̍������o��
	D3DXVECTOR3 posMtx = pos - m_pos;

	//���݃v���C���[�����u���b�N�ڂɂ��邩���m�F���鎮
	int			nMeshX = ((int)(posMtx.x)) / ((int)(m_fMeshWidth / m_nMeshX));
	int			nMeshZ = ((int)(posMtx.z)) / ((int)(m_fMeshDepth / m_nMeshZ)) * -1;

	//���݂̏�������Ă���u���b�N�̒��_�̏o����
	int			nMeshLU = nMeshX + nMeshZ * (m_nMeshX + 1);					//��@0�Ԗڂ̒��_
	int			nMeshRU = (nMeshX + 1) + nMeshZ * (m_nMeshX + 1);			//��@1�Ԗڂ̒��_
	int			nMeshLD = nMeshX + (nMeshZ + 1) * (m_nMeshX + 1);			//��@3�Ԗڂ̒��_
	int			nMeshRD = (nMeshX + 1) + (nMeshZ + 1) * (m_nMeshX + 1);		//��@4�Ԗڂ̒��_

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//============================
	//		�O�p�`
	//============================
	//�x�N�g���v�Z�� �ړI - ����
	vec0 = pVtx[nMeshLU].pos - pVtx[nMeshRD].pos;
	//�x�N�g���v�Z�� �����̈ʒu�@- ����
	vec1 = posMtx - pVtx[nMeshRD].pos;

	if ((vec0.x * vec1.z) - (vec0.z * vec1.x) <= 0)
	{//�O�p�`���E�ɂ���ꍇ
		//CDebugProc::Print("MeshField���E�͈� : TURE\n");
		vec0 = (posMtx - pVtx[nMeshLU].pos);

		//�e���_�̕ۊ�
		VtxPos[0] = pVtx[nMeshRU].pos;
		VtxPos[1] = pVtx[nMeshRD].pos;
		VtxPos[2] = pVtx[nMeshLU].pos;
	}
	else if ((vec0.x * vec1.z) - (vec0.z * vec1.x) >= 0)
	{//�O�p�`��荶�ɂ���ꍇ
		//CDebugProc::Print("MeshField�����͈� : TURE\n");
		vec0 = (posMtx - pVtx[nMeshRD].pos);

		//�e���_�̕ۊ�
		VtxPos[0] = pVtx[nMeshLD].pos;
		VtxPos[1] = pVtx[nMeshLU].pos;
		VtxPos[2] = pVtx[nMeshRD].pos;
	}

	//���_�o�b�t�@�̃A�����b�N����
	m_pVtxBuff->Unlock();

	//�����@���������O�ɕۑ�����
	fHeight = VtxPos[0].y;

	//3���_��0�̒l���Ђ��Ă����Ė@���ƃv���C���[�̃x�N�g���𒼌������邽��
	VtxPos[2].y -= VtxPos[0].y;
	VtxPos[1].y -= VtxPos[0].y;
	VtxPos[0].y -= VtxPos[0].y;

	//�@���̌v�Z
	vec0 = VtxPos[1] - VtxPos[0];

	vec1 = VtxPos[2] - VtxPos[0];

	//�O�ς̌v�Z
	D3DXVec3Cross(&nor, &vec0, &vec1);

	//���K��
	D3DXVec3Normalize(&nor, &nor);

	//�v���C���[�ւ̃x�N�g��
	vec0 = posMtx - VtxPos[0];

	//���όv�Z��
	vec0.y = (-(vec0.x * nor.x) - (vec0.z * nor.z)) / nor.y;

	//�v���C���[��y�ɓ����
	posMtx.y = vec0.y + fHeight + m_pos.y;

	if (pos.y < posMtx.y)
	{// �v���C���[��n�ʂɏ悹��
		pos.y = posMtx.y;
		return true;
	}
	return false;
}

//===============================================================================
//�@�v���C���[��pos.y����ς𗘗p���ďo��
//===============================================================================
float CMesh::GetHeightMesh(D3DXVECTOR3 pos)
{
	//�ϐ��錾
	D3DXVECTOR3 vec0;
	D3DXVECTOR3 vec1;
	D3DXVECTOR3 nor;
	D3DXVECTOR3 VtxPos[3];
	float		fHeight;

	//�v���C���[�ƃ��b�V���t�B�[���h�̈ʒu�̍������o��
	D3DXVECTOR3 posMtx = pos - m_pos;

	//���݃v���C���[�����u���b�N�ڂɂ��邩���m�F���鎮
	int			nMeshX = ((int)(posMtx.x)) / ((int)(m_fMeshWidth / m_nMeshX));
	int			nMeshZ = ((int)(posMtx.z)) / ((int)(m_fMeshDepth / m_nMeshZ)) * -1;

	//���݂̏�������Ă���u���b�N�̒��_�̏o����
	int			nMeshLU = nMeshX + nMeshZ * (m_nMeshX + 1);					//��@0�Ԗڂ̒��_
	int			nMeshRU = (nMeshX + 1) + nMeshZ * (m_nMeshX + 1);			//��@1�Ԗڂ̒��_
	int			nMeshLD = nMeshX + (nMeshZ + 1) * (m_nMeshX + 1);			//��@3�Ԗڂ̒��_
	int			nMeshRD = (nMeshX + 1) + (nMeshZ + 1) * (m_nMeshX + 1);		//��@4�Ԗڂ̒��_

																			//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//============================
	//		�O�p�`
	//============================
	//�x�N�g���v�Z�� �ړI - ����
	vec0 = pVtx[nMeshLU].pos - pVtx[nMeshRD].pos;
	//�x�N�g���v�Z�� �����̈ʒu�@- ����
	vec1 = posMtx - pVtx[nMeshRD].pos;

	if ((vec0.x * vec1.z) - (vec0.z * vec1.x) <= 0)
	{//�O�p�`���E�ɂ���ꍇ
	 //CDebugProc::Print("MeshField���E�͈� : TURE\n");
		vec0 = (posMtx - pVtx[nMeshLU].pos);

		//�e���_�̕ۊ�
		VtxPos[0] = pVtx[nMeshRU].pos;
		VtxPos[1] = pVtx[nMeshRD].pos;
		VtxPos[2] = pVtx[nMeshLU].pos;
	}
	else if ((vec0.x * vec1.z) - (vec0.z * vec1.x) >= 0)
	{//�O�p�`��荶�ɂ���ꍇ
	 //CDebugProc::Print("MeshField�����͈� : TURE\n");
		vec0 = (posMtx - pVtx[nMeshRD].pos);

		//�e���_�̕ۊ�
		VtxPos[0] = pVtx[nMeshLD].pos;
		VtxPos[1] = pVtx[nMeshLU].pos;
		VtxPos[2] = pVtx[nMeshRD].pos;
	}

	//���_�o�b�t�@�̃A�����b�N����
	m_pVtxBuff->Unlock();

	//�����@���������O�ɕۑ�����
	fHeight = VtxPos[0].y;

	//3���_��0�̒l���Ђ��Ă����Ė@���ƃv���C���[�̃x�N�g���𒼌������邽��
	VtxPos[2].y -= VtxPos[0].y;
	VtxPos[1].y -= VtxPos[0].y;
	VtxPos[0].y -= VtxPos[0].y;

	//�@���̌v�Z
	vec0 = VtxPos[1] - VtxPos[0];

	vec1 = VtxPos[2] - VtxPos[0];

	//�O�ς̌v�Z
	D3DXVec3Cross(&nor, &vec0, &vec1);

	//���K��
	D3DXVec3Normalize(&nor, &nor);

	//�v���C���[�ւ̃x�N�g��
	vec0 = posMtx - VtxPos[0];

	//���όv�Z��
	vec0.y = (-(vec0.x * nor.x) - (vec0.z * nor.z)) / nor.y;

	//�v���C���[��y�ɓ����
	posMtx.y = vec0.y + fHeight + m_pos.y;

	return posMtx.y;
}

//===============================================================================
//�@�v���C���[��pos�𗘗p���āA�㉺�ɏグ��������
//===============================================================================
void CMesh::SetHeight(D3DXVECTOR3 pos, float fValue, float fRange)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fLength;

	//�v���C���[�ƃ��b�V���t�B�[���h�̈ʒu�̍������o��
	D3DXVECTOR3 posMtx = pos - m_pos;

	//�c�̃u���b�N
	for (int nCntMeshZ = 0; nCntMeshZ <= m_nMeshZ; nCntMeshZ++)
	{
		//���̃u���b�N
		for (int nCntMeshX = 0; nCntMeshX <= m_nMeshX; nCntMeshX++)
		{
			//pos����Ώۂ̒��_�܂ł̋���
			fLength = powf(pVtx->pos.x - posMtx.x , 2) + powf(pVtx->pos.z - posMtx.z, 2);

			//�Ώۂ̒��_���͈͓�
			if (fLength <= powf(fRange,2))
			{
				//�͈͓��ł̋����̔䗦�ɉ���������
				float fHight = cosf((D3DX_PI / 2) / (fRange / sqrtf(fLength))) * fValue;

				pVtx->pos.y += fHight;
			}

			pVtx++;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
//===============================================================================
// �t�@�C���ɃZ�[�u
//===============================================================================
void CMesh::TextSave(void)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�t�@�C���̃|�C���^
	FILE *pFile;

	//�t�@�C���ݒ�
	pFile = fopen(TEXT_MESHNAME, "wb");

	//�v���C���[�̃e�L�X�g�f�[�^�̓ǂݍ���
	if (pFile != NULL)
	{
		//�c�̃u���b�N
		for (int nCntMeshZ = 0; nCntMeshZ <= m_nMeshZ; nCntMeshZ++)
		{
			//���̃u���b�N
			for (int nCntMeshX = 0; nCntMeshX <= m_nMeshX; nCntMeshX++)
			{
				//�t�@�C���� �o�C�g�̃f�[�^����������
				fwrite(&pVtx[(nCntMeshZ + (m_nMeshX + 1) + nCntMeshX)].pos.y, sizeof(float), ((m_nMeshX + 1) * (m_nMeshZ + 1)), pFile);
			}
		}
		//�t�@�C�������
		fclose(pFile);
	}
	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
//===============================================================================
// �t�@�C���Ƀ��[�h
//===============================================================================
void CMesh::TextLoad(void)
{
	//�t�@�C���̃|�C���^
	FILE *pFile;

	//�t�@�C���ݒ�
	pFile = fopen(TEXT_MESHNAME, "rb");

	//�v���C���[�̃e�L�X�g�f�[�^�̓ǂݍ���
	if (pFile != NULL)
	{
		//�c�̃u���b�N
		for (int nCntMeshZ = 0; nCntMeshZ <= m_nMeshZ; nCntMeshZ++)
		{
			//���̃u���b�N
			for (int nCntMeshX = 0; nCntMeshX <= m_nMeshX; nCntMeshX++)
			{

			}
		}
		//�t�@�C�������
		fclose(pFile);
	}
}
//�X�L�����b�V��
//�N�H�[�^�j�I��
//�X�v���C��
//�X�e���V���V���h�E
//�X�e���V���~���[
//�t�B�[�h�o�b�N�G�t�F�N�g

//=========================================================================================================================
// �����̌����Ă�������ɑ΂��Ă̌X�΂̊p�x�̎擾�i��艺��̔���ɕK�v�j
//=========================================================================================================================
float CMesh::GetInclineAngle(D3DXVECTOR3 pos, float fRot)
{
	float fSpeed = 0.0f;	// �Ԃ����x
	D3DXVECTOR3 vecPlayer;	// �v���C���[�̌����̃x�N�g��
	float fAngle;			// �v���C���[�̌����Ɩ@���̂Ȃ��p
	D3DXVECTOR3 vec0;
	D3DXVECTOR3 vec1;
	D3DXVECTOR3 VtxPos[3];
	D3DXVECTOR3 nor;

	//�v���C���[�ƃ��b�V���t�B�[���h�̈ʒu�̍������o��
	D3DXVECTOR3 posMtx = pos - m_pos;

	//���݃v���C���[�����u���b�N�ڂɂ��邩���m�F���鎮
	int			nMeshX = ((int)(posMtx.x)) / ((int)(m_fMeshWidth / m_nMeshX));
	int			nMeshZ = ((int)(posMtx.z)) / ((int)(m_fMeshDepth / m_nMeshZ)) * -1;

	//���݂̏�������Ă���u���b�N�̒��_�̏o����
	int			nMeshLU = nMeshX + nMeshZ * (m_nMeshX + 1);					//��@0�Ԗڂ̒��_
	int			nMeshRU = (nMeshX + 1) + nMeshZ * (m_nMeshX + 1);			//��@1�Ԗڂ̒��_
	int			nMeshLD = nMeshX + (nMeshZ + 1) * (m_nMeshX + 1);			//��@3�Ԗڂ̒��_
	int			nMeshRD = (nMeshX + 1) + (nMeshZ + 1) * (m_nMeshX + 1);		//��@4�Ԗڂ̒��_

																			//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//============================
	//		�O�p�`
	//============================
	//�x�N�g���v�Z�� �ړI - ����
	vec0 = pVtx[nMeshLU].pos - pVtx[nMeshRD].pos;
	//�x�N�g���v�Z�� �����̈ʒu�@- ����
	vec1 = posMtx - pVtx[nMeshRD].pos;

	if ((vec0.x * vec1.z) - (vec0.z * vec1.x) <= 0)
	{//�O�p�`���E�ɂ���ꍇ
	 //CDebugProc::Print("MeshField���E�͈� : TURE\n");
		vec0 = (posMtx - pVtx[nMeshLU].pos);

		//�e���_�̕ۊ�
		VtxPos[0] = pVtx[nMeshRU].pos;
		VtxPos[1] = pVtx[nMeshRD].pos;
		VtxPos[2] = pVtx[nMeshLU].pos;
	}
	else if ((vec0.x * vec1.z) - (vec0.z * vec1.x) >= 0)
	{//�O�p�`��荶�ɂ���ꍇ
	 //CDebugProc::Print("MeshField�����͈� : TURE\n");
		vec0 = (posMtx - pVtx[nMeshRD].pos);

		//�e���_�̕ۊ�
		VtxPos[0] = pVtx[nMeshLD].pos;
		VtxPos[1] = pVtx[nMeshLU].pos;
		VtxPos[2] = pVtx[nMeshRD].pos;
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	// �v���C���[�̂���|���S���̖@�����o��
	vec0 = VtxPos[1] - VtxPos[0];			// ����
	vec1 = VtxPos[2] - VtxPos[0];			// �E��
	D3DXVec3Cross(&nor, &vec0, &vec1);			// �O�ς����߂�
	D3DXVec3Normalize(&nor, &nor);	// ���K��

									// �v���C���[�̌����̃x�N�g���̐���
	vecPlayer = D3DXVECTOR3(pos.x + sinf(fRot), pos.y, pos.z + cosf(fRot)) - pos;

	// �v���C���[�̌����Ɩ@���̂Ȃ��p�����߂�
	fAngle = (vecPlayer.x * nor.x) + (vecPlayer.y * nor.y) + (vecPlayer.z * nor.z);
	fAngle = fAngle / (sqrtf(powf(vecPlayer.x, 2) + powf(vecPlayer.y, 2) + powf(vecPlayer.z, 2)) * (sqrtf(powf(nor.x, 2) + powf(nor.y, 2) + powf(nor.z, 2))));

	return fAngle;
}

//=========================================================================================================================
// �@���̎擾
//=========================================================================================================================
D3DXVECTOR3 CMesh::GetNor(D3DXVECTOR3 pos)
{
	// �v���C���[�̃x�N�g�������߂�
	D3DXVECTOR3 vecPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXVECTOR3 vec0;
	D3DXVECTOR3 vec1;
	D3DXVECTOR3 VtxPos[3];
	int			nNumber;
	D3DXVECTOR3 nor;

	//�v���C���[�ƃ��b�V���t�B�[���h�̈ʒu�̍������o��
	D3DXVECTOR3 posMtx = pos - m_pos;

	//���݃v���C���[�����u���b�N�ڂɂ��邩���m�F���鎮
	int			nMeshX = ((int)(posMtx.x)) / ((int)(m_fMeshWidth / m_nMeshX));
	int			nMeshZ = ((int)(posMtx.z)) / ((int)(m_fMeshDepth / m_nMeshZ)) * -1;

	//���݂̏�������Ă���u���b�N�̒��_�̏o����
	int			nMeshLU = nMeshX + nMeshZ * (m_nMeshX + 1);					//��@0�Ԗڂ̒��_
	int			nMeshRU = (nMeshX + 1) + nMeshZ * (m_nMeshX + 1);			//��@1�Ԗڂ̒��_
	int			nMeshLD = nMeshX + (nMeshZ + 1) * (m_nMeshX + 1);			//��@3�Ԗڂ̒��_
	int			nMeshRD = (nMeshX + 1) + (nMeshZ + 1) * (m_nMeshX + 1);		//��@4�Ԗڂ̒��_

																			//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//============================
	//		�O�p�`
	//============================
	//�x�N�g���v�Z�� �ړI - ����
	vec0 = pVtx[nMeshLU].pos - pVtx[nMeshRD].pos;
	//�x�N�g���v�Z�� �����̈ʒu�@- ����
	vec1 = posMtx - pVtx[nMeshRD].pos;

	if ((vec0.x * vec1.z) - (vec0.z * vec1.x) <= 0)
	{//�O�p�`���E�ɂ���ꍇ
		vec0 = (posMtx - pVtx[nMeshLU].pos);

		//�e���_�̕ۊ�
		VtxPos[0] = pVtx[nMeshRU].pos;
		VtxPos[1] = pVtx[nMeshRD].pos;
		VtxPos[2] = pVtx[nMeshLU].pos;

		nNumber = 3;
	}
	else if ((vec0.x * vec1.z) - (vec0.z * vec1.x) >= 0)
	{//�O�p�`��荶�ɂ���ꍇ
		vec0 = (posMtx - pVtx[nMeshRD].pos);

		//�e���_�̕ۊ�
		VtxPos[0] = pVtx[nMeshLD].pos;
		VtxPos[1] = pVtx[nMeshLU].pos;
		VtxPos[2] = pVtx[nMeshRD].pos;

		nNumber = 0;
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	// �v���C���[�̂���|���S���̖@�����o��
	vec0 = VtxPos[1] - VtxPos[0];			// ����
	vec1 = VtxPos[2] - VtxPos[0];			// �E��
	D3DXVec3Cross(&nor, &vec0, &vec1);			// �O�ς����߂�
	D3DXVec3Normalize(&nor, &nor);	// ���K��

	return nor;	// �@���x�N�g����Ԃ�
}

//=========================================================================================================================
// ���݂���n�ʂ��ǂ����̔���
//=========================================================================================================================
bool CMesh::OnField(D3DXVECTOR3 pos, float fMaxHeight)
{
	//�v���C���[�ƃ��b�V���t�B�[���h�̈ʒu�̍������o��
	D3DXVECTOR3 posMtx = pos - m_pos;

	//���݃v���C���[�����u���b�N�ڂɂ��邩���m�F���鎮
	int			nMeshX = ((int)(posMtx.x)) / ((int)(m_fMeshWidth / m_nMeshX));
	int			nMeshZ = ((int)(posMtx.z)) / ((int)(m_fMeshDepth / m_nMeshZ)) * -1;

	if (nMeshX >= m_nMeshX || nMeshX < 0)
	{
		return false;
	}
	if (nMeshZ >= m_nMeshZ || nMeshZ < 0)
	{
		return false;
	}

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (posMtx.x < 0 || posMtx.x > pVtx[m_nMeshX].pos.x)
	{
		// ���_�o�b�t�@���A�����b�N
		m_pVtxBuff->Unlock();

		return false;
	}
	if (posMtx.z > 0 || posMtx.z > pVtx[m_nMeshX].pos.z)
	{
		// ���_�o�b�t�@���A�����b�N
		m_pVtxBuff->Unlock();

		return false;
	}

	//���݂̏�������Ă���u���b�N�̒��_�̏o����
	int	nMeshLU = nMeshX + nMeshZ * (m_nMeshX + 1);					//��@0�Ԗڂ̒��_
	int	nMeshRU = (nMeshX + 1) + nMeshZ * (m_nMeshX + 1);			//��@1�Ԗڂ̒��_
	int	nMeshLD = nMeshX + (nMeshZ + 1) * (m_nMeshX + 1);			//��@3�Ԗڂ̒��_
	int	nMeshRD = (nMeshX + 1) + (nMeshZ + 1) * (m_nMeshX + 1);		//��@4�Ԗڂ̒��_

	int nMostUnder, nUnder0, nUnder1;

	if (pVtx[nMeshLU].pos.y < pVtx[nMeshRU].pos.y) { nUnder0 = nMeshLU; }
	else { nUnder0 = nMeshRU; }

	if (pVtx[nMeshLD].pos.y < pVtx[nMeshRD].pos.y) { nUnder1 = nMeshLD; }
	else { nUnder1 = nMeshRD; }

	if (pVtx[nUnder0].pos.y < pVtx[nUnder1].pos.y) { nMostUnder = nUnder0; }
	else { nMostUnder = nUnder1; }

	if (pVtx[nMostUnder].pos.y + m_pos.y > pos.y + fMaxHeight + 10.0f)
	{
		// ���_�o�b�t�@���A�����b�N
		m_pVtxBuff->Unlock();

		return false;
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return true;
}