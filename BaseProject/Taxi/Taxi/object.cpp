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

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_NAME_1	"data\\MODEL\\壊れるもの\\tree2.x"			//読み込むテクスチャファイル

#define TEXTURE_NAME_1	"data\\TEXTURE\\modeltex\\leaf.png"		//読み込むテクスチャファイル

#define MODEL_SPEED				(5.0f)
#define PLAYER_DEPTH			(50)		// プレイヤーの幅調整用
#define PLAYER_HEIGHT			(100.0f)	// プレイヤーの背の高さ
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
LPD3DXMESH CObject::m_pMeshModel[MAX_OBJECT] = {};						//メッシュ情報へのポインタ
LPD3DXBUFFER CObject::m_pBuffMatModel[MAX_OBJECT] = {};					//マテリアルの情報へのポインタ
DWORD CObject::m_nNumMatModel[MAX_OBJECT] = {};							//マテリアルの情報数
LPDIRECT3DTEXTURE9 CObject::m_pMeshTextures[MAX_OBJECT_TEXTURE] = {};
D3DXVECTOR3 CObject::m_LoadVtxMaxModel[MAX_OBJECT] = {};
D3DXVECTOR3 CObject::m_LoadVtxMinModel[MAX_OBJECT] = {};

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CObject::CObject() : CModel3D(OBJECT_PRIOTITY, CScene::OBJTYPE_OBJECT)
{
	m_ModelMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Spin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nStageCount = 0;
	m_bStageNext = false;
}
//===============================================================================
//　デストラクタ
//===============================================================================
CObject::~CObject(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CObject::Init(void)
{
	//アップデートを通さないオブジェクトのタイプ
	//int anUpdateType[UPDATE_TYPE_NUM] = { TYPE_TREE00, TYPE_TREE01, TYPE_BILL00, TYPE_BILL01,
	//									  TYPE_BILL02, TYPE_TVBILL, TYPE_TANUKI,
	//									  TYPE_OCLOCK, TYPE_REDBILL, TYPE_CORN2,
	//									  TYPE_STATION, TYPE_ESTA, TYPE_DAIMAL, TYPE_APIA,
	//									  TYPE_TOWER, TYPE_FOUNTAIN, TYPE_FERRISWGEEL, TYPE_TAPIOCA, TYPE_HOSPITAL, TYPE_LCDPANEL };

	////小さいオブジェクトのタイプ
	//int anSmallObjType[SMALL_OBJ_NUM] = { TYPE_DRINKMACHINE, TYPE_GRASS, TYPE_CARDBORD, TYPE_CORN,
	//									  TYPE_BENCH, TYPE_PHONEBOX, TYPE_LEAF, TYPE_SIGNBOARD, TYPE_BEERBOX, TYPE_FENCE};

	////高いオブジェクトのタイプ
	//int anHightObjType[HIGHT_OBJ_NUM] = { TYPE_STREETLIGHT, TYPE_TRAFFICLIGHT00, TYPE_TRAFFICLIGHT01, TYPE_ROAD};

	// 仮
	int anUpdateType[UPDATE_TYPE_NUM] = {};
	int anSmallObjType[SMALL_OBJ_NUM] = {};
	int anHightObjType[HIGHT_OBJ_NUM] = {};

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

	//アップデートを通すかどうかの判定
	bool bUpdate = false;
	for (int nCntType = 0; nCntType < UPDATE_TYPE_NUM; nCntType++)
	{
		if (m_nType == anUpdateType[nCntType])
		{
			CModel3D::SetUpdate(false);
			bUpdate = true;
			break;
		}
	}

	if (!bUpdate)
	{
		bool bSmallObj = false;
		for (int nCntType = 0; nCntType < SMALL_OBJ_NUM; nCntType++)
		{// 小さいオブジェクトかどうかの判定
			if (m_nType == anSmallObjType[nCntType])
			{
				CModel3D::SetSmallObj(true);
				bSmallObj = true;
				break;
			}
		}

		for (int nCntType = 0; nCntType < HIGHT_OBJ_NUM; nCntType++)
		{// 高いオブジェクトのタイプかどうかの判定
			if(m_nType == anHightObjType[nCntType])
			{
				CModel3D::SetHightObj(true);
				break;
			}
		}
	}

	//変数の初期化
	m_pObjBill = NULL;
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
	float fLength = CModel3D::GetLength();

	if (CModel3D::GetDelete() == true) { Uninit(); }
}
//=============================================================================
// 描画処理
//=============================================================================
void CObject::Draw(void)
{
	if (m_nType == 19) { return; }

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//位置の初期化
	D3DXVECTOR3 Modelpos = CModel3D::GetPosition();
	//プレイヤーの位置情報
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();
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
CObject * CObject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, float move, int nTexType, int nObjectType, MOVETYPE nMovetype, int nCollision)
{
	CObject *pObject = NULL;

	// NULLチェック
	if (pObject == NULL)
	{// メモリの動的確保

		pObject = new CObject;

		if (pObject != NULL)
		{
			// 種類の設定
			pObject->BindModel(m_pMeshModel[nObjectType], m_pBuffMatModel[nObjectType], m_nNumMatModel[nObjectType], m_pMeshTextures[nTexType],
							   m_LoadVtxMaxModel[nObjectType], m_LoadVtxMinModel[nObjectType]);
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
			// 動きの種類と移動量を設定
			pObject->SetMoveType(nMovetype);
			//pObject->m_move = D3DXVECTOR3(move, move, move);
			// コリジョンをONOFF
			pObject->m_nCollision = nCollision;
		}
	}

	return pObject;
}
//===============================================================================
// Xファイルの読み込み
//===============================================================================
HRESULT CObject::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//マテリアルデータへのポインタ
	D3DXMATERIAL *pMat;

	// Xファイルの読み込み
	D3DXLoadMeshFromX(MODEL_NAME_1, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[0], NULL, &m_nNumMatModel[0], &m_pMeshModel[0]);

	for (int nCount = 0; nCount < MAX_OBJECT; nCount++)
	{
		//マテリアル情報からテクスチャの取得
		pMat = (D3DXMATERIAL*)m_pBuffMatModel[nCount]->GetBufferPointer();
	}

	int nNumVtx;		//頂点数
	DWORD sizeFVF;		//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファへのポインタ

	//モデルの最大値・最小値を取得する
	for (int nCntModel = 0; nCntModel < MAX_OBJECT; nCntModel++)
	{
		m_LoadVtxMaxModel[nCntModel] = D3DXVECTOR3(-10000, -10000, -10000);	//最大値
		m_LoadVtxMinModel[nCntModel] = D3DXVECTOR3(10000, 10000, 10000);	//最小値

		//頂点数を取得
		nNumVtx = m_pMeshModel[nCntModel]->GetNumVertices();

		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(m_pMeshModel[nCntModel]->GetFVF());

		//頂点バッファのロック
		m_pMeshModel[nCntModel]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		//頂点座標の代入

			//最大値
			if (vtx.x > m_LoadVtxMaxModel[nCntModel].x)
			{
				m_LoadVtxMaxModel[nCntModel].x = vtx.x;
			}
			if (vtx.y > m_LoadVtxMaxModel[nCntModel].y)
			{
				m_LoadVtxMaxModel[nCntModel].y = vtx.y;
			}
			if (vtx.z > m_LoadVtxMaxModel[nCntModel].z)
			{
				m_LoadVtxMaxModel[nCntModel].z = vtx.z;
			}
			//最小値
			if (vtx.x < m_LoadVtxMinModel[nCntModel].x)
			{
				m_LoadVtxMinModel[nCntModel].x = vtx.x;
			}
			if (vtx.y < m_LoadVtxMinModel[nCntModel].y)
			{
				m_LoadVtxMinModel[nCntModel].y = vtx.y;
			}
			if (vtx.z < m_LoadVtxMinModel[nCntModel].z)
			{
				m_LoadVtxMinModel[nCntModel].z = vtx.z;
			}

			//サイズ文のポインタを進める
			pVtxBuff += sizeFVF;
		}

		//頂点バッファのアンロック
		m_pMeshModel[nCntModel]->UnlockVertexBuffer();
	}

	//使っているテクスチャ
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_1, &m_pMeshTextures[0]);

	return S_OK;
}
//===============================================================================
// テクスチャの破棄
//===============================================================================
void CObject::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_OBJECT; nCount++)
	{
		// メッシュの開放
		if (m_pMeshModel[nCount] != NULL)
		{
			m_pMeshModel[nCount]->Release();
			m_pMeshModel[nCount] = NULL;
		}
		// マテリアルの開放
		if (m_pBuffMatModel[nCount] != NULL)
		{
			m_pBuffMatModel[nCount]->Release();
			m_pBuffMatModel[nCount] = NULL;
		}
	}
	//テクスチャ
	for (int nCntTex = 0; nCntTex < MAX_OBJECT_TEXTURE; nCntTex++)
	{
		if (m_pMeshTextures[nCntTex] != NULL)
		{
			m_pMeshTextures[nCntTex]->Release();
			m_pMeshTextures[nCntTex] = NULL;
		}
	}
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

	D3DXVECTOR3 vtxMax = CModel3D::GetVtxMax();		// 頂点座標の最大値の取得

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
	//19以外はこの処理に入らない
	if (m_nType != 19) return false;

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
		MOVETYPE ModelMoveType = CModel3D::GetMoveType();	// 動きのタイプ
		D3DXVECTOR3 VtxMax = CModel3D::GetVtxMax();			// モデルの最大値
		D3DXVECTOR3 VtxMin = CModel3D::GetVtxMin();			// モデルの最小値
		D3DXVECTOR3 rot = CModel3D::GetRot();

		D3DXVECTOR3 ModelMax = CModel3D::GetPosition() + CModel3D::GetVtxMax();	// 位置込みの最大値
		D3DXVECTOR3 ModelMin = CModel3D::GetPosition() + CModel3D::GetVtxMin();	// 位置込みの最小値

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
