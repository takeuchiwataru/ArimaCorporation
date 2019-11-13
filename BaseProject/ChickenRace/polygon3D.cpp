//=============================================================================
//
// 地面３Dの処理 [polygon3D.cpp]
// Author : 有馬武志
//
//=============================================================================
#include "scene.h"
#include "polygon3D.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_NAME_1		"data\\TEXTURE\\A.png"				//読み込むテクスチャファイル

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
D3DXVECTOR3 CPolygon3D::m_nor[4] = {};
//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CPolygon3D::CPolygon3D()
{	
	m_pVtxBuff = NULL;
	m_pTexture = NULL;
	m_Pos = D3DXVECTOR3(0.0f,0.0f,0.0f);	//位置
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向き
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//動き
	m_bUse = false;							//種類
}
//===============================================================================
//　デストラクタ
//===============================================================================
CPolygon3D::~CPolygon3D()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPolygon3D::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ポリゴンの位置を設定
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//向き
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//動き
	m_bUse = false;									//使用しているかどうか

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

	//頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点設定
	pVtx[0].pos = D3DXVECTOR3(m_Pos.x - 300.0f, 0.0f, m_Pos.z - 300.0f);
	pVtx[1].pos = D3DXVECTOR3(m_Pos.x - 300.0f, 200.0f, m_Pos.z + 300.0f);
	pVtx[2].pos = D3DXVECTOR3(m_Pos.x + 300.0f, 200.0f, m_Pos.z - 300.0f);
	pVtx[3].pos = D3DXVECTOR3(m_Pos.x + 300.0f, 0.0f, m_Pos.z + 300.0f);
	//法線ベクトル
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
	//テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファのアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPolygon3D::Uninit(void)
{
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();

	//テクスチャーの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
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
void CPolygon3D::Update(void)
{
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();

	//外積とはプレイヤーが右にいるか左にいるか　を外積で出している。
	D3DXVECTOR3 vec0;
	D3DXVECTOR3 vec1;
	D3DXVECTOR3	nor0, nor1;					//法線

	//============================
	//		法線を求める
	//============================

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//
	// 頂点0番目
	//
	//ベクトル計算式
	vec0 = pVtx[1].pos - pVtx[0].pos;
	//目的引く自分
	vec1 = pVtx[2].pos - pVtx[0].pos;

	//外積計算
	D3DXVec3Cross(&nor0, &vec0, &vec1);
	//正規化
	D3DXVec3Normalize(&nor0, &nor0);

	//
	// 頂点3番目
	//
	//ベクトル計算式
	vec0 = pVtx[2].pos - pVtx[3].pos;//vec0 から　pos  //vec0　からの nor から yをだす
	//目的引く自分
	vec1 = pVtx[1].pos - pVtx[3].pos;

	//外積計算
	D3DXVec3Cross(&nor1, &vec0, &vec1);
	//正規化
	D3DXVec3Normalize(&nor1, &nor1);

	//頂点設定
	m_nor[0] = pVtx[0].nor = nor0;
	m_nor[1] = pVtx[1].nor = ((nor0 + nor1) / 2);
	m_nor[2] = pVtx[2].nor = ((nor0 + nor1) / 2);
	m_nor[3] = pVtx[3].nor = nor1;

	//頂点バッファのアンロックする
	m_pVtxBuff->Unlock();

	//デバック用
	CDebugProc::Print("nor[%.1f][%.1f][%.1f]\n", nor0.x, nor0.y, nor0.z);
	CDebugProc::Print("nor[%.1f][%.1f][%.1f]\n", nor1.x, nor1.y, nor1.z);

	//============================
	//     デバック用生成
	//============================
	if (pCInputKeyBoard->GetKeyboardTrigger(DIK_2) == true)
	{
		Uninit();
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void CPolygon3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxView;		//計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorldPolygon);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);

	D3DXMatrixMultiply(&m_mtxWorldPolygon, &m_mtxWorldPolygon, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);

	D3DXMatrixMultiply(&m_mtxWorldPolygon, &m_mtxWorldPolygon, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorldPolygon);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	pDevice->SetTexture(0, NULL);
}
//===============================================================================
//　クリエイト
//===============================================================================
CPolygon3D * CPolygon3D::Create(void)
{
	CPolygon3D *pScene3D = NULL;

	//NULLチェック
	if (pScene3D == NULL)
	{//メモリの動的確保

		pScene3D = new CPolygon3D;

		if (pScene3D != NULL)
		{
			//オブジェクトクラスの生成
			pScene3D->Init();
		}
	}

	return pScene3D;
}
//===============================================================================
//　プレイヤーのpos.yを内積を利用して出す
//===============================================================================
float CPolygon3D::GetHeight(D3DXVECTOR3 pos)
{
	//外積とはプレイヤーが右にいるか左にいるか　を外積で出している。
	D3DXVECTOR3 vec0;
	D3DXVECTOR3 vec1;
	D3DXVECTOR3 vec2;
	int			nNumber;

	float		fVec;

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//============================
	//		三角形１こめ
	//============================
	//ベクトル計算式 目的 - 現在
	vec0 = pVtx[1].pos - pVtx[2].pos;
	//ベクトル計算式 自分の位置　- 現在
	vec1 = D3DXVECTOR3(pos) - pVtx[2].pos;
	//右にいるか左にいるか -左　+右
	//外積用計算式
	fVec = (vec0.x * vec1.z) - (vec0.z * vec1.x);

	if (fVec >= 0)
	{
		CDebugProc::Print("△１範囲 : TURE\n");
		vec0 = (pos - pVtx[0].pos);
		nNumber = 0;
	}
	else if (fVec <= 0)
	{
		CDebugProc::Print("△２範囲 : TURE\n");
		vec0 = (pos - pVtx[3].pos);
		nNumber = 3;
	}

	//
	//内積
	//
	//内積の出し方
	((vec0.x * pVtx[nNumber].nor.x) + (vec0.y * pVtx[nNumber].nor.y) + (vec0.z * pVtx[nNumber].nor.z));

	vec0.y = (-(vec0.x * pVtx[nNumber].nor.x) - (vec0.z * pVtx[nNumber].nor.z)) / pVtx[nNumber].nor.y;

	pos.y = vec0.y;

	//デバック用
	//CDebugProc::Print("内積vec0[%.1f][%.1f][%.1f]\n", vec0.x, vec0.y, vec0.z);

	//CDebugProc::Print("pos.y = %.1f\n", pos.y);

	//頂点バッファのアンロックする
	m_pVtxBuff->Unlock();

	return pos.y;
}
//===============================================================================
//　polygon3Dの法線をあげる
//===============================================================================
void CPolygon3D::GetNor(D3DXVECTOR3 *nor)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	nor[0] = pVtx[0].nor;
	nor[1] = pVtx[1].nor;
	nor[2] = pVtx[2].nor;
	nor[3] = pVtx[3].nor;

	//頂点バッファのアンロックする
	m_pVtxBuff->Unlock();
}
