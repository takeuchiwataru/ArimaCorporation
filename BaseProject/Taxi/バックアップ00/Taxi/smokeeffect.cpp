//=============================================================================
//
// 雪のエフェクト処理 [.cpp]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#include "smokeeffect.h"
#include "player.h"
#include "game.h"
#include "particlebillboad.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define SIZE			(D3DXVECTOR2(10.0f, 10.0f))	// サイズ
#define LIFE			(15)						// ライフ
#define ADD_SIZE		(1.0f)						// サイズの加算
#define CUT_COLA		(0.1f)						// 色の変化量
#define FILENAME_TEX	("data\\TEXTURE\\game\\Particle\\smoke.png")
#define COLOR			(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f))

//=============================================================================
// 前方宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CSmokeEffect::m_pTexture = NULL;

//=============================================================================
// 生成処理
//=============================================================================
CSmokeEffect * CSmokeEffect::Create(D3DXVECTOR3 Pos)
{
	CSmokeEffect * pSmokeEffect = new CSmokeEffect;

	pSmokeEffect->SetPos(Pos);				//位置の設定
	pSmokeEffect->Init();					//初期化処理

	return pSmokeEffect;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CSmokeEffect::CSmokeEffect(){}

//=============================================================================
// デストラクタ
//=============================================================================
CSmokeEffect::~CSmokeEffect(){}

//=============================================================================
// テクスチャ読み込み処理
//=============================================================================
void CSmokeEffect::LoadTex(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXCreateTextureFromFile(pDevice, FILENAME_TEX, &m_pTexture);
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CSmokeEffect::Unload(void)
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
HRESULT CSmokeEffect::Init(void)
{
	int nRandSize = (rand() % 10) + 5;
	CParticleBillboad::SetSize(D3DXVECTOR2((float)nRandSize, (float)nRandSize));			//サイズの設定
	CParticleBillboad::Init();					//初期化処理
	CParticleBillboad::BindTexture(m_pTexture);	//テクスチャの設定
	CParticleBillboad::SetColor(COLOR);			//色の設定
	m_nLife = LIFE;
	m_fCol_A = 1.0f;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSmokeEffect::Uninit(void)
{
	CParticleBillboad::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CSmokeEffect::Update(void)
{
	CParticleBillboad::Update();

	//サイズを取得・更新
	D3DXVECTOR2 size = CParticleBillboad::GetSize();
	size.x += ADD_SIZE;
	size.y += ADD_SIZE;

	//サイズを再設定
	CParticleBillboad::RemakeSize(size);

	//表示時間の減算
	m_nLife--;

	if (m_nLife < 0)
	{//ライフが0以下で透明にしていく
		m_fCol_A -= CUT_COLA;
		CParticleBillboad::SetColor(D3DXCOLOR(COLOR.r, COLOR.g, COLOR.b, m_fCol_A));

		//透明になったら破棄
		if (m_fCol_A < 0.0f) { Uninit(); }
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CSmokeEffect::Draw(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// Zテスト処理を無効にする
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	CParticleBillboad::Draw();

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Zテスト処理をデフォルトにする
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}