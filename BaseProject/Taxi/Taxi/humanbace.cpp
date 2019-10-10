//=============================================================================
//
// ���q����̏��� [customer.cpp]
// Author : �������� Sato_Asumi
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "humanbace.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "loadText.h"
#include "meshfield.h"
#include "shadow.h"
#include "gamecamera.h"
#include "tutorial.h"
#include "shadow.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define FILE_TEXTURE00	("data\\TEXTURE\\modeltex\\human_boy.jpg")	//�ǂݍ��ރe�N�X�`��
#define FILE_TEXTURE01	("data\\TEXTURE\\modeltex\\human_girl.jpg")	//�ǂݍ��ރe�N�X�`��
#define ESCAPE_SPEED	(5.0f)										//������X�s�[�h
#define GRAVITY			(0.3f)										//�d��
#define SCALE			(D3DXVECTOR3(1.4f, 1.4f, 1.4f))				//���f���̃X�P�[��
#define SHADOW_SIZE		(30.0f)										//�e�̃T�C�Y

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CHumanBace::m_pTexture[CHumanBace::MODEL_MAX] = {};
int CHumanBace::m_nMaxMotion[CHumanBace::MODEL_MAX] = {};
CMotion::MOTION_INFO * CHumanBace::m_pMotionInfo[CHumanBace::MODEL_MAX] = {};
CLoadTextMotion::MODEL_INFO CHumanBace::m_ModelInfo[CHumanBace::MODEL_MAX];

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CHumanBace::CHumanBace(int nPriority, CScene::OBJTYPE type) : CScene(nPriority, type){}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CHumanBace::~CHumanBace(){}

//=============================================================================
// �t�@�C���̓ǂݍ���
//=============================================================================
void CHumanBace::LoadModel(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, FILE_TEXTURE00, &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, FILE_TEXTURE01, &m_pTexture[1]);

	//���[�V�������̎擾
	CManager::MODE mode = CManager::GetMode();
	CLoadTextMotion * pLoadTextBoy = NULL;
	CLoadTextMotion * pLoadTextGirl = NULL;

	if (mode == CManager::MODE_GAME)
	{//�Q�[��
		pLoadTextBoy = CGame::GetBoyMotion();
		pLoadTextGirl = CGame::GetGirlMotion();
	}
	else if (mode == CManager::MODE_TUTORIAL)
	{//�`���[�g���A��
		pLoadTextBoy = CTutorial::GetBoyMotion();
		pLoadTextGirl = CTutorial::GetGirlMotion();
	}

	m_pMotionInfo[0] = pLoadTextBoy->GetMotionInfo();	//�j�����[�V�������̎擾
	m_pMotionInfo[1] = pLoadTextGirl->GetMotionInfo();	//�������[�V�������̎擾

	m_nMaxMotion[0] = pLoadTextBoy->GetMaxMotion();		//�j�����[�V�����̍ő吔�̎擾
	m_nMaxMotion[1] = pLoadTextGirl->GetMaxMotion();	//�������[�V�����̍ő吔�̎擾

	//���f�������擾
	m_ModelInfo[0] = pLoadTextBoy->GetModelInfo();		//�j�����f���̏����擾
	m_ModelInfo[1] = pLoadTextGirl->GetModelInfo();		//�������f���̏����擾
}

//=============================================================================
// �t�@�C���̔j��
//=============================================================================
void CHumanBace::UnloadModel(void)
{
	//���[�V�����f�[�^�̔j��
	for (int nCntModelInfo = 0; nCntModelInfo < MODEL_MAX; nCntModelInfo++)
	{
		if (m_pMotionInfo[nCntModelInfo] != NULL)
		{
			m_pMotionInfo[nCntModelInfo] = NULL;
		}
	}

	//�e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < MODEL_MAX; nCntTex++)
	{
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CHumanBace::Init(void)
{
	//�ϐ��̏�����
	m_pos = VECTOR_ZERO;			//���S���W
	m_move = VECTOR_ZERO;			//�ړ�
	m_rot = VECTOR_ZERO;			//����
	m_vtxMax = VECTOR_ZERO;			//���f�����_�̍ő�l(Y)
	m_nCountTime = 0;				//�J�E���^�[
	m_pShadow = NULL;				//�e�|�C���^�̔j��
	m_pModel = NULL;				//���f���f�[�^�̔j��
	m_pMotion = NULL;				//���[�V�����|�C���^
	m_MotionType = MOTION_NEUTRAL;	//���[�V�����̏�Ԑݒ�
	m_bClipping = true;				//�N���b�s���O�t���O
	m_fLength = 0.0f;				//�`��͈�
	m_MotionLength = 0.0f;			//���[�V�����̍X�V�͈�

	CreateModel();		//���f���̐���
	CreateMotion();		//���[�V�����̐���

	//�e�̐���
	if (m_pShadow == NULL) { m_pShadow = CShadow::Create(m_pos, SHADOW_SIZE, SHADOW_SIZE, 0); }

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CHumanBace::Uninit(void)
{
	//�e�|�C���^�̏�����
	m_pShadow = NULL;

	//���[�V�����̔j��
	if (m_pMotion != NULL)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = NULL;
	}

	//���f���f�[�^�̔j��
	for (int nCntPart = 0; nCntPart < m_ModelInfo[(int)m_ModelType].nMaxParts; nCntPart++)
	{
		if (m_pModel[nCntPart] != NULL)
		{
			m_pModel[nCntPart]->Uninit();
			delete m_pModel[nCntPart];
			m_pModel[nCntPart] = NULL;
		}
	}
	if (m_pModel != NULL)
	{
		delete[] m_pModel;
		m_pModel = NULL;
	}

	//���S�t���O�𗧂Ă�
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CHumanBace::Update(void)
{
	//�ϐ��錾
	bool bDraw = false;
	CManager::MODE mode = CManager::GetMode();

	//�N���b�s���O����
	if (m_bClipping == true)
	{
		D3DXVECTOR3 vtxMax = m_pos + m_vtxMax;
		D3DXVECTOR3 vtxMin = m_pos + m_vtxMin;
		CGameCamera * pGameCamera = NULL;

		if (mode == CManager::MODE_GAME)
		{//�Q�[��
			pGameCamera = CGame::GetGameCamera();
		}
		else if (mode == CManager::MODE_TUTORIAL)
		{//�`���[�g���A��
			pGameCamera = CTutorial::GetCamera();
		}

		//�N���b�s���O����
		bDraw = pGameCamera->Clipping(vtxMax, vtxMin);
		SetDraw(bDraw);
	}
	
	if(m_bClipping){ m_move.y -= GRAVITY; }		//�d�͉��Z
		
	m_pos += m_move;		//�ړ�����

	if (m_bClipping) { CollisionMesh(); }		//�n�ʂ̂����蔻��

	//�e�̈ړ�
	if (m_pShadow != NULL) { m_pShadow->SetPositionShadow(D3DXVECTOR3(m_pos.x, m_pos.y + 0.1f, m_pos.z)); }

	//���[�V�����̍X�V����
	if (m_pMotion != NULL)
	{
		//�����𔻒肷��
		if ((m_MotionType == MOTION_NEUTRAL) && (m_fLength > m_fLength)) { return; }

		int nNumMotion = m_pMotion->Update();

		//���[�V�����̃^�C�v�ݒ�
		if (nNumMotion != m_MotionType) { m_MotionType = (MOTION_TYPE)nNumMotion; }
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CHumanBace::Draw(float fAlpha)
{
	//�����_�����O�N���X���擾
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX		  mtxRot, mtxTrans;			// �v�Z�p�}�g���b�N�X

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

	//���_�@���̎������K��
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	//���f���̕`��
	for (int nCntPart = 0; nCntPart < m_ModelInfo[(int)m_ModelType].nMaxModel; nCntPart++)
	{
		if (m_pModel[nCntPart] != NULL)
		{
			//�`�悷��
			m_pModel[nCntPart]->Draw(fAlpha);
		}
	}

	//���_�@���̎������K��
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}

//=============================================================================
// �ӂ̒��������߂�
//=============================================================================
float CHumanBace::GetLength(D3DXVECTOR3 StartPos, D3DXVECTOR3 EndPos)
{
	D3DXVECTOR3 Distance = StartPos - EndPos;
	float fLength = sqrtf((Distance.x * Distance.x) + (Distance.z * Distance.z));

	return fLength;
}

//=============================================================================
// �p�x�C��
//=============================================================================
void CHumanBace::RemakeAngle(float * pAngle)
{
	//�ړI�̊p�x�C��
	if (*pAngle < -D3DX_PI)
	{
		*pAngle += D3DX_PI * 2.0f;
	}
	if (*pAngle > D3DX_PI)
	{
		*pAngle -= D3DX_PI * 2.0f;
	}
}

//=============================================================================
// �����鏈��
//=============================================================================
void CHumanBace::MoveEscape(void)
{
	//���Ԃ̉��Z
	m_nCountTime++;

	//�v���C���[�Ƃ��q����̊p�x�����߂�
	CManager::MODE mode = CManager::GetMode();
	CPlayer * pPlayer = NULL;

	if (mode == CManager::MODE_GAME)
	{//�Q�[��
		pPlayer = CGame::GetPlayer();
	}
	else if (mode == CManager::MODE_TUTORIAL)
	{//�`���[�g���A��
		pPlayer = CTutorial::GetPlayer();
	}

	D3DXVECTOR3 Distance = m_pos - pPlayer->GetPos();
	float fAngle = atan2f(Distance.x, Distance.z);
	RemakeAngle(&fAngle);

	//�ړ�������
	m_move.x = sinf(fAngle) * ESCAPE_SPEED;
	m_move.z = cosf(fAngle) * ESCAPE_SPEED;

	//����������Ɍ�������
	m_rot.y = fAngle + D3DX_PI;
	RemakeAngle(&m_rot.y);
}

//=============================================================================
// �n�ʂ̂����蔻��
//=============================================================================
void CHumanBace::CollisionMesh(void)
{
	CScene *pScene = CScene::GetTop(MESH_PRIOTITY);

	//NULL�`�F�b�N
	while (pScene != NULL)
	{
		//Update��Uninit����Ă��܂��ꍇ�@Next��������\�������邩��Next�Ƀf�[�^���c���Ă���
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetObjType() == CScene::OBJTYPE_GROUND)
		{//�^�C�v���n�ʂ�������
			CMeshField *pField = (CMeshField*)pScene;

			if (pField->OnField(m_pos, m_vtxMax.y))
			{// �X�΂̌v�Z
				float fHight = pField->GetHeightMesh(m_pos);

				if (m_pos.y <= fHight)
				{
					m_pos.y = fHight;	//�n�ʂ̍������擾
					m_move.y = 0.0f;	//�ړ��ʂ�����������
					break;
				}
			}
		}
		//Next�Ɏ���Scene������
		pScene = pSceneNext;
	}
}

//=============================================================================
// ���_�̍ő�l���擾
//=============================================================================
D3DXVECTOR3 CHumanBace::GetVtxMax(void)
{
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);

	if (NULL != m_pModel)
	{// ���f�����g���Ă���Ƃ�
		for (int nCntParts = 0; nCntParts < m_ModelInfo[(int)m_ModelType].nMaxParts; nCntParts++)
		{// �p�[�c�����[�v
			D3DXVECTOR3 vtxMaxModel = m_pModel[nCntParts]->GetVtxMax();
			D3DXVECTOR3 Scale = m_pModel[nCntParts]->GetScale();

			if (vtxMaxModel.x * Scale.x > vtxMax.x)
			{// X�̔�r
				vtxMax.x = vtxMaxModel.x * Scale.x;
			}

			if (vtxMaxModel.y * Scale.y > vtxMax.y)
			{// Y�̔�r
				vtxMax.y = vtxMaxModel.y * Scale.y;
			}

			if (vtxMaxModel.z * Scale.z > vtxMax.z)
			{// Z�̔�r
				vtxMax.z = vtxMaxModel.z * Scale.z;
			}
		}
	}

	return vtxMax;
}

//=============================================================================
// ���_�̍ŏ��l�̎擾
//=============================================================================
D3DXVECTOR3 CHumanBace::GetVtxMin(void)
{
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);

	if (NULL != m_pModel)
	{// ���f�����g���Ă���Ƃ�
		for (int nCntParts = 0; nCntParts < m_ModelInfo[(int)m_ModelType].nMaxParts; nCntParts++)
		{// �p�[�c�����[�v
			D3DXVECTOR3 vtxMinModel = m_pModel[nCntParts]->GetVtxMin();
			D3DXVECTOR3 Scale = m_pModel[nCntParts]->GetScale();

			if (vtxMinModel.x * Scale.x < vtxMin.x)
			{// X�̔�r
				vtxMin.x = vtxMinModel.x * Scale.x;
			}

			if (vtxMinModel.y * Scale.y < vtxMin.y)
			{// Y�̔�r
				vtxMin.y = vtxMinModel.y * Scale.y;
			}

			if (vtxMinModel.z * Scale.z < vtxMin.z)
			{// Z�̔�r
				vtxMin.z = vtxMinModel.z * Scale.z;
			}
		}
	}

	return vtxMin;
}

//=============================================================================
// ���f���̐���
//=============================================================================
void CHumanBace::CreateModel(void)
{
	//���f�����𓮓I�m�ۂ���
	m_pModel = new CModel *[m_ModelInfo[(int)m_ModelType].nMaxParts];

	//���f���̐���
	for (int nCntParts = 0; nCntParts < m_ModelInfo[(int)m_ModelType].nMaxParts; nCntParts++)
	{
		if (nCntParts == 0)
		{
			m_pModel[nCntParts] = CModel::Create(m_ModelInfo[(int)m_ModelType].pOfSetPos[nCntParts], &m_ModelInfo[(int)m_ModelType].paFileName[nCntParts][0], SCALE);
		}
		else
		{
			m_pModel[nCntParts] = CModel::Create(m_ModelInfo[(int)m_ModelType].pOfSetPos[nCntParts], &m_ModelInfo[(int)m_ModelType].paFileName[nCntParts][0]);
		}

		//�e�N�X�`���̊�����
		if (m_pModel[nCntParts] != NULL) { m_pModel[nCntParts]->BindTexture(m_pTexture[(int)m_ModelType]); }
	}

	//���f���̐e�ݒ�
	for (int nCntParts = 0; nCntParts < m_ModelInfo[(int)m_ModelType].nMaxParts; nCntParts++)
	{
		if (nCntParts == 0)
		{
			m_pModel[0]->SetParent(NULL);
		}
		else
		{
			m_pModel[nCntParts]->SetParent(m_pModel[m_ModelInfo[(int)m_ModelType].pParent[nCntParts]]);
		}
	}

	//���f�����_�̍ő�l�E�ŏ��l�̎擾
	m_vtxMax = GetVtxMax();
	m_vtxMin = GetVtxMin();
}

//=============================================================================
// ���[�V�����̐���
//=============================================================================
void CHumanBace::CreateMotion(void)
{
	if (m_pMotion == NULL)	//���[�V�����̐���
	{
		m_pMotion = CMotion::Create();

		if (m_pMotion != NULL)
		{
			//���f���f�[�^�̐ݒ�
			m_pMotion->SetModel(m_pModel);									//���[�V�����f�[�^�Ƀ��f������n��
			m_pMotion->SetMaxModel(m_ModelInfo[m_ModelType].nMaxParts);		//���f���p�[�c�̍ő吔��n��

																			//���[�V�����f�[�^�̐ݒ�
			m_pMotion->SetMotion(m_pMotionInfo[m_ModelType]);				//���[�V�������̎擾
			m_pMotion->SetMaxMotion(CHumanBace::m_nMaxMotion[m_ModelType]);	//���[�V�����̍ő吔�̎擾

			//����������
			m_pMotion->Init();
		}
	}
}

//=============================================================================
// ���̃��[�V������ݒ肷��
//=============================================================================
void CHumanBace::SetNextMotion(MOTION_TYPE type)
{
	m_MotionType = type;	//���[�V�����̃^�C�v�ݒ�

	if (m_pMotion != NULL) { m_pMotion->SetNumMotion((int)m_MotionType); }	//���[�V������ݒ肷��
}

//=============================================================================
// �e�̃��l�̐F�̐ݒ�
//=============================================================================
void CHumanBace::SetShadowColor_A(float col_a)
{
	if (m_pShadow != NULL)
	{
		m_pShadow->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, col_a));
	}
}
