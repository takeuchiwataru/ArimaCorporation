//=============================================================================
//
// 車の動くポイントの処理 [car_point.cpp]
// Author : YUTARO　ABE
//
//=============================================================================
#include "scene.h"
#include "model3D.h"
#include "manager.h"
#include "meshfield.h"
#include "player.h"
#include "fade.h"
#include "car_point.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_NAME_1	"data\\MODEL\\point\\point.x"					//読み込むモデルファイル

#define TEXTURE_NAME_1	"data\\TEXTURE\\modeltex\\carup.jpg"		//読み込むテクスチャファイル

#define MODEL_SPEED (5.0f)
#define CAR_DEPTH	(10)			// NPC車の幅調整用
#define CAR_HEIGHT	(100.0f)		// NPC車の背の高さ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPD3DXMESH CCarPoint::m_pMeshModel[MAX_CARPOINT] = {};						//メッシュ情報へのポインタ
LPD3DXBUFFER CCarPoint::m_pBuffMatModel[MAX_CARPOINT] = {};					//マテリアルの情報へのポインタ
DWORD CCarPoint::m_nNumMatModel[MAX_CARPOINT] = {};							//マテリアルの情報数
LPDIRECT3DTEXTURE9 CCarPoint::m_pMeshTextures[MAX_CARPOINT_TEXTURE] = {};
D3DXVECTOR3 CCarPoint::m_LoadVtxMaxModel[MAX_CARPOINT] = {};
D3DXVECTOR3 CCarPoint::m_LoadVtxMinModel[MAX_CARPOINT] = {};


//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CCarPoint::CCarPoint() : CModel3D(CARPOINT_PRIOTITY, CScene::OBJTYPE_CARPOINT)
{
	m_nObjNumber = 0;
	m_bLook = false;
}
//===============================================================================
//　デストラクタ
//===============================================================================
CCarPoint::~CCarPoint()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCarPoint::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//3DモデルのInit
	CModel3D::Init();

	// 位置の初期化
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 scale = D3DXVECTOR3(3.0f, 5.0f, 3.0f);

	//位置の代入
	CModel3D::SetPosition(pos);
	CModel3D::SetScale(scale);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCarPoint::Uninit(void)
{
	//3DモデルのUninit
	CModel3D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CCarPoint::Update(void)
{
	//3DモデルのUpdate
	CModel3D::Update();
	//位置の初期化
	D3DXVECTOR3 pos = CModel3D::GetPosition();
	// 移動量の初期化
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//向きの代入
	D3DXVECTOR3 &rot = CModel3D::GetRot();
	//大きさの初期化
	D3DXVECTOR3 scale = D3DXVECTOR3(3.0f, 5.0f, 3.0f);


	// 各種情報の代入
	CModel3D::SetScale(scale);
	CModel3D::SetPosition(pos);
	CModel3D::SetMove(move);
}
//=============================================================================
// 描画処理
//=============================================================================
void CCarPoint::Draw(void)
{
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//頂点法線の自動正規化
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	if (pCInputKeyBoard->GetKeyboardTrigger(DIK_RETURN) == true)
	{
		m_bLook = m_bLook ? false : true;
	}
	if (m_bLook == true)
	{
		//3DモデルのDraw
		CModel3D::Draw();
	}

	//頂点法線の自動正規化
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}
//===============================================================================
//　クリエイト
//===============================================================================
CCarPoint * CCarPoint::Create(int nNum ,D3DXVECTOR3 pos)
{
	CCarPoint *pCarPoint = NULL;

	// NULLチェック
	if (pCarPoint == NULL)
	{// メモリの動的確保
		pCarPoint = new CCarPoint;
		if (pCarPoint != NULL)
		{
			// 種類の設定
			pCarPoint->BindModel(m_pMeshModel[0], m_pBuffMatModel[0], m_nNumMatModel[0], m_pMeshTextures[0],
				m_LoadVtxMaxModel[0], m_LoadVtxMinModel[0]);
			// オブジェクトクラスの生成
			pCarPoint->Init();;
			// 位置を代入
			pCarPoint->SetPosition(pos);
			//	ポイントの番号
			pCarPoint->m_nObjNumber = nNum;
		}
	}

	//	値を返す
	return pCarPoint;
}
//===============================================================================
// Xファイルの読み込み
//===============================================================================
HRESULT CCarPoint::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//マテリアルデータへのポインタ
	D3DXMATERIAL *pMat;

	// Xファイルの読み込み
	D3DXLoadMeshFromX(MODEL_NAME_1, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel[0], NULL, &m_nNumMatModel[0], &m_pMeshModel[0]);

	for (int nCount = 0; nCount < MAX_CARPOINT; nCount++)
	{
		//マテリアル情報からテクスチャの取得
		pMat = (D3DXMATERIAL*)m_pBuffMatModel[nCount]->GetBufferPointer();
	}

	int nNumVtx;		//頂点数
	DWORD sizeFVF;		//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファへのポインタ

						//モデルの最大値・最小値を取得する
	for (int nCntModel = 0; nCntModel < MAX_CARPOINT; nCntModel++)
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
void CCarPoint::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_CARPOINT; nCount++)
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
	for (int nCntTex = 0; nCntTex < MAX_CARPOINT_TEXTURE; nCntTex++)
	{
		if (m_pMeshTextures[nCntTex] != NULL)
		{
			m_pMeshTextures[nCntTex]->Release();
			m_pMeshTextures[nCntTex] = NULL;
		}
	}
}