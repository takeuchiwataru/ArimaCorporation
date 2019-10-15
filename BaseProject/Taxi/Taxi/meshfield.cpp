//=============================================================================
//
// メッシュの処理 [mesh.cpp]
// Author : 有馬武志
//
//=============================================================================
#include "meshfield.h"
#include "mesh.h"
#include "manager.h"
#include "scene.h"
#include "model.h"
#include "scene3D.h"
#include "game.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MESHFIELD_0		"data\\TEXTURE\\mesh\\field_000.jpg"	// 読み込むテクスチャファイル名
#define TEXTURE_MESHFIELD_1		"data\\TEXTURE\\mesh\\field_001.jpg"	// 読み込むテクスチャファイル名
#define TEXTURE_MESHFIELD_2		"data\\TEXTURE\\mesh\\field_002.png"	// 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CMeshField::m_pTexture[MAX_MESH_TEXTURE] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 CMeshField::m_pVtxBuff = NULL;				// 頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 CMeshField::m_pIdxBuff = NULL;				// インデックスへのポインタ

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CMeshField::CMeshField()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nWidthDivide = 5;
	m_nDepthDivide = 5;
	m_fTextXUV = 1.0f;
	m_fTextYUV = 1.0f;
	m_fWidthLength = 500.0f;
	m_fDepthLength = 500.0f;
	m_fVtxHeight_No0 = 0.0f;
	m_fVtxHeight_No1 = 0.0f;
	m_fVtxHeight_No2 = 0.0f;
	m_fVtxHeight_No3 = 0.0f;
	m_nTexType = 0;
}
//===============================================================================
//　デストラクタ
//===============================================================================
CMeshField::~CMeshField() { }

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMeshField::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	CMesh::Init();

	CScene::SetObjType(CScene::OBJTYPE_GROUND);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMeshField::Uninit(void)
{
	CMesh::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CMeshField::Update(void)
{
	CMesh::Update();
}
//=============================================================================
// 描画処理
//=============================================================================
void CMeshField::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//プレイヤーの位置情報
	D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();
	//ゲームの情報
	CManager::MODE pMode = CManager::GetMode();

	CMesh::Draw();
}
//===============================================================================
//　クリエイト
//===============================================================================
CMeshField * CMeshField::Create(D3DXVECTOR3 pos,int nMeshX, int nMeshZ, float fMeshXUV, float fMeshYUV, float fMeshWidth, float fMeshDepth, float fVtx0, float fVtxMeshX, float fVtxMeshZ, float fVtxMeshXMeshZ, int nTexType,int nMeshType)
{
	CMeshField *pMeshField = NULL;

	//NULLチェック
	if (pMeshField == NULL)
	{//メモリの動的確保

		pMeshField = new CMeshField;

		if (pMeshField != NULL)
		{
			//位置を変数に代入
			pMeshField->m_pos = pos;
			//位置の代入
			pMeshField->SetPosition(pos);
			//メッシュの種類
			pMeshField->SetMeshType(nMeshType);
			//メッシュの情報を親クラスに渡す
			pMeshField->SetMeshInfo(nMeshX, nMeshZ, fMeshXUV, fMeshYUV, fMeshDepth, fMeshWidth);
			//メッシュの頂点の高さ
			pMeshField->SetVtx(fVtx0, fVtxMeshX, fVtxMeshZ, fVtxMeshXMeshZ);
			//オブジェクトクラスの生成
			pMeshField->Init();
			//テクスチャの設定
			pMeshField->BindTexture(m_pTexture[nTexType]);

			//ファイル書き込み用変数
			pMeshField->m_nWidthDivide = nMeshX;
			pMeshField->m_nDepthDivide = nMeshZ;
			pMeshField->m_fTextXUV = fMeshXUV;
			pMeshField->m_fTextYUV = fMeshYUV;
			pMeshField->m_fWidthLength = fMeshWidth;
			pMeshField->m_fDepthLength = fMeshDepth;
			pMeshField->m_fVtxHeight_No0 = fVtx0;
			pMeshField->m_fVtxHeight_No1 = fVtxMeshX;
			pMeshField->m_fVtxHeight_No2 = fVtxMeshZ;
			pMeshField->m_fVtxHeight_No3 = fVtxMeshXMeshZ;
			pMeshField->m_nTexType = nTexType;
		}
	}

	return pMeshField;
}
//===============================================================================
// テクスチャの読み込み
//===============================================================================
HRESULT CMeshField::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD_0, &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD_1, &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD_2, &m_pTexture[2]);

	return S_OK;
}
//===============================================================================
// テクスチャの破棄
//===============================================================================
void CMeshField::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_MESH_TEXTURE; nCount++)
	{
		//テクスチャーの破棄
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}
