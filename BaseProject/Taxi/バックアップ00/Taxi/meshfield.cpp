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
#define TEXTURE_MESHFIELD_0		"data\\TEXTURE\\mesh\\stairs.png"	// 読み込むテクスチャファイル名
#define TEXTURE_MESHFIELD_1		"data\\TEXTURE\\mesh\\flooa000.jpg"	// 読み込むテクスチャファイル名
#define TEXTURE_MESHFIELD_2		"data\\TEXTURE\\mesh\\field03.jpg"	// 読み込むテクスチャファイル名
#define TEXTURE_MESHFIELD_3		"data\\TEXTURE\\mesh\\green2.jpg"	// 読み込むテクスチャファイル名
#define TEXTURE_MESHFIELD_4		"data\\TEXTURE\\mesh\\flooa001.jpg"	// 読み込むテクスチャファイル名
#define TEXTURE_MESHFIELD_5		"data\\TEXTURE\\mesh\\flooa002.jpg"	// 読み込むテクスチャファイル名
#define TEXTURE_MESHFIELD_6		"data\\TEXTURE\\mesh\\道路縦.jpg"	// 読み込むテクスチャファイル名
#define TEXTURE_MESHFIELD_7		"data\\TEXTURE\\mesh\\道路横.jpg"	// 読み込むテクスチャファイル名
#define TEXTURE_MESHFIELD_8		"data\\TEXTURE\\mesh\\建物下.jpg"	// 読み込むテクスチャファイル名
#define TEXTURE_MESHFIELD_9		"data\\TEXTURE\\mesh\\歩行線縦.jpg"	// 読み込むテクスチャファイル名
#define TEXTURE_MESHFIELD_10	"data\\TEXTURE\\mesh\\歩行線横.jpg"	// 読み込むテクスチャファイル名
#define TEXTURE_MESHFIELD_11	"data\\TEXTURE\\mesh\\交差点.jpg"	// 読み込むテクスチャファイル名
#define TEXTURE_MESHFIELD_12	"data\\TEXTURE\\mesh\\flooa003.jpg"	// 読み込むテクスチャファイル名
#define TEXTURE_MESHFIELD_13	"data\\TEXTURE\\mesh\\water.jpg"	// 読み込むテクスチャファイル名
#define TEXTURE_MESHFIELD_14	"data\\TEXTURE\\mesh\\col.jpg"		// 読み込むテクスチャファイル名

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
//int CMeshField::m_nMeshType = 0;
//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CMeshField::CMeshField()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
//===============================================================================
//　デストラクタ
//===============================================================================
CMeshField::~CMeshField()
{

}
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
	CMesh::Draw();
}
//===============================================================================
//　クリエイト
//===============================================================================
CMeshField * CMeshField::Create(D3DXVECTOR3 pos,int nMeshX, int nMeshZ, float fMeshXUV, float fMeshYUV, float fMeshWidth, float fMeshDepth,float fVtx0, float fVtxMeshX, float fVtxMeshZ, float fVtxMeshXMeshZ, int nTexType,int nMeshType)
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

			pMeshField->m_nMeshType = nTexType;
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
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD_3, &m_pTexture[3]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD_4, &m_pTexture[4]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD_5, &m_pTexture[5]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD_6, &m_pTexture[6]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD_7, &m_pTexture[7]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD_8, &m_pTexture[8]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD_9, &m_pTexture[9]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD_10, &m_pTexture[10]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD_11, &m_pTexture[11]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD_12, &m_pTexture[12]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD_13, &m_pTexture[14]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD_14, &m_pTexture[15]);

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
//=============================================================================
//	ステージ移動時に初期化するため
//=============================================================================
void CMeshField::DeleteMeshField(void)
{
	CScene *pScene;

	//プライオリティーチェック
	pScene = CScene::GetTop(MESH_PRIOTITY);

	//NULLチェック
	while (pScene != NULL)
	{
		//UpdateでUninitされてしまう場合　Nextが消える可能性があるからNextにデータを残しておく
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetDeath() == false)
		{//タイプが壁だったら
			if (pScene->GetObjType() == OBJTYPE_MESH)
			{
				pScene->Uninit();
			}
		}
		//Nextに次のSceneを入れる
		pScene = pSceneNext;
	}
}

