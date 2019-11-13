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
#define MOVE_CAMERA		(250.0f)							//カメラの移動量
#define MOVE_ANGLE		(0.06f)								//カメラ角度の移動量
#define HIGHT_V			(30.0f)							//視点の高さ
#define HIGHT_R			(25.0f)								//注視点の高さ

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
	m_fDistance = MOVE_CAMERA;
	m_fBackTime = 0.0f;
	m_fCntTime = 0.0f;
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
			m_posR = D3DXVECTOR3(-11000.0f, -1800.0f, -200.0f);
			m_posV = D3DXVECTOR3(-9000.0f, -1200.0f, 700.0f);
		}
		else
		{
			m_posR = D3DXVECTOR3(-11000.0f - (2.0f * nCount), -1800.0f + (2.0f * nCount), -200.0f + (1.0f * nCount));
			m_posV = D3DXVECTOR3(-9000.0f, -1200.0f + (4.0f * nCount), 700.0f);
		}
	}
	else if (nGameCounter < CUORSE_VIEW_TIPE_1)
	{// 2回目
		nCount = nGameCounter - CUORSE_VIEW_TIPE_0;

		if (nCount == 0)
		{// 初期値
			m_posR = D3DXVECTOR3(-2000, 0.0f, -4200.0f);
			m_posV = D3DXVECTOR3(-350, 800.0f, -3500.0f);
		}
		else
		{
			m_posR = D3DXVECTOR3(-2000 + (-3.0f * nCount), 0.0f, -4200.0f);
			m_posV = D3DXVECTOR3(-350, 800.0f, -3500.0f);
		}
	}
	else
	{// 3回目
		nCount = nGameCounter - CUORSE_VIEW_TIPE_1;

		if (nCount == 0)
		{// 初期値
			m_posR = D3DXVECTOR3(-450.0f, -30.0f, 0.0f);
			m_posV = D3DXVECTOR3(100.0f, 80.0f, -1.0f);
		}
		else
		{
			m_posV = D3DXVECTOR3(100.0f + (3.0f * nCount), 80.0f, -1.0f);
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
	if (m_fBackTime > 0.0f) { m_fBackTime--; }

	if (m_pPlayer != NULL)
	{
		D3DXVECTOR3 PlayerPos = m_pPlayer->GetPos();
		D3DXVECTOR3 PlayerRot = m_pPlayer->GetRot();
		D3DXVECTOR3 PlayerMove = m_pPlayer->GetMove();
		D3DXVECTOR3 WKPosR;

		//注視点の更新
		WKPosR = D3DXVECTOR3(0.0f, HIGHT_R, 0.0f) + PlayerPos;	//見る場所 + 水平移動分

		//プレイヤーに追従ようにする
		m_fRotDest = PlayerRot.y - m_rot.y;
		RemakeAngle(&m_fRotDest);

		m_rot.y += m_fRotDest * MOVE_ANGLE;
		RemakeAngle(&m_rot.y);

		float fTilt = m_pPlayer->GetfTilt() * 0.25f - 0.075f;
		float fWK = m_pPlayer->GetfTilt();
		float fMove = MOVE_CAMERA * (fWK * 1.0f + 1.0f);
		fWK += 1.0f;
		if (fTilt < -0.15f) { fTilt = -0.15f; m_fBackTime = 300.0f; }
		if (fTilt > 0.1f) { fTilt = 0.1f; }
		if (fTilt > 0.0f) { m_fBackTime = 0.0f; }

		float fRot = (D3DX_PI * fTilt);

		if (m_fBackTime > 0) { fMove *= 2.0f; }
		m_rot.x += (fRot - m_rot.x) * 0.03f;

		if (fMove - m_fDistance < 0.0f && m_fBackTime == 0)
		{//カメラが寄るなら早く
			m_fDistance += (fMove - m_fDistance) * 0.1f;
		}
		else
		{//引くのはゆっくり
			m_fDistance += (fMove - m_fDistance) * 0.025f;
		}
		//視点更新
		m_posV = D3DXVECTOR3(
			(sinf(m_rot.y) * -m_fDistance) * cosf(m_rot.x),	//X軸
			sinf(m_rot.x) * -m_fDistance + HIGHT_V,		//Y軸
			(cosf(m_rot.y) * -m_fDistance) * cosf(m_rot.x)) + PlayerPos;	//Z軸
		WKPosR += D3DXVECTOR3(sinf(m_rot.y), 0.0f, cosf(m_rot.y)) * (5.0f * ((-fWK + 2.0f) * 0.5f) + 30.0f);
		WKPosR.y += (-5.0f * ((fWK - 1.0f) * 1.0f) + 15.0f);
		m_posR += (WKPosR - m_posR) * 0.2f;

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