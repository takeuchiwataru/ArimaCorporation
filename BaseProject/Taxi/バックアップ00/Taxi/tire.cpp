//=============================================================================
//
// �^�C���̏��� [tire.cpp]
// Author : ��������
//
//=============================================================================
#include "tire.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "gamecamera.h"
#include "game.h"
#include "player.h"
#include "incline.h"
#include "scene.h"
#include "mesh.h"
#include "meshfield.h"
#include "tutorial.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FILE_NAME		("data/MODEL/�^�C��/tire.x")	//�ǂݍ��ރt�@�C����
#define FILE_TEXTURE	("data/TEXTURE/tire.jpg")		//�e�N�X�`���̓ǂݍ���
#define ADD_ROT			(0.2f)							//�^�C���̉�]��
#define OFFSET_WIDTH	(26.0f)							//����
#define GRASS_TEXNUM	(3)								//���ނ�̃e�N�X�`���ԍ�

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CTire::m_pTexture = NULL;
float CTire::m_fHight = 0.0f;
bool CTire::m_bJump = false;
bool CTire::m_bPass = false;

//=============================================================================
// ��������
//=============================================================================
CTire * CTire::Create(const D3DXVECTOR3 pos)
{
	//�C���X�^���X�̐���
	CTire * pModel;
	pModel = new CTire;

	//����������
	pModel->Init();

	//�ݒ菈��
	pModel->SetPos(pos);
	pModel->SetFirstPos(pos);

	return pModel;
}

//===============================================================================
//�@�e�N�X�`���̓ǂݍ���
//===============================================================================
void CTire::LoadTexture(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice,
							  FILE_TEXTURE,
							  &m_pTexture);
}

//===============================================================================
//�@�e�N�X�`���̔j��
//===============================================================================
void CTire::UnloadTexture(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//===============================================================================
//�@�R���X�g���N�^
//===============================================================================
CTire::CTire()
{
	m_pMesh = NULL;								//���b�V�����ւ̃|�C���^
	m_pBuffMat = NULL;							//�}�e���A���̏��ւ̃|�C���^
	m_nNumMat = 0;								//�}�e���A���̏��
	m_VtxMin, m_VtxMax;							//���f���̍ŏ��l�E�ő�l
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
	m_FirstPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�����ʒu
	m_pTextures = NULL;
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CTire::~CTire(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTire::Init(void)
{
	//�ϐ��̏�����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fvtxMaxY = 0.0f;

	CModel::Init(FILE_NAME);
	CModel::BindTexture(m_pTexture);

	//���f���̒��_�ő�l���擾
	m_fvtxMaxY = GetVtxMax().y;

	m_pIncline = CIncline::Create();
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTire::Uninit(void)
{
	if (m_pIncline != NULL)
	{// �X�Ώ����̔j��
		m_pIncline->Uninit();
		m_pIncline = NULL;
	}

	m_fHight = 0.0f;	//�����̏�����
	m_bPass = false;	//�����ʉ߃t���O��������

	CModel::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTire::Update(int nCntTire, float fGravity)
{
	//�ϐ��錾
	float fAngle = 0.0f;
	CManager::MODE mode = CManager::GetMode();
	CPlayer * pPlayer = NULL;

	if (mode == CManager::MODE_GAME)
	{//�Q�[��
		pPlayer = CGame::GetPlayer();		//�v���C���[�̎擾
	}
	else if (mode == CManager::MODE_TUTORIAL)
	{//�`���[�g���A��
		pPlayer = CTutorial::GetPlayer();	//�v���C���[�̎擾
	}

	if (mode == CManager::MODE_RESULT) { return; }

	CPlayer::STATE_SPEED PlayerStateSpeed = pPlayer->GetStateSpeed();
	CPlayer::STATE_HANDLE PlayerHandle = pPlayer->GetStateHandle();
	bool bGravity = false;
	CModel::Update();

	//�X�Ώ���
	if (nCntTire == 0) { SetIncline(pPlayer); }

	//�d�͂̉��Z
	if ((PlayerStateSpeed != CPlayer::STATE_SPEED_BRAKS) && (pPlayer->GetSmoke() == false))
	{
		m_move.y -= cosf(0) * fGravity;
		bGravity = true;
	}

	if (bGravity == true)
	{//�d�͂��L����������
		if ((PlayerHandle == CPlayer::HANDLE_LEFT) || (PlayerHandle == CPlayer::HANDLE_RIGHT))
		{
			m_move.y -= cosf(0) * fGravity;
		}
	}

	//�W�����v��ԂȂ�d�͂𑫂�
	if((!bGravity) && (pPlayer->GetJump() == true))
	{
		m_move.y -= cosf(0) * fGravity; 
	}

	//���������߂�
	if (nCntTire == 0)
	{
		m_move.y = m_fHight;
	}
	else if ((nCntTire == 2))
	{
		m_move.y = m_fHight;
	}
	else if (m_bJump == true)
	{
		m_move.y = m_fHight * 0.25f;
	}

	//�ړ��ʂ̉��Z
	m_WorldPos.y += m_move.y;

	//�����̍X�V����
	UpdateField(pPlayer);

	//�^�C���̌����̏���
	if (pPlayer->GetStateHandle() == CPlayer::HANDLE_LEFT)
	{
		//�p�x�����߂�
		fAngle = D3DX_PI * 0.7f;
		RemakeAngle(&fAngle);

		//�p�x�̐ݒ�
		m_Rot.y = fAngle;
	}
	else if (pPlayer->GetStateHandle() == CPlayer::HANDLE_RIGHT)
	{
		//�p�x�����߂�
		fAngle = D3DX_PI * -0.7f;
		RemakeAngle(&fAngle);

		//�p�x�̐ݒ�
		m_Rot.y = fAngle;
	}
	else
	{
		fAngle = D3DX_PI;	//�p�x�����߂�
		m_Rot.y = fAngle;	//�p�x�̐ݒ�
	}

	//�^�C���̉�]����
	if (pPlayer->GetStateSpeed() != CPlayer::STATE_SPEED_STOP)
	{
		m_Rot.x -= ADD_ROT;
		RemakeAngle(&m_Rot.z);
	}

	//�f�o�b�O�\��
	//CDebugProc::Print("���[���h���W : X %.2f, Y %.2f, Z %.2f\n", m_WorldPos.x, m_WorldPos.y, m_WorldPos.z);
	//CDebugProc::Print("�W�����v�̍��� : %.2f\n", m_fHight);
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTire::Draw(float fAlpha)
{
	CModel::Draw(1.0f);

	//���[���h���W�̎擾
	m_WorldPos = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);
}

//=============================================================================
// �^�C���̍�������
//=============================================================================
void CTire::UpdateField(CPlayer * pPlayer)
{
	CScene *pScene = CScene::GetTop(MESH_PRIOTITY);
	D3DXVECTOR3 PlayerPos = pPlayer->GetPos();

	//�n�ʂɏ���Ă��邩�ǂ���
	m_bLand = false;

	//NULL�`�F�b�N
	while (pScene != NULL)
	{
		//Update��Uninit����Ă��܂��ꍇ�@Next��������\�������邩��Next�Ƀf�[�^���c���Ă���
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetObjType() == CScene::OBJTYPE_GROUND)
		{//�^�C�v���n�ʂ�������
			CMeshField *pField = (CMeshField*)pScene;

			if (pField->OnField(m_WorldPos, m_fvtxMaxY))
			{// �X�΂̌v�Z
				float fHight = pField->GetHeightMesh(m_WorldPos);
				
				if (m_WorldPos.y <= fHight + TIRE_HIGHT)
				{
					m_WorldPos.y = fHight + TIRE_HIGHT;	//�n�ʂ̍������擾
					m_move.y = 0.0f;					//�ړ��ʂ�����������
					m_bLand = true;

					pPlayer->SetGrassEffect(false);	//���̃G�t�F�N�g�𐶐���Ԃł͂Ȃ�����

					//�W�����v�����ǂ���
					bool bJumpFlag = pPlayer->GetJump();
					if(bJumpFlag)
					{
						pPlayer->GetPlayerInfoPoint()->nCountTime -= (int)std::round(pPlayer->GetPlayerInfoPoint()->nCountTime * 0.05f);
					}
					else
					{
						if (pField->GetMeshType() == GRASS_TEXNUM)
						{//���ނ�̃e�N�X�`���Ȃ�
							pPlayer->SetGrassEffect(true);	//���̃G�t�F�N�g������Ԃɂ���
						}
					}

					//�W�����v�̏�Ԑݒ�
					pPlayer->SetJump(false);
					m_pIncline->SetJumpFlag(false);

					//�v���C���[�̂����̈ړ��ʂ�������
					D3DXVECTOR3 PlayerMove = pPlayer->GetMove();
					PlayerMove.y = 0.0f;
					pPlayer->SetMove(PlayerMove);
					break;
				}
			}
		}
		//Next�Ɏ���Scene������
		pScene = pSceneNext;
	}
}

//=============================================================================
// �X�΂̏���
//=============================================================================
void CTire::SetIncline(CPlayer * pPlayer)
{
	//�W�����v�t���O�̎擾
	bool bJump = pPlayer->GetJump();
	m_bJump = bJump;

	if (NULL != m_pIncline)
	{// �X�Ώ������g���Ă���Ƃ�
	 //�v���C�I���e�B�[�`�F�b�N
		CScene *pScene = CScene::GetTop(MESH_PRIOTITY);

		//NULL�`�F�b�N
		while (pScene != NULL)
		{
			//Update��Uninit����Ă��܂��ꍇ�@Next��������\�������邩��Next�Ƀf�[�^���c���Ă���
			CScene *pSceneNext = pScene->GetNext();

			if (pScene->GetObjType() == CScene::OBJTYPE_MESH)
			{//�^�C�v���n�ʂ�������
				CMeshField *pField = (CMeshField*)pScene;
				if (pField->OnField(m_WorldPos, m_fvtxMaxY))
				{// �X�΂̌v�Z
					m_pIncline->SetOnField(pField);
					if (!bJump)
					{// �W�����v���łȂ��Ƃ�
						m_pIncline->SetClimbSpeed(0.5f);				//���X�s�[�h
						m_pIncline->Update(m_WorldPos, m_Rot, pPlayer->GetPlayerInfoPoint()->fAccel);	//�X�΂̃X�s�[�h�v�Z

						//�X�s�[�h�̐ݒ�
						if (pPlayer->GetState() == CPlayer::STATE_DRIVE)
						{
							pPlayer->GetPlayerInfoPoint()->fAccel = m_pIncline->GetSpeed();
						}
						else
						{
							pPlayer->GetPlayerInfoPoint()->fAccel = m_pIncline->GetSpeed() * -1;
						}

						m_fHight = m_pIncline->GetJumpHeight();			//�W�����v�̍������擾
						bJump = m_pIncline->GetJumpFlag();				//�W�����v�t���O�̎擾
						pPlayer->SetJump(bJump);						//�v���C���[�Ƀt���O��ݒ�
						m_bJump = bJump;								//�W�����v��Ԃɂ���
					}
					break;
				}
			}
			//Next�Ɏ���Scene������
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// �p�x�C��
//=============================================================================
void CTire::RemakeAngle(float * pAngle)
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