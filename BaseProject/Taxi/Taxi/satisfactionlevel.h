//=============================================================================
//
// 満足度の処理 [satisfactionlevel.h]
// Author : shun yokomichi
//
//=============================================================================
#ifndef _SATISFACTION_H_
#define _SATISFACTION_H_

#include "main.h"
#include "billboord.h"
#include "scene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//=====================
//  CScene2Dの派生クラス
//=====================
class CSatisfaction : public CScene2D
{
public://誰でも扱える
	typedef enum
	{	// 色の変化
		COL_NOTCHANGE = 0,	// 通常色
		COL_CHANGE,			// 色を変える
		TYPE_MAX
	}COL_TYPE;

	typedef enum 
	{
		LEVEL_NONE = 0,		// 何もない
		LEVEL_UP,			// 上がる
		LEVEL_DOWN,			// 下がる
		LEVEL_MAX			// 総数
	}LEVEL_TYPE;

	CSatisfaction();
	~CSatisfaction();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CSatisfaction *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nLevel);
	static void SetLogoState(int nStateType, int nStateFont);
	void SetColAType(COL_TYPE type) { m_type = type; }		// タイプの設定
	void AddLevel(int nLevel);		// 満足度加算
	void Subtraction(int nLevel);	// 満足度減算
	int GetLevel(void) { return m_nLevel; }	// 満足度の取得
	void ChangeCol(D3DXCOLOR col);			// 色の変化

private:
	void ResultEvaluation(void);		// リザルト時の結果報告
	static int					m_nNextType;				//状態を貰うときのタイプ
	static int					m_nNextFont;				//状態の文字
	int							m_nStateType;				//状態を貰うときのタイプ
	int							m_nStateFont;				//状態の文字
	D3DXVECTOR2					m_size;						//サイズ
	float						m_fLength;					//距離
	float						m_fAngle;					//角度
	D3DXVECTOR3					m_rot;						//向き
	int							m_nCntTime;
	int							m_nCntNum;					// 星の数カウント
	int							m_nCntLevel;
	bool						m_bChange;
	int							m_nLevel;					// 満足度のレベル
	float						m_fSize;
	COL_TYPE					m_type;
	LEVEL_TYPE					m_LevelType;
	int							m_nLevelOld;
	float						m_Col;
	bool						m_bColFlag;
};

#endif