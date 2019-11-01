//=============================================================================
//
// ポーズ処理 [pause.h]
// Author : 有馬武志
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_PAUSE_TEXTURE	(4)

//=====================
//  CSceneの派生クラス
//=====================
class CPause : public CScene
{
public://誰でも扱える
	typedef enum
	{
		PAUSE_NORMAL = 0,		//選択していない状態
		PAUSE_SELECT,			//選択した状態
		PAUSE_MAX				//選択時の最大数
	}PAUSESTATE;

	typedef enum
	{
		CONTINU = 0,			//コンテニュー	【ゲームに戻る】
		RESULT,					//リザルト		【最初からやり直す】
		QUIT,					//クイット		【ゲームを終了】
		MAX_MEMU				//選択肢の最大数
	}MENU;

	static HRESULT Load(void);
	static void UnLoad(void);
	CPause(int nPriority = 7, CScene::OBJTYPE objType = CScene::OBJTYPE_PAUSE);
	~CPause();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CPause *Create();
	static bool &GetbPause() { return m_bPause; }
	static bool GetPauseEnd() { return m_bSerectEnd; }

private://個人でのみ使う
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_PAUSE_TEXTURE];//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;						//頂点バッファへのポインタ
	D3DXVECTOR3				m_pos;							//位置
	D3DXCOLOR				m_col;							//色
	float					m_SelectColor;					//選択しているときの色
	int						m_nCntState;					//カウントしている状態
	int						m_SelectNum;					//セレクトの数
	static bool				m_bSerectEnd;					//選び終わった状態
	MENU					m_menu;							//メニューの構造体
	PAUSESTATE				m_state;						//ポーズの状態
	D3DXVECTOR2				m_nSize;						//サイズ
	static bool				m_bPause;
};
#endif