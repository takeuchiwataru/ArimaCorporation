//=============================================================================
//
// NPC�Ԃ̏��� [car.cpp]
// Author : YUTARO�@ABE
//
//=============================================================================
#include "scene.h"
#include "model3D.h"
#include "manager.h"
#include "car.h"
#include "meshfield.h"
#include "player.h"
#include "fade.h"
#include "car_point.h"
#include "game.h"
#include "policelamp.h"
#include "customer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �y������
#define MODEL_NAME_00 	"data\\MODEL\\NPC\\00_CarRed.x"				//	�ǂݍ��ރ��f���t�@�C��
#define MODEL_NAME_01 	"data\\MODEL\\NPC\\01_CarBlue.x"			//	�ǂݍ��ރ��f���t�@�C��
#define MODEL_NAME_02 	"data\\MODEL\\NPC\\02_CarGreen.x"			//	�ǂݍ��ރ��f���t�@�C��
// �X�|�[�c�J�[
#define MODEL_NAME_10 	"data\\MODEL\\NPC\\10_CarRed.x"				//	�ǂݍ��ރ��f���t�@�C��
#define MODEL_NAME_11 	"data\\MODEL\\NPC\\11_CarBlue.x"			//	�ǂݍ��ރ��f���t�@�C��
#define MODEL_NAME_12	"data\\MODEL\\NPC\\12_CarGreen.x"			//	�ǂݍ��ރ��f���t�@�C��
// �o�X
#define MODEL_NAME_20	"data\\MODEL\\NPC\\20_BusRed.x"				//	�ǂݍ��ރ��f���t�@�C��
#define MODEL_NAME_21	"data\\MODEL\\NPC\\21_BusBlue.x"			//	�ǂݍ��ރ��f���t�@�C��
#define MODEL_NAME_22	"data\\MODEL\\NPC\\22_BusGreen.x"			//	�ǂݍ��ރ��f���t�@�C��
// �x�@
#define MODEL_NAME_99 	"data\\MODEL\\NPC\\99_Police.x"				//	�ǂݍ��ރ��f���t�@�C��
// �e�N�X�`��
#define TEXTURE_NAME_1	"data\\TEXTURE\\modeltex\\carup.jpg"		//	�ǂݍ��ރe�N�X�`���t�@�C��

// �Ԃ̒���
#define CAR_DROW			 (8500.0f)	// �Ԃ̕`�悷��͈�
#define CAR_POINT_LAST		 (m_nIndexPoint[CGame::GetCarRoute().nPointCount[nRoute] - 1])	//	�Ō�̃|�C���g
#define CAR_SPEED			 (4.75f)	// NPC�Ԃ̑��x
#define CAR_DEPTH			 (10)		// NPC�Ԃ̕������p
#define CAR_HEIGHT			 (100.0f)	// NPC�Ԃ̔w�̍���
#define CAR_POINT_LENGTH	 (75)		// NPC�ԂƂ̃|�C���g�̋���
#define CAR_PLAYER_LENGTH	 (500)		// �x�@�ƃv���C���[�̋���
//	�J�[�u�̒�������l
#define CURVE_LENGTH		 (150)		// �J�[�u���鎞�͈̔̓`�F�b�N����[����POINT]
#define CURVE_LENGTH_NEXT	 (15)		// �J�[�u���鎞�͈̔̓`�F�b�N����[���Ԓn�_�b�b�ŏI�n�_]
#define LAST_CURVE_LENGTH	 (15)		// �ŏI�n�_���玟�̃|�C���g�Ɍ������͈̓`�F�b�N����
#define CURVE_LENGTH_SWITCH	 (1000)		// �J�[�u�̎�ނ̐؂�ւ�����
#define CURVE_SWITCH_VALUE	 (1000)		// �X�v���C�g���ʏ�̃J�[�u�̐؂�ւ���͈͂̒l
//�x�@
#define CURVE_POLICE_CHECK	 (2000)		// �x�@���v���C���[�ɒʏZ����͈�
#define COLLISIOBOX_SCALE	 (D3DXVECTOR3(0.4f,0.4f,0.4f))

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPD3DXMESH CCarBase::m_pMesh[MAX_CAR] = {};							//	���b�V�����ւ̃|�C���^
LPD3DXBUFFER CCarBase::m_pBuffMat[MAX_CAR] = {};					//	�}�e���A���̏��ւ̃|�C���^
DWORD CCarBase::m_nNumMat[MAX_CAR] = {};							//	�}�e���A���̏��
LPDIRECT3DTEXTURE9 CCarBase::m_pMeshTextures[MAX_CAR_TEXTURE] = {};
D3DXVECTOR3 CCarBase::m_LoadVtxMax[MAX_CAR] = {};
D3DXVECTOR3 CCarBase::m_LoadVtxMin[MAX_CAR] = {};

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CCarBase::CCarBase() : CModel3D(CAR_PRIOTITY, CScene::OBJTYPE_CAR)
{
	//	�ϐ��̏�����
	m_ModelMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Spin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nSetObjectNum = 0;
	m_nTexType = 0;
	m_nCollision = 1;
	m_fAngle = 0.0f;
	m_InitPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bOtherCarHit = false;
}

//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CCarBase::~CCarBase()
{
}

//===============================================================================
//�@�N���G�C�g
//===============================================================================
CCarBase * CCarBase::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nTexType, int nObjectType,
	int nCollision, int route, bool bLoop)
{
	CCarBase *pCarBase = NULL;

	// NULL�`�F�b�N
	if (pCarBase == NULL)
	{// �������̓��I�m��
		if (nObjectType == MODEL_POLICE_CAR)
		{//	�x�@
			pCarBase = new CCarPolice;
		}
		else
		{//	��蕨
			pCarBase = new CCarNpc;
		}
		if (pCarBase != NULL)
		{
			//	���[�v���邩�ǂ���
			pCarBase->SetLoop(bLoop);
			//	�����ʒu
			pCarBase->SetInitPos(pos);
			//	�o�H�̐ݒ�
			pCarBase->SetRoute(route);
			//	�e�N�X�`���̐ݒ�
			pCarBase->m_nTexType = nTexType;
			// ��ނ̐ݒ�
			pCarBase->BindModel(m_pMesh[nObjectType], m_pBuffMat[nObjectType], m_nNumMat[nObjectType], m_pMeshTextures[nTexType],
								m_LoadVtxMax[nObjectType], m_LoadVtxMin[nObjectType]);
			// �I�u�W�F�N�g�N���X�̐���
			pCarBase->Init();
			// �I�u�W�F�N�g���Ƃ̐ݒ�p�^�C�v
			pCarBase->m_nType = nObjectType;
			//	��]�𔽉f
			pCarBase->SetRot(rot);
			// �ʒu����
			pCarBase->SetPosition(pos);
			// �T�C�Y��e�N���X�ɑ��
			pCarBase->SetScale(scale);
			// �R���W������ONOFF
			pCarBase->m_nCollision = nCollision;
		}
	}
	//	�l��Ԃ�
	return pCarBase;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCarBase::Init(void)
{
	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//	3D���f���̏�����
	CModel3D::Init();

	ScaleVtxCornerPos(COLLISIOBOX_SCALE);
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CCarBase::Uninit(void)
{
	//3D���f���̏I������
	CModel3D::Uninit();
}

//=============================================================================

// �X�V����
//=============================================================================
void CCarBase::Update(void)
{
	//	3D���f���̍X�V����
	CModel3D::Update();

	if (CModel3D::GetLength() > CAR_DROW)
	{//��苗���ȏゾ������`�悵�Ȃ�
		SetDraw(false);
	}
	else
	{
		SetDraw(true);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CCarBase::Draw(void)
{
	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���_�@���̎������K��
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	//3D���f����Draw
	CModel3D::Draw();

	//���_�@���̎������K��
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}

//=============================================================================
// �����̊Ǘ�
//=============================================================================
void  CCarBase::MoveMent(D3DXVECTOR3 VecA, float fAddRot, int bPoliceMoveValeu)
{
	//	�ʒu�̎擾
	D3DXVECTOR3 pos = CModel3D::GetPosition();

	// �ړ��ʂ̏�����
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//	�����̎擾
	D3DXVECTOR3 &rot = CModel3D::GetRot();

	//	�������x�N�g���̐ݒ�
	D3DXVECTOR3 m_VecA = VecA;

	float fAngle = 0.0f;								//	�p�x
	float fDiffese = 0.0f;								//	����

	//	�v���C���[�ƎԂ̊p�x�����߂�
	fAngle = atan2f(m_VecA.x, m_VecA.z);
	//	���݂̊p�x�ƖړI�̊p�x�̍������߂�
	fDiffese = fAngle - m_fAngle;

	//	�����̏C��
	if (fDiffese < -D3DX_PI)
	{
		fDiffese += D3DX_PI * 2.0f;
	}
	if (fDiffese > D3DX_PI)
	{
		fDiffese -= D3DX_PI * 2.0f;
	}
	//	�p�x�̕ω��ʂ����߂�
	m_fAngle += fDiffese;
	//	�p�x�̏C��l
	if (m_fAngle < -D3DX_PI)
	{
		m_fAngle += D3DX_PI * 2.0f;
	}
	if (m_fAngle > D3DX_PI)
	{
		m_fAngle -= D3DX_PI * 2.0f;
	}

	if (m_nType == MODEL_POLICE_CAR)
	{//	�x�@
		if (CCustomer::GetPutIn() == false)
		{//	���q�������Ԃ��Ă��Ȃ������ꍇ
			m_rotDest.y = (D3DX_PI - m_fAngle)*-1.0f;

			move.x += sinf(fAngle) * CAR_SPEED*2.35f;
			move.z += cosf(fAngle) * CAR_SPEED*2.35f;
		}
	}
	else
	{//	�x�@�ȊO
		m_rotDest.y = (D3DX_PI - m_fAngle)*-1.0f;

		move.x += sinf(fAngle) * CAR_SPEED*1.25f;
		move.z += cosf(fAngle) * CAR_SPEED*1.25f;

	}

	// �ړ��ʂ̑��
	pos.x += move.x;
	pos.z += move.z;

	//	�p�x�̃u�����h
	rot = AddRod(rot, m_rotDest, fAddRot);
	// �e����̐ݒ�
	CModel3D::SetPosition(pos);
	CModel3D::SetMove(move);
	CModel3D::SetRot(rot);
}

//=============================================================================
//	�p�x�����Z����
//=============================================================================
D3DXVECTOR3 CCarBase::AddRod(D3DXVECTOR3 rot, D3DXVECTOR3 rotDest, float fAddRot)
{
	//�ړI�̌����̏C��
	if (rotDest.y < -D3DX_PI)
	{
		rotDest.y += D3DX_PI * 2.0f;
	}
	else if (rotDest.y > D3DX_PI)
	{
		rotDest.y -= D3DX_PI * 2.0f;
	}
	//	��]�̊p�x�̃u�����h
	float fAngle = rotDest.y - rot.y;
	if (fAngle < -D3DX_PI)
	{
		fAngle += D3DX_PI * 2.0f;
	}
	else if (fAngle > D3DX_PI)
	{
		fAngle -= D3DX_PI * 2.0f;
	}
	//	��]�̃u�����h����l����]�ɑ��
	rot.y += fAngle * fAddRot;
	//	�����̏C��
	if (rot.y < -D3DX_PI)
	{
		rot.y += D3DX_PI * 2.0f;
	}
	else if (rot.y > D3DX_PI)
	{
		rot.y -= D3DX_PI * 2.0f;
	}

	//	�l��Ԃ�
	return rot;
}

//===============================================================================
// X�t�@�C���̓ǂݍ���
//===============================================================================
HRESULT CCarBase::Load(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//	���f���̖��O���i�[�̂���ϐ�
	char cModelName[356];

	for (int nCntIndex = 0; nCntIndex < m_MaxCarModel; nCntIndex++)
	{//	�Ԃ̃��f���̍ő吔����
		switch (nCntIndex)
		{//	�ǂݍ��ރ��f���̖��O
		case MODEL_NOMAL_CAR_000:
			wsprintf(cModelName, MODEL_NAME_00);	//	�ʏ�̎�[0]
			break;
		case MODEL_NOMAL_CAR_001:
			wsprintf(cModelName, MODEL_NAME_01);	//	�ʏ�̎�[1]
			break;
		case MODEL_NOMAL_CAR_002:
			wsprintf(cModelName, MODEL_NAME_02);	//	�ʏ�̎�[2]
			break;
		case MODEL_SPORTS_CAR_000:
			wsprintf(cModelName, MODEL_NAME_10);	//	�X�|�[�c�J�[[0]
			break;
		case MODEL_SPORTS_CAR_001:
			wsprintf(cModelName, MODEL_NAME_11);	//	�X�|�[�c�J�[[1]
			break;
		case MODEL_SPORTS_CAR_002:
			wsprintf(cModelName, MODEL_NAME_12);	//	�X�|�[�c�J�[[2]
			break;
		case MODEL_BUS_CAR_000:
			wsprintf(cModelName, MODEL_NAME_20);	//	�o�X[0]
			break;
		case MODEL_BUS_CAR_001:
			wsprintf(cModelName, MODEL_NAME_21);	//	�o�X[1]
			break;
		case MODEL_BUS_CAR_002:
			wsprintf(cModelName, MODEL_NAME_22);	//	�o�X[2]
			break;
		case MODEL_POLICE_CAR:
			wsprintf(cModelName, MODEL_NAME_99);	//	�x�@
			break;
		}

		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(cModelName,
			D3DXMESH_SYSTEMMEM,
			pDevice, NULL,
			&m_pBuffMat[nCntIndex],
			NULL,
			&m_nNumMat[nCntIndex],
			&m_pMesh[nCntIndex]);

		int nNumVtx;		//���_��
		DWORD sizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
		BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

		//���f���̍ő�l�E�ŏ��l���擾����

		m_LoadVtxMax[nCntIndex] = D3DXVECTOR3(-10000, -10000, -10000);	//�ő�l
		m_LoadVtxMin[nCntIndex] = D3DXVECTOR3(10000, 10000, 10000);	//�ŏ��l

																			//���_�����擾
		nNumVtx = m_pMesh[nCntIndex]->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(m_pMesh[nCntIndex]->GetFVF());

		//���_�o�b�t�@�̃��b�N
		m_pMesh[nCntIndex]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		//���_���W�̑��

															//�ő�l
			if (vtx.x > m_LoadVtxMax[nCntIndex].x)
			{
				m_LoadVtxMax[nCntIndex].x = vtx.x;
			}
			if (vtx.y > m_LoadVtxMax[nCntIndex].y)
			{
				m_LoadVtxMax[nCntIndex].y = vtx.y;
			}
			if (vtx.z > m_LoadVtxMax[nCntIndex].z)
			{
				m_LoadVtxMax[nCntIndex].z = vtx.z;
			}
			//�ŏ��l
			if (vtx.x < m_LoadVtxMin[nCntIndex].x)
			{
				m_LoadVtxMin[nCntIndex].x = vtx.x;
			}
			if (vtx.y < m_LoadVtxMin[nCntIndex].y)
			{
				m_LoadVtxMin[nCntIndex].y = vtx.y;
			}
			if (vtx.z < m_LoadVtxMin[nCntIndex].z)
			{
				m_LoadVtxMin[nCntIndex].z = vtx.z;
			}

			//�T�C�Y���̃|�C���^��i�߂�
			pVtxBuff += sizeFVF;
		}

		//���_�o�b�t�@�̃A�����b�N
		m_pMesh[nCntIndex]->UnlockVertexBuffer();
	}

	//�}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL *pMat;

	for (int nCount = 0; nCount < m_MaxCarModel; nCount++)
	{
		//�}�e���A����񂩂�e�N�X�`���̎擾
		pMat = (D3DXMATERIAL*)m_pBuffMat[nCount]->GetBufferPointer();
	}

	//	�l��Ԃ�
	return S_OK;
}

//===============================================================================
// �e�N�X�`���̔j��
//===============================================================================
void CCarBase::UnLoad(void)
{
	for (int nCount = 0; nCount < m_MaxCarModel; nCount++)
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
	//�e�N�X�`��
	for (int nCntTex = 0; nCntTex < MAX_CAR_TEXTURE; nCntTex++)
	{
		if (m_pMeshTextures[nCntTex] != NULL)
		{
			m_pMeshTextures[nCntTex]->Release();
			m_pMeshTextures[nCntTex] = NULL;
		}
	}
}

//****************************************************************************************************************************
//�@NPC�̏�蕨
//****************************************************************************************************************************

int CCarNpc::m_nStopRoute = NULL;

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CCarNpc::CCarNpc()
{
	//	�ϐ��̏�����
	m_VecA = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nRouteCounter = 0;
	m_nNumPoint = 0;
	m_CurvePosDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	for (int nCount = 0; nCount < 2; nCount++)
	{
		m_LengthValue[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	m_posSave = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCurveSwitch = 0;
	m_bCurve = false;
	m_fLengthPlayer = 0;
	m_fLengthPoint = 0;
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nRotState = 0;
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CCarNpc::~CCarNpc()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CCarNpc::Init(void)
{

	//����������
	CCarBase::Init();

	//	�o�H�̎擾
	int nRoute = CCarBase::GetRoute();

	//���[�g�̎擾
	CGame::Route GameRoute = CGame::GetCarRoute();

	if (nRoute > GameRoute.nRouteCount)
	{//	���[�g�̍ő�l�𒴂��Ă����ꍇ
		nRoute = 0;
	}

	for (int nCountPoint = 0; nCountPoint < GameRoute.nPointCount[nRoute]; nCountPoint++)
	{ //	�Ԃ̌o�H��POINT�ݒ�
		m_nIndexPoint[nCountPoint] = GameRoute.nIndexPoint[nRoute][nCountPoint];
	}
	//	�����ʒu�̎擾
	D3DXVECTOR3 pos = CCarBase::GetInitPos();

	//�|�C���g�̎擾
	CGame::Point GamePoint = CGame::GetCarPoint();

	for (int nCountPoint = 0; nCountPoint < GameRoute.nPointCount[nRoute]; nCountPoint++)
	{//�߂��|�C���g�̃\�[�g

		float fLengthPointA = sqrtf((GamePoint.pos[m_nIndexPoint[nCountPoint]].x - pos.x) * (GamePoint.pos[m_nIndexPoint[nCountPoint]].x - pos.x) +
			(GamePoint.pos[m_nIndexPoint[nCountPoint]].z - pos.z) * (GamePoint.pos[m_nIndexPoint[nCountPoint]].z - pos.z));

		float m_fLengthPointB = fLengthPointA;		//	�l�̑��

		if (0 == nCountPoint)
		{//	�����̒l�̑��
			m_nNumPoint = m_nIndexPoint[0];				//	�ԍ��̑��
			m_nRouteCounter = 0;						//	���[�g�̃J�E���g���
			m_fLengthPoint = m_fLengthPointB;			//	�����̓���ւ�
		}
		else if ((m_fLengthPoint > m_fLengthPointB))
		{//	��ԋ߂��o�H�̐ݒ�
			m_nNumPoint = m_nIndexPoint[nCountPoint];	//	�ԍ��̑��
			m_nRouteCounter = nCountPoint;				//	���[�g�̃J�E���g���
			m_fLengthPoint = m_fLengthPointB;			//	�����̓���ւ�
		}
	}
	//	�����ݒ�
	m_nStopRoute = 99;

	SetStateRoll(false);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CCarNpc::Uninit(void)
{
	//�I������
	CCarBase::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CCarNpc::Update(void)
{
	D3DXVECTOR3 pos = CModel3D::GetPosition();				//	�ʒu�̎擾
	D3DXVECTOR3 rot = CModel3D::GetRot();					//	�����̎擾
	D3DXVECTOR3 move = CModel3D::GetMove();					//	�����̎擾
	int nType = CCarBase::GetType();						//	�^�C�v�̎擾
	CPlayer * pPlayer = CGame::GetPlayer();					// �v���C���[���̎擾

	//	�ʒu�̕ۑ�
	m_posOld = pos;
	//	�X�V����
	CCarBase::Update();
	m_fLengthPlayer = CCarBase::GetLength();	//�v���C���[�Ƃ̋������擾

	// ��]�s�񂩂�I�C���[�p���擾
	D3DXMATRIX mtx = CModel3D::GetMtx();
	float fAngleX = asinf(-mtx._32);
	float fAngleZ = atan2(mtx._12, mtx._22);

	if (fAngleX < 0.01f && fAngleX > -0.01f && fAngleZ < 0.01f && fAngleZ > -0.01f)
	{// ���]���Ă��Ȃ��Ƃ�
		m_nRotState = 0;
	}
	else
	{//	���]�����ꍇ
		m_nRotState = 1;
	}

	switch (m_nRotState)
	{
	case 0:
		if (m_fLengthPlayer >= 300 && CCarBase::GetRoute() != CGame::GetStopRoute())
		{//	�Ԃ��o�X�������ꍇ
			MoveMent(GetCarPointVec(), 0.0525f, m_bCurve);
		}
		break;
	case 1:
		float fMoveY = CCarBase::GetMove().y;
		SetMove(D3DXVECTOR3(0.00f, fMoveY, 0.0f));

		if (m_fLengthPlayer >= 8000)
		{//	�v���C���[�Ƃ͈̔̓`�F�b�N
		 //	�o�H�̎擾
			int nRoute = CCarBase::GetRoute();

			//�|�C���g�̎擾
			CGame::Point GamePoint = CGame::GetCarPoint();

			for (int nCountPoint = 0; nCountPoint < CGame::GetCarRoute().nPointCount[nRoute]; nCountPoint++)
			{//�߂��|�C���g�̃\�[�g
				float fLengthPointA = sqrtf((GamePoint.pos[m_nIndexPoint[nCountPoint]].x - pos.x)*(GamePoint.pos[m_nIndexPoint[nCountPoint]].x - pos.x) +
					(GamePoint.pos[m_nIndexPoint[nCountPoint]].z - pos.z)*(GamePoint.pos[m_nIndexPoint[nCountPoint]].z - pos.z));

				float fLengthPointB = fLengthPointA;	//	�l�̑��
				if (0 == nCountPoint)
				{//	�����̒l�̑��
					m_nNumPoint = m_nIndexPoint[0];		//	�ԍ��̑��
					m_nRouteCounter = 0;				//	���[�g�̃J�E���g���
					m_fLengthPoint = fLengthPointB;		//	�����̓���ւ�
				}
				if ((m_fLengthPoint >= fLengthPointB) && (0 != nCountPoint))
				{//	��ԋ߂��o�H�̐ݒ�
					m_nNumPoint = m_nIndexPoint[nCountPoint];	//	�ԍ��̑��
					m_nRouteCounter = nCountPoint;		//	���[�g�̃J�E���g���
					m_fLengthPoint = fLengthPointB;		//	�����̓���ւ�
				}
			}
			CModel3D::SetBoolBlowOff(false);
			CModel3D::InitRot();
			CModel3D::Init();
			SetPosition(D3DXVECTOR3(GamePoint.pos[m_nIndexPoint[m_nRouteCounter]].x,0.0f, GamePoint.pos[m_nIndexPoint[m_nRouteCounter]].z));
			m_nRotState = 0;
		}
		break;
	}
	if ((move.x < 0.1f && move.x > -0.1f && move.z < 0.1f && move.z > -0.1) && (m_nRotState == 0) && pPlayer->GetCrcleCarIn() == true)
	{//	�Ԃ��~�܂����ꍇ
		m_nStopRoute = CCarBase::GetRoute();	//	�~�܂郋�[�g�̎擾
		CGame::SetStopRoute(m_nStopRoute);		//	�~�܂郋�[�g�̐ݒ�
		if (m_fLengthPlayer > 300 && CCarBase::GetRoute() == CGame::GetStopRoute())
		{//	�v���C���[��300�ȏ㗣��Ă���ԁ@�~�܂��Ă��郋�[�g�Ɠ����ԁ@
			pPlayer->SetCrcleCarIn(false);

			if (pPlayer->GetCrcleCarIn() == false)
			{//	�v���C���[�͈̔͂ɎԂ����Ȃ��ꍇ
				CGame::SetStopRoute(99);
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CCarNpc::Draw(void)
{
	CCarBase::Draw();
}

//=============================================================================
//	�Ԃ̃|�C���g�Ƃ̃x�N�g���̎擾
//=============================================================================
D3DXVECTOR3 CCarNpc::GetCarPointVec(void)
{
	D3DXVECTOR3 pos = CModel3D::GetPosition();				//	�ʒu�̎擾
	int nRoute = CCarBase::GetRoute();						//	�o�H�̎擾

	//	�͈͊O�̏ꍇ[�ʏ�̋Ȃ���]
	GoPointNormalCurve(pos, nRoute);

	//	�l��Ԃ�
	return m_VecA;
}

//=============================================================================
//	���̃|�C���g�Ɍ�����[�ʏ�̋Ȃ���]
//=============================================================================
void CCarNpc::GoPointNormalCurve(D3DXVECTOR3 pos, int nRoute)
{
	//�|�C���g�̎擾
	CGame::Point GamePoint = CGame::GetCarPoint();

	if (GamePoint.nNum[m_nNumPoint] == m_nNumPoint)
	{//	�i�ޔԍ��̐ݒ�
	 //	POINT�ƎԂ̋���
		float fLength = sqrtf((GamePoint.pos[m_nNumPoint].x - pos.x) *(GamePoint.pos[m_nNumPoint].x - pos.x) +
			(GamePoint.pos[m_nNumPoint].z - pos.z) * (GamePoint.pos[m_nNumPoint].z - pos.z));
		if (fLength <= CAR_POINT_LENGTH)
		{//	�͈͓�����
			m_nRouteCounter += 1;
			if (m_nNumPoint == CAR_POINT_LAST)
			{//	�Ō�̃|�C���g����ŏ��̃|�C���g��
				bool bLoop = CCarBase::GetLoop();	//���[�v��Ԃ̎擾
				if (!bLoop)
				{//	�Ԃ����[�v���Ȃ�
					m_nNumPoint = m_nIndexPoint[1];	//	�������ꏊ�̃��Z�b�g
					m_nRouteCounter = 1;			//	���[�g���Z�b�g
					SetPosition(D3DXVECTOR3(GamePoint.pos[m_nIndexPoint[m_nRouteCounter]].x, 0.0f, GamePoint.pos[m_nIndexPoint[m_nRouteCounter]].z));
				}
				else
				{//	�Ԃ����[�v����
					m_nNumPoint = m_nIndexPoint[0];	//	�������ꏊ�̃��Z�b�g
					m_nRouteCounter = 0;			//	���[�g���Z�b�g
				}
			}
			else
			{ //���[�g�̎��̐ݒ�
				m_nNumPoint = m_nIndexPoint[m_nRouteCounter];
			}

		}
		//	�Ԃƃ|�C���g�Ƃ̋��������߂�
		m_VecA = GamePoint.pos[m_nNumPoint] - pos;
	}
}


//=============================================================================
// ���b�V���̓����蔻��
//=============================================================================
void CCarNpc::CollisitionPriority2(void){}

//****************************************************************************************************************************
//�@�x�@
//****************************************************************************************************************************

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CCarPolice::CCarPolice()
{
	//	�ϐ��̏�����
	m_VecA = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nRouteCounter = 0;
	m_bPursuer = false;
	m_fLengthPoint = 0.0f;
	m_fLengthPointB = 0.0f;
	m_nNumSavePoint = 0;
	m_fLengthNextPoint = 0.0f;
	m_fLenPoliceNextPit = 0.0f;
	m_nNumPoint = 0;
	m_CurvePosDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	for (int nCount = 0; nCount < 2; nCount++)
	{
		m_LengthValue[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	for (int nCount = 0; nCount < INDXPOINT_MAX; nCount++)
	{
		m_nIndexPoint[nCount] = 0;
	}

	m_posSave = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCurveSwitch = 0;
	m_bRestart = false;
	m_fLengthPlayer = 0;
	m_nMoveState = 0;
	m_fLengthFar = 0.0f;
	m_nSirenCut = 0;
	m_pPoliceLamp = NULL;
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CCarPolice::~CCarPolice(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCarPolice::Init(void)
{
	//	����������
	CCarBase::Init();

	//	�o�H�̐ݒ�
	int nRoute = CCarBase::GetRoute();

	//���[�g�̎擾
	CGame::Route GameRoute = CGame::GetCarRoute();

	//�|�C���g�̎擾
	CGame::Point GamePoint = CGame::GetCarPoint();

	for (int nCountPoint = 0; nCountPoint < GameRoute.nPointCount[nRoute]; nCountPoint++)
	{//	�Ԃ̌o�H��POINT�ݒ�
		m_nIndexPoint[nCountPoint] = GameRoute.nIndexPoint[nRoute][nCountPoint];
	}
	//	�����ʒu�̎擾
	D3DXVECTOR3 pos = CCarBase::GetInitPos();

	for (int nCountPoint = 0; nCountPoint < GameRoute.nPointCount[nRoute]; nCountPoint++)
	{//�߂��|�C���g�̃\�[�g

		float fLengthPointA = sqrtf((GamePoint.pos[m_nIndexPoint[nCountPoint]].x - pos.x)*(GamePoint.pos[m_nIndexPoint[nCountPoint]].x - pos.x) +
			(GamePoint.pos[m_nIndexPoint[nCountPoint]].z - pos.z)*(GamePoint.pos[m_nIndexPoint[nCountPoint]].z - pos.z));

		float fLengthPointB = fLengthPointA;			//	�l�̑��

		if (0 == nCountPoint)
		{//	�����̒l�̑��
			m_nNumPoint = m_nIndexPoint[0];				//	�ԍ��̑��
			m_nRouteCounter = 0;						//	���[�g�̃J�E���g���
			m_fLengthPoint = fLengthPointB;				//	�����̓���ւ�
		}
		else if ((m_fLengthPoint > fLengthPointB))
		{//	��ԋ߂��o�H�̐ݒ�
			m_nNumPoint = m_nIndexPoint[nCountPoint];	//	�ԍ��̑��
			m_nRouteCounter = nCountPoint;				//	���[�g�̃J�E���g���
			m_fLengthPoint = fLengthPointB;				//	�����̓���ւ�
		}
	}

	SetHeavy(100.0f);
	CModel3D::SetBoolRoll(false);

	if (m_pPoliceLamp == NULL)
	{
		//	�x�@�̃����v�̐���
		D3DXVECTOR3 ModelPos = CModel3D::GetPosition();
		m_pPoliceLamp = CPoliceLamp::Create(D3DXVECTOR3(ModelPos.x, ModelPos.y + 50, ModelPos.z - 100), D3DXVECTOR2(20.0f, 20.0f));
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CCarPolice::Uninit(void)
{
	//�I������
	CCarBase::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CCarPolice::Update(void)
{
	//	�X�V����
	CCarBase::Update();

	D3DXVECTOR3 pos = CModel3D::GetPosition();				//	�ʒu�̎擾
	D3DXVECTOR3 rot = CModel3D::GetRot();					//	�����̎擾
	D3DXVECTOR3 move = CModel3D::GetMove();					//	�����̎擾
	int nType = CCarBase::GetType();						//	�^�C�v�̎擾
	CGame::Point GamePoint = CGame::GetCarPoint();			//	�|�C���g�̎擾

															//	�Ԃƃ|�C���g�̋���
	m_fLengthFar = sqrtf((GamePoint.pos[m_nNumPoint].x - pos.x)*(GamePoint.pos[m_nNumPoint].x - pos.x) +
		(GamePoint.pos[m_nNumPoint].z - pos.z)*(GamePoint.pos[m_nNumPoint].z - pos.z));

	//	�x�@�ƃv���C���[�̋���
	m_fLengthPlayer = CModel3D::GetLength();

	//	�X�V����
	CCarBase::Update();

	if (m_fLengthFar < 4000 || m_fLengthPlayer <= 1000)
	{//	�ړ�����
		m_nMoveState = 0;
		m_bRestart = false;
	}
	else if (m_bPursuer == true && m_fLengthPlayer >= 1000)
	{
		m_bPursuer = false;
		m_bRestart = true;
		m_nMoveState = 1;
	}
	else
	{
		m_bRestart = false;
	}

	switch (m_nMoveState)
	{
	case 0:
		if (m_bRestart == false)
		{//	�ړ�����
			MoveMent(GetPoliceVec(), 0.0525f, false);
		}
		break;
	case 1:
		SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		if (m_fLengthPlayer >= 4000 && m_fLengthFar > 3500)
		{
			//	�o�H�̎擾
			int nRoute = CCarBase::GetRoute();

			for (int nCountPoint = 0; nCountPoint < CGame::GetCarRoute().nPointCount[nRoute]; nCountPoint++)
			{//�߂��|�C���g�̃\�[�g
				float fLengthPointA = sqrtf((GamePoint.pos[m_nIndexPoint[nCountPoint]].x - pos.x)*(GamePoint.pos[m_nIndexPoint[nCountPoint]].x - pos.x) +
					(GamePoint.pos[m_nIndexPoint[nCountPoint]].z - pos.z)*(GamePoint.pos[m_nIndexPoint[nCountPoint]].z - pos.z));

				float fLengthPointB = fLengthPointA;	//	�l�̑��
				if (0 == nCountPoint)
				{//	�����̒l�̑��
					m_nNumPoint = m_nIndexPoint[0];		//	�ԍ��̑��
					m_nRouteCounter = 0;				//	���[�g�̃J�E���g���
					m_fLengthPoint = fLengthPointB;		//	�����̓���ւ�
				}
				if ((m_fLengthPoint >= fLengthPointB) && (0 != nCountPoint))
				{//	��ԋ߂��o�H�̐ݒ�
					m_nNumPoint = m_nIndexPoint[nCountPoint];	//	�ԍ��̑��
					m_nRouteCounter = nCountPoint;		//	���[�g�̃J�E���g���
					m_fLengthPoint = fLengthPointB;		//	�����̓���ւ�
				}
			}
			CModel3D::SetBoolBlowOff(false);
			CModel3D::InitRot();
			CModel3D::Init();
			SetPosition(D3DXVECTOR3(GamePoint.pos[m_nIndexPoint[m_nRouteCounter]].x, 0.0f, GamePoint.pos[m_nIndexPoint[m_nRouteCounter]].z));
			m_nMoveState = 0;
			m_bRestart = false;
		}
		break;
	}
	if (m_bPursuer == false)
	{//	�ǂ��Ă��Ȃ�
		m_nSirenCut = 0;
		if (m_pPoliceLamp != NULL)
		{
			m_pPoliceLamp->SetPosSize(D3DXVECTOR3(pos.x, pos.y + 60, pos.z), D3DXVECTOR2(0.0f, 0.0f));
			m_pPoliceLamp->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
		}
	}

	if (move.x < 0.1f && move.z < 0.1f&&move.x >- 0.1f && move.z > -0.1f)
	{
		// �ǂ���ԂłȂ��Ƃ��T�C�������~�߂�
		CManager::GetSound()->StopSound(CSound::SOUND_LABEL_BGM_POLICE_SIREN);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CCarPolice::Draw(void)
{
	//	�`�揈��
	CCarBase::Draw();

}
//=============================================================================
//	�x�@�ƃv���C���[�Ƃ̃x�N�g���̎擾
//=============================================================================
D3DXVECTOR3 CCarPolice::GetPoliceVec(void)
{
	//�ʒu�̎擾
	D3DXVECTOR3 pos = CModel3D::GetPosition();
	//	�o�H�̎擾
	int nRoute = CCarBase::GetRoute();

	 if(m_fLengthPlayer < CURVE_SWITCH_VALUE)
	{//	�͈͓�
		RangeCheckIn(pos, m_fLengthPlayer, nRoute);
	}
	else if (m_fLengthPlayer > CURVE_SWITCH_VALUE)
	{//	�͈͊O�̏ꍇ
		RangeCheckOut(pos, nRoute);

		// �ǂ���ԂłȂ��Ƃ��T�C�������~�߂�
		CManager::GetSound()->StopSound(CSound::SOUND_LABEL_BGM_POLICE_SIREN);
		m_bPursuer = false;
	}
	else
	{
		// �ǂ���ԂłȂ��Ƃ��T�C�������~�߂�
		CManager::GetSound()->StopSound(CSound::SOUND_LABEL_BGM_POLICE_SIREN);
	}



	//	�l��Ԃ�
	return m_VecA;
}

//=============================================================================
//	�x�@�ƃv���C���[�̋������͈͊O�̏ꍇ
//=============================================================================
void CCarPolice::RangeCheckOut(D3DXVECTOR3 pos, int nRoute)
{
	//�|�C���g�̎擾
	CGame::Point GamePoint = CGame::GetCarPoint();

	if (GamePoint.nNum[m_nNumPoint] == m_nNumPoint)
	{//	�i�ޔԍ��̐ݒ�
	 //	POINT�ƎԂ̋���
		float fLength = sqrtf((GamePoint.pos[m_nNumPoint].x - pos.x)*(GamePoint.pos[m_nNumPoint].x - pos.x) +
			(GamePoint.pos[m_nNumPoint].z - pos.z)*(GamePoint.pos[m_nNumPoint].z - pos.z));

		if (fLength <= CAR_POINT_LENGTH)
		{//	�͈͓�����
			m_nRouteCounter += 1;
			if (m_nNumPoint == CAR_POINT_LAST)
			{//	�Ō�̃|�C���g����ŏ��̃|�C���g��

				bool bLoop = CCarBase::GetLoop();	//���[�v��Ԃ̎擾

				if (!bLoop)
				{//	�Ԃ����[�v���Ȃ�
					m_nNumPoint = m_nIndexPoint[1];	//	�������ꏊ�̃��Z�b�g
					m_nRouteCounter = 1;			//	���[�g���Z�b�g
					SetPosition(D3DXVECTOR3(GamePoint.pos[m_nIndexPoint[0]].x, 0.0f, GamePoint.pos[m_nIndexPoint[0]].z));
				}
				else
				{//	�Ԃ����[�v����
					m_nNumPoint = m_nIndexPoint[0];	//	�������ꏊ�̃��Z�b�g
					m_nRouteCounter = 0;			//	���[�g���Z�b�g
				}
			}
			else
			{ //���[�g�̎��̐ݒ�
				m_nNumPoint = m_nIndexPoint[m_nRouteCounter];
			}


		}
		//	�Ԃƃ|�C���g�Ƃ̋��������߂�
		m_VecA = GamePoint.pos[m_nNumPoint] - pos;
	}
}
//=============================================================================
//	�x�@�ƃv���C���[�̋������͈͓��̏ꍇ
//=============================================================================
void CCarPolice::RangeCheckIn(D3DXVECTOR3 pos, float fLengthPlayer, int nRoute)
{
	if ((fLengthPlayer <= CURVE_POLICE_CHECK))
	{//	�͈͓�����
		m_nSirenCut++;
		if (CGame::GetGameCounter() > 500)
		{//	�Q�[���̎��Ԃ�6�b�ȏソ�����ꍇ
			if (!m_bPursuer) { CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_POLICE_SIREN); }
		}
		m_bPursuer = true;								//	�ǂ��Ă���
		m_VecA = CGame::GetPlayer()->GetPos() - pos;	//	�x�@�ƃv���C���[�Ƃ̃x�N�g���̎擾
		if (m_pPoliceLamp != NULL)
		{
			D3DXVECTOR3 ModelPos = CModel3D::GetPosition();

			if ((m_nSirenCut < 15))
			{//	�����v�̓_��
				m_pPoliceLamp->SetPosSize(D3DXVECTOR3(ModelPos.x, ModelPos.y + 60, ModelPos.z), D3DXVECTOR2(20.0f, 20.0f));
				m_pPoliceLamp->SetColor(D3DXCOLOR(1.0f, 0.2f, 0.1f, 1.0f));
			}
			else if ((m_nSirenCut < 30))
			{//	�����v�̓_��
				m_pPoliceLamp->SetPosSize(D3DXVECTOR3(ModelPos.x, ModelPos.y + 60, ModelPos.z), D3DXVECTOR2(25.0f, 25.0f));
				m_pPoliceLamp->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}
			else if ((m_nSirenCut > 30))
			{//	�J�E���^�[�̃��Z�b�g
				m_nSirenCut = 0;
			}
		}
	}
	if ((m_bPursuer == true) && (fLengthPlayer >= CURVE_POLICE_CHECK))
	{
		//�|�C���g�̎擾
		CGame::Point GamePoint = CGame::GetCarPoint();
		//���̃|�C���g�����߂�
		AskPoint(pos, nRoute);
		m_bPursuer = false;
	}
	else if ((m_bPursuer == false) && (fLengthPlayer >= CURVE_POLICE_CHECK))
	{//	���̃|�C���g�Ɍ�����
		GoPoint(pos, nRoute);
	}
}

//=============================================================================
//	���̃|�C���g�����߂�
//=============================================================================
void  CCarPolice::AskPoint(D3DXVECTOR3 pos, int nRoute)
{
	//�|�C���g�̎擾
	CGame::Point GamePoint = CGame::GetCarPoint();

	for (int nCountPoint = 0; nCountPoint < CGame::GetCarRoute().nPointCount[nRoute]; nCountPoint++)
	{//�߂��|�C���g�̃\�[�g
		float fLengthPointA = sqrtf((GamePoint.pos[m_nIndexPoint[nCountPoint]].x - pos.x)*(GamePoint.pos[m_nIndexPoint[nCountPoint]].x - pos.x) +
			(GamePoint.pos[m_nIndexPoint[nCountPoint]].z - pos.z)*(GamePoint.pos[m_nIndexPoint[nCountPoint]].z - pos.z));

		m_fLengthPointB = fLengthPointA;		//	�l�̑��
		if (0 == nCountPoint)
		{//	�����̒l�̑��
			m_nNumPoint = m_nIndexPoint[0];		//	�ԍ��̑��
			m_nRouteCounter = 0;				//	���[�g�̃J�E���g���
			m_fLengthPoint = m_fLengthPointB;	//	�����̓���ւ�
			m_bPursuer = false;					//	�ǂ��Ă��Ȃ�
		}
		if ((m_fLengthPoint >= m_fLengthPointB) && (0 != nCountPoint))
		{//	��ԋ߂��o�H�̐ݒ�
			m_nNumPoint = m_nIndexPoint[nCountPoint];	//	�ԍ��̑��
			m_nRouteCounter = nCountPoint;		//	���[�g�̃J�E���g���
			m_fLengthPoint = m_fLengthPointB;	//	�����̓���ւ�
			m_bPursuer = false;					//	�ǂ��Ă��Ȃ�
		}
	}
	if (m_nNumPoint == CAR_POINT_LAST)
	{//	�߂��|�C���g���Ō�̃|�C���g�̏ꍇ
	 /*	��ԋ߂��|�C���g�Ǝ��̃|�C���g�̋���*/
		m_fLengthNextPoint = sqrtf((GamePoint.pos[m_nNumPoint].x - GamePoint.pos[m_nIndexPoint[0]].x)*(GamePoint.pos[m_nNumPoint].x - GamePoint.pos[m_nIndexPoint[0]].x) +
			(GamePoint.pos[m_nNumPoint].z - GamePoint.pos[m_nIndexPoint[0]].z)*(GamePoint.pos[m_nNumPoint].z - GamePoint.pos[m_nIndexPoint[0]].z));
		/*	�x�@�Ǝ��̃|�C���g�̋���*/
		m_fLenPoliceNextPit = sqrtf((pos.x - GamePoint.pos[m_nIndexPoint[0]].x)*(pos.x - GamePoint.pos[m_nIndexPoint[0]].x) +
			(pos.z - GamePoint.pos[m_nIndexPoint[0]].z)*(pos.z - GamePoint.pos[m_nIndexPoint[0]].z));
	}
	else
	{//	����ȊO
	 /*	��ԋ߂��|�C���g�Ǝ��̃|�C���g�̋���*/
		m_fLengthNextPoint = sqrtf((GamePoint.pos[m_nNumPoint].x - GamePoint.pos[m_nNumPoint + 1].x)*(GamePoint.pos[m_nNumPoint].x - GamePoint.pos[m_nNumPoint + 1].x) +
			(GamePoint.pos[m_nNumPoint].z - GamePoint.pos[m_nNumPoint + 1].z)*(GamePoint.pos[m_nNumPoint].z - GamePoint.pos[m_nNumPoint + 1].z));
		/*	�x�@�Ǝ��̃|�C���g�̋���*/
		m_fLenPoliceNextPit = sqrtf((pos.x - GamePoint.pos[m_nNumPoint + 1].x)*(pos.x - GamePoint.pos[m_nNumPoint + 1].x) +
			(pos.z - GamePoint.pos[m_nNumPoint + 1].z)*(pos.z - GamePoint.pos[m_nNumPoint + 1].z));
	}
	if (m_fLengthNextPoint > m_fLenPoliceNextPit)
	{//	�����̔�r
		if (m_nNumPoint == CAR_POINT_LAST)
		{//	�������|�C���g���ő�Ȃ烊�Z�b�g
			m_nNumPoint = m_nIndexPoint[0];
		}
		else
		{//����ȊO
			m_nNumPoint = m_nIndexPoint[m_nNumPoint] + 1;
		}
	}
}

//=============================================================================
//	���̃|�C���g�Ɍ�����
//=============================================================================
void  CCarPolice::GoPoint(D3DXVECTOR3 pos, int nRoute)
{
	//�|�C���g�̎擾
	CGame::Point GamePoint = CGame::GetCarPoint();

	if (GamePoint.nNum[m_nNumPoint] == m_nNumPoint)
	{//	�i�ޔԍ��̐ݒ�
	 //	�������|�C���g�ƃv���C���[�̋���
		float fLength = sqrtf((GamePoint.pos[m_nNumPoint].x - pos.x)*(GamePoint.pos[m_nNumPoint].x - pos.x) +
			(GamePoint.pos[m_nNumPoint].z - pos.z)*(GamePoint.pos[m_nNumPoint].z - pos.z));
		if (fLength >= CURVE_LENGTH)
		{// �ʒu��ۑ�����
			m_posSave = pos;
		}
		//	�v�Z�p�̒l[0]
		m_LengthValue[0] = D3DXVECTOR3(m_posSave.x + ((GamePoint.pos[m_nNumPoint].x - m_posSave.x) / 2),
			m_posSave.y, m_posSave.z + ((GamePoint.pos[m_nNumPoint].z - m_posSave.z) / 2));

		//	�Ԃ��Ȃ���ŏI�n�_
		D3DXVECTOR3 Value = GamePoint.pos[m_nNumPoint];
		if ((m_nNumPoint % 4) == 0)
		{//	�]�������o���Č����_�̍ŏI�n�_�̐ݒ� //	�ŏI�n�_�̒l
			Value.x = GamePoint.pos[m_nNumPoint].x - CURVE_LENGTH;
		}
		else if ((m_nNumPoint % 4) == 1)
		{//	�]�������o���Č����_�̍ŏI�n�_�̐ݒ� //	�ŏI�n�_�̒l
			Value.z = GamePoint.pos[m_nNumPoint].z - CURVE_LENGTH;
		}
		else if ((m_nNumPoint % 4) == 2)
		{//	�]�������o���Č����_�̍ŏI�n�_�̐ݒ� //	�ŏI�n�_�̒l
			Value.x = GamePoint.pos[m_nNumPoint].x + CURVE_LENGTH;
		}
		else if ((m_nNumPoint % 4) == 3)
		{//	�]�������o���Č����_�̍ŏI�n�_�̐ݒ� //	�ŏI�n�_�̒l
			Value.z = GamePoint.pos[m_nNumPoint].z + CURVE_LENGTH;
		}

		//	�v�Z�p�̒l[1]
		m_LengthValue[1] = D3DXVECTOR3(Value.x + ((GamePoint.pos[m_nNumPoint].x - Value.x) / 2),
			Value.y, Value.z + ((GamePoint.pos[m_nNumPoint].z - Value.z) / 2));

		//	[0][1]�̒��_
		m_CurvePosDest = D3DXVECTOR3(
			m_LengthValue[0].x - ((m_LengthValue[0].x - m_LengthValue[1].x) / 2),
			m_LengthValue[0].y,
			m_LengthValue[0].z - ((m_LengthValue[0].z - m_LengthValue[1].z) / 2));

		//	�J�[�u�̒��Ԓn�_�ƎԂ̋���
		float fLengthA = sqrtf((m_CurvePosDest.x - pos.x)*(m_CurvePosDest.x - pos.x) +
			(m_CurvePosDest.z - pos.z)*(m_CurvePosDest.z - pos.z));

		//	�ŏI�n�_�ƎԂ̋���
		float fLengthB = sqrtf((Value.x - pos.x)*(Value.x - pos.x) +
			(Value.z - pos.z)*(Value.z - pos.z));

		if (fLengthB <= 45 && m_nCurveSwitch == 0)
		{
			m_nRouteCounter += 1;
			if (m_nNumPoint == CAR_POINT_LAST)
			{//	�Ō�̃|�C���g����ŏ��̃|�C���g��

				bool bLoop = CCarBase::GetLoop();	//���[�v��Ԃ̎擾

				if (!bLoop)
				{//	�Ԃ����[�v���Ȃ�
					m_nNumPoint = m_nIndexPoint[1];	//	�������ꏊ�̃��Z�b�g
					m_nRouteCounter = 1;			//	���[�g���Z�b�g
					SetPosition(D3DXVECTOR3(GamePoint.pos[m_nIndexPoint[0]].x, 0.0f, GamePoint.pos[m_nIndexPoint[0]].z));
				}
				else
				{//	�Ԃ����[�v���Ȃ�
					m_nNumPoint = m_nIndexPoint[0];	//	�������ꏊ�̃��Z�b�g
					m_nRouteCounter = 0;			//	���[�g���Z�b�g
				}
			}
			else
			{//	���[�g�̎��̐ݒ�
				m_nNumPoint = m_nIndexPoint[m_nRouteCounter];
			}


		}
		//	�Ԃ̌������|�C���g�̐؂�ւ�
		switch (m_nCurveSwitch)
		{
		case 0:	//	���̃|�C���g�֌�����
			m_VecA = GamePoint.pos[m_nNumPoint] - pos;
			//	���̓�����
			if (fLength <= CURVE_LENGTH && fLengthA < 100) { m_nCurveSwitch = 1; }
			break;
		case 1:	//	�J�[�u�̒��Ԓn�_�Ɍ�����
			m_VecA = m_CurvePosDest - pos;
			//	���̓�����
			if (fLengthA <= CURVE_LENGTH_NEXT) { m_nCurveSwitch = 2; }
			break;
		case 2:	//	�J�[�u�̍ŏI�n�_�֌�����
			m_VecA = Value - pos;
			//	���̓�����
			if (fLengthB <= LAST_CURVE_LENGTH) { m_nCurveSwitch = 0; }
			break;
		}
	}
}
