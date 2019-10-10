//=============================================================================
//
// テールランプ処理 [taillamp.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _TAILLAMP_H_
#define _TAILLAMP_H_

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
class CTailLamp : public CScene
{
public:
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_BACK,			// バック
		TYPE_BREAKING,		// ブレーキ
		TYPE_HAZARD_LAMP,	// ハザードランプ
		TYPE_MAX
	}TYPE;

	CTailLamp();
	~CTailLamp();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CTailLamp* Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXMATRIX *pMtxParent);	// 生成処理

	D3DXMATRIX *GetMtxParent(void) { return m_pMtxParent; };								// 親のマトリックスの取得
	void SetMtxParent(D3DXMATRIX *pMtxParent) { m_pMtxParent = pMtxParent; };				// 親のマトリックスの設定

	D3DXCOLOR GetColor(void) { return m_col; };												// 色の取得
	void SetColor(D3DXCOLOR col);															// 色の設定

	TYPE GetType(void) { return m_type; };													// 種類の取得
	void SetType(TYPE type);																// 種類の設定

private:
	LPDIRECT3DTEXTURE9		m_pTexture;			// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// 頂点バッファへのポインタ
	D3DXMATRIX				m_mtxWorld;			// ワールドマトリックス
	D3DXMATRIX				*m_pMtxParent;		// 親のワールドマトリックス
	D3DXVECTOR3				m_pos;				// 位置
	D3DXVECTOR3				m_rot;				// 向き
	D3DXCOLOR				m_col;				// 色
	D3DXVECTOR2				m_size;				// 大きさ
	TYPE					m_type;				// ランプの種類
	int						m_nCntFlash;		// ランプの点滅用カウンタ
};

#endif