//=========================================================================================================//
//
// [弾発射]->弾移動処理 [bullet.cpp]
// Author : Ryou Sugimoto
//
//=========================================================================================================//
#include "main.h"
#include "game.h"				//爆発を呼ぶため
#include "enemy.h"				//敵の場所確認のため
#include "sound.h"

//*********************************************************************************************************//
// マクロ定義
//*********************************************************************************************************//
#define BULLET_VERTEX_NUM			(4)					// 頂点の数
#define BULLET_PRIMITIVE_NUM		(2)					// プリミティブの数
#define BULLET_TextureName		"data/TEXTURE/danmaku002.png"
#define BULLET_TextureName1		"data/TEXTURE/KNIFE000.png"
#define BULLET_MAX_Texture			(2)					//画像の種類
#define	MAX_BULLET			(3840)				// 弾の最大数
#define HIT_PLUS			(3.0f)				//敵Hit判定の弾の大きさの倍率
#define HIT_SAFE			(0.55f)				//自機Hit判定の弾の大きさの倍率
#define KNIFE_STEEP			(D3DX_PI * -0.55f)	//ナイフの初期角度
#define KNIFE_STEEPENEMY	(D3DX_PI * -0.45f)	//ナイフの初期角度
#define BULLET_SLIP			(0.15f)				//弾の滑り
#define BULLET_DELETE		(200)
#define BULLET_DELETE1		(1)

//*********************************************************************************************************//
// グローバル変数
//*********************************************************************************************************//
LPDIRECT3DTEXTURE9		g_pTextureBullet[2] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	// 頂点バッファへのポインタ		

//=========================================================================================================//
// * 背景の描画処理1	初期化処理
//=========================================================================================================//
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	int nCntBullet;

	Bullet *g_aBullet;
	g_aBullet = GetBullet();

	// 弾の情報の初期化
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//g_aBullet[nCntBullet].pos = { 0.0f, 0.0f, 0.0f };
		//g_aBullet[nCntBullet].steep = 0.0f;
		//g_aBullet[nCntBullet].nType = 0;
		//g_aBullet[nCntBullet].nAtkType = 0;
		//g_aBullet[nCntBullet].nShot = 0;
		//g_aBullet[nCntBullet].nLengthX = 0;
		//g_aBullet[nCntBullet].nLengthY = 0;
		g_aBullet[nCntBullet].bUse = false;
	}

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, BULLET_TextureName, &g_pTextureBullet[0]);
	D3DXCreateTextureFromFile(pDevice, BULLET_TextureName1, &g_pTextureBullet[1]);

	//頂点BUFFERの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * BULLET_VERTEX_NUM * MAX_BULLET,		//確保するBUFFERのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ
							//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//頂点座標の設定
		/*pVtx[nCntBullet * 4].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(0, 0, 0.0f);*/

		pVtx[nCntBullet * 4].rhw = 1.0f;
		pVtx[nCntBullet * 4 + 1].rhw = 1.0f;
		pVtx[nCntBullet * 4 + 2].rhw = 1.0f;
		pVtx[nCntBullet * 4 + 3].rhw = 1.0f;

		//カラー設定
		pVtx[nCntBullet * 4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// A = 透明度
		pVtx[nCntBullet * 4 + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[nCntBullet * 4 + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[nCntBullet * 4 + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標設定
		/*pVtx[nCntBullet * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nCntBullet * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[nCntBullet * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[nCntBullet * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);*/
	}

	//頂点BUFFERをアンロックする
	g_pVtxBuffBullet->Unlock();
}
//=========================================================================================================//
// * 背景の描画処理2	終了処理
//=========================================================================================================//
void UninitBullet(void)
{//テクスチャの破棄
	int nCntTexture;

	for (nCntTexture = 0; nCntTexture < BULLET_MAX_Texture; nCntTexture++)
	{
		if (g_pTextureBullet[nCntTexture] != NULL)
		{
			g_pTextureBullet[nCntTexture]->Release();
			g_pTextureBullet[nCntTexture] = NULL;
		}
	}
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}
//=========================================================================================================//
// * 背景の描画処理3	更新処理
//=========================================================================================================//
void UpdateBullet(void)
{
	static int nCntTime = 100;
	static int nCntGuard = 100;
	int nCntBullet;			//弾数える～
	int nCntEnemy;			//敵数えるんじゃ～
	int nScore;

	bool bHit;
	VERTEX_2D *pVtx;		//頂点情報へのポインタ
	ENEMY *pEnemy = GetEnemy();

	Bullet *g_aBullet;
	PLAYER *pPlayer;
	g_aBullet = GetBullet();
	pPlayer = GetPlayer();
	nCntTime++;
	nCntGuard++;

	//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{

		if (g_aBullet[nCntBullet].bUse == true)
		{//弾を前に飛ばす
		 //MessageBox(0, "本当すか？", "終了", MB_YESNO);
			g_aBullet[nCntBullet].nCntState[2]++;
			g_aBullet[nCntBullet].fSetSteep[0] += g_aBullet[nCntBullet].fSetSteep[1];
			switch (g_aBullet[nCntBullet].state)
			{
			case BULLETSTATE_NONE:			//普通に飛ぶ
				g_aBullet[nCntBullet].move += D3DXVECTOR3(sinf(g_aBullet[nCntBullet].steep), cosf(g_aBullet[nCntBullet].steep), 0.0f) * g_aBullet[nCntBullet].fSpeed;
				break;
			case BULLETSTATE_START:			//開始時加速
				g_aBullet[nCntBullet].move += D3DXVECTOR3(sinf(g_aBullet[nCntBullet].steep), cosf(g_aBullet[nCntBullet].steep), 0.0f) * (g_aBullet[nCntBullet].fSpeed + g_aBullet[nCntBullet].fSpdPlus);
				g_aBullet[nCntBullet].state = BULLETSTATE_NONE;
				break;
			case BULLETSTATE_ACCELERATOR:	//加速し続ける
				g_aBullet[nCntBullet].fSpeed += g_aBullet[nCntBullet].fSpdPlus;
				g_aBullet[nCntBullet].move += D3DXVECTOR3(sinf(g_aBullet[nCntBullet].steep), cosf(g_aBullet[nCntBullet].steep), 0.0f) * g_aBullet[nCntBullet].fSpeed;
				break;
			case BULLETSTATE_STOP_S:			//開始時止まる
				g_aBullet[nCntBullet].nCntState[0]--;
				if (g_aBullet[nCntBullet].nCntState[0] < 1) { g_aBullet[nCntBullet].state = BULLETSTATE_START; }
				break;
			case BULLETSTATE_STOP_A:			//開始時止まる
				g_aBullet[nCntBullet].nCntState[0]--;
				if (g_aBullet[nCntBullet].nCntState[0] < 1) { g_aBullet[nCntBullet].state = BULLETSTATE_ACCELERATOR; }
				break;
			case BULLETSTATE_STOP_B:			//開始時止まる
				g_aBullet[nCntBullet].nCntState[0]--;
				if (g_aBullet[nCntBullet].nCntState[0] < 1) { g_aBullet[nCntBullet].state = BULLETSTATE_BACK; }
				break;
			case BULLETSTATE_BACK:				//-になっているスピードを制限付きで+に戻していく
				if (g_aBullet[nCntBullet].fSpeed < 0.5f) { g_aBullet[nCntBullet].fSpeed += g_aBullet[nCntBullet].fSpdPlus; }
				g_aBullet[nCntBullet].move += D3DXVECTOR3(sinf(g_aBullet[nCntBullet].steep), cosf(g_aBullet[nCntBullet].steep), 0.0f) * g_aBullet[nCntBullet].fSpeed;
				break;
			case BULLETSTATE_STRAIGHT_B:
				if (g_aBullet[nCntBullet].fSpeed < 0.5f) { g_aBullet[nCntBullet].fSpeed += g_aBullet[nCntBullet].fSpdPlus; }
			case BULLETSTATE_STRAIGHT:
				g_aBullet[nCntBullet].fCurve *= 0.95f;
				g_aBullet[nCntBullet].move += D3DXVECTOR3(sinf(g_aBullet[nCntBullet].steep), cosf(g_aBullet[nCntBullet].steep), 0.0f) * g_aBullet[nCntBullet].fSpeed;
				break;
			case BULLETSTATE_DELETE:		//消えていく
				g_aBullet[nCntBullet].move += D3DXVECTOR3(sinf(g_aBullet[nCntBullet].steep), cosf(g_aBullet[nCntBullet].steep), 0.0f) * g_aBullet[nCntBullet].fSpeed;
			case BULLETSTATE_FLASH:			//その場で消える
				g_aBullet[nCntBullet].fcolA -= 0.03f;
				pVtx[nCntBullet * 4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aBullet[nCntBullet].fcolA);	// A = 透明度
				pVtx[nCntBullet * 4 + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aBullet[nCntBullet].fcolA);
				pVtx[nCntBullet * 4 + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aBullet[nCntBullet].fcolA);
				pVtx[nCntBullet * 4 + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aBullet[nCntBullet].fcolA);
				if (g_aBullet[nCntBullet].fcolA <= 0.0f) { g_aBullet[nCntBullet].bUse = false; }
				break;
			case BULLETSTATE_SET_A:			//ばらつく球を置く
				g_aBullet[nCntBullet].move += D3DXVECTOR3(sinf(g_aBullet[nCntBullet].steep), cosf(g_aBullet[nCntBullet].steep), 0.0f) * g_aBullet[nCntBullet].fSpeed;
				if (g_aBullet[nCntBullet].nCntState[2] % 2 == 0)
				{
					SetAttack(0, 2, 8, 8, g_aBullet[nCntBullet].nType % 16 + 16, g_aBullet[nCntBullet].nCntEnemy, g_aBullet[nCntBullet].nCntState[1], g_aBullet[nCntBullet].fSetSteep[0], 0.4f, 0.0001f, -0.005f, g_aBullet[nCntBullet].pos, 30.0f, BULLETSTATE_STOP_A);
				}
				break;
			case BULLETSTATE_SET_A1:			//ばらつく球を置く
				g_aBullet[nCntBullet].move += D3DXVECTOR3(sinf(g_aBullet[nCntBullet].steep), cosf(g_aBullet[nCntBullet].steep), 0.0f) * g_aBullet[nCntBullet].fSpeed;
				if (g_aBullet[nCntBullet].nCntState[2] % 10 == 0)
				{
					SetAttack(0, 2, 10, 10, g_aBullet[nCntBullet].nType % 16 + 16, g_aBullet[nCntBullet].nCntEnemy, g_aBullet[nCntBullet].nCntState[1], g_aBullet[nCntBullet].fSetSteep[0], 0.4f, 0.0001f, -0.005f, g_aBullet[nCntBullet].pos, 30.0f, BULLETSTATE_STOP_A);
				}
				break;
			case BULLETSTATE_SET_B:			//BACKの弾を置く
				g_aBullet[nCntBullet].move += D3DXVECTOR3(sinf(g_aBullet[nCntBullet].steep), cosf(g_aBullet[nCntBullet].steep), 0.0f) * g_aBullet[nCntBullet].fSpeed;
				if (g_aBullet[nCntBullet].nCntState[2] % 3 == 0)
				{
					SetAttack(0, 5, 8, 8, g_aBullet[nCntBullet].nType % 16 + 48, g_aBullet[nCntBullet].nCntEnemy, g_aBullet[nCntBullet].nCntState[1], g_aBullet[nCntBullet].fSetSteep[0], -0.5f, 0.005f, 0.0f, g_aBullet[nCntBullet].pos, 30.0f, BULLETSTATE_STOP_B);
				}
				break;
			case BULLETSTATE_SET_B1:		//BACKの弾を置く
				g_aBullet[nCntBullet].move += D3DXVECTOR3(sinf(g_aBullet[nCntBullet].steep), cosf(g_aBullet[nCntBullet].steep), 0.0f) * g_aBullet[nCntBullet].fSpeed;
				if (g_aBullet[nCntBullet].nCntState[2] % 10 == 0)
				{
					SetAttack(0, 5, 8, 8, g_aBullet[nCntBullet].nType % 16 + 48, g_aBullet[nCntBullet].nCntEnemy, g_aBullet[nCntBullet].nCntState[1], g_aBullet[nCntBullet].fSetSteep[0], -0.5f, 0.005f, 0.0f, g_aBullet[nCntBullet].pos, 30.0f, BULLETSTATE_STOP_B);
				}
				break;
			case BULLETSTATE_SET_B2:		//BACKの弾を置く
				g_aBullet[nCntBullet].move += D3DXVECTOR3(sinf(g_aBullet[nCntBullet].steep), cosf(g_aBullet[nCntBullet].steep), 0.0f) * g_aBullet[nCntBullet].fSpeed;
				if (g_aBullet[nCntBullet].nCntState[2] % 30 == 0)
				{
					SetAttack(0, 5, 8, 8, g_aBullet[nCntBullet].nType % 16 + 48, g_aBullet[nCntBullet].nCntEnemy, g_aBullet[nCntBullet].nCntState[1], g_aBullet[nCntBullet].fSetSteep[0], -0.5f, 0.005f, 0.0f, g_aBullet[nCntBullet].pos, 30.0f, BULLETSTATE_STOP_B);
				}
				break;
			}
			g_aBullet[nCntBullet].steep += g_aBullet[nCntBullet].fCurve;

			if (g_aBullet[nCntBullet].nAtkType >= BULLET_ATKTYPE99)
			{//ナイフなら
				g_aBullet[nCntBullet].nAtkType++;
				if (g_aBullet[nCntBullet].nAtkType > KNIFE_TIME)
				{//ナイフ時間切れ->false
					g_aBullet[nCntBullet].bUse = false;
				}
				if (g_aBullet[nCntBullet].nShot == BULLETTYPE_PLAYER)
				{
					g_aBullet[nCntBullet].steep -= 0.471f;
					pVtx[nCntBullet * 4].pos = D3DXVECTOR3(pPlayer[0].pos.x + (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY) + (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX), pPlayer[0].pos.y + PLAYER_POSY3 - (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX) + (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY), 0.0f);
					pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(pPlayer[0].pos.x + (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY) - (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX), pPlayer[0].pos.y + PLAYER_POSY3 + (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX) + (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY), 0.0f);
					pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(pPlayer[0].pos.x - (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY) + (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX), pPlayer[0].pos.y + PLAYER_POSY3 - (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX) - (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY), 0.0f);
					pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(pPlayer[0].pos.x - (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY) - (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX), pPlayer[0].pos.y + PLAYER_POSY3 + (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX) - (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY), 0.0f);
				}
				else
				{
					g_aBullet[nCntBullet].steep += 0.471f;
					pVtx[nCntBullet * 4].pos = D3DXVECTOR3(pEnemy[g_aBullet[nCntBullet].nCntEnemy].pos.x + (sinf(g_aBullet[nCntBullet].steep + pEnemy[g_aBullet[nCntBullet].nCntEnemy].fsteep) * g_aBullet[nCntBullet].nLengthY) + (cosf(g_aBullet[nCntBullet].steep + pEnemy[g_aBullet[nCntBullet].nCntEnemy].fsteep) * g_aBullet[nCntBullet].nLengthX), pEnemy[g_aBullet[nCntBullet].nCntEnemy].pos.y - (sinf(g_aBullet[nCntBullet].steep + pEnemy[g_aBullet[nCntBullet].nCntEnemy].fsteep) * g_aBullet[nCntBullet].nLengthX) + (cosf(g_aBullet[nCntBullet].steep + pEnemy[g_aBullet[nCntBullet].nCntEnemy].fsteep) * g_aBullet[nCntBullet].nLengthY), 0.0f);
					pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(pEnemy[g_aBullet[nCntBullet].nCntEnemy].pos.x + (sinf(g_aBullet[nCntBullet].steep + pEnemy[g_aBullet[nCntBullet].nCntEnemy].fsteep) * g_aBullet[nCntBullet].nLengthY) - (cosf(g_aBullet[nCntBullet].steep + pEnemy[g_aBullet[nCntBullet].nCntEnemy].fsteep) * g_aBullet[nCntBullet].nLengthX), pEnemy[g_aBullet[nCntBullet].nCntEnemy].pos.y + (sinf(g_aBullet[nCntBullet].steep + pEnemy[g_aBullet[nCntBullet].nCntEnemy].fsteep) * g_aBullet[nCntBullet].nLengthX) + (cosf(g_aBullet[nCntBullet].steep + pEnemy[g_aBullet[nCntBullet].nCntEnemy].fsteep) * g_aBullet[nCntBullet].nLengthY), 0.0f);
					pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(pEnemy[g_aBullet[nCntBullet].nCntEnemy].pos.x - (sinf(g_aBullet[nCntBullet].steep + pEnemy[g_aBullet[nCntBullet].nCntEnemy].fsteep) * g_aBullet[nCntBullet].nLengthY) + (cosf(g_aBullet[nCntBullet].steep + pEnemy[g_aBullet[nCntBullet].nCntEnemy].fsteep) * g_aBullet[nCntBullet].nLengthX), pEnemy[g_aBullet[nCntBullet].nCntEnemy].pos.y - (sinf(g_aBullet[nCntBullet].steep + pEnemy[g_aBullet[nCntBullet].nCntEnemy].fsteep) * g_aBullet[nCntBullet].nLengthX) - (cosf(g_aBullet[nCntBullet].steep + pEnemy[g_aBullet[nCntBullet].nCntEnemy].fsteep) * g_aBullet[nCntBullet].nLengthY), 0.0f);
					pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(pEnemy[g_aBullet[nCntBullet].nCntEnemy].pos.x - (sinf(g_aBullet[nCntBullet].steep + pEnemy[g_aBullet[nCntBullet].nCntEnemy].fsteep) * g_aBullet[nCntBullet].nLengthY) - (cosf(g_aBullet[nCntBullet].steep + pEnemy[g_aBullet[nCntBullet].nCntEnemy].fsteep) * g_aBullet[nCntBullet].nLengthX), pEnemy[g_aBullet[nCntBullet].nCntEnemy].pos.y + (sinf(g_aBullet[nCntBullet].steep + pEnemy[g_aBullet[nCntBullet].nCntEnemy].fsteep) * g_aBullet[nCntBullet].nLengthX) - (cosf(g_aBullet[nCntBullet].steep + pEnemy[g_aBullet[nCntBullet].nCntEnemy].fsteep) * g_aBullet[nCntBullet].nLengthY), 0.0f);
				}
			}
			else
			{//通常弾なら向いてる方向に飛ぶ
				g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;
				pVtx[nCntBullet * 4].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY) + (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX), g_aBullet[nCntBullet].pos.y - (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX) + (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY), 0.0f);
				pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY) - (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX), g_aBullet[nCntBullet].pos.y + (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX) + (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY), 0.0f);
				pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY) + (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX), g_aBullet[nCntBullet].pos.y - (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX) - (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY), 0.0f);
				pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY) - (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX), g_aBullet[nCntBullet].pos.y + (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX) - (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY), 0.0f);
				//移動量低下(空気抵抗等のつもり)
				g_aBullet[nCntBullet].move.x += (0.0f - g_aBullet[nCntBullet].move.x) * BULLET_SLIP;
				g_aBullet[nCntBullet].move.y += (0.0f - g_aBullet[nCntBullet].move.y) * BULLET_SLIP;
			}

			if (g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].nLengthY <= -BULLET_DELETE || g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].nLengthY >= SCREEN_HEIGHT + BULLET_DELETE || g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].nLengthX <= -BULLET_DELETE || g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].nLengthX >= SCREEN_GAMEX + BULLET_DELETE)
			{//画面外+200の弾を消す
				g_aBullet[nCntBullet].bUse = false;
			}
			if (g_aBullet[nCntBullet].state >= BULLETSTATE_SET_A)
			{//セットする弾の場合 && その球を出した敵が攻撃可能状態なら
				if (g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].nLengthY <= -BULLET_DELETE1 || g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].nLengthY >= SCREEN_HEIGHT + BULLET_DELETE1 || g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].nLengthX <= -BULLET_DELETE1 || g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].nLengthX >= SCREEN_GAMEX + BULLET_DELETE1)
				{//画面外になったら
					StartBullet(g_aBullet[nCntBullet].nCntEnemy, g_aBullet[nCntBullet].nCntState[1]);
					g_aBullet[nCntBullet].bUse = false;
					if (nCntTime > 30)
					{
						PlaySound(SOUND_LABEL_KIRA);
						nCntTime = 0;
					}
				}
			}

			if (g_aBullet[nCntBullet].nShot == BULLETTYPE_PLAYER)
			{//自機が撃った弾なら
				pEnemy = GetEnemy();
				for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
				{//敵の数だけ回る
					if (pEnemy[nCntEnemy].bUse == true && pEnemy[nCntEnemy].state != ENEMYSTATE_TIME)
					{//敵がいる！
						bHit = HitAim(g_aBullet[nCntBullet], pEnemy[nCntEnemy].pos, pEnemy[nCntEnemy].nLength, HIT_PLUS);
						if (bHit == true)
						{//弾が当たってる
							nScore = 10;
							if (g_aBullet[nCntBullet].nAtkType == BULLET_ATKTYPE0)
							{//通常弾
								ExplosionEffect(g_aBullet[nCntBullet].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR{ 1.0f, 0.98f, 0.04f, 0.5f }, 55, 50, 0.1f, 0.5f, 0.1f, 0.5f);
								HitEnemy(nCntEnemy, 1, g_aBullet[nCntBullet].nAtkType);
								g_aBullet[nCntBullet].bUse = false;	//弾の消滅
							}
							else if (g_aBullet[nCntBullet].nAtkType == BULLET_ATKTYPE1)
							{//光線
								ExplosionEffect(pEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR{ 1.0f, 0.98f, 0.04f, 0.5f }, 55, 50, 0.1f, 0.5f, 0.1f, 0.5f);
								HitEnemy(nCntEnemy, 1, g_aBullet[nCntBullet].nAtkType);
								g_aBullet[nCntBullet].nAtkType = BULLET_ATKTYPE50;
								nScore = 30;
							}
							else if(g_aBullet[nCntBullet].nAtkType >= BULLET_ATKTYPE99)
							{//ナイフ
								PlaySound(SOUND_LABEL_HIT_KNIFE000);
								ExplosionEffect(pEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR{ 1.0f, 0.05f, 0.05f, 0.5f }, 55, 50, 0.1f, 0.5f, 0.1f, 0.5f);
								ExplosionEffect(pEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR{ 1.0f, 0.05f, 0.05f, 0.5f }, 55, 50, 0.1f, 0.5f, 0.1f, 0.5f);
								ExplosionEffect(pEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR{ 1.0f, 0.05f, 0.05f, 0.5f }, 55, 50, 0.1f, 0.5f, 0.1f, 0.5f);
								ExplosionEffect(pEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR{ 1.0f, 0.05f, 0.05f, 0.5f }, 55, 50, 0.1f, 0.5f, 0.1f, 0.5f);
								ExplosionEffect(pEnemy[nCntEnemy].pos, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), D3DXCOLOR{ 1.0f, 0.05f, 0.05f, 0.5f }, 55, 50, 0.1f, 0.5f, 0.1f, 0.5f);
								HitEnemy(nCntEnemy, 3, g_aBullet[nCntBullet].nAtkType);			//敵にダメージ
								nScore = 750;
							}
							else if (g_aBullet[nCntBullet].nAtkType == BULLET_ATKTYPE50)
							{//装飾弾
								ExplosionEffect(pEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR{ 1.0f, 0.98f, 0.04f, 0.5f }, 55, 50, 0.1f, 0.5f, 0.1f, 0.5f);
							}
							AddScore(nScore, 0);
						}//ヒット判定
					}//敵存在判定
				}//for括弧
			}
			else
			{//敵が撃った球なら
				if (pPlayer[0].bUse == true && g_aBullet[nCntBullet].fcolA == 1.0f)
				{//自機がいる！
					if (pPlayer[0].bGuard == false && pPlayer[0].nStealth > 254)
					{// ガード無し&&無敵状態でない!
						bHit = HitAim(g_aBullet[nCntBullet], D3DXVECTOR3{ pPlayer[0].pos.x, pPlayer[0].pos.y + PLAYER_POSY3, 0.0f }, 0, HIT_SAFE);
						if (bHit == true)
						{//弾が当たってる
							
							ExplosionEffect(D3DXVECTOR3{ pPlayer[0].pos.x, pPlayer[0].pos.y + PLAYER_POSY3, 0.0f }, pPlayer[0].move, D3DXCOLOR{ 1.0f, 0.98f, 0.04f, 0.8f }, 50, 100, 0.1f, 8.0f, 0.3f, 100.0f);
							ExplosionEffect(D3DXVECTOR3{ pPlayer[0].pos.x, pPlayer[0].pos.y + PLAYER_POSY3, 0.0f }, pPlayer[0].move, D3DXCOLOR{ 1.0f, 0.98f, 0.04f, 0.75f }, 100, 100, 0.5f, 13.0f, 0.1f, 3.0f);
							ExplosionEffect(D3DXVECTOR3{ pPlayer[0].pos.x, pPlayer[0].pos.y + PLAYER_POSY3, 0.0f }, pPlayer[0].move, D3DXCOLOR{ 0.49f, 1.0f, 0.97f, 0.75f }, 100, 100, 0.5f, 13.0f, 0.1f, 3.0f);
							ExplosionEffect(D3DXVECTOR3{ pPlayer[0].pos.x, pPlayer[0].pos.y + PLAYER_POSY3, 0.0f }, pPlayer[0].move, D3DXCOLOR{ 1.0f, 0.05f, 0.05f, 0.75f }, 100, 100, 0.5f, 13.0f, 0.1f, 3.0f);
							ExplosionEffect(D3DXVECTOR3{ pPlayer[0].pos.x, pPlayer[0].pos.y + PLAYER_POSY3, 0.0f }, pPlayer[0].move, D3DXCOLOR{ 1.0f, 0.05f, 0.05f, 0.75f }, 100, 100, 0.5f, 13.0f, 0.1f, 3.0f);
							ExplosionEffect(D3DXVECTOR3{ pPlayer[0].pos.x, pPlayer[0].pos.y + PLAYER_POSY3, 0.0f }, pPlayer[0].move, D3DXCOLOR{ 1.0f, 0.05f, 0.05f, 0.75f }, 100, 100, 0.5f, 13.0f, 0.1f, 3.0f);

							SetExplosion(D3DXVECTOR3{ pPlayer[0].pos.x, pPlayer[0].pos.y + PLAYER_POSY3, 0.0f }, COLLAR{ 255,252,10 }, HALF_EXPLOSION * 10, HALF_EXPLOSION * 10, 5, 4, 2, 0, ANIMATION_NORMAL);
							PlaySound(SOUND_LABEL_BOMB001);
							BreakShooter();
							ChangeGuard(2);
							pPlayer[0].bUse = false;
							AddScore(-5000, 0);
							if (g_aBullet[nCntBullet].state >= BULLETSTATE_SET_A) { StartBullet(g_aBullet[nCntBullet].nCntEnemy, g_aBullet[nCntBullet].nCntState[1]); }
						}//ヒット判定
					}
					else if(pPlayer[0].bGuard == true)
					{
						bHit = HitAim(g_aBullet[nCntBullet], D3DXVECTOR3{ pPlayer[0].pos.x, pPlayer[0].pos.y + PLAYER_POSY3, 0.0f }, 32, HIT_PLUS);
						if (bHit == true)
						{//ガード成功
							ChangeGuard(0);
							g_aBullet[nCntBullet].bUse = false;
							AddScore(300, 0);
							SetExplosion(g_aBullet[nCntBullet].pos, COLLAR{ 255, 255, 255, 200 }, 10, 10, 5, 4, 2, 0, ANIMATION_NORMAL);
							if(g_aBullet[nCntBullet].state >= BULLETSTATE_SET_A){ StartBullet(g_aBullet[nCntBullet].nCntEnemy, g_aBullet[nCntBullet].nCntState[1]); }
							if (nCntGuard > 0) 
							{ 
								PlaySound(SOUND_LABEL_GUARD000);
								nCntGuard = 0;
							}
						}
					}
				}//自機存在判定
			}
		}
	}
	//頂点BUFFERをアンロックする
	g_pVtxBuffBullet->Unlock();

}
//=========================================================================================================//
// * 弾の当たり判定
//=========================================================================================================//
bool HitAim(Bullet pBullet, D3DXVECTOR3 aim, int nLength, float fPlusHit)
{
	float fLengthX;			//弾の中心から目標の中心までの距離X
	float fLengthY;			//弾の中心から目標の中心までの距離Y
	float fLengthAim;		//目標までの距離(斜)
	float fAimSteep = 1.0;	//目標までの角度 -> 初期化
	float fBullSteep;		//弾の角度
	float fLengthBX;		//弾の有効の幅X
	float fLengthBY;		//弾の有効の幅Y
	float fLengthB;			//弾の有効の幅(合計)
	float fSteep[2] = { 1.0f, 1.0f };	//X軸のマイナスの保存用 -> 初期化

	float fPercentY = 1.0f;		//楕円時のYの倍率
	float fPercentX = 1.0f;		//楕円時のXの倍率

	fLengthX = aim.x - pBullet.pos.x;	//差Xを代入
	fLengthY = aim.y - pBullet.pos.y;	//差Yを代入
	if (fLengthX < 0.0f)
	{//Xの値がマイナスならプラスに
		fLengthX *= -1;	//X反転
		fSteep[0] *= -1;	//X方向マイナスを保存
	}
	if (fLengthY < 0.0f)
	{//Yの値がマイナスならプラスに
		fLengthY *= -1;		//Y反転
		fSteep[1] *= -1;	//Y方向マイナスを保存
	}
	

	//距離(斜)を2乗の状態で代入
	fLengthAim = (fLengthX * fLengthX) + (fLengthY * fLengthY);
	fLengthAim -= nLength * nLength;

	//％で目標への角度->sin, cosに変換 100％が1.57とする
	fAimSteep = fLengthY / (fLengthX + fLengthY) * (D3DX_PI / 2) * fSteep[1];
	fAimSteep = (D3DX_PI / 2 + fAimSteep) * fSteep[0];
	//fAimSteep = atan2f(fLengthX * fSteep[0], fLengthY * fSteep[1]);

	//弾の角度と目標の角度の合成
	fBullSteep = pBullet.steep;
	if (fBullSteep < 0)
	{//補正の必要がない場合
		fBullSteep = D3DX_PI + fBullSteep;
		fAimSteep -= fBullSteep;
	}
	else
	{//弾の角度を最小値に戻す
		fBullSteep = D3DX_PI - fBullSteep;
		fAimSteep += fBullSteep;
	}

	if (pBullet.nLengthY > pBullet.nLengthX || pBullet.nLengthY < pBullet.nLengthX)
	{//ポリゴンのXYの比率に応じて角度取り直し
		if (pBullet.nLengthY > pBullet.nLengthX)
		{//Y軸が長い場合　Yの比率保存
			fPercentX = sinf(fAimSteep) * pBullet.nLengthY / pBullet.nLengthX;
			fPercentY = cosf(fAimSteep) * 1.0f;
		}
		if (pBullet.nLengthY < pBullet.nLengthX)
		{//X軸が長い場合　Xの比率保存
			fPercentX = sinf(fAimSteep) * 1.0f;
			fPercentY = cosf(fAimSteep) * pBullet.nLengthX / pBullet.nLengthY;
		}

		fAimSteep = 1;	//角度初期化
		fSteep[0] = 1;	//マイナス保存用も初期化
		if (fPercentX < 0.0f)
		{//X軸のマイナス保存
			fPercentX *= -1;
			fSteep[0] = -1;
		}
		if (fPercentY < 0.0f)
		{//Y軸のマイナス保存
			fPercentY *= -1;
			fAimSteep = -1;
		}
		//％で目標への角度->sin, cosに変換 100％が1.57とする 楕円生成
		fAimSteep = fPercentY / (fPercentX + fPercentY) * (D3DX_PI / 2) * fAimSteep;
		fAimSteep = (D3DX_PI / 2 + fAimSteep) * fSteep[0];
	}

	fLengthBY = cosf(fAimSteep) * pBullet.nLengthY;
	fLengthBX = sinf(fAimSteep) * pBullet.nLengthX;

	fLengthB = (fLengthBX + fLengthBY) * (fLengthBX + fLengthBY);	//弾の有効な幅(合計)を代入

	if (fLengthAim < fLengthB * fPlusHit)
	{//判定　2乗された状態なので
		return true;
	}
	else
	{
		return false;
	}
}
//=========================================================================================================//
// * 弾の描画処理4	描画処理
//=========================================================================================================//
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet;

	Bullet *g_aBullet;
	g_aBullet = GetBullet();

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// αブレンディングを加算合成に設定
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, 2);
	
	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_NUM);
	// ポリゴンの描画
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			if (g_aBullet[nCntBullet].nAtkType < BULLET_ATKTYPE99) { pDevice->SetTexture(0, g_pTextureBullet[0]); }
			else { pDevice->SetTexture(0, g_pTextureBullet[1]); }
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, BULLET_PRIMITIVE_NUM);
		}
	}

	// αブレンディングを元に戻す
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
//=============================================================================
// 弾の設定処理
//=============================================================================
void SetBullet(D3DXVECTOR3 pos, float steep, int nLengthX, int nLengthY, int nType, int nAtkType, float fSpeed, BULLETTYPE nShot, int nCntEnemy, BULLETSTATE state, float fSpdPlus, float fCurve, int nCntstate0, int nCntstate1)
{
	int nCntBullet;			//弾数えるの～
	int nClear = 255;		//透明設定のため
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	ENEMY *pEnemy = GetEnemy();

	Bullet *g_aBullet;
	g_aBullet = GetBullet();

	//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{
			//まとめて情報を代入
			g_aBullet[nCntBullet] = { pos, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, state,{ nCntstate0, nCntstate1, 0 }, nLengthX, nLengthY, nType, nAtkType, nShot, nCntEnemy, steep, {pEnemy[nCntEnemy].attack[nCntstate1].fSetAngle[0],pEnemy[nCntEnemy].attack[nCntstate1].fSetAngle[1] }, fCurve, fSpeed, fSpdPlus, 1.0f, true };
			//g_aBullet[nCntBullet].pos = pos;
			//g_aBullet[nCntBullet].move = D3DXVECTOR3{ 0.0f, 0.0f, 0.0f };
			//g_aBullet[nCntBullet].state = state;
			//g_aBullet[nCntBullet].nCntState[0] = nCntstate0;
			//g_aBullet[nCntBullet].nCntState[1] = nCntstate1;
			//g_aBullet[nCntBullet].nCntState[2] = 0;
			//g_aBullet[nCntBullet].nLengthX = nLengthX;
			//g_aBullet[nCntBullet].nLengthY = nLengthY;
			//g_aBullet[nCntBullet].nType = nType;
			//g_aBullet[nCntBullet].nAtkType = nAtkType;
			//g_aBullet[nCntBullet].nShot = nShot;
			//g_aBullet[nCntBullet].nCntEnemy = nCntEnemy;
			//g_aBullet[nCntBullet].steep = steep;
			//g_aBullet[nCntBullet].fSetSteep[0] = pEnemy[nCntEnemy].attack[nCntstate1].fSetAngle[0];
			//g_aBullet[nCntBullet].fSetSteep[1] = pEnemy[nCntEnemy].attack[nCntstate1].fSetAngle[1];
			//g_aBullet[nCntBullet].fSpeed = fSpeed;
			//g_aBullet[nCntBullet].fSpdPlus = fSpdPlus;
			//g_aBullet[nCntBullet].fcolA = 1.0f;
			//g_aBullet[nCntBullet].bUse = true;

			if (g_aBullet[nCntBullet].nAtkType == BULLET_ATKTYPE99)
			{//ナイフ
				if (nShot == BULLETTYPE_PLAYER) { g_aBullet[nCntBullet].steep = KNIFE_STEEP; }
				if (nShot == BULLETTYPE_ENEMY)  { g_aBullet[nCntBullet].steep = KNIFE_STEEPENEMY + pEnemy[nCntEnemy].fsteep; }
				pVtx[nCntBullet * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCntBullet * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[nCntBullet * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[nCntBullet * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
			else
			{//通常弾
				g_aBullet[nCntBullet].steep = steep;		//弾の角度をもとにポリゴンの角度を調整
				pVtx[nCntBullet * 4].tex = D3DXVECTOR2((nType % 16) * 0.0625f, (nType / 16) * 0.1f);
				pVtx[nCntBullet * 4 + 1].tex = D3DXVECTOR2((nType % 16) * 0.0625f + 0.0625f, (nType / 16) * 0.1f);
				pVtx[nCntBullet * 4 + 2].tex = D3DXVECTOR2((nType % 16) * 0.0625f, (nType / 16) * 0.1f + 0.1f);
				pVtx[nCntBullet * 4 + 3].tex = D3DXVECTOR2((nType % 16) * 0.0625f + 0.0625f, (nType / 16) * 0.1f + 0.1f);
			}
			pVtx[nCntBullet * 4].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY) + (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX), g_aBullet[nCntBullet].pos.y - (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX) + (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY), 0.0f);
			pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY) - (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX), g_aBullet[nCntBullet].pos.y + (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX) + (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY), 0.0f);
			pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY) + (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX), g_aBullet[nCntBullet].pos.y - (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX) - (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY), 0.0f);
			pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY) - (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX), g_aBullet[nCntBullet].pos.y + (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX) - (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY), 0.0f);

			g_aBullet[nCntBullet].nShot = nShot;

			nClear = 255;
			if (g_aBullet[nCntBullet].nShot == 0 && g_aBullet[nCntBullet].nAtkType < BULLET_ATKTYPE99)
			{
				nClear = 180;
				if (g_aBullet[nCntBullet].nAtkType == BULLET_ATKTYPE1 || g_aBullet[nCntBullet].nAtkType == BULLET_ATKTYPE50)
				{
					nClear = 80;
				}
			}
			if (g_aBullet[nCntBullet].state == BULLETSTATE_FLASH) { nClear = 200; }
			
			pVtx[nCntBullet * 4].col = D3DCOLOR_RGBA(255, 255, 255, nClear);	// A = 透明度
			pVtx[nCntBullet * 4 + 1].col = D3DCOLOR_RGBA(255, 255, 255, nClear);
			pVtx[nCntBullet * 4 + 2].col = D3DCOLOR_RGBA(255, 255, 255, nClear);
			pVtx[nCntBullet * 4 + 3].col = D3DCOLOR_RGBA(255, 255, 255, nClear);
			break;
		}
	}
	//頂点BUFFERをアンロックする
	g_pVtxBuffBullet->Unlock();
}
//=============================================================================
// 弾の設定処理
//=============================================================================
void StartBullet(int nCntEnemy, int nCntAtk)
{
	int nCntBullet;
	Bullet *g_aBullet = GetBullet();

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].nCntEnemy == nCntEnemy && g_aBullet[nCntBullet].nCntState[1] == nCntAtk && g_aBullet[nCntBullet].bUse == true)
		{
			g_aBullet[nCntBullet].nCntState[0] = 0;
		}
	}
}
//=============================================================================
// 弾の設定処理
//=============================================================================
void BreakBullet(void)
{
	int nCntBullet;
	Bullet *g_aBullet = GetBullet();

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].bUse = false;
	}
}
//=============================================================================
// 弾の設定処理
//=============================================================================
void BreakBullet(int nCntEnemy)
{
	int nCntBullet;
	Bullet *g_aBullet;
	g_aBullet = GetBullet();

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{//指定したエネミーの弾を破壊
		if(g_aBullet[nCntBullet].nCntEnemy == nCntEnemy)
		{ 
			g_aBullet[nCntBullet].state = BULLETSTATE_DELETE;
			//ExplosionEffect(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].move, D3DXCOLOR{ 1.0f, 0.05f, 0.05f, 0.75f }, 100, 100, 0.1f, g_aBullet[nCntBullet].nLengthX * 0.05f, 0.1f, 1.0f);
		}
		//エフェクト欲しいな
	}
}
//=============================================================================
// 弾の取得
//=============================================================================
Bullet *GetBullet(void)
{
	static Bullet bullet[MAX_BULLET];//弾の最大数

	return &bullet[0];
}