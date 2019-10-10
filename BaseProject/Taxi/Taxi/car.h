//=============================================================================
//
// NPC車の処理 [object.h]
// Author : YUTARO　ABE
//
//=============================================================================
#ifndef _CAR_H_
#define _CAR_H_

#include "model3D.h"
#include "taillamp.h"
#include "policelamp.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXT_CARNAME	"data\\TEXT\\ステージ6\\car.txt"			//読み込むテキストファイル
#define MAX_CAR						(10)			// 車のモデル数
#define MAX_CAR_TEXTURE				(1)				// 車のテクスチャ数
#define CAR_PRIOTITY				(2)				// 車の優先順位
#define INDXPOINT_MAX				(10)			// 車のポイントの最大数

//*****************************************************************************
//	前方宣言
//*****************************************************************************

//=====================
//  CModel3Dの派生クラス
//=====================
class CCarBase : public CModel3D
{
public://誰でも扱える

	typedef enum
	{//車の種類
		MODEL_NOMAL_CAR_000 = 0,	//	通常の車[0]
		MODEL_NOMAL_CAR_001,		//	通常の車[1]
		MODEL_NOMAL_CAR_002,		//	通常の車[2]

		MODEL_SPORTS_CAR_000,		//	スポーツカー[0]
		MODEL_SPORTS_CAR_001,		//	スポーツカー[1]
		MODEL_SPORTS_CAR_002,		//	スポーツカー[2]

		MODEL_BUS_CAR_000,			//	バス[0]
		MODEL_BUS_CAR_001,			//	バス[1]
		MODEL_BUS_CAR_002,			//	バス[2]

		MODEL_POLICE_CAR,			//	警察

		CAR_MODEL_MAX
	}CAR_MODEL;

	const static int m_MaxCarModel = CAR_MODEL_MAX;			// 車の数

	CCarBase();
	~CCarBase();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MoveMent(D3DXVECTOR3 VecA, float fAddRot, int bPoliceMoveValeu);
	D3DXVECTOR3 AddRod(D3DXVECTOR3 rot, D3DXVECTOR3 rotDest, float fAddRot);
	static CCarBase *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nTexType, int nObjectType, int nCollision, int route, bool bLoop);
	//void CollisitionPriority2(void);

	bool GetPointFar(void) { return m_bPointFar; }					//	ポイントと遠いかどうか
	int GetType(void) { return m_nType; }							//	車の種類の取得
	int GetRoute(void) { return m_nRoute; }							//	ルートの取得
	void SetRoute(int nRoute) { m_nRoute = nRoute; }				//	ルートの設定
	void SetPointFar(bool bPointFar) { m_bPointFar = bPointFar; }	//	ポイントと遠いかどうか
	bool GetLoop(void) { return m_bLoop; }							//	移動でループするかどうかの取得
	void SetLoop(bool bLoop) { m_bLoop = bLoop; }					//	移動でループするかどうか設定

	D3DXVECTOR3 GetInitPos(void) { return m_InitPos; }				//	初期位置の取得
	void SetInitPos(D3DXVECTOR3 InitPos) { m_InitPos = InitPos; }	//	初期位置の設定

	bool GetOherHit(void) { return m_bOtherCarHit; }				//	敵同士の当たり判定取得
	void SetOherHit(bool bOtherCarHit) { m_bOtherCarHit = bOtherCarHit; }	//	敵同士の当たり判定設定

private://	個人でのみ使う
	static LPD3DXMESH	m_pMesh[MAX_CAR];							//	メッシュ情報へのポインタ
	static LPD3DXBUFFER m_pBuffMat[MAX_CAR];						//	マテリアルの情報へのポインタ
	static DWORD		m_nNumMat[MAX_CAR];							//	マテリアルの情報数
	static D3DXVECTOR3  m_LoadVtxMax[MAX_CAR];
	static D3DXVECTOR3  m_LoadVtxMin[MAX_CAR];
	static LPDIRECT3DTEXTURE9 m_pMeshTextures[MAX_CAR_TEXTURE];
	D3DXVECTOR3			m_Spin;										//  回転
	int					m_nCount;									//  カウンター
	D3DXVECTOR3			m_ModelMove;								//  移動の仕方
	int					m_nSelectNum;								//  オブジェクトの変更
	int					m_nSetObjectNum;							//  オブジェクトの数
	D3DXVECTOR3			m_rotDest;									//  目的の回転
	float				m_fAngle;									//  向き
	int					m_nRoute;									//  車の経路
	bool				m_bLoop;									//  車のループ
																	//  ファイル
	int					m_nType;									//  種類
	int					m_nTexType;									//  テクスチャの種類
	int					m_nCollision;								//  当たり判定のONOFF
	bool				m_bPointFar;								//	ポイントと遠いい場合
	D3DXVECTOR3			m_InitPos;									//	初期位置
	bool 				m_bOtherCarHit;								//	車同士当たっているかどうか
};

//=====================
//	NPCの車
//=====================
class CCarNpc : public CCarBase
{
public://誰でも扱える
	CCarNpc();
	~CCarNpc();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetCarPointVec(void);						//	車のポイントとのベクトルの取得
	void GoPointNormalCurve(D3DXVECTOR3 pos, int nRoute);	//	次のポイントに向かう[通常の曲がり]
	void CollisitionPriority2(void);

private://個人でのみ使う
	D3DXVECTOR3			m_posOld;							//  過去の位置
	D3DXVECTOR3			m_VecA;								//  向かうベクトルの値
	int					m_nRouteCounter;					//  ルートのカウント
	int					m_nNextPoint;						//  次のポイント
	int					m_nIndexPoint[INDXPOINT_MAX];		//  ポイントの番号
	int					m_nRoute;							//  車の経路
	int					m_nNumPoint;						//  向かうポイントの番号の加算
	float				m_fLengthPlayer;					//  プレイヤーとの距離
	float				m_fLengthPoint;						//	ポイントとの距離
															//	スプライト曲線用変数
	int					m_nCurveSwitch;						//	カーブの切り替え
	D3DXVECTOR3			m_CurvePosDest;						//	カーブする時の目的の位置
	D3DXVECTOR3			m_LengthValue[2];					//	計算用の距離を格納する値
	D3DXVECTOR3			m_posSave;							//	位置の保存
	bool				m_bCurve;							//	カーブ中かどうか
	int					m_nRotState;						//	倒れている状態
	static	int			m_nStopRoute;						//	止める車のルート

};

//=====================
//	警察
//=====================
class CCarPolice : public CCarBase
{
public://誰でも扱える
	CCarPolice();
	~CCarPolice();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetPoliceVec(void);							//	警察のベクトルの取得
	void RangeCheckOut(D3DXVECTOR3 pos, int nRoute);		//	範囲外確認
	void RangeCheckIn(D3DXVECTOR3 pos, float fLengthPlayer, int nRoute);	//	範囲内確認
	void AskPoint(D3DXVECTOR3 pos, int nRoute);				//	次のポイントを求める
	void GoPoint(D3DXVECTOR3 pos, int nRoute);				//	次のポイントに向かう

private://個人でのみ使う
	D3DXVECTOR3			m_VecA;								//  向かうベクトルの値
	int					m_nRouteCounter;					//  ルートのカウント
	int					m_nIndexPoint[INDXPOINT_MAX];		//  ポイントの番号
	int					m_nNumPoint;						//  向かうポイントの番号の加算
	float				m_fLengthPlayer;					//  プレイヤーとの距離
															//  警察
	bool			    m_bPursuer;							//	追っているかどうか
	float				m_fLengthPoint;						//	ポイントとの距離
	float				m_fLengthPointB;					//	計算用
	int					m_nNumSavePoint;					//	近いポイントの番号保存
	float				m_fLengthNextPoint;					//	近いポイントとその次のポイントの距離
	float				m_fLenPoliceNextPit;				//	警察と次のポイントの距離
	D3DXVECTOR3			m_CurvePosDest;						//	カーブする時の目的の位置
	D3DXVECTOR3			m_LengthValue[2];					//	計算用の距離を格納する値
	D3DXVECTOR3			m_posSave;							//	位置の保存
	int					m_nCurveSwitch;						//	カーブの切り替え
	int					m_nMoveState;						//	動きの状態
	float				m_fLengthFar;						//	車とポイントの距離
	bool				m_bRestart;							//	車を戻すかどうか
	CPoliceLamp			*m_pPoliceLamp;						//	警察のランプ
	int					m_nSirenCut;						//	サイレンカウンター

};

#endif