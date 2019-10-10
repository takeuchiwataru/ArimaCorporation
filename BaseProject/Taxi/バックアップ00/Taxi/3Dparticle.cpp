//=============================================================================
//
// 3Dパーティクル処理 [3Dparticle.cpp]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#include "3Dparticle.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "shadermgr.h"
#include "rotationshader.h"
#include "game.h"
#include "gamecamera.h"

//=============================================================================
// 生成処理
//=============================================================================
C3DParticle * C3DParticle::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//インスタンスの生成
	C3DParticle * p3DParticle;
	p3DParticle = new C3DParticle;

	//初期化処理
	p3DParticle->Init();

	//設定処理
	p3DParticle->Set(pos, size);

	return p3DParticle;
}

//=============================================================================
// コンストラクタ
//=============================================================================
C3DParticle::C3DParticle() : CScene(4, OBJTYPE_EFFECT)
{
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0, 0, 0);
	m_rot = D3DXVECTOR3(0, 0, 0);
}

//=============================================================================
// コンストラクタ
//=============================================================================
C3DParticle::C3DParticle(int nPriority, CScene::OBJTYPE obj) : CScene(nPriority, obj){}

//=============================================================================
// デストラクタ
//=============================================================================
C3DParticle::~C3DParticle(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT C3DParticle::Init(void)
{
	//変数宣言
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pRotationShader = NULL;
	m_pos = D3DXVECTOR3(0, 0, 0);
	m_rot = D3DXVECTOR3(0, 0, 0);
	m_Division = D3DXVECTOR2(1, 1);

	//レンダリングクラスを取得
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D * pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-50.0f, 50.0f, 50.0f);
	pVtx[1].pos = D3DXVECTOR3(50.0f, 0.0f, 50.0f);
	pVtx[2].pos = D3DXVECTOR3(-50.0f, 50.0f, -50.0f);
	pVtx[3].pos = D3DXVECTOR3(50.0f, 0.0f, -50.0f);

	pVtx[0].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//UV値の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	//シェーダーの取得
	/*m_pRotationShader = NULL;
	m_pRotationShader = CGame::GetShaderMgr()->GetRotatonShader();
	m_pShader = m_pRotationShader->GetShader();*/

	/*if (m_pRotationShader != NULL)
	{
		m_pRotationShader->SetCol_a(1.0f);
	}*/
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void C3DParticle::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture = NULL;
	}

	//死亡フラグを立てる
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void C3DParticle::Update(void)
{
	//クリッピング処理の追加
	CGameCamera * pGameCamera = CGame::GetGameCamera();
	SetDraw(pGameCamera->ClippingPos(m_pos));
}

//=============================================================================
// 描画処理
//=============================================================================
void C3DParticle::Draw(void)
{
	if (m_pVtxBuff != NULL)
	{
		//レンダリングクラスを取得
		CRenderer * pRenderer = NULL;
		pRenderer = CManager::GetRenderer();

		//デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

		D3DXMATRIX		  mtxRot, mtxTrans;			// 計算用マトリックス

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);			//両面カリング				
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);					// ライティングを無効にする

		//シェーダーの設定
		//if(m_pRotationShader != NULL){ m_pRotationShader->SetBegin(); }
		//m_pShader->SetTechnique("StandardShader");
		//m_pShader->Begin(0, 0);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		//各マトリックスをシェーダーに渡す
		if (m_pRotationShader != NULL)
		{
			//CGameCamera * pCamera = CGame::GetGameCamera();
			//D3DXMATRIX mtxView = pCamera->GetViewMatrix();
			//D3DXMATRIX mtxProjection = pCamera->GetProjectionMatrix();
			//m_pRotationShader->SetMatrix(&mtxRot, &m_mtxWorld, &mtxView, &mtxProjection);
			//m_pShader->SetMatrix("g_mtxRot", &mtxRot);					//向き
			//m_pShader->SetMatrix("g_mtxWorld", &m_mtxWorld);				//ワールドマトリックス
			//m_pShader->SetMatrix("g_mtxView", &mtxView);				//ビューマトリックス
			//m_pShader->SetMatrix("g_mtxProjection", &mtxProjection);	//プロジェクションマトリックス

			//シェーダーの描画を始める
			//m_pRotationShader->SetBeginPass();
			//シェーダーの描画を始める
			//m_pShader->BeginPass(0);
		}

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, m_pTexture);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			2);

		//シェーダーの書き込みを終了する
		//if (m_pRotationShader != NULL) { m_pRotationShader->EndShader(); }
		//シェーダーの書き込みを終了する
		//m_pShader->EndPass();
		//m_pShader->End();

		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);			//カリング
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);					// ライティングを無効にする
	}
}

//=============================================================================
// 設定処理
//=============================================================================
void C3DParticle::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	m_pos = pos;
	m_size = size;

	//変数宣言
	D3DXVECTOR3 Vec0[2] = {};
	D3DXVECTOR3 Vec1[2] = {};

	VERTEX_3D * pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-size.x, 0.0f, size.z);
	pVtx[1].pos = D3DXVECTOR3(size.x, 0.0f, size.z);
	pVtx[2].pos = D3DXVECTOR3(-size.x, 0.0f, -size.z);
	pVtx[3].pos = D3DXVECTOR3(size.x, 0.0f, -size.z);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 色の設定
//=============================================================================
void C3DParticle::SetColor(D3DXCOLOR col)
{
	VERTEX_3D * pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	//色のα値を設定する
	m_pRotationShader->SetCol_a(col.a);
}

//=============================================================================
// テクスチャの更新
//=============================================================================
void C3DParticle::SetMoveTex(float fMoveX, float fMoveY)
{
	VERTEX_3D * pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f * m_Division.x + fMoveX, 0.0f * m_Division.y + fMoveY);
	pVtx[1].tex = D3DXVECTOR2(1.0f * m_Division.x + fMoveX, 0.0f * m_Division.y + fMoveY);
	pVtx[2].tex = D3DXVECTOR2(0.0f * m_Division.x + fMoveX, 1.0f * m_Division.y + fMoveY);
	pVtx[3].tex = D3DXVECTOR2(1.0f * m_Division.x + fMoveX, 1.0f * m_Division.y + fMoveY);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// UVの設定
//=============================================================================
void C3DParticle::SetUV(int nWidth, int nHight)
{
	VERTEX_3D * pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float PosX = 1.0f * m_Division.x * nWidth;
	float PosY = 1.0f * m_Division.x * nHight;

	//頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f * m_Division.x + PosX, 0.0f * m_Division.y + PosY);
	pVtx[1].tex = D3DXVECTOR2(1.0f * m_Division.x + PosX, 0.0f * m_Division.y + PosY);
	pVtx[2].tex = D3DXVECTOR2(0.0f * m_Division.x + PosX, 1.0f * m_Division.y + PosY);
	pVtx[3].tex = D3DXVECTOR2(1.0f * m_Division.x + PosX, 1.0f * m_Division.y + PosY);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}