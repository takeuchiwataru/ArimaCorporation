//=============================================================================
//
// �I�u�W�F�N�g�̏��� [object.cpp]
// Author : �L�n���u
//
//=============================================================================
#include "scene.h"
#include "model3D.h"
#include "manager.h"
#include "object.h"
#include "meshfield.h"
#include "player.h"
#include "fade.h"
#include "shadow.h"
#include "tutorial.h"
#include "Character.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_SPEED				(5.0f)
#define OBJCT_ANGLE_REVISION	(0.2f)		// �p�x�␳
#define EFFECT_HIGHT			(250.0f)	// �G�~�b�^�[�̍���
#define FOUNTAIN_UP				(20.0f)		// �����̏㏸������l

//�X�V�͈�
#define FOUNTAIN_LENGTH			(15000)		//�����̍X�V�͈�
#define LEAF_LENGTH				(10500)		//�t�̍X�V����
#define WOOD_LENGTH				(8000)		//�؂��r���{�[�h�ɓ���ւ��鋗��
#define FLOWER_LENGTH			(5500)		//�Ԓd�̕`��͈�

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
D3DXVECTOR3 CObject::m_LoadVtxMaxModel[MAX_OBJECT] = {};
D3DXVECTOR3 CObject::m_LoadVtxMinModel[MAX_OBJECT] = {};

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CObject::CObject(int nPriority) : CModel3D(nPriority, CScene::OBJTYPE_OBJECT)
{
	m_ModelMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Spin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nStageCount = 0;
	m_bStageNext = false;
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CObject::~CObject() {}

//=============================================================================
// ����������
//=============================================================================
HRESULT CObject::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//3D���f����Init
	CModel3D::Init();

	// �ʒu�̏�����
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�e�N�X�`���ԍ��̏�����
	m_nTex = -1;

	//�ʒu�̑��
	CModel3D::SetPosition(pos);

	// �e����̑��
	CModel3D::SetScale(m_scale);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CObject::Uninit(void)
{
	//�I�u�W�F�N�g�r���{�[�h
	m_pObjBill = NULL;

	//3D���f����Uninit
	CModel3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CObject::Update(void)
{
	CModel3D::Update();

	//�����̎擾
	D3DXVECTOR3 &m_rot = CModel3D::GetRot();

	if (m_nType == 6)
	{
		m_rot.y -= 0.0004f;
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void CObject::Draw(void)
{
	//if (m_nType == 2) { return; }

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�ʒu�̏�����
	D3DXVECTOR3 Modelpos = CModel3D::GetPosition();
	//�Q�[���̏��
	CManager::MODE pMode = CManager::GetMode();

	//���_�@���̎������K��
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	//�`�揈��
	CModel3D::Draw();

	//���_�@���̎������K��
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}
//===============================================================================
//�@�N���G�C�g
//===============================================================================
CObject * CObject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, float move, int nTexType, int nObjectType, int nCollision)
{
	CObject *pObject = NULL;
	int nPriority = 3;

	// NULL�`�F�b�N
	if (pObject == NULL)
	{// �������̓��I�m��

		switch (nObjectType)
		{
		case CModel3D::MODEL_TYPE_MAP_FIRST:
		case CModel3D::MODEL_TYPE_MAP_SECOND:
		case CModel3D::MODEL_TYPE_BRIDGE:
		case CModel3D::MODEL_TYPE_MAP_MOUNTAIN:
		case CModel3D::MODEL_TYPE_MAP_SKY:

			nPriority = 0;
			break;
		}
		pObject = new CObject(nPriority);

		if (pObject != NULL)
		{
			// ���f���̐ݒ�
			pObject->SetModelType(nObjectType);
			// �e�N�X�`���̐ݒ�
			pObject->SetTextureType(nTexType);
			// �I�u�W�F�N�g���Ƃ̐ݒ�p�^�C�v
			pObject->m_nType = nObjectType;
			// �T�C�Y����
			pObject->m_scale = scale;
			// �T�C�Y��e�N���X�ɑ��
			pObject->SetScale(scale);
			// �I�u�W�F�N�g�N���X�̐���
			pObject->Init();
			//�e�N�X�`���ԍ�����
			pObject->m_nTex = nTexType;
			// �ʒu����
			pObject->SetPosition(pos);
			// ��]�𔽉f
			pObject->SetRot(rot);
			//pObject->m_move = D3DXVECTOR3(move, move, move);
			// �R���W������ONOFF
			pObject->m_nCollision = nCollision;

			CCharcter::ResetCheck(pos, pObject->GetfCola());
		}
	}

	return pObject;
}
//===============================================================================
// X�t�@�C���̓ǂݍ���
//===============================================================================
HRESULT CObject::Load(void)
{
	return S_OK;
}
//===============================================================================
// �e�N�X�`���̔j��
//===============================================================================
void CObject::UnLoad(void)
{

}
//=============================================================================
//	�X�e�[�W�ړ����ɏ��������邽��
//=============================================================================
void CObject::DeleteObject(void)
{
	CScene *pScene;

	//�v���C�I���e�B�[�`�F�b�N
	pScene = CScene::GetTop(OBJECT_PRIOTITY);

	//NULL�`�F�b�N
	while (pScene != NULL)
	{
		//Update��Uninit����Ă��܂��ꍇ�@Next��������\�������邩��Next�Ƀf�[�^���c���Ă���
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetDeath() == false)
		{//�^�C�v���ǂ�������
			if (pScene->GetObjType() == OBJTYPE_OBJECT)
			{
				pScene->Uninit();
			}
		}
		//Next�Ɏ���Scene������
		pScene = pSceneNext;
	}
}

//=============================================================================
// �����ɂ��ړ��ʂ̕ω�
//=============================================================================
D3DXVECTOR3 CObject::Fountain(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	D3DXVECTOR3 posObj = CModel3D::GetPosition();	// �ʒu�̎擾
	D3DXVECTOR3 vec = pos - posObj;					// �������瑊������ւ̃x�N�g��

													// �x�N�g���̒��������߂�
	float fLengthVec = D3DXVec3Length(&vec);		// �������瑊������ւ̃x�N�g���̒���
	float fLengthMove = D3DXVec3Length(&move);		// ����̈ړ��ʂ̒���

	D3DXVECTOR3 vecMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �p�x�����߂�
	float fAngle = acosf(move.x / sqrt(move.x * move.x + move.z * move.z));
	if (move.z < 0.0f)fAngle = 2.0f * D3DX_PI - fAngle;
	if (isnan(fAngle))return vecMove;

	D3DXVECTOR3 vtxMax = CModel3D::GetVtxMax(GetModelType());		// ���_���W�̍ő�l�̎擾

	int nDigit = (int)log10f(vtxMax.x) + 1;
	if (6 < nDigit)return move;

	nDigit = (int)log10f(vtxMax.z) + 1;
	if (6 < nDigit)return move;

	// �ړ��ʂ̌v�Z
	if (m_nType == 33)
	{// ���ΐ�
		vecMove = D3DXVECTOR3(0.8f * (move.x + (cosf(fAngle) * vtxMax.x)), FOUNTAIN_UP * 0.4f, 0.8f * (move.z + (sinf(fAngle) * vtxMax.z)));
	}
	else
	{// ����
		vecMove = D3DXVECTOR3(0.05f * (move.x + (cosf(fAngle) * vtxMax.x)), FOUNTAIN_UP, 0.05f * (move.z + (sinf(fAngle) * vtxMax.z)));
	}

	return vecMove;
}

//===============================================================================
// �����蔻��
//===============================================================================
bool CObject::CollisionObject(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld, D3DXVECTOR3 * pMove)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();

	//�������Ă��邩�������ĂȂ���
	bool bLand = false;

	switch (m_nCollision)
	{
	case 1:

		// �e����̎擾
		D3DXVECTOR3 ModelPos = CModel3D::GetPosition();		// �ʒu
		D3DXVECTOR3 ModelMove = CModel3D::GetMove();		// �ړ���
		D3DXVECTOR3 &VtxMax = CModel3D::GetVtxMax(GetModelType());			// ���f���̍ő�l
		D3DXVECTOR3 &VtxMin = CModel3D::GetVtxMin(GetModelType());			// ���f���̍ŏ��l
		D3DXVECTOR3 rot = CModel3D::GetRot();

		D3DXVECTOR3 ModelMax = CModel3D::GetPosition() + CModel3D::GetVtxMax(GetModelType());	// �ʒu���݂̍ő�l
		D3DXVECTOR3 ModelMin = CModel3D::GetPosition() + CModel3D::GetVtxMin(GetModelType());	// �ʒu���݂̍ŏ��l
																								// �ړ��ʂ̕ێ�
		if (ModelMove.x == 0.0f) ModelMove.x = m_ModelMove.x;
		if (ModelMove.y == 0.0f) ModelMove.y = m_ModelMove.y;
		if (ModelMove.z == 0.0f) ModelMove.z = m_ModelMove.z;

		if (pPos->x >= ModelMin.x - PLAYER_DEPTH && pPos->x <= ModelMax.x + PLAYER_DEPTH)
		{// Z�͈͓̔��ɂ���
			if (pPos->z >= ModelMin.z - PLAYER_DEPTH && pPos->z <= ModelMax.z + PLAYER_DEPTH)
			{// X�͈͓̔��ɂ���
				if (pPosOld->y >= ModelMax.y && pPos->y <= ModelMax.y)
				{// �I�u�W�F�N�g�̏ォ�瓖����ꍇ
					pPos->y = ModelMax.y + abs(ModelMove.y);
					pMove->y = 0.0f;

					if (ModelMove.x != 0.0f)
					{// X�ړ�����
						pPos->x += ModelMove.x;
					}

					if (ModelMove.z != 0.0f)
					{// Z�ړ�����
						pPos->z += ModelMove.z;
					}

					bLand = true;
				}
				else if (pPosOld->y + PLAYER_HEIGHT <= ModelMin.y && pPos->y + PLAYER_HEIGHT >= ModelMin.y)
				{// �I�u�W�F�N�g�̉����瓖����ꍇ
					pPos->y = ModelMin.y - PLAYER_HEIGHT;
					pMove->y = 0.0f;
				}

				if (!(pPos->y >= ModelMax.y) && !(pPos->y + PLAYER_HEIGHT <= ModelMin.y))
				{// �I�u�W�F�N�g���Ƃ̓����蔻��
					if (pPosOld->x - PLAYER_DEPTH >= ModelMax.x + rot.x && pPos->x - PLAYER_DEPTH < ModelMax.x + rot.x)
					{// �E���甲�����ꍇ
						pPos->x = ModelMax.x + PLAYER_DEPTH;
					}
					else if (pPosOld->x + PLAYER_DEPTH <= ModelMin.x && pPos->x + PLAYER_DEPTH > ModelMin.x)
					{// �����甲�����ꍇ
						pPos->x = ModelMin.x - PLAYER_DEPTH;
					}

					if (pPosOld->z - PLAYER_DEPTH >= ModelMax.z && pPos->z - PLAYER_DEPTH < ModelMax.z)
					{// �����甲�����ꍇ
						pPos->z = ModelMax.z + PLAYER_DEPTH + 0.1f;
					}
					else if (pPosOld->z + PLAYER_DEPTH <= ModelMin.z && pPos->z + PLAYER_DEPTH > ModelMin.z)
					{// ��O���甲�����ꍇ
						pPos->z = ModelMin.z - PLAYER_DEPTH - 0.1f;
					}
				}
			}
		}

		// �ʒu�̑��
		CModel3D::SetPosition(ModelPos);
		// �ړ��ʂ̑��
		m_ModelMove = ModelMove;

		break;
	}

	return bLand;
}
