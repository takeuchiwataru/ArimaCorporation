//=============================================================================
//
// メッシュシリンダー処理 [meshCylinder.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CMeshCylinder : public CScene
{
public://誰でも扱える
	CMeshCylinder();
	~CMeshCylinder();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMeshCylinder *Create(D3DXVECTOR3 pos, int nLap, int nStage, float fRadius, float fHeight, D3DXCOLOR col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

	void MakeVertex(int nLap, int nStage, float fRadius, float fHeight);	// 頂点情報の作成
	void ReMakeVertex(void);												// 頂点情報を作り直す

	void AlongField(bool bAlong, float fFloatHeight = 0.0f);	// 地面に沿わせるかどうか

	int GetNumVtx(void) { return m_nNumVtx; };					// 頂点数の取得
	int GetNumPolygon(void) { return m_nNumPolygon; };			// ポリゴン数の取得

	int GetLap(void) { return m_nLap; };						// 一周の個数の取得
	void SetLap(int nLap) { m_nLap = nLap; };					// 一周の個数の設定

	float GetRadius(void) { return m_fRadius; };				// 半径の取得
	void SetRadius(float fRadius) { m_fRadius = fRadius; };		// 半径の設定

	float GetHeight(void) { return m_fHeight; };				// 高さの取得
	void SetHeight(float fHeight) { m_fHeight = fHeight; };		// 高さの設定

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };			// 位置の設定


	D3DXCOLOR GetColor(void) { return m_col; };					// 色の取得
	void SetColor(D3DXCOLOR col);								// 色の設定

private://個人でのみ使う
	float GetHeightAlongField(D3DXVECTOR3 pos);		// 地面に沿った高さ

	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;			// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuff;			// インデックスへのポインタ
	D3DXMATRIX					m_mtxWorld;			// ワールドマトリックス
	int							m_nNumPolygon;		// ポリゴン数
	int							m_nNumVtx;			// 頂点数
	D3DXVECTOR3					m_pos;				// 位置
	int							m_nLap;				// 一周の個数
	int							m_nStage;			// 段数
	float						m_fRadius;			// 半径
	float						m_fHeight;			// 高さ
	D3DXCOLOR					m_col;				// 色
	bool						m_bAlong;			// 地面に沿わせるかどうか
	float						m_fFloatHeight;		// 地面から浮かす値
};
#endif