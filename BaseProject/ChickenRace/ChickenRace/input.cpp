//=============================================================================
//
// ���͏��� [input.cpp]
// Author : �L�n���u / �ڍ�������
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS	//Fire Wall�˔j
#include <stdio.h>				//�C���N���h�t�@�C��

#include "input.h"
#include "manager.h"
#include "Server.h"

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
int CInputJoyPad_0::m_nNumPad = 0;
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
int CInputXPad::m_nInputNum = 0;

//=============================================================================
// �R���X�g���N�^									(public)	*** CInputXPad ***
//=============================================================================
CInputXPad::CInputXPad()
{
	// �����l�ݒ�
	for (int nCntCont = 0; nCntCont < m_CONTROLLER; nCntCont++)
	{// �R���g���[���[���J�E���g
		m_LStickRot[nCntCont] = 0.0f;		// L�X�e�B�b�N
		m_RStickRot[nCntCont] = 0.0f;		// R�X�e�B�b�N
	}

	m_nInputNum = 0;						// ���͐�
}

//=============================================================================
// �f�X�g���N�^									(public)	*** CInputXPad ***
//=============================================================================
CInputXPad::~CInputXPad()
{

}

//=============================================================================
// ����������									(public)	*** CInputXPad ***
//=============================================================================
HRESULT CInputXPad::Init(HINSTANCE hInstance, HWND hWnd)
{
	// �R���g���[���[�̏�����
	ZeroMemory(m_Controllers, sizeof(CONTROLER_STATE) * m_CONTROLLER);

	return S_OK;
}

//=============================================================================
// �I������										(public)	*** CInputXPad ***
//=============================================================================
void CInputXPad::Uninit(void)
{

}

//=============================================================================
// �X�V����										(public)	*** CInputXPad ***
//=============================================================================
void CInputXPad::Update(void)
{
	UpdateControllerState();	// �r������

	for (DWORD nCntCont = 0; nCntCont < m_CONTROLLER; nCntCont++)
	{// �R���g���[���[�J�E���g
		if (m_Controllers[nCntCont].bConnected == true)
		{// �ڑ�����Ă�����
			if (m_bDeadZoneOn == true)
			{// �X�e�B�b�N
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

			WORD wButtons = 0;		// XINPUT�R���g���[���[�̓��͏��

			for (int nCount = 0; nCount < XPADOTHER_MAX; nCount++)
			{// ���͏��J�E���g
				switch (nCount)
				{
				case 0:		// �{�^��
					wButtons = m_Controllers[nCntCont].state.Gamepad.wButtons;
					break;
				case 1:		// ���g���K�[
					wButtons = m_Controllers[nCntCont].state.Gamepad.bLeftTrigger;
					break;
				case 2:		// �E�g���K�[
					wButtons = m_Controllers[nCntCont].state.Gamepad.bRightTrigger;
					break;
				case 3:		// ���X�e�B�b�N��
					if (m_Controllers[nCntCont].state.Gamepad.sThumbLY > m_STICKMAX * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 4:		// ���X�e�B�b�N��
					if (m_Controllers[nCntCont].state.Gamepad.sThumbLX < m_STICKMIN * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 5:		// ���X�e�B�b�N�E
					if (m_Controllers[nCntCont].state.Gamepad.sThumbLX > m_STICKMAX * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 6:		// ���X�e�B�b�N��
					if (m_Controllers[nCntCont].state.Gamepad.sThumbLY < m_STICKMIN * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 7:		// �E�X�e�B�b�N��
					if (m_Controllers[nCntCont].state.Gamepad.sThumbRY > m_STICKMAX * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 8:		// �E�X�e�B�b�N��
					if (m_Controllers[nCntCont].state.Gamepad.sThumbRX < m_STICKMIN * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 9:		// �E�X�e�B�b�N�E
					if (m_Controllers[nCntCont].state.Gamepad.sThumbRX > m_STICKMAX * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 10:		// �E�X�e�B�b�N��
					if (m_Controllers[nCntCont].state.Gamepad.sThumbRY < m_STICKMIN * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				}

				// �X�e�B�b�N����
				m_LStickRot[nCntCont] = atan2f((float)m_Controllers[nCntCont].state.Gamepad.sThumbLX, (float)m_Controllers[nCntCont].state.Gamepad.sThumbLY);
				m_RStickRot[nCntCont] = atan2f((float)m_Controllers[nCntCont].state.Gamepad.sThumbRX, (float)m_Controllers[nCntCont].state.Gamepad.sThumbRY);

				// �W���C�p�b�h�̓��͏��
				m_aJoyStateTrigger[nCntCont][nCount] = (m_aJoyStatePress[nCntCont][nCount] ^ wButtons) & wButtons;
				m_aJoyStateRelease[nCntCont][nCount] = (m_aJoyStatePress[nCntCont][nCount] ^ wButtons) & m_aJoyStatePress[nCntCont][nCount];
				m_aJoyStatePress[nCntCont][nCount] = wButtons;
			}
		}
	}
}

//=============================================================================
// X�p�b�h�̓��͏��(���͊m�F)���擾				(private)	*** CInputXPad ***
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
// X�p�b�h�̓��͏��(���͐����)���擾				(private)	*** CInputXPad ***
//=============================================================================
int CInputXPad::GetInputNum(void)
{
	return m_nInputNum;
}

//=============================================================================
// X�p�b�h�̓��͏��(�I�[�����)���擾				(private)	*** CInputXPad ***
//=============================================================================
bool CInputXPad::GetALL(int nType, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
	{
		switch (nType)
		{
		case 0:
			for (int nCount = 0; nCount <= XPADOTHER_TRIGGER_RIGHT; nCount++)
			{// ���͏��J�E���g
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
			{// ���͏��J�E���g
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
			{// ���͏��J�E���g
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
// X�p�b�h�̓��͏��(�v���X���)���擾				(private)	*** CInputXPad ***
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
// X�p�b�h�̓��͏��(�g���K�[���)���擾			(private)	*** CInputXPad ***
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
// X�p�b�h�̓��͏��(�����[�X���)���擾			(private)	*** CInputXPad ***
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
// X�p�b�h�̓��͏��(�X�e�B�b�N���)���擾			(private)	*** CInputXPad ***
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
// X�p�b�h�̓��͏��(�X�e�B�b�N���l���)���擾		(private)	*** CInputXPad ***
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
// X�p�b�h�̓��͏��(�X�e�B�b�N�������)���擾		(private)	*** CInputXPad ***
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
// X�p�b�h�̓��͏��(�X�e�B�b�N�ړ��ʏ��)���擾	(private)	*** CInputXPad ***
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
// �W���C�p�b�h�N���X�̃R���X�g���N�^
//=============================================================================
CInputJoyPad_0::CInputJoyPad_0()
{
	m_abConnection = false;
	m_nID = m_nNumPad;
	m_nNumPad++;
}
//=============================================================================
// �W���C�p�b�h�N���X�̃f�X�g���N�^
//=============================================================================
CInputJoyPad_0::~CInputJoyPad_0()
{

}
//=============================================================================
// �W���C�p�b�h�N���X�̐ڑ����菈��
//=============================================================================
bool CInputJoyPad_0::Connect(void)
{
	DWORD dwResult;

	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	dwResult = XInputGetState(m_nID, &state);

	if (dwResult == ERROR_SUCCESS)
	{
		//MessageBox(0, "�R���g���[���[���ڑ�����Ă��܂�", "", MB_OK );
		// �f�b�h�]�[���̐ݒ�
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
		//MessageBox(0, "�R���g���[���[���ڑ�����Ă��܂���", "", MB_OK);
		m_abConnection = false;
	}

	return m_abConnection;
}
//=============================================================================
// �W���C�p�b�h�N���X�̏���������
//=============================================================================
void CInputJoyPad_0::Init(void)
{
	DWORD dwResult;

	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	dwResult = XInputGetState(m_nID, &state);

	// �������[�h��ݒ�
	if (dwResult == ERROR_SUCCESS)
	{
		//MessageBox(0, "�R���g���[���[���ڑ�����Ă��܂�", "", MB_OK );
		// �f�b�h�]�[���̐ݒ�
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
		//MessageBox(0, "�R���g���[���[���ڑ�����Ă��܂���", "", MB_OK);
		m_abConnection = false;
	}

	//���͏��̏�����
	for (int nCntButton = 0; nCntButton < JOYPAD_MAX_KEY; nCntButton++)
	{//�{�^���̏�����
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
// �W���C�p�b�h�N���X�̏I������
//=============================================================================
void CInputJoyPad_0::Uninit(void)
{
	m_nNumPad--;
	XInputEnable(false);
	delete this;
}

//=============================================================================
// �W���C�p�b�h�N���X�̍X�V����
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
	{//�X�V�\�Ȃ�
		for (int nCntButton = 0; nCntButton < JOYPAD_MAX_KEY; nCntButton++)
		{//��F������
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
	{//�q�����Ă����
		if (CClient::GetpmyClient() != NULL) { m_bServerUp = true; }
		// �W���C�p�b�h���̎擾
		XINPUT_STATE state;

		XInputGetState(m_nID, &state);

		//�{�^��
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
		//L�X�e�B�b�N
		InputUpdate(state.Gamepad.sThumbLX > 10000, 14);
		InputUpdate(state.Gamepad.sThumbLX < -10000, 15);
		InputUpdate(state.Gamepad.sThumbLY > 10000, 16);
		InputUpdate(state.Gamepad.sThumbLY < -10000, 17);
		//R�X�e�B�b�N
		InputUpdate(state.Gamepad.sThumbRX > 10000, 18);
		InputUpdate(state.Gamepad.sThumbRX < -10000, 19);
		InputUpdate(state.Gamepad.sThumbRY > 10000, 20);
		InputUpdate(state.Gamepad.sThumbRY < -10000, 21);

		if (bUpdate)
		{//�X�V�\�Ȃ�
			m_nStickX[0] = state.Gamepad.sThumbLX;
			m_nStickX[1] = state.Gamepad.sThumbRX;
			m_nStickY[0] = state.Gamepad.sThumbLY;
			m_nStickY[1] = state.Gamepad.sThumbRY;
			StickUpdate();
		}
		//for (int nCntButton = 0; nCntButton < JOYPAD_MAX_KEY; nCntButton++)
		//{//���͐�������
		//	if (GetTrigger(nCntButton)) { m_bInput[nCntButton] = true; }
		//}
	}
	m_bServerUp = false;
}
//=============================================================================
// �W���C�p�b�h�@���͎��̍X�V����
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
// �W���C�p�b�h�@�X�e�B�b�N�X�V����
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
// �W���C�p�b�h�@�S�{�^���g���K�[����
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
// �W���C�p�b�h�@�g���K�[����
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
// �W���C�p�b�h�@������F�������Z�b�g
//=============================================================================
void CInputJoyPad_0::ReSetPress(void)
{
	for (int nCntButton = 0; nCntButton < JOYPAD_MAX_KEY; nCntButton++)
	{//�{�^���̏�����
		m_nPress[nCntButton] = 0;
	}
}
//=============================================================================
// �C���v�b�g�f�[�^�̑��M�p�ɑ��
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
	{//true::1 false::0 �ő��
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
// �C���v�b�g�f�[�^�̔��f
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

	//CManager::GetDLog()->Printf(CDebugLog::MODE_LOG, "�����_���Ȑ� %d\n", CServer::GetnRand());
}
//=============================================================================
// �C���v�b�g�f�[�^�̔��f
//=============================================================================
void CInputJoyPad_0::GetReflect(char *aReflect, int nNumClient)	//�C���v�b�g�f�[�^�̔��f
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

