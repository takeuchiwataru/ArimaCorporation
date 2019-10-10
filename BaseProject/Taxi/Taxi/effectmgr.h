//=============================================================================
//
// エフェクトマネージャー処理 [effectmgr.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _EFFECTMANAGER_H_
#define _EFFECTMANAGER_H_

#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TEX_NUM	(1)

//=============================================================================
// エフェクトマネージャークラス
//=============================================================================
class CEffectMgr
{
public:
	typedef enum
	{//エフェクトの種類
		TYPE_SPRING,
		TYPE_SUMMER,
		TYPE_FALL_YELLOW,
		TYPE_FALL_RED,
		TYPE_SNOW,
		TYPE_MAX,
	}TYPE;

	CEffectMgr();
	~CEffectMgr();

	static CEffectMgr * Create(D3DXVECTOR3 pos, TYPE type);
	void Init(void);
	void Uninit(void);
	void Update(void);
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }

	//----------------
	// Load & Unload
	//----------------
	static void LoadTex(void);
	static void UnloadTex(void);

private:
	void SetType(TYPE type) { m_Type = type; }

	//スタティック変数
	static LPDIRECT3DTEXTURE9 m_pTexture;

	//メンバ変数
	TYPE		m_Type;		//タイプ
	D3DXVECTOR3	m_pos;		//位置
	int			m_nCounter;	//カウンター
};
#endif