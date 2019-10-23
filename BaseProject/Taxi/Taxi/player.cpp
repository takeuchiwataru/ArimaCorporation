//=============================================================================
//
// プレイヤーの処理 [player.cpp]
// Author : 佐藤安純 Sato_Asumi
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

//=============================================================================
// マクロ定義
//=============================================================================
#define VECTOR_ZERO		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))				//ベクトルの初期化
#define FAILE_NAME		("data\\TEXT\\Player\\PlayerState.txt")		//読み込むファイル名
#define FILE_TEXTURE	("data\\TEXTURE\\modeltex\\carup.jpg")		//テクスチャの読み込み
#define LEFT_FRONT_POS	(D3DXVECTOR3(-26.0f, 3.0f, 45.0f))			//左前タイヤ位置
#define LEFT_BACK_POS	(D3DXVECTOR3(-26.0f, 3.0f, -40.0f))			//左後ろタイヤ位置
#define RIGHT_FRONT_POS	(D3DXVECTOR3(26.0f, 3.0f, 45.0f))			//右前タイヤ位置
#define RIGHT_BACK_POS	(D3DXVECTOR3(26.0f, 3.0f, -40.0f))			//右後ろタイヤ位置
#define ACCLE_TIME		(60.0f * 4)									//加速させる時間
#define AVERAGE			(2.0f)										//平均する値
#define ROLLOVER_STOP	(0.6f)										//横転防止角度
#define GRAVITY_FRONT	(0.5f)										//前方の重力
#define GRAVITY_BACK	(0.2f)										//後方の重力
#define DECELERATION	(0.5f)										//減速の割合
#define START_ENGINE	(200)										//エンジン音の再生時間
#define START_GEARCHANGE (300)										//スタート時のギア切替時間
#define EGG_SCALE		(10.0f)										//卵の大きさ
#define EGG_RANGE		(50.0f)										// 卵とプレイヤーの距離
#define EGG_POS			(7)											// 卵同士の間隔の広さ（増やすと広くなる）
#define SPEEDUP_TIME	(60)										// 加速している時間
#define SPEED			(5.0f)										// 加速する量

// プレイヤー情報
#define PLAYER_ACCEL	(1.0f)										//加速値（前進）
#define PLAYER_BRAKS	(-0.25f)									//加速値（後進）
#define PLAYER_DOWN		(0.055f)									//減速度
#define PLAYER_ADDROT	(0.015f)										//回転量

//車体の角度
#define SHAKE_X			(0.007f)									//X軸の揺れ
#define INPULS_X		(0.01f)										//X軸に加える角度
#define SHAKE_Z			(0.02f)										//Z軸の揺れ
#define SHAKE_DRIFT		(0.005f)										//ドリフト時の角度加算
#define SHAKE_BRAK		(0.01f)										//ブレーキ時の角度

//タイヤの番号
#define LEFT_FRONT		(0)											//左前タイヤ
#define LEFT_BACK		(1)											//左後ろタイヤ
#define RIGHT_FRONT		(2)											//右前タイヤ
#define RIGHT_BACK		(3)											//右後ろタイヤ

#define RIVER_SOUND_RANGE		(1000.0f)							// 川の音が聞こえる範囲
#define FOUNTAIN_SOUND_RANGE	(1000.0f)							// 噴水の音が聞こえる範囲

// コンボ時のスコア加算
#define JUMP_ADD_SCORE	(100)										// ジャンプ時のスコア加算量
#define JUMP_FREAM_TIME	(25)										// ジャンプ時のスコア加算時間
#define DRIFT_COMBO_TIME (25)										// ドリフト時のスコア加算時間
#define THROUGH_COMBO_TIME	(1)									// すれすれ時のスコア加算時間
#define DRIFT_SPACE_SMALL	(10)
#define DRIFT_SPACE_BIG		(250)
//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CModel * CPlayer::m_pModel = NULL;		//モデルのパーツポインタ
int	CPlayer::m_nMaxModel = 0;
int CPlayer::m_nMaxParts = 0;
int CPlayer::m_nMaxMotion = 0;
CMotion::MOTION_INFO * CPlayer::m_pMotionInfo = NULL;
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;

//=============================================================================
// 生成処理
//=============================================================================
CPlayer * CPlayer::Create(const D3DXVECTOR3 pos, int nPlayerNum, int nControllerNum)
{
	//インスタンスの生成
	CPlayer * pPlayer;
	pPlayer = new CPlayer;

	//初期化処理
	pPlayer->Init();
	pPlayer->m_nPlayerNum = nPlayerNum;
	pPlayer->m_nControllerNum = nControllerNum;

	//設定処理
	pPlayer->Set(pos, VECTOR_ZERO);
	return pPlayer;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CPlayer::CPlayer() : CScene(3, OBJTYPE_PLAYER) {}

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
	CLoadTextMotion * pLoadTextMotion = NULL;
	CManager::MODE mode = CManager::GetMode();

	if (mode == CManager::MODE_GAME)
	{//ゲーム
		pLoadTextMotion = CGame::GetPlayerMotion();
	}
	else if (mode == CManager::MODE_TUTORIAL)
	{//チュートリアル
		pLoadTextMotion = CTutorial::GetPlayerMotion();
	}

	m_pMotionInfo = pLoadTextMotion->GetMotionInfo();					//モーション情報の取得
	m_nMaxMotion = pLoadTextMotion->GetMaxMotion();						//モーションの最大数の取得

																		//モデル情報を取得
	CLoadTextMotion::MODEL_INFO ModelInfo = pLoadTextMotion->GetModelInfo();
	m_nMaxModel = ModelInfo.nMaxModel;	//モデルの最大数の取得
	m_nMaxParts = ModelInfo.nMaxParts;	//モデルのパーツ最大数の取得

										//モデルの生成
	for (int nCntParts = 0; nCntParts < ModelInfo.nMaxParts; nCntParts++)
	{
		m_pModel = CModel::Create(ModelInfo.pOfSetPos[nCntParts], &ModelInfo.paFileName[nCntParts][0]);

		//テクスチャの割当て
		//if (m_pModel != NULL) { m_pModel->BindTexture(m_pTexture); }
	}

	//モデルの親設定
	for (int nCntParts = 0; nCntParts < ModelInfo.nMaxParts; nCntParts++)
	{
		if (nCntParts == 0)
		{
			m_pModel->SetParent(NULL);
			break;
		}
	}
}

//=============================================================================
// Xファイルの破棄
//=============================================================================
void CPlayer::UnloadModel(void)
{
	if (m_pModel != NULL)
	{
		delete m_pModel;
		m_pModel = NULL;
	}

	//モーションデータの破棄
	if (m_pMotionInfo != NULL)
	{
		m_pMotionInfo = NULL;
	}

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
	m_state = STATE_DRIVE;					//状態設定
	m_StateSpeed = STATE_SPEED_STOP;		//スピードの状態設定
	m_StateHandle = HANDLE_MAX;				//ハンドルの状態
	m_PlayerInfo.nCountTime = 0;			//カウンター
	m_PlayerInfo.fAccel = PLAYER_ACCEL;		//加速値（前進）
	m_PlayerInfo.fBraks = PLAYER_BRAKS;		//加速値（後進）
	m_PlayerInfo.fDown = PLAYER_DOWN;		//減速値
	m_PlayerInfo.fAddRot = PLAYER_ADDROT;	//加える回転値
	m_PlayerInfo.fDistance = 0.0f;			//距離
	m_PlayerInfo.FirstPos = VECTOR_ZERO;	//初期位置
	m_bJump = false;						//ジャンプ状態
	m_bControl = true;						//コントローラーフラグ
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
	m_bJumpSave = false;
	m_bSpeed = false;

	for (int nCntEggPos = 0; nCntEggPos < MAX_FRAME; nCntEggPos++)
	{
		m_OldEggRot[nCntEggPos] = VECTOR_ZERO;
		m_abJump[nCntEggPos] = false;
	}

	//卵のポインタを初期化
	for (int nCntEgg = 0; nCntEgg < MAX_EGG; nCntEgg++)
	{
		m_pEgg[nCntEgg] = NULL;
	}

	//CreateTire();			//タイヤの生成

	if (m_pMotion == NULL)	//モーションの生成
	{
		m_pMotion = CMotion::Create();

		if (m_pMotion != NULL)
		{
			//モデルデータの設定
			m_pMotion->SetModel(&m_pModel);			//モーションデータにモデル情報を渡す
			m_pMotion->SetMaxModel(m_nMaxParts);	//モデルパーツの最大数を渡す

													//モーションデータの設定
			m_pMotion->SetMotion(m_pMotionInfo);	//モーション情報の取得
			m_pMotion->SetMaxMotion(m_nMaxMotion);	//モーションの最大数の取得

													//初期化処理
			m_pMotion->Init();
		}
	}

	//モデルの最小値・最大値の取得
	m_vtxMaxModel = m_pModel->GetVtxMax();
	m_vtxMinModel = m_pModel->GetVtxMin();
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	//モーションの破棄
	if (m_pMotion != NULL)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = NULL;
	}

	//タイヤモデルの破棄
	for (int nCntEgg = 0; nCntEgg < MAX_EGG; nCntEgg++)
	{
		if (m_pEgg[nCntEgg] != NULL)
		{
			m_pEgg[nCntEgg] = NULL;
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
		ControlKey();
	}
	else
	{//ハンドルを握っていない状態にする
		SetStateHandle(HANDLE_MAX);
	}

	/*if (m_pos.y < 0.0f)
	{
	m_pos.y = 0.0f;
	m_move.y = 0.0f;
	}*/

	//タイムアップ状態なら以降は更新しない
	//if (CTime::GetTime() == 0 && CManager::MODE_GAME == CManager::GetMode()) { return; }

	UpdateMove();			// 移動処理

	UpdateField();

	CollisionFeed();		// 餌の当たり判定

	ChaseEgg();	// 卵がついてくる処理
	BulletEgg();

	/*UpdateStateJump();		// ジャンプ状態の更新処理
	RiverInfluence();		// 川による影響
	*/
	CollisionObject();		// オブジェクトとの当たり判定
	/*UpdateShake();			//車の揺れの処理
	//CollisitionWall();	// 壁との当たり判定
	UpdateSmoke(TirePos);	//煙の更新処理
	UpdateGrass(TirePos);	//草エフェクトの更新処理
	*/
	/*m_nCntCombo++;

	// コンボ数
	if (m_pCombo != NULL && m_pCombo->GetFream() == 0)
	{	// NULLチェック & フレームが0の場合
	m_nCntCombo = 0;
	m_pCombo->Uninit();
	m_pCombo = NULL;
	}*/

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

	//モデルの描画
	if (m_pModel != NULL)
	{
		//描画する
		m_pModel->Draw(1.0f);
	}
}

//=============================================================================
// コントロールキー
//=============================================================================
void CPlayer::ControlKey(void)
{
	CInputKeyBoard * pInputKeyboard = CManager::GetInput();		//キーボードの取得
	CInputXPad * pXpad = CManager::GetXInput();					//ジョイパットの取得

																//前進後退の設定
	if (m_bDirive)
	{
		if ((pInputKeyboard->GetKeyboardPress(DIK_L) == true) ||
			(pXpad->GetPress(CInputXPad::XPADOTHER_TRIGGER_RIGHT, m_nControllerNum) == true) ||
			(pXpad->GetPress(XINPUT_GAMEPAD_RIGHT_SHOULDER, m_nControllerNum) == true))
		{
			SetState(STATE_DRIVE);		//前進状態に設定
		}
		if ((pInputKeyboard->GetKeyboardPress(DIK_K) == true) ||
			(pXpad->GetPress(CInputXPad::XPADOTHER_TRIGGER_LEFT, m_nControllerNum) == true) ||
			(pXpad->GetPress(XINPUT_GAMEPAD_LEFT_SHOULDER, m_nControllerNum) == true))
		{
			SetState(STATE_REVERSE);	//後退状態に設定
		}
	}

	//向きの設定
	if (m_StateSpeed != STATE_SPEED_STOP)
	{
		if ((pInputKeyboard->GetKeyboardPress(DIK_A) == true) || pXpad->GetPress(CInputXPad::XPADOTHER_STICK_L_LEFT, m_nControllerNum) == true || pXpad->GetPress(XINPUT_GAMEPAD_DPAD_LEFT, m_nControllerNum) == true)
		{ //左ハンドル状態
			SetStateHandle(HANDLE_LEFT);
		}
		else if ((pInputKeyboard->GetKeyboardPress(DIK_D) == true) || pXpad->GetPress(CInputXPad::XPADOTHER_STICK_L_RIGHT, m_nControllerNum) == true || pXpad->GetPress(XINPUT_GAMEPAD_DPAD_RIGHT, m_nControllerNum) == true)
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

	//アクセル
	if ((pInputKeyboard->GetKeyboardPress(DIK_K) == true) ||
		(pXpad->GetPress(CInputXPad::XPADOTHER_TRIGGER_LEFT, m_nControllerNum) == true) ||
		(pXpad->GetPress(XINPUT_GAMEPAD_LEFT_SHOULDER, m_nControllerNum) == true))
	{//減速状態
		SetStateSpeed(STATE_SPEED_BRAKS);
	}
	else if
		((pInputKeyboard->GetKeyboardPress(DIK_L) == true) ||
		(pXpad->GetPress(CInputXPad::XPADOTHER_TRIGGER_RIGHT, m_nControllerNum) == true) ||
			(pXpad->GetPress(XINPUT_GAMEPAD_RIGHT_SHOULDER, m_nControllerNum) == true))
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

				if (m_bSpeed == true)
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

	// ジャンプ
	if (m_bJump == false)
	{// ジャンプしていない
		m_bJumpSave = false;

		if (pInputKeyboard->GetKeyboardTrigger(DIK_W) == true || pXpad->GetTrigger(XINPUT_GAMEPAD_A, m_nControllerNum) == true)
		{// ジャンプキー
			m_bJumpSave = true;
			m_bJump = true;
			m_move.y += 7.5f;
		}
	}
	else
	{
		m_bJumpSave = false;
	}

	CSound *pSound = CManager::GetSound();

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
	m_OldPos = m_pos;	//前回の位置を保存する

	RemakeAngle(&m_rot.y);

	if (m_bSpeed == true)
	{// スピードアイテムを使ったとき
		m_nCountSpeed++;

		if (m_nCountSpeed > SPEEDUP_TIME)
		{
			m_bSpeed = false;
			m_nCountSpeed = 0;
		}
	}

	//状態ごとの更新処理
	switch (m_StateSpeed)
	{
	case STATE_SPEED_ACCEL:	//アクセル状態

							//ジャンプ状態なら
		if (m_bJump == true) { break; }

		/*CDebugProc::Print("初速加速あり\n");
		//速度を上げる
		m_PlayerInfo.nCountTime++;
		if (m_move.x < 3.0f && m_move.z < 3.0f &&m_move.x > -3.0f && m_move.z > -3.0f)
		{
		m_PlayerInfo.nCountTime = m_PlayerInfo.nCountTime*1.03f;
		CDebugProc::Print("初速UP状態 [1段階]\n");
		}
		else if (m_move.x < 6.0f && m_move.z < 6.0f &&m_move.x > -6.0f && m_move.z > -6.0f)
		{
		m_PlayerInfo.nCountTime = m_PlayerInfo.nCountTime*1.04f;
		CDebugProc::Print("初速UP状態 [2段階]\n");
		}
		else if (m_move.x < 9.0f && m_move.z < 9.0f &&m_move.x > -9.0f && m_move.z > -9.0f)
		{
		m_PlayerInfo.nCountTime = m_PlayerInfo.nCountTime*1.065f;
		CDebugProc::Print("初速UP状態 [3段階]\n");
		}

		//加速値を求める
		m_PlayerInfo.fAccel = (m_fMaxSpeed / ACCLE_TIME) * m_PlayerInfo.nCountTime;

		//減速させる
		if (m_state == STATE_DRIVE)
		{
		if (m_PlayerInfo.fAccel > m_fMaxSpeed) { m_PlayerInfo.fAccel = m_fMaxSpeed; }
		}
		else if (m_state == STATE_REVERSE)
		{
		if (m_PlayerInfo.fAccel < m_fMaxSpeed) { m_PlayerInfo.fAccel = m_fMaxSpeed; }
		}*/

		if (m_bSpeed == false)
		{
			m_fSpeed = m_PlayerInfo.fAccel * (m_PlayerInfo.nCountTime < 90 ? (m_PlayerInfo.nCountTime / 90) : 1.0f);
		}

		//進行方向の設定
		m_move.x += sinf(m_rot.y) * (m_fSpeed);
		m_move.z += cosf(m_rot.y) * (m_fSpeed);
		break;

	case STATE_SPEED_BRAKS: //ブレーキ状態

							//ジャンプ状態なら
		if (m_bJump == true) { break; }

		//速度を下げる
		//m_PlayerInfo.fAccel -= m_PlayerInfo.fBraks;

		/*if (m_state == STATE_DRIVE)
		{
		if (m_PlayerInfo.fAccel < 0.0f) { m_PlayerInfo.fAccel = 0.0f; }
		}
		else
		{
		if (m_PlayerInfo.fAccel > 0.0f) { m_PlayerInfo.fAccel = 0.0f; }
		}*/

		m_fSpeed = m_PlayerInfo.fBraks * (m_PlayerInfo.nCountTime < 90 ? (m_PlayerInfo.nCountTime / 90) : 1.0f);

		//進行方向の設定
		m_move.x += sinf(m_rot.y) * m_fSpeed;
		m_move.z += cosf(m_rot.y) * m_fSpeed;

		//揺れを無効にする
		m_bShake = false;
		break;
	case STATE_SPEED_DOWN: //ダウン状態
		m_PlayerInfo.nCountTime = 0;

		if (m_bSpeed == true)
		{// スピードアイテムを使ったとき
		 //進行方向の設定
			m_move.x += sinf(m_rot.y) * (m_fSpeed);
			m_move.z += cosf(m_rot.y) * (m_fSpeed);
		}

		break;
	}

	m_PlayerInfo.nCountTime++;

	//CDebugProc::Print("アクセル : %1f\n", m_PlayerInfo.fAccel);
	//CDebugProc::Print("スピード : %1f  %1f  %1f\n", m_move.x, m_move.y, m_move.z);

	//ハンドルの状態更新
	if (m_StateHandle == HANDLE_LEFT)
	{
		if (m_StateSpeed != STATE_SPEED_STOP)
			m_rot.y -= m_PlayerInfo.fAddRot * (m_PlayerInfo.nCountTime < 45 ? (m_PlayerInfo.nCountTime / 45) : 1.0f);
	}
	else if (m_StateHandle == HANDLE_RIGHT)
	{
		if (m_StateSpeed != STATE_SPEED_STOP)
			m_rot.y += m_PlayerInfo.fAddRot * (m_PlayerInfo.nCountTime < 45 ? (m_PlayerInfo.nCountTime / 45) : 1.0f);
	}

	// 重力
	//if (m_bShake == true)
	{
		m_move.y -= cosf(0) * 0.4f;
	}

	//中心座標更新
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;
	m_pos.z += m_move.z;

	//減速
	if (!m_bJump)
	{
		m_move.x += (0.0f - m_move.x) * m_PlayerInfo.fDown;
		m_move.z += (0.0f - m_move.z) * m_PlayerInfo.fDown;
	}

	//煙の表示
	/*if ((fDiffuse.x < 1.5f) && (fDiffuse.x > -1.5f))
	{
	if ((fDiffuse.z < 1.5f) && (fDiffuse.z > -1.5f))
	{
	m_bSmoke = false;
	m_bGrassEffect = false;		//草エフェクト非表示
	}
	else
	{
	m_bSmoke = true;
	}
	}
	else
	{
	m_bSmoke = true;
	}

	//お客さんを乗せるための停止判定
	if ((fDiffuse.x < 1.0f) && (fDiffuse.x > -1.0f))
	{
	if ((fDiffuse.z < 1.0f) && (fDiffuse.z > -1.0f))
	{
	m_bCustomrStop = true;
	}
	else
	{
	m_bCustomrStop = false;
	}
	}
	else
	{
	m_bCustomrStop = false;
	}*/

	/*if ((m_StateSpeed == STATE_SPEED_BRAKS))
	{
	//角度を変える
	m_rot.x += SHAKE_BRAK;
	}*/
}

//=============================================================================
// 揺れの更新処理
//=============================================================================
void CPlayer::UpdateShake(void)
{
	if (!m_bShake) { return; }

	m_nCntShake++;	//カウンターの加算

	if ((m_nCntShake % 8) == 0)
	{//車体を揺らす
		m_rot.x += SHAKE_X;

		if (((m_nCntShake % 24) == 0) && (m_state != STATE_SPEED_STOP))
		{
			int nRand = rand() % 5;

			if (nRand == 0)
			{
				m_rot.x += INPULS_X;
			}
		}
	}
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
					m_pos.y = fHight;					//地面の高さを取得
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
// 状態の設定
//=============================================================================
void CPlayer::SetState(CPlayer::STATE state)
{
	//値の反転
	if (m_state != state)
	{
		//m_PlayerInfo.fAccel *= -0.5f;
		//m_PlayerInfo.fBraks *= -1;
		//m_fMaxSpeed *= -1;

		CSound *pSound = CManager::GetSound();

		if (STATE_REVERSE == state)
		{// バック音
			pSound->StopSound(CSound::SOUND_LABEL_SE_ACCEL);
			pSound->PlaySoundA(CSound::SOUND_LABEL_SE_BACK);
		}
		else if (STATE_DRIVE == state)
		{
			pSound->StopSound(CSound::SOUND_LABEL_SE_BACK);
			pSound->SetVolume(CSound::SOUND_LABEL_SE_ACCEL, 0.7f);
			pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ACCEL);
		}
	}

	//状態の設定
	m_state = state;
}

//=============================================================================
// スピードの状態設定
//=============================================================================
void CPlayer::SetStateSpeed(CPlayer::STATE_SPEED state)
{
	//数値の設定
	if (m_StateSpeed != state)
	{
		m_PlayerInfo.nCountTime = 0;

		CSound *pSound = CManager::GetSound();

		/*if (STATE_SPEED_ACCEL == state && STATE_DRIVE == m_state)
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
// 車体の角度を求める
//=============================================================================
void CPlayer::CarCalculate(D3DXVECTOR3 * TirePos)
{
	if (m_bJump) { return; }
	//変数宣言
	float fHight = 0.0f;
	float fTireHight = 0.0f;
	bool bDecisionAngle = false;

	//車体の角度を求める
	if (!bDecisionAngle) { return; }

	//----------------------
	// X軸の角度を求める
	//----------------------
	float fHight00 = TirePos[LEFT_FRONT].y - TirePos[LEFT_BACK].y;			//左右の高さを求める
	float fHight01 = TirePos[RIGHT_FRONT].y - TirePos[RIGHT_BACK].y;

	float fLength00 = GetLength(TirePos[LEFT_FRONT], TirePos[LEFT_BACK]);		//２頂点の長さを求める
	float fLength01 = GetLength(TirePos[RIGHT_FRONT], TirePos[RIGHT_BACK]);

	float fAngleLeft = fHight00 / fLength00;		//角度を求める
	float fAngleRight = fHight01 / fLength01;
	RemakeAngle(&fAngleLeft);						//角度修正
	RemakeAngle(&fAngleRight);

	//平均値を求める
	float fAngleX = (fAngleLeft + fAngleRight) / AVERAGE;
	RemakeAngle(&fAngleX);

	//----------------------
	// Z軸の角度を求める
	//----------------------
	fHight00 = TirePos[LEFT_FRONT].y - TirePos[RIGHT_FRONT].y;			//前後の高さを求める
	fHight01 = TirePos[LEFT_BACK].y - TirePos[RIGHT_BACK].y;

	fLength00 = GetLength(TirePos[LEFT_FRONT], TirePos[RIGHT_FRONT]);		//２頂点の長さを求める
	fLength01 = GetLength(TirePos[LEFT_BACK], TirePos[RIGHT_BACK]);

	float fAngleFront = fHight00 / fLength00;		//角度を求める
	float fAngleBack = fHight01 / fLength01;
	RemakeAngle(&fAngleFront);						//角度修正
	RemakeAngle(&fAngleBack);

	//平均値を求める
	float fAngleZ = (fAngleFront + fAngleBack) / AVERAGE;
	RemakeAngle(&fAngleZ);

	//-------------------------
	// 向きの値修正(横転防止)
	//-------------------------
	fAngleX *= -1;
	fAngleZ *= -1;
	RemakeCarRot(&fAngleX);
	RemakeCarRot(&fAngleZ);

	//向きを設定する
	m_rot.x = fAngleX;
	m_rot.z = fAngleZ;

	//CDebugProc::Print("LEFT : %.2f, RIGHT : %.2f\n", fAngleLeft, fAngleRight);
	//CDebugProc::Print("FRONT : %.2f, BACK : %.2f\n", fAngleFront, fAngleBack);
	//CDebugProc::Print("ANGLE : X %.2f, Z %.2f\n", fAngleX, fAngleZ);
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
	/*if (m_state == STATE_DRIVE)
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
	CDebugProc::Print("位置 : X %.2f, Y %.2f, Z %.2f\n", m_pos.x, m_pos.y, m_pos.z);

	CDebugProc::Print("ジャンプ：%s\n", m_bJump ? "〇" : "×");

	//CDebugProc::Print("カウント : %f\n", m_PlayerInfo.nCountTime);

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
	if (m_nNumEgg >= MAX_EGG) { return; }

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

				if (pFeed->CollisionFeed(&m_pos, &m_OldPos))
				{// 衝突した
					EggAppear(pFeed);	// 卵出現
					pFeed->Uninit();	// 餌削除
					m_nNumEgg++;
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
			m_pEgg[m_nNumEgg] = CEgg::Create(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(EGG_SCALE, EGG_SCALE, EGG_SCALE), CEgg::EGGTYPE_ATTACK);
		}
		else if (pFeed->GetFeedType() == CFeed::FEEDTYPE_ANNOY)
		{// 妨害の卵生成
			m_pEgg[m_nNumEgg] = CEgg::Create(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(EGG_SCALE, EGG_SCALE, EGG_SCALE), CEgg::EGGTYPE_ANNOY);
		}
		else if (pFeed->GetFeedType() == CFeed::FEEDTYPE_SPEED)
		{// 加速の卵生成
			m_pEgg[m_nNumEgg] = CEgg::Create(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(EGG_SCALE, EGG_SCALE, EGG_SCALE), CEgg::EGGTYPE_SPEED);
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

	if (m_nCntFrame > MAX_FRAME)
	{
		m_nCntFrame = 0;
	}

	// 前のプレイヤーの角度を保存
	m_OldEggRot[m_nCntFrame] = m_rot;
	m_abJump[m_nCntFrame] = m_bJumpSave;

	if (m_nNumEgg <= 0) { return; }

	if (m_nNumEgg >= 1)
	{// 卵が一個の時
		if (m_pEgg[0]->GetState() == CEgg::EGGSTATE_CHASE)
		{
			// 前の向きを代入
			int nData = m_nCntFrame - EGG_POS;

			if (nData < 0)
			{
				nData += MAX_FRAME + 1;
			}

			// 卵の位置設定
			m_pEgg[0]->SetPosition(D3DXVECTOR3((sinf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE) + m_pos.x,
				m_pEgg[0]->SetHeight(),
				(cosf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE) + m_pos.z));

			// 卵の角度設定
			m_pEgg[0]->SetRot(m_OldEggRot[nData]);

			if (m_abJump[nData] == true)
			{
				m_pEgg[0]->Jump();
			}
		}
	}
	if (m_nNumEgg >= 2)
	{// 卵が二個の時
	 // 二個目
		int nData = m_nCntFrame - EGG_POS * 2;

		if (nData < 0)
		{
			nData += MAX_FRAME + 1;
		}

		m_pEgg[1]->SetPosition(D3DXVECTOR3((sinf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * 2) + m_pos.x,
			m_pEgg[1]->SetHeight(),
			(cosf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * 2) + m_pos.z));

		m_pEgg[1]->SetRot(m_OldEggRot[nData]);

		if (m_abJump[nData] == true)
		{
			m_pEgg[1]->Jump();
		}
	}
	if (m_nNumEgg >= 3)
	{
		// 三個目
		int nData = m_nCntFrame - EGG_POS * 3;

		if (nData < 0)
		{
			nData += MAX_FRAME + 1;
		}

		m_pEgg[2]->SetPosition(D3DXVECTOR3((sinf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * 3) + m_pos.x,
			m_pEgg[2]->SetHeight(),
			(cosf(m_OldEggRot[nData].y + D3DX_PI) * EGG_RANGE * 3) + m_pos.z));

		m_pEgg[2]->SetRot(m_OldEggRot[nData]);

		if (m_abJump[nData] == true)
		{
			m_pEgg[2]->Jump();
		}
	}
}

//=============================================================================
// 卵発射処理
//=============================================================================
void CPlayer::BulletEgg(void)
{
	if (m_nNumEgg > 0)
	{// 卵を持っているとき
		if (m_pEgg[0] != NULL && m_pEgg[0]->GetState() == CEgg::EGGSTATE_CHASE)
		{// 一個目の卵に情報が入っていて、プレイヤーについてくる時
			CInputKeyBoard * pInputKeyboard = CManager::GetInput();		//キーボードの取得
			CInputXPad * pXpad = CManager::GetXInput();					//ジョイパットの取得

			if (pInputKeyboard->GetKeyboardTrigger(DIK_SPACE) == true || pXpad->GetTrigger(XINPUT_GAMEPAD_B, m_nControllerNum) == true)
			{// 弾発射
				m_pEgg[0]->SetState(CEgg::EGGSTATE_BULLET);	// 状態を弾にする

				m_nNumEgg--;	// 所持数を減らす

				if (m_pEgg[0]->GetType() == CEgg::EGGTYPE_SPEED)
				{
					m_fSpeed += SPEED;

					m_bSpeed = true;

					m_pEgg[0]->Uninit();
				}

				// 情報入れ替え
				m_pEgg[0] = m_pEgg[1];
				m_pEgg[1] = m_pEgg[2];
				m_pEgg[2] = NULL;
			}
		}
	}
}