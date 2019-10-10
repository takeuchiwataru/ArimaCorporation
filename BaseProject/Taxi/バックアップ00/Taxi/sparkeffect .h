//=============================================================================
//
// 火花のエフェクト処理 [sparkeffect.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _SPARKEFFECT_H_
#define _SPARKEFFECT_H_

#include "main.h"
#include "particlebillboad.h"

//=============================================================================
// ゲームの季節エフェクトクラス
//=============================================================================
class CSparkEffect : CParticleBillboad
{
public:
	CSparkEffect();
	~CSparkEffect();

	static CSparkEffect * Create(D3DXVECTOR3 Pos);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void LoadTex(void);
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;		//テクスチャ
	float		m_fCol_A;		//色のα値
	float		m_fCol_G;		//色のｇ値
	float		m_fCol_B;		//色のｂ値
	float		m_fChangeCol_G;	//色の変化量
	float		m_fChangeCol_B;	//色の変化量
	int			m_nLife;		//ライフ
	D3DXVECTOR3 m_move;			//移動量
};
#endif
