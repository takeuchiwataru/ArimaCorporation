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
#define WINDOW_MAX_TEXTURE			(5)
#define WINDOW_TextureName0		"data/TEXTURE/taitol001.jpg"		//GAME スコア板
#define WINDOW_TextureName1		"data/TEXTURE/TUTORIAL000.jpg"		//チュートリアル
#define WINDOW_TextureName2		"data/TEXTURE/difficult000.png"		//Result　難易度ロゴ
#define WINDOW_TextureName3		"data/TEXTURE/RankingWindow000.png"	//Result　SCORE枠
#define WINDOW_TextureName4		"data/TEXTURE/RankingNumber000.png"	//Result　王冠
#define	MAX_WINDOW			(16)		// windowの最大数
#define HALFEXPLOSION		(30)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXCOLOR col;
	int nLengthX;
	int nLengthY;
	int nType;
	WINDOWSTATE state;
	WINDOWUSE use;
	int nCounterState[2];	//エネミーの状態管理用
	bool bUse;
}Window;

//*********************************************************************************************************//
// グローバル変数
//*********************************************************************************************************//
LPDIRECT3DTEXTURE9		g_pTextureGameWindow[WINDOW_MAX_TEXTURE] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameWindow = NULL;	// 頂点バッファへのポインタ
Window					g_aGameWindow[MAX_WINDOW];
int						g_nRank;

//=========================================================================================================//
// * 背景の描画処理1	初期化処理
//=========================================================================================================//
void InitGameWindow(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	int nCntWindow;
	g_nRank = 0;

	// 弾の情報の初期化
	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		//g_aWindow[nCntWindow].pos = { 0.0f, 0.0f, 0.0f };
		//g_aWindow[nCntWindow].nLengthX = 0;
		//g_aWindow[nCntWindow].nLengthY = 0;
		//g_aWindow[nCntWindow].nType = 0;
		//g_aWindow[nCntWindow].col = { 1.0f, 1.0f, 1.0f, };
		g_aGameWindow[nCntWindow].bUse = false;
	}

	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName0, &g_pTextureGameWindow[0]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName1, &g_pTextureGameWindow[1]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName2, &g_pTextureGameWindow[2]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName3, &g_pTextureGameWindow[3]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName4, &g_pTextureGameWindow[4]);

	//頂点BUFFERの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * WINDOW_VERTEX_NUM * MAX_WINDOW,		//確保するBUFFERのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffGameWindow,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ
	//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGameWindow->Lock(0, 0, (void**)&pVtx, 0);
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
	g_pVtxBuffGameWindow->Unlock();
}
//=========================================================================================================//
// * 背景の描画処理2	終了処理
//=========================================================================================================//
void UninitGameWindow(void)
{//テクスチャの破棄
	int nCntTexture;

	for (nCntTexture = 0; nCntTexture < WINDOW_MAX_TEXTURE; nCntTexture++)
	{
		if (g_pTextureGameWindow[nCntTexture] != NULL)
		{
			g_pTextureGameWindow[nCntTexture]->Release();
			g_pTextureGameWindow[nCntTexture] = NULL;
		}
	}
	if (g_pVtxBuffGameWindow != NULL)
	{
		g_pVtxBuffGameWindow->Release();
		g_pVtxBuffGameWindow = NULL;
	}
}
//=========================================================================================================//
// * 背景の描画処理3	更新処理
//=========================================================================================================//
void UpdateGameWindow(void)
{
	int nCntWindow;

	VERTEX_2D *pVtx;		//頂点情報へのポインタ
	//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGameWindow->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{//全ウィンドウ文回る
		if (g_aGameWindow[nCntWindow].bUse == true)
		{//使っていたら
			switch (g_aGameWindow[nCntWindow].state)
			{
			case WINDOWSTATE_RANKSET:
				g_aGameWindow[nCntWindow].col.a += 0.029f;
				g_aGameWindow[nCntWindow].pos.x -= 5.0f;
				if (g_aGameWindow[nCntWindow].col.a >= 1.0f)
				{
					g_aGameWindow[nCntWindow].col.a = 1.0f;
					g_aGameWindow[nCntWindow].state = WINDOWSTATE_NORMAL;
				}
				break;
			case WINDOWSTATE_RANKDELETE:
				g_aGameWindow[nCntWindow].col.a -= 0.029f;
				g_aGameWindow[nCntWindow].pos.x -= 5.0f;
				if (g_aGameWindow[nCntWindow].col.a <= 0.0f)
				{
					g_aGameWindow[nCntWindow].col.a = 0.0f;
					g_aGameWindow[nCntWindow].bUse = false;
				}
				break;
			}

			//頂点座標の設定
			pVtx[nCntWindow * 4].pos = D3DXVECTOR3(g_aGameWindow[nCntWindow].pos.x - g_aGameWindow[nCntWindow].nLengthX, g_aGameWindow[nCntWindow].pos.y - g_aGameWindow[nCntWindow].nLengthY, 0.0f);
			pVtx[nCntWindow * 4 + 1].pos = D3DXVECTOR3(g_aGameWindow[nCntWindow].pos.x + g_aGameWindow[nCntWindow].nLengthX, g_aGameWindow[nCntWindow].pos.y - g_aGameWindow[nCntWindow].nLengthY, 0.0f);
			pVtx[nCntWindow * 4 + 2].pos = D3DXVECTOR3(g_aGameWindow[nCntWindow].pos.x - g_aGameWindow[nCntWindow].nLengthX, g_aGameWindow[nCntWindow].pos.y + g_aGameWindow[nCntWindow].nLengthY, 0.0f);
			pVtx[nCntWindow * 4 + 3].pos = D3DXVECTOR3(g_aGameWindow[nCntWindow].pos.x + g_aGameWindow[nCntWindow].nLengthX, g_aGameWindow[nCntWindow].pos.y + g_aGameWindow[nCntWindow].nLengthY, 0.0f);
			//カラー設定
			pVtx[nCntWindow * 4].col = g_aGameWindow[nCntWindow].col;	// A = 透明度
			pVtx[nCntWindow * 4 + 1].col = g_aGameWindow[nCntWindow].col;
			pVtx[nCntWindow * 4 + 2].col = g_aGameWindow[nCntWindow].col;
			pVtx[nCntWindow * 4 + 3].col = g_aGameWindow[nCntWindow].col;
		}
	}
	//頂点BUFFERをアンロックする
	g_pVtxBuffGameWindow->Unlock();
}
//=========================================================================================================//
// * 背景の描画処理4	描画処理
//=========================================================================================================//
void DrawGameWindow(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntWindow;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffGameWindow, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_NUM);
	// ポリゴンの描画
	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aGameWindow[nCntWindow].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureGameWindow[g_aGameWindow[nCntWindow].nType]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWindow * 4, WINDOW_PRIMITIVE_NUM);
		}
	}

}
//=============================================================================
// 弾の設定処理
//=============================================================================
void SetGameWindow(D3DXVECTOR3 pos, D3DXCOLOR col, int nLengthX, int nLengthY, int nType, WINDOWSTATE state, WINDOWUSE use)
{
	int nCntWindow;
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

							//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGameWindow->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aGameWindow[nCntWindow].bUse == false)
		{
			g_aGameWindow[nCntWindow].pos = pos;
			g_aGameWindow[nCntWindow].nLengthX = nLengthX;
			g_aGameWindow[nCntWindow].nLengthY = nLengthY;
			g_aGameWindow[nCntWindow].nType = nType;
			g_aGameWindow[nCntWindow].state = state;
			g_aGameWindow[nCntWindow].use = use;
			g_aGameWindow[nCntWindow].nCounterState[0] = 0;
			g_aGameWindow[nCntWindow].nCounterState[1] = 1;
			g_aGameWindow[nCntWindow].bUse = true;

			pVtx[nCntWindow * 4].pos = D3DXVECTOR3(g_aGameWindow[nCntWindow].pos.x - g_aGameWindow[nCntWindow].nLengthX, g_aGameWindow[nCntWindow].pos.y - g_aGameWindow[nCntWindow].nLengthY, 0.0f);
			pVtx[nCntWindow * 4 + 1].pos = D3DXVECTOR3(g_aGameWindow[nCntWindow].pos.x + g_aGameWindow[nCntWindow].nLengthX, g_aGameWindow[nCntWindow].pos.y - g_aGameWindow[nCntWindow].nLengthY, 0.0f);
			pVtx[nCntWindow * 4 + 2].pos = D3DXVECTOR3(g_aGameWindow[nCntWindow].pos.x - g_aGameWindow[nCntWindow].nLengthX, g_aGameWindow[nCntWindow].pos.y + g_aGameWindow[nCntWindow].nLengthY, 0.0f);
			pVtx[nCntWindow * 4 + 3].pos = D3DXVECTOR3(g_aGameWindow[nCntWindow].pos.x + g_aGameWindow[nCntWindow].nLengthX, g_aGameWindow[nCntWindow].pos.y + g_aGameWindow[nCntWindow].nLengthY, 0.0f);

			g_aGameWindow[nCntWindow].col = col;
			pVtx[nCntWindow * 4].col = g_aGameWindow[nCntWindow].col;// A = 透明度
			pVtx[nCntWindow * 4 + 1].col = g_aGameWindow[nCntWindow].col;
			pVtx[nCntWindow * 4 + 2].col = g_aGameWindow[nCntWindow].col;
			pVtx[nCntWindow * 4 + 3].col = g_aGameWindow[nCntWindow].col;

			if (g_aGameWindow[nCntWindow].use == WINDOWUSE_RANKING)
			{//難易度ロゴ
				Difficulty *pDifficulty = GetDifficulty();

				//テクスチャ座標設定
				pVtx[nCntWindow * 4].tex = D3DXVECTOR2(0.0f, pDifficulty[0].nNumber * 0.25f);
				pVtx[nCntWindow * 4 + 1].tex = D3DXVECTOR2(1.0f, pDifficulty[0].nNumber * 0.25f);
				pVtx[nCntWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, pDifficulty[0].nNumber * 0.25f + 0.25f);
				pVtx[nCntWindow * 4 + 3].tex = D3DXVECTOR2(1.0f, pDifficulty[0].nNumber * 0.25f + 0.25f);
			}
			else if (g_aGameWindow[nCntWindow].use == WINDOWUSE_RANKNUMBER)
			{//順位
			 //テクスチャ座標設定
				pVtx[nCntWindow * 4].tex = D3DXVECTOR2(0.0f, g_nRank * 0.2f);
				pVtx[nCntWindow * 4 + 1].tex = D3DXVECTOR2(1.0f, g_nRank * 0.2f);
				pVtx[nCntWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, g_nRank * 0.2f + 0.2f);
				pVtx[nCntWindow * 4 + 3].tex = D3DXVECTOR2(1.0f, g_nRank * 0.2f + 0.2f);
				g_nRank = (g_nRank + 1) % 5;
			}
			break;
		}
	}
	//頂点BUFFERをアンロックする
	g_pVtxBuffGameWindow->Unlock();
}
//=============================================================================
// window破壊処理
//=============================================================================
void BreakGameWindow(void)
{//全ウィンドウ破棄
	int nCntWindow;

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aGameWindow[nCntWindow].bUse == true)
		{
			g_aGameWindow[nCntWindow].bUse = false;
		}
	}
}
//=============================================================================
// 指定したwindow分破壊
//=============================================================================
void BackGameWindow(int nNumber)
{
	int nCntWindow;			//windowを数える
	int nBreak = nNumber;	//nNumberいじったらどうなるかわからんし...

	for (nCntWindow = MAX_WINDOW; nCntWindow > 0; nCntWindow--)
	{//最大でも全部終わったら終了
		if (g_aGameWindow[nCntWindow - 1].bUse == true)
		{//後ろから見てtrueになっとるやつを殺す
			g_aGameWindow[nCntWindow - 1].bUse = false;
			nBreak--;
			if (nBreak < 1)
			{//0以下になったら終わり
				break;
			}
		}
	}
}
//=============================================================================
// 全window状態変更処理
//=============================================================================
void BreakGameWindow(WINDOWSTATE state)
{//全ウィンドウ破棄
	int nCntWindow;

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aGameWindow[nCntWindow].bUse == true)
		{
			g_aGameWindow[nCntWindow].state = state ;
		}
	}
}
//=============================================================================
// 指定したwindowの変更
//=============================================================================
void ChangeGameWindow(int nNumber, WINDOWSTATE state)
{
	g_aGameWindow[nNumber].state = state;
}