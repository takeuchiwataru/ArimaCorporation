//=============================================================================
//
// 壁処理 [wall.h]
// Author : 有馬武志
//
//=============================================================================
#ifndef _WALL_H_
#define _WALL_H_

#include "main.h"
#include "scene3D.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define WALL_PRIOTITY		(5)
#define	MAX_WALL_TEX		(2)

//=============================================================================
// クラス定義
//=============================================================================
class CWall : public CScene3D
{
public:
	CWall();
	~CWall();

	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CWall *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR3 rot, int nType);
	bool CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fDepth);
	void DeleteWall(void);

private:
	static LPDIRECT3DTEXTURE9	m_pTexture[MAX_WALL_TEX];	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;					// 頂点バッファへのポインタ
	D3DXMATRIX					m_mtxWorld;					// ワールドマトリックス
	D3DXVECTOR3					m_pos;						// 位置
	D3DXVECTOR2					m_size;						// 大きさ
	int							m_nType;					// 壁の方向
};
#endif
