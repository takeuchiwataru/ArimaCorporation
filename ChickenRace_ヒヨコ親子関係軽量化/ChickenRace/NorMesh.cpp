//=============================================================================
//
// mesh処理 [mesh.cpp]
// Author : Ryo Sugimoto
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "NorMesh.h"
#include "renderer.h"
#include "manager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//==================================================================================================//
//    * メッシュの生成関数 *
//==================================================================================================//
bool	CNorMesh::Create(CNorMesh *&pNorMesh)
{
	pNorMesh = NULL;
	pNorMesh = new CNorMesh;
	if (pNorMesh != NULL) { pNorMesh->Init(); return true; }
	//MessageBox(0, "生成できない！", "", MB_OK);

	return false;
}
//==================================================================================================//
//    * メッシュのセット関数 *
//==================================================================================================//
void CNorMesh::Set(TYPE type, D3DXVECTOR3 pos, int nVertical, int nCorss, bool bMesh)	//ゲージセット処理
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得

	m_Type = type;
	m_pos = pos;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nVertical = nVertical;
	m_nCross = nCorss;
	m_bMesh = bMesh;
	m_nNumVertex = (nVertical + 1) * (nCorss + 1);		//頂点数
	m_nNumIdx = (nVertical * nCorss) * 2 + (nVertical - 1) * 4 + 2;				//インデックス数
	m_nNumPolygon = (nVertical * nCorss) * 2 + (nVertical - 1) * 4;		//ポリゴン数

																		//頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIdx, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIdxBuff, NULL);
	VERTEX_3D *pVtx;	//頂点情報へのポインタ
	WORD * pIdx;		//インデックスデータへのポインタ
	float fUV_X = 0.25f, fUV_Y = 0.2f;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	//頂点バッファをロックし頂点データへのポインタを取得
												//頂点、法線、カラー、テクスチャの頂点設定
	for (int nCntVertical = 0; nCntVertical < nVertical + 1; nCntVertical++)
	{//縦軸
		for (int nCntCorss = 0; nCntCorss < nCorss + 1; nCntCorss++)
		{//横軸
		 //頂点座標
			switch (type)
			{
			case TYPE_FIELD:
				pVtx[0].pos = D3DXVECTOR3(-100.0f * 2 + 100.0f * nCntCorss, 0.0f, 100.0f * 2 - 100.0f * nCntVertical);
				break;
			case TYPE_WALL:
				pVtx[0].pos = D3DXVECTOR3(-100.0f * nCntCorss, 100.0f * nCntVertical, 100.0f * nCntCorss);
				break;
			case TYPE_CEILING:
				pVtx[0].pos = D3DXVECTOR3(-100.0f * 2 + 100.0f * nCntCorss, 100.0f - rand() % 10 * 10.0f, 100.0f * 2 + 100.0f * nCntVertical);
				if (nCntCorss == 0 || nCntCorss == nCorss || nCntVertical == 0 || nCntVertical == nVertical)
				{
					pVtx[0].pos.y = 0.0f;
				}
				break;
			case TYPE_CYLINDER:
				fUV_Y = 0.5f;
				pVtx[0].pos = D3DXVECTOR3(
					sinf((float)(nCntCorss + 1) / (float)(nCorss + 2)) * 10.0f
					, nCntVertical * 10.0f
					, cosf((float)(nCntCorss + 1) / (float)(nCorss + 2)) * 10.0f);
				break;
			}


			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[0].tex = D3DXVECTOR2(nCntCorss * fUV_X, nCntVertical * fUV_Y);
			pVtx += 1;
		}
	}

	m_pVtxBuff->Unlock();	//頂点バッファのアンロックする

							//インデックスバッファのロックし、インデックスデータへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// インデックスの設定
	for (int nCount_yz = 0; nCount_yz < nVertical; nCount_yz++)
	{// yzをカウント
		for (int nCount_x = 0; nCount_x < nCorss + 1; nCount_x++)
		{// xをカウント
		 // インデックス情報の設定
			pIdx[0] = (WORD)(((nCount_yz + 1) * (nCorss + 1)) + nCount_x);
			pIdx[1] = (WORD)(((nCount_yz) * (nCorss + 1)) + nCount_x);

			pIdx += 2;			// インデックスバッファを進める

			if (nVertical > 1 && nCount_yz != nVertical - 1 && nCount_x == nCorss)
			{// 末端に到達したら
			 // インデックス情報の設定（縮退ポリゴン）
				pIdx[0] = (WORD)(((nCount_yz) * (nCorss + 1)) + nCount_x);
				pIdx[1] = (WORD)(((nCount_yz + 2) * (nCorss + 1)));

				pIdx += 2;		// インデックスバッファを進める
			}
		}
	}
	//インデックスのバッファのアンロック
	m_pIdxBuff->Unlock();

	SetMtx();
}
//==================================================================================================//
//    * 法線のセット処理 *
//==================================================================================================//
void	CNorMesh::SetNor(void)
{
	VERTEX_3D *pVtx;	//頂点情報へのポインタ
	D3DXVECTOR3 vec0, vec1;
	D3DXVECTOR3 nor[2];
	int		nCntHit = 0;
	int nCntVertical, nCntCorss;
	VERTEX_3D *pVtxLU, *pVtxLD, *pVtxRU, *pVtxRD;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	//頂点バッファをロックし頂点データへのポインタを取得
	for (nCntVertical = 0; nCntVertical < m_nVertical + 1; nCntVertical++)
	{//縦軸
		for (nCntCorss = 0; nCntCorss < m_nCross + 1; nCntCorss++)
		{//横軸
			pVtx[nCntVertical * (m_nCross + 1) + nCntCorss].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}

	for (int nCntVertical = 0; nCntVertical < m_nVertical; nCntVertical++)
	{//縦軸
		for (int nCntCorss = 0; nCntCorss < m_nCross; nCntCorss++)
		{//横軸
		 //頂点取得
			pVtxLU = &pVtx[(nCntVertical) * (m_nCross + 1) + nCntCorss];
			pVtxLD = &pVtx[(nCntVertical + 1) * (m_nCross + 1) + nCntCorss];
			pVtxRU = &pVtx[(nCntVertical) * (m_nCross + 1) + nCntCorss + 1];
			pVtxRD = &pVtx[(nCntVertical + 1) * (m_nCross + 1) + nCntCorss + 1];
			//法線加算
			PlusNor(pVtxLD, pVtxLU, pVtxRD);
			PlusNor(pVtxRD, pVtxRD, pVtxLU);
		}
	}

	for (nCntVertical = 0; nCntVertical < m_nVertical + 1; nCntVertical++)
	{//縦軸
		for (nCntCorss = 0; nCntCorss < m_nCross + 1; nCntCorss++)
		{//横軸
			if (nCntVertical == 0)
			{//上端
				if (nCntCorss == 0 || nCntCorss == m_nCross) { pVtx[nCntVertical * (m_nCross + 1) + nCntCorss].nor /= 2.0f - (nCntCorss / m_nCross); }
				else { pVtx[nCntVertical * (m_nCross + 1) + nCntCorss].nor /= 3.0f; }
			}
			else if (nCntVertical == m_nVertical)
			{//下端
				if (nCntCorss == 0 || nCntCorss == m_nCross) { pVtx[nCntVertical * (m_nCross + 1) + nCntCorss].nor /= 1.0f + (nCntCorss / m_nCross); }
				else { pVtx[nCntVertical * (m_nCross + 1) + nCntCorss].nor /= 3.0f; }
			}
			else
			{//中間
				if (nCntCorss == 0 || nCntCorss == m_nCross) { pVtx[nCntVertical * (m_nCross + 1) + nCntCorss].nor /= 3.0f; }
				else { pVtx[nCntVertical * (m_nCross + 1) + nCntCorss].nor /= 6.0f; }
			}
		}
	}
	m_pVtxBuff->Unlock();	//頂点バッファのアンロックする
}
//==================================================================================================//
//    * マトリックス計算処理 *
//==================================================================================================//
void	CNorMesh::SetMtx(void)
{
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

												// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
}
//==================================================================================================//
//    * 法線の加算処理 *
//==================================================================================================//
void CNorMesh::PlusNor(VERTEX_3D *pVtx, VERTEX_3D *pVtxL, VERTEX_3D *pVtxR)	//法線のセット処理
{
	D3DXVECTOR3 vec0, vec1;
	D3DXVECTOR3 nor;

	//法線計算
	vec0 = pVtxL->pos - pVtx->pos;
	vec1 = pVtxR->pos - pVtx->pos;
	D3DXVec3Cross(&nor, (m_bMesh ? &vec0 : &vec1), (m_bMesh ? &vec1 : &vec0));	//外積計算
	D3DXVec3Normalize(&nor, &nor);												//正規化にする

																				//加算
	pVtx->nor += nor;
	pVtxL->nor += nor;
	pVtxR->nor += nor;
}
//=============================================================================
// メッシュの初期化処理
//=============================================================================
HRESULT  CNorMesh::Init(void)
{
	m_pTex = NULL;
	m_pVtxBuff = NULL;
	m_pIdxBuff = NULL;
	m_bMesh = true;
	return S_OK;
}
//=============================================================================
// メッシュの終了処理
//=============================================================================
void  CNorMesh::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	//インデックスバッファの解放
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	delete this;
}
//=============================================================================
// メッシュの描画処理
//=============================================================================
void  CNorMesh::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	if (m_pTex != NULL) { pDevice->SetTexture(0, m_pTex); }
	else { pDevice->SetTexture(0, NULL); }

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVertex, 0, m_nNumPolygon);

	pDevice->SetTexture(0, NULL);
}
