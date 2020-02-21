//=========================================================================================================//
//
// DirectX雛型処理 [main.cpp]
// Author : Ryou Sugimoto
//
//=========================================================================================================//
#include "main.h"
#include "input.h"
#include "game.h"
#include "enemy.h"
#include "Title.h"
#include "fade.h"
#include "sound.h"
#include <stdlib.h>				//ランド用
#include <time.h>				//同じくランド用

//*********************************************************************************************************//
// マクロ定義
//*********************************************************************************************************//
#define CLASS_NAME		"WindowClass"		// ウインドウクラスの名前
#define WINDOW_NAME		"チョコット射撃録"	// ウインドウの名前(キャプション名)
#define WINDOW_WIDTH	(1280)				// 画面サイズ(幅)
#define WINDOW_HEIGHT	(720)				// 画面サイズ(高さ)
#define MAX_RECAST		(2)					//行動不能・使用不能時間のため

//*********************************************************************************************************//
// プロトタイプ宣言
//*********************************************************************************************************//
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
#ifdef _DEBUG
void DrawFPS(void);
#endif

//*********************************************************************************************************//
// グローバル変数
//*********************************************************************************************************//
LPDIRECT3D9 g_pD3D = NULL;				// Direct3Dオブジェクトのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;	// Direct3Dデバイスのポインタ
#ifdef _DEBUG
LPD3DXFONT			g_pFont = NULL;			// フォントへのポインタ
int					g_nCountFPS = 0;		// FPSカウンタ
#endif

//=========================================================================================================//
// メイン関数
//=========================================================================================================//
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	int nKey = 0;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WindowProc,
		0,
		0,
		hInstance,
		LoadIcon(NULL, IDI_APPLICATION),
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		LoadIcon(NULL, IDI_APPLICATION)
	};
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	HWND hWnd;				//ウィンドウハンドル
	MSG msg;				//メッセージを格納する変数
	DWORD dwCurrentTime;	//現在時刻
	DWORD dwExecLastTime;	//最後に処理した時刻

#ifdef _DEBUG
	DWORD dwFrameCount;
	DWORD dwFPSLastTime;
#endif

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	//	指定したクライアント領域を確保するために必要なウインドウ座標を計算
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウの生成
	hWnd = CreateWindowEx(0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(rect.right - rect.left),
		(rect.bottom - rect.top),
		NULL,
		NULL,
		hInstance,
		NULL);

	// 初期化処理(ウィンドウを生成してから行う)
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	//分解機能を設定
	timeBeginPeriod(1);

	// 各カウンターを初期化
	dwCurrentTime = 0;
	dwExecLastTime = timeGetTime();		// システム時刻を取得(ミリ秒単位)
#ifdef _DEBUG
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();		// システム時刻を取得(ミリ秒単位)
#endif

	// ウインドウの表示(初期化処理の後に行う)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)	// メッセージを取得しなかった場合"0"を返す
		{	// Windowsの処理
			if (msg.message == WM_QUIT)
			{	// "WM_QUIT"メッセージが送られてきたらループを抜ける
				break;
			}
			else
			{
				// メッセージの翻訳と送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{	// DirectXの処理
			dwCurrentTime = timeGetTime();	//現在時刻を取得
			
#ifdef _DEBUG
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5秒経過
			 // FPSを測定
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;		// FPSを測定した時刻を保存
				dwFrameCount = 0;					// カウントをクリア
			}
#endif

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60分の1秒
				dwExecLastTime = dwCurrentTime;		//処理した時刻を保存
				// 更新処理
				Update();

				// 描画処理
				Draw();

#ifdef _DEBUG
				dwFrameCount++;		// カウントを加算
#endif

			}
		}
	}	// while括弧

		// 終了処理
	Uninit();

	//分解機能を戻す
	timeEndPeriod(1);
	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//=========================================================================================================//
// ウインドウプロシージャ
//=========================================================================================================//
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch (uMsg)
	{
	case WM_DESTROY:
		// "WM_QUIT"メッセージを送る
		PostQuitMessage(0);
		break;

	case WM_CLOSE:
		nID = MessageBox(hWnd, "本当に終了しますか？", "終了", MB_YESNO);
		if (nID == IDYES)
		{	// ウィンドウを破棄する(WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
		}
		else
		{// NOが押された場合
			return 0;	// 0を返さないと終了してしまう
		}
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:	// [ESC]キーが押された
						// ウィンドウを破棄する(WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);	// 既定の処理を返す
}

//=========================================================================================================//
// 初期化処理
//=========================================================================================================//
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	// プレゼンテーションパラメータ
	srand((unsigned int)time(0));
									// Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ワークをゼロクリア
	d3dpp.BackBufferWidth = WINDOW_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = WINDOW_HEIGHT;				// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;				// バックバッファの形式
	d3dpp.BackBufferCount = 1;							// バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファ(Ｚバッファ)とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;						// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート(現在の速度に合わせる)
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル(VSyncを待って描画)

	// Direct3Dデバイスの生成
	// [デバイス作成制御]<描画>と<頂点処理>をハードウェアで行なう
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,						// ディスプレイアダプタ
		D3DDEVTYPE_HAL,							// デバイスタイプ
		hWnd,									// フォーカスするウインドウへのハンドル
		D3DCREATE_HARDWARE_VERTEXPROCESSING,	// デバイス作成制御の組み合わせ
		&d3dpp,									// デバイスのプレゼンテーションパラメータ
		&g_pD3DDevice)))						// デバイスインターフェースへのポインタ
	{
		// 上記の設定が失敗したら
		// [デバイス作成制御]<描画>をハードウェアで行い、<頂点処理>はCPUで行なう
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// [デバイス作成制御]<描画>と<頂点処理>をCPUで行なう
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}
	// レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// TEXTUREステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小時の補間設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大時の補間設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定

#ifdef _DEBUG
	// デバッグ表示用フォントを設定
	// pDevice         : デバイスへのポインタ
	// Height          : 文字の高さ
	// Width           : 文字の幅
	// Weight          : フォントの太さ
	// MipLevels       : ミップマップレベル数
	// Italic          : イタリックフォント
	// CharSet         : フォントの文字セット
	// OutputPrecision : 実際のフォントと目的のフォントのサイズおよび特性の一致方法を指定
	// Quality         : 実際のフォントと目的のフォントとの一致方法を指定
	// PitchAndFamily  : ピッチとファミリインデックス
	// pFacename       : フォントの名前を保持する文字列
	// ppFont          : フォントへのポインタ
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFont);
#endif

	// 各種オブジェクトの初期化処理
	InitKeyboard(hInstance, hWnd);
	InitSound(hWnd);

	MODE *Mode;	//現在のモード確認のため

	Mode = GetMode();	//現在のモードをもらう
	srand((unsigned int)time(0));	//ランドのため

	// 各種オブジェクトの更初期化処
	
	InitFade(Mode[0]);
	InitWindow();
	InitFadeWindow();
	InitBGWindow();
	InitAnimation();
	switch (Mode[0])
	{
	case MODE_TITLE:
		InitTitle();
		break;

	case MODE_RANKING:
		InitRanking();
		break;

	case MODE_WATCH:
		InitWatch();
		break;

	case MODE_TUTORIAL:
		InitTutorial();
		break;

	case MODE_GAME:
		InitGame();
		break;

	case MODE_RESOLT:
		InitResult();
		break;
	}

	HWND *hwnd;
	hwnd = GethWnd();
	hwnd[0] = hWnd;

	return S_OK;
}

//=========================================================================================================//
// 終了処理
//=========================================================================================================//
void Uninit(void)
{
	// 各種オブジェクトの終了処理
	UninitKeyboard();

#ifdef _DEBUG
	if (g_pFont != NULL)
	{// デバッグ表示用フォントの開放
		g_pFont->Release();
		g_pFont = NULL;
	}
#endif

	// Direct3Dデバイスの開放
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの開放
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	MODE *Mode;	//現在のモード確認のため

	Mode = GetMode();	//現在のモードをもらう

	// 各種オブジェクトの更新処理
	UninitKeyboard();
	UninitSound();
	UninitAnimation();

	switch (Mode[0])
	{
	case MODE_TITLE:
		UninitTitle();
		break;

	case MODE_RANKING:
		UninitRanking();
		break;

	case MODE_WATCH:
		UninitWatch();
		break;

	case MODE_TUTORIAL:
	UninitTutorial();
		break;

	case MODE_GAME:
		UninitGame();
		break;

	case MODE_RESOLT:
		UninitResult();
		break;
	}
	UninitWindow();
	UninitBGWindow();
	UninitFade();
	UninitFadeWindow();
}

//=========================================================================================================//
// 更新処理
//=========================================================================================================//
void Update(void)
{
	MODE *Mode;	//現在のモード確認のため

	Mode = GetMode();	//現在のモードをもらう

	// 各種オブジェクトの更新処理
	UpdateKeyboard();

	switch (Mode[0])
	{
	case MODE_TITLE:
		UpdateTitle();
		break;

	case MODE_RANKING:
		UpdateRanking();
		break;

	case MODE_WATCH:
		UpdateWatch();
		break;

	case MODE_TUTORIAL:
		UpdateTutorial();
		break;

	case MODE_GAME:
		UpdateGame();
		break;

	case MODE_RESOLT:
		UpdateResult();
		break;
	}
	UpdateWindow();
	UpdateFade();
	UpdateFadeWindow();
	UpdateAnimation();
}
//=========================================================================================================//
// 描画処理
//=========================================================================================================//
void Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	g_pD3DDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//描画の開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		// 各種オブジェクトの描画処理
		DrawBGWindow();

		MODE *Mode;	//現在のモード確認のため

		Mode = GetMode();	//現在のモードをもらう

		switch (Mode[0])
		{
		case MODE_TITLE:
			DrawTitle();
			break;

		case MODE_RANKING:
			DrawRanking();
			break;

		case MODE_WATCH:
			DrawWatch();
			break;

		case MODE_TUTORIAL:
			DrawTutorial();
			break;

		case MODE_GAME:
			DrawGame();
			break;

		case MODE_RESOLT:
			DrawResult();
			break;
		}
		DrawWindow();
		DrawAnimation();
		DrawFade();
		DrawFadeWindow();

#ifdef _DEBUG
		// FPS表示
		DrawFPS();
#endif

		// 描画の終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
//=========================================================================================================//
// デバイスの取得
//=========================================================================================================//
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}
#ifdef _DEBUG
//=============================================================================
// FPS表示処理
//=============================================================================
void DrawFPS(void)
{
	RECT rect = { SCREEN_WIDTH - 55, SCREEN_HEIGHT - 24, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[256];

	// 文字列を代入
	wsprintf(&aStr[0], "FPS:%d\n",g_nCountFPS);

	// テキスト描画
	// hDC      : デバイスコンテキストのハンドル
	// lpString : 描画する文字列へのポインタ
	// nCount   : 文字列の文字数
	// lpRect   : 長方形領域
	// uFormat  : テキストの整形方法を指定
	//g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));
}
#endif

//=========================================================================================================//
// デバイスの取得
//=========================================================================================================//
void SetMode(MODE mode)
{
	MODE *Mode;	//現在のモード確認のため

	Mode = GetMode();	//現在のモードをもらう

	switch (Mode[0])
	{//現在の処理を終了する
	case MODE_TITLE:
		UninitTitle();
		break;

	case MODE_RANKING:
		UninitRanking();
		break;

	case MODE_WATCH:
		UninitWatch();
		break;

	case MODE_TUTORIAL:
		UninitTutorial();
		break;

	case MODE_GAME:
		UninitGame();
		break;

	case MODE_RESOLT:
		UninitResult();
		break;
	}//switch括弧

	switch (mode)
	{//次に行う処理の初期化
	case MODE_TITLE:
		InitTitle();
		break;

	case MODE_RANKING:
		InitRanking();
		break;

	case MODE_WATCH:
		InitWatch();
		break;

	case MODE_TUTORIAL:
		InitTutorial();
		break;

	case MODE_GAME:
		InitGame();
		break;

	case MODE_RESOLT:
		InitResult();
		break;
	}//switch括弧
	Mode[0] = mode;
}

//=========================================================================================================//
// モードの取得
//=========================================================================================================//
MODE *GetMode(void)
{
	static MODE Mode = MODE_TITLE;

	return &Mode;
}
//=========================================================================================================//
// windowの取得
//=========================================================================================================//
HWND *GethWnd(void)
{
	static HWND hWnd;

	return &hWnd;
}