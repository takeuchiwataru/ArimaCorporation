//=============================================================================
//
// 車の動くポイントの処理 [car_point.h]
// Author : YUTARO　ABE
//
//=============================================================================
#ifndef _CAR_POINT_H_
#define _CAR_POINT_H_

#include "model3D.h"
#include "camerabace.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_CARPOINT						(1)			// 車の動くポイントのモデル数
#define MAX_CARPOINT_TEXTURE				(1)			// 車の動くポイントのテクスチャ数
#define CARPOINT_PRIOTITY					(2)

//=====================
//  CModel3Dの派生クラス
//=====================
class CCarPoint : public CModel3D
{
public://誰でも扱える
	CCarPoint();
	~CCarPoint();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CCarPoint *Create(int nNum,D3DXVECTOR3 pos);

	int GetObjNumber(void) { return m_nObjNumber; }
;
private://個人でのみ使う
	static LPD3DXMESH	m_pMeshModel[MAX_CARPOINT];					//メッシュ情報へのポインタ
	static LPD3DXBUFFER m_pBuffMatModel[MAX_CARPOINT];				//マテリアルの情報へのポインタ
	static DWORD		m_nNumMatModel[MAX_CARPOINT];				//マテリアルの情報数
	static LPDIRECT3DTEXTURE9 m_pMeshTextures[MAX_CARPOINT_TEXTURE];
	static D3DXVECTOR3 m_LoadVtxMaxModel[MAX_CARPOINT];				//モデルの最大値
	static D3DXVECTOR3 m_LoadVtxMinModel[MAX_CARPOINT];				//モデルの最小値
	int					m_nObjNumber;								// オブジェクトの番号
	bool				m_bLook;									// 見るかどうか
};
#endif