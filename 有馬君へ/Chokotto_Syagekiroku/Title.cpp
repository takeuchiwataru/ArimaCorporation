//=========================================================================================================//
//
// �^�C�g���̏��� [Title.cpp]
// Author : Ryou Sugimoto
//
//=========================================================================================================//
#include "enemy.h"
#include "game.h"
#include "main.h"
#include"input.h"
#include "fade.h"
#include "sound.h"
#include <stdlib.h>				//�����h�p
#include <time.h>				//�����������h�p

//*********************************************************************************************************//
// �O���[�o���ϐ�
//*********************************************************************************************************//
bool g_bWindow;
int g_nCntTime;

//=========================================================================================================//
// * Title�̏���������
//=========================================================================================================//
void InitTitle(void)
{
	int nRand;
	InitFont();

	g_bWindow = false;
	g_nCntTime = 0;
	InitEnemy();

	nRand = rand() % 10;
	SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 0.0f, 0.0f), ENEMYTYPE_TITLE, ENEMYMODE_NONE, 0.0f, 0.0f, 0.622f, 0, nRand, ENEMY_HP3, ATTACK{BULLETSTATE_NOT}, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
	SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), COLLAR{ 255,255,255, 255 }, 0.0f, 1.0f, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, WINDOWSTATE_NORMAL);
	SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.6f, 0.0f), D3DXCOLOR{ 0.23f,0.23f,0.23f, 0.0f }, 150, 30, 8, 0, 0, WINDOWSTATE_STEALTH, WINDOWUSE_SELECT);
	SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.7f, 0.0f), D3DXCOLOR{ 0.23f,0.23f,0.23f, 0.0f }, 150, 30, 9, 1, 0, WINDOWSTATE_STEALTH, WINDOWUSE_SELECT);
	SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.8f, 0.0f), D3DXCOLOR{ 0.23f,0.23f,0.23f, 0.0f }, 150, 30, 10, 2, 0, WINDOWSTATE_STEALTH, WINDOWUSE_SELECT);

	StopSound();
	PlaySound(SOUND_LABEL_BGM000);
}
//=========================================================================================================//
// * Title�̏I������
//=========================================================================================================//
void UninitTitle(void)
{
	UninitFont();

	BreakEnemy();
	UninitEnemy();
	BreakWindow();
	BreakBGWindow();
	StopSound();
}
//=========================================================================================================//
// * Title�̍X�V����
//=========================================================================================================//
void UpdateTitle(void)
{
	Difficulty difficulty;
	static int nCountTime = 0;
	static int nNumber[2] = {0,3};
	int nRand[3];
	nCountTime++;
	g_nCntTime++;
	nRand[0] = rand() % (SCREEN_HEIGHT - 60) + 30;
	nRand[1] = rand() % 64;
	nRand[2] = rand() % 2;

	if (g_nCntTime > 3000)
	{//���b�o�����烉���L���O��ʂ�
		SetFade(MODE_RANKING, 0);
	}

	if (nCountTime % 60 == 0 && nRand[2] ==0)
	{
		SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, nRand[0] * 1.0f, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, -D3DX_PI * 0.5, 0.0f, 0.3f, 0, nRand[1], ENEMY_HP1, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
	}
	else if (nCountTime % 60 == 0 && nRand[2] == 1)
	{
		SetEnemy(D3DXVECTOR3(0, nRand[0] * 1.0f, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, D3DX_PI * 0.5, 0.0f, 0.3f, 0, nRand[1], ENEMY_HP1, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
	}

	UpdateEnemy();

	if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_Z) == true)
	{
		if (g_bWindow == false)
		{
			if (g_nCntTime < 128) {}//SELECT�܂ŃX�L�b�v
			else if (nNumber[0] == 0)
			{//��Փx�I����
				PlaySound(SOUND_LABEL_BOTTON000);
				g_bWindow = true;
				SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXCOLOR{ 1.0f,1.0f,1.0f, 1.0f }, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 2, 0, 0, WINDOWSTATE_NORMAL, WINDOWUSE_NORMAL);
				SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.26f, 0.0f), D3DXCOLOR{ 0.23f,0.23f,0.23f, 1.0f }, 150, 35, 11, 3, 0, WINDOWSTATE_NORMAL, WINDOWUSE_SELECT);
				SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.425f, 0.0f), D3DXCOLOR{ 0.23f,0.23f,0.23f, 1.0f }, 150, 35, 12, 4, 0, WINDOWSTATE_NORMAL, WINDOWUSE_SELECT);
				SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.59f, 0.0f), D3DXCOLOR{ 0.23f,0.23f,0.23f, 1.0f }, 150, 35, 13, 5, 0, WINDOWSTATE_NORMAL, WINDOWUSE_SELECT);
				SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.75f, 0.0f), D3DXCOLOR{ 0.23f,0.23f,0.23f, 1.0f }, 150, 35, 14, 6, 0, WINDOWSTATE_NORMAL, WINDOWUSE_SELECT);
			}
			else if (nNumber[0] == 1)
			{//TUTORIAL�N��
				PlaySound(SOUND_LABEL_BOTTON000);
				SetFade(MODE_TUTORIAL, 0);
			}
			else if (nNumber[0] == 2)
			{//�Q�[���I��
				HWND *hWnd;
				hWnd = GethWnd();
				DestroyWindow(hWnd[0]);
			}
		}
		else
		{
			if (nNumber[1] == 3)
			{
				difficulty.nDifficulty = 80;
				difficulty.nNumber = 0;
			}
			else if (nNumber[1] == 4)
			{
				difficulty.nDifficulty = 100;
				difficulty.nNumber = 1;
			}
			else if (nNumber[1] == 5)
			{
				difficulty.nDifficulty = 150;
				difficulty.nNumber = 2;
			}
			else if (nNumber[1] == 6)
			{
				difficulty.nDifficulty = 275;
				difficulty.nNumber = 3;
			}
			PlaySound(SOUND_LABEL_BOTTON000);
			SetDifficulty(difficulty);
			SetFade(MODE_GAME, 1);
			g_nCntTime = 128;
		}
			g_nCntTime = 128;
	}

	if (GetKeyboardTrigger(DIK_BACK) == true || GetKeyboardTrigger(DIK_X) == true && g_bWindow == true)
	{//�o�b�N�X���b�V���œ�Փx��ʂ���E�o
		g_bWindow = false;
		BackWindow(5);
	}

	if (GetKeyboardTrigger(DIK_W) == true || GetKeyboardTrigger(DIK_UP) == true)
	{//��ړ�
		if (g_nCntTime < 128) {}//SELECT�����N���܂ŃX�L�b�v
		else if (g_bWindow == false)
		{
			nNumber[0] = (nNumber[0] + 2) % 3;
		}
		else if (g_bWindow == true)
		{
			nNumber[1] = (nNumber[1] - 3 + 3) % 4 + 3;
		}
		g_nCntTime = 128;
	}
	if (GetKeyboardTrigger(DIK_S) == true || GetKeyboardTrigger(DIK_DOWN) == true)
	{//���ړ�
		if (g_nCntTime < 128) {}//SELECT�����N���܂ŃX�L�b�v
		else if (g_bWindow == false)
		{
			nNumber[0] = (nNumber[0] + 1) % 3;
		}
		else if (g_bWindow == true)
		{
			nNumber[1] = (nNumber[1] - 3 + 1) % 4 + 3;
		}
		g_nCntTime = 128;
	}
	if (g_nCntTime > 128 && g_bWindow == false)
	{//SELECT�����N��
		SelectWindow(nNumber[0]);
	}
	else if (g_nCntTime > 128)
	{
		SelectWindow(nNumber[1]);
	}
}
//=========================================================================================================//
// * Title�̕`�揈��
//=========================================================================================================//
void DrawTitle(void)
{
	DrawEnemy();
	DrawFont();
}