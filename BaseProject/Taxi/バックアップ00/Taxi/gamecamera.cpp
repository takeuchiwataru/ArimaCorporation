//=============================================================================
//
// ゲームカメラ処理 [gamecamera.cpp]
// Author : 佐藤安純
//
//=============================================================================
#include "gamecamera.h"
#include "game.h"
#include "player.h"
#include "customer.h"
#include "scene.h"
#include "object.h"
#include "manager.h"
#include "tutorial.h"
#include "startsignal.h"
//===============================================================================
//　マクロ定義
//===============================================================================
#define MOVE_CAMERA		(290.0f)							//カメラの移動量
#define MOVE_ANGLE		(0.06f)								//カメラ角度の移動量
#define WINDY			(0.4f)								//風圧
#define HIGHT_V			(100.0f)							//視点の高さ
#define HIGHT_R			(60.0f)								//注視点の高さ
#define PUTON_DISTANCE	(D3DXVECTOR3(150.0f, 0.0f, 150.0f))	//乗車状態のカメラ距離
#define ADD_ANGLE		(0.04f)								//角度の加算
#define POSV_DISTANCE	(200.0f)							//視点の距離
#define POSV_MOVE		(0.1f)								//移動量

//===============================================================================
//　コンストラクタ
//===============================================================================
CGameCamera::CGameCamera(){}

//===============================================================================
//　デストラクタ
//===============================================================================
CGameCamera::~CGameCamera(){}

//===============================================================================
//　初期化処理
//===============================================================================
HRESULT CGameCamera::Init(void)
{
	CCamera::Init();

	if (CManager::GetMode() == CManager::MODE_TUTORIAL) { m_mode = MODE_NORMAL; }
	else if(CManager::GetMode() == CManager::MODE_GAME) { m_mode = MODE_NORMAL; }

	//変数の初期化
	m_posV = D3DXVECTOR3(0.0f, 500.0f, 0.0f);	//視点の初期値
	m_posR = VECTOR_ZERO;						//注視点の初期値
	m_DestPosV = VECTOR_ZERO;					//目的の位置
	m_Move = VECTOR_ZERO;						//移動量
	m_fRotDest = 0;
	m_pCustomer = NULL;							//お客さんのポインタ
	m_pStartSignal = NULL;
	m_bSet = true;								//カメラの設定
	m_bSetAngle = false;						//角度を設定したかどうか
	m_fOldRotY = 0.0f;							//前回の向き
	m_fAngle = 0.0f;							//角度
	m_fHoldAngle = 0.0f;						//保管する角度
	m_fAddDirecting = 0.0f;
	m_Directing = DIRECTING_000;				// 最初のカメラワークに
	return S_OK;
}

//===============================================================================
//　終了処理
//===============================================================================
void CGameCamera::Uninit(void)
{
	//お客さんのポインタ破棄
	m_pCustomer = NULL;
	
	// スタート表示の破棄
	if (m_pStartSignal != NULL)
	{
		m_pStartSignal->Uninit();
		delete m_pStartSignal;
		m_pStartSignal = NULL;
	}
	
	CCamera::Uninit();
}

//===============================================================================
//　更新処理
//===============================================================================
void CGameCamera::Update(void)
{
	//状態ごとの更新処理
	switch (m_mode)
	{
	case MODE_NORMAL:   UpdateNormal();   break;	//通常状態
	case MODE_PUTON:    UpdatePutOn();    break;	//お客さんを乗せる状態
	case MODE_TAKEDOEN: UpdateTakeDown(); break;	//お客さんを下すとき
	case MODE_START:    UpdateStart();	  break;	//ゲームスタート状態
	}
}

//===============================================================================
//　カメラの設定
//===============================================================================
void CGameCamera::SetCamera(void)
{
	CCamera::SetCamera();
}

//=============================================================================
// ゲームカメラ通常更新処理
//=============================================================================
void CGameCamera::UpdateNormal(void)
{
	//プレイヤーの情報を取得する
	CPlayer * pPlayer = NULL;
	CManager::MODE mode = CManager::GetMode();

	if (mode == CManager::MODE_GAME)
	{//ゲーム
		pPlayer = CGame::GetPlayer();
	}
	else if (mode == CManager::MODE_TUTORIAL)
	{//チュートリアル
		pPlayer = CTutorial::GetPlayer();
	}

	D3DXVECTOR3 PlayerPos = pPlayer->GetPos();
	D3DXVECTOR3 PlayerRot = pPlayer->GetRot();
	D3DXVECTOR3 PlayerMove = pPlayer->GetMove();

	//注視点の更新
	m_posR.x = PlayerPos.x;
	m_posR.z = PlayerPos.z;
	m_posR.y = PlayerPos.y + HIGHT_R;

	//プレイヤーに追従ようにする
	m_fRotDest = PlayerRot.y - m_rot.y;
	RemakeAngle(&m_fRotDest);

	m_rot.y += m_fRotDest * MOVE_ANGLE;
	RemakeAngle(&m_rot.y);

	float fRot = m_rot.y - D3DX_PI;
	RemakeAngle(&fRot);

	//視点更新
	m_posV.x = PlayerPos.x - sinf(fRot) * -MOVE_CAMERA;
	m_posV.z = PlayerPos.z - cosf(fRot) * -MOVE_CAMERA;
	m_posV.y = PlayerPos.y + HIGHT_V;
}

//=============================================================================
// ゲームカメラお客さんを乗せた時の更新
//=============================================================================
void CGameCamera::UpdatePutOn(void)
{
	if (m_pCustomer == NULL) { return; }	//NULLチェック

	//お客さんの位置を取得す情報を取得する
	D3DXVECTOR3 CustomerPos = m_pCustomer->GetPos();
	D3DXVECTOR3 CustomerRot = m_pCustomer->GetRot();

	//注視点をお客さんに合わせる
	m_posR.x = CustomerPos.x;
	m_posR.z = CustomerPos.z;

	//m_fHoldAngle = CustomerRot.y + m_rot.y;

	//向きの設定
	m_rot.y += CustomerRot.y * 0.008f;
	RemakeAngle(&m_rot.y);
	
	//プレイヤーの位置を取得
	CPlayer * pPlayer = NULL;
	CManager::MODE mode = CManager::GetMode();

	if (mode == CManager::MODE_GAME)
	{//ゲーム
		pPlayer = CGame::GetPlayer();
	}
	else if (mode == CManager::MODE_TUTORIAL)
	{//チュートリアル
		pPlayer = CTutorial::GetPlayer();
	}

	D3DXVECTOR3 PlayerPos = pPlayer->GetPos();

	//視点の位置を求める
	m_posV.x = PlayerPos.x + (sinf(m_rot.y) * -150.0f);
	m_posV.z = PlayerPos.z + (cosf(m_rot.y) * -150.0f);

	//描画を再設定する
	DrawReset();
}

//=============================================================================
// ゲームカメラお客さんを下すときの更新
//=============================================================================
void CGameCamera::UpdateTakeDown(void)
{
	if (m_pCustomer == NULL) { return; }	//NULLチェック

	//お客さんの位置を取得す情報を取得する
	D3DXVECTOR3 CustomerPos = m_pCustomer->GetPos();
	D3DXVECTOR3 CustomerRot = m_pCustomer->GetRot();

	//注視点をお客さんに合わせる
	m_posR.x = CustomerPos.x;
	m_posR.z = CustomerPos.z;

	//角度を加算する
	m_rot.y += ADD_ANGLE;
	RemakeAngle(&m_rot.y);

	//視点を設定する
	m_posV.x = m_posR.x + sinf(m_rot.y) * -POSV_DISTANCE;
	m_posV.z = m_posR.z + cosf(m_rot.y) * -POSV_DISTANCE;
}

//=============================================================================
// ゲームカメラスタートのときの更新
//=============================================================================
void CGameCamera::UpdateStart(void) 
{
	//プレイヤーの情報を取得する
	CPlayer * pPlayer = CGame::GetPlayer();
	D3DXVECTOR3 PlayerPos = pPlayer->GetPos();
	D3DXVECTOR3 PlayerRot = pPlayer->GetRot();

	//注視点の更新
	m_posR.x = PlayerPos.x;
	m_posR.z = PlayerPos.z;
	m_posR.y = PlayerPos.y;

	switch (m_Directing)
	{
	case DIRECTING_000:		// 最初のカメラワーク
		//角度を加算する
		m_rot.y += 0.01f;
		RemakeAngle(&m_rot.y);

		//視点更新
		m_posV.x = PlayerPos.x - sinf(m_rot.y) * 80.0f;
		m_posV.z = PlayerPos.z + -140.0f;
		m_posV.y = PlayerPos.y + 60.0f;

		if (m_rot.y >= 1.0f) { m_Directing = DIRECTING_001; m_rot.y = 0.0f; }	// 次の演出へ
		StartSkip();
		break;

	case DIRECTING_001:		// 2つ目のカメラワーク
		m_rot.y += 0.01f;
		RemakeAngle(&m_rot.y);
		m_fAddDirecting += 0.3f;
		//視点更新
		m_posV.x = PlayerPos.x + sinf(m_rot.y) * -100.0f;
		m_posV.z = PlayerPos.z + cosf(m_rot.y) * -100.0f;
		m_posV.y = PlayerPos.y + 70.0f + m_fAddDirecting;

		if (m_rot.y >= 1.5f) { m_Directing = DIRECTING_002; m_rot.y = 0.0f; m_fAddDirecting = 0.0f; }	// 次の演出へ

		StartSkip();
		break;

	case DIRECTING_002:	// 3つ目のカメラワーク
		m_rot.y += 0.01f;
		RemakeAngle(&m_rot.y);
		m_fAddDirecting += 0.3f;
		if (m_rot.y >= 1.0f) { m_Directing = DIRECTING_END; m_rot.y = 0.0f; m_fAddDirecting = 0.0f;}	// 次の演出へ

		//視点更新
		m_posV.x = PlayerPos.x - sinf(m_rot.y) * -200.0f - m_fAddDirecting;
		m_posV.z = PlayerPos.z - cosf(m_rot.y) * -200.0f - m_fAddDirecting;
		m_posV.y = PlayerPos.y + 100.0f;

		StartSkip();
		break;

	case DIRECTING_END:	// HERE WA GOを表示
		if (m_pStartSignal == NULL)
		{
			m_pStartSignal = CStartSignal::Create(D3DXVECTOR2(70.0f, 70.0f), CStartSignal::SIGNAL_HERE);
		}
		m_Directing = DIRECTING_NONE;
		break;

	default:		// デフォルトの場合
		UpdateNormal();
		if (m_pStartSignal != NULL)
		{
			if (m_pStartSignal->GetEnd() == true) 
			{ //シグナルの破棄
				m_pStartSignal->Uninit(); 
				delete m_pStartSignal;
				m_pStartSignal = NULL; 
				m_mode = MODE_NORMAL; 
			}
			else { m_pStartSignal->Updata(); }
		}
		break;
	}
}

//=============================================================================
// スタート時のカメラスキップ機能
//=============================================================================
void CGameCamera::StartSkip(void)
{
	if (CManager::GetInput()->GetKeyboardTrigger(DIK_RETURN))
	{	// 演出をスキップ
		m_Directing = DIRECTING_END;
	}
}
//=============================================================================
// モードの設定
//=============================================================================
void CGameCamera::SetModeCamera(MODECAMERA mode)
{
	//向きを元に戻す
	if (mode == MODE_PUTON || mode == MODE_TAKEDOEN) { m_fOldRotY = m_rot.y; }

	m_oldmode = m_mode;		//前回の状態を保存
	m_mode = mode;			//状態設定
	m_bSet = false;	
}

//=============================================================================
// 描画するものを再設定する
//=============================================================================
void CGameCamera::DrawReset(void)
{
	//描画状態にする
	if (!m_pCustomer->GetDraw())
	{
		m_pCustomer->SetDraw(true);
		m_pCustomer->SetClipping(false);
	}

	//オブジェクトの描画を再検索・再設定する
	CScene *pScene;

	//プライオリティーチェック
	pScene = CScene::GetTop(OBJECT_PRIOTITY);

	//NULLチェック
	while (pScene != NULL)
	{
		//UpdateでUninitされてしまう場合　Nextが消える可能性があるからNextにデータを残しておく
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetObjType() == CScene::OBJTYPE_OBJECT)
		{//タイプが障害物だった
			CObject *pObject = (CObject*)pScene;
			
			if (pObject->GetDraw() == false)
			{//もう一度描画判定をする
				pObject->SetDraw(Clipping(pObject->GetVtxMin(), pObject->GetVtxMax()));
			}
		}

		//Nextに次のSceneを入れる
		pScene = pSceneNext;
	}
}

//=============================================================================
// スタート合図をフェード前に削除をする
//=============================================================================
void CGameCamera::UninitSignal(void)
{
	// スタート表示の破棄
	if (m_pStartSignal != NULL)
	{
		m_pStartSignal->Uninit();
		delete m_pStartSignal;
		m_pStartSignal = NULL;
	}
}