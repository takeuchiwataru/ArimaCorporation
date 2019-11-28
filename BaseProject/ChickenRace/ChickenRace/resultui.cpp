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
		m_pRank[nCntMember] = NULL;			// ランク
		m_pChar[nCntMember] = NULL;			// キャラ
		m_pPlayer[nCntMember] = NULL;		// プレイヤー
		m_pTime[nCntMember] = NULL;			// タイム
	}
	m_pPress = NULL;						// プレス
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
			strcpy(cName, "data/TEXTURE/game/charselect/char.png");
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
	int nMaxPlayer = CGame::GetMaxPlayer();
	int *pnRankingSort = CGame::GetRankingSort();
	int *nTimer = CGame::GetTimer();

	for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
	{// メンバーカウント
		// ランク
		if (m_pRank[nCntMember] == NULL)
		{// NULL
			m_pRank[nCntMember] = new CScene2D(6, CScene::OBJTYPE_2DPOLYGON);
			m_pRank[nCntMember]->Init();
			m_pRank[nCntMember]->SetPosSize(
				D3DXVECTOR3
				(
				(SCREEN_WIDTH * 0.5f) - (SCREEN_WIDTH * 0.4f),
					(SCREEN_HEIGHT * 0.12f) + (((SCREEN_HEIGHT * 0.05f) * 2.2f) * nCntMember),
					0.0f
				),
				D3DXVECTOR2(SCREEN_HEIGHT * 0.08f, SCREEN_HEIGHT * 0.05f));
			m_pRank[nCntMember]->BindTexture(m_pTexture[TEXTURE_RANK]);
			m_pRank[nCntMember]->SetTexture(nCntMember, 8, 1, 1);
		}

		// キャラ
		if (m_pChar[nCntMember] == NULL)
		{// NULL
			m_pChar[nCntMember] = new CScene2D(6, CScene::OBJTYPE_2DPOLYGON);
			m_pChar[nCntMember]->Init();
			m_pChar[nCntMember]->SetPosSize(
				D3DXVECTOR3
				(
				(SCREEN_WIDTH * 0.5f) - (SCREEN_WIDTH * 0.3f),
					(SCREEN_HEIGHT * 0.12f) + (((SCREEN_HEIGHT * 0.05f) * 2.2f) * nCntMember),
					0.0f
				),
				D3DXVECTOR2(SCREEN_HEIGHT * 0.05f, SCREEN_HEIGHT * 0.05f));
			m_pChar[nCntMember]->BindTexture(m_pTexture[TEXTURE_CHAR]);
		}

		// プレイヤー
		if (m_pPlayer[nCntMember] == NULL)
		{// NULL
			m_pPlayer[nCntMember] = new CScene2D(6, CScene::OBJTYPE_2DPOLYGON);
			m_pPlayer[nCntMember]->Init();
			m_pPlayer[nCntMember]->SetPosSize(
				D3DXVECTOR3
				(
					(SCREEN_WIDTH * 0.5f) - (SCREEN_WIDTH * 0.21f),
					(SCREEN_HEIGHT * 0.12f) + (((SCREEN_HEIGHT * 0.05f) * 2.2f) * nCntMember),
					0.0f
				),
				D3DXVECTOR2(SCREEN_HEIGHT * 0.08f, SCREEN_HEIGHT * 0.05f));
			m_pPlayer[nCntMember]->BindTexture(m_pTexture[TEXTURE_ICON]);
			m_pPlayer[nCntMember]->SetTexture((pnRankingSort[nCntMember] < nMaxPlayer ? pnRankingSort[nCntMember] : 4), 5, 2, 1);
		}

		// タイム
		if (m_pTime[nCntMember] == NULL)
		{// NULL
			m_pTime[nCntMember] = CTime::Create(
				D3DXVECTOR3
				(
					(SCREEN_WIDTH * 0.5f) + (SCREEN_WIDTH * 0.08f),
					(SCREEN_HEIGHT * 0.12f) + (((SCREEN_HEIGHT * 0.05f) * 2.2f) * nCntMember),
					0.0f
				),
				D3DXVECTOR3((SCREEN_HEIGHT * 0.03f), (SCREEN_HEIGHT * 0.05f), 0.0f));
			m_pTime[nCntMember]->TexTime(nTimer[pnRankingSort[nCntMember]], true);

			if (pnRankingSort[nCntMember] < nMaxPlayer)
				m_pTime[nCntMember]->Setcol(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		}
	}

	// プレス
	if (m_pPress == NULL)
	{// NULL
		m_pPress = new CScene2D(6, CScene::OBJTYPE_2DPOLYGON);
		m_pPress->Init();
		m_pPress->SetPosSize(
			D3DXVECTOR3
			(
				(SCREEN_WIDTH * 0.5f) + (SCREEN_WIDTH * 0.35f),
				(SCREEN_HEIGHT * 0.12f) + (SCREEN_HEIGHT * 0.8f),
				0.0f
			),
			D3DXVECTOR2(SCREEN_HEIGHT * 0.2f, SCREEN_HEIGHT * 0.05f));
		m_pPress->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		m_pPress->BindTexture(m_pTexture[TEXTURE_PRESS]);
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

	//自身の削除
	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CResultUI::Update(void)
{
	int nCounter = CResult::GetCounter();

	if (RESULT_WAIT < nCounter)
	{
		nCounter -= RESULT_WAIT;

		// プレス
		if (m_pPress != NULL)
		{// NULL以外
			nCounter %= 120;
			int nNum = nCounter % 60;

			m_pPress->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, (nCounter < 60 ? (float)((float)nNum / (float)60) : 1.0f - (float)((float)nNum / (float)60))));
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CResultUI::Draw(void)
{

}