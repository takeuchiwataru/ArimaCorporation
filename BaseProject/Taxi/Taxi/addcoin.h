//=============================================================================
//
// コイン追加の処理 [addcoin.h]
// Author : 有馬武志
//
//=============================================================================
#ifndef _ADDCOIN_H_
#define _ADDCOIN_H_

#include "main.h"
#include "scene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//=====================
//  CScene2Dの派生クラス
//=====================
class CAddCoin : public CScene2D
{
public://誰でも扱える
	CAddCoin(int nPriorityv = 6);
	~CAddCoin();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CAddCoin *Create(void);

	static void LoadTex(void);
	static void UnloadTex(void);
private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXVECTOR3 m_pos;		//位置
	D3DXVECTOR3 m_move;		//移動量
	float m_fAngle;			//角度
	int	  m_nCountTime;		//カウンター
	int	  m_nPattern;		//パターン
	int   m_nLife;			//表示時間
};
#endif