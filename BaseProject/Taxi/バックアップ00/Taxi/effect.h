//=============================================================================
//
// エフェクトの処理 effect.h]
// Author : 有馬武志
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"
#include "scene.h"
#include "scene3D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define EFFECT_PRIORTY		(4)
//=====================
//  CScene3Dの派生クラス
//=====================
class CEffect : public CScene3D
{
public://誰でも扱える
	typedef enum
	{
		EFFECTTYPE_NONE = 0,		//何もない状態
		EFFECTTYPE_SPLASHES,		//水しぶきのエフェクト
		EFFECTTYPE_ERUPTION,		//水煙エフェクト
		EFFECTTYPE_SMOKE,			//黒煙エフェクト
		EFFECTTYPE_SNOW,			//【天候】雪エフェクト
		EFFECTTYPE_RAIN,			//【天候】雨エフェクト
		EFFECTTYPE_FIRE,			//炎エフェクト
		EFFECTTYPE_POWDER,			//火の粉エフェクト
		EFFECTTYPE_MAX				//エフェクトの最大数
	}EFFECTTYPE;

	CEffect();
	~CEffect();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CEffect *Create(EFFECTTYPE Type,D3DXVECTOR3 pos,int nRadius, int nLife,int nTexType);
	void SetParticleEffect(EFFECTTYPE Type, int nRadius);

private://個人でのみ使う
	static LPDIRECT3DTEXTURE9	m_pTexture[MAX_BILLBOORD_TEX];	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;						// 頂点バッファへのポインタ
	D3DXVECTOR3					m_pos;							// 位置
	D3DXVECTOR3					m_posOld;						// 過去の位置
	D3DXVECTOR2					m_size;							// サイズ
	float						m_fRadius;						// 半径(大きさ)
	int							m_nLife;						// 表示時間(寿命)
	int							m_nType;						// 種類
	EFFECTTYPE					m_EffectType;					// エフェクトの種類
	int							m_nCntEffectState;				// 状態カウンタ
	int							m_nCountTime;					// 秒数カウント
	static int							m_nCntEffect;
};
#endif