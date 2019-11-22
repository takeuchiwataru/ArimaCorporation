//=============================================================================
//
// モデル演出処理 [ModelEffect.h]
// Author : Ryo Sugimoto
//
//=============================================================================
#ifndef _MODELEFFECT_H_
#define _MODELEFFECT_H_

#include "model3D.h"
//*****************************************************************************
// マクロの定義
//*****************************************************************************

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

	static	CModelEffect	*Create(D3DXVECTOR3 *pos, TYPE type, STATE state = STATE_NORMAL);
	void	Set(D3DXVECTOR3 *&pos, TYPE &type, STATE &state);
	HRESULT Init(void);
	void	Update(void);
	void	Draw(void);
private://*****************************************************************************
		//変数宣言//***********************************************************************
	D3DXVECTOR3		*m_pPos;
	D3DXVECTOR3		m_move;
	TYPE	m_Type;			//タイプ
	STATE	m_State;			//状態
	float	m_fCntState;	//状態管理用変数
};
#endif
