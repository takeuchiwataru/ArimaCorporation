//=============================================================================
//
// メイン処理 [main.h]
// Author : 有馬武志
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#define DIRECTINPUT_VERSION (0x0800)//ビルド時に警告対処用マクロ
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>

#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include "d3dx9.h"					 //描画処理に必要
#include "dinput.h"					//入力処理に必要
#include "xaudio2.h"
#include "string.h"
#include <iostream>
#include <cmath>
#include <crtdbg.h>
#include <stdlib.h>
#include <time.h>
#include <XInput.h>                     // XInputの処理に必要

//#define malloc(X) _malloc_dbg(X,_NORMAL_BLOCK,__FILE__,__LINE__)
#define _CRTDBG_MAP_ALLOC
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

//*****************************************************************************
// ライブラリファイルのリンク
//*****************************************************************************
#pragma comment(lib,"d3d9.lib")		//描画処理に必要
#pragma comment(lib,"d3dx9.lib")	//[d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")	//DirectXコンポーネント(部分)使用に必要
#pragma comment(lib,"winmm.lib")	//システム時刻取得に必要
#pragma comment(lib,"dinput8.lib")	//入力処理に必要
#pragma comment (lib, "XInput.lib")	// XInput処理に必要
#pragma comment(lib, "ws2_32.lib")		//winsockに使用

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCREEN_WIDTH	(1280)		// ウインドウの幅
#define SCREEN_HEIGHT	(720)		// ウインドウの高さ
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)				//頂点フォーマット
#define	FVF_VERTEX_3D   (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1) //頂点フォーマット

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//頂点座標
	float rhw;			//1.0fで固定
	D3DCOLOR col;		//頂点カラー
	D3DXVECTOR2 tex;	//テクスチャの座標
}VERTEX_2D;

//頂点フォーマットに合わせた構造体(３D)
typedef struct
{
	D3DXVECTOR3 pos;	//頂点座標
	D3DXVECTOR3 nor;	//法線ベクトル
	D3DCOLOR col;		//頂点カラー
	D3DXVECTOR2 tex;	//テクスチャ座標
}VERTEX_3D;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
int GetFps(void);

#endif