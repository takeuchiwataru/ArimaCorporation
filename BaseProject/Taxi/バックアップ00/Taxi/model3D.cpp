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

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define LENGTH_BLOW		(1000.0f)		//������є����L���ɂ��鋗��
#define LENGTH_SMALLOBJ	(6000.0f)		//�������I�u�W�F�N�g��`�悷��͈�
#define LENGTH_HIGHTOBJ	(8000.0f)		//�����I�u�W�F�N�g��`�悷��͈�
#define STOP_MOVE		(0.05f)			//�ړ����~����͈�
#define DELETE_TIME		(60 * 5)		//��������

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

//===============================================================================
//�@�I�[�o�[���[�h�R���X�g���N�^
//===============================================================================
CModel3D::CModel3D(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pMeshObject = NULL;					//���b�V�����ւ̃|�C���^
	m_pBuffMatObject = NULL;				//�}�e���A���̏��ւ̃|�C���^
	m_nNumMatObject = 0;					//�}�e���A���̏��
	m_mtxWorldObject;						//���[���h�}�g���b�N�X
	m_VtxMinModel, m_VtxMaxModel;			//���f���̍ŏ��l�E�ő�l
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);//�傫��
	m_VecAxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fValueRot = 0.0f;
	m_fMass = 2.0f;
	m_fRotY = 0.0f;
	m_blowOff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bcolChange = false;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �J���[
	m_bRoll = true;
	m_bStateRoll = false;
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	for (int nCntCorner = 0; nCntCorner < VTX_CORNER_MAX; nCntCorner++)
	{// �����蔻��p�{�b�N�X�̊p�W�̒��_���W
		m_aVtxCornerPos[nCntCorner] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CModel3D::~CModel3D() {}

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

	// �{�b�N�X�̊p�W�̒��_���W
	m_aVtxCornerPos[VTX_CORNER_LDF] = D3DXVECTOR3(m_VtxMinModel.x, m_VtxMinModel.y, m_VtxMinModel.z);
	m_aVtxCornerPos[VTX_CORNER_LUF] = D3DXVECTOR3(m_VtxMinModel.x, m_VtxMaxModel.y, m_VtxMinModel.z);
	m_aVtxCornerPos[VTX_CORNER_RDF] = D3DXVECTOR3(m_VtxMaxModel.x, m_VtxMinModel.y, m_VtxMinModel.z);
	m_aVtxCornerPos[VTX_CORNER_RUF] = D3DXVECTOR3(m_VtxMaxModel.x, m_VtxMaxModel.y, m_VtxMinModel.z);
	m_aVtxCornerPos[VTX_CORNER_RDB] = D3DXVECTOR3(m_VtxMaxModel.x, m_VtxMinModel.y, m_VtxMaxModel.z);
	m_aVtxCornerPos[VTX_CORNER_RUB] = D3DXVECTOR3(m_VtxMaxModel.x, m_VtxMaxModel.y, m_VtxMaxModel.z);
	m_aVtxCornerPos[VTX_CORNER_LDB] = D3DXVECTOR3(m_VtxMinModel.x, m_VtxMinModel.y, m_VtxMaxModel.z);
	m_aVtxCornerPos[VTX_CORNER_LUB] = D3DXVECTOR3(m_VtxMinModel.x, m_VtxMaxModel.y, m_VtxMaxModel.z);

	m_nIdxCornerPosMinY = VTX_CORNER_LDF;

	m_Rot.y = 0.0f;

	for (int nCntVtx = 0; nCntVtx < VTX_CORNER_MAX; nCntVtx++)
	{
		m_aVtxPosStorage[nCntVtx] = m_aVtxCornerPos[nCntVtx];
	}

	//�ϐ��̏�����
	m_bUpdate = true;			// �A�b�v�f�[�g��ʂ����ǂ���
	m_bUpdateBlow = true;		// ������΂����������邩�ǂ���
	m_bBlow = false;			// ������΂������ǂ���
	m_bDelete = false;			// �j�����邩�ǂ���
	m_nCountTime = 0;			// �J�E���^�[
	m_bSmallObjDraw = false;	// �������I�u�W�F�N�g�̕`��t���O
	m_bHightObjDraw = false;	// �����I�u�W�F�N�g�̕`��t���O
	m_bOnlyLengthDraw = false;	// �`�拗�����������߂�I�u�W�F�N�g�t���O
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
		SetDraw(CGame::GetGameCamera()->Clipping(m_VtxMinModel + m_Pos, m_VtxMaxModel + m_Pos));
	}
	else if (CManager::MODE_TUTORIAL == mode)
	{
		SetDraw(CTutorial::GetCamera()->Clipping(m_VtxMinModel + m_Pos, m_VtxMaxModel + m_Pos));
	}

	if (m_bBlow)
	{//������΂�����ԂȂ�
		m_nCountTime++;

		//��莞�Ԃ�������j����Ԃɂ���
		if ((m_nCountTime % DELETE_TIME) == 0) { m_bDelete = true; }
	}

	if ((false == GetDraw()) || (!m_bUpdate)) { return; }		//�`�悵�Ȃ���Ԃ�������

	if ((CManager::MODE_GAME == mode) || (CManager::MODE_TUTORIAL == mode))
	{
		//�������������
		if (LengthJudgment() == true) { return; }

		//�X�����͏�����ʂ��Ȃ�
		if (m_bHightObjDraw == true) { return; }

		//�����ʂ�����
		if ((!m_bUpdateBlow) &&											//���������ȏ�
			(m_Move.x < STOP_MOVE) && (m_Move.x > -STOP_MOVE) &&		// X�̈ړ��ʂ����l�ȉ�
			(m_Move.y < STOP_MOVE) && (m_Move.y > -STOP_MOVE) &&		// Y�̈ړ��l�����l�ȉ�
			(m_Move.z < STOP_MOVE) && (m_Move.z > -STOP_MOVE) &&		// Z�̈ړ��l�����l�ȉ�
			(m_blowOff.x < STOP_MOVE) && (m_blowOff.x > -STOP_MOVE) &&	// X�̐�����ԗʂ����l�ȉ�
			(m_blowOff.z < STOP_MOVE) && (m_blowOff.z > -STOP_MOVE))	// Z�̐�����ԗʂ����l�ȉ�
		{
			return;
		}

		// �ʒu�̕ۑ�
		D3DXVECTOR3 posOld = m_Pos;

		// �d��
		m_Move.y -= cosf(0) * 0.5f;

		if (m_bRoll && m_bBlow)
		{// �]���邩�ǂ���
			if (D3DXVec3Length(&m_blowOff) > m_fMass && m_bStateRoll)
			{
				// �Փˎ��̊p�x�v�Z
				RotCalculation();

				// ���_���W�̍Čv�Z
				RecalculationVtx();
			}

			// �n�ʂɂ����Ƃ�
			Landing(posOld);

			m_Pos += m_Move;
			m_Pos += m_blowOff;
		}

		// �ړ��ʂ̊���
		m_Move.x += (0 - m_Move.x) * 0.04f;
		m_Move.z += (0 - m_Move.z) * 0.04f;

		// ������Ԉړ��ʂ̊���
		m_blowOff.x += (0 - m_blowOff.x) * 0.04f;
		m_blowOff.z += (0 - m_blowOff.z) * 0.04f;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CModel3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxSpin, mtxScale;	//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;							//���݂̃}�e���A����ۑ�
	D3DXMATERIAL *pMat;								//�}�e���A���f�[�^�ւ̃|�C���^

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

	// �g��k���s��̍쐬
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&m_mtxWorldObject, &m_mtxWorldObject, &mtxScale);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorldObject, &m_mtxWorldObject, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorldObject);

	if (m_pBuffMatObject != NULL)
	{
		// ���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)m_pBuffMatObject->GetBufferPointer();
		float fcol_a;
		float fcol_r;
		float fcol_g;
		float fcol_b;

		for (int nCntMat = 0; nCntMat < (int)m_nNumMatObject; nCntMat++)
		{
			if (m_bcolChange == true)
			{	// �J���[�ύX(�����x)
				fcol_a = pMat[nCntMat].MatD3D.Diffuse.a;
				pMat[nCntMat].MatD3D.Diffuse.a = m_col.a;
				fcol_r = pMat[nCntMat].MatD3D.Diffuse.r;
				pMat[nCntMat].MatD3D.Diffuse.r = m_col.r;
				fcol_g = pMat[nCntMat].MatD3D.Diffuse.g;
				pMat[nCntMat].MatD3D.Diffuse.g = m_col.g;
				fcol_b = pMat[nCntMat].MatD3D.Diffuse.b;
				pMat[nCntMat].MatD3D.Diffuse.b = m_col.b;
			}

			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//�e�N�X�`��������ꍇ
				pDevice->SetTexture(0, m_pMeshTextures);
			}
			else
			{	// �e�N�X�`�����g���Ă��Ȃ�
				pDevice->SetTexture(0, NULL);
			}
			//�I�u�W�F�N�g(�p�[�c)�̕`��
			m_pMeshObject->DrawSubset(nCntMat);

			// �J���[�ύX
			if (m_bcolChange == true)
			{
				pMat[nCntMat].MatD3D.Diffuse.a = fcol_a;
				pMat[nCntMat].MatD3D.Diffuse.r = fcol_r;
				pMat[nCntMat].MatD3D.Diffuse.g = fcol_g;
				pMat[nCntMat].MatD3D.Diffuse.b = fcol_b;
			}
		}

		// �}�e���A�����f�t�H���g�ɖ߂�
		pDevice->SetMaterial(&matDef);

		m_bcolChange = false;	// �ύX�I��
	}
}
//===============================================================================
//�@�l��n��
//===============================================================================
void CModel3D::BindModel(LPD3DXMESH pMeshObject, LPD3DXBUFFER pBuffMatObject, DWORD nNumMatObject, LPDIRECT3DTEXTURE9 pMeshTextures, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin)
{
	m_pMeshObject = pMeshObject;
	m_pBuffMatObject = pBuffMatObject;
	m_nNumMatObject = nNumMatObject;
	m_pMeshTextures = pMeshTextures;
	m_VtxMaxModel = VtxMax;
	m_VtxMinModel = VtxMin;
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
// �p�x�̏�����
//===============================================================================
void CModel3D::InitRot(void)
{
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �N�H�[�^�j�I���̐��l�����Z�b�g
	m_VecAxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fValueRot = 0.0f;

	D3DXMatrixIdentity(&m_mtxRot);
	m_quat.x = 0.0f;
	m_quat.z = 0.0f;
}

//===============================================================================
// ���_�̍ő�l�ƍŏ��l���Čv�Z
//===============================================================================
void CModel3D::RecalculationVtx(void)
{
	D3DXMATRIX mtxRot, mtxTrans, mtxScale, mtxWorld, mtx;	// ���[���h�}�g���b�N�X�̕ۑ��p
	m_VtxMaxModel = D3DXVECTOR3(-10000, -10000, -10000);	// �ő�l
	m_VtxMinModel = D3DXVECTOR3(10000, 10000, 10000);		// �ŏ��l

	D3DXMatrixIdentity(&mtx);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxRot);

	// �N�H�[�^�j�I���Ɗ|�����킹��
	D3DXMatrixMultiply(&mtx, &mtx, &m_mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxTrans);

	// ��]�p�}�g���N�X
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);

	for (int nCntVtxCorner = 0; nCntVtxCorner < VTX_CORNER_MAX; nCntVtxCorner++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxWorld);

		// ��]�𔽉f
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, m_aVtxPosStorage[nCntVtxCorner].x, m_aVtxPosStorage[nCntVtxCorner].y, m_aVtxPosStorage[nCntVtxCorner].z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtx);

		// �{�b�N�X�̊p�W�̒��_���W
		m_aVtxCornerPos[nCntVtxCorner] = D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);

		/*		�ő�l		*/
		if (m_aVtxCornerPos[nCntVtxCorner].x > m_VtxMaxModel.x)
		{// X���W�̔�r
			m_VtxMaxModel.x = m_aVtxCornerPos[nCntVtxCorner].x;
		}
		if (m_aVtxCornerPos[nCntVtxCorner].y > m_VtxMaxModel.y)
		{// Y���W�̔�r
			m_VtxMaxModel.y = m_aVtxCornerPos[nCntVtxCorner].y;
		}
		if (m_aVtxCornerPos[nCntVtxCorner].z > m_VtxMaxModel.z)
		{// Z���W�̔�r
			m_VtxMaxModel.z = m_aVtxCornerPos[nCntVtxCorner].z;
		}

		/*		�ŏ��l		*/
		if (m_aVtxCornerPos[nCntVtxCorner].x < m_VtxMinModel.x)
		{// X���W�̔�r
			m_VtxMinModel.x = m_aVtxCornerPos[nCntVtxCorner].x;
		}
		if (m_aVtxCornerPos[nCntVtxCorner].y < m_VtxMinModel.y)
		{// Y���W�̔�r
			m_VtxMinModel.y = m_aVtxCornerPos[nCntVtxCorner].y;
			m_nIdxCornerPosMinY = nCntVtxCorner;
		}
		if (m_aVtxCornerPos[nCntVtxCorner].z < m_VtxMinModel.z)
		{// Z���W�̔�r
			m_VtxMinModel.z = m_aVtxCornerPos[nCntVtxCorner].z;
		}
	}
}

//===============================================================================
// ������΂�����
//===============================================================================
D3DXVECTOR3 CModel3D::BlowOff(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fHeavy)
{
	if (m_bUpdate)
	{
		D3DXVECTOR3 newV1, newV2;		// �Փˌ�̑��x
		float fElast = 1.0f;			// �e���W��

		// ����̑��x
		newV1.x = (-move.x + m_Move.x) * (1.0f + fElast) / (fHeavy / m_fMass + 1.0f) + move.x;
		newV1.z = (-move.z + m_Move.z) * (1.0f + fElast) / (fHeavy / m_fMass + 1.0f) + move.z;

		// �����̑��x
		newV2.x = (-m_Move.x + move.x) * (1.0f + fElast) / (m_fMass / fHeavy + 1.0f) + m_Move.x;
		newV2.z = (-m_Move.z + move.z) * (1.0f + fElast) / (m_fMass / fHeavy + 1.0f) + m_Move.z;
		if (m_bRoll == true)
		{
			// XZ���ւ̈ړ���
			m_blowOff.x = newV2.x;
			m_blowOff.z = newV2.z;
		}
		else
		{
			// XZ���ւ̈ړ���
			m_blowOff.x = -newV2.x;
			m_blowOff.z = -newV2.z;
		}

		// �ړ��ʂ����W�ɉ��Z
		m_Pos += m_blowOff;

		m_bBlow = true;			//������΂�����Ԃɂ���
		m_bStateRoll = true;	// �]�����Ă����Ԃɂ���

		return D3DXVECTOR3(newV1.x, move.y, newV1.z);
	}

	return move;
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
	vtxMaxObject.x += m_VtxMaxModel.x * m_scale.x;
	vtxMaxObject.y += m_VtxMaxModel.y * m_scale.y;
	vtxMaxObject.z += m_VtxMaxModel.z * m_scale.z;

	// ���_���W�̍ŏ��l�����߂�
	vtxMinObject.x += m_VtxMinModel.x * m_scale.x;
	vtxMinObject.y += m_VtxMinModel.y * m_scale.y;
	vtxMinObject.z += m_VtxMinModel.z * m_scale.z;

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

		// Y���̉�]���������߂�
		m_fRotY = CrossingAngle(vec0, move);
	}

	return bHit;
}

//===============================================================================
// �������̏���
//===============================================================================
void CModel3D::Landing(D3DXVECTOR3 posOld)
{
	if (m_bBlow)
	{// �������Ƃ�
	//�v���C�I���e�B�[�`�F�b�N
		CScene *pScene = CScene::GetTop(MESH_PRIOTITY);

		//NULL�`�F�b�N
		while (pScene != NULL)
		{
			//Update��Uninit����Ă��܂��ꍇ�@Next��������\�������邩��Next�Ƀf�[�^���c���Ă���
			CScene *pSceneNext = pScene->GetNext();

			if (pScene->GetObjType() == OBJTYPE_GROUND)
			{//�^�C�v���n�ʂ�������
				CMeshField *pField = (CMeshField*)pScene;
				if (pField->OnField(m_Pos, CModel3D::GetVtxMax().y))
				{// �n�ʂɏ悹��
					float fHeight = pField->GetHeightMesh(m_Pos);

					if (fHeight > m_Pos.y + CModel3D::GetVtxMin().y && fHeight < posOld.y + CModel3D::GetVtxMin().y)
					{// �󒆂���n�ʂɒ������Ƃ�
						m_Move.y = fHeight - (m_Pos.y + CModel3D::GetVtxMin().y);
						m_Pos.y = fHeight - CModel3D::GetVtxMin().y;
					}
					else if (fHeight > m_Pos.y + CModel3D::GetVtxMin().y && fHeight >= posOld.y + CModel3D::GetVtxMin().y)
					{// �n�ʂɖ��܂��Ă���Ƃ�
						m_Move.y = 0.0f;
						m_Pos.y = fHeight - CModel3D::GetVtxMin().y;
					}

					break;
				}
			}

			//Next�Ɏ���Scene������
			pScene = pSceneNext;
		}
	}
}

//===============================================================================
// �Q�̃x�N�g���̂Ȃ��p�x
//===============================================================================
float CModel3D::CrossingAngle(D3DXVECTOR3 vec0, D3DXVECTOR3 vec1)
{
	float fAngle;

	// �Q�̃x�N�g���̂Ȃ��p�����߂�
	fAngle = (vec0.x * vec1.x) + (vec0.y * vec1.y) + (vec0.z * vec1.z);
	fAngle = fAngle / (sqrtf(powf(vec0.x, 2) + powf(vec0.y, 2) + powf(vec0.z, 2)) * (sqrtf(powf(vec1.x, 2) + powf(vec1.y, 2) + powf(vec1.z, 2))));

	return acosf(fAngle);
}

//===============================================================================
// �Փˎ��̊p�x�v�Z
//===============================================================================
void CModel3D::RotCalculation(void)
{
	if (m_blowOff.x > m_fMass || m_blowOff.x < -m_fMass || m_blowOff.z > m_fMass || m_blowOff.z < -m_fMass)
	{// �ړ��ʂ���]���ɂ���
		m_VecAxis = D3DXVECTOR3(m_blowOff.z, m_fRotY, -m_blowOff.x);

		// �ړ��ʃx�N�g���̑傫���̎擾
		float fVecLength = D3DXVec3Length(&m_blowOff);

		// ��]�p���グ��
		m_fValueRot += fVecLength * 0.01f;
	}
	else if (m_bBlow)
	{// ���ȉ��Ŋp�x��␳����
		D3DXVECTOR3 length0 = m_aVtxCornerPos[(m_nIdxCornerPosMinY + 2) % VTX_CORNER_MAX] - m_aVtxCornerPos[m_nIdxCornerPosMinY];
		D3DXVECTOR3 length1 = m_aVtxCornerPos[(m_nIdxCornerPosMinY + (VTX_CORNER_MAX - 2)) % VTX_CORNER_MAX] - m_aVtxCornerPos[m_nIdxCornerPosMinY];

		float fLength0 = sqrtf(powf(length0.x, 2.0f) + powf(length0.z, 2.0f));
		float fLength1 = sqrtf(powf(length1.x, 2.0f) + powf(length1.z, 2.0f));

		D3DXVECTOR3 opposition;
		if (fLength0 > fLength1 && 0 == m_nIdxCornerPosMinY % 2)
		{// �E������
			opposition = m_aVtxCornerPos[(m_nIdxCornerPosMinY + 3) % VTX_CORNER_MAX];
		}
		else if (fLength0 > fLength1 && 1 == m_nIdxCornerPosMinY % 2)
		{// �E���㕔
			opposition = m_aVtxCornerPos[(m_nIdxCornerPosMinY + 1) % VTX_CORNER_MAX];
		}
		else if (fLength0 < fLength1 && 0 == m_nIdxCornerPosMinY % 2)
		{// ��������
			opposition = m_aVtxCornerPos[(m_nIdxCornerPosMinY + (VTX_CORNER_MAX - 1)) % VTX_CORNER_MAX];
		}
		else if (fLength0 < fLength1 && 1 == m_nIdxCornerPosMinY % 2)
		{// �����㕔
			opposition = m_aVtxCornerPos[(m_nIdxCornerPosMinY + (VTX_CORNER_MAX - 3)) % VTX_CORNER_MAX];
		}
		else
		{// ��O
			opposition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		if (opposition.y > m_aVtxCornerPos[m_nIdxCornerPosMinY].y)
		{
			// ���������߂�
			float fHeight = opposition.y - m_aVtxCornerPos[m_nIdxCornerPosMinY].y;

			// ��ӂ̒��������߂�
			float fBottomX = opposition.x - m_aVtxCornerPos[m_nIdxCornerPosMinY].x;
			float fBottomZ = opposition.z - m_aVtxCornerPos[m_nIdxCornerPosMinY].z;

			// �p�x�����߂�i���_�͐^�񒆂Ȃ̂Ŕ����̒l�ɂ���j
			float fAngleX = atanf(fHeight / fBottomX) * 0.5f;
			float fAngleZ = atanf(fHeight / fBottomZ) * 0.5f;

			/*					  */
			/*			X��		  */
			/*					  */
			// ���������߂�
			float fDiff = fAngleX - m_Rot.x;

			if (fDiff > D3DX_PI) { fDiff -= D3DX_PI * 2.0f; }
			if (fDiff < -D3DX_PI) { fDiff += D3DX_PI * 2.0f; }

			// �p�x��ړI�ɋ߂Â���
			m_Rot.x += fDiff * 0.1f;

			if (m_Rot.x > D3DX_PI) { m_Rot.x -= D3DX_PI * 2.0f; }
			if (m_Rot.x < -D3DX_PI) { m_Rot.x += D3DX_PI * 2.0f; }

			/*					  */
			/*			Z��		  */
			/*					  */
			// ���������߂�
			fDiff = fAngleZ - m_Rot.z;

			if (fDiff > D3DX_PI) { fDiff -= D3DX_PI * 2.0f; }
			if (fDiff < -D3DX_PI) { fDiff += D3DX_PI * 2.0f; }

			// �p�x��ړI�ɋ߂Â���
			m_Rot.z += fDiff * 0.1f;

			if (m_Rot.z > D3DX_PI) { m_Rot.z -= D3DX_PI * 2.0f; }
			if (m_Rot.z < -D3DX_PI) { m_Rot.z += D3DX_PI * 2.0f; }
		}

		if (m_bStateRoll)
		{// �]�����Ă����Ԃ̃t���O��������
			m_bStateRoll = false;
		}
	}
}

//=============================================================================
// �v���C���[�Ƃ̋�������
//=============================================================================
bool CModel3D::LengthJudgment(void)
{
	bool bReturn = false;	//�������s�t���O

	//���������߂�
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();
	D3DXVECTOR3 Distance = PlayerPos - m_Pos;
	m_fLength = sqrtf((Distance.x * Distance.x) + (Distance.z * Distance.z));

	//������΂��������X�V��Ԃɂ���
	m_bUpdateBlow = true;

	//���苗���ȏゾ������X�V���Ȃ���Ԃɂ���
	if (m_fLength > LENGTH_BLOW) { m_bUpdateBlow = false; }

	if ((m_fLength > LENGTH_SMALLOBJ) && (m_bSmallObjDraw == true))
	{//���ȏ�̋�����������`�悵�Ȃ���Ԃɂ���
		SetDraw(false);		//�`�悵�Ȃ���Ԃɂ���
		bReturn = true;
	}
	else if ((m_fLength > LENGTH_HIGHTOBJ) && (m_bHightObjDraw == true))
	{
		SetDraw(false);
		bReturn = true;
	}
	else if (m_bOnlyLengthDraw)
	{
		bReturn = true;
	}

	return bReturn;
}

//=============================================================================
// �����蔻��{�b�N�X�̊g�k
//=============================================================================
void CModel3D::ScaleVtxCornerPos(D3DXVECTOR3 scale)
{
	// �l��ۑ�
	m_scale = scale;
}