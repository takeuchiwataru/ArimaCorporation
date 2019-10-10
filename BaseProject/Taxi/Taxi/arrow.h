//=============================================================================
//
// 目的地の矢印の処理 [arrow.h]
// Author : shun yokomichi
//
//=============================================================================
#ifndef _ARROW_H_
#define _ARROW_H_

#include "main.h"
#include "model3D.h"
//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CLogo;
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//=====================
//  Model3Dの派生クラス
//=====================
class CArrow : public CModel3D
{
public://誰でも扱える
	typedef enum
	{	// 目的地の種類
		TYPE_DESTINATION = 0,	// 目的地案内
		TYPE_GOAL,				// ゴール時
		TYPE_MAX
	}ARROWTYPE;

	typedef enum
	{
		SCZLE_SMALL = 0,
		SCZLE_BIG,
		SCZLE_NOMAL,
		SCZLE_MAX
	}SCZLE_TYPE;

	CArrow();
	~CArrow();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CArrow *Create(int nArrowType, D3DXVECTOR3 Pos, D3DXVECTOR3 GoalPos, float fRadius);
	void SetGoalPos(D3DXVECTOR3 GoalPos) { m_goalpos = GoalPos; }
	static LPDIRECT3DTEXTURE9 GetTex() { return m_pTextureMT; }


private:
	D3DXVECTOR3 SetRot(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	D3DXVECTOR3 AreaDetermine(D3DXVECTOR3 pos, D3DXVECTOR3 scale);

	static LPD3DXMESH			m_pMeshModel;				//メッシュ情報へのポインタ
	static LPD3DXBUFFER			m_pBuffMatModel;			//マテリアルの情報へのポインタ
	static DWORD				m_nNumMatModel;				//マテリアルの情報数
	static D3DXVECTOR3			m_LoadVtxMax;
	static D3DXVECTOR3			m_LoadVtxMin;
	static int					m_nNextType;				//状態を貰うときのタイプ
	static int					m_nNextFont;				//状態の文字
	int							m_nStateType;				//状態を貰うときのタイプ
	int							m_nStateFont;				//状態の文字
	int							m_nArrow;					//矢印のタイプ
	float						m_fLength;					//距離
	float						m_fAngle;					//角度
	float						m_fRadius;					// ゴールの半径
	float						m_nSizeScale;				//スケール
	D3DXVECTOR3					m_rot;						//向き
	D3DXVECTOR3					m_goalpos;					//目的地の位置
	D3DXVECTOR3					m_scale;					//スケール
	CLogo *						m_pLogo;					//ロゴへのポインタ
	CLogo *						m_pCommand;					//ロゴへのポインタ
	SCZLE_TYPE					m_pScaleType;				// スケールの種類

	static LPDIRECT3DTEXTURE9 m_pTextureMT;					// レンダリング用のTexturte
	LPDIRECT3DSURFACE9		m_pRenderMT;					// Textureにレンダリングするためのパイプ
	LPDIRECT3DSURFACE9		m_pBuffMT;						// Textureに使用するバッファ
	D3DVIEWPORT9			m_viewportMT;					// Textureに使用するビューポート
	D3DXCOLOR				m_col;							// カラー
	D3DXCOLOR				m_colChange;					// 色変化用

};

#endif
