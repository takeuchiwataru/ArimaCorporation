//=========================================================================================================//
//
// 自機 処理 [player.cpp]
// Author : Ryou Sugimoto
//
//=========================================================================================================//
#include "main.h"
#include "input.h"
#include "game.h"
#include "sound.h"
#include <stdlib.h>				//ランド用
#include <time.h>				//同じくランド用

//*********************************************************************************************************//
// グローバル変数
//*********************************************************************************************************//
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;	//頂点BUFFERへのポインタ
PLAYER g_Player;
int nSet = 0;
//*********************************************************************************************************//
// マクロ定義
//*********************************************************************************************************//
#define PLAYER_TextureName		"data/TEXTURE/Player001.png"	//TEXTUREの名前
#define VERTEX_NUM				(4)				// 頂点の数
#define PRIMITIVE_NUM			(2)				// プリミティブの数
#define RECAST_TIME				(4)				//弾の発射速度

#define MOVEPOLYGON				(1.0f)			//ポリゴンの進む速さ
#define SLIPPLAYER				(0.12f)
#define WIDETEXTURE				(1.0f)			//TEXTUREの幅
#define HEIGHTTEXTURE			(1.0f)			//TEXTUREの高さ
#define BULLET_SPEED			(2.0f)			//弾のSPEED
#define BULLET0_LENGTHX			(8)				//通常弾X
#define BULLET0_LENGTHY			(8)				//通常弾Y
#define BULLET1_LENGTHX			(10)			//乱射弾X
#define BULLET1_LENGTHY			(13)			//乱射弾Y
#define BULLET2_LENGTHX			(12)			//ゲロビX
#define BULLET2_LENGTHY			(50)			//ゲロビY
#define KNIFE_LENGTHX			(13)			//ナイフX
#define KNIFE_LENGTHY			(80)			//ナイフY
#define GUARD_TIME				(60)			//GUARDの有効時間

//=========================================================================================================//
// * プレイヤーの描画処理1	初期化処理
//=========================================================================================================//
void InitPlayer(void)
{
	srand((unsigned int)time(0));
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	g_Player.pos = D3DXVECTOR3{ POSPLAYERX, POSPLAYERY, 0.0f };		//ポリゴンの中心
	g_Player.move = D3DXVECTOR3{ 0,0,0.0f };	//ポリゴンの移動
	g_Player.nStealth = 255;
	g_Player.nMode = 0;
	g_Player.nGuardTime = 0;
	g_Player.bGuard = false;
	g_Player.bCharge = false;
	g_Player.bKnifeLimit = false;
	g_Player.bUse = false;
	nSet = 0;

	D3DXCreateTextureFromFile(pDevice, PLAYER_TextureName, &g_pTexturePlayer);

	//頂点BUFFERの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_NUM,		//確保するBUFFERのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ
							//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - PLAYER_POSX3, g_Player.pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x + PLAYER_POSX3, g_Player.pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x - PLAYER_POSX3, g_Player.pos.y + PLAYER_POSY3 * 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + PLAYER_POSX3, g_Player.pos.y + PLAYER_POSY3 * 2, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラー設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// A = 透明度
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(WIDETEXTURE, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, HEIGHTTEXTURE);
	pVtx[3].tex = D3DXVECTOR2(WIDETEXTURE, HEIGHTTEXTURE);

	//頂点BUFFERをアンロックする
	g_pVtxBuffPlayer->Unlock();
}
//=========================================================================================================//
// * プレイヤーの描画処理2	終了処理
//=========================================================================================================//
void UninitPlayer(void)
{//テクスチャの破棄
	g_Player.bUse = false;
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}
//=========================================================================================================//
// * プレイヤーの描画処理3	更新処理
//=========================================================================================================//
void UpdatePlayer(void)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ
	static bool g_bChannel;
	static int g_nRecast = 0;
	static int nKnifeTime = 0;
	static int nBoostTime = 0;
	static float g_fChannel;
	int nSubShot;
	static bool bStartShooter = false;
	int nRand;

	float fRun = MOVEPOLYGON;
	float fSlip = SLIPPLAYER;
	int nLate = RECAST_TIME;
	int nCount;

	fSlip = SLIPPLAYER;

	if (g_Player.bUse == true)
	{
		if (g_Player.nGuardTime != GUARD_BREAK && g_Player.bGuard == false)
		{//Guard時間は60Fまで
			g_Player.nGuardTime++;
			if (g_Player.nGuardTime == GUARD_BREAK)
			{//チャージ完了
				PlaySound(SOUND_LABEL_BOTTON000);
				SetExplosion(D3DXVECTOR3{ g_Player.pos.x, g_Player.pos.y + PLAYER_POSY3, 0.0f }, COLLAR{ 255,255,200 }, 100, 100, 10, 1, 2, 1, ANIMATION_NORMAL);
				SetWindow(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y + PLAYER_POSY3, 0.0f), D3DXCOLOR{ 1.0f,1.0f,1.0f, 0.0f }, 32, 32, 20, 0, 0, WINDOWSTATE_GUARDOFF, WINDOWUSE_GUARD);
			}
		}
		else if (g_Player.bGuard == true)
		{//ガード中
			fSlip *= 0.8f;			//滑る！
			g_Player.nGuardTime--;	//ガードカウントDOWN
			if (g_Player.nGuardTime == GUARD_BREAK - GUARD_TIME)
			{//GUARD解除
				g_Player.bGuard = false;	//ガード解ける
				g_Player.nGuardTime = 0;	//初期化
				ChangeGuard(2);				//ガードの見た目削除
			}
		}

		g_nRecast++;	//弾の連射制限カウント用
		GetKeyboardQuick(DIK_A, false);
		if (g_Player.nStealth < 255)
		{
			g_Player.nStealth++;
		}
		if (g_Player.bKnifeLimit == false)
		{
			if (bStartShooter == false)
			{//初期シューターの数
				nSubShot = 0;
				g_fChannel = 0.01f;
				g_bChannel = true;
				for (nCount = 0; nCount < nSubShot; nCount++)
				{
					nRand = rand() % 64;
					SetShooter(D3DXVECTOR3(cosf(2.7f) * (-BULLET_SPEED * 0.5f), cosf(g_fChannel) * (BULLET_SPEED * 0.5f), 0.0f), nRand);
				}
				bStartShooter = true;
			}
			if (g_bChannel == true)
			{//マイナス！ 角度
				g_fChannel -= 0.02f;
				//MessageBox(0, "a", "a", MB_OK);
			}
			else
			{//プラス！　角度
				g_fChannel += 0.02f;
			}
			if (g_fChannel >= 3.14f)
			{//前乱射モード用
				g_fChannel = 3.14f;
				g_bChannel = true;
			}
			else if (g_fChannel <= 2.5f)
			{//前乱射モード用　範囲（幅）1.57が真横
				g_fChannel = 2.5f;
				g_bChannel = false;
			}//弾発射
			if (GetKeyboardTrigger(DIK_Z) == true || GetKeyboardTrigger(DIK_N) == true)
			{//撃ち方(モード)変更
				g_Player.nMode = (g_Player.nMode + 1) % 3;
			}
			if (GetKeyboardTrigger(DIK_X) == true || GetKeyboardTrigger(DIK_M) == true)
			{//ペットの整列　3パターン
				nSet = (nSet + 1) % 3;
			}

			if (g_Player.nMode == 2)
			{//ゲロビMODEなら攻撃速度MAX
				nLate = 2;
			}
			else
			{//通常なら
				nLate = RECAST_TIME;
			}
			if (g_nRecast % nLate == 0 && g_Player.bGuard == false && nKnifeTime == 0)
			{//弾発射
				PlaySound(SOUND_LABEL_SHOT000);
				if (g_Player.nMode == 2)
				{//MODE3なら
					SetBullet(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - 30.0f, 0.0f), D3DX_PI, BULLET2_LENGTHX, BULLET2_LENGTHY, 3, BULLET_ATKTYPE1, BULLET_SPEED * 3, BULLETTYPE_PLAYER, 0, BULLETSTATE_NONE, 0.0f, 0.0f, 0, 0);
					g_Player.move.y += cosf(0.0f) * fRun * 0.15f;	//反動
					fSlip *= 0.8f;
				}
				else if (g_Player.nMode == 1)
				{//MODE2なら
					SetBullet(g_Player.pos, D3DX_PI, BULLET0_LENGTHX, BULLET0_LENGTHY, 67, BULLET_ATKTYPE0, BULLET_SPEED, BULLETTYPE_PLAYER, 0, BULLETSTATE_NONE, 0.0f, 0.0f, 0, 0);
					SetBullet(g_Player.pos, g_fChannel, BULLET1_LENGTHX, BULLET1_LENGTHY, 19, BULLET_ATKTYPE0, BULLET_SPEED * 0.8f, BULLETTYPE_PLAYER, 0, BULLETSTATE_NONE, 0.0f, 0.0f, 0, 0);
					SetBullet(g_Player.pos, -g_fChannel, BULLET1_LENGTHX, BULLET1_LENGTHY, 19, BULLET_ATKTYPE0, BULLET_SPEED * 0.8f, BULLETTYPE_PLAYER, 0, BULLETSTATE_NONE, 0.0f, 0.0f, 0, 0);
				}
				else
				{//MODE1なら
					SetBullet(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y, 0.0f), D3DX_PI, BULLET0_LENGTHX, BULLET0_LENGTHY, 51, BULLET_ATKTYPE0, BULLET_SPEED, BULLETTYPE_PLAYER, 0, BULLETSTATE_NONE, 0.0f, 0.0f, 0, 0);
					SetBullet(D3DXVECTOR3(g_Player.pos.x + PLAYER_POSX3 + BULLET0_LENGTHX, g_Player.pos.y + PLAYER_POSY3, 0.0f), D3DX_PI, BULLET0_LENGTHX, BULLET0_LENGTHY, 51, BULLET_ATKTYPE0, BULLET_SPEED, BULLETTYPE_PLAYER, 0, BULLETSTATE_NONE, 0.0f, 0.0f, 0, 0);
					SetBullet(D3DXVECTOR3(g_Player.pos.x - PLAYER_POSX3 - BULLET0_LENGTHX, g_Player.pos.y + PLAYER_POSY3, 0.0f), D3DX_PI, BULLET0_LENGTHX, BULLET0_LENGTHY, 51, BULLET_ATKTYPE0, BULLET_SPEED, BULLETTYPE_PLAYER, 0, BULLETSTATE_NONE, 0.0f, 0.0f, 0, 0);
				}
			}
		}
		if (g_Player.nGuardTime == GUARD_BREAK || g_Player.bGuard == true)
		{//ガードに入れるまでのF
			if (GetKeyboardTrigger(DIK_LCONTROL) == true || GetKeyboardTrigger(DIK_RCONTROL) == true && g_Player.bGuard == false)
			{//Guard状態
				g_Player.bGuard = true;		//ガード状態へ
				ChangeGuard(1);		//GUARD発動 (見た目)
			}
		}
		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{//必殺のナイフ発動！連打してね_(:3」∠)_
			PlaySound(SOUND_LABEL_KNIFE000);
			SetBullet(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y + PLAYER_POSY3, 0.0f), D3DX_PI, KNIFE_LENGTHX, KNIFE_LENGTHY, 0, BULLET_ATKTYPE99, 0.0f, BULLETTYPE_PLAYER, 0, BULLETSTATE_NONE, 0.0f, 0.0f, 0, 0);
			nKnifeTime = 10;
		}
		else if (nKnifeTime > 0)
		{
			nKnifeTime--;
			fRun = fRun / 5.0f;
		}

		if (g_Player.nMode != 2 && g_Player.bGuard == false)
		{//Mode3なら動けない &&　ナイフもできない
			if (GetKeyboardQuick(DIK_D, true) == true || GetKeyboardQuick(DIK_RIGHT, true) == true ||
				GetKeyboardQuick(DIK_A, true) == true || GetKeyboardQuick(DIK_LEFT, true) == true ||
				GetKeyboardQuick(DIK_W, true) == true || GetKeyboardQuick(DIK_UP, true) == true ||
				GetKeyboardQuick(DIK_S, true) == true || GetKeyboardQuick(DIK_DOWN, true) == true)
			{//ステップ　キャンセルできるのは仕様だよ！
				nBoostTime = 6;	//実質5F
				if (g_Player.nStealth > 250)
				{//ステップ時5Fの無敵時間
					g_Player.nStealth = 250;
				}
			}
			if (nBoostTime > 0)
			{
				fRun *= 8;
				nBoostTime--;
				if (nBoostTime == 0)
				{
					fRun = MOVEPOLYGON;
					g_Player.move *= 0.3f;
				}
			}

			if (GetKeyboardPress(DIK_LSHIFT) == true || GetKeyboardPress(DIK_RSHIFT) == true)
			{//スロー
				fRun = fRun / 2.5f;
			}

			if (g_Player.bGuard == false)
			{
				if (GetKeyboardPress(DIK_D) == true || GetKeyboardPress(DIK_RIGHT) == true)
				{//D, →で右移動
					if (GetKeyboardPress(DIK_W) == true || GetKeyboardPress(DIK_UP) == true)
					{//斜めに切り替え　右上
						g_Player.move.x += sinf(D3DX_PI * 0.75f) * fRun;
						g_Player.move.y += cosf(D3DX_PI * 0.75f) * fRun;
					}
					else if (GetKeyboardPress(DIK_S) == true || GetKeyboardPress(DIK_DOWN) == true)
					{//斜めに切り替え　右↓
						g_Player.move.x += sinf(D3DX_PI / 4) * fRun;
						g_Player.move.y += cosf(D3DX_PI / 4) * fRun;
					}
					else
					{//分岐なし　右
						g_Player.move.x += sinf(D3DX_PI / 2) * fRun;
					}
				}
				else if (GetKeyboardPress(DIK_A) == true || GetKeyboardPress(DIK_LEFT) == true)
				{//A, ←で左移動
					if (GetKeyboardPress(DIK_W) == true || GetKeyboardPress(DIK_UP) == true)
					{//斜めに切り替え　左上
						g_Player.move.x += sinf(D3DX_PI * -0.75f) * fRun;
						g_Player.move.y += cosf(D3DX_PI * -0.75f) * fRun;
					}
					else if (GetKeyboardPress(DIK_S) == true || GetKeyboardPress(DIK_DOWN) == true)
					{//斜めに切り替え　左↓
						g_Player.move.x += sinf(D3DX_PI / -4) * fRun;
						g_Player.move.y += cosf(D3DX_PI / -4) * fRun;
					}
					else
					{//分岐なし　左
						g_Player.move.x += sinf(D3DX_PI / -2) * fRun;
					}
				}
				else if (GetKeyboardPress(DIK_W) == true || GetKeyboardPress(DIK_UP) == true)
				{//上移動
					g_Player.move.y += cosf(0.0f) * -fRun;
				}
				else if (GetKeyboardPress(DIK_S) == true || GetKeyboardPress(DIK_DOWN) == true)
				{//下移動
					g_Player.move.y += cosf(0.0f) * fRun;
				}
			}//Guard括弧
		}//MODE2括弧
		g_Player.pos += g_Player.move;//自機の移動
		if (g_Player.pos.y <= 0)
		{//上が出ていれば戻す
			g_Player.pos.y = 0;
		}
		if (g_Player.pos.y + (PLAYER_POSY3 * 2) >= SCREEN_HEIGHT)
		{//同じく下
			g_Player.pos.y = SCREEN_HEIGHT - (PLAYER_POSY3 * 2);
		}
		if (g_Player.pos.x + PLAYER_POSX3 >= SCREEN_GAMEX)
		{//同じく右
			g_Player.pos.x = SCREEN_GAMEX - PLAYER_POSX3;
		}
		if (g_Player.pos.x - PLAYER_POSX3 <= 0)
		{//同じく左
			g_Player.pos.x = PLAYER_POSX3;
		}

		UpdateShooter(g_Player.pos, nSet);

		//頂点BUFFERをロックし、頂点情報へのポインタを取得
		g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);
		//画像移動
		pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - 180.0f, g_Player.pos.y - 180.0f + 20.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x + 180.0f, g_Player.pos.y - 180.0f + 20.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x - 180.0f, g_Player.pos.y + 180.0f + 20.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + 180.0f, g_Player.pos.y + 180.0f + 20.0f, 0.0f);

		//移動量低下(空気抵抗等のつもり)
		g_Player.move.x += (0.0f - g_Player.move.x) * fSlip;
		g_Player.move.y += (0.0f - g_Player.move.y) * fSlip;

		if (g_Player.nMode == 2)
		{//ゲロビ撃ってるときは赤！
			pVtx[0].col = D3DCOLOR_RGBA(100, 100, 100, g_Player.nStealth);	// A = 透明度
			pVtx[1].col = D3DCOLOR_RGBA(100, 100, 100, g_Player.nStealth);
			pVtx[2].col = D3DCOLOR_RGBA(100, 100, 100, g_Player.nStealth);
			pVtx[3].col = D3DCOLOR_RGBA(100, 100, 100, g_Player.nStealth);
		}
		else
		{//基本マテリアル白
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_Player.nStealth);	// A = 透明度
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_Player.nStealth);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_Player.nStealth);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_Player.nStealth);
		}
		//頂点BUFFERをアンロックする
		g_pVtxBuffPlayer->Unlock();
	}
}
//=========================================================================================================//
// * プレイヤーの描画処理4	描画処理
//=========================================================================================================//
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスを取得する
	pDevice = GetDevice();

	//頂点BUFFERをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);	// 頂点フォーマットの設定

									//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePlayer);

	//ポリゴンの描画
	if (g_Player.bUse == true)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_NUM);
	}

}
//=========================================================================================================//
// * プレイヤーの情報代入処理
//=========================================================================================================//
PLAYER *GetPlayer(void)
{
	return &g_Player;
}