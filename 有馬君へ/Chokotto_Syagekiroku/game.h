#pragma once
//=========================================================================================================//
//
// ポリゴン：プロトタイプ宣言 [Scan.h]
// Author : Ryo Sugimoto
//
//=========================================================================================================//
#ifndef _POLYGON_H_
#define _POLYGON_H_

#include"main.h"

//*********************************************************************************************************//
// マクロ定義
//*********************************************************************************************************//
#define PLAYER_POSX			(SCREEN_GAMEX / 2 - 22)					//画像のX
#define PLAYER_POSY			(SCREEN_HEIGHT / 4 * 3)					//画像のY
#define PLAYER_POSX2			(SCREEN_GAMEX / 2 + 22)					//画像のX2
#define PLAYER_POSY2			(SCREEN_HEIGHT / 4 * 3 + 44)			//画像のY2
#define PLAYER_POSX3			((PLAYER_POSX2 - PLAYER_POSX) / 2)	//画像のXの幅 / 2
#define PLAYER_POSY3			((PLAYER_POSY2 - PLAYER_POSY) / 2)	//画像のYの幅 / 2
#define POSPLAYERX				((PLAYER_POSX + PLAYER_POSX2) / 2)	//Xの中心
#define POSPLAYERY				((PLAYER_POSY + PLAYER_POSY2) / 2)	//Yの中心

#define BULLET_ATKTYPE0			(0)										//通常弾
#define BULLET_ATKTYPE1			(1)										//光線
#define BULLET_ATKTYPE50		(50)									//装飾
#define BULLET_ATKTYPE99		(99)									//ナイフ！
#define KNIFE_TIME				(BULLET_ATKTYPE99 + 6)					//ナイフの有効時間

#define HALF_EXPLOSION			(30)									//爆発の幅高さ
#define GUARD_BREAK				(481)			//GUARD発動までの時間

#define MAX_SCORE		(5 * 4)		//5位*難易度分
#define RANK_SCORE		(5)			//ランキングに使われているスコア

#define	MAX_BULLET			(3840)				// 弾の最大数
#define BULLET_SLIP			(0.15f)				//弾の滑り
#define BULLET_DELETE		(200)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	GAMESTATE_NONE = 0,
	GAMESTATE_NORMAL,
	GAMESTATE_END,
	GAMESTATE_PAUSE

}GAMESTATE;

typedef enum
{
	BULLETTYPE_PLAYER = 0,
	BULLETTYPE_ENEMY,
	BULLETTYPE_MAX
}BULLETTYPE;

typedef struct
{
	GAMESTATE now;		//現在の状態
	MODE CHANGE;		//変えるモード
	int nCntGameState;	//カウント用

}GAME;

typedef struct
{
	D3DXVECTOR3 pos;	//ポリゴンの位置
	D3DXVECTOR3 move;	//ポリゴンの移動量
	int nStealth;		//復活時の無敵時間に使用
	int nMode;			//撃ち方変更用　ローカルでやると縛り時戻せなくなる
	int nGuardTime;		//ガードのリキャストタイム
	bool bGuard;		//Guard
	bool bCharge;		//GUARDのCharge状態
	bool bKnifeLimit;	//ナイフ縛り用
	bool bUse;			//生存 or 死亡

}PLAYER;

typedef enum
{
	BULLETSTATE_NONE = 0,		//何もなし
	BULLETSTATE_NOT,			//使用していない
	BULLETSTATE_START,			//開始のみ加速
	BULLETSTATE_ACCELERATOR,	//加速し続ける
	BULLETSTATE_STOP_S,			//開始止まる int変数により解除 STARTへ
	BULLETSTATE_STOP_A,			//開始止まる int変数により解除 ACCELERATORへ
	BULLETSTATE_STOP_B,			//開始止まる int変数により解除 BACKへ
	BULLETSTATE_BACK,			//後ろへ進む 丸形の弾でしか使うなよ！
	BULLETSTATE_STRAIGHT,		//カーブの値を下げていく
	BULLETSTATE_STRAIGHT_B,		//カーブの値を下げていく BACKも
	BULLETSTATE_PAUSE,			//途中停止 int変数により開始+解除
	BULLETSTATE_DELETE,			//消えていく
	BULLETSTATE_FLASH,			//攻撃時に光る
	BULLETSTATE_SET_A,			//小さい球を置きながら進む 置かれた球 = STOP
	BULLETSTATE_SET_B,			//小さい球を置きながら進む 置かれた球 = STOP
	BULLETSTATE_SET_S,			//小さい球を置きながら進む 置かれた球 = STOP
	BULLETSTATE_SET_A1,			//小さい球を置きながら進む 置かれた球 = STOP
	BULLETSTATE_SET_B1,			//小さい球を置きながら進む 置かれた球 = STOP
	BULLETSTATE_SET_S1,			//小さい球を置きながら進む 置かれた球 = STOP
	BULLETSTATE_SET_B2,			//小さい球を置きながら進む 置かれた球 = STOP

}BULLETSTATE;
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動
	BULLETSTATE state;	//弾の状態
	int nCntState[3];	//弾の状態用 STOP等の時間も保存
	int nLengthX;		//幅X
	int nLengthY;		//幅Y
	int nType;			//画像の種類
	int nAtkType;		//攻撃方法	0:通常弾 1:光線 99:ナイフ
	int nShot;			//誰が撃ったか
	int nCntEnemy;		//敵だってナイフ振りたいんです！
	float steep;		//傾き
	float fSetSteep[2];	//セット時の角度
	float fCurve;		//カーブ
	float fSpeed;		//弾のスピード
	float fSpdPlus;		//スピードの加速値
	float fcolA;		//透明度
	bool bUse;			//使用or不使用
}Bullet;

typedef struct
{//難易度の設定
	int nDifficulty;	//難易度
	int nNumber;		//難易度ごとのナンバー

}Difficulty;

typedef struct
{//ゲームの敵出現等のカウント
	int nCntGameOver[2];	//ゲームオーバー[2]
	int nCntData;			//敵出現
	int nCntClear;			//Clearカウント
	int nEnd;				//死亡カウント

}GameCounter;

typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 move;		// 移動値の残り火
	D3DXCOLOR col;			// 色
	float fSteep;			//角度
	float fSpeed;
	float fRadius;			// 半径(大きさ)
	bool bUse;				// 使用しているかどうか

} EFFECT;

//*********************************************************************************************************//
// プロトタイプ宣言　Game
//*********************************************************************************************************//
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
GAME *GetGameState(void);
void SetGameState(GAMESTATE game, MODE mode, int nCntGameState);

//*********************************************************************************************************//
// プロトタイプ宣言　Chapter
//*********************************************************************************************************//
void ChapterEnemy(void);
void EASYMode(void);
void NORMALMode(void);
void HARDMode(void);
void LUNATICMode(void);
void SetDifficulty(Difficulty diffculty);
Difficulty *GetDifficulty(void);
void SetGamecounter(GameCounter gamecounter);
GameCounter *GetGamecounter(void);
void SetFieldBGM(void);

//*********************************************************************************************************//
// プロトタイプ宣言　ポリゴン
//*********************************************************************************************************//
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(void);

//*****************************************************************************
// プロトタイプ宣言　弾
//*****************************************************************************
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, float steep, int nLengthX, int nLengthY, int nType, int nAtkType, float fSpeed, BULLETTYPE nShot, int nCntEnemy, BULLETSTATE state, float fSpdPlus, float fCurve, int nCntstate0, int nCntstate1);
void StartBullet(int nCntEnemy, int nCntAtk);
void BreakBullet(void);
void BreakBullet(int nCntEnemy);
bool HitAim(Bullet pBullet, D3DXVECTOR3 aim, int nLength, float fPlusHit);
Bullet *GetBullet(void);

//*****************************************************************************
// プロトタイプ宣言　サブ　シューター
//*****************************************************************************
void InitShooter(void);
void UninitShooter(void);
void UpdateShooter(D3DXVECTOR3 pos, int nSet);
void DrawShooter(void);
void SetShooter(D3DXVECTOR3 move, int nType);
void BreakShooter(void);

//*********************************************************************************************************//
// プロトタイプ宣言　背景
//*********************************************************************************************************//
void InitBG(void);
void UninitBG(void);
void UpdateBG();
void DrawBG(int nBG);

//*****************************************************************************
// プロトタイプ宣言  スコア
//*****************************************************************************
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void AddScore(int nValue, int nNumber);
void SetScore(D3DXVECTOR3 pos, D3DXCOLOR col, int nScore, int nNumber, SCORESTATE state);
void BreakScore(void);
SCORE *GetGameScore(void);
void ChangeScore(int nNumber, SCORESTATE state);

//*****************************************************************************
// プロトタイプ宣言  window
//*****************************************************************************
void InitGameWindow(void);
void UninitGameWindow(void);
void UpdateGameWindow(void);
void DrawGameWindow(void);
void SetGameWindow(D3DXVECTOR3 pos, D3DXCOLOR col, int nLengthX, int nLengthY, int nType, WINDOWSTATE state, WINDOWUSE use);
void BreakGameWindow(void);
void BackGameWindow(int nNumber);
void ChangeGameWindow(int nNumber, WINDOWSTATE state);

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);

void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSteep, float fSpeed, float fRadius);
void ExplosionEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nSRand, int nLRand, float fSRand0, float fSRand1, float fLRand0, float fLRand1);
void SetBulletEffect(Bullet bullet);
EFFECT *GetEffect(void);
Bullet *GetBulletEffect(void);

#endif