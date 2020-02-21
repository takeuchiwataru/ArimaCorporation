//=========================================================================================================//
//
// window処理 [window.cpp]
// Author : Ryou Sugimoto
//
//=========================================================================================================//
#include "main.h"
#include "game.h"

//*********************************************************************************************************//
// マクロ定義
//*********************************************************************************************************//
#define WINDOW_VERTEX_NUM			(4)								// 頂点の数
#define WINDOW_PRIMITIVE_NUM		(2)								// プリミティブの数
#define WINDOW_MAX_TEXTURE			(5)								//TEXTUREの数
#define WINDOW_TextureName0		"data/TEXTURE/20.jpg"			//タイトル
#define WINDOW_TextureName1		"data/TEXTURE/BG001.jpg"			//ゲーム画面
#define WINDOW_TextureName2		"data/TEXTURE/Tutorial001.jpg"		//Tutorial
#define WINDOW_TextureName3		"data/TEXTURE/pipo-fog003.png"		//Smoke0
#define WINDOW_TextureName4		"data/TEXTURE/pipo-fog002.png"		//Smoke1
#define	MAX_WINDOW			(4)		// windowの最大数
#define HALFEXPLOSION		(30)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	COLLAR col;			//色
	int nLengthX;		//長さX
	int nLengthY;		//長さY
	int nType;			//画像
	WINDOWSTATE state;	//使用状態
	float fSpeed;		//SCROLLSPEED
	float fPos[3];		//0：現在値, 1：縦引き用, 2：横現在値
	bool bUse;
}BGWindow;

//*********************************************************************************************************//
// グローバル変数
//*********************************************************************************************************//
LPDIRECT3DTEXTURE9		g_pTextureBGWindow[WINDOW_MAX_TEXTURE] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBGWindow = NULL;	// 頂点バッファへのポインタ
BGWindow g_aBGWindow[MAX_WINDOW];

//=========================================================================================================//
// * 背景の描画処理1	初期化処理
//=========================================================================================================//
void InitBGWindow(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	int nCntWindow;

	// 弾の情報の初期化
	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		//g_aWindow[nCntWindow].pos = { 0.0f, 0.0f, 0.0f };
		//g_aWindow[nCntWindow].nLengthX = 0;
		//g_aWindow[nCntWindow].nLengthY = 0;
		//g_aWindow[nCntWindow].nType = 0;
		//g_aWindow[nCntWindow].col = { 1.0f, 1.0f, 1.0f, };
		g_aBGWindow[nCntWindow].bUse = false;
	}

	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName0, &g_pTextureBGWindow[0]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName1, &g_pTextureBGWindow[1]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName2, &g_pTextureBGWindow[2]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName3, &g_pTextureBGWindow[3]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName4, &g_pTextureBGWindow[4]);

	//頂点BUFFERの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * WINDOW_VERTEX_NUM * MAX_WINDOW,		//確保するBUFFERのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffBGWindow,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ
	//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBGWindow->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		//頂点座標の設定
		pVtx[nCntWindow * 4].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[nCntWindow * 4 + 1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[nCntWindow * 4 + 2].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[nCntWindow * 4 + 3].pos = D3DXVECTOR3(0, 0, 0.0f);

		pVtx[nCntWindow * 4].rhw = 1.0f;
		pVtx[nCntWindow * 4 + 1].rhw = 1.0f;
		pVtx[nCntWindow * 4 + 2].rhw = 1.0f;
		pVtx[nCntWindow * 4 + 3].rhw = 1.0f;

		//カラー設定
		pVtx[nCntWindow * 4].col = D3DCOLOR_RGBA(0, 0, 0, 255);	// A = 透明度
		pVtx[nCntWindow * 4 + 1].col = D3DCOLOR_RGBA(0, 0, 0, 255);
		pVtx[nCntWindow * 4 + 2].col = D3DCOLOR_RGBA(0, 0, 0, 255);
		pVtx[nCntWindow * 4 + 3].col = D3DCOLOR_RGBA(0, 0, 0, 255);

		//テクスチャ座標設定
		pVtx[nCntWindow * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nCntWindow * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[nCntWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[nCntWindow * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点BUFFERをアンロックする
	g_pVtxBuffBGWindow->Unlock();
}
//=========================================================================================================//
// * 背景の描画処理2	終了処理
//=========================================================================================================//
void UninitBGWindow(void)
{//テクスチャの破棄
	int nCntTexture;

	for (nCntTexture = 0; nCntTexture < WINDOW_MAX_TEXTURE; nCntTexture++)
	{
		if (g_pTextureBGWindow[nCntTexture] != NULL)
		{
			g_pTextureBGWindow[nCntTexture]->Release();
			g_pTextureBGWindow[nCntTexture] = NULL;
		}
	}
	if (g_pVtxBuffBGWindow != NULL)
	{
		g_pVtxBuffBGWindow->Release();
		g_pVtxBuffBGWindow = NULL;
	}
}
//=========================================================================================================//
// * 背景の描画処理3	更新処理
//=========================================================================================================//
void UpdateBGWindow(void)
{
	int nCntWindow;

	VERTEX_2D *pVtx;		//頂点情報へのポインタ
	//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBGWindow->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aBGWindow[nCntWindow].bUse == true)
		{//使っているwindowなら

			switch (g_aBGWindow[nCntWindow].state)
			{
			case WINDOWSTATE_SCROLL0: //縦にSCROLL
				g_aBGWindow[nCntWindow].fPos[1] -= g_aBGWindow[nCntWindow].fSpeed;
				break;

			case WINDOWSTATE_SCROLL1:	//斜めSCROLL
				g_aBGWindow[nCntWindow].fPos[1] -= g_aBGWindow[nCntWindow].fSpeed;	//下へ
				g_aBGWindow[nCntWindow].fPos[2] += g_aBGWindow[nCntWindow].fSpeed;	//左へ
				break;

			default:
				break;
			}

			pVtx[nCntWindow * 4].tex = D3DXVECTOR2(g_aBGWindow[nCntWindow].fPos[2] - 1.0f, g_aBGWindow[nCntWindow].fPos[1] - g_aBGWindow[nCntWindow].fPos[0]);
			pVtx[nCntWindow * 4 + 1].tex = D3DXVECTOR2(g_aBGWindow[nCntWindow].fPos[2], g_aBGWindow[nCntWindow].fPos[1] - g_aBGWindow[nCntWindow].fPos[0]);
			pVtx[nCntWindow * 4 + 2].tex = D3DXVECTOR2(g_aBGWindow[nCntWindow].fPos[2] - 1.0f, g_aBGWindow[nCntWindow].fPos[1]);
			pVtx[nCntWindow * 4 + 3].tex = D3DXVECTOR2(g_aBGWindow[nCntWindow].fPos[2], g_aBGWindow[nCntWindow].fPos[1]);

		}
	}

	//頂点BUFFERをアンロックする
	g_pVtxBuffBGWindow->Unlock();
}
//=========================================================================================================//
// * 背景の描画処理4	描画処理
//=========================================================================================================//
void DrawBGWindow(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntWindow;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBGWindow, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_NUM);
	// ポリゴンの描画
	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aBGWindow[nCntWindow].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBGWindow[g_aBGWindow[nCntWindow].nType]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWindow * 4, WINDOW_PRIMITIVE_NUM);
		}
	}

}
//=============================================================================
// 弾の設定処理
//=============================================================================
void SetBGWindow(D3DXVECTOR3 pos, COLLAR col,float fSpeed, float fPos, int nLengthX, int nLengthY, int nType, WINDOWSTATE state)
{
	int nCntWindow;
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

							//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBGWindow->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aBGWindow[nCntWindow].bUse == false)
		{
			g_aBGWindow[nCntWindow].pos = pos;
			g_aBGWindow[nCntWindow].nLengthX = nLengthX;
			g_aBGWindow[nCntWindow].nLengthY = nLengthY;
			g_aBGWindow[nCntWindow].nType = nType;
			g_aBGWindow[nCntWindow].state = state;
			g_aBGWindow[nCntWindow].fSpeed = fSpeed;
			g_aBGWindow[nCntWindow].fPos[0] = fPos;
			g_aBGWindow[nCntWindow].fPos[1] = 1.0f;
			g_aBGWindow[nCntWindow].fPos[2] = 1.0f;
			g_aBGWindow[nCntWindow].bUse = true;

			pVtx[nCntWindow * 4].pos = D3DXVECTOR3(g_aBGWindow[nCntWindow].pos.x - g_aBGWindow[nCntWindow].nLengthX, g_aBGWindow[nCntWindow].pos.y - g_aBGWindow[nCntWindow].nLengthY, 0.0f);
			pVtx[nCntWindow * 4 + 1].pos = D3DXVECTOR3(g_aBGWindow[nCntWindow].pos.x + g_aBGWindow[nCntWindow].nLengthX, g_aBGWindow[nCntWindow].pos.y - g_aBGWindow[nCntWindow].nLengthY, 0.0f);
			pVtx[nCntWindow * 4 + 2].pos = D3DXVECTOR3(g_aBGWindow[nCntWindow].pos.x - g_aBGWindow[nCntWindow].nLengthX, g_aBGWindow[nCntWindow].pos.y + g_aBGWindow[nCntWindow].nLengthY, 0.0f);
			pVtx[nCntWindow * 4 + 3].pos = D3DXVECTOR3(g_aBGWindow[nCntWindow].pos.x + g_aBGWindow[nCntWindow].nLengthX, g_aBGWindow[nCntWindow].pos.y + g_aBGWindow[nCntWindow].nLengthY, 0.0f);

			pVtx[nCntWindow * 4].tex = D3DXVECTOR2(0.0f, g_aBGWindow[nCntWindow].fPos[1] - g_aBGWindow[nCntWindow].fPos[0]);
			pVtx[nCntWindow * 4 + 1].tex = D3DXVECTOR2(1.0f, g_aBGWindow[nCntWindow].fPos[1] - g_aBGWindow[nCntWindow].fPos[0]);
			pVtx[nCntWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, g_aBGWindow[nCntWindow].fPos[1]);
			pVtx[nCntWindow * 4 + 3].tex = D3DXVECTOR2(1.0f, g_aBGWindow[nCntWindow].fPos[1]);

			g_aBGWindow[nCntWindow].col.colA = col.colA;
			g_aBGWindow[nCntWindow].col.colB = col.colB;
			g_aBGWindow[nCntWindow].col.colG = col.colG;
			g_aBGWindow[nCntWindow].col.colR = col.colR;

			pVtx[nCntWindow * 4].col = D3DCOLOR_RGBA(col.colR, col.colG, col.colB, col.colA);	// A = 透明度
			pVtx[nCntWindow * 4 + 1].col = D3DCOLOR_RGBA(col.colR, col.colG, col.colB, col.colA);
			pVtx[nCntWindow * 4 + 2].col = D3DCOLOR_RGBA(col.colR, col.colG, col.colB, col.colA);
			pVtx[nCntWindow * 4 + 3].col = D3DCOLOR_RGBA(col.colR, col.colG, col.colB, col.colA);
			break;
		}
	}
	//頂点BUFFERをアンロックする
	g_pVtxBuffBGWindow->Unlock();
}
//=============================================================================
// 弾の設定処理
//=============================================================================
void BreakBGWindow(void)
{//全ウィンドウ破棄
	int nCntWindow;

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aBGWindow[nCntWindow].bUse == true)
		{
			g_aBGWindow[nCntWindow].bUse = false;
		}
	}
}
