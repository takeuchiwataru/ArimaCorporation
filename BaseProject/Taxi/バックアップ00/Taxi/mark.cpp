//=============================================================================
//
// 客の頭上に出るマークの処理 [mark.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "mark.h"
#include "manager.h"
#include "renderer.h"
#include "customer.h"
#include "game.h"
#include "tutorial.h"
#include "player.h"

//==================================
// マクロ定義
//==================================
#define MARK_TEXTURE	"data/TEXTURE/game/CustomerMark/CustomerMark.png"			//読み込むテクスチャファイル名
#define MARK_MODEL		"data/MODEL/お客さんマーク/CustomerMark.x"
#define MARK_LENGTH		(12500)	//描画範囲
#define UPDATE_LENGTH	(7500)	//更新範囲


//==================================
// 静的メンバ変数宣言
//==================================
LPD3DXMESH CMark::m_pLoadMesh = NULL;
LPD3DXBUFFER CMark::m_pLoadBuffMat = NULL;
DWORD CMark::m_nLoadNumMat = 0;

//==================================
// 生成処理
//==================================
CMark* CMark::Create(D3DXVECTOR3 pos)
{
	CMark *pMark;

	pMark = new CMark;	// メモリを確保

	if (pMark != NULL)
	{// メモリ確保成功
		pMark->m_pos = pos;
		pMark->Init();
	}

	return pMark;
}

//=========================================
// コンストラクタ
//=========================================
CMark::CMark()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_fCntPos = 0.0f;
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
}

//=========================================
// デストラクタ
//=========================================
CMark::~CMark(){}

//=========================================
// モデルの読み込み処理
//=========================================
void CMark::LoadModel(void)
{
	//レンダリングクラスを取得
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// Xファイルの読み込み
	D3DXLoadMeshFromX(MARK_MODEL,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pLoadBuffMat,
		NULL,
		&m_nLoadNumMat,
		&m_pLoadMesh);
}

//=========================================
// モデルの破棄
//=========================================
void CMark::UnloadModel(void)
{
	//メッシュの破棄
	if (m_pLoadMesh != NULL)
	{
		m_pLoadMesh->Release();
		m_pLoadMesh = NULL;
	}

	//マテリアルの破棄
	if (m_pLoadBuffMat != NULL)
	{
		m_pLoadBuffMat->Release();
		m_pLoadBuffMat = NULL;
	}

	//マテリアルの最大数を初期化
	m_nLoadNumMat = 0;
}

//=========================================
// 初期化処理
//=========================================
HRESULT CMark::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャ生成
	D3DXCreateTextureFromFile(pDevice, MARK_TEXTURE, &m_pTexture);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	m_size = D3DXVECTOR2(50.0f, 50.0f);

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
void CMark::Uninit(void)
{
	// 自分を破棄
	CScene::Release();
}

//=========================================
// 更新処理
//=========================================
void CMark::Update(void)
{
	//プレイヤーとの距離を求める
	CManager::MODE mode = CManager::GetMode();
	D3DXVECTOR3 PlayerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (mode == CManager::MODE_GAME)
	{
		PlayerPos = CGame::GetPlayer()->GetPos();
	}
	else if (mode == CManager::MODE_TUTORIAL)
	{
		PlayerPos = CTutorial::GetPlayer()->GetPos();
	}

	D3DXVECTOR3 Distance = PlayerPos - m_pos;
	float fLength = sqrtf((Distance.x * Distance.x) + (Distance.z * Distance.z));

	if (fLength < UPDATE_LENGTH)
	{//更新範囲
	 // 高さを変える
		m_fCntPos += 0.05f;
		m_pos.y += sinf(m_fCntPos) * 1.0f;

		// モデルを回転させる
		m_rot.y += 0.02f;
	}
	else if (fLength < MARK_LENGTH)
	{
		SetDraw(true);	//描画状態にする
	}
	else
	{//描画しない状態にする
		SetDraw(false);
	}
}

//=========================================
// 描画処理
//=========================================
void CMark::Draw(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxView, mtxRot, mtxTrans;	// 計算用マトリックス

	//アルファテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//モデルの描画
	DrawModel();

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

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//アルファテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, D3DZB_FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

//=========================================
// モデルの描画処理
//=========================================
void CMark::DrawModel(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;					//現在のマテリアルを保存
	D3DXMATERIAL *pMat;						//マテリアルデータへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pLoadBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nLoadNumMat; nCntMat++)
	{
		//マテリアルの設定
		pMat[nCntMat].MatD3D.Diffuse = m_col;
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャをNULLにする
		pDevice->SetTexture(0, NULL);

		//オブジェクト(パーツ)の描画
		m_pLoadMesh->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//=========================================
// 色の設定
//=========================================
void CMark::SetColor(D3DXCOLOR col)
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
