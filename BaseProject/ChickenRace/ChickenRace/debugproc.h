//=============================================================================
//
// �f�o�b�N�p�t�H���g���� [debugproc.h]
// Author : �L�n���u
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
// �}�N����`
//*************************************************************************************************************************
#define MAX_FONT			(1024)		//�g����t�H���g�̍ő吔

//=====================
//	   ��{�N���X
//=====================
class CDebugProc
{
public://�N�ł�������
	CDebugProc();
	~CDebugProc();
	void Init(void);
	void Uninit(void);
	static void Print(char *fmt, ...);
	static void Draw(void);

protected://�l�ł̂ݎg��
	static LPD3DXFONT m_pFont;
	static char m_aStn[MAX_FONT];

};
#endif