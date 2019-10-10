//=============================================================================
//
// 報告の処理 [report.h]
// Author : shun yokomichi
//
//=============================================================================
#ifndef _REPORT_H_
#define _REPORT_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define REPORT_LOGO_NUM		(4)

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CSatisfaction;
class CTotalScore;
class CLogo;

//=====================
//  CScene2Dの派生クラス
//=====================
class CReport : public CScene
{
public://誰でも扱える
	typedef enum
	{	// 画面表示の種類
		TYPE_NONE = 0,		// 何もない
		TYPE_TOTALLOGO,		// トータルスコアのロゴ表示
		TYPE_TOTALSCORE,	// トータルスコアの表示
		TYPE_CLAS,			// 満足度のロゴ表示
		TYPE_CLASLEVEL,		// 満足度の評価表示
		TYPE_OK_KEY,		// 決定ボタン表示
		TYPE_ALL_DISPLAY,	// ボタンが押されたら全て表示
		TYPE_MAX			// 総数
	}REPORTTYPE;

	CReport();
	~CReport();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CReport *Create(REPORTTYPE type, int nTotalScore, int nLevel);

	void SetReporh(REPORTTYPE type) { m_nType = m_type;  m_type = type; }

private:
	void SetDisplay();
	void produce();
	void produceAll();

	CSatisfaction	*m_pSatisfaction;				//満足度のポインタ
	CTotalScore		*m_pTotalScore;					//トータルスコアのポインタ
	CLogo			*m_apLogo[REPORT_LOGO_NUM];		//ロゴへのポインタ

	int				m_nTotalScore;					// トータルスコア保存用
	int				m_nLevel;						// 満足度のレベル保存用
	int				m_nCntLogo;						//ロゴのカウント
	D3DXVECTOR2		m_size;							//サイズ
	D3DXVECTOR3		m_rot;							//向き
	D3DXCOLOR		m_col;							//カラー
	float			m_fLength;						//距離
	float			m_fAngle;						//角度
	REPORTTYPE		m_type;							//種類
	int				m_nType;						//種類保存

};
#endif