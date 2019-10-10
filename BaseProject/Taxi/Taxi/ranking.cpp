//=============================================================================
//
// �����L���O�̏��� [ranking.cpp]
// Author : shun yokomichi
//
//=============================================================================
#include "ranking.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "background.h"
#include "texture.h"
#include "satisfactionlevel.h"
#include "logo.h"
#include "totalscore.h"
#include "select.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FAREM_COUNT					(60 * 10)													// �t�F�[�h�̃t���[����
#define RAKING_LOOG_POS				(D3DXVECTOR3(230.0f, 70.0f, 0.0f))							// �����L���O�̃��S(�ʒu)
#define RAKING_LOOG_SIZE			(D3DXVECTOR2(200.0f, 60.0f))								// �����L���O�̃��S(�T�C�Y)
#define RAKING_RANK_POS				(D3DXVECTOR3(80.0f + (nCntRank * 670.0f), 460.0f, 0.0f))	// ���ʂ̈ʒu
#define RAKING_RANK_SIZE			(D3DXVECTOR2(50.0f, 220.0f))								// ���ʂ̃T�C�Y
#define RAKING_CLASS_POS			(D3DXVECTOR3(310.0f, 200.0f, 0.0f))							// CLASS�̈ʒu
#define RAKING_CLASS_SIZE			(D3DXVECTOR2(130.0f, 30.0f))								// CLASS�̃T�C�Y
#define RAKING_TOTAL_POS			(D3DXVECTOR3(950.0f, RAKING_CLASS_POS.y, 0.0f))				// �g�[�^���X�R�A�̃��S(�ʒu)
#define RAKING_TOTAL_SIZE			(D3DXVECTOR2(RAKING_CLASS_SIZE.x, RAKING_CLASS_SIZE.y))		// �g�[�^���X�R�A�̃��S(�T�C�Y)
#define RAKING_NUMBER_POS			(D3DXVECTOR3(1180.0f, 270.0f + (nCntRnaking * 95.0f), 0.0f))// �X�R�A�̈ʒu
#define RAKING_NUMBER_SIZE			(D3DXVECTOR2(20.0f, 20.0f))									// �X�R�A�̃T�C�Y
#define RAKING_SATISFACTION_POS		(D3DXVECTOR3(180.0f, 270.0f + (nCntSatisfaction * 95.0f), 0.0f))// �����x�̈ʒu
#define RAKING_SATISFACTION_SIZE	(D3DXVECTOR2(20.0f, 30.0f))										// �����x�̃T�C�Y
#define RAKING_BG_POS				(D3DXVECTOR3(310.0f + (nCntBg * 650.0f), 450.0f, 0.0f))			// �������̔w�i(�ʒu)
#define RAKING_BG_SIZE				(D3DXVECTOR2(290.0f, 250.0f))									// �������̔w�i(�T�C�Y)
#define RANK_IN_COL					(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))								// �����N�C�����̐F

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
int				 CRanking::m_aTotalScore[MAX_LEVEL_NUM][MAX_RANKING] = { { 15000, 13000, 11000, 10000, 8000 } , { 18000, 15000, 13000, 10000, 8000 } };
int				 CRanking::m_aSatisfactionLevel[MAX_LEVEL_NUM][MAX_RANKING] = { { 17, 14, 10, 6, 5 } ,{ 15, 11, 8, 6, 5 } };
int				 CRanking::m_nNum = MAX_RANKING;
int				 CRanking::m_nSatisfaction = MAX_RANKING;
CTotalScore		 *CRanking::m_apTotalScore[MAX_RANKING] = {};
CSatisfaction	 *CRanking::m_pSatisfaction[MAX_RANKING] = {};
//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CRanking::CRanking()
{
	m_nCntFrame = 0;
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CRanking::~CRanking()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CRanking::Init()
{
	int nTotalScore = 0;
	int nSatisfaction = 0;
	int nSelectLevel = CSelect::GetSelectLevel();
	//===================================
	//		 Load�̓ǂݍ��ݏꏊ
	//===================================
	//�t�F�[�h�̃e�N�X�`���̓ǂݍ���
	CFade::Load();

	RankingLoad();

	CBackGround::Create(CTexture::TYPE_BACKGROUND001);

	for (int nCntBg = 0; nCntBg < 2; nCntBg++)
	{	// �������̔w�i
		CLogo::Create(RAKING_BG_POS, RAKING_BG_SIZE, CTexture::TYPE_BG_FREAM, 0, CLogo::TYPE_LOGO);
	}

	CLogo::Create(RAKING_LOOG_POS, RAKING_LOOG_SIZE, CTexture::TYPE_RANKING_LOGO000, 0, CLogo::TYPE_LOGO);	// ���S
	for (int nCntRank = 0; nCntRank < 2; nCntRank++)
	{
		CLogo::Create(RAKING_RANK_POS, RAKING_RANK_SIZE, CTexture::TYPE_RANKING_RANK, 0, CLogo::TYPE_LOGO);		// ����
	}
	CLogo::Create(RAKING_CLASS_POS, RAKING_CLASS_SIZE, CTexture::TYPE_RANKING_CLASS, 0, CLogo::TYPE_LOGO);	// �N���X
	CLogo::Create(RAKING_TOTAL_POS, RAKING_TOTAL_SIZE, CTexture::TYPE_RANKING_TOTALLOGO, 0, CLogo::TYPE_LOGO);	// �g�[�^���X�R�A

	// �g�[�^���X�R�A�̐���
	for (int nCntRnaking = 0; nCntRnaking < MAX_RANKING; nCntRnaking++)
	{
		nTotalScore = m_aTotalScore[nSelectLevel][nCntRnaking];
		m_apTotalScore[nCntRnaking] = CTotalScore::Create(RAKING_NUMBER_POS);
		m_apTotalScore[nCntRnaking]->SetRankingScore(nTotalScore);
		nTotalScore /= 10;
	}

	// �����x�̐���
	for (int nCntSatisfaction = 0; nCntSatisfaction < MAX_RANKING; nCntSatisfaction++)
	{
		nSatisfaction = m_aSatisfactionLevel[nSelectLevel][nCntSatisfaction];
		m_pSatisfaction[nCntSatisfaction] = CSatisfaction::Create(RAKING_SATISFACTION_POS, RAKING_SATISFACTION_SIZE, nSatisfaction);
	}

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CRanking::Uninit(void)
{
	//===================================
	//	�@�@UnLoad�̔j������ꏊ
	//===================================
	//�t�F�[�h�̃e�N�X�`���̔j��
	CFade::UnLoad();

	// �����x�̔j��
	for (int nCntSatisfaction = 0; nCntSatisfaction < MAX_RANKING; nCntSatisfaction++)
	{
		if (m_pSatisfaction[nCntSatisfaction] != NULL)
		{
			m_pSatisfaction[nCntSatisfaction]->Uninit();
			m_pSatisfaction[nCntSatisfaction] = NULL;
		}
	}

	// �g�[�^���X�R�A�̂̔j��
	for (int nCntRnaking = 0; nCntRnaking < MAX_RANKING; nCntRnaking++)
	{
		if (m_apTotalScore[nCntRnaking] != NULL)
		{
			m_apTotalScore[nCntRnaking]->Uninit();
			m_apTotalScore[nCntRnaking] = NULL;
		}
	}
	//�t�F�[�h�ȊO�폜
	CScene::NotFadeReleseAll();
}
//=============================================================================
// �X�V����
//=============================================================================
void CRanking::Update(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CXInput *pCInputJoypad = CManager::GetXInput();
	CInputMouse *pCInputMouse = CManager::GetInputMouse();

	//�T�E���h�̏��
	CSound *pSound = CManager::GetSound();

	//�t�F�[�h�̃|�C���g
	CFade::FADE pFade = CFade::GetFade();

	if (pCInputJoypad->GetAnyButton()
		|| pCInputKeyBoard->GetKeyboardAny(1) == true)
	{//�����L���O����^�C�g����
	 //�t�F�[�h���n�܂�����
		if (pFade == CFade::FADE_NONE)
		{
			//�|�[�Y�̑I���̌��艹
			pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_SELECT);
			CFade::Create(CManager::MODE_TITLE);
		}
	}

	if (m_nCntFrame == FAREM_COUNT) { CFade::Create(CManager::MODE_TITLE);}

	m_nCntFrame++;

	if (m_nSatisfaction < MAX_RANKING)
	{
		if (m_pSatisfaction[m_nSatisfaction] != NULL) { m_pSatisfaction[m_nSatisfaction]->ChangeCol(RANK_IN_COL); }
	}
	if (m_nNum >= MAX_RANKING) { return; }	// �����L���O�̍ő吔�ȏ�̏ꍇ �X�V�����̏I��
	if (m_apTotalScore[m_nNum] != NULL) { m_apTotalScore[m_nNum]->ChangeCol(RANK_IN_COL); }

}
//=============================================================================
// �`�揈��
//=============================================================================
void CRanking::Draw(void)
{
	for (int nCntSatisfaction = 0; nCntSatisfaction < MAX_RANKING; nCntSatisfaction++)
	{	// �����x��NULL�`�F�b�N���ĕ`��
		if (m_pSatisfaction[nCntSatisfaction] != NULL)
		{
			m_pSatisfaction[nCntSatisfaction]->Draw();
		}
	}

	for (int nCntRnaking = 0; nCntRnaking < MAX_RANKING; nCntRnaking++)
	{	// �X�R�A��NULL�`�F�b�N���ĕ`��
		if (m_apTotalScore[nCntRnaking] != NULL)
		{
			m_apTotalScore[nCntRnaking]->Draw();
		}
	}
}

//=============================================================================
// �����L���O�̏�������
//=============================================================================
void CRanking::RankingSave(int nTotalScore, int nLvevl)
{
	CTotalScore *pTotalScore = NULL;		// �|�C���^����

	int nData;		// �f�[�^����ւ��p
	int nScore = nTotalScore;		// ���݂̃X�R�A�ۊ�
	int nSatisfaction = nLvevl;
	int nSelectLevel = CSelect::GetSelectLevel();
	m_nNum = MAX_RANKING;
	m_nSatisfaction = MAX_RANKING;

	// �����N�̓���ւ�
	for (int nCount = 0; nCount < MAX_RANKING; nCount++)
	{
		if (m_aTotalScore[nSelectLevel][MAX_RANKING - 1] <= nScore)
		{ // �����N�C������Ȃ�
			m_nNum -= 1;
			m_aTotalScore[nSelectLevel][MAX_RANKING - 1] = nScore;
		}

		for (int nCount001 = 0; nCount001 < MAX_RANKING - 1; nCount001++)
		{
			for (int nCount002 = nCount001 + 1; nCount002 < MAX_RANKING; nCount002++)
			{
				if (m_aTotalScore[nSelectLevel][nCount002] >= m_aTotalScore[nSelectLevel][nCount001])
				{	// ���ʂ̓���ւ�
					nData = m_aTotalScore[nSelectLevel][nCount001];
					m_aTotalScore[nSelectLevel][nCount001] = m_aTotalScore[nSelectLevel][nCount002];
					m_aTotalScore[nSelectLevel][nCount002] = nData;
					m_nNum--;
				}
			}
		}
		break;
	}

	// �����x�̓���ւ�
	for (int nCount = 0; nCount < MAX_RANKING; nCount++)
	{
		if (m_aSatisfactionLevel[nSelectLevel][MAX_RANKING - 1] <= nSatisfaction)
		{ // �����N�C������Ȃ�
			m_nSatisfaction -= 1;
			m_aSatisfactionLevel[nSelectLevel][MAX_RANKING - 1] = nSatisfaction;
		}

		for (int nCount001 = 0; nCount001 < MAX_RANKING - 1; nCount001++)
		{
			for (int nCount002 = nCount001 + 1; nCount002 < MAX_RANKING; nCount002++)
			{
				if (m_aSatisfactionLevel[nSelectLevel][nCount002] >= m_aSatisfactionLevel[nSelectLevel][nCount001])
				{	// ���ʂ̓���ւ�
					nData = m_aSatisfactionLevel[nSelectLevel][nCount001];
					m_aSatisfactionLevel[nSelectLevel][nCount001] = m_aSatisfactionLevel[nSelectLevel][nCount002];
					m_aSatisfactionLevel[nSelectLevel][nCount002] = nData;
					m_nSatisfaction--;
				}
			}
		}
		break;
	}


	// �t�@�C���̏�������
	FILE *pFile = NULL;

	// �����L���O�̏��ʂ̏�������
	if (nSelectLevel == 0)
	{
		pFile = fopen("data/TEXT/Ranking/ranking.bin", "wb");
	}
	else if (nSelectLevel == 1)
	{
		pFile = fopen("data/TEXT/Ranking/ranking001.bin", "wb");
	}

	if (pFile != NULL)
	{
		fwrite(m_aTotalScore[nSelectLevel], sizeof(int), MAX_RANKING, pFile);

		fclose(pFile);
	}

	// �����x�̏��ʂ���������
	if (nSelectLevel == 0)
	{
		pFile = fopen("data/TEXT/Ranking/SatisfactionLevel.bin", "wb");
	}
	else 	if (nSelectLevel == 1)
	{
		pFile = fopen("data/TEXT/Ranking/SatisfactionLevel001.bin", "wb");
	}

	if (pFile != NULL)
	{
		fwrite(m_aSatisfactionLevel[nSelectLevel], sizeof(int), MAX_RANKING, pFile);

		fclose(pFile);
	}
}

//=============================================================================
// �����L���O�̓ǂݍ���
//=============================================================================
void CRanking::RankingLoad(void)
{
	// �ǂݍ���
	FILE *pFile = NULL;
	int nSelectLevel = CSelect::GetSelectLevel();

	// �X�R�A�̃����L���O�̓ǂݍ���
	if (nSelectLevel == 0)
	{
		pFile = fopen("data/TEXT/Ranking/ranking.bin", "rb");
	}
	else if (nSelectLevel == 1)
	{
		pFile = fopen("data/TEXT/Ranking/ranking001.bin", "rb");
	}

	if (pFile != NULL)
	{
		fread(m_aTotalScore[nSelectLevel], sizeof(int), MAX_RANKING, pFile);

		fclose(pFile);
	}

	// �����x�̃����L���O�̓ǂݍ���
	if (nSelectLevel == 0)
	{
		pFile = fopen("data/TEXT/Ranking/SatisfactionLevel.bin", "wb");
	}
	else 	if (nSelectLevel == 1)
	{
		pFile = fopen("data/TEXT/Ranking/SatisfactionLevel001.bin", "wb");
	}
	if (pFile != NULL)
	{
		fwrite(m_aSatisfactionLevel[nSelectLevel], sizeof(int), MAX_RANKING, pFile);

		fclose(pFile);
	}
}