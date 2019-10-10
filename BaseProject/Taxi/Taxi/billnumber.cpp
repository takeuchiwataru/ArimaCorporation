//=============================================================================
//
// ビルボード数字処理 [number.cpp]
// Author : 有馬　武志
//
//=============================================================================
#include "billnumber.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"

//=============================================================================
//	静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9	CBillNumber::m_pTexture = NULL;

//=============================================================================
//	マクロ定義
//=============================================================================
#define BILLNUMBER_TEX			"data/TEXTURE/number000.png"	// 数字テクスチャ
#define BILLNUMBER_SIZE_X		(5.0f)							// 数字の横幅
#define BILLNUMBER_SIZE_Y		(15.0f)							// 数字の縦幅

//=============================================================================
// コンストラクタ
//=============================================================================
CBillNumber::CBillNumber()
{
	m_pVtxBuff = NULL;
	m_nNumber = 0;
}
//=============================================================================
// デストラクタ
//=============================================================================
CBillNumber::~CBillNumber()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBillNumber::Init(D3DXVECTOR3 pos)
{
	// デバイス取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	
	//ゲームの情報
	CManager::MODE pMode = CManager::GetMode();

	// 頂点情報の作成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL
	);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	m_pos = pos;

	//頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (pMode == CManager::MODE_GAME)
	{
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - BILLNUMBER_SIZE_X, m_pos.y + BILLNUMBER_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + BILLNUMBER_SIZE_X, m_pos.y + BILLNUMBER_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - BILLNUMBER_SIZE_X, m_pos.y - BILLNUMBER_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + BILLNUMBER_SIZE_X, m_pos.y - BILLNUMBER_SIZE_Y, 0.0f);
	}

	for (int nCntrhw = 0; nCntrhw < 4; nCntrhw++)
	{
		pVtx[nCntrhw].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	}

	for (int nCntCol = 0; nCntCol < 4; nCntCol++)
	{
		pVtx[nCntCol].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBillNumber::Uninit(void)
{
	// 頂点バッファ破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CBillNumber::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CBillNumber::Draw(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 	pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxView, mtxRot, mtxTrans;				//計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//逆行列を設定
	m_mtxWorld._11 = mtxView._11;
	m_mtxWorld._12 = mtxView._21;
	m_mtxWorld._13 = mtxView._31;
	m_mtxWorld._21 = mtxView._12;
	m_mtxWorld._22 = mtxView._22;
	m_mtxWorld._23 = mtxView._32;
	m_mtxWorld._31 = mtxView._13;
	m_mtxWorld._32 = mtxView._23;
	m_mtxWorld._33 = mtxView._33;

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// ナンバーの設定
//=============================================================================
void CBillNumber::SetBillNumber(int nNumber)
{
	VERTEX_3D *pVtx;				//頂点情報へのポインタ

	//頂点バッファをロックし頂点データのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//入れ替える
	m_nNumber = nNumber;

	////テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2((m_nNumber % 10) * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((m_nNumber % 10) * 0.1f + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2((m_nNumber % 10) * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((m_nNumber % 10) * 0.1f + 0.1f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//	読み込み処理
//=============================================================================
HRESULT CBillNumber::Load(void)
{
	// デバイス取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャ生成
	D3DXCreateTextureFromFile(pDevice, BILLNUMBER_TEX, &m_pTexture);

	return S_OK;
}

//=============================================================================
//	開放処理
//=============================================================================
void CBillNumber::Unload(void)
{
	//テクスチャ破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
//===============================================================================
//  カラーの設定処理
//===============================================================================
void CBillNumber::SetColor(D3DXCOLOR *Color)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = Color[0];
	pVtx[1].col = Color[1];
	pVtx[2].col = Color[2];
	pVtx[3].col = Color[3];

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}