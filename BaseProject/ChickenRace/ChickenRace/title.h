//============================================================================
//
// メイン処理 [title.h]
// Author : 有馬 武志
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_MAP_OBJECT			(3500)
#define MAX_MAP_FEED			(100)
#define MAX_MEMBER				(8)

#define RACEINIT_TITLEMODE		(0)
#define RACEMOVE_TITLEMODE		(600 + RACEINIT_TITLEMODE)
#define CAMERA_P_TITLEMODE_1	(360 + RACEMOVE_TITLEMODE)
#define CAMERA_UP_TITLEMODE		(720 + CAMERA_P_TITLEMODE_1)
#define CAMERA_P_TITLEMODE_2	(720 + CAMERA_UP_TITLEMODE)
#define RANKING_TITLEMODE		(600)
#define HOME_TITLEMODE			(720 + CAMERA_P_TITLEMODE_2 + RANKING_TITLEMODE)
#define CHARUP_TITLEMODE		(300 + HOME_TITLEMODE)
#define RESET_TITLEMODE			(300 + CHARUP_TITLEMODE)

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CTitleCamera;

class CTitleMenu;
class CPlayer;
class CResultUI;

//=====================
// 基本クラス
//=====================
class CTitle
{
public:
	CTitle();								// コンストラクタ
	~CTitle();								// デストラクタ
	HRESULT Init();							// 初期化処理
	void	Uninit(void);					// 終了処理
	void	Update(void);					// 更新処理
	static void	Draw(void);					// 描画処理

	static CPlayer **GetPlayer(void) { return m_pPlayer; }

	static void SetCounter(int nCounter) { m_nTitleCounter = nCounter; }
	static int GetCounter(void) { return m_nTitleCounter; }

	// メニュー
	static void SetMenu(bool bMenu) { m_bMenu = bMenu; }
	static bool GetMenu(void) { return m_bMenu; }
	// オンライン
	static void SetOnline(bool bOnline) { m_bOnline = bOnline; }
	static bool GetOnline(void) { return m_bOnline; }
	// ホスト
	static void SetHost(bool bHost) { m_bHost = bHost; }
	static bool GetHost(void) { return m_bHost; }

	static CTitleCamera * GetTitleCamera(void) { return m_pTitleCamera; }

	void SetStage(void);
	void SetTitleMode(void);
	void Ranking(void);

private:
	typedef struct
	{
		int					m_nType;			// 種類
		int					m_nTexType;			// テクスチャの種類
		int					m_nCollision;		// 当たり判定のONOFF
		D3DXVECTOR3			m_pos;				// 位置
		D3DXVECTOR3			m_rot;				// 回転
		D3DXVECTOR3			m_scale;			// サイズ
	}Map;

	//テキストの値を管理する場所
	typedef struct
	{
		D3DXVECTOR3			m_pos;				// 位置
		int					m_nZone;			// ゾーン
		int					m_nType;			// 食べ物の種類
	}Feed;

	//---------------------------------
	//ファイルを読み込む
	//---------------------------------
	void TextLoad(int nLoadNumber);
	void FeedTextLoad(void);

	char *ReadLine(FILE *pFile, char *pDst);	//1行読み込み
	char *GetLineTop(char *pStr);				//行の先頭を取得
	int  PopString(char *pStr, char *pDest);	//行の最後を切り捨て

	int m_nSetObjectNum;						//オブジェクトを置いた数
	int	m_nSetFeedNum;							//食べ物の置いた数

	Map m_Map[MAX_MAP_OBJECT];					//設置するオブジェクトの構造体
	Feed m_aFeed[MAX_MAP_FEED];					//設置する食べ物の構造体

	int					m_nFrame;			// 季節のカウンター
	int					m_nCntTex;			// テクスチャのカウンター
	int					m_nCntCreate;		// 生成時間
	int					m_nLeafposX;		// 落ち葉の 生成座標
	int					m_nLeafsize;		// 落ち葉の サイズ
	static int			m_nTitleCounter;	// タイトルのカウンター
	bool				m_bOnOff;

	static CTitleCamera	*m_pTitleCamera;	//タイトルカメラのポインタ

	static bool			m_bMenu;			// メニュー
	static bool			m_bOnline;			// オンライン
	static bool			m_bHost;			// ホスト

	CTitleMenu			*m_pTitleMenu;		// タイトルメニュー

	static CPlayer *m_pPlayer[MAX_MEMBER];		//プレイヤーのポインタ

	static int m_nRanking[MAX_MEMBER];			// ランキング

	CResultUI			*m_pResultUI;			// リザルトUI

};
#endif