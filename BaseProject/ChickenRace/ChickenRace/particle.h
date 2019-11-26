//=============================================================================
//
// パーティクルの処理 [particle.h]
// Author : 長山拓実
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"
#include "billboord.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_PARTICLE3D		(256)							// パーティクルの数
#define MAX_EFFECT_TEX		(2)								// エフェクトのテクスチャ数

//========================================
// クラスの定義
//========================================
//=====================
// パーティクルクラス
//=====================
class CParticle : public CBillBoord
{
public:
	CParticle();	// コンストラクタ
	~CParticle();	// デストラクタ

	typedef enum
	{// キー要素
		TYPE_NORMAL = 0,	// 通常状態
		TYPE_DOWN,			// 減速状態
		PARTICLESTATE_MAX			// プレイヤーの種類の総数
	} PARTICLE_TYPE;

	HRESULT Init(void);	// 3Dパーティクル初期化処理
	void Uninit(void);	// 3Dパーティクル終了処理
	void Update(void);	// 3Dパーティクル更新処理
	void Draw(void);	// 3Dパーティクル描画処理
	static HRESULT Load(void);
	static void UnLoad(void);

	static CParticle *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, D3DXVECTOR2 size, int nLife, PARTICLE_TYPE type);	// オブジェクトの生成


private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_EFFECT_TEX];

	int m_nLife;
	D3DXCOLOR m_col;
	D3DXVECTOR2 m_Size;
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_rot;
	PARTICLE_TYPE m_ParticleType;
};

#endif