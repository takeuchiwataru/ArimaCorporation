//=============================================================================
//
// ランキング処理 [ranking.h]
// Author : shun yokomichi
//
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_RANKING				(5)		// 満足度の数
#define MAX_RANKING_NUMBER		(7)		// ランキングの数字
#define MAX_LEVEL_NUM			(2)

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CTotalScore;
class CSatisfaction;

//=====================
// 基本クラス
//=====================
class CRanking
{
public:
	CRanking();								// コンストラクタ
	~CRanking();							// デストラクタ
	HRESULT Init();							// 初期化処理
	void	Uninit(void);					// 終了処理
	void	Update(void);					// 更新処理
	void	Draw(void);						// 描画処理

	static void	RankingSave(int nTotalScore, int nLvevl);		// ランキングデータの書き込み
	static void Reset(void) { m_nNum = MAX_RANKING; m_nSatisfaction = MAX_RANKING; }

private:
	void	RankingLoad(void);								// ランキングデータの読み込み

	static int			m_nNum;
	static int			m_nSatisfaction;
	static int			m_aTotalScore[MAX_LEVEL_NUM][MAX_RANKING];
	static int			m_aSatisfactionLevel[MAX_LEVEL_NUM][MAX_RANKING];
	int					m_nCntFrame;		// フェード時間
	bool				m_bOnOff;
	static CTotalScore		*m_apTotalScore[MAX_RANKING];
	static CSatisfaction	*m_pSatisfaction[MAX_RANKING];
};
#endif