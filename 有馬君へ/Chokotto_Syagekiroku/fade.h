//=============================================================================
//
// �t�F�[�h���� [fade.h]
// Author : 
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"

//*************************************
// �t�F�[�h�̏��
//*************************************
typedef enum
{
	FADE_NONE = 0,		// �������Ă��Ȃ����
	FADE_IN,			// �t�F�[�h�C�����
	FADE_OUT,			// �t�F�[�h�A�E�g���
	FADE_ENTER,			//EBTER�҂�
	FADE_MAX
} FADE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitFade(MODE modeNext);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);

void SetFade(MODE modeNext, int nTypeFade);
FADE *GetFade(void);

#endif
