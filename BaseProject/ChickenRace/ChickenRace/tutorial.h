//=============================================================================
//
// チュートリアル処理 [tutorial.h]
// Author : 佐藤 安純
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SCORE_UI_TUTORIAL	(6)
//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CGameCamera;
class CMeshField;
class CPlayer;
class CLoadTextMotion;
class CGround;
class CScene2D;
class CInputKeyBoard;
class CXInput;
class CLoadTextEffect;
class CLogo;

//=================================
// チュートリアルクラス
//=================================
class CTutorial
{
public:
	typedef enum
	{
		TYPE_START = 0,
		TYPE_START1,
		TYPE_START2,
		TYPE_START3,
		TYPE_ACCEL,
		TYPE_BRAK,
		TYPE_LIGHT_HANDLE,
		TYPE_LEFT_HAMDLE,
		TYPE_MIDDLE1,
		TYPE_BACK,
		TYPE_FRONT,
		TYPE_MIDDLE2,
		TYPE_RIDE1,
		TYPE_RIDE2,
		TYPE_MIDDLE3,
		TYPE_MIDDLE4,
		TYPE_MIDDLE5,
		TYPE_RIDESTART,
		TYPE_END,
		TYPE_END1,
		TYPE_END2,
		TYPE_END3,
		TYPE_MAX,
	}TYPE;

	CTutorial();							// コンストラクタ
	~CTutorial();							// デストラクタ
	HRESULT Init();							// 初期化処理
	void	Uninit(void);					// 終了処理
	void	Update(void);					// 更新処理
	void	Draw(void);						// 描画処理

	//---------------------------
	// Geter
	//---------------------------
	static CMeshField *GetMeshField(void) { return m_pMeshField; }				//メッシュフィールド
	static CGameCamera * GetCamera(void) { return m_pCamera; }					//カメラ
	static CPlayer * GetPlayer(void) { return m_pPlayer; }						//プレイヤー
	static CLoadTextMotion * GetPlayerMotion(void) { return m_pPlayerMotion; }	//プレイヤーモーション
	static CGround * GetRiver(void) { return m_pGround; }						//川のポインタ
	static CLoadTextEffect * GetLoadEffect(void) { return m_pLoadEffect; }		//読み込んだエフェクトのポインタ
	static CLoadTextMotion * GetBoyMotion(void) { return m_pBoyMotion; }		//男性モーションの取得
	static CLoadTextMotion * GetGirlMotion(void) { return m_pGirlMotion; }		//女性モーションの取得
	static bool GetCustomerPutin(void) { return m_bCustomer; }							//お客さんの乗車状態
	static CLogo * GetScoreUI(int nNum) { return m_pScoreUI[nNum]; }						//スコア関係のUIの取得
	static TYPE GetType(void) { return m_type; }

private:
	void MeshFildCreate(void);	//メッシュフィールドの生成
	void ObjectCreate(void);	//オブジェクトの生成
	void CreateLine(void);		//文章の生成
	void UpdateEnd(void);		//終了
	void ChangeLine(void);		//文章の変更
	void UpdateFade(void);		//フェード
	void WallTexLoad(void);		//壁のテクスチャ読み込み
	void UninitBottun(void);	//ボタンの破棄
	void CreateBottun(void);	//ボタンの生成
	void CreateSkip(bool bInput);		//スキップキーの生成
	void UninitControlBottun(void);	//コントローラーのボタンの破棄
	void UpdateAnim(void);

	//ファイル読み込み用の変数
	char *ReadLine(FILE *pFile, char *pDst);	//1行読み込み
	char *GetLineTop(char *pStr);				//行の先頭を取得
	int  PopString(char *pStr, char *pDest);	//行の最後を切り捨て

	static CMeshField *m_pMeshField;			//メッシュフィールドのポインタ
	static CGameCamera * m_pCamera;				//カメラ
	static CPlayer * m_pPlayer;					//プレイヤー
	static CLoadTextMotion * m_pPlayerMotion;	//プレイヤーモーション
	static CGround * m_pGround;					//川のポインタ
	static CLoadTextEffect * m_pLoadEffect;		//エフェクトの読み込みポインタ
	static CLoadTextMotion * m_pBoyMotion;		//男性モーション読み込み
	static CLoadTextMotion * m_pGirlMotion;		//女性モーション読み込み
	static bool m_bCustomer;				//お客さんの乗車状態フラグ
	static CLogo *m_pScoreUI[MAX_SCORE_UI_TUTORIAL];		//ロゴへのポインタ スコアUIに関係している物

	LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャ
	static TYPE m_type;					//タイプ
	bool m_bLine[TYPE_MAX];			//文章のフラグ
	CScene2D * m_pLine;				//文章ポリゴン
	int	m_nCountTime;				//時間カウンター
	CLogo * m_pSpace;				//スペースキー
	CLogo * m_pImage;				//イメージ画像
	CLogo * m_pSkip;				//スキップキー
	CLogo * m_pBotunn;				//ボタン画像
	bool m_bInput;					//入力状態
	int	 m_nCountTimeKey;			//時間のカウンター
	int  m_nAinNum;					//アニメーションパターン
};
#endif