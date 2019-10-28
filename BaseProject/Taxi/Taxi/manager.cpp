//=============================================================================
//
// マネージャー処理 [manager.cpp]
// Author : 有馬武志
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "main.h"
#include "sound.h"
#include "input.h"
#include "camerabace.h"
#include "light.h"
#include "debugproc.h"
#include "result.h"
#include "tutorial.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CRenderer *CManager::m_pRenderer = NULL;
CSound *CManager::m_pSound = NULL;
CInputKeyBoard *CManager::m_pInputKeyBoard = NULL;
CInputMouse *CManager::m_pInputMouse = NULL;
CInputJoypad *CManager::m_pJoyPad = NULL;
CInputXPad *CManager::m_pXPad = NULL;
//CCamera *CManager::m_pCamera = NULL;
CLight *CManager::m_pLight = NULL;
CDebugProc *CManager::m_pDebugProc = NULL;
CGame *CManager::m_pGame = NULL;
CTitle *CManager::m_pTitle = NULL;
CResult *CManager::m_pResult = NULL;
CRanking *CManager::m_pRanking = NULL;
CSelect  *CManager::m_pSelect = NULL;
CTutorial * CManager::m_pTutorial = NULL;
bool CManager::m_bInput = true;

//ゲームの一番最初
CManager::MODE CManager::m_mode = CManager::MODE_GAME;

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CManager::CManager()
{}

//===============================================================================
//　デストラクタ
//===============================================================================
CManager::~CManager()
{}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindows)
{
	//NULLチェック
	if (m_pRenderer == NULL)
	{
		//動的確保
		m_pRenderer = new CRenderer;

		//NULLチェック
		if (m_pRenderer != NULL)
		{
			// 初期化処理
			if (FAILED(m_pRenderer->Init(hWnd, bWindows)))		//FALSE.TRUE
			{
				return -1;
			}
		}
		else
		{//警告文
			MessageBox(0, "警告：メモリがないです", "警告", MB_OK);
		}
	}
	else
	{//警告文
		MessageBox(0, "警告：何かが入ってます", "警告", MB_OK);
	}

	//キーボード生成
	m_pInputKeyBoard = CInputKeyBoard::Create(hInstance, hWnd);

	//マウス生成
	m_pInputMouse = CInputMouse::Create(hInstance, hWnd);

	//ジョイパッド生成
	m_pJoyPad = CInputJoypad::Create(hInstance, hWnd);

	// XInputクラスの生成
	if (m_pXPad == NULL)
	{
		m_pXPad = new CInputXPad;

		if (FAILED(m_pXPad->Init(hInstance, hWnd)))
		{
			m_pXPad->Uninit();			// 終了処理

			delete m_pXPad;				// メモリ開放
			m_pXPad = NULL;				// NULL
		}
	}

	//NULLチェック
	if (m_pSound == NULL)
	{
		//サウンドの動的確保
		m_pSound = new CSound;

		//NULLチェック
		if (m_pSound != NULL)
		{
			//サウンドの初期化処理
			m_pSound->InitSound(hWnd);
		}
	}

	//NULLチェック
	//if (m_pCamera == NULL)
	//{
	//	//カメラの動的確保
	//	m_pCamera = new CCamera;

	//	//NULLチェック
	//	if (m_pCamera != NULL)
	//	{
	//		//サウンドの初期化処理
	//		m_pCamera->Init();
	//	}
	//}

	//NULLチェック
	if (m_pLight == NULL)
	{
		//ライトの動的確保
		m_pLight = new CLight;

		//NULLチェック
		if (m_pLight != NULL)
		{
			//サウンドの初期化処理
			m_pLight->Init(hInstance, hWnd);
		}
	}

	//NULLチェック
	if (m_pDebugProc == NULL)
	{
		//デバックフォント
		m_pDebugProc = new CDebugProc;

		//NULLチェック
		if (m_pDebugProc != NULL)
		{
			m_pDebugProc->Init();
		}
	}

	//モードの設定
	SetMode(m_mode);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CManager::Uninit(void)
{
	//画面の破棄
	switch (m_mode)
	{
	case MODE_TITLE:	//タイトル
		if (m_pTitle != NULL)
		{
			m_pTitle->Uninit();
			delete m_pTitle;
			m_pTitle = NULL;
		}
		break;
	case MODE_TUTORIAL:	//チュートリアル
		if (m_pTutorial != NULL)
		{
			m_pTutorial->Uninit();
			delete m_pTutorial;
			m_pTutorial = NULL;
		}
		break;

	case MODE_GAME:		//ゲーム
		if (m_pGame != NULL)
		{
			m_pGame->Uninit();
			delete m_pGame;
			m_pGame = NULL;
		}
		break;
	case MODE_RESULT:	//リザルト
		if (m_pResult != NULL)
		{
			m_pResult->Uninit();
			delete m_pResult;
			m_pResult = NULL;
		}
	case MODE_RANKING:	// ランキング
		if (m_pRanking != NULL)
		{
			m_pRanking->Uninit();
			delete m_pRanking;
			m_pRanking = NULL;
		}
		break;
	case MODE_SELECT:	// セレクト
		if (m_pSelect != NULL)
		{
			m_pSelect->Uninit();
			delete m_pSelect;
			m_pSelect = NULL;
		}
		break;
	}

	//全ての削除
	CScene::ReleseAll();

	//Sound　NULLチェック
	if (m_pSound != NULL)
	{
		//サウンドの終了処理
		m_pSound->UninitSound();
		//メモリの解放
		delete m_pSound;
		m_pSound = NULL;
	}

	//Renderer NULLチェック
	if (m_pRenderer != NULL)
	{
		// 終了処理
		m_pRenderer->Uninit();
		//メモリの解放
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	//pCInput NULLチェック
	if (m_pInputKeyBoard != NULL)
	{
		// 終了処理
		m_pInputKeyBoard->Uninit();
		//メモリの解放
		delete m_pInputKeyBoard;
		m_pInputKeyBoard = NULL;
	}

	//マウスInputの NULLチェック
	if (m_pInputMouse != NULL)
	{
		// 終了処理
		m_pInputMouse->Uninit();
		//メモリの解放
		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}

	//マウスInputの NULLチェック
	if (m_pJoyPad != NULL)
	{
		// 終了処理
		m_pJoyPad->Uninit();
		//メモリの解放
		delete m_pJoyPad;
		m_pJoyPad = NULL;
	}

	if (m_pXPad != NULL)
	{// XInputの破棄
		m_pXPad->Uninit();			// 終了処理

		delete m_pXPad;				// メモリ開放
		m_pXPad = NULL;				// NULL
	}

	//カメラの終了処理
	//if(m_pCamera != NULL)
	//{
	//	// 終了処理
	//	m_pCamera->Uninit();
	//	//メモリの解放
	//	delete m_pCamera;
	//	m_pCamera = NULL;
	//}

	//ライトの終了処理
	if (m_pLight != NULL)
	{
		// 終了処理
		m_pLight->Uninit();
		//メモリの解放
		delete m_pLight;
		m_pLight = NULL;
	}

	//フォントの終了処理
	if (m_pDebugProc != NULL)
	{
		// 終了処理
		m_pDebugProc->Uninit();
		//メモリの解放
		delete m_pDebugProc;
		m_pDebugProc = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CManager::Update(void)
{
	// 更新処理
	m_pRenderer->Update();

	// キーボード更新処理
	m_pInputKeyBoard->Update();

	//マウス更新処理
	m_pInputMouse->Update();

	//ジョイパッド更新処理
	m_pJoyPad->Update();

	// XInputの更新処理
	if (m_pXPad != NULL)
		m_pXPad->Update();

	//ライトの更新処理
	m_pLight->Update();

	//カメラの更新処理
	//m_pCamera->Update();

	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();

	//デバック用
	//CDebugProc::Print("F1 ワイヤーフレーム　F2　通常\n");

	switch (m_mode)
	{
	case MODE_TITLE:	//タイトル
		if (m_pTitle != NULL)
		{
			m_pTitle->Update();
		}
		break;
	case MODE_TUTORIAL: //チュートリアル
		if (m_pTutorial != NULL) { m_pTutorial->Update(); }
		break;

	case MODE_GAME:		//ゲーム
		if (m_pGame != NULL)
		{
			m_pGame->Update();
		}
		break;
	case MODE_RESULT:	//リザルト
		if (m_pResult != NULL)
		{
			m_pResult->Update();
		}
		break;
	case MODE_RANKING:	// ランキング
		if (m_pRanking != NULL)
		{
			m_pRanking->Update();
		}
		break;
	case MODE_SELECT:	// セレクト
		if (m_pSelect != NULL)
		{
			m_pSelect->Update();
		}
		break;
	}

	//モードの切り替わり時、その中でReleaseAllしています。
}
//=============================================================================
// 描画処理
//=============================================================================
void CManager::Draw(void)
{
	// 描画処理
	m_pRenderer->Draw();
}
//=============================================================================
// モードの設定処理
//=============================================================================
void CManager::SetMode(MODE mode)
{
	switch (m_mode)
	{
	case MODE_TITLE:	//タイトル
		if (m_pTitle != NULL)
		{
			m_pTitle->Uninit();
			delete m_pTitle;
			m_pTitle = NULL;
		}
		break;

	case MODE_TUTORIAL: //チュートリアル
		if (m_pTutorial != NULL)
		{
			m_pTutorial->Uninit();
			delete m_pTutorial;
			m_pTutorial = NULL;
		}
		break;

	case MODE_GAME:		//ゲーム
		if (m_pGame != NULL)
		{
			m_pGame->Uninit();
			delete m_pGame;
			m_pGame = NULL;
		}
		break;
	case MODE_RESULT:	//リザルト
		if (m_pResult != NULL)
		{
			m_pResult->Uninit();
			delete m_pResult;
			m_pResult = NULL;
		}
		break;
	case MODE_RANKING:	// ランキング
		if (m_pRanking != NULL)
		{
			m_pRanking->Uninit();
			delete m_pRanking;
			m_pRanking = NULL;
		}
		break;
	case MODE_SELECT:
		if (m_pSelect != NULL)
		{
			m_pSelect->Uninit();
			delete m_pSelect;
			m_pSelect = NULL;
		}
		break;
	}

	m_mode = mode;

	switch (mode)
	{
	case MODE_TITLE:	//タイトル
		if (m_pTitle == NULL)
		{//メモリを動的確保
			m_pTitle = new CTitle;

			if (m_pTitle != NULL)
			{
				m_pTitle->Init();
				m_pSound->StopSound();
				m_pSound->SetVolume(CSound::SOUND_LABEL_BGM_TITLE, 2.8f);

				m_pSound->PlaySound(CSound::SOUND_LABEL_BGM_TITLE);
			}
		}
		break;

	case MODE_TUTORIAL: //チュートリアル
		if (m_pTutorial == NULL)
		{
			m_pTutorial = new CTutorial;

			if (m_pTutorial != NULL)
			{
				m_pTutorial->Init();
				m_pSound->StopSound();
				m_pSound->SetVolume(CSound::SOUND_LABEL_BGM_GAME, 0.5f);
				m_pSound->PlaySound(CSound::SOUND_LABEL_BGM_GAME);
			}
		}
		break;

	case MODE_GAME:		//ゲーム
		if (m_pGame == NULL)
		{//メモリを動的確保
			m_pGame = new CGame;

			if (m_pGame != NULL)
			{
				m_pGame->Init();
			}
		}
		break;

	case MODE_RESULT:	//リザルト
		if (m_pResult == NULL)
		{//メモリを動的確保
			m_pResult = new CResult;

			if (m_pResult != NULL)
			{
				m_pResult->Init();
				m_pSound->StopSound();
				//m_pSound->SetVolume(CSound::SOUND_LABEL_BGM_RESULT, 0.8f);

				//m_pSound->PlaySound(CSound::SOUND_LABEL_BGM_RESULT);
			}
		}
		break;
	case MODE_RANKING:	//ランキング
		if (m_pRanking == NULL)
		{//メモリを動的確保
			m_pRanking = new CRanking;

			if (m_pRanking != NULL)
			{
				m_pRanking->Init();
				m_pSound->StopSound();
				//m_pSound->SetVolume(CSound::SOUND_LABEL_BGM_RESULT, 0.8f);

				//m_pSound->PlaySound(CSound::SOUND_LABEL_BGM_RESULT);
			}
		}
		break;

	case MODE_SELECT:	// セレクト
		if (m_pSelect == NULL)
		{	// メモリの動的確保
			m_pSelect = new CSelect;

			if (m_pSelect != NULL)
			{
				m_pSelect->Init();
				m_pSound->StopSound();
				//m_pSound->SetVolume(CSound::SOUND_LABEL_BGM_SELECT, 1.1f);
				//m_pSound->PlaySound(CSound::SOUND_LABEL_BGM_SELECT);
			}
		}
	}
}
