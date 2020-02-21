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

//=========================================================================================================//
// * ゲームの初期化処理
//=========================================================================================================//
void InitGame(void)
{
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
	SetBGWindow(D3DXVECTOR3(SCREEN_GAMEX * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), COLLAR{ 255,255,255, 255 }, 0.0005f, 0.2f, SCREEN_GAMEX / 2, SCREEN_HEIGHT / 2, 2, WINDOWSTATE_SCROLL0);

	//SetBGWindow(D3DXVECTOR3(SCREEN_GAMEX * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), COLLAR{ 100,100,100, 255 }, 0.0005f, 0.2f, SCREEN_GAMEX / 2, SCREEN_HEIGHT / 2, 1, WINDOWSTATE_SCROLL0);
	SetBGWindow(D3DXVECTOR3(SCREEN_GAMEX * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), COLLAR{ 200,200,200, 150 }, 0.001f, 1.0f, SCREEN_GAMEX / 2, SCREEN_HEIGHT / 2, 4, WINDOWSTATE_SCROLL1);

	SetGameWindow(D3DXVECTOR3(SCREEN_GAMEX + (SCREEN_WIDTH - SCREEN_GAMEX) / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR{ 1.0f, 1.0f, 1.0f, 1.0f }, (SCREEN_WIDTH - SCREEN_GAMEX) / 2, SCREEN_HEIGHT / 2, 0, WINDOWSTATE_NORMAL, WINDOWUSE_NORMAL);
	SetGameWindow(D3DXVECTOR3(SCREEN_GAMEX + (SCREEN_WIDTH - SCREEN_GAMEX) / 2, 100, 0.0f), D3DXCOLOR{ 1.0f, 1.0f, 1.0f, 1.0f }, 200, 30, 2, WINDOWSTATE_NORMAL, WINDOWUSE_RANKING);
	SetScore(D3DXVECTOR3(SCREEN_GAMEX + (SCREEN_WIDTH - SCREEN_GAMEX) * 0.7f, 250, 0.0f), D3DXCOLOR{1.0f, 0.99f, 0.04f, 1.0f}, 0, 0, SCORESTATE_NONE);
	//初めの残機
	SetWindow(D3DXVECTOR3(SCREEN_GAMEX + 80, 400, 0.0f), D3DXCOLOR{ 1.0f,1.0f,1.0f, 1.0f }, 30, 30, 7, 0, 0, WINDOWSTATE_NORMAL, WINDOWUSE_RESIDUALAIRPORT);
	SetWindow(D3DXVECTOR3(SCREEN_GAMEX + 120, 400, 0.0f), D3DXCOLOR{ 1.0f,1.0f,1.0f, 1.0f }, 30, 30, 7, 0, 0, WINDOWSTATE_NORMAL, WINDOWUSE_RESIDUALAIRPORT);
	SetWindow(D3DXVECTOR3(SCREEN_GAMEX + 160, 400, 0.0f), D3DXCOLOR{ 1.0f,1.0f,1.0f, 1.0f }, 30, 30, 7, 0, 0, WINDOWSTATE_NORMAL, WINDOWUSE_RESIDUALAIRPORT);
	SetWindow(D3DXVECTOR3(SCREEN_GAMEX + 200, 400, 0.0f), D3DXCOLOR{ 1.0f,1.0f,1.0f, 1.0f }, 30, 30, 7, 0, 0, WINDOWSTATE_NORMAL, WINDOWUSE_RESIDUALAIRPORT);

}
//=========================================================================================================//
// * ゲームの終了処理
//=========================================================================================================//
void UninitGame(void)
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
void UpdateGame(void)
{
	GAME *pGame;	//現在の状態をポインタで取得
	static int nNumber;

	pGame = GetGameState();

	switch (pGame[0].now)
	{//ゲーム状態に応じた処理開始
	case GAMESTATE_NONE:
		break;

	case GAMESTATE_NORMAL:
		UpdateBGWindow();
		UpdateBullet();
		UpdateExplosion();
		UpdatePlayer();
		UpdateEnemy();
		UpdateEffect();
		UpdateGameWindow();
		ChapterEnemy();
		break;

	case GAMESTATE_END:	//終了までのカウントダウン
		pGame[0].nCntGameState++;
		if (pGame[0].nCntGameState > 200)
		{
			pGame[0].now = GAMESTATE_NONE;
			SetFade(pGame[0].CHANGE, 0);
		}
		break;

	case GAMESTATE_PAUSE:
		if (GetKeyboardTrigger(DIK_W) == true || GetKeyboardTrigger(DIK_UP) == true)
		{//上移動
			nNumber = (nNumber + 2) % 3;
		}
		if (GetKeyboardTrigger(DIK_S) == true || GetKeyboardTrigger(DIK_DOWN) == true)
		{//下移動
			nNumber = (nNumber + 1) % 3;
		}

		SelectWindow(nNumber);
		if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_Z) == true)
		{
			PlaySound(SOUND_LABEL_BOTTON000);
			if (nNumber == 0)
			{//戻る
				pGame[0].now = GAMESTATE_NORMAL;
				BackWindow(4);
			}
			else if (nNumber == 1)
			{//初めから
				pGame[0].now = GAMESTATE_NONE;
				SetFade(MODE_GAME, 0);
			}
			else if (nNumber == 2)
			{//Titleへ Titleでセットエネミーしているためプレイヤーを殺しとく
				PLAYER *pPlayer;

				pGame[0].now = GAMESTATE_NONE;
				pPlayer = GetPlayer();
				SetFade(MODE_TITLE, 0);		//Titleへ
			}
		}
		break;
	}
	if (GetKeyboardTrigger(DIK_P) == true && pGame[0].now != GAMESTATE_PAUSE)
	{//Pを押したらpause画面
		nNumber = 0;
		PlaySound(SOUND_LABEL_PAUSE000);
		pGame[0].now = GAMESTATE_PAUSE;
		SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXCOLOR{ 1.0f,1.0f,1.0f, 1.0f }, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 15, 0, 0, WINDOWSTATE_NORMAL, WINDOWUSE_NORMAL);
		SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.3f, 0.0f), D3DXCOLOR{ 0.23f,0.23f,0.23f, 1.0f }, 150, 30, 16, 0, 0, WINDOWSTATE_SELECTOFF, WINDOWUSE_SELECT);
		SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXCOLOR{ 0.23f,0.23f,0.23f, 1.0f }, 150, 30, 17, 1, 0, WINDOWSTATE_SELECTOFF, WINDOWUSE_SELECT);
		SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.7f, 0.0f), D3DXCOLOR{ 0.23f,0.23f,0.23f, 1.0f }, 150, 30, 18, 2, 0, WINDOWSTATE_SELECTOFF, WINDOWUSE_SELECT);
	}
	else if (GetKeyboardTrigger(DIK_P) == true)
	{//Pを押したらpause解く
		PlaySound(SOUND_LABEL_PAUSE000);
		pGame[0].now = GAMESTATE_NORMAL;
		BackWindow(4);
	}
}
//=========================================================================================================//
// * ゲームの描画処理
//=========================================================================================================//
void DrawGame(void)
{
	DrawExplosion();
	DrawEffect();

	DrawPlayer();
	DrawShooter();
	DrawEnemy();
	DrawBullet();
	DrawGameWindow();
	DrawScore();
}
//=========================================================================================================//
// * ゲームの状態管理　取得
//=========================================================================================================//
GAME *GetGameState(void)
{
	static GAME game = GAME{ GAMESTATE_NORMAL , MODE_GAME , 0 };	//初回初期化

	return &game;
}
//=========================================================================================================//
// * ゲームの状態管理　set
//=========================================================================================================//
void SetGameState(GAMESTATE game, MODE mode, int nCntGameState)
{
	GAME *pGame;	//現在の状態をポインタで取得

	pGame = GetGameState();

	pGame[0].now = game;
	pGame[0].CHANGE = mode;
	pGame[0].nCntGameState = nCntGameState;
}
