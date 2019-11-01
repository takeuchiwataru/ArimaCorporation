//=============================================================================
//
// 入力処理 [input.cpp]
// Author : 有馬武志 / 目黒未来也
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS	//Fire Wall突破
#include <stdio.h>				//インクルドファイル

#include "input.h"
#include "manager.h"
#include "Server.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define DI_JOY_I_INPUT   (1000)  // スティックの方向入力受付を行う範囲

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECTINPUT8 CInput::m_pInput = NULL;
D3DXVECTOR3 CInputMouse::m_MousePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
int CInputJoyPad_0::m_nNumPad = 0;
//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CInput::CInput()
{
	m_pDevice = NULL;
}
//===============================================================================
//　デストラクタ
//===============================================================================
CInput::~CInput()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	//NULLチェック
	if (m_pInput == NULL)
	{//DirectInputオブジェクトの生成
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CInput::Uninit(void)
{
	//入力デバイス(キーボード)の開放
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();	//キーボードへのアクセス権を放棄
		m_pDevice->Release();
		m_pDevice = NULL;
	}
	//DirectInputオブジェクトの開放
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void CInput::Update(void){}


//ここから派生クラスのキーボード


//===============================================================================
//　キーボードのデフォルトコンストラクタ
//===============================================================================
CInputKeyBoard::CInputKeyBoard()
{
	//キー情報のクリア
	for (int nCnt = 0; nCnt < NUM_KEY_MAX; nCnt++)
	{
		m_aKeyState[nCnt] = 0;			//キーボードの入力情報（プレス情報）
		m_aKeyStateTrigger[nCnt] = 0;	//キーボードの入力情報 (トリガー情報)
	}
}
//===============================================================================
//　キーボードのデストラクタ
//===============================================================================
CInputKeyBoard::~CInputKeyBoard()
{

}
//=============================================================================
// キーボードの初期化処理
//=============================================================================
HRESULT CInputKeyBoard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance,hWnd);

	//入力デバイス(キーボード)の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//キーボードへのアクセス権を獲得(入力制御開始)
	m_pDevice->Acquire();

	return S_OK;
}
//=============================================================================
// キーボードの終了処理
//=============================================================================
void CInputKeyBoard::Uninit(void)
{
	CInput::Uninit();
}
//=============================================================================
// キーボードの更新処理
//=============================================================================
void CInputKeyBoard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];
	int nCntKey;

	//入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];			//※トリガーのときは一番上
			m_aKeyState[nCntKey] = aKeyState[nCntKey];																//キーボードの入力情報保存
		}
	}
	else
	{
		m_pDevice->Acquire();		//キーボードへのアクセス権を獲得
	}
}
//===============================================================================
// キーボードのクリエイト
//===============================================================================
CInputKeyBoard *CInputKeyBoard::Create(HINSTANCE hInstance,HWND hWnd)
{
	CInputKeyBoard *pCInputKeyBoard = NULL;

	//NULLチェック
	if (pCInputKeyBoard == NULL)
	{
		//動的確保
		pCInputKeyBoard = new CInputKeyBoard;

		//NULLチェック
		if (pCInputKeyBoard != NULL)
		{
			//初期化処理
			pCInputKeyBoard->Init(hInstance, hWnd);
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

	return pCInputKeyBoard;
}
//=========================================================================================================================
// キーボードの入力情報(プレス情報)を取得
//=========================================================================================================================
bool CInputKeyBoard::GetKeyboardPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}
//=========================================================================================================================
// キーボードの入力情報(トリガー情報)を取得
//=========================================================================================================================
bool CInputKeyBoard::GetKeyboardTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}
//=========================================================================================================================
// キーボードの入力情報(全ボタン情報)を取得
//=========================================================================================================================
bool CInputKeyBoard::GetKeyboardAny(int nNumber)
{
	if (nNumber == 1)
	{
		for (int nCount = 0; nCount < NUM_KEY_MAX; nCount++)
		{
			if (m_aKeyStateTrigger[nCount] == 0x80)
			{
				return true;
			}
		}
	}
	return false;
}

//ここから派生クラスのマウス


//===============================================================================
//　マウスのデフォルトコンストラクタ
//===============================================================================
CInputMouse::CInputMouse()
{
	//キー情報のクリア
	for (int nCnt = 0; nCnt < NUM_MOUSE_MAX; nCnt++)
	{
		m_moveRect;					// 画面上で動ける範囲
		m_x = 0;					// X座標
		m_y = 0;					// Y座標
		m_lButton = 0;				// 左ボタン
		m_rButton = 0;				// 右ボタン
		m_cButton = 0;				// 真ん中ボタン
		m_moveAdd = 0;				// 移動量
		m_imgRect;					// マウス用画像矩形
		m_imgWidth = 0;				// マウス画像幅
		m_imgHeight = 0;			// マウス画像高さ
		m_MousePos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		m_Pos, m_WPos;
		m_MouseState;
		m_MouseStatePress = {};		// マウスの入力情報（プレス情報）
		m_MouseStateTrigger = {};	// マウスの入力情報（トリガー情報）
		m_MouseStateRelease = {};	// マウスの入力情報（リリース情報
	}
}
//===============================================================================
//　マウスのデストラクタ
//===============================================================================
CInputMouse::~CInputMouse()
{

}
//=============================================================================
// マウスの初期化処理
//=============================================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	// DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
	{
		return E_FAIL;
	}

	// 入力デバイス(キーボード)の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
	{
		return E_FAIL;
	}

	// 軸モードを設定（相対値モードに設定）
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;
	//diprop.dwData = DIPROPAXISMODE_ABS; // 絶対値モードの場合
	if (FAILED(m_pDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		return E_FAIL;
	}

	// マウスへのアクセス権を獲得(入力制御開始)
	m_pDevice->Acquire();

	return S_OK;
}
//=============================================================================
// マウスの終了処理
//=============================================================================
void CInputMouse::Uninit(void)
{
	CInput::Uninit();
}
//=============================================================================
// マウスの更新処理
//=============================================================================
void CInputMouse::Update(void)
{
	//変数宣言
	int nCntMouse;

	POINT pt;
	GetCursorPos(&pt);

	//POINT型をD3DXVECTOR3型の物に代入する
	m_MousePos = D3DXVECTOR3((float)pt.x, (float)pt.y, 0.0f);

	//入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(m_MouseState), &m_MouseState)))
	{
		for (nCntMouse = 0; nCntMouse < NUM_MOUSE_MAX; nCntMouse++)
		{
			m_MouseStateRelease.rgbButtons[nCntMouse] = (m_MouseStatePress.rgbButtons[nCntMouse] ^ m_MouseState.rgbButtons[nCntMouse]) & m_MouseStatePress.rgbButtons[nCntMouse];
			m_MouseStateTrigger.rgbButtons[nCntMouse] = (m_MouseStatePress.rgbButtons[nCntMouse] ^ m_MouseState.rgbButtons[nCntMouse]) & m_MouseState.rgbButtons[nCntMouse];
			m_MouseStatePress.rgbButtons[nCntMouse] = m_MouseState.rgbButtons[nCntMouse];
		}
	}
	else
	{
		m_pDevice->Acquire();
	}
}
//===============================================================================
// マウスのクリエイト
//===============================================================================
CInputMouse *CInputMouse::Create(HINSTANCE hInstance, HWND hWnd)
{
	CInputMouse *pCInputMouse = NULL;

	//NULLチェック
	if (pCInputMouse == NULL)
	{
		//動的確保
		pCInputMouse = new CInputMouse;

		//NULLチェック
		if (pCInputMouse != NULL)
		{
			//初期化処理
			pCInputMouse->Init(hInstance, hWnd);
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

	return pCInputMouse;
}
//=========================================================================================================================
// マウスの入力情報(プレス情報)を取得
//=========================================================================================================================
bool CInputMouse::GetMousePress(int nKey)
{
	return (m_MouseStatePress.rgbButtons[nKey] & 0x80) ? true : false;
}

//=========================================================================================================================
// マウスの入力情報(トリガー情報)を取得
//=========================================================================================================================
bool CInputMouse::GetMouseTrigger(int nKey)
{
	return (m_MouseStateTrigger.rgbButtons[nKey] & 0x80) ? true : false;
}

//=========================================================================================================================
// マウスの入力情報(リリース情報)を取得
//=========================================================================================================================
bool CInputMouse::GetMouseRelease(int nKey)
{
	return (m_MouseStateRelease.rgbButtons[nKey] & 0x80) ? true : false;
}

//ここから派生クラスのジョイパッド
//=============================================================================
//コントローラークラス コンストラクタ
//=============================================================================
CInputJoypad::CInputJoypad()
{
	//キー情報をクリア
	for (int nCnt = 0; nCnt < NUM_BUTTON_MAX; nCnt++)
	{
		m_aKeyState[nCnt] = 0;
		m_aKeyStateTrigger[nCnt] = 0;
	}
}

//=============================================================================
//コントローラークラス デストラクタ
//=============================================================================
CInputJoypad::~CInputJoypad()
{
}

//=============================================================================
// コントローラークラス初期化
//=============================================================================
HRESULT CInputJoypad::Init(HINSTANCE hInstance, HWND hWnd)
{
	//HWND = ウィンドウがアクティブの時にする
	//初期化してから
	CInput::Init(hInstance, hWnd);

	HRESULT hr = m_pInput->CreateDevice(GUID_Joystick, &m_pDevice, NULL);

	//エラー判定
	/*if (FAILED(hr))
	{
		hr = m_pInput->CreateDevice(GUID_Joystick, &m_pDevice, NULL);
		if (FAILED(hr))
		{
			return E_FAIL;
		}
	}*/

	// 入力デバイス(コントローラー)の生成
	if (FAILED(m_pInput->CreateDevice(GUID_Joystick,&m_pDevice,NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIJoystick)))
	{
		return E_FAIL;
	}
	// 協調モードを設定	 *入力をとる取らないだから変わらない
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// コントローラーのアクセス権を獲得(入力制御開始)
	if (m_pDevice != NULL)
	{// 生成できた
		DIPROPRANGE diprg;

		// 軸の値の範囲を設定
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = -DI_JOY_I_INPUT;
		diprg.lMax = DI_JOY_I_INPUT;

		// 軸の設定
		// 左アナログスティック
		//X軸
		diprg.diph.dwObj = DIJOFS_X;
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
		//Y軸
		diprg.diph.dwObj = DIJOFS_Y;
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
		//Z軸
		diprg.diph.dwObj = DIJOFS_Z;
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 右アナログスティック
		//RX軸
		diprg.diph.dwObj = DIJOFS_RX;
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
		//RY軸
		diprg.diph.dwObj = DIJOFS_RY;
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
		//RZ軸
		diprg.diph.dwObj = DIJOFS_RZ;
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

		// ?
		//diprg.diph.dwObj = DIJOFS_SLIDER(0);
		//g_pDevJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 十字キー(上を0度とし時計回りに角度 * 100)
		diprg.diph.dwObj = DIJOFS_POV(0);
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

		// ジョイスティックへのアクセス権を獲得(入力制御開始)
		m_pDevice->Acquire();
		//m_pDevice->Poll();
	}

	return S_OK;
}

//=============================================================================
// コントローラークラス終了処理
//=============================================================================
void CInputJoypad::Uninit(void)
{
	// 入力デバイス(コントローラー)の開放
	CInput::Uninit();

	// DirectInputオブジェクトの開放
	//if (m_pInput != NULL)
	//{
	//	m_pInput->Release();
	//	m_pInput = NULL;
	//}

	//消されていなかったときに消す
	// 入力デバイス(キーボード)の開放
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}
}

//=============================================================================
// コントローラークラス更新処理
//=============================================================================
void CInputJoypad::Update(void)
{
	DIJOYSTATE dijs;

	if (m_pDevice != NULL)
	{
		m_pDevice->Poll();

		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(dijs), &dijs)))
		{
			// 入力デバイスからデータ(軸)を取得
			m_LeftAxizX = (float)dijs.lX;
			m_LeftAxizY = (float)dijs.lY;
			m_RightAxizX = (float)dijs.lRx;
			m_RightAxizY = (float)dijs.lRy;

			//左スティックが上に倒された
			if (m_LeftAxizY > 100)
			{
				dijs.rgbButtons[STICK_L_UP] = 0x80;
			}
			//左スティックが下に倒された
			if (m_LeftAxizY < -100)
			{
				dijs.rgbButtons[STICK_L_DOWN] = 0x80;
			}
			//左スティックが右に倒された
			if (m_LeftAxizX > 100)
			{
				dijs.rgbButtons[STICK_L_RIGHT] = 0x80;
			}
			//左スティックが左に倒された
			if (m_LeftAxizX < -100)
			{
				dijs.rgbButtons[STICK_L_LEFT] = 0x80;
			}


			//右スティックが上に倒された
			if (m_RightAxizY > 100)
			{
				dijs.rgbButtons[STICK_R_UP] = 0x80;
			}
			//右スティックが下に倒された
			if (m_RightAxizY < -100)
			{
				dijs.rgbButtons[STICK_R_DOWN] = 0x80;
			}
			//右スティックが右に倒された
			if (m_RightAxizX > 100)
			{
				dijs.rgbButtons[STICK_R_RIGHT] = 0x80;
			}
			//右スティックが左に倒された
			if (m_RightAxizX < -100)
			{
				dijs.rgbButtons[STICK_R_LEFT] = 0x80;
			}
			//十字キーが上に倒された
			if (dijs.rgdwPOV[0] == 0)
			{
				dijs.rgbButtons[POV_UP] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			//十字キーが下に倒された
			if (dijs.rgdwPOV[0] == 18000)
			{
				dijs.rgbButtons[POV_DOWN] = 0x80;
				dijs.rgdwPOV[0] /= 100;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			//十字キーが右に倒された
			if (dijs.rgdwPOV[0] == 9000)
			{
				dijs.rgbButtons[POV_RIGHT] = 0x80;
				dijs.rgdwPOV[0] /= 100;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			//十字キーが左に倒された
			if (dijs.rgdwPOV[0] == 27000)
			{
				dijs.rgbButtons[POV_LEFT] = 0x80;
				dijs.rgdwPOV[0] /= 100;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}


			//十字キーが右上に倒された
			if (dijs.rgdwPOV[0] == 4500)
			{
				dijs.rgbButtons[POV_UP_RIGHT] = 0x80;
				dijs.rgdwPOV[0] /= 100;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			//十字キーが右下に倒された
			if (dijs.rgdwPOV[0] == 13500)
			{
				dijs.rgbButtons[POV_DOWN_RIGHT] = 0x80;
				dijs.rgdwPOV[0] /= 100;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			//十字キーが左下に倒された
			if (dijs.rgdwPOV[0] == 22500)
			{
				dijs.rgbButtons[POV_DOWN_LEFT] = 0x80;
				dijs.rgdwPOV[0] /= 100;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			//十字キーが左下に倒された
			if (dijs.rgdwPOV[0] == 31500)
			{
				dijs.rgbButtons[POV_UP_LEFT] = 0x80;
				dijs.rgdwPOV[0] /= 100;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}

			for (int nCntKey = 0; nCntKey < NUM_BUTTON_MAX; nCntKey++)
			{
				m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ dijs.rgbButtons[nCntKey]) & m_aKeyState[nCntKey];
				m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ dijs.rgbButtons[nCntKey]) & dijs.rgbButtons[nCntKey];
				m_aKeyState[nCntKey] = dijs.rgbButtons[nCntKey];
			}
		}
		else
		{
			// キーボードへのアクセス権を獲得(入力制御開始)
			m_pDevice->Acquire();
		}
	}
}

//=============================================================================
// コントローラークラス入力情報(プレス情報)を取得
//=============================================================================
bool CInputJoypad::GetPress(int nKey)
{
	return(m_aKeyState[nKey] & 0x80) ? true : false;
}
//=============================================================================
// コントローラークラス入力情報(トリガー情報)を取得
//=============================================================================
bool CInputJoypad::GetTrigger(int nKey)
{
	return(m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}
//=============================================================================
// コントローラークラス入力情報(リリース情報)を取得
//=============================================================================
bool CInputJoypad::GetRelease(int nKey)
{
	return(m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//=============================================================================
// コントローラーの左スティックの向き
//=============================================================================
float CInputJoypad::GetLeftAxiz(void)
{
	float Axiz = atan2f(m_LeftAxizX, m_LeftAxizY);
	return Axiz;
}

//=============================================================================
// コントローラーの右スティックの向きを取得
//=============================================================================
float CInputJoypad::GetRightAxiz(void)
{
	float Axiz = atan2f(m_RightAxizX, m_RightAxizY);
	return Axiz;
}

//=============================================================================
// コントローラーの 十字キー取得
//=============================================================================
float CInputJoypad::GetRadian(void)
{
	return m_Radian;
}

//=============================================================================
// コントローラーの生成処理
//=============================================================================
CInputJoypad * CInputJoypad::Create(HINSTANCE hInstance, HWND hWnd)
{
	CInputJoypad *pCInputJoyPad = NULL;

	//NULLチェック
	if (pCInputJoyPad == NULL)
	{
		//動的確保
		pCInputJoyPad = new CInputJoypad;

		//NULLチェック
		if (pCInputJoyPad != NULL)
		{
			//初期化処理
			pCInputJoyPad->Init(hInstance, hWnd);
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

	return pCInputJoyPad;
}

bool CInputJoypad::GetJoyPadAny(int nNumber)
{
	if (nNumber == 1)
	{
		for (int nCount = 0; nCount < NUM_KEY_MAX; nCount++)
		{
			if (m_aKeyStateTrigger[nCount] == 0x80)
			{
				return true;
			}
		}
	}
	return false;
}

//*****************************************************************************
//    XInputの処理
//*****************************************************************************
int CInputXPad::m_nInputNum = 0;

//=============================================================================
// コンストラクタ									(public)	*** CInputXPad ***
//=============================================================================
CInputXPad::CInputXPad()
{
	// 初期値設定
	for (int nCntCont = 0; nCntCont < m_CONTROLLER; nCntCont++)
	{// コントローラー数カウント
		m_LStickRot[nCntCont] = 0.0f;		// Lスティック
		m_RStickRot[nCntCont] = 0.0f;		// Rスティック
	}

	m_nInputNum = 0;						// 入力数
}

//=============================================================================
// デストラクタ									(public)	*** CInputXPad ***
//=============================================================================
CInputXPad::~CInputXPad()
{

}

//=============================================================================
// 初期化処理									(public)	*** CInputXPad ***
//=============================================================================
HRESULT CInputXPad::Init(HINSTANCE hInstance, HWND hWnd)
{
	// コントローラーの初期化
	ZeroMemory(m_Controllers, sizeof(CONTROLER_STATE) * m_CONTROLLER);

	return S_OK;
}

//=============================================================================
// 終了処理										(public)	*** CInputXPad ***
//=============================================================================
void CInputXPad::Uninit(void)
{

}

//=============================================================================
// 更新処理										(public)	*** CInputXPad ***
//=============================================================================
void CInputXPad::Update(void)
{
	UpdateControllerState();	// 途中入力

	for (DWORD nCntCont = 0; nCntCont < m_CONTROLLER; nCntCont++)
	{// コントローラーカウント
		if (m_Controllers[nCntCont].bConnected == true)
		{// 接続されていたら
			if (m_bDeadZoneOn == true)
			{// スティック
			 // Zero value if thumbsticks are within the dead zone
				if ((m_Controllers[nCntCont].state.Gamepad.sThumbLX < INPUT_DEADZONE &&
					m_Controllers[nCntCont].state.Gamepad.sThumbLX > -INPUT_DEADZONE) &&
					(m_Controllers[nCntCont].state.Gamepad.sThumbLY < INPUT_DEADZONE &&
						m_Controllers[nCntCont].state.Gamepad.sThumbLY > -INPUT_DEADZONE))
				{
					m_Controllers[nCntCont].state.Gamepad.sThumbLX = 0;
					m_Controllers[nCntCont].state.Gamepad.sThumbLY = 0;
				}
				//m_Controllers[nCntCont].state.Gamepad.wButtons
				if ((m_Controllers[nCntCont].state.Gamepad.sThumbRX < INPUT_DEADZONE &&
					m_Controllers[nCntCont].state.Gamepad.sThumbRX > -INPUT_DEADZONE) &&
					(m_Controllers[nCntCont].state.Gamepad.sThumbRY < INPUT_DEADZONE &&
						m_Controllers[nCntCont].state.Gamepad.sThumbRY > -INPUT_DEADZONE))
				{
					m_Controllers[nCntCont].state.Gamepad.sThumbRX = 0;
					m_Controllers[nCntCont].state.Gamepad.sThumbRY = 0;
				}
			}

			WORD wButtons = 0;		// XINPUTコントローラーの入力情報

			for (int nCount = 0; nCount < XPADOTHER_MAX; nCount++)
			{// 入力情報カウント
				switch (nCount)
				{
				case 0:		// ボタン
					wButtons = m_Controllers[nCntCont].state.Gamepad.wButtons;
					break;
				case 1:		// 左トリガー
					wButtons = m_Controllers[nCntCont].state.Gamepad.bLeftTrigger;
					break;
				case 2:		// 右トリガー
					wButtons = m_Controllers[nCntCont].state.Gamepad.bRightTrigger;
					break;
				case 3:		// 左スティック上
					if (m_Controllers[nCntCont].state.Gamepad.sThumbLY > m_STICKMAX * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 4:		// 左スティック左
					if (m_Controllers[nCntCont].state.Gamepad.sThumbLX < m_STICKMIN * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 5:		// 左スティック右
					if (m_Controllers[nCntCont].state.Gamepad.sThumbLX > m_STICKMAX * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 6:		// 左スティック下
					if (m_Controllers[nCntCont].state.Gamepad.sThumbLY < m_STICKMIN * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 7:		// 右スティック上
					if (m_Controllers[nCntCont].state.Gamepad.sThumbRY > m_STICKMAX * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 8:		// 右スティック左
					if (m_Controllers[nCntCont].state.Gamepad.sThumbRX < m_STICKMIN * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 9:		// 右スティック右
					if (m_Controllers[nCntCont].state.Gamepad.sThumbRX > m_STICKMAX * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 10:		// 右スティック下
					if (m_Controllers[nCntCont].state.Gamepad.sThumbRY < m_STICKMIN * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				}

				// スティック向き
				m_LStickRot[nCntCont] = atan2f((float)m_Controllers[nCntCont].state.Gamepad.sThumbLX, (float)m_Controllers[nCntCont].state.Gamepad.sThumbLY);
				m_RStickRot[nCntCont] = atan2f((float)m_Controllers[nCntCont].state.Gamepad.sThumbRX, (float)m_Controllers[nCntCont].state.Gamepad.sThumbRY);

				// ジョイパッドの入力情報
				m_aJoyStateTrigger[nCntCont][nCount] = (m_aJoyStatePress[nCntCont][nCount] ^ wButtons) & wButtons;
				m_aJoyStateRelease[nCntCont][nCount] = (m_aJoyStatePress[nCntCont][nCount] ^ wButtons) & m_aJoyStatePress[nCntCont][nCount];
				m_aJoyStatePress[nCntCont][nCount] = wButtons;
			}
		}
	}
}

//=============================================================================
// Xパッドの入力情報(入力確認)を取得				(private)	*** CInputXPad ***
//=============================================================================
HRESULT CInputXPad::UpdateControllerState(void)
{
	DWORD dwResult;
	m_nInputNum = 0;
	for (DWORD nCntCont = 0; nCntCont < m_CONTROLLER; nCntCont++)
	{
		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState(nCntCont, &m_Controllers[nCntCont].state);

		if (dwResult == ERROR_SUCCESS)
		{
			m_Controllers[nCntCont].bConnected = true;
			m_nInputNum++;
		}
		else
		{
			m_Controllers[nCntCont].bConnected = false;
		}
	}

	return S_OK;
}
//=============================================================================
// Xパッドの入力情報(入力数情報)を取得				(private)	*** CInputXPad ***
//=============================================================================
int CInputXPad::GetInputNum(void)
{
	return m_nInputNum;
}

//=============================================================================
// Xパッドの入力情報(オール情報)を取得				(private)	*** CInputXPad ***
//=============================================================================
bool CInputXPad::GetALL(int nType, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
	{
		switch (nType)
		{
		case 0:
			for (int nCount = 0; nCount <= XPADOTHER_TRIGGER_RIGHT; nCount++)
			{// 入力情報カウント
				if (nCount == XPADOTHER_TRIGGER_LEFT || nCount == XPADOTHER_TRIGGER_RIGHT)
				{
					if (m_aJoyStatePress[nIdxPad][nCount] == 0x80)
						return true;
				}
				else
				{
					if (m_aJoyStatePress[nIdxPad][nCount] != 0)
						return true;
				}
			}

			break;
		case 1:
			for (int nCount = 0; nCount <= XPADOTHER_TRIGGER_RIGHT; nCount++)
			{// 入力情報カウント
				if (nCount == XPADOTHER_TRIGGER_LEFT || nCount == XPADOTHER_TRIGGER_RIGHT)
				{
					if (m_aJoyStateTrigger[nIdxPad][nCount] == 0x80)
						return true;
				}
				else
				{
					if (m_aJoyStateTrigger[nIdxPad][nCount] != 0)
						return true;
				}
			}

			break;
		default:
			for (int nCount = 0; nCount <= XPADOTHER_TRIGGER_RIGHT; nCount++)
			{// 入力情報カウント
				if (nCount == XPADOTHER_TRIGGER_LEFT || nCount == XPADOTHER_TRIGGER_RIGHT)
				{
					if (m_aJoyStateRelease[nIdxPad][nCount] == 0x80)
						return true;
				}
				else
				{
					if (m_aJoyStateRelease[nIdxPad][nCount] != 0)
						return true;
				}
			}

			break;
		}
	}

	return false;
}

//=============================================================================
// Xパッドの入力情報(プレス情報)を取得				(private)	*** CInputXPad ***
//=============================================================================
bool CInputXPad::GetPress(int nButton, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
		return (m_aJoyStatePress[nIdxPad][0] & nButton) ? true : false;
	else
		return false;
}
bool CInputXPad::GetPress(XPADOTHER nButton, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
		return (m_aJoyStatePress[nIdxPad][nButton] & 0x80) ? true : false;
	else
		return false;
}

//=============================================================================
// Xパッドの入力情報(トリガー情報)を取得			(private)	*** CInputXPad ***
//=============================================================================
bool CInputXPad::GetTrigger(int nButton, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
		return (m_aJoyStateTrigger[nIdxPad][0] & nButton) ? true : false;
	else
		return false;
}
bool CInputXPad::GetTrigger(XPADOTHER nButton, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
		return (m_aJoyStateTrigger[nIdxPad][nButton] & 0x80) ? true : false;
	else
		return false;
}

//=============================================================================
// Xパッドの入力情報(リリース情報)を取得			(private)	*** CInputXPad ***
//=============================================================================
bool CInputXPad::GetRelease(int nButton, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
		return (m_aJoyStateRelease[nIdxPad][0] & nButton) ? true : false;
	else
		return false;
}
bool CInputXPad::GetRelease(XPADOTHER nButton, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
		return (m_aJoyStateRelease[nIdxPad][nButton] & 0x80) ? true : false;
	else
		return false;
}

//=============================================================================
// Xパッドの入力情報(スティック情報)を取得			(private)	*** CInputXPad ***
//=============================================================================
bool CInputXPad::GetStick(int nLR, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
	{
		if (nLR == 0)
		{
			for (int nCntStick = XPADOTHER_STICK_L_UP; nCntStick <= XPADOTHER_STICK_L_DOWN; nCntStick++)
				if (m_aJoyStatePress[nIdxPad][nCntStick] == 0x80)
					return true;
		}
		else
		{
			for (int nCntStick = XPADOTHER_STICK_R_UP; nCntStick <= XPADOTHER_STICK_R_DOWN; nCntStick++)
				if (m_aJoyStatePress[nIdxPad][nCntStick] == 0x80)
					return true;
		}
	}

	return false;
}

//=============================================================================
// Xパッドの入力情報(スティック数値情報)を取得		(private)	*** CInputXPad ***
//=============================================================================
D3DXVECTOR2 CInputXPad::GetStickNum(int nLR, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
	{
		if (nLR == 0)
			return D3DXVECTOR2(m_Controllers[nIdxPad].state.Gamepad.sThumbLX, m_Controllers[nIdxPad].state.Gamepad.sThumbLY);
		else
			return D3DXVECTOR2(m_Controllers[nIdxPad].state.Gamepad.sThumbRX, m_Controllers[nIdxPad].state.Gamepad.sThumbRY);
	}

	return D3DXVECTOR2(0, 0);
}

//=============================================================================
// Xパッドの入力情報(スティック向き情報)を取得		(private)	*** CInputXPad ***
//=============================================================================
float CInputXPad::GetStickRot(int nLR, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
	{
		if (nLR == 0)
			return m_LStickRot[nIdxPad];
		else
			return m_RStickRot[nIdxPad];
	}

	return 0.0f;
}

//=============================================================================
// Xパッドの入力情報(スティック移動量情報)を取得	(private)	*** CInputXPad ***
//=============================================================================
D3DXVECTOR2 CInputXPad::GetStickMove(int nLR, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
	{
		if (nLR == 0)
			return D3DXVECTOR2(m_Controllers[nIdxPad].state.Gamepad.sThumbLX, m_Controllers[nIdxPad].state.Gamepad.sThumbLY);
		else
			return D3DXVECTOR2(m_Controllers[nIdxPad].state.Gamepad.sThumbRX, m_Controllers[nIdxPad].state.Gamepad.sThumbRY);
	}

	return D3DXVECTOR2(0.0f, 0.0f);
}

//=============================================================================
// ジョイパッドクラスのコンストラクタ
//=============================================================================
CInputJoyPad_0::CInputJoyPad_0()
{
	m_abConnection = false;
	m_nID = m_nNumPad;
	m_nNumPad++;
}
//=============================================================================
// ジョイパッドクラスのデストラクタ
//=============================================================================
CInputJoyPad_0::~CInputJoyPad_0()
{

}
//=============================================================================
// ジョイパッドクラスの接続判定処理
//=============================================================================
bool CInputJoyPad_0::Connect(void)
{
	DWORD dwResult;

	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	dwResult = XInputGetState(m_nID, &state);

	if (dwResult == ERROR_SUCCESS)
	{
		//MessageBox(0, "コントローラーが接続されています", "", MB_OK );
		// デッドゾーンの設定
		if ((state.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
			(state.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
		{
			state.Gamepad.sThumbLX = 0;
			state.Gamepad.sThumbLY = 0;
		}

		if ((state.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
			(state.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
				state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
		{
			state.Gamepad.sThumbRX = 0;
			state.Gamepad.sThumbRY = 0;
		}

		m_abConnection = true;
	}
	else
	{
		//MessageBox(0, "コントローラーが接続されていません", "", MB_OK);
		m_abConnection = false;
	}

	return m_abConnection;
}
//=============================================================================
// ジョイパッドクラスの初期化処理
//=============================================================================
void CInputJoyPad_0::Init(void)
{
	DWORD dwResult;

	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	dwResult = XInputGetState(m_nID, &state);

	// 協調モードを設定
	if (dwResult == ERROR_SUCCESS)
	{
		//MessageBox(0, "コントローラーが接続されています", "", MB_OK );
		// デッドゾーンの設定
		if ((state.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
			(state.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
		{
			state.Gamepad.sThumbLX = 0;
			state.Gamepad.sThumbLY = 0;
		}

		if ((state.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
			(state.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
				state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
		{
			state.Gamepad.sThumbRX = 0;
			state.Gamepad.sThumbRY = 0;
		}

		m_abConnection = true;
	}
	else
	{
		//MessageBox(0, "コントローラーが接続されていません", "", MB_OK);
		m_abConnection = false;
	}

	//入力情報の初期化
	for (int nCntButton = 0; nCntButton < JOYPAD_MAX_KEY; nCntButton++)
	{//ボタンの初期化
		m_bInput[nCntButton] = true;
		m_bButtonOld[nCntButton] = false;
		m_bButton[nCntButton] = false;
		m_nPress[nCntButton] = 0;
		m_nRelease[nCntButton] = 0;
		m_bButtonServer[nCntButton] = false;
	}
	m_nPress[LEFT_STICK] = 0; m_nPress[RIGHT_STICK] = 0;
	m_fStickAngle[LEFT_STICK] = 99.9f; m_fStickAngle[RIGHT_STICK] = 99.9f;
	m_bServerUp = false;
}

//=============================================================================
// ジョイパッドクラスの終了処理
//=============================================================================
void CInputJoyPad_0::Uninit(void)
{
	m_nNumPad--;
	XInputEnable(false);
	delete this;
}

//=============================================================================
// ジョイパッドクラスの更新処理
//=============================================================================
void CInputJoyPad_0::Update(void)
{
	bool bUpdate = false;
	if (CClient::GetpmyClient() == NULL) { bUpdate = true; }
	else
	{
		if (CManager::GetnInput() == 0) { bUpdate = true; }
	}

	if (bUpdate)
	{//更新可能なら
		for (int nCntButton = 0; nCntButton < JOYPAD_MAX_KEY; nCntButton++)
		{//毎F初期化
			m_bButtonOld[nCntButton] = m_bButton[nCntButton];
			m_bButton[nCntButton] = false;
			if (m_nPress[nCntButton] > 999999) { m_nPress[nCntButton] = 999; }
			if (m_nRelease[nCntButton] < -999999) { m_nRelease[nCntButton] = -999; }
		}
		m_bDefeat[LEFT_STICK] = false;
		m_bDefeat[RIGHT_STICK] = false;
		m_fStickAngle[LEFT_STICK] = -99.0f;
		m_fStickAngle[RIGHT_STICK] = -99.0f;

		m_nStickX[0] = 0;
		m_nStickX[1] = 0;
		m_nStickY[0] = 0;
		m_nStickY[1] = 0;
	}

	if (Connect())
	{//繋がっていれば
		if (CClient::GetpmyClient() != NULL) { m_bServerUp = true; }
		// ジョイパッド情報の取得
		XINPUT_STATE state;

		XInputGetState(m_nID, &state);

		//ボタン
		InputUpdate((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP ? true : false), 0);
		InputUpdate((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN ? true : false), 1);
		InputUpdate((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT ? true : false), 2);
		InputUpdate((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT ? true : false), 3);
		InputUpdate((state.Gamepad.wButtons & XINPUT_GAMEPAD_START ? true : false), 4);
		InputUpdate((state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK ? true : false), 5);
		InputUpdate(state.Gamepad.bLeftTrigger > 0, 6);
		InputUpdate(state.Gamepad.bRightTrigger > 0, 7);
		InputUpdate((state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER ? true : false), 8);
		InputUpdate((state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER ? true : false), 9);
		InputUpdate((state.Gamepad.wButtons & XINPUT_GAMEPAD_A ? true : false), 10);
		InputUpdate((state.Gamepad.wButtons & XINPUT_GAMEPAD_B ? true : false), 11);
		InputUpdate((state.Gamepad.wButtons & XINPUT_GAMEPAD_X ? true : false), 12);
		InputUpdate((state.Gamepad.wButtons & XINPUT_GAMEPAD_Y ? true : false), 13);
		//Lスティック
		InputUpdate(state.Gamepad.sThumbLX > 10000, 14);
		InputUpdate(state.Gamepad.sThumbLX < -10000, 15);
		InputUpdate(state.Gamepad.sThumbLY > 10000, 16);
		InputUpdate(state.Gamepad.sThumbLY < -10000, 17);
		//Rスティック
		InputUpdate(state.Gamepad.sThumbRX > 10000, 18);
		InputUpdate(state.Gamepad.sThumbRX < -10000, 19);
		InputUpdate(state.Gamepad.sThumbRY > 10000, 20);
		InputUpdate(state.Gamepad.sThumbRY < -10000, 21);

		if (bUpdate)
		{//更新可能なら
			m_nStickX[0] = state.Gamepad.sThumbLX;
			m_nStickX[1] = state.Gamepad.sThumbRX;
			m_nStickY[0] = state.Gamepad.sThumbLY;
			m_nStickY[1] = state.Gamepad.sThumbRY;
			StickUpdate();
		}
		//for (int nCntButton = 0; nCntButton < JOYPAD_MAX_KEY; nCntButton++)
		//{//入力制限解除
		//	if (GetTrigger(nCntButton)) { m_bInput[nCntButton] = true; }
		//}
	}
	m_bServerUp = false;
}
//=============================================================================
// ジョイパッド　入力時の更新処理
//=============================================================================
void CInputJoyPad_0::InputUpdate(bool bInput, int input)
{
	if (m_bServerUp)
	{
		if (bInput)
		{
			m_bButtonServer[input] = true;
		}
		return;
	}

	if (bInput)
	{
		m_bButton[input] = true;
		if (m_nPress[input] < 0) { m_nPress[input] = 0; }
		if (m_bInput[input]) { m_nPress[input]++; }
		m_nRelease[input] = 0;
	}
	else
	{
		if (m_nPress[input] > 0) { m_nPress[input] *= -1; }
		m_nRelease[input]--;
	}
}
//=============================================================================
// ジョイパッド　スティック更新処理
//=============================================================================
void CInputJoyPad_0::StickUpdate(void)
{
	if (GetStickDefeat(LEFT_STICK))
	{
		m_bDefeat[LEFT_STICK] = true;
		m_fStickAngle[LEFT_STICK] = atan2f((float)m_nStickX[0], (float)m_nStickY[0]);
	}
	if (GetStickDefeat(RIGHT_STICK))
	{
		m_bDefeat[RIGHT_STICK] = true;
		m_fStickAngle[RIGHT_STICK] = atan2f((float)m_nStickX[1], (float)m_nStickY[1]);
	}
}
//=============================================================================
// ジョイパッド　全ボタントリガー処理
//=============================================================================
bool CInputJoyPad_0::GetAllTrigger(void)
{
	for (int nCount = 0; nCount < INPUT_MAX; nCount++)
	{
		if ((m_bButtonOld[nCount] ? false : (m_bButton[nCount] ? true : false)))
		{
			return true;
		}
	}
	return false;
}
//=============================================================================
// ジョイパッド　トリガー処理
//=============================================================================
bool CInputJoyPad_0::GetTrigger(int input)
{
	if (CClient::GetpmyClient() == NULL) 
	{ 
		return (m_bButtonOld[input] ? false : (m_bButton[input] ? true : false));
	}
	else
	{
		if (CManager::GetnInput() == 0) 
		{ 
			return (m_bButtonOld[input] ? false : (m_bButton[input] ? true : false));
		}
	}
	return false;
}

//=============================================================================
// ジョイパッド　離したF数をリセット
//=============================================================================
void CInputJoyPad_0::ReSetPress(void)
{
	for (int nCntButton = 0; nCntButton < JOYPAD_MAX_KEY; nCntButton++)
	{//ボタンの初期化
		m_nPress[nCntButton] = 0;
	}
}
//=============================================================================
// インプットデータの送信用に代入
//=============================================================================
void CInputJoyPad_0::GetAllAssign(char *aAssign)
{
	char aStr[NUM_KEY_MAX * 2 + 8];
	char aWKStr[128];
	int nCntScan = 0;

	strcpy(&aStr[0], "");

	wsprintf(&aWKStr[0], "%d %d %d %d ", m_nStickX[0], m_nStickX[1], m_nStickY[0], m_nStickY[1]);
	//wsprintf(&aWKStr[0], "%d %d %d %d ", -10, -200, -3, -4000);

	strcpy(&aStr[0], &aWKStr[0]);
	//nCntScan += strlen(aWKStr);

	for (int nCntButton = 0; nCntButton < JOYPAD_MAX_KEY; nCntButton++)
	{//true::1 false::0 で代入
		if (m_bButtonServer[nCntButton])
		{
			strcat(&aStr[nCntScan], "1 ");
		}
		else { strcat(&aStr[nCntScan], "0 "); }
		//nCntScan += 2;
		//aStr[nCntScan + 1] = ' ';
		//aStr[nCntScan] = '1';
		//nCntScan += 2;
	}
	strcat(aAssign, &aStr[0]);
	strcat(aAssign, "A");
	for (int nCntButton = 0; nCntButton < JOYPAD_MAX_KEY; nCntButton++)
	{
		m_bButtonServer[nCntButton] = false;
	}
	return;
}
//=============================================================================
// インプットデータの反映
//=============================================================================
void CInputJoyPad_0::GetAllReflect(char *aReflect, int &nCntScan)
{
	int nButton;
	int nNumber;
	int nDefeat[2];
	char aStr[64];
	static int nCnt = 0;

	sscanf(&aReflect[nCntScan], "%d", &nNumber);
	nCntScan += 2;
	CInputJoyPad_0 *&pPad = CManager::GetInputJoyPad0(nNumber);

	for (int nCntButton = 0; nCntButton < JOYPAD_MAX_KEY; nCntButton++)
	{
		sscanf(&aReflect[nCntScan], "%d", &nButton);
		nCntScan += 2;

		pPad->InputUpdate((nButton == 1 ? true : false), nCntButton);
		if (pPad->GetTrigger(nCntButton))
		{
			nCnt++;
		}
	}

	return;
	sscanf(&aReflect[nCntScan], "%d %d %f %f "
		, &nDefeat[0], &nDefeat[1]
		, &pPad->m_fStickAngle[0], &pPad->m_fStickAngle[1]);
	if (nDefeat[0] == 1) { pPad->m_bDefeat[0] = true; }
	if (nDefeat[1] == 1) { pPad->m_bDefeat[1] = true; }
	snprintf(aStr, sizeof(aStr), "%f %f ", pPad->m_fStickAngle[LEFT_STICK], pPad->m_fStickAngle[RIGHT_STICK]);
	nCntScan += 4 + strlen(aStr);

	//CManager::GetDLog()->Printf(CDebugLog::MODE_LOG, "ランダムな数 %d\n", CServer::GetnRand());
}
//=============================================================================
// インプットデータの反映
//=============================================================================
void CInputJoyPad_0::GetReflect(char *aReflect, int nNumClient)	//インプットデータの反映
{
	int nButton;
	int nNumber;
	char aWKStr[1280];
	static int nCnt = 0;
	int nCntScan = 0;

	for (int nCount = 0; nCount < nNumClient; nCount++)
	{
		strcpy(&aWKStr[0], &aReflect[nCntScan]);
		sscanf(&aReflect[nCntScan], "%d", &nNumber);
		nCntScan += 2;
		//continue;

		CInputJoyPad_0 *&pPad = CManager::GetInputJoyPad0(nNumber);
		sscanf(&aReflect[nCntScan], "%d %d %d %d", &pPad->m_nStickX[0], &pPad->m_nStickX[1], &pPad->m_nStickY[0], &pPad->m_nStickY[1]);
		wsprintf(&aWKStr[0], "%d %d %d %d ", pPad->m_nStickX[0], pPad->m_nStickX[1], pPad->m_nStickY[0], pPad->m_nStickY[1]);

		nCntScan += strlen(aWKStr);

		//pPad->StickUpdate();

		for (int nCntButton = 0; nCntButton < JOYPAD_MAX_KEY; nCntButton++)
		{
			sscanf(&aReflect[nCntScan], "%d", &nButton);
			nCntScan += 2;

			pPad->InputUpdate((nButton == 1 ? true : false), nCntButton);
			if (pPad->GetTrigger(nCntButton))
			{
				nCnt++;
			}
		}

	}

	return;
}

