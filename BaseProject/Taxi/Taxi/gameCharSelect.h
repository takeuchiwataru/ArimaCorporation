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
	CScene2D *m_pPlayerBG[MAX_PLAYER];
	CScene2D *m_pCharacter[MAX_CHARCTER];

};
#endif