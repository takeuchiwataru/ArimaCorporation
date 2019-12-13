//=============================================================================
//
// �Q�[���̏��� [game.cpp]
// Author : �L�n�@���u
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS	//Fire Wall�˔j
#include <string.h>				//�X�g�����O�g�p�̂���
#include "game.h"
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "player.h"
#include "input.h"
#include "fade.h"
#include "pause.h"
#include "sound.h"
#include "billboord.h"
#include "meshfield.h"
#include "object.h"
#include "fade.h"
#include "wall.h"
#include "shadow.h"
#include "loadText.h"
#include "gamecamera.h"
#include "select.h"
#include "feed.h"
#include "egg.h"
#include "gameCharSelect.h"
#include "gamePlay.h"
#include "chick.h"
#include "ColMesh.h"
#include "particle.h"
#include "time.h"
#include "model.h"
#include "number.h"
#include "Character.h"
#include "toonshader.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXT_OBJECTNAME1		"data\\TEXT\\�Q�[���}�b�v\\objecy.txt"			// �ǂݍ��ރe�L�X�g�t�@�C��
#define TEXT_MESHFIELDNAME1		"data\\TEXT\\�Q�[���}�b�v\\meshfield.txt"		// �ǂݍ��ރe�L�X�g�t�@�C��
#define TEXT_WALLNAME1			"data\\TEXT\\�Q�[���}�b�v\\wall.txt"			// �ǂݍ��ރe�L�X�g�t�@�C��
#define TEXT_FEEDNAME			"data\\TEXT\\�Q�[���}�b�v\\feed.txt"			// �ǂݍ��ރe�L�X�g�t�@�C��
#define TEXT_PLAYER_MOTION		"data\\TEXT\\Player\\Player.txt"				// �v���C���[�̃��[�V�����t�@�C��
#define VECTOR_ZERO				(D3DXVECTOR3(0.0f, 0.0f, 0.0f))

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CGameCharSelect *CGame::m_pGameCharSelect = NULL;	// �Q�[���i�L�����I���j
CGamePlay *CGame::m_pGamePlay = NULL;				// �Q�[���i�v���C�j

CPlayer *CGame::m_pPlayer[MAX_MEMBER] = { NULL };
CPause *CGame::m_pPause = NULL;
//CLoadTextMotion * CGame::m_pPlayerMotion = NULL;
CGameCamera * CGame::m_pCuorseCamera = NULL;
CGameCamera * CGame::m_pGameCamera[MAX_PLAYER] = { NULL };
bool CGame::m_bHelp = false;
bool CGame::m_bPause = false;
CGame::GAMEMODE CGame::m_gameMode = CGame::GAMEMODE_NONE;
CGame::GAMEMODE CGame::m_gameModeNext = CGame::GAMEMODE_NONE;
CGame::GAMESTATE CGame::m_gameState = CGame::GAMESTATE_NONE;
int	CGame::m_nCntSetStage = 0;
int CGame::m_nGameCounter = 0;

//�E�H�[�N�X���[�p
bool CGame::m_bDrawUI = false;

int CGame::m_nMaxPlayer = 0;						// �v���C���[��
int CGame::m_nCharSelectNum[MAX_MEMBER] = { 0 };	// �L�����I��ԍ�
int CGame::m_nControllerNum[MAX_PLAYER] = { 0 };	// �R���g���[���[�ԍ�
int CGame::m_nRanking[MAX_MEMBER] = { 0 };			// �����L���O
int CGame::m_nRankingSort[MAX_MEMBER] = { 0 };		// �����L���O
bool CGame::m_bGoul[MAX_PLAYER] = { false };		// �S�[��

int CGame::m_nCameraNumber = 0;						// ���ݎg�p���Ă���J�����ԍ�

int CGame::m_nTime[MAX_MEMBER] = { 0 };				// �^�C��

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CGame::CGame()
{
	m_gameMode = GAMEMODE_NONE;
	m_gameModeNext = GAMEMODE_NONE;
	m_gameState = GAMESTATE_NONE;
	m_nCounterGameState = 0;
	m_NowGameState = GAMESTATE_NONE;
	m_nCntSetStage = 0;
	m_bPause = false;
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGame::~CGame() {}

//=============================================================================
// ����������
//=============================================================================
HRESULT CGame::Init()
{
	//====================================================================
	//					�e�N�X�`�����f���̓ǂݍ��ݏꏊ
	//====================================================================

	CFade::Load();				//�t�F�[�h�̃e�N�X�`���̓ǂݍ���
	CMeshField::Load();			//���b�V���t�B�[���h�̃e�N�X�`���̓ǂݍ���
	CWall::Load();				//�ǂ̃e�N�X�`���̓ǂݍ���
	CShadow::Load();			//�e�̃e�N�X�`���ǂݍ���
	CObject::Load();			//�I�u�W�F�N�g�̃e�N�X�`���̓ǂݍ���
	CFeed::Load();				//�H�ו��̃e�N�X�`���̓ǂݍ���
	CEgg::Load();				//���̃e�N�X�`���̓ǂݍ���
	CChick::Load();				//�Ђ悱�̃e�N�X�`���̓ǂݍ���
	CParticle::Load();			// �p�[�e�B�N���̃e�N�X�`���̃��[�h
	m_pPause->Load();			//�|�[�Y�̃e�N�X�`���̓ǂݍ���
	CToonShader::Load();

	CGameCharSelect::Load();	// �Q�[���i�L�����I���j
	CGamePlay::Load();			// �Q�[���i�v���C�j

	CPlayer::Load();			//���f���̓ǂݍ���
	CModel::Load();				//�L�������f���̓ǂݍ���

	CTime::Load();
	CNumber::Load();

	//====================================================================
	//						 �K�v�ȕϐ��̏�����
	//====================================================================

	m_gameMode = GAMEMODE_CHARSELECT;			// �Q�[�����[�h
	m_gameModeNext = m_gameMode;		// ���̃Q�[�����[�h
	m_gameState = GAMESTATE_NORMAL;		//�ʏ��Ԃ�
	m_nCntSetStage = 0;					//�ǂ��̃X�e�[�W����J�n���邩
	m_bPause = false;					//�|�[�Y��������
	m_nGameCounter = 0;					//�J�E���^�[�̏�����

	m_nCameraNumber = 0;				// ���ݎg�p���Ă���J�����ԍ�

	m_nMaxPlayer = 0;					// �v���C���[��
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		m_nControllerNum[nCntPlayer] = nCntPlayer;		// �R���g���[���[�ԍ�
		m_bGoul[nCntPlayer] = false;					// �S�[��
	}

	for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
	{
		m_nCharSelectNum[nCntMember] = nCntMember;		// �L�����I��ԍ�
		m_nRanking[nCntMember] = nCntMember;			// �����L���O
		m_nRankingSort[nCntMember] = nCntMember;
		m_nTime[nCntMember] = 0;
	}
														// �f�o�b�O�p
	if (m_gameMode == GAMEMODE_COURSE_VIEW)
		m_nMaxPlayer = 1;
	else if (m_gameMode == GAMEMODE_PLAY)
		m_nMaxPlayer = 1;

	SetGameMode(m_gameMode);			// �Q�[�����[�h�ݒ�

	// �J�E���g���Ȃ���
	//if (m_gameMode == GAMEMODE_PLAY) 
	//	m_nGameCounter = START_SET_TIME;

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CGame::Uninit(void)
{
	//===================================
	//	�@�@UnLoad�̔j������ꏊ
	//===================================
	CModel3D::ModelShaderDeleter();
	CModel3D::UnLoad();
	CMeshField::UnLoad();			//���b�V���t�B�[���h�e�N�X�`���̔j��
	CFade::UnLoad();				//�t�F�[�h�̃e�N�X�`���̔j��
	CParticle::UnLoad();			// �p�[�e�B�N���̃e�N�X�`���̃��[�h
	CObject::UnLoad();				//�I�u�W�F�N�g�̃e�N�X�`���̔j��
	CWall::UnLoad();				//�ǂ̃e�N�X�`���̔j
	CShadow::UnLoad();				//�e�̃e�N�X�`���̔j��
	CFeed::UnLoad();				//�a�̃e�N�X�`���̔j��
	CEgg::UnLoad();					//���̃e�N�X�`���̔j��
	CChick::UnLoad();				//�Ђ悱�̃e�N�X�`���̔j��

	CGameCharSelect::Unload();		// �Q�[���i�L�����I���j
	CGamePlay::Unload();			// �Q�[���i�v���C�j

	//���f���̔j��
	CPlayer::Unload();

	CTime::Unload();
	CNumber::Unload();

	//�|�[�Y�폜
	if (m_pPause != NULL)
	{// NULL�ȊO
		m_pPause->Uninit();
		m_pPause = NULL;
	}

	// �Q�[���i�L�����I���j
	if (m_pGameCharSelect != NULL)
	{// NULL�ȊO
		m_pGameCharSelect->Uninit();
		m_pGameCharSelect = NULL;
	}
	// �Q�[���i�v���C�j
	if (m_pGamePlay != NULL)
	{// NULL�ȊO
		m_pGamePlay->Uninit();
		m_pGamePlay = NULL;
	}

	////�v���C���[�̃��[�V�����̔j��
	//if (m_pPlayerMotion != NULL)
	//{
	//	m_pPlayerMotion->Uninit();
	//	delete m_pPlayerMotion;
	//	m_pPlayerMotion = NULL;
	//}

	//�J�����̔j��
	if (m_pCuorseCamera != NULL)
	{
		m_pCuorseCamera->Uninit();
		delete m_pCuorseCamera;
		m_pCuorseCamera = NULL;
	}

	for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
	{// �����o�[�J�E���g
		if (m_pPlayer[nCntMember] != NULL)
		{// NULL�ȊO
			m_pPlayer[nCntMember]->Uninit();
			m_pPlayer[nCntMember] = NULL;
		}
	}

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// �v���C���[�J�E���g
		if (m_pGameCamera[nCntPlayer] != NULL)
		{// NULL�ȊO
			m_pGameCamera[nCntPlayer]->Uninit();
			delete m_pGameCamera[nCntPlayer];
			m_pGameCamera[nCntPlayer] = NULL;
		}
	}

	//�}�b�v�̔j��
	CCOL_MESH_MANAGER::EndMap();

	//�t�F�[�h�ȊO�̔j��
	CScene::NotFadeReleseAll();
}
//=============================================================================
// �X�V����
//=============================================================================
void CGame::Update(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputJoyPad_0 * pXpad = CManager::GetInputJoyPad0(m_nControllerNum[0]);		//�W���C�p�b�g�̎擾
	CSound *pSound = CManager::GetSound();						//�T�E���h�̏��
	CFade::FADE fade = CFade::GetFade();

	// �擾
	bool bOnine = CTitle::GetOnline();

	if (m_gameMode != m_gameModeNext)
	{// �Q�[�����[�h���ς����
		SetGameMode(m_gameModeNext);
		return;
	}

	switch (m_gameMode)
	{// �Q�[�����[�h
	case GAMEMODE_CHARSELECT:
		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
			if (m_pGameCamera[nCntPlayer] != NULL) { m_pGameCamera[nCntPlayer]->SetPlayer((m_pPlayer[m_nCharSelectNum[nCntPlayer]] != NULL ? m_pPlayer[m_nCharSelectNum[nCntPlayer]] : NULL)); }

		break;
	case GAMEMODE_COURSESELECT:
		//if (pCInputKeyBoard->GetKeyboardTrigger(DIK_RETURN) == true)
		//	CFade::Create(GAMEMODE_PLAY);

		break;
	case GAMEMODE_COURSE_VIEW:
		for (int nCntPlayer = 0; nCntPlayer < MAX_MEMBER; nCntPlayer++)
			if (m_pPlayer[nCntPlayer] != NULL)
				m_pPlayer[nCntPlayer]->SetControl(false);

		fade = CFade::GetFade(); 
		if (fade == CFade::FADE_NONE)
		{// �t�F�[�h���Ă��Ȃ�
			if (m_bPause == false)
			{//�|�[�Y���Ă��Ȃ�
				if (pCInputKeyBoard->GetKeyboardTrigger(DIK_RETURN) == true ||
					pCInputKeyBoard->GetKeyboardTrigger(DIK_Z) == true ||
					pXpad->GetTrigger(INPUT_START) == true)
					CFade::Create(CGame::GAMEMODE_PLAY);
			}
		}
		break;
	case GAMEMODE_PLAY:
		//���݂̏�Ԃ�ۑ�
		m_NowGameState = GetGameState();

		switch (m_NowGameState)
		{// �Q�[�����
		case GAMESTATE_NORMAL:	//�ʏ�̏��
			if (m_nGameCounter < START_SET_TIME)
			{
				for (int nCntPlayer = 0; nCntPlayer < MAX_MEMBER; nCntPlayer++)
					if (m_pPlayer[nCntPlayer] != NULL)
						m_pPlayer[nCntPlayer]->SetControl(false);
			}
			else if (m_nGameCounter == START_SET_TIME)
			{
				for (int nCntPlayer = 0; nCntPlayer < MAX_MEMBER; nCntPlayer++)
					if (m_pPlayer[nCntPlayer] != NULL)
						m_pPlayer[nCntPlayer]->SetControl(true);
			}

			if (m_bPause == false)
				Ranking();		// �����L���O

			break;
		case GAMESTATE_END:			//�Q�[���I�����
			m_nCounterGameState++;

			if (180 < m_nCounterGameState)
			{//��ʁi���[�h�j�̐ݒ�
				CFade::Create(CManager::MODE_RESULT);
			}
			break;
		}

		break;
	}

	fade = CFade::GetFade();
	if (fade == CFade::FADE_NONE)
	{// �t�F�[�h���Ă��Ȃ�
		if (m_gameMode == GAMEMODE_COURSE_VIEW || (m_gameMode == GAMEMODE_PLAY && (START_SET_TIME) < m_nGameCounter))
		{// ���[�X��
			if (bOnine == false)
			{// �I�����C������Ȃ�
			 //�|�[�Y�̏���
				if (pCInputKeyBoard->GetKeyboardTrigger(DIK_P) == true ||
					pXpad->GetTrigger(INPUT_START) == true)
				{//P�L�[�������ꂽ��
					m_bPause = m_bPause ? false : true;

					switch (m_bPause)
					{// �|�[�Y
					case true:
						if (m_pPause == NULL)
						{
							//�|�[�Y���J����
							//pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_OPEN);

							//�|�[�Y�̐���
							m_pPause = CPause::Create();

							//�|�[�Y�ƃt�F�[�h������
							CScene::SetUpdatePri(7);
						}
						break;
					case false:
						if (m_pPause != NULL)
						{
							//�|�[�Y����鉹
							//pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_CLOSE);

							//�|�[�Y���폜
							m_pPause->Uninit();
							m_pPause = NULL;

							//�A�b�v�f�[�g���Ԃ����ׂĉ�
							CScene::SetUpdatePri(0);
						}
					}
				}
			}
		}
	}

	if (m_bPause == false)
	{//�J����
		//�J�����̍X�V����
		if (m_pCuorseCamera != NULL) { m_pCuorseCamera->Update(); }

		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
			if (m_pGameCamera[nCntPlayer] != NULL) { m_pGameCamera[nCntPlayer]->Update(); }

		//	�J�E���^�[�i�߂�
		m_nGameCounter++;
	}

	//CDebugProc::Print("MaxPlayer:%d\n", m_nMaxPlayer);	// �v���C���[��
	//CDebugProc::Print("m_bPause:%d\n", m_bPause);		// �|�[�Y
}

//=============================================================================
// �`�揈��
//=============================================================================
void CGame::Draw(void)
{
	// �擾
	bool bOnine = CTitle::GetOnline();

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_nCameraNumber = 0;

	if (m_gameMode == GAMEMODE_CHARSELECT)
	{// ���̑�
		D3DVIEWPORT9 viewport;
		pDevice->GetViewport(&viewport);	// �r���[�|�[�g�擾

											// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
		pDevice->Clear(0,
			NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(157, 184, 224, 255),
			1.0f,
			0);

		m_nCameraNumber = -1;

		if (m_pCuorseCamera != NULL) { m_pCuorseCamera->SetCamera(); }

		//�S�Ă̕`��
		CScene::DrawAll();

		pDevice->SetViewport(&viewport);	// �r���[�|�[�g�ݒ�

		//�QD�̕`��
		CScene::DrawSeting(true);

		for (int nCntPlayer = 0; nCntPlayer < m_nMaxPlayer; nCntPlayer++)
		{// �v���C���[�J�E���g
		 // �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
			pDevice->Clear(0,
				NULL,
				(D3DCLEAR_ZBUFFER),
				D3DCOLOR_RGBA(157, 184, 224, 255),
				1.0f,
				0);

			m_nCameraNumber = nCntPlayer;

			//�J�����̐ݒ�
			if (m_pGameCamera[nCntPlayer] != NULL) { m_pGameCamera[nCntPlayer]->SetCamera(); }

			//�S�Ă̕`��
			CScene::DrawSeting(true, 0, true, 5);
		}

		pDevice->SetViewport(&viewport);	// �r���[�|�[�g�ݒ�

											//�QD�̕`��
		CScene::DrawSeting(true, 6, true);
	}
	else if (m_gameMode == GAMEMODE_COURSE_VIEW)
	{// ���̑�
	 // �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
		pDevice->Clear(0,
			NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(157, 184, 224, 255),
			1.0f,
			0);

		m_nCameraNumber = -1;

		if (m_pCuorseCamera != NULL) { m_pCuorseCamera->SetCamera(); }

		//�S�Ă̕`��
		CScene::DrawAll();
	}
	else if (m_gameMode == GAMEMODE_PLAY)
	{// �v���C
		D3DVIEWPORT9 viewport;
		pDevice->GetViewport(&viewport);	// �r���[�|�[�g�擾

											// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
		pDevice->Clear(0,
			NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(157, 184, 224, 255),
			1.0f,
			0);

		if (bOnine == true)
		{// �I�����C��
			int nClient = CClient::GetnID();
			m_nCameraNumber = nClient;
			if (m_pGameCamera[nClient] != NULL) { m_pGameCamera[nClient]->SetCamera(); }

			//�S�Ă̕`��
			CScene::DrawAll();
		}
		else
		{// ���[�J��
			for (int nCntPlayer = 0; nCntPlayer < m_nMaxPlayer; nCntPlayer++)
			{// �v���C���[�J�E���g
			 // �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
				pDevice->Clear(0,
					NULL,
					(D3DCLEAR_ZBUFFER),
					D3DCOLOR_RGBA(157, 184, 224, 255),
					1.0f,
					0);

				m_nCameraNumber = nCntPlayer;

				//�J�����̐ݒ�
				if (m_pGameCamera[nCntPlayer] != NULL) { m_pGameCamera[nCntPlayer]->SetCamera(); }

				//�S�Ă̕`��
				CScene::DrawAll();
			}
		}

		pDevice->SetViewport(&viewport);	// �r���[�|�[�g�ݒ�

		//2D�̕`��
		CScene::DrawSeting(true, 5, true, 6);

		// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
		pDevice->Clear(0,
			NULL,
			(D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(157, 184, 224, 255),
			1.0f,
			0);

		m_nCameraNumber = -1;

		if (m_pCuorseCamera != NULL) { m_pCuorseCamera->SetCamera(); }

		CScene::DrawMap();

		pDevice->SetViewport(&viewport);	// �r���[�|�[�g�ݒ�

		//2D�̕`��
		CScene::DrawSeting(true, 7, true);
	}
	else
	{// ���̑�
	 // �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
		pDevice->Clear(0,
			NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
			D3DCOLOR_RGBA(157, 184, 224, 255),
			1.0f,
			0);

		//�S�Ă̕`��
		CScene::DrawAll();
	}
}

//=============================================================================
// �|�[�Y�����������ǂ���
//=============================================================================
void CGame::SetPause(bool bPause)
{
	m_bPause = bPause;

	switch (m_bPause)
	{
	case true:
		if (m_pPause == NULL)
		{
			//�|�[�Y�̐���
			m_pPause = CPause::Create();

			CScene::SetUpdatePri(7);
		}
		break;
	case false:
		if (m_pPause != NULL)
		{
			m_pPause->Uninit();
			m_pPause = NULL;

			CScene::SetUpdatePri(0);
		}
	}
}

//=============================================================================
// �Q�[�����[�h�ݒ�
//=============================================================================
void CGame::SetGameMode(GAMEMODE gameMode)
{
	CSound *pSound = CManager::GetSound();						//�T�E���h�̏��

	switch (m_gameMode)
	{// �Q�[�����[�h
	case GAMEMODE_CHARSELECT:		// �L�����I��
		if (m_pGameCharSelect != NULL)
		{// NULL�ȊO
			m_pGameCharSelect->Uninit();
			m_pGameCharSelect = NULL;
		}

		if (m_pCuorseCamera != NULL)
		{
			m_pCuorseCamera->Uninit();
			delete m_pCuorseCamera;
			m_pCuorseCamera = NULL;
		}

		break;
	case GAMEMODE_COURSESELECT:		// �R�[�X�I��		

		break;
	case GAMEMODE_COURSE_VIEW:		// �R�[�X����		
		if (m_pCuorseCamera != NULL)
		{
			m_pCuorseCamera->Uninit();
			delete m_pCuorseCamera;
			m_pCuorseCamera = NULL;
		}

		break;
	case GAMEMODE_PLAY:				// �v���C
		if (m_pGamePlay != NULL)
		{// NULL�ȊO
			m_pGamePlay->Uninit();
			m_pGamePlay = NULL;
		}

		if (m_pCuorseCamera != NULL)
		{
			m_pCuorseCamera->Uninit();
			delete m_pCuorseCamera;
			m_pCuorseCamera = NULL;
		}

		//�t�F�[�h�ȊO�̔j��
		CScene::NotFadeReleseAll();
		break;
	}

	SetPlayer(false, m_gameMode);

	// �Q�[�����[�h��ύX
	m_gameMode = gameMode;

	switch (m_gameMode)
	{// �Q�[�����[�h�ύX��
	case GAMEMODE_CHARSELECT:		// �L�����I��
		pSound->StopSound();
		//�L�����N�^�[�Z���N�g�̋�
		pSound->SetVolume(CSound::SOUND_LABEL_BGM_CHARACTERSERECT, 0.3f);
		pSound->PlaySound(CSound::SOUND_LABEL_BGM_CHARACTERSERECT);

		if (m_pGameCharSelect == NULL)
			m_pGameCharSelect = CGameCharSelect::Create();

		for (int nCntPlayer = 0; nCntPlayer < MAX_MEMBER; nCntPlayer++)
			m_nCharSelectNum[nCntPlayer] = nCntPlayer;			// �L�����I��ԍ�

																//�Q�[���J�����̐���
		if (m_pCuorseCamera == NULL)
		{// NULL
			m_pCuorseCamera = new CGameCamera;

			if (m_pCuorseCamera != NULL)
			{// NULL�ȊO
			 // ����������
				m_pCuorseCamera->Init();
				// �^�C�v�ݒ菈��
				m_pCuorseCamera->SetType(CGameCamera::CAMERA_CHARSELECT);
			}
		}

		if (m_nCntSetStage == 0)
			SetStage();

		break;
	case GAMEMODE_COURSESELECT:		// �R�[�X�I��

		break;
	case GAMEMODE_COURSE_VIEW:		// �R�[�X����
		pSound->StopSound();
		//�^�C�g���̋�
		pSound->SetVolume(CSound::SOUND_LABEL_BGM_GAME, 0.4f);
		pSound->PlaySound(CSound::SOUND_LABEL_BGM_GAME);

		//�Q�[���J�����̐���
		if (m_pCuorseCamera == NULL)
		{// NULL
			m_pCuorseCamera = new CGameCamera;

			if (m_pCuorseCamera != NULL)
			{// NULL�ȊO
			 // ����������
				m_pCuorseCamera->Init();
				// �^�C�v�ݒ菈��
				m_pCuorseCamera->SetType(CGameCamera::CAMERA_COURSE);
			}
		}

		if (m_nCntSetStage == 0)
			SetStage();

		break;
	case GAMEMODE_PLAY:				// �v���C
		pSound->StopSound();
		//�^�C�g���̋�
		pSound->SetVolume(CSound::SOUND_LABEL_BGM_GAME, 0.4f);
		pSound->PlaySound(CSound::SOUND_LABEL_BGM_GAME);

		if (m_pGamePlay == NULL)
			m_pGamePlay = CGamePlay::Create();

		// �擾
		bool bOnine = CTitle::GetOnline();

		//�Q�[���J�����̐���
		if (m_pCuorseCamera == NULL)
		{// NULL
			m_pCuorseCamera = new CGameCamera;

			if (m_pCuorseCamera != NULL)
			{// NULL�ȊO
			 // ����������
				m_pCuorseCamera->Init();
				// �^�C�v�ݒ菈��
				m_pCuorseCamera->SetType(CGameCamera::CAMERA_NONE);
				m_pCuorseCamera->SetCameraPosR(D3DXVECTOR3(-11004.0f, -3000.0f, -799.0f));
				m_pCuorseCamera->SetCameraPosV(D3DXVECTOR3(-11000.0f, 54000.0f, -800.0f));
				m_pCuorseCamera->GetfCameraAngle() = 25.0f;
				// �r���[�|�[�g
				if (m_nMaxPlayer == 1 || m_nMaxPlayer == 2 || bOnine == true)
				{
					m_pCuorseCamera->SetViewPort(
						(DWORD)((SCREEN_WIDTH * 1.0f) - (SCREEN_HEIGHT * 0.25f)),
						(DWORD)((SCREEN_HEIGHT * 0.5f) - (SCREEN_HEIGHT * 0.25f)),
						(DWORD)((SCREEN_HEIGHT * 0.25f)),
						(DWORD)((SCREEN_HEIGHT * 0.5f))
					);
				}
				else if (m_nMaxPlayer == 3)
				{
					m_pCuorseCamera->SetViewPort(
						(DWORD)((SCREEN_WIDTH * 0.75f) - (SCREEN_HEIGHT * 0.125f)),
						(DWORD)((SCREEN_HEIGHT * 0.75f) - (SCREEN_HEIGHT * 0.25f)),
						(DWORD)((SCREEN_HEIGHT * 0.25f)),
						(DWORD)((SCREEN_HEIGHT * 0.5f))
					);
				}
				else
				{
					m_pCuorseCamera->SetViewPort(
						(DWORD)((SCREEN_WIDTH * 0.5f) - (SCREEN_HEIGHT * 0.125f)),
						(DWORD)((SCREEN_HEIGHT * 0.5f) - (SCREEN_HEIGHT * 0.25f)),
						(DWORD)((SCREEN_HEIGHT * 0.25f)),
						(DWORD)((SCREEN_HEIGHT * 0.5f))
					);
				}
			}
		}

		if (m_nCntSetStage == 0)
			SetStage();

		break;
	}

	SetPlayer(true, m_gameMode);

	m_nGameCounter = 0;
}

//=============================================================================
// �X�e�[�W�ݒ�
//=============================================================================
void CGame::SetStage(void)
{
	//�T�E���h�̃|�C���^
	CSound *pSound = CManager::GetSound();

	if (m_nCntSetStage == 0)
	{
		//=====================================================================
		//	�X�e�[�W1
		//=====================================================================

		//�}�b�v��ǂݍ���
		TextLoad(6);
		WallTextLoad(6);
		MeshTextLoad(6);
		FeedTextLoad();

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

		m_nCntSetStage = 1;
	}
}

//=============================================================================
// �v���C���[�ݒ�
//=============================================================================
void CGame::SetPlayer(bool bCreate, int nMode)
{
	// �擾
	bool bOnine = CTitle::GetOnline();

	if (bCreate == false)
	{
		switch (nMode)
		{
		case GAMEMODE_CHARSELECT:		// �L�����I��
			// �L��������ւ�
			for (int nCntMember = m_nMaxPlayer; nCntMember < MAX_MEMBER; nCntMember++)
			{// �����o�[�J�E���g
				bool bCheck = false;
				do
				{
					bCheck = false;
					for (int nCntPlayer = 0; nCntPlayer < MAX_MEMBER; nCntPlayer++)
					{// �v���C���[�J�E���g
						if (nCntMember != nCntPlayer)
						{
							if (m_nCharSelectNum[nCntMember] == m_nCharSelectNum[nCntPlayer])
							{
								m_nCharSelectNum[nCntMember] = (m_nCharSelectNum[nCntMember] + 1) % MAX_MEMBER;
								bCheck = true;
								break;
							}
						}
					}
				} while (bCheck != false);
			}
		case GAMEMODE_COURSESELECT:		// �R�[�X�I��
		case GAMEMODE_COURSE_VIEW:		// �R�[�X�r���[�I��
		case GAMEMODE_PLAY:				// �v���C�I��
			for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
			{// �����o�[�J�E���g
				if (m_pPlayer[nCntMember] != NULL)
				{// NULL�ȊO
					m_pPlayer[nCntMember]->Uninit();
					m_pPlayer[nCntMember] = NULL;
				}
			}

			for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
			{// �v���C���[�J�E���g
				if (m_pGameCamera[nCntPlayer] != NULL)
				{// NULL�ȊO
					m_pGameCamera[nCntPlayer]->Uninit();
					delete m_pGameCamera[nCntPlayer];
					m_pGameCamera[nCntPlayer] = NULL;
				}
			}
			break;
		}
	}
	else
	{
		float frot = (-D3DX_PI * 0.5f);

		switch (nMode)
		{
		case GAMEMODE_CHARSELECT:		// �L�����I��
			for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
			{// �����o�[�J�E���g
			 //�v���C���[�̐���
				if (m_pPlayer[nCntMember] == NULL)
					m_pPlayer[nCntMember] = CPlayer::Create(
						D3DXVECTOR3(
						((-250.0f + (50.0f * (nCntMember / 4))) + 2950.0f),
							(-90.0f),
							(210.0f - (((70.0f * 2.0f) / 3.0f) * (nCntMember % 4)))),
						D3DXVECTOR3(0.0f, frot, 0.0f),
						nCntMember, 0, m_nCharSelectNum[nCntMember], CPlayer::PLAYERTYPE_SELECT);
			}

			for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
			{// �v���C���[�J�E���g
				if (m_pGameCamera[nCntPlayer] == NULL)
				{// NULL
					m_pGameCamera[nCntPlayer] = new CGameCamera;

					if (m_pGameCamera[nCntPlayer] != NULL)
					{// NULL�ȊO
					 // ����������
						m_pGameCamera[nCntPlayer]->Init();
						// �^�C�v�ݒ菈��
						m_pGameCamera[nCntPlayer]->SetType(CGameCamera::CAMERA_CHARUP);

						// �r���[�|�[�g�ݒ�
						m_pGameCamera[nCntPlayer]->SetViewPort(
							(DWORD)(nCntPlayer % 2 == 0 ? 0.0f + (SCREEN_HEIGHT * ((0.5f - 0.47f) * 0.5f)) : (SCREEN_WIDTH - (SCREEN_HEIGHT * 0.47f)) - (SCREEN_HEIGHT * ((0.5f - 0.47f) * 0.5f))),
							(DWORD)(nCntPlayer / 2 == 0 ? 0.0f + (SCREEN_HEIGHT * ((0.5f - 0.47f) * 0.5f)) : (SCREEN_HEIGHT - (SCREEN_HEIGHT * 0.47f)) - (SCREEN_HEIGHT * ((0.5f - 0.47f) * 0.5f))),
							(DWORD)(SCREEN_HEIGHT * 0.47f),
							(DWORD)(SCREEN_HEIGHT * 0.47f)
						);
					}
				}
			}
			break;
		case GAMEMODE_COURSESELECT:		// �R�[�X�I��
			break;
		case GAMEMODE_COURSE_VIEW:		// �R�[�X�r���[�I��
		case GAMEMODE_PLAY:				// �v���C�I��
			for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
			{// �����o�[�J�E���g
			 //�v���C���[�̐���
				if (m_pPlayer[nCntMember] == NULL)
					m_pPlayer[nCntMember] = CPlayer::Create(
						D3DXVECTOR3(-250.0f + (100.0f * (nCntMember / 4)), -90.0f, -100.0f + (((70.0f * 2.0f) / 3.0f) * (nCntMember % 4)) + (35.0f * (nCntMember / 4))),
						D3DXVECTOR3(0.0f, frot, 0.0f),
						nCntMember, (nCntMember < m_nMaxPlayer ? m_nControllerNum[nCntMember] : 0), m_nCharSelectNum[nCntMember], (nCntMember < m_nMaxPlayer ? CPlayer::PLAYERTYPE_PLAYER : CPlayer::PLAYERTYPE_ENEMY));

				if (nMode == GAMEMODE_PLAY)
				{
					if (nCntMember < m_nMaxPlayer)
					{// �v���C���[		
						if (m_pPlayer[nCntMember] != NULL)
						{// NULL�ȊO
						 //�Q�[���J�����̐���
							if (m_pGameCamera[nCntMember] == NULL)
							{// NULL
								m_pGameCamera[nCntMember] = new CGameCamera;

								if (m_pGameCamera[nCntMember] != NULL)
								{// NULL�ȊO
								 // ����������
									m_pGameCamera[nCntMember]->Init();
									// �^�C�v�ݒ菈��
									m_pGameCamera[nCntMember]->SetType(CGameCamera::CAMERA_PLAYER);
									// �Ǐ]�v���C���[�ݒ�
									m_pGameCamera[nCntMember]->SetPlayer(m_pPlayer[nCntMember]);

									// �r���[�|�[�g�ݒ�
									if (bOnine == true)
									{// �I�����C��
										m_pGameCamera[nCntMember]->SetViewPort(
											(DWORD)(0.0f),
											(DWORD)(0.0f),
											(DWORD)((SCREEN_WIDTH * 1.0f)),
											(DWORD)((SCREEN_HEIGHT * 1.0f))
										);
									}
									else
									{// ���[�J��
										m_pGameCamera[nCntMember]->SetViewPort(
											// ��
											//(DWORD)((SCREEN_WIDTH * 0.5f) * ((nCntMember) % 2)), 
											//(DWORD)((SCREEN_HEIGHT * 0.5f) * ((nCntMember) / 2)), 
											//(DWORD)((SCREEN_WIDTH * ((m_nMaxPlayer - 1) == 0 ? 1.0f : 0.5f))),
											//(DWORD)((SCREEN_HEIGHT * ((m_nMaxPlayer - 1) / 2 == 0 ? 1.0f : 0.5f)))
											// �c
											//(DWORD)((SCREEN_WIDTH * 0.5f) * ((nCntMember) / 2)),
											//(DWORD)((SCREEN_HEIGHT * 0.5f) * ((nCntMember) % 2)),
											//(DWORD)((SCREEN_WIDTH * ((m_nMaxPlayer - 1) / 2 == 0 ? 1.0f : 0.5f))),
											//(DWORD)((SCREEN_HEIGHT * ((m_nMaxPlayer - 1) == 0 ? 1.0f : 0.5f)))
											// �c->��
											(DWORD)((SCREEN_WIDTH * 0.5f) * ((m_nMaxPlayer - 1) / 2 == 0 ? ((nCntMember) / 2) : ((nCntMember) % 2))),
											(DWORD)((SCREEN_HEIGHT * 0.5f) * ((m_nMaxPlayer - 1) / 2 == 0 ? ((nCntMember) % 2) : ((nCntMember) / 2))),
											(DWORD)((SCREEN_WIDTH * ((m_nMaxPlayer - 1) / 2 == 0 ? 1.0f : 0.5f))),
											(DWORD)((SCREEN_HEIGHT * ((m_nMaxPlayer - 1) == 0 ? 1.0f : 0.5f)))
										);
									}

									// �J��������
									m_pGameCamera[nCntMember]->SetColl(true);
								}
							}
						}
					}
				}
			}
			break;
		}
	}
}

//===============================================================================
// �����L���O
//===============================================================================
void CGame::Ranking(void)
{
	CSound *pSound = CManager::GetSound();						//�T�E���h�̏��

	int	nGoulNum = 0;						// �S�[����

	for (int nCount = 0; nCount < MAX_MEMBER; nCount++)
	{// �v���C���[�J�E���g
	 // �����L���O
		m_nRanking[nCount] = CRoad_Manager::GetManager()->GetRank(nCount) - 1;

		if (m_pPlayer[nCount] != NULL)
		{// NULL�ȊO
			if (nCount < m_nMaxPlayer)
			{
				if (m_pPlayer[nCount]->GetPlayerType() == CPlayer::PLAYERTYPE_PLAYER &&
					m_pPlayer[nCount]->GetGoal() == true)
				{// �v���C���[���S�[������
					m_bGoul[nCount] = true;		// �S�[��
					nGoulNum++;					// �S�[���l��
				}
			}

			if (START_SET_TIME <= m_nGameCounter)
				if (m_pPlayer[nCount]->GetGoal() == false)
					m_nTime[nCount]++;
		}
		//CDebugProc::Print("�v���C���[�ԍ� [%d] : ���� [%d]\n", nCount, m_nRanking[nCount]);
	}

	if (nGoulNum == m_nMaxPlayer)
	{// �S�ăS�[������
		SetGameState(GAMESTATE_END);

		for (int nCount = 0; nCount < MAX_MEMBER; nCount++)
		{// �v���C���[�J�E���g
			for (int nCntCheck = 0; nCntCheck < MAX_MEMBER; nCntCheck++)
				if (m_nRanking[nCntCheck] == nCount)
				{
					m_nRankingSort[nCount] = nCntCheck;
					break;
				}
		}

		return;
	}
}

//===============================================================================
// �t�@�C�����烍�[�h
//===============================================================================
void CGame::TextLoad(int nLoadNumber)
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
// ���b�V���t�B�[���h���t�@�C�����烍�[�h
//===============================================================================
void CGame::MeshTextLoad(int nLoadNumber)
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
	pFile = fopen(TEXT_MESHFIELDNAME1, "r");

	//NULL�`�F�b�N
	if (pFile != NULL)
	{
		//������̐擪��ݒ�
		pStrcur = ReadLine(pFile, &aLine[0]);
		//����������o��
		strcpy(aStr, pStrcur);

		//������̃f�[�^ ��r���镶���� ��r���镶����
		if (memcmp(pStrcur, "MESHFIELD_SETNUM = ", strlen("MESHFIELD_SETNUM = ")) == 0)
		{
			//���o��
			pStrcur += strlen("MESHFIELD_SETNUM = ");
			//������̐擪��ݒ�
			strcpy(aStr, pStrcur);
			//�����񔲂��o��
			m_nSetMeshFieldNum = atoi(pStrcur);
		}

		//�I�u�W�F�N�g�̐�����
		for (int nCntObject = 0; nCntObject < m_nSetMeshFieldNum; nCntObject++)
		{
			//������̐擪��ݒ�
			pStrcur = ReadLine(pFile, &aLine[0]);
			//����������o��
			strcpy(aStr, pStrcur);

			if (memcmp(pStrcur, "MESHFIELD_START", strlen("MESHFIELD_START")) == 0)
			{
				while (1)
				{
					//������̐擪��ݒ�
					pStrcur = ReadLine(pFile, &aLine[0]);

					//��ނ̂�ǂݍ���
					if (memcmp(pStrcur, "TEXTURETYPE = ", strlen("TEXTURETYPE = ")) == 0)
					{
						//���o��
						pStrcur += strlen("TEXTURETYPE = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_Mesh[nCntObject].m_nTexType = atoi(pStrcur);
					}
					//���̕�������ǂݍ���
					if (memcmp(pStrcur, "X_DIVIDE = ", strlen("X_DIVIDE = ")) == 0)
					{
						//���o��
						pStrcur += strlen("X_DIVIDE = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_Mesh[nCntObject].m_nWidthDivide = atoi(pStrcur);
					}
					//�c�̕�������ǂݍ���
					if (memcmp(pStrcur, "Z_DIVIDE = ", strlen("Z_DIVIDE = ")) == 0)
					{
						//���o��
						pStrcur += strlen("Z_DIVIDE = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_Mesh[nCntObject].m_nDepthDivide = atoi(pStrcur);
					}
					//���̃e�N�X�`�����W��ǂݍ���
					if (memcmp(pStrcur, "X_TEXUV = ", strlen("X_TEXUV = ")) == 0)
					{
						//���o��
						pStrcur += strlen("X_TEXUV = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_Mesh[nCntObject].m_fTexXUV = (float)atoi(pStrcur);
					}
					//�c�̃e�N�X�`�����W��ǂݍ���
					if (memcmp(pStrcur, "Y_TEXUV = ", strlen("Y_TEXUV = ")) == 0)
					{
						//���o��
						pStrcur += strlen("Y_TEXUV = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_Mesh[nCntObject].m_fTexYUV = (float)atoi(pStrcur);
					}
					//���̒�����ǂݍ���
					if (memcmp(pStrcur, "X_LENGTH = ", strlen("X_LENGTH = ")) == 0)
					{
						//���o��
						pStrcur += strlen("X_LENGTH = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_Mesh[nCntObject].m_fWidthLength = (float)atoi(pStrcur);
					}
					//�c�̒�����ǂݍ���
					if (memcmp(pStrcur, "Z_LENGTH = ", strlen("Z_LENGTH = ")) == 0)
					{
						//���o��
						pStrcur += strlen("Z_LENGTH = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_Mesh[nCntObject].m_fDepthLength = (float)atoi(pStrcur);
					}
					//�P���_�̍���
					if (memcmp(pStrcur, "VTX0_HEIGHT = ", strlen("VTX0_HEIGHT = ")) == 0)
					{
						//���o��
						pStrcur += strlen("VTX0_HEIGHT = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_Mesh[nCntObject].m_fVtxHeight_No0 = (float)atoi(pStrcur);
					}
					//�Q���_�̍���
					if (memcmp(pStrcur, "VTX1_HEIGHT = ", strlen("VTX1_HEIGHT = ")) == 0)
					{
						//���o��
						pStrcur += strlen("VTX1_HEIGHT = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_Mesh[nCntObject].m_fVtxHeight_No1 = (float)atoi(pStrcur);
					}
					//�R���_�̍���
					if (memcmp(pStrcur, "VTX2_HEIGHT = ", strlen("VTX2_HEIGHT = ")) == 0)
					{
						//���o��
						pStrcur += strlen("VTX2_HEIGHT = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_Mesh[nCntObject].m_fVtxHeight_No2 = (float)atoi(pStrcur);
					}
					//�S���_�̍���
					if (memcmp(pStrcur, "VTX3_HEIGHT = ", strlen("VTX3_HEIGHT = ")) == 0)
					{
						//���o��
						pStrcur += strlen("VTX3_HEIGHT = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_Mesh[nCntObject].m_fVtxHeight_No3 = (float)atoi(pStrcur);
					}
					//�P���_�̍���
					if (memcmp(pStrcur, "VTX0_SIDE = ", strlen("VTX0_SIDE = ")) == 0)
					{
						//���o��
						pStrcur += strlen("VTX0_SIDE = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_Mesh[nCntObject].m_fVtxSide_No0 = (float)atoi(pStrcur);
					}
					//�Q���_�̍���
					if (memcmp(pStrcur, "VTX1_SIDE = ", strlen("VTX1_SIDE = ")) == 0)
					{
						//���o��
						pStrcur += strlen("VTX1_SIDE = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_Mesh[nCntObject].m_fVtxSide_No1 = (float)atoi(pStrcur);
					}
					//�R���_�̍���
					if (memcmp(pStrcur, "VTX2_SIDE = ", strlen("VTX2_SIDE = ")) == 0)
					{
						//���o��
						pStrcur += strlen("VTX2_SIDE = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_Mesh[nCntObject].m_fVtxSide_No2 = (float)atoi(pStrcur);
					}
					//�S���_�̍���
					if (memcmp(pStrcur, "VTX3_SIDE = ", strlen("VTX3_SIDE = ")) == 0)
					{
						//���o��
						pStrcur += strlen("VTX3_SIDE = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_Mesh[nCntObject].m_fVtxSide_No3 = (float)atoi(pStrcur);
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
						m_Mesh[nCntObject].m_pos.x = (float)atof(pStrcur);
						//���������i�߂�
						pStrcur += nWord;

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_Mesh[nCntObject].m_pos.y = (float)atof(pStrcur);
						//���������i�߂�
						pStrcur += nWord;

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_Mesh[nCntObject].m_pos.z = (float)atof(pStrcur);

					}
					else if (memcmp(pStrcur, "MESHFIELD_END", strlen("MESHFIELD_END")) == 0)
					{
						break;
					}
				}
			}
		}
	}
}

//===============================================================================
// �ǂ��t�@�C�����烍�[�h
//===============================================================================
void CGame::WallTextLoad(int nLoadNumber)
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
	pFile = fopen(TEXT_WALLNAME1, "r");

	//NULL�`�F�b�N
	if (pFile != NULL)
	{
		//������̐擪��ݒ�
		pStrcur = ReadLine(pFile, &aLine[0]);
		//����������o��
		strcpy(aStr, pStrcur);

		//������̃f�[�^ ��r���镶���� ��r���镶����
		if (memcmp(pStrcur, "WALL_SETNUM = ", strlen("WALL_SETNUM = ")) == 0)
		{
			//���o��
			pStrcur += strlen("WALL_SETNUM = ");
			//������̐擪��ݒ�
			strcpy(aStr, pStrcur);
			//�����񔲂��o��
			m_nSetWallNum = atoi(pStrcur);
		}

		//�I�u�W�F�N�g�̐�����
		for (int nCntObject = 0; nCntObject < m_nSetWallNum; nCntObject++)
		{
			//������̐擪��ݒ�
			pStrcur = ReadLine(pFile, &aLine[0]);
			//����������o��
			strcpy(aStr, pStrcur);

			if (memcmp(pStrcur, "WALL_START", strlen("WALL_START")) == 0)
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
						m_aWall[nCntObject].m_pos.x = (float)atof(pStrcur);
						//���������i�߂�
						pStrcur += nWord;

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_aWall[nCntObject].m_pos.y = (float)atof(pStrcur);
						//���������i�߂�
						pStrcur += nWord;

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_aWall[nCntObject].m_pos.z = (float)atof(pStrcur);

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
						m_aWall[nCntObject].m_rot.x = (float)atof(pStrcur);
						//���������i�߂�
						pStrcur += nWord;

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_aWall[nCntObject].m_rot.y = (float)atof(pStrcur);
						//���������i�߂�
						pStrcur += nWord;

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_aWall[nCntObject].m_rot.z = (float)atof(pStrcur);

					}
					//WIDTH��ǂݍ���
					if (memcmp(pStrcur, "WIDTH = ", strlen("WIDTH = ")) == 0)
					{
						//���o��
						pStrcur += strlen("WIDTH = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_aWall[nCntObject].m_fWidthDivide = (float)atof(pStrcur);
					}
					//HIGHT��ǂݍ���
					if (memcmp(pStrcur, "HIGHT = ", strlen("HIGHT = ")) == 0)
					{
						//���o��
						pStrcur += strlen("HIGHT = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_aWall[nCntObject].m_fHightDivide = (float)atof(pStrcur);
					}
					//HIGHT��ǂݍ���
					if (memcmp(pStrcur, "TEXTURE = ", strlen("TEXTURE = ")) == 0)
					{
						//���o��
						pStrcur += strlen("TEXTURE = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);

						//��������Ԃ��Ă��炤
						nWord = PopString(pStrcur, &aStr[0]);
						//������ϊ�
						m_aWall[nCntObject].m_nTexType = (int)atof(pStrcur);
					}
					else if (memcmp(pStrcur, "WALL_END", strlen("WALL_END")) == 0)
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
void CGame::FeedTextLoad(void)
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
char *CGame::ReadLine(FILE *pFile, char *pDst)
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
char *CGame::GetLineTop(char * pStr)
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
int CGame::PopString(char * pStr, char * pDest)
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