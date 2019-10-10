//=============================================================================
//
// 軌跡の処理 [meshorbit.cpp]
// Author : 有馬武志
//
//=============================================================================
#include "scene.h"
#include "meshorbit.h"
#include "manager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_NAME_1		"data\\TEXTURE\\gradation005.jpg"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CMeshOrbit::m_pTexture[MAX_MESHORBIT_TEXTURE] = {};

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CMeshOrbit::CMeshOrbit() : CScene(2, CScene::OBJTYPE_MESHORBIT)
{
	m_pVtxBuff = NULL;								// 頂点バッファへのポインタ
	m_pIdxBuff = NULL;								// インデックスへのポインタ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向き
	m_nType = 0;									// 種類
}
//===============================================================================
//　デストラクタ
//===============================================================================
CMeshOrbit::~CMeshOrbit()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMeshOrbit::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//初期化処理
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向き
	m_nType = 0;									// 種類
	m_bOrbitOnOff = false;							// 軌跡の表示のONOFF

	//両端のオフセットの初期化
	m_aOffSet[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_aOffSet[1] = D3DXVECTOR3(0.0f, 50.0f,0.0f);

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * MESHORBIT_X,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL
	);

	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;	

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMesh = 0; nCntMesh < MESHORBIT_X; nCntMesh+= 2)
	{
		// 頂点の座標
		pVtx[nCntMesh].pos = D3DXVECTOR3(0.0f, 0.0f, nCntMesh * MESH_ORBITWIDTH);
		pVtx[nCntMesh + 1].pos = D3DXVECTOR3(0.0f, MESH_ORBITDEPTH, nCntMesh * MESH_ORBITWIDTH);
	}

	for (int nCntMesh = 0; nCntMesh < MESHORBIT_X; nCntMesh++)
	{
		// 法線の設定
		pVtx[nCntMesh].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 頂点カラー
		pVtx[nCntMesh].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		
		//テクスチャ座標
		pVtx[nCntMesh].tex = D3DXVECTOR2(nCntMesh * 1.0f, nCntMesh * 1.0f);
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMeshOrbit::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CMeshOrbit::Update(void)
{
	CDebugProc::Print("発生！\n");
}
//=============================================================================
// 描画処理
//=============================================================================
void CMeshOrbit::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;	

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 計算用マトリックス
	D3DXMATRIX mtxTrans;

	for (int nCount = (MESHORBIT_X / 2) - 1; nCount > 0; nCount--)
	{//一番後ろから入れ替えていく
		pVtx[nCount * 2] = pVtx[(nCount * 2) - 2];
		pVtx[nCount * 2 + 1] = pVtx[(nCount * 2) - 2 + 1];
	}

	for (int nCount = 0; nCount < 2; nCount++)
	{//ベクトルをトランスフォーム
		D3DXVec3TransformCoord(&pVtx[nCount].pos, &m_aOffSet[nCount], m_pMtxParent);
	}

	//アルファテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//アルファテスト処理
	//デフォルトの場合FALSEになっているからTRUEにする
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// カリングしない
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture[0]);

	//攻撃するときのみ描画する
	//if (m_bOrbitOnOff == true)
	{// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, MESHORBIT_X - 2);
	}

	// 裏面をカリング
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//デフォルトの場合FALSEになっているからTRUEにする
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}
//===============================================================================
//　クリエイト
//===============================================================================
CMeshOrbit * CMeshOrbit::Create()
{
	CMeshOrbit *pMeshOrbit = NULL;

	//NULLチェック
	if (pMeshOrbit == NULL)
	{//メモリの動的確保

		pMeshOrbit = new CMeshOrbit;

		if (pMeshOrbit != NULL)
		{
			//オブジェクトクラスの生成
			pMeshOrbit->Init();
		}
	}

	return pMeshOrbit;
}
//===============================================================================
// テクスチャの読み込み
//===============================================================================
HRESULT CMeshOrbit::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_1, &m_pTexture[0]);

	return S_OK;
}
//===============================================================================
// テクスチャの破棄
//===============================================================================
void CMeshOrbit::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_MESHORBIT_TEXTURE; nCount++)
	{
		//テクスチャーの破棄
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}
//===============================================================================
// 共有テクスチャの割り当て
//===============================================================================
void CMeshOrbit::BindTexture(LPDIRECT3DTEXTURE9 Texture)
{
	m_pTexture[0] = Texture;
}
//===============================================================================
//　位置の代入
//===============================================================================
void CMeshOrbit::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}
//スキンメッシュ
//クォータニオン
//スプライン
//ステンシルシャドウ
//ステンシルミラー
//フィードバックエフェクト
