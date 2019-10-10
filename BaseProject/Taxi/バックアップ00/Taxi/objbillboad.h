//=============================================================================
//
// オブジェクトビルボード処理 [objbillboad.h]
// Author : 佐藤 安純	Sato Asumi
//
//=============================================================================
#ifndef _OBJBILLBOAD_H_
#define _OBJBILLBOAD_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define OBJBILL_TEX	(2)

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CObjBillboad : public CScene
{
public:
	typedef enum
	{
		TYPE_KONE = 0,
		TYPE_WOOD,
		TYPE_MAX,
	}TYPE;

	CObjBillboad();
	~CObjBillboad();
	static CObjBillboad * Create(D3DXVECTOR3 pos, int nObjType);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetModelInfo(D3DXVECTOR3 Scale, int ModelTex, int nCollision);

	//---------------------
	// Load & Unload
	//---------------------
	static void LoadTexture(void);
	static void UnloadTexture(void);

private:
	void SetTexNum(int nObjType);
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };
	static LPDIRECT3DTEXTURE9 m_pTexture[OBJBILL_TEX];
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// 頂点バッファへのポインタ
	D3DXMATRIX				m_mtxWorld;			// ワールドマトリックス
	int m_nTexNum;								//ビルボードのテクスチャ番号
	int m_nCollision;							//モデルのあたり判定
	int m_nModelNumTex;							//モデルのテクスチャ番号
	int m_nObjType;								//オブジェクトのタイプ
	D3DXVECTOR3 m_pos;							//位置
	D3DXVECTOR2 m_size;							//ビルボードのサイズ
	D3DXVECTOR3 m_ModelSize;					//モデルのサイズ
	D3DXVECTOR3 m_ModelPos;						//モデルの位置
	float m_fDrawLength;						//描画範囲
	float m_fModelLength;						//モデル表示範囲
	bool m_bSetModel;							//モデルを表示するかどうか
};
#endif