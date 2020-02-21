//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*************************************
// �T�E���h�t�@�C��
//*************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// BGM0
	SOUND_LABEL_RANKING000,		// RANKING
	SOUND_LABEL_TUTORIAL000,	// TUTORIAL

	SOUND_LABEL_FBGM000,		// FIELDBGM0
	SOUND_LABEL_FBGM001,		// FIELDBGM0
	SOUND_LABEL_FBGM002,		// FIELDBGM0
	SOUND_LABEL_FBGM003,		// FIELDBGM0

	SOUND_LABEL_BOTTON000,		//���艹 && �`���[�W��
	SOUND_LABEL_BOTTON001,		//���艹
	SOUND_LABEL_BOTTON002,		//�J�[�\���ړ���
	SOUND_LABEL_BOTTON003,		//�폜��
	SOUND_LABEL_PAUSE000,		//PAUSE��
	SOUND_LABEL_SHOT000,		//�e���ˉ�0
	SOUND_LABEL_SHOT001,		//�e���ˉ�1
	SOUND_LABEL_SHOT002,		//�e���ˉ�2
	SOUND_LABEL_SHOT003,		//�e���ˉ�3
	SOUND_LABEL_SHOT004,		//�e���ˉ�4
	SOUND_LABEL_KNIFE000,		//�i�C�t�g�p���̉�
	SOUND_LABEL_HIT_Shot000,	//�e���q�b�g�����Ƃ��̉�
	SOUND_LABEL_HIT_KNIFE000,	//�i�C�t���q�b�g�����Ƃ��̉�
	SOUND_LABEL_GUARD000,		//�K�[�h�������̉�
	SOUND_LABEL_BOMB000,		//�G���G�̔���
	SOUND_LABEL_BOMB001,		//�{�X�̔���
	SOUND_LABEL_KIRA,			//�~�܂��Ă���e��START��
	SOUND_LABEL_MODE_CHANGE,	//BOSS���[�h�`�F���W
	SOUND_LABEL_WIND,			//BOSS���[�h�`�F���W�̎��̕��̉�
	SOUND_LABEL_GAMEOVER000,	//�Q�[���I�[�o�[
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
