//=============================================================================
//
// フェードの処理 [fade.h]
// Author : 有馬武志
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "scene2D.h"
#include "main.h"
#include "game.h"
#include "manager.h"

//*****************************************************************************
// マクロ定義　免許書
//*****************************************************************************
#define	MAX_TYPE	(10)

//=====================
//  CScene2Dの派生クラス
//=====================
class CFade : public CScene2D
{
public://誰でも扱える
	typedef enum
	{
		FADE_NONE = 0,		//何もしていない状態
		FADE_IN,			//フェードイン状態
		FADE_OUT,			//フェードアウト状態
		FADE_MAX			//状態の最大数
	} FADE;

	typedef enum
	{
		MODEFADE_NONE = 0,	//何もしていない状態
		MODEFADE_NOLMAL,	//通常状態
		MODEFADE_GAME,		//ゲームのフェード
		MODEFADE_MODE,		//モードのフェード
		MODEFADE_MAX		//フェードの最大数
	} MODEFADE;

	CFade();
	~CFade();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static FADE GetFade(void) { return m_fade; }
	static CFade *Create(CManager::MODE modeNext);
	static CFade *Create(CGame::GAMEMODE modeNext);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_TEXTURE];	//共有テクスチャのポインタ
	static FADE				  m_fade;					//フェード状態
	static int				  m_nModeNext;				//次のモード
	static MODEFADE			  m_modegame;				//フェードのモード
	D3DXCOLOR				  m_colorFade;				//フェード色
	D3DXVECTOR2				  m_Size;					//サイズ
	static CGame			  *m_pGame;
	CScene2D *				  m_pNowLoading;			//ローディング
	CScene2D *				  m_pAnim;					//アニメーション
	int						  m_nAnimNum;				//アニメーションの番号
	int						  m_nCountAnim;				//アニメーションカウンター
};
#endif