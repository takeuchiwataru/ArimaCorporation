//=============================================================================
//
// �Z���N�g�̏��� [select.cpp]
// Author : shun yokomichi
//
//=============================================================================
#include "select.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "background.h"
#include "texture.h"
#include "logo.h"
#include "satisfactionlevel.h"
#include "totalscore.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SELECT_LOGO_POS			(D3DXVECTOR3(SCREEN_WIDTH / 2, 70.0f, 0.0f))					// �ʒu(�^�C�g�����S000)
#define SELECT_LOGO_SIZE		(D3DXVECTOR2(200.0f, 60.0f))									// �T�C�Y(�^�C�g�����S000)
#define GAME_POS				(D3DXVECTOR3(SCREEN_WIDTH / 2, 310.0f, 0.0f))					// �Q�[�����(TEXTURE)�̈ʒu
#define GAME_SIZE				(D3DXVECTOR2(500.0f, 250.0f))									// �Q�[�����(TEXTURE)�̃T�C�Y
#define QUIT_POS				(D3DXVECTOR3(GAME_POS.x, GAME_POS.y + 330.0f, 0.0f))			// Quit�̈ʒu
#define QUIT_SIZE				(D3DXVECTOR2(530.0f, 70.0f))									// Quit�̃T�C�Y
#define TOTAL_POS				(D3DXVECTOR3(320.0f, GAME_POS.y + 250.0f, 0.0f))				// �g�[�^���̈ʒu
#define TOTAL_SIZE				(D3DXVECTOR2(200.0f, 60.0f))									// �g�[�^���̃T�C�Y
#define SATYIFACTION_POS		(D3DXVECTOR3(250.0f - 100.0f, TOTAL_POS.y + 60.0f, 0.0f))	// �����x�̈ʒu
#define SATYIFACTION_SIZE		(D3DXVECTOR2(15.0f / 4, 25.0f))									// �����x�̃T�C�Y
#define SCORE_POS				(D3DXVECTOR3(250.0f + 800.0f, TOTAL_POS.y + 60.0f, 0.0f))	// ���݂̍ō��X�R�A�̈ʒu
#define MONEY_POS				(D3DXVECTOR3(250.0f + 530.0f, TOTAL_POS.y + 60.0f, 0.0f))	// \�}�[�N�̈ʒu
#define MONEY_SIZE				(D3DXVECTOR2(25.0f, 23.0f))										// \�}�[�N�̃T�C�Y
#define CIRCLE_POS				(D3DXVECTOR3(SCREEN_WIDTH / 2, 520.0f, 0.0f))					// �T�[�N���̈ʒu
#define CIRCLE_SIZE				(D3DXVECTOR2(100.0f, 100.0f))									// �T�[�N���̃T�C�Y
#define FREAM_POS				(D3DXVECTOR3(QUIT_POS.x, QUIT_POS.y, 0.0f))						// �t���[���̈ʒu
#define FREAM_SIZE000			(D3DXVECTOR2(GAME_SIZE.x + 25.0f , GAME_SIZE.y + 20.0f))		// �t���[���̃T�C�Y(�Q�[�����)
#define FREAM_SIZE001			(D3DXVECTOR2(QUIT_SIZE.x + 20.0f , QUIT_SIZE.y + 18.0f))		// �t���[���̃T�C�Y(Quit)

//-------------------------------------------
// �t�@�C����
//-------------------------------------------
#define TOTALSCORE_TXT_NAME		("data/TEXT/Ranking/ranking.bin")
#define TOTALSCORE_TXT_NAME2	("data/TEXT/Ranking/ranking001.bin")
#define SATISFACTION_TXT_NAME	("data/TEXT/Ranking/SatisfactionLevel.bin")
#define SATISFACTION_TXT_NAME2	("data/TEXT/Ranking/SatisfactionLevel001.bin")

//-------------------------------------------
// ���̑�
//-------------------------------------------
#define ROT_MOVE				(0.5f)				// ��]���x
#define ANIM_COUNT				(120)				// �A�j���[�V�����J�E���^�[
#define PATTERN_NUM				(4)					// �p�^�[���̐�
#define MENU_POS				(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100.0f + (nCntMenu * 300.0f), 0.0f))	// continu �` Quit�܂ňʒu
#define DEFAULT_POS_LEFT		(D3DXVECTOR3(0.0f, MENU_POS.y, 0.0f))						// �f�t�H���g�̈ʒu(��)�̏ꍇ
#define DEFAULT_POS_RIGHT		(D3DXVECTOR3(SCREEN_WIDTH, MENU_POS.y, 0.0f))				// �f�t�H���g�̈ʒu(�E)�̏ꍇ
#define END_POS_LEFT			(D3DXVECTOR3(-200.0f, MENU_POS.y, 0.0f))					// �I����̈ʒu(��)�̏ꍇ
#define END_POS_RIGHT			(D3DXVECTOR3(SCREEN_WIDTH + 200.0f, MENU_POS.y, 0.0f))		// �I����̈ʒu(�E)�̏ꍇ
#define MENU_SIZE				(D3DXVECTOR2(550.0f, 150.0f))								// continu �` Quit�܂ŃT�C�Y
#define MOVE					(27.0f)														// ���E�̈ړ���

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
int CSelect::m_nStageNumber = 0;

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CSelect::CSelect()
{
	m_pCircle = NULL;
	m_pFream = NULL;
	m_pBg = NULL;
	m_bPress = false;
	m_bSame = false;
	m_fAddRot = D3DX_PI;
	m_nCntAnim = 0;
	m_nCntPattern = 0;
	m_nStageNum = 1;
	m_nStageNumber = 0;
	m_bEnter = false;
	m_bSelectFlag = false;
	m_bLevelFlag = false;
	m_bMove = false;
	for (int nCntStage = 0; nCntStage < MAX_SELECT_STAGE; nCntStage++)
	{
		m_apMenu[nCntStage] = NULL;
		m_pHuman[nCntStage] = NULL;
		m_nSizeOld[nCntStage] = {};
		m_pSatisfaction[nCntStage] = NULL;
		m_pTotalScore[nCntStage] = NULL;
		m_apHighScore[nCntStage] = NULL;
		m_apMoney[nCntStage] = NULL;
	}
	for (int nCntNum = 0; nCntNum < MAX_SELECT_NUM; nCntNum++)
	{
		m_apSelectLevel[nCntNum] = NULL;
	}
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSelect::~CSelect()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CSelect::Init()
{
	int nTotalScore = 0;
	int nSatisfaction = 0;
	//===================================
	//		 Load�̓ǂݍ��ݏꏊ
	//===================================
	//�t�F�[�h�̃e�N�X�`���̓ǂݍ���
	CFade::Load();

	CBackGround::Create(CTexture::TYPE_BACKGROUND000);

	// �t���[���̐���
	if (m_pFream == NULL)
	{
		m_pFream = new CScene2D;
		m_pFream->Init();
		m_pFream->SetPosSize(FREAM_POS, FREAM_SIZE001);
		m_pFream->SetColor(&D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f));
	}

	// �I����ʂ̐���
	for (int nCntSelect = 0; nCntSelect < MAX_SELECT_STAGE; nCntSelect++)
	{
		if (m_apMenu[nCntSelect] == NULL)
		{
			if (nCntSelect == 0)
			{	// Quit�摜
				m_apMenu[nCntSelect] = CLogo::Create(QUIT_POS, QUIT_SIZE, CTexture::TYPE_SELECT_QUIT, 0, CLogo::TYPE_LOGO);
				m_nSizeOld[nCntSelect] = FREAM_SIZE001;
				m_apMenu[nCntSelect]->SetTexture(0, 1, 2, 1);
			}
			else
			{	// �X�e�[�W�摜
				m_apMenu[nCntSelect] = CLogo::Create(GAME_POS, GAME_SIZE, CTexture::TYPE_SELECT_GAME, 0, CLogo::TYPE_LOGO);
				m_nSizeOld[nCntSelect] = FREAM_SIZE000;
			}
		}
	}

	// �Z���N�g���S
	CLogo::Create(SELECT_LOGO_POS, SELECT_LOGO_SIZE, CTexture::TYPE_SELECT_LOGO, 0, CLogo::TYPE_LOGO);


	// �T�[�N���A�C�R���̐���
	if (m_pCircle == NULL)
	{
		m_pCircle = CLogo::Create(CIRCLE_POS, CIRCLE_SIZE, CTexture::TYPE_SELECT_CIRCLE, 0, CLogo::TYPE_LOGO);
	}

	for (int nCntHuman = 0; nCntHuman < MAX_SELECT_STAGE; nCntHuman++)
	{
		if (m_pHuman[nCntHuman] == NULL)
		{
			if (nCntHuman == 0) { m_pHuman[nCntHuman] = CLogo::Create(D3DXVECTOR3(80.0f, 670.0f, 0.0f), D3DXVECTOR2(80.0f, 120.0f), CTexture::TYPE_SELECT_HUMAN002, 0, CLogo::TYPE_LOGO); }
			else { m_pHuman[nCntHuman] = CLogo::Create(D3DXVECTOR3(1190.0f, 670.0f, 0.0f), D3DXVECTOR2(90.0f, 120.0f), CTexture::TYPE_SELECT_HUMAN003, 0, CLogo::TYPE_LOGO); }
		}

	}
	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CSelect::Uninit(void)
{
	//===================================
	//	�@�@UnLoad�̔j������ꏊ
	//===================================
	//�t�F�[�h�̃e�N�X�`���̔j��
	CFade::UnLoad();

	// �I����ʂ̔j��
	for (int nCntSelect = 0; nCntSelect < MAX_SELECT_STAGE; nCntSelect++)
	{
		if (m_apMenu[nCntSelect] != NULL)
		{
			m_apMenu[nCntSelect]->Uninit();
			m_apMenu[nCntSelect] = NULL;
		}
	}

	// �T�[�N���A�C�R���̔j��
	if (m_pCircle != NULL)
	{
		m_pCircle->Uninit();
		m_pCircle = NULL;
	}

	// �t���[���̔j��
	if (m_pFream != NULL)
	{
		m_pFream->Uninit();
		m_pFream = NULL;
	}

	for (int nCntHuman = 0; nCntHuman < MAX_SELECT_STAGE; nCntHuman++)
	{

		// �����x�̔j��
		if (m_pSatisfaction[nCntHuman] != NULL)
		{
			m_pSatisfaction[nCntHuman]->Uninit();
			m_pSatisfaction[nCntHuman] = NULL;
		}

		// �g�[�^���X�R�A�̔j��
		if (m_pTotalScore[nCntHuman] != NULL)
		{
			m_pTotalScore[nCntHuman]->Uninit();
			m_pTotalScore[nCntHuman] = NULL;
		}

		if (m_pHuman[nCntHuman] != NULL)
		{
			m_pHuman[nCntHuman]->Uninit();
			m_pHuman[nCntHuman] = NULL;
		}

		if (m_apHighScore[nCntHuman] != NULL)
		{
			m_apHighScore[nCntHuman]->Uninit();
			m_apHighScore[nCntHuman] = NULL;
		}

		if (m_apMoney[nCntHuman] != NULL)
		{
			m_apMoney[nCntHuman]->Uninit();
			m_apMoney[nCntHuman] = NULL;
		}
	}

	if (m_pBg != NULL)
	{
		m_pBg->Uninit();
		m_pBg = NULL;
	}

	for (int nCntLevel = 0; nCntLevel < MAX_SELECT_NUM; nCntLevel++)
	{
		if (m_apSelectLevel[nCntLevel] != NULL)
		{
			m_apSelectLevel[nCntLevel]->Uninit();
			m_apSelectLevel[nCntLevel] = NULL;
		}
	}
	//�t�F�[�h�ȊO�폜
	CScene::NotFadeReleseAll();
}
//=============================================================================
// �X�V����
//=============================================================================
void CSelect::Update(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CXInput *pCInputJoypad = CManager::GetXInput();
	CInputMouse *pCInputMouse = CManager::GetInputMouse();

	if (m_bSelectFlag == false)
	{
		if (pCInputJoypad->GetAnyButton() == true)
		{//�����L���O����^�C�g����
			if (m_nStageNum == 0)
			{
				Fade();	//�t�F�[�h���n�܂�����
			}
			else if (m_nStageNum == 1) { m_bSelectFlag = true; }
			CManager::SetInputFlag(false);	//�Q�[���p�b�h�̓��͏�Ԃɂ���
		}
		else if (pCInputKeyBoard->GetKeyboardTrigger(DIK_RETURN))
		{
			if (m_nStageNum == 0)
			{
				Fade();	//�t�F�[�h���n�܂�����
			}
			else if (m_nStageNum == 1) { m_bSelectFlag = true; }
			CManager::SetInputFlag(false);	//�Q�[���p�b�h�̓��͏�Ԃɂ���
		}
	}
	else if (m_bSelectFlag == true)
	{
		SelectLevel();
	}

	if (m_nStageNum == 1 && m_bMove == true)
	{
		if(m_nStageNumber != 2)
		{
			if (pCInputJoypad->GetAnyButton() == true)
			{//�����L���O����^�C�g����
				Fade();	//�t�F�[�h���n�܂�����
				CManager::SetInputFlag(false);	//�Q�[���p�b�h�̓��͏�Ԃɂ���
			}
			else if (pCInputKeyBoard->GetKeyboardTrigger(DIK_RETURN))
			{
				Fade();	//�t�F�[�h���n�܂�����
				CManager::SetInputFlag(true);	//�L�[�{�[�h�̓��͏�Ԃɂ���
			}
		}
		else if (m_nStageNumber == 2)
		{
			if (pCInputJoypad->GetAnyButton() == true)
			{//�����L���O����^�C�g����
				BackUninit();
			}
			else if (pCInputKeyBoard->GetKeyboardTrigger(DIK_RETURN))
			{
				BackUninit();
			}
		}
		
	}

	if (m_bEnter == false && m_bSelectFlag == false)
	{//	���艟���Ă��Ȃ��ꍇ����
		SlectStage();		// �I��
		// �e�N�X�`���A�j���[�V�����̐ݒ�
		TexAnim();
	}
	else if (m_bSelectFlag == true && m_bEnter == false)
	{
		Select();
	}

	// ��ʑI��
	for (int nCntSelect = 0; nCntSelect < MAX_SELECT_STAGE; nCntSelect++)
	{
		if (m_apMenu[nCntSelect] != NULL)
		{
			m_apMenu[nCntSelect]->Update();
		}
	}

	SelectHuman();

	// �T�[�N���A�C�R���̍X�V
	if (m_pCircle != NULL) { m_pCircle->SetRot(m_fAddRot, CIRCLE_SIZE);}

	for (int nCntSelect = 0; nCntSelect < MAX_SELECT_STAGE; nCntSelect++)
	{
		// �����x�̍X�V
		if (m_pSatisfaction[nCntSelect] != NULL) { m_pSatisfaction[nCntSelect]->Update(); }

		// �g�[�^���X�R�A�̍X�V
		if (m_pTotalScore[nCntSelect] != NULL) { m_pTotalScore[nCntSelect]->Update(); }

		if (m_apHighScore[nCntSelect] != NULL) { m_apHighScore[nCntSelect]->Update(); }

		if (m_apMoney[nCntSelect] != NULL) { m_apMoney[nCntSelect]->Update(); }
	}

	for (int nCntLogo = 0; nCntLogo < MAX_SELECT_NUM; nCntLogo++)
	{	// ���S�̍X�V
		if (m_apSelectLevel[nCntLogo] != NULL)
		{
			m_apSelectLevel[nCntLogo]->Update();
		}
	}

	if (m_pBg != NULL) { m_pBg->Update(); }


	CDebugProc::Print("%d\n", m_nStageNumber);


}
//=============================================================================
// �`�揈��
//=============================================================================
void CSelect::Draw(void)
{
	for (int nCntSelect = 0; nCntSelect < MAX_SELECT_STAGE; nCntSelect++)
	{
		if (m_apMenu[nCntSelect] != NULL) { m_apMenu[nCntSelect]->Draw(); }
	}

	// �T�[�N���A�C�R���̕`��
	if (m_pCircle != NULL) { m_pCircle->Draw(); }

	// �t���[���̕`��
	if (m_pFream != NULL) { m_pFream->Draw(); }


	for (int nCntHuman = 0; nCntHuman < MAX_SELECT_STAGE; nCntHuman++)
	{
		if (m_pHuman[nCntHuman] != NULL)
		{
			m_pHuman[nCntHuman]->Draw();
		}
	}

	if (m_bSelectFlag == true)
	{
		if (m_pBg != NULL) { m_pBg->Draw(); }
		for (int nCntHuman = 0; nCntHuman < MAX_SELECT_STAGE; nCntHuman++)
		{
			// �����x�̕`��
			if (m_pSatisfaction[nCntHuman] != NULL) { m_pSatisfaction[nCntHuman]->Draw(); }

			// �g�[�^���X�R�A�̂̕`��
			if (m_pTotalScore[nCntHuman] != NULL) { m_pTotalScore[nCntHuman]->Draw(); }

			if (m_apHighScore[nCntHuman] != NULL) { m_apHighScore[nCntHuman]->Draw(); }

			if (m_apMoney[nCntHuman] != NULL) { m_apMoney[nCntHuman]->Draw(); }
		}

	}
}

//=============================================================================
// �X�e�[�W�I��
//=============================================================================
void CSelect::SlectStage(void)
{
	//�T�E���h�̏��
	CSound *pSound = CManager::GetSound();

	if (CManager::GetInput()->GetKeyboardTrigger(DIK_A) || CManager::GetInput()->GetKeyboardTrigger(DIK_LEFT) ||
		CManager::GetInput()->GetKeyboardTrigger(DIK_W) || CManager::GetXInput()->GetTrigger(CXInput::XIJS_BUTTON_18) || CManager::GetXInput()->GetTrigger(CXInput::XIJS_BUTTON_16))
	{	// ���������ꍇ ���Ɉړ�
		//SelectHuman();
		if (m_bSelectFlag == false)
		{
			if (m_apMenu[0] != NULL) { m_apMenu[0]->SetTexture(m_nStageNum, 1, 2, 1); }
			m_nStageNum = (m_nStageNum + 1) % MAX_SELECT_STAGE;
			m_bPress = true;
		}
		//	�I����
		pSound->PlaySound(CSound::SOUND_LABEL_SE_STAGESELECT);
	}
	else if (CManager::GetInput()->GetKeyboardTrigger(DIK_D) || CManager::GetInput()->GetKeyboardTrigger(DIK_RIGHT) ||
		CManager::GetInput()->GetKeyboardTrigger(DIK_S) || CManager::GetXInput()->GetTrigger(CXInput::XIJS_BUTTON_19) || CManager::GetXInput()->GetTrigger(CXInput::XIJS_BUTTON_17))
	{	// ���������ꍇ �E�Ɉړ�
		//SelectHuman();
		if (m_bSelectFlag == false)
		{
			if (m_apMenu[0] != NULL) { m_apMenu[0]->SetTexture(m_nStageNum, 1, 2, 1); }
			m_nStageNum = (m_nStageNum + (MAX_SELECT_STAGE - 1)) % MAX_SELECT_STAGE;
			m_bPress = true;
		}
		//	�I����
		pSound->PlaySound(CSound::SOUND_LABEL_SE_STAGESELECT);
	}

	// �t���[���̈ʒu�ݒ�
	if (m_pFream != NULL) { m_pFream->SetPosSize(m_apMenu[m_nStageNum]->GetPosition(), m_nSizeOld[m_nStageNum]); }

	// �T�[�N���A�C�R���̉�]����
	if (m_bPress == true && m_pCircle != NULL)
	{
		float fRot = (-D3DX_PI *m_nStageNum);			// �����̐ݒ�
		if (fRot == -D3DX_PI) { m_fAddRot -= ROT_MOVE; }
		else if (fRot == 0)  { m_fAddRot -= ROT_MOVE; }

		if (m_fAddRot <= -D3DX_PI && fRot == -D3DX_PI) { m_fAddRot = D3DX_PI; m_bPress = false; }
		if (m_fAddRot <= 0 && fRot == 0)		  { m_fAddRot = 0;		 m_bPress = false; }
	}
}

//=============================================================================
// �e�N�X�`���A�j���[�V����
//=============================================================================
void CSelect::TexAnim(void)
{
	if ((m_nCntAnim % ANIM_COUNT) == 0)
	{
		m_nCntPattern++;

		if (m_nCntPattern >= PATTERN_NUM) { m_nCntPattern = 0; }
		m_apMenu[1]->SetTexture(m_nCntPattern, PATTERN_NUM, 1, 1);
	}

	m_nCntAnim++;
}

//=============================================================================
// Text�̓ǂݍ���(�����L���O)
//=============================================================================
int CSelect::RankingLoad(int nType, int nSelectLevel)
{
	// �ǂݍ���
	FILE *pFile;
	int nTotalScore = 0;
	char *pFileName = {};

	// �X�R�A�̃����L���O�̓ǂݍ���
	if (nType == 0 && nSelectLevel == 0) { pFileName = TOTALSCORE_TXT_NAME; }
	else if (nType == 0 && nSelectLevel == 1) { pFileName = TOTALSCORE_TXT_NAME2; }
	else if (nType == 1 && nSelectLevel  == 0) { pFileName = SATISFACTION_TXT_NAME; }
	else if (nType == 1 && nSelectLevel == 1) { pFileName = SATISFACTION_TXT_NAME2; }

	pFile = fopen(pFileName, "rb");

	if (pFile != NULL)
	{
		fread(&nTotalScore, sizeof(int), 1, pFile);

		fclose(pFile);
	}

	return nTotalScore;
}

//=============================================================================
// �l�̑I������
//=============================================================================
void CSelect::SelectHuman(void)
{
	if (m_nStageNum == 0)
	{
		for (int nCntHuman = 0; nCntHuman < MAX_SELECT_STAGE; nCntHuman++)
		{
			if (m_pHuman[nCntHuman] != NULL)
			{
				if (nCntHuman == 0)
				{
					m_pHuman[nCntHuman]->SetPosSize(D3DXVECTOR3(80.0f, 610.0f, 0.0f), D3DXVECTOR2(150.0f, 250.0f));
					m_pHuman[nCntHuman]->BindTexture(*CTexture::GetTexture(CTexture::TYPE_SELECT_HUMAN002));
				}
				else
				{
					m_pHuman[nCntHuman]->SetPosSize(D3DXVECTOR3(1210.0f, 610.0f, 0.0f), D3DXVECTOR2(160.0f, 250.0f));
					m_pHuman[nCntHuman]->BindTexture(*CTexture::GetTexture(CTexture::TYPE_SELECT_HUMAN003));
				}
			}
		}
	}
	else if(m_nStageNum == 1)
	{
		for (int nCntHuman = 0; nCntHuman < MAX_SELECT_STAGE; nCntHuman++)
		{
			if (m_pHuman[nCntHuman] != NULL)
			{
				if (nCntHuman == 0)
				{
					m_pHuman[nCntHuman]->SetPosSize(D3DXVECTOR3(80.0f, 350.0f, 0.0f), D3DXVECTOR2(100.0f, 250.0f));
					m_pHuman[nCntHuman]->BindTexture(*CTexture::GetTexture(CTexture::TYPE_SELECT_HUMAN000));
				}
				else
				{
					m_pHuman[nCntHuman]->SetPosSize(D3DXVECTOR3(1190.0f, 350.0f, 0.0f), D3DXVECTOR2(110.0f, 250.0f));
					m_pHuman[nCntHuman]->BindTexture(*CTexture::GetTexture(CTexture::TYPE_SELECT_HUMAN001));
				}
			}
		}
	}
}

//=============================================================================
// �t�F�[�h�X�V����
//=============================================================================
void CSelect::Fade(void)
{
	//�t�F�[�h�̃|�C���g
	CFade::FADE pFade = CFade::GetFade();

	//�T�E���h�̏��
	CSound *pSound = CManager::GetSound();

	if (pFade == CFade::FADE_NONE)
	{
		m_bEnter = true;
		//�|�[�Y�̑I���̌��艹
		pSound->PlaySound(CSound::SOUND_LABEL_SE_SELECT_ENTER);
		if (m_nStageNum == 0 && m_bSelectFlag == false) { CFade::Create(CManager::MODE_TITLE); }
		else if (m_bSelectFlag == true) { CFade::Create(CManager::MODE_TUTORIAL); }
	}
}

//=============================================================================
// ��Փx�I���̏���
//=============================================================================
void CSelect::SelectLevel(void)
{
	D3DXVECTOR3 pos[MAX_SELECT_NUM];

	if (m_pBg == NULL)
	{
		m_pBg = new CBackGround(5);
		m_pBg->Init();
		m_pBg->BindTexture(*CTexture::GetTexture(CTexture::TYPE_PAUSE_BG));
		m_pBg->SetPosSize(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR2((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2)));
	}

	// ���j���[�̐���
	for (int nCntMenu = 0; nCntMenu < MAX_SELECT_NUM; nCntMenu++)
	{
		if (m_apSelectLevel[nCntMenu] == NULL)
		{
			if (nCntMenu == 0)
			{
				m_apSelectLevel[nCntMenu] = CLogo::Create(DEFAULT_POS_RIGHT, MENU_SIZE, CTexture::TYPE_SELECT_EASY + nCntMenu, 0, CLogo::TYPE_LOGO);
			}
			else if(nCntMenu == 1){ m_apSelectLevel[nCntMenu] = CLogo::Create(DEFAULT_POS_LEFT, MENU_SIZE, CTexture::TYPE_SELECT_EASY+ nCntMenu, 0, CLogo::TYPE_LOGO); }
			else { m_apSelectLevel[nCntMenu] = CLogo::Create(D3DXVECTOR3(1200.0f, 650.0f, 0.0f), D3DXVECTOR2(80.0f, 50.0f), CTexture::TYPE_SELECT_BACK, 0, CLogo::TYPE_LOGO); }

			if (nCntMenu == 0) { m_apSelectLevel[nCntMenu]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)); }
			else { m_apSelectLevel[nCntMenu]->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f)); }
		}
	}

	// ���j���̈ړ�
	for (int nCntMenu = 0; nCntMenu < MAX_SELECT_NUM; nCntMenu++)
	{
		if (m_apSelectLevel[nCntMenu] != NULL)
		{
			pos[nCntMenu] = m_apSelectLevel[nCntMenu]->GetPosition();
			if (nCntMenu == 0)
			{
				pos[nCntMenu] = m_apSelectLevel[nCntMenu]->MoveLimit(pos[nCntMenu], MENU_POS, 1, -MOVE);
			}
			else if(nCntMenu == 1){ pos[nCntMenu] = m_apSelectLevel[nCntMenu]->MoveLimit(pos[nCntMenu], MENU_POS, 0, MOVE); }
		}
	}


	m_bMove = m_apSelectLevel[0]->GetMoveFlag();
	if (m_bMove == true)
	{
		// �����x�̐���
		if (m_pSatisfaction[0] == NULL)
		{
			m_pSatisfaction[0] = CSatisfaction::Create(D3DXVECTOR3(SATYIFACTION_POS.x, 325.0f, 0.0f), SATYIFACTION_SIZE, RankingLoad(1, 0));
			m_pSatisfaction[0]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		}

		if (m_pSatisfaction[1] == NULL)
		{
			m_pSatisfaction[1] = CSatisfaction::Create(SATYIFACTION_POS, SATYIFACTION_SIZE, RankingLoad(1, 1));
			m_pSatisfaction[1]->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.0f, 1.0f));
		}

		// ���݂�1�ʂ̃X�R�A�Ɩ����x�̃��S
		if (m_apHighScore[0] == NULL)
		{
			m_apHighScore[0] = CLogo::Create(D3DXVECTOR3(TOTAL_POS.x, 265.0f, 0.0f), TOTAL_SIZE, CTexture::TYPE_SELECT_HIGH_SCORE, 0, CLogo::TYPE_LOGO);
			m_apHighScore[0]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}

		if (m_apHighScore[1] == NULL)
		{ 
			m_apHighScore[1] = CLogo::Create(TOTAL_POS, TOTAL_SIZE, CTexture::TYPE_SELECT_HIGH_SCORE, 0, CLogo::TYPE_LOGO);
			m_apHighScore[1]->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
		}

		if (m_apMoney[0] == NULL)
		{
			m_apMoney[0] = CLogo::Create(D3DXVECTOR3(MONEY_POS.x, 325.0f, 0.0f), MONEY_SIZE, CTexture::TYPE_MONEY_TEN, 0, CLogo::TYPE_LOGO);
			m_apMoney[0]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}

		if (m_apMoney[1] == NULL) 
		{ 
			m_apMoney[1] = CLogo::Create(MONEY_POS, MONEY_SIZE, CTexture::TYPE_MONEY_TEN, 0, CLogo::TYPE_LOGO); 
			m_apMoney[1]->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
		}

		// �g�[�^���X�R�A�̐���
		if (m_pTotalScore[0] == NULL)
		{
			m_pTotalScore[0] = CTotalScore::Create(D3DXVECTOR3(SCORE_POS.x , 325.0f, 0.0f));
			m_pTotalScore[0]->SetRankingScore(RankingLoad(0, 0));
			m_pTotalScore[0]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}

		if (m_pTotalScore[1] == NULL)
		{
			m_pTotalScore[1] = CTotalScore::Create(SCORE_POS);
			m_pTotalScore[1]->SetRankingScore(RankingLoad(0, 1));
			m_pTotalScore[1]->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
		}
	}

	for (int nCntLogo = 0; nCntLogo < MAX_SELECT_NUM; nCntLogo++)
	{	// ���S�̍X�V
		if (m_apSelectLevel[nCntLogo] != NULL)
		{
			m_apSelectLevel[nCntLogo]->SetPosition(pos[nCntLogo]);
		}
	}
}

//=============================================================================
// �X�e�[�W�I��
//=============================================================================
void CSelect::Select(void)
{
	//�T�E���h�̏��
	CSound *pSound = CManager::GetSound();

	if (CManager::GetInput()->GetKeyboardTrigger(DIK_A) || CManager::GetInput()->GetKeyboardTrigger(DIK_LEFT) ||
		CManager::GetInput()->GetKeyboardTrigger(DIK_W) || CManager::GetXInput()->GetTrigger(CXInput::XIJS_BUTTON_18) || CManager::GetXInput()->GetTrigger(CXInput::XIJS_BUTTON_16))
	{	// ���������ꍇ ���Ɉړ�

		if (m_apSelectLevel[m_nStageNumber] != NULL) { m_apSelectLevel[m_nStageNumber]->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f)); }
		if (m_nStageNumber == 0 || m_nStageNumber == 1)
		{
			if (m_pSatisfaction[m_nStageNumber] != NULL) { m_pSatisfaction[m_nStageNumber]->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.0f, 1.0f)); }
			if (m_apHighScore[m_nStageNumber] != NULL) { m_apHighScore[m_nStageNumber]->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f)); }
			if (m_apMoney[m_nStageNumber] != NULL) { m_apMoney[m_nStageNumber]->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f)); }
			if (m_pTotalScore[m_nStageNumber] != NULL) { m_pTotalScore[m_nStageNumber]->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f)); }
		}
		m_nStageNumber = (m_nStageNumber + (MAX_SELECT_NUM - 1)) % MAX_SELECT_NUM;

		if (m_apSelectLevel[m_nStageNumber] != NULL) { m_apSelectLevel[m_nStageNumber]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)); }
		if (m_nStageNumber == 0 || m_nStageNumber == 1)
		{
			if (m_pSatisfaction[m_nStageNumber] != NULL) { m_pSatisfaction[m_nStageNumber]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)); }
			if (m_apHighScore[m_nStageNumber] != NULL) { m_apHighScore[m_nStageNumber]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)); }
			if (m_apMoney[m_nStageNumber] != NULL) { m_apMoney[m_nStageNumber]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)); }
			if (m_pTotalScore[m_nStageNumber] != NULL) { m_pTotalScore[m_nStageNumber]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)); }
		}
		m_bPress = true;
		//	�I����
		pSound->PlaySound(CSound::SOUND_LABEL_SE_STAGESELECT);
	}
	else if (CManager::GetInput()->GetKeyboardTrigger(DIK_D) || CManager::GetInput()->GetKeyboardTrigger(DIK_RIGHT) ||
		CManager::GetInput()->GetKeyboardTrigger(DIK_S) || CManager::GetXInput()->GetTrigger(CXInput::XIJS_BUTTON_19) || CManager::GetXInput()->GetTrigger(CXInput::XIJS_BUTTON_17))
	{	// ���������ꍇ �E�Ɉړ�
		//SelectHuman();
		if (m_apSelectLevel[m_nStageNumber] != NULL) { m_apSelectLevel[m_nStageNumber]->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f)); }
		if (m_nStageNumber == 0 || m_nStageNumber == 1)
		{
			if (m_pSatisfaction[m_nStageNumber] != NULL) { m_pSatisfaction[m_nStageNumber]->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.0f, 1.0f)); }
			if (m_apHighScore[m_nStageNumber] != NULL) { m_apHighScore[m_nStageNumber]->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f)); }
			if (m_apMoney[m_nStageNumber] != NULL) { m_apMoney[m_nStageNumber]->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f)); }
			if (m_pTotalScore[m_nStageNumber] != NULL) { m_pTotalScore[m_nStageNumber]->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f)); }
		}

		m_nStageNumber = (m_nStageNumber + 1) % MAX_SELECT_NUM;

		if (m_apSelectLevel[m_nStageNumber] != NULL) { m_apSelectLevel[m_nStageNumber]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)); }

		if (m_nStageNumber == 0 || m_nStageNumber == 1)
		{
			if (m_pSatisfaction[m_nStageNumber] != NULL) { m_pSatisfaction[m_nStageNumber]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)); }
			if (m_apHighScore[m_nStageNumber] != NULL) { m_apHighScore[m_nStageNumber]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)); }
			if (m_apMoney[m_nStageNumber] != NULL) { m_apMoney[m_nStageNumber]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)); }
			if (m_pTotalScore[m_nStageNumber] != NULL) { m_pTotalScore[m_nStageNumber]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)); }
		}
		m_bPress = true;
		
		//	�I����
		pSound->PlaySound(CSound::SOUND_LABEL_SE_STAGESELECT);
	}
}


//=============================================================================
// �߂鎞�ɔj��
//=============================================================================
void CSelect::BackUninit(void)
{
	m_bSelectFlag = false;
	m_bMove = false;
	m_nStageNumber = 0;
	if (m_pBg != NULL)
	{
		m_pBg->Uninit();
		m_pBg = NULL;
	}

	for (int nCntLevel = 0; nCntLevel < MAX_SELECT_NUM; nCntLevel++)
	{
		if (m_apSelectLevel[nCntLevel] != NULL)
		{
			m_apSelectLevel[nCntLevel]->Uninit();
			m_apSelectLevel[nCntLevel] = NULL;
		}
	}

	for (int nCntHuman = 0; nCntHuman < MAX_SELECT_STAGE; nCntHuman++)
	{

		// �����x�̔j��
		if (m_pSatisfaction[nCntHuman] != NULL)
		{
			m_pSatisfaction[nCntHuman]->Uninit();
			m_pSatisfaction[nCntHuman] = NULL;
		}

		// �g�[�^���X�R�A�̔j��
		if (m_pTotalScore[nCntHuman] != NULL)
		{
			m_pTotalScore[nCntHuman]->Uninit();
			m_pTotalScore[nCntHuman] = NULL;
		}

		if (m_apHighScore[nCntHuman] != NULL)
		{
			m_apHighScore[nCntHuman]->Uninit();
			m_apHighScore[nCntHuman] = NULL;
		}

		if (m_apMoney[nCntHuman] != NULL)
		{
			m_apMoney[nCntHuman]->Uninit();
			m_apMoney[nCntHuman] = NULL;
		}
	}
}