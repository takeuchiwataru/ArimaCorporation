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
#include "fade.h"

//===============================================================================
//　マクロ定義
//===============================================================================
#define MOVE_CAMERA		(500.0f)							//カメラの移動量
#define MOVE_ANGLE		(0.06f)								//カメラ角度の移動量
#define HIGHT_V			(130.0f)							//視点の高さ
#define HIGHT_R			(60.0f)								//注視点の高さ

//===============================================================================
//　コンストラクタ
//===============================================================================
CGameCamera::CGameCamera() {}

//===============================================================================
//　デストラクタ
//===============================================================================
CGameCamera::~CGameCamera() {}

//===============================================================================
//　初期化処理
//===============================================================================
HRESULT CGameCamera::Init(void)
{
	CCamera::Init();

	//変数の初期化
	m_posV = D3DXVECTOR3(0.0f, 500.0f, 0.0f);	//視点の初期値
	m_posR = VECTOR_ZERO;						//注視点の初期値

	m_cameraType = CAMERA_NONE;
	m_pPlayer = NULL;
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
	switch (m_cameraType)
	{
	case CAMERA_COURSE:
		UpdateCourse(); //コース状態
		break;
	case CAMERA_PLAYER:
		UpdatePlayer(); //プレイヤー状態
		break;
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
// ゲームカメラ（コース）更新処理
//=============================================================================
void CGameCamera::UpdateCourse(void)
{
	int nGameCounter = CGame::GetGameCounter();
	int nCount = nGameCounter;

	if (nGameCounter < CUORSE_VIEW_TIPE_0)
	{// 1回目
		nCount = nGameCounter;

		if (nCount == 0)
		{// 初期値
			m_posR = D3DXVECTOR3(0.0f, 500.0f, 500.0f);
			m_posV = D3DXVECTOR3(0.0f, 1000.0f, -1200.0f);
		}
		else
		{
			m_posR = D3DXVECTOR3(0.0f, 500.0f, 500.0f + (5.0f * nCount));
		}
	}
	else if (nGameCounter < CUORSE_VIEW_TIPE_1)
	{// 2回目
		nCount = nGameCounter - CUORSE_VIEW_TIPE_0;

		if (nCount == 0)
		{// 初期値
			m_posR = D3DXVECTOR3(0.0f + 1000.0f, 300.0f, 100.0f);
			m_posV = D3DXVECTOR3(0.0f + 1000.0f + 1500.0f, 550.0f, -1.0f);
		}
		else
		{
			m_posR = D3DXVECTOR3(0.0f + 1000.0f, 300.0f, 100.0f + (4.0f * nCount));
			m_posV = D3DXVECTOR3(0.0f + 1000.0f + 1500.0f, 550.0f, -1.0f + (2.0f * nCount));
		}
	}
	else
	{// 3回目
		nCount = nGameCounter - CUORSE_VIEW_TIPE_1;

		if (nCount == 0)
		{// 初期値
			m_posR = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
			m_posV = D3DXVECTOR3(0.0f, 200.0f, -800.0f);
		}
		else
		{
			m_posV = D3DXVECTOR3(0.0f, 200.0f, -800.0f + (-2.0f * nCount));
		}
	}

	if (nGameCounter == CUORSE_VIEW_TIME)
		CFade::Create(CGame::GAMEMODE_PLAY);
}

//=============================================================================
// ゲームカメラ（プレイヤー）更新処理
//=============================================================================
void CGameCamera::UpdatePlayer(void)
{
	//プレイヤーの情報を取得する
	CManager::MODE mode = CManager::GetMode();

	if (m_pPlayer != NULL)
	{
		D3DXVECTOR3 PlayerPos = m_pPlayer->GetPos();
		D3DXVECTOR3 PlayerRot = m_pPlayer->GetRot();
		D3DXVECTOR3 PlayerMove = m_pPlayer->GetMove();

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