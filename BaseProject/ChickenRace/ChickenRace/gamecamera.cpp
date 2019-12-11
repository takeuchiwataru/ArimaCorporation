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
#define MOVE_DISANGLE	(10.0f)							//カメラの画角移動量
#define MOVE_CAMERA		(90.0f)							//カメラの移動量
#define VECU_CAMERA		(30.0f)							//カメラの移動量
#define MOVE_ANGLE		(0.06f)								//カメラ角度の移動量
#define HIGHT_V			(40.0f)							//視点の高さ
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
	m_fCameraAngle = 65;
	m_cameraType = CAMERA_NONE;
	m_pPlayer = NULL;
	m_fDistance = MOVE_CAMERA;
	m_fPlusDis = 0.0f;
	m_fGAngle = 65.0f;
	
	m_nTopPlayer = 0;
	
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
	case CAMERA_CHARSELECT:
		UpdateCharSelect();	//キャラ選択状態
		break;
	case CAMERA_CHARUP:
		UpdateCharUp();		//キャラアップ状態
		break;
	case CAMERA_COURSE:
		UpdateCourse();		//コース状態
		break;
	case CAMERA_GOUL:
		UpdateGoul();		//ゴール状態
		break; 
	case CAMERA_PLAYER:
		UpdatePlayer();		//プレイヤー状態
		break;
	}

	m_fRot = atan2f(m_posR.x - m_posV.x, m_posR.z - m_posV.z);
}

//===============================================================================
//　カメラの設定
//===============================================================================
void CGameCamera::SetCamera(void)
{
	CCamera::SetCamera();
}

//=============================================================================
// リセット
//=============================================================================
void CGameCamera::SetTypeReset(void)
{
	switch (m_cameraType)
	{
	case CAMERA_GOUL:
		UpdateGoul(true);	//ゴール状態
		break;
	case CAMERA_PLAYER:
		UpdatePlayer(true);		//プレイヤー状態
		break;
	}
}

//=============================================================================
// ゲームカメラ（キャラ選択）更新処理
//=============================================================================
void CGameCamera::UpdateCharSelect(void)
{
	m_fCameraAngle = 90;
	m_posR = D3DXVECTOR3(3000.0f, -90.0f, 140.0f);
	m_posV = D3DXVECTOR3(2600.0f, -10.0f, 141.0f);
}

//=============================================================================
// ゲームカメラ（キャラアップ）更新処理
//=============================================================================
void CGameCamera::UpdateCharUp(void)
{
	m_fCameraAngle = 90;
	if (m_pPlayer != NULL)
	{// NULL以外
		int nCounter = CGame::GetGameCounter();
		float frot = ((float)nCounter * 0.008f);

		D3DXVECTOR3 pos = m_pPlayer->Getpos();
		m_posR = pos + D3DXVECTOR3(0.0f, 30.0f, 0.0f);
		m_posV = m_posR + D3DXVECTOR3(sinf(-(D3DX_PI * 0.5f) + frot) * 30.0f, 0.0f, cosf(-(D3DX_PI * 0.5f) + frot) * 30.0f);
	}
}

//=============================================================================
// ゲームカメラ（コース）更新処理
//=============================================================================
void CGameCamera::UpdateCourse(void)
{
	int nGameCounter = CGame::GetGameCounter();
	int nCount = nGameCounter;

	m_fCameraAngle = 90;
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
// ゲームカメラ（ゴール）更新処理
//=============================================================================
void CGameCamera::UpdateGoul(bool bSet)
{
	//m_posR = D3DXVECTOR3(-22590.0f, -3285.0f, -136.0f);
	//m_posV = D3DXVECTOR3(-22358.0f, -3285.0f, -121.0f);
	m_vecU.z = 0.0f;

	float fAngle = atan2f(m_posV.x - m_posR.x, m_posV.z - m_posR.z);
	/*m_posR = m_posR + D3DXVECTOR3(
	sinf(fAngle - (D3DX_PI * 0.52f)) * 250.0f, 0.0f,
	cosf(fAngle - (D3DX_PI * 0.52f)) * 250.0f);
	m_posR = m_posR + D3DXVECTOR3(
	sinf(fAngle - (D3DX_PI * 0.52f) - (D3DX_PI * 0.5f)) * 400.0f, 50.0f,
	cosf(fAngle - (D3DX_PI * 0.52f) - (D3DX_PI * 0.5f)) * 400.0f);
	m_posV = m_posR + D3DXVECTOR3(
	sinf(fAngle - (D3DX_PI * 0.52f)) * 1160.0f, 100.0f,
	cosf(fAngle - (D3DX_PI * 0.52f)) * 1160.0f);*/

	/*m_posR = m_posR + D3DXVECTOR3(
	sinf(fAngle - (D3DX_PI * 0.54f)) * 250.0f, 0.0f,
	cosf(fAngle - (D3DX_PI * 0.54f)) * 250.0f);
	m_posR = m_posR + D3DXVECTOR3(
	sinf(fAngle - (D3DX_PI * 0.54f) - (D3DX_PI * 0.5f)) * 400.0f, 50.0f,
	cosf(fAngle - (D3DX_PI * 0.54f) - (D3DX_PI * 0.5f)) * 400.0f);
	m_posV = m_posR + D3DXVECTOR3(
	sinf(fAngle - (D3DX_PI * 0.54f)) * 1080.0f, -10.0f,
	cosf(fAngle - (D3DX_PI * 0.54f)) * 1080.0f);*/

	/*m_posR = m_posR + D3DXVECTOR3(
	sinf(fAngle - (-D3DX_PI * 0.26f)) * -1200.0f, 0.0f,
	cosf(fAngle - (-D3DX_PI * 0.26f)) * -1200.0f);
	m_posR = m_posR + D3DXVECTOR3(
	sinf(fAngle - (-D3DX_PI * 0.26f) - (D3DX_PI * 0.5f)) * 300.0f, 0.0f,
	cosf(fAngle - (-D3DX_PI * 0.26f) - (D3DX_PI * 0.5f)) * 300.0f);
	m_posV = m_posR + D3DXVECTOR3(
	sinf(fAngle - (-D3DX_PI * 0.26f)) * 700.0f, 50.0f,
	cosf(fAngle - (-D3DX_PI * 0.26f)) * 700.0f);*/

	// プレイヤー
	CPlayer **pPlayer = NULL;
	switch (CManager::GetMode())
	{
	case CManager::MODE_TITLE:
		pPlayer = CTitle::GetPlayer();
		break;
	case CManager::MODE_GAME:
		pPlayer = CGame::GetPlayer();
		break;
	}	// ランキング
	int *pnRanking = CGame::GetRanking();

	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float frot = 0.0f;
	int nNum = 0;

	// ゴール数
	for (int nCount = 0; nCount < MAX_MEMBER; nCount++)
	{// メンバーカウント
		if (pPlayer[nCount] != NULL)
			if (pPlayer[nCount]->GetGoal() == true)
				nNum++;
	}

	// １位
	for (int nCount = 0; nCount < MAX_MEMBER; nCount++)
	{// メンバーカウント
		if (pnRanking[nCount] == nNum && pPlayer[nCount] != NULL)
		{// レース中でトップ
			if (pPlayer[nCount]->GetGoal() == false)
			{// ゴールしていない
				pos = pPlayer[nCount]->GetPos();
				frot = pPlayer[nCount]->Getrot().y;
				break;
			}
		}
	}

	// ゴール数が変わった
	if (m_nTopPlayer != nNum || bSet == true)
	{
		m_posR = pos;
		m_nTopPlayer = nNum;
	}

	// 位置設定
	m_posR += (pos - m_posR) * 0.08f;
	//m_posR = m_posR + D3DXVECTOR3(sinf(frot + D3DX_PI) * 5.0f, 0.0f, cosf(frot + D3DX_PI) * 5.0f);
	m_posV = m_posR + D3DXVECTOR3(sinf(frot) * 250.0f, 300.0f, cosf(frot) * 250.0f);
}

//=============================================================================
// ゲームカメラ（プレイヤー）更新処理
//=============================================================================
void CGameCamera::UpdatePlayer(bool bSet)
{
	//プレイヤーの情報を取得する
	CManager::MODE mode = CManager::GetMode();
	m_fPlusDis *= 0.95f;

	if (m_pPlayer != NULL)
	{
		D3DXVECTOR3 PlayerPos = m_pPlayer->GetPos();
		D3DXVECTOR3 PlayerRot = m_pPlayer->GetRot();
		D3DXVECTOR3 PlayerMove = m_pPlayer->GetMove();
		D3DXVECTOR3 VecUPos;
		D3DXVECTOR3 WKPosR;
		float		fDistance = 0.0f;
		float		fRotX = 0.0f;
		float		fRotY = m_pPlayer->GetRot().y;
		float		fCntTime = m_pPlayer->GetfCntTime();
		float		fSet = 0.2f;
		m_vecU.z = m_pPlayer->Getrot().z * -1.5f;
		fRotY += D3DX_PI * 0.5f;

		//画角変更
		if (fCntTime < 0.0f) { fCntTime = 0.0f; }
		m_fGAngle = powf(fCntTime, 2) * 0.4f + 65.0f + m_fPlusDis * 40.0f;
		if (m_fGAngle > 120.0f) { m_fGAngle = 120.0f; }

		if (m_fCameraAngle < m_fGAngle) { m_fCameraAngle += (m_fGAngle - m_fCameraAngle) * 0.25f; }
		else { m_fCameraAngle += (m_fGAngle - m_fCameraAngle) * 0.01f; }

		//プレイヤーに追従ようにする
		m_fRotDest = PlayerRot.y - m_rot.y;
		RemakeAngle(&m_fRotDest);

		m_rot.y += m_fRotDest * MOVE_ANGLE;

		if (bSet == true)
		{
			m_rot.y = PlayerRot.y;
			fSet = 1.0f;
		}

		RemakeAngle(&m_rot.y);

		float fTilt = m_pPlayer->GetfTiltV() * 3.0f - 0.1f;
		float fWK = -m_pPlayer->GetfTiltV();
		float fMove = MOVE_CAMERA * (fWK * 1.5f + 1.0f);
		if (m_pPlayer->GetbJump()) { fMove += MOVE_CAMERA * 0.5f; }
		fWK += 1.0f;

		//CRoad_Pointer::BentRotX(m_pPlayer->Getpos(), m_pPlayer->GetpEnmPoint(), fRotX, fDistance);
		if (fTilt > D3DX_PI * 0.75f)	{ fTilt = D3DX_PI * 0.75f; }
		if (fTilt < D3DX_PI * -0.75f)	{ fTilt = D3DX_PI * -0.75f; }

		fRotX = fTilt;
		if (m_pPlayer->GetbJump()) { fRotX -= D3DX_PI * 0.15f; }
		RemakeAngle(&fRotX);

		//視点更新
		fMove -= m_vecU.z * m_vecU.z * MOVE_CAMERA * 0.75f;
		if (fMove < MOVE_CAMERA * 0.25f) { fMove = MOVE_CAMERA * 0.25f; }
		VecUPos = D3DXVECTOR3(sinf(fRotY), 0.0f, cosf(fRotY)) * (m_vecU.z * VECU_CAMERA);

		fMove *= (1.0f + m_fPlusDis * 0.75f);
		fMove += MOVE_DISANGLE * (8.5f - (m_fCameraAngle - 65.0f) * 0.2f);
		m_rot.x += (fRotX - m_rot.x) * 0.05f;

		if (bSet == true)
		{
			m_rot.x = fRotX;
		}

		m_fDistance += (fMove - m_fDistance) * 0.05f;

		PlayerPos += VecUPos;
		m_posV = D3DXVECTOR3(
			(sinf(m_rot.y) * -m_fDistance) * cosf(m_rot.x),	//X軸
			sinf(m_rot.x) * -m_fDistance + HIGHT_V,		//Y軸
			(cosf(m_rot.y) * -m_fDistance) * cosf(m_rot.x)) + PlayerPos;	//Z軸
																			//注視点の更新
		WKPosR = D3DXVECTOR3(0.0f, HIGHT_R, 0.0f) + PlayerPos;	//見る場所 + 水平移動分
		WKPosR += D3DXVECTOR3(sinf(m_rot.y), 0.0f, cosf(m_rot.y)) * (5.0f * ((-fWK + 2.0f) * 0.5f) + 30.0f);
		WKPosR.y += (-5.0f * ((fWK - 1.0f) * 1.0f) + 15.0f);
		m_posR += (WKPosR - m_posR) * fSet;
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
				pObject->SetDraw(Clipping(pObject->GetposR(), CModel3D::GetVtxMin(pObject->GetModelType()), CModel3D::GetVtxMax(pObject->GetModelType())));
			}
		}

		//Nextに次のSceneを入れる
		pScene = pSceneNext;
	}
}