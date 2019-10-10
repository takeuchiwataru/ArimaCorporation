//=============================================================================
//
// ���C������ [input.h]
// Author : �L�n���u / �ڍ�������
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define	NUM_KEY_MAX			(256)				// �L�[�̍ő吔
#define	NUM_MOUSE_MAX		(8)					// �}�E�X�̍ő吔
#define MAX_JOYPAD			(3)					// �ڑ��ł���p�b�h�̐�
#define REPEAT_FRAME		(20)				// ���s�[�g�����N�����n�߂�t���[�����̍��v
#define REPEAT_TRIGGER		(6)					// ���s�[�g�����N������t���[���̊Ԋu
#define	NUM_BUTTON_MAX		(32)				// �L�[�̍ő吔

//=====================
//	   ��{�N���X
//=====================
class CInput
{
public://�N�ł�������
	CInput();
	virtual ~CInput();
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;
protected://�l�ł̂ݎg��
	static LPDIRECTINPUT8 m_pInput;			//DirerctInput�I�u�W�F�N�g�ւ̃|�C���^ //�S��������������Ă���΂�������static
	LPDIRECTINPUTDEVICE8 m_pDevice;			//���̓f�o�C�X�i�L�[�{�[�h�j�ւ̃|�C���^
};

//=====================
//	   �h���N���X
//=====================
class CInputKeyBoard : public CInput
{
public://�N�ł�������
	CInputKeyBoard();
	~CInputKeyBoard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetKeyboardPress(int nKey);		//�L�[�{�[�h�v���X
	bool GetKeyboardTrigger(int nKey);		//�L�[�{�[�h�g���K�[
	bool GetKeyboardAny(int nNumber);		//�L�[�{�[�h�g���K�[
	static CInputKeyBoard *Create(HINSTANCE hInstance, HWND hWnd);
private://�l�ł̂ݎg��
	BYTE m_aKeyState[NUM_KEY_MAX];			//�L�[�{�[�h�̓��͏��i�v���X���j
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];	//�L�[�{�[�h�̓��͏�� (�g���K�[���)
};

//=====================
//	   �h���N���X
//=====================
class CInputMouse : public CInput
{
public://�N�ł�������
	CInputMouse();
	~CInputMouse();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetMousePress(int Key);			// �}�E�X�v���X
	bool GetMouseTrigger(int Key);			// �}�E�X�g���K�[
	bool GetMouseRelease(int Key);			// �}�E�X�����[�X
	//DIMOUSESTATE2 GetMousePointer(void);
	static CInputMouse *Create(HINSTANCE hInstance, HWND hWnd);
	static D3DXVECTOR3 *GetMousePos(void) { return &m_MousePos; }

private://�l�ł̂ݎg��
	RECT					m_moveRect;				// ��ʏ�œ�����͈�
	int						m_x;					// X���W
	int						m_y;					// Y���W
	bool					m_lButton;				// ���{�^��
	bool					m_rButton;				// �E�{�^��
	bool					m_cButton;				// �^�񒆃{�^��
	int						m_moveAdd;				// �ړ���
	RECT					m_imgRect;				// �}�E�X�p�摜��`
	int						m_imgWidth;				// �}�E�X�摜��
	int						m_imgHeight;			// �}�E�X�摜����
	POINT					m_Pos, m_WPos;
	static D3DXVECTOR3		m_MousePos;
	DIMOUSESTATE2			m_MouseState;
	DIMOUSESTATE2			m_MouseStatePress;		// �}�E�X�̓��͏��i�v���X���j
	DIMOUSESTATE2			m_MouseStateTrigger;	// �}�E�X�̓��͏��i�g���K�[���j
	DIMOUSESTATE2			m_MouseStateRelease;	// �}�E�X�̓��͏��i�����[�X���
};

//*********************************************************************
//�R���g���[���[�̒�`
//*********************************************************************
//�h���N���X
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
		POV_UP_RIGHT,	//�\���L�[�@�E��
		POV_UP_LEFT,	//�\���L�[�@����
		POV_DOWN_RIGHT,	//�\���L�[�@�E��
		POV_DOWN_LEFT,	//�\���L�[�@����
		POV_UP,			//�\���L�[�@��
		POV_DOWN,		//�\���L�[�@��
		POV_RIGHT,		//�\���L�[�@�E
		POV_LEFT,		//�\���L�[�@��
		STICK_L_UP,		//���X�e�B�b�N�@��
		STICK_L_DOWN,	//���X�e�B�b�N�@��
		STICK_L_RIGHT,	//���X�e�B�b�N�@�E
		STICK_L_LEFT,	//���X�e�B�b�N�@��
		STICK_R_UP,		//�E�X�e�B�b�N�@��
		STICK_R_DOWN,	//�E�X�e�B�b�N�@��
		STICK_R_RIGHT,	//�E�X�e�B�b�N�@�E
		STICK_R_LEFT,	//�E�X�e�B�b�N�@��
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
	BYTE m_aKeyState[NUM_BUTTON_MAX];					//�R���g���[���[�̓��͏��(�v���X)
	BYTE m_aKeyStateTrigger[NUM_BUTTON_MAX];			//�R���g���[���[�̓��͏��(�g���K�[)
	BYTE m_aKeyStateRelease[NUM_BUTTON_MAX];			//�R���g���[���[�̓��͏��(�����[�X)
	float m_LeftAxizX;								//����
	float m_LeftAxizY;								//����
	float m_RightAxizX;								//����
	float m_RightAxizY;								//����
	float m_Radian;									//
};

//*****************************************************************************
//     XInput�N���X�̒�`
//*****************************************************************************
class CXInput
{
public:     // �N�ł��A�N�Z�X�\
#define MIN_GAMEPAD_LEFT_THUMB_X  (25000)   // ���X�e�B�b�N�̉������̓��͂��󂯕t���鎲�̍ŏ��l
#define MIN_GAMEPAD_LEFT_THUMB_Y  (25000)   // ���X�e�B�b�N�̏c�����̓��͂��󂯕t���鎲�̍ŏ��l

#define MIN_GAMEPAD_RIGHT_THUMB_X (25000)   // �E�X�e�B�b�N�̉������̓��͂��󂯕t���鎲�̍ŏ��l
#define MIN_GAMEPAD_RIGHT_THUMB_Y (25000)   // �E�X�e�B�b�N�̏c�����̓��͂��󂯕t���鎲�̍ŏ��l

#define MIN_GAMEPAD_LEFT_TRIGGER  (130)     // LT�{�^���̓��͂��󂯕t����l�̍ŏ��l
#define MIN_GAMEPAD_RIGHT_TRIGGER (130)     // RT�{�^���̓��͂��󂯕t����l�̍ŏ��l
	typedef enum
	{
		XIJS_BUTTON_0,     // �\���L�[�����
		XIJS_BUTTON_1,     // �\���L�[������
		XIJS_BUTTON_2,     // �\���L�[������
		XIJS_BUTTON_3,     // �\���L�[�E����
		XIJS_BUTTON_4,     // START�{�^��
		XIJS_BUTTON_5,     // BACK�{�^��
		XIJS_BUTTON_6,     // ���X�e�B�b�N�������ݓ���
		XIJS_BUTTON_7,     // �E�X�e�B�b�N�������ݓ���
		XIJS_BUTTON_8,     // LB�{�^��
		XIJS_BUTTON_9,     // RB�{�^��
		XIJS_BUTTON_10,    // A�{�^��
		XIJS_BUTTON_11,    // B�{�^��
		XIJS_BUTTON_12,    // X�{�^��
		XIJS_BUTTON_13,    // Y�{�^��
		XIJS_BUTTON_14,    // LT�{�^��
		XIJS_BUTTON_15,    // RT�{�^��
		XIJS_BUTTON_16,    // ���X�e�B�b�N�����
		XIJS_BUTTON_17,    // ���X�e�B�b�N������
		XIJS_BUTTON_18,    // ���X�e�B�b�N������
		XIJS_BUTTON_19,    // ���X�e�B�b�N�E����
		XIJS_BUTTON_20,    // �E�X�e�B�b�N�����
		XIJS_BUTTON_21,    // �E�X�e�B�b�N������
		XIJS_BUTTON_22,    // �E�X�e�B�b�N������
		XIJS_BUTTON_23,    // �E�X�e�B�b�N�E����

						   // �ȉ����蓖�ĂȂ�
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

private:    // ���̃N���X�������A�N�Z�X�\
	typedef struct
	{
		XINPUT_STATE m_State;     // �Q�[���p�b�h�̏��
		bool m_bConnected;        // �ڑ����ꂽ���ǂ���
	}CONTROLER_STATE;

	CONTROLER_STATE m_aGamePad;												// �Q�[���p�b�h�̏��
	DWORD           m_wButtons;												// �{�^���̏��
	BYTE            m_bLeftTrigger;											// LT�{�^���̏��
	BYTE            m_bRightTrigger;										// RT�{�^���̏��
	SHORT           m_sThumbLX;									// ���X�e�B�b�N��X��
	SHORT           m_sThumbLY;									// ���X�e�B�b�N��Y��
	SHORT           m_sThumbRX;									// �E�X�e�B�b�N��X��
	SHORT           m_sThumbRY;									// �E�X�e�B�b�N��Y��
	BYTE            m_aGamePadState[XIJS_BUTTON_MAX];			// �Q�[���p�b�h�̓��͏��(�v���X���)
	BYTE            m_aGamePadStateTrigger[XIJS_BUTTON_MAX];	// �Q�[���p�b�h�̓��͏��(�g���K�[���)
	BYTE            m_aGamePadStateRelease[XIJS_BUTTON_MAX];	// �Q�[���p�b�h�̓��͏��(�����[�X���)
	BYTE            m_aGamePadStateRepeat[XIJS_BUTTON_MAX];		// �Q�[���p�b�h�̓��͏��(���s�[�g���)
	DWORD           m_aGamePadCounter[XIJS_BUTTON_MAX];			// �Q�[���p�b�h�̓��͂���Ă�Ԃ𐔂���J�E���^�[
};
#endif