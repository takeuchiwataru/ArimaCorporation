//=============================================================================
//
// 敵処理 [enemy.h]
// Author :
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_ENEMY	(32)	// 敵の最大数

//*********************************************************************************************************//
// マクロ定義
//*********************************************************************************************************//
#define GAME_WIDE			(800)		//ゲーム画面（メイン）の幅
#define ENEMY_HP1			(5)			//雑魚敵のHP
#define ENEMY_HP2			(50)		//中ボスのHP
#define ENEMY_HP3			(1000)		//ボスのHP 
#define ENEMY_ATTACK		(30)		//雑魚敵の攻撃SPEED
#define ENEMY_ATTACK1		(8)			//雑魚敵の攻撃SPEED
#define ENEMY_ATTACK2		(2)			//雑魚敵の攻撃SPEED
#define ENEMY_ATKTIME0		(0)			//雑魚敵は休み時間なし
#define ENEMY_ATKTIME1		(135)		//休み時間までの攻撃時間
#define ENEMY_ATKTIME2		(150)		//休み時間までの攻撃時間
#define ENEMY_BREAKTIME0	(0)			//雑魚敵は休み時間なし
#define ENEMY_BREAKTIME1	(90)		//敵の休憩時間
#define ENEMY_BREAKTIME2	(90)		//敵の休憩時間
#define SHOTCOL_STAGE1		(8)			//敵の弾の色　ステージ1

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	ENEMYSTATE_NORMAL = 0,	//敵の通常状態
	ENEMYSTATE_DAMAGE,		//敵のダメージ状態
	ENEMYSTATE_TIME,		//無敵状態 && 攻撃しない
	ENEMYSTATE_TIME1,		//無敵状態 && 攻撃しない
	ENEMYSTATE_Title,
	ENEMYSTATE_MAX			//敵の状態の合計

}ENEMYSTATE;

typedef enum
{
	ENEMYMOVE_NORMAL = 0,	//何も加えない 雑魚的
	ENEMYMOVE_NONE,			//動かない
	ENEMYMOVE_RETURN,		//真ん中に戻る 攻撃はする
	ENEMYMOVE_MOVE,			//ボスの移動
	ENEMYMOVE_STOP,			//ボスの一時的ストップ
	ENEMYMOVE_MAX			//敵の状態の合計

}ENEMYMOVE;

typedef enum
{
	BULLETAIM_NORMAL = 0,	//弾の通常状態
	BULLETAIM_TIME,			//弾の休憩状態
	BULLETAIM_STOP,			//タイミング調整のため初めだけずらす
	BULLETAIM_MAX			//弾の状態の合計

}BULLETAIM;

typedef enum
{
	ENEMYTYPE_NORMAL = 0,	//雑魚敵
	ENEMYTYPE_SMALL,		//小ボス
	ENEMYTYPE_MEDIUM,		//中ボス
	ENEMYTYPE_BIG,			//ボス
	ENEMYTYPE_MAX,			//敵種類の合計
	ENEMYTYPE_TITLE2 = 6,
	ENEMYTYPE_TITLE = 23

}ENEMYTYPE;

typedef enum
{
	ENEMYMODE_NONE = 0,			//敵の通常状態　変化なし
	ENEMYMODE_NONE_1,			//ボス時の取り巻き
	ENEMYMODE_MBOSS_EASY,		//中ボス EASY
	ENEMYMODE_MBOSS_NORMAL,		//中ボス NORMAL
	ENEMYMODE_MBOSS_HARD,		//中ボス HARD
	ENEMYMODE_MBOSS_LUNATIC,	//中ボス Lunatic
	ENEMYMODE_LBOSS_EASY_1,		//ボス EASY1
	ENEMYMODE_LBOSS_NORMAL_1,	//ボス NORMAL1
	ENEMYMODE_LBOSS_HARD_1,		//ボス HARD1
	ENEMYMODE_LBOSS_LUNATIC_1,	//ボス Lunatic1
	ENEMYMODE_LBOSS_EASY_2,		//ボス EASY2
	ENEMYMODE_LBOSS_NORMAL_2,	//ボス NORMAL2
	ENEMYMODE_LBOSS_HARD_2,		//ボス HARD2
	ENEMYMODE_LBOSS_LUNATIC_2,	//ボス Lunatic2
	ENEMYMODE_LBOSS_EASY_3,		//ボス EASY2
	ENEMYMODE_LBOSS_NORMAL_3,	//ボス NORMAL2
	ENEMYMODE_LBOSS_HARD_3,		//ボス HARD2
	ENEMYMODE_LBOSS_LUNATIC_3,	//ボス Lunatic2
	ENEMYMODE_MAX				//敵の状態の合計

}ENEMYMODE;

typedef struct
{
	BULLETSTATE state;	//弾の状態
	BULLETAIM aim;		//弾の発射状態
	int nCntState[2];	//停止時間等保存
	int nTypeAttcak;	//攻撃の種類
	int nCol;			//弾の色
	int nAttackTime[2];	//0：休憩時間, 1：労働時間
	int nLength[2];		//弾の長さXY
	int nBulletNum;		//弾の量
	int nCntTime;		//時間カウント
	int nRecast;		//攻撃のリキャストタイム
	int nAtkPlus;		//タイミングずらし用　同じ攻撃を選んだ場合の
	float fSpdPlus;		//加速値
	float fAngle;		//発射する角度
	float fAnglePlus;	//角度のplus
	float fCurve;		//弾にplusする角度
	float fSpeed;		//弾のスピード
	float fPos;			//発射場所から離れる距離
	float fSetAngle[2];	//セットするための角度

}ATTACK;

typedef struct
{
	ENEMYSTATE state;	//エネミーの状態
	ENEMYMOVE movestate;//エネミーの状態 変更用
	ENEMYMODE mode;		//敵の変化状態
	ENEMYTYPE type;		//敵の種類	0:小　1:中　2:大
	ATTACK attack[5];	//装備の種類
	int nCounterState;	//エネミーの状態管理用
	int nCounterMove;	//エネミーの移動管理用
	D3DXVECTOR3 pos;	//初期位置
	D3DXVECTOR3 move;	//移動量保存(慣性のため)
	float fsteep;		//向き角度
	float fChannel;		//方向転換の量, 方向( + or - )
	float fSpeed;		//移動SPEED
	float fAngle;		//Animation用の傾き
	int nAttack;		//攻撃パターン
	int nReplus;		//攻撃タイミング用
	int nTex;			//画像指定
	int nHP;			//エネミーのHP
	int nCollar;		//エネミーの被ダメ時の色の変化用
	int nLength;		//敵の半分の長さ
	bool bAngle;		//角度+-の保存
	bool bUse;			//使用 or 未使用

}ENEMY;

typedef struct
{
	int nEnemyCounter;			//ボス含めカウント
	bool g_bBossKill;			//ボスの撃破判定のため

}IfBoss;	//ボスの出現条件

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, ENEMYTYPE nLarge, ENEMYMODE mode, float steep, float fChannel, float fSpeed, int nReplus, int nType, int nHP, ATTACK attcak0, ATTACK attcak1, ATTACK attcak2, ATTACK attcak3, ATTACK attcak4);
void SetAttack(int nAtk, int nNumBullet, int nLengthX, int nLengthY, int nCol, int nCntEnemy, int nEnemyAtk, float fSteep, float fSpeed, float fSpdPlus, float fCurve, D3DXVECTOR3 pos, float fPos, BULLETSTATE state);
void BreakEnemy(void);
void DamageBoss(int nCntEnemy, int nDamage);
void DamageBossTitle(int nDamage);
void AttackEnemy(int nCntEnemy, int nCntRecast, float fChannel360, float fChannel90, float fPlayerAngle);
void HitEnemy(int nCntEnemmy, int nDamageEnemy, int nAtkType);
void BossChange(int nCntEnemy);
void DeadEnemy(void);
IfBoss *GetBoss(void);
ENEMY *GetEnemy(void);

#endif
