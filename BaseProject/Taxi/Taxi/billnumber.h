//=============================================================================
//
// ビルボード数字処理 [billnumber.h]
// Author : 有馬　武志
//
//=============================================================================
#ifndef _BILLNUMBER_H_
#define _BILLNUMBER_H_

#include "scene2D.h"

//=============================================================================
// クラス定義
//=============================================================================
class CBillNumber
{
public:
	CBillNumber();
	~CBillNumber();

	static HRESULT Load(void);	//	読み込み
	static void Unload(void);	//	開放

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetBillNumber(int nNumber);
	int GetNumber(void) { return m_nNumber; }
	void SetColor(D3DXCOLOR *Color);

private:
	static LPDIRECT3DTEXTURE9	m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;		// 頂点バッファへのポインタ
	D3DXMATRIX					m_mtxWorld;		//ワールドマトリックス
	D3DXVECTOR3					m_pos;			//位置
	int							m_nNumber;		//数字を入れる
};

#endif