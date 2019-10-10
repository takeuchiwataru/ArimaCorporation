//=============================================================================
//
// 警察のランプ処理 [policelanmp.h]
// Author : 有馬武志
//
//=============================================================================
#ifndef _POLICELAMP_H_
#define _POLICELAMP_H_

#include "main.h"
#include "scene3D.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define POLICELAMP_PRIOTITY		(5)
#define	MAX_POLICELAMP_TEX		(1)

//=============================================================================
// クラス定義
//=============================================================================
class CPoliceLamp : public CScene3D
{
public:
	CPoliceLamp();
	~CPoliceLamp();

	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CPoliceLamp *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);

private:
	static LPDIRECT3DTEXTURE9	m_pTexture[MAX_POLICELAMP_TEX];	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;					// 頂点バッファへのポインタ
	D3DXMATRIX					m_mtxWorld;					// ワールドマトリックス
	int							m_nType;					// 壁の方向
};
#endif
