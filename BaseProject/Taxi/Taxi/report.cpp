//=============================================================================
//
// 報告の処理 [report.cpp]
// Author : 有馬武志
//
//=============================================================================
#include "scene.h"
#include "scene2D.h"
#include "report.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "sound.h"
#include "game.h"
#include "logo.h"
#include "texture.h"
#include "satisfactionlevel.h"
#include "totalscore.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RESULT_LOGO_POS		(D3DXVECTOR3(240.0f, 80.0f, 0.0f))			// リザルトロゴ(位置)
#define RESULT_LOGO_SIZE	(D3DXVECTOR2(150.0f, 50.0f))				// リザルトロゴ(サイズ)
#define TOTAL_LOGO_POS		(D3DXVECTOR3(270.0f, 220.0f, 0.0f))			// トータルロゴ(位置)
#define TOTAL_LOGO_SIZE		(D3DXVECTOR2(220.0f, 25.0f))				// トータルロゴ(サイズ)
#define TOTALSCORE_POS		(D3DXVECTOR3(400.0f, 310.0f, 0.0f))			// トータルスコア(位置)
#define CLAS_POS			(D3DXVECTOR3(140.0f, 460.0f, 0.0f))			// 満足度ロゴ(位置)
#define CLAS_SIZE			(D3DXVECTOR2(100.0f, 30.0f))				// 満足度ロゴ(サイズ)
#define CLAS_LEVEL_POS		(D3DXVECTOR3(30.0f,  540.0f, 0.0f))			// 満足度(位置)
#define CLAS_LEVEL_SIZE		(D3DXVECTOR2(15.0f, 35.0f))					// 満足度(サイズ)
#define PRESS_BUTTON_POS	(D3DXVECTOR3(140.0f, 670.0f, 0.0f))			// 決定ボタン(位置)
#define PRESS_BUTTON_SIZE	(D3DXVECTOR2(100.0f, 30.0f))				// 決定ボタン(サイズ)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CReport::CReport() : CScene(4)//優先優位イニシャライズ
{
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = 0.0f;
	m_fAngle = 0.0f;
	m_nCntLogo = 0;
	m_pSatisfaction = NULL;
	m_pTotalScore = NULL;
	m_type = TYPE_NONE;
	m_nType = 0;
	for (int nCntLogo = 0; nCntLogo < REPORT_LOGO_NUM; nCntLogo++)
	{
		m_apLogo[nCntLogo] = NULL;
	}
}
//===============================================================================
//　デストラクタ
//===============================================================================
CReport::~CReport()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CReport::Init(void)
{
	// ロゴ生成
	m_apLogo[0] = CLogo::Create(RESULT_LOGO_POS, RESULT_LOGO_SIZE, CTexture::TYPE_RESLUT_LOGO000, 0, CLogo::TYPE_LOGO);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CReport::Uninit(void)
{
	// 満足度の破棄
	if (m_pSatisfaction != NULL)
	{
		m_pSatisfaction->Uninit();
		m_pSatisfaction = NULL;
	}

	// トータルスコアの破棄
	if (m_pTotalScore != NULL)
	{
		m_pTotalScore->Uninit();
		m_pTotalScore = NULL;
	}

	// ロゴの破棄
	for (int nCntLogo = 0; nCntLogo < REPORT_LOGO_NUM; nCntLogo++)
	{
		if (m_apLogo[nCntLogo] != NULL)
		{
			m_apLogo[nCntLogo]->Uninit();
			m_apLogo[nCntLogo] = NULL;
		}
	}

	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CReport::Update(void)
{
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputMouse *pCInputMouse = CManager::GetInputMouse();
	//サウンドの情報
	CSound *pSound = CManager::GetSound();

	//現在のモードの取得
	CManager::MODE GameMode = CManager::GetMode();

	SetDisplay();

}
//=============================================================================
// 描画処理
//=============================================================================
void CReport::Draw(void)
{
	// NULLチェックして描画
	if (m_pSatisfaction != NULL)
	{
		m_pSatisfaction->Draw();
	}
	if (m_pTotalScore != NULL)
	{
		m_pTotalScore->Draw();
	}
	for (int nCntLogo = 0; nCntLogo < REPORT_LOGO_NUM; nCntLogo++)
	{
		if (m_apLogo[nCntLogo] != NULL)
		{
			m_apLogo[nCntLogo]->Draw();
		}
	}
}
//===============================================================================
//　表示する
//===============================================================================
void CReport::SetDisplay()
{
	if (m_type == TYPE_ALL_DISPLAY)
	{	// 全て表示
		produceAll();

		if (m_pTotalScore != NULL) { m_pTotalScore->RouletteNone();	}	// ルーレットをしない(スコアを出す)
	}
	else
	{	// 1個ずつ表示
		produce();
	}
}
//===============================================================================
//　生成する
//===============================================================================
void CReport::produce()
{
	if (m_apLogo[1] == NULL && m_nCntLogo == 0 && m_type == TYPE_TOTALLOGO)
	{
		m_nCntLogo++;
		m_apLogo[1] = CLogo::Create(TOTAL_LOGO_POS, TOTAL_LOGO_SIZE, CTexture::TYPE_RESLUT_LOGO001, 0, CLogo::TYPE_LOGO);
	}
	else if (m_pTotalScore == NULL && m_nCntLogo == 1 && m_type == TYPE_TOTALSCORE)
	{
		m_nCntLogo++;
		m_pTotalScore = CTotalScore::Create(TOTALSCORE_POS);
		m_pTotalScore->AddTotalScore(m_nTotalScore);
		CLogo::Create(D3DXVECTOR3(TOTALSCORE_POS.x - 320.0f, TOTALSCORE_POS.y, 0.0f), D3DXVECTOR2(30.0f, 30.0f), CTexture::TYPE_MONEY_TEN, 0, CLogo::TYPE_LOGO);
	}
	else if (m_apLogo[2] == NULL && m_nCntLogo == 2 && m_type == TYPE_CLAS)
	{
		m_nCntLogo++;
		m_apLogo[2] = CLogo::Create(CLAS_POS, CLAS_SIZE, CTexture::TYPE_RESULT_LOGO002, 0, CLogo::TYPE_LOGO);
	}
	else if (m_pSatisfaction == NULL && m_nCntLogo == 3 && m_type == TYPE_CLASLEVEL)
	{
		m_nCntLogo++;
		m_pSatisfaction = CSatisfaction::Create(CLAS_LEVEL_POS, CLAS_LEVEL_SIZE, m_nLevel);
	}
	else if (m_apLogo[3] == NULL && m_nCntLogo == 4 && m_type == TYPE_OK_KEY)
	{
		m_nCntLogo++;
		m_apLogo[3] = CLogo::Create(PRESS_BUTTON_POS, PRESS_BUTTON_SIZE, CTexture::TYPE_RESULT_BUTTON, 0, CLogo::TYPE_LOGO);
	}
}
//===============================================================================
//　全て生成する
//===============================================================================
void CReport::produceAll()
{
	for (int nCntProduce = m_nType; nCntProduce < TYPE_MAX - 1; nCntProduce++, m_type = (REPORTTYPE)nCntProduce)
	{
		produce();
	}
}
//===============================================================================
//　クリエイト
//===============================================================================
CReport * CReport::Create(REPORTTYPE type, int nTotalScore, int nLevel)
{
	CReport *pReport = NULL;

	//NULLチェック
	if (pReport == NULL)
	{//メモリの動的確保

		pReport = new CReport;

		if (pReport != NULL)
		{
			//種類を割り当てる
			pReport->m_type = type;
			// トータルスコアを割り当てる
			pReport->m_nTotalScore = nTotalScore;
			// 満足度のレベルを割り当てる
			pReport->m_nLevel = nLevel;
			//オブジェクトクラスの生成
			pReport->Init();
		}
	}
	return pReport;
}
