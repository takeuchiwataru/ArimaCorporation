//=============================================================================
//
// モデル演出処理 [ModelEffect.h]
// Author : Ryo Sugimoto
//
//=============================================================================
#ifndef _MODELEFFECT_H_
#define _MODELEFFECT_H_

#include "model3D.h"
#include "2DPolygon.h"
//*****************************************************************************
// マクロの定義
//*****************************************************************************
#define SMOKE_TIME	(30.0f)
//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CModelEffect : public CModel3D
{
public:
	typedef enum
	{
		TYPE_SMOKE,		//土煙
		TYPE_MAX
	}TYPE;
	typedef enum
	{
		STATE_NORMAL,	//通常
		STATE_LOCK,		//追従
		STATE_MAX
	}STATE;
	CModelEffect() {};
	~CModelEffect() {};

	static	CModelEffect	*Create(D3DXVECTOR3 *pos, D3DXVECTOR3 &move, TYPE type, STATE state = STATE_NORMAL);
	void	Set(D3DXVECTOR3 *&pos, D3DXVECTOR3 &move, TYPE &type, STATE &state);
	HRESULT Init(void);
	void	Update(void);
	void	Draw(void);
private://*****************************************************************************
		//変数宣言//***********************************************************************
	D3DXVECTOR3		*m_pPos;
	D3DXVECTOR3		m_move;
	C2D::DRAW_TYPE	m_DrawType;		// 描画する方法(加算合成など)

	TYPE	m_Type;			//タイプ
	STATE	m_State;			//状態
	float	m_fCntState;	//状態管理用変数
};
#endif
