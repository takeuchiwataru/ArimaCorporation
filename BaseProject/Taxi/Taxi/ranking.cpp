//=============================================================================
//
// ランキングの処理 [ranking.cpp]
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
#include "logo.h"
#include "select.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FAREM_COUNT					(60 * 10)													// フェードのフレーム数
#define RAKING_LOOG_POS				(D3DXVECTOR3(230.0f, 70.0f, 0.0f))							// ランキングのロゴ(位置)
#define RAKING_LOOG_SIZE			(D3DXVECTOR2(200.0f, 60.0f))								// ランキングのロゴ(サイズ)
#define RAKING_RANK_POS				(D3DXVECTOR3(80.0f + (nCntRank * 670.0f), 460.0f, 0.0f))	// 順位の位置
#define RAKING_RANK_SIZE			(D3DXVECTOR2(50.0f, 220.0f))								// 順位のサイズ
#define RAKING_CLASS_POS			(D3DXVECTOR3(310.0f, 200.0f, 0.0f))							// CLASSの位置
#define RAKING_CLASS_SIZE			(D3DXVECTOR2(130.0f, 30.0f))								// CLASSのサイズ
#define RAKING_TOTAL_POS			(D3DXVECTOR3(950.0f, RAKING_CLASS_POS.y, 0.0f))				// トータルスコアのロゴ(位置)
#define RAKING_TOTAL_SIZE			(D3DXVECTOR2(RAKING_CLASS_SIZE.x, RAKING_CLASS_SIZE.y))		// トータルスコアのロゴ(サイズ)
#define RAKING_NUMBER_POS			(D3DXVECTOR3(1180.0f, 270.0f + (nCntRnaking * 95.0f), 0.0f))// スコアの位置
#define RAKING_NUMBER_SIZE			(D3DXVECTOR2(20.0f, 20.0f))									// スコアのサイズ
#define RAKING_SATISFACTION_POS		(D3DXVECTOR3(180.0f, 270.0f + (nCntSatisfaction * 95.0f), 0.0f))// 満足度の位置
#define RAKING_SATISFACTION_SIZE	(D3DXVECTOR2(20.0f, 30.0f))										// 満足度のサイズ
#define RAKING_BG_POS				(D3DXVECTOR3(310.0f + (nCntBg * 650.0f), 450.0f, 0.0f))			// 半透明の背景(位置)
#define RAKING_BG_SIZE				(D3DXVECTOR2(290.0f, 250.0f))									// 半透明の背景(サイズ)
#define RANK_IN_COL					(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))								// ランクイン時の色

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

int				 CRanking::m_nNum = MAX_RANKING;
//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CRanking::CRanking()
{
	m_nCntFrame = 0;
}
//=============================================================================
// デストラクタ
//=============================================================================
CRanking::~CRanking()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CRanking::Init()
{
	int nTotalScore = 0;
	int nSatisfaction = 0;
	int nSelectLevel = CSelect::GetSelectLevel();
	//===================================
	//		 Loadの読み込み場所
	//===================================
	//フェードのテクスチャの読み込み
	CFade::Load();

	RankingLoad();

	CBackGround::Create(CTexture::TYPE_BACKGROUND001);

	for (int nCntBg = 0; nCntBg < 2; nCntBg++)
	{	// 半透明の背景
		CLogo::Create(RAKING_BG_POS, RAKING_BG_SIZE, CTexture::TYPE_BG_FREAM, 0, CLogo::TYPE_LOGO);
	}

	CLogo::Create(RAKING_LOOG_POS, RAKING_LOOG_SIZE, CTexture::TYPE_RANKING_LOGO000, 0, CLogo::TYPE_LOGO);	// ロゴ
	for (int nCntRank = 0; nCntRank < 2; nCntRank++)
	{
		CLogo::Create(RAKING_RANK_POS, RAKING_RANK_SIZE, CTexture::TYPE_RANKING_RANK, 0, CLogo::TYPE_LOGO);		// 順位
	}
	CLogo::Create(RAKING_CLASS_POS, RAKING_CLASS_SIZE, CTexture::TYPE_RANKING_CLASS, 0, CLogo::TYPE_LOGO);	// クラス
	CLogo::Create(RAKING_TOTAL_POS, RAKING_TOTAL_SIZE, CTexture::TYPE_RANKING_TOTALLOGO, 0, CLogo::TYPE_LOGO);	// トータルスコア

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CRanking::Uninit(void)
{
	//===================================
	//	　　UnLoadの破棄する場所
	//===================================
	//フェードのテクスチャの破棄
	CFade::UnLoad();

	//フェード以外削除
	CScene::NotFadeReleseAll();
}
//=============================================================================
// 更新処理
//=============================================================================
void CRanking::Update(void)
{
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CXInput *pCInputJoypad = CManager::GetXInput();
	CInputMouse *pCInputMouse = CManager::GetInputMouse();

	//サウンドの情報
	CSound *pSound = CManager::GetSound();

	//フェードのポイント
	CFade::FADE pFade = CFade::GetFade();

	if (pCInputJoypad->GetAnyButton()
		|| pCInputKeyBoard->GetKeyboardAny(1) == true)
	{//ランキングからタイトルへ
	 //フェードが始まったら
		if (pFade == CFade::FADE_NONE)
		{
			//ポーズの選択の決定音
			pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_SELECT);
			CFade::Create(CManager::MODE_TITLE);
		}
	}

	if (m_nCntFrame == FAREM_COUNT) { CFade::Create(CManager::MODE_TITLE);}

	m_nCntFrame++;

	if (m_nNum >= MAX_RANKING) { return; }	// ランキングの最大数以上の場合 更新処理の終了
}
//=============================================================================
// 描画処理
//=============================================================================
void CRanking::Draw(void)
{
	
}

//=============================================================================
// ランキングの書き込み
//=============================================================================
void CRanking::RankingSave(int nTotalScore, int nLvevl)
{
	//CTotalScore *pTotalScore = NULL;		// ポインタ生成

	//int nData;		// データ入れ替え用
	//int nScore = nTotalScore;		// 現在のスコア保管
	//int nSatisfaction = nLvevl;
	//int nSelectLevel = CSelect::GetSelectLevel();
	//m_nNum = MAX_RANKING;
	//m_nSatisfaction = MAX_RANKING;

	//// ランクの入れ替え
	//for (int nCount = 0; nCount < MAX_RANKING; nCount++)
	//{
	//	if (m_aTotalScore[nSelectLevel][MAX_RANKING - 1] <= nScore)
	//	{ // ランクインするなら
	//		m_nNum -= 1;
	//		m_aTotalScore[nSelectLevel][MAX_RANKING - 1] = nScore;
	//	}

	//	for (int nCount001 = 0; nCount001 < MAX_RANKING - 1; nCount001++)
	//	{
	//		for (int nCount002 = nCount001 + 1; nCount002 < MAX_RANKING; nCount002++)
	//		{
	//			if (m_aTotalScore[nSelectLevel][nCount002] >= m_aTotalScore[nSelectLevel][nCount001])
	//			{	// 順位の入れ替え
	//				nData = m_aTotalScore[nSelectLevel][nCount001];
	//				m_aTotalScore[nSelectLevel][nCount001] = m_aTotalScore[nSelectLevel][nCount002];
	//				m_aTotalScore[nSelectLevel][nCount002] = nData;
	//				m_nNum--;
	//			}
	//		}
	//	}
	//	break;
	//}

	//// 満足度の入れ替え
	//for (int nCount = 0; nCount < MAX_RANKING; nCount++)
	//{
	//	if (m_aSatisfactionLevel[nSelectLevel][MAX_RANKING - 1] <= nSatisfaction)
	//	{ // ランクインするなら
	//		m_nSatisfaction -= 1;
	//		m_aSatisfactionLevel[nSelectLevel][MAX_RANKING - 1] = nSatisfaction;
	//	}

	//	for (int nCount001 = 0; nCount001 < MAX_RANKING - 1; nCount001++)
	//	{
	//		for (int nCount002 = nCount001 + 1; nCount002 < MAX_RANKING; nCount002++)
	//		{
	//			if (m_aSatisfactionLevel[nSelectLevel][nCount002] >= m_aSatisfactionLevel[nSelectLevel][nCount001])
	//			{	// 順位の入れ替え
	//				nData = m_aSatisfactionLevel[nSelectLevel][nCount001];
	//				m_aSatisfactionLevel[nSelectLevel][nCount001] = m_aSatisfactionLevel[nSelectLevel][nCount002];
	//				m_aSatisfactionLevel[nSelectLevel][nCount002] = nData;
	//				m_nSatisfaction--;
	//			}
	//		}
	//	}
	//	break;
	//}


	//// ファイルの書き込み
	//FILE *pFile = NULL;

	//// ランキングの順位の書き込み
	//if (nSelectLevel == 0)
	//{
	//	pFile = fopen("data/TEXT/Ranking/ranking.bin", "wb");
	//}
	//else if (nSelectLevel == 1)
	//{
	//	pFile = fopen("data/TEXT/Ranking/ranking001.bin", "wb");
	//}

	//if (pFile != NULL)
	//{
	//	fwrite(m_aTotalScore[nSelectLevel], sizeof(int), MAX_RANKING, pFile);

	//	fclose(pFile);
	//}

	//// 満足度の順位を書き込み
	//if (nSelectLevel == 0)
	//{
	//	pFile = fopen("data/TEXT/Ranking/SatisfactionLevel.bin", "wb");
	//}
	//else 	if (nSelectLevel == 1)
	//{
	//	pFile = fopen("data/TEXT/Ranking/SatisfactionLevel001.bin", "wb");
	//}

	//if (pFile != NULL)
	//{
	//	fwrite(m_aSatisfactionLevel[nSelectLevel], sizeof(int), MAX_RANKING, pFile);

	//	fclose(pFile);
	//}
}

//=============================================================================
// ランキングの読み込み
//=============================================================================
void CRanking::RankingLoad(void)
{
	//// 読み込み
	//FILE *pFile = NULL;
	//int nSelectLevel = CSelect::GetSelectLevel();

	//// スコアのランキングの読み込み
	//if (nSelectLevel == 0)
	//{
	//	pFile = fopen("data/TEXT/Ranking/ranking.bin", "rb");
	//}
	//else if (nSelectLevel == 1)
	//{
	//	pFile = fopen("data/TEXT/Ranking/ranking001.bin", "rb");
	//}

	//if (pFile != NULL)
	//{
	//	fread(m_aTotalScore[nSelectLevel], sizeof(int), MAX_RANKING, pFile);

	//	fclose(pFile);
	//}

	//// 満足度のランキングの読み込み
	//if (nSelectLevel == 0)
	//{
	//	pFile = fopen("data/TEXT/Ranking/SatisfactionLevel.bin", "wb");
	//}
	//else 	if (nSelectLevel == 1)
	//{
	//	pFile = fopen("data/TEXT/Ranking/SatisfactionLevel001.bin", "wb");
	//}
	//if (pFile != NULL)
	//{
	//	fwrite(m_aSatisfactionLevel[nSelectLevel], sizeof(int), MAX_RANKING, pFile);

	//	fclose(pFile);
	//}
}