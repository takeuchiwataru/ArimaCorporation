//=============================================================================
//
// 火花のエフェクト処理 [sparkeffect.cpp]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#include "sparkeffect .h"
#include "player.h"
#include "game.h"
#include "particlebillboad.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define SIZE			(D3DXVECTOR2(4.0f, 1.0f))				// サイズ
#define LIFE			(20)									// ライフ
#define CUT_COLA		(0.1f)									// 色の変化量
#define FILENAME_TEX	("data\\TEXTURE\\effect000.jpg")		// 読み込むテクスチャファイル名
#define COL_YELLOW		(D3DXCOLOR(1.0f, 1.0f, 0.25f, 1.0f))	// 黄色の設定
#define COL_RED			(D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f))		// 赤色の設定
#define GRAVITY			(-0.3f)									// 重力

//=============================================================================
// 前方宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CSparkEffect::m_pTexture = NULL;

//=============================================================================
// 生成処理
//=============================================================================
CSparkEffect * CSparkEffect::Create(D3DXVECTOR3 Pos)
{
	CSparkEffect * pSmokeEffect = new CSparkEffect;

	pSmokeEffect->SetPos(Pos);				//位置の設定
	pSmokeEffect->Init();					//初期化処理

	return pSmokeEffect;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CSparkEffect::CSparkEffect(){}

//=============================================================================
// デストラクタ
//=============================================================================
CSparkEffect::~CSparkEffect(){}

//=============================================================================
// テクスチャ読み込み処理
//=============================================================================
void CSparkEffect::LoadTex(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXCreateTextureFromFile(pDevice, FILENAME_TEX, &m_pTexture);
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CSparkEffect::Unload(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSparkEffect::Init(void)
{
	//変数宣言
	float fAngle = (float)(rand() % 314) * 0.01f;
	float fHight = (float)((rand() % 3) + 2);
	float fSpeed = (float)((rand() % 13) + 1) * 0.1f;

	//変数の初期化
	CParticleBillboad::SetSize(SIZE);			//サイズの設定
	CParticleBillboad::Init();					//初期化処理
	CParticleBillboad::BindTexture(m_pTexture);	//テクスチャの設定
	CParticleBillboad::SetColor(COL_YELLOW);	//色の設定
	m_nLife = LIFE;
	m_fCol_A = 1.0f;

	//移動量を求める
	m_move.x = sinf(fAngle) * fSpeed;
	m_move.z = cosf(fAngle) * fSpeed;
	m_move.y = fHight;

	//色の変化量を求める
	m_fCol_G = COL_YELLOW.g;
	m_fCol_B = COL_YELLOW.b;
	m_fChangeCol_G = (m_fCol_G - COL_RED.g) / (float)m_nLife;
	m_fChangeCol_B = (m_fCol_B - COL_RED.b) / (float)m_nLife;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSparkEffect::Uninit(void)
{
	CParticleBillboad::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CSparkEffect::Update(void)
{
	CParticleBillboad::Update();

	//位置の取得
	D3DXVECTOR3 pos = CParticleBillboad::GetPos();

	//位置の更新
	pos += m_move;
	CParticleBillboad::SetPos(pos);

	//重力の加算
	m_move.y += GRAVITY;

	//表示時間の減算
	m_nLife--;

	if (m_nLife < 0)
	{//ライフが0以下で透明にしていく
		m_fCol_A -= CUT_COLA;
	}

	//色の設定
	m_fCol_G -= m_fChangeCol_G;
	m_fCol_B -= m_fChangeCol_B;
	CParticleBillboad::SetColor(D3DXCOLOR(COL_RED.r, m_fCol_G, m_fCol_B, m_fCol_A));

	//透明になったら破棄
	if (m_fCol_A < 0.0f) { Uninit(); }
}

//=============================================================================
// 描画処理
//=============================================================================
void CSparkEffect::Draw(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// Zテスト処理を無効にする
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	CParticleBillboad::Draw();

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// Zテスト処理をデフォルトにする
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}