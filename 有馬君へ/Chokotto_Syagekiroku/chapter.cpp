//=========================================================================================================//
//
// 画面切り替え処理 [chapter.cpp]
// Author : Ryou Sugimoto
//
//=========================================================================================================//
#include "main.h"
#include "game.h"
#include "enemy.h"
#include "input.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMYHP_EASY	(4)
#define ENEMYHP_NORMAL	(5)
#define ENEMYHP_HARD	(6)
#define ENEMYHP_LUNATIC	(7)
#define ENEMYSTOP		(30)
#define ENEMYSTOP1		(20)

//=============================================================================
// 敵の出現設定
//=============================================================================
void ChapterEnemy(void)
{
	bool bRespawn;
	IfBoss *ifboss = GetBoss();

	PLAYER *pPlayer = GetPlayer();
	Difficulty *pDifficulty = GetDifficulty();//難易度ごとの敵情報取得
	GameCounter *pGameCounter = GetGamecounter();
	FADE *pFade = GetFade();
	if (pFade[0] == FADE_IN && pGameCounter[0].nCntData == 0)
	{ 
		int nRand = rand() % 64;
		pPlayer[0].bUse = true;
		SetShooter(D3DXVECTOR3(cosf(D3DX_PI * 0.25) * 0.5f, cosf(D3DX_PI * 0.25) * 0.5f, 0.0f), nRand);
		nRand = rand() % 64;
		SetShooter(D3DXVECTOR3(cosf(D3DX_PI * -0.25) * 0.5f, cosf(D3DX_PI * -0.25) * 0.5f, 0.0f), nRand);
		pGameCounter[0].nCntData++;
	}

	if (pPlayer[0].bUse == true && pFade[0] == FADE_NONE)
	{//ゲーム開始
		if (pGameCounter[0].nCntData == 1)
		{//音楽再生
			SetFieldBGM();
			//pGameCounter[0].nCntData = 2300;
		}

		if (pGameCounter[0].nCntData < 2001 || ifboss[0].nEnemyCounter == 1)
		{//ボス手前でカウントストップ　ボスのみになったらSTART
			pGameCounter[0].nCntData++;
		}
		if (pGameCounter[0].nCntData > 60)
		{//始まって1秒は敵が出ない
			if (ifboss[0].g_bBossKill == true && pPlayer[0].bUse == true)
			{//ボスを倒していたら
				pGameCounter[0].nCntClear++;
				if (pGameCounter[0].nCntClear > 100 && pGameCounter[0].nEnd == 2)
				{//カウントの初期化　Mode切り替え
					ifboss[0].g_bBossKill = false;
					ifboss[0].nEnemyCounter = 1;
					SetGameState(GAMESTATE_END, MODE_RESOLT, 200);
				}
				if (GetKeyboardTrigger(DIK_RETURN) == true)
				{
					pGameCounter[0].nCntClear = 150;
				}
				if (pGameCounter[0].nCntClear % 150 == 0)
				{
					if (pGameCounter[0].nCntClear == 150 && pGameCounter[0].nEnd == 0)
					{
						pGameCounter[0].nCntClear = 0;
						pGameCounter[0].nEnd = 1;
						SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXCOLOR{ 1.0f,1.0f,1.0f, 0.0f }, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 6, 0, 0, WINDOWSTATE_FADEIN, WINDOWUSE_NORMAL);
						SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.75f, 0.0f), D3DXCOLOR{ 1.0f,0.99f,0.04f, 0.0f }, 150, 50, 5, 0, 0, WINDOWSTATE_FLASH, WINDOWUSE_NORMAL);
					}
					else if (pGameCounter[0].nCntClear == 150 && pGameCounter[0].nEnd == 1)
					{
						pGameCounter[0].nCntClear = 0;
						pGameCounter[0].nEnd = 2;
					}
				}
			}
			if (pGameCounter[0].nCntData % 30 == 0)
			{
				if (pDifficulty[0].nNumber == 0)	  { EASYMode(); }
				else if (pDifficulty[0].nNumber == 1) { NORMALMode(); }
				else if (pDifficulty[0].nNumber == 2) { HARDMode(); }
				else if (pDifficulty[0].nNumber == 3) { LUNATICMode(); }
			}
		}
	}
	else if(pPlayer[0].bUse == false && pGameCounter[0].nCntData > 0)
	{//プレイヤーが死亡
		pGameCounter[0].nCntGameOver[0]++;
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{//エンターキーで早送り
			pGameCounter[0].nCntGameOver[0] = 200;
			if (pGameCounter[0].nEnd == 2)
			{
				pGameCounter[0].nCntGameOver[1] += 100;
			}
		}
		if (pGameCounter[0].nEnd == 2)
		{
			pGameCounter[0].nCntGameOver[1]++;
			if (pGameCounter[0].nCntGameOver[1] > 200 || pGameCounter[0].nCntGameOver[0] > 800)
			{
				SetGameState(GAMESTATE_END, MODE_RESOLT, 200);
			}
		}
		if (pGameCounter[0].nCntGameOver[0] % 100 == 0 && pGameCounter[0].nEnd == 0)
		{
			bRespawn = BreakAirport();
			if (bRespawn == false)
			{//タイトルに戻る
				pGameCounter[0].nEnd = 1;
				StopSound();
				PlaySound(SOUND_LABEL_GAMEOVER000);
				SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXCOLOR{ 1.0f,1.0f,1.0f, 0.0f }, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 3, 0, 0, WINDOWSTATE_STEALTH, WINDOWUSE_NORMAL);
			}
			else
			{//復活
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
				PlaySound(SOUND_LABEL_BOTTON000);
			}
			pGameCounter[0].nCntGameOver[0] = 0;
		}
		else if (pGameCounter[0].nCntGameOver[0] % 200 == 0)
		{
			SetWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXCOLOR{ 1.0f,1.0f,1.0f, 0.0f }, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 4, 0, 0, WINDOWSTATE_STEALTH, WINDOWUSE_NORMAL);
			pGameCounter[0].nEnd = 2;
		}
	}
}
//=============================================================================
// 難易度ごとに出現情報変更 EASY
//=============================================================================
void EASYMode(void)
{
	GameCounter *pGameCounter = GetGamecounter();
	int nRand = rand() % 64;

	if (pGameCounter[0].nCntData > 60 && pGameCounter[0].nCntData < 291)
	{//attack = { BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 10,20 },3,4,5,{ 60,70 },8,9,10,11,13.3f,14.5f };
		SetEnemy(D3DXVECTOR3(GAME_WIDE, SCREEN_HEIGHT / 3, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, D3DX_PI * -0.5f, 0.0f, 0.3f, pGameCounter[0].nCntData, nRand, ENEMYHP_EASY,
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_STOP,{ 0,0 },0,40,{ 60,30 },{ 10, 10 }, 2,0,6,ENEMYSTOP,0.001f, 0.0f,0.08f, -0.005f, -0.5f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
	}//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
	else if (pGameCounter[0].nCntData > 350 && pGameCounter[0].nCntData < 591)
	{//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
		SetEnemy(D3DXVECTOR3(0, SCREEN_HEIGHT / 3, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, D3DX_PI * 0.75f, 0.0f, 0.3f, pGameCounter[0].nCntData, nRand, ENEMYHP_EASY,
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_STOP,{ 0,0 },0,40,{ 60,30 },{ 10, 10 }, 2,0,6,ENEMYSTOP,0.001f, 0.0f,0.08f, -0.005f, -0.5f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
	}//初期位置, 敵の種類, 向き(角度), 方向転換速度と方向( + or - ), 移動速度, 攻撃パターン, 攻撃速度, 攻撃タイミング, 労働時間, 休憩時間, 画像, HP
	else if (pGameCounter[0].nCntData > 650 && pGameCounter[0].nCntData < 891)
	{//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
		SetEnemy(D3DXVECTOR3(GAME_WIDE, SCREEN_HEIGHT / 3, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, D3DX_PI * -0.75f, 0.0f, 0.3f, pGameCounter[0].nCntData, nRand, ENEMYHP_EASY,
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_STOP,{ 0,0 },6,56,{ 60,0 },{ 8, 8 }, 8,0,1,ENEMYSTOP,0.001f, 0.0f,0.0f, 0.0f, 0.3f, 30.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
	}//初期位置, 敵の種類, 向き(角度), 方向転換速度と方向( + or - ), 移動速度, 攻撃パターン, 攻撃速度, 攻撃タイミング, 労働時間, 休憩時間, 画像, HP
	else if (pGameCounter[0].nCntData > 950 && pGameCounter[0].nCntData < 1191)
	{//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
		SetEnemy(D3DXVECTOR3(0, SCREEN_HEIGHT / 3, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, D3DX_PI * 0.5f, 0.0f, 0.3f, pGameCounter[0].nCntData, nRand, ENEMYHP_EASY,
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_STOP,{ 0,0 },6,56,{ 60,0 },{ 8, 8 }, 8,0,1,ENEMYSTOP,0.001f, 0.0f,0.0f, 0.0f, 0.3f, 30.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
	}//初期位置, 敵の種類, 向き(角度), 方向転換速度と方向( + or - ), 移動速度, 攻撃パターン, 攻撃速度, 攻撃タイミング, 労働時間, 休憩時間, 画像, HP
	else if (pGameCounter[0].nCntData > 1250 && pGameCounter[0].nCntData < 1491)
	{//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
		SetEnemy(D3DXVECTOR3(GAME_WIDE, 0, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, D3DX_PI * -0.25f, 0.0f, 0.3f, pGameCounter[0].nCntData, nRand, ENEMYHP_EASY,
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },1,52,{ 60,1 },{ 6, 6 }, 8,0,1,0,0.0f, 0.0f,0.0f, 0.0f, 0.7f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
	}//初期位置, 敵の種類, 向き(角度), 方向転換速度と方向( + or - ), 移動速度, 攻撃パターン, 攻撃速度, 攻撃タイミング, 労働時間, 休憩時間, 画像, HP
	else if (pGameCounter[0].nCntData > 1550 && pGameCounter[0].nCntData < 1791)
	{//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
		SetEnemy(D3DXVECTOR3(0, 0, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, D3DX_PI * 0.25f, 0.0f, 0.3f, pGameCounter[0].nCntData, nRand, ENEMYHP_EASY,
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },1,52,{ 60,1 },{ 6, 6 }, 8,0,1,0,0.0f, 0.0f,0.0f, 0.0f, 0.7f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
	}//初期位置, 敵の種類, 向き(角度), 方向転換速度と方向( + or - ), 移動速度, 攻撃パターン, 攻撃速度, 攻撃タイミング, 労働時間, 休憩時間, 画像, HP
	else if (pGameCounter[0].nCntData == 1980)//小ボス
	{//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
		SetEnemy(D3DXVECTOR3(GAME_WIDE / 2, 0, 0.0f), ENEMYTYPE_SMALL, ENEMYMODE_NONE, 0.0f, 0.0f, 0.2f, pGameCounter[0].nCntData, 0, 100,
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },0,51,{ 1,30 },{ 8, 8 }, 3,0,4,0,0.0f, 0.0f,0.03f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },0,59,{ 1,30 },{ 8, 8 }, 3,0,4,0,0.0f, 0.0f,-0.03f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
		SetEnemy(D3DXVECTOR3(GAME_WIDE / 2 - 200, 0, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, 0.0f, 0.0f, 0.3f, pGameCounter[0].nCntData, nRand, ENEMYHP_EASY,
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },0,56,{ 60,0 },{ 8, 8 }, 8,0,1,0,0.0f, 0.0f,0.08f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
		SetEnemy(D3DXVECTOR3(GAME_WIDE / 2 + 200, 0, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, 0.0f, 0.0f, 0.3f, pGameCounter[0].nCntData, nRand, ENEMYHP_EASY,
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },0,56,{ 60,0 },{ 8, 8 }, 8,0,1,0,0.0f, 0.0f,0.08f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
		pGameCounter[0].nCntData++;
	}//初期位置, 敵の種類, 向き(角度), 方向転換速度と方向( + or - ), 移動速度, 攻撃パターン, 攻撃速度, 攻撃タイミング, 労働時間, 休憩時間, 画像, HP
	else if (pGameCounter[0].nCntData == 2190)//中ボス
	{//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
		SetEnemy(D3DXVECTOR3(GAME_WIDE / 2, 0, 0.0f), ENEMYTYPE_MEDIUM, ENEMYMODE_MBOSS_EASY, 0.0f, 0.0f, 0.2f, pGameCounter[0].nCntData, 2, 350,
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },0,56,{ 150,1 },{ 8,8 }, 32,0,1,0,0.0f, 0.0f,0.03f, 0.0f, 0.4f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_STOP,{ 0,0 },0,52,{ 150,1 },{ 8,8 }, 32,0,1,20,0.0f, 0.0f,0.03f, 0.0f, 0.35f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
		pGameCounter[0].nCntData++;
	}//初期位置, 敵の種類, 向き(角度), 方向転換速度と方向( + or - ), 移動速度, 攻撃パターン, 攻撃速度, 攻撃タイミング, 労働時間, 休憩時間, 画像, HP
	if (pGameCounter[0].nCntData == 2400)
	{//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
		SetEnemy(D3DXVECTOR3(GAME_WIDE * 0.5f, 0, 0.0f), ENEMYTYPE_BIG, ENEMYMODE_LBOSS_EASY_3, 0.0f, 0.0f, 0.4f, pGameCounter[0].nCntData, 1, 400,
			ATTACK{ BULLETSTATE_STRAIGHT_B, BULLETAIM_NORMAL,{ 0,0 },0,52,{ 200,60 },{ 8,8 }, 8,0,6,0,0.01f, 0.0f,0.1f, 0.3f, -1.0f, 50.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_STRAIGHT, BULLETAIM_NORMAL,{ 0,0 },0,58,{ 200,60 },{ 8,8 }, 8,0,6,0,0.01f, 0.0f,0.1f, 0.3f, 0.5f, 50.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT },
			ATTACK{ BULLETSTATE_NOT },
			ATTACK{ BULLETSTATE_NOT });
		pGameCounter[0].nCntData++;
	}
}
//=============================================================================
// 難易度ごとに出現情報変更 NORMAL
//=============================================================================
void NORMALMode(void)
{
	GameCounter *pGameCounter = GetGamecounter();
	int nRand = rand() % 64;

	if (pGameCounter[0].nCntData > 60 && pGameCounter[0].nCntData < 291)
	{//attack = { BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 10,20 },3,4,5,{ 60,70 },8,9,10,11,13.3f,14.5f };
		SetEnemy(D3DXVECTOR3(GAME_WIDE, 0, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, -0.4f, -0.007f, 0.3f, pGameCounter[0].nCntData, nRand, ENEMYHP_NORMAL,
			ATTACK{ BULLETSTATE_STOP_A, BULLETAIM_STOP,{ 0,0 },0,24,{ 60,30 },{ 10, 12 }, 2,0,3,ENEMYSTOP,0.001f, 0.0f,0.08f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT },ATTACK{ BULLETSTATE_NOT },ATTACK{ BULLETSTATE_NOT },ATTACK{ BULLETSTATE_NOT });
	}//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
	else if (pGameCounter[0].nCntData > 350 && pGameCounter[0].nCntData < 591)
	{//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
		SetEnemy(D3DXVECTOR3(0, 0, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, 0.4f, 0.007f, 0.3f, pGameCounter[0].nCntData, nRand, ENEMYHP_NORMAL,
			ATTACK{ BULLETSTATE_STOP_A, BULLETAIM_STOP,{ 0,0 },0,24,{ 60,30 },{ 10, 12 }, 2,0,3,ENEMYSTOP,0.001f, 0.0f,0.08f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
	}//初期位置, 敵の種類, 向き(角度), 方向転換速度と方向( + or - ), 移動速度, 攻撃パターン, 攻撃速度, 攻撃タイミング, 労働時間, 休憩時間, 画像, HP
	else if (pGameCounter[0].nCntData > 650 && pGameCounter[0].nCntData < 891)
	{//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
		SetEnemy(D3DXVECTOR3(GAME_WIDE * 0.6f, 0, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, -0.3f, -0.002f, 0.3f, pGameCounter[0].nCntData, nRand, ENEMYHP_NORMAL,
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_STOP,{ 0,0 },0,56,{ 60,0 },{ 8, 8 }, 8,0,1,ENEMYSTOP,0.001f, 0.0f,0.08f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
	}//初期位置, 敵の種類, 向き(角度), 方向転換速度と方向( + or - ), 移動速度, 攻撃パターン, 攻撃速度, 攻撃タイミング, 労働時間, 休憩時間, 画像, HP
	else if (pGameCounter[0].nCntData > 950 && pGameCounter[0].nCntData < 1191)
	{//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
		SetEnemy(D3DXVECTOR3(GAME_WIDE * 0.3f, 0, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, 0.3f, 0.002f, 0.3f, pGameCounter[0].nCntData, nRand, ENEMYHP_NORMAL,
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_STOP,{ 0,0 },0,56,{ 60,0 },{ 8, 8 }, 8,0,1,ENEMYSTOP,0.001f, 0.0f,0.08f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
	}//初期位置, 敵の種類, 向き(角度), 方向転換速度と方向( + or - ), 移動速度, 攻撃パターン, 攻撃速度, 攻撃タイミング, 労働時間, 休憩時間, 画像, HP
	else if (pGameCounter[0].nCntData > 1250 && pGameCounter[0].nCntData < 1491)
	{//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
		SetEnemy(D3DXVECTOR3(GAME_WIDE, 0, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, D3DX_PI * -0.25f, 0.0f, 0.3f, pGameCounter[0].nCntData, nRand, ENEMYHP_NORMAL,
			ATTACK{ BULLETSTATE_START, BULLETAIM_STOP,{ 0,0 },3,0,{ 60,0 },{ 8, 8 }, 3,0,1,ENEMYSTOP1,5.0f, 0.0f,0.0f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
	}//初期位置, 敵の種類, 向き(角度), 方向転換速度と方向( + or - ), 移動速度, 攻撃パターン, 攻撃速度, 攻撃タイミング, 労働時間, 休憩時間, 画像, HP
	else if (pGameCounter[0].nCntData > 1550 && pGameCounter[0].nCntData < 1791)
	{//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
		SetEnemy(D3DXVECTOR3(0, 0, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, D3DX_PI * 0.25f, 0.0f, 0.3f, pGameCounter[0].nCntData, nRand, ENEMYHP_NORMAL,
			ATTACK{ BULLETSTATE_START, BULLETAIM_STOP,{ 0,0 },3,0,{ 60,0 },{ 8, 8 }, 3,0,1,ENEMYSTOP1,20.0f, 0.0f,0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
	}//初期位置, 敵の種類, 向き(角度), 方向転換速度と方向( + or - ), 移動速度, 攻撃パターン, 攻撃速度, 攻撃タイミング, 労働時間, 休憩時間, 画像, HP
	else if (pGameCounter[0].nCntData == 1980)//小ボス
	{//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
		SetEnemy(D3DXVECTOR3(GAME_WIDE / 2, 0, 0.0f), ENEMYTYPE_SMALL, ENEMYMODE_NONE, 0.0f, 0.0f, 0.2f, pGameCounter[0].nCntData, 4, 100,
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },0,51,{ 1,30 },{ 8, 8 }, 4,0,3,0,0.0f, 0.0f,0.03f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },0,59,{ 1,30 },{ 8, 8 }, 4,0,3,0,0.0f, 0.0f,-0.03f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
		SetEnemy(D3DXVECTOR3(GAME_WIDE / 2 - 200, 0, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, 0.0f, 0.0f, 0.3f, pGameCounter[0].nCntData, nRand, ENEMYHP_NORMAL,
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },0,56,{ 60,0 },{ 8, 8 }, 8,0,1,0,0.0f, 0.0f,0.08f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
		SetEnemy(D3DXVECTOR3(GAME_WIDE / 2 + 200, 0, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, 0.0f, 0.0f, 0.3f, pGameCounter[0].nCntData, nRand, ENEMYHP_NORMAL,
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },0,56,{ 60,0 },{ 8, 8 }, 8,0,1,0,0.0f, 0.0f,0.08f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
		pGameCounter[0].nCntData++;
	}//初期位置, 敵の種類, 向き(角度), 方向転換速度と方向( + or - ), 移動速度, 攻撃パターン, 攻撃速度, 攻撃タイミング, 労働時間, 休憩時間, 画像, HP
	else if (pGameCounter[0].nCntData == 2190)//中ボス
	{//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
		SetEnemy(D3DXVECTOR3(GAME_WIDE / 2, 0, 0.0f), ENEMYTYPE_MEDIUM, ENEMYMODE_MBOSS_NORMAL, 0.0f, 0.0f, 0.2f, pGameCounter[0].nCntData, 8, 400,
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },0,53,{90,30 },{ 8, 8 }, 32,0,4,0,0.0f, 0.0f,0.03f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT },ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
		pGameCounter[0].nCntData++;
	}//初期位置, 敵の種類, 向き(角度), 方向転換速度と方向( + or - ), 移動速度, 攻撃パターン, 攻撃速度, 攻撃タイミング, 労働時間, 休憩時間, 画像, HP
	else if (pGameCounter[0].nCntData == 2400)
	{//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
		SetEnemy(D3DXVECTOR3(GAME_WIDE * 0.5f, 0, 0.0f), ENEMYTYPE_BIG, ENEMYMODE_LBOSS_NORMAL_3, 0.0f, 0.0f, 0.4f, pGameCounter[0].nCntData, 3, 450,
			ATTACK{ BULLETSTATE_STOP_A, BULLETAIM_NORMAL,{ 0,0 },0,56,{ 60,30 },{ 8, 8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT },
			ATTACK{ BULLETSTATE_NOT },
			ATTACK{ BULLETSTATE_NOT },
			ATTACK{ BULLETSTATE_NOT });
		pGameCounter[0].nCntData++;
	}
}
//=============================================================================
// 難易度ごとに出現情報変更 HARD
//=============================================================================
void HARDMode(void)
{
	GameCounter *pGameCounter = GetGamecounter();
	int nRand = rand() % 64;

	if (pGameCounter[0].nCntData > 60 && pGameCounter[0].nCntData < 291)
	{//attack = { BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 10,20 },3,4,5,{ 60,70 },8,9,10,11,13.3f,14.5f };
		SetEnemy(D3DXVECTOR3(GAME_WIDE, 0, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, -0.4f, -0.007f, 0.3f, pGameCounter[0].nCntData, nRand, ENEMYHP_HARD,
			ATTACK{ BULLETSTATE_STOP_B, BULLETAIM_STOP,{ 0,0 },0,19,{ 60,30 },{ 10, 12 }, 4,0,3,ENEMYSTOP1,0.01f, 0.0f,0.08f, -0.005f, -0.3f, 40.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
	}//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
	else if (pGameCounter[0].nCntData > 350 && pGameCounter[0].nCntData < 591)
	{//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
		SetEnemy(D3DXVECTOR3(0, 0, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, 0.4f, 0.007f, 0.3f, pGameCounter[0].nCntData, nRand, ENEMYHP_HARD,
			ATTACK{ BULLETSTATE_STOP_B, BULLETAIM_STOP,{ 0,0 },0,19,{ 60,30 },{ 10, 12 }, 4,0,3,ENEMYSTOP1,0.01f, 0.0f,0.08f, -0.005f, -0.3f, 40.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
	}//初期位置, 敵の種類, 向き(角度), 方向転換速度と方向( + or - ), 移動速度, 攻撃パターン, 攻撃速度, 攻撃タイミング, 労働時間, 休憩時間, 画像, HP
	else if (pGameCounter[0].nCntData > 650 && pGameCounter[0].nCntData < 891)
	{//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離, セット2つ		休憩, 連射は最低1
		SetEnemy(D3DXVECTOR3(GAME_WIDE * 0.6f, 0, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, -0.3f, -0.002f, 0.3f, pGameCounter[0].nCntData, nRand, ENEMYHP_HARD,
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_STOP,{ 0,0 },1,59,{ 1,30 },{ 7, 7 }, 8,0,3,ENEMYSTOP1,0.0f, 0.0f,0.0f, 0.0f, 0.6f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
	}//初期位置, 敵の種類, 向き(角度), 方向転換速度と方向( + or - ), 移動速度, 攻撃パターン, 攻撃速度, 攻撃タイミング, 労働時間, 休憩時間, 画像, HP
	else if (pGameCounter[0].nCntData > 950 && pGameCounter[0].nCntData < 1191)
	{//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
		SetEnemy(D3DXVECTOR3(GAME_WIDE * 0.3f, 0, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, 0.3f, 0.002f, 0.3f, pGameCounter[0].nCntData, nRand, ENEMYHP_HARD,
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_STOP,{ 0,0 },1,59,{ 1,30 },{ 7, 7 }, 8,0,3,ENEMYSTOP1,0.0f, 0.0f,0.0f, 0.0f, 0.6f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
	}//初期位置, 敵の種類, 向き(角度), 方向転換速度と方向( + or - ), 移動速度, 攻撃パターン, 攻撃速度, 攻撃タイミング, 労働時間, 休憩時間, 画像, HP
	else if (pGameCounter[0].nCntData > 1250 && pGameCounter[0].nCntData < 1491)
	{//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
		SetEnemy(D3DXVECTOR3(GAME_WIDE, 0, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, D3DX_PI * -0.25f, 0.0f, 0.3f, pGameCounter[0].nCntData, nRand, ENEMYHP_HARD,
			ATTACK{ BULLETSTATE_BACK, BULLETAIM_NORMAL,{ 0,0 },0,41,{ 2500,1 },{ 20,20 }, 32,0,1,ENEMYSTOP1,0.009f, 0.0f,0.03f, -0.004f, -0.5f, 30.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },0,56,{ 20,0 },{ 8, 8 }, 8,0,1,80,0.0f, 0.0f,0.08f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
	}//初期位置, 敵の種類, 向き(角度), 方向転換速度と方向( + or - ), 移動速度, 攻撃パターン, 攻撃速度, 攻撃タイミング, 労働時間, 休憩時間, 画像, HP
	else if (pGameCounter[0].nCntData > 1550 && pGameCounter[0].nCntData < 1791)
	{//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
		SetEnemy(D3DXVECTOR3(0, 0, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, D3DX_PI * 0.25f, 0.0f, 0.3f, pGameCounter[0].nCntData, nRand, ENEMYHP_HARD,
			ATTACK{ BULLETSTATE_BACK, BULLETAIM_NORMAL,{ 0,0 },0,41,{ 2500,1 },{ 20,20 }, 32,0,1,ENEMYSTOP1,0.009f, 0.0f,0.03f, 0.004f, -0.5f, 30.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },0,56,{ 20,0 },{ 8, 8 }, 8,0,1,80,0.0f, 0.0f,0.08f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
	}//初期位置, 敵の種類, 向き(角度), 方向転換速度と方向( + or - ), 移動速度, 攻撃パターン, 攻撃速度, 攻撃タイミング, 労働時間, 休憩時間, 画像, HP
	else if (pGameCounter[0].nCntData == 1980)//小ボス
	{//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
		SetEnemy(D3DXVECTOR3(GAME_WIDE / 2, 0, 0.0f), ENEMYTYPE_SMALL, ENEMYMODE_NONE, 0.0f, 0.0f, 0.2f, pGameCounter[0].nCntData, 9, 200,
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },0,51,{ 1,600000 },{ 8, 8 }, 8,0,5,0,0.0f, 0.0f,0.03f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },0,59,{ 1,600000 },{ 8, 8 }, 8,0,5,0,0.0f, 0.0f,0.015f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
		SetEnemy(D3DXVECTOR3(GAME_WIDE / 2 - 200, 0, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, 0.0f, 0.0f, 0.3f, pGameCounter[0].nCntData, nRand, ENEMYHP_HARD,
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },0,56,{ 20,0 },{ 8, 8 }, 8,0,1,0,0.0f, 0.0f,0.08f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
		SetEnemy(D3DXVECTOR3(GAME_WIDE / 2 + 200, 0, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, 0.0f, 0.0f, 0.3f, pGameCounter[0].nCntData, nRand, ENEMYHP_HARD,
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },0,56,{ 20,0 },{ 8, 8 }, 8,0,1,0,0.0f, 0.0f,0.08f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
		pGameCounter[0].nCntData++;
	}//初期位置, 敵の種類, 向き(角度), 方向転換速度と方向( + or - ), 移動速度, 攻撃パターン, 攻撃速度, 攻撃タイミング, 労働時間, 休憩時間, 画像, HP
	else if (pGameCounter[0].nCntData == 2190)//中ボス
	{//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
		SetEnemy(D3DXVECTOR3(GAME_WIDE / 2, 0, 0.0f), ENEMYTYPE_MEDIUM, ENEMYMODE_MBOSS_HARD, 0.0f, 0.0f, 0.2f, pGameCounter[0].nCntData, 4, 450,
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },0,51,{ 1,600000 },{ 8, 8 }, 3,0,4,0,0.0f, 0.0f,0.03f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },0,59,{ 1,600000 },{ 8, 8 }, 3,0,4,0,0.0f, 0.0f,-0.03f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_BACK, BULLETAIM_NORMAL,{ 0,0 },0,41,{ 250,1 },{ 20,20 }, 32,0,1,0,0.009f, 0.0f,0.03f, 0.004f, -0.5f, 30.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
		pGameCounter[0].nCntData++;
	}//初期位置, 敵の種類, 向き(角度), 方向転換速度と方向( + or - ), 移動速度, 攻撃パターン, 攻撃速度, 攻撃タイミング, 労働時間, 休憩時間, 画像, HP
	else if (pGameCounter[0].nCntData == 2400)
	{//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
		SetEnemy(D3DXVECTOR3(GAME_WIDE * 0.5f, 0, 0.0f), ENEMYTYPE_BIG, ENEMYMODE_LBOSS_HARD_3, 0.0f, 0.0f, 0.4f, pGameCounter[0].nCntData, 7, 550,
			ATTACK{ BULLETSTATE_STOP_S, BULLETAIM_NORMAL,{ 0,0 },0,51,{ 90,30 },{ 8, 8 }, 3,0,4,0,10.0f, 0.0f,0.03f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_STOP_S, BULLETAIM_NORMAL,{ 0,0 },0,59,{ 90,30 },{ 8, 8 }, 3,0,4,0,10.0f, 0.0f,-0.03f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_BACK, BULLETAIM_NORMAL,{ 0,0 },0,36,{ 60,1 },{ 20,20 }, 16,0,1,60,0.01f, 0.0f,-0.03f, 0.006f, 0.5f, 50.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT },
			ATTACK{ BULLETSTATE_NOT });
		pGameCounter[0].nCntData++;
	}
}
//=============================================================================
// 難易度ごとに出現情報変更 LUNATIC
//=============================================================================
void LUNATICMode(void)
{
	GameCounter *pGameCounter = GetGamecounter();
	int nRand = rand() % 64;

	if (pGameCounter[0].nCntData > 60 && pGameCounter[0].nCntData < 291)
	{//attack = { BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 10,20 },3,4,5,{ 60,70 },8,9,10,11,13.3f,14.5f };
		SetEnemy(D3DXVECTOR3(0, SCREEN_HEIGHT / 3, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, D3DX_PI * 0.5f, 0.0f, 0.3f, pGameCounter[0].nCntData, nRand, ENEMYHP_LUNATIC,
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },1,61,{ 0,10000 },{ 6, 6 }, 8,0,6,0,0.0f, 0.0f,0.0f, 0.0f, 0.7f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
	}//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
	else if (pGameCounter[0].nCntData > 350 && pGameCounter[0].nCntData < 591)
	{//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
		SetEnemy(D3DXVECTOR3(GAME_WIDE, 0, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, -0.4f, -0.007f, 0.3f, pGameCounter[0].nCntData, nRand, ENEMYHP_LUNATIC,
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },1,61,{ 0,10000 },{ 6, 6 }, 8,0,6,0,0.0f, 0.0f,0.0f, 0.0f, 0.7f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
	}//初期位置, 敵の種類, 向き(角度), 方向転換速度と方向( + or - ), 移動速度, 攻撃パターン, 攻撃速度, 攻撃タイミング, 労働時間, 休憩時間, 画像, HP
	else if (pGameCounter[0].nCntData > 650 && pGameCounter[0].nCntData < 891)
	{//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
		SetEnemy(D3DXVECTOR3(GAME_WIDE / 2, 0, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, 0.0f, 0.0f, 0.5f, pGameCounter[0].nCntData, nRand, ENEMYHP_LUNATIC,
			ATTACK{ BULLETSTATE_BACK, BULLETAIM_NORMAL,{ 0,0 },0,44,{ 250,1 },{ 20,20 }, 32,0,1,ENEMYSTOP1,0.009f, 0.0f,0.03f, 0.004f, -0.5f, 30.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_STOP,{ 0,0 },0,60,{ 20,0 },{ 8, 8 }, 8,0,1,80,0.0f, 0.0f,0.08f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
	}//初期位置, 敵の種類, 向き(角度), 方向転換速度と方向( + or - ), 移動速度, 攻撃パターン, 攻撃速度, 攻撃タイミング, 労働時間, 休憩時間, 画像, HP
	else if (pGameCounter[0].nCntData == 1020)
	{//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
		SetEnemy(D3DXVECTOR3(GAME_WIDE, 200, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, D3DX_PI * -0.5f, 0.0f, 0.4f, pGameCounter[0].nCntData, nRand, ENEMYHP_LUNATIC,
			ATTACK{ BULLETSTATE_SET_A, BULLETAIM_STOP,{ 0,0 },0,29,{ 110,0 },{ 10, 12 }, 1,0,1,ENEMYSTOP,0.0f, 0.0f,0.0f, 0.0f, 0.8f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
		SetEnemy(D3DXVECTOR3(0, 100, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, D3DX_PI * 0.5f, 0.0f, 0.4f, pGameCounter[0].nCntData, nRand, ENEMYHP_LUNATIC,
			ATTACK{ BULLETSTATE_SET_B1, BULLETAIM_STOP,{ 0,0 },0,30,{ 110,0 },{ 10, 12 }, 1,0,1,ENEMYSTOP,0.001f, 0.0f,0.0f, 0.0f, 0.8f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
	}//初期位置, 敵の種類, 向き(角度), 方向転換速度と方向( + or - ), 移動速度, 攻撃パターン, 攻撃速度, 攻撃タイミング, 労働時間, 休憩時間, 画像, HP
	else if (pGameCounter[0].nCntData > 1250 && pGameCounter[0].nCntData < 1491)
	{//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
		SetEnemy(D3DXVECTOR3(GAME_WIDE - 100, 0, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, 0.0f, 0.0f, 0.3f, pGameCounter[0].nCntData, nRand, ENEMYHP_LUNATIC,
			ATTACK{ BULLETSTATE_ACCELERATOR, BULLETAIM_STOP,{ 0,0 },1,61,{ 1,60 },{ 8,8 }, 22,0,30,ENEMYSTOP,0.001f, 0.0f,0.0f, 0.0f, 0.1f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
	}//初期位置, 敵の種類, 向き(角度), 方向転換速度と方向( + or - ), 移動速度, 攻撃パターン, 攻撃速度, 攻撃タイミング, 労働時間, 休憩時間, 画像, HP
	else if (pGameCounter[0].nCntData > 1550 && pGameCounter[0].nCntData < 1791)
	{//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
		SetEnemy(D3DXVECTOR3(100, 0, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, 0.0f, 0.0f, 0.3f, pGameCounter[0].nCntData, nRand, ENEMYHP_LUNATIC,
			ATTACK{ BULLETSTATE_ACCELERATOR, BULLETAIM_STOP,{ 0,0 },1,61,{ 1,60 },{ 8,8 }, 22,0,30,ENEMYSTOP,0.001f, 0.0f,0.0f, 0.0f, 0.1f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
	}//初期位置, 敵の種類, 向き(角度), 方向転換速度と方向( + or - ), 移動速度, 攻撃パターン, 攻撃速度, 攻撃タイミング, 労働時間, 休憩時間, 画像, HP
	else if (pGameCounter[0].nCntData == 1980)//小ボス
	{//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
		SetEnemy(D3DXVECTOR3(GAME_WIDE / 2, 0, 0.0f), ENEMYTYPE_SMALL, ENEMYMODE_NONE, 0.0f, 0.0f, 0.2f, pGameCounter[0].nCntData, 3, 100,
			ATTACK{ BULLETSTATE_START, BULLETAIM_NORMAL,{ 0,0 },2,0,{ 120,120 },{ 8, 8 }, 5,0,10,0,5.0f, D3DX_PI * -0.5,D3DX_PI * 0.0056f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_START, BULLETAIM_NORMAL,{ 0,0 },2,0,{ 120,120 },{ 8, 8 }, 5,0,10,0,5.0f, D3DX_PI * 0.5,D3DX_PI * 0.0056f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
		SetEnemy(D3DXVECTOR3(GAME_WIDE, 100, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, D3DX_PI * -0.5f, 0.0f, 0.4f, pGameCounter[0].nCntData, nRand, ENEMYHP_LUNATIC,
			ATTACK{ BULLETSTATE_SET_B1, BULLETAIM_STOP,{ 0,0 },0,30,{ 500,0 },{ 10, 12 }, 1,0,1,60,0.0f, 0.0f,0.0f, 0.0f, 0.8f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
		SetEnemy(D3DXVECTOR3(0, 100, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE, D3DX_PI * 0.5f, 0.0f, 0.4f, pGameCounter[0].nCntData, nRand, ENEMYHP_LUNATIC,
			ATTACK{ BULLETSTATE_SET_B1, BULLETAIM_STOP,{ 0,0 },0,30,{ 500,0 },{ 10, 12 }, 1,0,1,60,0.001f, 0.0f,0.0f, 0.0f, 0.8f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
		pGameCounter[0].nCntData++;
	}//初期位置, 敵の種類, 向き(角度), 方向転換速度と方向( + or - ), 移動速度, 攻撃パターン, 攻撃速度, 攻撃タイミング, 労働時間, 休憩時間, 画像, HP
	else if (pGameCounter[0].nCntData == 2190)//中ボス
	{//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
		SetEnemy(D3DXVECTOR3(GAME_WIDE / 2, 0, 0.0f), ENEMYTYPE_MEDIUM, ENEMYMODE_MBOSS_LUNATIC, 0.0f, 0.0f, 0.2f, pGameCounter[0].nCntData, 5, 500,
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },0,62,{ 1,30000 },{ 8, 8 }, 5,0,5,0,0.0f, 0.0f,0.03f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },0,61,{ 1,30000 },{ 8, 8 }, 5,0,5,0,0.0f, 0.0f,-0.03f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_BACK, BULLETAIM_NORMAL,{ 0,0 },0,36,{ 250,1 },{ 25,25 }, 16,0,1,0,0.01f, 0.0f,0.03f, -0.009f, -0.5f, 50.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
		pGameCounter[0].nCntData++;
	}//初期位置, 敵の種類, 向き(角度), 方向転換速度と方向( + or - ), 移動速度, 攻撃パターン, 攻撃速度, 攻撃タイミング, 労働時間, 休憩時間, 画像, HP
	if (pGameCounter[0].nCntData == 2400)
	{//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
		SetEnemy(D3DXVECTOR3(GAME_WIDE * 0.5f, 0, 0.0f), ENEMYTYPE_BIG, ENEMYMODE_LBOSS_LUNATIC_3, 0.0f, 0.0f, 0.4f, pGameCounter[0].nCntData, 9, 600,
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },6,61,{ 30,0 },{ 8, 8 }, 8,0,1,0,0.001f, 0.0f,0.0f, 0.0f, 0.3f, 30.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },6,59,{ 30,0 },{ 8, 8 }, 8,0,1,0,0.001f, D3DX_PI * 0.1f,0.0f, 0.0f, 0.3f, 30.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },6,59,{ 30,0 },{ 8, 8 }, 8,0,1,0,0.001f, D3DX_PI * -0.1f,0.0f, 0.0f, 0.3f, 30.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_ACCELERATOR, BULLETAIM_STOP,{ 0,0 },0,35,{ 100,50 },{ 15,15 }, 3,0,3,75,0.005f, 0.0f,0.1f, 0.0f, 0.1f, 0.0f, 0.0f, 0.08f },
			ATTACK{ BULLETSTATE_NOT });
		pGameCounter[0].nCntData++;
	}
}
//=============================================================================
// 難易度のセット
//=============================================================================
void SetDifficulty(Difficulty diffculty)
{
	Difficulty *pDifficulty;

	pDifficulty = GetDifficulty();

	pDifficulty[0] = diffculty;
}
//=============================================================================
// 難易度の取得
//=============================================================================
Difficulty *GetDifficulty(void)
{
	static Difficulty difficulty;

	return &difficulty;
}
//=============================================================================
// カウントの初期化
//=============================================================================
void SetGamecounter(GameCounter gamecounter)
{
	GameCounter *pGamecounter;

	pGamecounter = GetGamecounter();

	pGamecounter[0] = gamecounter;
}
//=============================================================================
// カウントの取得
//=============================================================================
GameCounter *GetGamecounter(void)
{
	static GameCounter gamecounter;

	return &gamecounter;
}
//=============================================================================
// カウントの初期化
//=============================================================================
void SetFieldBGM(void)
{
	StopSound();
	int nRnd = rand() % 4;

	if (nRnd == 0)
	{
		PlaySound(SOUND_LABEL_FBGM000);
	}
	else if (nRnd == 1)
	{
		PlaySound(SOUND_LABEL_FBGM001);
	}
	else if (nRnd == 2)
	{
		PlaySound(SOUND_LABEL_FBGM002);
	}
	else if (nRnd == 3)
	{
		PlaySound(SOUND_LABEL_FBGM003);
	}
}