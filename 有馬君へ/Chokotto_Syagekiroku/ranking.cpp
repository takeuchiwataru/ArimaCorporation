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

//*********************************************************************************************************//
// �O���[�o���ϐ�
//*********************************************************************************************************//
int		g_nRankTime;		//ranking�̃^�C�}�[
int		g_nCntRank;			//ranking�𐔂���

//=========================================================================================================//
// * �Q�[���̏���������
//=========================================================================================================//
void InitRanking(void)
{
	g_nRankTime = 1;
	g_nCntRank = 0;
	InitExplosion();
	InitGameWindow();
	InitScore();
	InitFont();


	//���[�h
	FILE *pFile;
	SCORE *pScore = GetScore();
	Difficulty *pDifficulty = GetDifficulty();
	pDifficulty[0].nNumber = 0;

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
	SetExplosion(D3DXVECTOR3{ SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f }, COLLAR{ 255,255,255, 200 }, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1, 5, 2, 3, ANIMATION_LOOP);
	PlaySound(SOUND_LABEL_RANKING000);
}
//=========================================================================================================//
// * �Q�[���̏I������
//=========================================================================================================//
void UninitRanking(void)
{
	BreakScore();
	BreakWindow();
	BreakExplosion();
	BreakGameWindow();
	BreakFont();
	UninitFont();
	UninitExplosion();
	UninitGameWindow();
	UninitScore();
	StopSound();
}
//=========================================================================================================//
// * �Q�[���̍X�V����
//=========================================================================================================//
void UpdateRanking(void)
{//�e��Փx��ranking�\��
	SCORE *pScore = GetScore();
	Difficulty *pDifficulty = GetDifficulty();

	g_nRankTime++;

	if (g_nRankTime % 900 > 59 && g_nRankTime % 900 < 161)
	{//6��ʂ� �Z�b�g ��Փx����
		if (g_nRankTime % 20 == 0)
		{//�Z�b�g
			if (g_nCntRank == 0)
			{//��Փx�̃��S
				SetGameWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.625f, SCREEN_HEIGHT * 0.1f, 0.0f), D3DXCOLOR{ 1.0f, 1.0f, 1.0f, 0.0f }, 300, 45, 2, WINDOWSTATE_RANKSET, WINDOWUSE_RANKING);
			}//�X�R�A�\���̘g
			else if (g_nCntRank == 1) { SetGameWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.625f, SCREEN_HEIGHT * 0.3f, 0.0f), D3DXCOLOR{ 0.6f, 0.55f, 0.0f, 0.0f }, 450, 45, 3, WINDOWSTATE_RANKSET, WINDOWUSE_NORMAL);}
			else if (g_nCntRank == 2) {	SetGameWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.625f, SCREEN_HEIGHT * 0.45f, 0.0f), D3DXCOLOR{ 0.6f, 0.6f, 0.6f, 0.0f }, 450, 45, 3, WINDOWSTATE_RANKSET, WINDOWUSE_NORMAL);}
			else if (g_nCntRank == 3) { SetGameWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.625f, SCREEN_HEIGHT * 0.6f, 0.0f), D3DXCOLOR{ 0.6f, 0.5f, 0.4f, 0.0f }, 450, 45, 3, WINDOWSTATE_RANKSET, WINDOWUSE_NORMAL); }
			else if (g_nCntRank == 4) { SetGameWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.625f, SCREEN_HEIGHT * 0.75f, 0.0f), D3DXCOLOR{ 0.06f, 0.08f, 0.6f, 0.0f }, 450, 45, 3, WINDOWSTATE_RANKSET, WINDOWUSE_NORMAL); }
			else if (g_nCntRank == 5) { SetGameWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.625f, SCREEN_HEIGHT * 0.9f, 0.0f), D3DXCOLOR{ 0.06f, 0.08f, 0.6f, 0.0f }, 450, 45, 3, WINDOWSTATE_RANKSET, WINDOWUSE_NORMAL); }
			
			if (g_nCntRank != 0)
			{
				//��ʂƂ��̉���
				SetGameWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.325f, SCREEN_HEIGHT * (0.3f + (g_nCntRank - 1) * 0.15f), 0.0f), D3DXCOLOR{ 1.0f, 1.0f, 1.0f, 0.0f }, 30, 30, 4, WINDOWSTATE_RANKSET, WINDOWUSE_RANKNUMBER);
				//���ꂼ��̃X�R�A
				SetScore(D3DXVECTOR3(SCREEN_WIDTH * 0.625f, SCREEN_HEIGHT * (0.3f + (g_nCntRank - 1) * 0.15f), 0.0f), D3DXCOLOR{ 1.0f, 0.99f, 0.04f, 0.0f }, pScore[pDifficulty[0].nNumber * 5 + (g_nCntRank - 1)].g_nScore, (g_nCntRank - 1), SCORESTATE_RANKSET);
				//���ꂼ��̖��O
				SetFont(RECT{ 860 , 195 + (g_nCntRank - 1) * 110, 1200, 300 + (g_nCntRank - 1) * 110 }, D3DXCOLOR{ 1.0f, 1.0f, 1.0 , 0.0f }, &pScore[pDifficulty[0].nNumber * 5 + (g_nCntRank - 1)].aStr[0], g_nCntRank - 1, FONTSTATE_RANKSET);
			}

			//if (g_nCntRank == 5) { MessageBox(0, "", "", MB_OK); }
			g_nCntRank = (g_nCntRank + 1) % 6;	//����6���ŉ�]
		}
	}
	if (g_nRankTime % 900 > 799 || g_nRankTime % 900 == 0)
	{//6��ʂ� �f���[�g ��Փx����
		if (g_nRankTime % 20 == 0)
		{//�f���[�g
			if (g_nCntRank == 0) { ChangeGameWindow( 0, WINDOWSTATE_RANKDELETE); }
			else
			{
				ChangeGameWindow((g_nCntRank - 1) * 2 + 1, WINDOWSTATE_RANKDELETE);
				ChangeGameWindow((g_nCntRank - 1) * 2 + 2, WINDOWSTATE_RANKDELETE);
				ChangeScore(g_nCntRank - 1, SCORESTATE_RANKDELETE);
				ChangeFont(true, "", g_nCntRank - 1, FONTSTATE_RANKDELETE);
			}
			g_nCntRank = (g_nCntRank + 1) % 6;	//����6���ŉ�]
		}
	}

	if (g_nRankTime % 900 == 0 && g_nRankTime != 0)
	{
		pDifficulty[0].nNumber++;
	}

	UpdateGameWindow();
	UpdateScore();
	UpdateFont();
	UpdateExplosion();

	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{//�G���^�[�Ń^�C�g����
		SetFade(MODE_TITLE, 0);
	}
	else if (g_nRankTime > 3600)
	{//�G���^�[�Ń^�C�g����
		SetFade(MODE_WATCH, 0);
	}
}
//=========================================================================================================//
// * �Q�[���̕`�揈��
//=========================================================================================================//
void DrawRanking(void)
{
	DrawExplosion();
	DrawGameWindow();
	DrawScore();
	DrawFont();
}