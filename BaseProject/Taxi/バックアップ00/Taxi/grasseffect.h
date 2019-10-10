//=============================================================================
//
// 草のエフェクト処理 [grasseffect.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _GRASSEFFECT_H_
#define _GRASSEFFECT_H_

#include "main.h"
#include "particlebillboad.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_PATTERN		(4)

//=============================================================================
// ゲームの季節エフェクトクラス
//=============================================================================
class CGrassEffect : CParticleBillboad
{
public:
	CGrassEffect();
	~CGrassEffect();

	static CGrassEffect * Create(D3DXVECTOR3 Pos, int nNumPattern);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void LoadTex(void);
	static void Unload(void);

private:
	void SetNumPattern(int NumPattern) { m_nNumPattern = NumPattern; }
	static LPDIRECT3DTEXTURE9 m_pTexture;		//テクスチャ
	D3DXVECTOR2 m_size;			//サイズ
	D3DXVECTOR3 m_move;			//移動量
	float		m_fCol_A;		//色のα値
	int			m_nLife;		//ライフ
	int			m_nNumPattern;	//パターン番号
};
#endif
