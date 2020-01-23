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
		TYPE_NORMAL = 0,
		TYPE_UP,
		TYPE_DOWN,
		TYPE_TURN,	// 回転しませんごめんなさい
		TYPE_MAX
	} PARTICLE_TYPE;

	typedef enum
	{// キー要素
		TEXTURE_STAR = 0,	// 星
		TEXTURE_SMOKE,		// 煙
		TEXTURE_MAX			// テクスチャの種類の総数
	} TEXTURE_TYPE;

	HRESULT Init(void);	// 3Dパーティクル初期化処理
	void Uninit(void);	// 3Dパーティクル終了処理
	void Update(void);	// 3Dパーティクル更新処理
	void Draw(void);	// 3Dパーティクル描画処理
	static HRESULT Load(void);
	static void UnLoad(void);

	static CParticle *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, D3DXVECTOR2 size, int nLife, TEXTURE_TYPE typeT, PARTICLE_TYPE typeP, int nPlayerNum);	// オブジェクトの生成


private:
	void Move(void);
	static LPDIRECT3DTEXTURE9 m_apTexture[TEXTURE_MAX];
	int m_nLife;
	float m_fAlfa;
	int m_nPlayerNum;
	D3DXCOLOR m_col;
	D3DXVECTOR2 m_Size;
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_rot;
	PARTICLE_TYPE m_Type;
};

#endif