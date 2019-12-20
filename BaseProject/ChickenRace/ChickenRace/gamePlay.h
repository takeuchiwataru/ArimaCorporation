//=============================================================================
//
// ゲーム（プレイ）の処理 [game.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _GAMEPLAY_H_
#define _GAMEPLAY_H_

#include "main.h"
#include "game.h"
#include "scene.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_LINE	(2)
#define COUNT_DOWN	(3)

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;
class CTime;

//=====================
// 基本クラス
//=====================
class CGamePlay : public CScene
{// ゲーム（プレイ）
public:
	typedef enum
	{
		TEXTURE_COUNT = 0,
		TEXTURE_GO,
		TEXTURE_RANK,
		TEXTURE_ITEMFRAME,
		TEXTURE_ITEM,
		TEXTURE_ITEMCLOSE,
		TEXTURE_GOUL,
		TEXTURE_VIEW,
		TEXTURE_CAMERAHINT,
		TEXTURE_MAX
	}TEXTURE;

	CGamePlay();									//コンストラクタ
	~CGamePlay();									//デストラクタ

	static HRESULT Load(void);
	static void Unload(void);
	static CGamePlay *Create(void);

	HRESULT Init();									//初期化処理
	void Uninit(void);								//終了処理
	void Update(void);								//更新処理
	void Draw(void);								//描画処理

private:
	static LPDIRECT3DTEXTURE9	m_pTexture[TEXTURE_MAX];	// テクスチャへのポインタ

	CScene2D *m_pFade[MAX_PLAYER];					// フェード
	CScene2D *m_pLine[MAX_LINE];					// 線
	CScene2D *m_pCountDown[COUNT_DOWN];				// カウントダウン
	CScene2D *m_pRanking[MAX_PLAYER];				// ランキング
	CScene2D *m_pItemFrame[MAX_PLAYER][MAX_EGG];	// アイテム
	CScene2D *m_pItem[MAX_PLAYER][MAX_EGG];			// アイテム
	CScene2D *m_pItemClose[MAX_PLAYER][MAX_EGG];	// アイテム×
	int		 m_nCloseCounter[MAX_PLAYER];			// ×カウント
	CScene2D *m_pGoul[MAX_PLAYER];					// ゴール
	CTime	*m_pTime[MAX_PLAYER];					// タイム
	CScene2D *m_pView[MAX_PLAYER];					// 観戦中
	CScene2D *m_pCameraHint[MAX_PLAYER];			// カメラ説明

	int		m_nGoulCounter[MAX_PLAYER];				// ゴールカウント
	int		m_nCameraNumber[MAX_PLAYER];			// カメラ番号
};
#endif