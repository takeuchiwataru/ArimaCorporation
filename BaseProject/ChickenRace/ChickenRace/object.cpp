//=============================================================================
//
// オブジェクトの処理 [object.cpp]
// Author : 有馬武志
//
//=============================================================================
#include "scene.h"
#include "model3D.h"
#include "manager.h"
#include "object.h"
#include "meshfield.h"
#include "player.h"
#include "fade.h"
#include "shadow.h"
#include "tutorial.h"
#include "Character.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_SPEED				(5.0f)
#define OBJCT_ANGLE_REVISION	(0.2f)		// 角度補正
#define EFFECT_HIGHT			(250.0f)	// エミッターの高さ
#define FOUNTAIN_UP				(20.0f)		// 噴水の上昇させる値

//更新範囲
#define FOUNTAIN_LENGTH			(15000)		//噴水の更新範囲
#define LEAF_LENGTH				(10500)		//葉の更新処理
#define WOOD_LENGTH				(8000)		//木をビルボードに入れ替える距離
#define FLOWER_LENGTH			(5500)		//花壇の描画範囲

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
D3DXVECTOR3 CObject::m_LoadVtxMaxModel[MAX_OBJECT] = {};
D3DXVECTOR3 CObject::m_LoadVtxMinModel[MAX_OBJECT] = {};

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CObject::CObject(int nPriority) : CModel3D(nPriority, CScene::OBJTYPE_OBJECT)
{
	m_ModelMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Spin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nStageCount = 0;
	m_bStageNext = false;

	m_fRoll = 0.0f;									// 回転
}
//===============================================================================
//　デストラクタ
//===============================================================================
CObject::~CObject() {}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CObject::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//3DモデルのInit
	CModel3D::Init();

	// 位置の初期化
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//テクスチャ番号の初期化
	m_nTex = -1;

	//位置の代入
	CModel3D::SetPosition(pos);

	// 各種情報の代入
	CModel3D::SetScale(m_scale);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CObject::Uninit(void)
{
	//オブジェクトビルボード
	m_pObjBill = NULL;

	//3DモデルのUninit
	CModel3D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CObject::Update(void)
{
	CModel3D::Update();

	//距離の取得
	D3DXVECTOR3 &m_rot = CModel3D::GetRot();

	if (m_nType == 6)
	{
		m_rot.y -= 0.0004f;
	}
	else if (m_nType == 20)
	{
		if (CManager::GetMode() == CManager::MODE_TITLE)
		{
			// タイトルカウンター
			int nTitleCounter = CTitle::GetCounter() % RESET_TITLEMODE;

			if (RACEMOVE_TITLEMODE - 30 < nTitleCounter && nTitleCounter <= RACEMOVE_TITLEMODE)
			{
				if (CModel3D::GetPosition().z < 0.0f)
				{
					float fAngle = atan2f(0.0f, -70.0f);
					CModel3D::SetPosition(
						D3DXVECTOR3(-350.0f, -113.0f, -71.0f - 70.0f) +
						D3DXVECTOR3(sinf(fAngle - m_fRoll) * -70.0f, 0.0f, cosf(fAngle - m_fRoll) * -70.0f));
					CModel3D::SetRot(D3DXVECTOR3(0.0f, -(D3DX_PI / 2.0f), 0.0f) - D3DXVECTOR3(0.0f, m_fRoll * 1.1f, 0.0f));
				}
				else
				{
					float fAngle = atan2f(0.0f, 70.0f);
					CModel3D::SetPosition(
						D3DXVECTOR3(-350.0f, -113.0f, 49.0f + 70.0f) +
						D3DXVECTOR3(sinf(fAngle + m_fRoll) * -70.0f, 0.0f, cosf(fAngle + m_fRoll) * -70.0f));
					CModel3D::SetRot(D3DXVECTOR3(0.0f, -(D3DX_PI / 2.0f), 0.0f) + D3DXVECTOR3(0.0f, m_fRoll * 1.1f, 0.0f));
				}
				m_fRoll += ((D3DX_PI / 2.0f) / 30.0f);
			}
			else if (nTitleCounter == HOME_TITLEMODE)
			{
				if (CModel3D::GetPosition().z < 0.0f)
				{
					CModel3D::SetPosition(D3DXVECTOR3(-350.0f, -113.0f, -71.0f));
					CModel3D::SetRot(D3DXVECTOR3(0.0f, -(D3DX_PI / 2.0f), 0.0f));
				}
				else
				{
					CModel3D::SetPosition(D3DXVECTOR3(-350.0f, -113.0f, 49.0f));
					CModel3D::SetRot(D3DXVECTOR3(0.0f, -(D3DX_PI / 2.0f), 0.0f));
				}
				m_fRoll = 0.0f;
			}
		}
		else if (CManager::GetMode() == CManager::MODE_GAME)
		{
			if (CGame::GetGameMode() == CGame::GAMEMODE_PLAY)
			{
				// ゲームカウンター
				int nGameCounter = CGame::GetGameCounter();
				int nCounter = nGameCounter - (START_SET_TIME - START_COUNT_TIME);

				if ((START_SET_TIME - START_COUNT_TIME) <= nGameCounter && nGameCounter < START_SET_TIME && nCounter / 60 == 2 && 30 < (nCounter % 60))
				{
					if (CModel3D::GetPosition().z < 0.0f)
					{
						float fAngle = atan2f(0.0f, -70.0f);
						CModel3D::SetPosition(
							D3DXVECTOR3(-350.0f, -113.0f, -71.0f - 70.0f) +
							D3DXVECTOR3(sinf(fAngle - m_fRoll) * -70.0f, 0.0f, cosf(fAngle - m_fRoll) * -70.0f));
						CModel3D::SetRot(D3DXVECTOR3(0.0f, -(D3DX_PI / 2.0f), 0.0f) - D3DXVECTOR3(0.0f, m_fRoll * 1.1f, 0.0f));
					}
					else
					{
						float fAngle = atan2f(0.0f, 70.0f);
						CModel3D::SetPosition(
							D3DXVECTOR3(-350.0f, -113.0f, 49.0f + 70.0f) +
							D3DXVECTOR3(sinf(fAngle + m_fRoll) * -70.0f, 0.0f, cosf(fAngle + m_fRoll) * -70.0f));
						CModel3D::SetRot(D3DXVECTOR3(0.0f, -(D3DX_PI / 2.0f), 0.0f) + D3DXVECTOR3(0.0f, m_fRoll * 1.1f, 0.0f));
					}
					m_fRoll += ((D3DX_PI / 2.0f) / 30.0f);
				}
			}
		}
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void CObject::Draw(void)
{
	//if (m_nType == 2) { return; }

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//位置の初期化
	D3DXVECTOR3 Modelpos = CModel3D::GetPosition();
	//ゲームの情報
	CManager::MODE pMode = CManager::GetMode();

	//頂点法線の自動正規化
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	//描画処理
	CModel3D::Draw();

	//頂点法線の自動正規化
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}
//===============================================================================
//　クリエイト
//===============================================================================
CObject * CObject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, float move, int nTexType, int nObjectType, int nCollision)
{
	CObject *pObject = NULL;
	int nPriority = 3;

	// NULLチェック
	if (pObject == NULL)
	{// メモリの動的確保

		switch (nObjectType)
		{
		case CModel3D::MODEL_TYPE_MAP_FIRST:
		case CModel3D::MODEL_TYPE_MAP_SECOND:
		case CModel3D::MODEL_TYPE_BRIDGE:
		case CModel3D::MODEL_TYPE_MAP_MOUNTAIN:
		case CModel3D::MODEL_TYPE_MAP_SKY:

			nPriority = 0;
			break;
		}
		pObject = new CObject(nPriority);

		if (pObject != NULL)
		{
			// モデルの設定
			pObject->SetModelType(nObjectType);
			// テクスチャの設定
			pObject->SetTextureType(nTexType);
			// オブジェクトごとの設定用タイプ
			pObject->m_nType = nObjectType;
			// サイズを代入
			pObject->m_scale = scale;
			// サイズを親クラスに代入
			pObject->SetScale(scale);
			// オブジェクトクラスの生成
			pObject->Init();
			//テクスチャ番号を代入
			pObject->m_nTex = nTexType;
			// 位置を代入
			pObject->SetPosition(pos);
			// 回転を反映
			pObject->SetRot(rot);
			//pObject->m_move = D3DXVECTOR3(move, move, move);
			// コリジョンをONOFF
			pObject->m_nCollision = nCollision;

			CCharcter::ResetCheck(pos, pObject->GetfCola());
		}
	}

	return pObject;
}
//===============================================================================
// Xファイルの読み込み
//===============================================================================
HRESULT CObject::Load(void)
{
	return S_OK;
}
//===============================================================================
// テクスチャの破棄
//===============================================================================
void CObject::UnLoad(void)
{

}
//=============================================================================
//	ステージ移動時に初期化するため
//=============================================================================
void CObject::DeleteObject(void)
{
	CScene *pScene;

	//プライオリティーチェック
	pScene = CScene::GetTop(OBJECT_PRIOTITY);

	//NULLチェック
	while (pScene != NULL)
	{
		//UpdateでUninitされてしまう場合　Nextが消える可能性があるからNextにデータを残しておく
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetDeath() == false)
		{//タイプが壁だったら
			if (pScene->GetObjType() == OBJTYPE_OBJECT)
			{
				pScene->Uninit();
			}
		}
		//Nextに次のSceneを入れる
		pScene = pSceneNext;
	}
}

//=============================================================================
// 噴水による移動量の変化
//=============================================================================
D3DXVECTOR3 CObject::Fountain(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	D3DXVECTOR3 posObj = CModel3D::GetPosition();	// 位置の取得
	D3DXVECTOR3 vec = pos - posObj;					// 自分から相手方向へのベクトル

													// ベクトルの長さを求める
	float fLengthVec = D3DXVec3Length(&vec);		// 自分から相手方向へのベクトルの長さ
	float fLengthMove = D3DXVec3Length(&move);		// 相手の移動量の長さ

	D3DXVECTOR3 vecMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 角度を求める
	float fAngle = acosf(move.x / sqrt(move.x * move.x + move.z * move.z));
	if (move.z < 0.0f)fAngle = 2.0f * D3DX_PI - fAngle;
	if (isnan(fAngle))return vecMove;

	D3DXVECTOR3 vtxMax = CModel3D::GetVtxMax(GetModelType());		// 頂点座標の最大値の取得

	int nDigit = (int)log10f(vtxMax.x) + 1;
	if (6 < nDigit)return move;

	nDigit = (int)log10f(vtxMax.z) + 1;
	if (6 < nDigit)return move;

	// 移動量の計算
	if (m_nType == 33)
	{// 消火栓
		vecMove = D3DXVECTOR3(0.8f * (move.x + (cosf(fAngle) * vtxMax.x)), FOUNTAIN_UP * 0.4f, 0.8f * (move.z + (sinf(fAngle) * vtxMax.z)));
	}
	else
	{// 噴水
		vecMove = D3DXVECTOR3(0.05f * (move.x + (cosf(fAngle) * vtxMax.x)), FOUNTAIN_UP, 0.05f * (move.z + (sinf(fAngle) * vtxMax.z)));
	}

	return vecMove;
}

//===============================================================================
// 当たり判定
//===============================================================================
bool CObject::CollisionObject(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld, D3DXVECTOR3 * pMove)
{
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();

	//あたっているかあたってないか
	bool bLand = false;

	switch (m_nCollision)
	{
	case 1:

		// 各種情報の取得
		D3DXVECTOR3 ModelPos = CModel3D::GetPosition();		// 位置
		D3DXVECTOR3 ModelMove = CModel3D::GetMove();		// 移動量
		D3DXVECTOR3 &VtxMax = CModel3D::GetVtxMax(GetModelType());			// モデルの最大値
		D3DXVECTOR3 &VtxMin = CModel3D::GetVtxMin(GetModelType());			// モデルの最小値
		D3DXVECTOR3 rot = CModel3D::GetRot();

		D3DXVECTOR3 ModelMax = CModel3D::GetPosition() + CModel3D::GetVtxMax(GetModelType());	// 位置込みの最大値
		D3DXVECTOR3 ModelMin = CModel3D::GetPosition() + CModel3D::GetVtxMin(GetModelType());	// 位置込みの最小値
																								// 移動量の保持
		if (ModelMove.x == 0.0f) ModelMove.x = m_ModelMove.x;
		if (ModelMove.y == 0.0f) ModelMove.y = m_ModelMove.y;
		if (ModelMove.z == 0.0f) ModelMove.z = m_ModelMove.z;

		if (pPos->x >= ModelMin.x - PLAYER_DEPTH && pPos->x <= ModelMax.x + PLAYER_DEPTH)
		{// Zの範囲内にいる
			if (pPos->z >= ModelMin.z - PLAYER_DEPTH && pPos->z <= ModelMax.z + PLAYER_DEPTH)
			{// Xの範囲内にいる
				if (pPosOld->y >= ModelMax.y && pPos->y <= ModelMax.y)
				{// オブジェクトの上から当たる場合
					pPos->y = ModelMax.y + abs(ModelMove.y);
					pMove->y = 0.0f;

					if (ModelMove.x != 0.0f)
					{// X移動同期
						pPos->x += ModelMove.x;
					}

					if (ModelMove.z != 0.0f)
					{// Z移動同期
						pPos->z += ModelMove.z;
					}

					bLand = true;
				}
				else if (pPosOld->y + PLAYER_HEIGHT <= ModelMin.y && pPos->y + PLAYER_HEIGHT >= ModelMin.y)
				{// オブジェクトの下から当たる場合
					pPos->y = ModelMin.y - PLAYER_HEIGHT;
					pMove->y = 0.0f;
				}

				if (!(pPos->y >= ModelMax.y) && !(pPos->y + PLAYER_HEIGHT <= ModelMin.y))
				{// オブジェクト横との当たり判定
					if (pPosOld->x - PLAYER_DEPTH >= ModelMax.x + rot.x && pPos->x - PLAYER_DEPTH < ModelMax.x + rot.x)
					{// 右から抜けた場合
						pPos->x = ModelMax.x + PLAYER_DEPTH;
					}
					else if (pPosOld->x + PLAYER_DEPTH <= ModelMin.x && pPos->x + PLAYER_DEPTH > ModelMin.x)
					{// 左から抜けた場合
						pPos->x = ModelMin.x - PLAYER_DEPTH;
					}

					if (pPosOld->z - PLAYER_DEPTH >= ModelMax.z && pPos->z - PLAYER_DEPTH < ModelMax.z)
					{// 奥から抜けた場合
						pPos->z = ModelMax.z + PLAYER_DEPTH + 0.1f;
					}
					else if (pPosOld->z + PLAYER_DEPTH <= ModelMin.z && pPos->z + PLAYER_DEPTH > ModelMin.z)
					{// 手前から抜けた場合
						pPos->z = ModelMin.z - PLAYER_DEPTH - 0.1f;
					}
				}
			}
		}

		// 位置の代入
		CModel3D::SetPosition(ModelPos);
		// 移動量の代入
		m_ModelMove = ModelMove;

		break;
	}

	return bLand;
}
