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
#include "enemy.h"
#include "chick.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXT_OBJECTNAME1		"data\\TEXT\\�Q�[���}�b�v\\objecy.txt"			// �ǂݍ��ރe�L�X�g�t�@�C��
#define TEXT_MESHFIELDNAME1		"data\\TEXT\\�Q�[���}�b�v\\meshfield.txt"		// �ǂݍ��ރe�L�X�g�t�@�C��
#define TEXT_WALLNAME1			"data\\TEXT\\�Q�[���}�b�v\\wall.txt"			// �ǂݍ��ރe�L�X�g�t�@�C��
#define TEXT_PLAYER_MOTION		"data\\TEXT\\Player\\Player.txt"			// �v���C���[�̃��[�V�����t�@�C��
#define VECTOR_ZERO				(D3DXVECTOR3(0.0f, 0.0f, 0.0f))

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CGameCharSelect *CGame::m_pGameCharSelect = NULL;	// �Q�[���i�L�����I���j
CGamePlay *CGame::m_pGamePlay = NULL;				// �Q�[���i�v���C�j

CPlayer *CGame::m_pPlayer[MAX_PLAYER] = { NULL };
CPause *CGame::m_pPause = NULL;
CLoadTextMotion * CGame::m_pPlayerMotion = NULL;
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
int CGame::m_nCharSelectNum[MAX_PLAYER] = { 0 };	// �L�����I��ԍ�
int CGame::m_nControllerNum[MAX_PLAYER] = { 0 };	// �R���g���[���[�ԍ�
int CGame::m_nRanking[MAX_PLAYER] = { 0 };			// �����L���O
bool CGame::m_bGoul[MAX_PLAYER] = { false };		// �S�[��

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
	CBillBoord::Load();			//�r���{�[�h�e�N�X�`���̓ǂݍ���
	CWall::Load();				//�ǂ̃e�N�X�`���̓ǂݍ���
	CShadow::Load();			//�e�̃e�N�X�`���ǂݍ���
	CObject::Load();			//�I�u�W�F�N�g�̃e�N�X�`���̓ǂݍ���
	CFeed::Load();				//�H�ו��̃e�N�X�`���̓ǂݍ���
	CEgg::Load();				//���̃e�N�X�`���̓ǂݍ���
	CEnemy::Load();				//�G�i���j�̃e�N�X�`���̓ǂݍ���
	CChick::Load();				//�Ђ悱�̃e�N�X�`���̓ǂݍ���
	m_pPause->Load();			//�|�[�Y�̃e�N�X�`���̓ǂݍ���

	CGameCharSelect::Load();	// �Q�[���i�L�����I���j
	CGamePlay::Load();			// �Q�[���i�v���C�j

	//====================================================================
	//						 �K�v�ȕϐ��̏�����
	//====================================================================

	m_gameMode = GAMEMODE_CHARSELECT;	// �Q�[�����[�h
	m_gameModeNext = m_gameMode;		// ���̃Q�[�����[�h
	m_gameState = GAMESTATE_NORMAL;		//�ʏ��Ԃ�
	m_nCntSetStage = 0;					//�ǂ��̃X�e�[�W����J�n���邩
	m_bPause = false;					//�|�[�Y��������
	m_nGameCounter = 0;					//�J�E���^�[�̏�����

	m_nMaxPlayer = 0;					// �v���C���[��
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		m_nCharSelectNum[nCntPlayer] = nCntPlayer;		// �L�����I��ԍ�
		m_nControllerNum[nCntPlayer] = nCntPlayer;		// �R���g���[���[�ԍ�
		m_nRanking[nCntPlayer] = nCntPlayer;			// �����L���O
		m_bGoul[nCntPlayer] = false;					// �S�[��
	}

	// �f�o�b�O�p
	if (m_gameMode == GAMEMODE_PLAY)
		m_nMaxPlayer = 2;

	SetGameMode(m_gameMode);			// �Q�[�����[�h�ݒ�

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

	CMeshField::UnLoad();			//���b�V���t�B�[���h�e�N�X�`���̔j��
	CFade::UnLoad();				//�t�F�[�h�̃e�N�X�`���̔j��
	CBillBoord::UnLoad();			//�r���{�[�h�e�N�X�`���̔j��
	CObject::UnLoad();				//�I�u�W�F�N�g�̃e�N�X�`���̔j��
	CWall::UnLoad();				//�ǂ̃e�N�X�`���̔j
	CShadow::UnLoad();				//�e�̃e�N�X�`���̔j��
	CFeed::UnLoad();				//�a�̃e�N�X�`���̔j��
	CEgg::UnLoad();					//���̃e�N�X�`���̔j��
	CEnemy::UnLoad();				//�G�̃e�N�X�`���̔j��
	CChick::UnLoad();				//�Ђ悱�̃e�N�X�`���̔j��

	CGameCharSelect::Unload();		// �Q�[���i�L�����I���j
	CGamePlay::Unload();			// �Q�[���i�v���C�j

									//�v���C���[���f���̔j��
	CPlayer::UnloadModel();

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

	//�v���C���[�̃��[�V�����̔j��
	if (m_pPlayerMotion != NULL)
	{
		m_pPlayerMotion->Uninit();
		delete m_pPlayerMotion;
		m_pPlayerMotion = NULL;
	}

	//�J�����̔j��
	if (m_pCuorseCamera != NULL)
	{
		m_pCuorseCamera->Uninit();
		delete m_pCuorseCamera;
		m_pCuorseCamera = NULL;
	}

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if (m_pPlayer[nCntPlayer] != NULL)
		{// NULL�ȊO
			m_pPlayer[nCntPlayer]->Uninit();
			m_pPlayer[nCntPlayer] = NULL;
		}

		if (m_pGameCamera[nCntPlayer] != NULL)
		{
			m_pGameCamera[nCntPlayer]->Uninit();
			delete m_pGameCamera[nCntPlayer];
			m_pGameCamera[nCntPlayer] = NULL;
		}
	}

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
	CInputXPad * pXpad = CManager::GetXInput();					//�W���C�p�b�g�̎擾
	CSound *pSound = CManager::GetSound();						//�T�E���h�̏��
	CFade::FADE fade = CFade::GetFade();

	if (m_gameMode != m_gameModeNext)
	{
		SetGameMode(m_gameModeNext);
		return;
	}

	switch (m_gameMode)
	{
	case GAMEMODE_CHARSELECT:
		//if (pCInputKeyBoard->GetKeyboardTrigger(DIK_RETURN) == true)
		//	CFade::Create(GAMEMODE_PLAY);
		break;
	case GAMEMODE_COURSESELECT:
		if (pCInputKeyBoard->GetKeyboardTrigger(DIK_RETURN) == true)
			CFade::Create(GAMEMODE_PLAY);

		break;
	case GAMEMODE_COURSE_VIEW:
		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
			if (m_pPlayer[nCntPlayer] != NULL)
				m_pPlayer[nCntPlayer]->SetControl(false);

		break;
	case GAMEMODE_PLAY:
		//���݂̏�Ԃ�ۑ�
		m_NowGameState = GetGameState();

		switch (m_NowGameState)
		{
		case GAMESTATE_NORMAL:	//�ʏ�̏��
			if (m_nGameCounter < START_SET_TIME)
			{
				for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
					if (m_pPlayer[nCntPlayer] != NULL)
						m_pPlayer[nCntPlayer]->SetControl(false);
			}
			else if (m_nGameCounter == START_SET_TIME)
			{
				for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
					if (m_pPlayer[nCntPlayer] != NULL)
						m_pPlayer[nCntPlayer]->SetControl(true);
			}

			Ranking();		// �����L���O

			break;
		case GAMESTATE_END:			//�Q�[���I�����
			m_nCounterGameState++;

			if (180 < m_nCounterGameState)
			{//��ʁi���[�h�j�̐ݒ�
				CFade::Create(CManager::MODE_TITLE);
			}
			break;
		}

		break;
	}

	if (fade == CFade::FADE_NONE)
	{
		//�|�[�Y�̏���
		if (pCInputKeyBoard->GetKeyboardTrigger(DIK_P) == true)
		{//P�L�[�������ꂽ��
			m_bPause = m_bPause ? false : true;

			switch (m_bPause)
			{
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

	if (m_bPause == false)
	{//�J����
		if (pCInputKeyBoard->GetKeyboardTrigger(DIK_RETURN) == true)
		{
			//m_bDrawUI = m_bDrawUI ? false : true;

			//�|�[�Y�̑I���̌��艹
			CFade::Create(CManager::MODE_RESULT);
			return;
		}

		//�J�����̍X�V����
		if (m_pCuorseCamera != NULL) { m_pCuorseCamera->Update(); }

		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
			if (m_pGameCamera[nCntPlayer] != NULL) { m_pGameCamera[nCntPlayer]->Update(); }

		//	�J�E���^�[�i�߂�
		m_nGameCounter++;
	}
	else
	{
		//m_pPause->Update();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CGame::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (m_gameMode == GAMEMODE_COURSE_VIEW)
	{// ���̑�
	 // �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
		pDevice->Clear(0,
			NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(157, 184, 224, 255),
			1.0f,
			0);

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

		for (int nCntPlayer = 0; nCntPlayer < m_nMaxPlayer; nCntPlayer++)
		{// �v���C���[�J�E���g
		 //�J�����̐ݒ�
			if (m_pGameCamera[nCntPlayer] != NULL) { m_pGameCamera[nCntPlayer]->SetCamera(); }

			//�S�Ă̕`��
			CScene::DrawAll();
		}

		pDevice->SetViewport(&viewport);	// �r���[�|�[�g�ݒ�

											//�QD�̕`��
		CScene::Draw2D();
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

	if (m_pPause != NULL)
	{
	//	m_pPause->Draw();
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

		//�t�F�[�h�ȊO�̔j��
		CScene::NotFadeReleseAll();
		break;
	}

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

		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
			m_nCharSelectNum[nCntPlayer] = nCntPlayer;			// �L�����I��ԍ�

		break;
	case GAMEMODE_COURSESELECT:		// �R�[�X�I��

		break;
	case GAMEMODE_COURSE_VIEW:		// �R�[�X����
		pSound->StopSound();
		//�^�C�g���̋�
		pSound->SetVolume(CSound::SOUND_LABEL_BGM_GAME, 0.4f);
		pSound->PlaySound(CSound::SOUND_LABEL_BGM_GAME);

		if (m_nCntSetStage == 0)
			SetStage();

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

		break;
	case GAMEMODE_PLAY:				// �v���C
		pSound->StopSound();
		//�^�C�g���̋�
		pSound->SetVolume(CSound::SOUND_LABEL_BGM_GAME, 0.4f);
		pSound->PlaySound(CSound::SOUND_LABEL_BGM_GAME);

		if (m_pGamePlay == NULL)
			m_pGamePlay = CGamePlay::Create();

		if (m_nCntSetStage == 0)
			SetStage();

		break;
	}

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

		//===================================
		//		Create
		//===================================

		if (m_pPlayerMotion == NULL) { m_pPlayerMotion = CLoadTextMotion::Create(TEXT_PLAYER_MOTION); }	//�v���C���[�̃��[�V�����ǂݍ���
		CPlayer::LoadModel();	//���f���̓ǂݍ���

		for (int nCntPlayer = 0; nCntPlayer < m_nMaxPlayer; nCntPlayer++)
		{// �v���C���[�J�E���g
		 //�v���C���[�̐���
			if (m_pPlayer[nCntPlayer] == NULL)
				m_pPlayer[nCntPlayer] = CPlayer::Create(D3DXVECTOR3(-150.0f + (100.0f * nCntPlayer), 0.0f, (-50.0f * nCntPlayer)), nCntPlayer, m_nControllerNum[nCntPlayer]);
			//m_pPlayer[nCntPlayer] = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), nCntPlayer, m_nControllerNum[nCntPlayer]);

			if (m_pPlayer[nCntPlayer] != NULL)
			{// NULL�ȊO
			 //�Q�[���J�����̐���
				if (m_pGameCamera[nCntPlayer] == NULL)
				{// NULL
					m_pGameCamera[nCntPlayer] = new CGameCamera;

					if (m_pGameCamera[nCntPlayer] != NULL)
					{// NULL�ȊO
					 // ����������
						m_pGameCamera[nCntPlayer]->Init();
						// �^�C�v�ݒ菈��
						m_pGameCamera[nCntPlayer]->SetType(CGameCamera::CAMERA_PLAYER);
						// �Ǐ]�v���C���[�ݒ�
						m_pGameCamera[nCntPlayer]->SetPlayer(m_pPlayer[nCntPlayer]);
						// �r���[�|�[�g�ݒ�
						m_pGameCamera[nCntPlayer]->SetViewPort(
							// ��
							//(DWORD)((SCREEN_WIDTH * 0.5f) * ((nCntPlayer) % 2)), 
							//(DWORD)((SCREEN_HEIGHT * 0.5f) * ((nCntPlayer) / 2)), 
							//(DWORD)((SCREEN_WIDTH * ((m_nMaxPlayer - 1) == 0 ? 1.0f : 0.5f))),
							//(DWORD)((SCREEN_HEIGHT * ((m_nMaxPlayer - 1) / 2 == 0 ? 1.0f : 0.5f)))
							// �c
							//(DWORD)((SCREEN_WIDTH * 0.5f) * ((nCntPlayer) / 2)),
							//(DWORD)((SCREEN_HEIGHT * 0.5f) * ((nCntPlayer) % 2)),
							//(DWORD)((SCREEN_WIDTH * ((m_nMaxPlayer - 1) / 2 == 0 ? 1.0f : 0.5f))),
							//(DWORD)((SCREEN_HEIGHT * ((m_nMaxPlayer - 1) == 0 ? 1.0f : 0.5f)))
							// �c->��
							(DWORD)((SCREEN_WIDTH * 0.5f) * ((m_nMaxPlayer - 1) / 2 == 0 ? ((nCntPlayer) / 2) : ((nCntPlayer) % 2))),
							(DWORD)((SCREEN_HEIGHT * 0.5f) * ((m_nMaxPlayer - 1) / 2 == 0 ? ((nCntPlayer) % 2) : ((nCntPlayer) / 2))),
							(DWORD)((SCREEN_WIDTH * ((m_nMaxPlayer - 1) / 2 == 0 ? 1.0f : 0.5f))),
							(DWORD)((SCREEN_HEIGHT * ((m_nMaxPlayer - 1) == 0 ? 1.0f : 0.5f)))
						);
					}
				}
			}
		}

		for (int nCount = 0; nCount < m_nSetObjectNum; nCount++)
		{
			//�I�u�W�F�N�g�̐���
			CObject::Create(m_Map[nCount].m_pos, m_Map[nCount].m_rot, m_Map[nCount].m_scale, 0.0f, m_Map[nCount].m_nTexType, m_Map[nCount].m_nType, CModel3D::MOVETYPE_NOT, m_Map[nCount].m_nCollision);
		}
		for (int nCount = 0; nCount < m_nSetMeshFieldNum; nCount++)
		{
			//�t�B�[���h�̐���
			CMeshField::Create(m_Mesh[nCount].m_pos, m_Mesh[nCount].m_nWidthDivide, m_Mesh[nCount].m_nDepthDivide, m_Mesh[nCount].m_fTexXUV, m_Mesh[nCount].m_fTexYUV,
				m_Mesh[nCount].m_fWidthLength, m_Mesh[nCount].m_fDepthLength,
				m_Mesh[nCount].m_fVtxHeight_No0, m_Mesh[nCount].m_fVtxHeight_No1, m_Mesh[nCount].m_fVtxHeight_No2, m_Mesh[nCount].m_fVtxHeight_No3,
				m_Mesh[nCount].m_fVtxSide_No0, m_Mesh[nCount].m_fVtxSide_No1, m_Mesh[nCount].m_fVtxSide_No2, m_Mesh[nCount].m_fVtxSide_No3,
				m_Mesh[nCount].m_nTexType, 0);
		}
		for (int nCount = 0; nCount < m_nSetWallNum; nCount++)
		{
			//�ǂ̐���
			CWall::Create(m_aWall[nCount].m_pos, D3DXVECTOR2(m_aWall[nCount].m_fWidthDivide, m_aWall[nCount].m_fHightDivide), m_aWall[nCount].m_rot, m_aWall[nCount].m_nTexType);
		}

		// �a�̐���
		CFeed::Create(D3DXVECTOR3(0.0f, 1.0f, 700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), CFeed::FEEDTYPE_ATTACK);
		CFeed::Create(D3DXVECTOR3(100.0f, 1.0f, 700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), CFeed::FEEDTYPE_ANNOY);
		CFeed::Create(D3DXVECTOR3(200.0f, 1.0f, 700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), CFeed::FEEDTYPE_SPEED);

		CFeed::Create(D3DXVECTOR3(-300.0f, 1.0f, 1700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), CFeed::FEEDTYPE_ATTACK);
		CFeed::Create(D3DXVECTOR3(-300.0f, 1.0f, 1800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), CFeed::FEEDTYPE_ANNOY);
		CFeed::Create(D3DXVECTOR3(-300.0f, 1.0f, 1900.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), CFeed::FEEDTYPE_SPEED);

		// �G�̐���
		CEnemy::Create(D3DXVECTOR3(-300.0f, 1.0f, 1500.0f));

		m_nCntSetStage = 1;
	}
}

//===============================================================================
// �����L���O
//===============================================================================
void CGame::Ranking(void)
{
	CSound *pSound = CManager::GetSound();						//�T�E���h�̏��

																// �����𑪂�
	float fLenght[MAX_PLAYER] = { 0.0f };	// ����
	int	nGoulNum = 0;						// �S�[����

	for (int nCntPlayer = 0; nCntPlayer < m_nMaxPlayer; nCntPlayer++)
	{// �v���C���[�J�E���g
		if (m_pPlayer[nCntPlayer] != NULL)
		{// NULL�ȊO
		 // �ʒu�擾
			D3DXVECTOR3 pos = m_pPlayer[nCntPlayer]->GetPos();
			if (m_bGoul[nCntPlayer] == false)
			{// �S�[�����Ă��Ȃ�
				fLenght[nCntPlayer] = 26000.0f - pos.z;
				if (fLenght[nCntPlayer] < 0.0f)
				{// �S�[���ɂ���
					fLenght[nCntPlayer] = 0.0f;
					m_bGoul[nCntPlayer] = true;

					// ���͂��Ȃ�
					m_pPlayer[nCntPlayer]->SetControl(false);

					//���艹�̐ݒ�
					pSound->SetVolume(CSound::SOUND_LABEL_SE_CHARACTERSERECT, 1.3f);
					pSound->PlaySound(CSound::SOUND_LABEL_SE_CHARACTERSERECT);

					nGoulNum++;		// �S�[�����J�E���g
				}
			}
			else
			{// �S�[������
				nGoulNum++;			// �S�[�����J�E���g
			}
			// �ʒu�ݒ�
			m_pPlayer[nCntPlayer]->SetPos(pos);
		}
	}

	for (int nCntPlayer = 0; nCntPlayer < m_nMaxPlayer; nCntPlayer++)
	{// �v���C���[�J�E���g
		if (m_bGoul[nCntPlayer] == false)
		{// �S�[�����Ă��Ȃ�
			int nRank = 0;
			for (int nCntCheck = 0; nCntCheck < m_nMaxPlayer; nCntCheck++)
			{// �`�F�b�N�J�E���g
				if (nCntPlayer != nCntCheck)
				{// �����ȊO
					if (m_bGoul[nCntPlayer] == false && m_bGoul[nCntCheck] == false)
					{// �S�[�����Ă��Ȃ�
						if (fLenght[nCntCheck] < fLenght[nCntPlayer])	// �������O�͉��l���邩
							nRank++;	// �J�E���g
					}
				}
			}
			// �O�ɂ����l�� + �S�[���ς݂̐l��
			m_nRanking[nCntPlayer] = nRank + nGoulNum;
		}
		CDebugProc::Print("�����L���O : %d\n", m_nRanking[nCntPlayer]);
	}


	if (nGoulNum == m_nMaxPlayer)
	{// �S�ăS�[������
		SetGameState(GAMESTATE_END);
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