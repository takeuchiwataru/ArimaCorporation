//=============================================================================
//
// 背景の処理 [background.h]
// Author : shun yokomichi
//
//=============================================================================
#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#include "scene2D.h"
#include "texture.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
class CBackGround : public CScene2D
{
public://誰でも扱える
	CBackGround(int nPriorityv = 1);
	~CBackGround();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CBackGround *Create(CTexture::TEXTYPE type);

private:
	int							m_nStateType;				//状態を貰うときのタイプ
	int							m_nStateFont;				//状態の文字
	D3DXVECTOR2					m_size;						//サイズ
	D3DXVECTOR3					m_rot;						//向き
};



#endif