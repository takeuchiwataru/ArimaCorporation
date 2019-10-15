//=============================================================================
//
// 餌の処理 [feed.cpp]
// Author : 長山拓実
//
//=============================================================================
#include "scene.h"
#include "feed.h"
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
#define FEED_NAME	"data\\MODEL\\box.x"			// 読み込むモデルファイル

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
LPD3DXMESH CFeed::m_pMeshModel = NULL;						//メッシュ情報へのポインタ
LPD3DXBUFFER CFeed::m_pBuffMatModel = NULL;					//マテリアルの情報へのポインタ
DWORD CFeed::m_nNumMatModel = NULL;							//マテリアルの情報数
LPDIRECT3DTEXTURE9 CFeed::m_pMeshTextures = NULL;
D3DXVECTOR3 CFeed::m_VtxMaxModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 CFeed::m_VtxMinModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CFeed::CFeed() : CModel3D(FEED_PRIOTITY, CScene::OBJTYPE_OBJECT)
{
	m_scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 大きさ
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
//===============================================================================
//　デストラクタ
//===============================================================================
CFeed::~CFeed() {}

//===============================================================================
//　生成
//===============================================================================
CFeed * CFeed::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nObjectType)
{
	CFeed *pObject = NULL;

	// NULLチェック
	if (pObject == NULL)
	{// メモリの動的確保

		pObject = new CFeed;

		if (pObject != NULL)
		{
			// 種類の設定
			pObject->BindModel(m_pMeshModel, m_pBuffMatModel, m_nNumMatModel, m_pMeshTextures,
				m_VtxMaxModel, m_VtxMinModel);
			// オブジェクトごとの設定用タイプ
			pObject->m_nType = nObjectType;
			// サイズを代入
			pObject->m_scale = scale;
			// サイズを親クラスに代入
			pObject->SetScale(scale);
			// オブジェクトクラスの生成
			pObject->Init();
			// 位置を代入
			pObject->SetPosition(pos);
			// 回転を反映
			pObject->SetRot(rot);
		}
	}

	return pObject;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CFeed::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//3DモデルのInit
	CModel3D::Init();

	// 位置の初期化
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//位置の代入
	CModel3D::SetPosition(pos);

	// 各種情報の代入
	CModel3D::SetScale(m_scale);

	//変数の初期化
	m_pObjBill = NULL;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CFeed::Uninit(void)
{
	//オブジェクトビルボード
	m_pObjBill = NULL;

	//3DモデルのUninit
	CModel3D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CFeed::Update(void)
{
	CModel3D::Update();

	//距離の取得
	float fLength = CModel3D::GetLength();

	if (CModel3D::GetDelete() == true) { Uninit(); }
}
//=============================================================================
// 描画処理
//=============================================================================
void CFeed::Draw(void)
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
// Xファイルの読み込み
//===============================================================================
HRESULT CFeed::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//マテリアルデータへのポインタ
	D3DXMATERIAL *pMat;

	// Xファイルの読み込み
	D3DXLoadMeshFromX(FEED_NAME, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel, NULL, &m_nNumMatModel, &m_pMeshModel);


	//マテリアル情報からテクスチャの取得
	pMat = (D3DXMATERIAL*)m_pBuffMatModel->GetBufferPointer();


	int nNumVtx;		//頂点数
	DWORD sizeFVF;		//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファへのポインタ

	m_VtxMaxModel = D3DXVECTOR3(-10000, -10000, -10000);	//最大値
	m_VtxMinModel = D3DXVECTOR3(10000, 10000, 10000);	//最小値

																		//頂点数を取得
	nNumVtx = m_pMeshModel->GetNumVertices();

	//頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(m_pMeshModel->GetFVF());

	//頂点バッファのロック
	m_pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		//頂点座標の代入

														//最大値
		if (vtx.x > m_VtxMaxModel.x)
		{
			m_VtxMaxModel.x = vtx.x;
		}
		if (vtx.y > m_VtxMaxModel.y)
		{
			m_VtxMaxModel.y = vtx.y;
		}
		if (vtx.z > m_VtxMaxModel.z)
		{
			m_VtxMaxModel.z = vtx.z;
		}
		//最小値
		if (vtx.x < m_VtxMinModel.x)
		{
			m_VtxMinModel.x = vtx.x;
		}
		if (vtx.y < m_VtxMinModel.y)
		{
			m_VtxMinModel.y = vtx.y;
		}
		if (vtx.z < m_VtxMinModel.z)
		{
			m_VtxMinModel.z = vtx.z;
		}

		//サイズ文のポインタを進める
		pVtxBuff += sizeFVF;
	}

	//頂点バッファのアンロック
	m_pMeshModel->UnlockVertexBuffer();


	//使っているテクスチャ
	//D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_1, &m_pMeshTextures[0]);

	return S_OK;
}

//===============================================================================
// テクスチャの破棄
//===============================================================================
void CFeed::UnLoad(void)
{
	// メッシュの開放
	if (m_pMeshModel != NULL)
	{
		m_pMeshModel->Release();
		m_pMeshModel = NULL;
	}

	// マテリアルの開放
	if (m_pBuffMatModel != NULL)
	{
		m_pBuffMatModel->Release();
		m_pBuffMatModel = NULL;
	}

	//テクスチャ
	if (m_pMeshTextures != NULL)
	{
		m_pMeshTextures->Release();
		m_pMeshTextures = NULL;
	}
}


//===============================================================================
// 当たり判定
//===============================================================================
bool CFeed::Collision(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld, D3DXVECTOR3 * pMove)
{
	//19以外はこの処理に入らない
	if (m_nType != 19) return false;

	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();

	//あたっているかあたってないか
	bool bHit = false;

	// 各種情報の取得
	D3DXVECTOR3 ModelPos = CModel3D::GetPosition();		// 位置
	D3DXVECTOR3 VtxMax = CModel3D::GetVtxMax();			// モデルの最大値
	D3DXVECTOR3 VtxMin = CModel3D::GetVtxMin();			// モデルの最小値
	D3DXVECTOR3 rot = CModel3D::GetRot();

	D3DXVECTOR3 ModelMax = CModel3D::GetPosition() + CModel3D::GetVtxMax();	// 位置込みの最大値
	D3DXVECTOR3 ModelMin = CModel3D::GetPosition() + CModel3D::GetVtxMin();	// 位置込みの最小値

	if (pPos->x >= ModelMin.x - PLAYER_DEPTH && pPos->x <= ModelMax.x + PLAYER_DEPTH)
	{// Zの範囲内にいる
		if (pPos->z >= ModelMin.z - PLAYER_DEPTH && pPos->z <= ModelMax.z + PLAYER_DEPTH)
		{// Xの範囲内にいる
			if (pPosOld->y >= ModelMax.y && pPos->y <= ModelMax.y)
			{// オブジェクトの上から当たる場合
				bHit = true;
			}
			else if (pPosOld->y + PLAYER_HEIGHT <= ModelMin.y && pPos->y + PLAYER_HEIGHT >= ModelMin.y)
			{// オブジェクトの下から当たる場合
				bHit = true;
			}

			if (!(pPos->y >= ModelMax.y) && !(pPos->y + PLAYER_HEIGHT <= ModelMin.y))
			{// オブジェクト横との当たり判定
				bHit = true;
			}
		}
	}

	// 位置の代入
	CModel3D::SetPosition(ModelPos);

	return bHit;
}