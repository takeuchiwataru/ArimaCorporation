//=============================================================================
//
// トータルスコア処理 [totalscore.cpp]
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
// マクロ定義
//=============================================================================
#define CENTER_WIDTH				(SCREEN_WIDTH / 2)	// Xの中心座標
#define TIMER_SPACE					(30.0f)				// 数字と数字の間のサイズ
#define TIMER_POSITION_Y			(70.0f)				// タイマーのY座標
#define POWER_X						(10)				// べき乗((X * X) Y)する数値

//=======================================
// リザルト時使用
//=======================================
#define RESULT_TIMER_TIMER_SPACE	(45.0f)				// リザルト時の数字と数字の間隔
#define RESULT_ROULETTE_COUNT		(10)				// ルーレットのカウント切替
#define RESULT_ROULETTE_CAHNGE		(50)				// ルーレットから切り替わるフレーム数
#define RESULT_SIZE					(D3DXVECTOR2(30.0f, 50.0f))	// サイズ設定

//=======================================
// ゲーム時使用
//=======================================
#define GAME_ROULETTE_COUNT			(20)				// ルーレットのカウント切替
#define GAME_ROULETTE_CAHNGE		(30)				// ルーレットから切り替わるフレーム数
#define GAME_SIZE					(D3DXVECTOR2(15.0f, 13.0f))	// サイズの設定

//=======================================
// ランキング時使用
//=======================================
#define RANKING_TIMER_TIMER_SPACE	(60.0f)				// リザルト時の数字と数字の間隔
#define COL_CHANGE_TIME				(5)					// 色の変化フレーム数
#define RANKING_SIZE				(D3DXVECTOR2(30.0f, 45.0f))	// サイズ設定

//=======================================
// セレクト時使用
//=======================================
#define SELECT_TIMER_TIMER_SPACE	(45.0f)				// セレクト時の数字と数字の間隔
#define SELECT_SIZE					(D3DXVECTOR2(30.0f, 30.0f))	//　サイズの設定
//=============================================================================
//	静的メンバ変数
//=============================================================================
int					CTotalScore::m_nTotalScore	= 0;
int					CTotalScore::m_nTotalNum	= 1;
CTotalScore::STATE	CTotalScore::m_state		= STATE_NONE;

//=============================================================================
// 生成処理
//=============================================================================
CTotalScore *CTotalScore::Create(D3DXVECTOR3 pos)
{
	CTotalScore *pTotalScore = NULL;

	//NULLチェック
	if (pTotalScore == NULL)
	{//動的確保
		pTotalScore = new CTotalScore;

		//NULLチェック
		if (pTotalScore != NULL)
		{
			//位置の設定
			pTotalScore->m_pos = pos;
			//初期化処理
			pTotalScore->Init();
		}
	}

	return pTotalScore;
}

//=============================================================================
// コンストラクタ
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
// デストラクタ
//=============================================================================
CTotalScore::~CTotalScore()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTotalScore::Init(void)
{
	//変数の初期化
	int nType = 0;
	m_nTotalScore = 0;
	m_nTotalNum = 1;
	m_type = TYPE::TYPE_USE;
	m_bPlaySound = false;
	m_bPlaySoundResult = false;
	m_nCountTime = 0;

	//画面のモード取得
	CManager::MODE mode = CManager::GetMode();
	if ((mode == CManager::MODE_GAME) || (mode == CManager::MODE_TUTORIAL)) { nType = 1; }


	for (int nCntScore = 0; nCntScore < TOTALSCORE_MAX; nCntScore++)
	{
		m_apNumber[nCntScore] = new CNumber;

		if ((mode == CManager::MODE_GAME) || (mode == CManager::MODE_TUTORIAL))
		{	// ゲーム用
			m_apNumber[nCntScore]->Init(D3DXVECTOR3((m_pos.x - TIMER_SPACE * nCntScore), m_pos.y, m_pos.z), nType);
			m_apNumber[nCntScore]->SetSize(GAME_SIZE, D3DXVECTOR2((m_pos.x - TIMER_SPACE * nCntScore), m_pos.y));
			m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		}
		else if (mode == CManager::MODE_RESULT)
		{	// リザルト用
			m_apNumber[nCntScore]->Init(D3DXVECTOR3((m_pos.x - RESULT_TIMER_TIMER_SPACE * nCntScore), m_pos.y, m_pos.z), nType);
			m_apNumber[nCntScore]->SetSize(RESULT_SIZE, D3DXVECTOR2((m_pos.x - RESULT_TIMER_TIMER_SPACE * nCntScore), m_pos.y));
		}
		else if (mode == CManager::MODE_RANKING)
		{	// ランキング用
			m_apNumber[nCntScore]->Init(D3DXVECTOR3((m_pos.x - RANKING_TIMER_TIMER_SPACE * nCntScore), m_pos.y, m_pos.z), nType);
			m_apNumber[nCntScore]->SetSize(RANKING_SIZE, D3DXVECTOR2((m_pos.x - RANKING_TIMER_TIMER_SPACE * nCntScore), m_pos.y));
		}
		else if (mode == CManager::MODE_SELECT)
		{	// セレクト用
			m_apNumber[nCntScore]->Init(D3DXVECTOR3((m_pos.x - SELECT_TIMER_TIMER_SPACE * nCntScore), m_pos.y, m_pos.z), nType);
			m_apNumber[nCntScore]->SetSize(SELECT_SIZE, D3DXVECTOR2((m_pos.x - SELECT_TIMER_TIMER_SPACE * nCntScore), m_pos.y));
		}
		m_apNumber[nCntScore]->SetCol(m_col);
	}
	return S_OK;
}

//=============================================================================
// 終了処理
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
// 更新処理
//=============================================================================
void CTotalScore::Update(void)
{
	//現在のモードの取得
	CManager::MODE GameMode = CManager::GetMode();
	int nTexData = 0;
	int nNumData = 0;

	if (GameMode == CManager::MODE_GAME || GameMode == CManager::MODE_RESULT || GameMode == CManager::MODE_TUTORIAL)
	{
		nNumData = m_nTotalNum;
		nTexData = m_nTotalScore;

		if (m_state == STATE_ROULETTE)
		{
			//音の再生
			if (!m_bPlaySound)
			{
				CManager::GetSound()->SetVolume(CSound::SOUND_LABEL_SE_ADDMONEY, 1.5f);
				CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ADDMONEY);	//可算音の再生
				m_bPlaySound = true;	//音を再生した状態にする
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
				{	// テクスチャアニメーション
					m_apNumber[nCntScore]->SetNumber(m_nCntAdd);
				}

			}
			m_nCntRoulette++;
			m_nCntAdd++;

			//コインの生成
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

			//音の再生を止める
			CSound * pSound = CManager::GetSound(); 
			pSound->StopSound(CSound::SOUND_LABEL_SE_ADDMONEY);
			m_bPlaySound = false;	//音を再生していない状態にする

			//モードがリザルト画面だったら
			if (GameMode == CManager::MODE_RESULT)
			{
				int FirstScore = CResult::GetFirstScore();

				if (m_nTotalScore >= FirstScore)
				{//一位のスコアよりも大きかったら歓声
					if(!m_bPlaySoundResult)
					{
						pSound->SetVolume(CSound::SOUND_LABEL_SE_BIGCHEER, 2.0f);
						pSound->PlaySoundA(CSound::SOUND_LABEL_SE_BIGCHEER);
						m_bPlaySoundResult = true;
					}
				}
				else
				{//小さめの歓声
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
	{	// ランキング と セレクト用
		nTexData = m_nRankingScore;
		nNumData = NumberRoulette(nTexData, GameMode, m_nRankingTotalNum);
	}

	for (int nCntScore = 0; nCntScore < nNumData; nCntScore++)
	{	// 色の設定
		m_apNumber[nCntScore]->SetCol(m_col);
	}

	SetType();
}

//=============================================================================
// 描画処理
//=============================================================================
void CTotalScore::Draw(void)
{
	int nNumData = 0;
	//現在のモードの取得
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
// トータルスコアへの加算
//=============================================================================
void CTotalScore::AddTotalScore(int nTotalScore)
{
	m_nTotalScore += nTotalScore;
	m_state = STATE_ROULETTE;
	int nNum = (int)std::pow(POWER_X, m_nTotalNum);

	while ((m_nTotalScore / nNum) != 0)
	{
		m_nTotalNum++;
		nNum = (int)std::pow(POWER_X, m_nTotalNum);		// べき乗する(二乗や三条など)
		if (m_nTotalNum > TOTALSCORE_MAX - 1) { m_nTotalNum = TOTALSCORE_MAX - 1; }
	}
}

//=============================================================================
// トータルスコアの設定(ランキング用)
//=============================================================================
void CTotalScore::SetRankingScore(int nTotalScore)
{
	m_nRankingScore += nTotalScore;
	RouletteNone();
	int nNum = (int)std::pow(POWER_X, m_nRankingTotalNum);
	while ((m_nRankingScore / nNum) != 0)
	{
		m_nRankingTotalNum++;
		nNum = (int)std::pow(POWER_X, m_nRankingTotalNum);		// べき乗する(二乗や三条など)
		if (m_nRankingTotalNum > TOTALSCORE_MAX - 1) { m_nRankingTotalNum = TOTALSCORE_MAX - 1; }
	}
}

//=============================================================================
// ルーレットをしないでトータルスコアを出す
//=============================================================================
void CTotalScore::RouletteNone(void)
{
	m_nCntRoulette = RESULT_ROULETTE_CAHNGE + 1;
	m_nCntNum = m_nTotalNum - 1;
	m_nCntNum = m_nRankingTotalNum - 1;
	m_state = STATE_DISSCORE;
}
//=============================================================================
// ルーレットのように回す処理
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
	{// テクスチャに反映
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

	// トータルスコアの表示状態の場合 カウンターのリセット
	if (m_state == STATE_DISSCORE) { m_nCntAdd = 0; m_nCntRoulette = 0; }

	return nNumData;
}

//=============================================================================
// 色の点滅
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
// タイプの設定
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