//=============================================================================
//
// ゲームカメラ処理 [gamecamera.cpp]
// Author : 佐藤安純
//
//=============================================================================
#include "gamecamera.h"
#include "game.h"
#include "player.h"
#include "scene.h"
#include "object.h"
#include "manager.h"
#include "tutorial.h"
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

	//変数の初期化
	m_posV = D3DXVECTOR3(0.0f, 500.0f, 0.0f);	//視点の初期値
	m_posR = VECTOR_ZERO;						//注視点の初期値
	m_DestPosV = VECTOR_ZERO;					//目的の位置
	m_Move = VECTOR_ZERO;						//移動量
	m_fRotDest = 0;
	m_bSet = true;								//カメラの設定
	m_bSetAngle = false;						//角度を設定したかどうか
	m_fOldRotY = 0.0f;							//前回の向き
	m_fAngle = 0.0f;							//角度
	m_fHoldAngle = 0.0f;						//保管する角度
	m_fAddDirecting = 0.0f;
	return S_OK;
}

//===============================================================================
//　終了処理
//===============================================================================
void CGameCamera::Uninit(void)
{
	CCamera::Uninit();
}

//===============================================================================
//　更新処理
//===============================================================================
void CGameCamera::Update(void)
{
	UpdateNormal(); //通常状態
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
// 描画するものを再設定する
//=============================================================================
void CGameCamera::DrawReset(void)
{
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