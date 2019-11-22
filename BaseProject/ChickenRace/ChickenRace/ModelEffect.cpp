//=============================================================================
//
// モデル演出処理 [ModelEffect.cpp]
// Author : Ryo Sugimoto
//
//=============================================================================
#include "ModelEffect.h"
#include "model3D.h"
#include "object.h"
#include "manager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//==================================================================================================//
//    * モデル演出の生成関数 *
//==================================================================================================//
CModelEffect	*CModelEffect::Create(D3DXVECTOR3 *pos, TYPE type, STATE state)
{
	CModelEffect *pEffect = NULL;
	pEffect = new CModelEffect;

	if (pEffect != NULL) 
	{
		pEffect->Init();
		pEffect->Set(pos, type, state);
	}

	return pEffect;
}
//==================================================================================================//
//    * モデル演出の生成関数 *
//==================================================================================================//
void	CModelEffect::Set(D3DXVECTOR3 *&pos, TYPE &type, STATE &state)
{
	D3DXVECTOR3 &m_pos = GetposR();

	Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_Type = type;
	m_State = state;

	switch (m_Type)
	{
	case TYPE_SMOKE:	
		CObject::SetModel(this, 0, 0);
		m_pos = *pos;
		m_move.y = 10.0f;
		break;
	}
}
//==================================================================================================//
//    * モデル演出の初期化関数 *
//==================================================================================================//
HRESULT CModelEffect::Init(void)
{
	m_pPos = NULL;
	m_move = INIT_VECTOR;
	m_fCntState = 0.0f;
	m_Type = TYPE_MAX;
	return S_OK;
}
//==================================================================================================//
//    * モデル演出の更新関数 *
//==================================================================================================//
void	CModelEffect::Update(void)
{
	D3DXVECTOR3 &m_pos = GetposR();
	m_pos += m_move;

	switch (m_Type)
	{
	case TYPE_SMOKE:
		if (m_fCntState > 30.0f) { Uninit(); return; }
		m_move *= 0.99f;
		break;
	}
	m_fCntState += 1.0f;
}
//==================================================================================================//
//    * モデル演出の描画関数 *
//==================================================================================================//
void	CModelEffect::Draw(void)
{
	//switch (m_Type)
	//{//合成方法を代入
	//default:
	//	break;
	//}

	//CModel3D::Draw();

	//合成を戻す
}
