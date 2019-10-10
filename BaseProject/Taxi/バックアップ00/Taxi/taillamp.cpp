//=============================================================================
//
// テールランプ処理 [taillamp.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "taillamp.h"
#include "manager.h"
#include "renderer.h"

//==================================
// マクロ定義
//==================================
#define TAILLAMP_TEXTURE	"data\\TEXTURE\\effect000.jpg"			// 読み込むテクスチャファイル名
#define TAILLAMP_INTERVAL	(60)			// 点滅の間隔

//==================================
// 静的メンバ変数宣言
//==================================

//==================================
// 生成処理
//==================================
CTailLamp* CTailLamp::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXMATRIX *pMtxParent)
{
	CTailLamp *pTailLamp;

	pTailLamp = new CTailLamp;	// メモリを確保

	if (pTailLamp != NULL)
	{// メモリ確保成功
		pTailLamp->m_pos = pos;
		pTailLamp->m_size = size;
		pTailLamp->m_pMtxParent = pMtxParent;
		pTailLamp->Init();
	}

	return pTailLamp;
}

//=========================================
// コンストラクタ
//=========================================
CTailLamp::CTailLamp()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_type = TYPE_NONE;
	m_nCntFlash = 0;
}

//=========================================
// デストラクタ
//=========================================
CTailLamp::~CTailLamp()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CTailLamp::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_col = D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f);

	// テクスチャ生成
	D3DXCreateTextureFromFile(pDevice, TAILLAMP_TEXTURE, &m_pTexture);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_3D,
								D3DPOOL_MANAGED,
								&m_pVtxBuff,
								NULL);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, m_size.y, 0.0f) * 0.5f;
	pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, 0.0f) * 0.5f;
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, 0.0f) * 0.5f;
	pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y, 0.0f) * 0.5f;

	//法線ベクトル
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//頂点カラー
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファのアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CTailLamp::Uninit(void)
{
	if (NULL != m_pVtxBuff)
	{// 頂点情報の破棄
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// 自分を破棄
	CScene::Release();
}

//=========================================
// 更新処理
//=========================================
void CTailLamp::Update(void)
{
	if (TYPE_BACK == m_type)
	{// バックのとき
		m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		if (0 == m_nCntFlash)
		{// 光らせる
			SetColor(D3DXCOLOR(m_col.r, m_col.g, m_col.b, 1.0f));
		}
		else if (TAILLAMP_INTERVAL == m_nCntFlash)
		{// 光らせない
			SetColor(D3DXCOLOR(m_col.r, m_col.g, m_col.b, 0.0f));
		}

		m_nCntFlash = (m_nCntFlash + 1) % (TAILLAMP_INTERVAL * 2);
	}
	else if (TYPE_BREAKING == m_type)
	{// ブレーキのとき
		m_col = D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f);
		m_nCntFlash = 0;
		SetColor(D3DXCOLOR(m_col.r, m_col.g, m_col.b, 1.0f));
	}
	else if (TYPE_HAZARD_LAMP == m_type)
	{// ハザードランプのとき
		m_col = D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f);
		if (0 == m_nCntFlash)
		{// 光らせる
			SetColor(D3DXCOLOR(m_col.r, m_col.g, m_col.b, 1.0f));
		}
		else if (TAILLAMP_INTERVAL / 2 == m_nCntFlash)
		{// 光らせない
			SetColor(D3DXCOLOR(m_col.r, m_col.g, m_col.b, 0.0f));
		}

		m_nCntFlash = (m_nCntFlash + 1) % TAILLAMP_INTERVAL;
	}
	else if (TYPE_NONE == m_type)
	{// テールランプを使わないとき
		m_nCntFlash = 0;
		SetColor(D3DXCOLOR(m_col.r, m_col.g, m_col.b, 0.0f));
	}
}

//=========================================
// 描画処理
//=========================================
void CTailLamp::Draw(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス

	//アルファテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// 親のマトリックスと掛け合わせる
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, m_pMtxParent);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//アルファテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, D3DZB_FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

//=========================================
// 色の設定
//=========================================
void CTailLamp::SetColor(D3DXCOLOR col)
{
	m_col = col;

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラー
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//頂点バッファのアンロックする
	m_pVtxBuff->Unlock();
}

//=========================================
// 種類の設定
//=========================================
void CTailLamp::SetType(TYPE type)
{
	// カウントを０に戻す
	m_nCntFlash = 0;

	// タイプを入れる
	m_type = type;
}