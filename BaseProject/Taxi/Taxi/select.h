//=============================================================================
//
// セレクト処理 [select.h]
// Author : shun yokomichi
//
//=============================================================================
#ifndef _SELECT_H_
#define _SELECT_H_

#include "main.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SELECT_STAGE		(2)		// ステージ数
#define MAX_SELECT_NUM			(3)
//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;

//=====================
// 基本クラス
//=====================
class CSelect
{
public:
	CSelect();								// コンストラクタ
	~CSelect();								// デストラクタ
	HRESULT Init();							// 初期化処理
	void	Uninit(void);					// 終了処理
	void	Update(void);					// 更新処理
	void	Draw(void);						// 描画処理
	static int GetSelectLevel(void) { return m_nStageNumber;}
private:
	void	SlectStage(void);				// ステージ選択
	void	Select(void);					// 難易度選択
	void	SelectLevel(void);				// 難易度選択
	void	TexAnim(void);					// テクスチャアニメーション
	int		RankingLoad(int nType, int SelectLevel);			// ランキングの読み込み
	void	SelectHuman(void);
	void	Fade(void);						//フェード処理
	void	BackUninit(void);

	int				m_nStageNum;					// ステージ番号
	static int		m_nStageNumber;					// ステージ番号
	int				m_nScore;						// スコア
	int				m_nCntAnim;						// アニメーションカウンター
	int				m_nCntPattern;					// パターンカウンター
	D3DXVECTOR2		m_nSizeOld[MAX_SELECT_STAGE];	// サイズの保存
	CScene2D		*m_pFream;						// フレーム
	CScene2D		*m_pBg;							// 難易度選択時の背景
	bool			m_bPress;						// 押下時のフラグ
	bool			m_bSame;						// 同じ場合のフラグ
	bool			m_bSelectFlag;					// 選択フラグ
	bool			m_bLevelFlag;					// 難易度選択フラグ
	float			m_fAddRot;						// 向きの加算
	bool			m_bEnter;						// 決定押したかどうか
	bool			m_bMove;
};
#endif