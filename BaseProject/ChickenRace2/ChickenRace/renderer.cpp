//=============================================================================
//
// レンダラー処理 [renderer.cpp]
// Author : 有馬武志
//
//=============================================================================
#include "renderer.h"
#include "scene.h"
#include "manager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

//=============================================================================
// メイン関数
//=============================================================================

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CRenderer::CRenderer()
{
	//値をクリア
	m_pD3D = NULL;
	m_pD3DDevice = NULL;

#ifdef _DEBUG
	//m_pFont = NULL;
#endif

}
//===============================================================================
//　デストラクタ
//===============================================================================
CRenderer::~CRenderer()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;										// バックバッファの数
	d3dpp.BackBufferWidth = SCREEN_WIDTH;							// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;							// カラーモードの指定
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						// 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil = TRUE;							// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;					// デプスバッファとして16bitを使う
	d3dpp.Windowed = true;											// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_pD3DDevice)))
			{
				// 生成失敗
				return E_FAIL;
			}
		}
	}

	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ライティングモード有効
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
																			
	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理を設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

	////ハードウェアの能力を表す構造体
	//D3DCAPS9 caps;

	////フォグパラメータ
	//const FLOAT fogStart = 300;
	//const FLOAT fogEnd = 800;

	//ZeroMemory(&caps, sizeof(D3DCAPS9));

	//m_pD3DDevice->GetDeviceCaps(&caps);
	//m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);								//フォグを使用する
	//m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_ARGB(0, 0, 0, 0));			//フォグの色を設定する
	//m_pD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);						//頂点フォグを使用しない
	//m_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);					//フォグの適用方法
	//m_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&fogStart));				//フォグの開始位置
	//m_pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD *)(&fogEnd));					//フォグの終了位置

	m_bOnOff = false;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CRenderer::Uninit(void)
{
#ifdef _DEBUG
	// デバッグ情報表示用フォントの破棄
	//if (m_pFont != NULL)
	//{
	//	m_pFont->Release();
	//	m_pFont = NULL;
	//}
#endif
	// デバイスの破棄
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}
	// Direct3Dオブジェクトの破棄
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CRenderer::Update(void)
{
	//全ての更新処理
	CScene::UpdateAll();

	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();

	if (pCInputKeyBoard != NULL)
	{
		if (pCInputKeyBoard->GetKeyboardPress(DIK_F1) == true)
		{//矢印の上		
			m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, 2);						// ワイヤーフレーム指定
		}
		if (pCInputKeyBoard->GetKeyboardPress(DIK_F2) == true)
		{//矢印の上		
			m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, 0);						// ワイヤーフレーム指定
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CRenderer::Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	m_pD3DDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
		D3DCOLOR_RGBA(157, 184, 224, 255),
		1.0f,
		0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		D3DVIEWPORT9 viewport;
		m_pD3DDevice->GetViewport(&viewport);

		switch (CManager::GetMode())
		{
		case CManager::MODE_TITLE:
			CTitle::Draw();
			break;
		case CManager::MODE_GAME:
			CGame::Draw();
			break;
		case CManager::MODE_RESULT:
			CResult::Draw();
			break;
		}

		m_pD3DDevice->SetViewport(&viewport);

#ifdef _DEBUG
		int nFps = GetFps();
		CDebugProc::Draw();	
		
		CDebugProc::Print("FPS %d\n", nFps);		
#endif
		// Direct3Dによる描画の終了
		m_pD3DDevice->EndScene();
	}
	else
	{
		MessageBox(0, "Drawの処理でNULL", "警告", MB_OK);
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}