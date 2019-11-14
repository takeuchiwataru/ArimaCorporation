//=============================================================================
//
// ���C������ [main.cpp]
// Author : �L�n���u
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS	//Fire Wall�˔j
#include <stdio.h>				//�C���N���h�t�@�C��

#include "main.h"
#include "manager.h"
#include "resource.h"
#include <windows.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME		"AppClass"						//�E�C���h�E�̃N���X��
#define WINDOW_NAME		"�U�E�`�L�����[�X"				//�E�C���h�E�̃L���v�V������
#define NAME			"Section5.2 open file dialog"	//�^�C�g���o�[�ɕ\������e�L�X�g

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
#ifdef _DEBUG
int						g_nCountFPS;			// FPS�J�E���^
#endif

//=============================================================================
// ���C���֐�
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{	
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	CManager *pManager = NULL;

	//
	HICON hIcon = ::LoadIcon
	(
		hInstance,							// �A�v���P�[�V�����C���X�^���X�̃n���h��
		MAKEINTRESOURCE(IDI_ICON)			// ���\�[�X���̕�����܂��̓��\�[�X���ʎq
	);

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					// WNDCLASSEX�̃������T�C�Y���w��
		CS_CLASSDC,							// �\������E�B���h�E�̃X�^�C����ݒ�
		WndProc,							// �E�B���h�E�v���V�[�W���̃A�h���X���w��
		0,									// �ʏ�͎w�肵�Ȃ�����"0"���w��
		0,									// �ʏ�͎w�肵�Ȃ�����"0"���w��
		hInstance,							// WinMain�̈����̃C���X�^���X�n���h�����w��
		hIcon,								// �^�X�N�o�[�Ɏg�p����A�C�R�����w��
		LoadCursor(NULL, IDC_ARROW),		// �g�p����}�E�X�J�[�\�����w��
		(HBRUSH)(COLOR_WINDOW + 1),			// �E�B���h�E�̃N���C�A���g�̈�̔w�i�F��ݒ�
		"IDR_MENU1",						// Windows�ɂ��郁�j���[��ݒ�
		CLASS_NAME,							// �E�B���h�E�N���X�̖��O
		hIcon								// �t�@�C���A�C�R���Ɏg�p����A�C�R�����w��
	};
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	HWND hWnd;
	MSG msg;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �w�肵���N���C�A���g�̈���m�ۂ��邽�߂ɕK�v�ȃE�B���h�E���W���v�Z
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindow(CLASS_NAME,
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

	if (pManager == NULL)
	{//���I�m��
		pManager = new CManager;
	}

	if (pManager != NULL)
	{
		if (FAILED(pManager->Init(hInstance, hWnd, TRUE)))		//FALSE�t���X�N���[��.TRUE�E�B���h�E
		{
			return -1;
		}

		// ����\��ݒ�
		timeBeginPeriod(1);

		// �t���[���J�E���g������
		dwCurrentTime =
			dwFrameCount = 0;
		dwExecLastTime =
			dwFPSLastTime = timeGetTime();

		// �E�C���h�E�̕\��
		ShowWindow(hWnd, nCmdShow);
		//�}�E�X�|�C���^�[��\��
		//ShowCursor(FALSE);
		UpdateWindow(hWnd);

		// ���b�Z�[�W���[�v
		while (1)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
					break;
				}
				else
				{
					// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			else
			{
				dwCurrentTime = timeGetTime();	// ���݂̎��Ԃ��擾
				if ((dwCurrentTime - dwFPSLastTime) >= 500)
				{// 0.5�b���ƂɎ��s
#ifdef _DEBUG
				// FPS���Z�o
					g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
					dwFPSLastTime = dwCurrentTime;	// ���݂̎��Ԃ�ۑ�
					dwFrameCount = 0;
				}

				if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
				{// 1/60�b�o��
					dwExecLastTime = dwCurrentTime;	// ���݂̎��Ԃ�ۑ�

					pManager->Update();

					pManager->Draw();

					dwFrameCount++;
				}
			}
		}
	}

	if (pManager != NULL)
	{
		pManager->Uninit();

		delete pManager;
		pManager = NULL;
	}

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// ����\��߂�
	timeEndPeriod(1);

	return (int)msg.wParam;
}
//=============================================================================
// �t�@�C�����J���̂ɕK�v�Ȋ֐�
//=============================================================================
bool openReadFile(HWND hWnd)
{
	//�ϐ��錾
	OPENFILENAME    ofn;
	char            filename[256], msg[300], buf[1024], Path[MAX_PATH + 1],cWsprint[256];
	HANDLE          hFile;
	DWORD           dwBytes;

	GetModuleFileName(NULL, Path, MAX_PATH);

	char drive[MAX_PATH + 1], dir[MAX_PATH + 1], fname[MAX_PATH + 1], ext[MAX_PATH + 1];

	_splitpath(Path, drive, dir, fname, ext);//�p�X�����\���v�f�ɕ������܂�

	//�h���C�u�ƃf�B���N�g��������
	wsprintf(cWsprint, "%s%s",drive,dir);

	//�ϐ��̏�����
	filename[0] = '\0';														//�擪�̕�����0�����ڂ���n�߂�
	memset(&ofn, 0, sizeof(OPENFILENAME));									//�\���̂�0�ŃN���A
	ofn.lStructSize = sizeof(OPENFILENAME);									//�\���̂̃T�C�Y�̎w��
	ofn.lpstrInitialDir = cWsprint;											//�n�܂�A�h���X
	ofn.lpstrFilter = "text file(*.txt)\0*.txt\0all file(*.*)\0*.*\0\0";	//�t�B���^�̎w��
	ofn.lpstrFile = filename;												//���ʂ��i�[�����o�b�t�@�̎w��
	ofn.nMaxFile = sizeof(filename);										//�o�b�t�@�̃T�C�Y
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;						//����͏��ɁA�u�������p�X�̂ݓ��͉v�u�������t�@�C�����̂ݓ��͉v�u�ǂݎ���p�`�F�b�N�������v
	ofn.lpstrDefExt = "txt";												//�W���̊g���q�̎w��

	//�_�C�A���O���J��
	if (GetOpenFileName(&ofn) != TRUE)
	{
		return FALSE;
	}

	//�t�@�C�����J���i�Ȃ���΍쐬�j
	hFile = CreateFile(filename, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE) 
	{
		wsprintf(msg, "%s failed to open.", filename);
		MessageBox(NULL, msg, NAME, MB_OK);
		return FALSE;
	}

	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
	ReadFile(hFile, buf, sizeof(buf) - 1, &dwBytes, NULL);
	*((u_char*)buf + dwBytes) = 0;  //�ǂݏI����'\0'��}��

	//�t�@�C�������
	CloseHandle(hFile);

	return TRUE;
}

//=============================================================================
// �E�C���h�E�v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
		//switch (LOWORD(wParam))
		//{
		//case IDM_FILEOPEN: //�J��
		//	openReadFile(hWnd);
		//	//InvalidateRect(hWnd, NULL, TRUE);  //�̈斳����
		//	//UpdateWindow(hWnd);                //�ĕ`�施��
		//	break;
		//case IDM_EXIT: //�I��
		//	PostMessage(hWnd, WM_CLOSE, 0, 0);
		//	break;
		//}
		break;
	case WM_CREATE:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:				// [ESC]�L�[�������ꂽ
			DestroyWindow(hWnd);	// �E�B���h�E��j������悤�w������

			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
#ifdef _DEBUG
//=============================================================================
// FPS
//=============================================================================
int GetFps(void)
{
	return g_nCountFPS;
}
#endif
