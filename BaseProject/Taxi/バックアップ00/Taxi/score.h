//=============================================================================
//
// スコア処理 [score.h]
// Author : 有馬　武志
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
//　前方宣言
//*****************************************************************************
class CNumber;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCORE_MAX		(7)		// スコアの桁数

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CScore : public CScene
{
public:
	typedef enum
	{
		STATE_NONE = 0,	// 何もない
		STATE_SET,		// 設定状態
		STATE_ROULETTE,	// ルーレット状態
		STATE_DESSCORE,	// スコアの表示
		STATE_ADD_TOTAL,// トータルスコアに加算
		STATE_ADD,		// スコアに加算
		STATE_RESET,	// 失敗の場合
		STATE_MAX		// 総数
	}STATE;

	typedef enum
	{
		TYPE_USE = 0,
		TYPE_NOT_USE,
		TYPE_MAX
	}TYPE;

	CScore(int nPriority = 6, OBJTYPE objType = OBJTYPE_SCORE);
	~CScore();

	static CScore *Create(D3DXVECTOR3 pos);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void Reset(void) { m_nScore = 0; m_state = STATE_RESET; };
	static void AddTotalScore(void);				// トータルスコアに加算

	//===================
	// 設定 取得 の関数
	//===================
	static int GetScore(void) { return m_nScore; };
	static void AddScore(int nScore);
	static void SetScore(int nScore);
	static TYPE GetType() { return m_type; }
	static void SetType(TYPE type) { m_type = type; }
	void SetType();
private:
	void DebugKey(void);
	void NumberRoulette(int nTexData);
	void pow(void);

	CNumber *m_apNumber[SCORE_MAX];	// ナンバーへのポインタ
	static int m_nScore;			// スコア
	static int m_nScoreNum;			// スコアの数字数
	D3DXVECTOR3 m_pos;				// 位置
	static STATE	m_state;		// 状態
	int				m_nCntRoulette;	//ルーレットのカウンター
	int				m_nCntNum;		// 切り替わる番号記録
	int				m_nCntAdd;		// ルーレット時、数字を加算する
	static TYPE			m_type;

};

#endif