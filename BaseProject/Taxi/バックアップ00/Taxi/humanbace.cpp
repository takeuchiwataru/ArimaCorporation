//=============================================================================
//
// お客さんの処理 [customer.cpp]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "humanbace.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "loadText.h"
#include "meshfield.h"
#include "shadow.h"
#include "gamecamera.h"
#include "tutorial.h"
#include "shadow.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define FILE_TEXTURE00	("data\\TEXTURE\\modeltex\\human_boy.jpg")	//読み込むテクスチャ
#define FILE_TEXTURE01	("data\\TEXTURE\\modeltex\\human_girl.jpg")	//読み込むテクスチャ
#define ESCAPE_SPEED	(5.0f)										//逃げるスピード
#define GRAVITY			(0.3f)										//重力
#define SCALE			(D3DXVECTOR3(1.4f, 1.4f, 1.4f))				//モデルのスケール
#define SHADOW_SIZE		(30.0f)										//影のサイズ

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CHumanBace::m_pTexture[CHumanBace::MODEL_MAX] = {};
int CHumanBace::m_nMaxMotion[CHumanBace::MODEL_MAX] = {};
CMotion::MOTION_INFO * CHumanBace::m_pMotionInfo[CHumanBace::MODEL_MAX] = {};
CLoadTextMotion::MODEL_INFO CHumanBace::m_ModelInfo[CHumanBace::MODEL_MAX];

//=============================================================================
// コンストラクタ
//=============================================================================
CHumanBace::CHumanBace(int nPriority, CScene::OBJTYPE type) : CScene(nPriority, type){}

//=============================================================================
// デストラクタ
//=============================================================================
CHumanBace::~CHumanBace(){}

//=============================================================================
// ファイルの読み込み
//=============================================================================
void CHumanBace::LoadModel(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, FILE_TEXTURE00, &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, FILE_TEXTURE01, &m_pTexture[1]);

	//モーション情報の取得
	CManager::MODE mode = CManager::GetMode();
	CLoadTextMotion * pLoadTextBoy = NULL;
	CLoadTextMotion * pLoadTextGirl = NULL;

	if (mode == CManager::MODE_GAME)
	{//ゲーム
		pLoadTextBoy = CGame::GetBoyMotion();
		pLoadTextGirl = CGame::GetGirlMotion();
	}
	else if (mode == CManager::MODE_TUTORIAL)
	{//チュートリアル
		pLoadTextBoy = CTutorial::GetBoyMotion();
		pLoadTextGirl = CTutorial::GetGirlMotion();
	}

	m_pMotionInfo[0] = pLoadTextBoy->GetMotionInfo();	//男性モーション情報の取得
	m_pMotionInfo[1] = pLoadTextGirl->GetMotionInfo();	//女性モーション情報の取得

	m_nMaxMotion[0] = pLoadTextBoy->GetMaxMotion();		//男性モーションの最大数の取得
	m_nMaxMotion[1] = pLoadTextGirl->GetMaxMotion();	//女性モーションの最大数の取得

	//モデル情報を取得
	m_ModelInfo[0] = pLoadTextBoy->GetModelInfo();		//男性モデルの情報を取得
	m_ModelInfo[1] = pLoadTextGirl->GetModelInfo();		//女性モデルの情報を取得
}

//=============================================================================
// ファイルの破棄
//=============================================================================
void CHumanBace::UnloadModel(void)
{
	//モーションデータの破棄
	for (int nCntModelInfo = 0; nCntModelInfo < MODEL_MAX; nCntModelInfo++)
	{
		if (m_pMotionInfo[nCntModelInfo] != NULL)
		{
			m_pMotionInfo[nCntModelInfo] = NULL;
		}
	}

	//テクスチャの破棄
	for (int nCntTex = 0; nCntTex < MODEL_MAX; nCntTex++)
	{
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CHumanBace::Init(void)
{
	//変数の初期化
	m_pos = VECTOR_ZERO;			//中心座標
	m_move = VECTOR_ZERO;			//移動
	m_rot = VECTOR_ZERO;			//向き
	m_vtxMax = VECTOR_ZERO;			//モデル頂点の最大値(Y)
	m_nCountTime = 0;				//カウンター
	m_pShadow = NULL;				//影ポインタの破棄
	m_pModel = NULL;				//モデルデータの破棄
	m_pMotion = NULL;				//モーションポインタ
	m_MotionType = MOTION_NEUTRAL;	//モーションの状態設定
	m_bClipping = true;				//クリッピングフラグ
	m_fLength = 0.0f;				//描画範囲
	m_MotionLength = 0.0f;			//モーションの更新範囲

	CreateModel();		//モデルの生成
	CreateMotion();		//モーションの生成

	//影の生成
	if (m_pShadow == NULL) { m_pShadow = CShadow::Create(m_pos, SHADOW_SIZE, SHADOW_SIZE, 0); }

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CHumanBace::Uninit(void)
{
	//影ポインタの初期化
	m_pShadow = NULL;

	//モーションの破棄
	if (m_pMotion != NULL)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = NULL;
	}

	//モデルデータの破棄
	for (int nCntPart = 0; nCntPart < m_ModelInfo[(int)m_ModelType].nMaxParts; nCntPart++)
	{
		if (m_pModel[nCntPart] != NULL)
		{
			m_pModel[nCntPart]->Uninit();
			delete m_pModel[nCntPart];
			m_pModel[nCntPart] = NULL;
		}
	}
	if (m_pModel != NULL)
	{
		delete[] m_pModel;
		m_pModel = NULL;
	}

	//死亡フラグを立てる
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CHumanBace::Update(void)
{
	//変数宣言
	bool bDraw = false;
	CManager::MODE mode = CManager::GetMode();

	//クリッピング処理
	if (m_bClipping == true)
	{
		D3DXVECTOR3 vtxMax = m_pos + m_vtxMax;
		D3DXVECTOR3 vtxMin = m_pos + m_vtxMin;
		CGameCamera * pGameCamera = NULL;

		if (mode == CManager::MODE_GAME)
		{//ゲーム
			pGameCamera = CGame::GetGameCamera();
		}
		else if (mode == CManager::MODE_TUTORIAL)
		{//チュートリアル
			pGameCamera = CTutorial::GetCamera();
		}

		//クリッピング処理
		bDraw = pGameCamera->Clipping(vtxMax, vtxMin);
		SetDraw(bDraw);
	}
	
	if(m_bClipping){ m_move.y -= GRAVITY; }		//重力加算
		
	m_pos += m_move;		//移動処理

	if (m_bClipping) { CollisionMesh(); }		//地面のあたり判定

	//影の移動
	if (m_pShadow != NULL) { m_pShadow->SetPositionShadow(D3DXVECTOR3(m_pos.x, m_pos.y + 0.1f, m_pos.z)); }

	//モーションの更新処理
	if (m_pMotion != NULL)
	{
		//距離を判定する
		if ((m_MotionType == MOTION_NEUTRAL) && (m_fLength > m_fLength)) { return; }

		int nNumMotion = m_pMotion->Update();

		//モーションのタイプ設定
		if (nNumMotion != m_MotionType) { m_MotionType = (MOTION_TYPE)nNumMotion; }
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CHumanBace::Draw(float fAlpha)
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

	//頂点法線の自動正規化
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	//モデルの描画
	for (int nCntPart = 0; nCntPart < m_ModelInfo[(int)m_ModelType].nMaxModel; nCntPart++)
	{
		if (m_pModel[nCntPart] != NULL)
		{
			//描画する
			m_pModel[nCntPart]->Draw(fAlpha);
		}
	}

	//頂点法線の自動正規化
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}

//=============================================================================
// 辺の長さを求める
//=============================================================================
float CHumanBace::GetLength(D3DXVECTOR3 StartPos, D3DXVECTOR3 EndPos)
{
	D3DXVECTOR3 Distance = StartPos - EndPos;
	float fLength = sqrtf((Distance.x * Distance.x) + (Distance.z * Distance.z));

	return fLength;
}

//=============================================================================
// 角度修正
//=============================================================================
void CHumanBace::RemakeAngle(float * pAngle)
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
// 逃げる処理
//=============================================================================
void CHumanBace::MoveEscape(void)
{
	//時間の加算
	m_nCountTime++;

	//プレイヤーとお客さんの角度を求める
	CManager::MODE mode = CManager::GetMode();
	CPlayer * pPlayer = NULL;

	if (mode == CManager::MODE_GAME)
	{//ゲーム
		pPlayer = CGame::GetPlayer();
	}
	else if (mode == CManager::MODE_TUTORIAL)
	{//チュートリアル
		pPlayer = CTutorial::GetPlayer();
	}

	D3DXVECTOR3 Distance = m_pos - pPlayer->GetPos();
	float fAngle = atan2f(Distance.x, Distance.z);
	RemakeAngle(&fAngle);

	//移動させる
	m_move.x = sinf(fAngle) * ESCAPE_SPEED;
	m_move.z = cosf(fAngle) * ESCAPE_SPEED;

	//逃げる方向に向かせる
	m_rot.y = fAngle + D3DX_PI;
	RemakeAngle(&m_rot.y);
}

//=============================================================================
// 地面のあたり判定
//=============================================================================
void CHumanBace::CollisionMesh(void)
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

			if (pField->OnField(m_pos, m_vtxMax.y))
			{// 傾斜の計算
				float fHight = pField->GetHeightMesh(m_pos);

				if (m_pos.y <= fHight)
				{
					m_pos.y = fHight;	//地面の高さを取得
					m_move.y = 0.0f;	//移動量を初期化する
					break;
				}
			}
		}
		//Nextに次のSceneを入れる
		pScene = pSceneNext;
	}
}

//=============================================================================
// 頂点の最大値を取得
//=============================================================================
D3DXVECTOR3 CHumanBace::GetVtxMax(void)
{
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);

	if (NULL != m_pModel)
	{// モデルが使われているとき
		for (int nCntParts = 0; nCntParts < m_ModelInfo[(int)m_ModelType].nMaxParts; nCntParts++)
		{// パーツ分ループ
			D3DXVECTOR3 vtxMaxModel = m_pModel[nCntParts]->GetVtxMax();
			D3DXVECTOR3 Scale = m_pModel[nCntParts]->GetScale();

			if (vtxMaxModel.x * Scale.x > vtxMax.x)
			{// Xの比較
				vtxMax.x = vtxMaxModel.x * Scale.x;
			}

			if (vtxMaxModel.y * Scale.y > vtxMax.y)
			{// Yの比較
				vtxMax.y = vtxMaxModel.y * Scale.y;
			}

			if (vtxMaxModel.z * Scale.z > vtxMax.z)
			{// Zの比較
				vtxMax.z = vtxMaxModel.z * Scale.z;
			}
		}
	}

	return vtxMax;
}

//=============================================================================
// 頂点の最小値の取得
//=============================================================================
D3DXVECTOR3 CHumanBace::GetVtxMin(void)
{
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);

	if (NULL != m_pModel)
	{// モデルが使われているとき
		for (int nCntParts = 0; nCntParts < m_ModelInfo[(int)m_ModelType].nMaxParts; nCntParts++)
		{// パーツ分ループ
			D3DXVECTOR3 vtxMinModel = m_pModel[nCntParts]->GetVtxMin();
			D3DXVECTOR3 Scale = m_pModel[nCntParts]->GetScale();

			if (vtxMinModel.x * Scale.x < vtxMin.x)
			{// Xの比較
				vtxMin.x = vtxMinModel.x * Scale.x;
			}

			if (vtxMinModel.y * Scale.y < vtxMin.y)
			{// Yの比較
				vtxMin.y = vtxMinModel.y * Scale.y;
			}

			if (vtxMinModel.z * Scale.z < vtxMin.z)
			{// Zの比較
				vtxMin.z = vtxMinModel.z * Scale.z;
			}
		}
	}

	return vtxMin;
}

//=============================================================================
// モデルの生成
//=============================================================================
void CHumanBace::CreateModel(void)
{
	//モデル情報を動的確保する
	m_pModel = new CModel *[m_ModelInfo[(int)m_ModelType].nMaxParts];

	//モデルの生成
	for (int nCntParts = 0; nCntParts < m_ModelInfo[(int)m_ModelType].nMaxParts; nCntParts++)
	{
		if (nCntParts == 0)
		{
			m_pModel[nCntParts] = CModel::Create(m_ModelInfo[(int)m_ModelType].pOfSetPos[nCntParts], &m_ModelInfo[(int)m_ModelType].paFileName[nCntParts][0], SCALE);
		}
		else
		{
			m_pModel[nCntParts] = CModel::Create(m_ModelInfo[(int)m_ModelType].pOfSetPos[nCntParts], &m_ModelInfo[(int)m_ModelType].paFileName[nCntParts][0]);
		}

		//テクスチャの割当て
		if (m_pModel[nCntParts] != NULL) { m_pModel[nCntParts]->BindTexture(m_pTexture[(int)m_ModelType]); }
	}

	//モデルの親設定
	for (int nCntParts = 0; nCntParts < m_ModelInfo[(int)m_ModelType].nMaxParts; nCntParts++)
	{
		if (nCntParts == 0)
		{
			m_pModel[0]->SetParent(NULL);
		}
		else
		{
			m_pModel[nCntParts]->SetParent(m_pModel[m_ModelInfo[(int)m_ModelType].pParent[nCntParts]]);
		}
	}

	//モデル頂点の最大値・最小値の取得
	m_vtxMax = GetVtxMax();
	m_vtxMin = GetVtxMin();
}

//=============================================================================
// モーションの生成
//=============================================================================
void CHumanBace::CreateMotion(void)
{
	if (m_pMotion == NULL)	//モーションの生成
	{
		m_pMotion = CMotion::Create();

		if (m_pMotion != NULL)
		{
			//モデルデータの設定
			m_pMotion->SetModel(m_pModel);									//モーションデータにモデル情報を渡す
			m_pMotion->SetMaxModel(m_ModelInfo[m_ModelType].nMaxParts);		//モデルパーツの最大数を渡す

																			//モーションデータの設定
			m_pMotion->SetMotion(m_pMotionInfo[m_ModelType]);				//モーション情報の取得
			m_pMotion->SetMaxMotion(CHumanBace::m_nMaxMotion[m_ModelType]);	//モーションの最大数の取得

			//初期化処理
			m_pMotion->Init();
		}
	}
}

//=============================================================================
// 次のモーションを設定する
//=============================================================================
void CHumanBace::SetNextMotion(MOTION_TYPE type)
{
	m_MotionType = type;	//モーションのタイプ設定

	if (m_pMotion != NULL) { m_pMotion->SetNumMotion((int)m_MotionType); }	//モーションを設定する
}

//=============================================================================
// 影のα値の色の設定
//=============================================================================
void CHumanBace::SetShadowColor_A(float col_a)
{
	if (m_pShadow != NULL)
	{
		m_pShadow->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, col_a));
	}
}
