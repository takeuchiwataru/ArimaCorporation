//=============================================================================
//
// タイトル（メニュー）の処理 [titlemenu.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _TITLEMENU_H_
#define _TITLEMENU_H_

#include "main.h"
#include "game.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_MAINMENU	(3)
#define MAX_SUBMENU		(3)

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;

//=====================
// 基本クラス
//=====================
class CTitleMenu : public CScene
{// タイトル（メニュー）
public:
	typedef enum
	{
		TEXTURE_PRESS = 0,
		TEXTURE_MAINMENU,
		TEXTURE_MAX
	}TEXTURE;

	CTitleMenu();							//コンストラクタ
	~CTitleMenu();							//デストラクタ

	static HRESULT Load(void);
	static void Unload(void);
	static CTitleMenu *Create(void);

	HRESULT Init();							//初期化処理
	void Uninit(void);						//終了処理
	void Update(void);						//更新処理
	void Draw(void);						//描画処理

private:
	void ControllTitle(void);
	void ControllMainMenu(void);
	void ControllSubMenu(void);
	void ControllEntry(void);

	void EditPress(void);
	void EditMainMenu(void);
	void EditSubMenu(void);


	static LPDIRECT3DTEXTURE9	m_pTexture[TEXTURE_MAX];	// テクスチャへのポインタ

	CScene2D			*m_pPress;			// プレス
	int					m_nCntPress;		// プレスカウント

	CScene2D			*m_pMainMenu[MAX_MAINMENU];		// メインメニュー
	int					m_nCntMainMenu;					// メインメニューカウント

	int					m_nSelectNum;		// 選択番号

	bool				m_bSubMenu;			// サブメニュー

	bool				m_bEntry;			// 受付中
};
#endif