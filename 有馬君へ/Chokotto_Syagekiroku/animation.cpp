//=========================================================================================================//
//
// アニメーション処理 [animation.cpp]
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
#define EXPLOSION_MAX_TEX			(1)
#define EXPLOSION_TextureName		"data/TEXTURE/pipo-btleffect084.png"
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
}Animation;

//*********************************************************************************************************//
// グローバル変数
//*********************************************************************************************************//
LPDIRECT3DTEXTURE9			g_pTextureAnimation[EXPLOSION_MAX_TEX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffAnimation = NULL;						// 頂点バッファへのポインタ
Animation					g_aAnimation[MAX_EXPLOSION];					//最大のAnimation数

//=========================================================================================================//
// * 背景の描画処理1	初期化処理
//=========================================================================================================//
void InitAnimation(void)
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
		g_aAnimation[nCntBullet].bUse = false;
	}

	//TEXTURE読み込み
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TextureName, &g_pTextureAnimation[0]);

	//頂点BUFFERの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * EXPLOSION_VERTEX_NUM * MAX_EXPLOSION,		//確保するBUFFERのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffAnimation,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ
							//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffAnimation->Lock(0, 0, (void**)&pVtx, 0);
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
	g_pVtxBuffAnimation->Unlock();
}
//=========================================================================================================//
// * 背景の描画処理2	終了処理
//=========================================================================================================//
void UninitAnimation(void)
{//テクスチャの破棄
	int nCntTexture;

	for (nCntTexture = 0; nCntTexture < EXPLOSION_MAX_TEX; nCntTexture++)
	{
		if (g_pTextureAnimation[nCntTexture] != NULL)
		{
			g_pTextureAnimation[nCntTexture]->Release();
			g_pTextureAnimation[nCntTexture] = NULL;
		}
	}
	if (g_pVtxBuffAnimation != NULL)
	{
		g_pVtxBuffAnimation->Release();
		g_pVtxBuffAnimation = NULL;
	}
}
//=========================================================================================================//
// * 背景の描画処理3	更新処理
//=========================================================================================================//
void UpdateAnimation(void)
{
	int nCntBullet;
	PLAYER *pPlayer;
	VERTEX_2D *pVtx;		//頂点情報へのポインタ
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスを取得する
	pDevice = GetDevice();

	//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffAnimation->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{

		if (g_aAnimation[nCntBullet].bUse == true)
		{//弾を前に飛ばす
		 //MessageBox(0, "本当すか？", "終了", MB_YESNO);
			g_aAnimation[nCntBullet].nCntAnim++;

			if (g_aAnimation[nCntBullet].nType == 1)
			{
				pPlayer = GetPlayer();

				pVtx[nCntBullet * 4].pos = D3DXVECTOR3(pPlayer[0].pos.x - g_aAnimation[nCntBullet].nLengthX, pPlayer[0].pos.y + PLAYER_POSY3 - g_aAnimation[nCntBullet].nLengthY, 0.0f);
				pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(pPlayer[0].pos.x + g_aAnimation[nCntBullet].nLengthX, pPlayer[0].pos.y + PLAYER_POSY3 - g_aAnimation[nCntBullet].nLengthY, 0.0f);
				pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(pPlayer[0].pos.x - g_aAnimation[nCntBullet].nLengthX, pPlayer[0].pos.y + PLAYER_POSY3 + g_aAnimation[nCntBullet].nLengthY, 0.0f);
				pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(pPlayer[0].pos.x + g_aAnimation[nCntBullet].nLengthX, pPlayer[0].pos.y + PLAYER_POSY3 + g_aAnimation[nCntBullet].nLengthY, 0.0f);
			}
			else
			{
				pVtx[nCntBullet * 4].pos = D3DXVECTOR3(g_aAnimation[nCntBullet].pos.x - g_aAnimation[nCntBullet].nLengthX, g_aAnimation[nCntBullet].pos.y - g_aAnimation[nCntBullet].nLengthY, 0.0f);
				pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(g_aAnimation[nCntBullet].pos.x + g_aAnimation[nCntBullet].nLengthX, g_aAnimation[nCntBullet].pos.y - g_aAnimation[nCntBullet].nLengthY, 0.0f);
				pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(g_aAnimation[nCntBullet].pos.x - g_aAnimation[nCntBullet].nLengthX, g_aAnimation[nCntBullet].pos.y + g_aAnimation[nCntBullet].nLengthY, 0.0f);
				pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(g_aAnimation[nCntBullet].pos.x + g_aAnimation[nCntBullet].nLengthX, g_aAnimation[nCntBullet].pos.y + g_aAnimation[nCntBullet].nLengthY, 0.0f);

			}
			pVtx[nCntBullet * 4].tex = D3DXVECTOR2((g_aAnimation[nCntBullet].nPtnAnim % g_aAnimation[nCntBullet].nPtnWide) * (1.0f / g_aAnimation[nCntBullet].nPtnWide), (g_aAnimation[nCntBullet].nPtnAnim / g_aAnimation[nCntBullet].nPtnWide) * (1.0f / g_aAnimation[nCntBullet].nPtnHeight));
			pVtx[nCntBullet * 4 + 1].tex = D3DXVECTOR2((g_aAnimation[nCntBullet].nPtnAnim % g_aAnimation[nCntBullet].nPtnWide) * (1.0f / g_aAnimation[nCntBullet].nPtnWide) + (1.0f / g_aAnimation[nCntBullet].nPtnWide), (g_aAnimation[nCntBullet].nPtnAnim / g_aAnimation[nCntBullet].nPtnWide) * (1.0f / g_aAnimation[nCntBullet].nPtnHeight));
			pVtx[nCntBullet * 4 + 2].tex = D3DXVECTOR2((g_aAnimation[nCntBullet].nPtnAnim % g_aAnimation[nCntBullet].nPtnWide) * (1.0f / g_aAnimation[nCntBullet].nPtnWide), (g_aAnimation[nCntBullet].nPtnAnim / g_aAnimation[nCntBullet].nPtnWide) * (1.0f / g_aAnimation[nCntBullet].nPtnHeight) + (1.0f / g_aAnimation[nCntBullet].nPtnHeight));
			pVtx[nCntBullet * 4 + 3].tex = D3DXVECTOR2((g_aAnimation[nCntBullet].nPtnAnim % g_aAnimation[nCntBullet].nPtnWide) * (1.0f / g_aAnimation[nCntBullet].nPtnWide) + (1.0f / g_aAnimation[nCntBullet].nPtnWide), (g_aAnimation[nCntBullet].nPtnAnim / g_aAnimation[nCntBullet].nPtnWide) * (1.0f / g_aAnimation[nCntBullet].nPtnHeight) + (1.0f / g_aAnimation[nCntBullet].nPtnHeight));
			if (g_aAnimation[nCntBullet].nCntAnim % g_aAnimation[nCntBullet].nPtnSpeed == 0)
			{
				g_aAnimation[nCntBullet].nPtnAnim++;
				if (g_aAnimation[nCntBullet].nPtnAnim > g_aAnimation[nCntBullet].nPtnHeight * g_aAnimation[nCntBullet].nPtnWide && g_aAnimation[nCntBullet].anim != ANIMATION_LOOP)
				{//全部1回表示したら消える
					g_aAnimation[nCntBullet].bUse = false;
				}
			}
		}
	}
	//頂点BUFFERをアンロックする
	g_pVtxBuffAnimation->Unlock();

}
//=========================================================================================================//
// * 背景の描画処理4	描画処理
//=========================================================================================================//
void DrawAnimation(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffAnimation, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{
		if (g_aAnimation[nCntBullet].bUse == true)
		{
			// テクスチャの設定 ポリゴンの描画
			pDevice->SetTexture(0, g_pTextureAnimation[g_aAnimation[nCntBullet].nType]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, EXPLOSION_PRIMITIVE_NUM);
		}
	}

}
//=============================================================================
// 弾の設定処理
//=============================================================================
void SetAnimation(D3DXVECTOR3 pos, COLLAR col , int nLengthX, int nLengthY, int nPtnWide, int nPtnHeight, int nPtnSpeed, int nType, ANIMATION anim)
{
	int nCntBullet;
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffAnimation->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{
		if (g_aAnimation[nCntBullet].bUse == false)
		{
			g_aAnimation[nCntBullet].pos = pos;
			g_aAnimation[nCntBullet].nCntAnim = -1;
			g_aAnimation[nCntBullet].nPtnAnim = -1;
			g_aAnimation[nCntBullet].nPtnHeight = nPtnHeight;
			g_aAnimation[nCntBullet].nPtnWide = nPtnWide;
			g_aAnimation[nCntBullet].nPtnSpeed = nPtnSpeed;
			g_aAnimation[nCntBullet].nLengthX = nLengthX;
			g_aAnimation[nCntBullet].nLengthY = nLengthY;
			g_aAnimation[nCntBullet].nType = nType;
			g_aAnimation[nCntBullet].anim = anim;
			g_aAnimation[nCntBullet].bUse = true;

			pVtx[nCntBullet * 4].pos = D3DXVECTOR3(g_aAnimation[nCntBullet].pos.x - g_aAnimation[nCntBullet].nLengthX, g_aAnimation[nCntBullet].pos.y - g_aAnimation[nCntBullet].nLengthY, 0.0f);
			pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(g_aAnimation[nCntBullet].pos.x + g_aAnimation[nCntBullet].nLengthX, g_aAnimation[nCntBullet].pos.y - g_aAnimation[nCntBullet].nLengthY, 0.0f);
			pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(g_aAnimation[nCntBullet].pos.x - g_aAnimation[nCntBullet].nLengthX, g_aAnimation[nCntBullet].pos.y + g_aAnimation[nCntBullet].nLengthY, 0.0f);
			pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(g_aAnimation[nCntBullet].pos.x + g_aAnimation[nCntBullet].nLengthX, g_aAnimation[nCntBullet].pos.y + g_aAnimation[nCntBullet].nLengthY, 0.0f);

			pVtx[nCntBullet * 4].col = D3DCOLOR_RGBA(col.colR, col.colG, col.colB, 200);	// A = 透明度
			pVtx[nCntBullet * 4 + 1].col = D3DCOLOR_RGBA(col.colR, col.colG, col.colB, 200);
			pVtx[nCntBullet * 4 + 2].col = D3DCOLOR_RGBA(col.colR, col.colG, col.colB, 200);
			pVtx[nCntBullet * 4 + 3].col = D3DCOLOR_RGBA(col.colR, col.colG, col.colB, 200);
			break;
		}
	}
	//頂点BUFFERをアンロックする
	g_pVtxBuffAnimation->Unlock();
}
//=========================================================================================================//
// * 全Animation破棄処理
//=========================================================================================================//
void BreakAnimation(void)
{
	int nCntAnim;

	for (nCntAnim = 0; nCntAnim < MAX_EXPLOSION; nCntAnim++)
	{
		g_aAnimation[nCntAnim].bUse = false;
	}
}
