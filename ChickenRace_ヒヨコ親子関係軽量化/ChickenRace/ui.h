//=============================================================================
//
// UIの処理 [ui.h]
// Author : 有馬武志
//
//=============================================================================
#ifndef _UI_H_
#define _UI_H_

#include "main.h"
#include "scene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//=====================
//  CScene2Dの派生クラス
//=====================
class CUi : public CScene2D
{
public://誰でも扱える
	typedef enum
	{
		UI_TITLE_LOGO = 0,		//タイトルロゴ
		UI_PRESS_BUTTON,		//プレスボタン指示
		UI_MAX					//最大数
	}UI;

	CUi();
	~CUi();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CUi *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, CUi::UI m_Ui);
	void UiMovement(void);

private:
	static LPDIRECT3DTEXTURE9	m_pTexture[MAX_TEXTURE];	//共有テクスチャのポインタ
	static int					m_nNextType;				//状態を貰うときのタイプ
	static int					m_nNextFont;				//状態の文字
	static UI					m_Ui;						//UIの種類
	float						m_fAlpha;					//透明度
	int							m_nStateType;				//状態を貰うときのタイプ
	int							m_nStateFont;				//状態の文字
	int							m_nType;					//種類
	int							m_nTransformType;			//画像の形を変えれるタイプ
	D3DXVECTOR2					m_size;						//サイズ
	float						m_fLength;					//距離
	float						m_fAngle;					//角度
	D3DXVECTOR3					m_rot;						//向き
	D3DXVECTOR3					m_pos;						//位置
	int							m_nCntTutorial;
	bool						m_bOneCnt;
};
#endif