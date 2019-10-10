//=============================================================================
//
// コイン追加の処理 [addcoin.cpp]
// Author : 有馬武志
//
//=============================================================================
#include "addcoin.h"
#include "scene2D.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "billboord.h"
#include "game.h"
#include "player.h"
#include "arrow.h"
#include "texture.h"
#include "tutorial.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define VECTORE_ZERO		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
#define LIFE				(35)
#define SPEED				(10.0f)
#define POS					(D3DXVECTOR3(700.0f, 400.0f, 0.0f))
#define AIM_POS				(D3DXVECTOR3(1000.0f, 200.0f, 0.0f))
#define MAX_PATTERN			(4)	
#define UV					(0.25f)
#define LOAD_TEX			("data\\TEXTURE\\game\\Score\\Coin.png")
#define SIZE				(D3DXVECTOR2(25.0f, 25.0f))

//*****************************************************************************
// 前方宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CAddCoin::m_pTexture = NULL;

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CAddCoin::CAddCoin(int nPriorityv) : CScene2D(nPriorityv, CScene::OBJTYPE_LOGO){}

//===============================================================================
//　デストラクタ
//===============================================================================
CAddCoin::~CAddCoin(){}

//===============================================================================
//　クリエイト
//===============================================================================
CAddCoin * CAddCoin::Create(void)
{
	CAddCoin *pLogo = NULL;

	//NULLチェック
	if (pLogo == NULL)
	{//メモリの動的確保
		pLogo = new CAddCoin(6);

		if (pLogo != NULL)
		{
			pLogo->Init();
		}
	}

	return pLogo;
}

//===============================================================================
//　テクスチャ読み込み処理
//===============================================================================
void CAddCoin::LoadTex(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, LOAD_TEX, &m_pTexture);
}

//===============================================================================
//　テクスチャの破棄処理
//===============================================================================
void CAddCoin::UnloadTex(void)
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
HRESULT CAddCoin::Init(void)
{
	//変数の初期化
	m_nPattern = 0;
	m_nCountTime = 0;
	m_nLife = LIFE;

	//位置設定
	float fRand = (float)((rand() % 50) + 10);
	D3DXVECTOR3 pos = D3DXVECTOR3(POS.x + fRand, POS.y + fRand, 0.0f);

	CScene2D::Init();
	CScene2D::BindTexture(m_pTexture);		//テクスチャの割り当て
	CScene2D::SetPosSize(pos, SIZE);
	CScene2D::SetUVWidth(UV, m_nPattern);

	//距離を求める
	D3DXVECTOR3 Distance = pos - AIM_POS;
	m_fAngle = atan2f(Distance.x, Distance.y);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CAddCoin::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CAddCoin::Update(void)
{
	//テクスチャのUV更新
	m_nCountTime++;

	if ((m_nCountTime % 6) == 0)
	{
		m_nPattern++;

		if ((m_nPattern % MAX_PATTERN) == 0) { m_nPattern = 0; }

		//UV値の設定
		CScene2D::SetUVWidth(UV, m_nPattern);
	}

	//位置の取得
	D3DXVECTOR3 pos;
	pos = GetPosition();

	//移動量の更新
	m_move.x = sinf(m_fAngle) * -SPEED;
	m_move.y = cosf(m_fAngle) * -SPEED;

	//位置の更新
	pos += m_move;
	CScene2D::SetPosSize(pos, SIZE);

	CScene2D::Update();

	//表示時間の更新
	m_nLife--;

	//破棄する
	if (m_nLife < 0) { Uninit(); }
}
//=============================================================================
// 描画処理
//=============================================================================
void CAddCoin::Draw(void)
{
	CScene2D::Draw();
}
