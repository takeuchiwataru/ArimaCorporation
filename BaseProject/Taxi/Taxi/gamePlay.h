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

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;

//=====================
// 基本クラス
//=====================
class CGamePlay : public CScene
{// ゲーム（プレイ）
public:
	typedef enum
	{
		TEXTURE_RANK = 0,
		TEXTURE_MAX
	}TEXTURE;

	CGamePlay();							//コンストラクタ
	~CGamePlay();							//デストラクタ

	static HRESULT Load(void);
	static void Unload(void);
	static CGamePlay *Create(void);

	HRESULT Init();							//初期化処理
	void Uninit(void);						//終了処理
	void Update(void);						//更新処理
	void Draw(void);						//描画処理

private:
	static LPDIRECT3DTEXTURE9	m_pTexture[TEXTURE_MAX];	// テクスチャへのポインタ

	CScene2D *m_pRanking[MAX_PLAYER];		// ランキング
	CScene2D *m_pItem[MAX_PLAYER][MAX_EGG];	// アイテム

};
#endif