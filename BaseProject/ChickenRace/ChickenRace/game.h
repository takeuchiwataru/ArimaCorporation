//=============================================================================
//
// ゲームの処理 [game.h]
// Author : 有馬 武志
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_MAP_OBJECT		(3500)
#define MAX_MAP_CAR			(300)
#define MAX_MAP_MESH		(1400)
#define MAX_MAP_WALL		(550)
#define MAX_MAP_FEED		(100)
#define MAX_MAP_ROUTE		(300)
#define MAX_MAP_POINT		(300)
#define MAX_POINT			(10)
#define MAX_SCORE_UI		(6)

#define MAX_MEMBER			(8)
#define MAX_PLAYER			(4)
#define MAX_CHARCTER		(8)

#define CUORSE_VIEW_TIME	(360)
#define CUORSE_VIEW_TIPE_0	(120)
#define CUORSE_VIEW_TIPE_1	(240)

#define START_SET_TIME		(360)
#define START_COUNT_TIME	(180)

#define MAX_TIME			(35999)

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPlayer;
class CNumber;
class CPause;
class CObject;
class CCarBase;
class CModel;
class CMeshField;
class CPolygon3D;
class CBillboord;
class CWall;
class CShadow;
class CLogo;
class CLoadTextMotion;
class CGameCamera;

class CGameCharSelect;
class CGamePlay;
class CResultUI;

//=====================
// 基本クラス
//=====================
class CGame
{
public:
	typedef enum
	{
		GAMEMODE_NONE = 0,
		GAMEMODE_CHARSELECT,				// キャラ選択
		GAMEMODE_COURSESELECT,				// コース選択
		GAMEMODE_COURSE_VIEW,				// コース見る
		GAMEMODE_PLAY,						// プレイ
		GAMEMODE_MAX						// 最大数
	}GAMEMODE;

	typedef enum
	{
		GAMESTATE_NONE = 0,					//何もしていない状態
		GAMESTATE_NORMAL,					//通常状態
		GAMESTATE_CLEAR,					//ゲームクリア
		GAMESTATE_OVER,						//ゲームオーバー
		GAMESTATE_END,						//ゲーム終了
		GAMESTATE_MAX						//最大数
	}GAMESTATE;

	CGame();								//コンストラクタ
	~CGame();								//デストラクタ
	HRESULT Init();							//初期化処理
	void Uninit(void);						//終了処理
	void Update(void);						//更新処理
	static void Draw(void);					//描画処理
	static void SetGameState(GAMESTATE state) { m_gameState = state; }
	static void SetPause(bool bPause);
	static bool GetPause(void) { return m_bPause; }
	static bool GetHelp(void) { return m_bHelp; }
	static GAMESTATE GetGameState(void) { return m_gameState; }
	static CPlayer **GetPlayer(void) { return m_pPlayer; }
	static CLoadTextMotion * GetPlayerMotion(void) { return m_pPlayerMotion; }	//プレイヤーのモーションの取得
	static CGameCamera * GetCourseCamera(void) { return m_pCuorseCamera; }
	static CGameCamera * GetGameCamera(int nNum)
	{
		if (0 <= nNum && nNum < MAX_PLAYER)
			return m_pGameCamera[nNum];
		else
			return NULL;
	}			//ゲームカメラの取得

	static int GetGameCounter(void) { return m_nGameCounter; }					//ゲームのカウンター
	static void SetGameModeNext(GAMEMODE gameModeNext) { m_gameModeNext = gameModeNext; };
	void SetGameMode(GAMEMODE gameMode);
	static GAMEMODE GetGameMode(void) { return m_gameMode; }
	void SetStage(void);
	void SetPlayer(bool bCreate, int nMode);
	void Ranking(void);

	//---------------------------------
	//ファイルを読み込む
	//---------------------------------
	void TextLoad(int nLoadNumber);
	void MeshTextLoad(int nLoadNumber);
	void WallTextLoad(int nLoadNumber);
	void FeedTextLoad(void);

	char *ReadLine(FILE *pFile, char *pDst);	//1行読み込み
	char *GetLineTop(char *pStr);				//行の先頭を取得
	int  PopString(char *pStr, char *pDest);	//行の最後を切り捨て

												//ウォークスルー用
	static bool GetDrawUI(void) { return m_bDrawUI; };

	// プレイヤー最大数
	static void SetMaxPlayer(int nMaxPlayer) { if (0 <= m_nMaxPlayer && m_nMaxPlayer < 4) m_nMaxPlayer = nMaxPlayer; }
	static int GetMaxPlayer(void) { return m_nMaxPlayer; }

	// キャラ選択番号
	static void SetCharSelectNum(int *nCharSelectNum)
	{
		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
			m_nCharSelectNum[nCntPlayer] = nCharSelectNum[nCntPlayer];
	}
	static int *GetCharSelectNum(void) { return m_nCharSelectNum; }

	// コントローラー番号
	static void SetControllerNum(int *nControllerNum)
	{
		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
			m_nControllerNum[nCntPlayer] = nControllerNum[nCntPlayer];
	}
	static int *GetControllerNum(void) { return m_nControllerNum; }
	static int GetControllerNum(int nNum) { return m_nControllerNum[nNum]; }

	// ランキング
	static int GetRanking(int nPlayerNum) { return m_nRanking[nPlayerNum]; }
	static int *GetRanking(void) { return m_nRanking; }

	static int GetRankingSort(int nPlayerNum) { return m_nRankingSort[nPlayerNum]; }
	static int *GetRankingSort(void) { return m_nRankingSort; }

	// ゴール
	static bool *GetGoul(void) { return m_bGoul; }

	static int GetCameraNumber(void) { return m_nCameraNumber; }
	
	static int *GetTimer(void) { return m_nTime; }

private:
	//テキストの値を管理する場所
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
		int					m_nTexType;						// テクスチャの種類
		int					m_nWidthDivide;					// 横の分割数を入れる
		int					m_nDepthDivide;					// 奥の分割数を入れる
		float				m_fWidthLength;					// 横の長さ
		float				m_fDepthLength;					// 奥の長さ
		float				m_fTexXUV;						// XUV座標
		float				m_fTexYUV;						// YUV座標
		float				m_fVtxHeight_No0;				// 頂点番号0を操作
		float				m_fVtxHeight_No1;				// 頂点番号1を操作
		float				m_fVtxHeight_No2;				// 頂点番号2を操作
		float				m_fVtxHeight_No3;				// 頂点番号3を操作
		float				m_fVtxSide_No0;					// 頂点番号0を操作
		float				m_fVtxSide_No1;					// 頂点番号1を操作
		float				m_fVtxSide_No2;					// 頂点番号2を操作
		float				m_fVtxSide_No3;					// 頂点番号3を操作
		D3DXVECTOR3			m_pos;							// 位置
	}Mesh;

	//テキストの値を管理する場所
	typedef struct
	{
		D3DXVECTOR3			m_pos;				// 位置
		D3DXVECTOR3			m_rot;				// 回転
		float				m_fWidthDivide;		// 横幅
		float				m_fHightDivide;		// 縦幅
		int					m_nTexType;			// テクスチャの種類
	}Wall;

	//テキストの値を管理する場所
	typedef struct
	{
		D3DXVECTOR3			m_pos;				// 位置
		int					m_nZone;			// ゾーン
		int					m_nType;			// 食べ物の種類
	}Feed;

	static GAMEMODE m_gameMode;					//ゲームモード
	static GAMEMODE m_gameModeNext;				//次のゲームモード

	static CGameCharSelect *m_pGameCharSelect;	// ゲーム（キャラ選択）
	static CGamePlay*m_pGamePlay;				// ゲーム（プレイ）

	static GAMESTATE m_gameState;				//ゲーム状態
	GAMESTATE m_NowGameState;					//現在のゲーム状況
	int m_nCounterGameState;					//状態カウンタ
	static CPlayer *m_pPlayer[MAX_MEMBER];		//プレイヤーのポインタ
	static CPause *m_pPause;					//ポーズのポインタ
	static CGameCamera * m_pCuorseCamera;		//コースのポインタ
	static CGameCamera * m_pGameCamera[MAX_PLAYER];//カメラのポインタ
	static bool m_bPause;						//現在ポーズかポーズじゃないか
	static bool m_bHelp;						//現在ヘルプかヘルプじゃないか
	static int	m_nCntSetStage;					// ステージセットカウンタ
	static CLoadTextMotion * m_pPlayerMotion;	//プレイヤーのモーション読み込み
	static int m_nGameCounter;					//ゲームのカウンター

	int m_nSetObjectNum;						//オブジェクトを置いた数
	int m_nSetMeshFieldNum;						//メッシュフィールドを置いた数
	int	m_nSetWallNum;							//壁の置いた数
	int	m_nSetFeedNum;							//食べ物の置いた数

	Map m_Map[MAX_MAP_OBJECT];					//設置するオブジェクトの構造体
	Mesh m_Mesh[MAX_MAP_MESH];					//設置するメッシュフィールドの構造体
	Wall m_aWall[MAX_MAP_WALL];					//設置する壁の構造体
	Feed m_aFeed[MAX_MAP_FEED];					//設置する食べ物の構造体

	static int m_nMaxPlayer;					// プレイヤー数
	static int m_nCharSelectNum[MAX_MEMBER];	// キャラ選択番号

	static int m_nControllerNum[MAX_PLAYER];	// コントローラー番号

	static int m_nRanking[MAX_MEMBER];			// ランキング
	static int m_nRankingSort[MAX_MEMBER];		// ランキング
	static bool m_bGoul[MAX_PLAYER];			// ゴール判定

												//ウォークスルー用
	static bool m_bDrawUI;

	static int m_nCameraNumber;					// 現在使用しているカメラ番号

	static int m_nTime[MAX_MEMBER];				// タイム

	CResultUI			*m_pResultUI;			// リザルトUI

};
#endif