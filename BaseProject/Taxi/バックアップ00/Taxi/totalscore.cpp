//=============================================================================
//
// �g�[�^���X�R�A���� [totalscore.cpp]
// Author : shun yokomichi
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "totalscore.h"
#include "number.h"
#include "fade.h"
#include "logo.h"
#include "tutorial.h"
#include "score.h"
#include "result.h"
#include "addcoin.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define CENTER_WIDTH				(SCREEN_WIDTH / 2)	// X�̒��S���W
#define TIMER_SPACE					(30.0f)				// �����Ɛ����̊Ԃ̃T�C�Y
#define TIMER_POSITION_Y			(70.0f)				// �^�C�}�[��Y���W
#define POWER_X						(10)				// �ׂ���((X * X) Y)���鐔�l

//=======================================
// ���U���g���g�p
//=======================================
#define RESULT_TIMER_TIMER_SPACE	(45.0f)				// ���U���g���̐����Ɛ����̊Ԋu
#define RESULT_ROULETTE_COUNT		(10)				// ���[���b�g�̃J�E���g�ؑ�
#define RESULT_ROULETTE_CAHNGE		(50)				// ���[���b�g����؂�ւ��t���[����
#define RESULT_SIZE					(D3DXVECTOR2(30.0f, 50.0f))	// �T�C�Y�ݒ�

//=======================================
// �Q�[�����g�p
//=======================================
#define GAME_ROULETTE_COUNT			(20)				// ���[���b�g�̃J�E���g�ؑ�
#define GAME_ROULETTE_CAHNGE		(30)				// ���[���b�g����؂�ւ��t���[����
#define GAME_SIZE					(D3DXVECTOR2(15.0f, 13.0f))	// �T�C�Y�̐ݒ�

//=======================================
// �����L���O���g�p
//=======================================
#define RANKING_TIMER_TIMER_SPACE	(60.0f)				// ���U���g���̐����Ɛ����̊Ԋu
#define COL_CHANGE_TIME				(5)					// �F�̕ω��t���[����
#define RANKING_SIZE				(D3DXVECTOR2(30.0f, 45.0f))	// �T�C�Y�ݒ�

//=======================================
// �Z���N�g���g�p
//=======================================
#define SELECT_TIMER_TIMER_SPACE	(45.0f)				// �Z���N�g���̐����Ɛ����̊Ԋu
#define SELECT_SIZE					(D3DXVECTOR2(30.0f, 30.0f))	//�@�T�C�Y�̐ݒ�
//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================
int					CTotalScore::m_nTotalScore	= 0;
int					CTotalScore::m_nTotalNum	= 1;
CTotalScore::STATE	CTotalScore::m_state		= STATE_NONE;

//=============================================================================
// ��������
//=============================================================================
CTotalScore *CTotalScore::Create(D3DXVECTOR3 pos)
{
	CTotalScore *pTotalScore = NULL;

	//NULL�`�F�b�N
	if (pTotalScore == NULL)
	{//���I�m��
		pTotalScore = new CTotalScore;

		//NULL�`�F�b�N
		if (pTotalScore != NULL)
		{
			//�ʒu�̐ݒ�
			pTotalScore->m_pos = pos;
			//����������
			pTotalScore->Init();
		}
	}

	return pTotalScore;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTotalScore::CTotalScore(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_nCntRoulette		= 0;
	m_nCntNum			= 0;
	m_nCntAdd			= 0;
	m_nRankingScore		= 0;
	m_nRankingTotalNum	= 1;
	m_nCntColTime		= 0;
	m_col				= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_bChangeCol		= false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTotalScore::~CTotalScore()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTotalScore::Init(void)
{
	//�ϐ��̏�����
	int nType = 0;
	m_nTotalScore = 0;
	m_nTotalNum = 1;
	m_type = TYPE::TYPE_USE;
	m_bPlaySound = false;
	m_bPlaySoundResult = false;
	m_nCountTime = 0;

	//��ʂ̃��[�h�擾
	CManager::MODE mode = CManager::GetMode();
	if ((mode == CManager::MODE_GAME) || (mode == CManager::MODE_TUTORIAL)) { nType = 1; }


	for (int nCntScore = 0; nCntScore < TOTALSCORE_MAX; nCntScore++)
	{
		m_apNumber[nCntScore] = new CNumber;

		if ((mode == CManager::MODE_GAME) || (mode == CManager::MODE_TUTORIAL))
		{	// �Q�[���p
			m_apNumber[nCntScore]->Init(D3DXVECTOR3((m_pos.x - TIMER_SPACE * nCntScore), m_pos.y, m_pos.z), nType);
			m_apNumber[nCntScore]->SetSize(GAME_SIZE, D3DXVECTOR2((m_pos.x - TIMER_SPACE * nCntScore), m_pos.y));
			m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		}
		else if (mode == CManager::MODE_RESULT)
		{	// ���U���g�p
			m_apNumber[nCntScore]->Init(D3DXVECTOR3((m_pos.x - RESULT_TIMER_TIMER_SPACE * nCntScore), m_pos.y, m_pos.z), nType);
			m_apNumber[nCntScore]->SetSize(RESULT_SIZE, D3DXVECTOR2((m_pos.x - RESULT_TIMER_TIMER_SPACE * nCntScore), m_pos.y));
		}
		else if (mode == CManager::MODE_RANKING)
		{	// �����L���O�p
			m_apNumber[nCntScore]->Init(D3DXVECTOR3((m_pos.x - RANKING_TIMER_TIMER_SPACE * nCntScore), m_pos.y, m_pos.z), nType);
			m_apNumber[nCntScore]->SetSize(RANKING_SIZE, D3DXVECTOR2((m_pos.x - RANKING_TIMER_TIMER_SPACE * nCntScore), m_pos.y));
		}
		else if (mode == CManager::MODE_SELECT)
		{	// �Z���N�g�p
			m_apNumber[nCntScore]->Init(D3DXVECTOR3((m_pos.x - SELECT_TIMER_TIMER_SPACE * nCntScore), m_pos.y, m_pos.z), nType);
			m_apNumber[nCntScore]->SetSize(SELECT_SIZE, D3DXVECTOR2((m_pos.x - SELECT_TIMER_TIMER_SPACE * nCntScore), m_pos.y));
		}
		m_apNumber[nCntScore]->SetCol(m_col);
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTotalScore::Uninit(void)
{
	for (int nCntScore = 0; nCntScore < TOTALSCORE_MAX; nCntScore++)
	{
		if (m_apNumber[nCntScore] != NULL)
		{
			m_apNumber[nCntScore]->Uninit();
			m_apNumber[nCntScore] = NULL;
		}
	}

	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTotalScore::Update(void)
{
	//���݂̃��[�h�̎擾
	CManager::MODE GameMode = CManager::GetMode();
	int nTexData = 0;
	int nNumData = 0;

	if (GameMode == CManager::MODE_GAME || GameMode == CManager::MODE_RESULT || GameMode == CManager::MODE_TUTORIAL)
	{
		nNumData = m_nTotalNum;
		nTexData = m_nTotalScore;

		if (m_state == STATE_ROULETTE)
		{
			//���̍Đ�
			if (!m_bPlaySound)
			{
				CManager::GetSound()->SetVolume(CSound::SOUND_LABEL_SE_ADDMONEY, 1.5f);
				CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ADDMONEY);	//�Z���̍Đ�
				m_bPlaySound = true;	//�����Đ�������Ԃɂ���
			}

			if (m_nCntRoulette >= RESULT_ROULETTE_CAHNGE && GameMode == CManager::MODE_RESULT ||
				m_nCntRoulette >= GAME_ROULETTE_CAHNGE && GameMode == CManager::MODE_GAME ||
				m_nCntRoulette >= GAME_ROULETTE_CAHNGE && GameMode == CManager::MODE_TUTORIAL)
			{
				nNumData = NumberRoulette(nTexData, GameMode, m_nTotalNum);
			}
			else
			{
				for (int nCntScore = 0; nCntScore < nNumData; nCntScore++)
				{	// �e�N�X�`���A�j���[�V����
					m_apNumber[nCntScore]->SetNumber(m_nCntAdd);
				}

			}
			m_nCntRoulette++;
			m_nCntAdd++;

			//�R�C���̐���
			if (GameMode == CManager::MODE_GAME || GameMode == CManager::MODE_TUTORIAL)
			{
				m_nCountTime++;

				if((m_nCountTime % 6) == 0)
				{
					for (int nCnt = 0; nCnt < 2; nCnt++)
					{
						CAddCoin::Create();
					}
				}
			}
		}
		else if (m_state == STATE_DISSCORE)
		{
			for (int nCntScore = 0; nCntScore < nNumData; nCntScore++)
			{
				m_apNumber[nCntScore]->SetNumber(nTexData);
				nTexData /= 10;
			}

			//���̍Đ����~�߂�
			CSound * pSound = CManager::GetSound(); 
			pSound->StopSound(CSound::SOUND_LABEL_SE_ADDMONEY);
			m_bPlaySound = false;	//�����Đ����Ă��Ȃ���Ԃɂ���

			//���[�h�����U���g��ʂ�������
			if (GameMode == CManager::MODE_RESULT)
			{
				int FirstScore = CResult::GetFirstScore();

				if (m_nTotalScore >= FirstScore)
				{//��ʂ̃X�R�A�����傫�������犽��
					if(!m_bPlaySoundResult)
					{
						pSound->SetVolume(CSound::SOUND_LABEL_SE_BIGCHEER, 2.0f);
						pSound->PlaySoundA(CSound::SOUND_LABEL_SE_BIGCHEER);
						m_bPlaySoundResult = true;
					}
				}
				else
				{//�����߂̊���
					if (!m_bPlaySoundResult)
					{
						pSound->SetVolume(CSound::SOUND_LABEL_SE_SMALLCHEER, 2.0f);
						pSound->PlaySoundA(CSound::SOUND_LABEL_SE_SMALLCHEER);
						m_bPlaySoundResult = true;
					}
				}
			}
		}
	}
	else if (GameMode == CManager::MODE_RANKING || GameMode == CManager::MODE_SELECT)
	{	// �����L���O �� �Z���N�g�p
		nTexData = m_nRankingScore;
		nNumData = NumberRoulette(nTexData, GameMode, m_nRankingTotalNum);
	}

	for (int nCntScore = 0; nCntScore < nNumData; nCntScore++)
	{	// �F�̐ݒ�
		m_apNumber[nCntScore]->SetCol(m_col);
	}

	SetType();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTotalScore::Draw(void)
{
	int nNumData = 0;
	//���݂̃��[�h�̎擾
	CManager::MODE GameMode = CManager::GetMode();

	if(CManager::MODE_GAME == GameMode || CManager::MODE_RESULT == GameMode || CManager::MODE_TUTORIAL == GameMode)
	{
		nNumData = m_nTotalNum;
	}
	else if (CManager::MODE_RANKING == GameMode || CManager::MODE_SELECT == GameMode) 
	{ 
		nNumData = m_nRankingTotalNum; 
	}
	for (int nCntScore = 0; nCntScore < nNumData; nCntScore++)
	{
		if (m_apNumber[nCntScore] != NULL)
		{
			m_apNumber[nCntScore]->Draw();
		}
	}
}

//=============================================================================
// �g�[�^���X�R�A�ւ̉��Z
//=============================================================================
void CTotalScore::AddTotalScore(int nTotalScore)
{
	m_nTotalScore += nTotalScore;
	m_state = STATE_ROULETTE;
	int nNum = (int)std::pow(POWER_X, m_nTotalNum);

	while ((m_nTotalScore / nNum) != 0)
	{
		m_nTotalNum++;
		nNum = (int)std::pow(POWER_X, m_nTotalNum);		// �ׂ��悷��(����O���Ȃ�)
		if (m_nTotalNum > TOTALSCORE_MAX - 1) { m_nTotalNum = TOTALSCORE_MAX - 1; }
	}
}

//=============================================================================
// �g�[�^���X�R�A�̐ݒ�(�����L���O�p)
//=============================================================================
void CTotalScore::SetRankingScore(int nTotalScore)
{
	m_nRankingScore += nTotalScore;
	RouletteNone();
	int nNum = (int)std::pow(POWER_X, m_nRankingTotalNum);
	while ((m_nRankingScore / nNum) != 0)
	{
		m_nRankingTotalNum++;
		nNum = (int)std::pow(POWER_X, m_nRankingTotalNum);		// �ׂ��悷��(����O���Ȃ�)
		if (m_nRankingTotalNum > TOTALSCORE_MAX - 1) { m_nRankingTotalNum = TOTALSCORE_MAX - 1; }
	}
}

//=============================================================================
// ���[���b�g�����Ȃ��Ńg�[�^���X�R�A���o��
//=============================================================================
void CTotalScore::RouletteNone(void)
{
	m_nCntRoulette = RESULT_ROULETTE_CAHNGE + 1;
	m_nCntNum = m_nTotalNum - 1;
	m_nCntNum = m_nRankingTotalNum - 1;
	m_state = STATE_DISSCORE;
}
//=============================================================================
// ���[���b�g�̂悤�ɉ񂷏���
//=============================================================================
int CTotalScore::NumberRoulette(int nTexData, CManager::MODE mode, int nNumData)
{
	if (CManager::MODE_GAME == mode || CManager::MODE_TUTORIAL == mode)
	{
		if (m_nCntRoulette == GAME_ROULETTE_CAHNGE) { m_nCntNum = 0; }
		if ((m_nCntRoulette % GAME_ROULETTE_COUNT) == 0) { m_nCntNum++; }
	}
	else if (CManager::MODE_RESULT == mode)
	{
		if (m_nCntRoulette == RESULT_ROULETTE_COUNT) { m_nCntNum = 0; }
		if ((m_nCntRoulette % RESULT_ROULETTE_CAHNGE) == 0) { m_nCntNum++; }
	}

	if (m_nCntNum > nNumData - 1) { m_nCntNum = nNumData - 1; m_state = STATE_DISSCORE; }

	for (int nCntScore = 0; nCntScore < nNumData; nCntScore++)
	{// �e�N�X�`���ɔ��f
		if (CManager::MODE_RANKING == mode || CManager::MODE_SELECT == mode)
		{
			m_apNumber[nCntScore]->SetNumber(nTexData);
			nTexData /= 10;
		}
		else
		{
			if (nCntScore <= m_nCntNum)
			{
				m_apNumber[nCntScore]->SetNumber(nTexData);
				nTexData /= 10;
			}
			else { m_apNumber[nCntScore]->SetNumber(m_nCntAdd); }
		}
	}

	// �g�[�^���X�R�A�̕\����Ԃ̏ꍇ �J�E���^�[�̃��Z�b�g
	if (m_state == STATE_DISSCORE) { m_nCntAdd = 0; m_nCntRoulette = 0; }

	return nNumData;
}

//=============================================================================
// �F�̓_��
//=============================================================================
void CTotalScore::ChangeCol(D3DXCOLOR col)
{
	if ((m_nCntColTime % COL_CHANGE_TIME) == 0)
	{
		if (m_bChangeCol == false) { m_col = col;  m_bChangeCol = true; }
		else if (m_bChangeCol == true) { m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); m_bChangeCol = false; }
		for (int nCntScore = 0; nCntScore < TOTALSCORE_MAX; nCntScore++)
		{
			m_apNumber[nCntScore]->SetCol(m_col);
		}

	}
	m_nCntColTime++;
}

//=============================================================================
// �^�C�v�̐ݒ�
//=============================================================================
void CTotalScore::SetType()
{
	CManager::MODE mode = CManager::GetMode();
	
	if (CScore::GetType() == CScore::TYPE_NOT_USE)
	{
		if (mode == CManager::MODE_GAME)
		{
			CGame::GetScoreUI(0)->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			CGame::GetScoreUI(2)->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			CGame::GetScoreUI(4)->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else if (mode == CManager::MODE_TUTORIAL)
		{
			CTutorial::GetScoreUI(0)->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			CTutorial::GetScoreUI(2)->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			CTutorial::GetScoreUI(4)->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
	else if (CScore::GetType() == CScore::TYPE_USE)
	{
		if (mode == CManager::MODE_GAME)
		{
			CGame::GetScoreUI(0)->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
			CGame::GetScoreUI(2)->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
			CGame::GetScoreUI(4)->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
		}
		else if (mode == CManager::MODE_TUTORIAL)
		{
			CTutorial::GetScoreUI(0)->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
			CTutorial::GetScoreUI(2)->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
			CTutorial::GetScoreUI(4)->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
		}
	}
}