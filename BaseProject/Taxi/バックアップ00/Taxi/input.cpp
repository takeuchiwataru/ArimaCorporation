//=============================================================================
//
// ���͏��� [input.cpp]
// Author : �L�n���u / �ڍ�������
//
//=============================================================================
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define DI_JOY_I_INPUT   (1000)  // �X�e�B�b�N�̕������͎�t���s���͈�

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECTINPUT8 CInput::m_pInput = NULL;
D3DXVECTOR3 CInputMouse::m_MousePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CInput::CInput()
{
	m_pDevice = NULL;
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CInput::~CInput()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	//NULL�`�F�b�N
	if (m_pInput == NULL)
	{//DirectInput�I�u�W�F�N�g�̐���
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CInput::Uninit(void)
{
	//���̓f�o�C�X(�L�[�{�[�h)�̊J��
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();	//�L�[�{�[�h�ւ̃A�N�Z�X�������
		m_pDevice->Release();
		m_pDevice = NULL;
	}
	//DirectInput�I�u�W�F�N�g�̊J��
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}
//=============================================================================
// �X�V����
//=============================================================================
void CInput::Update(void){}


//��������h���N���X�̃L�[�{�[�h


//===============================================================================
//�@�L�[�{�[�h�̃f�t�H���g�R���X�g���N�^
//===============================================================================
CInputKeyBoard::CInputKeyBoard()
{
	//�L�[���̃N���A
	for (int nCnt = 0; nCnt < NUM_KEY_MAX; nCnt++)
	{
		m_aKeyState[nCnt] = 0;			//�L�[�{�[�h�̓��͏��i�v���X���j
		m_aKeyStateTrigger[nCnt] = 0;	//�L�[�{�[�h�̓��͏�� (�g���K�[���)
	}
}
//===============================================================================
//�@�L�[�{�[�h�̃f�X�g���N�^
//===============================================================================
CInputKeyBoard::~CInputKeyBoard()
{

}
//=============================================================================
// �L�[�{�[�h�̏���������
//=============================================================================
HRESULT CInputKeyBoard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance,hWnd);

	//���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//�������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	m_pDevice->Acquire();

	return S_OK;
}
//=============================================================================
// �L�[�{�[�h�̏I������
//=============================================================================
void CInputKeyBoard::Uninit(void)
{
	CInput::Uninit();
}
//=============================================================================
// �L�[�{�[�h�̍X�V����
//=============================================================================
void CInputKeyBoard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];
	int nCntKey;

	//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];			//���g���K�[�̂Ƃ��͈�ԏ�
			m_aKeyState[nCntKey] = aKeyState[nCntKey];																//�L�[�{�[�h�̓��͏��ۑ�
		}
	}
	else
	{
		m_pDevice->Acquire();		//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	}
}
//===============================================================================
// �L�[�{�[�h�̃N���G�C�g
//===============================================================================
CInputKeyBoard *CInputKeyBoard::Create(HINSTANCE hInstance,HWND hWnd)
{
	CInputKeyBoard *pCInputKeyBoard = NULL;

	//NULL�`�F�b�N
	if (pCInputKeyBoard == NULL)
	{
		//���I�m��
		pCInputKeyBoard = new CInputKeyBoard;

		//NULL�`�F�b�N
		if (pCInputKeyBoard != NULL)
		{
			//����������
			pCInputKeyBoard->Init(hInstance, hWnd);
		}
		else
		{//�x����
			MessageBox(0, "�x���F���������Ȃ��ł�", "�x��", MB_OK);
		}
	}
	else
	{//�x����
		MessageBox(0, "�x���F�����������Ă܂�", "�x��", MB_OK);
	}

	return pCInputKeyBoard;
}
//=========================================================================================================================
// �L�[�{�[�h�̓��͏��(�v���X���)���擾
//=========================================================================================================================
bool CInputKeyBoard::GetKeyboardPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}
//=========================================================================================================================
// �L�[�{�[�h�̓��͏��(�g���K�[���)���擾
//=========================================================================================================================
bool CInputKeyBoard::GetKeyboardTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}
//=========================================================================================================================
// �L�[�{�[�h�̓��͏��(�S�{�^�����)���擾
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

//��������h���N���X�̃}�E�X


//===============================================================================
//�@�}�E�X�̃f�t�H���g�R���X�g���N�^
//===============================================================================
CInputMouse::CInputMouse()
{
	//�L�[���̃N���A
	for (int nCnt = 0; nCnt < NUM_MOUSE_MAX; nCnt++)
	{
		m_moveRect;					// ��ʏ�œ�����͈�
		m_x = 0;					// X���W
		m_y = 0;					// Y���W
		m_lButton = 0;				// ���{�^��
		m_rButton = 0;				// �E�{�^��
		m_cButton = 0;				// �^�񒆃{�^��
		m_moveAdd = 0;				// �ړ���
		m_imgRect;					// �}�E�X�p�摜��`
		m_imgWidth = 0;				// �}�E�X�摜��
		m_imgHeight = 0;			// �}�E�X�摜����
		m_MousePos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		m_Pos, m_WPos;
		m_MouseState;
		m_MouseStatePress = {};		// �}�E�X�̓��͏��i�v���X���j
		m_MouseStateTrigger = {};	// �}�E�X�̓��͏��i�g���K�[���j
		m_MouseStateRelease = {};	// �}�E�X�̓��͏��i�����[�X���
	}
}
//===============================================================================
//�@�}�E�X�̃f�X�g���N�^
//===============================================================================
CInputMouse::~CInputMouse()
{

}
//=============================================================================
// �}�E�X�̏���������
//=============================================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	// DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
	{
		return E_FAIL;
	}

	// ���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
	{
		return E_FAIL;
	}

	// �����[�h��ݒ�i���Βl���[�h�ɐݒ�j
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;
	//diprop.dwData = DIPROPAXISMODE_ABS; // ��Βl���[�h�̏ꍇ
	if (FAILED(m_pDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		return E_FAIL;
	}

	// �}�E�X�ւ̃A�N�Z�X�����l��(���͐���J�n)
	m_pDevice->Acquire();

	return S_OK;
}
//=============================================================================
// �}�E�X�̏I������
//=============================================================================
void CInputMouse::Uninit(void)
{
	CInput::Uninit();
}
//=============================================================================
// �}�E�X�̍X�V����
//=============================================================================
void CInputMouse::Update(void)
{
	//�ϐ��錾
	int nCntMouse;

	POINT pt;
	GetCursorPos(&pt);

	//POINT�^��D3DXVECTOR3�^�̕��ɑ������
	m_MousePos = D3DXVECTOR3((float)pt.x, (float)pt.y, 0.0f);

	//���̓f�o�C�X����f�[�^���擾
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
// �}�E�X�̃N���G�C�g
//===============================================================================
CInputMouse *CInputMouse::Create(HINSTANCE hInstance, HWND hWnd)
{
	CInputMouse *pCInputMouse = NULL;

	//NULL�`�F�b�N
	if (pCInputMouse == NULL)
	{
		//���I�m��
		pCInputMouse = new CInputMouse;

		//NULL�`�F�b�N
		if (pCInputMouse != NULL)
		{
			//����������
			pCInputMouse->Init(hInstance, hWnd);
		}
		else
		{//�x����
			MessageBox(0, "�x���F���������Ȃ��ł�", "�x��", MB_OK);
		}
	}
	else
	{//�x����
		MessageBox(0, "�x���F�����������Ă܂�", "�x��", MB_OK);
	}

	return pCInputMouse;
}
//=========================================================================================================================
// �}�E�X�̓��͏��(�v���X���)���擾
//=========================================================================================================================
bool CInputMouse::GetMousePress(int nKey)
{
	return (m_MouseStatePress.rgbButtons[nKey] & 0x80) ? true : false;
}

//=========================================================================================================================
// �}�E�X�̓��͏��(�g���K�[���)���擾
//=========================================================================================================================
bool CInputMouse::GetMouseTrigger(int nKey)
{
	return (m_MouseStateTrigger.rgbButtons[nKey] & 0x80) ? true : false;
}

//=========================================================================================================================
// �}�E�X�̓��͏��(�����[�X���)���擾
//=========================================================================================================================
bool CInputMouse::GetMouseRelease(int nKey)
{
	return (m_MouseStateRelease.rgbButtons[nKey] & 0x80) ? true : false;
}

//��������h���N���X�̃W���C�p�b�h
//=============================================================================
//�R���g���[���[�N���X �R���X�g���N�^
//=============================================================================
CInputJoypad::CInputJoypad()
{
	//�L�[�����N���A
	for (int nCnt = 0; nCnt < NUM_BUTTON_MAX; nCnt++)
	{
		m_aKeyState[nCnt] = 0;
		m_aKeyStateTrigger[nCnt] = 0;
	}
}

//=============================================================================
//�R���g���[���[�N���X �f�X�g���N�^
//=============================================================================
CInputJoypad::~CInputJoypad()
{
}

//=============================================================================
// �R���g���[���[�N���X������
//=============================================================================
HRESULT CInputJoypad::Init(HINSTANCE hInstance, HWND hWnd)
{
	//HWND = �E�B���h�E���A�N�e�B�u�̎��ɂ���
	//���������Ă���
	CInput::Init(hInstance, hWnd);

	HRESULT hr = m_pInput->CreateDevice(GUID_Joystick, &m_pDevice, NULL);

	//�G���[����
	/*if (FAILED(hr))
	{
		hr = m_pInput->CreateDevice(GUID_Joystick, &m_pDevice, NULL);
		if (FAILED(hr))
		{
			return E_FAIL;
		}
	}*/

	// ���̓f�o�C�X(�R���g���[���[)�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_Joystick,&m_pDevice,NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIJoystick)))
	{
		return E_FAIL;
	}
	// �������[�h��ݒ�	 *���͂��Ƃ���Ȃ�������ς��Ȃ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �R���g���[���[�̃A�N�Z�X�����l��(���͐���J�n)
	if (m_pDevice != NULL)
	{// �����ł���
		DIPROPRANGE diprg;

		// ���̒l�͈̔͂�ݒ�
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = -DI_JOY_I_INPUT;
		diprg.lMax = DI_JOY_I_INPUT;

		// ���̐ݒ�
		// ���A�i���O�X�e�B�b�N
		//X��
		diprg.diph.dwObj = DIJOFS_X;
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
		//Y��
		diprg.diph.dwObj = DIJOFS_Y;
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
		//Z��
		diprg.diph.dwObj = DIJOFS_Z;
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �E�A�i���O�X�e�B�b�N
		//RX��
		diprg.diph.dwObj = DIJOFS_RX;
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
		//RY��
		diprg.diph.dwObj = DIJOFS_RY;
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
		//RZ��
		diprg.diph.dwObj = DIJOFS_RZ;
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

		// ?
		//diprg.diph.dwObj = DIJOFS_SLIDER(0);
		//g_pDevJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �\���L�[(���0�x�Ƃ����v���Ɋp�x * 100)
		diprg.diph.dwObj = DIJOFS_POV(0);
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �W���C�X�e�B�b�N�ւ̃A�N�Z�X�����l��(���͐���J�n)
		m_pDevice->Acquire();
		//m_pDevice->Poll();
	}

	return S_OK;
}

//=============================================================================
// �R���g���[���[�N���X�I������
//=============================================================================
void CInputJoypad::Uninit(void)
{
	// ���̓f�o�C�X(�R���g���[���[)�̊J��
	CInput::Uninit();

	// DirectInput�I�u�W�F�N�g�̊J��
	//if (m_pInput != NULL)
	//{
	//	m_pInput->Release();
	//	m_pInput = NULL;
	//}

	//������Ă��Ȃ������Ƃ��ɏ���
	// ���̓f�o�C�X(�L�[�{�[�h)�̊J��
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}
}

//=============================================================================
// �R���g���[���[�N���X�X�V����
//=============================================================================
void CInputJoypad::Update(void)
{
	DIJOYSTATE dijs;

	if (m_pDevice != NULL)
	{
		m_pDevice->Poll();

		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(dijs), &dijs)))
		{
			// ���̓f�o�C�X����f�[�^(��)���擾
			m_LeftAxizX = (float)dijs.lX;
			m_LeftAxizY = (float)dijs.lY;
			m_RightAxizX = (float)dijs.lRx;
			m_RightAxizY = (float)dijs.lRy;

			//���X�e�B�b�N����ɓ|���ꂽ
			if (m_LeftAxizY > 100)
			{
				dijs.rgbButtons[STICK_L_UP] = 0x80;
			}
			//���X�e�B�b�N�����ɓ|���ꂽ
			if (m_LeftAxizY < -100)
			{
				dijs.rgbButtons[STICK_L_DOWN] = 0x80;
			}
			//���X�e�B�b�N���E�ɓ|���ꂽ
			if (m_LeftAxizX > 100)
			{
				dijs.rgbButtons[STICK_L_RIGHT] = 0x80;
			}
			//���X�e�B�b�N�����ɓ|���ꂽ
			if (m_LeftAxizX < -100)
			{
				dijs.rgbButtons[STICK_L_LEFT] = 0x80;
			}


			//�E�X�e�B�b�N����ɓ|���ꂽ
			if (m_RightAxizY > 100)
			{
				dijs.rgbButtons[STICK_R_UP] = 0x80;
			}
			//�E�X�e�B�b�N�����ɓ|���ꂽ
			if (m_RightAxizY < -100)
			{
				dijs.rgbButtons[STICK_R_DOWN] = 0x80;
			}
			//�E�X�e�B�b�N���E�ɓ|���ꂽ
			if (m_RightAxizX > 100)
			{
				dijs.rgbButtons[STICK_R_RIGHT] = 0x80;
			}
			//�E�X�e�B�b�N�����ɓ|���ꂽ
			if (m_RightAxizX < -100)
			{
				dijs.rgbButtons[STICK_R_LEFT] = 0x80;
			}
			//�\���L�[����ɓ|���ꂽ
			if (dijs.rgdwPOV[0] == 0)
			{
				dijs.rgbButtons[POV_UP] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			//�\���L�[�����ɓ|���ꂽ
			if (dijs.rgdwPOV[0] == 18000)
			{
				dijs.rgbButtons[POV_DOWN] = 0x80;
				dijs.rgdwPOV[0] /= 100;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			//�\���L�[���E�ɓ|���ꂽ
			if (dijs.rgdwPOV[0] == 9000)
			{
				dijs.rgbButtons[POV_RIGHT] = 0x80;
				dijs.rgdwPOV[0] /= 100;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			//�\���L�[�����ɓ|���ꂽ
			if (dijs.rgdwPOV[0] == 27000)
			{
				dijs.rgbButtons[POV_LEFT] = 0x80;
				dijs.rgdwPOV[0] /= 100;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}


			//�\���L�[���E��ɓ|���ꂽ
			if (dijs.rgdwPOV[0] == 4500)
			{
				dijs.rgbButtons[POV_UP_RIGHT] = 0x80;
				dijs.rgdwPOV[0] /= 100;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			//�\���L�[���E���ɓ|���ꂽ
			if (dijs.rgdwPOV[0] == 13500)
			{
				dijs.rgbButtons[POV_DOWN_RIGHT] = 0x80;
				dijs.rgdwPOV[0] /= 100;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			//�\���L�[�������ɓ|���ꂽ
			if (dijs.rgdwPOV[0] == 22500)
			{
				dijs.rgbButtons[POV_DOWN_LEFT] = 0x80;
				dijs.rgdwPOV[0] /= 100;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			//�\���L�[�������ɓ|���ꂽ
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
			// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
			m_pDevice->Acquire();
		}
	}
}

//=============================================================================
// �R���g���[���[�N���X���͏��(�v���X���)���擾
//=============================================================================
bool CInputJoypad::GetPress(int nKey)
{
	return(m_aKeyState[nKey] & 0x80) ? true : false;
}
//=============================================================================
// �R���g���[���[�N���X���͏��(�g���K�[���)���擾
//=============================================================================
bool CInputJoypad::GetTrigger(int nKey)
{
	return(m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}
//=============================================================================
// �R���g���[���[�N���X���͏��(�����[�X���)���擾
//=============================================================================
bool CInputJoypad::GetRelease(int nKey)
{
	return(m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//=============================================================================
// �R���g���[���[�̍��X�e�B�b�N�̌���
//=============================================================================
float CInputJoypad::GetLeftAxiz(void)
{
	float Axiz = atan2f(m_LeftAxizX, m_LeftAxizY);
	return Axiz;
}

//=============================================================================
// �R���g���[���[�̉E�X�e�B�b�N�̌������擾
//=============================================================================
float CInputJoypad::GetRightAxiz(void)
{
	float Axiz = atan2f(m_RightAxizX, m_RightAxizY);
	return Axiz;
}

//=============================================================================
// �R���g���[���[�� �\���L�[�擾
//=============================================================================
float CInputJoypad::GetRadian(void)
{
	return m_Radian;
}

//=============================================================================
// �R���g���[���[�̐�������
//=============================================================================
CInputJoypad * CInputJoypad::Create(HINSTANCE hInstance, HWND hWnd)
{
	CInputJoypad *pCInputJoyPad = NULL;

	//NULL�`�F�b�N
	if (pCInputJoyPad == NULL)
	{
		//���I�m��
		pCInputJoyPad = new CInputJoypad;

		//NULL�`�F�b�N
		if (pCInputJoyPad != NULL)
		{
			//����������
			pCInputJoyPad->Init(hInstance, hWnd);
		}
		else
		{//�x����
			MessageBox(0, "�x���F���������Ȃ��ł�", "�x��", MB_OK);
		}
	}
	else
	{//�x����
		MessageBox(0, "�x���F�����������Ă܂�", "�x��", MB_OK);
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
//    XInput�̏���
//*****************************************************************************
//=============================================================================
// ��������
//=============================================================================
CXInput *CXInput::Create(void)
{
	CXInput *pXInput = NULL;

	if (pXInput == NULL)
	{// �������̊m�ۂ�����
		pXInput = new CXInput;

		pXInput->Init();
	}

	return pXInput;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CXInput::CXInput()
{
	// �e��l�̃N���A
	for (int nCntButton = 0; nCntButton < XIJS_BUTTON_MAX; nCntButton++)
	{// �{�^���̐������J��Ԃ�
		m_aGamePadState[nCntButton] = 0x00;			// �v���X���
		m_aGamePadStateTrigger[nCntButton] = 0x00;	// �g���K�[���
		m_aGamePadStateRelease[nCntButton] = 0x00;	// �����[�X���
		m_aGamePadStateRepeat[nCntButton] = 0x00;	// ���s�[�g���
		m_aGamePadCounter[nCntButton] = 0;			// ���̓J�E���^�[
	}
	m_aGamePad.m_bConnected = false;				// �ڑ��̏��
	m_aGamePad.m_State.Gamepad.wButtons = 0;		// �{�^���̃r�b�g���
	m_aGamePad.m_State.Gamepad.bLeftTrigger = 0;	// LT�{�^���̏��
	m_aGamePad.m_State.Gamepad.bRightTrigger = 0;	// RT�{�^���̏��
	m_aGamePad.m_State.Gamepad.sThumbLX = 0;		// ���X�e�B�b�N��X��
	m_aGamePad.m_State.Gamepad.sThumbLY = 0;		// ���X�e�B�b�N��Y��
	m_aGamePad.m_State.Gamepad.sThumbRX = 0;		// �E�X�e�B�b�N��X��
	m_aGamePad.m_State.Gamepad.sThumbRY = 0;		// �E�X�e�B�b�N��Y��

	m_bLeftTrigger = 0;								// LT�{�^���̏��
	m_bRightTrigger = 0;							// RT�{�^���̏��
	m_sThumbLX = 0;									// ���X�e�B�b�N��X���̒l
	m_sThumbLY = 0;									// ���X�e�B�b�N��Y���̒l
	m_sThumbRX = 0;									// �E�X�e�B�b�N��X���̒l
	m_sThumbRY = 0;									// �E�X�e�B�b�N��Y���̒l
	m_wButtons = 0;									// �{�^���̏��
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CXInput::~CXInput()
{
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CXInput::Init(void)
{
	// �Q�[���p�b�h�̃N���A
	ZeroMemory(&m_aGamePad, sizeof(CONTROLER_STATE));

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CXInput::Uninit(void)
{
	// �Q�[���p�b�h�̊J��
	XInputEnable(false);
}

//=============================================================================
//    �X�V����
//=============================================================================
void CXInput::Update(void)
{
	DWORD dwResult;                             // �Q�[���p�b�h���擾�ł������ǂ���
	BYTE aGamePadState[XIJS_BUTTON_MAX] = {};   // �Q�[���p�b�h�̃{�^�����͏��

	 // �Q�[���p�b�h�̏�Ԃ��擾
	dwResult = XInputGetState(0, &m_aGamePad.m_State);

	if (dwResult == ERROR_SUCCESS)
	{// ��Ԏ擾�ɐ�������
		 // ��Ԃ��i�[����
		m_aGamePad.m_bConnected = true;									// �ڑ����ꂽ��Ԃɂ���
		m_wButtons = m_aGamePad.m_State.Gamepad.wButtons;				// �{�^���̃r�b�g�����擾
		m_bLeftTrigger = m_aGamePad.m_State.Gamepad.bLeftTrigger;		// LT�{�^���̏�Ԃ��擾
		m_bRightTrigger = m_aGamePad.m_State.Gamepad.bRightTrigger;		// RT�{�^���̏�Ԃ��擾
		m_sThumbLX = m_aGamePad.m_State.Gamepad.sThumbLX;				// ���X�e�B�b�N��X�����擾
		m_sThumbLY = m_aGamePad.m_State.Gamepad.sThumbLY;				// ���X�e�B�b�N��Y�����擾
		m_sThumbRX = m_aGamePad.m_State.Gamepad.sThumbRX;				// �E�X�e�B�b�N��X�����擾
		m_sThumbRY = m_aGamePad.m_State.Gamepad.sThumbRY;				// �E�X�e�B�b�N��Y�����擾

		// ���͂̏�Ԃ��擾����
		// �{�^��
		DWORD wButtons = m_wButtons;				// �{�^���̃r�b�g���
		if (wButtons >= XINPUT_GAMEPAD_Y)
		{// Y�{�^����������Ă���
			aGamePadState[XIJS_BUTTON_13] = 0x80;	// ���͂��N��
			wButtons -= XINPUT_GAMEPAD_Y;
		}
		if (wButtons >= XINPUT_GAMEPAD_X)
		{// X�{�^����������Ă���
			aGamePadState[XIJS_BUTTON_12] = 0x80;	// ���͂��N��
			wButtons -= XINPUT_GAMEPAD_X;
		}
		if (wButtons >= XINPUT_GAMEPAD_B)
		{// B�{�^����������Ă���
			aGamePadState[XIJS_BUTTON_11] = 0x80;	// ���͂��N��
			wButtons -= XINPUT_GAMEPAD_B;
		}
		if (wButtons >= XINPUT_GAMEPAD_A)
		{// A�{�^����������Ă���
			aGamePadState[XIJS_BUTTON_10] = 0x80;	// ���͂��N��
			wButtons -= XINPUT_GAMEPAD_A;
		}
		if (wButtons >= XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{// RB�{�^����������Ă���
			aGamePadState[XIJS_BUTTON_9] = 0x80;	// ���͂��N��
			wButtons -= XINPUT_GAMEPAD_RIGHT_SHOULDER;
		}
		if (wButtons >= XINPUT_GAMEPAD_LEFT_SHOULDER)
		{// LB�{�^����������Ă���
			aGamePadState[XIJS_BUTTON_8] = 0x80;	// ���͂��N��
			wButtons -= XINPUT_GAMEPAD_LEFT_SHOULDER;
		}
		if (wButtons >= XINPUT_GAMEPAD_RIGHT_THUMB)
		{// �E�X�e�B�b�N���������܂�Ă���
			aGamePadState[XIJS_BUTTON_7] = 0x80;	// ���͂��N��
			wButtons -= XINPUT_GAMEPAD_RIGHT_THUMB;
		}
		if (wButtons >= XINPUT_GAMEPAD_LEFT_THUMB)
		{// �E�X�e�B�b�N���������܂�Ă���
			aGamePadState[XIJS_BUTTON_6] = 0x80;	// ���͂��N��
			wButtons -= XINPUT_GAMEPAD_LEFT_THUMB;
		}
		if (wButtons >= XINPUT_GAMEPAD_BACK)
		{// BACK�{�^����������Ă���
			aGamePadState[XIJS_BUTTON_5] = 0x80;	// ���͂��N��
			wButtons -= XINPUT_GAMEPAD_BACK;
		}
		if (wButtons >= XINPUT_GAMEPAD_START)
		{// BACK�{�^����������Ă���
			aGamePadState[XIJS_BUTTON_4] = 0x80;	// ���͂��N��
			wButtons -= XINPUT_GAMEPAD_START;
		}
		if (wButtons >= XINPUT_GAMEPAD_DPAD_RIGHT)
		{// �\���L�[�̉E��������Ă���
			aGamePadState[XIJS_BUTTON_3] = 0x80;	// ���͂��N��
			wButtons -= XINPUT_GAMEPAD_DPAD_RIGHT;
		}
		if (wButtons >= XINPUT_GAMEPAD_DPAD_LEFT)
		{// �\���L�[�̍���������Ă���
			aGamePadState[XIJS_BUTTON_2] = 0x80;	// ���͂��N��
			wButtons -= XINPUT_GAMEPAD_DPAD_LEFT;
		}
		if (wButtons >= XINPUT_GAMEPAD_DPAD_DOWN)
		{// �\���L�[�̉���������Ă���
			aGamePadState[XIJS_BUTTON_1] = 0x80;	// ���͂��N��
			wButtons -= XINPUT_GAMEPAD_DPAD_DOWN;
		}
		if (wButtons >= XINPUT_GAMEPAD_DPAD_UP)
		{// �\���L�[�̏オ������Ă���
			aGamePadState[XIJS_BUTTON_0] = 0x80;	// ���͂��N��
		}

		// LT�g���K�[
		if (m_bLeftTrigger >= MIN_GAMEPAD_LEFT_TRIGGER)
		{// LT�g���K�[��������Ă���
			aGamePadState[XIJS_BUTTON_14] = 0x80;	// ���͂��N������
		}
		// RT�g���K�[
		if (m_bRightTrigger >= MIN_GAMEPAD_RIGHT_TRIGGER)
		{// LR�g���K�[��������Ă���
			aGamePadState[XIJS_BUTTON_15] = 0x80;	// ���͂��N������
		}

		// ���X�e�B�b�N
		if (m_sThumbLX >= MIN_GAMEPAD_LEFT_THUMB_X)
		{// ���X�e�B�b�N���E�ɓ|���ꂽ
			aGamePadState[XIJS_BUTTON_19] = 0x80;	// ���͂��N������
		}
		if (m_sThumbLX <= -MIN_GAMEPAD_LEFT_THUMB_X)
		{// ���X�e�B�b�N�����ɓ|���ꂽ
			aGamePadState[XIJS_BUTTON_18] = 0x80;	// ���͂��N������
		}
		if (m_sThumbLY >= MIN_GAMEPAD_LEFT_THUMB_Y)
		{// ���X�e�B�b�N����ɓ|���ꂽ
			aGamePadState[XIJS_BUTTON_16] = 0x80;	// ���͂��N������
		}
		if (m_sThumbLY <= -MIN_GAMEPAD_LEFT_THUMB_Y)
		{// ���X�e�B�b�N�����ɓ|���ꂽ
			aGamePadState[XIJS_BUTTON_17] = 0x80;	// ���͂��N������
		}

		// �E�X�e�B�b�N
		if (m_sThumbRX >= MIN_GAMEPAD_LEFT_THUMB_X)
		{// �E�X�e�B�b�N���E�ɓ|���ꂽ
			aGamePadState[XIJS_BUTTON_23] = 0x80;	// ���͂��N������
		}
		if (m_sThumbRX <= -MIN_GAMEPAD_LEFT_THUMB_X)
		{// �E�X�e�B�b�N�����ɓ|���ꂽ
			aGamePadState[XIJS_BUTTON_22] = 0x80;	// ���͂��N������
		}
		if (m_sThumbRY >= MIN_GAMEPAD_LEFT_THUMB_Y)
		{// �E�X�e�B�b�N����ɓ|���ꂽ
			aGamePadState[XIJS_BUTTON_20] = 0x80;	// ���͂��N������
		}
		if (m_sThumbRY <= -MIN_GAMEPAD_LEFT_THUMB_Y)
		{// �E�X�e�B�b�N�����ɓ|���ꂽ
			aGamePadState[XIJS_BUTTON_21] = 0x80;	// ���͂��N������
		}

		// �{�^���̏�Ԃ��i�[����
		for (int nCntButton = 0; nCntButton < XIJS_BUTTON_MAX; nCntButton++)
		{// �{�^���̐������J��Ԃ�
			m_aGamePadStateTrigger[nCntButton] = (m_aGamePadState[nCntButton] ^ aGamePadState[nCntButton]) & aGamePadState[nCntButton];		// �{�^���̓��͏��(�g���K�[���)�ۑ�
			m_aGamePadStateRelease[nCntButton] = (m_aGamePadState[nCntButton] ^ aGamePadState[nCntButton]) & m_aGamePadState[nCntButton];	// �{�^���̓��͏��(�����[�X���)�ۑ�
			if (aGamePadState[nCntButton] == 0x80)
			{// �{�^�������͂���Ă���
				m_aGamePadCounter[nCntButton]++;	// �J�E���^�[��i�߂�
				if (m_aGamePadCounter[nCntButton] >= REPEAT_FRAME || 1 == m_aGamePadCounter[nCntButton])
				{// 20�t���[�����{�^����������Ă���
					if (0 == m_aGamePadCounter[nCntButton] % REPEAT_TRIGGER || 1 == m_aGamePadCounter[nCntButton])
					{// 3�t���[������
						m_aGamePadStateRepeat[nCntButton] = 0x80;	// �{�^���̓��͏��(���s�[�g���)���N��
					}
					else
					{// ����ȊO�̃t���[��
						m_aGamePadStateRepeat[nCntButton] = 0;		// �{�^���̓��͏��(���s�[�g���)���ꎞ��~
					}
				}
				else
				{// 20�t���[���ȑO
					m_aGamePadStateRepeat[nCntButton] = 0;			// �{�^���̓��͏��(���s�[�g���)���ꎞ��~
				}
			}
			else
			{// �Q�[���p�b�h�̃{�^�������͂���Ă��Ȃ�
				m_aGamePadCounter[nCntButton] = 0;		// �J�E���^�[��߂�
				m_aGamePadStateRepeat[nCntButton] = 0;	// �{�^���̓��͏��(���s�[�g���)���~
			}
			m_aGamePadState[nCntButton] = aGamePadState[nCntButton];	// �{�^���̓��͏��(�v���X���)�ۑ�
		}
	}
	else
	{// ��Ԏ擾�Ɏ��s����
		m_aGamePad.m_bConnected = false;	// �ڑ�����Ă��Ȃ���Ԃɂ���
	}
}

//=============================================================================
//    �Q�[���p�b�h�̓��͏����擾
//=============================================================================
bool CXInput::GetAnyButton(void)
{
	for (int nCount = 0; nCount < XIJS_BUTTON_16; nCount++)
	{// �X�e�B�b�N�ȊO�̃{�^���𒲂ׂ�
		if (m_aGamePadStateTrigger[nCount] == 0x80)
		{// ���͂��ꂽ
			return true;
		}
	}

	// ���͂��ꂽ�{�^�����Ȃ�����
	return false;
}
