//=============================================================================
//
// 影の処理 [shadow.cpp]
// Author : 有馬武志
//
//=============================================================================
#include "scene.h"
#include "shadow.h"
#include "manager.h"
#include "renderer.h"
#include "scene3D.h"
#include "mesh.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	SHADOW_TEXTURE_NAME_1	"data\\TEXTURE\\effect000.jpg"			//読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
LPDIRECT3DTEXTURE9	CShadow::m_pTexture[MAX_BILLBOORD_TEX] = {};
float CShadow::m_fMeshHeight = 0.0f;

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CShadow::CShadow() : CScene3D(SHADOW_PRIOTITY, CScene::OBJTYPE_SHADOW)
{
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
//===============================================================================
//　デストラクタ
//===============================================================================
CShadow::~CShadow(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CShadow::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	CScene3D::Init();

	//これはビルボードかエフェクトかの判別
	CScene3D::SetTypeNumber(CScene3D::TYPE_SHADOW);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CShadow::Uninit(void)
{
	CScene3D::Uninit();
}
//=============================================================================
// 更新処理
//=============================================================================
void CShadow::Update(void){}

//=============================================================================
// 描画処理
//=============================================================================
void CShadow::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//アルファテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//アルファテスト処理
	//デフォルトの場合FALSEになっているからTRUEにする
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	//減算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CScene3D::Draw();

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//デフォルトの場合FALSEになっているからTRUEにする
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}
//===============================================================================
//　クリエイト
//===============================================================================
CShadow * CShadow::Create(D3DXVECTOR3 pos, float fWidth, float fDepth,int nTexType)
{
	CShadow *pShadow = NULL;

	//NULLチェック
	if (pShadow == NULL)
	{//メモリの動的確保

		pShadow = new CShadow;

		if (pShadow != NULL)
		{
			//オブジェクトクラスの生成
			pShadow->Init();
			//テクスチャの割り当て
			pShadow->BindTexture(m_pTexture[nTexType]);
			//位置からサイズを出す
			pShadow->SetGroundPosSize(pos, D3DXVECTOR2(fWidth, fDepth));
			//横幅の設定
			pShadow->m_fWidth = fWidth;
			//奥行の設定
			pShadow->m_fDepth = fDepth;
			//位置の割り当て
			pShadow->SetPosition(pos);
			//位置の設定
			pShadow->m_pos = pos;
		}
	}

	return pShadow;
}

//=============================================================================
// 影の位置を設定
//=============================================================================
void CShadow::SetPositionShadow(D3DXVECTOR3 pos)
{
	//位置を動かす
	m_pos = pos;
	CScene3D::SetPosition(m_pos);
}
//===============================================================================
// テクスチャの読み込み
//===============================================================================
HRESULT CShadow::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, SHADOW_TEXTURE_NAME_1, &m_pTexture[0]);

	return S_OK;
}

//===============================================================================
// テクスチャの破棄
//===============================================================================
void CShadow::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_BILLBOORD_TEX; nCount++)
	{
		//テクスチャーの破棄
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}
