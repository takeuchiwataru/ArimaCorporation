//=============================================================================
//
// ロゴの処理 [logo.h]
// Author : 有馬武志
//
//=============================================================================
#ifndef _LOGO_H_
#define _LOGO_H_

#include "main.h"
#include "billboord.h"
#include "scene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//=====================
//  CScene2Dの派生クラス
//=====================
class CLogo : public CScene2D
{
public://誰でも扱える
	typedef enum
	{
		TYPE_LOGO = 0,			// 通常のロゴ
		TYPE_MULTIRENDERING,	// マルチレンダリング
		TYPE_MAX
	}TYPE;

	CLogo(int nPriorityv = 5);
	~CLogo();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CLogo *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nType, int nTransformType, TYPE type);
	static void SetLogoState(int nStateType, int nStateFont);
	D3DXVECTOR3 MoveLimit(D3DXVECTOR3 pos, D3DXVECTOR3 PosLimit, int nOperation, float fMove);
	bool GetMoveFlag(void) { return m_bMoveFlag; }
	void Zoom(D3DXVECTOR3 pos, D3DXVECTOR2 EndSize, D3DXVECTOR2 AddSize, bool bZoom);
	D3DXVECTOR2 SetSize(D3DXVECTOR2 size) { m_size = size; return m_size; }


private:
	void TexAnim(void);
	void SetCol(void);
	// 拡大縮小の処理

	D3DXVECTOR3 Move(D3DXVECTOR3 pos, float fMove);

	static int					m_nNextType;				//状態を貰うときのタイプ
	static int					m_nNextFont;				//状態の文字
	int							m_nStateType;				//状態を貰うときのタイプ
	int							m_nStateFont;				//状態の文字
	int							m_nType;					//種類
	int							m_nPolygonType;				//種類(画像)
	int							m_nTransformType;			//画像の形を変えれるタイプ
	int							m_nCount;					//カウンター
	D3DXVECTOR3					m_posOld;					//位置保存
	D3DXVECTOR2					m_size;						//サイズ
	float						m_fLength;					//距離
	float						m_fAngle;					//角度
	D3DXVECTOR3					m_rot;						//向き
	D3DXVECTOR3					m_moveLimit;				// 移動制限
	int							m_nCntColA;					// 透明度のカウンター
	bool						m_bColA;					// 透明度のフラグ
	bool						m_bMoveFlag;				// 移動処理時のフラグ
	bool						m_bFlag;					//フラグ
	bool						m_bDataFlag;				//終了フラグ
	int							m_nCntData;					// カウント
	int							m_nCntTime;
	bool						m_bVibration;				//振動音再生したかどうか
};

#endif