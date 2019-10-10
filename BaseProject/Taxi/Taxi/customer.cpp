//=============================================================================
//
// お客さんの処理 [customer.cpp]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "customer.h"
#include "manager.h"
#include "debugproc.h"
#include "input.h"
#include "gamecamera.h"
#include "scene.h"
#include "model.h"
#include "motion.h"
#include "wall.h"
#include "loadText.h"
#include "meshCylinder.h"
#include "destination.h"
#include "time.h"
#include "score.h"
#include "arrow.h"
#include "mark.h"
#include "reputation.h"
#include "satisfactionlevel.h"
#include "tutorial.h"
#include "sound.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define LENGTH			(500.0f)	//プレイヤーの半径
#define PUTIN_LENGTH	(100.0f)	//乗車半径
#define MOVE_SPEED		(2.8f)		//移動スピード
#define MOVE_ANGLE		(0.2f)		//角度の変化量
#define DOWN_POS		(70.0f)		//降車位置
#define COUNT_TIME		(60 * 2)	//移動時間
#define COUNT_ESCAPE	(60)		//逃げる時間
#define MONEY_ADD		(10)		//加算するお金
#define METER			(100.0f)	//100メートル
#define ESCAPE_LENGTH	(100.0f)	//逃げる範囲
#define CALLTAXI_LENGTH	(2000.0f)	//タクシーを呼ぶ範囲
#define MOTION_LENGTH	(1700.0f)	//モーション再生距離
#define DRAW_LENGTH		(4500)		//人の描画距離

//客ランクの距離
#define LOW_RANK		(15000.0f)
#define NORMAL_RANK		(20000.0f)
#define HIGHT_RANK		(25000.0f)

//客評価秒数
#define BAD			(5)
#define SLOW		(9)
#define NORMAL		(14)

//リザルトの最大数
#define MAX_RESULT	(3)

//色の定数
#define RED				(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f))		//赤色
#define YELLOW			(D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.5f))		//黄色
#define GREEN			(D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.5f))		//緑色
#define MAX_COL			(3)										//お客さんの色の最大数
#define GOALCOL			(D3DXCOLOR(0.1f, 1.0f, 0.0f, 0.5f))		//目的地の色
#define COLOR_ZERO		(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f))		//色の初期値

//シリンダーの定数
#define LAP				(8)			//横の頂点数
#define STAGE			(1)			//段数
#define HIGHT			(40.0f)		//高さ
#define GOAL_HIGHT		(100.0f)	//ゴール高さ

// 矢印の定数
#define ARROW_POS		(D3DXVECTOR3(CPlayer::GetPos().x, CPlayer::GetPos().y + 100.0f, CPlayer::GetPos().z))	// 矢印の位置

//マークの定数
#define OFFSET			(200.0f)	//オフセット

//=============================================================================
// 静的メンバ変数の初期化
//=============================================================================
bool CCustomer::m_bPutIn = false;

//=============================================================================
// 生成処理
//=============================================================================
CCustomer * CCustomer::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 GoalPos, const int nGoalTex, int nAddTime, int nRideTime, MODEL_TYPE type)
{
	//インスタンスの生成
	CCustomer * pCustomer;
	pCustomer = new CCustomer;

	//設定処理
	pCustomer->SetModelType(type);

	//初期化処理
	pCustomer->Init();

	//設定処理
	pCustomer->Set(pos, GoalPos);
	pCustomer->SetGoalTex(nGoalTex);
	pCustomer->SetAddTime(nAddTime);
	pCustomer->SetRideTime(nRideTime);
	return pCustomer;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CCustomer::CCustomer() : CHumanBace(3, CScene::OBJTYPE_CUSTOMER){}

//=============================================================================
// デストラクタ
//=============================================================================
CCustomer::~CCustomer(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCustomer::Init(void)
{
	//親クラスの初期化
	CHumanBace::Init();

	//変数の初期化
	m_OldPos	= VECTOR_ZERO;		//前回の座標
	m_OldDiffuse = VECTOR_ZERO;		//前回の差分
	m_MoveMotion = VECTOR_ZERO;		//モーションの移動
	m_state = STATE_MAX;			//状態設定
	m_OldState = STATE_MAX;			//前回の状態を保存する
	m_fAngle = 0.0f;				//角度
	m_pMeshCylinder = NULL;			//メッシュシリンダー
	m_pDestination = NULL;			//目的地画像
	m_nAddTime = 0;					//加算する時間
	m_pArrow = NULL;				//矢印のポインタ
	m_pMark = NULL;					//マークのポインタ
	m_Color = COLOR_ZERO;			//色
	m_bDrawMeshCylinder = false;	//シリンダーの描画フラグ
	m_bGotOn = false;				//乗った状態
	m_bResult = false;				//結果を求めたかどうか
	m_MotionLength = MOTION_LENGTH;	//モーションの更新範囲
	m_bPutIn = false;				//乗車中かどうか
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCustomer::Uninit(void)
{
	//シリンダーの破棄
	if (m_pMeshCylinder != NULL) { m_pMeshCylinder = NULL; }

	// 目的地写真の破棄
	if (m_pDestination != NULL) { m_pDestination->Uninit(); m_pDestination = NULL; }

	// 矢印の破棄
	if (m_pArrow != NULL) { m_pArrow->Uninit(); m_pArrow = NULL; }

	//マークの破棄
	if (m_pMark != NULL) { m_pMark = NULL; }

	//親クラスの破棄
	CHumanBace::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CCustomer::Update(void)
{
	//プレイヤーを取得
	CManager::MODE mode = CManager::GetMode();
	CPlayer * pPlayer = NULL;
	CGameCamera * pGameCamera = NULL;
	bool bUpdate = true;

	if (mode == CManager::MODE_GAME)
	{//ゲーム
		pPlayer = CGame::GetPlayer();
		pGameCamera = CGame::GetGameCamera();
	}
	else if (mode == CManager::MODE_TUTORIAL)
	{//チュートリアル
		pPlayer = CTutorial::GetPlayer();
		pGameCamera = CTutorial::GetCamera();
	}

	D3DXVECTOR3 PlayerPos = pPlayer->GetPos();				//プレイヤーの位置を取得
	bool bPlayerPutin = pPlayer->GetPutin();				//乗車状態の取得
	m_fLength = CHumanBace::GetLength(m_pos, PlayerPos);	//距離を求める

	//---------------------
	// 円の表示ON・OFF
	//---------------------
	if (!m_bDrawMeshCylinder)	//シリンダーが描画状態なら
	{
		if (bPlayerPutin == true)
		{//シリンダーの破棄
			UninitMeshCylinder();
		}
		else
		{//シリンダーの生成
			if (m_pMeshCylinder == NULL) { m_pMeshCylinder = CMeshCylinder::Create(m_pos, LAP, STAGE, LENGTH, HIGHT, m_Color); }
		}
		if (m_pMark != NULL)
		{//	マークの位置設定
			m_pMark->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y + OFFSET, m_pos.z));
		}

		//チュートリアル状態だったら
		if (mode == CManager::MODE_TUTORIAL)
		{
			if ((CTutorial::GetCustomerPutin() == false) && (m_pMeshCylinder != NULL))
			{//メッシュシリンダーの破棄
				m_pMeshCylinder->Uninit();
				m_pMeshCylinder = NULL;
			}
			else if ((CTutorial::GetCustomerPutin() == true) && (m_pMeshCylinder == NULL))
			{//シリンダーの生成
				m_pMeshCylinder = CMeshCylinder::Create(m_pos, LAP, STAGE, LENGTH, HIGHT, m_Color);
			}

			if (bPlayerPutin == true)
			{//シリンダーの破棄
				UninitMeshCylinder();
			}
		}
	}

	//描画判定
	if (m_fLength > DRAW_LENGTH)
	{
		SetDraw(false);		//描画しない
		return;
	}
	else
	{
		if (!bPlayerPutin){ SetDraw(true); }
	}

	//NPCの範囲判定
	if ((pPlayer->GetCustomerStop() == true) && (!m_bGotOn))
	{
		float fLength = CHumanBace::GetLength(m_FirstPos, PlayerPos);	//距離を求める

		if ((m_state == STATE_CALLTAXI) || (m_state == STATE_ESCAPE) || (m_state == STATE_RETURN))
		{//乗車状態判定
			if ((fLength < LENGTH) && (bPlayerPutin == false))
			{
				//状態ごとの更新処理
				if ((mode == CManager::MODE_TUTORIAL))
				{
					if (CTutorial::GetCustomerPutin() != true) { bUpdate = false; }
				}

				if (bUpdate)
				{//更新可能状態だったら乗車状態にする
					SetState(STATE_PUTIN);
					pGameCamera->SetModeCamera(CGameCamera::MODE_PUTON);	//お客さんを乗せる状態にする
					pGameCamera->SetCustomerInfo(this);						//お客さんの情報をカメラに渡す
					pPlayer->SetCustomerMove(true);
				}
			}
		}
		else if(m_state == STATE_MOVE)
		{//降車状態判定
			float fLength = CHumanBace::GetLength(m_GoalPos, PlayerPos);	//距離を求める
			if ((fLength < LENGTH)) { SetState(STATE_DOWN); }				//降車状態にする
			pPlayer->SetCustomerMove(false);
		}
	}
	else if ((m_fLength < ESCAPE_LENGTH) && (m_state != STATE_MOVE) && (m_state != STATE_DOWN) && (m_state != STATE_DOWNMOVE) && (m_state != STATE_MISS))
	{
		//サウンドの情報
		CSound *pSound = CManager::GetSound();

		if (m_ModelType == 0)
		{//	男性回避
			pSound->SetVolume(CSound::SOUND_LABEL_MAN_AVOID, 30.0f);
			pSound->PlaySound(CSound::SOUND_LABEL_MAN_AVOID);
		}
		else if (m_ModelType == 1)
		{//	女性回避
			pSound->SetVolume(CSound::SOUND_LABEL_WOMAN_AVOID, 2.0f);
			pSound->PlaySound(CSound::SOUND_LABEL_WOMAN_AVOID);
		}
		else
		{//	子供回避
			pSound->SetVolume(CSound::SOUND_LABEL_CHILD_AVOID, 11.0f);
			pSound->PlaySound(CSound::SOUND_LABEL_CHILD_AVOID);
		}

		if (m_state != STATE_ESCAPE) { SetState(STATE_ESCAPE); }
	}
	else if ((m_fLength < CALLTAXI_LENGTH) && (m_state == STATE_MAX) && (bPlayerPutin == false))
	{//タクシーを呼ぶモーションにする
		SetState(STATE_CALLTAXI);
	}
	else if((m_state == STATE_CALLTAXI) && ((m_fLength > CALLTAXI_LENGTH)))
	{//ニュートラルモーションにする
		if (m_MotionType != MOTION_NEUTRAL) { SetNextMotion(MOTION_NEUTRAL); }
		SetState(STATE_MAX);
	}

	//状態ごとの更新処理
	if ((mode == CManager::MODE_TUTORIAL))
	{
		if (CTutorial::GetCustomerPutin() != true) { bUpdate = false; }
	}

	if(bUpdate)
	{//更新可能状態ならば
		switch (m_state)
		{
		case STATE_PUTIN:	 UpdatePutin(pPlayer, pGameCamera);		break;		// 乗車処理
		case STATE_MOVE:	 UpdateMove(PlayerPos);					break;		// 乗車移動処理
		case STATE_DOWN:	 UpdateDown(pGameCamera);				break;		// 降車処理
		case STATE_MISS:	 UpdateMiss(pPlayer, pGameCamera);		break;		// 送迎失敗処理
		case STATE_DOWNMOVE: UpdateDownMove();						break;		// 降車後の移動処理
		case STATE_ESCAPE:	 UpdateEscape();						break;		// 回避処理
		case STATE_CALLTAXI: UpdateCallTaxi(PlayerPos);				break;		// タクシーを呼ぶ処理
		case STATE_RETURN:	 UpdateReturn();						break;		// 元の位置に戻る処理
		case STATE_END:		 UpdateEnd(pPlayer);					break;		// 終了状態処理
		}
	}

	//親クラスの更新処理
	CHumanBace::Update();

	// 目的地写真の破棄
	if (m_pDestination != NULL && m_pDestination->GetDelet() == true)
	{
		m_pDestination->Uninit();
		m_pDestination = NULL;
	}

	//矢印の位置設定
	if (m_pArrow != NULL) { m_pArrow->SetPosition(ARROW_POS); }
}

//=============================================================================
// 描画処理
//=============================================================================
void CCustomer::Draw(void)
{
	//親クラスの描画処理
	CHumanBace::Draw(1.0f);
}

//=============================================================================
// 設定処理
//=============================================================================
void CCustomer::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	m_pos = pos;		//位置
	m_GoalPos = size;	//ゴールの位置
	m_FirstPos = pos;	//初期位置

	//料金ランクの設定
	m_Color = SetRank();

	//シリンダーの生成
	if (m_pMeshCylinder == NULL)
	{
		m_pMeshCylinder = CMeshCylinder::Create(pos, LAP, STAGE, LENGTH, HIGHT, m_Color);
	}

	//マークの生成
	if (m_pMark == NULL)
	{
		CModel ** pModel = CHumanBace::GetpModel();
		m_pMark = CMark::Create(D3DXVECTOR3(m_pos.x, m_pos.y + OFFSET, m_pos.z));

		//色の設定
		if (m_pMark != NULL)
		{
			m_pMark->SetColor(D3DXCOLOR(m_Color.r, m_Color.g, m_Color.b, 1.0f));
			m_pMark->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y + OFFSET, m_pos.z));
		}
	}
}

//=============================================================================
// 乗車状態
//=============================================================================
void CCustomer::UpdatePutin(CPlayer * pPlayer, CGameCamera * pGameCamera)
{

	//	乗車中に設定
	m_bPutIn = true;

	////プレイヤーを取得
	D3DXVECTOR3 PlayerPos = pPlayer->GetPos();

	//プレイヤーを操作できない状態にする
	pPlayer->SetControl(false);

	//マークを破棄する
	if (m_pMark != NULL)
	{
		m_pMark->Uninit();
		m_pMark = NULL;
	}

	if (m_fLength > PUTIN_LENGTH)
	{//乗車範囲外だったら
		//プレイヤーに近づく
		D3DXVECTOR3 Distance = m_pos - PlayerPos;
		float fAngle = atan2f(Distance.x, Distance.z);		//プレイヤーとの角度を求める
		float fDiffese = fAngle - m_fAngle;					//現在の角度と目的の角度の差を求める

		CHumanBace::RemakeAngle(&fDiffese);	//角度の修正
		m_fAngle += fDiffese * MOVE_ANGLE;	//角度の変化量を求める
		CHumanBace::RemakeAngle(&m_fAngle);

		//敵の変更角度の座標を求める
		m_move.x = sinf(m_fAngle) * -MOVE_SPEED;
		m_move.z = cosf(m_fAngle) * -MOVE_SPEED;

		m_rot.y = m_fAngle;		//向きの更新

		// 目的地写真の生成
		if (m_pDestination == NULL) { m_pDestination = CDestination::Create(m_nGoalTex); }

		//移動モーションにする
		if (m_MotionType != MOTION_WALK) { SetNextMotion(MOTION_WALK); }
	}
	else
	{//乗車範囲内だったら
		m_move = VECTOR_ZERO;
		SetDraw(false);
		m_bClipping = false;				//クリッピングを無効にする
		m_bDrawMeshCylinder = true;
		SetState(STATE_MOVE);				//移動状態にする
		pPlayer->SetControl(true);			//プレイヤーを操作できる状態にする
		pPlayer->SetPutin(true);			//お客さんを乗せた状態にする
		CTime::AddTime(m_nAddTime);			//時間の加算
		CTime::SetCutomerTime(m_nRideTime);	//お客さんの乗車制限時間
		SetShadowColor_A(0.0f);				//影を透明にする

		//カメラを通常状態に戻す
		pGameCamera->SetModeCamera(CGameCamera::MODE_NORMAL);

		//サウンドの情報
		CSound *pSound = CManager::GetSound();

			if (m_ModelType == 0)
			{//	男性感謝
				pSound->SetVolume(CSound::SOUND_LABEL_MAN_THANK, 9.0f);
				pSound->PlaySound(CSound::SOUND_LABEL_MAN_THANK);
			}
			else if (m_ModelType == 1)
			{//	女性感謝
				pSound->SetVolume(CSound::SOUND_LABEL_WOMAN_THANK, 8.0f);
				pSound->PlaySound(CSound::SOUND_LABEL_WOMAN_THANK);
			}
			else
			{//	子供怒る
				pSound->SetVolume(CSound::SOUND_LABEL_CHILD_THANK, 11.0f);
				pSound->PlaySound(CSound::SOUND_LABEL_CHILD_THANK);
			}

		//加算するスコアを求める
		SetFirstScore();
		CScore::SetScore(m_nFirstScore);	//加算するスコア

		//シリンダーの破棄
		UninitMeshCylinder();

		//ゴールシリンダーの生成
		if (m_pMeshCylinder == NULL)
		{
			m_pMeshCylinder = CMeshCylinder::Create(m_GoalPos, LAP, STAGE, LENGTH, GOAL_HIGHT, GOALCOL);
		}
		//矢印の生成
		if (m_pArrow == NULL) { m_pArrow = CArrow::Create(CArrow::TYPE_DESTINATION, ARROW_POS, m_GoalPos, m_pMeshCylinder->GetRadius()); }

	}
	//	乗車中の解除
	m_bPutIn = false;
}

//=============================================================================
// 移動状態の処理
//=============================================================================
void CCustomer::UpdateMove(D3DXVECTOR3 PlayerPos)
{//失敗した状態にする
	if (CTime::GetRideEnd() == true){	SetState(STATE_MISS); }
	m_pos = PlayerPos;
}

//=============================================================================
// 降車状態
//=============================================================================
void CCustomer::UpdateDown(CGameCamera * pGameCamera)
{
	if (!m_bResult)
	{
		//カメラの向きを取得
		float fCameraRot = pGameCamera->GetRot();

		//角度の設定・修正
		float fAngle = D3DX_PI * -0.5f + fCameraRot;
		CHumanBace::RemakeAngle(&fAngle);

		//車の横にお客さんを出現させる
		m_pos.x += sinf(fAngle) * DOWN_POS;
		m_pos.z += cosf(fAngle) * DOWN_POS;

		//角度を設定する
		m_rot.y = fAngle;

		SetDraw(true);				//モデルを描画する
		m_bClipping = true;			//クリッピングを有効にする
		SetShadowColor_A(1.0f);		//影を表示する
		UninitMeshCylinder();		//シリンダーの破棄
		if (m_pArrow != NULL) { m_pArrow->Uninit(); m_pArrow = NULL; }		// 矢印の破棄

		//カメラのモード設定
		pGameCamera->SetModeCamera(CGameCamera::MODE_TAKEDOEN);

		//評価処理
		CustomerResult();

		//サウンドの情報
		CSound *pSound = CManager::GetSound();

		if (m_MotionType == MOTION_SHINOBU)
		{//感謝モーション
			if (m_ModelType == 0)
			{//	男性感謝
				pSound->SetVolume(CSound::SOUND_LABEL_MAN_THANK, 9.0f);
				pSound->PlaySound(CSound::SOUND_LABEL_MAN_THANK);
			}
			else if (m_ModelType == 1)
			{//	女性感謝
				pSound->SetVolume(CSound::SOUND_LABEL_WOMAN_THANK, 8.0f);
				pSound->PlaySound(CSound::SOUND_LABEL_WOMAN_THANK);
			}
			else
			{//	子供怒る
				pSound->SetVolume(CSound::SOUND_LABEL_CHILD_THANK, 11.0f);
				pSound->PlaySound(CSound::SOUND_LABEL_CHILD_THANK);
			}
		}

		if (m_MotionType == MOTION_KICK || m_MotionType == MOTION_ANGER)
		{//怒るモーション
			if (m_ModelType == 0)
			{//	男性怒る
				pSound->SetVolume(CSound::SOUND_LABEL_MAN_ANGER, 9.0f);
				pSound->PlaySound(CSound::SOUND_LABEL_MAN_ANGER);
			}
			else if (m_ModelType != 0 && m_ModelType != 1)
			{//	子供怒る
				pSound->SetVolume(CSound::SOUND_LABEL_CHILD_ANGER, 10.0f);
				pSound->PlaySound(CSound::SOUND_LABEL_CHILD_ANGER);
			}
		}
		return;
	}

	//降りた状態にする
	if (m_MotionType == MOTION_NEUTRAL)
	{
		SetState(STATE_DOWNMOVE);	//状態を設定する

		//歩くモーションにする
		if (m_MotionType != MOTION_WALK) { SetNextMotion(MOTION_WALK); }

		//角度設定
		float fCameraRot = pGameCamera->GetRot();
		float fAngle = D3DX_PI * -0.5f + fCameraRot;
		CHumanBace::RemakeAngle(&fAngle);
		m_rot.y = fAngle + D3DX_PI;
		CHumanBace::RemakeAngle(&m_rot.y);

		//カメラのモード設定
		pGameCamera->SetModeCamera(CGameCamera::MODE_NORMAL);

		//移動させる
		m_move.x += sinf(fAngle) * MOVE_SPEED;
		m_move.z += cosf(fAngle) * MOVE_SPEED;
	}
}

//=============================================================================
// 送迎失敗
//=============================================================================
void CCustomer::UpdateMiss(CPlayer * pPlayer, CGameCamera * pGameCamera)
{
	//カメラの向きを取得
	float fCameraRot = pGameCamera->GetRot();

	//角度の設定・修正
	float fAngle = D3DX_PI * -0.5f + fCameraRot;
	CHumanBace::RemakeAngle(&fAngle);

	//車の横にお客さんを出現させる
	m_pos.x += sinf(fAngle) * DOWN_POS;
	m_pos.z += cosf(fAngle) * DOWN_POS;

	//移動させる
	m_move.x += sinf(fAngle) * MOVE_SPEED;
	m_move.z += cosf(fAngle) * MOVE_SPEED;
	fAngle *= -1;
	RemakeAngle(&fAngle);

	//角度を設定する
	m_rot.y = fAngle;

	//モデルを描画する
	SetDraw(true);
	m_bClipping = true;		//クリッピングを有効にする

	//影を表示する
	SetShadowColor_A(1.0f);

	//シリンダーの破棄
	UninitMeshCylinder();

	// 矢印の破棄
	if (m_pArrow != NULL) { m_pArrow->Uninit(); m_pArrow = NULL; }

	//移動モーションにする
	if (m_MotionType != MOTION_WALK) { SetNextMotion(MOTION_WALK); }

	// 評判の生成
	CReputation::Create(-1);

	pPlayer->SetPutin(false);	//お客さんを乗せられる状態にする
	CScore::Reset();			//スコアの値を破棄する
	SetState(STATE_DOWNMOVE);	//降りた状態にする

}

//=============================================================================
// 降りた状態の更新
//=============================================================================
void CCustomer::UpdateDownMove(void)
{
	m_nCountTime++;
	m_bGotOn = true;	//乗った状態にする

	if (m_nCountTime > COUNT_TIME)
	{
		m_move = VECTOR_ZERO;	//移動量を初期化する
		m_nCountTime = 0;		//カウンターの初期化
		SetState(STATE_END);
	}
}

//=============================================================================
// 逃げる処理
//=============================================================================
void CCustomer::UpdateEscape(void)
{

	//逃げる移動処理
	CHumanBace::MoveEscape();

	//逃げるモーションにする
	if (m_MotionType != MOTION_ESCAPE) { SetNextMotion(MOTION_ESCAPE);}

	//通常の状態に戻す
	if (m_nCountTime > COUNT_ESCAPE)
	{
		m_move = VECTOR_ZERO;	//移動量を初期化する
		m_nCountTime = 0;		//カウンターの初期化

		SetState(STATE_RETURN);	//元の位置に戻る状態にする
	}
}

//=============================================================================
// タクシーを呼ぶ
//=============================================================================
void CCustomer::UpdateCallTaxi(D3DXVECTOR3 PlayerPos)
{
	//タクシーを呼ぶモーションにする
	if (m_MotionType != MOTION_TAXICOLL) { SetNextMotion(MOTION_TAXICOLL); }

	//タクシーの方向に向ける
	D3DXVECTOR3 Distance = m_pos - PlayerPos;
	float fAngle = atan2f(Distance.x, Distance.z);
	RemakeAngle(&fAngle);
	m_rot.y = fAngle;
}

//=============================================================================
// 乗車後の処理
//=============================================================================
void CCustomer::UpdateEnd(CPlayer * pPlayer)
{
	//プレイヤーの位置を取得
	D3DXVECTOR3 PlayerPos = pPlayer->GetPos();

	//ニュートラルモーションにする
	if (m_MotionType != MOTION_NEUTRAL) { SetNextMotion(MOTION_NEUTRAL); }
}

//=============================================================================
// お客さんの位置を戻す処理
//=============================================================================
void CCustomer::UpdateReturn(void)
{
	//プレイヤーに近づく
	D3DXVECTOR3 Distance = m_pos - m_OldPos;
	float fLength = GetLength(m_pos, m_OldPos);			//現在の位置と前回の位置の距離を求める

	if (fLength < 1.0f && fLength > -1.0f)
	{
		SetState(m_OldState);	//前回の状態に戻す
		m_move = VECTOR_ZERO;
	}
	else
	{
		float fAngle = atan2f(Distance.x, Distance.z);		//プレイヤーとの角度を求める
		float fDiffese = fAngle - m_fAngle;					//現在の角度と目的の角度の差を求める

		CHumanBace::RemakeAngle(&fDiffese);	//角度の修正
		m_fAngle += fDiffese * MOVE_ANGLE;	//角度の変化量を求める
		CHumanBace::RemakeAngle(&m_fAngle);

		//敵の変更角度の座標を求める
		m_move.x = sinf(m_fAngle) * -MOVE_SPEED;
		m_move.z = cosf(m_fAngle) * -MOVE_SPEED;

		m_rot.y = m_fAngle;		//向きの更新

		//移動モーションにする
		if (m_MotionType != MOTION_WALK) { SetNextMotion(MOTION_WALK); }
	}
}

//=============================================================================
// 料金ランクを求める
//=============================================================================
D3DXCOLOR CCustomer::SetRank(void)
{
	//変数宣言
	D3DXCOLOR aCol[MAX_COL + 1] = {RED, YELLOW, GREEN, GREEN };					//色の設定
	float faDistance[MAX_COL + 1] = { 0, LOW_RANK, NORMAL_RANK, HIGHT_RANK };	//数字ランクの設定
	int nCntRank = 0;

	//現在の位置とゴールまでの距離を求める
	float fLength = CHumanBace::GetLength(m_pos, m_GoalPos);

	for (nCntRank = 0; nCntRank < MAX_COL; nCntRank++)
	{//ランクの範囲内かどうか
		if ((faDistance[nCntRank] <= fLength) && (fLength < faDistance[nCntRank + 1])) { break; }
	}

	return aCol[nCntRank];
}

//=============================================================================
// 乗車料金を求める
//=============================================================================
void CCustomer::SetFirstScore(void)
{
	//距離を求める
	float fLength = CHumanBace::GetLength(m_pos, m_GoalPos);
	float fMeter = (float)(fLength / METER);
	m_nFirstScore = (int)(fMeter * MONEY_ADD);
}

//=============================================================================
// シリンダーの破棄
//=============================================================================
void CCustomer::UninitMeshCylinder(void)
{
	if (m_pMeshCylinder != NULL)
	{
		m_pMeshCylinder->Uninit();
		m_pMeshCylinder = NULL;
	}
}

//=============================================================================
// お客さんの評価処理
//=============================================================================
void CCustomer::CustomerResult(void)
{
	//変数宣言
	int nCntRank;
	int nTimeRank[MAX_RESULT + 1] = {0, BAD, SLOW, NORMAL};												//時間の設定
	MOTION_TYPE MotionType[MAX_RESULT + 1] = {MOTION_ANGER, MOTION_KICK, MOTION_SHINOBU, MOTION_BOW};	//モーションの設定

	//時間の取得
	int nTime = CTime::GetCutomerTime();

	//どのランクにいるのか
	for (nCntRank = 0; nCntRank < MAX_RESULT; nCntRank++)
	{
		if ((nTimeRank[nCntRank] < nTime) && (nTimeRank[nCntRank + 1] >= nTime))
		{
			break;
		}
	}

	//モーションを設定する
	if (m_ModelType != MotionType[nCntRank]) { SetNextMotion(MotionType[nCntRank]); }

	// 評判の生成
	CReputation::Create(nCntRank);

	CTime::ResetCutomerTime();	//制限時間をリセットする
	CScore::AddTotalScore();	//トータルスコア加算
	m_bResult = true;

	CManager::MODE mode = CManager::GetMode();		//モードの取得
	CPlayer * pPlayer = NULL;

	if (mode == CManager::MODE_GAME)
	{//ゲーム
		pPlayer = CGame::GetPlayer();
	}
	else if (mode == CManager::MODE_TUTORIAL)
	{//チュートリアル
		pPlayer = CTutorial::GetPlayer();
	}

	pPlayer->SetPutin(false);	//お客さんを乗せられる状態にする

}

//=============================================================================
// 状態の設定
//=============================================================================
void CCustomer::SetState(STATE state)
{
	m_OldState = m_state;	//前回の状態を保存する
	m_state = state;		//状態を設定する

	if (m_OldState == STATE_ESCAPE) 
	{
		m_OldState = STATE_END;
	}

	if (m_state == STATE_ESCAPE)
	{
		m_OldPos = m_pos;
	}
}