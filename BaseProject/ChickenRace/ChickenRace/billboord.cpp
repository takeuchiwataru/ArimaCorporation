//=============================================================================
//
// ビルボードの処理 [scene2D.cpp]
// Author : 有馬武志
//
//=============================================================================
#include "scene.h"
#include "billboord.h"
#include "manager.h"
#include "renderer.h"
#include "scene3D.h"
#include "game.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BILL_TEXTURE_NAME_1		"data\\TEXTURE\\game\\charselect\\icon.png"			//読み込むテクスチャファイル

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
LPDIRECT3DTEXTURE9	CBillBoord::m_pTexture[MAX_BILLBOORD_TEX] = {};

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CBillBoord::CBillBoord() : CScene3D(4, CScene::OBJTYPE_BILLBOORD)
{
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
}
//===============================================================================
//　デストラクタ
//===============================================================================
CBillBoord::~CBillBoord()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBillBoord::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	CScene3D::Init();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBillBoord::Uninit(void)
{
	CScene3D::Uninit();
}
//=============================================================================
// 更新処理
//=============================================================================
void CBillBoord::Update(void)
{
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CScene3D::Update();
}
//=============================================================================
// 描画処理
//=============================================================================
void CBillBoord::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//位置の初期化
	D3DXVECTOR3 BillPos = CScene3D::GetPosition();
	//ゲームの情報
	CManager::MODE pMode = CManager::GetMode();

	// Zライト
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	pDevice->SetRenderState(D3DRS_LIGHTING, false);

	CScene3D::Draw();

	pDevice->SetRenderState(D3DRS_LIGHTING, true);

	// Zライト
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}
//===============================================================================
//　クリエイト
//===============================================================================
CBillBoord * CBillBoord::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,int nTexType)
{
	CBillBoord *pBillBoord = NULL;

	//NULLチェック
	if (pBillBoord == NULL)
	{//メモリの動的確保

		pBillBoord = new CBillBoord;

		if (pBillBoord != NULL)
		{
			//オブジェクトクラスの生成
			pBillBoord->Init();
			//テクスチャの割り当て
			pBillBoord->BindTexture(m_pTexture[nTexType]);
			//位置の割り当て
			pBillBoord->SetPosSize(pos, size);
			//サイズを入れる
			pBillBoord->m_size = size;
		}
	}

	return pBillBoord;
}
//===============================================================================
// テクスチャの読み込み
//===============================================================================
HRESULT CBillBoord::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, BILL_TEXTURE_NAME_1, &m_pTexture[0]);

	return S_OK;
}
//===============================================================================
// テクスチャの破棄
//===============================================================================
void CBillBoord::UnLoad(void)
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
//===============================================================================
// テクスチャの破棄
//===============================================================================
void CBillBoord::ResetBillboord(void)
{
	CScene *pScene;

	//プライオリティーチェック
	pScene = CScene::GetTop(4);

	//NULLチェック
	while (pScene != NULL)
	{
		//UpdateでUninitされてしまう場合　Nextが消える可能性があるからNextにデータを残しておく
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetDeath() == false)
		{//タイプが壁だったら
			if (pScene->GetObjType() == OBJTYPE_BILLBOORD)
			{
				//何もしていない状態
				pScene->Uninit();
			}
		}
		//Nextに次のSceneを入れる
		pScene = pSceneNext;
	}
	//表示の状態を渡す
	//CLogo::SetLogoState(0, 5);
}

