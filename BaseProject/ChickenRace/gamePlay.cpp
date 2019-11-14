//=============================================================================
//
// ゲーム（プレイ）の処理 [game.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS	//Fire Wall突破
#include <string.h>				//ストリング使用のため
#include "gamePlay.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "input.h"
#include "fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define COUNTDOWN_SIZE		(SCREEN_HEIGHT * 0.12f)										// カウントダウンの大きさ
#define RANKING_SIZE_1P_X	(0.13f)														// 1P時のランキングの大きさX
#define RANKING_SIZE_1P_Y	(0.09f)														// 1P時のランキングの大きさY
#define RANKING_SIZE_4P_X	(0.07f)														// 4P時のランキングの大きさX
#define RANKING_SIZE_4P_Y	(0.05f)														// 4P時のランキングの大きさY
#define ITEM_SIZE_1P		(0.11f)														// 1P時のアイテムの大きさ
#define ITEM_SIZE_4P		(0.06f)														// 4P時のアイテムの大きさ
#define GOUL_SIZE_2P		(D3DXVECTOR2(SCREEN_WIDTH * 0.20f, SCREEN_WIDTH * 0.07f))	// 2P時のゴールの大きさ
#define GOUL_SIZE_4P		(D3DXVECTOR2(SCREEN_WIDTH * 0.15f, SCREEN_WIDTH * 0.05f))	// 4P時のゴールの大きさ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	CGamePlay::m_pTexture[CGamePlay::TEXTURE_MAX] = { NULL };

//=============================================================================
// コンストラクタ
//=============================================================================
CGamePlay::CGamePlay()
{
	for (int nCntLine = 0; nCntLine < MAX_LINE; nCntLine++)
	{// 線カウント
		m_pLine[nCntLine] = NULL;					// 線
	}

	for (int nCntDown = 0; nCntDown < COUNT_DOWN; nCntDown++)
	{// ダウンカウント
		m_pCountDown[nCntDown] = NULL;				// カウントダウン
	}

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// プレイヤーカウント
		m_pRanking[nCntPlayer] = NULL;				// ランキング

		for (int nCntItem = 0; nCntItem < MAX_EGG; nCntItem++)
		{// アイテムフレームカウント
			m_pItemFrame[nCntPlayer][nCntItem] = NULL;	// アイテムフレームフレーム
			m_pItem[nCntPlayer][nCntItem] = NULL;	// アイテムフレーム
		}

		m_pGoul[nCntPlayer] = NULL;					// ランキング
	}
}
//=============================================================================
// デストラクタ
//=============================================================================
CGamePlay::~CGamePlay() {}

//=============================================================================
// ロード
//=============================================================================
HRESULT CGamePlay::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{// テクスチャカウント
		char cName[128] = {};

		switch (nCntTex)
		{
		case TEXTURE_COUNT:
			strcpy(cName, "data/TEXTURE/game/play/count.png");
			break;
		case TEXTURE_RANK:
			strcpy(cName, "data/TEXTURE/game/play/rank.png");
			break;
		case TEXTURE_ITEM:
			strcpy(cName, "data/TEXTURE/game/play/eggslot.png");
			break;
		case TEXTURE_GOUL:
			strcpy(cName, "data/TEXTURE/game/play/finish.png");
			break;
		}

		// テクスチャの生成
		if (m_pTexture[nCntTex] == NULL)
			D3DXCreateTextureFromFile(pDevice, cName, &m_pTexture[nCntTex]);
	}

	return S_OK;
}

//=============================================================================
// アンロード
//=============================================================================
void CGamePlay::Unload(void)
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
}

//=============================================================================
// クリエイト
//=============================================================================
CGamePlay *CGamePlay::Create(void)
{
	CGamePlay *pGamePlay = NULL;

	// メモリ確保
	if (pGamePlay == NULL)
		pGamePlay = new CGamePlay;

	if (pGamePlay != NULL)
	{// NULL以外
		pGamePlay->Init();	// 初期化処理
	}

	return pGamePlay;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGamePlay::Init()
{
	// 取得
	bool bOnine = CTitle::GetOnline();
	int nClient = 0;

	if (bOnine == true)
		nClient = CClient::GetnID();

	// プレイヤー最大数取得
	int nMaxPlayer = CGame::GetMaxPlayer();

	if (bOnine == false)
	{// オンラインじゃない
		if (1 < nMaxPlayer)
		{// 2人以上
			for (int nCntLine = 0; nCntLine < ((nMaxPlayer - 1) / 2 == 0 ? 1 : MAX_LINE); nCntLine++)
			{// 線カウント
				if (m_pLine[nCntLine] == NULL)
				{// NULL以外
					D3DXVECTOR2 size = D3DXVECTOR2(0.0f, 0.0f);

					if (nCntLine == 0)
						size = D3DXVECTOR2((SCREEN_WIDTH * 0.5f), (SCREEN_WIDTH * 0.0006f));
					else
						size = D3DXVECTOR2((SCREEN_WIDTH * 0.0006f), (SCREEN_HEIGHT * 0.5f));

					m_pLine[nCntLine] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
					m_pLine[nCntLine]->Init();
					m_pLine[nCntLine]->SetPosSize(
						D3DXVECTOR3((SCREEN_WIDTH * 0.5f), (SCREEN_HEIGHT * 0.5f), 0.0f),
						size);
					m_pLine[nCntLine]->SetColor(&D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
				}
			}
		}
	}

	for (int nCntDown = 0; nCntDown < COUNT_DOWN; nCntDown++)
	{// ダウンカウント
		if (m_pCountDown[nCntDown] == NULL)
		{// NULL
			m_pCountDown[nCntDown] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
			m_pCountDown[nCntDown]->Init();
			m_pCountDown[nCntDown]->SetPosSize(
				D3DXVECTOR3(
					((SCREEN_WIDTH * 0.5f) - (COUNTDOWN_SIZE * 2.1f)) + ((COUNTDOWN_SIZE * 2.1f) * nCntDown),
					(SCREEN_HEIGHT * 0.5f),
					0.0f), 
				D3DXVECTOR2(
					COUNTDOWN_SIZE,
					COUNTDOWN_SIZE));
			m_pCountDown[nCntDown]->BindTexture(m_pTexture[(TEXTURE)(TEXTURE_COUNT)]);
			m_pCountDown[nCntDown]->SetTexture(nCntDown, 3, 1, 1);
			m_pCountDown[nCntDown]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
	}

	for (int nCntPlayer = 0; nCntPlayer < nMaxPlayer; nCntPlayer++)
	{// プレイヤーカウント
		// ランキング
		if (m_pRanking[nCntPlayer] == NULL)
		{// NULL
			m_pRanking[nCntPlayer] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
			m_pRanking[nCntPlayer]->Init();
			// 各ビューポートの中心点から計算（中心点 -> 指定の位置へ）
			if (bOnine == true)
			{// オンライン
				m_pRanking[nCntPlayer]->SetPosSize(
					D3DXVECTOR3(
						(SCREEN_WIDTH * 0.5f) + (((SCREEN_WIDTH * 0.5f) - (SCREEN_HEIGHT * RANKING_SIZE_1P_X)) * -1.0f),
						(SCREEN_HEIGHT * 0.5f) + ((SCREEN_HEIGHT * 0.5f) - (SCREEN_HEIGHT * RANKING_SIZE_1P_Y)),
						0.0f),
					D3DXVECTOR2(
						SCREEN_HEIGHT * RANKING_SIZE_1P_X,
						SCREEN_HEIGHT * RANKING_SIZE_1P_Y * 0.8f));
			}
			else
			{// オンラインじゃない
				m_pRanking[nCntPlayer]->SetPosSize(
					//D3DXVECTOR3(
					//	(SCREEN_WIDTH * ((nMaxPlayer - 1) == 0 ? 0.5f : (nCntPlayer % 2 == 0 ? 0.25f : 0.75f)))			+ (((SCREEN_WIDTH * ((nMaxPlayer - 1) == 0 ? 0.5f : 0.25f))		- 
					//	(SCREEN_HEIGHT * ((nMaxPlayer - 1) / 2 == 0 ? RANKING_SIZE_2P : RANKING_SIZE_4P))) * (nCntPlayer % 2 == 0 ? -1.0f : 1.0f)),
					//	(SCREEN_HEIGHT * ((nMaxPlayer - 1) / 2 == 0 ? 0.5f : (nCntPlayer / 2 == 0 ? 0.25f : 0.75f)))	+ ((SCREEN_HEIGHT * ((nMaxPlayer - 1) / 2 == 0 ? 0.5f : 0.25f)) - 
					//	(SCREEN_HEIGHT * ((nMaxPlayer - 1) / 2 == 0 ? RANKING_SIZE_2P : RANKING_SIZE_4P))),
					//	0.0f),
					//D3DXVECTOR2(
					//	SCREEN_HEIGHT * ((nMaxPlayer - 1) / 2 == 0 ? RANKING_SIZE_2P : RANKING_SIZE_4P),
					//	SCREEN_HEIGHT * ((nMaxPlayer - 1) / 2 == 0 ? RANKING_SIZE_2P : RANKING_SIZE_4P)));
					D3DXVECTOR3(
					(SCREEN_WIDTH * ((nMaxPlayer - 1) / 2 == 0 ? 0.5f : (nCntPlayer % 2 == 0 ? 0.25f : 0.75f))) + (((SCREEN_WIDTH * ((nMaxPlayer - 1) / 2 == 0 ? 0.5f : 0.25f)) -
						(SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? RANKING_SIZE_1P_X : RANKING_SIZE_4P_X))) * ((nMaxPlayer - 1) / 2 == 0 ? -1.0f : (nCntPlayer % 2 == 0 ? -1.0f : 1.0f))),
						(SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? 0.5f : ((nMaxPlayer - 1) / 2 == 0 ? (nCntPlayer % 2 == 0 ? 0.25f : 0.75f) : (nCntPlayer / 2 == 0 ? 0.25f : 0.75f)))) + ((SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? 0.5f : 0.25f)) -
						(SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? RANKING_SIZE_1P_Y : RANKING_SIZE_4P_Y))),
						0.0f),
					D3DXVECTOR2(
						SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? RANKING_SIZE_1P_X : RANKING_SIZE_4P_X),
						SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? RANKING_SIZE_1P_Y * 0.8f : RANKING_SIZE_4P_Y * 0.8f)));
			}
			m_pRanking[nCntPlayer]->BindTexture(m_pTexture[TEXTURE_RANK]);
			m_pRanking[nCntPlayer]->SetTexture(nCntPlayer, 8, 1, 1);

			if (bOnine == true && nClient != nCntPlayer)
				m_pRanking[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}

		// アイテム
		for (int nCntItem = 0; nCntItem < MAX_EGG; nCntItem++)
		{// アイテムカウント
			// アイテムフレーム
			if (m_pItemFrame[nCntPlayer][nCntItem] == NULL)
			{// NULL
				m_pItemFrame[nCntPlayer][nCntItem] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
				m_pItemFrame[nCntPlayer][nCntItem]->Init();
				// 各ビューポートの中心点から計算（中心点 -> 指定の位置へ）
				if (bOnine == true)
				{// オンライン
					m_pItemFrame[nCntPlayer][nCntItem]->SetPosSize(
						D3DXVECTOR3(
						(SCREEN_WIDTH * 0.5f) + (((SCREEN_WIDTH * 0.5f) - (SCREEN_HEIGHT * ITEM_SIZE_1P)) * -1.0f),
							(SCREEN_HEIGHT * 0.5f) - ((SCREEN_HEIGHT * 0.5f) - (SCREEN_HEIGHT * ITEM_SIZE_1P)) + (((SCREEN_HEIGHT * ITEM_SIZE_1P) * 2.1f) * nCntItem),
							0.0f),
						D3DXVECTOR2(
							SCREEN_HEIGHT * ITEM_SIZE_1P,
							SCREEN_HEIGHT * ITEM_SIZE_1P));
					m_pItemFrame[nCntPlayer][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
				}
				else
				{// オンラインじゃない
					m_pItemFrame[nCntPlayer][nCntItem]->SetPosSize(
						//D3DXVECTOR3(
						//	(SCREEN_WIDTH * ((nMaxPlayer - 1) == 0 ? 0.5f : (nCntPlayer % 2 == 0 ? 0.25f : 0.75f)))			+ (((SCREEN_WIDTH * ((nMaxPlayer - 1) == 0 ? 0.5f : 0.25f))		- 
						//	(SCREEN_HEIGHT * ((nMaxPlayer - 1) / 2 == 0 ? ITEM_SIZE_2P : ITEM_SIZE_4P))) * (nCntPlayer % 2 == 0 ? -1.0f : 1.0f)),
						//	(SCREEN_HEIGHT * ((nMaxPlayer - 1) / 2 == 0 ? 0.5f : (nCntPlayer / 2 == 0 ? 0.25f : 0.75f)))	- ((SCREEN_HEIGHT * ((nMaxPlayer - 1) / 2 == 0 ? 0.5f : 0.25f)) - 
						//	(SCREEN_HEIGHT * ((nMaxPlayer - 1) / 2 == 0 ? ITEM_SIZE_2P : ITEM_SIZE_4P))) +
						//	(((SCREEN_HEIGHT * ((nMaxPlayer - 1) / 2 == 0 ? ITEM_SIZE_2P : ITEM_SIZE_4P)) * 2.1f) * nCntItem),
						//	0.0f),
						//D3DXVECTOR2(
						//	SCREEN_HEIGHT * ((nMaxPlayer - 1) / 2 == 0 ? ITEM_SIZE_2P : ITEM_SIZE_4P),
						//	SCREEN_HEIGHT * ((nMaxPlayer - 1) / 2 == 0 ? ITEM_SIZE_2P : ITEM_SIZE_4P)));
						D3DXVECTOR3(
						(SCREEN_WIDTH * ((nMaxPlayer - 1) / 2 == 0 ? 0.5f : (nCntPlayer % 2 == 0 ? 0.25f : 0.75f))) + (((SCREEN_WIDTH * ((nMaxPlayer - 1) / 2 == 0 ? 0.5f : 0.25f)) -
							(SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? ITEM_SIZE_1P : ITEM_SIZE_4P))) * ((nMaxPlayer - 1) / 2 == 0 ? -1.0f : (nCntPlayer % 2 == 0 ? -1.0f : 1.0f))),
							(SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? 0.5f : ((nMaxPlayer - 1) / 2 == 0 ? (nCntPlayer % 2 == 0 ? 0.25f : 0.75f) : (nCntPlayer / 2 == 0 ? 0.25f : 0.75f)))) - ((SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? 0.5f : 0.25f)) -
							(SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? ITEM_SIZE_1P : ITEM_SIZE_4P))) +
								(((SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? ITEM_SIZE_1P : ITEM_SIZE_4P)) * 2.1f) * nCntItem),
							0.0f),
						D3DXVECTOR2(
							SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? ITEM_SIZE_1P : ITEM_SIZE_4P),
							SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? ITEM_SIZE_1P : ITEM_SIZE_4P)));
					m_pItemFrame[nCntPlayer][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
				}

				if (bOnine == true && nClient != nCntPlayer)
					m_pItemFrame[nCntPlayer][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			}
			// アイテム
			if (m_pItem[nCntPlayer][nCntItem] == NULL)
			{// NULL
				m_pItem[nCntPlayer][nCntItem] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
				m_pItem[nCntPlayer][nCntItem]->Init();
				// 各ビューポートの中心点から計算（中心点 -> 指定の位置へ）
				if (bOnine == true)
				{// オンライン
					m_pItem[nCntPlayer][nCntItem]->SetPosSize(
						D3DXVECTOR3(
							(SCREEN_WIDTH * 0.5f) + (((SCREEN_WIDTH * 0.5f) - (SCREEN_HEIGHT * ITEM_SIZE_1P)) * -1.0f),
							(SCREEN_HEIGHT * 0.5f) - ((SCREEN_HEIGHT * 0.5f) - (SCREEN_HEIGHT * ITEM_SIZE_1P)) + (((SCREEN_HEIGHT * ITEM_SIZE_1P) * 2.1f) * nCntItem),
							0.0f),
						D3DXVECTOR2(
							SCREEN_HEIGHT * ITEM_SIZE_1P,
							SCREEN_HEIGHT * ITEM_SIZE_1P));
					m_pItem[nCntPlayer][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
				}
				else
				{// オンラインじゃない
					m_pItem[nCntPlayer][nCntItem]->SetPosSize(
						//D3DXVECTOR3(
						//	(SCREEN_WIDTH * ((nMaxPlayer - 1) == 0 ? 0.5f : (nCntPlayer % 2 == 0 ? 0.25f : 0.75f)))			+ (((SCREEN_WIDTH * ((nMaxPlayer - 1) == 0 ? 0.5f : 0.25f))		- 
						//	(SCREEN_HEIGHT * ((nMaxPlayer - 1) / 2 == 0 ? ITEM_SIZE_2P : ITEM_SIZE_4P))) * (nCntPlayer % 2 == 0 ? -1.0f : 1.0f)),
						//	(SCREEN_HEIGHT * ((nMaxPlayer - 1) / 2 == 0 ? 0.5f : (nCntPlayer / 2 == 0 ? 0.25f : 0.75f)))	- ((SCREEN_HEIGHT * ((nMaxPlayer - 1) / 2 == 0 ? 0.5f : 0.25f)) - 
						//	(SCREEN_HEIGHT * ((nMaxPlayer - 1) / 2 == 0 ? ITEM_SIZE_2P : ITEM_SIZE_4P))) +
						//	(((SCREEN_HEIGHT * ((nMaxPlayer - 1) / 2 == 0 ? ITEM_SIZE_2P : ITEM_SIZE_4P)) * 2.1f) * nCntItem),
						//	0.0f),
						//D3DXVECTOR2(
						//	SCREEN_HEIGHT * ((nMaxPlayer - 1) / 2 == 0 ? ITEM_SIZE_2P : ITEM_SIZE_4P),
						//	SCREEN_HEIGHT * ((nMaxPlayer - 1) / 2 == 0 ? ITEM_SIZE_2P : ITEM_SIZE_4P)));
						D3DXVECTOR3(
							(SCREEN_WIDTH * ((nMaxPlayer - 1) / 2 == 0 ? 0.5f : (nCntPlayer % 2 == 0 ? 0.25f : 0.75f))) + (((SCREEN_WIDTH * ((nMaxPlayer - 1) / 2 == 0 ? 0.5f : 0.25f)) -
							(SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? ITEM_SIZE_1P : ITEM_SIZE_4P))) * ((nMaxPlayer - 1) / 2 == 0 ? -1.0f : (nCntPlayer % 2 == 0 ? -1.0f : 1.0f))),
							(SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? 0.5f : ((nMaxPlayer - 1) / 2 == 0 ? (nCntPlayer % 2 == 0 ? 0.25f : 0.75f) : (nCntPlayer / 2 == 0 ? 0.25f : 0.75f)))) - ((SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? 0.5f : 0.25f)) -
							(SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? ITEM_SIZE_1P : ITEM_SIZE_4P))) +
								(((SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? ITEM_SIZE_1P : ITEM_SIZE_4P)) * 2.1f) * nCntItem),
							0.0f),
						D3DXVECTOR2(
							SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? ITEM_SIZE_1P : ITEM_SIZE_4P),
							SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? ITEM_SIZE_1P : ITEM_SIZE_4P)));
					m_pItem[nCntPlayer][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
				}
				m_pItem[nCntPlayer][nCntItem]->BindTexture(m_pTexture[TEXTURE_ITEM]);

				if (bOnine == true && nClient != nCntPlayer)
					m_pItem[nCntPlayer][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			}
		}

		if (m_pGoul[nCntPlayer] == NULL)
		{// NULL以外
			m_pGoul[nCntPlayer] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
			m_pGoul[nCntPlayer]->Init();
			if (bOnine == true)
			{// オンライン
				m_pGoul[nCntPlayer]->SetPosSize(
					D3DXVECTOR3(
						(SCREEN_WIDTH * 0.5f),
						(SCREEN_HEIGHT * 0.5f),
						0.0f),
						GOUL_SIZE_2P);
			}
			else
			{// オンラインじゃない
				m_pGoul[nCntPlayer]->SetPosSize(
					D3DXVECTOR3(
						(SCREEN_WIDTH * ((nMaxPlayer - 1) / 2 == 0 ? 0.5f : (nCntPlayer % 2 == 0 ? 0.25f : 0.75f))),
						(SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? 0.5f : ((nMaxPlayer - 1) / 2 == 0 ? (nCntPlayer % 2 == 0 ? 0.25f : 0.75f) : (nCntPlayer / 2 == 0 ? 0.25f : 0.75f)))),
						0.0f),
						((nMaxPlayer - 1) / 2 == 0 ? GOUL_SIZE_2P : GOUL_SIZE_4P));
			}
			m_pGoul[nCntPlayer]->BindTexture(m_pTexture[TEXTURE_GOUL]);
			//m_pGoul[nCntPlayer]->SetTexture(nCntDown, 3, 1, 1);
			m_pGoul[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
	}

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CGamePlay::Uninit(void)
{
	for (int nCntLine = 0; nCntLine < MAX_LINE; nCntLine++)
	{// 線カウント
		if (m_pLine[nCntLine] != NULL)
		{// NULL以外
			m_pLine[nCntLine]->Uninit();
			m_pLine[nCntLine] = NULL;
		}
	}

	for (int nCntDown = 0; nCntDown < COUNT_DOWN; nCntDown++)
	{// ダウンカウント
		if (m_pCountDown[nCntDown] != NULL)
		{// NULL以外
			m_pCountDown[nCntDown]->Uninit();
			m_pCountDown[nCntDown] = NULL;
		}
	}

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// プレイヤーカウント
		if (m_pRanking[nCntPlayer] != NULL)
		{// NULL以外
			m_pRanking[nCntPlayer]->Uninit();
			m_pRanking[nCntPlayer] = NULL;
		}

		for (int nCntItem = 0; nCntItem < MAX_EGG; nCntItem++)
		{// アイテムカウント
			if (m_pItemFrame[nCntPlayer][nCntItem] != NULL)
			{// NULL以外
				m_pItemFrame[nCntPlayer][nCntItem]->Uninit();
				m_pItemFrame[nCntPlayer][nCntItem] = NULL;
			}
			if (m_pItem[nCntPlayer][nCntItem] != NULL)
			{// NULL以外
				m_pItem[nCntPlayer][nCntItem]->Uninit();
				m_pItem[nCntPlayer][nCntItem] = NULL;
			}
		}

		if (m_pGoul[nCntPlayer] != NULL)
		{// NULL以外
			m_pGoul[nCntPlayer]->Uninit();
			m_pGoul[nCntPlayer] = NULL;
		}
	}

	//自身の削除
	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CGamePlay::Update(void)
{
	// プレイヤー
	CPlayer **pPlayer = CGame::GetPlayer();

	// 取得
	bool bOnine = CTitle::GetOnline();
	int nClient = 0;

	if (bOnine == true)
		nClient = CClient::GetnID();

	// ランキング
	int *pnRanking = CGame::GetRanking();
	// ゲームカウンター
	int nGameCounter = CGame::GetGameCounter();
	int nCounter = nGameCounter - (START_SET_TIME - START_COUNT_TIME);
	bool *pbGoul = CGame::GetGoul();

	CSound *pSound = CManager::GetSound();

	// カウントダウン
	if ((START_SET_TIME - START_COUNT_TIME) <= nGameCounter && nGameCounter < START_SET_TIME)
	{
		if (m_pCountDown[(nCounter / 60)] != NULL)
		{// NULL以外
			m_pCountDown[(nCounter / 60)]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			if (nCounter / 60 == 0 && nCounter % 60 == 0)
			{
				//ゲーム開始のカウントダウン
				pSound->SetVolume(CSound::SOUND_LABEL_SE_STARTCOUNT, 1.0f);
				pSound->PlaySound(CSound::SOUND_LABEL_SE_STARTCOUNT);
			}
		}
	}
	else
	{
		for (int nCntDown = 0; nCntDown < COUNT_DOWN; nCntDown++)
		{// ダウンカウント
			if (m_pCountDown[nCntDown] != NULL)
			{// NULL以外
				m_pCountDown[nCntDown]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			}
		}
	}

	// ランキング
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// プレイヤーカウント
		if (m_pRanking[nCntPlayer] != NULL)
		{// NULL以外
			m_pRanking[nCntPlayer]->SetTexture(pnRanking[nCntPlayer], 8, 1, 1);
		}

		if (pbGoul[nCntPlayer] == true)
		{
			if (m_pGoul[nCntPlayer] != NULL)
			{// NULL以外
				if (bOnine == true)
				{// オンライン
					if (nClient == nCntPlayer)
						m_pGoul[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
				else
				{// オンラインじゃない
					m_pGoul[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
			}
		}
	}

	int nPlayerNum = 0;
	for (int nCntMember= 0; nCntMember < MAX_MEMBER; nCntMember++)
	{// プレイヤーカウント
		if (pPlayer[nCntMember] != NULL)
		{// NULL以外
			if (pPlayer[nCntMember]->GetPlayerType() == CPlayer::PLAYERTYPE_PLAYER)
			{
				for (int nCntItem =0; nCntItem < MAX_EGG; nCntItem++)
				{// アイテムカウント
					// アイテム
					if (m_pItem[nPlayerNum][nCntItem] != NULL)
					{// NULL以外
						if ((bOnine == true && nClient == nPlayerNum) || bOnine == false)
						{// オンラインで自分なら、オンラインじゃない
							int nNum = pPlayer[nCntMember]->GetItemNum();
							if (nCntItem < nNum)
							{
								int nType = pPlayer[nCntMember]->GetItemType(nCntItem);
								switch (nType)
								{
								case CPlayer::BULLET_EGG_ATTACK:
									m_pItem[nPlayerNum][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
									break;
								case CPlayer::BULLET_EGG_ANNOY:
									m_pItem[nPlayerNum][nCntItem]->SetColor(&D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
									break;
								case CPlayer::BULLET_EGG_SPEED:
									m_pItem[nPlayerNum][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
									break;
								case CPlayer::BULLET_CHICK_ATTACK:
									m_pItem[nPlayerNum][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.3f));
									break;
								case CPlayer::BULLET_CHICK_ANNOY:
									m_pItem[nPlayerNum][nCntItem]->SetColor(&D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.3f));
									break;
								case CPlayer::BULLET_CHICK_SPEED:
									m_pItem[nPlayerNum][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.3f));
									break;
								}
							}
							else
							{
								m_pItem[nPlayerNum][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
							}
						}
						else
						{
							m_pItem[nPlayerNum][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
						}
					}
				}
				nPlayerNum++;
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CGamePlay::Draw(void)
{

}