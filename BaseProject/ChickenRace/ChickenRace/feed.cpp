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
D3DXVECTOR3 CFeed::m_VtxMaxModel[FEEDTYPE_MAX] = {};
D3DXVECTOR3 CFeed::m_VtxMinModel[FEEDTYPE_MAX] = {};

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CFeed::CFeed() : CModel3D(FEED_PRIOTITY, CScene::OBJTYPE_FEED)
{
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_feedType = FEEDTYPE::FEEDTYPE_ATTACK;
}
//===============================================================================
//　デストラクタ
//===============================================================================
CFeed::~CFeed() {}

//===============================================================================
//　生成
//===============================================================================
CFeed * CFeed::Create(D3DXVECTOR3 pos, int nZone, int nType)
{
	CFeed *pFeed = NULL;

	// NULLチェック
	if (pFeed == NULL)
	{// メモリの動的確保

		pFeed = new CFeed;

		if (pFeed != NULL)
		{
			// モデルの設定
			pFeed->SetModelType(nType + MODEL_TYPE_FEED_K);
			// テクスチャの設定
			pFeed->SetTextureType(nType + TEXTURE_TYPE_FEED_K);
			//種類の代入
			pFeed->m_feedType = (FEEDTYPE)nType;
			// オブジェクトクラスの生成
			pFeed->Init();
			// 位置を代入
			pFeed->SetPosition(pos);
		}
	}

	return pFeed;
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

	switch (m_feedType)
	{
	case FEEDTYPE::FEEDTYPE_ATTACK:	m_rot.x = 0.7f;	break;
	case FEEDTYPE::FEEDTYPE_ANNOY:	break;
	case FEEDTYPE::FEEDTYPE_SPEED:	m_rot.x = -0.7f; break;
	}

	//位置の代入
	CModel3D::SetPosition(pos);

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

	m_rot.y += 0.007f;

	CModel3D::SetRot(m_rot);
}
//=============================================================================
// 描画処理
//=============================================================================
void CFeed::Draw(void)
{
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
// Xファイルの読み込み
//===============================================================================
HRESULT CFeed::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	int			nNumVtx = 0;						//頂点数
	DWORD		sizeFVF;							//頂点フォーマットのサイズ
	BYTE		*pVtxBuff;							//頂点バッファへのポインタ

													//モデルの最大値・最小値を取得する
	for (int nCntModel = 0; nCntModel < FEEDTYPE_MAX; nCntModel++)
	{
		m_VtxMaxModel[nCntModel] = D3DXVECTOR3(-10000, -10000, -10000);	//最大値
		m_VtxMinModel[nCntModel] = D3DXVECTOR3(10000, 10000, 10000);	//最小値

		CModel3D::MODEL_TYPE type;
		switch (nCntModel)
		{
		case FEEDTYPE_ATTACK:	type = MODEL_TYPE_FEED_K;	break;
		case FEEDTYPE_ANNOY:	type = MODEL_TYPE_FEED_B;	break;
		case FEEDTYPE_SPEED:	type = MODEL_TYPE_FEED_S;	break;
		}

		LPD3DXMESH &Mesh = MeshLoad(type);

		if (Mesh != NULL)
		{
			//頂点数を取得
			nNumVtx = Mesh->GetNumVertices();

			//頂点フォーマットのサイズを取得
			sizeFVF = D3DXGetFVFVertexSize(Mesh->GetFVF());

			//頂点バッファのロック
			Mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		//頂点座標の代入

																//最大値
				if (vtx.x > m_VtxMaxModel[nCntModel].x)
				{
					m_VtxMaxModel[nCntModel].x = vtx.x;
				}
				if (vtx.y > m_VtxMaxModel[nCntModel].y)
				{
					m_VtxMaxModel[nCntModel].y = vtx.y;
				}
				if (vtx.z > m_VtxMaxModel[nCntModel].z)
				{
					m_VtxMaxModel[nCntModel].z = vtx.z;
				}
				//最小値
				if (vtx.x < m_VtxMinModel[nCntModel].x)
				{
					m_VtxMinModel[nCntModel].x = vtx.x;
				}
				if (vtx.y < m_VtxMinModel[nCntModel].y)
				{
					m_VtxMinModel[nCntModel].y = vtx.y;
				}
				if (vtx.z < m_VtxMinModel[nCntModel].z)
				{
					m_VtxMinModel[nCntModel].z = vtx.z;
				}

				//サイズ文のポインタを進める
				pVtxBuff += sizeFVF;
			}

			//頂点バッファのアンロック
			Mesh->UnlockVertexBuffer();
		}
	}

	return S_OK;
}

//===============================================================================
// テクスチャの破棄
//===============================================================================
void CFeed::UnLoad(void)
{

}


//===============================================================================
// 当たり判定
//===============================================================================
bool CFeed::CollisionFeed(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld)
{
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();

	//あたっているかあたってないか
	bool bHit = false;

	// 各種情報の取得
	D3DXVECTOR3 ModelPos = CModel3D::GetPosition();		// 位置
	D3DXVECTOR3 VtxMax = m_VtxMaxModel[m_feedType];			// モデルの最大値
	D3DXVECTOR3 VtxMin = m_VtxMinModel[m_feedType];			// モデルの最小値
	D3DXVECTOR3 rot = CModel3D::GetRot();

	D3DXVECTOR3 ModelMax = ModelPos + VtxMax;	// 位置込みの最大値
	D3DXVECTOR3 ModelMin = ModelPos + VtxMin;	// 位置込みの最小値

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
