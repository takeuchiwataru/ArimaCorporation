//=============================================================================
//
// トータルスコア処理 [totalscore.h]
// Author : 有馬　武志
//
//=============================================================================
#ifndef _TOTALSCORE_H_
#define _TOTALSCORE_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
//　前方宣言
//*****************************************************************************
class CNumber;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TOTALSCORE_MAX	(7)		// タイマーの桁数

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTotalScore : public CScene
{
public:
	typedef enum
	{
		STATE_NONE = 0,		// 何もない
		STATE_ROULETTE,		// ルーレット状態
		STATE_DISSCORE,		// トータルスコアの表示
		STATE_MAX			// 総数
	}STATE;

	typedef enum
	{
		TYPE_USE = 0,
		TYPE_NOT_USE,
		TYPE_MAX
	}TYPE;


	// 基本的な関数
	CTotalScore(int nPriority = 6, OBJTYPE objType = OBJTYPE_TOTALSCORE);
	~CTotalScore();

	static CTotalScore *Create(D3DXVECTOR3 pos);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 関数
	void RouletteNone(void);						//	ルーレット状態にしない
	void SetRankingScore(int nTotalScore);			// ランキング用のトータルスコア設定
	void SetCol(D3DXCOLOR col) { m_col = col; }		// 色の設定
	void ChangeCol(D3DXCOLOR col);					// 色の点滅
	void SetType();


	// 設定と取得の関数
	static int GetScore(void) { return m_nTotalScore; };
	static void AddTotalScore(int nTotalScore);

private:
	int NumberRoulette(int nTexData, CManager::MODE mode, int nNumData);

	CNumber			*m_apNumber[TOTALSCORE_MAX];	// ナンバーへのポインタ
	D3DXVECTOR3		m_pos;							// 位置
	D3DXCOLOR		m_col;							// 色
	static STATE	m_state;						// 状態
	static int		m_nTotalScore;					// スコア
	static int		m_nTotalNum;					// トータルスコアの数字数
	int				m_nCntRoulette;					//ルーレットのカウンター
	int				m_nCntNum;						// 切り替わる番号記録
	int				m_nCntAdd;						// ルーレット時、数字を加算する
	TYPE			m_type;


	// ランキング用の変数
	int				m_nRankingScore;				// ランキング用のスコア保存用
	int				m_nRankingTotalNum;				// ランキング用のスコア数字数
	int				m_nCntColTime;					// 色の変化の時間
	int				m_nCountTime;					// 時間のカウンター
	bool			m_bChangeCol;					// 色の変化のフラグ
	bool			m_bPlaySound;					// 音の再生フラグ
	bool			m_bPlaySoundResult;				// リザルト用の音の再生フラグ
};

#endif