//=============================================================================
//
// �^�C�g���̏��� [title.cpp]
// Author : �L�n ���u
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS	//Fire Wall�˔j
#include <string.h>				//�X�g�����O�g�p�̂���
#include "title.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "ui.h"
#include "titlecamera.h"
#include "object.h"

#include "titlemenu.h"
#include "player.h"
#include "model.h"
#include "feed.h"
#include "resultui.h"
#include "particle.h"
#include "ground.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define EFFECT_FRAME	(300)		// �G�߂̃G�t�F�N�g�t���[��
#define SEASON_MAX		(4)			// �G�߂̃J�E���g
#define EFFECT_CREATE	(10)		// �G�t�F�N�g��������
#define TITLE_POS		(D3DXVECTOR3(SCREEN_WIDTH /2, SCREEN_HEIGHT / 2 - 80.0f, 0.0f))	// �ʒu(�^�C�g�����S000)
#define TITLE_SIZE		(D3DXVECTOR2(480.0f, 100.0f))										// �T�C�Y(�^�C�g�����S000)
#define LOGO_POS		(D3DXVECTOR3(TITLE_POS.x + 130.0f, TITLE_POS.y + 100.0f, 0.0f))		// �ʒu(�^�C�g�����S001)
#define LOGO_SIZE		(D3DXVECTOR2(125.0f, 35.0f))										// �T�C�Y(�^�C�g�����S001)
#define PRESS_POS		(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT/ 2) + 200.0f, 0.0f))// �ʒu(Press Start Enter)
#define PRESS_SIZE		(D3DXVECTOR2(250.0f, 20.0f))										// �T�C�Y(Press Start Enter)

#define TEXT_OBJECTNAME1		"data\\TEXT\\�Q�[���}�b�v\\objecy.txt"			// �ǂݍ��ރe�L�X�g�t�@�C��

#define TEXT_FEEDNAME			"data\\TEXT\\�Q�[���}�b�v\\feed.txt"			// �ǂݍ��ރe�L�X�g�t�@�C��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
int				CTitle::m_nTitleCounter = 0;		// �^�C�g���̃J�E���^�[

CTitleCamera	*CTitle::m_pTitleCamera = NULL;
bool			CTitle::m_bMenu = false;			// ���j���[
bool			CTitle::m_bOnline = false;			// �I�����C��
bool			CTitle::m_bHost = false;			// �z�X�g

CPlayer			*CTitle::m_pPlayer[MAX_MEMBER] = { NULL };
int				CTitle::m_nRanking[MAX_MEMBER] = { 0 };			// �����L���O

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CTitle::CTitle()
{
	m_nSetObjectNum = 0;

	for (int nCount = 0; nCount < MAX_MAP_OBJECT; nCount++)
		m_Map[nCount] = {};

	m_nCntTex = 0;
	m_nFrame = 0;
	m_nLeafposX = 0;
	m_nLeafsize = 0;
	m_nCntCreate = 0;
	m_bOnOff = false;
	m_nTitleCounter = 0;

	m_bMenu = false;			// ���j���[
	m_bOnline = false;			// �I�����C��
	m_bHost = false;			// �z�X�g

	m_pTitleMenu = NULL;		// �^�C�g�����j���[

	m_pResultUI = NULL;			// UI���j���[

}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTitle::~CTitle()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CTitle::Init()
{
	//===================================
	//		 Load�̓ǂݍ��ݏꏊ
	//===================================
	CFade::Load();				//�t�F�[�h�̃e�N�X�`���̓ǂݍ���
	CUi::Load();				//UI�̃e�N�X�`���̓ǂݍ���
	CTitleMenu::Load();
	CObject::Load();			//�I�u�W�F�N�g�̃e�N�X�`���̓ǂݍ���
	CToonShader::Load();
	CResultUI::Load();
	CParticle::Load();			// �p�[�e�B�N���̃e�N�X�`���̃��[�h
	CGround::Load();

	CChick::Load();				//�Ђ悱�̃e�N�X�`���̓ǂݍ���
	CPlayer::Load();			//���f���̓ǂݍ���
	CModel::Load();				//�L�������f���̓ǂݍ���

								//�}�b�v��ǂݍ���
	TextLoad(6);

	//	�ϐ��̏�����
	m_nTitleCounter = 0;

	if (m_pTitleCamera == NULL)
	{
		m_pTitleCamera = new CTitleCamera;
		if (m_pTitleCamera != NULL)
		{
			m_pTitleCamera->Init();
		}
	}

	// ���U���g
	if (m_pResultUI == NULL)
	{
		m_pResultUI = CResultUI::Create();
	}

	// �^�C�g�����j���[
	if (m_pTitleMenu == NULL)
	{
		m_pTitleMenu = CTitleMenu::Create();
	}

	SetStage();

	CManager::OnlineSeting(false);	// �I�����C���ݒ�

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CTitle::Uninit(void)
{
	//===================================
	//	�@�@UnLoad�̔j������ꏊ
	//===================================
	CFade::UnLoad();	//�t�F�[�h�̃e�N�X�`���̔j��
	CUi::UnLoad();		//UI�̃e�N�X�`���̔j��
	CTitleMenu::Unload();
	CObject::UnLoad();				//�I�u�W�F�N�g�̃e�N�X�`���̔j��
	CResultUI::Unload();
	CParticle::UnLoad();			// �p�[�e�B�N���̃e�N�X�`���̃��[�h
	CGround::UnLoad();

	//���f���̔j��
	CChick::UnLoad();				//�Ђ悱�̃e�N�X�`���̔j��
	CPlayer::Unload();			//���f���̓ǂݍ���

	CModel3D::UnLoad();
	CModel3D::ModelShaderDeleter();

	// �^�C�g���J�����̔j��
	if (m_pTitleCamera != NULL)
	{
		m_pTitleCamera->Uninit();
		delete m_pTitleCamera;
		m_pTitleCamera = NULL;
	}

	// ���U���g
	if (m_pResultUI != NULL)
	{
		m_pResultUI->Uninit();
		m_pResultUI = NULL;
	}

	// �^�C�g�����j���[
	if (m_pTitleMenu != NULL)
	{
		m_pTitleMenu->Uninit();
		m_pTitleMenu = NULL;
	}

	for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
	{// �����o�[�J�E���g
		if (m_pPlayer[nCntMember] != NULL)
		{// NULL�ȊO
			m_pPlayer[nCntMember]->Uninit();
			m_pPlayer[nCntMember] = NULL;
		}
	}

	//�}�b�v�̔j��
	CCOL_MESH_MANAGER::EndMap();

	//�t�F�[�h�ȊO�폜
	CScene::NotFadeReleseAll();
}
//=============================================================================
// �X�V����
//=============================================================================
void CTitle::Update(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputJoyPad_0 * pXpad = CManager::GetInputJoyPad0(0);					//�W���C�p�b�g�̎擾
	CInputMouse *pCInputMouse = CManager::GetInputMouse();

	//�T�E���h�̏��
	CSound *pSound = CManager::GetSound();

	//�t�F�[�h�̃|�C���g
	CFade::FADE pFade = CFade::GetFade();

	Ranking();
	SetTitleMode();		// �^�C�g�����[�h

						//	�^�C�g���̃J�E���^�[���Z
	m_nTitleCounter++;

	if (m_pTitleCamera != NULL) { m_pTitleCamera->Updata(); }
}
//=============================================================================
// �`�揈��
//=============================================================================
void CTitle::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	pDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(157, 184, 224, 255),
		1.0f,
		0);

	//�^�C�g���J�����̐���
	if (m_pTitleCamera != NULL) { m_pTitleCamera->SetCamera(); }

	//�S�Ă̕`��
	CScene::DrawAll();
}

//=============================================================================
// �X�e�[�W�ݒ�
//=============================================================================
void CTitle::SetStage(void)
{
	//�}�b�v��ǂݍ���
	TextLoad(6);
	FeedTextLoad();

	//�R�̊O���̊C
	CGround::Create(D3DXVECTOR3(13500.0, -9000.0, 8000.0), D3DXVECTOR3(-0.6f, 0.0f, 0.0f), D3DXVECTOR2(5700.0f, 8300.0f), CGround::TYPE_WATER);
	CGround::Create(D3DXVECTOR3(13500.0, -8950.0, 8000.0), D3DXVECTOR3(0.3f, 0.0f, 0.0f), D3DXVECTOR2(5700.0f, 8300.0f), CGround::TYPE_WATER_HIGHLIGHT);

	//��̏���
	CGround::Create(D3DXVECTOR3(-12000.0f, -4000.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.4f), D3DXVECTOR2(5000.0f, 9000.0f), CGround::TYPE_WATER);
	CGround::Create(D3DXVECTOR3(-12000.0f, -3950.0f, 0.0f), D3DXVECTOR3(-0.2f, 0.0f, 0.0f), D3DXVECTOR2(5000.0f, 9000.0f), CGround::TYPE_WATER_HIGHLIGHT);

	//===================================
	//		Create
	//===================================

	CCOL_MESH_MANAGER::LoadMap();

	for (int nCount = 0; nCount < m_nSetObjectNum; nCount++)
	{
		//�I�u�W�F�N�g�̐���
		CObject::Create(m_Map[nCount].m_pos, m_Map[nCount].m_rot, m_Map[nCount].m_scale, 0.0f, m_Map[nCount].m_nTexType, m_Map[nCount].m_nType, m_Map[nCount].m_nCollision);
	}
	for (int nCount = 0; nCount < m_nSetFeedNum; nCount++)
	{
		//�H�ו��̐���		
		CFeed::Create(m_aFeed[nCount].m_pos, m_aFeed[nCount].m_nZone, m_aFeed[nCount].m_nType);
	}

	for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
	{// �����o�[�J�E���g
		float frot = (-D3DX_PI * 0.5f);
		//�v���C���[�̐���
		if (m_pPlayer[nCntMember] == NULL)
			m_pPlayer[nCntMember] = CPlayer::Create(
				D3DXVECTOR3(-250.0f + (100.0f * (nCntMember / 4)), -90.0f, -100.0f + (((70.0f * 2.0f) / 3.0f) * (nCntMember % 4)) + (35.0f * (nCntMember / 4))),
				D3DXVECTOR3(0.0f, frot, 0.0f),
				nCntMember, nCntMember, nCntMember, CPlayer::PLAYERTYPE_ENEMY);
		if (m_pPlayer[nCntMember] != NULL)
			m_pPlayer[nCntMember]->SetControl(false);
	}
}

//=============================================================================
// �^�C�g�����[�h�ݒ�
//=============================================================================
void CTitle::SetTitleMode(void)
{
	int nCounter = m_nTitleCounter % RESET_TITLEMODE;

	if (nCounter == RACEINIT_TITLEMODE)
	{// �R�[�X
		for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
		{// �����o�[�J�E���g
			float frot = (-D3DX_PI * 0.5f);
			if (m_pPlayer[nCntMember] != NULL)
			{// NULL�ȊO
				m_pPlayer[nCntMember]->SetControl(false);

				m_pPlayer[nCntMember]->SetPos(
					D3DXVECTOR3(-250.0f + (100.0f * (nCntMember / 4)), -90.0f, -100.0f + (((70.0f * 2.0f) / 3.0f) * (nCntMember % 4)) + (35.0f * (nCntMember / 4))));
				m_pPlayer[nCntMember]->SetRot(
					D3DXVECTOR3(0.0f, frot, 0.0f));
				m_pPlayer[nCntMember]->SetPlayerType(CPlayer::PLAYERTYPE_ENEMY);
			}
		}

		if (m_pTitleCamera != NULL)
			m_pTitleCamera->SetType(CTitleCamera::CAMERA_COURSE);
	}
	else if (nCounter == RACEMOVE_TITLEMODE)
	{// ����
		for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
		{// �����o�[�J�E���g

			if (m_pPlayer[nCntMember] != NULL)
			{// NULL�ȊO
				m_pPlayer[nCntMember]->SetControl(true);
			}
		}
	}
	else if (nCounter == CAMERA_P_TITLEMODE_1)
	{// �J�������v���C���[��
		if (m_pTitleCamera != NULL)
		{
			m_pTitleCamera->SetType(CTitleCamera::CAMERA_PLAYER);
			m_pTitleCamera->SetPlayer(m_pPlayer[(CServer::Rand() % MAX_MEMBER)]);
			m_pTitleCamera->SetTypeReset();
		}
	}
	else if (nCounter == CAMERA_UP_TITLEMODE)
	{// �ォ��
		if (m_pTitleCamera != NULL)
		{
			m_pTitleCamera->SetType(CTitleCamera::CAMERA_GOUL);
			m_pTitleCamera->SetTypeReset();
		}
	}
	else if (nCounter == CAMERA_P_TITLEMODE_2)
	{// �J�������v���C���[��
		if (m_pTitleCamera != NULL)
		{
			m_pTitleCamera->SetType(CTitleCamera::CAMERA_PLAYER);
			m_pTitleCamera->SetTypeReset();
		}
	}
	else if (nCounter == (HOME_TITLEMODE - RANKING_TITLEMODE))
	{// 

	}
	else if (nCounter == HOME_TITLEMODE)
	{// ������
		for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
		{// �����o�[�J�E���g
			if (m_pPlayer[nCntMember] != NULL)
			{// NULL�ȊO
				m_pPlayer[nCntMember]->Uninit();
				m_pPlayer[nCntMember] = NULL;
			}
		}

		CScene *pScene = NULL;
		CScene *pSceneNext = NULL;

		pScene = CScene::GetTop(0);

		//NULL�`�F�b�N
		while (pScene != NULL)
		{
			//Update��Uninit����Ă��܂��ꍇ�@Next��������\�������邩��Next�Ƀf�[�^���c���Ă���
			CScene *pSceneNext = pScene->GetNext();

			if (pScene->GetDeath() == false)
			{//�^�C�v���ǂ�������
				if (pScene->GetObjType() == CScene::OBJTYPE_CHICK || pScene->GetObjType() == CScene::OBJTYPE_EGG)
				{
					//�������Ă��Ȃ����
					pScene->Uninit();
				}
			}
			//Next�Ɏ���Scene������
			pScene = pSceneNext;
		}

		for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
		{// �����o�[�J�E���g
			float frot = (-D3DX_PI * 0.5f);
			if (m_pPlayer[nCntMember] == NULL)
				m_pPlayer[nCntMember] = CPlayer::Create(
					D3DXVECTOR3(((-250.0f + (50.0f * (nCntMember / 4))) + 2950.0f), (-90.0f), (210.0f - (((70.0f * 2.0f) / 3.0f) * (nCntMember % 4)))),
					D3DXVECTOR3(0.0f, frot, 0.0f),
					nCntMember, nCntMember, nCntMember, CPlayer::PLAYERTYPE_SELECT);
			if (m_pPlayer[nCntMember] != NULL)
			{
				m_pPlayer[nCntMember]->SetControl(false);
				m_pTitleCamera->SetTypeReset();
			}
		}

		if (m_pTitleCamera != NULL)
			m_pTitleCamera->SetType(CTitleCamera::CAMERA_CHARSELECT);
	}
	else if (nCounter == CHARUP_TITLEMODE)												
	{// �L�����A�b�v
		if (m_pTitleCamera != NULL)
		{
			m_pTitleCamera->SetType(CTitleCamera::CAMERA_CHARUP);
			m_pTitleCamera->SetPlayer(m_pPlayer[(CServer::Rand() % MAX_MEMBER)]);
		}
	}
}

//===============================================================================
// �����L���O
//===============================================================================
void CTitle::Ranking(void)
{
	CSound *pSound = CManager::GetSound();						//�T�E���h�̏��

	int	nGoulNum = 0;						// �S�[����

	for (int nCount = 0; nCount < MAX_MEMBER; nCount++)
	{// �v���C���[�J�E���g
	 // �����L���O
		m_nRanking[nCount] = CRoad_Manager::GetManager()->GetRank(nCount) - 1;
	}
}

//===============================================================================
// �t�@�C�����烍�[�h
//===============================================================================
void CTitle::TextLoad(int nLoadNumber)
{
	//�t�@�C���p�ϐ�
	FILE *pFile;		//�t�@�C���|�C���^
	char *pStrcur;		//���݂̐擪�̕�����
	char aLine[256];	//������
	char aStr[256];		//�ꎞ�ۑ�������
	int nIndex = 0;		//���݂̃C���f�b�N�X
	int nWord = 0;		//�|�b�v�ŕԂ��ꂽ�l��ێ�

						//�t�@�C���|�C���^�̏���������
	pFile = NULL;

	//�t�@�C�����J�� �ǂݍ���
	pFile = fopen(TEXT_OBJECTNAME1, "r");

	//NULL�`�F�b�N
	if (pFile != NULL)
	{
		//������̐擪��ݒ�
		pStrcur = ReadLine(pFile, &aLine[0]);
		//����������o��
		strcpy(aStr, pStrcur);

		//������̃f�[�^ ��r���镶���� ��r���镶����
		if (memcmp(pStrcur, "OBJECT_SETNUM = ", strlen("OBJECT_SETNUM = ")) == 0)
		{
			//���o��
			pStrcur += strlen("OBJECT_SETNUM = ");
			//������̐擪��ݒ�
			strcpy(aStr, pStrcur);
			//�����񔲂��o��
			m_nSetObjectNum = atoi(pStrcur);
		}

		//�I�u�W�F�N�g�̐�����
		for (int nCntObject = 0; nCntObject < m_nSetObjectNum; nCntObject++)
		{
			//������̐擪��ݒ�
			pStrcur = ReadLine(pFile, &aLine[0]);
			//����������o��
			strcpy(aStr, pStrcur);

			if (memcmp(pStrcur, "OBJECT_START", strlen("OBJECT_START")) == 0)
			{
				while (1)
				{
					//������̐擪��ݒ�
					pStrcur = ReadLine(pFile, &aLine[0]);

					//��ނ̂�ǂݍ���
					if (memcmp(pStrcur, "TYPE = ", strlen("TYPE = ")) == 0)
					{
						//���o��
						pStrcur += strlen("TYPE = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_Map[nCntObject].m_nType = atoi(pStrcur);
					}
					//��ނ̂�ǂݍ���
					if (memcmp(pStrcur, "TEXTURETYPE = ", strlen("TEXTURETYPE = ")) == 0)
					{
						//���o��
						pStrcur += strlen("TEXTURETYPE = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_Map[nCntObject].m_nTexType = atoi(pStrcur);
					}
					//�����蔻���ǂݍ���
					if (memcmp(pStrcur, "COLLISION = ", strlen("COLLISION = ")) == 0)
					{
						//���o��
						pStrcur += strlen("COLLISION = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_Map[nCntObject].m_nCollision = atoi(pStrcur);
					}
					//POS��ǂݍ���
					if (memcmp(pStrcur, "POS = ", strlen("POS = ")) == 0)
					{
						//���o��
						pStrcur += strlen("POS = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_Map[nCntObject].m_pos.x = (float)atof(pStrcur);
						//���������i�߂�
						pStrcur += nWord;

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_Map[nCntObject].m_pos.y = (float)atof(pStrcur);
						//���������i�߂�
						pStrcur += nWord;

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_Map[nCntObject].m_pos.z = (float)atof(pStrcur);

					}
					//ROT��ǂݍ���
					if (memcmp(pStrcur, "ROT = ", strlen("ROT = ")) == 0)
					{
						//���o��
						pStrcur += strlen("ROT = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_Map[nCntObject].m_rot.x = (float)atof(pStrcur);

						//���������i�߂�
						pStrcur += nWord;
						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_Map[nCntObject].m_rot.y = (float)atof(pStrcur);

						//���������i�߂�
						pStrcur += nWord;
						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_Map[nCntObject].m_rot.z = (float)atof(pStrcur);
					}
					//SIZ��ǂݍ���
					if (memcmp(pStrcur, "SIZ = ", strlen("SIZ = ")) == 0)
					{
						//���o��
						pStrcur += strlen("SIZ = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_Map[nCntObject].m_scale.x = (float)atof(pStrcur);

						//���������i�߂�
						pStrcur += nWord;
						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_Map[nCntObject].m_scale.y = (float)atof(pStrcur);

						//���������i�߂�
						pStrcur += nWord;
						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_Map[nCntObject].m_scale.z = (float)atof(pStrcur);
					}
					else if (memcmp(pStrcur, "OBJECT_END", strlen("OBJECT_END")) == 0)
					{
						break;
					}
				}
			}
		}
	}
}

//===============================================================================
// �H�ו����t�@�C�����烍�[�h
//===============================================================================
void CTitle::FeedTextLoad(void)
{
	//�t�@�C���p�ϐ�
	FILE *pFile;		//�t�@�C���|�C���^
	char *pStrcur;		//���݂̐擪�̕�����
	char aLine[256];	//������
	char aStr[256];		//�ꎞ�ۑ�������
	int nIndex = 0;		//���݂̃C���f�b�N�X
	int nWord = 0;		//�|�b�v�ŕԂ��ꂽ�l��ێ�

						//�t�@�C�����J�� �ǂݍ���
	pFile = fopen(TEXT_FEEDNAME, "r");

	//NULL�`�F�b�N
	if (pFile != NULL)
	{
		//������̐擪��ݒ�
		pStrcur = ReadLine(pFile, &aLine[0]);
		//����������o��
		strcpy(aStr, pStrcur);

		//������̃f�[�^ ��r���镶���� ��r���镶����
		if (memcmp(pStrcur, "FEED_SETNUM = ", strlen("FEED_SETNUM = ")) == 0)
		{
			//���o��
			pStrcur += strlen("FEED_SETNUM = ");
			//������̐擪��ݒ�
			strcpy(aStr, pStrcur);
			//�����񔲂��o��
			m_nSetFeedNum = atoi(pStrcur);
		}

		//�I�u�W�F�N�g�̐�����
		for (int nCntObject = 0; nCntObject < m_nSetFeedNum; nCntObject++)
		{
			//������̐擪��ݒ�
			pStrcur = ReadLine(pFile, &aLine[0]);
			//����������o��
			strcpy(aStr, pStrcur);

			if (memcmp(pStrcur, "FEED_START", strlen("FEED_START")) == 0)
			{
				while (1)
				{
					//������̐擪��ݒ�
					pStrcur = ReadLine(pFile, &aLine[0]);

					//POS��ǂݍ���
					if (memcmp(pStrcur, "POS = ", strlen("POS = ")) == 0)
					{
						//���o��
						pStrcur += strlen("POS = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_aFeed[nCntObject].m_pos.x = (float)atof(pStrcur);
						//���������i�߂�
						pStrcur += nWord;

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_aFeed[nCntObject].m_pos.y = (float)atof(pStrcur);
						//���������i�߂�
						pStrcur += nWord;

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_aFeed[nCntObject].m_pos.z = (float)atof(pStrcur);

					}
					//ZONE��ǂݍ���
					if (memcmp(pStrcur, "ZONE = ", strlen("ZONE = ")) == 0)
					{
						//���o��
						pStrcur += strlen("ZONE = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_aFeed[nCntObject].m_nZone = atoi(pStrcur);
					}
					//TYPE��ǂݍ���
					if (memcmp(pStrcur, "TYPE = ", strlen("TYPE = ")) == 0)
					{
						//���o��
						pStrcur += strlen("TYPE = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_aFeed[nCntObject].m_nType = atoi(pStrcur);
					}
					else if (memcmp(pStrcur, "FEED_END", strlen("FEED_END")) == 0)
					{
						break;
					}
				}
			}
		}
	}

	//m_pMarkFeed->SetFeedNum(CMarkFeed::GetFeedNum() + m_nSetFeedNum);
}

//=============================================================================
//�@�t�@�C���ǂݍ��ݖ�������r��
//=============================================================================
char *CTitle::ReadLine(FILE *pFile, char *pDst)
{
	while (1)
	{
		//�P�s���ǂݍ���
		fgets(&pDst[0], 256, pFile);

		//������̃f�[�^ ��r���镶���� ��r���镶����
		if (memcmp(pDst, "#", strlen("#")) == 0)
		{
			pDst += strlen("\n");
		}
		//������̃f�[�^ ��r���镶���� ��r���镶����
		else if (memcmp(pDst, "\t", strlen("\t")) == 0)
		{
			pDst += strlen("\t");
			while (1)
			{
				if (memcmp(pDst, "\t", strlen("\t")) == 0)
				{
					pDst += strlen("\t");
				}
				else
				{
					break;
				}
			}
			break;
		}
		//������̃f�[�^ ��r���镶���� ��r���镶����
		else if (memcmp(pDst, " ", strlen(" ")) == 0)
		{
			pDst += strlen(" ");
			while (1)
			{
				if (memcmp(pDst, " ", strlen(" ")) == 0)
				{
					pDst += strlen(" ");
				}
				else
				{
					break;
				}
				break;
			}
		}
		//������̃f�[�^ ��r���镶���� ��r���镶����
		else if (memcmp(pDst, "\n", strlen("\n")) == 0)
		{
			pDst += strlen("\n");
		}
		else
		{
			break;
		}
	}
	return pDst;
}

//=============================================================================
//�@�t�@�C���ǂݍ��ݐ擪��r��
//=============================================================================
char *CTitle::GetLineTop(char * pStr)
{
	while (1)
	{
		//������̃f�[�^ ��r���镶���� ��r���镶����
		if (memcmp(pStr, " ", strlen(" ")) == 0)
		{
			pStr += strlen(" ");
			while (1)
			{
				if (memcmp(pStr, " ", strlen(" ")) == 0)
				{
					pStr += strlen(" ");
				}
				else
				{
					break;
				}
				break;
			}
		}
		//������̃f�[�^ ��r���镶���� ��r���镶����
		else if (memcmp(pStr, "\t", strlen("\t")) == 0)
		{
			pStr += strlen("\t");
			while (1)
			{
				if (memcmp(pStr, "\t", strlen("\t")) == 0)
				{
					pStr += strlen("\t");
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			break;
		}
	}
	return pStr;
}

//=============================================================================
//�@��������Ԃ�
//=============================================================================
int CTitle::PopString(char * pStr, char * pDest)
{
	int nWord = 0;

	while (1)
	{	//���o��
		pStr += 1;
		nWord += 1;
		if (memcmp(pStr, " ", strlen(" ")) == 0)
		{	//���o��
			pStr = "\0";
			nWord += 1;
			break;
		}
		if (memcmp(pStr, "\t", strlen("\t")) == 0)
		{	//���o��
			pStr = "\0";
			nWord += (int)strlen("\t");
			break;
		}
		//������̃f�[�^ ��r���镶���� ��r���镶����
		else if (memcmp(pStr, "\n", strlen("\n")) == 0)
		{
			//���o��
			nWord += (int)strlen("\n");
			break;
		}
	}
	strcpy(pDest, pStr);
	//������̐���Ԃ�
	return nWord;
}