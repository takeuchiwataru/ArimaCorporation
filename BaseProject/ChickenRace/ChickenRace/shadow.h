//=============================================================================
//
// 影の処理 [shadow.h]
// Author : 有馬武志
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"
#include "scene.h"
#include "scene3D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SHADOW_PRIOTITY		(3)

//=====================
//  CScene3Dの派生クラス
//=====================
class CShadow : public CScene3D
{
public://誰でも扱える
	CShadow();
	~CShadow();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CShadow *Create(D3DXVECTOR3 pos, float fWidth, float fDepth,int nTexType);
	void SetPositionShadow(D3DXVECTOR3 pos);
	float GetShadowHeight(void) { return m_fMeshHeight; }

private://個人でのみ使う
	static LPDIRECT3DTEXTURE9	m_pTexture[MAX_BILLBOORD_TEX];	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;						// 頂点バッファへのポインタ
	D3DXVECTOR3					m_pos;							// 位置
	float						m_fWidth;						// 横幅
	float						m_fDepth;						// 奥行	
	static float				m_fMeshHeight;					// 影からみたメッシュの高さ
	
};
#endif