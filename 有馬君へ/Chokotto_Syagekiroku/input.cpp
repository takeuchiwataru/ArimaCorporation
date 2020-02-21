//=============================================================================
//
// 入力処理 [input.cpp]
// Author :Ryo Sugimoto
//
//=============================================================================
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_KEY_MAX			(256)			// キーの最大数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECTINPUT8 g_pInput = NULL;				//DIRECTINPUTオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;	//入力デバイスへのキーボードのポインタ
BYTE g_aKeyState[NUM_KEY_MAX];				//キーボード入力情報(プレス情報)
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];		//キーボード入力情報(トリガー情報)

//=============================================================================
// キーボードの初期化
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	// 入力デバイス(キーボード)の生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard,
		&g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if(FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if(FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================
// キーボードの終了処理
//=============================================================================
void UninitKeyboard(void)
{
	// 入力デバイス(キーボード)の開放
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	// DirectInputオブジェクトの開放
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//=============================================================================
// キーボードの更新処理
//=============================================================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];
	int nCntKey;

	// 入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState),
		&aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			g_aKeyState[nCntKey] = aKeyState[nCntKey];	//キーボード入力情報保存
			
			//g_aKeyStateTrigger[nCntKey] = ((g_aKeyState[nCntKey] + 1) * aKeyState[nCntKey]) % 2;
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();	//キーボードへのアクセス権獲得
	}
}

//=============================================================================
// キーボードの入力情報(プレス情報)を取得
//=============================================================================
bool GetKeyboardPress(int nKey)
{	//キーボードの入力情報(プレス情報)を獲得
		return(g_aKeyState[nKey] & 0x080) ? true : false;
}
//=============================================================================
// キーボードの入力情報(トリガー情報)を取得
//=============================================================================
bool GetKeyboardTrigger(int nKey)
{	//キーボードの入力情報(プレス情報)を獲得
	return(g_aKeyStateTrigger[nKey] & 0x080) ? true : false;
}
//=============================================================================
// キーボードの入力情報(Quick情報)を取得
//=============================================================================
bool GetKeyboardQuick(int nKey, bool bUse)
{
	static int nQuick[NUM_KEY_MAX];		//全てのキー情報取得のため
	static int nTime = 0;				//連続入力制限用
	int nCntKeyboard;
	bool bQuick = false;

	if (bUse == false)
	{//カウントか入力確認
		nTime--;
		for (nCntKeyboard = 0; nCntKeyboard < NUM_KEY_MAX; nCntKeyboard++)
		{//時間経過用
			nQuick[nCntKeyboard]--;
		}
	}
	else
	{
		if (GetKeyboardTrigger(nKey) == true)
		{//Trigger入力確認
			if (nQuick[nKey] > 0 && nTime < 0)
			{//2回入力
				nQuick[nKey] = 0;
				bQuick = true;
				nTime = 35;	//連続入力制限 35F
			}
			else
			{//1度目
				nQuick[nKey] = 10;
				bQuick = false;
			}
		}
	}

	return bQuick;
}
