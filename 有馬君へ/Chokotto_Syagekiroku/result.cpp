//=========================================================================================================//
//
// �^�C�g���̏��� [Title.cpp]
// Author : Ryou Sugimoto
//
//=========================================================================================================//
#define _CRT_SECURE_NO_WARNINGS	//Fire Wall�˔j
#include <stdio.h>				//�C���N���h�t�@�C�� �Z�[�u���[�h�̂���
#include "game.h"
#include "main.h"
#include"input.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*********************************************************************************************************//
// �O���[�o���ϐ�
//*********************************************************************************************************//


//=========================================================================================================//
// * �Q�[���̏���������
//=========================================================================================================//
void InitResult(void)
{
	InitExplosion();
	InitGameWindow();
	InitFont();
	InitScore();

	//�������I
	Result *pResult = GetResult();

	pResult[0].nCntScore = 0;
	pResult[0].nCntResult = 1;
	pResult[0].nRankin = 0;
	pResult[0].nHightFont = 0;
	pResult[0].nWideFont = 0;
	strcpy(&pResult[0].aStr[0], "");

	//���[�h
	FILE *pFile;
	SCORE *pScore = GetScore();
	Difficulty *pDifficulty = GetDifficulty();

	pFile = fopen("Score.bin", "rb");

	if (pFile != NULL)
	{//Save�@����
		fread(&pScore[0], sizeof(SCORE), MAX_SCORE, pFile);

		fclose(pFile);
	}
	else
	{//���s
		MessageBox(0, "", "���[�h�Ɏ��s���܂���", MB_OK);
	}

	//BG����I
	SetExplosion(D3DXVECTOR3{ SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f }, COLLAR{ 255,255,255 }, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1, 6, 3, 2, ANIMATION_LOOP);
	//��Փx�̃��S
	SetGameWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.1f, 0.0f), D3DXCOLOR{ 1.0f, 1.0f, 1.0f, 1.0f }, 300, 45, 2, WINDOWSTATE_NORMAL, WINDOWUSE_RANKING);
	//�X�R�A�\���̘g
	SetGameWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.3f, 0.0f), D3DXCOLOR{ 0.6f, 0.55f, 0.0f, 1.0f }, 450, 45, 3, WINDOWSTATE_NORMAL, WINDOWUSE_NORMAL);
	SetGameWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.45f, 0.0f), D3DXCOLOR{ 0.6f, 0.6f, 0.6f, 1.0f }, 450, 45, 3, WINDOWSTATE_NORMAL, WINDOWUSE_NORMAL);
	SetGameWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.6f, 0.0f), D3DXCOLOR{ 0.6f, 0.5f, 0.4f, 1.0f }, 450, 45, 3, WINDOWSTATE_NORMAL, WINDOWUSE_NORMAL);
	SetGameWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.75f, 0.0f), D3DXCOLOR{ 0.06f, 0.08f, 0.6f, 1.0f }, 450, 45, 3, WINDOWSTATE_NORMAL, WINDOWUSE_NORMAL);
	SetGameWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.9f, 0.0f), D3DXCOLOR{ 0.06f, 0.08f, 0.6f, 1.0f }, 450, 45, 3, WINDOWSTATE_NORMAL, WINDOWUSE_NORMAL);
	//��ʂƂ��̉���
	SetGameWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.2f, SCREEN_HEIGHT * 0.3f, 0.0f), D3DXCOLOR{ 1.0f, 1.0f, 1.0f, 1.0f }, 30, 30, 4, WINDOWSTATE_NORMAL, WINDOWUSE_RANKNUMBER);
	SetGameWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.2f, SCREEN_HEIGHT * 0.45f, 0.0f), D3DXCOLOR{ 1.0f, 1.0f, 1.0f, 1.0f }, 30, 30, 4, WINDOWSTATE_NORMAL, WINDOWUSE_RANKNUMBER);
	SetGameWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.2f, SCREEN_HEIGHT * 0.6f, 0.0f), D3DXCOLOR{ 1.0f, 1.0f, 1.0f, 1.0f }, 30, 30, 4, WINDOWSTATE_NORMAL, WINDOWUSE_RANKNUMBER);
	SetGameWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.2f, SCREEN_HEIGHT * 0.75f, 0.0f), D3DXCOLOR{ 1.0f, 1.0f, 1.0f, 1.0f }, 30, 30, 4, WINDOWSTATE_NORMAL, WINDOWUSE_RANKNUMBER);
	SetGameWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.2f, SCREEN_HEIGHT * 0.9f, 0.0f), D3DXCOLOR{ 1.0f, 1.0f, 1.0f, 1.0f }, 30, 30, 4, WINDOWSTATE_NORMAL, WINDOWUSE_RANKNUMBER);
	//���ꂼ��̃X�R�A
	SetScore(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.3f, 0.0f), D3DXCOLOR{ 1.0f, 0.99f, 0.04f, 0.0f }, pScore[pDifficulty[0].nNumber * 5].g_nScore, 1, SCORESTATE_FADE);
	SetScore(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.45f, 0.0f), D3DXCOLOR{ 1.0f, 0.99f, 0.04f, 0.0f }, pScore[pDifficulty[0].nNumber * 5 + 1].g_nScore, 2, SCORESTATE_FADE);
	SetScore(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.6f, 0.0f), D3DXCOLOR{ 1.0f, 0.99f, 0.04f, 0.0f }, pScore[pDifficulty[0].nNumber * 5 + 2].g_nScore, 3, SCORESTATE_FADE);
	SetScore(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.75f, 0.0f), D3DXCOLOR{ 1.0f, 0.99f, 0.04f, 0.0f }, pScore[pDifficulty[0].nNumber * 5 + 3].g_nScore, 4, SCORESTATE_FADE);
	SetScore(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.9f, 0.0f), D3DXCOLOR{ 1.0f, 0.99f, 0.04f, 0.0f }, pScore[pDifficulty[0].nNumber * 5 + 4].g_nScore, 5, SCORESTATE_FADE);
	//���ꂼ��̖��O
	SetFont(RECT{ 700 , 195, 1200, 300 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &pScore[pDifficulty[0].nNumber * 5].aStr[0], 0, FONTSTATE_NONE);
	SetFont(RECT{ 700 , 305, 1200, 410 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &pScore[pDifficulty[0].nNumber * 5 + 1].aStr[0], 1, FONTSTATE_NONE);
	SetFont(RECT{ 700 , 415, 1200, 520 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &pScore[pDifficulty[0].nNumber * 5 + 2].aStr[0], 2, FONTSTATE_NONE);
	SetFont(RECT{ 700 , 525, 1200, 630 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &pScore[pDifficulty[0].nNumber * 5 + 3].aStr[0], 3, FONTSTATE_NONE);
	SetFont(RECT{ 700 , 635, 1200, 740 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &pScore[pDifficulty[0].nNumber * 5 + 4].aStr[0], 4, FONTSTATE_NONE);
}
//=========================================================================================================//
// * �Q�[���̏I������
//=========================================================================================================//
void UninitResult(void)
{
	//�Z�[�u
	FILE *pFile;
	SCORE *pScore = GetScore();

	pFile = fopen("Score.bin", "wb");

	if (pFile != NULL)
	{//Save�@����
		fwrite(&pScore[0], sizeof(SCORE), MAX_SCORE, pFile);

		fclose(pFile);
	}
	else
	{//���s
		MessageBox(0, "", "�Z�[�u�Ɏ��s���܂���", MB_OK);
	}

	BreakExplosion();
	BreakGameWindow();
	UninitExplosion();
	UninitScore();
	UninitFont();
}
//=========================================================================================================//
// * �Q�[���̍X�V����
//=========================================================================================================//
void UpdateResult(void)
{
	int *nSetFont = GetSetFont();
	Result *pResult = GetResult();
	SCORE *nGameScore = GetGameScore();
	SCORE *pScore = GetScore();
	Difficulty *pDifficulty = GetDifficulty();
	int nCntRank;	//�����L���O�𐔂���
	int nLength;	//������̒�����}�邽��

	//RANKING����ւ�
	if (pResult[0].nCntResult % 20 == 0 && pResult[0].nCntResult > 80 && pResult[0].nCntScore < 5)
	{

		if (pScore[pDifficulty[0].nNumber * 5 + pResult[0].nCntScore].g_nScore < nGameScore[0].g_nScore)
		{//�n�C�X�R�A���o���ꍇ
			PlaySound(SOUND_LABEL_HIT_KNIFE000);
			SetScore(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * (0.3f + pResult[0].nCntScore * 0.15f) - 100, 0.0f), D3DXCOLOR{ 1.0f, 0.99f, 0.04f, 0.0f }, nGameScore[0].g_nScore, 0, SCORESTATE_SET);	//�Z�b�g�X�R�A
			SetAnimation(D3DXVECTOR3{ SCREEN_WIDTH * 0.37f, SCREEN_HEIGHT * (0.3f + pResult[0].nCntScore * 0.15f), 0.0f }, COLLAR{ 255,30,30 }, 50, 50, 9, 1, 2, 0, ANIMATION_NORMAL);	//�Z�b�gEXPLOSION

			for (nCntRank = 4; pResult[0].nCntScore <= nCntRank; nCntRank--)
			{//���̂ق�����X�R�A����ւ�
				pScore[pDifficulty[0].nNumber * 5 + nCntRank].g_nScore = pScore[pDifficulty[0].nNumber * 5 + nCntRank - 1].g_nScore;	//�X�V
				strcpy(&pScore[pDifficulty[0].nNumber * 5 + nCntRank].aStr[0], &pScore[pDifficulty[0].nNumber * 5 + nCntRank - 1].aStr[0]);	//���O�X�V
				if (nCntRank == 4)
				{//5�ʒE�� �����ړI��
					ChangeScore(nCntRank + 1, SCORESTATE_DELETE);	//�f���[�g�X�R�A
					ChangeFont(true, "", nCntRank, FONTSTATE_DELETE);//�f���[�g�l�[��
				}
				else
				{//�T�K�[��
					ChangeScore(nCntRank + 1, SCORESTATE_RANKDOWN);	//DOWN�X�R�A
					ChangeFont(true, "", nCntRank, FONTSTATE_RANKDOWN);//DOWN�l�[��
				}
			}
			pScore[pDifficulty[0].nNumber * 5 + pResult[0].nCntScore].g_nScore = nGameScore[0].g_nScore;		//�X�V
			strcpy(&pScore[pDifficulty[0].nNumber * 5 + pResult[0].nCntScore].aStr[0], "�������̃v���O���}�[");	//���O���(��)
			pResult[0].nRankin = pDifficulty[0].nNumber * 5 + pResult[0].nCntScore;								//�ꏊ�ۑ�
			pResult[0].nCntScore = 5; //�X�V�I��
			pResult[0].nCntResult = -60;//�n�C�X�R�A�̔���
		}
		else
		{//�n�C�X�R�A�łȂ������ꍇ
			PlaySound(SOUND_LABEL_GUARD000);
			SetAnimation(D3DXVECTOR3{ SCREEN_WIDTH * 0.37f, SCREEN_HEIGHT * (0.3f + pResult[0].nCntScore * 0.15f), 0.0f }, COLLAR{ 255,255,255 }, 50, 50, 9, 1, 2, 0, ANIMATION_NORMAL);	//�Z�b�gEXPLOSION
			pResult[0].nCntScore++;	//���̃X�R�A�m�F��
		}
	}

	UpdateExplosion();
	UpdateScore();

	if (pResult[0].nCntResult == 0 && pResult[0].nCntScore > 4)
	{//���O���͏��� �n�C�X�R�A���o���ꍇ
		if (pResult[0].nCntScore == 5)
		{//window�W�J 50���ƑI��g
			BreakFont();
			nSetFont[0] = 1;
			SetFont(RECT{ 450 , 100, 1200, 200 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &pResult[0].aStr[0], 5, FONTSTATE_NONE);
			SetFont(RECT{ 300 , 100, 1200, 200 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, "���O���� > ", 6, FONTSTATE_NONE);
			SetFont(RECT{ 300 , 670, 1200, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, "* Z/����", 7, FONTSTATE_NONE);
			SetFont(RECT{ 550 , 670, 1200, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, "* X/�߂�", 8, FONTSTATE_NONE);
			SetFont(RECT{ 800 , 670, 1200, 720 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, "* CTRL/�I���", 9, FONTSTATE_NONE);
			SetGameWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXCOLOR{ 0.0f, 0.0f, 0.0f, 0.8f }, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, WINDOWSTATE_NORMAL, WINDOWUSE_NORMAL);
			SetWindow(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, SCREEN_HEIGHT / 16, 21, 2, 0, WINDOWSTATE_SELECTON, WINDOWUSE_FONT);
			pResult[0].nCntScore++;
		}
		if (GetKeyboardTrigger(DIK_UP) == true || GetKeyboardTrigger(DIK_DOWN) == true || GetKeyboardTrigger(DIK_LEFT) == true || GetKeyboardTrigger(DIK_RIGHT) == true)
		{ PlaySound(SOUND_LABEL_BOTTON002); }

		if (GetKeyboardTrigger(DIK_UP) == true)
		{//��ԏザ��Ȃ���� �I�������
			if (pResult[0].nHightFont == 5)
			{//�I���, �����ɏo��Ƃ��@���ꂼ��^��ɍ��킹��
				if		(pResult[0].nWideFont == 0)	{ pResult[0].nWideFont = 5; }
				else if (pResult[0].nWideFont == 1)	{ pResult[0].nWideFont = 7; }
				else								{ pResult[0].nWideFont = 9; }
			}
			pResult[0].nHightFont--;
			if (pResult[0].nHightFont == -1)
			{
				if (pResult[0].nWideFont > 8)		{ pResult[0].nWideFont = 2; }	//�I���ɍ��킹��
				else if (pResult[0].nWideFont > 5)	{ pResult[0].nWideFont = 1; }	//�J�i�ɍ��킹��
				else								{ pResult[0].nWideFont = 0; }	//�Ђ�ɍ��킹��
				pResult[0].nHightFont = 5;
			}
		}
		if (GetKeyboardTrigger(DIK_DOWN) == true)
		{//��ԉ�����Ȃ���� �I��������
			if (pResult[0].nHightFont == 4)
			{//�I���, �����ɓ���Ƃ�
				if		(pResult[0].nWideFont > 8)	{ pResult[0].nWideFont = 2; }	//�I���ɍ��킹��
				else if (pResult[0].nWideFont > 5)	{ pResult[0].nWideFont = 1; }	//�J�i�ɍ��킹��
				else								{ pResult[0].nWideFont = 0; }	//�Ђ�ɍ��킹��
			}
			pResult[0].nHightFont++;
			if (pResult[0].nHightFont == 6)
			{//�I���, �����ɏo��Ƃ��@���ꂼ��^��ɍ��킹��
				if (pResult[0].nWideFont == 0)		{ pResult[0].nWideFont = 5; }
				else if (pResult[0].nWideFont == 1) { pResult[0].nWideFont = 7; }
				else								{ pResult[0].nWideFont = 9; }
				pResult[0].nHightFont = 0;
			}
		}
		if (GetKeyboardTrigger(DIK_LEFT) == true)
		{//��ԍ�����Ȃ���� �I��������
			pResult[0].nWideFont--;
			if (pResult[0].nHightFont == 5 && pResult[0].nWideFont == -1) { pResult[0].nWideFont = 2; }
			else if (pResult[0].nWideFont == -1) { pResult[0].nWideFont = 10; }
		}
		if (GetKeyboardTrigger(DIK_RIGHT) == true)
		{//��ԉE����Ȃ���� �I�����E��
			pResult[0].nWideFont++;
			if (pResult[0].nHightFont == 5 && pResult[0].nWideFont == 3) { pResult[0].nWideFont = 0; }
			else if (pResult[0].nWideFont == 11) { pResult[0].nWideFont = 0; }
		}
		if (GetKeyboardTrigger(DIK_LCONTROL) == true)
		{//�I���ɍ��킹��
			pResult[0].nWideFont = 2;
			pResult[0].nHightFont = 5;
		}
		//�g�̍X�V
		FontWindowMove(pResult[0].nWideFont, pResult[0].nHightFont);

		if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_Z) == true)
		{//�I���ɉ���������
			if (pResult[0].nHightFont == 5 && pResult[0].nWideFont == 2)
			{//�I��
				PlaySound(SOUND_LABEL_BOTTON000);
				nLength = (int)strlen(&pResult[0].aStr[0]);		//�������m�F
				if(nLength != 0) {strcpy(&pScore[pResult[0].nRankin].aStr[0], &pResult[0].aStr[0]);}
				nSetFont[0] = 0;
				BackWindow(1);
				BackGameWindow(1);
				pResult[0].nCntResult++;
				//�t�H���g�̒��蒼���Ɠ��͗p�̍폜
				ChangeFont(false, "", 5, FONTSTATE_NONE);
				ChangeFont(false, "", 6, FONTSTATE_NONE);
				ChangeFont(false, "", 7, FONTSTATE_NONE);
				ChangeFont(false, "", 8, FONTSTATE_NONE);
				ChangeFont(false, "", 9, FONTSTATE_NONE);
				SetFont(RECT{ 700 , 195, 1200, 300 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &pScore[pDifficulty[0].nNumber * 5].aStr[0], 0, FONTSTATE_NONE);
				SetFont(RECT{ 700 , 305, 1200, 410 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &pScore[pDifficulty[0].nNumber * 5 + 1].aStr[0], 1, FONTSTATE_NONE);
				SetFont(RECT{ 700 , 415, 1200, 520 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &pScore[pDifficulty[0].nNumber * 5 + 2].aStr[0], 2, FONTSTATE_NONE);
				SetFont(RECT{ 700 , 525, 1200, 630 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &pScore[pDifficulty[0].nNumber * 5 + 3].aStr[0], 3, FONTSTATE_NONE);
				SetFont(RECT{ 700 , 635, 1200, 740 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &pScore[pDifficulty[0].nNumber * 5 + 4].aStr[0], 4, FONTSTATE_NONE);
			}
			else if (pResult[0].nHightFont == 5 && pResult[0].nWideFont == 1)
			{
				nSetFont[0] = 2;
				PlaySound(SOUND_LABEL_BOTTON001);
			}	//�J�^�J�i��
			else if (pResult[0].nHightFont == 5 && pResult[0].nWideFont == 0)
			{
				nSetFont[0] = 1;
				PlaySound(SOUND_LABEL_BOTTON001);
			}	//�Ђ炪�Ȃ�
			else
			{ 
				nLength = (int)strlen(&pResult[0].aStr[0]);		//�������m�F
				if (nLength < 23) { InputName(nSetFont[0]); }	//���������]���Ă���Ȃ�
				else
				{//���������ő�
					if		(pResult[0].nHightFont == 1 || pResult[0].nHightFont == 3 && pResult[0].nWideFont == 3) { InputName(nSetFont[0]); }	//�ϊ��Ȃ�
					else if (pResult[0].nHightFont == 4 && pResult[0].nWideFont == 0) { InputName(nSetFont[0]); }	//�ϊ��Ȃ�
					else { PlaySound(SOUND_LABEL_BOTTON003); }//�G���[��
				}
			}
		}
		if (GetKeyboardTrigger(DIK_BACK) == true || GetKeyboardTrigger(DIK_X) == true)
		{//�ЂƂ���������
			PlaySound(SOUND_LABEL_BOTTON003);
			nLength = (int)strlen(&pResult[0].aStr[0]);//�������m�F
			if (nLength > 1)
			{ //�����������Ă����
				strcpy(&pResult[0].aStr[nLength - 2], "\0");
				ChangeFont(true, &pResult[0].aStr[0], 5, FONTSTATE_NONE);
				nLength = (int)strlen(&pResult[0].aStr[0]);//�������m�F
				if (nLength == 0)
				{ 
					ChangeFont(false, &pResult[0].aStr[0], 5, FONTSTATE_NONE);
					SetFont(RECT{ 450 , 100, 1200, 200 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 1.0f }, &pResult[0].aStr[0], 5, FONTSTATE_NONE);
				}
				
				//�폜��
			}
			else {}//�폜���s��
		}
	}
	else if (pResult[0].nCntScore > 4 && pResult[0].nCntResult > 0)
	{//���O���͏������I�� || �n�C�X�R�A���o�Ă��Ȃ� Title��
		if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_Z) == true)
		{
			SetFade(MODE_TITLE, 0);
		}
	}
	else
	{//���ԃJ�E���g
		pResult[0].nCntResult++;
	}

	UpdateFont();
}
//=========================================================================================================//
// * �Q�[���̕`�揈��
//=========================================================================================================//
void DrawResult(void)
{
	DrawExplosion();
	DrawGameWindow();
	DrawScore();
	DrawFont();
}
//=========================================================================================================//
// * �Z�[�u���[�h�̂���
//=========================================================================================================//
Result *GetResult(void)
{
	static Result result;

	return &result;
}
//=========================================================================================================//
// * �Z�[�u���[�h�̂���
//=========================================================================================================//
SCORE *GetScore(void)
{
	static SCORE score[MAX_SCORE];
	//int nCntScore;

	//for (nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	//{//�����L���O�̏�����
	//	score[nCntScore].bUse = false;
	//	score[nCntScore].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	//	score[nCntScore].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//	score[nCntScore].state = SCORESTATE_NONE;
	//	strcpy(&score[nCntScore].aStr[0], "�������̃v���O���}�[");	//���O���(��)

	//	if (nCntScore < 5)
	//	{//EASY
	//		score[nCntScore].g_nScore = 0;
	//	}
	//	else if (nCntScore < 10)
	//	{//NORMAL
	//		score[nCntScore].g_nScore = 0;
	//	}
	//	else if (nCntScore < 15)
	//	{//HARD
	//		score[nCntScore].g_nScore = 0;
	//	}
	//	else if (nCntScore < 20)
	//	{//���i�e�B�b�N
	//		score[nCntScore].g_nScore = 0;
	//	}
	//}

	return &score[0];
}
//=========================================================================================================//
// * ���O���͂̑I���ɉ���������
//=========================================================================================================//
void InputName(int nMode)
{
	Result *pResult = GetResult();

	PlaySound(SOUND_LABEL_BOTTON001);
	if (nMode == 1)
	{//�Ђ�
		if (pResult[0].nWideFont == 0)
		{//��s
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "�I"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "�H"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "�@"); }
			if (pResult[0].nHightFont == 4) { InputName3(); }
		}
		else if (pResult[0].nWideFont == 1)
		{//��s
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "�["); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "�]"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "�`"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "��"); }
		}
		else if (pResult[0].nWideFont == 2)
		{//��s
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "��"); }
		}
		else if (pResult[0].nWideFont == 3)
		{//��s
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 1) { InputName1(); }
			if (pResult[0].nHightFont == 3) { InputName2(); }
		}
		else if (pResult[0].nWideFont == 4)
		{//�܍s
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "��"); }
		}
		else if (pResult[0].nWideFont == 5)
		{//�͍s
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "��"); }
		}
		else if (pResult[0].nWideFont == 6)
		{//�ȍs
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "��"); }
		}
		else if (pResult[0].nWideFont == 7)
		{//���s
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "��"); }
		}
		else if (pResult[0].nWideFont == 8)
		{//���s
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "��"); }
		}
		else if (pResult[0].nWideFont == 9)
		{//���s
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "��"); }
		}
		else if (pResult[0].nWideFont == 10)
		{//���s
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "��"); }
		}
	}
	else if (nMode == 2)//---------------------------------------------------------------------//
	{//�J�i
		if (pResult[0].nWideFont == 0)
		{//���s
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "�I"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "�H"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "�@"); }
			if (pResult[0].nHightFont == 4) { InputName3(); }
		}
		else if (pResult[0].nWideFont == 1)
		{//���s
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "�["); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "�]"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "�`"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "��"); }
		}
		else if (pResult[0].nWideFont == 2)
		{//���s
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "��"); }
		}
		else if (pResult[0].nWideFont == 3)
		{//���s
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 1) { InputName1(); }
			if (pResult[0].nHightFont == 3) { InputName2(); }
		}
		else if (pResult[0].nWideFont == 4)
		{//�}�s
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "�}"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "�~"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "��"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "��"); }
		}
		else if (pResult[0].nWideFont == 5)
		{//�n�s
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "�n"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "�q"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "�t"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "�w"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "�z"); }
		}
		else if (pResult[0].nWideFont == 6)
		{//�i�s
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "�i"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "�j"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "�k"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "�l"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "�m"); }
		}
		else if (pResult[0].nWideFont == 7)
		{//�^�s
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "�^"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "�`"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "�c"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "�e"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "�g"); }
		}
		else if (pResult[0].nWideFont == 8)
		{//�T�s
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "�T"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "�V"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "�X"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "�Z"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "�\"); }
		}
		else if (pResult[0].nWideFont == 9)
		{//�J�s
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "�J"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "�L"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "�N"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "�P"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "�R"); }
		}
		else if (pResult[0].nWideFont == 10)
		{//�A�s
			if (pResult[0].nHightFont == 0) { strcat(&pResult[0].aStr[0], "�A"); }
			if (pResult[0].nHightFont == 1) { strcat(&pResult[0].aStr[0], "�C"); }
			if (pResult[0].nHightFont == 2) { strcat(&pResult[0].aStr[0], "�E"); }
			if (pResult[0].nHightFont == 3) { strcat(&pResult[0].aStr[0], "�G"); }
			if (pResult[0].nHightFont == 4) { strcat(&pResult[0].aStr[0], "�I"); }
		}
	}

	//�������Z�b�g����
	ChangeFont(true, &pResult[0].aStr[0], 5, FONTSTATE_NONE);
}
//=========================================================================================================//
// * ���O���͂̑I���ɉ����������@�V�ϊ�
//=========================================================================================================//
void InputName1(void)
{
	Result *pResult = GetResult();
	int nLength;
	nLength = (int)strlen(&pResult[0].aStr[0]);//�������m�F

	if (nLength > 1)
	{//�V�ɕϊ�
		if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		//���s
		else if	(strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		//���s
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		//���s
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		//�͍s
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }

		//�J�s
		else if (strcmp(&pResult[0].aStr[nLength - 2], "�E") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "�J") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "�K"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "�L") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "�M"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "�N") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "�O"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "�P") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "�Q"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "�R") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "�S"); }
		//�T�s
		else if (strcmp(&pResult[0].aStr[nLength - 2], "�T") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "�U"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "�V") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "�W"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "�X") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "�Y"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "�Z") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "�["); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "�\") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "�]"); }
		//�^�s
		else if (strcmp(&pResult[0].aStr[nLength - 2], "�^") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "�_"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "�`") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "�a"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "�c") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "�d"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "�e") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "�f"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "�g") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "�h"); }
		//�n�s
		else if (strcmp(&pResult[0].aStr[nLength - 2], "�n") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "�o"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "�q") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "�r"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "�t") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "�u"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "�w") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "�x"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "�z") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "�{"); }
	}
	else {}//���s��
}
//=========================================================================================================//
// * ���O���͂̑I���ɉ����������@�K�ϊ�
//=========================================================================================================//
void InputName2(void)
{
	Result *pResult = GetResult();
	int nLength;
	nLength = (int)strlen(&pResult[0].aStr[0]);//�������m�F

	if (nLength > 1)
	{//�V�ɕϊ�
	 //���s
		//�͍s
		if		(strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		//�n�s
		else if (strcmp(&pResult[0].aStr[nLength - 2], "�n") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "�p"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "�q") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "�s"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "�t") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "�v"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "�w") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "�z") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "�|"); }
	}
	else {}//���s��
}
//=========================================================================================================//
// * ���O���͂̑I���ɉ����������@�������ϊ�
//=========================================================================================================//
void InputName3(void)
{
	Result *pResult = GetResult();
	int nLength;
	nLength = (int)strlen(&pResult[0].aStr[0]);//�������m�F

	if (nLength > 1)
	{//�������ɕϊ�
		if		(strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }

		//�J�^�J�i
		else if (strcmp(&pResult[0].aStr[nLength - 2], "�A") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "�@"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "�C") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "�B"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "�E") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "�D"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "�G") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "�F"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "�I") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "�H"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "�c") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "�b"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "�J") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "�P") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
		else if (strcmp(&pResult[0].aStr[nLength - 2], "��") == 0) { strcpy(&pResult[0].aStr[nLength - 2], "��"); }
	}
	else {}//���s��
}