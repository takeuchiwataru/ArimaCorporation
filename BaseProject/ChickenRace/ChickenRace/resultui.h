//=============================================================================
//
// リザルトUIの処理 [resultui.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _RESULTUI_H_
#define _RESULTUI_H_

#include "main.h"
#include "game.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;
class CTime;

//=====================
// 基本クラス
//=====================
class CResultUI : public CScene
{// リザルトUI
public:
	typedef enum
	{
		TEXTURE_RANK = 0,
		TEXTURE_CHAR,
		TEXTURE_ICON,
		TEXTURE_PRESS,
		TEXTURE_MAX
	}TEXTURE;

	CResultUI();								//コンストラクタ
	~CResultUI();								//デストラクタ

	static HRESULT Load(void);
	static void Unload(void);
	static CResultUI *Create(void);

	HRESULT Init();								//初期化処理
	void Uninit(void);							//終了処理
	void Update(void);							//更新処理
	void Draw(void);							//描画処理

private:
	void Title(void);
	void Result(void);

	static LPDIRECT3DTEXTURE9	m_pTexture[TEXTURE_MAX];	// テクスチャへのポインタ

	CScene2D			*m_pRankBG[MAX_MEMBER];	// ランクBG
	CScene2D			*m_pRank[MAX_MEMBER];	// ランク
	CScene2D			*m_pChar[MAX_MEMBER];	// キャラ
	CScene2D			*m_pPlayer[MAX_MEMBER];	// プレイヤー
	CTime				*m_pTime[MAX_MEMBER];	// タイム
	CScene2D			*m_pPress;				// プレス
	CScene2D			*m_pFade;				// フェード
};
#endif