//=============================================================================
//
// mesh���� [mesh.cpp]
// Author : Ryo Sugimoto
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "NorMesh.h"
#include "DispEffect.h"
#include "renderer.h"
#include "manager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ECT_CROSS	(12.0f)
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
	float fRotY;

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
			case TYPE_CYLINDER:
				fUV_Y = 0.5f;
				fRotY = (1.0f / nCorss) * (float)nCntCorss;
				pVtx[0].pos = D3DXVECTOR3(
					sinf(fRotY * D3DX_PI * 2.0f) * 100.0f
					, nCntVertical * 50.0f
					, cosf(fRotY * D3DX_PI * 2.0f) * 100.0f);
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
//    * �V�����_�[�̃Z�b�g���� *
//==================================================================================================//1
void	CNorMesh::SetCylinder(float fLengthY, float fLengthXZU, float fLengthXZD, D3DXCOLOR col, D3DXVECTOR2 UV0, D3DXVECTOR2 UV1)
{
	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^
	WORD * pIdx;		//�C���f�b�N�X�f�[�^�ւ̃|�C���^
	float fRotY, fLength;
	float fValue = UV1.x - UV0.x;

	if (fLengthXZU < 0.0f) { fLengthXZU = 0.0f; }
	if (fLengthXZD < 0.0f) { fLengthXZD = 0.0f; }

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	//���_�o�b�t�@�����b�N�����_�f�[�^�ւ̃|�C���^���擾
												//���_�A�@���A�J���[�A�e�N�X�`���̒��_�ݒ�
	for (int nCntVertical = 0; nCntVertical < m_nVertical + 1; nCntVertical++)
	{//�c��
		fLength = (nCntVertical % 2 == 0 ? fLengthXZD : fLengthXZU);
		for (int nCntCorss = 0; nCntCorss < m_nCross + 1; nCntCorss++)
		{//����
			fRotY = (1.0f / m_nCross) * (float)nCntCorss;
			pVtx[0].pos = D3DXVECTOR3(
				sinf(fRotY * D3DX_PI * 2.0f) * fLength
				, nCntVertical * fLengthY
				, cosf(fRotY * D3DX_PI * 2.0f) * fLength);
			pVtx[0].col = col;
			if (nCntVertical % 2 == 0) { pVtx[0].tex = D3DXVECTOR2(UV0.x + fValue * nCntCorss, UV0.y); }
			else { pVtx[0].tex = D3DXVECTOR2(UV0.x + fValue * nCntCorss, UV1.y); }

			pVtx += 1;
		}
	}

	m_pVtxBuff->Unlock();	//���_�o�b�t�@�̃A�����b�N����

							//�C���f�b�N�X�o�b�t�@�̃��b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

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
	m_nNumDraw = 1;
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

	for (int nCnt = 0; nCnt < m_nNumDraw; nCnt++)
	{// �|���S���̕`��
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVertex, 0, m_nNumPolygon);
	}
	pDevice->SetTexture(0, NULL);
}

//=============================================================================
// �V�����_�[�̐�������
//=============================================================================
CCylinder	*CCylinder::Create(D3DXVECTOR3 &pos, D3DXVECTOR3 &rot, TYPE type)
{
	CCylinder *pEffect = NULL;
	pEffect = new CCylinder;

	if (pEffect != NULL)
	{
		pEffect->Init();
		pEffect->Set(pos, rot, type);
	}

	return pEffect;
}
//=============================================================================
// �V�����_�[�̐ݒ菈��
//=============================================================================
void	CCylinder::Set(D3DXVECTOR3 &pos, D3DXVECTOR3 &rot, TYPE type)
{
	CNorMesh::Set(CNorMesh::TYPE_CYLINDER, pos, 1, (int)ECT_CROSS, false);

	BindTexture(CDispEffect::GetpTexAll(CDispEffect::TEX_Warning));
	STATE state = STATE_NORMAL;
	m_Type = type;
	m_pPos = &pos;
	m_pRot = &rot;
	switch (type)
	{
	case TYPE_BOOST:
		m_AddPos = D3DXVECTOR3(0.0f, 20.0f, -10.0f);
		m_AddRot.x = D3DX_PI * 0.5f;
		state = STATE_SPREAD;
		m_fPlusA = 0.035f;
		m_fCntState = 3.0f;
		m_fLengthXZ[0] = -30.0f;
		m_fLengthXZ[1] = 5.0f;
		m_fLengthY = 30.0f;
		GetnNumDraw() = 1;
		break;
	case TYPE_RARE:
		state = STATE_NORMAL;
		m_col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		m_fPlusA = 0.0f;
		m_fCntState = 0.0f;
		m_fLengthY = 15.0f;
		GetnNumDraw() = 1;
		m_fLengthXZ[0] = 10.0f; m_fLengthXZ[1] = 10.0f;
		break;
	case TYPE_HATK:
		state = STATE_FADE;
		m_col = D3DXCOLOR(1.0f, 0.1f, 0.02f, 1.0f);
		m_fPlusA = 0.03f;
		m_fCntState = 3.0f;
		m_fLengthY = 60.0f;
		GetnNumDraw() = 3;
		m_fLengthXZ[0] = 25.0f;
		m_fLengthXZ[1] = 10.0f;
		break;
	case TYPE_EATK:
	case TYPE_HATK_S:
		state = STATE_END;
		m_col = D3DXCOLOR(1.0f, 0.1f, 0.02f, 1.0f);
		m_fPlusA = 0.03f;
		m_fCntState = 3.0f;
		m_fLengthY = 80.0f;
		GetnNumDraw() = 3;
		switch (type)
		{
		case TYPE_EATK:		m_fLengthXZ[0] = 120.0f;	m_fLengthXZ[1] = 100.0f;	break;
		case TYPE_HATK_S:	m_fLengthXZ[0] = 120.0f;	m_fLengthXZ[1] = 100.0f;	break;
		}
		break;
	case TYPE_HATK_SC:
	case TYPE_HATK_SH:
		state = STATE_TIME;
		m_col = D3DXCOLOR(1.0f, 0.1f, 0.02f, 1.0f);
		m_fPlusA = 0.03f;
		m_fCntState = ATTACK_TIME * 60;
		m_pPos = NULL;	m_pRot = NULL;
		switch (type)
		{
		case TYPE_HATK_SC:
			m_fLengthY = 1000.0f;
			GetnNumDraw() = 1;
			m_fLengthXZ[0] = 30.0f;	m_fLengthXZ[1] = 30.0f;
			CCylinder::Create(pos, rot, CCylinder::TYPE_HATK_SH);
			break;
		case TYPE_HATK_SH:
			m_fLengthY = 25.0f;
			GetnNumDraw() = 3;
			m_fLengthXZ[0] = FALL_CHICK_RANGE * 0.75f + 10.0f;	m_fLengthXZ[1] = FALL_CHICK_RANGE * 0.75f;
			break;
		}
		break;
	case TYPE_HDEF:
		state = STATE_TIME;
		m_col = D3DXCOLOR(0.02f, 0.1f, 1.0f, 1.0f);
		m_fPlusA = 0.03f;
		m_fCntState = DISTIME;
		m_fLengthY = 25.0f;
		GetnNumDraw() = 3;
		m_fLengthXZ[0] = ANNOY_RANGE + 10.0f; m_fLengthXZ[1] = ANNOY_RANGE;
		break;
	}
	SetCylinder(m_fLengthY, m_fLengthXZ[0], m_fLengthXZ[1]);
	SetState(state);
}
//=============================================================================
// �V�����_�[�̏�Ԑݒ菈��
//=============================================================================
void	CCylinder::SetState(STATE state)	//��Ԑݒ菈��
{
	m_State = state;
	switch (state)
	{
	case STATE_END:	m_pPos = NULL; m_pRot = NULL;	break;
	}
}
//=============================================================================
// �V�����_�[�̐ݒ菈��
//=============================================================================
HRESULT CCylinder::Init(void)
{
	CNorMesh::Init();
	m_col = INIT_COL;
	m_AddPos = INIT_VECTOR;
	m_AddRot = INIT_VECTOR;
	m_State = STATE_END;
	m_fUV_X = 1.0f / 8.0f;
	m_fLengthXZ[0] = 100.0f;
	m_fLengthXZ[1] = 100.0f;
	m_fLengthY = 10.0f;
	m_fPlusA = 0.0f;
	m_fCntState = 0.0f;
	m_fCntTime = 0.0f;

	m_nPtn = 0;
	m_nSpeed = 5;
	m_nCntSpd = 0;
	return S_OK;
}
//=============================================================================
// �V�����_�[�̐ݒ菈��
//=============================================================================
void	CCylinder::Update(void)
{
	m_col.a -= m_fPlusA;
	if (m_col.a < 0.0f) { Uninit(); return; }

	m_nCntSpd++;
	if (m_nCntSpd > m_nSpeed)
	{
		m_nCntSpd = 0;
		m_nPtn++;
	}

	if (m_Type == TYPE_HATK_SC || m_Type == TYPE_HATK_SH)
	{
		int nDestRank, nChar;
		int nTarget = 99;
		bool bGoal;
		D3DXVECTOR3 Goal;
		for (int nCntChar = 0; nCntChar < MAX_MEMBER; nCntChar++)
		{// 1�ʂ̂��������
			bGoal = CGame::GetPlayer()[nCntChar]->GetGoal();
			nDestRank = CGame::GetRanking(nCntChar);

			if (!bGoal && nTarget > nDestRank)
			{//�S�[�����Ă��Ȃ� && �^�[�Q�b�g��菇�ʂ���
				nTarget = nDestRank;
				nChar = nCntChar;
			}
		}
		Goal = CGame::GetPlayer()[nChar]->Getpos();
		GetPos() += (Goal - GetPos()) * 0.5f;
	}

	switch (m_State)
	{
	case STATE_NORMAL:
		SetCylinder(m_fLengthY, m_fLengthXZ[0], m_fLengthXZ[1], m_col
			, D3DXVECTOR2((1.0f / (ECT_CROSS + 2.0f)) * m_nPtn, 1.0f), D3DXVECTOR2((1.0f / (ECT_CROSS + 2.0f)) * (m_nPtn + 1), 0.0f));
		break;
	case STATE_SPREAD:	//�L����
		m_fLengthXZ[0] += powf(m_fCntState, 2) * 0.5f;
		m_fLengthXZ[1] += powf(m_fCntState, 2);

		if (m_fCntState < 1.0f) { m_fCntState -= m_fPlusA * 0.1f; }
		else { m_fCntState -= m_fPlusA; }
		if (m_fCntState < 0.1f) { m_fCntState = 0.1f; }
		SetCylinder(m_fLengthY, m_fLengthXZ[0], m_fLengthXZ[1], m_col
			, D3DXVECTOR2((1.0f / (ECT_CROSS + 2.0f)) * m_nPtn, 0.0f), D3DXVECTOR2((1.0f / (ECT_CROSS + 2.0f)) * (m_nPtn + 1), 0.5f));
		break;
	case STATE_FADE:	//�����x�X�V
	case STATE_END:
		SetCylinder(m_fLengthY, m_fLengthXZ[0], m_fLengthXZ[1], m_col
			, D3DXVECTOR2((1.0f / (ECT_CROSS + 2.0f)) * m_nPtn, 1.0f), D3DXVECTOR2((1.0f / (ECT_CROSS + 2.0f)) * (m_nPtn + 1), 0.0f));
		break;
	case STATE_TIME:	//�����x�X�V
		m_fCntTime += 1.0f;
		m_col.a = 1.0f;
		if (m_fCntTime > m_fCntState) { m_State = STATE_END; }
		SetCylinder(m_fLengthY, m_fLengthXZ[0], m_fLengthXZ[1], m_col
			, D3DXVECTOR2((1.0f / (ECT_CROSS + 2.0f)) * m_nPtn, 1.0f), D3DXVECTOR2((1.0f / (ECT_CROSS + 2.0f)) * (m_nPtn + 1), 0.0f));
		break;
	}

	//�ʒu�X�V
	if (m_pPos != NULL) { GetPos() = *m_pPos + m_AddPos; }
	if (m_pRot != NULL) { GetRot() = D3DXVECTOR3(0.0f, m_pRot->y, 0.0f) + m_AddRot; }
	SetMtx();
}
//=============================================================================
// �V�����_�[�̐ݒ菈��
//=============================================================================
void	CCylinder::Uninit(void)
{
	CNorMesh::Uninit();
	CScene::Release();
}
//=============================================================================
// �V�����_�[�̐ݒ菈��
//=============================================================================
void	CCylinder::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�A���t�@�e�X�g����(�������ɂ��摜�̐؂��茻�ۂ�L���ɂ���)
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 30);//REF = Reference:�Q��
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);//FUNC = Function:�@�\ //GREATER = �`���傫��
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);			// ���ʕ`��
	C2D::DrawPrepare(C2D::DRAW_TYPE_ADD, pDevice);

	CNorMesh::Draw();

	C2D::DrawPrepare(C2D::DRAW_TYPE_NORMAL, pDevice);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);    // ���ʂ��J�����O
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);			// �ʏ�ʕ`��

}