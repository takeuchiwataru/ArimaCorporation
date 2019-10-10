//=============================================================================
//
// 煙のエフェクト処理 [smokeeffect.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _SMOKEEFFECT_H_
#define _SMOKEEFFECT_H_

#include "main.h"
#include "particlebillboad.h"

//=============================================================================
// ゲームの季節エフェクトクラス
//=============================================================================
class CSmokeEffect : CParticleBillboad
{
public:
	CSmokeEffect();
	~CSmokeEffect();

	static CSmokeEffect * Create(D3DXVECTOR3 Pos);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void LoadTex(void);
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;		//テクスチャ
	D3DXVECTOR2 m_size;			//サイズ
	float		m_fCol_A;		//色のα値
	int			m_nLife;		//ライフ
};
#endif
