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

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CBillBoord::CBillBoord() : CScene3D(4, CScene::OBJTYPE_BILLBOORD)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);

	m_bMap = false;
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
CBillBoord * CBillBoord::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, bool bMap)
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
			//位置の割り当て
			pBillBoord->SetPosSize(pos, size);
			//サイズを入れる
			pBillBoord->m_size = size;
			
			pBillBoord->m_bMap = bMap;
		}
	}

	return pBillBoord;
}