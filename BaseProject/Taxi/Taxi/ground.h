//=============================================================================
//
// 川の処理 [ground.h]
// Author :有馬　武志
//
//=============================================================================
#ifndef _GROUND_H_
#define _GROUND_H_

#include "main.h"
#include "scene3D.h"

#define	MAX_GROUND_TEX		(1)
#define GROUND_PRIOTITY		(3)

//=============================================================================
// クラス定義
//=============================================================================
class CGround : public CScene3D
{
public:
	CGround();
	~CGround();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CGround *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXVECTOR2 size, int nType);

	D3DXVECTOR3 RiverMove(void);							// 川の流れによる移動量の変化
	void Sound(bool bPlay);										// 川の音の再生

private:
	static LPDIRECT3DTEXTURE9		m_pTexture[MAX_GROUND_TEX];	// テクスチャへのポインタ
	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;					// 頂点バッファへのポインタ
	D3DXMATRIX						m_mtxWorld;					// ワールドマトリックス
	D3DXVECTOR3						m_pos;						// 位置
	D3DXVECTOR3						m_rot;						// 回転
	D3DXVECTOR3						m_move;						// 移動量
	D3DXVECTOR2						m_size;						// 大きさ
	int								m_nCnt;						// カウンター
	int								m_nType;					// 床の種類
	float							m_fTexMoveSpeed;			// テクスチャの移動速度
	bool							m_bSound;					// 音を再生中かどうか
};
#endif
