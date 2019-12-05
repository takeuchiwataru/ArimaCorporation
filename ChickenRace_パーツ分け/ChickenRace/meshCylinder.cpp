//=============================================================================
//
// メッシュシリンダー処理 [meshCylinder.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "meshCylinder.h"
#include "manager.h"
#include "meshfield.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//===============================================================================
// 生成処理
//===============================================================================
CMeshCylinder * CMeshCylinder::Create(D3DXVECTOR3 pos, int nLap, int nStage, float fRadius, float fHeight, D3DXCOLOR col)
{
	CMeshCylinder *pMeshCylinder = NULL;

	pMeshCylinder = new CMeshCylinder;	//メモリの動的確保

	if (pMeshCylinder != NULL)
	{// メモリ確保成功
		pMeshCylinder->m_pos = pos;									// 位置の設定
		pMeshCylinder->m_col = col;									// 色の設定
		pMeshCylinder->MakeVertex(nLap, nStage, fRadius, fHeight);	// 頂点の作成
		pMeshCylinder->Init();										// 初期化処理
	}

	return pMeshCylinder;
}

//===============================================================================
// デフォルトコンストラクタ
//===============================================================================
CMeshCylinder::CMeshCylinder():CScene(4, CScene::OBJTYPE_CYLINDER)
{
	m_nNumVtx = 0;
	m_nNumPolygon = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nLap = 0;
	m_nStage = 0;
	m_fRadius = 0.0f;
	m_fHeight = 0.0f;
	m_bAlong = false;
	m_fFloatHeight = 0.0f;
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//===============================================================================
// デストラクタ
//===============================================================================
CMeshCylinder::~CMeshCylinder(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMeshCylinder::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMeshCylinder::Uninit(void)
{
	if (NULL != m_pVtxBuff)
	{// 頂点バッファの破棄
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	if (NULL != m_pIdxBuff)
	{// インデックスバッファの破棄
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// 自分を破棄
	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CMeshCylinder::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CMeshCylinder::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ワールドマトリックスの保存用
	D3DXMATRIX mtxRot, mtxTrans;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// ライト無効
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// カリングなし
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVtx, 0, m_nNumPolygon);

	// 裏面をカリング
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// ライト有効
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=========================================================================================================================
// 頂点情報の作成
//=========================================================================================================================
void CMeshCylinder::MakeVertex(int nLap, int nStage, float fRadius, float fHeight)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	int nCntLap;
	int nCntStage;
	int nCntIdx;

	// 各種値の設定
	m_nLap = nLap;
	m_nStage = nStage;
	m_fRadius = fRadius;
	m_fHeight = fHeight;

	m_nNumVtx = (m_nLap + 1) * (m_nStage + 1);						// 頂点数
	m_nNumPolygon = m_nLap * m_nStage * 2 + (4 * (m_nStage - 1));	// ポリゴン数

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// インデックスバッファを生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * (m_nNumPolygon + 2),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,			// 16ビットのデータを確保
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fAngle = 1.0f / (m_nLap / 2);	// 一周の角度

	for (nCntStage = 0; nCntStage <= m_nStage; nCntStage++)
	{
		for (nCntLap = 0; nCntLap <= m_nLap; nCntLap++)
		{
			// 頂点の座標
			pVtx[0].pos.x = sinf(D3DX_PI * (0.0f + (fAngle * nCntLap))) * m_fRadius;
			if (m_bAlong)
			{// 地面に沿わせる
				pVtx[0].pos.y = GetHeightAlongField(pVtx[0].pos + m_pos) + m_fFloatHeight + (m_fHeight - ((nCntStage * m_fHeight) / m_nStage));
			}
			else
			{// 地面に添わせない
				pVtx[0].pos.y = m_fHeight - ((nCntStage * m_fHeight) / m_nStage) * 1.0f;
			}
			pVtx[0].pos.z = cosf(D3DX_PI * (0.0f + (fAngle * nCntLap))) * m_fRadius;

			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 頂点カラー
			pVtx[0].col = m_col;

			//テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(nCntLap * 1.0f, nCntStage * 1.0f);

			pVtx += 1;
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	WORD *pIdx;		// インデックスデータへのポインタ

	// インデックスバッファをロックし、インデックスデータへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (nCntIdx = 0; nCntIdx <= m_nLap; nCntIdx++)
	{// 頂点の順番を設定
		pIdx[0] = (m_nLap + 1) + nCntIdx;
		pIdx[1] = 0 + nCntIdx;

		pIdx += 2;
	}

	// 頂点バッファをアンロック
	m_pIdxBuff->Unlock();
}

//=========================================================================================================================
// 頂点情報を作り直す
//=========================================================================================================================
void CMeshCylinder::ReMakeVertex(void)
{
	if (NULL != m_pVtxBuff)
	{// 頂点バッファの破棄
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	if (NULL != m_pIdxBuff)
	{// インデックスバッファの破棄
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// カウンタ
	int nCntLap;
	int nCntStage;
	int nCntIdx;

	m_nNumVtx = (m_nLap + 1) * (m_nStage + 1);						// 頂点数
	m_nNumPolygon = m_nLap * m_nStage * 2 + (4 * (m_nStage - 1));	// ポリゴン数

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// インデックスバッファを生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * (m_nNumPolygon + 2),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,			// 16ビットのデータを確保
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	const float fAngle = 1.0f / (m_nLap / 2);	// 一周の角度

	for (nCntStage = 0; nCntStage <= m_nStage; nCntStage++)
	{
		for (nCntLap = 0; nCntLap <= m_nLap; nCntLap++)
		{
			// 頂点の座標
			pVtx[0].pos.x = sinf(D3DX_PI * (0.0f + (fAngle * nCntLap))) * m_fRadius;
			if (m_bAlong)
			{// 地面に沿わせる
				pVtx[0].pos.y = GetHeightAlongField(pVtx[0].pos) + m_fFloatHeight + (m_fHeight - ((nCntStage * m_fHeight) / m_nStage));
			}
			else
			{// 地面に添わせない
				pVtx[0].pos.y = m_fHeight - ((nCntStage * m_fHeight) / m_nStage) * 1.0f;
			}

			pVtx[0].pos.z = cosf(D3DX_PI * (0.0f + (fAngle * nCntLap))) * m_fRadius;

			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 頂点カラー
			pVtx[0].col = m_col;

			//テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(nCntLap * 1.0f, nCntStage * 1.0f);

			pVtx += 1;
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	WORD *pIdx;		// インデックスデータへのポインタ

	// インデックスバッファをロックし、インデックスデータへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (nCntStage = 0, nCntIdx = 0; nCntStage < m_nStage; nCntStage++)
	{
		for (nCntLap = 0; nCntLap <= m_nLap; nCntLap++, nCntIdx++)
		{
			pIdx[0] = (m_nLap + 1) + nCntIdx;
			pIdx[1] = 0 + nCntIdx;

			pIdx += 2;

			if (nCntStage < m_nStage - 1 && nCntLap == m_nLap)
			{// 折り返し時
				pIdx[0] = 0 + nCntIdx;
				pIdx[1] = (m_nLap + 1) + nCntIdx + 1;

				pIdx += 2;
			}
		}
	}

	// 頂点バッファをアンロック
	m_pIdxBuff->Unlock();
}

//=========================================================================================================================
// 地面に沿わせるかどうか
//=========================================================================================================================
void CMeshCylinder::AlongField(bool bAlong, float fFloatHeight)
{
	if (bAlong != m_bAlong)
	{// フラグが変更されたとき
		m_bAlong = bAlong;
		m_fFloatHeight = fFloatHeight;

		VERTEX_3D *pVtx;	// 頂点情報へのポインタ

		// 頂点バッファをロックし、頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
		{
			pVtx[nCntVtx].pos.y = 0.0f;
		}

		for (int nCntStage = 0; nCntStage <= m_nStage; nCntStage++)
		{
			for (int nCntLap = 0; nCntLap <= m_nLap; nCntLap++)
			{
				if (m_bAlong)
				{// 地面に沿わせる
					pVtx[0].pos.y = GetHeightAlongField(pVtx[0].pos) + m_fFloatHeight + (m_fHeight - ((nCntStage * m_fHeight) / m_nStage));
				}
				else
				{// 地面に沿わせない
					pVtx[0].pos.y = m_fHeight - ((nCntStage * m_fHeight) / m_nStage) * 1.0f;
				}

				pVtx += 1;	// ポインタを進める
			}
		}

		// 頂点バッファをアンロック
		m_pVtxBuff->Unlock();
	}
}

//=========================================================================================================================
// 地面に沿った高さの取得
//=========================================================================================================================
float CMeshCylinder::GetHeightAlongField(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 pos1 = pos;

	//プライオリティーチェック
	CScene *pScene = CScene::GetTop(MESH_PRIOTITY);

	//NULLチェック
	while (pScene != NULL)
	{
		//UpdateでUninitされてしまう場合　Nextが消える可能性があるからNextにデータを残しておく
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetObjType() == OBJTYPE_MESH)
		{//タイプが地面だったら
			CMeshField *pField = (CMeshField*)pScene;
			if (pField->OnField(pos1 + D3DXVECTOR3(0.0f, 50.0f, 0.0f), 50.0f))
			{// 傾斜の計算
				pField->GetHeight(pos1);
				break;
			}
		}
		//Nextに次のSceneを入れる
		pScene = pSceneNext;
	}

	return pos1.y;
}

//=========================================================================================================================
// 色の設定
//=========================================================================================================================
void CMeshCylinder::SetColor(D3DXCOLOR col)
{
	m_col = col;

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
	{// 頂点数分ループ
		pVtx[0].col = m_col;	// 頂点カラー
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}