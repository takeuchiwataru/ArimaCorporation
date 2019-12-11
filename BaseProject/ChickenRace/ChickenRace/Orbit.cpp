//=============================================================================
//
// mesh処理 [mesh.cpp]
// Author : Ryo Sugimoto
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "Orbit.h"
#include "renderer.h"
#include "manager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BULOCK_X					(20.0f)											//ブロックの奥行
#define BULOCK_Y					(400.0f)											//ブロックの横行
#define BULOCK_Z					(20.0f)											//ブロックの横行
#define VERTEX						((VERTICAL + 1) * (CORSS + 1))						//指定した縦と横+１の値
#define IDX							((VERTICAL * CORSS) * 2 + (VERTICAL - 1) * 4 + 2)	//インデックスの式
#define POLYGON						((VERTICAL * CORSS) * 2 + (VERTICAL - 1) * 4)		//ポリゴン数の式
#define WALL_CURVE					(3.0f)		//壁の曲がり具合
#define FIELD_SIZE					(100.0f)		//一マスの大きさ
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//==================================================================================================//
//    * 生成関数 *
//==================================================================================================//
CEfcOrbit	*CEfcOrbit::Create(void)
{
	CEfcOrbit *pOrbit = NULL;	pOrbit = new CEfcOrbit;
	if (pOrbit != NULL) { pOrbit->Init(); }
	return pOrbit;
}
//=============================================================================
// メッシュオービット設定処理
//=============================================================================
CEfcOrbit	*CEfcOrbit::Set(D3DXMATRIX &mtxWorld, D3DXVECTOR3 length0, D3DXVECTOR3 length1, bool &bDelete, D3DXCOLOR col, int nUV, int nHeight, int nReset)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得

																		//代入
	m_pmtxWorld = &mtxWorld;	m_bDelete = &bDelete;
	m_col = col;
	//m_pTex = Load(tex);
	m_nUV = nUV;				m_nHeight = nHeight;
	m_fEnd = 0.0f;

	m_nCross = 10;										//横の長さ
	m_nNumPolygon = (1 * m_nCross) * 2 + (1 - 1) * 4;	//面の数
	m_fPlusCol = 1.0f / m_nCross;
	m_fUV = 0.0f;
	m_nReset = nReset;
	m_nInitReset = nReset;
	m_DrawType = C2D::DRAW_TYPE_ADD;

	for (int nCount = 0; nCount < 2; nCount++)
	{//マトリックス計算
		if (nCount == 1) { length0 = length1; }
		// 自身の位置を計算
		D3DXMatrixTranslation(&m_mtxWorld[nCount], length0.x, length0.y, length0.z);
	}

	//頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * ((1 + 1) * (m_nCross + 1)), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	//メッシュ設定
	//CMesh::Set(CMesh::TYPE_ORBIT, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 100, true);
	SetObjType(OBJTYPE_ORBIT);
	SetVtx(true);
	ReSet();

	return this;
}
//=============================================================================
//初期化処理
//=============================================================================
HRESULT	CEfcOrbit::Init(void)
{
	m_Type = TYPE_FADE;
	m_pTex = NULL;
	return S_OK;
}
//=============================================================================
//終了処理
//=============================================================================
void	CEfcOrbit::Uninit(void)
{
	CScene::Release();
}
//=============================================================================
//更新処理
//=============================================================================
void	CEfcOrbit::Update(void)
{
	if (m_nReset > 1) { m_nReset--; }
	else
	{
		SwapVtx();		//頂点の入れ替え

						//最新の頂点を代入
		if (m_pmtxWorld != NULL) { SetVtx(false); }
		if (m_bDelete == NULL)
		{
			m_fEnd += m_fPlusCol;
			if (m_Type == TYPE_FADE)
			{
				m_fPlusCol *= 0.95f;
				if (m_fPlusCol < 0.0001f) { Uninit(); return; }
			}
			else
			{
				if (m_fEnd >= 1.0f) { Uninit(); return; }
			}
		}
	}
}
//=============================================================================
//描画処理
//=============================================================================
void	CEfcOrbit::Draw(void)
{
	//デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxView;	//計算用マトリックス
	D3DXMATRIX	mtxWorld;					//ワールドマトリックス

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);    // 裏面をカリング

	if (C2D::DrawPrepare(GetDrawType(), pDevice)) { return; }

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTex);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, m_nNumPolygon);
	pDevice->SetTexture(0, NULL);

	C2D::DrawPrepare(C2D::DRAW_TYPE_NORMAL, pDevice);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);    // 裏面をカリング
}
//=============================================================================
//最新の頂点を計算して代入
//=============================================================================
void	CEfcOrbit::SetVtx(bool bAll)
{
	if (m_pmtxWorld == NULL) { return; }
	if (m_nReset == 1) { m_nReset--; SetVtx(true); }
	else if (m_nReset > 0) { m_nReset--; return; }
	D3DXMATRIX mtxWK;

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	//頂点バッファをロックし頂点データへのポインタを取得

	for (int nCount = 0; nCount < 2; nCount++)
	{
		// 計算用マトリックスに代入
		mtxWK = m_mtxWorld[nCount];

		// 親の角度を用いて移動を反映
		D3DXMatrixMultiply(&mtxWK, &mtxWK, m_pmtxWorld);

		pVtx[nCount].pos = D3DXVECTOR3(mtxWK._41, mtxWK._42, mtxWK._43);
		pVtx[nCount].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, (bAll ? 0.0f : m_col.a));
		pVtx[nCount].tex = D3DXVECTOR2(m_fUV * 0.1f, (float)m_nUV / (float)m_nHeight + nCount * (1.0f / (float)m_nHeight));
		pVtx[nCount].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		m_fUV += 0.1f;
	}

	if (bAll)
	{//全ての頂点に代入
		for (int nCount = 1; nCount < m_nCross + 1; nCount++)
		{//横の分割数分回転
			pVtx[(nCount) * 2] = pVtx[0];
			pVtx[(nCount) * 2 + 1] = pVtx[1];
		}
	}

	m_pVtxBuff->Unlock();	//頂点バッファのアンロックする
}
//=============================================================================
//頂点の入れ替え
//=============================================================================
void	CEfcOrbit::SwapVtx(void)
{
	D3DXCOLOR				col;

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	//頂点バッファをロックし頂点データへのポインタを取得

	for (int nCount = m_nCross - 1; nCount >= 0; nCount--)
	{//横の分割数分回転

		switch (m_Type)
		{
		case TYPE_NONE://透明度薄く
			col = pVtx[(nCount * 2)].col;		pVtx[(nCount * 2)].col = D3DXCOLOR(col.r, col.g, col.b, col.a - m_fPlusCol);
			col = pVtx[(nCount * 2) + 1].col;	pVtx[(nCount * 2) + 1].col = D3DXCOLOR(col.r, col.g, col.b, col.a - m_fPlusCol);
			break;
		case TYPE_FADE:
			if (nCount <= 3)
			{
				col = pVtx[(nCount * 2)].col;		pVtx[(nCount * 2)].col = D3DXCOLOR(col.r, col.g, col.b, col.a + m_fPlusCol * 1.5f);
				col = pVtx[(nCount * 2) + 1].col;	pVtx[(nCount * 2) + 1].col = D3DXCOLOR(col.r, col.g, col.b, col.a + m_fPlusCol * 1.5f);
			}
			else
			{
				col = pVtx[(nCount * 2)].col;		pVtx[(nCount * 2)].col = D3DXCOLOR(col.r, col.g, col.b, col.a - m_fPlusCol);
				col = pVtx[(nCount * 2) + 1].col;	pVtx[(nCount * 2) + 1].col = D3DXCOLOR(col.r, col.g, col.b, col.a - m_fPlusCol);
			}
			break;
		}


		//入れ替え
		pVtx[(nCount * 2) + 2] = pVtx[(nCount * 2)];
		pVtx[(nCount * 2) + 3] = pVtx[(nCount * 2) + 1];
	}

	m_pVtxBuff->Unlock();	//頂点バッファのアンロックする
}

//=============================================================================
//頂点のリセット
//=============================================================================
void	CEfcOrbit::ReSetAll(void)
{
	CScene *pScene;

	pScene = CScene::GetTop(ORBIT_PRIORITY);
	while (pScene != NULL)
	{//終わるまで
		if (pScene->GetObjType() == OBJTYPE_ORBIT)
		{//メッシュ発見
			if (((CEfcOrbit*)pScene)->m_nInitReset == 0) { pScene->Uninit(); }
			else { ((CEfcOrbit*)pScene)->ReSet(); }
		}
		pScene = pScene->GetNext();
	}
}
//=============================================================================
//削除確認と破棄
//=============================================================================
void	CEfcOrbit::Delete(bool bEnd)
{
	CScene *pScene;

	pScene = CScene::GetTop(ORBIT_PRIORITY);
	while (pScene != NULL)
	{//終わるまで
		if (!pScene->GetDeath())
		{//削除していないなら
			if (pScene->GetObjType() == OBJTYPE_ORBIT)
			{//メッシュ発見
				if (((CEfcOrbit*)pScene)->m_bDelete != NULL)
				{//終了なら破棄
					if (((CEfcOrbit*)pScene)->m_bDelete[0])
					{
						if (bEnd) { ((CEfcOrbit*)pScene)->m_pmtxWorld = NULL; }
						((CEfcOrbit*)pScene)->m_bDelete = NULL;
					}
				}
			}
		}
		pScene = pScene->GetNext();
	}
}
//=============================================================================
//削除確認と破棄
//=============================================================================
void	CEfcOrbit::DeleteAll(void)
{
	CScene *pScene;

	pScene = CScene::GetTop(ORBIT_PRIORITY);
	while (pScene != NULL)
	{//終わるまで
		if (!pScene->GetDeath())
		{//削除していないなら
			if (pScene->GetObjType() == OBJTYPE_ORBIT)
			{//メッシュ発見
				((CEfcOrbit*)pScene)->m_pmtxWorld = NULL;
				((CEfcOrbit*)pScene)->m_bDelete = NULL;
			}
		}
		pScene = pScene->GetNext();
	}
}
//=============================================================================
//頂点のリセット
//=============================================================================
void	CEfcOrbit::ReSet(void)
{
	m_fUV = 0.0f;
	m_nReset = m_nInitReset;
	SetVtx(true);
}
