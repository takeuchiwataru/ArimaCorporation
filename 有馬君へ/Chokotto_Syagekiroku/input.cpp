//=============================================================================
//
// ���͏��� [input.cpp]
// Author :Ryo Sugimoto
//
//=============================================================================
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	NUM_KEY_MAX			(256)			// �L�[�̍ő吔

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECTINPUT8 g_pInput = NULL;				//DIRECTINPUT�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;	//���̓f�o�C�X�ւ̃L�[�{�[�h�̃|�C���^
BYTE g_aKeyState[NUM_KEY_MAX];				//�L�[�{�[�h���͏��(�v���X���)
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];		//�L�[�{�[�h���͏��(�g���K�[���)

//=============================================================================
// �L�[�{�[�h�̏�����
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	// ���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard,
		&g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if(FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if(FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================
// �L�[�{�[�h�̏I������
//=============================================================================
void UninitKeyboard(void)
{
	// ���̓f�o�C�X(�L�[�{�[�h)�̊J��
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	// DirectInput�I�u�W�F�N�g�̊J��
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//=============================================================================
// �L�[�{�[�h�̍X�V����
//=============================================================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];
	int nCntKey;

	// ���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState),
		&aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			g_aKeyState[nCntKey] = aKeyState[nCntKey];	//�L�[�{�[�h���͏��ۑ�
			
			//g_aKeyStateTrigger[nCntKey] = ((g_aKeyState[nCntKey] + 1) * aKeyState[nCntKey]) % 2;
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();	//�L�[�{�[�h�ւ̃A�N�Z�X���l��
	}
}

//=============================================================================
// �L�[�{�[�h�̓��͏��(�v���X���)���擾
//=============================================================================
bool GetKeyboardPress(int nKey)
{	//�L�[�{�[�h�̓��͏��(�v���X���)���l��
		return(g_aKeyState[nKey] & 0x080) ? true : false;
}
//=============================================================================
// �L�[�{�[�h�̓��͏��(�g���K�[���)���擾
//=============================================================================
bool GetKeyboardTrigger(int nKey)
{	//�L�[�{�[�h�̓��͏��(�v���X���)���l��
	return(g_aKeyStateTrigger[nKey] & 0x080) ? true : false;
}
//=============================================================================
// �L�[�{�[�h�̓��͏��(Quick���)���擾
//=============================================================================
bool GetKeyboardQuick(int nKey, bool bUse)
{
	static int nQuick[NUM_KEY_MAX];		//�S�ẴL�[���擾�̂���
	static int nTime = 0;				//�A�����͐����p
	int nCntKeyboard;
	bool bQuick = false;

	if (bUse == false)
	{//�J�E���g�����͊m�F
		nTime--;
		for (nCntKeyboard = 0; nCntKeyboard < NUM_KEY_MAX; nCntKeyboard++)
		{//���Ԍo�ߗp
			nQuick[nCntKeyboard]--;
		}
	}
	else
	{
		if (GetKeyboardTrigger(nKey) == true)
		{//Trigger���͊m�F
			if (nQuick[nKey] > 0 && nTime < 0)
			{//2�����
				nQuick[nKey] = 0;
				bQuick = true;
				nTime = 35;	//�A�����͐��� 35F
			}
			else
			{//1�x��
				nQuick[nKey] = 10;
				bQuick = false;
			}
		}
	}

	return bQuick;
}
