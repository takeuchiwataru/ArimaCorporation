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
#define MAX_MAP_OBJECT	(3500)
#define MAX_MAP_CAR		(300)
#define MAX_MAP_MESH	(1400)
#define MAX_MAP_WALL	(550)
#define MAX_MAP_ROUTE	(300)
#define MAX_MAP_POINT	(300)
#define MAX_POINT		(10)
#define MAX_SCORE_UI	(6)

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
class CBillBoord;
class CWall;
class CShadow;
class CLogo;
class CLoadTextMotion;
class CSatisfaction;
class CGameCamera;
class CShaderMgr;
class CEffectMgr;
class CPoliceLamp;
class CGround;
class CLoadTextEffect;

//=====================
// 基本クラス
//=====================
class CGame
{
public:
	typedef enum
	{
		GAMESTATE_NONE = 0,					//何もしていない状態
		GAMESTATE_NORMAL,					//通常状態
		GAMESTATE_CLEAR,					//ゲームクリア
		GAMESTATE_OVER,						//ゲームオーバー
		GAMESTATE_END,						//ゲーム終了
		GAMESTATE_MAX						//最大数
	}GAMESTATE;

	typedef enum
	{//各ステージ
		STAGE_NONE = 0,						//何もしてない状態
		STAGENUM_1,							//ステージ1
		STAGENUM_2,							//ステージ2
		STAGENUM_3,							//ステージ3
		STAGENUM_4,							//ステージ4
		STAGENUM_5,							//ステージ5
		STAGENUM_6,							//ステージ6
		STAGENUM_7,							//ステージ7
	}STAGENUM;

	//テキストの値を管理する場所
	typedef struct
	{
		int					nRouteCount;							// ルートのカウンター
		int					nPointCount[MAX_MAP_ROUTE];				// ポイントのカウンター
		int					nIndexPoint[MAX_MAP_ROUTE][MAX_POINT];	// 車が動く番号 [ルート番号]　[ポイントの番号]　

	}Route;

	//テキストの値を管理する場所
	typedef struct
	{
		int					nCount;					// カウンター
		int					nNum[MAX_MAP_POINT];	// 番号
		D3DXVECTOR3			pos[MAX_MAP_POINT];		// 位置
	}Point;



	CGame();								//コンストラクタ
	~CGame();								//デストラクタ
	HRESULT Init();							//初期化処理
	void Uninit(void);						//終了処理
	void Update(void);						//更新処理
	void Draw(void);						//描画処理
	static void SetGameState(GAMESTATE state) { m_gameState = state; }
	static void SetPause(bool bPause);
	static bool GetPause(void) { return m_bPause; }
	static bool GetHelp(void) { return m_bHelp; }
	static GAMESTATE GetGameState(void) { return m_gameState; }
	static CPlayer *&GetPlayer(void) { return m_pPlayer; }
	static CPolygon3D *GetPolygon3D(void) { return m_pPolygon3D; }
	static CModel *GetModel(void) { return m_pModel; }
	static CMeshField *GetMeshField(void) { return m_pMeshField; }
	static CObject *Get3DObject(void) { return m_pObject; }
	static CCarBase *GetCar(void) { return m_pCar; }
	static CBillBoord *GetBillboord(void) { return m_pBillBoord; }
	static CLoadTextMotion * GetPlayerMotion(void) { return m_pPlayerMotion; }	//プレイヤーのモーションの取得
	static CLoadTextMotion * GetBoyMotion(void) { return m_pBoyMotion; }		//お客さんのモーションの取得
	static CLoadTextMotion * GetGirlMotion(void) { return m_pGirlMotion; }		//女性モーションの取得
	static CSatisfaction * GetSatisfaction(void) { return m_pSatisfaction; }	//満足度の取得
	static CGameCamera * GetGameCamera(void) { return m_pGameCamera; }			//ゲームカメラの取得
	static CShaderMgr * GetShaderMgr(void) { return m_pShaderMgr; }				//シェーダーマネージャーの取得
	static CGround * GetRiver(void) { return m_pGround; }						//川の取得
	static CLoadTextEffect * GetLoadEffect(void) { return m_pLoadTextEffect; }	//読み込んだエフェクトの取得
	static int GetGameCounter(void) { return m_nGameCounter; }					//ゲームのカウンター
	static CLogo * GetScoreUI(int nNum) { return m_pScoreUI[nNum]; }						//スコア関係のUIの取得

	static Route GetCarRoute(void) { return m_Route; }	//	車のルートの取得
	static Point GetCarPoint(void) { return m_Point; }	//	車のポイントの取得
														//ステージ
	static STAGENUM GetNumState(void) { return m_StageState; }
	void SetNumState(STAGENUM stagenum);
	void SetStageState(STAGENUM stage);
	void SetStage(void);

	//---------------------------------
	//ファイルを読み込む
	//---------------------------------
	void TextLoad(int nLoadNumber);
	void MeshTextLoad(int nLoadNumber);
	void WallTextLoad(int nLoadNumber);
	void NpcTextLoad(int nLoadNumber);

	void PointTextLoad(int nLoadNumber);	//	車の動くポイントの読み込み
	void RouteLoad(int nLoadNumber);		//	経路のパターンの読み込み

	static void SetStopRoute(int nStopRoute) { m_nStopRoute = nStopRoute; }
	static int GetStopRoute(void) { return m_nStopRoute; }


	char *ReadLine(FILE *pFile, char *pDst);	//1行読み込み
	char *GetLineTop(char *pStr);				//行の先頭を取得
	int  PopString(char *pStr, char *pDest);	//行の最後を切り捨て

	//ウォークスルー用
	static bool GetDrawUI(void) { return m_bDrawUI; };

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
		int					m_nType;			// 種類
		int					m_nTexType;			// テクスチャの種類
		int					m_nCollision;		// 当たり判定のONOFF
		D3DXVECTOR3			m_pos;				// 位置
		D3DXVECTOR3			m_rot;				// 回転
		D3DXVECTOR3			m_scale;			// サイズ
		int					m_nRoute;			// ルートの設定
		bool				bLoop;				// ループするかどうか
	}Npc;

	static GAMESTATE m_gameState;				//ゲーム状態
	GAMESTATE m_NowGameState;					//現在のゲーム状況
	int m_nCounterGameState;					//状態カウンタ
	static CPlayer *m_pPlayer;					//プレイヤーのポインタ
	static CNumber *m_pNumber;					//数字のポインタ
	static CPause *m_pPause;					//ポーズのポインタ
	static CPolygon3D *m_pPolygon3D;			//３Dポリゴンのポインタ
	static CModel *m_pModel;					//モデルのポインタ
	static CMeshField *m_pMeshField;			//メッシュフィールドのポインタ
	static CObject *m_pObject;					//オブジェクトのポインタ
	static CCarBase *m_pCar;					//NPCのポインタ
	static CBillBoord *m_pBillBoord;			//ビルボードのポインタ
	static CWall *m_pWall;						//壁のポインタ
	static CShadow *m_pShadow;					//影のポインタ
	static CGameCamera * m_pGameCamera;			//カメラのポインタ
	static bool m_bPause;						//現在ポーズかポーズじゃないか
	static bool m_bHelp;						//現在ヘルプかヘルプじゃないか
	static int	m_nCntSetStage;					// ステージセットカウンタ
	static STAGENUM m_StageState;				// ステージ状態
	static STAGENUM m_ConteniueStage;			//コンテニュー時そこのエリアからスタート
	static CLoadTextMotion * m_pPlayerMotion;	//プレイヤーのモーション読み込み
	static CLoadTextMotion * m_pBoyMotion;		//男性モーション読み込み
	static CLoadTextMotion * m_pGirlMotion;		//女性モーション読み込み
	static CSatisfaction * m_pSatisfaction;		//満足度へのポインタ
	static Route m_Route;						//車のマップのルート用
	static Point m_Point;						//設置するpointの構造体
	static CShaderMgr * m_pShaderMgr;			//シェーダーマネージャー
	static CPoliceLamp *m_pPoliceLamp;			//車のランプ点灯
	static CGround *m_pGround;					//川へのポインタ変数
	static CLoadTextEffect * m_pLoadTextEffect;	//エフェクトの読み込み
	static int m_nGameCounter;					//ゲームのカウンター
	static CLogo *m_pScoreUI[MAX_SCORE_UI];		//ロゴへのポインタ スコアUIに関係している物

	int m_nSetObjectNum;						//オブジェクトを置いた数
	int m_nSetMeshFieldNum;						//メッシュフィールドを置いた数
	int	m_nSetWallNum;							//壁の置いた数
	int	m_nSetNpcNum;							//NPCの置いた数
	int	m_nSetPointNum;							//Pointの置いた数
	int	m_nEnemyCount;							//敵の数
	Map m_Map[MAX_MAP_OBJECT];					//設置するオブジェクトの構造体
	Mesh m_Mesh[MAX_MAP_MESH];					//設置するメッシュフィールドの構造体
	Wall m_aWall[MAX_MAP_WALL];					//設置する壁の構造体
	Npc m_aNpc[MAX_MAP_CAR];					//設置するNPCの構造体

	static int m_nStopRoute;					//ルートで車が止まる

	//ウォークスルー用
	static bool m_bDrawUI;
};
#endif