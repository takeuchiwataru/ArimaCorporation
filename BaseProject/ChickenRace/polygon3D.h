//=============================================================================
//
// メイン処理 [scene3D.h]
// Author : 有馬武志
//
//=============================================================================
#ifndef _POLYGON3D_H_
#define _POLYGON3D_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//=====================
//  CSceneの派生クラス
//=====================
class CPolygon3D : public CScene
{
public://誰でも扱える
	CPolygon3D();
	~CPolygon3D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindTexture(LPDIRECT3DTEXTURE9 Texture);
	D3DXVECTOR3 GetPosition(void) { return m_Pos; }
	D3DXVECTOR3 GetMove(void) { return m_Move; }
	static D3DXVECTOR3 *GetNor(void) { return &m_nor[0]; }
	static CPolygon3D *Create();
	float GetHeight(D3DXVECTOR3 pos);
	void GetNor(D3DXVECTOR3 *nor);
	void SetMove(D3DXVECTOR3 m_move) { m_Move = m_move; }
	void SetPosition(D3DXVECTOR3 pos) { m_Pos = pos; }

private://個人でのみ使う
	LPDIRECT3DTEXTURE9		m_pTexture;						//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;						//頂点バッファへのポインタ
	D3DXVECTOR3				m_Pos;							//位置
	D3DXVECTOR3				m_Rot;							//向き
	D3DXVECTOR3				m_Move;							//動き
	static D3DXVECTOR3		m_nor[4];
	D3DXMATRIX				m_mtxWorldPolygon;				//ワールドマトリックス
	int						m_nType;						//種類
	int						m_nIdxShadow;
	bool					m_bUse;							//種類
};
#endif