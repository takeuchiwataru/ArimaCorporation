//=============================================================================
//
// 入力処理 [input.cpp]
// Author : 有馬武志 / 目黒未来也
//
//=============================================================================
#include "input.h"

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
//=============================================================================
// 生成処理
//=============================================================================
CXInput *CXInput::Create(void)
{
	CXInput *pXInput = NULL;

	if (pXInput == NULL)
	{// メモリの確保をする
		pXInput = new CXInput;

		pXInput->Init();
	}

	return pXInput;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CXInput::CXInput()
{
	// 各種値のクリア
	for (int nCntButton = 0; nCntButton < XIJS_BUTTON_MAX; nCntButton++)
	{// ボタンの数だけ繰り返し
		m_aGamePadState[nCntButton] = 0x00;			// プレス情報
		m_aGamePadStateTrigger[nCntButton] = 0x00;	// トリガー情報
		m_aGamePadStateRelease[nCntButton] = 0x00;	// リリース情報
		m_aGamePadStateRepeat[nCntButton] = 0x00;	// リピート情報
		m_aGamePadCounter[nCntButton] = 0;			// 入力カウンター
	}
	m_aGamePad.m_bConnected = false;				// 接続の状態
	m_aGamePad.m_State.Gamepad.wButtons = 0;		// ボタンのビット情報
	m_aGamePad.m_State.Gamepad.bLeftTrigger = 0;	// LTボタンの状態
	m_aGamePad.m_State.Gamepad.bRightTrigger = 0;	// RTボタンの状態
	m_aGamePad.m_State.Gamepad.sThumbLX = 0;		// 左スティックのX軸
	m_aGamePad.m_State.Gamepad.sThumbLY = 0;		// 左スティックのY軸
	m_aGamePad.m_State.Gamepad.sThumbRX = 0;		// 右スティックのX軸
	m_aGamePad.m_State.Gamepad.sThumbRY = 0;		// 右スティックのY軸

	m_bLeftTrigger = 0;								// LTボタンの状態
	m_bRightTrigger = 0;							// RTボタンの状態
	m_sThumbLX = 0;									// 左スティックのX軸の値
	m_sThumbLY = 0;									// 左スティックのY軸の値
	m_sThumbRX = 0;									// 右スティックのX軸の値
	m_sThumbRY = 0;									// 右スティックのY軸の値
	m_wButtons = 0;									// ボタンの状態
}

//=============================================================================
//    デストラクタ
//=============================================================================
CXInput::~CXInput()
{
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CXInput::Init(void)
{
	// ゲームパッドのクリア
	ZeroMemory(&m_aGamePad, sizeof(CONTROLER_STATE));

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CXInput::Uninit(void)
{
	// ゲームパッドの開放
	XInputEnable(false);
}

//=============================================================================
//    更新処理
//=============================================================================
void CXInput::Update(void)
{
	DWORD dwResult;                             // ゲームパッドが取得できたかどうか
	BYTE aGamePadState[XIJS_BUTTON_MAX] = {};   // ゲームパッドのボタン入力情報

	 // ゲームパッドの状態を取得
	dwResult = XInputGetState(0, &m_aGamePad.m_State);

	if (dwResult == ERROR_SUCCESS)
	{// 状態取得に成功した
		 // 状態を格納する
		m_aGamePad.m_bConnected = true;									// 接続された状態にする
		m_wButtons = m_aGamePad.m_State.Gamepad.wButtons;				// ボタンのビット情報を取得
		m_bLeftTrigger = m_aGamePad.m_State.Gamepad.bLeftTrigger;		// LTボタンの状態を取得
		m_bRightTrigger = m_aGamePad.m_State.Gamepad.bRightTrigger;		// RTボタンの状態を取得
		m_sThumbLX = m_aGamePad.m_State.Gamepad.sThumbLX;				// 左スティックのX軸を取得
		m_sThumbLY = m_aGamePad.m_State.Gamepad.sThumbLY;				// 左スティックのY軸を取得
		m_sThumbRX = m_aGamePad.m_State.Gamepad.sThumbRX;				// 右スティックのX軸を取得
		m_sThumbRY = m_aGamePad.m_State.Gamepad.sThumbRY;				// 右スティックのY軸を取得

		// 入力の状態を取得する
		// ボタン
		DWORD wButtons = m_wButtons;				// ボタンのビット情報
		if (wButtons >= XINPUT_GAMEPAD_Y)
		{// Yボタンが押されている
			aGamePadState[XIJS_BUTTON_13] = 0x80;	// 入力を起動
			wButtons -= XINPUT_GAMEPAD_Y;
		}
		if (wButtons >= XINPUT_GAMEPAD_X)
		{// Xボタンが押されている
			aGamePadState[XIJS_BUTTON_12] = 0x80;	// 入力を起動
			wButtons -= XINPUT_GAMEPAD_X;
		}
		if (wButtons >= XINPUT_GAMEPAD_B)
		{// Bボタンが押されている
			aGamePadState[XIJS_BUTTON_11] = 0x80;	// 入力を起動
			wButtons -= XINPUT_GAMEPAD_B;
		}
		if (wButtons >= XINPUT_GAMEPAD_A)
		{// Aボタンが押されている
			aGamePadState[XIJS_BUTTON_10] = 0x80;	// 入力を起動
			wButtons -= XINPUT_GAMEPAD_A;
		}
		if (wButtons >= XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{// RBボタンが押されている
			aGamePadState[XIJS_BUTTON_9] = 0x80;	// 入力を起動
			wButtons -= XINPUT_GAMEPAD_RIGHT_SHOULDER;
		}
		if (wButtons >= XINPUT_GAMEPAD_LEFT_SHOULDER)
		{// LBボタンが押されている
			aGamePadState[XIJS_BUTTON_8] = 0x80;	// 入力を起動
			wButtons -= XINPUT_GAMEPAD_LEFT_SHOULDER;
		}
		if (wButtons >= XINPUT_GAMEPAD_RIGHT_THUMB)
		{// 右スティックが押し込まれている
			aGamePadState[XIJS_BUTTON_7] = 0x80;	// 入力を起動
			wButtons -= XINPUT_GAMEPAD_RIGHT_THUMB;
		}
		if (wButtons >= XINPUT_GAMEPAD_LEFT_THUMB)
		{// 右スティックが押し込まれている
			aGamePadState[XIJS_BUTTON_6] = 0x80;	// 入力を起動
			wButtons -= XINPUT_GAMEPAD_LEFT_THUMB;
		}
		if (wButtons >= XINPUT_GAMEPAD_BACK)
		{// BACKボタンが押されている
			aGamePadState[XIJS_BUTTON_5] = 0x80;	// 入力を起動
			wButtons -= XINPUT_GAMEPAD_BACK;
		}
		if (wButtons >= XINPUT_GAMEPAD_START)
		{// BACKボタンが押されている
			aGamePadState[XIJS_BUTTON_4] = 0x80;	// 入力を起動
			wButtons -= XINPUT_GAMEPAD_START;
		}
		if (wButtons >= XINPUT_GAMEPAD_DPAD_RIGHT)
		{// 十字キーの右が押されている
			aGamePadState[XIJS_BUTTON_3] = 0x80;	// 入力を起動
			wButtons -= XINPUT_GAMEPAD_DPAD_RIGHT;
		}
		if (wButtons >= XINPUT_GAMEPAD_DPAD_LEFT)
		{// 十字キーの左が押されている
			aGamePadState[XIJS_BUTTON_2] = 0x80;	// 入力を起動
			wButtons -= XINPUT_GAMEPAD_DPAD_LEFT;
		}
		if (wButtons >= XINPUT_GAMEPAD_DPAD_DOWN)
		{// 十字キーの下が押されている
			aGamePadState[XIJS_BUTTON_1] = 0x80;	// 入力を起動
			wButtons -= XINPUT_GAMEPAD_DPAD_DOWN;
		}
		if (wButtons >= XINPUT_GAMEPAD_DPAD_UP)
		{// 十字キーの上が押されている
			aGamePadState[XIJS_BUTTON_0] = 0x80;	// 入力を起動
		}

		// LTトリガー
		if (m_bLeftTrigger >= MIN_GAMEPAD_LEFT_TRIGGER)
		{// LTトリガーが押されている
			aGamePadState[XIJS_BUTTON_14] = 0x80;	// 入力を起動する
		}
		// RTトリガー
		if (m_bRightTrigger >= MIN_GAMEPAD_RIGHT_TRIGGER)
		{// LRトリガーが押されている
			aGamePadState[XIJS_BUTTON_15] = 0x80;	// 入力を起動する
		}

		// 左スティック
		if (m_sThumbLX >= MIN_GAMEPAD_LEFT_THUMB_X)
		{// 左スティックが右に倒された
			aGamePadState[XIJS_BUTTON_19] = 0x80;	// 入力を起動する
		}
		if (m_sThumbLX <= -MIN_GAMEPAD_LEFT_THUMB_X)
		{// 左スティックが左に倒された
			aGamePadState[XIJS_BUTTON_18] = 0x80;	// 入力を起動する
		}
		if (m_sThumbLY >= MIN_GAMEPAD_LEFT_THUMB_Y)
		{// 左スティックが上に倒された
			aGamePadState[XIJS_BUTTON_16] = 0x80;	// 入力を起動する
		}
		if (m_sThumbLY <= -MIN_GAMEPAD_LEFT_THUMB_Y)
		{// 左スティックが下に倒された
			aGamePadState[XIJS_BUTTON_17] = 0x80;	// 入力を起動する
		}

		// 右スティック
		if (m_sThumbRX >= MIN_GAMEPAD_LEFT_THUMB_X)
		{// 右スティックが右に倒された
			aGamePadState[XIJS_BUTTON_23] = 0x80;	// 入力を起動する
		}
		if (m_sThumbRX <= -MIN_GAMEPAD_LEFT_THUMB_X)
		{// 右スティックが左に倒された
			aGamePadState[XIJS_BUTTON_22] = 0x80;	// 入力を起動する
		}
		if (m_sThumbRY >= MIN_GAMEPAD_LEFT_THUMB_Y)
		{// 右スティックが上に倒された
			aGamePadState[XIJS_BUTTON_20] = 0x80;	// 入力を起動する
		}
		if (m_sThumbRY <= -MIN_GAMEPAD_LEFT_THUMB_Y)
		{// 右スティックが下に倒された
			aGamePadState[XIJS_BUTTON_21] = 0x80;	// 入力を起動する
		}

		// ボタンの状態を格納する
		for (int nCntButton = 0; nCntButton < XIJS_BUTTON_MAX; nCntButton++)
		{// ボタンの数だけ繰り返し
			m_aGamePadStateTrigger[nCntButton] = (m_aGamePadState[nCntButton] ^ aGamePadState[nCntButton]) & aGamePadState[nCntButton];		// ボタンの入力情報(トリガー情報)保存
			m_aGamePadStateRelease[nCntButton] = (m_aGamePadState[nCntButton] ^ aGamePadState[nCntButton]) & m_aGamePadState[nCntButton];	// ボタンの入力情報(リリース情報)保存
			if (aGamePadState[nCntButton] == 0x80)
			{// ボタンが入力されている
				m_aGamePadCounter[nCntButton]++;	// カウンターを進める
				if (m_aGamePadCounter[nCntButton] >= REPEAT_FRAME || 1 == m_aGamePadCounter[nCntButton])
				{// 20フレーム分ボタンが押されている
					if (0 == m_aGamePadCounter[nCntButton] % REPEAT_TRIGGER || 1 == m_aGamePadCounter[nCntButton])
					{// 3フレームごと
						m_aGamePadStateRepeat[nCntButton] = 0x80;	// ボタンの入力情報(リピート情報)を起動
					}
					else
					{// それ以外のフレーム
						m_aGamePadStateRepeat[nCntButton] = 0;		// ボタンの入力情報(リピート情報)を一時停止
					}
				}
				else
				{// 20フレーム以前
					m_aGamePadStateRepeat[nCntButton] = 0;			// ボタンの入力情報(リピート情報)を一時停止
				}
			}
			else
			{// ゲームパッドのボタンが入力されていない
				m_aGamePadCounter[nCntButton] = 0;		// カウンターを戻す
				m_aGamePadStateRepeat[nCntButton] = 0;	// ボタンの入力情報(リピート情報)を停止
			}
			m_aGamePadState[nCntButton] = aGamePadState[nCntButton];	// ボタンの入力情報(プレス情報)保存
		}
	}
	else
	{// 状態取得に失敗した
		m_aGamePad.m_bConnected = false;	// 接続されていない状態にする
	}
}

//=============================================================================
//    ゲームパッドの入力情報を取得
//=============================================================================
bool CXInput::GetAnyButton(void)
{
	for (int nCount = 0; nCount < XIJS_BUTTON_16; nCount++)
	{// スティック以外のボタンを調べる
		if (m_aGamePadStateTrigger[nCount] == 0x80)
		{// 入力された
			return true;
		}
	}

	// 入力されたボタンがなかった
	return false;
}
