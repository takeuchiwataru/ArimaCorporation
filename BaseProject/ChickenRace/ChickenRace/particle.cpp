//=============================================================================
//
// パーティクルの処理 [particle.cpp]
// Author : 長山拓実
//
//=============================================================================
#include "particle.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define PARTICLE_TEXTURE_0		"data\\TEXTURE\\game\\ParticleEffect\\Star.png"		//読み込むテクスチャファイル
#define PARTICLE_TEXTURE_1		"data\\TEXTURE\\game\\ParticleEffect\\.png"			//読み込むテクスチャファイル

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9	CParticle::m_apTexture[MAX_EFFECT_TEX] = {};

//=============================================================================
// パーティクルの生成処理
//=============================================================================
CParticle *CParticle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, D3DXVECTOR2 size, int nLife, PARTICLE_TYPE type)
{
	CParticle *pParticle = NULL;

	if (pParticle == NULL)
	{
		// パーティクルクラスの生成
		pParticle = new CParticle;

		if (pParticle != NULL)
		{
			pParticle->Init();							// 初期化処理
			pParticle->m_move = move;					// 移動量の設定
			pParticle->SetPosSize(pos, size);			// 幅の設定
			pParticle->m_Size = size;
			pParticle->m_nLife = nLife;					// ライフの設定
			pParticle->BindTexture(m_apTexture[type]);	// テクスチャの設定
			pParticle->SetColor(col);					// 色の設定
		}
	}
	return pParticle;
}

//=============================================================================
// パーティクルクラスのコンストラクタ
//=============================================================================
CParticle::CParticle()
{
	// 値をクリア
	m_nLife = 0;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// パーティクルクラスのデストラクタ
//=============================================================================
CParticle::~CParticle()
{
}

//=============================================================================
// パーティクルの初期化処理
//=============================================================================
HRESULT CParticle::Init(void)
{
	// 値をクリア
	m_nLife = 0;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 2Dオブジェクト初期化処理
	CBillBoord::Init();

	// オブジェクトの種類の設定
	SetObjType(CScene::OBJTYPE_PARTICLE);

	return S_OK;
}

//=============================================================================
// パーティクルの終了処理
//=============================================================================
void CParticle::Uninit(void)
{
	// 2Dオブジェクト終了処理
	CBillBoord::Uninit();
}

//=============================================================================
// パーティクルの更新処理
//=============================================================================
void CParticle::Update(void)
{
	// 位置を取得
	D3DXVECTOR3 pos;
	pos = CBillBoord::GetPosition();

	//// 頂点バッファを取得
	//LPDIRECT3DVERTEXBUFFER9 VtxBuff;
	//VtxBuff = CBillBoord::GetVtxBuff();

	//VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	//// 頂点バッファをロックし、頂点データへのポインタを取得
	//VtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	
	// 一定時間経過
	m_nLife--;

	m_Size.x -= 0.1f;
	m_Size.y -= 0.1f;

	if (m_Size.x <= 0 && m_Size.y <= 0)
	{
		m_Size.x = 0.0f;
		m_Size.y = 0.0f;
	}

	pos -= m_move;

	switch (m_ParticleType)
	{
		// 通常状態
	case TYPE_NORMAL:
		break;
		// 減速状態
	case TYPE_DOWN:
		// 重力加算
		m_move.y += 0.5f;

		//CPlayer **pPlayer = NULL;
		//pPlayer = CGame::GetPlayer();

		//pos.x = pPlayer[m_nPlayer]->GetPos().x;
		//pos.z = pPlayer[m_nPlayer]->GetPos().z;

		break;
	}

	// 頂点バッファをアンロック
	//VtxBuff->Unlock();

	// 位置の設定
	CBillBoord::SetPosition(pos);

	// 幅の設定
	CBillBoord::SetPosSize(pos, m_Size);

	if (m_nLife <= 0)
	{
		// 消す
		Uninit();
	}
}

//=============================================================================
// パーティクルの描画処理
//=============================================================================
void CParticle::Draw(void)
{
	// レンダラーを取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = NULL;

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 深度バッファを有効にする
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// 2Dオブジェクト描画処理
	CBillBoord::Draw();

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// 深度バッファを無効にする
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

//===============================================================================
// テクスチャの読み込み
//===============================================================================
HRESULT CParticle::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	for (int nCntTex = 0; nCntTex < MAX_EFFECT_TEX; nCntTex++)
	{
		// テクスチャの生成
		m_apTexture[nCntTex] = NULL;
	}

	D3DXCreateTextureFromFile(pDevice, PARTICLE_TEXTURE_0, &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, PARTICLE_TEXTURE_1, &m_apTexture[1]);

	return S_OK;
}

//===============================================================================
// テクスチャの破棄
//===============================================================================
void CParticle::UnLoad(void)
{
	for (int nCntTex = 0; nCntTex < MAX_EFFECT_TEX; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}