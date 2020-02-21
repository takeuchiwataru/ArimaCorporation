//=========================================================================================================//
//
// ゲーム処理 [game.cpp]
// Author : Ryou Sugimoto
//
//=========================================================================================================//
#include "input.h"
#include "game.h"
#include "enemy.h"
#include "fade.h"
#include "sound.h"

//*********************************************************************************************************//
// グローバル変数
//*********************************************************************************************************//
int g_nCntWacth;

//=========================================================================================================//
// * ゲームの初期化処理
//=========================================================================================================//
void InitWatch(void)
{
	g_nCntWacth = 0;
	Difficulty difficulty;
	int nRand;
	nRand = rand() % 4;
	if (nRand == 0)
	{
		difficulty.nDifficulty = 80;
		difficulty.nNumber = 0;
	}
	else if (nRand == 1)
	{
		difficulty.nDifficulty = 100;
		difficulty.nNumber = 1;
	}
	else if (nRand == 2)
	{
		difficulty.nDifficulty = 150;
		difficulty.nNumber = 2;
	}
	else if (nRand == 3)
	{
		difficulty.nDifficulty = 275;
		difficulty.nNumber = 3;
	}
	SetDifficulty(difficulty);

	SetGamecounter(GameCounter{ 0, 0, 0, 0, 0 });
	SetGameState(GAMESTATE_NORMAL, MODE_GAME, 0);

	InitBullet();
	InitPlayer();
	InitShooter();
	InitExplosion();
	InitEnemy();
	InitGameWindow();
	InitScore();
	InitEffect();
	SetBGWindow(D3DXVECTOR3(SCREEN_GAMEX * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), COLLAR{ 100,100,100, 255 }, 0.0005f, 0.2f, SCREEN_GAMEX / 2, SCREEN_HEIGHT / 2, 1, WINDOWSTATE_SCROLL0);
	SetBGWindow(D3DXVECTOR3(SCREEN_GAMEX * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), COLLAR{ 200,200,200, 150 }, 0.001f, 1.0f, SCREEN_GAMEX / 2, SCREEN_HEIGHT / 2, 4, WINDOWSTATE_SCROLL1);

	SetGameWindow(D3DXVECTOR3(SCREEN_GAMEX + (SCREEN_WIDTH - SCREEN_GAMEX) / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR{ 1.0f, 1.0f, 1.0f, 1.0f }, (SCREEN_WIDTH - SCREEN_GAMEX) / 2, SCREEN_HEIGHT / 2, 0, WINDOWSTATE_NORMAL, WINDOWUSE_NORMAL);
	SetGameWindow(D3DXVECTOR3(SCREEN_GAMEX + (SCREEN_WIDTH - SCREEN_GAMEX) / 2, 100, 0.0f), D3DXCOLOR{ 1.0f, 1.0f, 1.0f, 1.0f }, 200, 30, 2, WINDOWSTATE_NORMAL, WINDOWUSE_RANKING);
	SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.8f, SCREEN_HEIGHT * 0.9f, 0.0f), D3DXCOLOR{ 1.0f,0.99f,0.04f, 0 }, 150, 30, 0, 0, 0, WINDOWSTATE_FLASH, WINDOWUSE_NORMAL);
	//SetScore(D3DXVECTOR3(SCREEN_GAMEX + (SCREEN_WIDTH - SCREEN_GAMEX) * 0.7f, 250, 0.0f), D3DXCOLOR{ 1.0f, 0.99f, 0.04f, 1.0f }, 0, 0, SCORESTATE_NONE);
	PLAYER *pPlayer = GetPlayer();
	pPlayer[0].nStealth = 0;
	pPlayer[0].pos = D3DXVECTOR3{ GAME_WIDE / 2, 1300,0.0f };
}
//=========================================================================================================//
// * ゲームの終了処理
//=========================================================================================================//
void UninitWatch(void)
{
	GAME *pGame;	//現在の状態をポインタで取得
	pGame = GetGameState();
	pGame[0].now = GAMESTATE_NONE;
	BreakExplosion();
	BreakScore();
	BreakWindow();
	BreakBGWindow();
	BreakGameWindow();
	BreakBullet();
	BreakEnemy();
	BreakShooter();
	UninitBullet();
	UninitPlayer();
	UninitShooter();
	UninitExplosion();
	UninitEnemy();
	UninitGameWindow();
	UninitEffect();
	UninitScore();
}
//=========================================================================================================//
// * ゲームの更新処理
//=========================================================================================================//
void UpdateWatch(void)
{
	GAME *pGame;	//現在の状態をポインタで取得
	static int nNumber;

	pGame = GetGameState();
	g_nCntWacth++;

	switch (pGame[0].now)
	{//ゲーム状態に応じた処理開始
	case GAMESTATE_NONE:
		break;

	case GAMESTATE_NORMAL:
		UpdateBGWindow();
		UpdateBullet();
		UpdateExplosion();
		UpdateEnemy();
		UpdateEffect();
		UpdateGameWindow();
		ChapterEnemy();
		DamageBossTitle(1);
		break;

	case GAMESTATE_END:	//終了までのカウントダウン
		pGame[0].nCntGameState++;
		if (pGame[0].nCntGameState > 200)
		{
			pGame[0].now = GAMESTATE_NONE;
			SetFade(pGame[0].CHANGE, 0);
		}
		break;
	}

	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{//エンターでタイトルへ
		SetFade(MODE_TITLE, 0);
	}

}
//=========================================================================================================//
// * ゲームの描画処理
//=========================================================================================================//
void DrawWatch(void)
{
	DrawExplosion();
	DrawEffect();

	DrawEnemy();
	DrawBullet();
	DrawGameWindow();
	DrawScore();
}