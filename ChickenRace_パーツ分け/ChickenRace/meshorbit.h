//=============================================================================
//
// 軌跡の処理 [meshorbit.h]
// Author : 有馬武志
//
//=============================================================================
#ifndef _MESHORBIT_H_
#define _MESHORBIT_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_MESHORBIT_TEXTURE		(1)				//テクスチャの最大数
#define MESHORBIT_X					(20)			//x軸方向のマス数
#define MESHORBIT_Z					(1)				//z軸方向のマス数
#define MESH_ORBITWIDTH				(50.0f)			//幅
#define MESH_ORBITDEPTH				(50.0f)			//奥行
#define MAX_OFFSET					(2)				//オフセットの最大数

//=====================
//  CSceneの派生クラス
//=====================
class CMeshOrbit : public CScene
{
public://誰でも扱える
	CMeshOrbit();
	~CMeshOrbit();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CMeshOrbit *Create();
	void BindTexture(LPDIRECT3DTEXTURE9 Texture);
	void SetPosition(D3DXVECTOR3 pos);
	void SetMatRarent(D3DXMATRIX *pMtx) { m_pMtxParent = pMtx; }
	void SetOrbitOn(bool bOrbitOnOff) { m_bOrbitOnOff = bOrbitOnOff; }

private://個人でのみ使う
	static LPDIRECT3DTEXTURE9	m_pTexture[MAX_MESHORBIT_TEXTURE];		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;									// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;									// インデックスへのポインタ
	D3DXVECTOR3				m_pos;										// 位置
	D3DXVECTOR3				m_rot;										// 向き
	D3DXMATRIX				m_mtxWorld, m_local;						// ワールドマトリックス
	D3DXMATRIX				*m_pMtxParent;								// 親のマトリックスのポインタ
	D3DXVECTOR3				m_aPosVertex[MESHORBIT_X];					// 計算した頂点座標
	D3DXVECTOR3				m_aOffSet[MAX_OFFSET];						// 両端のオフセット
	int						m_nType;									// 種類
	bool					m_bOrbitOnOff;								// 軌跡の描画

};
#endif