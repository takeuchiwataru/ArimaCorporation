//=============================================================================
//
// タイマー処理 [time.h]
// Author : 有馬　武志
//
//=============================================================================
#ifndef _TIME_H_
#define _TIME_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
//　前方宣言
//*****************************************************************************
class CNumber;
class CLogo;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TIME_MAX			(3)		// タイマーの桁数
#define CUTOMERTIME_MAX		(2)		// お客様の時間桁数

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTime : public CScene
{
public:
	typedef enum
	{
		STATE_NONE = 0,		// 何もない
		STATE_TIMEADD,		// 制限時間加算
		STATE_MAX			// 総数
	}STATETIME;

	typedef enum
	{
		STATECUTOMERADD_NONE = 0,	// 何もない
		STATECUTOMERADD_CUTOMERADD,	// 制限時間加算
		STATECUTOMERADD_MAX			// 総数
	}STATECUTOMERADD;

	CTime(int nPriority = 6, OBJTYPE objType = OBJTYPE_TIME);
	~CTime();

	static CTime *Create(D3DXVECTOR3 pos);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetCutomer(void);
	void TexTime(int nTexData);
	void TexTimCutomerTimee(int nTexData);

	//============================
	// 設定　取得　の関数
	//============================
	static int GetTime(void) { return m_nTime; };
	static int GetCutomerTime(void) { return m_nCutomerTime;}
	static void AddTime(int nTime);
	static void SetCutomerTime(int nCutomerTime)
	{ m_nCutomerTime = nCutomerTime; m_bRideEnd = false; m_stateCutomer = STATECUTOMERADD_CUTOMERADD;}
	static bool GetRideEnd(void) { return m_bRideEnd; }
	static void ResetCutomerTime(void) { m_nCutomerTime = 0; }

private:
	void DebugKey(void);
	void TimeManagement(void);
	int PowerCalculation(int nData, int nOperation);
	CNumber *m_apNumber[TIME_MAX];	// ナンバーへのポインタ
	CNumber *m_apCutomerTiem[CUTOMERTIME_MAX];
	static STATETIME		m_stateTime;				// 種類
	static STATECUTOMERADD	m_stateCutomer;
	static int				m_nTime;			// 時間
	static int				m_nCutomerTime;		// お客様の時間
	static bool				m_bRideEnd;			// 乗車時間が過ぎた場合のフラグ
	int						m_nTimeCount;		// 時間のカウンタ
	int						m_nCutmoerCount;	// 乗車時間のカウンタ
	int						m_nTimeNum;			// 時間の表示数字数
	int						m_nCutmoerNum;		// 乗客時間の表示数字数
	int						m_nCntDirecting;	// 演出時間
	D3DXVECTOR3				m_pos;				// 位置
	CLogo					*m_pLogo;			// ロゴへのポインタ
	bool					m_bUse;				// 使用しているかどうか
};
#endif