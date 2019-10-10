//=============================================================================
//
// 雪のエフェクト処理 [.cpp]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#include "grasseffect.h"
#include "player.h"
#include "game.h"
#include "particlebillboad.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define SIZE			(D3DXVECTOR2(10.0f, 10.0f))	// サイズ
#define LIFE			(10)						// ライフ
#define ADD_SIZE		(1.0f)						// サイズの加算
#define CUT_COLA		(0.1f)						// 色の変化量
#define FILENAME_TEX	("data\\TEXTURE\\game\\Particle\\Grass.png")
#define GRAVITY			(-0.15f)					//重力

//=============================================================================
// 前方宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CGrassEffect::m_pTexture = NULL;

//=============================================================================
// 生成処理
//=============================================================================
CGrassEffect * CGrassEffect::Create(D3DXVECTOR3 Pos, int nNumPattern)
{
	CGrassEffect * pGrassEffect = new CGrassEffect;

	pGrassEffect->SetPos(Pos);				//位置の設定
	pGrassEffect->SetNumPattern(nNumPattern);	//UVパターンの番号を設定
	pGrassEffect->Init();					//初期化処理

	return pGrassEffect;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CGrassEffect::CGrassEffect(){}

//=============================================================================
// デストラクタ
//=============================================================================
CGrassEffect::~CGrassEffect(){}

//=============================================================================
// テクスチャ読み込み処理
//=============================================================================
void CGrassEffect::LoadTex(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXCreateTextureFromFile(pDevice, FILENAME_TEX, &m_pTexture);
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CGrassEffect::Unload(void)
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
HRESULT CGrassEffect::Init(void)
{
	//変数宣言
	float fAngle = (float)(rand() % 10) * 0.1f;
	float fHight = (float)((rand() % 4) + 2);
	float fSpeed = (float)((rand() % 15) + 3) * 0.1f;

	int nRandSize = (rand() % 6) + 1;
	CParticleBillboad::SetSize(D3DXVECTOR2((float)nRandSize, (float)nRandSize));			//サイズの設定
	CParticleBillboad::Init();					//初期化処理
	CParticleBillboad::BindTexture(m_pTexture);	//テクスチャの設定
	CParticleBillboad::SetUV(D3DXVECTOR2((0.25f * m_nNumPattern), 0.0f), D3DXVECTOR2(((0.25f * m_nNumPattern) + 0.25f), 1.0f));		//UV設定
	m_nLife = LIFE;
	m_fCol_A = 1.0f;

	//移動量を求める
	m_move.x = sinf(D3DX_PI * fAngle) * fSpeed;
	m_move.z = cosf(D3DX_PI * fAngle) * fSpeed;
	m_move.y = fHight;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGrassEffect::Uninit(void)
{
	CParticleBillboad::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CGrassEffect::Update(void)
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
		CParticleBillboad::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fCol_A));

		//透明になったら破棄
		if (m_fCol_A < 0.0f) { Uninit(); }
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CGrassEffect::Draw(void)
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