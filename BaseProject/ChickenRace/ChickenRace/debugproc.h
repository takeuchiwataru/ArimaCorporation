//=============================================================================
//
// デバック用フォント処理 [debugproc.h]
// Author : 有馬武志
//
//=============================================================================
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

#define _CRT_SECURE_NO_WARNINGS
#include "main.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define MAX_FONT			(1024)		//使えるフォントの最大数

//=====================
//	   基本クラス
//=====================
class CDebugProc
{
public://誰でも扱える
	CDebugProc();
	~CDebugProc();
	void Init(void);
	void Uninit(void);
	static void Print(char *fmt, ...);
	static void Draw(void);

protected://個人でのみ使う
	static LPD3DXFONT m_pFont;
	static char m_aStn[MAX_FONT];

};
#endif