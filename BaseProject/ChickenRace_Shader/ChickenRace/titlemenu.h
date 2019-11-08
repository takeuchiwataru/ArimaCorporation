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
#define MAX_FLAG		(2)

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
		TEXTURE_LOGO = 0,
		TEXTURE_FLAG,
		TEXTURE_PRESS,
		TEXTURE_MENU,
		TEXTURE_NAME_M,
		TEXTURE_NAME_S,
		TEXTURE_INPUT,
		TEXTURE_ENTRY,
		TEXTURE_HINT,
		TEXTURE_NUM,
		TEXTURE_SEARCH,
		TEXTURE_MAX
	}TEXTURE;

	CTitleMenu();								//コンストラクタ
	~CTitleMenu();								//デストラクタ

	static HRESULT Load(void);
	static void Unload(void);
	static CTitleMenu *Create(void);

	HRESULT Init();								//初期化処理
	void Uninit(void);							//終了処理
	void Update(void);							//更新処理
	void Draw(void);							//描画処理

private:
	void ControllTitle(void);
	void ControllMainMenu(void);
	void ControllSubMenu(void);
	void ControllEntry(void);

	void EditLogo(void);
	void EditChicken(void);
	void EditFlag(void);
	void EditPress(void);
	void EditMenuBG(void);
	void EditMainMenu(void);
	void EditSubMenu(void);
	void EditEntry(void);

	static LPDIRECT3DTEXTURE9	m_pTexture[TEXTURE_MAX];	// テクスチャへのポインタ

	CScene2D			*m_pLogo;				// ロゴ
	CScene2D			*m_pChicken;			// チキン
	CScene2D			*m_pFlag[MAX_FLAG];		// 旗
	int					m_nCntLogo;				// ロゴカウント

	CScene2D			*m_pPress;				// プレス
	int					m_nCntPress;			// プレスカウント

	CScene2D			*m_pMenuBG;				// メニューBG
	
	CScene2D			*m_pMainMenu[MAX_MAINMENU];		// メインメニュー
	CScene2D			*m_pNameMenu[MAX_MAINMENU];		// メニュー名
	int					m_nCntMainMenu;					// メインメニューカウント

	int					m_nSelectNum;			// 選択番号

	bool				m_bSubMenu;				// サブメニュー

	CScene2D			*m_pEntry;				// 受付中
	CScene2D			*m_pHint;				// 説明
	CScene2D			*m_pMember[MAX_PLAYER];	// 人数
	CScene2D			*m_pEnter;				// 決定
	CScene2D			*m_pCancel;				// キャンセル
	CScene2D			*m_pSearch;				// サーチ
	bool				m_bEntry;				// 受付中
	int					m_nCntEntry;			// 受付中カウント
	bool				m_bSearch;				// サーチ
};
#endif