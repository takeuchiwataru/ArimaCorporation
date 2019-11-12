//=============================================================================
//
// ���C������ [main.h]
// Author : �L�n���u
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#define DIRECTINPUT_VERSION (0x0800)//�r���h���Ɍx���Ώ��p�}�N��
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>

#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include "d3dx9.h"					 //�`�揈���ɕK�v
#include "dinput.h"					//���͏����ɕK�v
#include "xaudio2.h"
#include "string.h"
#include <iostream>
#include <cmath>
#include <crtdbg.h>
#include <stdlib.h>
#include <time.h>
#include <XInput.h>                     // XInput�̏����ɕK�v

//#define malloc(X) _malloc_dbg(X,_NORMAL_BLOCK,__FILE__,__LINE__)
#define _CRTDBG_MAP_ALLOC
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

//*****************************************************************************
// ���C�u�����t�@�C���̃����N
//*****************************************************************************
#pragma comment(lib,"d3d9.lib")		//�`�揈���ɕK�v
#pragma comment(lib,"d3dx9.lib")	//[d3d9.lib]�̊g�����C�u����
#pragma comment(lib,"dxguid.lib")	//DirectX�R���|�[�l���g(����)�g�p�ɕK�v
#pragma comment(lib,"winmm.lib")	//�V�X�e�������擾�ɕK�v
#pragma comment(lib,"dinput8.lib")	//���͏����ɕK�v
#pragma comment (lib, "XInput.lib")	// XInput�����ɕK�v
#pragma comment(lib, "ws2_32.lib")		//winsock�Ɏg�p

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCREEN_WIDTH	(1280)		// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(720)		// �E�C���h�E�̍���
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)				//���_�t�H�[�}�b�g
#define	FVF_VERTEX_3D   (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1) //���_�t�H�[�}�b�g

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//���_���W
	float rhw;			//1.0f�ŌŒ�
	D3DCOLOR col;		//���_�J���[
	D3DXVECTOR2 tex;	//�e�N�X�`���̍��W
}VERTEX_2D;

//���_�t�H�[�}�b�g�ɍ��킹���\����(�RD)
typedef struct
{
	D3DXVECTOR3 pos;	//���_���W
	D3DXVECTOR3 nor;	//�@���x�N�g��
	D3DCOLOR col;		//���_�J���[
	D3DXVECTOR2 tex;	//�e�N�X�`�����W
}VERTEX_3D;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
int GetFps(void);

#endif