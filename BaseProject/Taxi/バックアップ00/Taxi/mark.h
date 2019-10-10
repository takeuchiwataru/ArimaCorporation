//=============================================================================
//
// 客の頭上に出るマークの処理 [mark.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _MARK_H_
#define _MARK_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CMark : public CScene
{
public:
	CMark();
	~CMark();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMark* Create(D3DXVECTOR3 offset);	// 生成処理
	static void LoadModel(void);
	static void UnloadModel(void);

	D3DXCOLOR GetColor(void) { return m_col; };		// 色の取得
	void SetColor(D3DXCOLOR col);					// 色の設定
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }	// 位置の設定


private:
	void DrawModel(void);						//モデルの描画処理
	static LPD3DXMESH		m_pLoadMesh;		//メッシュ情報へのポインタ
	static LPD3DXBUFFER		m_pLoadBuffMat;		//マテリアルの情報へのポインタ
	static DWORD			m_nLoadNumMat;		//マテリアルの情報数

	LPDIRECT3DTEXTURE9		m_pTexture;			// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// 頂点バッファへのポインタ
	D3DXMATRIX				m_mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3				m_pos;				// 位置
	D3DXVECTOR3				m_rot;				// 向き
	D3DXCOLOR				m_col;				// 色
	D3DXVECTOR2				m_size;				// 大きさ
	float					m_fCntPos;			// 位置を変えるためのカウンタ
};

#endif