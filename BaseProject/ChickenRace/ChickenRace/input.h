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

#define INPUT_DEADZONE		( 0.24f * FLOAT(0x7FFF) )  // Default to 24% of the +/- 32767 range.   This is a reasonable default value but can be altered if needed.

//*****************************************************************************
// �}�N���̒�`
//*****************************************************************************
#define JOYPAD_MAX			(4)		// �W���C�p�b�h�̐ڑ���
#define JOYPAD_MAX_BUTTON	(14)	// �W���C�p�b�h�̃{�^����
#define JOYPAD_MAX_STICK	(2)		// �W���C�p�b�h�̓��͌�
#define LEFT_STICK			(0)		// �W���C�p�b�h��L�X�e�B�b�N�̔ԍ�
#define RIGHT_STICK			(1)		// �W���C�p�b�h��R�X�e�B�b�N�̔ԍ�
#define JOYPAD_MAX_KEY		(JOYPAD_MAX_BUTTON + JOYPAD_MAX_STICK * 4)	// �W���C�p�b�h�̓��͌�

//*****************************************************************************
// �񋓂̒�`
//*****************************************************************************
typedef enum
{//���͏��
	INPUT_UP, INPUT_DOWN, INPUT_LEFT, INPUT_RIGHT,
	INPUT_START, INPUT_BACK,
	INPUT_L2, INPUT_R2, INPUT_L1, INPUT_R1,
	INPUT_A, INPUT_B, INPUT_X, INPUT_Y,
	INPUT_LS_R, INPUT_LS_L, INPUT_LS_U, INPUT_LS_D,
	INPUT_RS_R, INPUT_RS_L, INPUT_RS_U, INPUT_RS_D,
	INPUT_MAX
}JOY_INPUT;

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
class CInputXPad
{// X�p�b�h
public:
	const static int m_CONTROLLER = 4;			// �R���g���[���̍ő吔
	const static int m_STICKMAX = 32767;		// �R���g���[���̍ő吔
	const static int m_STICKMIN = -32768;		// �R���g���[���̍ő吔

	typedef enum
	{// �{�^���ȊO�̃^�C�v
		XPADOTHER_BUTTON = 0,					// �{�^��
		XPADOTHER_TRIGGER_LEFT,					// L�g���K�[
		XPADOTHER_TRIGGER_RIGHT,				// R�g���K�[
		XPADOTHER_STICK_L_UP,					// L�X�e�B�b�N��
		XPADOTHER_STICK_L_LEFT,					// L�X�e�B�b�N��
		XPADOTHER_STICK_L_RIGHT,				// L�X�e�B�b�N�E
		XPADOTHER_STICK_L_DOWN,					// L�X�e�B�b�N��
		XPADOTHER_STICK_R_UP,					// R�X�e�B�b�N��
		XPADOTHER_STICK_R_LEFT,					// R�X�e�B�b�N��
		XPADOTHER_STICK_R_RIGHT,				// R�X�e�B�b�N�E
		XPADOTHER_STICK_R_DOWN,					// R�X�e�B�b�N��
		XPADOTHER_MAX
	}XPADOTHER;

	typedef struct
	{// XINPUT�R���g���[���[
		XINPUT_STATE state;		// ���
		bool bConnected;		// ���͗L��
	}CONTROLER_STATE;

	CInputXPad();										// �R���X�g���N�^
	virtual ~CInputXPad();								// �f�X�g���N�^

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);		// ����������
	void Uninit(void);									// �I������
	void Update(void);									// �X�V����

	static int GetInputNum(void);						// ���͐��擾

	bool GetALL(int nType, int nIdxPad);				// �I�[��
	bool GetPress(int nButton, int nIdxPad);			// �v���X
	bool GetPress(XPADOTHER nButton, int nIdxPad);		// �v���X�i���̑��j
	bool GetTrigger(int nButton, int nIdxPad);			// �g���K�[
	bool GetTrigger(XPADOTHER nButton, int nIdxPad);	// �g���K�[�i���̑��j
	bool GetRelease(int nButton, int nIdxPad);			// �����[�X
	bool GetRelease(XPADOTHER nButton, int nIdxPad);	// �����[�X�i���̑��j

	bool GetStick(int nLR, int nIdxPad);				// �X�e�B�b�N
	D3DXVECTOR2 GetStickNum(int nLR, int nIdxPad);		// �X�e�B�b�N���l
	float GetStickRot(int nLR, int nIdxPad);			// �X�e�B�b�N����
	D3DXVECTOR2 GetStickMove(int nLR, int nIdxPad);		// �X�e�B�b�N�ړ���

private:
	HRESULT UpdateControllerState(void);				// �R���g���[�����͐��X�V
	static int		m_nInputNum;						// �R���g���[�����͐�

	CONTROLER_STATE m_Controllers[m_CONTROLLER];		// �R���g���[��
	bool			m_bDeadZoneOn;						// �X�e�B�b�N

	WORD m_aJoyStatePress[m_CONTROLLER][XPADOTHER_MAX];		// �v���X	
	WORD m_aJoyStateTrigger[m_CONTROLLER][XPADOTHER_MAX];	// �g���K�[
	WORD m_aJoyStateRelease[m_CONTROLLER][XPADOTHER_MAX];	// �����[�X

	float m_LStickRot[m_CONTROLLER];					// L�X�e�B�b�N����
	float m_RStickRot[m_CONTROLLER];					// R�X�e�B�b�N����
};
//*****************************************************************************
// �N���X�̒�`
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

	void GetAllAssign(char *aAssign);			//�C���v�b�g�f�[�^�̑��M�p�ɑ��
	static void GetAllReflect(char *aReflect, int &nCntScan);	//�C���v�b�g�f�[�^�̔��f
	static void GetReflect(char *aReflect, int nNumClient);	//�C���v�b�g�f�[�^�̔��f

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
	{//�X�e�B�b�N���|��Ă����true
		int nPlus = nStick * 4;
		if (m_bButton[INPUT_LS_R + nPlus] || m_bButton[INPUT_LS_L + nPlus] || m_bButton[INPUT_LS_U + nPlus] || m_bButton[INPUT_LS_D + nPlus]) { return true; }
		return false;
	};

private:
	bool Connect(void);

	static int m_nNumPad;					//�W���C�p�b�h�̑���
	int  m_nID;								//�W���C�p�b�h�̔ԍ�
	bool m_abConnection;					// �W���C�p�b�h�̐ڑ����
	int		m_nStickX[2];
	int		m_nStickY[2];
	bool	m_bInput[JOYPAD_MAX_KEY];		//���͐����̗L��
	bool	m_bServerUp;					//�T�[�o�[�������̍X�V

	int	 m_nPress[JOYPAD_MAX_KEY];			//���������Ă���F��
	int	 m_nRelease[JOYPAD_MAX_KEY];		//������F��
	bool m_bButtonOld[JOYPAD_MAX_KEY];		//�O��̃{�^��
	bool m_bButton[JOYPAD_MAX_KEY];			//����̃{�^��
	bool m_bButtonServer[JOYPAD_MAX_KEY];	//�T�[�o�[�ɑ���t����{�^��
	bool m_bDefeat[JOYPAD_MAX_STICK];		//�X�e�B�b�N���|��Ă��邩
	float m_fStickAngle[JOYPAD_MAX_STICK];	//�X�e�B�b�N�̊p�x
};
#endif