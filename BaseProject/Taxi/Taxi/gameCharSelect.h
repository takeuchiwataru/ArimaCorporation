//=============================================================================
//
// ゲーム（キャラ選択）の処理 [game.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _GAMECHARSELECT_H_
#define _GAMECHARSELECT_H_

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

//=====================
// 基本クラス
//=====================
class CGameCharSelect : public CScene
{// ゲーム（キャラ選択）
public:
	typedef enum
	{
		TEXTURE_CHAR = 0,
		TEXTURE_FRAME,
		TEXTURE_NUMBER,
		TEXTURE_ENTER,
		TEXTURE_MAX
	}TEXTURE;

	CGameCharSelect();						//コンストラクタ
	~CGameCharSelect();						//デストラクタ

	static HRESULT Load(void);
	static void Unload(void);
	static CGameCharSelect *Create(void);

	HRESULT Init();							//初期化処理
	void Uninit(void);						//終了処理
	void Update(void);						//更新処理
	void Draw(void);						//描画処理

private:
	static LPDIRECT3DTEXTURE9	m_pTexture[TEXTURE_MAX];	// テクスチャへのポインタ

	CScene2D *m_pPlayerNum[MAX_PLAYER];		// プレイヤー番号
	CScene2D *m_pPlayerBG[MAX_PLAYER];		// プレイヤー背景
	CScene2D *m_pSelect[MAX_PLAYER];		// 選択
	CScene2D *m_pEnter[MAX_PLAYER];			// 決定

	bool	m_bEntry[MAX_PLAYER];			// エントリー
	bool	m_bEnter[MAX_PLAYER];			// 決定したか

	int		m_nEntryCounter;				// エントリーカウント

	CScene2D *m_pCharacter[MAX_CHARCTER];	
};
#endif