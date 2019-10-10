//=============================================================================
//
// 評判の処理 [reputation.h]
// Author : shun yokomichi
//
//=============================================================================
#ifndef _REPUTATION_H_
#define _REPUTATION_H_

#include "main.h"
#include "scene2D.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CLogo;
class CScene2D;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_FRAME	(3)		// コメントの最大数

//=====================
//  CScene2Dの派生クラス
//=====================
class CReputation : public CScene2D
{
public://誰でも扱える
	typedef enum
	{	// 評判の種類(5段階 + 失敗)
		REPUTATION_ONE = 0,	// 評判1(低い)
		REPUTATION_TWO,		// 評判2(普通より少し低い)
		REPUTATION_THREE,	// 評判3(普通)
		REPUTATION_FOUR,	// 評判4(普通用少し高い)
		REPUTATION_FIVE,	// 評判5(高い)
		REPUTATION_FAIL,	// 失敗
		REPUTATION_MAX		// 総数
	}REPUTATIONTYPE;

	CReputation();
	~CReputation();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CReputation *Create(int nSatisfaction);
	static void SetLogoState(int nStateType, int nStateFont);

	void FrameDisplay(void);
	void Satisfaction(void);
private:
	void SetLevel(void);

	static int					m_nNextType;				//状態を貰うときのタイプ
	static int					m_nNextFont;				//状態の文字
	int							m_nStateType;				//状態を貰うときのタイプ
	int							m_nStateFont;				//状態の文字
	int							m_nSatisfaction;			//満足度
	D3DXVECTOR2					m_size;						//サイズ
	float						m_fLength;					//距離
	float						m_fAngle;					//角度
	D3DXVECTOR3					m_rot;						//向き
	int							m_nCntTime;
	int							m_nCntNum;					// コメントの回数
	int							m_nNumTexAnim[MAX_FRAME - 1];// 評価の記録
	REPUTATIONTYPE				m_Type;						// コメントの種類
	CLogo *						m_pBack;					// ロゴへのポインタ(後ろ背景)
	CLogo *						m_apFrame[MAX_FRAME];		// ロゴへのポインタ(コメントフレーム)
	CLogo *						m_apComment[MAX_FRAME];		// ロゴへのポインタ(コメント)
	int							m_nCommentNum[MAX_FRAME];
	int							m_nTexType;
	bool						m_bUninitFlag;
	int							m_nCntUninitTime;
};

#endif