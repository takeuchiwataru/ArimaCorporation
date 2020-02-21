//=========================================================================================================//
//
// タイトルの処理 [Title.cpp]
// Author : Ryou Sugimoto
//
//=========================================================================================================//
#include "enemy.h"
#include "game.h"
#include "main.h"
#include"input.h"
#include "fade.h"
#include "sound.h"

//=========================================================================================================//
// * ゲームの初期化処理
//=========================================================================================================//
void InitTutorial(void)
{
	SetGamecounter(GameCounter{ 0, 0, 0, 0, 0 });
	SetGameState(GAMESTATE_NORMAL, MODE_GAME, 0);

	InitBullet();
	InitPlayer();
	InitShooter();
	InitExplosion();
	InitEnemy();
	InitGameWindow();
	InitEffect();
	InitScore();
	PLAYER *pPlayer;//プレイヤー情報獲得
	pPlayer = GetPlayer();
	int nRand = rand() % 64;
	pPlayer[0].bUse = true;
	SetShooter(D3DXVECTOR3(cosf(D3DX_PI * 0.25) * 0.5f, cosf(D3DX_PI * 0.25) * 0.5f, 0.0f), nRand);
	nRand = rand() % 64;
	SetShooter(D3DXVECTOR3(cosf(D3DX_PI * -0.25) * 0.5f, cosf(D3DX_PI * -0.25) * 0.5f, 0.0f), nRand);

	PlaySound(SOUND_LABEL_TUTORIAL000);
	SetBGWindow(D3DXVECTOR3(SCREEN_GAMEX * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), COLLAR{ 255,255,255, 255 }, 0.0005f, 0.2f, SCREEN_GAMEX / 2, SCREEN_HEIGHT / 2, 2, WINDOWSTATE_SCROLL0);
	SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.8f, SCREEN_HEIGHT * 0.9f, 0.0f), D3DXCOLOR{ 1.0f,0.99f,0.04f, 0 }, 150, 30, 0, 0, 0, WINDOWSTATE_FLASH, WINDOWUSE_NORMAL);
	SetGameWindow(D3DXVECTOR3(SCREEN_GAMEX + (SCREEN_WIDTH - SCREEN_GAMEX) / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR{ 1.0f, 1.0f, 1.0f, 1.0f }, (SCREEN_WIDTH - SCREEN_GAMEX) / 2, SCREEN_HEIGHT / 2, 1, WINDOWSTATE_NORMAL, WINDOWUSE_NORMAL);
}
//=========================================================================================================//
// * ゲームの終了処理
//=========================================================================================================//
void UninitTutorial(void)
{
	StopSound();
	BreakScore();
	BreakWindow();
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
void UpdateTutorial(void)
{
	static int nCntDead = 1;	//死んだときのカウント
	static int nNumber = 0;
	static bool bPause = false;

	if (bPause == false)
	{
		IfBoss *ifboss;//ボス情報獲得
		ifboss = GetBoss();

		PLAYER *pPlayer;//プレイヤー情報獲得
		pPlayer = GetPlayer();

		if (pPlayer[0].bUse == false)
		{//死んだ場合カウント開始
			nCntDead++;
		}

		if (ifboss[0].nEnemyCounter == 1)
		{
			SetEnemy(D3DXVECTOR3(0, SCREEN_HEIGHT * 0.2, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, D3DX_PI * 0.5, 0.0f, 0.05f, 0, 0, 999, 
				ATTACK{ BULLETSTATE_ACCELERATOR, BULLETAIM_NORMAL,{ 0,0 },0,16,{ 180,0 },{ 10,12 }, 2,0,1,0,0.01f, 0.0f,0.003f, -0.03f, 0.0f, 0.0f },
				ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
		}//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ. 距離		休憩, 連射は最低1
		if (nCntDead == 100)
		{
			nCntDead = 0;
			int nRand = rand() % 64;
			pPlayer[0].bUse = true;
			SetShooter(D3DXVECTOR3(cosf(D3DX_PI * 0.25) * 0.5f, cosf(D3DX_PI * 0.25) * 0.5f, 0.0f), nRand);
			nRand = rand() % 64;
			SetShooter(D3DXVECTOR3(cosf(D3DX_PI * -0.25) * 0.5f, cosf(D3DX_PI * -0.25) * 0.5f, 0.0f), nRand);
			pPlayer[0].move = { 0.0f,0.0f,0.0f };
			pPlayer[0].nStealth = 100;
			pPlayer[0].nMode = 0;
			pPlayer[0].nGuardTime = GUARD_BREAK;
			SetExplosion(D3DXVECTOR3{ pPlayer[0].pos.x, pPlayer[0].pos.y + PLAYER_POSY3, 0.0f }, COLLAR{ 255,255,200 }, 100, 100, 10, 1, 2, 1, ANIMATION_NORMAL);
			SetWindow(D3DXVECTOR3(pPlayer[0].pos.x, pPlayer[0].pos.y + PLAYER_POSY3, 0.0f), D3DXCOLOR{ 1.0f,1.0f,1.0f, 0.0f }, 32, 32, 20, 0, 0, WINDOWSTATE_GUARDOFF, WINDOWUSE_GUARD);
		}

		UpdateBGWindow();
		UpdateBullet();
		UpdateExplosion();
		UpdatePlayer();
		UpdateEnemy();
		UpdateEffect();

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{
			SetFade(MODE_TITLE, 0);
		}
	}
	else
	{
		if (GetKeyboardTrigger(DIK_W) == true || GetKeyboardTrigger(DIK_UP) == true)
		{//上移動
			if (nNumber != 0)
			{
				nNumber--;
			}
		}
		if (GetKeyboardTrigger(DIK_S) == true || GetKeyboardTrigger(DIK_DOWN) == true)
		{//下移動
			if (nNumber != 2)
			{
				nNumber++;
			}
		}

		SelectWindow(nNumber);
		if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_Z) == true)
		{
			bPause = false;
			PlaySound(SOUND_LABEL_BOTTON000);
			if (nNumber == 0)
			{//戻る
				BackWindow(4);
				SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.8f, SCREEN_HEIGHT * 0.9f, 0.0f), D3DXCOLOR{ 1.0f,0.99f,0.04f, 0 }, 150, 30, 0, 0, 0, WINDOWSTATE_FLASH, WINDOWUSE_NORMAL);
			}
			else if (nNumber == 1)
			{//初めから
				SetFade(MODE_TUTORIAL, 0);
			}
			else if (nNumber == 2)
			{//Titleへ Titleでセットエネミーしているためプレイヤーを殺しとく	
				SetFade(MODE_TITLE, 0);		//Titleへ
			}
		}
	}
	if (GetKeyboardTrigger(DIK_P) == true && bPause == false)
	{//Pを押したらpause画面
		BackWindow(1);
		PlaySound(SOUND_LABEL_PAUSE000);
		bPause = true;
		SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXCOLOR{ 1.0f,1.0f,1.0f, 1.0f }, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 15, 0, 0, WINDOWSTATE_NORMAL, WINDOWUSE_NORMAL);
		SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.3f, 0.0f), D3DXCOLOR{ 0.23f,0.23f,0.23f, 1.0f }, 150, 30, 16, 0, 0, WINDOWSTATE_SELECTOFF, WINDOWUSE_SELECT);
		SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXCOLOR{ 0.23f,0.23f,0.23f, 1.0f }, 150, 30, 17, 1, 0, WINDOWSTATE_SELECTOFF, WINDOWUSE_SELECT);
		SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.7f, 0.0f), D3DXCOLOR{ 0.23f,0.23f,0.23f, 1.0f }, 150, 30, 18, 2, 0, WINDOWSTATE_SELECTOFF, WINDOWUSE_SELECT);
	}
	else if (GetKeyboardTrigger(DIK_P) == true)
	{//Pを押したらpause解く
		PlaySound(SOUND_LABEL_PAUSE000);
		bPause = false;
		BackWindow(4);
		SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.8f, SCREEN_HEIGHT * 0.9f, 0.0f), D3DXCOLOR{ 1.0f,0.99f,0.04f, 0 }, 150, 30, 0, 0, 0, WINDOWSTATE_FLASH, WINDOWUSE_NORMAL);
	}


}
//=========================================================================================================//
// * ゲームの描画処理
//=========================================================================================================//
void DrawTutorial(void)
{
	DrawExplosion();
	DrawPlayer();
	DrawShooter();
	DrawEnemy();
	DrawBullet();
	DrawGameWindow();
	DrawEffect();
}