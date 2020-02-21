//=============================================================================
//
// 敵処理 [enemy.cpp]
// Author :Ryo Sugimoto
//
//=============================================================================
#include "enemy.h"
#include "game.h"
#include "sound.h"
#include "fade.h"
#include <stdlib.h>				//ランド用
#include <time.h>				//同じくランド用

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMY_PRIMITIVE_NUM		(2)					// プリミティブの数
#define	MAX_TYPE_ENEMY		(4)					// 敵の種類
#define HALF_ENEMYX			(20)
#define HALF_ENEMYY			(20)
#define HALF_ENEMYX1		(15)
#define HALF_ENEMYY1		(15)

#define HIT_ENEMYX			(22)				//敵本体とのあたり判定X
#define HIT_ENEMYY			(22)				//敵本体とのあたり判定Y
#define ENEMY_Texture_Name0		"data/TEXTURE/migawari001.png"				//TEXTUREの名前
#define ENEMY_Texture_Name1		"data/TEXTURE/BOSS003.png"					//BOSS
#define CATCH_ENEMY			(20)				//敵の捕獲率％
#define ENEMY_MAX_Texture	(2)					//画像の種類
#define MAX_ATTCAK			(5)					//攻撃の種類

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureEnemy[2] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;		// 頂点バッファへのポインタ
int g_nCntSound = 0;
int nTimeSE[3] = { 0,0,0 };
//=============================================================================
// 初期化処理
//=============================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEnemy;

	ENEMY *g_aEnemy;
	g_aEnemy = GetEnemy();

	IfBoss *ifboss;
	ifboss = GetBoss();
	ifboss[0].g_bBossKill = false;
	ifboss[0].nEnemyCounter = 1;

	// デバイスの取得
	pDevice = GetDevice();

	// 敵の情報の初期化
	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
		//g_aEnemy[nCntEnemy].nCounterState = 0;
		//g_aEnemy[nCntEnemy].pos = { 0.0f, 0.0f, 0.0f };
		//g_aEnemy[nCntEnemy].move = { 0.0f, 0.0f, 0.0f };
		//g_aEnemy[nCntEnemy].fsteep = 0.0f;
		//g_aEnemy[nCntEnemy].fChannel = 0.0f;
		//g_aEnemy[nCntEnemy].fSpeed = 0.0f;
		//g_aEnemy[nCntEnemy].nAttack = 0;
		//g_aEnemy[nCntEnemy].nRecast = 0;
		//g_aEnemy[nCntEnemy].nTex = 0;
		//g_aEnemy[nCntEnemy].nHP = 0;
		//g_aEnemy[nCntEnemy].nCollar = 255;
		//g_aEnemy[nCntEnemy].fAngle = 3.14f;				//角度保存
		//g_aEnemy[nCntEnemy].bAngle = true;				//角度の+-の保存
		g_aEnemy[nCntEnemy].bUse = false;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, ENEMY_Texture_Name0, &g_pTextureEnemy[0]);
	D3DXCreateTextureFromFile(pDevice, ENEMY_Texture_Name1, &g_pTextureEnemy[1]);

	// 頂点情報の作成
	VERTEX_2D *pVtx;

	// 頂点バッファを生成
    pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
									D3DUSAGE_WRITEONLY,
									FVF_VERTEX_2D,
									D3DPOOL_MANAGED,
									&g_pVtxBuffEnemy,
									NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for(nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//頂点座標の設定
		//pVtx[nCntEnemy * 4].pos = D3DXVECTOR3(0, 0, 0.0f);
		//pVtx[nCntEnemy * 4 + 1].pos = D3DXVECTOR3(0, 0, 0.0f);
		//pVtx[nCntEnemy * 4 + 2].pos = D3DXVECTOR3(0, 0, 0.0f);
		//pVtx[nCntEnemy * 4 + 3].pos = D3DXVECTOR3(0, 0, 0.0f);

		pVtx[nCntEnemy * 4].rhw = 1.0f;
		pVtx[nCntEnemy * 4 + 1].rhw = 1.0f;
		pVtx[nCntEnemy * 4 + 2].rhw = 1.0f;
		pVtx[nCntEnemy * 4 + 3].rhw = 1.0f;

		//カラー設定
		pVtx[nCntEnemy * 4].col = D3DCOLOR_RGBA(255, g_aEnemy[nCntEnemy].nCollar, g_aEnemy[nCntEnemy].nCollar, 255);	// A = 透明度
		pVtx[nCntEnemy * 4 + 1].col = D3DCOLOR_RGBA(255, g_aEnemy[nCntEnemy].nCollar, g_aEnemy[nCntEnemy].nCollar, 255);
		pVtx[nCntEnemy * 4 + 2].col = D3DCOLOR_RGBA(255, g_aEnemy[nCntEnemy].nCollar, g_aEnemy[nCntEnemy].nCollar, 255);
		pVtx[nCntEnemy * 4 + 3].col = D3DCOLOR_RGBA(255, g_aEnemy[nCntEnemy].nCollar, g_aEnemy[nCntEnemy].nCollar, 255);

		//テクスチャ座標設定
		//pVtx[nCntEnemy * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
		//pVtx[nCntEnemy * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		//pVtx[nCntEnemy * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		//pVtx[nCntEnemy * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	int nCntEnemy;

	// テクスチャの開放
	for (nCntEnemy = 0; nCntEnemy < ENEMY_MAX_Texture; nCntEnemy++)
	{
		if (g_pTextureEnemy[nCntEnemy] != NULL)
		{
			g_pTextureEnemy[nCntEnemy]->Release();
			g_pTextureEnemy[nCntEnemy] = NULL;
		}
	}

	// 頂点バッファの開放
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	static float fChannel360[2] = { 0.0f, 0.0f };
	static float fChannel90;
	static bool bChannel90;
	static int nCntRecast = 0;	//攻撃速度(連射)の調整のため
	int nRand[2];
	float fPlusAngle;			//敵のAnimation用
	float fPlayerAngle;			//プレイヤーへの角度計算のため
	g_nCntSound++;
	nTimeSE[0]++;
	nTimeSE[1]++;
	nTimeSE[2]++;

	ENEMY *g_aEnemy;
	g_aEnemy = GetEnemy();

	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	IfBoss *ifboss = GetBoss();;
	ifboss = GetBoss();

	MODE *pMode;
	pMode = GetMode();

	nCntRecast++;
	fChannel360[0] += D3DX_PI * 0.01f;
	fChannel360[1] -= D3DX_PI * 0.01f;

	int nCntEnemy;
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	if (bChannel90 == true)
	{//マイナス！ 角度
		fChannel90 -= 0.03f;
	}
	else
	{//プラス！　角度
		fChannel90 += 0.03f;
	}

	if (fChannel90 >= D3DX_PI / 4.0f)
	{//範囲（幅)
		fChannel90 = D3DX_PI / 4.0f;
		bChannel90 = true;
	}
	else if (fChannel90 <= -D3DX_PI / 4.0f)
	{//範囲（幅)
		fChannel90 = -D3DX_PI / 4.0f;
		bChannel90 = false;
	}//弾発射

	//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{

		if (g_aEnemy[nCntEnemy].bUse == true)
		{//敵の行動
		 //MessageBox(0, "本当すか？", "終了", MB_YESNO);
			if (g_aEnemy[nCntEnemy].fAngle <= D3DX_PI * 0.85f || g_aEnemy[nCntEnemy].fAngle >= D3DX_PI * 1.15f)
			{//角度切り替え時被ダメ
				//g_aEnemy[nCntEnemy].bAngle = g_aEnemy[nCntEnemy].bAngle ? true : false;
				if (g_aEnemy[nCntEnemy].fAngle <= D3DX_PI * 0.85f) { g_aEnemy[nCntEnemy].bAngle = true; }
				if (g_aEnemy[nCntEnemy].fAngle >= D3DX_PI * 1.15f) { g_aEnemy[nCntEnemy].bAngle = false; }
				//MessageBox(0, "", "", MB_OK);
				
				if (g_aEnemy[nCntEnemy].type > ENEMYTYPE_NORMAL && g_aEnemy[nCntEnemy].type != ENEMYTYPE_TITLE)
				{//雑魚敵・タイトル以外はダメージ
					DamageBoss(nCntEnemy, 1);
				}
			}
			fPlusAngle = 0.03f;
			if (g_aEnemy[nCntEnemy].bAngle == false) 
			{ fPlusAngle *= -1; }
			g_aEnemy[nCntEnemy].fAngle += fPlusAngle;
			g_aEnemy[nCntEnemy].fsteep += g_aEnemy[nCntEnemy].fChannel;
			g_aEnemy[nCntEnemy].move += D3DXVECTOR3(sinf(g_aEnemy[nCntEnemy].fsteep), cosf(g_aEnemy[nCntEnemy].fsteep), 0.0f) * g_aEnemy[nCntEnemy].fSpeed;
			g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;
			pVtx[nCntEnemy * 4].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (sinf(g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].nLength) + (cosf(g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].nLength), g_aEnemy[nCntEnemy].pos.y - (sinf(g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].nLength) + (cosf(g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].nLength), 0.0f);
			pVtx[nCntEnemy * 4 + 1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (sinf(g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].nLength) - (cosf(g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].nLength), g_aEnemy[nCntEnemy].pos.y + (sinf(g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].nLength) + (cosf(g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].nLength), 0.0f);
			pVtx[nCntEnemy * 4 + 2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (sinf(g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].nLength) + (cosf(g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].nLength), g_aEnemy[nCntEnemy].pos.y - (sinf(g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].nLength) - (cosf(g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].nLength), 0.0f);
			pVtx[nCntEnemy * 4 + 3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (sinf(g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].nLength) - (cosf(g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].nLength), g_aEnemy[nCntEnemy].pos.y + (sinf(g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].nLength) - (cosf(g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].nLength), 0.0f);

			g_aEnemy[nCntEnemy].move.x += (0.0f - g_aEnemy[nCntEnemy].move.x) * 0.1f;
			g_aEnemy[nCntEnemy].move.y += (0.0f - g_aEnemy[nCntEnemy].move.y) * 0.1f;

			if (g_aEnemy[nCntEnemy].pos.y + HALF_ENEMYX * 3 <= 0 || g_aEnemy[nCntEnemy].pos.y - HALF_ENEMYX * 3 >= SCREEN_HEIGHT || g_aEnemy[nCntEnemy].pos.x + HALF_ENEMYY * 3 <= 0 || g_aEnemy[nCntEnemy].pos.x - HALF_ENEMYY * 3 >= GAME_WIDE)
			{//画面外の敵を消す
				if (pMode[0] != MODE_TITLE)
				{
					g_aEnemy[nCntEnemy].bUse = false;
					ifboss[0].nEnemyCounter--;
				}
			}

			pPlayer = GetPlayer();
			if (pPlayer[0].bUse == true && g_aEnemy[nCntEnemy].type < ENEMYTYPE_MEDIUM)
			{//小ボスと雑魚敵なら本体に当たる
				if (g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].nLength < pPlayer[0].pos.x && g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].nLength > pPlayer[0].pos.x && g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].nLength < pPlayer[0].pos.y + PLAYER_POSY3 && g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].nLength > pPlayer[0].pos.y + PLAYER_POSY3 && pPlayer[0].nStealth > 254)
				{//敵本体に当たった
					SetExplosion(D3DXVECTOR3{ pPlayer[0].pos.x, pPlayer[0].pos.y + PLAYER_POSY3, 0.0f }, COLLAR{ 255,252,252 }, HALF_EXPLOSION * 10, HALF_EXPLOSION * 10, 5, 4, 2, 0, ANIMATION_NORMAL);

					ExplosionEffect(D3DXVECTOR3{ pPlayer[0].pos.x, pPlayer[0].pos.y + PLAYER_POSY3, 0.0f }, pPlayer[0].move, D3DXCOLOR{ 1.0f, 0.98f, 0.04f, 0.8f }, 50, 100, 0.1f, 8.0f, 0.3f, 100.0f);
					ExplosionEffect(D3DXVECTOR3{ pPlayer[0].pos.x, pPlayer[0].pos.y + PLAYER_POSY3, 0.0f }, pPlayer[0].move, D3DXCOLOR{ 1.0f, 0.98f, 0.04f, 0.75f }, 100, 100, 0.5f, 13.0f, 0.1f, 3.0f);
					ExplosionEffect(D3DXVECTOR3{ pPlayer[0].pos.x, pPlayer[0].pos.y + PLAYER_POSY3, 0.0f }, pPlayer[0].move, D3DXCOLOR{ 0.49f, 1.0f, 0.97f, 0.75f }, 100, 100, 0.5f, 13.0f, 0.1f, 3.0f);
					ExplosionEffect(D3DXVECTOR3{ pPlayer[0].pos.x, pPlayer[0].pos.y + PLAYER_POSY3, 0.0f }, pPlayer[0].move, D3DXCOLOR{ 1.0f, 0.05f, 0.05f, 0.75f }, 100, 100, 0.5f, 13.0f, 0.1f, 3.0f);
					ExplosionEffect(D3DXVECTOR3{ pPlayer[0].pos.x, pPlayer[0].pos.y + PLAYER_POSY3, 0.0f }, pPlayer[0].move, D3DXCOLOR{ 1.0f, 0.05f, 0.05f, 0.75f }, 100, 100, 0.5f, 13.0f, 0.1f, 3.0f);
					ExplosionEffect(D3DXVECTOR3{ pPlayer[0].pos.x, pPlayer[0].pos.y + PLAYER_POSY3, 0.0f }, pPlayer[0].move, D3DXCOLOR{ 1.0f, 0.05f, 0.05f, 0.75f }, 100, 100, 0.5f, 13.0f, 0.1f, 3.0f);

					pPlayer[0].bUse = false;	//自機の破壊
					BreakShooter();				//サブシューターの破壊
					ChangeGuard(2);
					AddScore(-10000, 0);
					PlaySound(SOUND_LABEL_BOMB001);
				}
			}

			//カラー設定
			pVtx[nCntEnemy * 4].col = D3DCOLOR_RGBA(255, g_aEnemy[nCntEnemy].nCollar, g_aEnemy[nCntEnemy].nCollar, 255);	// A = 透明度
			pVtx[nCntEnemy * 4 + 1].col = D3DCOLOR_RGBA(255, g_aEnemy[nCntEnemy].nCollar, g_aEnemy[nCntEnemy].nCollar, 255);
			pVtx[nCntEnemy * 4 + 2].col = D3DCOLOR_RGBA(255, g_aEnemy[nCntEnemy].nCollar, g_aEnemy[nCntEnemy].nCollar, 255);
			pVtx[nCntEnemy * 4 + 3].col = D3DCOLOR_RGBA(255, g_aEnemy[nCntEnemy].nCollar, g_aEnemy[nCntEnemy].nCollar, 255);

			switch (g_aEnemy[nCntEnemy].movestate)
			{
			case ENEMYMOVE_NORMAL:	//何も加えない
				break;
			case ENEMYMOVE_NONE:	//動かない
				g_aEnemy[nCntEnemy].fSpeed = 0.0f;
				break;
			case ENEMYMOVE_RETURN:	//中心に角度を代入
				g_aEnemy[nCntEnemy].fsteep = atan2f(GAME_WIDE / 2 - g_aEnemy[nCntEnemy].pos.x, SCREEN_HEIGHT / 4 - g_aEnemy[nCntEnemy].pos.y);
				break;
			case ENEMYMOVE_STOP:	//一定時間止まる
				g_aEnemy[nCntEnemy].nCounterMove++;
				if (g_aEnemy[nCntEnemy].nCounterMove > 60)
				{//動くようになる
					nRand[0] = rand() % 200;
					nRand[1] = rand() % 150;
					if (g_aEnemy[nCntEnemy].pos.x > GAME_WIDE / 4 * 3)		{ nRand[0] += 110; }	//右よりだったら左に
					else if (g_aEnemy[nCntEnemy].pos.x < GAME_WIDE / 4)		{ nRand[0] -= 110; }	//左よりだったら右に
					if (g_aEnemy[nCntEnemy].pos.y > SCREEN_HEIGHT / 3)		{ nRand[1] += 85; }		//下よりだったら上に
					else if (g_aEnemy[nCntEnemy].pos.y < SCREEN_HEIGHT / 5)	{ nRand[1] -= 85; }		//上よりだったら下に
					g_aEnemy[nCntEnemy].fsteep = atan2f( 100 - nRand[0] * 1.0f, 75 - nRand[1] * 1.0f);
					g_aEnemy[nCntEnemy].nCounterMove = 0;
					g_aEnemy[nCntEnemy].fSpeed = 0.2f;
					g_aEnemy[nCntEnemy].movestate = ENEMYMOVE_MOVE;
				}
				break;
			case ENEMYMOVE_MOVE:	//適当に動く（BOSS）
				g_aEnemy[nCntEnemy].nCounterMove++;
				if (g_aEnemy[nCntEnemy].nCounterMove > 50)
				{//動かなくなる
					g_aEnemy[nCntEnemy].nCounterMove = 0;
					g_aEnemy[nCntEnemy].fSpeed = 0.0f;
					g_aEnemy[nCntEnemy].movestate = ENEMYMOVE_STOP;
				}
				break;
			}

			switch (g_aEnemy[nCntEnemy].state)
			{

			case ENEMYSTATE_DAMAGE:
				g_aEnemy[nCntEnemy].nCollar += 15;					//ダメージ状態の時間
				if (g_aEnemy[nCntEnemy].nCollar > 254)
				{
					g_aEnemy[nCntEnemy].nCollar = 255;				//すぎた場合のために色戻す
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;	//通常状態に戻す
				}
			case ENEMYSTATE_NORMAL:
				fPlayerAngle = atan2f(pPlayer[0].pos.x - g_aEnemy[nCntEnemy].pos.x, pPlayer[0].pos.y + PLAYER_POSY3 - g_aEnemy[nCntEnemy].pos.y);
				AttackEnemy(nCntEnemy, nCntRecast,fChannel360[0], fChannel90, fPlayerAngle);
				break;
			case ENEMYSTATE_TIME://無敵&&攻撃しない
				g_aEnemy[nCntEnemy].nCounterState--;
				if (g_aEnemy[nCntEnemy].nCounterState == 0)
				{//しばらくたったら
					g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
					PlaySound(SOUND_LABEL_WIND);
				}//中心に戻る
				if(g_aEnemy[nCntEnemy].pos.x - GAME_WIDE / 2 < -3 || g_aEnemy[nCntEnemy].pos.x - GAME_WIDE / 2 + 1 > 3 || g_aEnemy[nCntEnemy].pos.y - SCREEN_HEIGHT / 4 < -3 || g_aEnemy[nCntEnemy].pos.y - SCREEN_HEIGHT / 4 > 3)
				{ g_aEnemy[nCntEnemy].fSpeed = 0.4f; }		//真ん中でなければ移動
				else
				{
					g_aEnemy[nCntEnemy].fSpeed = 0.0f;
					g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}	//真ん中なら止まる
				g_aEnemy[nCntEnemy].fsteep = atan2f(GAME_WIDE / 2 - g_aEnemy[nCntEnemy].pos.x, SCREEN_HEIGHT / 4 - g_aEnemy[nCntEnemy].pos.y);
				break;
			case ENEMYSTATE_TIME1://無敵&&攻撃しない
				g_aEnemy[nCntEnemy].nCounterState--;
				if (g_aEnemy[nCntEnemy].nCounterState == 0)
				{//しばらくたったら
					g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
					PlaySound(SOUND_LABEL_WIND);
				}//中心に戻る
				if (g_aEnemy[nCntEnemy].pos.x - GAME_WIDE / 2 < -3 || g_aEnemy[nCntEnemy].pos.x - GAME_WIDE / 2 + 1 > 3 || g_aEnemy[nCntEnemy].pos.y - SCREEN_HEIGHT / 2 < -3 || g_aEnemy[nCntEnemy].pos.y - SCREEN_HEIGHT / 2 > 3)
				{
					g_aEnemy[nCntEnemy].fSpeed = 0.4f;
				}		//真ん中でなければ移動
				else
				{
					g_aEnemy[nCntEnemy].fSpeed = 0.0f;
					g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}	//真ん中なら止まる
				g_aEnemy[nCntEnemy].fsteep = atan2f(GAME_WIDE / 2 - g_aEnemy[nCntEnemy].pos.x, SCREEN_HEIGHT / 2 - g_aEnemy[nCntEnemy].pos.y);
				break;
			case ENEMYSTATE_Title://無敵&&攻撃しない
				g_aEnemy[nCntEnemy].nCounterState--;
				if (g_aEnemy[nCntEnemy].nCounterState == -43)
				{//しばらくたったら
					g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
					g_aEnemy[nCntEnemy].movestate = ENEMYMOVE_NONE;
					g_aEnemy[nCntEnemy].fSpeed = 0.0f;
				}//中心に戻る
				g_aEnemy[nCntEnemy].fSpeed = 0.4f;
				g_aEnemy[nCntEnemy].fsteep = 0.0f;
				break;
			}//switch括弧
		}//true括弧
	}//for括弧
	//頂点BUFFERをアンロックする
	g_pVtxBuffEnemy->Unlock();

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	ENEMY *g_aEnemy;
	g_aEnemy = GetEnemy();

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_NORMAL)
			{//雑魚のTEXTURE設定
				pDevice->SetTexture(0, g_pTextureEnemy[0]);
			}
			else
			{//ボスのTEXTURE設定
				pDevice->SetTexture(0, g_pTextureEnemy[1]);
			}
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, ENEMY_PRIMITIVE_NUM);
		}
	}
}

//=============================================================================
// * 敵の設置(設定)
//=============================================================================
void SetEnemy(D3DXVECTOR3 pos, ENEMYTYPE nLarge, ENEMYMODE mode, float fsteep, float fChannel, float fSpeed, int nReplus, int nTex, int nHP, ATTACK attcak0, ATTACK attcak1, ATTACK attcak2, ATTACK attcak3, ATTACK attcak4)
{
	int nCntEnemy;	//エネミーカウント用
	ENEMY *g_aEnemy;
	IfBoss *ifboss;

	g_aEnemy = GetEnemy();
	ifboss = GetBoss();

	// 頂点情報の作成
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for(nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{
			g_aEnemy[nCntEnemy].nCounterMove = 0;
			g_aEnemy[nCntEnemy].type = nLarge;				//敵の種類設定
			g_aEnemy[nCntEnemy].fsteep = fsteep;			//初期角度
			g_aEnemy[nCntEnemy].fChannel = fChannel;		//角度変更(旋回)用
			g_aEnemy[nCntEnemy].fSpeed = fSpeed;			//移動SPEED代入
			g_aEnemy[nCntEnemy].nReplus = nReplus;			//出現タイミング保存->攻撃時のタイミングずらしのため
			g_aEnemy[nCntEnemy].nHP = nHP;					//HP代入
			g_aEnemy[nCntEnemy].fAngle = 3.14f;				//角度保存
			g_aEnemy[nCntEnemy].mode = mode;
			g_aEnemy[nCntEnemy].bAngle = true;				//角度の+-の保存
			g_aEnemy[nCntEnemy].attack[0] = attcak0;
			g_aEnemy[nCntEnemy].attack[1] = attcak1;
			g_aEnemy[nCntEnemy].attack[2] = attcak2;
			g_aEnemy[nCntEnemy].attack[3] = attcak3;
			g_aEnemy[nCntEnemy].attack[4] = attcak4;
			g_aEnemy[nCntEnemy].bUse = true;				//使用中判定

			g_aEnemy[nCntEnemy].pos = pos;					//場所代入
			g_aEnemy[nCntEnemy].move = { 0.0f, 0.0f, 0.0f };//移動量初期化
			if (nLarge == ENEMYTYPE_NORMAL) { g_aEnemy[nCntEnemy].nLength = HALF_ENEMYX; }
			else { g_aEnemy[nCntEnemy].nLength = HALF_ENEMYX + (nLarge - 1) * HALF_ENEMYX1; }

			pVtx[nCntEnemy * 4].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (sinf(g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].nLength) + (cosf(g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].nLength), g_aEnemy[nCntEnemy].pos.y - (sinf(g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].nLength) + (cosf(g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].nLength), 0.0f);
			pVtx[nCntEnemy * 4 + 1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (sinf(g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].nLength) - (cosf(g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].nLength), g_aEnemy[nCntEnemy].pos.y + (sinf(g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].nLength) + (cosf(g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].nLength), 0.0f);
			pVtx[nCntEnemy * 4 + 2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (sinf(g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].nLength) + (cosf(g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].nLength), g_aEnemy[nCntEnemy].pos.y - (sinf(g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].nLength) - (cosf(g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].nLength), 0.0f);
			pVtx[nCntEnemy * 4 + 3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (sinf(g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].nLength) - (cosf(g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].nLength), g_aEnemy[nCntEnemy].pos.y + (sinf(g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].nLength) - (cosf(g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].nLength), 0.0f);

			g_aEnemy[nCntEnemy].nTex = nTex;//画像設定
			if (g_aEnemy[nCntEnemy].type > ENEMYTYPE_NORMAL)
			{//ボス等
				g_aEnemy[nCntEnemy].state = ENEMYSTATE_TIME;	//敵を通常状態に設定
				g_aEnemy[nCntEnemy].nCounterState = 60;			//敵の状態管理初期化
				g_aEnemy[nCntEnemy].movestate = ENEMYMOVE_STOP;
				pVtx[nCntEnemy * 4].tex = D3DXVECTOR2((nTex % 2) * 0.5f, (nTex / 2) * 0.2f);
				pVtx[nCntEnemy * 4 + 1].tex = D3DXVECTOR2((nTex % 2) * 0.5f + 0.5f, (nTex / 2) * 0.2f);
				pVtx[nCntEnemy * 4 + 2].tex = D3DXVECTOR2((nTex % 2) * 0.5f, (nTex / 2) * 0.2f + 0.2f);
				pVtx[nCntEnemy * 4 + 3].tex = D3DXVECTOR2((nTex % 2) * 0.5f + 0.5f, (nTex / 2) * 0.2f + 0.2f);
				if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_TITLE)
				{//Titleエネミーの場合
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_Title;
				}
				if(g_aEnemy[nCntEnemy].type == ENEMYTYPE_SMALL){ g_aEnemy[nCntEnemy].movestate = ENEMYMOVE_NONE; }
			}
			else
			{//雑魚敵
				g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;	//敵を通常状態に設定
				g_aEnemy[nCntEnemy].nCounterState = 0;			//敵の状態管理初期化
				g_aEnemy[nCntEnemy].movestate = ENEMYMOVE_NORMAL;
				pVtx[nCntEnemy * 4].tex = D3DXVECTOR2((nTex % 4) * 0.25f, (nTex / 4) * 0.5f);
				pVtx[nCntEnemy * 4 + 1].tex = D3DXVECTOR2((nTex % 4) * 0.25f + 0.25f, (nTex / 4) * 0.5f);
				pVtx[nCntEnemy * 4 + 2].tex = D3DXVECTOR2((nTex % 4) * 0.25f, (nTex / 4) * 0.5f + 0.5f);
				pVtx[nCntEnemy * 4 + 3].tex = D3DXVECTOR2((nTex % 4) * 0.25f + 0.25f, (nTex / 4) * 0.5f + 0.5f);
			}

			g_aEnemy[nCntEnemy].nCollar = 255;//カラー設定
			pVtx[nCntEnemy * 4].col = D3DCOLOR_RGBA(255, g_aEnemy[nCntEnemy].nCollar, g_aEnemy[nCntEnemy].nCollar, 255);	// A = 透明度
			pVtx[nCntEnemy * 4 + 1].col = D3DCOLOR_RGBA(255, g_aEnemy[nCntEnemy].nCollar, g_aEnemy[nCntEnemy].nCollar, 255);
			pVtx[nCntEnemy * 4 + 2].col = D3DCOLOR_RGBA(255, g_aEnemy[nCntEnemy].nCollar, g_aEnemy[nCntEnemy].nCollar, 255);
			pVtx[nCntEnemy * 4 + 3].col = D3DCOLOR_RGBA(255, g_aEnemy[nCntEnemy].nCollar, g_aEnemy[nCntEnemy].nCollar, 255);
			ifboss[0].nEnemyCounter++;
			break;
		}
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}
//=============================================================================
// * 敵の全消し
//=============================================================================
void BreakEnemy(void)
{//敵のRESET　タイトル等に戻った時使用
	int nCntEnemy;
	ENEMY *g_aEnemy;
	g_aEnemy = GetEnemy();

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{//全部falseに
		g_aEnemy[nCntEnemy].bUse = false;
	}
}
//=============================================================================
// ボス敵へのDamage
//=============================================================================
void DamageBoss(int nCntEnemy, int nDamage)
{//ダメージよりHPが多いなら
	ENEMY *g_aEnemy = GetEnemy();
	if(g_aEnemy[nCntEnemy].nHP > nDamage) { g_aEnemy[nCntEnemy].nHP -= nDamage; }
}
//=============================================================================
// ボス敵へのDamage
//=============================================================================
void DamageBossTitle(int nDamage)
{//観戦時の場合+
	ENEMY *g_aEnemy = GetEnemy();
	IfBoss *ifboss = GetBoss();
	Difficulty *pDifficulty = GetDifficulty();//難易度ごとの敵情報取得
	int nCntEnemy;
	int nCntExplosion;
	static int nCntTime = 0;
	
	nCntTime++;
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//if (g_aEnemy[nCntEnemy].bUse == true && nCntTime % 60 == 0 && g_aEnemy[nCntEnemy].type == ENEMYTYPE_NORMAL)
		//{
		//	g_aEnemy[nCntEnemy].nHP -= nCntEnemy, 3 + (pDifficulty[0].nNumber / 3);
		//}
		if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].type > ENEMYTYPE_NORMAL && g_aEnemy[nCntEnemy].state != ENEMYSTATE_TIME)
		{
			g_aEnemy[nCntEnemy].nHP -= nDamage;
		}
		if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].nHP < 1)
		{//敵のHPが0
			PlaySound(SOUND_LABEL_BOMB000);
			if (g_aEnemy[nCntEnemy].mode == ENEMYMODE_NONE || g_aEnemy[nCntEnemy].mode == ENEMYMODE_NONE_1)
			{//変化なし
				SetExplosion(g_aEnemy[nCntEnemy].pos, COLLAR{ 230,30,30 }, g_aEnemy[nCntEnemy].nLength * 6, g_aEnemy[nCntEnemy].nLength * 6, 5, 4, 2, 0, ANIMATION_NORMAL);
				for (nCntExplosion = 0; nCntExplosion < 3; nCntExplosion++)
				{
					ExplosionEffect(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(g_aEnemy[nCntEnemy].move), D3DXCOLOR{ 1.0f, 0.05f, 0.05f, 0.8f }, 50, 100, 0.1f, 0.1f, 0.3f, g_aEnemy[nCntEnemy].nLength * 0.8f);
					ExplosionEffect(g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].move, D3DXCOLOR{ 1.0f, 0.98f, 0.04f, 0.75f }, 100, 100, 0.1f, g_aEnemy[nCntEnemy].nLength * 0.05f, 0.1f, 1.0f);
					ExplosionEffect(g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].move, D3DXCOLOR{ 0.49f, 1.0f, 0.97f, 0.75f }, 100, 100, 0.1f, g_aEnemy[nCntEnemy].nLength * 0.05f, 0.1f, 1.0f);
					ExplosionEffect(g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].move, D3DXCOLOR{ 1.0f, 0.05f, 0.05f, 0.75f }, 100, 100, 0.1f, g_aEnemy[nCntEnemy].nLength * 0.05f, 0.1f, 1.0f);
					ExplosionEffect(g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].move, D3DXCOLOR{ 1.0f, 0.05f, 0.05f, 0.75f }, 100, 100, 0.1f, g_aEnemy[nCntEnemy].nLength * 0.05f, 0.1f, 1.0f);
					ExplosionEffect(g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].move, D3DXCOLOR{ 1.0f, 0.05f, 0.05f, 0.75f }, 100, 100, 0.1f, g_aEnemy[nCntEnemy].nLength * 0.05f, 0.1f, 1.0f);
				}
				g_aEnemy[nCntEnemy].bUse = false;	//敵の消滅
				if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_BIG)
				{//倒したのがボスだったら
					PlaySound(SOUND_LABEL_BOMB001);
					SetFade(MODE_TITLE, 0);
					BreakBullet(nCntEnemy);
					DeadEnemy();
				}
				else if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_MEDIUM)
				{//倒したのが中ボス
					PlaySound(SOUND_LABEL_BOMB001);
					BreakBullet(nCntEnemy);
					DeadEnemy();
				}
				ifboss[0].nEnemyCounter--;
			}
			else
			{//変化あり
				BossChange(nCntEnemy);
			}
		}//撃破判定
	}
}
//=============================================================================
// * 敵の攻撃
//=============================================================================
void AttackEnemy(int nCntEnemy, int nCntRecast, float fChannel360, float fChannel90, float fPlayerAngle)
{//敵の攻撃処理
	int nCntBullet;
	int nCntBullet1;
	int nCntAtk;
	int nRand = rand() % 628;
	float fRand = nRand * 0.01f;
	float fAngle;
	ENEMY *g_aEnemy = GetEnemy();
	PLAYER *pPlayer = GetPlayer();

	for (nCntAtk = 0; nCntAtk < MAX_ATTCAK; nCntAtk++)
	{
		if (g_aEnemy[nCntEnemy].attack[nCntAtk].state != BULLETSTATE_NOT && g_aEnemy[nCntEnemy].attack[nCntAtk].nRecast != 0 && pPlayer->bUse == true)
		{//使用していい攻撃なら
			g_aEnemy[nCntEnemy].attack[nCntAtk].nCntTime++;
			g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle += g_aEnemy[nCntEnemy].attack[nCntAtk].fAnglePlus;
			g_aEnemy[nCntEnemy].attack[nCntAtk].fSetAngle[0] = g_aEnemy[nCntEnemy].attack[nCntAtk].fSetAngle[1] + g_aEnemy[nCntEnemy].attack[nCntAtk].fSetAngle[0];
			if (g_aEnemy[nCntEnemy].attack[nCntAtk].aim == BULLETAIM_STOP && g_aEnemy[nCntEnemy].attack[nCntAtk].nAtkPlus < g_aEnemy[nCntEnemy].attack[nCntAtk].nCntTime)
			{//タイミングずらしのため初めだけ
				g_aEnemy[nCntEnemy].attack[nCntAtk].aim = BULLETAIM_NORMAL;
				g_aEnemy[nCntEnemy].attack[nCntAtk].nCntTime = 0;
			}
			if (g_aEnemy[nCntEnemy].attack[nCntAtk].aim == BULLETAIM_TIME && g_aEnemy[nCntEnemy].attack[nCntAtk].nCntTime > g_aEnemy[nCntEnemy].attack[nCntAtk].nAttackTime[0])
			{//休憩終わり
				g_aEnemy[nCntEnemy].attack[nCntAtk].aim = BULLETAIM_NORMAL;
				g_aEnemy[nCntEnemy].attack[nCntAtk].nCntTime = 2;		//休憩終了+2　じゃないと一発制限ができないため
			}
			if (g_aEnemy[nCntEnemy].attack[nCntAtk].aim == BULLETAIM_NORMAL && (nCntRecast + g_aEnemy[nCntEnemy].nReplus) % g_aEnemy[nCntEnemy].attack[nCntAtk].nRecast == 0)
			{
				if (g_aEnemy[nCntEnemy].attack[nCntAtk].nCntTime + 1 >= g_aEnemy[nCntEnemy].attack[nCntAtk].nAttackTime[1])
				{//休憩入りまーす
					g_aEnemy[nCntEnemy].attack[nCntAtk].aim = BULLETAIM_TIME;
					g_aEnemy[nCntEnemy].attack[nCntAtk].nCntTime = 0;
					if (g_aEnemy[nCntEnemy].attack[nCntAtk].state == BULLETSTATE_STOP_A || g_aEnemy[nCntEnemy].attack[nCntAtk].state == BULLETSTATE_STOP_S) { PlaySound(SOUND_LABEL_SHOT002); }
					else if (nTimeSE[1] > 9)
					{//30F以上空いていれば
						PlaySound(SOUND_LABEL_SHOT003);
						nTimeSE[1] = 0;
					}
				}
				else if(nTimeSE[0] > 3)
				{//3F以上空いていれば
					PlaySound(SOUND_LABEL_SHOT001); 
					nTimeSE[0] = 0;
				}
				//SetBullet(g_aEnemy[nCntEnemy].pos, 0.0f, g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[0], g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[0], g_aEnemy[nCntEnemy].attack[nCntAtk].nCol % 16 + 32, BULLET_ATKTYPE50, 0.0f, BULLETTYPE_ENEMY, nCntEnemy, BULLETSTATE_FLASH, 0.0f, g_aEnemy[nCntEnemy].attack[nCntAtk].fCurve, g_aEnemy[nCntEnemy].attack[nCntAtk].nAttackTime[1] - g_aEnemy[nCntEnemy].attack[nCntAtk].nCntTime, 0);

				if (g_aEnemy[nCntEnemy].attack[nCntAtk].nTypeAttcak == 0)
				{//全方向
					for (fAngle = 0.0f, nCntBullet = 0; nCntBullet < g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum; fAngle += 2.0f / g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum, nCntBullet++)
					{
						SetBullet(g_aEnemy[nCntEnemy].pos + D3DXVECTOR3(sinf(D3DX_PI * fAngle + g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle), cosf(D3DX_PI * fAngle + g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle), 0.0f) * g_aEnemy[nCntEnemy].attack[nCntAtk].fPos, g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle + D3DX_PI * fAngle, g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[0], g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[1], g_aEnemy[nCntEnemy].attack[nCntAtk].nCol, BULLET_ATKTYPE0, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpeed, BULLETTYPE_ENEMY, nCntEnemy, g_aEnemy[nCntEnemy].attack[nCntAtk].state, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpdPlus, g_aEnemy[nCntEnemy].attack[nCntAtk].fCurve, g_aEnemy[nCntEnemy].attack[nCntAtk].nAttackTime[1] - g_aEnemy[nCntEnemy].attack[nCntAtk].nCntTime, nCntAtk);
					}
				}//出現地, 移動方向, 半幅X,半高Y, 画像, SPEED, 発砲者 
				else if (g_aEnemy[nCntEnemy].attack[nCntAtk].nTypeAttcak == 1)
				{//自機に向かって全方向
					for (fAngle = 0.0f, nCntBullet = 0; nCntBullet < g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum; fAngle += 2.0f / g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum, nCntBullet++)
					{
						SetBullet(g_aEnemy[nCntEnemy].pos + D3DXVECTOR3(sinf(D3DX_PI * fAngle + g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle), cosf(D3DX_PI * fAngle + g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle), 0.0f) * g_aEnemy[nCntEnemy].attack[nCntAtk].fPos, fPlayerAngle + g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle + D3DX_PI * fAngle, g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[0], g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[1], g_aEnemy[nCntEnemy].attack[nCntAtk].nCol, BULLET_ATKTYPE0, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpeed, BULLETTYPE_ENEMY, nCntEnemy, g_aEnemy[nCntEnemy].attack[nCntAtk].state, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpdPlus, g_aEnemy[nCntEnemy].attack[nCntAtk].fCurve, g_aEnemy[nCntEnemy].attack[nCntAtk].nAttackTime[1] - g_aEnemy[nCntEnemy].attack[nCntAtk].nCntTime, nCntAtk);
					}
				}//出現地, 移動方向, 半幅X,半高Y, 画像, SPEED, 発砲者 
				else if (g_aEnemy[nCntEnemy].attack[nCntAtk].nTypeAttcak == 2)
				{//ショットガン 減速有 ？方向 先端でかい弾
					SetBullet(g_aEnemy[nCntEnemy].pos,g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle, g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[0] * 3, g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[1] * 4, 112 + g_aEnemy[nCntEnemy].attack[nCntAtk].nCol, BULLET_ATKTYPE0, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpeed + 0.3f, BULLETTYPE_ENEMY, nCntEnemy, g_aEnemy[nCntEnemy].attack[nCntAtk].state, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpdPlus + fRand * 0.3f, g_aEnemy[nCntEnemy].attack[nCntAtk].fCurve, g_aEnemy[nCntEnemy].attack[nCntAtk].nAttackTime[1] - g_aEnemy[nCntEnemy].attack[nCntAtk].nCntTime, 0);
					for (nCntBullet = 0; nCntBullet < g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum; nCntBullet++)
					{
						nRand = rand() % 75;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle + nRand * 0.01f, g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[0], g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[1], 64 + g_aEnemy[nCntEnemy].attack[nCntAtk].nCol, BULLET_ATKTYPE0, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpeed + fRand * 0.03f, BULLETTYPE_ENEMY, nCntEnemy, g_aEnemy[nCntEnemy].attack[nCntAtk].state, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpdPlus + fRand * 0.2f, g_aEnemy[nCntEnemy].attack[nCntAtk].fCurve, g_aEnemy[nCntEnemy].attack[nCntAtk].nAttackTime[1] - g_aEnemy[nCntEnemy].attack[nCntAtk].nCntTime, 0);
						nRand = rand() % 75;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle + nRand * -0.01f, g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[0], g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[1], 64 + g_aEnemy[nCntEnemy].attack[nCntAtk].nCol, BULLET_ATKTYPE0, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpeed + fRand * 0.03f, BULLETTYPE_ENEMY, nCntEnemy, g_aEnemy[nCntEnemy].attack[nCntAtk].state, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpdPlus + fRand * 0.2f, g_aEnemy[nCntEnemy].attack[nCntAtk].fCurve, g_aEnemy[nCntEnemy].attack[nCntAtk].nAttackTime[1] - g_aEnemy[nCntEnemy].attack[nCntAtk].nCntTime, 0);
					}
					if (nTimeSE[2] > 29) 
					{
						PlaySound(SOUND_LABEL_SHOT004);
						nTimeSE[2] = 0;
					}
				}//出現地, 移動方向, 半幅X,半高Y, 画像, SPEED, 発砲者 
				else if (g_aEnemy[nCntEnemy].attack[nCntAtk].nTypeAttcak == 3)
				{//自機に向かってショットガン 減速有 ？方向 先端でかい弾
					SetBullet(g_aEnemy[nCntEnemy].pos, fPlayerAngle + g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle, g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[0] * 3, g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[1] * 4, 112 + g_aEnemy[nCntEnemy].attack[nCntAtk].nCol, BULLET_ATKTYPE0, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpeed + 0.3f, BULLETTYPE_ENEMY, nCntEnemy, g_aEnemy[nCntEnemy].attack[nCntAtk].state, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpdPlus + fRand * 0.3f, g_aEnemy[nCntEnemy].attack[nCntAtk].fCurve, g_aEnemy[nCntEnemy].attack[nCntAtk].nAttackTime[1] - g_aEnemy[nCntEnemy].attack[nCntAtk].nCntTime, 0);
					for (nCntBullet = 0; nCntBullet < g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum; nCntBullet++)
					{
						nRand = rand() % 75;
						SetBullet(g_aEnemy[nCntEnemy].pos, fPlayerAngle + g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle + nRand * 0.01f, g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[0], g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[1], 64 + g_aEnemy[nCntEnemy].attack[nCntAtk].nCol, BULLET_ATKTYPE0, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpeed + fRand * 0.03f, BULLETTYPE_ENEMY, nCntEnemy, g_aEnemy[nCntEnemy].attack[nCntAtk].state, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpdPlus + fRand * 0.2f, g_aEnemy[nCntEnemy].attack[nCntAtk].fCurve, g_aEnemy[nCntEnemy].attack[nCntAtk].nAttackTime[1] - g_aEnemy[nCntEnemy].attack[nCntAtk].nCntTime, 0);
						nRand = rand() % 75;
						SetBullet(g_aEnemy[nCntEnemy].pos, fPlayerAngle + g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle + nRand * -0.01f, g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[0], g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[1], 64 + g_aEnemy[nCntEnemy].attack[nCntAtk].nCol, BULLET_ATKTYPE0, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpeed + fRand * 0.03f, BULLETTYPE_ENEMY, nCntEnemy, g_aEnemy[nCntEnemy].attack[nCntAtk].state, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpdPlus + fRand * 0.2f, g_aEnemy[nCntEnemy].attack[nCntAtk].fCurve, g_aEnemy[nCntEnemy].attack[nCntAtk].nAttackTime[1] - g_aEnemy[nCntEnemy].attack[nCntAtk].nCntTime, 0);
					}
					if (nTimeSE[2] > 29)
					{
						PlaySound(SOUND_LABEL_SHOT004);
						nTimeSE[2] = 0;
					}
				}//出現地, 移動方向, 半幅X,半高Y, 画像, SPEED, 発砲者 
				else if (g_aEnemy[nCntEnemy].attack[nCntAtk].nTypeAttcak == 4)
				{//ランド玉　360度　1つ
					for (nCntBullet = 0; nCntBullet < g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum; nCntBullet++)
					{
						nRand = rand() % 628;
						SetBullet(g_aEnemy[nCntEnemy].pos, nRand * 0.01f, g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[0], g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[1], g_aEnemy[nCntEnemy].attack[nCntAtk].nCol, BULLET_ATKTYPE0, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpeed, BULLETTYPE_ENEMY, nCntEnemy, g_aEnemy[nCntEnemy].attack[nCntAtk].state, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpdPlus, g_aEnemy[nCntEnemy].attack[nCntAtk].fCurve, g_aEnemy[nCntEnemy].attack[nCntAtk].nAttackTime[1] - g_aEnemy[nCntEnemy].attack[nCntAtk].nCntTime, 0);
					}
				}//出現地, 移動方向, 半幅X,半高Y, 画像, SPEED, 発砲者 
				else if (g_aEnemy[nCntEnemy].attack[nCntAtk].nTypeAttcak == 5)
				{//1方向に飛ぶ　配置だけ全
					for (fAngle = 0.0f, nCntBullet = 0; nCntBullet < g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum; fAngle += 2.0f / g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum, nCntBullet++)
					{
						SetBullet(g_aEnemy[nCntEnemy].pos + D3DXVECTOR3(sinf(D3DX_PI * fAngle + g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle), cosf(D3DX_PI * fAngle), 0.0f) * g_aEnemy[nCntEnemy].attack[nCntAtk].fPos, g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle, g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[0], g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[1], g_aEnemy[nCntEnemy].attack[nCntAtk].nCol, BULLET_ATKTYPE0, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpeed, BULLETTYPE_ENEMY, nCntEnemy, g_aEnemy[nCntEnemy].attack[nCntAtk].state, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpdPlus, g_aEnemy[nCntEnemy].attack[nCntAtk].fCurve, g_aEnemy[nCntEnemy].attack[nCntAtk].nAttackTime[1] - g_aEnemy[nCntEnemy].attack[nCntAtk].nCntTime, 0);
					}
				}//出現地, 移動方向, 半幅X,半高Y, 画像, SPEED, 発砲者 
				else if (g_aEnemy[nCntEnemy].attack[nCntAtk].nTypeAttcak == 6)
				{//1方向に飛ぶ　配置だけ全
					for (fAngle = 0.0f, nCntBullet = 0; nCntBullet < g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum; fAngle += 2.0f / g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum, nCntBullet++)
					{
						SetBullet(g_aEnemy[nCntEnemy].pos + D3DXVECTOR3(sinf(D3DX_PI * fAngle + g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle), cosf(D3DX_PI * fAngle), 0.0f) * g_aEnemy[nCntEnemy].attack[nCntAtk].fPos, fPlayerAngle + g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle, g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[0], g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[1], g_aEnemy[nCntEnemy].attack[nCntAtk].nCol, BULLET_ATKTYPE0, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpeed, BULLETTYPE_ENEMY, nCntEnemy, g_aEnemy[nCntEnemy].attack[nCntAtk].state, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpdPlus, g_aEnemy[nCntEnemy].attack[nCntAtk].fCurve, g_aEnemy[nCntEnemy].attack[nCntAtk].nAttackTime[1] - g_aEnemy[nCntEnemy].attack[nCntAtk].nCntTime, 0);
					}
				}//出現地, 移動方向, 半幅X,半高Y, 画像, SPEED, 発砲者 
				else if (g_aEnemy[nCntEnemy].attack[nCntAtk].nTypeAttcak == 7)
				{//ショットガン 減速有 ？方向 先端でかい弾
					for (fAngle = 0.0f, nCntBullet1 = 0; nCntBullet1 < g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum; fAngle += 2.0f / g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum, nCntBullet1++)
					{
						SetBullet(g_aEnemy[nCntEnemy].pos, fPlayerAngle + g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle + D3DX_PI * fAngle, g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[0] * 3, g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[1] * 4, 112 + g_aEnemy[nCntEnemy].attack[nCntAtk].nCol, BULLET_ATKTYPE0, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpeed + 0.3f, BULLETTYPE_ENEMY, nCntEnemy, g_aEnemy[nCntEnemy].attack[nCntAtk].state, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpdPlus + fRand * 0.3f, g_aEnemy[nCntEnemy].attack[nCntAtk].fCurve, g_aEnemy[nCntEnemy].attack[nCntAtk].nAttackTime[1] - g_aEnemy[nCntEnemy].attack[nCntAtk].nCntTime, 0);
						for (nCntBullet = 0; nCntBullet < g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum; nCntBullet++)
						{
							nRand = rand() % 75;
							SetBullet(g_aEnemy[nCntEnemy].pos, fPlayerAngle + g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle + nRand * 0.01f + D3DX_PI * fAngle, g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[0], g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[1], 64 + g_aEnemy[nCntEnemy].attack[nCntAtk].nCol, BULLET_ATKTYPE0, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpeed + fRand * 0.03f, BULLETTYPE_ENEMY, nCntEnemy, g_aEnemy[nCntEnemy].attack[nCntAtk].state, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpdPlus + fRand * 0.2f, g_aEnemy[nCntEnemy].attack[nCntAtk].fCurve, g_aEnemy[nCntEnemy].attack[nCntAtk].nAttackTime[1] - g_aEnemy[nCntEnemy].attack[nCntAtk].nCntTime, 0);
							nRand = rand() % 75;
							SetBullet(g_aEnemy[nCntEnemy].pos, fPlayerAngle + g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle + nRand * -0.01f + D3DX_PI * fAngle, g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[0], g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[1], 64 + g_aEnemy[nCntEnemy].attack[nCntAtk].nCol, BULLET_ATKTYPE0, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpeed + fRand * 0.03f, BULLETTYPE_ENEMY, nCntEnemy, g_aEnemy[nCntEnemy].attack[nCntAtk].state, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpdPlus + fRand * 0.2f, g_aEnemy[nCntEnemy].attack[nCntAtk].fCurve, g_aEnemy[nCntEnemy].attack[nCntAtk].nAttackTime[1] - g_aEnemy[nCntEnemy].attack[nCntAtk].nCntTime, 0);
						}
					}
					if (nTimeSE[2] > 29)
					{
						PlaySound(SOUND_LABEL_SHOT004);
						nTimeSE[2] = 0;
					}
				}//出現地, 移動方向, 半幅X,半高Y, 画像, SPEED, 発砲者 
				else if (g_aEnemy[nCntEnemy].attack[nCntAtk].nTypeAttcak == 8)
				{//全方向
					for (fAngle = 0.0f, nCntBullet = 0; nCntBullet < g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum; fAngle += 2.0f / g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum, nCntBullet++)
					{
						SetBullet(g_aEnemy[nCntEnemy].pos + D3DXVECTOR3(sinf(D3DX_PI * fAngle + g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle), cosf(D3DX_PI * fAngle + g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle), 0.0f) * g_aEnemy[nCntEnemy].attack[nCntAtk].fPos, g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle + D3DX_PI * fAngle, g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[0], g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[1], g_aEnemy[nCntEnemy].attack[nCntAtk].nCol, BULLET_ATKTYPE0, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpeed, BULLETTYPE_ENEMY, nCntEnemy, g_aEnemy[nCntEnemy].attack[nCntAtk].state, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpdPlus, g_aEnemy[nCntEnemy].attack[nCntAtk].fCurve, g_aEnemy[nCntEnemy].attack[nCntAtk].nAttackTime[1] - g_aEnemy[nCntEnemy].attack[nCntAtk].nCntTime, nCntAtk);
						SetBullet(g_aEnemy[nCntEnemy].pos + D3DXVECTOR3(sinf(D3DX_PI * fAngle + g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle), cosf(D3DX_PI * fAngle + g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle), 0.0f) * g_aEnemy[nCntEnemy].attack[nCntAtk].fPos, g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle + D3DX_PI * fAngle, g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[0], g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[1], g_aEnemy[nCntEnemy].attack[nCntAtk].nCol, BULLET_ATKTYPE0, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpeed, BULLETTYPE_ENEMY, nCntEnemy, g_aEnemy[nCntEnemy].attack[nCntAtk].state, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpdPlus, g_aEnemy[nCntEnemy].attack[nCntAtk].fCurve * -1.0f, g_aEnemy[nCntEnemy].attack[nCntAtk].nAttackTime[1] - g_aEnemy[nCntEnemy].attack[nCntAtk].nCntTime, nCntAtk);
					}
				}//出現地, 移動方向, 半幅X,半高Y, 画像, SPEED, 発砲者 
				else if (g_aEnemy[nCntEnemy].attack[nCntAtk].nTypeAttcak == 10)
				{//画面は時に4方向十字に発射するエネミーを投げる
					for (nCntBullet = 0; nCntBullet < g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum; nCntBullet++)
					{
						nRand = rand() % 64;
						SetEnemy(D3DXVECTOR3(GAME_WIDE - 30, 0, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE_1, 0.0f, 0.0f, 0.3f, 0, nRand, g_aEnemy[nCntEnemy].attack[nCntAtk].nCol,
							ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },0,51,{ 60,0 },{ 8, 8 }, 4,0,1,0,0.0f, 0.0f,0.0f, 0.0f, 0.2f, 0.0f },
							ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
						SetEnemy(D3DXVECTOR3(30, 0, 0.0f), ENEMYTYPE_NORMAL, ENEMYMODE_NONE_1, 0.0f, 0.0f, 0.3f, 0, nRand, g_aEnemy[nCntEnemy].attack[nCntAtk].nCol,
							ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },0,59,{ 60,0 },{ 8, 8 }, 4,0,1,0,0.0f, 0.0f,0.0f, 0.0f, 0.2f, 0.0f },
							ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT }, ATTACK{ BULLETSTATE_NOT });
					}
				}//出現地, 移動方向, 半幅X,半高Y, 画像, SPEED, 発砲者 
				else if (g_aEnemy[nCntEnemy].attack[nCntAtk].nTypeAttcak == 10)
				{//プレイヤーに向けて8方向
					//SetBullet(g_aEnemy[nCntEnemy].pos, fPlayerAngle, 8, 8, 64 + g_aEnemy[nCntEnemy].attack[nCntAtk].nCol, BULLET_ATKTYPE0, 3.0f, BULLETTYPE_ENEMY, nCntEnemy);
				}//出現地, 移動方向, 半幅X,半高Y, 画像, SPEED, 発砲者 
				else if (g_aEnemy[nCntEnemy].attack[nCntAtk].nTypeAttcak == 11)
				{//ナイフ
					SetBullet(g_aEnemy[nCntEnemy].pos, 0.0f, 13, 80, 0, BULLET_ATKTYPE99, 4.0f, BULLETTYPE_ENEMY, nCntEnemy, BULLETSTATE_NONE, 0.0f, 0.0f, 0, 0);
				}//出現地, 移動方向, 半幅X,半高Y, 画像, SPEED, 発砲者 
				else if (g_aEnemy[nCntEnemy].attack[nCntAtk].nTypeAttcak == 50)
				{//全体縦にランド弾
					for (nCntBullet = 0; nCntBullet < g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum; nCntBullet++)
					{
						nRand = rand() % GAME_WIDE;
						SetBullet(D3DXVECTOR3(nRand * 1.0f, -15.0f, 0.0f), g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle, g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[0], g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[1], g_aEnemy[nCntEnemy].attack[nCntAtk].nCol, BULLET_ATKTYPE0, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpeed, BULLETTYPE_ENEMY, nCntEnemy, g_aEnemy[nCntEnemy].attack[nCntAtk].state, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpdPlus, g_aEnemy[nCntEnemy].attack[nCntAtk].fCurve, g_aEnemy[nCntEnemy].attack[nCntAtk].nAttackTime[1] - g_aEnemy[nCntEnemy].attack[nCntAtk].nCntTime, nCntAtk);
						//SetBullet(D3DXVECTOR3(nRand * 1.0f, -15.0f, 0.0f), g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle, g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[0], g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[1], g_aEnemy[nCntEnemy].attack[nCntAtk].nCol, BULLET_ATKTYPE0, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpeed, BULLETTYPE_ENEMY, nCntEnemy, g_aEnemy[nCntEnemy].attack[nCntAtk].state, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpdPlus, g_aEnemy[nCntEnemy].attack[nCntAtk].fCurve, g_aEnemy[nCntEnemy].attack[nCntAtk].nAttackTime[1] - g_aEnemy[nCntEnemy].attack[nCntAtk].nCntTime, nCntAtk);
					}
				}//出現地, 移動方向, 半幅X,半高Y, 画像, SPEED, 発砲者 
				else if (g_aEnemy[nCntEnemy].attack[nCntAtk].nTypeAttcak == 51)
				{//全体上横から0ウェポンの攻撃
					for (nCntBullet = 0; nCntBullet < g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum; nCntBullet++)
					{
						for (fAngle = 0.0f, nCntBullet = 0; nCntBullet < g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum; fAngle += 2.0f / g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum, nCntBullet++)
						{
							SetBullet(D3DXVECTOR3(GAME_WIDE / 2, - 195,0.0f), g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle + D3DX_PI * fAngle, g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[0], g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[1], g_aEnemy[nCntEnemy].attack[nCntAtk].nCol, BULLET_ATKTYPE0, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpeed, BULLETTYPE_ENEMY, nCntEnemy, g_aEnemy[nCntEnemy].attack[nCntAtk].state, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpdPlus, g_aEnemy[nCntEnemy].attack[nCntAtk].fCurve, g_aEnemy[nCntEnemy].attack[nCntAtk].nAttackTime[1] - g_aEnemy[nCntEnemy].attack[nCntAtk].nCntTime, nCntAtk);
						}
						for (fAngle = 0.0f, nCntBullet = 0; nCntBullet < g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum; fAngle += 2.0f / g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum, nCntBullet++)
						{
							SetBullet(D3DXVECTOR3(GAME_WIDE + 195, SCREEN_HEIGHT / 3, 0.0f), g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle + D3DX_PI * fAngle, g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[0], g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[1], g_aEnemy[nCntEnemy].attack[nCntAtk].nCol, BULLET_ATKTYPE0, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpeed, BULLETTYPE_ENEMY, nCntEnemy, g_aEnemy[nCntEnemy].attack[nCntAtk].state, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpdPlus, g_aEnemy[nCntEnemy].attack[nCntAtk].fCurve, g_aEnemy[nCntEnemy].attack[nCntAtk].nAttackTime[1] - g_aEnemy[nCntEnemy].attack[nCntAtk].nCntTime, nCntAtk);
						}
						for (fAngle = 0.0f, nCntBullet = 0; nCntBullet < g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum; fAngle += 2.0f / g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum, nCntBullet++)
						{
							SetBullet(D3DXVECTOR3(-195, SCREEN_HEIGHT / 3,0.0f), g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle + D3DX_PI * fAngle, g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[0], g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[1], g_aEnemy[nCntEnemy].attack[nCntAtk].nCol, BULLET_ATKTYPE0, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpeed, BULLETTYPE_ENEMY, nCntEnemy, g_aEnemy[nCntEnemy].attack[nCntAtk].state, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpdPlus, g_aEnemy[nCntEnemy].attack[nCntAtk].fCurve, g_aEnemy[nCntEnemy].attack[nCntAtk].nAttackTime[1] - g_aEnemy[nCntEnemy].attack[nCntAtk].nCntTime, nCntAtk);
						}
					}
				}//出現地, 移動方向, 半幅X,半高Y, 画像, SPEED, 発砲者 
				else if (g_aEnemy[nCntEnemy].attack[nCntAtk].nTypeAttcak == 52)
				{//敵横上からウェポン0の攻撃
					for (nCntBullet = 0; nCntBullet < g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum; nCntBullet++)
					{
						for (fAngle = 0.0f, nCntBullet = 0; nCntBullet < g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum; fAngle += 2.0f / g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum, nCntBullet++)
						{
							SetBullet(g_aEnemy[nCntEnemy].pos + D3DXVECTOR3(sinf(D3DX_PI * fAngle + g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle), cosf(D3DX_PI * fAngle), 0.0f) * g_aEnemy[nCntEnemy].attack[nCntAtk].fPos + D3DXVECTOR3(g_aEnemy[nCntEnemy].nLength * 1.0f, -g_aEnemy[nCntEnemy].nLength * 1.0f,0.0f), g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle + D3DX_PI * fAngle, g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[0], g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[1], g_aEnemy[nCntEnemy].attack[nCntAtk].nCol, BULLET_ATKTYPE0, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpeed, BULLETTYPE_ENEMY, nCntEnemy, g_aEnemy[nCntEnemy].attack[nCntAtk].state, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpdPlus, g_aEnemy[nCntEnemy].attack[nCntAtk].fCurve, g_aEnemy[nCntEnemy].attack[nCntAtk].nAttackTime[1] - g_aEnemy[nCntEnemy].attack[nCntAtk].nCntTime, nCntAtk);
						}
					}
				}//出現地, 移動方向, 半幅X,半高Y, 画像, SPEED, 発砲者 
				else if (g_aEnemy[nCntEnemy].attack[nCntAtk].nTypeAttcak == 53)
				{//敵横上からウェポン0の攻撃
					for (nCntBullet = 0; nCntBullet < g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum; nCntBullet++)
					{
						for (fAngle = 0.0f, nCntBullet = 0; nCntBullet < g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum; fAngle += 2.0f / g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum, nCntBullet++)
						{
							SetBullet(g_aEnemy[nCntEnemy].pos + D3DXVECTOR3(sinf(D3DX_PI * fAngle + g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle), cosf(D3DX_PI * fAngle), 0.0f) * g_aEnemy[nCntEnemy].attack[nCntAtk].fPos + D3DXVECTOR3(-g_aEnemy[nCntEnemy].nLength * 1.0f, -g_aEnemy[nCntEnemy].nLength * 1.0f, 0.0f), g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle + D3DX_PI * fAngle, g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[0], g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[1], g_aEnemy[nCntEnemy].attack[nCntAtk].nCol, BULLET_ATKTYPE0, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpeed, BULLETTYPE_ENEMY, nCntEnemy, g_aEnemy[nCntEnemy].attack[nCntAtk].state, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpdPlus, g_aEnemy[nCntEnemy].attack[nCntAtk].fCurve, g_aEnemy[nCntEnemy].attack[nCntAtk].nAttackTime[1] - g_aEnemy[nCntEnemy].attack[nCntAtk].nCntTime, nCntAtk);
						}
					}
				}//出現地, 移動方向, 半幅X,半高Y, 画像, SPEED, 発砲者 
				else if (g_aEnemy[nCntEnemy].attack[nCntAtk].nTypeAttcak == 54)
				{//敵横上からウェポン0の攻撃
					for (nCntBullet = 0; nCntBullet < g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum; nCntBullet++)
					{
						for (fAngle = 0.0f, nCntBullet = 0; nCntBullet < g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum; fAngle += 2.0f / g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum, nCntBullet++)
						{
							SetBullet(g_aEnemy[nCntEnemy].pos + D3DXVECTOR3(sinf(D3DX_PI * fAngle + g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle), cosf(D3DX_PI * fAngle), 0.0f) * g_aEnemy[nCntEnemy].attack[nCntAtk].fPos + D3DXVECTOR3(g_aEnemy[nCntEnemy].nLength * 1.0f, -g_aEnemy[nCntEnemy].nLength * 1.0f, 0.0f), g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle + D3DX_PI * fAngle, g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[0], g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[1], g_aEnemy[nCntEnemy].attack[nCntAtk].nCol, BULLET_ATKTYPE0, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpeed, BULLETTYPE_ENEMY, nCntEnemy, g_aEnemy[nCntEnemy].attack[nCntAtk].state, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpdPlus, g_aEnemy[nCntEnemy].attack[nCntAtk].fCurve, g_aEnemy[nCntEnemy].attack[nCntAtk].nAttackTime[1] - g_aEnemy[nCntEnemy].attack[nCntAtk].nCntTime, nCntAtk);
						}
						for (fAngle = 0.0f, nCntBullet = 0; nCntBullet < g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum; fAngle += 2.0f / g_aEnemy[nCntEnemy].attack[nCntAtk].nBulletNum, nCntBullet++)
						{
							SetBullet(g_aEnemy[nCntEnemy].pos + D3DXVECTOR3(sinf(D3DX_PI * fAngle + g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle), cosf(D3DX_PI * fAngle), 0.0f) * g_aEnemy[nCntEnemy].attack[nCntAtk].fPos + D3DXVECTOR3(-g_aEnemy[nCntEnemy].nLength * 1.0f, -g_aEnemy[nCntEnemy].nLength * 1.0f, 0.0f), g_aEnemy[nCntEnemy].attack[nCntAtk].fAngle + D3DX_PI * fAngle, g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[0], g_aEnemy[nCntEnemy].attack[nCntAtk].nLength[1], g_aEnemy[nCntEnemy].attack[nCntAtk].nCol, BULLET_ATKTYPE0, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpeed, BULLETTYPE_ENEMY, nCntEnemy, g_aEnemy[nCntEnemy].attack[nCntAtk].state, g_aEnemy[nCntEnemy].attack[nCntAtk].fSpdPlus, g_aEnemy[nCntEnemy].attack[nCntAtk].fCurve, g_aEnemy[nCntEnemy].attack[nCntAtk].nAttackTime[1] - g_aEnemy[nCntEnemy].attack[nCntAtk].nCntTime, nCntAtk);
						}
					}
				}//出現地, 移動方向, 半幅X,半高Y, 画像, SPEED, 発砲者 
			}
		}
	}

}
//=============================================================================
// * 敵の攻撃
//=============================================================================
void SetAttack(int nAtk, int nNumBullet, int nLengthX, int nLengthY, int nCol, int nCntEnemy, int nEnemyAtk, float fSteep, float fSpeed, float fSpdPlus, float fCurve, D3DXVECTOR3 pos, float fPos, BULLETSTATE state)
{//敵の攻撃処理
	int nCntBullet;
	int nRand = rand() % 628;
	float fRand = nRand * 0.01f;
	float fAngle;
	ENEMY *g_aEnemy = GetEnemy();
	PLAYER *pPlayer = GetPlayer();

	if (nAtk == 0)
	{//全方向
		for (fAngle = 0.0f, nCntBullet = 0; nCntBullet < nNumBullet; fAngle += 2.0f / nNumBullet, nCntBullet++)
		{
			SetBullet(pos + D3DXVECTOR3(sinf(D3DX_PI * fAngle), cosf(D3DX_PI * fAngle), 0.0f) * fPos, D3DX_PI * fAngle + fSteep, nLengthX, nLengthY, nCol, BULLET_ATKTYPE0, fSpeed, BULLETTYPE_ENEMY, nCntEnemy, state, fSpdPlus, fCurve, 300, nEnemyAtk);
		}
	}//出現地, 移動方向, 半幅X,半高Y, 画像, SPEED, 発砲者 
	else if (nAtk == 6)
	{//1方向に飛ぶ　配置だけ全
		for (fAngle = 0.0f, nCntBullet = 0; nCntBullet < nNumBullet; fAngle += 2.0f / nNumBullet, nCntBullet++)
		{
			SetBullet(pos + D3DXVECTOR3(sinf(D3DX_PI * fAngle), cosf(D3DX_PI * fAngle), 0.0f) * fPos, D3DX_PI * fAngle + fSteep, nLengthX, nLengthY, nCol, BULLET_ATKTYPE0, fSpeed, BULLETTYPE_ENEMY, nCntEnemy, state, fSpdPlus, fCurve, 200, nEnemyAtk);
		}
	}//出現地, 移動方向, 半幅X,半高Y, 画像, SPEED, 発砲者 
}

//=============================================================================
// * 敵のダメージ処理
//=============================================================================
void HitEnemy(int nCntEnemy, int nDamageEnemy, int nAtkType)
{
	int nCntExplosion;
	int nRand;				//捕獲計算のため
	int nScore;
	PLAYER *pPlayer;
	ENEMY *g_aEnemy = GetEnemy();
	IfBoss *ifboss = GetBoss();

	pPlayer = GetPlayer();

	g_aEnemy[nCntEnemy].nHP -= nDamageEnemy;
	if (g_aEnemy[nCntEnemy].nHP < 1)
	{//敵のHPが0
		PlaySound(SOUND_LABEL_BOMB000);
		if (g_aEnemy[nCntEnemy].mode == ENEMYMODE_NONE || g_aEnemy[nCntEnemy].mode == ENEMYMODE_NONE_1)
		{//変化なし
			nRand = rand() % 100;
			if (nRand < CATCH_ENEMY && pPlayer[0].bUse == true && g_aEnemy[nCntEnemy].type == ENEMYTYPE_SMALL && pPlayer[0].bKnifeLimit == false)
			{//捕獲
				SetShooter(D3DXVECTOR3(cosf(0.25f * nRand) * 0.4f, cosf(0.25f * nRand) * 0.4f, 0.0f), g_aEnemy[nCntEnemy].nTex);
				SetExplosion(g_aEnemy[nCntEnemy].pos, COLLAR{ 116,255,236 }, g_aEnemy[nCntEnemy].nLength * 6, g_aEnemy[nCntEnemy].nLength * 6, 5, 4, 2, 0, ANIMATION_NORMAL);
				ExplosionEffect(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(g_aEnemy[nCntEnemy].move), D3DXCOLOR{ 0.49f, 1.0f, 0.97f, 0.8f }, 50, 100, 0.1f, 0.1f, 0.3f, g_aEnemy[nCntEnemy].nLength * 0.8f);
				ExplosionEffect(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(g_aEnemy[nCntEnemy].move), D3DXCOLOR{ 0.49f, 1.0f, 0.97f, 0.8f }, 50, 100, 0.1f, 0.1f, 0.3f, g_aEnemy[nCntEnemy].nLength * 0.8f);
				ExplosionEffect(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(g_aEnemy[nCntEnemy].move), D3DXCOLOR{ 0.49f, 1.0f, 0.97f, 0.8f }, 50, 100, 0.1f, 0.1f, 0.3f, g_aEnemy[nCntEnemy].nLength * 0.8f);
			}
			else
			{
				SetExplosion(g_aEnemy[nCntEnemy].pos, COLLAR{ 230,30,30 }, g_aEnemy[nCntEnemy].nLength * 6, g_aEnemy[nCntEnemy].nLength * 6, 5, 4, 2, 0, ANIMATION_NORMAL);
				ExplosionEffect(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(g_aEnemy[nCntEnemy].move), D3DXCOLOR{ 1.0f, 0.05f, 0.05f, 0.8f }, 50, 100, 0.1f, 0.1f, 0.3f, g_aEnemy[nCntEnemy].nLength * 0.8f);
				ExplosionEffect(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(g_aEnemy[nCntEnemy].move), D3DXCOLOR{ 1.0f, 0.05f, 0.05f, 0.8f }, 50, 100, 0.1f, 0.1f, 0.3f, g_aEnemy[nCntEnemy].nLength * 0.8f);
				ExplosionEffect(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(g_aEnemy[nCntEnemy].move), D3DXCOLOR{ 1.0f, 0.05f, 0.05f, 0.8f }, 50, 100, 0.1f, 0.1f, 0.3f, g_aEnemy[nCntEnemy].nLength * 0.8f);
			}
			for (nCntExplosion = 0; nCntExplosion < 3; nCntExplosion++)
			{
				ExplosionEffect(g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].move, D3DXCOLOR{ 1.0f, 0.98f, 0.04f, 0.75f }, 100, 100, 0.1f, g_aEnemy[nCntEnemy].nLength * 0.05f, 0.1f, 1.0f);
				ExplosionEffect(g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].move, D3DXCOLOR{ 0.49f, 1.0f, 0.97f, 0.75f }, 100, 100, 0.1f, g_aEnemy[nCntEnemy].nLength * 0.05f, 0.1f, 1.0f);
				ExplosionEffect(g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].move, D3DXCOLOR{ 1.0f, 0.05f, 0.05f, 0.75f }, 100, 100, 0.1f, g_aEnemy[nCntEnemy].nLength * 0.05f, 0.1f, 1.0f);
				ExplosionEffect(g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].move, D3DXCOLOR{ 1.0f, 0.05f, 0.05f, 0.75f }, 100, 100, 0.1f, g_aEnemy[nCntEnemy].nLength * 0.05f, 0.1f, 1.0f);
				ExplosionEffect(g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].move, D3DXCOLOR{ 1.0f, 0.05f, 0.05f, 0.75f }, 100, 100, 0.1f, g_aEnemy[nCntEnemy].nLength * 0.05f, 0.1f, 1.0f);
			}			g_aEnemy[nCntEnemy].bUse = false;	//敵の消滅
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_BIG)
			{//倒したのがボスだったら
				PlaySound(SOUND_LABEL_BOMB001);
				ifboss[0].g_bBossKill = true;
				nScore = 100000;
				BreakBullet(nCntEnemy);
				DeadEnemy();
			}
			else if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_MEDIUM)
			{//倒したのが中ボス
				PlaySound(SOUND_LABEL_BOMB001);
				nScore = 37500;
				BreakBullet(nCntEnemy);
				DeadEnemy();
			}
			else if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_SMALL)
			{//小ボスなら
				nScore = 20000;
			}
			else if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_NORMAL)
			{//雑魚なら
				nScore = 5000;
			}

			if (nAtkType >= BULLET_ATKTYPE99)
			{
				nScore = nScore * 120 / 100;
			}
			AddScore(nScore, 0);
			ifboss[0].nEnemyCounter--;
		}
		else
		{//変化あり
			BossChange(nCntEnemy);
		}
	}//撃破判定
	else
	{//ダメージ判定
		g_aEnemy[nCntEnemy].nCollar = 0;
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
		if(g_nCntSound > 3 && nAtkType < BULLET_ATKTYPE99)
		{ 
			PlaySound(SOUND_LABEL_HIT_Shot000);
			g_nCntSound = 0;
		}
	}
}
//=============================================================================
// * 敵のMode変化
//=============================================================================
void BossChange(int nCntEnemy)
{
	ENEMY *g_aEnemy = GetEnemy();

	DeadEnemy();
	BreakBullet(nCntEnemy);
	g_aEnemy[nCntEnemy].state = ENEMYSTATE_TIME;
	g_aEnemy[nCntEnemy].nCounterState = 100;
	g_aEnemy[nCntEnemy].nCollar = 255;
	SetWindow(D3DXVECTOR3(SCREEN_GAMEX / 3 * 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR{ 1.0f, 1.0f, 1.0f, 0.0f }, GAME_WIDE / 2, GAME_WIDE / 2, 22, 0, g_aEnemy[nCntEnemy].nTex, WINDOWSTATE_ENEMYCHANGE_ZOOM, WINDOWUSE_ENEMY);
	SetWindow(D3DXVECTOR3(0, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR{ 1.0f, 1.0f, 1.0f, 0.0f }, GAME_WIDE / 2, SCREEN_HEIGHT / 6, 23, 0, 0, WINDOWSTATE_ENEMYCHANGE_FLOW, WINDOWUSE_NORMAL);
	PlaySound(SOUND_LABEL_MODE_CHANGE);
	switch (g_aEnemy[nCntEnemy].mode)
	{//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ		休憩, 連射は最低1
	case ENEMYMODE_MBOSS_EASY://ウェポンの切り替え
		g_aEnemy[nCntEnemy].mode = ENEMYMODE_NONE;
		g_aEnemy[nCntEnemy].movestate = ENEMYMOVE_NONE;
		g_aEnemy[nCntEnemy].nHP = 700;
		g_aEnemy[nCntEnemy].attack[0] = ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },0,37,{ 90,1 },{ 25,25 }, 8,0,1,0,0.001f, 0.0f,0.003f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[1] = ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },0,25,{ 1,30 },{ 9, 10 }, 3,0,5,0,0.0f, 0.0f,0.03f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f },
		g_aEnemy[nCntEnemy].attack[2] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[3] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[4] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		break;
		//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ		休憩, 連射は最低1
	case ENEMYMODE_MBOSS_NORMAL://ウェポンの切り替え
		g_aEnemy[nCntEnemy].mode = ENEMYMODE_NONE;
		g_aEnemy[nCntEnemy].movestate = ENEMYMOVE_NONE;
		g_aEnemy[nCntEnemy].nHP = 800;
		g_aEnemy[nCntEnemy].attack[0] = ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },0,117,{ 90,60 },{ 7,18 }, 8,0,3,0,0.0f, 0.3f,0.03f, 0.0f, 0.3f,0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[1] = ATTACK{ BULLETSTATE_ACCELERATOR, BULLETAIM_STOP,{ 0,0 },0,35,{ 61,100 },{ 25,25 }, 3,0,20,20,0.005f, 0.0f,0.1f, 0.005f, 0.1f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[2] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[3] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[4] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		break;
		//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
	case ENEMYMODE_MBOSS_HARD://ウェポンの切り替え
		g_aEnemy[nCntEnemy].mode = ENEMYMODE_NONE;
		g_aEnemy[nCntEnemy].movestate = ENEMYMOVE_NONE;
		g_aEnemy[nCntEnemy].nHP = 800;
		g_aEnemy[nCntEnemy].attack[0] = ATTACK{ BULLETSTATE_SET_B2, BULLETAIM_NORMAL,{ 0,0 },1,19,{ 200,0 },{ 30,30 }, 4,0,3,0,0.0f, 0.0f,0.0f, 0.0f, 0.8f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[1] = ATTACK{ BULLETSTATE_ACCELERATOR, BULLETAIM_STOP,{ 0,0 },0,35,{ 150,50 },{ 15,15 }, 3,0,3,150,0.005f, 0.0f,0.1f, 0.0f, 0.1f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[2] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[3] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[4] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		break;
		//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
	case ENEMYMODE_MBOSS_LUNATIC://ウェポンの切り替え
		g_aEnemy[nCntEnemy].mode = ENEMYMODE_NONE;
		g_aEnemy[nCntEnemy].movestate = ENEMYMOVE_NONE;
		g_aEnemy[nCntEnemy].nHP = 850;
		g_aEnemy[nCntEnemy].attack[0] = ATTACK{ BULLETSTATE_SET_A, BULLETAIM_NORMAL,{ 0,0 },0,29,{ 200,0 },{ 30, 30 }, 4,0,1,0,0.0f, D3DX_PI * 0.25,0.0f, 0.0f, 0.8f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[1] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },52,25,{ 1,30 },{ 8, 8 }, 3,0,5,0,0.0f, 0.0f,0.03f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[2] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[3] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[4] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		break;

		//ボス===================================================================================================================================//
	case ENEMYMODE_LBOSS_EASY_1://ウェポンの切り替え
		g_aEnemy[nCntEnemy].mode = ENEMYMODE_NONE;
		g_aEnemy[nCntEnemy].movestate = ENEMYMOVE_NONE;
		g_aEnemy[nCntEnemy].nHP = 1500;
		g_aEnemy[nCntEnemy].attack[0] = ATTACK{ BULLETSTATE_BACK, BULLETAIM_NORMAL,{ 0,0 },0,41,{ 250,1 },{ 20,20 }, 16,0,1,0,0.009f, 0.0f,0.03f, -0.008f, -0.5f, 50.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[1] = ATTACK{ BULLETSTATE_BACK, BULLETAIM_NORMAL,{ 0,0 },0,36,{ 250,1 },{ 20,20 }, 16,0,1,0,0.01f, 0.0f,-0.03f, 0.008f, 0.5f, 50.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[2] = ATTACK{ BULLETSTATE_NONE, BULLETAIM_STOP,{ 0,0 },0,117,{ 160,91 },{ 7,18 }, 8,0,5,100,0.0f, 0.3f,0.03f, 0.0f, 0.3f,0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[3] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[4] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		break;
		//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ		休憩, 連射は最低1
	case ENEMYMODE_LBOSS_NORMAL_1://ウェポンの切り替え
		g_aEnemy[nCntEnemy].mode = ENEMYMODE_NONE;
		g_aEnemy[nCntEnemy].movestate = ENEMYMOVE_NONE;
		g_aEnemy[nCntEnemy].nHP = 1300;
		g_aEnemy[nCntEnemy].attack[0] = ATTACK{ BULLETSTATE_ACCELERATOR, BULLETAIM_NORMAL,{ 0,0 },0,40,{ 180,0 },{ 20,20 }, 32,0,1,0,0.002f, 0.0f,0.01f, 0.0f, 0.1f, -10.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[1] = ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },0,20,{ 1,10000 },{ 9, 10 }, 3,0,3,0,0.0f, 0.0f,0.03f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[2] = ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },10,10,{ 1,10000 },{ 8,8 }, 1,0,45,0,0.0f, 0.0f,0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[3] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[4] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		break;

	case ENEMYMODE_LBOSS_HARD_1://ウェポンの切り替え
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_TIME1;
		g_aEnemy[nCntEnemy].mode = ENEMYMODE_NONE;
		g_aEnemy[nCntEnemy].movestate = ENEMYMOVE_NONE;
		g_aEnemy[nCntEnemy].nHP = 1500;
		g_aEnemy[nCntEnemy].attack[0] = ATTACK{ BULLETSTATE_STRAIGHT_B, BULLETAIM_NORMAL,{ 0,0 },0,52,{ 1,600000 },{ 8,8 }, 8,0,3,0,0.01f, 0.0f,0.005f, 0.3f, -1.0f, 50.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[1] = ATTACK{ BULLETSTATE_STRAIGHT, BULLETAIM_NORMAL,{ 0,0 },0,58,{ 1,600000 },{ 8,8 }, 8,0,3,0,0.01f, 0.0f,0.005f, 0.3f, 0.5f, 50.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[2] = ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },4,46,{ 200,100 },{ 12,12 }, 1,0,1,0,0.01f, 0.0f,0.03f, 0.0f, 0.5f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[3] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_STOP,{ 0,0 },0,39,{ 300,1 },{ 6,6 }, 64,0,1,150,0.0f, 0.0f,0.05f, 0.0f, 0.35f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[4] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,20,{ 1,10000 },{ 9, 10 }, 3,0,3,0,0.0f, 0.0f,0.03f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f };
		break;

	case ENEMYMODE_LBOSS_LUNATIC_1://ウェポンの切り替え
		g_aEnemy[nCntEnemy].mode = ENEMYMODE_NONE;
		g_aEnemy[nCntEnemy].movestate = ENEMYMOVE_NONE;
		g_aEnemy[nCntEnemy].nHP = 1250;
		g_aEnemy[nCntEnemy].attack[0] = ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },0,51,{ 1,30000 },{ 8,8 }, 3,0,6,0,0.001f, 0.0f,0.03f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[1] = ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },0,61,{ 1,30000 },{ 8,8 }, 3,0,6,0,0.001f, 0.0f,-0.03f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[2] = ATTACK{ BULLETSTATE_BACK, BULLETAIM_NORMAL,{ 0,0 },0,45,{ 200,31 },{ 50,50 }, 16,0,15,0,0.003f, 0.0f,0.025f, 0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[3] = ATTACK{ BULLETSTATE_BACK, BULLETAIM_STOP,{ 0,0 },0,35,{ 200,31 },{ 15,15 }, 32,0,15,47,0.003f, 0.0f,0.025f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[4] = ATTACK{ BULLETSTATE_BACK, BULLETAIM_STOP,{ 0,0 },0,36,{ 200,31 },{ 15,15 }, 32,0,15,92,0.003f, 0.0f,-0.025f, 0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		break;
		//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
	case ENEMYMODE_LBOSS_EASY_2://ウェポンの切り替え
		g_aEnemy[nCntEnemy].mode = ENEMYMODE_LBOSS_EASY_1;
		g_aEnemy[nCntEnemy].movestate = ENEMYMOVE_MOVE;
		g_aEnemy[nCntEnemy].nHP = 450;
		g_aEnemy[nCntEnemy].attack[0] = ATTACK{ BULLETSTATE_STOP_S, BULLETAIM_NORMAL,{ 0,0 },0,56,{ 60,30 },{ 10, 10 }, 8,0,3,0,10.0f, 0.0f,0.03f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[1] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[2] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[3] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[4] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		break;
		//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
	case ENEMYMODE_LBOSS_NORMAL_2://ウェポンの切り替え
		g_aEnemy[nCntEnemy].mode = ENEMYMODE_LBOSS_NORMAL_1;
		g_aEnemy[nCntEnemy].movestate = ENEMYMOVE_MOVE;
		g_aEnemy[nCntEnemy].nHP = 500;
		g_aEnemy[nCntEnemy].attack[0] = ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },0,56,{ 150,1 },{ 8,8 }, 64,0,1,0,0.0f, 0.0f,0.03f, 0.0f, 0.4f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[1] = ATTACK{ BULLETSTATE_NONE, BULLETAIM_STOP,{ 0,0 },0,52,{ 150,1 },{ 8,8 }, 64,0,1,20,0.0f, 0.0f,0.03f, 0.0f, 0.35f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[2] = ATTACK{ BULLETSTATE_NONE, BULLETAIM_STOP,{ 0,0 },0,62,{ 150,1 },{ 8,8 }, 64,0,1,40,0.0f, 0.0f,0.03f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[3] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[4] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		break;
		//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
	case ENEMYMODE_LBOSS_HARD_2://ウェポンの切り替え
		g_aEnemy[nCntEnemy].mode = ENEMYMODE_LBOSS_HARD_1;
		g_aEnemy[nCntEnemy].movestate = ENEMYMOVE_NONE;
		g_aEnemy[nCntEnemy].nHP = 450;
		g_aEnemy[nCntEnemy].attack[0] = ATTACK{ BULLETSTATE_BACK, BULLETAIM_NORMAL,{ 0,0 },0,51,{ 200,1 },{ 8,8 }, 100,0,1,0,0.0f, 0.0f,0.02f, 0.0f, -0.35f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[1] = ATTACK{ BULLETSTATE_BACK, BULLETAIM_STOP,{ 0,0 },0,53,{ 200,1 },{ 8,8 }, 100,0,1,50,0.0f, 0.01f,0.02f, 0.0f, -0.35f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[2] = ATTACK{ BULLETSTATE_BACK, BULLETAIM_STOP,{ 0,0 },0,55,{ 200,1 },{ 8,8 }, 100,0,1,100,0.0f, 0.02f,0.02f, 0.0f, -0.35f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[3] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[4] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		break;

	case ENEMYMODE_LBOSS_LUNATIC_2://ウェポンの切り替え
		g_aEnemy[nCntEnemy].mode = ENEMYMODE_LBOSS_LUNATIC_1;
		g_aEnemy[nCntEnemy].movestate = ENEMYMOVE_NONE;
		g_aEnemy[nCntEnemy].nHP = 550;
		g_aEnemy[nCntEnemy].attack[0] = ATTACK{ BULLETSTATE_SET_B2, BULLETAIM_STOP,{ 0,0 },54,19,{ 301,1 },{ 25,25 }, 1,0,1,250,0.001f, 0.0f,0.0f, 0.0f, 0.85f, 0.0f, 0.0f, 0.1f };
		g_aEnemy[nCntEnemy].attack[1] = ATTACK{ BULLETSTATE_BACK, BULLETAIM_NORMAL,{ 0,0 },0,46,{ 200,100 },{ 40,40 }, 3,0,20,0,0.01f, 0.0f,0.03f, 0.0f, -0.8f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[2] = ATTACK{ BULLETSTATE_NONE, BULLETAIM_STOP,{ 0,0 },0,51,{ 180,120 },{ 8, 8 }, 3,0,3,130,0.0f, 0.0f,0.03f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[3] = ATTACK{ BULLETSTATE_NONE, BULLETAIM_STOP,{ 0,0 },0,59,{ 180,120 },{ 8, 8 }, 3,0,3,130,0.0f, 0.0f,-0.03f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[4] = ATTACK{ BULLETSTATE_NONE, BULLETAIM_STOP,{ 0,0 },7,0,{ 301,1 },{ 8, 8 }, 8,0,1,250,5.0f, 0.0f,0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.08f };
		break;
		//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
	case ENEMYMODE_LBOSS_EASY_3://ウェポンの切り替え
		g_aEnemy[nCntEnemy].mode = ENEMYMODE_LBOSS_EASY_2;
		g_aEnemy[nCntEnemy].movestate = ENEMYMOVE_NONE;
		g_aEnemy[nCntEnemy].nHP = 1000;
		g_aEnemy[nCntEnemy].attack[0] = ATTACK{ BULLETSTATE_ACCELERATOR, BULLETAIM_NORMAL,{ 0,0 },50,73,{ 1,60000 },{ 6,10 }, 1,0,2,0,0.0015f, 0.0f,0.0f, 0.0f, 0.2f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[1] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[2] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[3] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[4] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		break;
		//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
	case ENEMYMODE_LBOSS_NORMAL_3://ウェポンの切り替え
		g_aEnemy[nCntEnemy].mode = ENEMYMODE_LBOSS_NORMAL_2;
		g_aEnemy[nCntEnemy].movestate = ENEMYMOVE_NONE;
		g_aEnemy[nCntEnemy].nHP = 600;
		g_aEnemy[nCntEnemy].attack[0] = ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },4,120,{ 1,10000 },{ 7,17 }, 2,0,1,0,0.0f, 0.0f,0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[1] = ATTACK{ BULLETSTATE_BACK, BULLETAIM_NORMAL,{ 0,0 },0,36,{ 250,1 },{ 25,25 }, 8,0,1,0,0.01f, 0.0f,0.03f, -0.008f, -0.5f, 50.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[2] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[3] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[4] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		break;
		//{nCntState}, ウェポン, 色, { 休憩, 労働 }, { 長さX, 長さY }, 弾数, 時間0固定, 連射, タイミング, f加速, f角度, f角度+, カーブ, f速さ, 距離		休憩, 連射は最低1
	case ENEMYMODE_LBOSS_HARD_3://ウェポンの切り替え
		g_aEnemy[nCntEnemy].mode = ENEMYMODE_LBOSS_HARD_2;
		g_aEnemy[nCntEnemy].movestate = ENEMYMOVE_NONE;
		g_aEnemy[nCntEnemy].nHP = 600;
		g_aEnemy[nCntEnemy].attack[0] = ATTACK{ BULLETSTATE_NONE, BULLETAIM_NORMAL,{ 0,0 },51,67,{ 100,1 },{ 7,10 }, 64,0,1,0,0.0f, 0.1f,0.0f, 0.0f, 0.25f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[1] = ATTACK{ BULLETSTATE_NONE, BULLETAIM_STOP,{ 0,0 },0,52,{ 100,1 },{ 9,9 }, 32,0,1,50,0.0f, 0.0f,0.0f, 0.0f, 0.3f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[2] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[3] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[4] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		break;

	case ENEMYMODE_LBOSS_LUNATIC_3://ウェポンの切り替え
		g_aEnemy[nCntEnemy].mode = ENEMYMODE_LBOSS_LUNATIC_2;
		g_aEnemy[nCntEnemy].movestate = ENEMYMOVE_MOVE;
		g_aEnemy[nCntEnemy].nHP = 650;
		g_aEnemy[nCntEnemy].attack[0] = ATTACK{ BULLETSTATE_SET_B1, BULLETAIM_NORMAL,{ 0,0 },1,45,{ 300,0 },{ 30,30 }, 3,0,3,0,0.0f, 0.0f,0.0f, 0.0f, 0.8f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[1] = ATTACK{ BULLETSTATE_BACK, BULLETAIM_NORMAL,{ 0,0 },0,51,{ 143,13 },{ 8,8 }, 2,0,1,0,0.0001f, 0.0f,0.03f, 0.0f, -0.4f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[2] = ATTACK{ BULLETSTATE_BACK, BULLETAIM_STOP,{ 0,0 },0,52,{ 143,13 },{ 8,8 }, 2,0,1,20,0.0001f, 0.0f,0.03f, 0.0f, -0.45f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[3] = ATTACK{ BULLETSTATE_BACK, BULLETAIM_STOP,{ 0,0 },0,55,{ 143,13 },{ 8,8 }, 2,0,1,40,0.0001f, 0.0f,0.03f, 0.0f, -0.5f, 0.0f, 0.0f, 0.08f };
		g_aEnemy[nCntEnemy].attack[4] = ATTACK{ BULLETSTATE_NOT, BULLETAIM_NORMAL,{ 0,0 },0,8,{ 60,30 },{ 8,8 }, 8,0,3,0,0.001f, 0.0f,0.03f, -0.005f, 0.3f, 0.0f, 0.0f, 0.08f };
		break;
	}
}
//=============================================================================
// * ボス死亡時雑魚も破壊
//=============================================================================
void DeadEnemy(void)
{
	int nCntEnemy;
	ENEMY *g_aEnemy = GetEnemy();

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].mode == ENEMYMODE_NONE_1 && g_aEnemy[nCntEnemy].bUse == true)
		{//破壊対象なら
			HitEnemy(nCntEnemy, 9999, BULLET_ATKTYPE99);
			BreakBullet(nCntEnemy);
		}
	}
}
//=============================================================================
// * 敵の取得
//=============================================================================
IfBoss *GetBoss(void)
{
	static IfBoss ifboss = { 1, false };

	return &ifboss;
}
//=============================================================================
// 敵の取得
//=============================================================================
ENEMY *GetEnemy(void)
{
	static ENEMY enemy[MAX_ENEMY];//弾の最大数

	return &enemy[0];
}
