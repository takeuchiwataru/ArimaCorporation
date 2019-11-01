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

#define INPUT_DEADZONE		( 0.24f * FLOAT(0x7FFF) )  // Default to 24% of the +/- 32767 range.   This is a reasonable default value but can be altered if needed.

//*****************************************************************************
// マクロの定義
//*****************************************************************************
#define JOYPAD_MAX			(4)		// ジョイパッドの接続数
#define JOYPAD_MAX_BUTTON	(14)	// ジョイパッドのボタン数
#define JOYPAD_MAX_STICK	(2)		// ジョイパッドの入力個数
#define LEFT_STICK			(0)		// ジョイパッドのLスティックの番号
#define RIGHT_STICK			(1)		// ジョイパッドのRスティックの番号
#define JOYPAD_MAX_KEY		(JOYPAD_MAX_BUTTON + JOYPAD_MAX_STICK * 4)	// ジョイパッドの入力個数

//*****************************************************************************
// 列挙の定義
//*****************************************************************************
typedef enum
{//入力情報
	INPUT_UP, INPUT_DOWN, INPUT_LEFT, INPUT_RIGHT,
	INPUT_START, INPUT_BACK,
	INPUT_L2, INPUT_R2, INPUT_L1, INPUT_R1,
	INPUT_A, INPUT_B, INPUT_X, INPUT_Y,
	INPUT_LS_R, INPUT_LS_L, INPUT_LS_U, INPUT_LS_D,
	INPUT_RS_R, INPUT_RS_L, INPUT_RS_U, INPUT_RS_D,
	INPUT_MAX
}JOY_INPUT;

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
class CInputXPad
{// Xパッド
public:
	const static int m_CONTROLLER = 4;			// コントローラの最大数
	const static int m_STICKMAX = 32767;		// コントローラの最大数
	const static int m_STICKMIN = -32768;		// コントローラの最大数

	typedef enum
	{// ボタン以外のタイプ
		XPADOTHER_BUTTON = 0,					// ボタン
		XPADOTHER_TRIGGER_LEFT,					// Lトリガー
		XPADOTHER_TRIGGER_RIGHT,				// Rトリガー
		XPADOTHER_STICK_L_UP,					// Lスティック上
		XPADOTHER_STICK_L_LEFT,					// Lスティック左
		XPADOTHER_STICK_L_RIGHT,				// Lスティック右
		XPADOTHER_STICK_L_DOWN,					// Lスティック下
		XPADOTHER_STICK_R_UP,					// Rスティック上
		XPADOTHER_STICK_R_LEFT,					// Rスティック左
		XPADOTHER_STICK_R_RIGHT,				// Rスティック右
		XPADOTHER_STICK_R_DOWN,					// Rスティック下
		XPADOTHER_MAX
	}XPADOTHER;

	typedef struct
	{// XINPUTコントローラー
		XINPUT_STATE state;		// 情報
		bool bConnected;		// 入力有無
	}CONTROLER_STATE;

	CInputXPad();										// コンストラクタ
	virtual ~CInputXPad();								// デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);		// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理

	static int GetInputNum(void);						// 入力数取得

	bool GetALL(int nType, int nIdxPad);				// オール
	bool GetPress(int nButton, int nIdxPad);			// プレス
	bool GetPress(XPADOTHER nButton, int nIdxPad);		// プレス（その他）
	bool GetTrigger(int nButton, int nIdxPad);			// トリガー
	bool GetTrigger(XPADOTHER nButton, int nIdxPad);	// トリガー（その他）
	bool GetRelease(int nButton, int nIdxPad);			// リリース
	bool GetRelease(XPADOTHER nButton, int nIdxPad);	// リリース（その他）

	bool GetStick(int nLR, int nIdxPad);				// スティック
	D3DXVECTOR2 GetStickNum(int nLR, int nIdxPad);		// スティック数値
	float GetStickRot(int nLR, int nIdxPad);			// スティック向き
	D3DXVECTOR2 GetStickMove(int nLR, int nIdxPad);		// スティック移動量

private:
	HRESULT UpdateControllerState(void);				// コントローラ入力数更新
	static int		m_nInputNum;						// コントローラ入力数

	CONTROLER_STATE m_Controllers[m_CONTROLLER];		// コントローラ
	bool			m_bDeadZoneOn;						// スティック

	WORD m_aJoyStatePress[m_CONTROLLER][XPADOTHER_MAX];		// プレス	
	WORD m_aJoyStateTrigger[m_CONTROLLER][XPADOTHER_MAX];	// トリガー
	WORD m_aJoyStateRelease[m_CONTROLLER][XPADOTHER_MAX];	// リリース

	float m_LStickRot[m_CONTROLLER];					// Lスティック向き
	float m_RStickRot[m_CONTROLLER];					// Rスティック向き
};
//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CInputJoyPad_0
{
public:
	CInputJoyPad_0();
	~CInputJoyPad_0();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void InputUpdate(bool bInput, int input);
	void StickUpdate(void);
	void Skip(void);

	void GetAllAssign(char *aAssign);			//インプットデータの送信用に代入
	static void GetAllReflect(char *aReflect, int &nCntScan);	//インプットデータの反映
	static void GetReflect(char *aReflect, int nNumClient);	//インプットデータの反映

	bool GetConnect(void) { return m_abConnection; };

	bool GetAllTrigger(void);
	bool GetPress(int input) { return m_bButton[input]; };
	bool GetTrigger(int input);
	bool GetTPress(int input) { if (m_nPress[input] < 30) { return GetTrigger(input); } else if (m_nPress[input] % 2 == 0) { return m_bButton[input]; } else { return false; } return false; };
	bool GetRelease(int input) { return (m_bButtonOld[input] ? (m_bButton[input] ? false : true) : false); };
	float GetfStickAngle(int nStick) { return m_fStickAngle[nStick]; };
	void ReSetPress(void);
	void NoInput(int input) { m_bInput[input] = false; m_nPress[input] = 0; }
	int &GetnPress(int input) { return m_nPress[input]; }
	int &GetnRelease(int input) { return m_nRelease[input]; }
	int &GetnStickX(int nStick) { return m_nStickX[nStick]; };
	int &GetnStickY(int nStick) { return m_nStickY[nStick]; };
	bool GetStickDefeat(int nStick)
	{//スティックが倒れていればtrue
		int nPlus = nStick * 4;
		if (m_bButton[INPUT_LS_R + nPlus] || m_bButton[INPUT_LS_L + nPlus] || m_bButton[INPUT_LS_U + nPlus] || m_bButton[INPUT_LS_D + nPlus]) { return true; }
		return false;
	};

private:
	bool Connect(void);

	static int m_nNumPad;					//ジョイパッドの総数
	int  m_nID;								//ジョイパッドの番号
	bool m_abConnection;					// ジョイパッドの接続状態
	int		m_nStickX[2];
	int		m_nStickY[2];
	bool	m_bInput[JOYPAD_MAX_KEY];		//入力制限の有無
	bool	m_bServerUp;					//サーバー生存時の更新

	int	 m_nPress[JOYPAD_MAX_KEY];			//押し続けているF数
	int	 m_nRelease[JOYPAD_MAX_KEY];		//離したF数
	bool m_bButtonOld[JOYPAD_MAX_KEY];		//前回のボタン
	bool m_bButton[JOYPAD_MAX_KEY];			//今回のボタン
	bool m_bButtonServer[JOYPAD_MAX_KEY];	//サーバーに送り付けるボタン
	bool m_bDefeat[JOYPAD_MAX_STICK];		//スティックが倒れているか
	float m_fStickAngle[JOYPAD_MAX_STICK];	//スティックの角度
};
#endif