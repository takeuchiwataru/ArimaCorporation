//=============================================================================
//
// ゲーム（プレイ）の処理 [game.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
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
#define RANKING_SIZE_1P		(0.13f)					// 1P時のランキングの大きさ
#define RANKING_SIZE_4P		(0.07f)					// 4P時のランキングの大きさ
#define ITEM_SIZE_1P		(0.11f)					// 1P時のアイテムの大きさ
#define ITEM_SIZE_4P		(0.06f)					// 4P時のアイテムの大きさ

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
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// プレイヤーカウント
		m_pRanking[nCntPlayer] = NULL;

		for (int nCntItem = 0; nCntItem < MAX_EGG; nCntItem++)
		{// アイテムカウント
			m_pItem[nCntPlayer][nCntItem] = NULL;
		}
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
		case TEXTURE_RANK:
			strcpy(cName, "data/TEXTURE/game/play/rank.png");
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
	// プレイヤー最大数取得
	int nMaxPlayer = CGame::GetMaxPlayer();

	for (int nCntPlayer = 0; nCntPlayer < nMaxPlayer; nCntPlayer++)
	{// プレイヤーカウント
		// ランキング
		if (m_pRanking[nCntPlayer] == NULL)
		{// NULL
			m_pRanking[nCntPlayer] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
			m_pRanking[nCntPlayer]->Init();
			// 各ビューポートの中心点から計算（中心点 -> 指定の位置へ）
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
					(SCREEN_WIDTH * ((nMaxPlayer - 1) / 2 == 0 ? 0.5f : (nCntPlayer % 2 == 0 ? 0.25f : 0.75f)))		+ (((SCREEN_WIDTH * ((nMaxPlayer - 1) / 2 == 0 ? 0.5f : 0.25f))	- 
					(SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? RANKING_SIZE_1P : RANKING_SIZE_4P))) * ((nMaxPlayer - 1) / 2 == 0 ? -1.0f : (nCntPlayer % 2 == 0 ? -1.0f : 1.0f))),
					(SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? 0.5f : ((nMaxPlayer - 1) / 2 == 0 ? (nCntPlayer % 2 == 0 ? 0.25f : 0.75f) : (nCntPlayer / 2 == 0 ? 0.25f : 0.75f))))	+ ((SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? 0.5f : 0.25f)) -
					(SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? RANKING_SIZE_1P : RANKING_SIZE_4P))),
					0.0f),
				D3DXVECTOR2(
					SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? RANKING_SIZE_1P : RANKING_SIZE_4P),
					SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? RANKING_SIZE_1P : RANKING_SIZE_4P)));
			m_pRanking[nCntPlayer]->BindTexture(m_pTexture[TEXTURE_RANK]);
			m_pRanking[nCntPlayer]->SetTexture(nCntPlayer, 8, 1, 1);
		}

		// アイテム
		for (int nCntItem = 0; nCntItem < MAX_EGG; nCntItem++)
		{// アイテムカウント
			if (m_pItem[nCntPlayer][nCntItem] == NULL)
			{// NULL
				m_pItem[nCntPlayer][nCntItem] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
				m_pItem[nCntPlayer][nCntItem]->Init();
				// 各ビューポートの中心点から計算（中心点 -> 指定の位置へ）
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
		}
	}

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CGamePlay::Uninit(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// プレイヤーカウント
		if (m_pRanking[nCntPlayer] != NULL)
		{// NULL以外
			m_pRanking[nCntPlayer]->Uninit();
			m_pRanking[nCntPlayer] = NULL;
		}

		for (int nCntItem = 0; nCntItem < MAX_EGG; nCntItem++)
		{// アイテムカウント
			if (m_pItem[nCntPlayer][nCntItem] != NULL)
			{// NULL以外
				m_pItem[nCntPlayer][nCntItem]->Uninit();
				m_pItem[nCntPlayer][nCntItem] = NULL;
			}
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
	int *pnRanking = CGame::GetRanking();

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// プレイヤーカウント
		if (m_pRanking[nCntPlayer] != NULL)
		{// NULL以外
			m_pRanking[nCntPlayer]->SetTexture(pnRanking[nCntPlayer], 8, 1, 1);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CGamePlay::Draw(void)
{

}