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
#define MAX_HINT	(2)

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
		TEXTURE_BUTTON,
		TEXTURE_YOU,
		TEXTURE_READY,
		TEXTURE_TUTORIAL,
		TEXTURE_HINT,
		TEXTURE_RETURN_BG,
		TEXTURE_RETURN_TREE,
		TEXTURE_RETURN_HOME,
		TEXTURE_RETURN_CHAR,
		TEXTURE_RETURN_GRASS,
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
	void Online(void);
	void Local(void);
	void Tutorial(void);

	static LPDIRECT3DTEXTURE9	m_pTexture[TEXTURE_MAX];	// テクスチャへのポインタ

	CScene2D *m_pPlayerNum[MAX_PLAYER];		// プレイヤー番号
	CScene2D *m_pPlayerBG[MAX_PLAYER];		// プレイヤー背景
	CScene2D *m_pSelect[MAX_PLAYER];		// 選択
	CScene2D *m_pEnter[MAX_PLAYER];			// 決定
	CScene2D *m_pButton[MAX_PLAYER];		// ボタン

	CScene2D *m_pYor;						// あなた
	CScene2D *m_pReturnFrame;				// 戻るフレーム
	CScene2D *m_pReturnButton;				// 戻る
	CScene2D *m_pReturnBG;					// 戻る背景
	CScene2D *m_pReturnTree;				// 戻る木
	CScene2D *m_pReturnHome;				// 戻る小屋
	CScene2D *m_pReturnChar;				// 戻るキャラ
	CScene2D *m_pReturnGrass;				// 戻る草
	int		m_nReturnCounter;				// 戻るカウント

	CScene2D *m_pReady;						// 準備
	bool	m_bReady;						// 準備

	CScene2D *m_pTutorial;					// チュートリアル
	CScene2D *m_pHint[MAX_HINT];			// ヒント
	CScene2D *m_pGo;						// Go

	int		m_nTutorialNum;					// チュートリアル番号
	int		m_nTutorialCounter;				// チュートリアルカウント

	bool	m_bEntry[MAX_PLAYER];			// エントリー
	bool	m_bEnter[MAX_PLAYER];			// 決定したか

	int		m_nEntryCounter;				// エントリーカウント

	CScene2D *m_pCharacter[MAX_CHARCTER];
};
#endif