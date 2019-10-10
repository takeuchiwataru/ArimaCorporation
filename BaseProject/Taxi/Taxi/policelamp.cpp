//=============================================================================
//
// 警察のランプ処理 [policelanmp.cpp]
// Author : 阿部　佑太朗
//
//=============================================================================
#include "policelamp.h"
#include "manager.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define POLICELAMP_TEX	"data\\TEXTURE\\effect000.jpg"			// テクスチャの名前

//=============================================================================
// 静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9 CPoliceLamp::m_pTexture[MAX_POLICELAMP_TEX] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CPoliceLamp::CPoliceLamp() : CScene3D(POLICELAMP_PRIOTITY, CScene::OBJTYPE_3DPOLYGON)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CPoliceLamp::~CPoliceLamp()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPoliceLamp::Init()
{
	//初期化処理
	CScene3D::Init();

	//種類の設定
	SetTypeNumber(TYPE_NONE);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPoliceLamp::Uninit(void)
{
	CScene3D::Uninit();
}
//=============================================================================
// 更新処理
//=============================================================================
void CPoliceLamp::Update(void)
{
	CScene3D::Update();
}
//=============================================================================
// 描画処理
//=============================================================================
void CPoliceLamp::Draw(void)
{

	// デバイス取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//アルファテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CScene3D::Draw();

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//アルファテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, D3DZB_FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}
//=============================================================================
// 生成処理
//=============================================================================
CPoliceLamp * CPoliceLamp::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	CPoliceLamp *pPoliceLamp = NULL;
	//NULLチェック
	if (pPoliceLamp == NULL)
	{//メモリの動的確保

		pPoliceLamp = new CPoliceLamp;

		if (pPoliceLamp != NULL)
		{
			//初期化処理
			pPoliceLamp->Init();
			//位置の設定
			pPoliceLamp->SetPosSize(pos, size);
			//テクスチャの設定
			pPoliceLamp->BindTexture(m_pTexture[0]);
		}
	}

	return pPoliceLamp;
}

//=============================================================================
// 読み込み処理
//=============================================================================
HRESULT CPoliceLamp::Load(void)
{
	// デバイス取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャ生成
	D3DXCreateTextureFromFile(pDevice, POLICELAMP_TEX, &m_pTexture[0]);

	return S_OK;
}

//=============================================================================
// 開放処理
//=============================================================================
void CPoliceLamp::UnLoad(void)
{
	// テクスチャ破棄
	if (m_pTexture[0]!= NULL)
	{
		m_pTexture[0]->Release();
		m_pTexture[0] = NULL;
	}
}