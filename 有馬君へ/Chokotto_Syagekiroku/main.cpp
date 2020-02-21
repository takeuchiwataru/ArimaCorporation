//=========================================================================================================//
//
// DirectX���^���� [main.cpp]
// Author : Ryou Sugimoto
//
//=========================================================================================================//
#include "main.h"
#include "input.h"
#include "game.h"
#include "enemy.h"
#include "Title.h"
#include "fade.h"
#include "sound.h"
#include <stdlib.h>				//�����h�p
#include <time.h>				//�����������h�p

//*********************************************************************************************************//
// �}�N����`
//*********************************************************************************************************//
#define CLASS_NAME		"WindowClass"		// �E�C���h�E�N���X�̖��O
#define WINDOW_NAME		"�`���R�b�g�ˌ��^"	// �E�C���h�E�̖��O(�L���v�V������)
#define WINDOW_WIDTH	(1280)				// ��ʃT�C�Y(��)
#define WINDOW_HEIGHT	(720)				// ��ʃT�C�Y(����)
#define MAX_RECAST		(2)					//�s���s�\�E�g�p�s�\���Ԃ̂���

//*********************************************************************************************************//
// �v���g�^�C�v�錾
//*********************************************************************************************************//
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
#ifdef _DEBUG
void DrawFPS(void);
#endif

//*********************************************************************************************************//
// �O���[�o���ϐ�
//*********************************************************************************************************//
LPDIRECT3D9 g_pD3D = NULL;				// Direct3D�I�u�W�F�N�g�̃|�C���^
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;	// Direct3D�f�o�C�X�̃|�C���^
#ifdef _DEBUG
LPD3DXFONT			g_pFont = NULL;			// �t�H���g�ւ̃|�C���^
int					g_nCountFPS = 0;		// FPS�J�E���^
#endif

//=========================================================================================================//
// ���C���֐�
//=========================================================================================================//
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	int nKey = 0;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WindowProc,
		0,
		0,
		hInstance,
		LoadIcon(NULL, IDI_APPLICATION),
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		LoadIcon(NULL, IDI_APPLICATION)
	};
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	HWND hWnd;				//�E�B���h�E�n���h��
	MSG msg;				//���b�Z�[�W���i�[����ϐ�
	DWORD dwCurrentTime;	//���ݎ���
	DWORD dwExecLastTime;	//�Ō�ɏ�����������

#ifdef _DEBUG
	DWORD dwFrameCount;
	DWORD dwFPSLastTime;
#endif

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	//	�w�肵���N���C�A���g�̈���m�ۂ��邽�߂ɕK�v�ȃE�C���h�E���W���v�Z
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// �E�B���h�E�̐���
	hWnd = CreateWindowEx(0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(rect.right - rect.left),
		(rect.bottom - rect.top),
		NULL,
		NULL,
		hInstance,
		NULL);

	// ����������(�E�B���h�E�𐶐����Ă���s��)
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	//�����@�\��ݒ�
	timeBeginPeriod(1);

	// �e�J�E���^�[��������
	dwCurrentTime = 0;
	dwExecLastTime = timeGetTime();		// �V�X�e���������擾(�~���b�P��)
#ifdef _DEBUG
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();		// �V�X�e���������擾(�~���b�P��)
#endif

	// �E�C���h�E�̕\��(�����������̌�ɍs��)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// ���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)	// ���b�Z�[�W���擾���Ȃ������ꍇ"0"��Ԃ�
		{	// Windows�̏���
			if (msg.message == WM_QUIT)
			{	// "WM_QUIT"���b�Z�[�W�������Ă����烋�[�v�𔲂���
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��Ƒ��o
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{	// DirectX�̏���
			dwCurrentTime = timeGetTime();	//���ݎ������擾
			
#ifdef _DEBUG
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5�b�o��
			 // FPS�𑪒�
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;		// FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;					// �J�E���g���N���A
			}
#endif

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60����1�b
				dwExecLastTime = dwCurrentTime;		//��������������ۑ�
				// �X�V����
				Update();

				// �`�揈��
				Draw();

#ifdef _DEBUG
				dwFrameCount++;		// �J�E���g�����Z
#endif

			}
		}
	}	// while����

		// �I������
	Uninit();

	//�����@�\��߂�
	timeEndPeriod(1);
	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//=========================================================================================================//
// �E�C���h�E�v���V�[�W��
//=========================================================================================================//
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch (uMsg)
	{
	case WM_DESTROY:
		// "WM_QUIT"���b�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;

	case WM_CLOSE:
		nID = MessageBox(hWnd, "�{���ɏI�����܂����H", "�I��", MB_YESNO);
		if (nID == IDYES)
		{	// �E�B���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)
			DestroyWindow(hWnd);
		}
		else
		{// NO�������ꂽ�ꍇ
			return 0;	// 0��Ԃ��Ȃ��ƏI�����Ă��܂�
		}
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:	// [ESC]�L�[�������ꂽ
						// �E�B���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)
			DestroyWindow(hWnd);
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);	// ����̏�����Ԃ�
}

//=========================================================================================================//
// ����������
//=========================================================================================================//
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// �f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;	// �v���[���e�[�V�����p�����[�^
	srand((unsigned int)time(0));
									// Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ���[�N���[���N���A
	d3dpp.BackBufferWidth = WINDOW_WIDTH;				// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = WINDOW_HEIGHT;				// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;				// �o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;							// �o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// �_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	d3dpp.EnableAutoDepthStencil = TRUE;						// �f�v�X�o�b�t�@(�y�o�b�t�@)�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;						// �E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g(���݂̑��x�ɍ��킹��)
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��(VSync��҂��ĕ`��)

	// Direct3D�f�o�C�X�̐���
	// [�f�o�C�X�쐬����]<�`��>��<���_����>���n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,						// �f�B�X�v���C�A�_�v�^
		D3DDEVTYPE_HAL,							// �f�o�C�X�^�C�v
		hWnd,									// �t�H�[�J�X����E�C���h�E�ւ̃n���h��
		D3DCREATE_HARDWARE_VERTEXPROCESSING,	// �f�o�C�X�쐬����̑g�ݍ��킹
		&d3dpp,									// �f�o�C�X�̃v���[���e�[�V�����p�����[�^
		&g_pD3DDevice)))						// �f�o�C�X�C���^�[�t�F�[�X�ւ̃|�C���^
	{
		// ��L�̐ݒ肪���s������
		// [�f�o�C�X�쐬����]<�`��>���n�[�h�E�F�A�ōs���A<���_����>��CPU�ōs�Ȃ�
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// [�f�o�C�X�쐬����]<�`��>��<���_����>��CPU�ōs�Ȃ�
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}
	// �����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// TEXTURE�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// �T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k�����̕�Ԑݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g�厞�̕�Ԑݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�

#ifdef _DEBUG
	// �f�o�b�O�\���p�t�H���g��ݒ�
	// pDevice         : �f�o�C�X�ւ̃|�C���^
	// Height          : �����̍���
	// Width           : �����̕�
	// Weight          : �t�H���g�̑���
	// MipLevels       : �~�b�v�}�b�v���x����
	// Italic          : �C�^���b�N�t�H���g
	// CharSet         : �t�H���g�̕����Z�b�g
	// OutputPrecision : ���ۂ̃t�H���g�ƖړI�̃t�H���g�̃T�C�Y����ѓ����̈�v���@���w��
	// Quality         : ���ۂ̃t�H���g�ƖړI�̃t�H���g�Ƃ̈�v���@���w��
	// PitchAndFamily  : �s�b�`�ƃt�@�~���C���f�b�N�X
	// pFacename       : �t�H���g�̖��O��ێ����镶����
	// ppFont          : �t�H���g�ւ̃|�C���^
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFont);
#endif

	// �e��I�u�W�F�N�g�̏���������
	InitKeyboard(hInstance, hWnd);
	InitSound(hWnd);

	MODE *Mode;	//���݂̃��[�h�m�F�̂���

	Mode = GetMode();	//���݂̃��[�h�����炤
	srand((unsigned int)time(0));	//�����h�̂���

	// �e��I�u�W�F�N�g�̍X��������
	
	InitFade(Mode[0]);
	InitWindow();
	InitFadeWindow();
	InitBGWindow();
	InitAnimation();
	switch (Mode[0])
	{
	case MODE_TITLE:
		InitTitle();
		break;

	case MODE_RANKING:
		InitRanking();
		break;

	case MODE_WATCH:
		InitWatch();
		break;

	case MODE_TUTORIAL:
		InitTutorial();
		break;

	case MODE_GAME:
		InitGame();
		break;

	case MODE_RESOLT:
		InitResult();
		break;
	}

	HWND *hwnd;
	hwnd = GethWnd();
	hwnd[0] = hWnd;

	return S_OK;
}

//=========================================================================================================//
// �I������
//=========================================================================================================//
void Uninit(void)
{
	// �e��I�u�W�F�N�g�̏I������
	UninitKeyboard();

#ifdef _DEBUG
	if (g_pFont != NULL)
	{// �f�o�b�O�\���p�t�H���g�̊J��
		g_pFont->Release();
		g_pFont = NULL;
	}
#endif

	// Direct3D�f�o�C�X�̊J��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	// Direct3D�I�u�W�F�N�g�̊J��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	MODE *Mode;	//���݂̃��[�h�m�F�̂���

	Mode = GetMode();	//���݂̃��[�h�����炤

	// �e��I�u�W�F�N�g�̍X�V����
	UninitKeyboard();
	UninitSound();
	UninitAnimation();

	switch (Mode[0])
	{
	case MODE_TITLE:
		UninitTitle();
		break;

	case MODE_RANKING:
		UninitRanking();
		break;

	case MODE_WATCH:
		UninitWatch();
		break;

	case MODE_TUTORIAL:
	UninitTutorial();
		break;

	case MODE_GAME:
		UninitGame();
		break;

	case MODE_RESOLT:
		UninitResult();
		break;
	}
	UninitWindow();
	UninitBGWindow();
	UninitFade();
	UninitFadeWindow();
}

//=========================================================================================================//
// �X�V����
//=========================================================================================================//
void Update(void)
{
	MODE *Mode;	//���݂̃��[�h�m�F�̂���

	Mode = GetMode();	//���݂̃��[�h�����炤

	// �e��I�u�W�F�N�g�̍X�V����
	UpdateKeyboard();

	switch (Mode[0])
	{
	case MODE_TITLE:
		UpdateTitle();
		break;

	case MODE_RANKING:
		UpdateRanking();
		break;

	case MODE_WATCH:
		UpdateWatch();
		break;

	case MODE_TUTORIAL:
		UpdateTutorial();
		break;

	case MODE_GAME:
		UpdateGame();
		break;

	case MODE_RESOLT:
		UpdateResult();
		break;
	}
	UpdateWindow();
	UpdateFade();
	UpdateFadeWindow();
	UpdateAnimation();
}
//=========================================================================================================//
// �`�揈��
//=========================================================================================================//
void Draw(void)
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	g_pD3DDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//�`��̊J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		// �e��I�u�W�F�N�g�̕`�揈��
		DrawBGWindow();

		MODE *Mode;	//���݂̃��[�h�m�F�̂���

		Mode = GetMode();	//���݂̃��[�h�����炤

		switch (Mode[0])
		{
		case MODE_TITLE:
			DrawTitle();
			break;

		case MODE_RANKING:
			DrawRanking();
			break;

		case MODE_WATCH:
			DrawWatch();
			break;

		case MODE_TUTORIAL:
			DrawTutorial();
			break;

		case MODE_GAME:
			DrawGame();
			break;

		case MODE_RESOLT:
			DrawResult();
			break;
		}
		DrawWindow();
		DrawAnimation();
		DrawFade();
		DrawFadeWindow();

#ifdef _DEBUG
		// FPS�\��
		DrawFPS();
#endif

		// �`��̏I��
		g_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
//=========================================================================================================//
// �f�o�C�X�̎擾
//=========================================================================================================//
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}
#ifdef _DEBUG
//=============================================================================
// FPS�\������
//=============================================================================
void DrawFPS(void)
{
	RECT rect = { SCREEN_WIDTH - 55, SCREEN_HEIGHT - 24, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[256];

	// ���������
	wsprintf(&aStr[0], "FPS:%d\n",g_nCountFPS);

	// �e�L�X�g�`��
	// hDC      : �f�o�C�X�R���e�L�X�g�̃n���h��
	// lpString : �`�悷�镶����ւ̃|�C���^
	// nCount   : ������̕�����
	// lpRect   : �����`�̈�
	// uFormat  : �e�L�X�g�̐��`���@���w��
	//g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));
}
#endif

//=========================================================================================================//
// �f�o�C�X�̎擾
//=========================================================================================================//
void SetMode(MODE mode)
{
	MODE *Mode;	//���݂̃��[�h�m�F�̂���

	Mode = GetMode();	//���݂̃��[�h�����炤

	switch (Mode[0])
	{//���݂̏������I������
	case MODE_TITLE:
		UninitTitle();
		break;

	case MODE_RANKING:
		UninitRanking();
		break;

	case MODE_WATCH:
		UninitWatch();
		break;

	case MODE_TUTORIAL:
		UninitTutorial();
		break;

	case MODE_GAME:
		UninitGame();
		break;

	case MODE_RESOLT:
		UninitResult();
		break;
	}//switch����

	switch (mode)
	{//���ɍs�������̏�����
	case MODE_TITLE:
		InitTitle();
		break;

	case MODE_RANKING:
		InitRanking();
		break;

	case MODE_WATCH:
		InitWatch();
		break;

	case MODE_TUTORIAL:
		InitTutorial();
		break;

	case MODE_GAME:
		InitGame();
		break;

	case MODE_RESOLT:
		InitResult();
		break;
	}//switch����
	Mode[0] = mode;
}

//=========================================================================================================//
// ���[�h�̎擾
//=========================================================================================================//
MODE *GetMode(void)
{
	static MODE Mode = MODE_TITLE;

	return &Mode;
}
//=========================================================================================================//
// window�̎擾
//=========================================================================================================//
HWND *GethWnd(void)
{
	static HWND hWnd;

	return &hWnd;
}