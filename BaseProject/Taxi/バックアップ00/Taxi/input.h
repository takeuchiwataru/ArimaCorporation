//=============================================================================
//
// メイン処理 [input.h]
// Author : 有馬武志 / 目黒未来也
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define	NUM_KEY_MAX			(256)				// キーの最大数
#define	NUM_MOUSE_MAX		(8)					// マウスの最大数
#define MAX_JOYPAD			(3)					// 接続できるパッドの数
#define REPEAT_FRAME		(20)				// リピート情報を起動し始めるフレーム数の合計
#define REPEAT_TRIGGER		(6)					// リピート情報を起動するフレームの間隔
#define	NUM_BUTTON_MAX		(32)				// キーの最大数

//=====================
//	   基本クラス
//=====================
class CInput
{
public://誰でも扱える
	CInput();
	virtual ~CInput();
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;
protected://個人でのみ使う
	static LPDIRECTINPUT8 m_pInput;			//DirerctInputオブジェクトへのポインタ //全員が一個だけもっていればいいからstatic
	LPDIRECTINPUTDEVICE8 m_pDevice;			//入力デバイス（キーボード）へのポインタ
};

//=====================
//	   派生クラス
//=====================
class CInputKeyBoard : public CInput
{
public://誰でも扱える
	CInputKeyBoard();
	~CInputKeyBoard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetKeyboardPress(int nKey);		//キーボードプレス
	bool GetKeyboardTrigger(int nKey);		//キーボードトリガー
	bool GetKeyboardAny(int nNumber);		//キーボードトリガー
	static CInputKeyBoard *Create(HINSTANCE hInstance, HWND hWnd);
private://個人でのみ使う
	BYTE m_aKeyState[NUM_KEY_MAX];			//キーボードの入力情報（プレス情報）
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];	//キーボードの入力情報 (トリガー情報)
};

//=====================
//	   派生クラス
//=====================
class CInputMouse : public CInput
{
public://誰でも扱える
	CInputMouse();
	~CInputMouse();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetMousePress(int Key);			// マウスプレス
	bool GetMouseTrigger(int Key);			// マウストリガー
	bool GetMouseRelease(int Key);			// マウスリリース
	//DIMOUSESTATE2 GetMousePointer(void);
	static CInputMouse *Create(HINSTANCE hInstance, HWND hWnd);
	static D3DXVECTOR3 *GetMousePos(void) { return &m_MousePos; }

private://個人でのみ使う
	RECT					m_moveRect;				// 画面上で動ける範囲
	int						m_x;					// X座標
	int						m_y;					// Y座標
	bool					m_lButton;				// 左ボタン
	bool					m_rButton;				// 右ボタン
	bool					m_cButton;				// 真ん中ボタン
	int						m_moveAdd;				// 移動量
	RECT					m_imgRect;				// マウス用画像矩形
	int						m_imgWidth;				// マウス画像幅
	int						m_imgHeight;			// マウス画像高さ
	POINT					m_Pos, m_WPos;
	static D3DXVECTOR3		m_MousePos;
	DIMOUSESTATE2			m_MouseState;
	DIMOUSESTATE2			m_MouseStatePress;		// マウスの入力情報（プレス情報）
	DIMOUSESTATE2			m_MouseStateTrigger;	// マウスの入力情報（トリガー情報）
	DIMOUSESTATE2			m_MouseStateRelease;	// マウスの入力情報（リリース情報
};

//*********************************************************************
//コントローラーの定義
//*********************************************************************
//派生クラス
class CInputJoypad : public CInput
{
public:
	typedef enum
	{
		BUTTON_A = 0,
		BUTTON_B,
		BUTTON_X,
		BUTTON_Y,
		BUTTON_LB,
		BUTTON_RB,
		BUTTON_BACK,
		BUTTON_START,
		BUTTON_LS,
		BUTTON_RS,
		BUTTON_10,
		BUTTON_11,
		BUTTON_12,
		BUTTON_13,
		BUTTON_14,
		BUTTON_15,
		POV_UP_RIGHT,	//十字キー　右上
		POV_UP_LEFT,	//十字キー　左上
		POV_DOWN_RIGHT,	//十字キー　右下
		POV_DOWN_LEFT,	//十字キー　左下
		POV_UP,			//十字キー　上
		POV_DOWN,		//十字キー　下
		POV_RIGHT,		//十字キー　右
		POV_LEFT,		//十字キー　左
		STICK_L_UP,		//左スティック　上
		STICK_L_DOWN,	//左スティック　下
		STICK_L_RIGHT,	//左スティック　右
		STICK_L_LEFT,	//左スティック　左
		STICK_R_UP,		//右スティック　上
		STICK_R_DOWN,	//右スティック　下
		STICK_R_RIGHT,	//右スティック　右
		STICK_R_LEFT,	//右スティック　左
		MAX_BUTTON,
	}DIJS_BUTTON;
	CInputJoypad();
	~CInputJoypad();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit();
	void Update();
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
	bool GetJoyPadAny(int nKey);

	float GetLeftAxiz(void);
	float GetRightAxiz(void);
	float GetRadian(void);

	static CInputJoypad *Create(HINSTANCE hInstance, HWND hWnd);


protected:
	BYTE m_aKeyState[NUM_BUTTON_MAX];					//コントローラーの入力情報(プレス)
	BYTE m_aKeyStateTrigger[NUM_BUTTON_MAX];			//コントローラーの入力情報(トリガー)
	BYTE m_aKeyStateRelease[NUM_BUTTON_MAX];			//コントローラーの入力情報(リリース)
	float m_LeftAxizX;								//向き
	float m_LeftAxizY;								//向き
	float m_RightAxizX;								//向き
	float m_RightAxizY;								//向き
	float m_Radian;									//
};

//*****************************************************************************
//     XInputクラスの定義
//*****************************************************************************
class CXInput
{
public:     // 誰でもアクセス可能
#define MIN_GAMEPAD_LEFT_THUMB_X  (25000)   // 左スティックの横方向の入力を受け付ける軸の最小値
#define MIN_GAMEPAD_LEFT_THUMB_Y  (25000)   // 左スティックの縦方向の入力を受け付ける軸の最小値

#define MIN_GAMEPAD_RIGHT_THUMB_X (25000)   // 右スティックの横方向の入力を受け付ける軸の最小値
#define MIN_GAMEPAD_RIGHT_THUMB_Y (25000)   // 右スティックの縦方向の入力を受け付ける軸の最小値

#define MIN_GAMEPAD_LEFT_TRIGGER  (130)     // LTボタンの入力を受け付ける値の最小値
#define MIN_GAMEPAD_RIGHT_TRIGGER (130)     // RTボタンの入力を受け付ける値の最小値
	typedef enum
	{
		XIJS_BUTTON_0,     // 十字キー上入力
		XIJS_BUTTON_1,     // 十字キー下入力
		XIJS_BUTTON_2,     // 十字キー左入力
		XIJS_BUTTON_3,     // 十字キー右入力
		XIJS_BUTTON_4,     // STARTボタン
		XIJS_BUTTON_5,     // BACKボタン
		XIJS_BUTTON_6,     // 左スティック押し込み入力
		XIJS_BUTTON_7,     // 右スティック押し込み入力
		XIJS_BUTTON_8,     // LBボタン
		XIJS_BUTTON_9,     // RBボタン
		XIJS_BUTTON_10,    // Aボタン
		XIJS_BUTTON_11,    // Bボタン
		XIJS_BUTTON_12,    // Xボタン
		XIJS_BUTTON_13,    // Yボタン
		XIJS_BUTTON_14,    // LTボタン
		XIJS_BUTTON_15,    // RTボタン
		XIJS_BUTTON_16,    // 左スティック上入力
		XIJS_BUTTON_17,    // 左スティック下入力
		XIJS_BUTTON_18,    // 左スティック左入力
		XIJS_BUTTON_19,    // 左スティック右入力
		XIJS_BUTTON_20,    // 右スティック上入力
		XIJS_BUTTON_21,    // 右スティック下入力
		XIJS_BUTTON_22,    // 右スティック左入力
		XIJS_BUTTON_23,    // 右スティック右入力

						   // 以下割り当てなし
						   XIJS_BUTTON_24,
						   XIJS_BUTTON_25,
						   XIJS_BUTTON_26,
						   XIJS_BUTTON_27,
						   XIJS_BUTTON_28,
						   XIJS_BUTTON_29,
						   XIJS_BUTTON_30,
						   XIJS_BUTTON_31,
						   XIJS_BUTTON_MAX,
	}XIJS_BUTTON;

	CXInput();
	~CXInput();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	static CXInput *Create(void);

	BYTE GetLeftTrigger(void) { return m_bLeftTrigger; };
	BYTE GetRightTrigger(void) { return m_bRightTrigger; };
	float GetLeftAxiz(void) { return atan2f(m_sThumbLX, m_sThumbLY); };
	float GetRightAxiz(void) { return atan2f(m_sThumbRX, m_sThumbRY); };
	SHORT GetThumbLX(void) { return m_sThumbLX; };
	SHORT GetThumbLY(void) { return m_sThumbLY; };
	SHORT GetThumbRX(void) { return m_sThumbRX; };
	SHORT GetThumbRY(void) { return m_sThumbRY; };

	bool GetPress(XIJS_BUTTON Button) { return (m_aGamePadState[Button] & 0x80) ? true : false; };
	bool GetTrigger(XIJS_BUTTON Button) { return (m_aGamePadStateTrigger[Button] & 0x80) ? true : false; };
	bool GetRelease(XIJS_BUTTON Button) { return (m_aGamePadStateRelease[Button] & 0x80) ? true : false; };
	bool GetRepeat(XIJS_BUTTON Button) { return (m_aGamePadStateRepeat[Button] & 0x80) ? true : false; };
	bool GetAnyButton(void);

private:    // このクラスだけがアクセス可能
	typedef struct
	{
		XINPUT_STATE m_State;     // ゲームパッドの状態
		bool m_bConnected;        // 接続されたかどうか
	}CONTROLER_STATE;

	CONTROLER_STATE m_aGamePad;												// ゲームパッドの情報
	DWORD           m_wButtons;												// ボタンの状態
	BYTE            m_bLeftTrigger;											// LTボタンの状態
	BYTE            m_bRightTrigger;										// RTボタンの状態
	SHORT           m_sThumbLX;									// 左スティックのX軸
	SHORT           m_sThumbLY;									// 左スティックのY軸
	SHORT           m_sThumbRX;									// 右スティックのX軸
	SHORT           m_sThumbRY;									// 右スティックのY軸
	BYTE            m_aGamePadState[XIJS_BUTTON_MAX];			// ゲームパッドの入力情報(プレス情報)
	BYTE            m_aGamePadStateTrigger[XIJS_BUTTON_MAX];	// ゲームパッドの入力情報(トリガー情報)
	BYTE            m_aGamePadStateRelease[XIJS_BUTTON_MAX];	// ゲームパッドの入力情報(リリース情報)
	BYTE            m_aGamePadStateRepeat[XIJS_BUTTON_MAX];		// ゲームパッドの入力情報(リピート情報)
	DWORD           m_aGamePadCounter[XIJS_BUTTON_MAX];			// ゲームパッドの入力されてる間を数えるカウンター
};
#endif