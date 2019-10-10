//=============================================================================
//
// スコア処理 [score.cpp]
// Author : shun yokomichi
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "score.h"
#include "number.h"
#include "logo.h"
#include "totalscore.h"
#include "numberdisplay.h"
#include "combo.h"
#include "texture.h"
#include "tutorial.h"
//=============================================================================
// マクロ定義
//=============================================================================
#define CENTER_WIDTH		(SCREEN_WIDTH / 2)	// Xの中心座標
#define TIMER_SPACE			(30.0f)				// 数字と数字の間のサイズ
#define TIMER_POSITION_Y	(70.0f)				// タイマーのY座標
#define ROULETTE_COUNT		(2)					// ルーレットのカウント切替
#define ROULETTE_CAHNGE		(8)					// ルーレットから切り替わるフレーム数
#define POWER_X				(10)				// べき乗((X * X) Y)する数値
//=============================================================================
//	静的メンバ変数
//=============================================================================
int				CScore::m_nScore	= 0;
int				CScore::m_nScoreNum = 1;
CScore::STATE	CScore::m_state		= STATE_NONE;
CScore::TYPE	CScore::m_type		= TYPE_NOT_USE;
//=============================================================================
// 生成処理
//=============================================================================
CScore *CScore::Create(D3DXVECTOR3 pos)
{
	CScore *pScore = NULL;

	//NULLチェック
	if (pScore == NULL)
	{//動的確保
		pScore = new CScore;

		//NULLチェック
		if (pScore != NULL)
		{
			//位置の設定
			pScore->m_pos = pos;
			//初期化処理
			pScore->Init();
		}
	}

	return pScore;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CScore::CScore(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_nCntRoulette	= 0;
	m_nCntAdd		= 0;
	m_nCntNum		= 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CScore::~CScore(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScore::Init(void)
{
	m_nScoreNum = 1;
	m_nScore = 0;
	m_type = TYPE::TYPE_NOT_USE;
	for (int nCntTime = 0; nCntTime < SCORE_MAX; nCntTime++)
	{
		m_apNumber[nCntTime] = new CNumber;
		m_apNumber[nCntTime]->Init(D3DXVECTOR3((m_pos.x - TIMER_SPACE * nCntTime), m_pos.y, m_pos.z), 1);
		m_apNumber[nCntTime]->SetCol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
		m_apNumber[nCntTime]->SetSize(D3DXVECTOR2(15.0f, 13.0f), D3DXVECTOR2((m_pos.x - TIMER_SPACE * nCntTime), m_pos.y));
	}


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CScore::Uninit(void)
{
	for (int nCntTime = 0; nCntTime < SCORE_MAX; nCntTime++)
	{
		m_apNumber[nCntTime]->Uninit();
		m_apNumber[nCntTime] = NULL;
	}

	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CScore::Update(void)
{
	//現在のモードの取得
	CManager::MODE GameMode = CManager::GetMode();

	//DebugKey();	// デバック用

	int nTexData = m_nScore;

	if (m_state == STATE_SET)
	{	// 設定状態なら
		m_type = TYPE::TYPE_USE;
		int nNum = (int)std::pow(POWER_X, m_nScoreNum);
		while ((m_nScore / nNum) != 0)
		{
			m_nScoreNum++;
			nNum = (int)std::pow(POWER_X, m_nScoreNum);		// べき乗する(二乗や三条など)
			if (m_nScoreNum > SCORE_MAX - 1) { m_nScoreNum = SCORE_MAX - 1; }
		}
		m_state = STATE_ROULETTE;
	}

	if (m_state == STATE_ADD_TOTAL)
	{	// 加算時の処理
		m_nScoreNum = 1;
		m_nCntRoulette = ROULETTE_CAHNGE + 1;
		m_nCntNum = m_nScoreNum - 1;
		m_state = STATE_ROULETTE;
		m_type = TYPE::TYPE_NOT_USE;
	}

	if (m_state == STATE_RESET)
	{	// リセット状態
		m_nScoreNum = 1;
		m_nCntRoulette = 0;
		m_nCntNum = m_nScoreNum - 1;
		m_state = STATE_ROULETTE;
		m_type = TYPE::TYPE_NOT_USE;
	}

	if (m_state == STATE_ROULETTE)
	{	// ルーレット状態の処理
		if (m_nCntRoulette >= ROULETTE_CAHNGE)
		{
			NumberRoulette(nTexData);
		}
		else
		{
			for (int nCntTime = 0; nCntTime < m_nScoreNum; nCntTime++)
			{// テクスチャに反映
				m_apNumber[nCntTime]->SetNumber(m_nCntAdd);
			}
		}
		m_nCntRoulette++;
		m_nCntAdd++;
	}

	SetType();

}

//=============================================================================
// 描画処理
//=============================================================================
void CScore::Draw(void)
{
	for (int nCntTime = 0; nCntTime < m_nScoreNum; nCntTime++)
	{
		m_apNumber[nCntTime]->Draw();
	}
}

//=============================================================================
// スコア設定処理
//=============================================================================
void CScore::SetScore(int nScore)
{
	m_nScore = nScore;
	m_state = STATE_SET;
}

//=============================================================================
// スコア加算処理
//=============================================================================
void CScore::AddScore(int nScore)
{
	m_nScore += nScore;
	CNumberDisplay::Create(nScore);
	m_state = STATE_SET;
}

//=============================================================================
// べき乗計算
//=============================================================================
void CScore::pow(void)
{
	int nNum = (int)std::pow(POWER_X, m_nScoreNum);
	while ((m_nScore / nNum) != 0)
	{
		m_nScoreNum++;
		nNum = (int)std::pow(POWER_X, m_nScoreNum);		// べき乗する(二乗や三条など)
		if (m_nScoreNum > SCORE_MAX - 1) { m_nScoreNum = SCORE_MAX - 1; }
	}
}

//=============================================================================
// トータルスコアへの加算処理
//=============================================================================
void CScore::AddTotalScore(void)
{
	CTotalScore::AddTotalScore(m_nScore);
	m_state = STATE_ADD_TOTAL;
	m_nScore = 0;
}

//=============================================================================
// ルーレットのように回す処理
//=============================================================================
void CScore::NumberRoulette(int nTexData)
{
	if (m_nCntRoulette == ROULETTE_COUNT) { m_nCntNum = 0; }
	if ((m_nCntRoulette % ROULETTE_CAHNGE) == 0) { m_nCntNum++; }
	if (m_nCntNum > m_nScoreNum - 1) { m_nCntNum = m_nScoreNum - 1; m_state = STATE_DESSCORE; }
	for (int nCntScore = 0; nCntScore < m_nScoreNum; nCntScore++)
	{// テクスチャに反映
		if (nCntScore <= m_nCntNum)
		{
			m_apNumber[nCntScore]->SetNumber(nTexData);
			nTexData /= 10;
		}
		else { m_apNumber[nCntScore]->SetNumber(m_nCntAdd); }
	}

	// トータルスコアの表示状態の場合 カウンターのリセット
	if (m_state == STATE_DESSCORE) { m_nCntAdd = 0; m_nCntRoulette = 0; }

}

//=============================================================================
// デバック用
//=============================================================================
void CScore::DebugKey(void)
{
	if (CManager::GetInput()->GetKeyboardTrigger(DIK_2))
	{	// スコア加算
		AddScore(100);
		CNumberDisplay::Create(100);
		if (CCombo::GetCombo() == 0)
		{
			CCombo::Create(CCombo::TYPE_THROUGH);
		}
		else
		{
			CCombo::AddCombo(1);
		}
	}
	if (CManager::GetInput()->GetKeyboardTrigger(DIK_3))
	{
		Reset();
	}
}

//=============================================================================
// タイプの設定
//=============================================================================
void CScore::SetType()
{
	CManager::MODE mode = CManager::GetMode();
	if (m_type == TYPE::TYPE_USE)
	{
		if (mode == CManager::MODE_GAME)
		{
			CGame::GetScoreUI(1)->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			CGame::GetScoreUI(3)->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			CGame::GetScoreUI(5)->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else if (mode == CManager::MODE_TUTORIAL)
		{
			CTutorial::GetScoreUI(1)->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			CTutorial::GetScoreUI(3)->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			CTutorial::GetScoreUI(5)->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
	else if (m_type == TYPE::TYPE_NOT_USE)
	{
		if (mode == CManager::MODE_GAME)
		{
			CGame::GetScoreUI(1)->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
			CGame::GetScoreUI(3)->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
			CGame::GetScoreUI(5)->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
		}
		else if (mode == CManager::MODE_TUTORIAL)
		{
			CTutorial::GetScoreUI(1)->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
			CTutorial::GetScoreUI(3)->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
			CTutorial::GetScoreUI(5)->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
		}
	}
}