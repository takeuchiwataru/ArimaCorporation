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