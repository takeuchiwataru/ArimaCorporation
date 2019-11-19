//=============================================================================
//
// プレイヤーの処理 [player.cpp]
// Author : 坂川詠祐
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"
#include "camerabace.h"
#include "scene.h"
#include "model.h"
#include "motion.h"
#include "loadText.h"
#include "wall.h"
#include "incline.h"
#include "sound.h"
#include "tutorial.h"
#include "gamecamera.h"
#include "object.h"
#include "feed.h"
#include "egg.h"
#include "billboord.h"
#include "chick.h"
#include "ColMesh.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define VECTOR_ZERO		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))				//ベクトルの初期化
#define FAILE_NAME		("data\\TEXT\\Player\\PlayerState.txt")		//読み込むファイル名
#define FILE_TEXTURE	("data\\TEXTURE\\modeltex\\ニワトリ.jpg")	//テクスチャの読み込み
#define AVERAGE			(2.0f)										//平均する値
#define ROLLOVER_STOP	(0.6f)										//横転防止角度
#define GRAVITY_BACK	(0.2f)										//後方の重力
#define DECELERATION	(0.5f)										//減速の割合
#define EGG_RANGE		(50.0f)										// 卵とプレイヤーの距離
#define EGG_POS			(7)											// 卵同士の間隔の広さ（増やすと広くなる）
#define SPEEDUP_TIME	(60)										// 加速している時間
#define SPEEDDOWN		(0.95f)										// 減速させる値
#define CHICK_SCALE		(D3DXVECTOR3(1.0f, 1.0f, 1.0f))				//ひよこの大きさ
#define THROW			(13.0f)										// 卵を投げる力
#define EGG_RAND		(2.0f)										// 卵に乗るときのジャンプ力
#define EGG_HEIGHT		(40.0f)										// 卵に乗ったように見える高さ

// プレイヤー情報
#define PLAYER_ACCEL	(0.5f)										//加速値（前進）
#define PLAYER_BRAKS	(-0.2f)										//加速値（後進）
#define PLAYER_DOWN		(0.08f)										//減速度
#define PLAYER_ADDROT	(0.005f)									//回転量
#define PLAYER_DOWNROT	(0.2f)										//回転量

#define PLAYER_JUMP		(2.0f)										//回転量
#define PLAYER_GRAVITY	(0.09f)										//回転量

//車体の角度
#define SHAKE_X			(0.007f)									//X軸の揺れ
#define INPULS_X		(0.01f)										//X軸に加える角度
#define SHAKE_Z			(0.02f)										//Z軸の揺れ
#define SHAKE_DRIFT		(0.005f)									//ドリフト時の角度加算
#define SHAKE_BRAK		(0.01f)										//ブレーキ時の角度

#define RIVER_SOUND_RANGE		(1000.0f)							// 川の音が聞こえる範囲
#define FOUNTAIN_SOUND_RANGE	(1000.0f)							// 噴水の音が聞こえる範囲

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
//CModel * CPlayer::m_pModel = NULL;		//モデルのパーツポインタ
//int	CPlayer::m_nMaxModel = 0;
//int CPlayer::m_nMaxParts = 0;
//int CPlayer::m_nMaxMotion = 0;
//CMotion::MOTION_INFO * CPlayer::m_pMotionInfo = NULL;
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;
CChick *CPlayer::m_pAnnoyChick[MAX_MEMBER] = {};

LPD3DXMESH	CPlayer::m_pMesh[MAX_PARTS] = {};					//メッシュ情報の初期化
LPD3DXBUFFER CPlayer::m_pBuffMat[MAX_PARTS] = {};				//マテリアルの情報の初期化
DWORD CPlayer::m_nNumMat[MAX_PARTS] = {};						//マテリアルの情報数の初期化

//--------------------------------------------
//グローバル変数
//--------------------------------------------
int g_nNumModel;
char g_aFileNameModel[MAX_PARTS][256];

//=============================================================================
// 生成処理
//=============================================================================
CPlayer * CPlayer::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, int nPlayerNum, int nControllerNum, PLAYERTYPE playerType)
{
	//インスタンスの生成
	CPlayer * pPlayer;
	pPlayer = new CPlayer;

	//初期化処理
	pPlayer->Init();
	pPlayer->m_nPlayerNum = nPlayerNum;
	pPlayer->m_nControllerNum = nControllerNum;
	pPlayer->m_PlayerType = playerType;

	//設定処理
	pPlayer->Set(pos, VECTOR_ZERO);
	pPlayer->SetRot(rot);

	return pPlayer;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CPlayer::CPlayer() : CScene(3, OBJTYPE_PLAYER)
{
	//値の初期化
	for (int nCount = 0; nCount < MAX_PARTS; nCount++)
	{
		m_apModel[nCount] = NULL;
	}

	m_pPlayerNum = NULL;
	m_pPlayerpos = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CPlayer::~CPlayer() {}

//=============================================================================
// Xファイルの読み込み
//=============================================================================
void CPlayer::LoadModel(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice,
		FILE_TEXTURE,
		&m_pTexture);

	//モーション情報の取得
	//CLoadTextMotion * pLoadTextMotion = NULL;
	CManager::MODE mode = CManager::GetMode();

	//pLoadTextMotion = CTutorial::GetPlayerMotion();

	//m_pMotionInfo = pLoadTextMotion->GetMotionInfo();					//モーション情報の取得
	//m_nMaxMotion = pLoadTextMotion->GetMaxMotion();						//モーションの最大数の取得

	//																	//モデル情報を取得
	//CLoadTextMotion::MODEL_INFO ModelInfo = pLoadTextMotion->GetModelInfo();
	//m_nMaxModel = ModelInfo.nMaxModel;	//モデルの最大数の取得
	//m_nMaxParts = ModelInfo.nMaxParts;	//モデルのパーツ最大数の取得

	//									//モデルの生成
	//for (int nCntParts = 0; nCntParts < ModelInfo.nMaxParts; nCntParts++)
	//{
	//	m_pModel = CModel::Create(ModelInfo.pOfSetPos[nCntParts], &ModelInfo.paFileName[nCntParts][0]);

	//	//テクスチャの割当て
	//	if (m_pModel != NULL) { m_pModel->BindTexture(m_pTexture); }
	//}

	////モデルの親設定
	//for (int nCntParts = 0; nCntParts < ModelInfo.nMaxParts; nCntParts++)
	//{
	//	if (nCntParts == 0)
	//	{
	//		m_pModel->SetParent(NULL);
	//		break;
	//	}
	//}
}

//=============================================================================
// Xファイルの破棄
//=============================================================================
void CPlayer::UnloadModel(void)
{
	////モーションデータの破棄
	//if (m_pMotionInfo != NULL)
	//{
	//	m_pMotionInfo = NULL;
	//}

	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer::Init(void)
{
	//変数の初期化
	m_pos = VECTOR_ZERO;				//中心座標
	m_OldPos = VECTOR_ZERO;				//前回の座標
	m_move = VECTOR_ZERO;				//移動
	m_pos = VECTOR_ZERO;
	m_rot = VECTOR_ZERO;				//向き
	m_nCntFrame = -1;				//向き
	m_OldDiffuse = VECTOR_ZERO;				//前回の差分
	m_MoveMotion = VECTOR_ZERO;				//モーションの移動
	m_vtxMaxModel = VECTOR_ZERO;			//モデルの頂点最大値
	m_vtxMinModel = VECTOR_ZERO;			//モデルの頂点最小値
	m_pMotion = NULL;						//モーションポインタ
	m_pText = NULL;							//プレイヤーのテキストポインタ
	m_StateSpeed = STATE_SPEED_STOP;		//スピードの状態設定
	m_StateHandle = HANDLE_MAX;				//ハンドルの状態
	m_PlayerInfo.nCountTime = 0;			//カウンター
	m_PlayerInfo.fAccel = PLAYER_ACCEL;		//加速値（前進）
	m_PlayerInfo.fBraks = PLAYER_BRAKS;		//加速値（後進）
	m_PlayerInfo.fDown = PLAYER_DOWN;		//減速値
	m_PlayerInfo.fAddRot = PLAYER_ADDROT;	//加える回転値
	m_PlayerInfo.fDistance = 0.0f;			//距離
	m_PlayerInfo.FirstPos = VECTOR_ZERO;	//初期位置
	m_bJump = true;							//ジャンプ状態
	m_bControl = false;						//コントローラーフラグ
	m_nCountJumpTime = 0;					//ジャンプ状態の時間を加算
	m_fvtxMaxY = 0.0f;						//モデル頂点の最大値（Y）
	m_fMass = 200.0f;						// 質量
	m_fSpeed = 0.0f;						// 速さ
	m_nCountSpeed = 0;
	m_nCountTime = 0;						// 時間カウンター
	m_bCrcleCarIn = false;
	m_pLoadEffect = NULL;					// エフェクトツールポインタ
	m_nCntCombo = 0;
	m_nCntShake = 0;
	m_bShake = true;
	m_nCntFlag = 0;
	m_bDirive = true;
	m_nNumEgg = 0;
	m_nNumChick = 0;
	m_nNumItem = 0;
	m_bJumpSave = false;
	m_nCntDamage = 0;
	m_bDamage = false;
	m_State = PLAYERSTATE_NORMAL;
	m_nPlayerRank = 0;
	m_nCntChick = 0;
	m_nDestRank = 0;
	m_nAnnoySTimer = 0;
	m_bAnnoyS = false;

	m_nDriftCounter = 0;		// ドリフトカウント

	m_pPoint = CRoad_Manager::GetManager()->GetTop(0);
	m_fLength = 5.0f;
	m_bDivided = false;
	m_nMap = 0;
	m_fRoad = 0.0f;
	m_FNor = INIT_VECTOR;
	m_fTilt = 0.0f;
	m_fCTilt = 0.0f;

	m_nPlayerNum = 0;					// プレイヤー番号
	m_nControllerNum = 0;				// コントローラー番号

	m_nAnimnow = PLAYERANIM_NEUTRAL;	//ニュートラル状態
	m_nCountFlame = 0;
	m_nKey = 0;

	m_bGoal = false;					// ゴール
	m_fAddRot = 0.0f;					// 加算角度

	// プレイヤー番号（追従）
	if (m_pPlayerNum == NULL)
		m_pPlayerNum = CBillBoord::Create(m_pos + D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXVECTOR2(10.0f, 10.0f), 1);

	if (m_pPlayerpos == NULL)
		m_pPlayerpos = CBillBoord::Create(m_pos + D3DXVECTOR3(0.0f, 500.0f, 0.0f), D3DXVECTOR2(300.0f, 300.0f), 0, true);

	for (int nCntEggPos = 0; nCntEggPos < MAX_FRAME; nCntEggPos++)
	{
		m_OldEggRot[nCntEggPos] = VECTOR_ZERO;
		m_abJump[nCntEggPos] = false;
	}

	//卵のポインタを初期化
	for (int nCntEgg = 0; nCntEgg < MAX_EGG; nCntEgg++)
	{
		m_pEgg[nCntEgg] = NULL;
		m_pChick[nCntEgg] = NULL;

		m_bulletType[nCntEgg] = BULLET_EGG_ATTACK;
	}

	for (int nCntChick = 0; nCntChick < MAX_PLAYER; nCntChick++)
	{// 減速鶏のポインタを初期化
		m_pAnnoyChick[nCntChick] = NULL;
	}

	FileLoad();

	//テクスチャの割当て
	for (int nCount = 0; nCount < MAX_PARTS; nCount++)
	{
		if (m_apModel[nCount] != NULL) { m_apModel[nCount]->BindTexture(m_pTexture); }
	}

	//if (m_pMotion == NULL)	//モーションの生成
	//{
	//	m_pMotion = CMotion::Create();

	//	if (m_pMotion != NULL)
	//	{
	//		//モデルデータの設定
	//		m_pMotion->SetModel(&m_pModel);			//モーションデータにモデル情報を渡す
	//		m_pMotion->SetMaxModel(m_nMaxParts);	//モデルパーツの最大数を渡す

	//												//モーションデータの設定
	//		m_pMotion->SetMotion(m_pMotionInfo);	//モーション情報の取得
	//		m_pMotion->SetMaxMotion(m_nMaxMotion);	//モーションの最大数の取得

	//												//初期化処理
	//		m_pMotion->Init();
	//	}
	//}

	//モデルの最小値・最大値の取得
	//m_vtxMaxModel = m_pModel->GetVtxMax();
	//m_vtxMinModel = m_pModel->GetVtxMin();
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	if (m_pPlayerNum != NULL)
	{
		m_pPlayerNum->Uninit();
		m_pPlayerNum = NULL;
	}

	if (m_pPlayerpos != NULL)
	{
		m_pPlayerpos->Uninit();
		m_pPlayerpos = NULL;
	}

	////モーションの破棄
	//if (m_pMotion != NULL)
	//{
	//	m_pMotion->Uninit();
	//	delete m_pMotion;
	//	m_pMotion = NULL;
	//}

	//タイヤモデルの破棄
	for (int nCntEgg = 0; nCntEgg < MAX_EGG; nCntEgg++)
	{
		if (m_pEgg[nCntEgg] != NULL)
		{
			m_pEgg[nCntEgg] = NULL;
		}

		if (m_pChick[nCntEgg] != NULL)
		{
			m_pChick[nCntEgg] = NULL;
		}
	}

	for (int nCntChick = 0; nCntChick < MAX_PLAYER; nCntChick++)
	{
		if (m_pAnnoyChick[nCntChick] != NULL)
		{
			m_pChick[nCntChick] = NULL;
		}
	}

	for (int nCount = 0; nCount < MAX_PARTS; nCount++)
	{
		if (m_apModel[nCount] != NULL)
		{
			//3DモデルのUninit
			m_apModel[nCount]->Uninit();
			delete m_apModel[nCount];
			m_apModel[nCount] = NULL;
		}
	}

	//テキストの破棄
	if (m_pText != NULL)
	{
		m_pText->Uninit();
		delete m_pText;
		m_pText = NULL;
	}

	//死亡フラグを立てる
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPlayer::Update(void)
{
	//変数宣言
	D3DXVECTOR3 TirePos[MAX_TIRE];
	CManager::MODE mode = CManager::GetMode();
	CGameCamera * pGameCamera = NULL;
	m_bShake = true;

	if (m_bControl)
	{//コントロールキー
		if (m_bGoal == false)
		{
			if (m_PlayerType == PLAYERTYPE_PLAYER)
				ControlKey();
			else
				UpdateAI();
		}
		else
		{

		}
	}
	else
	{//ハンドルを握っていない状態にする
		SetStateHandle(HANDLE_MAX);
		SetStateSpeed(STATE_SPEED_DOWN);
	}

	//タイムアップ状態なら以降は更新しない
	//if (CTime::GetTime() == 0 && CManager::MODE_GAME == CManager::GetMode()) { return; }

	UpdateMove();			// 移動処理

	UpdateField();

	/*CollisionFeed();		// 餌の当たり判定

	CollisionEgg();			// 卵との当たり判定

	CollisionChick();		// ひよことの当たり判定


	ChaseEgg();				// 卵がついてくる処理

	CollisionCharacter();	// キャラクター同士の当たり判定
	//CollisionObject();		// オブジェクトとの当たり判定

	ChickAppear();

	// 強い減速ひよこがくっつく
	ChaseAnnoyS();*/

							//マップとの当たり判定
	if (!CCOL_MESH_MANAGER::Collision(m_pos, m_OldPos, m_move, m_fLength, m_FNor, m_bJump, m_nMap, true)) { m_bJump = false; }

	if (m_pPlayerNum != NULL)
	{
		m_pPlayerNum->SetPosSize(m_pos + D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXVECTOR2(10.0f, 10.0f));

		if (m_PlayerType == CPlayer::PLAYERTYPE_PLAYER)
			m_pPlayerNum->SetTexture(m_nPlayerNum, 5, 1, 1);
		else
			m_pPlayerNum->SetTexture(4, 5, 1, 1);
	}

	if (m_pPlayerpos != NULL)
	{
		m_pPlayerpos->SetPosSize(m_pos + D3DXVECTOR3(0.0f, 500.0f, 0.0f), D3DXVECTOR2(300.0f, 300.0f));

		if (m_PlayerType == PLAYERTYPE_PLAYER)
			m_pPlayerpos->SetColor(D3DXCOLOR((m_nPlayerNum % 2 == 0 ? 1.0f : 0.0f), (m_nPlayerNum / 2 == 1 ? 1.0f : 0.0f), (m_nPlayerNum == 1 ? 1.0f : 0.0f), 1.0f));
		else
			m_pPlayerpos->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	m_nMotionType = m_nAnimnow;

	//モーション更新
	UpdateMotion();

	DebugProc();		// デバック表示
}

//=============================================================================
// 描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	//レンダリングクラスを取得
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX		  mtxRot, mtxTrans;			// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCount = 0; nCount < MAX_PARTS; nCount++)
	{//モデルの描画
		if (m_apModel[nCount] != NULL)
		{
			//描画する
			m_apModel[nCount]->Draw(1.0f);
		}
	}
}
//=============================================================================
// AIの更新処理
//=============================================================================
void CPlayer::UpdateAI(void)
{
	CGame::GetGameCamera(m_nPlayerNum);
	m_fRoad = m_nPlayerNum * (140.0f / 8.0f) - 70.0f;
	bool bGoal = false;
	float fRot = CRoad_Pointer::NextPoint(m_pos, m_pPoint, m_nPlayerNum, m_fRoad, m_bDivided, bGoal, m_nMap);
	fRot = fRot - m_rot.y;
	RemakeAngle(&fRot);

	m_rot.y += fRot * 0.05f;
	RemakeAngle(&m_rot.y);
	SetStateSpeed(STATE_SPEED_ACCEL);

	if (bGoal == true)
	{
		m_nAnimnow = PLAYERANIM_NEUTRAL;
		m_bGoal = true;
		m_move *= 0.0f;
		SetStateHandle(HANDLE_MAX);
		SetStateSpeed(STATE_SPEED_DOWN);
		return;
	}
}
//=============================================================================
// コントロールキー
//=============================================================================
void CPlayer::ControlKey(void)
{
	// 取得
	bool bOnline = CTitle::GetOnline();

	CSound *pSound = CManager::GetSound();

	CInputKeyBoard * pInputKeyboard = CManager::GetInput();		//キーボードの取得
	CInputJoyPad_0 * pXpad = CManager::GetInputJoyPad0(m_nControllerNum);		//ジョイパットの取得

	bool bGoal = false;
	CRoad_Pointer::NextPoint(m_pos, m_pPoint, m_nPlayerNum, m_fRoad, m_bDivided, bGoal, m_nMap);

	if (bGoal == true)
	{
		m_bGoal = true;
		m_move *= 0.0f;
		SetStateHandle(HANDLE_MAX);
		SetStateSpeed(STATE_SPEED_DOWN);
		m_nAnimnow = PLAYERANIM_NEUTRAL;
		return;
	}

	CDebugProc::Print("位置 : X %.2f, Y %.2f, Z %.2f\n", m_pos.x, m_pos.y, m_pos.z);

	//向きの設定
	if (m_StateSpeed != STATE_SPEED_STOP)
	{
		if ((bOnline == false &&
			(pInputKeyboard->GetKeyboardPress(DIK_A) == true ||
				pInputKeyboard->GetKeyboardPress(DIK_LEFT) == true)) ||
			pXpad->GetPress(INPUT_LS_L) == true ||
			pXpad->GetPress(INPUT_LEFT) == true)
		{ //左ハンドル状態
			SetStateHandle(HANDLE_LEFT);
		}
		else if ((bOnline == false &&
			(pInputKeyboard->GetKeyboardPress(DIK_D) == true ||
				pInputKeyboard->GetKeyboardPress(DIK_RIGHT) == true)) ||
			pXpad->GetPress(INPUT_LS_R) == true ||
			pXpad->GetPress(INPUT_RIGHT) == true)
		{//右ハンドル状態
			SetStateHandle(HANDLE_RIGHT);
		}
		else
		{//ハンドルを触っていない状態
			SetStateHandle(HANDLE_MAX);
		}
	}
	else
	{
		SetStateHandle(HANDLE_MAX);
	}

	// 操作
	if (
		((bOnline == false && pInputKeyboard->GetKeyboardPress(DIK_K) == true) ||
		 (pXpad->GetPress(INPUT_L1) == true) ||
		 (pXpad->GetPress(INPUT_L2) == true)) &&
		((bOnline == false && pInputKeyboard->GetKeyboardPress(DIK_L) == true) ||
		 (pXpad->GetPress(INPUT_R1) == true) ||
		 (pXpad->GetPress(INPUT_R2) == true))		
		)
	{// ドリフト
		if (m_PlayerInfo.nCountTime < 90)
			SetStateSpeed(STATE_SPEED_ACCEL);
		else
			SetStateSpeed(STATE_SPEED_DRIFT);
	}
	else
	{
		//アクセル
		if ((bOnline == false && pInputKeyboard->GetKeyboardPress(DIK_K) == true) ||
			(pXpad->GetPress(INPUT_L1) == true) ||
			(pXpad->GetPress(INPUT_L2) == true))
		{//減速状態
			SetStateSpeed(STATE_SPEED_BRAKS);
		}
		else if
			((bOnline == false && pInputKeyboard->GetKeyboardPress(DIK_L) == true) ||
			(pXpad->GetPress(INPUT_R1) == true) ||
				(pXpad->GetPress(INPUT_R2) == true))
		{ //アクセルを状態
			SetStateSpeed(STATE_SPEED_ACCEL);
		}
		else
		{
			//停止判定
			D3DXVECTOR3 fDiffuse = m_pos - m_OldPos;

			if (fDiffuse.x < 0.10f && fDiffuse.x > -0.10f)
			{
				if (fDiffuse.z < 0.10f && fDiffuse.z > -0.10f)
				{
					SetStateSpeed(STATE_SPEED_STOP);

					if (m_State == PLAYERSTATE_SPEEDUP)
					{// スピードアイテムを使ったとき
					 //進行方向の設定
						m_move.x += sinf(m_rot.y) * (m_fSpeed);
						m_move.z += cosf(m_rot.y) * (m_fSpeed);
					}
				}
				else
				{
					SetStateSpeed(STATE_SPEED_DOWN);
				}
			}
			else
			{
				SetStateSpeed(STATE_SPEED_DOWN);
			}
		}
	}

	//if (m_StateSpeed == STATE_SPEED_ACCEL)
	//{
	//	//走る音
	//	pSound->SetVolume(CSound::SOUND_LABEL_SE_RAN, 3.0f);
	//	pSound->PlaySound(CSound::SOUND_LABEL_SE_RAN);
	//}

	// ジャンプ
	if (m_bJump == false)
	{// ジャンプしていない
		m_bJumpSave = false;

		if ((bOnline == false &&
			(pInputKeyboard->GetKeyboardTrigger(DIK_W) == true ||
				pInputKeyboard->GetKeyboardTrigger(DIK_UP) == true)) ||
			pXpad->GetTrigger(INPUT_A) == true)
		{// ジャンプキー
			m_bJumpSave = true;
			m_bJump = true;
			m_move.y += PLAYER_JUMP;
		}
	}
	else
	{
		m_bJumpSave = false;
	}

	if (pInputKeyboard->GetKeyboardTrigger(DIK_SPACE) == true ||
		pXpad->GetTrigger(INPUT_B) == true)
	{// 弾発射
		BulletEgg();
	}

	/*if ((pInputKeyboard->GetKeyboardPress(DIK_N) == true) || (pInputJoypad->GetPress(CXInput::XIJS_BUTTON_0) == true))
	{
	pSound->SetVolume(CSound::SOUND_LABEL_SE_KLAXON, 1.5f);
	pSound->PlaySound(CSound::SOUND_LABEL_SE_KLAXON);
	}*/
}

//=============================================================================
// 移動処理
//=============================================================================
void CPlayer::UpdateMove(void)
{
	CSound *pSound = CManager::GetSound();

	m_OldPos = m_pos;	//前回の位置を保存する

	if (m_bJump == false)
	{//傾斜処理
		D3DXVECTOR3 Vec2;
		Vec2 = D3DXVECTOR3(sinf(m_rot.y), 0.0f, cosf(m_rot.y));
		m_fTilt = (m_FNor.x * Vec2.x) + (m_FNor.y * Vec2.y) + (m_FNor.z * Vec2.z) /
			(sqrtf(powf(m_FNor.x, 2) + powf(m_FNor.y, 2) + powf(m_FNor.z, 2)) * sqrtf(powf(Vec2.x, 2) + powf(Vec2.y, 2) + powf(Vec2.z, 2)));
		acosf(m_fTilt);
		m_fTilt = (m_fTilt * -1.0f) * 1.0f + 0.1f;
	}
	else { m_fTilt = 0.0f; }
	if (m_fTilt > 0.05f) { m_fTilt = 0.05f; }
	m_fCTilt = m_fTilt;

	RemakeAngle(&m_rot.y);

	//bool bGoal = false;
	//float fRot = CRoad_Pointer::NextPoint(m_pos, m_pPoint, m_nPlayerNum, m_bDivided, bGoal, m_nMap);
	//m_rot.y += (fRot - m_rot.y);
	//if (bGoal) { m_move *= 0.0f; }


	if (m_State == PLAYERSTATE_SPEEDUP)
	{// スピードアイテムを使ったとき
		m_nCountSpeed++;

		if (m_nCountSpeed > SPEEDUP_TIME)
		{
			m_State = PLAYERSTATE_NORMAL;
			m_nCountSpeed = 0;

			// ジャンプ
			m_bJumpSave = true;
			m_bJump = true;
			m_move.y += EGG_RAND;
		}
	}

	float fAccel = m_PlayerInfo.fAccel;
	float fBraks = m_PlayerInfo.fBraks;
	float fAddRot = m_PlayerInfo.fAddRot;
	float fDown = m_PlayerInfo.fDown;

	fAddRot *= 0.65f;

	//状態ごとの更新処理
	switch (m_StateSpeed)
	{
	case STATE_SPEED_ACCEL:	//アクセル状態

		//走るモーション
		m_nAnimnow = PLAYERANIM_RUN;

		//ジャンプ状態なら
		if (m_bJump == true) { break; }

		if (m_State == PLAYERSTATE_NORMAL)
		{
			m_fSpeed = fAccel * (m_PlayerInfo.nCountTime < 90 ? (m_PlayerInfo.nCountTime / 90) : 1.0f) * (1.0f - m_fTilt * 1.5f);
		}

		//進行方向の設定
		m_move.x += sinf(m_rot.y) * (m_fSpeed);
		m_move.z += cosf(m_rot.y) * (m_fSpeed);
		break;
	case STATE_SPEED_BRAKS: //ブレーキ状態

		//ジャンプ状態なら
		if (m_bJump == true) { break; }

		m_fSpeed = fBraks * (m_PlayerInfo.nCountTime < 90 ? (m_PlayerInfo.nCountTime / 90) : 1.0f);

		//進行方向の設定
		m_move.x += sinf(m_rot.y) * m_fSpeed;
		m_move.z += cosf(m_rot.y) * m_fSpeed;

		//揺れを無効にする
		m_bShake = false;
		break;
	case STATE_SPEED_DRIFT:	//ドリフト状態

		fAccel *= 0.345f;
		fAddRot = m_PlayerInfo.fAddRot;
		fDown *= 0.35f;

		//走るモーション
		m_nAnimnow = PLAYERANIM_RUN;

		//ジャンプ状態なら
		if (m_bJump == true) { break; }

		if (m_State == PLAYERSTATE_NORMAL)
		{
			m_fSpeed = fAccel * (m_PlayerInfo.nCountTime < 90 ? (m_PlayerInfo.nCountTime / 90) : 1.0f) * (1.0f - m_fTilt * 1.5f);
		}

		//進行方向の設定
		m_move.x += sinf(m_rot.y) * (m_fSpeed);
		m_move.z += cosf(m_rot.y) * (m_fSpeed);

		break;
	case STATE_SPEED_DOWN: //ダウン状態
		m_PlayerInfo.nCountTime = 0;
		//CDebugProc::Print("DWON***\n");

		if (m_State == PLAYERSTATE_SPEEDUP)
		{// スピードアイテムを使ったとき
		 //進行方向の設定
			m_move.x += sinf(m_rot.y) * (m_fSpeed);
			m_move.z += cosf(m_rot.y) * (m_fSpeed);
		}

		break;
	default:
		//走るモーション
		m_nAnimnow = PLAYERANIM_NEUTRAL;
		break;
	}

	switch (m_State)
	{// 弾を食らったとき
	case PLAYERSTATE_NORMAL:
		break;

	case PLAYERSTATE_SPEEDDOWN:
		//進行方向の設定
		m_move.x *= SPEEDDOWN;
		m_move.z *= SPEEDDOWN;

		//スピードダウンの音
		pSound->SetVolume(CSound::SOUND_LABEL_SE_SPEEDDOWN, 2.0f);
		pSound->PlaySound(CSound::SOUND_LABEL_SE_SPEEDDOWN);

		break;

	case PLAYERSTATE_DAMAGE:

		m_fSpeed = 0.0f;

		//進行方向の設定
		m_move.x = sinf(m_rot.y) * (m_fSpeed);
		m_move.z = cosf(m_rot.y) * (m_fSpeed);

		m_PlayerInfo.nCountTime = 0;
		break;
	}

	if (m_bDamage == true)
	{
		m_nCntDamage++;

		int nDamageTime = 0;	// 状態が変わる時間の長さ

		if (m_State == PLAYERSTATE_DAMAGE)
		{// 攻撃を食らったとき
			nDamageTime = DAMAGE_TIME;
		}
		else if (m_State == PLAYERSTATE_SPEEDDOWN)
		{// スピードダウンを食らったとき
			nDamageTime = SPEEDDOWN_TIME;
		}

		if (m_nCntDamage > nDamageTime)
		{
			m_State = PLAYERSTATE_NORMAL;
			m_nCntDamage = 0;

			m_bDamage = false;
		}
	}

	m_PlayerInfo.nCountTime++;

	//CDebugProc::Print("アクセル : %1f\n", m_PlayerInfo.fAccel);
	//CDebugProc::Print("スピード : %1f  %1f  %1f\n", m_move.x, m_move.y, m_move.z);

	//ハンドルの状態更新
	if (m_StateHandle == HANDLE_LEFT)
	{
		if (m_StateSpeed != STATE_SPEED_STOP)
		{
			m_fAddRot -= fAddRot * (m_PlayerInfo.nCountTime < 45 ? (m_PlayerInfo.nCountTime / 45) : 1.0f);
		}
	}
	else if (m_StateHandle == HANDLE_RIGHT)
	{
		if (m_StateSpeed != STATE_SPEED_STOP)
		{
			m_fAddRot += fAddRot * (m_PlayerInfo.nCountTime < 45 ? (m_PlayerInfo.nCountTime / 45) : 1.0f);
		}
	}

	// 重力
	//if (m_bShake == true)
	{
		m_move.y -= cosf(0) * PLAYER_GRAVITY;
	}

	//中心座標更新
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;
	m_pos.z += m_move.z;

	m_rot.y += m_fAddRot;

	//減速
	if (!m_bJump)
	{
		m_move.x += (0.0f - m_move.x) * fDown;
		m_move.z += (0.0f - m_move.z) * fDown;
	}

	m_fAddRot += (0.0f - m_fAddRot) * PLAYER_DOWNROT;

	CDebugProc::Print("addrot : %f\n", m_fAddRot);
	CDebugProc::Print("fSpeed : %f\n", m_fSpeed);
}

//=============================================================================
// プレイヤーの高さ処理
//=============================================================================
void CPlayer::UpdateField(void)
{
	CScene *pScene = CScene::GetTop(MESH_PRIOTITY);

	//NULLチェック
	while (pScene != NULL)
	{
		//UpdateでUninitされてしまう場合　Nextが消える可能性があるからNextにデータを残しておく
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetObjType() == CScene::OBJTYPE_GROUND)
		{//タイプが地面だったら
			CMeshField *pField = (CMeshField*)pScene;

			if (pField->OnField(m_pos, m_fvtxMaxY))
			{// 傾斜の計算
				float fHight = pField->GetHeightMesh(m_pos);

				//if (m_pos.y <= fHight)
				if (m_bJump == false || (m_bJump == true && m_pos.y < fHight))
				{
					float fEggHeight = 0.0f;

					if (m_State == PLAYERSTATE_SPEEDUP)
					{// 卵に乗ったように見える高さ
						fEggHeight = EGG_HEIGHT;
					}

					m_pos.y = fHight + fEggHeight;		//地面の高さを取得
					m_move.y = 0.0f;					//移動量を初期化する

														//ジャンプ中かどうか
														//bool bJumpFlag = pPlayer->GetJump();
														//if (bJumpFlag)
														//{
														//	pPlayer->GetPlayerInfoPoint()->nCountTime -= (int)std::round(pPlayer->GetPlayerInfoPoint()->nCountTime * 0.05f);
														//}

														//ジャンプの状態設定
					m_bJump = false;
					//m_pIncline->SetJumpFlag(false);

					break;
				}
			}
		}
		//Nextに次のSceneを入れる
		pScene = pSceneNext;
	}
}

//=============================================================================
// スピードの状態設定
//=============================================================================
void CPlayer::SetStateSpeed(CPlayer::STATE_SPEED state)
{
	//数値の設定
	if (m_StateSpeed != state)
	{
		if ((m_StateSpeed == STATE_SPEED_ACCEL && state == STATE_SPEED_DRIFT) ||
			(state == STATE_SPEED_ACCEL && m_StateSpeed == STATE_SPEED_DRIFT))
		{
			m_nDriftCounter = 0;
		}
		else
		{
			m_PlayerInfo.nCountTime = 0;
		}

		//CDebugProc::Print("CHANGE***\n");

		CSound *pSound = CManager::GetSound();

		/*if (STATE_SPEED_ACCEL == state && STATE_DRIVE == m_MoveState)
		{// アクセル音
		pSound->SetVolume(CSound::SOUND_LABEL_SE_ACCEL, 0.7f);
		pSound->PlaySound(CSound::SOUND_LABEL_SE_ACCEL);
		}
		else if (STATE_SPEED_BRAKS == state)
		{// ブレーキ音
		if (m_move.x >= 0.1f || m_move.x <= -0.1f || m_move.z >= 0.1f || m_move.z <= -0.1f)
		{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_BRAKING);
		pSound->StopSound(CSound::SOUND_LABEL_SE_ACCEL);
		}
		}*/
	}

	//状態の設定
	m_StateSpeed = state;
}

//=============================================================================
// 設定処理
//=============================================================================
void CPlayer::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	m_pos = pos;
}

//=============================================================================
// オブジェクトとの当たり判定
//=============================================================================
void CPlayer::CollisionObject(void)
{
	CSound *pSound = CManager::GetSound();
	CScene *pScene;

	//吹っ飛ば差ないオブジェクト番号の登録
	/*int anUpdateType[UPDATE_TYPE_NUM + HIGHT_OBJ_NUM + 2] = { TYPE_TREE00, TYPE_TREE01, TYPE_BILL00, TYPE_BILL01,
	TYPE_BILL02, TYPE_TVBILL, TYPE_FLOWER, TYPE_TANUKI,
	TYPE_OCLOCK, TYPE_REDBILL, TYPE_TREE02,  TYPE_CORN2,
	TYPE_STATION, TYPE_ESTA, TYPE_DAIMAL, TYPE_APIA,
	TYPE_TOWER, TYPE_FOUNTAIN, TYPE_FERRISWGEEL,
	TYPE_STREETLIGHT, TYPE_TRAFFICLIGHT00, TYPE_TRAFFICLIGHT01, TYPE_ROAD , TYPE_TAPIOCA, TYPE_HOSPITAL };*/

	int anUpdateType[UPDATE_TYPE_NUM + HIGHT_OBJ_NUM + 2] = { 0 };	// 仮
	bool bType = false;	//タイプのフラグ

	for (int nCntPriority = 2; nCntPriority <= OBJECT_PRIOTITY; nCntPriority++)
	{
		// プライオリティーチェック
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// プライオリティー内のリスト構造を最後まで見る
			CScene *pSceneNext = pScene->GetNext();		// 次のオブジェクトを保存

			if (pScene->GetObjType() == OBJTYPE_OBJECT)
			{// タイプが障害物だったら
				CObject *pObject = (CObject*)pScene;	// オブジェクトクラスのポインタ変数にする
				int nType = pObject->GetType();			// 障害物の種類を取得
														//当たり判定用のオブジェクトにのみ当たる
				if (nType == 2) { pObject->CollisionObject(&m_pos, &m_OldPos, &m_move); }
			}

			// Nextに次のSceneを入れる
			pScene = pSceneNext;
		}
	}

	//デバック表示
	/*if (m_bCrcleCarIn == true)
	{
	CDebugProc::Print("入っている\n");
	}
	else if (m_bCrcleCarIn == false)
	{
	CDebugProc::Print("入っていない\n");
	}*/
}

//=============================================================================
// 壁との当たり判定
//=============================================================================
void CPlayer::CollisitionWall(void)
{
	CScene *pScene;
	//プライオリティーチェック
	pScene = CScene::GetTop(WALL_PRIOTITY);

	//NULLチェック
	while (pScene != NULL)
	{
		//UpdateでUninitされてしまう場合　Nextが消える可能性があるからNextにデータを残しておく
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetDeath() == false)
		{
			//タイプが壁だったら
			if (pScene->GetObjType() == OBJTYPE_WALL)
			{
				bool bTouch = ((CWall*)pScene)->CollisionWall(&m_pos, &m_OldPos, &m_move, 20.0f);

				if (bTouch)
				{ //当たっているなら減速する
					m_PlayerInfo.fAccel = m_PlayerInfo.fAccel * DECELERATION;
					m_PlayerInfo.nCountTime = 0;
					break;
				}
			}
		}

		//Nextに次のSceneを入れる
		pScene = pSceneNext;
	}
}

//=============================================================================
// ジャンプ状態の更新処理
//=============================================================================
void CPlayer::UpdateStateJump(void)
{
	if (m_bJump)
	{//ジャンプ状態の時間を加算
		m_nCountJumpTime++;

		/*//X軸の角度制限
		if (m_rot.x > 0.5f)
		{
		m_rot.x = 0.5f;
		}
		else if (m_rot.x < -0.5f)
		{
		m_rot.x = -0.5f;
		}

		//Z軸の角度制限
		if (m_rot.z > 0.5f)
		{
		m_rot.z = 0.5f;
		}
		else if (m_rot.z < -0.5f)
		{
		m_rot.z = -0.5f;
		}*/
		return;
	}
	m_nCountJumpTime = 0;
}

//=============================================================================
// 辺の長さを求める
//=============================================================================
float CPlayer::GetLength(D3DXVECTOR3 StartPos, D3DXVECTOR3 EndPos)
{
	D3DXVECTOR3 Distance = StartPos - EndPos;
	float fLength = sqrtf((Distance.x * Distance.x) + (Distance.z * Distance.z));

	return fLength;
}

//=============================================================================
// 車の角度修正
//=============================================================================
void CPlayer::RemakeCarRot(float * pAngle)
{
	if (*pAngle > ROLLOVER_STOP)
	{
		*pAngle = ROLLOVER_STOP;
	}
	else if (*pAngle < -ROLLOVER_STOP)
	{
		*pAngle = -ROLLOVER_STOP;
	}
}

//=============================================================================
// 角度修正
//=============================================================================
void CPlayer::RemakeAngle(float * pAngle)
{
	//目的の角度修正
	if (*pAngle < -D3DX_PI)
	{
		*pAngle += D3DX_PI * 2.0f;
	}
	if (*pAngle > D3DX_PI)
	{
		*pAngle -= D3DX_PI * 2.0f;
	}
}

//=============================================================================
// デバック表示
//=============================================================================
void CPlayer::DebugProc(void)
{
	//状態の表示
	/*if (m_MoveState == STATE_DRIVE)
	{
	CDebugProc::Print("状態 : STATE_DRIVE\n");
	}
	else
	{
	CDebugProc::Print("状態 : STATE_REVERSE\n");
	}

	//走行状態の標示
	if (m_StateSpeed == STATE_SPEED_STOP)
	{
	CDebugProc::Print("停止状態\n");
	}
	*/
	//位置表示
	//CDebugProc::Print("位置 : X %.2f, Y %.2f, Z %.2f\n", m_pos.x, m_pos.y, m_pos.z);
	//CDebugProc::Print("移動 : X %.2f, Y %.2f, Z %.2f\n", m_move.x, m_move.y, m_move.z);
	//
	//CDebugProc::Print("ジャンプ：%s\n", m_bJump ? "〇" : "×");

	//CDebugProc::Print("カウント : %f\n", m_PlayerInfo.nCountTime);

	//	CDebugProc::Print("アイテム：%d\n", m_nNumItem);
	//	for (int nCount = 0; nCount < MAX_EGG; nCount++)
	//		CDebugProc::Print("アイテム種類：%d\n", m_bulletType[nCount]);

	//CDebugProc::Print("カウント：%f\n", m_PlayerInfo.nCountTime);
	//
	//switch (m_StateSpeed)
	//{
	//case STATE_SPEED_ACCEL:
	//	CDebugProc::Print("STATE_SPEED_ACCEL\n");
	//	break;
	//case STATE_SPEED_BRAKS:
	//	CDebugProc::Print("STATE_SPEED_BRAKS\n");
	//	break;
	//case STATE_SPEED_DOWN:
	//	CDebugProc::Print("STATE_SPEED_DOWN\n");
	//	break;
	//case STATE_SPEED_STOP:
	//	CDebugProc::Print("STATE_SPEED_STOP\n");
	//	break;
	//}
}

//=============================================================================
// 音の再生
//=============================================================================
void CPlayer::PlaySoundObj(int nType, CSound * pSound)
{
	/*switch (nType)
	{*/
	/*case TYPE_PHONEBOX: pSound->PlaySoundA(CSound::SOUND_LABEL_SE_STEAL); break;
	case TYPE_CARDBORD: pSound->PlaySoundA(CSound::SOUND_LABEL_SE_SMALLBOX); break;
	case TYPE_CORN:
	pSound->SetVolume(CSound::SOUND_LABEL_SE_SMALLBOX, 0.5f);
	pSound->PlaySoundA(CSound::SOUND_LABEL_SE_SMALLBOX);
	break;
	case TYPE_LEAF:

	break;
	case TYPE_FENCE:*/
	//if ((m_nCountSound % 5) == 0) { m_nCountSound = 0; }

	//if (m_nCountSound == 0)
	//{//フェンス00
	//	pSound->PlaySoundA(CSound::SOUND_LABEL_SE_FANCE00);
	//}
	//else if (m_nCountSound == 1)
	//{//フェンス01
	//	pSound->PlaySoundA(CSound::SOUND_LABEL_SE_FANCE00);
	//}
	//else if (m_nCountSound == 2)
	//{//フェンス02
	//	pSound->PlaySoundA(CSound::SOUND_LABEL_SE_FANCE00);
	//}
	//else if (m_nCountSound == 3)
	//{//フェンス02
	//	pSound->PlaySoundA(CSound::SOUND_LABEL_SE_FANCE00);
	//}
	//else if (m_nCountSound == 4)
	//{//フェンス02
	//	pSound->PlaySoundA(CSound::SOUND_LABEL_SE_FANCE00);
	//}

	//m_nCountSound++;	//カウンターの加算
	//break;

	//case TYPE_BENCH:
	//if ((m_nCountWood % 2) == 0) { m_nCountWood = 0; }	//カウンターをリセットする

	////音の再生
	//if (m_nCountWood == 0)
	//{
	//	pSound->SetVolume(CSound::SOUND_LABEL_SE_WOOD00, 2.0f);
	//	pSound->PlaySoundA(CSound::SOUND_LABEL_SE_WOOD00);
	//}
	//else if (m_nCountWood)
	//{
	//	pSound->PlaySoundA(CSound::SOUND_LABEL_SE_WOOD01);
	//}

	////カウンターの加算
	//m_nCountWood++;
	//break;

	//case TYPE_SIGNBOARD: pSound->PlaySoundA(CSound::SOUND_LABEL_SE_WOOD01); break;
	//}
}

//=============================================================================
// 餌との当たり判定
//=============================================================================
void CPlayer::CollisionFeed(void)
{
	if (m_nNumItem >= MAX_EGG) { return; }

	CSound *pSound = CManager::GetSound();
	CScene *pScene;

	for (int nCntPriority = 2; nCntPriority <= FEED_PRIOTITY; nCntPriority++)
	{
		// プライオリティーチェック
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// プライオリティー内のリスト構造を最後まで見る
			CScene *pSceneNext = pScene->GetNext();		// 次のオブジェクトを保存

			if (pScene->GetObjType() == OBJTYPE_FEED)
			{// タイプが障害物だったら
				CFeed *pFeed = (CFeed*)pScene;	// オブジェクトクラスのポインタ変数にする

				if (pFeed->GetDeath() != true)
				{
					if (pFeed->CollisionFeed(&m_pos, &m_OldPos) == true)
					{// 衝突した
						EggAppear(pFeed);	// 卵出現
						pFeed->Uninit();	// 餌削除
						m_nNumEgg++;
						m_nNumItem++;

						break;
					}
				}
			}

			// Nextに次のSceneを入れる
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// 卵の出現処理
//=============================================================================
void CPlayer::EggAppear(CFeed *pFeed)
{
	if (m_pEgg[m_nNumEgg] == NULL)
	{
		if (pFeed->GetFeedType() == CFeed::FEEDTYPE_ATTACK)
		{// 攻撃の卵生成
			m_pEgg[m_nNumEgg] = CEgg::Create(m_pos,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(EGG_SCALE, EGG_SCALE, EGG_SCALE),
				CEgg::EGGTYPE_ATTACK,
				CEgg::BULLETTYPE_PLAYER,
				m_nPlayerNum);

			m_bulletType[m_nNumChick + m_nNumEgg] = BULLET_EGG_ATTACK;
		}
		else if (pFeed->GetFeedType() == CFeed::FEEDTYPE_ANNOY)
		{// 妨害の卵生成
			m_pEgg[m_nNumEgg] = CEgg::Create(m_pos,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(EGG_SCALE, EGG_SCALE, EGG_SCALE),
				CEgg::EGGTYPE_ANNOY,
				CEgg::BULLETTYPE_PLAYER,
				m_nPlayerNum);

			m_bulletType[m_nNumChick + m_nNumEgg] = BULLET_EGG_ANNOY;
		}
		else if (pFeed->GetFeedType() == CFeed::FEEDTYPE_SPEED)
		{// 加速の卵生成
			m_pEgg[m_nNumEgg] = CEgg::Create(m_pos,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(EGG_SCALE, EGG_SCALE, EGG_SCALE),
				CEgg::EGGTYPE_SPEED,
				CEgg::BULLETTYPE_PLAYER,
				m_nPlayerNum);

			m_bulletType[m_nNumChick + m_nNumEgg] = BULLET_EGG_SPEED;
		}
	}

	m_pEgg[m_nNumEgg]->SetState(CEgg::EGGSTATE_CHASE);
}


//=============================================================================
// 卵がついてくる処理
//=============================================================================
void CPlayer::ChaseEgg(void)
{
	m_nCntFrame++;

	if (m_nCntFrame >= MAX_FRAME)
	{
		m_nCntFrame = 0;
	}

	// 前のプレイヤーの角度を保存
	m_OldEggRot[m_nCntFrame] = m_rot;
	m_abJump[m_nCntFrame] = m_bJumpSave;

	if (m_nNumEgg + m_nNumChick <= 0) { return; }

	if (m_nNumEgg >= 1)
	{// 卵が一個の時
		if (m_pEgg[0]->GetState() == CEgg::EGGSTATE_CHASE)
		{
			// 前の向きを代入
			int nData = m_nCntFrame - EGG_POS * (1 + m_nNumChick);

			if (nData < 0)
			{
				nData += MAX_FRAME;
			}

			// 卵の位置設定
			m_pEgg[0]->SetPos(D3DXVECTOR3((sinf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * (1 + m_nNumChick)) + m_pos.x,
				m_pEgg[0]->GetPos().y,
				(cosf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * (1 + m_nNumChick)) + m_pos.z));

			// 卵の角度設定
			m_pEgg[0]->SetRot(m_OldEggRot[nData]);

			if (m_abJump[nData] == true)
			{
				m_pEgg[0]->Jump(EGGJUMP);
			}
		}
	}
	if (m_nNumEgg >= 2)
	{// 卵が二個の時
	 // 二個目
		int nData = m_nCntFrame - EGG_POS * 2;

		if (nData < 0)
		{
			nData += MAX_FRAME;
		}

		m_pEgg[1]->SetPos(D3DXVECTOR3((sinf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * (2 + m_nNumChick)) + m_pos.x,
			m_pEgg[1]->GetPos().y,
			(cosf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * (2 + m_nNumChick)) + m_pos.z));

		m_pEgg[1]->SetRot(m_OldEggRot[nData]);

		if (m_abJump[nData] == true)
		{
			m_pEgg[1]->Jump(EGGJUMP);
		}
	}
	if (m_nNumEgg >= 3)
	{
		// 三個目
		int nData = m_nCntFrame - EGG_POS * 3;

		if (nData < 0)
		{
			nData += MAX_FRAME;
		}

		m_pEgg[2]->SetPos(D3DXVECTOR3((sinf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * (3 + m_nNumChick)) + m_pos.x,
			m_pEgg[2]->GetPos().y,
			(cosf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * (3 + m_nNumChick)) + m_pos.z));

		m_pEgg[2]->SetRot(m_OldEggRot[nData]);

		if (m_abJump[nData] == true)
		{
			m_pEgg[2]->Jump(EGGJUMP);
		}
	}

	if (m_nNumChick >= 1)
	{// 卵が一個の時
		if (m_pChick[0]->GetState() == CChick::STATE_CHASE)
		{
			// 前の向きを代入
			int nData = m_nCntFrame - EGG_POS;

			if (nData < 0)
			{
				nData += MAX_FRAME;
			}

			// 卵の位置設定
			m_pChick[0]->SetPos(D3DXVECTOR3((sinf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE) + m_pos.x,
				m_pChick[0]->SetHeight(),
				(cosf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE) + m_pos.z));

			// 卵の角度設定
			m_pChick[0]->SetRot(m_OldEggRot[nData]);

			if (m_abJump[nData] == true)
			{
				m_pChick[0]->Jump(EGGJUMP);
			}
		}
	}
	if (m_nNumChick >= 2)
	{// ひよこが二匹の時
	 // 二匹目
		int nData = m_nCntFrame - EGG_POS * 2;

		if (nData < 0)
		{
			nData += MAX_FRAME;
		}

		m_pChick[1]->SetPos(D3DXVECTOR3((sinf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * 2) + m_pos.x,
			m_pChick[1]->SetHeight(),
			(cosf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * 2) + m_pos.z));

		m_pChick[1]->SetRot(m_OldEggRot[nData]);

		if (m_abJump[nData] == true)
		{
			m_pChick[1]->Jump(EGGJUMP);
		}
	}
	if (m_nNumChick >= 3)
	{
		// 二匹目
		int nData = m_nCntFrame - EGG_POS * 3;

		if (nData < 0)
		{
			nData += MAX_FRAME;
		}

		m_pChick[2]->SetPos(D3DXVECTOR3((sinf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * 3) + m_pos.x,
			m_pChick[2]->SetHeight(),
			(cosf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * 3) + m_pos.z));

		m_pChick[2]->SetRot(m_OldEggRot[nData]);

		if (m_abJump[nData] == true)
		{
			m_pChick[2]->Jump(EGGJUMP);
		}
	}
}

//=============================================================================
// 卵発射処理
//=============================================================================
void CPlayer::BulletEgg(void)
{
	if (m_nNumEgg + m_nNumChick > 0)
	{// 卵かひよこを持っているとき
		if (m_pChick[0] != NULL && m_pChick[0]->GetState() == CChick::STATE_CHASE)
		{
			m_pChick[0]->SetState(CChick::STATE_BULLET);	// 状態を弾にする
			m_pChick[0]->SetRank(CGame::GetRanking(m_nPlayerNum));
			m_nPlayerRank = CGame::GetRanking(m_nPlayerNum);
			int nRank = CGame::GetRanking(m_nPlayerNum) - 1;

			m_nNumChick--;	// 所持数を減らす

			m_nNumItem--;

			switch (m_pChick[0]->GetType())
			{
				// 攻撃
			case CChick::TYPE_ATTACK:
				if (nRank >= 0)
				{
					for (int nCntChar = 0; nCntChar < MAX_MEMBER; nCntChar++)
					{// ひとつ前の順位のやつを見つける
						int nData = CGame::GetRanking(nCntChar);

						if (nRank == nData)
						{
							m_nDestRank = nCntChar;
							m_pChick[0]->SetDestRank(nCntChar);
							break;
						}
					}
				}
				else
				{
					m_pChick[0]->SetDis(false);
				}

				m_pChick[0]->SetRot(m_rot);
				break;

				// 加速
			case CChick::TYPE_SPEED:
				m_State = PLAYERSTATE_SPEEDUP;
				m_fSpeed += SPEED;

				m_pChick[0]->Uninit();
				m_pChick[0] = NULL;
				break;

				// 減速
			case CChick::TYPE_ANNOY:
				m_pChick[0]->SetDis(false);
				break;

				// 強い攻撃
			case CChick::TYPE_ATTACK_S:
				for (int nCntChar = 0; nCntChar < MAX_MEMBER; nCntChar++)
				{// 1位のやつを見つける
					int nDestRank = CGame::GetRanking(nCntChar);

					if (nDestRank == 0)
					{
						m_nDestRank = nCntChar;
						m_pChick[0]->SetDestRank(nCntChar);
						break;
					}
				}
				m_pChick[0]->SetRot(m_rot);
				break;

				// 減速
			case CChick::TYPE_ANNOY_S:
				m_pChick[0]->SetDis(false);
				break;
			}

			// 情報入れ替え
			m_pChick[0] = m_pChick[1];
			m_pChick[1] = m_pChick[2];
			m_pChick[2] = NULL;

			m_bulletType[0] = m_bulletType[1];
			m_bulletType[1] = m_bulletType[2];
			m_bulletType[2] = BULLET_EGG_ATTACK;
		}
		else if (m_pEgg[0] != NULL && m_pEgg[0]->GetState() == CEgg::EGGSTATE_CHASE)
		{// 一個目の卵に情報が入っていて、プレイヤーについてくる時
			m_pEgg[0]->SetState(CEgg::EGGSTATE_BULLET);	// 状態を弾にする
			m_pEgg[0]->SetRank(CGame::GetRanking(m_nPlayerNum));
			m_nPlayerRank = CGame::GetRanking(m_nPlayerNum);

			m_nNumEgg--;	// 所持数を減らす

			m_nNumItem--;

			switch (m_pEgg[0]->GetType())
			{
				// 攻撃
			case CEgg::EGGTYPE_ATTACK:
				m_pEgg[0]->Jump(THROW);
				m_pEgg[0]->SetRot(m_rot);
				m_pEgg[0]->SetThrow(TRUE);
				break;

				// 加速
			case CEgg::EGGTYPE_SPEED:
				m_State = PLAYERSTATE_SPEEDUP;
				m_fSpeed += SPEED;

				// ジャンプ
				m_bJumpSave = true;
				m_bJump = true;
				m_move.y += EGG_RAND;
				break;
			}

			// 情報入れ替え
			m_pEgg[0] = m_pEgg[1];
			m_pEgg[1] = m_pEgg[2];
			m_pEgg[2] = NULL;

			m_bulletType[0] = m_bulletType[1];
			m_bulletType[1] = m_bulletType[2];
			m_bulletType[2] = BULLET_EGG_ATTACK;
		}
	}
}

//=============================================================================
// 卵との当たり判定
//=============================================================================
void CPlayer::CollisionEgg(void)
{
	CSound *pSound = CManager::GetSound();
	CScene *pScene;

	for (int nCntPriority = 2; nCntPriority <= EGG_PRIOTITY; nCntPriority++)
	{
		// プライオリティーチェック
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// プライオリティー内のリスト構造を最後まで見る
			CScene *pSceneNext = pScene->GetNext();		// 次のオブジェクトを保存

			if (pScene->GetObjType() == OBJTYPE_EGG)
			{// タイプが障害物だったら
				CEgg *pEgg = (CEgg*)pScene;	// オブジェクトクラスのポインタ変数にする

				if (pEgg->GetNumPlayer() != m_nPlayerNum)
				{
					if (pEgg->CollisionEgg(&m_pos, &m_OldPos) == true)
					{// 衝突した
						switch (pEgg->GetType())
						{
							// 攻撃
						case CEgg::EGGTYPE_ATTACK:
							// ダメージ状態にする
							if (m_bDamage == false)
							{
								m_bDamage = true;
								m_State = PLAYERSTATE_DAMAGE;
							}
							pEgg->Uninit();	// 卵削除
							break;

							// 減速
						case CEgg::EGGTYPE_ANNOY:
							if (m_bDamage == false)
							{
								m_bDamage = true;
								m_State = PLAYERSTATE_SPEEDDOWN;
							}
							pEgg->Uninit();	// 卵削除
							break;
						}
					}
				}
			}
			// Nextに次のSceneを入れる
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// ひよことの当たり判定
//=============================================================================
void CPlayer::CollisionChick(void)
{
	CSound *pSound = CManager::GetSound();
	CScene *pScene;

	for (int nCntPriority = 2; nCntPriority <= EGG_PRIOTITY; nCntPriority++)
	{
		// プライオリティーチェック
		pScene = CScene::GetTop(nCntPriority);

		while (pScene != NULL)
		{// プライオリティー内のリスト構造を最後まで見る
			CScene *pSceneNext = pScene->GetNext();		// 次のオブジェクトを保存

			if (pScene->GetObjType() == OBJTYPE_CHICK)
			{
				CChick *pChick = (CChick*)pScene;	// オブジェクトクラスのポインタ変数にする

				if (pChick->GetNumPlayer() != m_nPlayerNum && pChick->GetType() != CChick::TYPE_ANNOY_S)
				{
					if (pChick->CollisionChick(&m_pos, &m_OldPos) == true)
					{// 衝突した
						switch (pChick->GetType())
						{
							// 攻撃
						case CChick::TYPE_ATTACK:
							// ダメージ状態にする
							if (m_bDamage == false)
							{
								m_bDamage = true;
								m_State = PLAYERSTATE_DAMAGE;
							}
							pChick->Uninit();	// ひよこ削除
							break;

							// 強い攻撃
						case CChick::TYPE_ATTACK_S:
							// ダメージ状態にする
							if (m_bDamage == false && pChick->GetAttackS() == true)
							{
								m_bDamage = true;
								m_State = PLAYERSTATE_DAMAGE;
							}
							break;

							// 減速
						case CChick::TYPE_ANNOY:
							if (m_bDamage == false)
							{
								m_bDamage = true;
								m_State = PLAYERSTATE_SPEEDDOWN;
							}
							break;
						}
					}
				}
			}
			// Nextに次のSceneを入れる
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// ひよこの出現処理
//=============================================================================
void CPlayer::ChickAppear(void)
{
	if (m_pEgg[0] != NULL)
	{
		if (m_pEgg[0]->GetHatchingTimer() > HATCHING_TIME)
		{// 孵化する時間が経過
		 // タイマーを0にもどす
			m_pEgg[0]->SetHatchingTimer(0);

			if (m_pEgg[0] != NULL)
			{
				switch (m_pEgg[0]->GetType())
				{
					// 攻撃
				case CEgg::EGGTYPE_ATTACK:
					if (CGame::GetRanking(m_nPlayerNum) < 3)
					{
						m_pChick[m_nNumChick] = CChick::Create(m_pos,
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							CHICK_SCALE,
							CChick::TYPE_ATTACK,
							CChick::BULLETTYPE_PLAYER,
							CChick::STATE_CHASE,
							m_nPlayerNum);
					}
					else if (CGame::GetRanking(m_nPlayerNum) >= 3)
					{
						m_pChick[m_nNumChick] = CChick::Create(m_pos,
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							CHICK_SCALE,
							CChick::TYPE_ATTACK_S,
							CChick::BULLETTYPE_PLAYER,
							CChick::STATE_CHASE,
							m_nPlayerNum);
					}

					m_bulletType[m_nNumChick] = BULLET_CHICK_ATTACK;

					break;

					// 減速
				case CEgg::EGGTYPE_ANNOY:
					if (CGame::GetRanking(m_nPlayerNum) < 3)
					{
						m_pChick[m_nNumChick] = CChick::Create(m_pos,
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							CHICK_SCALE,
							CChick::TYPE_ANNOY,
							CChick::BULLETTYPE_PLAYER,
							CChick::STATE_CHASE,
							m_nPlayerNum);
					}
					else if (CGame::GetRanking(m_nPlayerNum) >= 3)
					{
						m_pChick[m_nNumChick] = CChick::Create(m_pos,
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							CHICK_SCALE,
							CChick::TYPE_ANNOY_S,
							CChick::BULLETTYPE_PLAYER,
							CChick::STATE_CHASE,
							m_nPlayerNum);
					}

					m_bulletType[m_nNumChick] = BULLET_CHICK_ANNOY;

					break;

					// 加速
				case CEgg::EGGTYPE_SPEED:
					/*if (CGame::GetRanking(m_nPlayerNum) < 3)
					{*/
					m_pChick[m_nNumChick] = CChick::Create(m_pos,
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						CHICK_SCALE,
						CChick::TYPE_SPEED,
						CChick::BULLETTYPE_PLAYER,
						CChick::STATE_CHASE,
						m_nPlayerNum);
					/*}*/
					/*else if (CGame::GetRanking(m_nPlayerNum) >= 3)
					{
					m_pChick[m_nNumChick] = CChick::Create(m_pos,
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					CHICK_SCALE,
					CChick::TYPE_SPEED_S,
					CChick::BULLETTYPE_PLAYER,
					CChick::STATE_CHASE,
					m_nPlayerNum);
					}*/

					m_bulletType[m_nNumChick] = BULLET_CHICK_SPEED;

					break;
				}

				m_pEgg[0]->Uninit();
				m_pEgg[0] = m_pEgg[1];
				m_pEgg[1] = m_pEgg[2];
				m_pEgg[2] = NULL;
				m_nNumEgg--;

				m_nNumChick++;
			}
		}
	}
}

//=============================================================================
// 降らすひよこの出現処理
//=============================================================================
void CPlayer::FallChicks(D3DXVECTOR3 pos)
{
	CScene *pScene;

	for (int nCntChick = 0; nCntChick < CHICK_FALL_NUM; nCntChick++)
	{
		int fx = rand() % FALL_CHICK_RANGE;
		int fz = rand() % FALL_CHICK_RANGE;

		// ひよこ出現
		CChick::Create(D3DXVECTOR3(pos.x + ((FALL_CHICK_RANGE / 2) - fx), pos.y + 50.0f + (nCntChick * 100.0f), pos.z + ((FALL_CHICK_RANGE / 2) - fz)),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			CHICK_SCALE,
			CChick::TYPE_ATTACK_S,
			CChick::BULLETTYPE_PLAYER,
			CChick::STATE_BULLET,
			m_nPlayerNum);

		m_nCntChick++;
	}

	for (int nCntPriority = 2; nCntPriority <= EGG_PRIOTITY; nCntPriority++)
	{
		// プライオリティーチェック
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// プライオリティー内のリスト構造を最後まで見る
			CScene *pSceneNext = pScene->GetNext();		// 次のオブジェクトを保存

			if (pScene->GetObjType() == OBJTYPE_CHICK)
			{// タイプが障害物だったら
				CChick *pChick = (CChick*)pScene;	// オブジェクトクラスのポインタ変数にする

				if (pChick->GetState() == CChick::STATE_BULLET && pChick->GetType() == CChick::TYPE_ATTACK_S && pChick->GetAttackS() == false)
				{
					pChick->SetAttackS(true);
					pChick->SetRank(CGame::GetRanking(m_nPlayerNum));
					pChick->SetDestRank(m_nDestRank);
				}
			}
			// Nextに次のSceneを入れる
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// 強い減速ひよこの出現処理
//=============================================================================
void CPlayer::AnnoyChicks(void)
{
	CPlayer **pPlayer = CGame::GetPlayer();

	for (int nCntPlayer = 0; nCntPlayer < MAX_MEMBER; nCntPlayer++)
	{
		if (pPlayer[nCntPlayer] != NULL)
		{
			if (nCntPlayer != m_nPlayerNum)
			{
				if (m_pAnnoyChick[nCntPlayer] == NULL)
				{
					m_pAnnoyChick[nCntPlayer] = CChick::Create(
						D3DXVECTOR3(pPlayer[nCntPlayer]->GetPos().x, pPlayer[nCntPlayer]->GetPos().y + 100.0f, pPlayer[nCntPlayer]->GetPos().z),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						CHICK_SCALE,
						CChick::TYPE_ANNOY_S,
						CChick::BULLETTYPE_PLAYER,
						CChick::STATE_BULLET,
						m_nPlayerNum);

					if (m_pAnnoyChick[nCntPlayer]->GetState() == CChick::STATE_BULLET
						&& m_pAnnoyChick[nCntPlayer]->GetType() == CChick::TYPE_ANNOY_S
						&& m_pAnnoyChick[nCntPlayer]->GetAttackS() == false
						&& m_pAnnoyChick[nCntPlayer]->GetDis() == true)
					{
						m_pAnnoyChick[nCntPlayer]->SetAttackS(true);
						m_pAnnoyChick[nCntPlayer]->SetRank(CGame::GetRanking(m_nPlayerNum));
					}

					m_bAnnoyS = true;
				}
			}
		}
	}
}

//=============================================================================
// 強い減速ひよこがくっつく処理
//=============================================================================
void CPlayer::ChaseAnnoyS(void)
{
	if (m_pAnnoyChick[m_nPlayerNum] != NULL)
	{
		// 位置更新
		m_pAnnoyChick[m_nPlayerNum]->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y + 60.0f, m_pos.z));

		// 食らっている時間をカウント
		m_nAnnoySTimer++;

		if (m_nAnnoySTimer > 100)
		{// 一定時間たったら
			m_nAnnoySTimer = 0;
			m_pAnnoyChick[m_nPlayerNum]->Uninit();
			m_pAnnoyChick[m_nPlayerNum] = NULL;
		}

		if (m_bDamage == false)
		{
			m_bDamage = true;
			m_State = PLAYERSTATE_SPEEDDOWN;
		}
	}
}

//=============================================================================
// キャラ同士の衝突判定処理
//=============================================================================
void CPlayer::CollisionCharacter(void)
{
	CPlayer **pPlayer = CGame::GetPlayer();

	for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
	{// プレイヤーカウント
		if (pPlayer[nCntMember] != NULL)
		{// NULL以外
			if (pPlayer[nCntMember] != this)
			{// 自分以外
			 // 距離計算
				D3DXVECTOR3 pos = pPlayer[nCntMember]->GetPos();
				float fLenght = sqrtf(powf(pos.x - m_pos.x, 2.0f) + powf(pos.z - m_pos.z, 2.0f));

				if (fLenght < 10.0f)
				{// 範囲内
				 // 角度計算
					float fAngle = atan2f(m_pos.x - pos.x, m_pos.z - pos.z);

					// 位置調整
					m_pos = pos + D3DXVECTOR3(sinf(fAngle) * 10.0f, 0.0f, cosf(fAngle) * 10.0f);
				}
			}
		}
	}
}
//=============================================================================
// プレイヤーのモーション
//=============================================================================
void CPlayer::UpdateMotion(void)
{
	//モーション
	KEY *pKey, *pNextKey;
	float fRateMotion;
	float fDiffMotion;
	D3DXVECTOR3 rotmotion;
	D3DXVECTOR3 posmotion;
	//D3DXVECTOR3 posParent;


	//キーが最大数を上回らないように
	if (m_aMotionInfo[m_nMotionType].nNumKey <= m_nKey)
	{
		m_nKey = 0;
	}

	//モーション更新
	for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
	{
		if (m_apModel[nCntParts] != NULL)
		{
			//現在のキーを取得
			pKey = &m_pKeyInfo[m_nMotionType][m_nKey].aKey[nCntParts];
			//次のキーを取得
			pNextKey = &m_pKeyInfo[m_nMotionType][(m_nKey + 1) % m_aMotionInfo[m_nMotionType].nNumKey].aKey[nCntParts];

			//現在のキーから次のキーへの再生フレーム数におけるモーションカウンターの相対値を算出
			fRateMotion = (float)m_nCountFlame / (float)m_pKeyInfo[m_nMotionType][m_nKey].nFrame;

			//ROT
			//現在のキーと次のキーの各要素の差分を算出
			fDiffMotion = pNextKey->frotX - pKey->frotX;
			//相対値を差分を使って各要素の値を算出
			rotmotion.x = pKey->frotX + (fDiffMotion * fRateMotion);

			//POS
			//現在のキーと次のキーの各要素の差分を算出
			fDiffMotion = pNextKey->fposX - pKey->fposX;
			//相対値を差分を使って各要素の値を算出
			posmotion.x = pKey->fposX + (fDiffMotion * fRateMotion);


			//現在のキーと次のキーの各要素の差分を算出
			fDiffMotion = pNextKey->frotY - pKey->frotY;
			//相対値を差分を使って各要素の値を算出
			rotmotion.y = pKey->frotY + (fDiffMotion * fRateMotion);
			//POS
			//現在のキーと次のキーの各要素の差分を算出
			fDiffMotion = pNextKey->fposY - pKey->fposY;
			//相対値を差分を使って各要素の値を算出
			posmotion.y = pKey->fposY + (fDiffMotion * fRateMotion);


			//現在のキーと次のキーの各要素の差分を算出
			fDiffMotion = pNextKey->frotZ - pKey->frotZ;
			//相対値を差分を使って各要素の値を算出
			rotmotion.z = pKey->frotZ + (fDiffMotion * fRateMotion);
			//POS
			//現在のキーと次のキーの各要素の差分を算出
			fDiffMotion = pNextKey->fposZ - pKey->fposZ;
			//相対値を差分を使って各要素の値を算出
			posmotion.z = pKey->fposZ + (fDiffMotion * fRateMotion);


			//パーツを動かす
			m_apModel[nCntParts]->SetRot(rotmotion);

			//POS
			m_apModel[nCntParts]->SetPos(D3DXVECTOR3(m_OffSetPos[nCntParts].x + posmotion.x,
				m_OffSetPos[nCntParts].y + posmotion.y,
				m_OffSetPos[nCntParts].z + posmotion.z));

		}
	}

	//ループの判定
	switch (m_aMotionInfo[m_nMotionType].bLoop)
	{
	case true:
		//ループする
		//フレームを進める
		m_nCountFlame++;
		//キーの更新
		if (m_nCountFlame >= m_pKeyInfo[m_nMotionType][m_nKey].nFrame)
		{
			if (m_aMotionInfo[m_nMotionType].nNumKey - 1 == m_nKey)
			{
				m_nKey = 0;
			}
			else
			{
				m_nKey += 1;
			}
			m_nCountFlame = 0;
		}

		break;
	case false:
		//ループしない
		if (m_aMotionInfo[m_nMotionType].nNumKey - 1 > m_nKey)
		{//フレームを進める
			m_nCountFlame++;
		}
		else if (m_aMotionInfo[m_nMotionType].nNumKey - 1 == m_nKey)
		{
			//if (m_nAnimnow == PLAYERANIM_ATTACK)
			//{//攻撃モーション
			//	m_nAttackDelay++;
			//	if (m_nAttackDelay > 20)
			//	{
			//		m_bAttack = false;
			//		m_nAttackDelay = 0;
			//	}
			//}
			m_bMotionEnd = true;
		}
		//キーの更新
		if (m_nCountFlame >= m_pKeyInfo[m_nMotionType][m_nKey].nFrame)
		{
			if (m_aMotionInfo[m_nMotionType].nNumKey > m_nKey)
			{
				m_nKey += 1;
			}
			m_nCountFlame = 0;
		}
		break;
	}

#ifdef  _DEBUG
	CDebugProc::Print(" Numキー  : (%d)\n", m_nKey);
	CDebugProc::Print(" m_nCountFlame  : (%d)\n", m_nCountFlame);

#endif

}

//=============================================================================
// ファイル読み込み
//=============================================================================
void CPlayer::FileLoad(void)
{
	//デバイスを取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//ファイル読み込み用変数
	FILE *pFile;		//ファイルポインタ
	char *pStrcur;		//現在の先頭の文字列
	char aLine[256];	//文字列
	char aStr[256];		//一時保存文字列
	int nIndex = 0;		//現在のインデックス
	int nWord = 0;		//ポップで返された値を保持

	D3DXVECTOR3 ParentPos;	//親の位置情報を取得
#if 1
							//ファイルを開く 読み込み
	pFile = fopen(FILE_NAME_PRISONER, "r");
	//NULLチェック
	if (pFile != NULL)
	{
		for (int nCntMotion = 0; nCntMotion < MAX_MOTION; )
		{
			//文字列の先頭を設定
			pStrcur = ReadLine(pFile, &aLine[0]);
			//文字列を取り出す
			strcpy(aStr, pStrcur);

			//文字列のデータ 比較する文字列 比較する文字数
			if (memcmp(pStrcur, "NUM_MODEL = ", strlen("NUM_MODEL = ")) == 0)
			{
				//頭出し
				pStrcur += strlen("NUM_MODEL = ");
				//文字列の先頭を設定
				strcpy(aStr, pStrcur);
				//文字列抜き出し
				g_nNumModel = atoi(pStrcur);

				for (int nCntModel = 0; nCntModel < g_nNumModel; nCntModel++)
				{
					//文字列の先頭を設定
					pStrcur = ReadLine(pFile, &aLine[0]);
					//文字列を取り戻す
					strcpy(aStr, pStrcur);
					if (memcmp(pStrcur, "MODEL_FILENAME = ", strlen("MODEL_FILENAME = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("MODEL_FILENAME = ");

						//文字列を設定
						strcpy(aStr, pStrcur);

						//必要な文字列の最後の文字までの文字数を数える
						int nNullNum = PopString(pStrcur, &aStr[0]);

						//文字列を取り戻す
						strcpy(aStr, pStrcur);

						//最後の文字にNULL文字を入れる
						aStr[nNullNum - 1] = '\0';

						//対象の文字列から抜き出し
						strcpy(&g_aFileNameModel[nCntModel][0], aStr);

						// Xファイルの読み込み
						D3DXLoadMeshFromX(&g_aFileNameModel[nCntModel][0],
							D3DXMESH_SYSTEMMEM,
							pDevice,
							NULL,
							&m_pBuffMat[nCntModel],
							NULL,
							&m_nNumMat[nCntModel],
							&m_pMesh[nCntModel]);
					}
				}
				//文字列の先頭を設定
				pStrcur = ReadLine(pFile, &aLine[0]);
				//文字列を取り出す
				strcpy(aStr, pStrcur);
			}


			//文字列のデータ 比較する文字列 比較する文字数
			if (memcmp(pStrcur, "CHARACTERSET", 12) == 0)
			{
				while (1)
				{
					//文字列の先頭を設定
					pStrcur = ReadLine(pFile, &aLine[0]);
					//文字列を取り出す
					strcpy(aStr, pStrcur);
					if (memcmp(pStrcur, "NUM_PARTS = ", strlen("NUM_PARTS = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("NUM_PARTS = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						m_nNumParts = atoi(pStrcur);
					}
					if (memcmp(pStrcur, "PARTSSET", strlen("PARTSSET")) == 0)
					{
						while (1)
						{
							//文字列の先頭を設定
							pStrcur = ReadLine(pFile, &aLine[0]);

							//INDEXを読み込み
							if (memcmp(pStrcur, "INDEX = ", strlen("INDEX = ")) == 0)
							{
								//頭出し
								pStrcur += strlen("INDEX = ");
								//文字列の先頭を設定
								strcpy(aStr, pStrcur);
								//文字列抜き出し
								nIndex = atoi(pStrcur);
							}
							//PARENTを読み込み
							if (memcmp(pStrcur, "PARENT = ", strlen("PARENT = ")) == 0)
							{
								//頭出し
								pStrcur += strlen("PARENT = ");
								//文字列の先頭を設定
								strcpy(aStr, pStrcur);
								//文字列抜き出し
								m_aIndexParent[nIndex] = atoi(pStrcur);
							}
							//POSを読み込み
							if (memcmp(pStrcur, "POS = ", strlen("POS = ")) == 0)
							{
								//頭出し
								pStrcur += strlen("POS = ");
								//文字列の先頭を設定
								strcpy(aStr, pStrcur);

								//文字数を返してもらう
								nWord = PopString(pStrcur, &aStr[0]);
								//文字列変換
								m_aKayOffset[nIndex].fposX = (float)atof(pStrcur);
								//文字数分進める
								pStrcur += nWord;

								//文字数を返してもらう
								nWord = PopString(pStrcur, &aStr[0]);
								//文字列変換
								m_aKayOffset[nIndex].fposY = (float)atof(pStrcur);
								//文字数分進める
								pStrcur += nWord;

								//文字数を返してもらう
								nWord = PopString(pStrcur, &aStr[0]);
								//文字列変換
								m_aKayOffset[nIndex].fposZ = (float)atof(pStrcur);

							}
							//ROTを読み込み
							if (memcmp(pStrcur, "ROT = ", strlen("ROT = ")) == 0)
							{
								//頭出し
								pStrcur += strlen("ROT = ");
								//文字列の先頭を設定
								strcpy(aStr, pStrcur);

								//文字数を返してもらう
								nWord = PopString(pStrcur, &aStr[0]);
								//文字列変換
								m_aKayOffset[nIndex].frotX = (float)atof(pStrcur);

								//文字数分進める
								pStrcur += nWord;
								//文字数を返してもらう
								nWord = PopString(pStrcur, &aStr[0]);
								//文字列変換
								m_aKayOffset[nIndex].frotY = (float)atof(pStrcur);

								//文字数分進める
								pStrcur += nWord;
								//文字数を返してもらう
								nWord = PopString(pStrcur, &aStr[0]);
								//文字列変換
								m_aKayOffset[nIndex].frotZ = (float)atof(pStrcur);
							}
							//パーツセット終了
							else if (memcmp(pStrcur, "END_PARTSSET", strlen("END_PARTSSET")) == 0)
							{
								//NULLチェック
								if (m_apModel[nIndex] == NULL)
								{//動的確保
									m_apModel[nIndex] = new CModel;
									//NULLチェック
									if (m_apModel[nIndex] != NULL)
									{
										//モデルの生成
										m_apModel[nIndex]->BindModel(m_pMesh[nIndex], m_pBuffMat[nIndex], m_nNumMat[nIndex]);
										m_apModel[nIndex]->Init();
									}
								}

								//モデルを生成	オフセット設定
								/*m_apModel[nIndex] = CModel::Create(
								D3DXVECTOR3(m_pos.x + m_aKayOffset[nIndex].fposX,
								m_pos.y + m_aKayOffset[nIndex].fposY,
								m_pos.z + m_aKayOffset[nIndex].fposZ), m_rot);*/

								m_apModel[nIndex]->SetPos(D3DXVECTOR3(m_pos.x + m_aKayOffset[nIndex].fposX,
									m_pos.y + m_aKayOffset[nIndex].fposY,
									m_pos.z + m_aKayOffset[nIndex].fposZ));

								//posを代入
								ParentPos = m_apModel[nIndex]->GetPos();
								m_OffSetPos[nIndex] = m_apModel[nIndex]->GetPos();

								//モデルを割り当て
								m_apModel[nIndex]->BindModel(m_pMesh[nIndex], m_pBuffMat[nIndex], m_nNumMat[nIndex]);

								if (m_aIndexParent[nIndex] == -1)
								{
									//モデルの親を指定
									m_apModel[nIndex]->SetParent(NULL);
									ParentPos = m_apModel[nIndex]->GetPos();
								}
								else
								{
									//モデルの親を指定
									m_apModel[nIndex]->SetParent(m_apModel[m_aIndexParent[nIndex]]);
								}

								break;
							}
						}
					}
					//キャラクターセット終了
					else if (memcmp(pStrcur, "END_CHARACTERSET", strlen("END_CHARACTERSET")) == 0)
					{
						break;
					}
				}
				//文字列の先頭を設定
				pStrcur = ReadLine(pFile, &aLine[0]);
				//文字列を取り出す
				strcpy(aStr, pStrcur);
			}

			//モーション読み込み
			if (memcmp(pStrcur, "MOTIONSET", strlen("MOTIONSET")) == 0)
			{
				//頭出し
				pStrcur += strlen("MOTIONSET");

				while (1)
				{
					//文字列の先頭を設定
					pStrcur = ReadLine(pFile, &aLine[0]);
					//文字列を取り出す
					strcpy(aStr, pStrcur);

					if (memcmp(pStrcur, "LOOP = ", strlen("LOOP = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("LOOP = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);

						switch (atoi(pStrcur))
						{
						case 0:
							//文字列抜き出し
							m_aMotionInfo[nCntMotion].bLoop = false;
							break;
						case 1:
							//文字列抜き出し
							m_aMotionInfo[nCntMotion].bLoop = true;
							break;
						}
						//文字列の先頭を設定
						pStrcur = ReadLine(pFile, &aLine[0]);
					}
					if (memcmp(pStrcur, "NUM_KEY = ", strlen("NUM_KEY = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("NUM_KEY = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						m_aMotionInfo[nCntMotion].nNumKey = atoi(pStrcur);

						//文字列の先頭を設定
						pStrcur = ReadLine(pFile, &aLine[0]);
						//文字列を取り出す
						strcpy(aStr, pStrcur);
					}

					//キーの設定
					for (int nCntKey = 0; nCntKey < m_aMotionInfo[nCntMotion].nNumKey;)
					{
						if (memcmp(pStrcur, "KEYSET", strlen("KEYSET")) == 0)
						{
							//頭出し
							pStrcur += strlen("KEYSET");
							//文字列の先頭を設定
							strcpy(aStr, pStrcur);
							//文字列の先頭を設定
							pStrcur = ReadLine(pFile, &aLine[0]);

							if (memcmp(pStrcur, "FRAME = ", strlen("FRAME = ")) == 0)
							{
								//頭出し
								pStrcur += strlen("FRAME = ");

								m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].nFrame = atoi(pStrcur);

								//文字列の先頭を設定
								strcpy(aStr, pStrcur);
								//文字列の先頭を設定
								pStrcur = ReadLine(pFile, &aLine[0]);
							}

							//パーツ分回す
							for (int nCntParts = 0; nCntParts < m_nNumParts;)
							{
								if (memcmp(pStrcur, "KEY", strlen("KEY")) == 0)
								{
									//文字列の先頭を設定
									pStrcur = ReadLine(pFile, &aLine[0]);

									if (memcmp(pStrcur, "POS = ", strlen("POS = ")) == 0)
									{
										//頭出し
										pStrcur += strlen("POS = ");
										//文字列の先頭を設定
										strcpy(aStr, pStrcur);

										//文字数を返してもらう
										nWord = PopString(pStrcur, &aStr[0]);
										//POS.X代入
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].fposX = (float)atof(pStrcur);
										//文字数分進める
										pStrcur += nWord;

										//文字数を返してもらう
										nWord = PopString(pStrcur, &aStr[0]);
										//POS.Y代入
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].fposY = (float)atof(pStrcur);
										//文字数分進める
										pStrcur += nWord;

										//文字数を返してもらう
										nWord = PopString(pStrcur, &aStr[0]);
										//POS.Z代入
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].fposZ = (float)atof(pStrcur);
										//文字列の先頭を設定
										pStrcur = ReadLine(pFile, &aLine[0]);
									}
									//ROTを読み込み
									if (memcmp(pStrcur, "ROT = ", strlen("ROT = ")) == 0)
									{
										//頭出し
										pStrcur += strlen("ROT = ");
										//文字列の先頭を設定
										strcpy(aStr, pStrcur);

										//文字数を返してもらう
										nWord = PopString(pStrcur, &aStr[0]);
										//RotX
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].frotX = (float)atof(pStrcur);
										//文字数分進める
										pStrcur += nWord;

										//文字数を返してもらう
										nWord = PopString(pStrcur, &aStr[0]);
										//RotY
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].frotY = (float)atof(pStrcur);
										//文字数分進める
										pStrcur += nWord;

										//文字数を返してもらう
										nWord = PopString(pStrcur, &aStr[0]);
										//RotZ
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].frotZ = (float)atof(pStrcur);

										//文字列の先頭を設定
										pStrcur = ReadLine(pFile, &aLine[0]);
									}
									if (memcmp(pStrcur, "END_KEY", strlen("END_KEY")) == 0)
									{
										//頭出し
										pStrcur += strlen("END_KEY");
										//文字列の先頭を設定
										strcpy(aStr, pStrcur);
										//文字列の先頭を設定
										pStrcur = ReadLine(pFile, &aLine[0]);
										//パーツのカウントを進める
										nCntParts++;
									}
								}
								else
								{
									//文字列の先頭を設定
									pStrcur = ReadLine(pFile, &aLine[0]);
								}
							}
							if (memcmp(pStrcur, "END_KEYSET", strlen("END_KEYSET")) == 0)
							{
								//文字列の先頭を設定
								pStrcur = ReadLine(pFile, &aLine[0]);
								//カウントを進める
								nCntKey++;
							}
						}
						else
						{
							//文字列の先頭を設定
							pStrcur = ReadLine(pFile, &aLine[0]);
						}

					}
					if (memcmp(pStrcur, "END_MOTIONSET", strlen("END_MOTIONSET")) == 0)
					{
						//モーションの情報をセット
						m_pKeyInfo[nCntMotion] = &m_aMotionInfo[nCntMotion].aKayInfo[0];
						nCntMotion++;
						break;
					}
				}
			}
			//スクリプトの終わり
			if (memcmp(pStrcur, "END_SCRIPT	", strlen("END_SCRIPT")) == 0)
			{
				break;
			}
		}
	}
	else
	{	//ファイルが開けなかった
		printf("ファイルが開けませんでした\n");
	}
	//ファイルを閉じる
	fclose(pFile);
#endif

}

//=============================================================================
//　ファイル読み込み無効文を排除
//=============================================================================
char *CPlayer::ReadLine(FILE *pFile, char *pDst)
{
	while (1)
	{
		//１行分読み込み
		fgets(&pDst[0], 256, pFile);

		//文字列のデータ 比較する文字列 比較する文字数
		if (memcmp(pDst, "#", strlen("#")) == 0)
		{
			pDst += strlen("\n");
		}
		//文字列のデータ 比較する文字列 比較する文字数
		else if (memcmp(pDst, "\t", strlen("\t")) == 0)
		{
			pDst += strlen("\t");
			while (1)
			{
				if (memcmp(pDst, "\t", strlen("\t")) == 0)
				{
					pDst += strlen("\t");
				}
				else
				{
					break;
				}
			}
			break;
		}
		//文字列のデータ 比較する文字列 比較する文字数
		else if (memcmp(pDst, " ", strlen(" ")) == 0)
		{
			pDst += strlen(" ");
			while (1)
			{
				if (memcmp(pDst, " ", strlen(" ")) == 0)
				{
					pDst += strlen(" ");
				}
				else
				{
					break;
				}
				break;
			}
		}
		//文字列のデータ 比較する文字列 比較する文字数
		else if (memcmp(pDst, "\n", strlen("\n")) == 0)
		{
			pDst += strlen("\n");
		}
		else
		{
			break;
		}
	}
	return pDst;
}

//=============================================================================
//　ファイル読み込み先頭を排除
//=============================================================================
char * CPlayer::GetLineTop(char * pStr)
{
	while (1)
	{
		//文字列のデータ 比較する文字列 比較する文字数
		if (memcmp(pStr, " ", strlen(" ")) == 0)
		{
			pStr += strlen(" ");
			while (1)
			{
				if (memcmp(pStr, " ", strlen(" ")) == 0)
				{
					pStr += strlen(" ");
				}
				else
				{
					break;
				}
				break;
			}
		}
		//文字列のデータ 比較する文字列 比較する文字数
		else if (memcmp(pStr, "\t", strlen("\t")) == 0)
		{
			pStr += strlen("\t");
			while (1)
			{
				if (memcmp(pStr, "\t", strlen("\t")) == 0)
				{
					pStr += strlen("\t");
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			break;
		}
	}
	return pStr;
}

//=============================================================================
//　文字数を返す
//=============================================================================
int CPlayer::PopString(char * pStr, char * pDest)
{
	int nWord = 0;

	while (1)
	{	//頭出し
		pStr += 1;
		nWord += 1;
		if (memcmp(pStr, " ", strlen(" ")) == 0)
		{	//頭出し
			pStr = "\0";
			nWord += 1;
			break;
		}
		if (memcmp(pStr, "\t", strlen("\t")) == 0)
		{	//頭出し
			pStr = "\0";
			nWord += (int)strlen("\t");
			break;
		}
		//文字列のデータ 比較する文字列 比較する文字数
		else if (memcmp(pStr, "\n", strlen("\n")) == 0)
		{
			//頭出し
			nWord += (int)strlen("\n");
			break;
		}
	}
	strcpy(pDest, pStr);
	//文字列の数を返す
	return nWord;
}