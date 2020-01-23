//==============================================================================
//
// タイトルカメラ処理 [titlecamera.cpp]
// Auther : 有馬 武志
//
//==============================================================================
#include "titlecamera.h"
#include "manager.h"
#include "title.h"
#include "game.h"
#include "player.h"
#include "object.h"

//===============================================================================
//　マクロ定義
//===============================================================================
#define MOVE_CAMERA		(90.0f)							//カメラの移動量
#define VECU_CAMERA		(30.0f)							//カメラの移動量
#define MOVE_ANGLE		(0.06f)								//カメラ角度の移動量
#define HIGHT_V			(40.0f)								//視点の高さ
#define HIGHT_R			(25.0f)								//注視点の高さ

//===============================================================================
//　コンストラクタ
//===============================================================================
CTitleCamera::CTitleCamera() {}

//===============================================================================
//　デストラクタ
//===============================================================================
CTitleCamera::~CTitleCamera() {}

//===============================================================================
//　初期化
//===============================================================================
HRESULT CTitleCamera::Init(void)
{
	CCamera::Init();
	m_posR = D3DXVECTOR3(-450.0f, -30.0f, 0.0f);	//視点の初期値
	m_posV = D3DXVECTOR3(270.0f, 80.0f, -1.0f);		//注視点の初期値
	m_fRotDest = 0;
	m_nTypeCounter = 0;

	m_fCameraAngle = 90;
	m_cameraType = CAMERA_NONE;
	m_pPlayer = NULL;
	m_fDistance = MOVE_CAMERA;
	m_fBackTime = 0.0f;
	m_fCntTime = 0.0f;
	m_fPlusDis = 0.0f;

	return S_OK;
}

//===============================================================================
//　終了処理
//===============================================================================
void CTitleCamera::Uninit(void)
{
	CCamera::Uninit();
}

//===============================================================================
//　更新処理
//===============================================================================
void CTitleCamera::Updata(void)
{
	switch (m_cameraType)
	{
	case CAMERA_NORMAL:
		UpdateNormal();	//キャラ選択状態
		break;
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

	m_nTypeCounter++;
}
//===============================================================================
//　カメラの設定
//===============================================================================
void CTitleCamera::SetCamera(void)
{
	CCamera::SetCamera();
}

//=============================================================================
// リセット
//=============================================================================
void CTitleCamera::SetTypeReset(void)
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
	m_vecU.z = 0.0f;
}

//=============================================================================
// タイトルカメラ（ノーマル）更新処理
//=============================================================================
void CTitleCamera::UpdateNormal(void)
{
	m_posR = D3DXVECTOR3(-450.0f, -30.0f, 0.0f);	//視点の初期値
	m_posV = D3DXVECTOR3(270.0f, 80.0f, -1.0f);		//注視点の初期値
}

//=============================================================================
// タイトルカメラ（キャラ選択）更新処理
//=============================================================================
void CTitleCamera::UpdateCharSelect(void)
{
	m_posR = D3DXVECTOR3(3000.0f, -90.0f, 140.0f);
	m_posV = D3DXVECTOR3(2600.0f, -10.0f, 141.0f);
}

//=============================================================================
// タイトルカメラ（キャラアップ）更新処理
//=============================================================================
void CTitleCamera::UpdateCharUp(void)
{
	if (m_pPlayer != NULL)
	{// NULL以外
		float frot = ((float)m_nTypeCounter * 0.008f);

		D3DXVECTOR3 pos = m_pPlayer->Getpos();
		m_posR = pos + D3DXVECTOR3(0.0f, 30.0f, 0.0f);
		m_posV = m_posR + D3DXVECTOR3(sinf(-(D3DX_PI * 0.5f) + frot) * 30.0f, 0.0f, cosf(-(D3DX_PI * 0.5f) + frot) * 30.0f);
	}
}

//=============================================================================
// タイトルカメラ（コース）更新処理
//=============================================================================
void CTitleCamera::UpdateCourse(void)
{
	int nCount = m_nTypeCounter;

	if (m_nTypeCounter < CUORSE_VIEW_TIPE_0)
	{// 1回目
		nCount = m_nTypeCounter;

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
	else if (m_nTypeCounter < CUORSE_VIEW_TIPE_1)
	{// 2回目
		nCount = m_nTypeCounter - CUORSE_VIEW_TIPE_0;

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
	else if (m_nTypeCounter < CUORSE_VIEW_TIME - 60)
	{// 3回目
		nCount = m_nTypeCounter - CUORSE_VIEW_TIPE_1;

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

	//if (m_nTypeCounter == CUORSE_VIEW_TIME - 120)
	//	SetType(CAMERA_NORMAL);
}

//=============================================================================
// タイトルカメラ（ゴール）更新処理
//=============================================================================
void CTitleCamera::UpdateGoul(bool bSet)
{
	m_vecU.z = 0.0f;

	D3DXVECTOR3 pos = m_pPlayer->GetPos();
	float frot = m_pPlayer->Getrot().y;

	m_posR = pos;

	// 位置設定
	m_posR += (pos - m_posR) * 0.08f;
	m_posV = m_posR + D3DXVECTOR3(sinf(frot) * 200.0f, 300.0f, cosf(frot) * 200.0f);
}

//=============================================================================
// タイトルカメラ（プレイヤー）更新処理
//=============================================================================
void CTitleCamera::UpdatePlayer(bool bSet)
{
	//プレイヤーの情報を取得する
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
		float		fSet = 0.2f;
		m_vecU.z = m_pPlayer->Getrot().z * -1.0f;
		fRotY += D3DX_PI * 0.5f;

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

		float fTilt = m_pPlayer->GetfTiltV() * 0.25f - 0.09f;
		float fWK = -m_pPlayer->GetfTiltV();
		float fMove = MOVE_CAMERA * (fWK * 1.5f + 1.0f);
		if (m_pPlayer->GetbJump()) { fMove += MOVE_CAMERA * 0.5f; }
		fWK += 1.0f;

		CRoad_Pointer::BentRotX(m_pPlayer->Getpos(), m_pPlayer->GetpEnmPoint(), fRotX, fDistance);
		if (m_pPlayer->GetbJump()) { fRotX -= D3DX_PI * 0.15f; }
		RemakeAngle(&fRotX);

		//視点更新
		fMove -= m_vecU.z * m_vecU.z * MOVE_CAMERA * 0.75f;
		if (fMove < MOVE_CAMERA * 0.25f) { fMove = MOVE_CAMERA * 0.25f; }
		VecUPos = D3DXVECTOR3(sinf(fRotY), 0.0f, cosf(fRotY)) * (m_vecU.z * VECU_CAMERA);

		fMove *= (1.0f + m_fPlusDis);
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
void CTitleCamera::DrawReset(void)
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
