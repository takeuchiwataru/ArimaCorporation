//=============================================================================
//
// NPC車の処理 [car.cpp]
// Author : YUTARO　ABE
//
//=============================================================================
#include "scene.h"
#include "model3D.h"
#include "manager.h"
#include "car.h"
#include "meshfield.h"
#include "player.h"
#include "fade.h"
#include "car_point.h"
#include "game.h"
#include "policelamp.h"
#include "customer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// 軽自動車
#define MODEL_NAME_00 	"data\\MODEL\\NPC\\00_CarRed.x"				//	読み込むモデルファイル
#define MODEL_NAME_01 	"data\\MODEL\\NPC\\01_CarBlue.x"			//	読み込むモデルファイル
#define MODEL_NAME_02 	"data\\MODEL\\NPC\\02_CarGreen.x"			//	読み込むモデルファイル
// スポーツカー
#define MODEL_NAME_10 	"data\\MODEL\\NPC\\10_CarRed.x"				//	読み込むモデルファイル
#define MODEL_NAME_11 	"data\\MODEL\\NPC\\11_CarBlue.x"			//	読み込むモデルファイル
#define MODEL_NAME_12	"data\\MODEL\\NPC\\12_CarGreen.x"			//	読み込むモデルファイル
// バス
#define MODEL_NAME_20	"data\\MODEL\\NPC\\20_BusRed.x"				//	読み込むモデルファイル
#define MODEL_NAME_21	"data\\MODEL\\NPC\\21_BusBlue.x"			//	読み込むモデルファイル
#define MODEL_NAME_22	"data\\MODEL\\NPC\\22_BusGreen.x"			//	読み込むモデルファイル
// 警察
#define MODEL_NAME_99 	"data\\MODEL\\NPC\\99_Police.x"				//	読み込むモデルファイル
// テクスチャ
#define TEXTURE_NAME_1	"data\\TEXTURE\\modeltex\\carup.jpg"		//	読み込むテクスチャファイル

// 車の調整
#define CAR_DROW			 (8500.0f)	// 車の描画する範囲
#define CAR_POINT_LAST		 (m_nIndexPoint[CGame::GetCarRoute().nPointCount[nRoute] - 1])	//	最後のポイント
#define CAR_SPEED			 (4.75f)	// NPC車の速度
#define CAR_DEPTH			 (10)		// NPC車の幅調整用
#define CAR_HEIGHT			 (100.0f)	// NPC車の背の高さ
#define CAR_POINT_LENGTH	 (75)		// NPC車とのポイントの距離
#define CAR_PLAYER_LENGTH	 (500)		// 警察とプレイヤーの距離
//	カーブの調整する値
#define CURVE_LENGTH		 (150)		// カーブする時の範囲チェック距離[初期POINT]
#define CURVE_LENGTH_NEXT	 (15)		// カーブする時の範囲チェック距離[中間地点｜｜最終地点]
#define LAST_CURVE_LENGTH	 (15)		// 最終地点から次のポイントに向かう範囲チェック距離
#define CURVE_LENGTH_SWITCH	 (1000)		// カーブの種類の切り替え距離
#define CURVE_SWITCH_VALUE	 (1000)		// スプライトか通常のカーブの切り替える範囲の値
//警察
#define CURVE_POLICE_CHECK	 (2000)		// 警察がプレイヤーに通住する範囲
#define COLLISIOBOX_SCALE	 (D3DXVECTOR3(0.4f,0.4f,0.4f))

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPD3DXMESH CCarBase::m_pMesh[MAX_CAR] = {};							//	メッシュ情報へのポインタ
LPD3DXBUFFER CCarBase::m_pBuffMat[MAX_CAR] = {};					//	マテリアルの情報へのポインタ
DWORD CCarBase::m_nNumMat[MAX_CAR] = {};							//	マテリアルの情報数
LPDIRECT3DTEXTURE9 CCarBase::m_pMeshTextures[MAX_CAR_TEXTURE] = {};
D3DXVECTOR3 CCarBase::m_LoadVtxMax[MAX_CAR] = {};
D3DXVECTOR3 CCarBase::m_LoadVtxMin[MAX_CAR] = {};

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CCarBase::CCarBase() : CModel3D(CAR_PRIOTITY, CScene::OBJTYPE_CAR)
{
	//	変数の初期化
	m_ModelMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Spin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nSetObjectNum = 0;
	m_nTexType = 0;
	m_nCollision = 1;
	m_fAngle = 0.0f;
	m_InitPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bOtherCarHit = false;
}

//===============================================================================
//　デストラクタ
//===============================================================================
CCarBase::~CCarBase()
{
}

//===============================================================================
//　クリエイト
//===============================================================================
CCarBase * CCarBase::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nTexType, int nObjectType,
	int nCollision, int route, bool bLoop)
{
	CCarBase *pCarBase = NULL;

	// NULLチェック
	if (pCarBase == NULL)
	{// メモリの動的確保
		if (nObjectType == MODEL_POLICE_CAR)
		{//	警察
			pCarBase = new CCarPolice;
		}
		else
		{//	乗り物
			pCarBase = new CCarNpc;
		}
		if (pCarBase != NULL)
		{
			//	ループするかどうか
			pCarBase->SetLoop(bLoop);
			//	初期位置
			pCarBase->SetInitPos(pos);
			//	経路の設定
			pCarBase->SetRoute(route);
			//	テクスチャの設定
			pCarBase->m_nTexType = nTexType;
			// 種類の設定
			pCarBase->BindModel(m_pMesh[nObjectType], m_pBuffMat[nObjectType], m_nNumMat[nObjectType], m_pMeshTextures[nTexType],
								m_LoadVtxMax[nObjectType], m_LoadVtxMin[nObjectType]);
			// オブジェクトクラスの生成
			pCarBase->Init();
			// オブジェクトごとの設定用タイプ
			pCarBase->m_nType = nObjectType;
			//	回転を反映
			pCarBase->SetRot(rot);
			// 位置を代入
			pCarBase->SetPosition(pos);
			// サイズを親クラスに代入
			pCarBase->SetScale(scale);
			// コリジョンをONOFF
			pCarBase->m_nCollision = nCollision;
		}
	}
	//	値を返す
	return pCarBase;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCarBase::Init(void)
{
	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//	3Dモデルの初期化
	CModel3D::Init();

	ScaleVtxCornerPos(COLLISIOBOX_SCALE);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCarBase::Uninit(void)
{
	//3Dモデルの終了処理
	CModel3D::Uninit();
}

//=============================================================================

// 更新処理
//=============================================================================
void CCarBase::Update(void)
{
	//	3Dモデルの更新処理
	CModel3D::Update();

	if (CModel3D::GetLength() > CAR_DROW)
	{//一定距離以上だったら描画しない
		SetDraw(false);
	}
	else
	{
		SetDraw(true);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CCarBase::Draw(void)
{
	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//頂点法線の自動正規化
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	//3DモデルのDraw
	CModel3D::Draw();

	//頂点法線の自動正規化
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}

//=============================================================================
// 動きの管理
//=============================================================================
void  CCarBase::MoveMent(D3DXVECTOR3 VecA, float fAddRot, int bPoliceMoveValeu)
{
	//	位置の取得
	D3DXVECTOR3 pos = CModel3D::GetPosition();

	// 移動量の初期化
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//	向きの取得
	D3DXVECTOR3 &rot = CModel3D::GetRot();

	//	向かうベクトルの設定
	D3DXVECTOR3 m_VecA = VecA;

	float fAngle = 0.0f;								//	角度
	float fDiffese = 0.0f;								//	差分

	//	プレイヤーと車の角度を求める
	fAngle = atan2f(m_VecA.x, m_VecA.z);
	//	現在の角度と目的の角度の差を求める
	fDiffese = fAngle - m_fAngle;

	//	差分の修正
	if (fDiffese < -D3DX_PI)
	{
		fDiffese += D3DX_PI * 2.0f;
	}
	if (fDiffese > D3DX_PI)
	{
		fDiffese -= D3DX_PI * 2.0f;
	}
	//	角度の変化量を求める
	m_fAngle += fDiffese;
	//	角度の修正l
	if (m_fAngle < -D3DX_PI)
	{
		m_fAngle += D3DX_PI * 2.0f;
	}
	if (m_fAngle > D3DX_PI)
	{
		m_fAngle -= D3DX_PI * 2.0f;
	}

	if (m_nType == MODEL_POLICE_CAR)
	{//	警察
		if (CCustomer::GetPutIn() == false)
		{//	お客さんを乗車していなっかた場合
			m_rotDest.y = (D3DX_PI - m_fAngle)*-1.0f;

			move.x += sinf(fAngle) * CAR_SPEED*2.35f;
			move.z += cosf(fAngle) * CAR_SPEED*2.35f;
		}
	}
	else
	{//	警察以外
		m_rotDest.y = (D3DX_PI - m_fAngle)*-1.0f;

		move.x += sinf(fAngle) * CAR_SPEED*1.25f;
		move.z += cosf(fAngle) * CAR_SPEED*1.25f;

	}

	// 移動量の代入
	pos.x += move.x;
	pos.z += move.z;

	//	角度のブレンド
	rot = AddRod(rot, m_rotDest, fAddRot);
	// 各種情報の設定
	CModel3D::SetPosition(pos);
	CModel3D::SetMove(move);
	CModel3D::SetRot(rot);
}

//=============================================================================
//	角度を加算する
//=============================================================================
D3DXVECTOR3 CCarBase::AddRod(D3DXVECTOR3 rot, D3DXVECTOR3 rotDest, float fAddRot)
{
	//目的の向きの修正
	if (rotDest.y < -D3DX_PI)
	{
		rotDest.y += D3DX_PI * 2.0f;
	}
	else if (rotDest.y > D3DX_PI)
	{
		rotDest.y -= D3DX_PI * 2.0f;
	}
	//	回転の角度のブレンド
	float fAngle = rotDest.y - rot.y;
	if (fAngle < -D3DX_PI)
	{
		fAngle += D3DX_PI * 2.0f;
	}
	else if (fAngle > D3DX_PI)
	{
		fAngle -= D3DX_PI * 2.0f;
	}
	//	回転のブレンドする値を回転に代入
	rot.y += fAngle * fAddRot;
	//	向きの修正
	if (rot.y < -D3DX_PI)
	{
		rot.y += D3DX_PI * 2.0f;
	}
	else if (rot.y > D3DX_PI)
	{
		rot.y -= D3DX_PI * 2.0f;
	}

	//	値を返す
	return rot;
}

//===============================================================================
// Xファイルの読み込み
//===============================================================================
HRESULT CCarBase::Load(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//	モデルの名前を格納のする変数
	char cModelName[356];

	for (int nCntIndex = 0; nCntIndex < m_MaxCarModel; nCntIndex++)
	{//	車のモデルの最大数分回す
		switch (nCntIndex)
		{//	読み込むモデルの名前
		case MODEL_NOMAL_CAR_000:
			wsprintf(cModelName, MODEL_NAME_00);	//	通常の車[0]
			break;
		case MODEL_NOMAL_CAR_001:
			wsprintf(cModelName, MODEL_NAME_01);	//	通常の車[1]
			break;
		case MODEL_NOMAL_CAR_002:
			wsprintf(cModelName, MODEL_NAME_02);	//	通常の車[2]
			break;
		case MODEL_SPORTS_CAR_000:
			wsprintf(cModelName, MODEL_NAME_10);	//	スポーツカー[0]
			break;
		case MODEL_SPORTS_CAR_001:
			wsprintf(cModelName, MODEL_NAME_11);	//	スポーツカー[1]
			break;
		case MODEL_SPORTS_CAR_002:
			wsprintf(cModelName, MODEL_NAME_12);	//	スポーツカー[2]
			break;
		case MODEL_BUS_CAR_000:
			wsprintf(cModelName, MODEL_NAME_20);	//	バス[0]
			break;
		case MODEL_BUS_CAR_001:
			wsprintf(cModelName, MODEL_NAME_21);	//	バス[1]
			break;
		case MODEL_BUS_CAR_002:
			wsprintf(cModelName, MODEL_NAME_22);	//	バス[2]
			break;
		case MODEL_POLICE_CAR:
			wsprintf(cModelName, MODEL_NAME_99);	//	警察
			break;
		}

		// Xファイルの読み込み
		D3DXLoadMeshFromX(cModelName,
			D3DXMESH_SYSTEMMEM,
			pDevice, NULL,
			&m_pBuffMat[nCntIndex],
			NULL,
			&m_nNumMat[nCntIndex],
			&m_pMesh[nCntIndex]);

		int nNumVtx;		//頂点数
		DWORD sizeFVF;		//頂点フォーマットのサイズ
		BYTE *pVtxBuff;		//頂点バッファへのポインタ

		//モデルの最大値・最小値を取得する

		m_LoadVtxMax[nCntIndex] = D3DXVECTOR3(-10000, -10000, -10000);	//最大値
		m_LoadVtxMin[nCntIndex] = D3DXVECTOR3(10000, 10000, 10000);	//最小値

																			//頂点数を取得
		nNumVtx = m_pMesh[nCntIndex]->GetNumVertices();

		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(m_pMesh[nCntIndex]->GetFVF());

		//頂点バッファのロック
		m_pMesh[nCntIndex]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		//頂点座標の代入

															//最大値
			if (vtx.x > m_LoadVtxMax[nCntIndex].x)
			{
				m_LoadVtxMax[nCntIndex].x = vtx.x;
			}
			if (vtx.y > m_LoadVtxMax[nCntIndex].y)
			{
				m_LoadVtxMax[nCntIndex].y = vtx.y;
			}
			if (vtx.z > m_LoadVtxMax[nCntIndex].z)
			{
				m_LoadVtxMax[nCntIndex].z = vtx.z;
			}
			//最小値
			if (vtx.x < m_LoadVtxMin[nCntIndex].x)
			{
				m_LoadVtxMin[nCntIndex].x = vtx.x;
			}
			if (vtx.y < m_LoadVtxMin[nCntIndex].y)
			{
				m_LoadVtxMin[nCntIndex].y = vtx.y;
			}
			if (vtx.z < m_LoadVtxMin[nCntIndex].z)
			{
				m_LoadVtxMin[nCntIndex].z = vtx.z;
			}

			//サイズ文のポインタを進める
			pVtxBuff += sizeFVF;
		}

		//頂点バッファのアンロック
		m_pMesh[nCntIndex]->UnlockVertexBuffer();
	}

	//マテリアルデータへのポインタ
	D3DXMATERIAL *pMat;

	for (int nCount = 0; nCount < m_MaxCarModel; nCount++)
	{
		//マテリアル情報からテクスチャの取得
		pMat = (D3DXMATERIAL*)m_pBuffMat[nCount]->GetBufferPointer();
	}

	//	値を返す
	return S_OK;
}

//===============================================================================
// テクスチャの破棄
//===============================================================================
void CCarBase::UnLoad(void)
{
	for (int nCount = 0; nCount < m_MaxCarModel; nCount++)
	{
		// メッシュの開放
		if (m_pMesh[nCount] != NULL)
		{
			m_pMesh[nCount]->Release();
			m_pMesh[nCount] = NULL;
		}
		// マテリアルの開放
		if (m_pBuffMat[nCount] != NULL)
		{
			m_pBuffMat[nCount]->Release();
			m_pBuffMat[nCount] = NULL;
		}
	}
	//テクスチャ
	for (int nCntTex = 0; nCntTex < MAX_CAR_TEXTURE; nCntTex++)
	{
		if (m_pMeshTextures[nCntTex] != NULL)
		{
			m_pMeshTextures[nCntTex]->Release();
			m_pMeshTextures[nCntTex] = NULL;
		}
	}
}

//****************************************************************************************************************************
//　NPCの乗り物
//****************************************************************************************************************************

int CCarNpc::m_nStopRoute = NULL;

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CCarNpc::CCarNpc()
{
	//	変数の初期化
	m_VecA = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nRouteCounter = 0;
	m_nNumPoint = 0;
	m_CurvePosDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	for (int nCount = 0; nCount < 2; nCount++)
	{
		m_LengthValue[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	m_posSave = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCurveSwitch = 0;
	m_bCurve = false;
	m_fLengthPlayer = 0;
	m_fLengthPoint = 0;
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nRotState = 0;
}
//===============================================================================
//　デストラクタ
//===============================================================================
CCarNpc::~CCarNpc()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCarNpc::Init(void)
{

	//初期化処理
	CCarBase::Init();

	//	経路の取得
	int nRoute = CCarBase::GetRoute();

	//ルートの取得
	CGame::Route GameRoute = CGame::GetCarRoute();

	if (nRoute > GameRoute.nRouteCount)
	{//	ルートの最大値を超えていた場合
		nRoute = 0;
	}

	for (int nCountPoint = 0; nCountPoint < GameRoute.nPointCount[nRoute]; nCountPoint++)
	{ //	車の経路のPOINT設定
		m_nIndexPoint[nCountPoint] = GameRoute.nIndexPoint[nRoute][nCountPoint];
	}
	//	初期位置の取得
	D3DXVECTOR3 pos = CCarBase::GetInitPos();

	//ポイントの取得
	CGame::Point GamePoint = CGame::GetCarPoint();

	for (int nCountPoint = 0; nCountPoint < GameRoute.nPointCount[nRoute]; nCountPoint++)
	{//近いポイントのソート

		float fLengthPointA = sqrtf((GamePoint.pos[m_nIndexPoint[nCountPoint]].x - pos.x) * (GamePoint.pos[m_nIndexPoint[nCountPoint]].x - pos.x) +
			(GamePoint.pos[m_nIndexPoint[nCountPoint]].z - pos.z) * (GamePoint.pos[m_nIndexPoint[nCountPoint]].z - pos.z));

		float m_fLengthPointB = fLengthPointA;		//	値の代入

		if (0 == nCountPoint)
		{//	初期の値の代入
			m_nNumPoint = m_nIndexPoint[0];				//	番号の代入
			m_nRouteCounter = 0;						//	ルートのカウント代入
			m_fLengthPoint = m_fLengthPointB;			//	距離の入れ替え
		}
		else if ((m_fLengthPoint > m_fLengthPointB))
		{//	一番近い経路の設定
			m_nNumPoint = m_nIndexPoint[nCountPoint];	//	番号の代入
			m_nRouteCounter = nCountPoint;				//	ルートのカウント代入
			m_fLengthPoint = m_fLengthPointB;			//	距離の入れ替え
		}
	}
	//	初期設定
	m_nStopRoute = 99;

	SetStateRoll(false);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCarNpc::Uninit(void)
{
	//終了処理
	CCarBase::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CCarNpc::Update(void)
{
	D3DXVECTOR3 pos = CModel3D::GetPosition();				//	位置の取得
	D3DXVECTOR3 rot = CModel3D::GetRot();					//	向きの取得
	D3DXVECTOR3 move = CModel3D::GetMove();					//	動きの取得
	int nType = CCarBase::GetType();						//	タイプの取得
	CPlayer * pPlayer = CGame::GetPlayer();					// プレイヤー情報の取得

	//	位置の保存
	m_posOld = pos;
	//	更新処理
	CCarBase::Update();
	m_fLengthPlayer = CCarBase::GetLength();	//プレイヤーとの距離を取得

	// 回転行列からオイラー角を取得
	D3DXMATRIX mtx = CModel3D::GetMtx();
	float fAngleX = asinf(-mtx._32);
	float fAngleZ = atan2(mtx._12, mtx._22);

	if (fAngleX < 0.01f && fAngleX > -0.01f && fAngleZ < 0.01f && fAngleZ > -0.01f)
	{// 横転していないとき
		m_nRotState = 0;
	}
	else
	{//	横転した場合
		m_nRotState = 1;
	}

	switch (m_nRotState)
	{
	case 0:
		if (m_fLengthPlayer >= 300 && CCarBase::GetRoute() != CGame::GetStopRoute())
		{//	車がバスだった場合
			MoveMent(GetCarPointVec(), 0.0525f, m_bCurve);
		}
		break;
	case 1:
		float fMoveY = CCarBase::GetMove().y;
		SetMove(D3DXVECTOR3(0.00f, fMoveY, 0.0f));

		if (m_fLengthPlayer >= 8000)
		{//	プレイヤーとの範囲チェック
		 //	経路の取得
			int nRoute = CCarBase::GetRoute();

			//ポイントの取得
			CGame::Point GamePoint = CGame::GetCarPoint();

			for (int nCountPoint = 0; nCountPoint < CGame::GetCarRoute().nPointCount[nRoute]; nCountPoint++)
			{//近いポイントのソート
				float fLengthPointA = sqrtf((GamePoint.pos[m_nIndexPoint[nCountPoint]].x - pos.x)*(GamePoint.pos[m_nIndexPoint[nCountPoint]].x - pos.x) +
					(GamePoint.pos[m_nIndexPoint[nCountPoint]].z - pos.z)*(GamePoint.pos[m_nIndexPoint[nCountPoint]].z - pos.z));

				float fLengthPointB = fLengthPointA;	//	値の代入
				if (0 == nCountPoint)
				{//	初期の値の代入
					m_nNumPoint = m_nIndexPoint[0];		//	番号の代入
					m_nRouteCounter = 0;				//	ルートのカウント代入
					m_fLengthPoint = fLengthPointB;		//	距離の入れ替え
				}
				if ((m_fLengthPoint >= fLengthPointB) && (0 != nCountPoint))
				{//	一番近い経路の設定
					m_nNumPoint = m_nIndexPoint[nCountPoint];	//	番号の代入
					m_nRouteCounter = nCountPoint;		//	ルートのカウント代入
					m_fLengthPoint = fLengthPointB;		//	距離の入れ替え
				}
			}
			CModel3D::SetBoolBlowOff(false);
			CModel3D::InitRot();
			CModel3D::Init();
			SetPosition(D3DXVECTOR3(GamePoint.pos[m_nIndexPoint[m_nRouteCounter]].x,0.0f, GamePoint.pos[m_nIndexPoint[m_nRouteCounter]].z));
			m_nRotState = 0;
		}
		break;
	}
	if ((move.x < 0.1f && move.x > -0.1f && move.z < 0.1f && move.z > -0.1) && (m_nRotState == 0) && pPlayer->GetCrcleCarIn() == true)
	{//	車が止まった場合
		m_nStopRoute = CCarBase::GetRoute();	//	止まるルートの取得
		CGame::SetStopRoute(m_nStopRoute);		//	止まるルートの設定
		if (m_fLengthPlayer > 300 && CCarBase::GetRoute() == CGame::GetStopRoute())
		{//	プレイヤーと300以上離れている車　止まっているルートと同じ車　
			pPlayer->SetCrcleCarIn(false);

			if (pPlayer->GetCrcleCarIn() == false)
			{//	プレイヤーの範囲に車がいない場合
				CGame::SetStopRoute(99);
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CCarNpc::Draw(void)
{
	CCarBase::Draw();
}

//=============================================================================
//	車のポイントとのベクトルの取得
//=============================================================================
D3DXVECTOR3 CCarNpc::GetCarPointVec(void)
{
	D3DXVECTOR3 pos = CModel3D::GetPosition();				//	位置の取得
	int nRoute = CCarBase::GetRoute();						//	経路の取得

	//	範囲外の場合[通常の曲がり]
	GoPointNormalCurve(pos, nRoute);

	//	値を返す
	return m_VecA;
}

//=============================================================================
//	次のポイントに向かう[通常の曲がり]
//=============================================================================
void CCarNpc::GoPointNormalCurve(D3DXVECTOR3 pos, int nRoute)
{
	//ポイントの取得
	CGame::Point GamePoint = CGame::GetCarPoint();

	if (GamePoint.nNum[m_nNumPoint] == m_nNumPoint)
	{//	進む番号の設定
	 //	POINTと車の距離
		float fLength = sqrtf((GamePoint.pos[m_nNumPoint].x - pos.x) *(GamePoint.pos[m_nNumPoint].x - pos.x) +
			(GamePoint.pos[m_nNumPoint].z - pos.z) * (GamePoint.pos[m_nNumPoint].z - pos.z));
		if (fLength <= CAR_POINT_LENGTH)
		{//	範囲内条件
			m_nRouteCounter += 1;
			if (m_nNumPoint == CAR_POINT_LAST)
			{//	最後のポイントから最初のポイントへ
				bool bLoop = CCarBase::GetLoop();	//ループ状態の取得
				if (!bLoop)
				{//	車がループしない
					m_nNumPoint = m_nIndexPoint[1];	//	向かう場所のリセット
					m_nRouteCounter = 1;			//	ルートリセット
					SetPosition(D3DXVECTOR3(GamePoint.pos[m_nIndexPoint[m_nRouteCounter]].x, 0.0f, GamePoint.pos[m_nIndexPoint[m_nRouteCounter]].z));
				}
				else
				{//	車がループする
					m_nNumPoint = m_nIndexPoint[0];	//	向かう場所のリセット
					m_nRouteCounter = 0;			//	ルートリセット
				}
			}
			else
			{ //ルートの次の設定
				m_nNumPoint = m_nIndexPoint[m_nRouteCounter];
			}

		}
		//	車とポイントとの距離を求める
		m_VecA = GamePoint.pos[m_nNumPoint] - pos;
	}
}


//=============================================================================
// メッシュの当たり判定
//=============================================================================
void CCarNpc::CollisitionPriority2(void){}

//****************************************************************************************************************************
//　警察
//****************************************************************************************************************************

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CCarPolice::CCarPolice()
{
	//	変数の初期化
	m_VecA = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nRouteCounter = 0;
	m_bPursuer = false;
	m_fLengthPoint = 0.0f;
	m_fLengthPointB = 0.0f;
	m_nNumSavePoint = 0;
	m_fLengthNextPoint = 0.0f;
	m_fLenPoliceNextPit = 0.0f;
	m_nNumPoint = 0;
	m_CurvePosDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	for (int nCount = 0; nCount < 2; nCount++)
	{
		m_LengthValue[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	for (int nCount = 0; nCount < INDXPOINT_MAX; nCount++)
	{
		m_nIndexPoint[nCount] = 0;
	}

	m_posSave = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCurveSwitch = 0;
	m_bRestart = false;
	m_fLengthPlayer = 0;
	m_nMoveState = 0;
	m_fLengthFar = 0.0f;
	m_nSirenCut = 0;
	m_pPoliceLamp = NULL;
}
//===============================================================================
//　デストラクタ
//===============================================================================
CCarPolice::~CCarPolice(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCarPolice::Init(void)
{
	//	初期化処理
	CCarBase::Init();

	//	経路の設定
	int nRoute = CCarBase::GetRoute();

	//ルートの取得
	CGame::Route GameRoute = CGame::GetCarRoute();

	//ポイントの取得
	CGame::Point GamePoint = CGame::GetCarPoint();

	for (int nCountPoint = 0; nCountPoint < GameRoute.nPointCount[nRoute]; nCountPoint++)
	{//	車の経路のPOINT設定
		m_nIndexPoint[nCountPoint] = GameRoute.nIndexPoint[nRoute][nCountPoint];
	}
	//	初期位置の取得
	D3DXVECTOR3 pos = CCarBase::GetInitPos();

	for (int nCountPoint = 0; nCountPoint < GameRoute.nPointCount[nRoute]; nCountPoint++)
	{//近いポイントのソート

		float fLengthPointA = sqrtf((GamePoint.pos[m_nIndexPoint[nCountPoint]].x - pos.x)*(GamePoint.pos[m_nIndexPoint[nCountPoint]].x - pos.x) +
			(GamePoint.pos[m_nIndexPoint[nCountPoint]].z - pos.z)*(GamePoint.pos[m_nIndexPoint[nCountPoint]].z - pos.z));

		float fLengthPointB = fLengthPointA;			//	値の代入

		if (0 == nCountPoint)
		{//	初期の値の代入
			m_nNumPoint = m_nIndexPoint[0];				//	番号の代入
			m_nRouteCounter = 0;						//	ルートのカウント代入
			m_fLengthPoint = fLengthPointB;				//	距離の入れ替え
		}
		else if ((m_fLengthPoint > fLengthPointB))
		{//	一番近い経路の設定
			m_nNumPoint = m_nIndexPoint[nCountPoint];	//	番号の代入
			m_nRouteCounter = nCountPoint;				//	ルートのカウント代入
			m_fLengthPoint = fLengthPointB;				//	距離の入れ替え
		}
	}

	SetHeavy(100.0f);
	CModel3D::SetBoolRoll(false);

	if (m_pPoliceLamp == NULL)
	{
		//	警察のランプの生成
		D3DXVECTOR3 ModelPos = CModel3D::GetPosition();
		m_pPoliceLamp = CPoliceLamp::Create(D3DXVECTOR3(ModelPos.x, ModelPos.y + 50, ModelPos.z - 100), D3DXVECTOR2(20.0f, 20.0f));
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCarPolice::Uninit(void)
{
	//終了処理
	CCarBase::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CCarPolice::Update(void)
{
	//	更新処理
	CCarBase::Update();

	D3DXVECTOR3 pos = CModel3D::GetPosition();				//	位置の取得
	D3DXVECTOR3 rot = CModel3D::GetRot();					//	向きの取得
	D3DXVECTOR3 move = CModel3D::GetMove();					//	動きの取得
	int nType = CCarBase::GetType();						//	タイプの取得
	CGame::Point GamePoint = CGame::GetCarPoint();			//	ポイントの取得

															//	車とポイントの距離
	m_fLengthFar = sqrtf((GamePoint.pos[m_nNumPoint].x - pos.x)*(GamePoint.pos[m_nNumPoint].x - pos.x) +
		(GamePoint.pos[m_nNumPoint].z - pos.z)*(GamePoint.pos[m_nNumPoint].z - pos.z));

	//	警察とプレイヤーの距離
	m_fLengthPlayer = CModel3D::GetLength();

	//	更新処理
	CCarBase::Update();

	if (m_fLengthFar < 4000 || m_fLengthPlayer <= 1000)
	{//	移動する
		m_nMoveState = 0;
		m_bRestart = false;
	}
	else if (m_bPursuer == true && m_fLengthPlayer >= 1000)
	{
		m_bPursuer = false;
		m_bRestart = true;
		m_nMoveState = 1;
	}
	else
	{
		m_bRestart = false;
	}

	switch (m_nMoveState)
	{
	case 0:
		if (m_bRestart == false)
		{//	移動する
			MoveMent(GetPoliceVec(), 0.0525f, false);
		}
		break;
	case 1:
		SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		if (m_fLengthPlayer >= 4000 && m_fLengthFar > 3500)
		{
			//	経路の取得
			int nRoute = CCarBase::GetRoute();

			for (int nCountPoint = 0; nCountPoint < CGame::GetCarRoute().nPointCount[nRoute]; nCountPoint++)
			{//近いポイントのソート
				float fLengthPointA = sqrtf((GamePoint.pos[m_nIndexPoint[nCountPoint]].x - pos.x)*(GamePoint.pos[m_nIndexPoint[nCountPoint]].x - pos.x) +
					(GamePoint.pos[m_nIndexPoint[nCountPoint]].z - pos.z)*(GamePoint.pos[m_nIndexPoint[nCountPoint]].z - pos.z));

				float fLengthPointB = fLengthPointA;	//	値の代入
				if (0 == nCountPoint)
				{//	初期の値の代入
					m_nNumPoint = m_nIndexPoint[0];		//	番号の代入
					m_nRouteCounter = 0;				//	ルートのカウント代入
					m_fLengthPoint = fLengthPointB;		//	距離の入れ替え
				}
				if ((m_fLengthPoint >= fLengthPointB) && (0 != nCountPoint))
				{//	一番近い経路の設定
					m_nNumPoint = m_nIndexPoint[nCountPoint];	//	番号の代入
					m_nRouteCounter = nCountPoint;		//	ルートのカウント代入
					m_fLengthPoint = fLengthPointB;		//	距離の入れ替え
				}
			}
			CModel3D::SetBoolBlowOff(false);
			CModel3D::InitRot();
			CModel3D::Init();
			SetPosition(D3DXVECTOR3(GamePoint.pos[m_nIndexPoint[m_nRouteCounter]].x, 0.0f, GamePoint.pos[m_nIndexPoint[m_nRouteCounter]].z));
			m_nMoveState = 0;
			m_bRestart = false;
		}
		break;
	}
	if (m_bPursuer == false)
	{//	追っていない
		m_nSirenCut = 0;
		if (m_pPoliceLamp != NULL)
		{
			m_pPoliceLamp->SetPosSize(D3DXVECTOR3(pos.x, pos.y + 60, pos.z), D3DXVECTOR2(0.0f, 0.0f));
			m_pPoliceLamp->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
		}
	}

	if (move.x < 0.1f && move.z < 0.1f&&move.x >- 0.1f && move.z > -0.1f)
	{
		// 追う状態でないときサイレンを止める
		CManager::GetSound()->StopSound(CSound::SOUND_LABEL_BGM_POLICE_SIREN);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CCarPolice::Draw(void)
{
	//	描画処理
	CCarBase::Draw();

}
//=============================================================================
//	警察とプレイヤーとのベクトルの取得
//=============================================================================
D3DXVECTOR3 CCarPolice::GetPoliceVec(void)
{
	//位置の取得
	D3DXVECTOR3 pos = CModel3D::GetPosition();
	//	経路の取得
	int nRoute = CCarBase::GetRoute();

	 if(m_fLengthPlayer < CURVE_SWITCH_VALUE)
	{//	範囲内
		RangeCheckIn(pos, m_fLengthPlayer, nRoute);
	}
	else if (m_fLengthPlayer > CURVE_SWITCH_VALUE)
	{//	範囲外の場合
		RangeCheckOut(pos, nRoute);

		// 追う状態でないときサイレンを止める
		CManager::GetSound()->StopSound(CSound::SOUND_LABEL_BGM_POLICE_SIREN);
		m_bPursuer = false;
	}
	else
	{
		// 追う状態でないときサイレンを止める
		CManager::GetSound()->StopSound(CSound::SOUND_LABEL_BGM_POLICE_SIREN);
	}



	//	値を返す
	return m_VecA;
}

//=============================================================================
//	警察とプレイヤーの距離が範囲外の場合
//=============================================================================
void CCarPolice::RangeCheckOut(D3DXVECTOR3 pos, int nRoute)
{
	//ポイントの取得
	CGame::Point GamePoint = CGame::GetCarPoint();

	if (GamePoint.nNum[m_nNumPoint] == m_nNumPoint)
	{//	進む番号の設定
	 //	POINTと車の距離
		float fLength = sqrtf((GamePoint.pos[m_nNumPoint].x - pos.x)*(GamePoint.pos[m_nNumPoint].x - pos.x) +
			(GamePoint.pos[m_nNumPoint].z - pos.z)*(GamePoint.pos[m_nNumPoint].z - pos.z));

		if (fLength <= CAR_POINT_LENGTH)
		{//	範囲内条件
			m_nRouteCounter += 1;
			if (m_nNumPoint == CAR_POINT_LAST)
			{//	最後のポイントから最初のポイントへ

				bool bLoop = CCarBase::GetLoop();	//ループ状態の取得

				if (!bLoop)
				{//	車がループしない
					m_nNumPoint = m_nIndexPoint[1];	//	向かう場所のリセット
					m_nRouteCounter = 1;			//	ルートリセット
					SetPosition(D3DXVECTOR3(GamePoint.pos[m_nIndexPoint[0]].x, 0.0f, GamePoint.pos[m_nIndexPoint[0]].z));
				}
				else
				{//	車がループする
					m_nNumPoint = m_nIndexPoint[0];	//	向かう場所のリセット
					m_nRouteCounter = 0;			//	ルートリセット
				}
			}
			else
			{ //ルートの次の設定
				m_nNumPoint = m_nIndexPoint[m_nRouteCounter];
			}


		}
		//	車とポイントとの距離を求める
		m_VecA = GamePoint.pos[m_nNumPoint] - pos;
	}
}
//=============================================================================
//	警察とプレイヤーの距離が範囲内の場合
//=============================================================================
void CCarPolice::RangeCheckIn(D3DXVECTOR3 pos, float fLengthPlayer, int nRoute)
{
	if ((fLengthPlayer <= CURVE_POLICE_CHECK))
	{//	範囲内条件
		m_nSirenCut++;
		if (CGame::GetGameCounter() > 500)
		{//	ゲームの時間が6秒以上たった場合
			if (!m_bPursuer) { CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_POLICE_SIREN); }
		}
		m_bPursuer = true;								//	追っている
		m_VecA = CGame::GetPlayer()->GetPos() - pos;	//	警察とプレイヤーとのベクトルの取得
		if (m_pPoliceLamp != NULL)
		{
			D3DXVECTOR3 ModelPos = CModel3D::GetPosition();

			if ((m_nSirenCut < 15))
			{//	ランプの点灯
				m_pPoliceLamp->SetPosSize(D3DXVECTOR3(ModelPos.x, ModelPos.y + 60, ModelPos.z), D3DXVECTOR2(20.0f, 20.0f));
				m_pPoliceLamp->SetColor(D3DXCOLOR(1.0f, 0.2f, 0.1f, 1.0f));
			}
			else if ((m_nSirenCut < 30))
			{//	ランプの点灯
				m_pPoliceLamp->SetPosSize(D3DXVECTOR3(ModelPos.x, ModelPos.y + 60, ModelPos.z), D3DXVECTOR2(25.0f, 25.0f));
				m_pPoliceLamp->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}
			else if ((m_nSirenCut > 30))
			{//	カウンターのリセット
				m_nSirenCut = 0;
			}
		}
	}
	if ((m_bPursuer == true) && (fLengthPlayer >= CURVE_POLICE_CHECK))
	{
		//ポイントの取得
		CGame::Point GamePoint = CGame::GetCarPoint();
		//次のポイントを求める
		AskPoint(pos, nRoute);
		m_bPursuer = false;
	}
	else if ((m_bPursuer == false) && (fLengthPlayer >= CURVE_POLICE_CHECK))
	{//	次のポイントに向かう
		GoPoint(pos, nRoute);
	}
}

//=============================================================================
//	次のポイントを求める
//=============================================================================
void  CCarPolice::AskPoint(D3DXVECTOR3 pos, int nRoute)
{
	//ポイントの取得
	CGame::Point GamePoint = CGame::GetCarPoint();

	for (int nCountPoint = 0; nCountPoint < CGame::GetCarRoute().nPointCount[nRoute]; nCountPoint++)
	{//近いポイントのソート
		float fLengthPointA = sqrtf((GamePoint.pos[m_nIndexPoint[nCountPoint]].x - pos.x)*(GamePoint.pos[m_nIndexPoint[nCountPoint]].x - pos.x) +
			(GamePoint.pos[m_nIndexPoint[nCountPoint]].z - pos.z)*(GamePoint.pos[m_nIndexPoint[nCountPoint]].z - pos.z));

		m_fLengthPointB = fLengthPointA;		//	値の代入
		if (0 == nCountPoint)
		{//	初期の値の代入
			m_nNumPoint = m_nIndexPoint[0];		//	番号の代入
			m_nRouteCounter = 0;				//	ルートのカウント代入
			m_fLengthPoint = m_fLengthPointB;	//	距離の入れ替え
			m_bPursuer = false;					//	追っていない
		}
		if ((m_fLengthPoint >= m_fLengthPointB) && (0 != nCountPoint))
		{//	一番近い経路の設定
			m_nNumPoint = m_nIndexPoint[nCountPoint];	//	番号の代入
			m_nRouteCounter = nCountPoint;		//	ルートのカウント代入
			m_fLengthPoint = m_fLengthPointB;	//	距離の入れ替え
			m_bPursuer = false;					//	追っていない
		}
	}
	if (m_nNumPoint == CAR_POINT_LAST)
	{//	近いポイントが最後のポイントの場合
	 /*	一番近いポイントと次のポイントの距離*/
		m_fLengthNextPoint = sqrtf((GamePoint.pos[m_nNumPoint].x - GamePoint.pos[m_nIndexPoint[0]].x)*(GamePoint.pos[m_nNumPoint].x - GamePoint.pos[m_nIndexPoint[0]].x) +
			(GamePoint.pos[m_nNumPoint].z - GamePoint.pos[m_nIndexPoint[0]].z)*(GamePoint.pos[m_nNumPoint].z - GamePoint.pos[m_nIndexPoint[0]].z));
		/*	警察と次のポイントの距離*/
		m_fLenPoliceNextPit = sqrtf((pos.x - GamePoint.pos[m_nIndexPoint[0]].x)*(pos.x - GamePoint.pos[m_nIndexPoint[0]].x) +
			(pos.z - GamePoint.pos[m_nIndexPoint[0]].z)*(pos.z - GamePoint.pos[m_nIndexPoint[0]].z));
	}
	else
	{//	それ以外
	 /*	一番近いポイントと次のポイントの距離*/
		m_fLengthNextPoint = sqrtf((GamePoint.pos[m_nNumPoint].x - GamePoint.pos[m_nNumPoint + 1].x)*(GamePoint.pos[m_nNumPoint].x - GamePoint.pos[m_nNumPoint + 1].x) +
			(GamePoint.pos[m_nNumPoint].z - GamePoint.pos[m_nNumPoint + 1].z)*(GamePoint.pos[m_nNumPoint].z - GamePoint.pos[m_nNumPoint + 1].z));
		/*	警察と次のポイントの距離*/
		m_fLenPoliceNextPit = sqrtf((pos.x - GamePoint.pos[m_nNumPoint + 1].x)*(pos.x - GamePoint.pos[m_nNumPoint + 1].x) +
			(pos.z - GamePoint.pos[m_nNumPoint + 1].z)*(pos.z - GamePoint.pos[m_nNumPoint + 1].z));
	}
	if (m_fLengthNextPoint > m_fLenPoliceNextPit)
	{//	距離の比較
		if (m_nNumPoint == CAR_POINT_LAST)
		{//	向かうポイントが最大ならリセット
			m_nNumPoint = m_nIndexPoint[0];
		}
		else
		{//それ以外
			m_nNumPoint = m_nIndexPoint[m_nNumPoint] + 1;
		}
	}
}

//=============================================================================
//	次のポイントに向かう
//=============================================================================
void  CCarPolice::GoPoint(D3DXVECTOR3 pos, int nRoute)
{
	//ポイントの取得
	CGame::Point GamePoint = CGame::GetCarPoint();

	if (GamePoint.nNum[m_nNumPoint] == m_nNumPoint)
	{//	進む番号の設定
	 //	向かうポイントとプレイヤーの距離
		float fLength = sqrtf((GamePoint.pos[m_nNumPoint].x - pos.x)*(GamePoint.pos[m_nNumPoint].x - pos.x) +
			(GamePoint.pos[m_nNumPoint].z - pos.z)*(GamePoint.pos[m_nNumPoint].z - pos.z));
		if (fLength >= CURVE_LENGTH)
		{// 位置を保存する
			m_posSave = pos;
		}
		//	計算用の値[0]
		m_LengthValue[0] = D3DXVECTOR3(m_posSave.x + ((GamePoint.pos[m_nNumPoint].x - m_posSave.x) / 2),
			m_posSave.y, m_posSave.z + ((GamePoint.pos[m_nNumPoint].z - m_posSave.z) / 2));

		//	車が曲がる最終地点
		D3DXVECTOR3 Value = GamePoint.pos[m_nNumPoint];
		if ((m_nNumPoint % 4) == 0)
		{//	余りを割り出して交差点の最終地点の設定 //	最終地点の値
			Value.x = GamePoint.pos[m_nNumPoint].x - CURVE_LENGTH;
		}
		else if ((m_nNumPoint % 4) == 1)
		{//	余りを割り出して交差点の最終地点の設定 //	最終地点の値
			Value.z = GamePoint.pos[m_nNumPoint].z - CURVE_LENGTH;
		}
		else if ((m_nNumPoint % 4) == 2)
		{//	余りを割り出して交差点の最終地点の設定 //	最終地点の値
			Value.x = GamePoint.pos[m_nNumPoint].x + CURVE_LENGTH;
		}
		else if ((m_nNumPoint % 4) == 3)
		{//	余りを割り出して交差点の最終地点の設定 //	最終地点の値
			Value.z = GamePoint.pos[m_nNumPoint].z + CURVE_LENGTH;
		}

		//	計算用の値[1]
		m_LengthValue[1] = D3DXVECTOR3(Value.x + ((GamePoint.pos[m_nNumPoint].x - Value.x) / 2),
			Value.y, Value.z + ((GamePoint.pos[m_nNumPoint].z - Value.z) / 2));

		//	[0][1]の中点
		m_CurvePosDest = D3DXVECTOR3(
			m_LengthValue[0].x - ((m_LengthValue[0].x - m_LengthValue[1].x) / 2),
			m_LengthValue[0].y,
			m_LengthValue[0].z - ((m_LengthValue[0].z - m_LengthValue[1].z) / 2));

		//	カーブの中間地点と車の距離
		float fLengthA = sqrtf((m_CurvePosDest.x - pos.x)*(m_CurvePosDest.x - pos.x) +
			(m_CurvePosDest.z - pos.z)*(m_CurvePosDest.z - pos.z));

		//	最終地点と車の距離
		float fLengthB = sqrtf((Value.x - pos.x)*(Value.x - pos.x) +
			(Value.z - pos.z)*(Value.z - pos.z));

		if (fLengthB <= 45 && m_nCurveSwitch == 0)
		{
			m_nRouteCounter += 1;
			if (m_nNumPoint == CAR_POINT_LAST)
			{//	最後のポイントから最初のポイントへ

				bool bLoop = CCarBase::GetLoop();	//ループ状態の取得

				if (!bLoop)
				{//	車がループしない
					m_nNumPoint = m_nIndexPoint[1];	//	向かう場所のリセット
					m_nRouteCounter = 1;			//	ルートリセット
					SetPosition(D3DXVECTOR3(GamePoint.pos[m_nIndexPoint[0]].x, 0.0f, GamePoint.pos[m_nIndexPoint[0]].z));
				}
				else
				{//	車がループしない
					m_nNumPoint = m_nIndexPoint[0];	//	向かう場所のリセット
					m_nRouteCounter = 0;			//	ルートリセット
				}
			}
			else
			{//	ルートの次の設定
				m_nNumPoint = m_nIndexPoint[m_nRouteCounter];
			}


		}
		//	車の向かうポイントの切り替え
		switch (m_nCurveSwitch)
		{
		case 0:	//	次のポイントへ向かう
			m_VecA = GamePoint.pos[m_nNumPoint] - pos;
			//	次の動きへ
			if (fLength <= CURVE_LENGTH && fLengthA < 100) { m_nCurveSwitch = 1; }
			break;
		case 1:	//	カーブの中間地点に向かう
			m_VecA = m_CurvePosDest - pos;
			//	次の動きへ
			if (fLengthA <= CURVE_LENGTH_NEXT) { m_nCurveSwitch = 2; }
			break;
		case 2:	//	カーブの最終地点へ向かう
			m_VecA = Value - pos;
			//	次の動きへ
			if (fLengthB <= LAST_CURVE_LENGTH) { m_nCurveSwitch = 0; }
			break;
		}
	}
}
