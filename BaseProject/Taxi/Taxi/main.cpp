//=============================================================================
//
// メイン処理 [main.cpp]
// Author : 有馬武志
//
//=============================================================================
#include "main.h"
#include "manager.h"
#include "resource.h"
#include <windows.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"						//ウインドウのクラス名
#define WINDOW_NAME		"ザ・チキンレース"				//ウインドウのキャプション名
#define NAME			"Section5.2 open file dialog"	//タイトルバーに表示するテキスト

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
#ifdef _DEBUG
int						g_nCountFPS;			// FPSカウンタ
#endif

//=============================================================================
// メイン関数
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{	
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	CManager *pManager = NULL;

	//
	HICON hIcon = ::LoadIcon
	(
		hInstance,							// アプリケーションインスタンスのハンドル
		MAKEINTRESOURCE(IDI_ICON)			// リソース名の文字列またはリソース識別子
	);

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					// WNDCLASSEXのメモリサイズを指定
		CS_CLASSDC,							// 表示するウィンドウのスタイルを設定
		WndProc,							// ウィンドウプロシージャのアドレスを指定
		0,									// 通常は指定しないため"0"を指定
		0,									// 通常は指定しないため"0"を指定
		hInstance,							// WinMainの引数のインスタンスハンドルを指定
		hIcon,								// タスクバーに使用するアイコンを指定
		LoadCursor(NULL, IDC_ARROW),		// 使用するマウスカーソルを指定
		(HBRUSH)(COLOR_WINDOW + 1),			// ウィンドウのクライアント領域の背景色を設定
		"IDR_MENU1",						// Windowsにつけるメニューを設定
		CLASS_NAME,							// ウィンドウクラスの名前
		hIcon								// ファイルアイコンに使用するアイコンを指定
	};
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	HWND hWnd;
	MSG msg;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// 指定したクライアント領域を確保するために必要なウィンドウ座標を計算
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウの作成
	hWnd = CreateWindow(CLASS_NAME,
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

	if (pManager == NULL)
	{//動的確保
		pManager = new CManager;
	}

	if (pManager != NULL)
	{
		if (FAILED(pManager->Init(hInstance, hWnd, TRUE)))		//FALSEフルスクリーン.TRUEウィンドウ
		{
			return -1;
		}

		// 分解能を設定
		timeBeginPeriod(1);

		// フレームカウント初期化
		dwCurrentTime =
			dwFrameCount = 0;
		dwExecLastTime =
			dwFPSLastTime = timeGetTime();

		// ウインドウの表示
		ShowWindow(hWnd, nCmdShow);
		//マウスポインター非表示
		//ShowCursor(FALSE);
		UpdateWindow(hWnd);

		// メッセージループ
		while (1)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{// PostQuitMessage()が呼ばれたらループ終了
					break;
				}
				else
				{
					// メッセージの翻訳とディスパッチ
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			else
			{
				dwCurrentTime = timeGetTime();	// 現在の時間を取得
				if ((dwCurrentTime - dwFPSLastTime) >= 500)
				{// 0.5秒ごとに実行
#ifdef _DEBUG
				// FPSを算出
					g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
					dwFPSLastTime = dwCurrentTime;	// 現在の時間を保存
					dwFrameCount = 0;
				}

				if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
				{// 1/60秒経過
					dwExecLastTime = dwCurrentTime;	// 現在の時間を保存

					pManager->Update();

					pManager->Draw();

					dwFrameCount++;
				}
			}
		}
	}

	if (pManager != NULL)
	{
		pManager->Uninit();

		delete pManager;
		pManager = NULL;
	}

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 分解能を戻す
	timeEndPeriod(1);

	return (int)msg.wParam;
}
//=============================================================================
// ファイルを開くのに必要な関数
//=============================================================================
bool openReadFile(HWND hWnd)
{
	//変数宣言
	OPENFILENAME    ofn;
	char            filename[256], msg[300], buf[1024], Path[MAX_PATH + 1],cWsprint[256];
	HANDLE          hFile;
	DWORD           dwBytes;

	GetModuleFileName(NULL, Path, MAX_PATH);

	char drive[MAX_PATH + 1], dir[MAX_PATH + 1], fname[MAX_PATH + 1], ext[MAX_PATH + 1];

	_splitpath(Path, drive, dir, fname, ext);//パス名を構成要素に分解します

	//ドライブとディレクトリを結合
	wsprintf(cWsprint, "%s%s",drive,dir);

	//変数の初期化
	filename[0] = '\0';														//先頭の文字を0文字目から始める
	memset(&ofn, 0, sizeof(OPENFILENAME));									//構造体を0でクリア
	ofn.lStructSize = sizeof(OPENFILENAME);									//構造体のサイズの指定
	ofn.lpstrInitialDir = cWsprint;											//始まるアドレス
	ofn.lpstrFilter = "text file(*.txt)\0*.txt\0all file(*.*)\0*.*\0\0";	//フィルタの指定
	ofn.lpstrFile = filename;												//結果が格納されるバッファの指定
	ofn.nMaxFile = sizeof(filename);										//バッファのサイズ
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;						//今回は順に、「正しいパスのみ入力可」「正しいファイル名のみ入力可」「読み取り専用チェックを消す」
	ofn.lpstrDefExt = "txt";												//標準の拡張子の指定

	//ダイアログを開く
	if (GetOpenFileName(&ofn) != TRUE)
	{
		return FALSE;
	}

	//ファイルを開く（なければ作成）
	hFile = CreateFile(filename, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE) 
	{
		wsprintf(msg, "%s failed to open.", filename);
		MessageBox(NULL, msg, NAME, MB_OK);
		return FALSE;
	}

	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
	ReadFile(hFile, buf, sizeof(buf) - 1, &dwBytes, NULL);
	*((u_char*)buf + dwBytes) = 0;  //読み終わりに'\0'を挿入

	//ファイルを閉じる
	CloseHandle(hFile);

	return TRUE;
}

//=============================================================================
// ウインドウプロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
		//switch (LOWORD(wParam))
		//{
		//case IDM_FILEOPEN: //開く
		//	openReadFile(hWnd);
		//	//InvalidateRect(hWnd, NULL, TRUE);  //領域無効化
		//	//UpdateWindow(hWnd);                //再描画命令
		//	break;
		//case IDM_EXIT: //終了
		//	PostMessage(hWnd, WM_CLOSE, 0, 0);
		//	break;
		//}
		break;
	case WM_CREATE:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:				// [ESC]キーが押された
			DestroyWindow(hWnd);	// ウィンドウを破棄するよう指示する

			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
#ifdef _DEBUG
//=============================================================================
// FPS
//=============================================================================
int GetFps(void)
{
	return g_nCountFPS;
}
#endif
