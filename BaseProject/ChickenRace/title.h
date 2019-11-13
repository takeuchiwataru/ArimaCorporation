//=============================================================================
//
// メイン処理 [title.h]
// Author : 有馬 武志
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CTitleCamera;

class CTitleMenu;

//=====================
// 基本クラス
//=====================
class CTitle
{
public:
	CTitle();								// コンストラクタ
	~CTitle();								// デストラクタ
	HRESULT Init();							// 初期化処理
	void	Uninit(void);					// 終了処理
	void	Update(void);					// 更新処理
	static void	Draw(void);					// 描画処理

	static int GetCounter(void) { return m_nTitleCounter; }

	// メニュー
	static void SetMenu(bool bMenu)		{ m_bMenu = bMenu; }
	static bool GetMenu(void)			{ return m_bMenu; }
	// オンライン
	static void SetOnline(bool bOnline) { m_bOnline = bOnline; }
	static bool GetOnline(void)			{ return m_bOnline; }
	// ホスト
	static void SetHost(bool bHost)		{ m_bHost = bHost; }
	static bool GetHost(void)			{ return m_bHost; }

private:
	int					m_nFrame;			// 季節のカウンター
	int					m_nCntTex;			// テクスチャのカウンター
	int					m_nCntCreate;		// 生成時間
	int					m_nLeafposX;		// 落ち葉の 生成座標
	int					m_nLeafsize;		// 落ち葉の サイズ
	static int			m_nTitleCounter;	// タイトルのカウンター
	bool				m_bOnOff;

	static CTitleCamera	*m_pTitleCamera;	//タイトルカメラのポインタ

	static bool			m_bMenu;			// メニュー
	static bool			m_bOnline;			// オンライン
	static bool			m_bHost;			// ホスト

	CTitleMenu			*m_pTitleMenu;		// タイトルメニュー
};
#endif