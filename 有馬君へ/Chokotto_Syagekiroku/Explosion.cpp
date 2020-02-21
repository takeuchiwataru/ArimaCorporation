//=========================================================================================================//
//
// [弾発射]->弾移動:爆発処理 [bullet.cpp]
// Author : Ryou Sugimoto
//
//=========================================================================================================//
#include "main.h"
#include "game.h"

//*********************************************************************************************************//
// マクロ定義
//*********************************************************************************************************//
#define EXPLOSION_VERTEX_NUM		(4)					// 頂点の数
#define EXPLOSION_PRIMITIVE_NUM		(2)					// プリミティブの数
#define EXPLOSION_MAX_TEX			(4)
#define EXPLOSION_TextureName		"data/TEXTURE/Explosion003.png"
#define EXPLOSION_TextureName1		"data/TEXTURE/pipo-btleffect108f.png"
#define EXPLOSION_TextureName2		"data/TEXTURE/warp1_640.png"
#define EXPLOSION_TextureName3		"data/TEXTURE/ranking000.png"
#define	MAX_EXPLOSION				(32)		// 弾の最大数
#define HALFEXPLOSION				(30)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 col;	//色
	int nLengthX;		//Xの長さ
	int nLengthY;		//Yの長さ
	int nCntAnim;		//Animationのカウント
	int nPtnAnim;		//指定された座標で表示
	int nPtnHeight;		//画像の縦の分割数
	int nPtnWide;		//画像の横の分割数
	int nPtnSpeed;		//Animation速度
	int nType;			//画像の指定
	ANIMATION anim;		//アニメーションの使い方
	bool bUse;
}Explosin;

//*********************************************************************************************************//
// グローバル変数
//*********************************************************************************************************//
LPDIRECT3DTEXTURE9			g_pTextureExplosion[EXPLOSION_MAX_TEX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffExplosion = NULL;						// 頂点バッファへのポインタ
Explosin					g_aExplosion[MAX_EXPLOSION];					//最大のAnimation数

//=========================================================================================================//
// * 背景の描画処理1	初期化処理
//=========================================================================================================//
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	int nCntBullet;

	// 弾の情報の初期化
	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{
		//g_aExplosion[nCntBullet].pos = { 0.0f, 0.0f, 0.0f };
		//g_aExplosion[nCntBullet].col = { 1.0f, 1.0f, 1.0f, };
		//g_aExplosion[nCntBullet].nCntAnim = -1;
		//g_aExplosion[nCntBullet].nPtnAnim = -1;
		//g_aExplosion[nCntBullet].nType = 0;
		g_aExplosion[nCntBullet].bUse = false;
	}

	//TEXTURE読み込み
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TextureName, &g_pTextureExplosion[0]);
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TextureName1, &g_pTextureExplosion[1]);
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TextureName2, &g_pTextureExplosion[2]);
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TextureName3, &g_pTextureExplosion[3]);

	//頂点BUFFERの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * EXPLOSION_VERTEX_NUM * MAX_EXPLOSION,		//確保するBUFFERのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ
							//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{
		//頂点座標の設定
		//pVtx[nCntBullet * 4].pos = D3DXVECTOR3(0, 0, 0.0f);
		//pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(0, 0, 0.0f);
		//pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(0, 0, 0.0f);
		//pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(0, 0, 0.0f);

		pVtx[nCntBullet * 4].rhw = 1.0f;
		pVtx[nCntBullet * 4 + 1].rhw = 1.0f;
		pVtx[nCntBullet * 4 + 2].rhw = 1.0f;
		pVtx[nCntBullet * 4 + 3].rhw = 1.0f;

		////カラー設定
		//pVtx[nCntBullet * 4].col = D3DCOLOR_RGBA(0,0,0, 255);	// A = 透明度
		//pVtx[nCntBullet * 4 + 1].col = D3DCOLOR_RGBA(0,0,0, 255);
		//pVtx[nCntBullet * 4 + 2].col = D3DCOLOR_RGBA(0,0,0, 255);
		//pVtx[nCntBullet * 4 + 3].col = D3DCOLOR_RGBA(0,0,0, 255);

		//テクスチャ座標設定
		//pVtx[nCntBullet * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
		//pVtx[nCntBullet * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		//pVtx[nCntBullet * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		//pVtx[nCntBullet * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点BUFFERをアンロックする
	g_pVtxBuffExplosion->Unlock();
}
//=========================================================================================================//
// * 背景の描画処理2	終了処理
//=========================================================================================================//
void UninitExplosion(void)
{//テクスチャの破棄
	int nCntTexture;

	for (nCntTexture = 0; nCntTexture < EXPLOSION_MAX_TEX; nCntTexture++)
	{
		if (g_pTextureExplosion[nCntTexture] != NULL)
		{
			g_pTextureExplosion[nCntTexture]->Release();
			g_pTextureExplosion[nCntTexture] = NULL;
		}
	}
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}
//=========================================================================================================//
// * 背景の描画処理3	更新処理
//=========================================================================================================//
void UpdateExplosion(void)
{
	int nCntBullet;
	PLAYER *pPlayer;
	VERTEX_2D *pVtx;		//頂点情報へのポインタ
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスを取得する
	pDevice = GetDevice();

	//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{

		if (g_aExplosion[nCntBullet].bUse == true)
		{//弾を前に飛ばす
		 //MessageBox(0, "本当すか？", "終了", MB_YESNO);
			g_aExplosion[nCntBullet].nCntAnim++;

			if (g_aExplosion[nCntBullet].nType == 1)
			{
				pPlayer = GetPlayer();

				pVtx[nCntBullet * 4].pos = D3DXVECTOR3(pPlayer[0].pos.x - g_aExplosion[nCntBullet].nLengthX, pPlayer[0].pos.y + PLAYER_POSY3 - g_aExplosion[nCntBullet].nLengthY, 0.0f);
				pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(pPlayer[0].pos.x + g_aExplosion[nCntBullet].nLengthX, pPlayer[0].pos.y + PLAYER_POSY3 - g_aExplosion[nCntBullet].nLengthY, 0.0f);
				pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(pPlayer[0].pos.x - g_aExplosion[nCntBullet].nLengthX, pPlayer[0].pos.y + PLAYER_POSY3 + g_aExplosion[nCntBullet].nLengthY, 0.0f);
				pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(pPlayer[0].pos.x + g_aExplosion[nCntBullet].nLengthX, pPlayer[0].pos.y + PLAYER_POSY3 + g_aExplosion[nCntBullet].nLengthY, 0.0f);
			}
			else
			{
				pVtx[nCntBullet * 4].pos = D3DXVECTOR3(g_aExplosion[nCntBullet].pos.x - g_aExplosion[nCntBullet].nLengthX, g_aExplosion[nCntBullet].pos.y - g_aExplosion[nCntBullet].nLengthY, 0.0f);
				pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(g_aExplosion[nCntBullet].pos.x + g_aExplosion[nCntBullet].nLengthX, g_aExplosion[nCntBullet].pos.y - g_aExplosion[nCntBullet].nLengthY, 0.0f);
				pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(g_aExplosion[nCntBullet].pos.x - g_aExplosion[nCntBullet].nLengthX, g_aExplosion[nCntBullet].pos.y + g_aExplosion[nCntBullet].nLengthY, 0.0f);
				pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(g_aExplosion[nCntBullet].pos.x + g_aExplosion[nCntBullet].nLengthX, g_aExplosion[nCntBullet].pos.y + g_aExplosion[nCntBullet].nLengthY, 0.0f);

			}
			pVtx[nCntBullet * 4].tex = D3DXVECTOR2((g_aExplosion[nCntBullet].nPtnAnim % g_aExplosion[nCntBullet].nPtnWide) * (1.0f / g_aExplosion[nCntBullet].nPtnWide), (g_aExplosion[nCntBullet].nPtnAnim / g_aExplosion[nCntBullet].nPtnWide) * (1.0f / g_aExplosion[nCntBullet].nPtnHeight));
			pVtx[nCntBullet * 4 + 1].tex = D3DXVECTOR2((g_aExplosion[nCntBullet].nPtnAnim % g_aExplosion[nCntBullet].nPtnWide) * (1.0f / g_aExplosion[nCntBullet].nPtnWide) + (1.0f / g_aExplosion[nCntBullet].nPtnWide), (g_aExplosion[nCntBullet].nPtnAnim / g_aExplosion[nCntBullet].nPtnWide) * (1.0f / g_aExplosion[nCntBullet].nPtnHeight));
			pVtx[nCntBullet * 4 + 2].tex = D3DXVECTOR2((g_aExplosion[nCntBullet].nPtnAnim % g_aExplosion[nCntBullet].nPtnWide) * (1.0f / g_aExplosion[nCntBullet].nPtnWide), (g_aExplosion[nCntBullet].nPtnAnim / g_aExplosion[nCntBullet].nPtnWide) * (1.0f / g_aExplosion[nCntBullet].nPtnHeight) + (1.0f / g_aExplosion[nCntBullet].nPtnHeight));
			pVtx[nCntBullet * 4 + 3].tex = D3DXVECTOR2((g_aExplosion[nCntBullet].nPtnAnim % g_aExplosion[nCntBullet].nPtnWide) * (1.0f / g_aExplosion[nCntBullet].nPtnWide) + (1.0f / g_aExplosion[nCntBullet].nPtnWide), (g_aExplosion[nCntBullet].nPtnAnim / g_aExplosion[nCntBullet].nPtnWide) * (1.0f / g_aExplosion[nCntBullet].nPtnHeight) + (1.0f / g_aExplosion[nCntBullet].nPtnHeight));
			if (g_aExplosion[nCntBullet].nCntAnim % g_aExplosion[nCntBullet].nPtnSpeed == 0)
			{
				g_aExplosion[nCntBullet].nPtnAnim++;
				if (g_aExplosion[nCntBullet].nPtnAnim > g_aExplosion[nCntBullet].nPtnHeight * g_aExplosion[nCntBullet].nPtnWide && g_aExplosion[nCntBullet].anim != ANIMATION_LOOP)
				{//全部1回表示したら消える
					g_aExplosion[nCntBullet].bUse = false;
				}
			}
		}
	}
	//頂点BUFFERをアンロックする
	g_pVtxBuffExplosion->Unlock();

}
//=========================================================================================================//
// * 背景の描画処理4	描画処理
//=========================================================================================================//
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{
		if (g_aExplosion[nCntBullet].bUse == true)
		{
			// テクスチャの設定 ポリゴンの描画
			pDevice->SetTexture(0, g_pTextureExplosion[g_aExplosion[nCntBullet].nType]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, EXPLOSION_PRIMITIVE_NUM);
		}
	}

}
//=============================================================================
// 弾の設定処理
//=============================================================================
void SetExplosion(D3DXVECTOR3 pos, COLLAR col , int nLengthX, int nLengthY, int nPtnWide, int nPtnHeight, int nPtnSpeed, int nType, ANIMATION anim)
{
	int nCntBullet;
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{
		if (g_aExplosion[nCntBullet].bUse == false)
		{
			g_aExplosion[nCntBullet].pos = pos;
			g_aExplosion[nCntBullet].nCntAnim = -1;
			g_aExplosion[nCntBullet].nPtnAnim = -1;
			g_aExplosion[nCntBullet].nPtnHeight = nPtnHeight;
			g_aExplosion[nCntBullet].nPtnWide = nPtnWide;
			g_aExplosion[nCntBullet].nPtnSpeed = nPtnSpeed;
			g_aExplosion[nCntBullet].nLengthX = nLengthX;
			g_aExplosion[nCntBullet].nLengthY = nLengthY;
			g_aExplosion[nCntBullet].nType = nType;
			g_aExplosion[nCntBullet].anim = anim;
			g_aExplosion[nCntBullet].bUse = true;

			pVtx[nCntBullet * 4].pos = D3DXVECTOR3(g_aExplosion[nCntBullet].pos.x - g_aExplosion[nCntBullet].nLengthX, g_aExplosion[nCntBullet].pos.y - g_aExplosion[nCntBullet].nLengthY, 0.0f);
			pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(g_aExplosion[nCntBullet].pos.x + g_aExplosion[nCntBullet].nLengthX, g_aExplosion[nCntBullet].pos.y - g_aExplosion[nCntBullet].nLengthY, 0.0f);
			pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(g_aExplosion[nCntBullet].pos.x - g_aExplosion[nCntBullet].nLengthX, g_aExplosion[nCntBullet].pos.y + g_aExplosion[nCntBullet].nLengthY, 0.0f);
			pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(g_aExplosion[nCntBullet].pos.x + g_aExplosion[nCntBullet].nLengthX, g_aExplosion[nCntBullet].pos.y + g_aExplosion[nCntBullet].nLengthY, 0.0f);

			pVtx[nCntBullet * 4].col = D3DCOLOR_RGBA(col.colR, col.colG, col.colB, 200);	// A = 透明度
			pVtx[nCntBullet * 4 + 1].col = D3DCOLOR_RGBA(col.colR, col.colG, col.colB, 200);
			pVtx[nCntBullet * 4 + 2].col = D3DCOLOR_RGBA(col.colR, col.colG, col.colB, 200);
			pVtx[nCntBullet * 4 + 3].col = D3DCOLOR_RGBA(col.colR, col.colG, col.colB, 200);
			break;
		}
	}
	//頂点BUFFERをアンロックする
	g_pVtxBuffExplosion->Unlock();
}
//=========================================================================================================//
// * 全Animation破棄処理
//=========================================================================================================//
void BreakExplosion(void)
{
	int nCntAnim;

	for (nCntAnim = 0; nCntAnim < MAX_EXPLOSION; nCntAnim++)
	{
		g_aExplosion[nCntAnim].bUse = false;
	}
}
