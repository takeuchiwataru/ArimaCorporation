//=============================================================================
//
// リザルトUIの処理 [resultui.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS

#include "resultui.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "input.h"
#include "fade.h"
#include "time.h"

#include "number.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	CResultUI::m_pTexture[CResultUI::TEXTURE_MAX] = { NULL };

//=============================================================================
// コンストラクタ
//=============================================================================
CResultUI::CResultUI()
{
	for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
	{
		m_pRankBG[nCntMember] = NULL;		// ランクBG
		m_pRank[nCntMember] = NULL;			// ランク
		m_pChar[nCntMember] = NULL;			// キャラ
		m_pPlayer[nCntMember] = NULL;		// プレイヤー
		m_pTime[nCntMember] = NULL;			// タイム
	}
	m_pPress = NULL;						// プレス
	m_pFade = NULL;							// フェード
}
//=============================================================================
// デストラクタ
//=============================================================================
CResultUI::~CResultUI() {}

//=============================================================================
// ロード
//=============================================================================
HRESULT CResultUI::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{// テクスチャカウント
		char cName[128] = {};

		switch (nCntTex)
		{
		case TEXTURE_RANK:
			strcpy(cName, "data/TEXTURE/game/play/rank.png");
			break;
		case TEXTURE_CHAR:
			strcpy(cName, "data/TEXTURE/game/play/icon.png");
			break;
		case TEXTURE_ICON:
			strcpy(cName, "data/TEXTURE/game/charselect/icon.png");
			break;
		case TEXTURE_PRESS:
			strcpy(cName, "data/TEXTURE/Title/UI/PressButton.png");
			break;
		}

		// テクスチャの生成
		if (m_pTexture[nCntTex] == NULL)
			D3DXCreateTextureFromFile(pDevice, cName, &m_pTexture[nCntTex]);
	}

	CTime::Load();
	CNumber::Load();

	return S_OK;
}

//=============================================================================
// アンロード
//=============================================================================
void CResultUI::Unload(void)
{
	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{// テクスチャカウント
	 //テクスチャーの破棄
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}

	CTime::Unload();
	CNumber::Unload();
}

//=============================================================================
// クリエイト
//=============================================================================
CResultUI *CResultUI::Create(void)
{
	CResultUI *pResultUI = NULL;

	// メモリ確保
	if (pResultUI == NULL)
		pResultUI = new CResultUI;

	if (pResultUI != NULL)
	{// NULL以外
		pResultUI->Init();	// 初期化処理
	}

	return pResultUI;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CResultUI::Init()
{
	CManager::MODE mode = CManager::GetMode();
	int *pnCharSelectNum = NULL;
	int nMaxPlayer = 0;
	int *pnRankingSort = NULL;
	int *nTimer = NULL;

	if (mode == CManager::MODE_TITLE)
	{

	}
	else
	{
		pnCharSelectNum = CGame::GetCharSelectNum();
		nMaxPlayer = CGame::GetMaxPlayer();
		pnRankingSort = CGame::GetRankingSort();
		nTimer = CGame::GetTimer();
	}

	for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
	{// メンバーカウント
		if (mode != CManager::MODE_TITLE || (mode == CManager::MODE_TITLE && nCntMember < 3))
		{
			if (mode != CManager::MODE_TITLE || (mode == CManager::MODE_TITLE && nCntMember == 0))
			{
				// ランクBG
				if (m_pRankBG[nCntMember] == NULL)
				{// NULL
					m_pRankBG[nCntMember] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
					m_pRankBG[nCntMember]->Init();
					if (mode == CManager::MODE_TITLE)
					{
						m_pRankBG[nCntMember]->SetPosSize(
							D3DXVECTOR3
							(
							(SCREEN_WIDTH * 0.5f) + (SCREEN_WIDTH),
								(SCREEN_HEIGHT * 0.05f),
								0.0f
							),
							D3DXVECTOR2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.05f));
					}
					else
					{
						m_pRankBG[nCntMember]->SetPosSize(
							D3DXVECTOR3
							(
							(SCREEN_WIDTH * 0.5f) - (SCREEN_WIDTH * 0.17f) - (SCREEN_WIDTH),
								(SCREEN_HEIGHT * 0.12f) + (((SCREEN_HEIGHT * 0.05f) * 2.2f) * nCntMember),
								0.0f
							),
							D3DXVECTOR2(SCREEN_WIDTH * 0.29f, SCREEN_HEIGHT * 0.05f));
					}
					m_pRankBG[nCntMember]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
				}
			}

			// ランク
			if (m_pRank[nCntMember] == NULL)
			{// NULL
				m_pRank[nCntMember] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
				m_pRank[nCntMember]->Init();
				if (mode == CManager::MODE_TITLE)
				{
					m_pRank[nCntMember]->SetPosSize(
						D3DXVECTOR3
						(
						(SCREEN_HEIGHT * 0.07f) + ((SCREEN_HEIGHT * 0.6f) * nCntMember) + (SCREEN_WIDTH),
							(SCREEN_HEIGHT * 0.05f),
							0.0f
						),
						D3DXVECTOR2(SCREEN_HEIGHT * 0.07f, SCREEN_HEIGHT * 0.04f));
				}
				else
				{
					m_pRank[nCntMember]->SetPosSize(
						D3DXVECTOR3
						(
						(SCREEN_WIDTH * 0.5f) - (SCREEN_WIDTH * 0.4f) - (SCREEN_WIDTH),
							(SCREEN_HEIGHT * 0.12f) + (((SCREEN_HEIGHT * 0.05f) * 2.2f) * nCntMember),
							0.0f
						),
						D3DXVECTOR2(SCREEN_HEIGHT * 0.08f, SCREEN_HEIGHT * 0.05f));
				}
				m_pRank[nCntMember]->BindTexture(m_pTexture[TEXTURE_RANK]);
				m_pRank[nCntMember]->SetTexture(nCntMember, 8, 1, 1);
			}

			// キャラ
			if (m_pChar[nCntMember] == NULL)
			{// NULL
				m_pChar[nCntMember] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
				m_pChar[nCntMember]->Init();
				if (mode == CManager::MODE_TITLE)
				{
					m_pChar[nCntMember]->SetPosSize(
						D3DXVECTOR3
						(
						(SCREEN_HEIGHT * 0.07f) + (SCREEN_HEIGHT * 0.10f) + ((SCREEN_HEIGHT * 0.6f) * nCntMember) + (SCREEN_WIDTH),
							(SCREEN_HEIGHT * 0.05f),
							0.0f
						),
						D3DXVECTOR2(SCREEN_HEIGHT * 0.04f, SCREEN_HEIGHT * 0.04f));
				}
				else
				{
					m_pChar[nCntMember]->SetPosSize(
						D3DXVECTOR3
						(
						(SCREEN_WIDTH * 0.5f) - (SCREEN_WIDTH * 0.3f) - (SCREEN_WIDTH),
							(SCREEN_HEIGHT * 0.12f) + (((SCREEN_HEIGHT * 0.05f) * 2.2f) * nCntMember),
							0.0f
						),
						D3DXVECTOR2(SCREEN_HEIGHT * 0.05f, SCREEN_HEIGHT * 0.05f));
				}
				m_pChar[nCntMember]->BindTexture(m_pTexture[TEXTURE_CHAR]);
				
				if (mode == CManager::MODE_TITLE)
					m_pChar[nCntMember]->SetTexture(nCntMember, 8, 1, 1);
				else
					m_pChar[nCntMember]->SetTexture(pnCharSelectNum[pnRankingSort[nCntMember]], 8, 1, 1);
			}

			if (mode != CManager::MODE_TITLE)
			{
				// プレイヤー
				if (m_pPlayer[nCntMember] == NULL)
				{// NULL
					m_pPlayer[nCntMember] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
					m_pPlayer[nCntMember]->Init();
					m_pPlayer[nCntMember]->SetPosSize(
						D3DXVECTOR3
						(
						(SCREEN_WIDTH * 0.5f) - (SCREEN_WIDTH * 0.21f) - (SCREEN_WIDTH),
							(SCREEN_HEIGHT * 0.12f) - (SCREEN_HEIGHT * 0.012f) + (((SCREEN_HEIGHT * 0.05f) * 2.2f) * nCntMember),
							0.0f
						),
						D3DXVECTOR2(SCREEN_HEIGHT * 0.08f, SCREEN_HEIGHT * 0.05f));
					m_pPlayer[nCntMember]->BindTexture(m_pTexture[TEXTURE_ICON]);
					m_pPlayer[nCntMember]->SetTexture((pnRankingSort[nCntMember] < nMaxPlayer ? pnRankingSort[nCntMember] : 4), 5, 2, 1);
				}
			}

			// タイム
			if (m_pTime[nCntMember] == NULL)
			{// NULL
				if (mode == CManager::MODE_TITLE)
				{
					m_pTime[nCntMember] = CTime::Create(
						D3DXVECTOR3
						(
						(SCREEN_HEIGHT * 0.07f) + (SCREEN_HEIGHT * 0.45f) + ((SCREEN_HEIGHT * 0.6f) * nCntMember) + (SCREEN_WIDTH),
							(SCREEN_HEIGHT * 0.05f),
							0.0f
						),
						D3DXVECTOR3((SCREEN_HEIGHT * 0.02f), (SCREEN_HEIGHT * 0.04f), 0.0f));
				}
				else
				{
					m_pTime[nCntMember] = CTime::Create(
						D3DXVECTOR3
						(
						(SCREEN_WIDTH * 0.5f) + (SCREEN_WIDTH * 0.08f) - (SCREEN_WIDTH),
							(SCREEN_HEIGHT * 0.12f) + (((SCREEN_HEIGHT * 0.05f) * 2.2f) * nCntMember),
							0.0f
						),
						D3DXVECTOR3((SCREEN_HEIGHT * 0.03f), (SCREEN_HEIGHT * 0.05f), 0.0f));
				}

				if (mode != CManager::MODE_TITLE)
				{
					m_pTime[nCntMember]->TexTime(nTimer[pnRankingSort[nCntMember]], true);

					if (pnRankingSort[nCntMember] < nMaxPlayer)
						m_pTime[nCntMember]->Setcol(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
				}
			}
		}
	}

	if (mode != CManager::MODE_TITLE)
	{
		// プレス
		if (m_pPress == NULL)
		{// NULL
			m_pPress = new CScene2D(6, CScene::OBJTYPE_2DPOLYGON);
			m_pPress->Init();
			m_pPress->SetPosSize(
				D3DXVECTOR3
				(
				(SCREEN_WIDTH * 0.5f) + (SCREEN_WIDTH * 0.31f),
					(SCREEN_HEIGHT * 0.12f) + (SCREEN_HEIGHT * 0.8f),
					0.0f
				),
				D3DXVECTOR2(SCREEN_HEIGHT * 0.2f, SCREEN_HEIGHT * 0.05f));
			m_pPress->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_pPress->BindTexture(m_pTexture[TEXTURE_PRESS]);
		}

		// フェード
		if (m_pFade == NULL)
		{// NULL
			m_pFade = new CScene2D(6, CScene::OBJTYPE_2DPOLYGON);
			m_pFade->Init();
			m_pFade->SetPosSize(
				D3DXVECTOR3
				(
				(SCREEN_WIDTH * 0.5f),
					(SCREEN_HEIGHT * 0.5f),
					0.0f
				),
				D3DXVECTOR2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f));
			m_pFade->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
	}

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CResultUI::Uninit(void)
{
	for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
	{// メンバーカウント
	 // ランクBG
		if (m_pRankBG[nCntMember] != NULL)
		{// NULL以外
			m_pRankBG[nCntMember]->Uninit();
			m_pRankBG[nCntMember] = NULL;
		}

		// ランク
		if (m_pRank[nCntMember] != NULL)
		{// NULL以外
			m_pRank[nCntMember]->Uninit();
			m_pRank[nCntMember] = NULL;
		}

		// キャラ
		if (m_pChar[nCntMember] != NULL)
		{// NULL以外
			m_pChar[nCntMember]->Uninit();
			m_pChar[nCntMember] = NULL;
		}

		// プレイヤー
		if (m_pPlayer[nCntMember] != NULL)
		{// NULL以外
			m_pPlayer[nCntMember]->Uninit();
			m_pPlayer[nCntMember] = NULL;
		}

		// タイム
		if (m_pTime[nCntMember] != NULL)
		{// NULL以外
			m_pTime[nCntMember]->Uninit();
			m_pTime[nCntMember] = NULL;
		}
	}

	// プレス
	if (m_pPress != NULL)
	{// NULL以外
		m_pPress->Uninit();
		m_pPress = NULL;
	}

	// フェード
	if (m_pFade != NULL)
	{// NULL以外
		m_pFade->Uninit();
		m_pFade = NULL;
	}

	//自身の削除
	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CResultUI::Update(void)
{
	switch (CManager::GetMode())
	{
	case CManager::MODE_TITLE:
		Title();
		break;
	default:
		Result();
		break;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CResultUI::Draw(void)
{

}

//=============================================================================
// タイトル処理
//=============================================================================
void CResultUI::Title(void)
{
	int nCounter = CTitle::GetCounter() % RESET_TITLEMODE;

	if ((HOME_TITLEMODE - RANKING_TITLEMODE) <= nCounter && nCounter < HOME_TITLEMODE)
	{
		int nFrame = nCounter - (HOME_TITLEMODE - RANKING_TITLEMODE);

		for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
		{// メンバーカウント
			if (nFrame < 30 || (RANKING_TITLEMODE - 30) <= nFrame)
			{
				float fDiff = 0.0f;

				if (nFrame < 30)
				{
					fDiff = (SCREEN_WIDTH) * (1.0f - (float)((float)((nFrame % 30) + 1) / (float)30));
				}
				else
				{
					fDiff = (-SCREEN_WIDTH) * (float)((float)((nFrame % 30) + 1) / (float)30);
				}

				// ランクBG
				if (m_pRankBG[nCntMember] != NULL)
				{// NULL以外
					m_pRankBG[nCntMember]->SetPosSize(
						D3DXVECTOR3
						(
						(SCREEN_WIDTH * 0.5f) + (fDiff),
							(SCREEN_HEIGHT * 0.05f),
							0.0f
						),
						D3DXVECTOR2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.05f));
				}

				// ランク
				if (m_pRank[nCntMember] != NULL)
				{// NULL以外
					m_pRank[nCntMember]->SetPosSize(
						D3DXVECTOR3
						(
						(SCREEN_HEIGHT * 0.07f) + ((SCREEN_HEIGHT * 0.6f) * nCntMember) + (fDiff),
							(SCREEN_HEIGHT * 0.05f),
							0.0f
						),
						D3DXVECTOR2(SCREEN_HEIGHT * 0.07f, SCREEN_HEIGHT * 0.04f));
				}

				// キャラ
				if (m_pChar[nCntMember] != NULL)
				{// NULL以外
					m_pChar[nCntMember]->SetPosSize(
						D3DXVECTOR3
						(
						(SCREEN_HEIGHT * 0.07f) + (SCREEN_HEIGHT * 0.10f) + ((SCREEN_HEIGHT * 0.6f) * nCntMember) + (fDiff),
							(SCREEN_HEIGHT * 0.05f),
							0.0f
						),
						D3DXVECTOR2(SCREEN_HEIGHT * 0.04f, SCREEN_HEIGHT * 0.04f));
				}

				// タイム
				if (m_pTime[nCntMember] != NULL)
				{// NULL以外
					m_pTime[nCntMember]->Setpos(
						D3DXVECTOR3
						(
						(SCREEN_HEIGHT * 0.07f) + (SCREEN_HEIGHT * 0.45f) + ((SCREEN_HEIGHT * 0.6f) * nCntMember) + (fDiff),
							(SCREEN_HEIGHT * 0.05f),
							0.0f
						));
				}
			}
		}
	}
}

//=============================================================================
// リザルト処理
//=============================================================================
void CResultUI::Result(void)
{
	int nCounter = CResult::GetCounter();

	if (RESULT_RANK_APP < nCounter)
	{
		int nFrame = nCounter - RESULT_RANK_APP;

		for (int nCntMember = (MAX_MEMBER - 1); 0 <= nCntMember; nCntMember--)
		{// メンバーカウント
			if (((MAX_MEMBER - 1) - nCntMember) == (nFrame / 10))
			{
				float fDiff = -(SCREEN_WIDTH) * (1.0f - (float)((float)((nFrame % 10) + 1) / (float)10));

				// ランクBG
				if (m_pRankBG[nCntMember] != NULL)
				{// NULL以外
					m_pRankBG[nCntMember]->SetPosSize(
						D3DXVECTOR3
						(
						(SCREEN_WIDTH * 0.5f) - (SCREEN_WIDTH * 0.17f) + fDiff,
							(SCREEN_HEIGHT * 0.12f) + (((SCREEN_HEIGHT * 0.05f) * 2.2f) * nCntMember),
							0.0f
						),
						D3DXVECTOR2(SCREEN_WIDTH * 0.29f, SCREEN_HEIGHT * 0.05f));
				}

				// ランク
				if (m_pRank[nCntMember] != NULL)
				{// NULL以外
					m_pRank[nCntMember]->SetPosSize(
						D3DXVECTOR3
						(
						(SCREEN_WIDTH * 0.5f) - (SCREEN_WIDTH * 0.4f) + fDiff,
							(SCREEN_HEIGHT * 0.12f) + (((SCREEN_HEIGHT * 0.05f) * 2.2f) * nCntMember),
							0.0f
						),
						D3DXVECTOR2(SCREEN_HEIGHT * 0.08f, SCREEN_HEIGHT * 0.05f));
				}

				// キャラ
				if (m_pChar[nCntMember] != NULL)
				{// NULL以外
					m_pChar[nCntMember]->SetPosSize(
						D3DXVECTOR3
						(
						(SCREEN_WIDTH * 0.5f) - (SCREEN_WIDTH * 0.3f) + fDiff,
							(SCREEN_HEIGHT * 0.12f) + (((SCREEN_HEIGHT * 0.05f) * 2.2f) * nCntMember),
							0.0f
						),
						D3DXVECTOR2(SCREEN_HEIGHT * 0.05f, SCREEN_HEIGHT * 0.05f));
				}

				// プレイヤー
				if (m_pPlayer[nCntMember] != NULL)
				{// NULL以外
					m_pPlayer[nCntMember]->SetPosSize(
						D3DXVECTOR3
						(
						(SCREEN_WIDTH * 0.5f) - (SCREEN_WIDTH * 0.21f) + fDiff,
							(SCREEN_HEIGHT * 0.12f) - (SCREEN_HEIGHT * 0.012f) + (((SCREEN_HEIGHT * 0.05f) * 2.2f) * nCntMember),
							0.0f
						),
						D3DXVECTOR2(SCREEN_HEIGHT * 0.08f, SCREEN_HEIGHT * 0.05f));
				}

				// タイム
				if (m_pTime[nCntMember] != NULL)
				{// NULL以外
					m_pTime[nCntMember]->Setpos(D3DXVECTOR3
					(
						(SCREEN_WIDTH * 0.5f) + (SCREEN_WIDTH * 0.08f) + fDiff,
						(SCREEN_HEIGHT * 0.12f) + (((SCREEN_HEIGHT * 0.05f) * 2.2f) * nCntMember),
						0.0f
					));
				}
			}
		}
	}

	if (RESULT_WAIT < nCounter)
	{
		int nFrame = nCounter - RESULT_WAIT;

		// プレス
		if (m_pPress != NULL)
		{// NULL以外
			nFrame %= 120;
			int nNum = nFrame % 60;

			m_pPress->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, (nFrame < 60 ? (float)((float)nNum / (float)60) : 1.0f - (float)((float)nNum / (float)60))));
		}
	}

	if ((RESULT_RANK_APP - 15) <= nCounter && nCounter < (RESULT_RANK_APP + 15))
	{
		int nFrame = nCounter - (RESULT_RANK_APP - 15);
		int nNum = (nFrame % 10) + 1;

		if (m_pFade != NULL)
		{// NULL以外
			if (nFrame / 10 != 1)
				m_pFade->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, (nFrame < 10 ? (float)((float)nNum / (float)10) : 1.0f - (float)((float)nNum / (float)10))));
		}
	}
}