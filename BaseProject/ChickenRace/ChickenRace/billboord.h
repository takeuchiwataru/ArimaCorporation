//=============================================================================
//
// ビルボードの処理 [billboord.h]
// Author : 有馬武志
//
//=============================================================================
#ifndef _BILLBOORD_H_
#define _BILLBOORD_H_

#include "main.h"
#include "scene.h"
#include "scene3D.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//=====================
//  CSceneの派生クラス
//=====================
class CBillBoord : public CScene3D
{
public://誰でも扱える
	CBillBoord();
	~CBillBoord();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CBillBoord *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, bool bMap = false);

	bool GetMapOn(void) { return m_bMap; }

private://個人でのみ使う
	D3DXVECTOR3					m_pos;							// 位置
	D3DXVECTOR2					m_size;							// サイズ

	bool						m_bMap;							// マップ
};
#endif