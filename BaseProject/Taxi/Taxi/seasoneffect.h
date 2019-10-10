//=============================================================================
//
// 季節エフェクトの処理 [seasoneffect.h]
// Author : shun yokomichi
//
//=============================================================================
#ifndef _SEASONEFFECT_H_
#define _SEASONEFFECT_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RADIUS_CONDIUIONS	(20)								// 半径の大きさの条件値
//=====================
//  CScene2Dの派生クラス
//=====================
class CSeasonEffect : public CScene2D
{
public://誰でも扱える
	typedef enum
	{
		SEASONTYPE_NONE = 0,		//何もない状態
		SEASONTYPE_SPRING,			// 春
		SEASONTYPE_SUMMER,			// 夏
		SEASONTYPE_FALL,			// 秋
		SEASONTYPE_WINTRE,			// 冬
		SEASONTYPE_MAX				//エフェクトの最大数
	}SEASONTYPE;

	CSeasonEffect(int nPriorityv = 4);
	~CSeasonEffect();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CSeasonEffect *Create(int Type,D3DXVECTOR3 pos,int nRadius,int nTexType);

private://個人でのみ使う
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;						// 頂点バッファへのポインタ
	D3DXVECTOR3					m_pos;							// 位置
	D3DXVECTOR3					m_posOld;						// 過去の位置
	D3DXVECTOR2					m_size;							// サイズ
	D3DXVECTOR3					m_rot;							// 向き
	float						m_fRadius;						// 半径(大きさ)
	float						m_fAngle;						// 角度
	float						m_fLength;						// 距離
	int							m_nType;						// 種類
	SEASONTYPE					m_SeasonType;					// 季節の種類
	int							m_nCntEffectState;				// 状態カウンタ
	int							m_nCountTime;					// 秒数カウント
	static int					m_nCntEffect;
};
#endif