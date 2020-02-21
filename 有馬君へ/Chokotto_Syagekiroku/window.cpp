//=========================================================================================================//
//
// window処理 [window.cpp]
// Author : Ryou Sugimoto
//
//=========================================================================================================//
#include "main.h"
#include "game.h"
#include "input.h"

//*********************************************************************************************************//
// マクロ定義
//*********************************************************************************************************//
#define WINDOW_VERTEX_NUM			(4)					// 頂点の数
#define WINDOW_PRIMITIVE_NUM		(2)					// プリミティブの数
#define WINDOW_MAX_TEXTURE			(26)
#define WINDOW_TextureName0		"data/TEXTURE/press_enter.png"		//タイトル エンターキー
#define WINDOW_TextureName1		"data/TEXTURE/now loading"			//NOW LOADING
#define WINDOW_TextureName2		"data/TEXTURE/SELECTWINDOW.png"		//Title 難易度選択
#define WINDOW_TextureName3		"data/TEXTURE/gameover.jpg"			//ゲームオーバー0
#define WINDOW_TextureName4		"data/TEXTURE/gameover000.png"		//ゲームオーバー1
#define WINDOW_TextureName5		"data/TEXTURE/gameclear_logo.png"	//ゲームクリアロゴ
#define WINDOW_TextureName6		"data/TEXTURE/GameClear000.jpg"		//ゲームクリアBG
#define WINDOW_TextureName7		"data/TEXTURE/Player002.png"		//フナッシー！残機
#define WINDOW_TextureName8		"data/TEXTURE/START.png"			//Title　STARTロゴ
#define WINDOW_TextureName9		"data/TEXTURE/TUTORIAL001.png"		//Title　TUTORIALロゴ
#define WINDOW_TextureName10	"data/TEXTURE/END.png"				//Title　ＥＮＤロゴ
#define WINDOW_TextureName11	"data/TEXTURE/Easy.png"				//Title　ＥＡＳＹロゴ
#define WINDOW_TextureName12	"data/TEXTURE/Normal.png"			//Title　NORMALロゴ
#define WINDOW_TextureName13	"data/TEXTURE/Hard.png"				//Title　ＨＡＲＤロゴ
#define WINDOW_TextureName14	"data/TEXTURE/Lunatic.png"			//Title　LUNATICロゴ
#define WINDOW_TextureName15	"data/TEXTURE/pause100.png"			//PAUSE枠
#define WINDOW_TextureName16	"data/TEXTURE/pause000.png"			//PAUSEロゴ
#define WINDOW_TextureName17	"data/TEXTURE/pause001.png"			//PAUSEロゴ
#define WINDOW_TextureName18	"data/TEXTURE/pause002.png"			//PAUSEロゴ
#define WINDOW_TextureName19	"data/TEXTURE/Guard000.png"			//GUARD　使用状態
#define WINDOW_TextureName20	"data/TEXTURE/Guard001.png"			//GUARD　チャージ済み
#define WINDOW_TextureName21	"data/TEXTURE/FontWindow.png"		//FONTのための枠
#define WINDOW_TextureName22	"data/TEXTURE/BOSS003.png"			//Gameボス
#define WINDOW_TextureName23	"data/TEXTURE/BOSS_Change004.png"	//GameボスMODEChange時
#define WINDOW_TextureName24	""									//マテリアルの色
#define	MAX_WINDOW			(24)		// windowの最大数
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
	float fSteep;			//角度
	WINDOWSTATE state;
	WINDOWUSE use;
	int nCounterState[2];	//エネミーの状態管理用
	bool bUse;
}Window;

//*********************************************************************************************************//
// グローバル変数
//*********************************************************************************************************//
LPDIRECT3DTEXTURE9		g_pTextureWindow[WINDOW_MAX_TEXTURE] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWindow = NULL;	// 頂点バッファへのポインタ
Window g_aWindow[MAX_WINDOW];

//=========================================================================================================//
// * 背景の描画処理1	初期化処理
//=========================================================================================================//
void InitWindow(void)
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
		g_aWindow[nCntWindow].bUse = false;
	}

	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName0, &g_pTextureWindow[0]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName1, &g_pTextureWindow[1]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName2, &g_pTextureWindow[2]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName3, &g_pTextureWindow[3]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName4, &g_pTextureWindow[4]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName5, &g_pTextureWindow[5]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName6, &g_pTextureWindow[6]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName7, &g_pTextureWindow[7]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName8, &g_pTextureWindow[8]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName9, &g_pTextureWindow[9]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName10, &g_pTextureWindow[10]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName11, &g_pTextureWindow[11]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName12, &g_pTextureWindow[12]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName13, &g_pTextureWindow[13]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName14, &g_pTextureWindow[14]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName15, &g_pTextureWindow[15]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName16, &g_pTextureWindow[16]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName17, &g_pTextureWindow[17]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName18, &g_pTextureWindow[18]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName19, &g_pTextureWindow[19]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName20, &g_pTextureWindow[20]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName21, &g_pTextureWindow[21]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName22, &g_pTextureWindow[22]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName23, &g_pTextureWindow[23]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName24, &g_pTextureWindow[24]);

	//頂点BUFFERの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * WINDOW_VERTEX_NUM * MAX_WINDOW,		//確保するBUFFERのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffWindow,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ
							//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWindow->Lock(0, 0, (void**)&pVtx, 0);
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
	g_pVtxBuffWindow->Unlock();
}
//=========================================================================================================//
// * 背景の描画処理2	終了処理
//=========================================================================================================//
void UninitWindow(void)
{//テクスチャの破棄
	int nCntTexture;

	for (nCntTexture = 0; nCntTexture < WINDOW_MAX_TEXTURE; nCntTexture++)
	{
		if (g_pTextureWindow[nCntTexture] != NULL)
		{
			g_pTextureWindow[nCntTexture]->Release();
			g_pTextureWindow[nCntTexture] = NULL;
		}
	}
	if (g_pVtxBuffWindow != NULL)
	{
		g_pVtxBuffWindow->Release();
		g_pVtxBuffWindow = NULL;
	}
}
//=========================================================================================================//
// * 背景の描画処理3	更新処理
//=========================================================================================================//
void UpdateWindow(void)
{
	int nCntWindow;
	static float fStealth = 0.02f;
	VERTEX_2D *pVtx;		//頂点情報へのポインタ
	PLAYER *pPlayer;

	pPlayer = GetPlayer();

							//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWindow->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aWindow[nCntWindow].bUse == true && g_aWindow[nCntWindow].state != WINDOWSTATE_NORMAL)
		{
			switch (g_aWindow[nCntWindow].state)
			{
			case WINDOWSTATE_STEALTH://設定された透明度から明るくなり255で通常になる
				g_aWindow[nCntWindow].col.a += 0.012f;
				if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_UP) == true || GetKeyboardTrigger(DIK_DOWN) == true || GetKeyboardTrigger(DIK_Z) == true || g_aWindow[nCntWindow].col.a >= 1.0f)
				{//キー入力でスキップ
					g_aWindow[nCntWindow].col.a = 1.0f;
					g_aWindow[nCntWindow].state = WINDOWSTATE_NORMAL;
				}
				break;
			case WINDOWSTATE_FADEIN:	//Stealthの早送りなし
				g_aWindow[nCntWindow].col.a += 0.012f;
				if (g_aWindow[nCntWindow].col.a >= 1.0f)
				{
					g_aWindow[nCntWindow].col.a = 1.0f;
					g_aWindow[nCntWindow].state = WINDOWSTATE_NORMAL;
				}
				break;
			case WINDOWSTATE_FADEOUT://設定された透明度から暗くなり0で消える
				g_aWindow[nCntWindow].col.a -= 0.01f;
				if (g_aWindow[nCntWindow].col.a <= 0.0f)
				{
					g_aWindow[nCntWindow].col.a = 0.0f;
					g_aWindow[nCntWindow].bUse = false;
				}
				break;

			case WINDOWSTATE_FLASH://点滅で表示
				g_aWindow[nCntWindow].nCounterState[0]++;
				if (g_aWindow[nCntWindow].nCounterState[1] % 2 == 0 && g_aWindow[nCntWindow].nCounterState[0] % 45 == 0)
				{
					g_aWindow[nCntWindow].col.a = 1.0f;
					g_aWindow[nCntWindow].nCounterState[1]++;
				}
				else if (g_aWindow[nCntWindow].nCounterState[0] % 45 == 0)
				{
					g_aWindow[nCntWindow].col.a = 0.0f;
					g_aWindow[nCntWindow].nCounterState[1]++;
				}
				break;

			case WINDOWSTATE_SELECTON://SELECT処理で使用され、選択されているとき
				g_aWindow[nCntWindow].col.r += fStealth;
				g_aWindow[nCntWindow].col.g += fStealth;
				g_aWindow[nCntWindow].col.b += fStealth;
				if (g_aWindow[nCntWindow].col.r >= 1.0f || g_aWindow[nCntWindow].col.r <= 0.22f)
				{//明るさが最大, 最小になったら反転
					fStealth *= -1.0f;
				}
				break;

			case WINDOWSTATE_SELECTOFF://SELECT処理で使用され、選択されていないとき
				g_aWindow[nCntWindow].col.r = 0.23f;
				g_aWindow[nCntWindow].col.g = 0.23f;
				g_aWindow[nCntWindow].col.b = 0.23f;
				break;

			case WINDOWSTATE_GUARDON:	//GUARD使用状態
				g_aWindow[nCntWindow].pos = D3DXVECTOR3(pPlayer[0].pos.x, pPlayer[0].pos.y + PLAYER_POSY3, 0.0f);
				g_aWindow[nCntWindow].fSteep -= 0.05f;
				if (g_aWindow[nCntWindow].nCounterState[0] != 0)
				{
					g_aWindow[nCntWindow].nCounterState[0]--;
					if (g_aWindow[nCntWindow].nCounterState[0] == 0)
					{
						g_aWindow[nCntWindow].col.r = 1.0f;
						g_aWindow[nCntWindow].col.g = 1.0f;
						g_aWindow[nCntWindow].col.b = 1.0f;
					}
				}
				break;

			case WINDOWSTATE_GUARDOFF:
				g_aWindow[nCntWindow].fSteep -= 0.02f;
				g_aWindow[nCntWindow].pos = D3DXVECTOR3(pPlayer[0].pos.x, pPlayer[0].pos.y + PLAYER_POSY3, 0.0f);
				if (g_aWindow[nCntWindow].col.a != 0.4f)
				{
					g_aWindow[nCntWindow].col.a += 0.02f;
					if (g_aWindow[nCntWindow].col.a > 0.399f)
					{
						g_aWindow[nCntWindow].col.a = 0.4f;
					}
				}
				break;
			case WINDOWSTATE_ENEMYCHANGE_FLOW:
				if (g_aWindow[nCntWindow].nCounterState[0] == 0)
				{//左から右に流れながら表示
					g_aWindow[nCntWindow].col.a += 0.05f;
					g_aWindow[nCntWindow].pos.x += 20.0f;
					if (g_aWindow[nCntWindow].col.a >= 1.0f)
					{
						g_aWindow[nCntWindow].col.a = 1.0f;
						g_aWindow[nCntWindow].nCounterState[0] = 1;
					}
				}
				else
				{//時間経過で
					g_aWindow[nCntWindow].nCounterState[0]++;
					if (g_aWindow[nCntWindow].nCounterState[0] > 60)
					{//さらに時間経過で
						g_aWindow[nCntWindow].col.a -= 0.029f;
						g_aWindow[nCntWindow].pos.x += 8.0f;
						if (g_aWindow[nCntWindow].col.a <= 0.0f)
						{//右に流れながら消える
							g_aWindow[nCntWindow].col.a = 0.0f;
							g_aWindow[nCntWindow].bUse = false;
						}
					}
				}
				break;
			case WINDOWSTATE_ENEMYCHANGE_ZOOM:
				if (g_aWindow[nCntWindow].nCounterState[0] == 0)
				{//左から右に流れながら表示
					g_aWindow[nCntWindow].col.a += 0.05f;
					g_aWindow[nCntWindow].pos.x += 20.0f;
					if (g_aWindow[nCntWindow].col.a >= 1.0f)
					{
						g_aWindow[nCntWindow].col.a = 1.0f;
						g_aWindow[nCntWindow].nCounterState[0] = 1;
					}
				}
				else
				{//時間経過で
					g_aWindow[nCntWindow].nCounterState[0]++;
					if (g_aWindow[nCntWindow].nCounterState[0] > 60)
					{//さらに時間経過で
						g_aWindow[nCntWindow].col.a -= 0.029f;
						g_aWindow[nCntWindow].nLengthX += 3;
						g_aWindow[nCntWindow].nLengthY += 3;
						if (g_aWindow[nCntWindow].col.a <= 0.0f)
						{//ズームしながら消える
							g_aWindow[nCntWindow].col.a = 0.0f;
							g_aWindow[nCntWindow].bUse = false;
						}
					}
				}
				break;
			}

			pVtx[nCntWindow * 4].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x + (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthY) + (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthX), g_aWindow[nCntWindow].pos.y - (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthX) + (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthY), 0.0f);
			pVtx[nCntWindow * 4 + 1].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x + (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthY) - (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthX), g_aWindow[nCntWindow].pos.y + (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthX) + (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthY), 0.0f);
			pVtx[nCntWindow * 4 + 2].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x - (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthY) + (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthX), g_aWindow[nCntWindow].pos.y - (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthX) - (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthY), 0.0f);
			pVtx[nCntWindow * 4 + 3].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x - (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthY) - (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthX), g_aWindow[nCntWindow].pos.y + (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthX) - (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthY), 0.0f);

			pVtx[nCntWindow * 4].col = g_aWindow[nCntWindow].col;	// A = 透明度
			pVtx[nCntWindow * 4 + 1].col = g_aWindow[nCntWindow].col;
			pVtx[nCntWindow * 4 + 2].col = g_aWindow[nCntWindow].col;
			pVtx[nCntWindow * 4 + 3].col = g_aWindow[nCntWindow].col;
		}
	}
	//頂点BUFFERをアンロックする
	g_pVtxBuffWindow->Unlock();
}
//=========================================================================================================//
// * 背景の描画処理4	描画処理
//=========================================================================================================//
void DrawWindow(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntWindow;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffWindow, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_NUM);
	// ポリゴンの描画
	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aWindow[nCntWindow].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureWindow[g_aWindow[nCntWindow].nType]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWindow * 4, WINDOW_PRIMITIVE_NUM);
		}
	}

}
//=============================================================================
// 弾の設定処理
//=============================================================================
void SetWindow(D3DXVECTOR3 pos, D3DXCOLOR col, int nLengthX, int nLengthY, int nType, int nCntUse, int nEnemy, WINDOWSTATE state, WINDOWUSE use)
{
	int nCntWindow;
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

							//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWindow->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntWindow = 1; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (use == WINDOWUSE_GUARD)
		{
			nCntWindow = 0;
			if (g_aWindow[nCntWindow].bUse == true)
			{
				break;
			}
		}
		if (g_aWindow[nCntWindow].bUse == false)
		{
			g_aWindow[nCntWindow].pos = pos;
			g_aWindow[nCntWindow].nLengthX = nLengthX;
			g_aWindow[nCntWindow].nLengthY = nLengthY;
			g_aWindow[nCntWindow].nType = nType;
			g_aWindow[nCntWindow].state = state;
			g_aWindow[nCntWindow].use = use;
			g_aWindow[nCntWindow].nCounterState[0] = nCntUse;
			g_aWindow[nCntWindow].nCounterState[1] = 1;
			g_aWindow[nCntWindow].bUse = true;

			g_aWindow[nCntWindow].fSteep = D3DX_PI;
			/*pVtx[nCntWindow * 4].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x - g_aWindow[nCntWindow].nLengthX, g_aWindow[nCntWindow].pos.y - g_aWindow[nCntWindow].nLengthY, 0.0f);
			pVtx[nCntWindow * 4 + 1].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x + g_aWindow[nCntWindow].nLengthX, g_aWindow[nCntWindow].pos.y - g_aWindow[nCntWindow].nLengthY, 0.0f);
			pVtx[nCntWindow * 4 + 2].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x - g_aWindow[nCntWindow].nLengthX, g_aWindow[nCntWindow].pos.y + g_aWindow[nCntWindow].nLengthY, 0.0f);
			pVtx[nCntWindow * 4 + 3].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x + g_aWindow[nCntWindow].nLengthX, g_aWindow[nCntWindow].pos.y + g_aWindow[nCntWindow].nLengthY, 0.0f);*/

			pVtx[nCntWindow * 4].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x + (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthY) + (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthX), g_aWindow[nCntWindow].pos.y - (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthX) + (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthY), 0.0f);
			pVtx[nCntWindow * 4 + 1].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x + (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthY) - (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthX), g_aWindow[nCntWindow].pos.y + (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthX) + (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthY), 0.0f);
			pVtx[nCntWindow * 4 + 2].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x - (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthY) + (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthX), g_aWindow[nCntWindow].pos.y - (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthX) - (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthY), 0.0f);
			pVtx[nCntWindow * 4 + 3].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x - (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthY) - (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthX), g_aWindow[nCntWindow].pos.y + (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthX) - (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthY), 0.0f);

			g_aWindow[nCntWindow].col = col;
			pVtx[nCntWindow * 4].col = col;	// A = 透明度
			pVtx[nCntWindow * 4 + 1].col = col;
			pVtx[nCntWindow * 4 + 2].col = col;
			pVtx[nCntWindow * 4 + 3].col = col;
			if (use == WINDOWUSE_ENEMY)
			{//テクスチャ座標設定
				pVtx[nCntWindow * 4].tex = D3DXVECTOR2((nEnemy % 2) * 0.5f, (nEnemy / 2) * 0.2f);
				pVtx[nCntWindow * 4 + 1].tex = D3DXVECTOR2((nEnemy % 2) * 0.5f + 0.5f, (nEnemy / 2) * 0.2f);
				pVtx[nCntWindow * 4 + 2].tex = D3DXVECTOR2((nEnemy % 2) * 0.5f, (nEnemy / 2) * 0.2f + 0.2f);
				pVtx[nCntWindow * 4 + 3].tex = D3DXVECTOR2((nEnemy % 2) * 0.5f + 0.5f, (nEnemy / 2) * 0.2f + 0.2f);
			}
			else
			{//テクスチャ座標設定
				pVtx[nCntWindow * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCntWindow * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[nCntWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[nCntWindow * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
			break;
		}
	}
	//頂点BUFFERをアンロックする
	g_pVtxBuffWindow->Unlock();
}
//=============================================================================
// windowの破壊
//=============================================================================
void BreakWindow(void)
{//全ウィンドウ破棄
	int nCntWindow;

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aWindow[nCntWindow].bUse == true)
		{
			g_aWindow[nCntWindow].bUse = false;
		}
	}
}
//=============================================================================
// 残機破壊
//=============================================================================
bool BreakAirport(void)
{//残機が残っていればtrue
	bool bRespawn = false;
	int nCntWindow;

	for (nCntWindow = MAX_WINDOW; nCntWindow > 0; nCntWindow--)
	{
		if (g_aWindow[nCntWindow - 1].bUse == true && g_aWindow[nCntWindow - 1].use == WINDOWUSE_RESIDUALAIRPORT && g_aWindow[nCntWindow - 1].state == WINDOWSTATE_NORMAL)
		{
			g_aWindow[nCntWindow - 1].state = WINDOWSTATE_FADEOUT;
			bRespawn = true;
			break;
		}
	}

	return bRespawn;
}
//=============================================================================
// 選択処理
//=============================================================================
void SelectWindow(int nNumber)
{
	int nCntWindow;

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aWindow[nCntWindow].use == WINDOWUSE_SELECT && g_aWindow[nCntWindow].bUse == true)
		{
			if (g_aWindow[nCntWindow].nCounterState[0] == nNumber)
			{
				g_aWindow[nCntWindow].state = WINDOWSTATE_SELECTON;
			}
			else
			{
				g_aWindow[nCntWindow].state = WINDOWSTATE_SELECTOFF;
			}
		}
	}
}
//=============================================================================
// 指定したwindow分破壊
//=============================================================================
void BackWindow(int nNumber)
{
	int nCntWindow;			//windowを数える
	int nBreak = nNumber;	//nNumberいじったらどうなるかわからんし...

	for (nCntWindow = MAX_WINDOW; nCntWindow > 0; nCntWindow--)
	{//最大でも全部終わったら終了
		if (g_aWindow[nCntWindow - 1].bUse == true)
		{//後ろから見てtrueになっとるやつを殺す
			g_aWindow[nCntWindow - 1].bUse = false;
			nBreak--;
			if (nBreak < 1)
			{//0以下になったら終わり
				break;
			}
		}
	}
}
//=============================================================================
// 指定したwindow分破壊
//=============================================================================
void ChangeGuard(int nUse)
{
	int nCntWindow;			//windowを数える

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{//最大でも全部終わったら終了
		if (g_aWindow[nCntWindow].bUse == true && g_aWindow[nCntWindow].use == WINDOWUSE_GUARD)
		{//GUARDのポリゴンの状態変更
			if (nUse == 0)
			{//GUARD成功
				g_aWindow[nCntWindow].col.r = 1.0f;
				g_aWindow[nCntWindow].col.g = 0.99f;
				g_aWindow[nCntWindow].col.b = 0.03f;

				g_aWindow[nCntWindow].nCounterState[0] = 5;
			}
			else if (nUse == 1)
			{
				g_aWindow[nCntWindow].state = WINDOWSTATE_GUARDON;
				g_aWindow[nCntWindow].nLengthX = 32;
				g_aWindow[nCntWindow].nLengthY = 32;
				g_aWindow[nCntWindow].col.a = 0.85f;
				g_aWindow[nCntWindow].nType = 19;
			}
			else if(nUse == 2)
			{
				g_aWindow[nCntWindow].bUse = false;
			}
		}
	}
}
//=============================================================================
// 50音表示の枠移動処理
//=============================================================================
void FontWindowMove(int nWide, int nHeight)
{
	int nCntWindow;

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aWindow[nCntWindow].use == WINDOWUSE_FONT)
		{
			if (nHeight == 5)
			{//ひら カナ 終わりの場合
				g_aWindow[nCntWindow].nLengthY = SCREEN_HEIGHT / 30;
				g_aWindow[nCntWindow].pos.y = 4.9f * SCREEN_HEIGHT * 0.118f + SCREEN_HEIGHT * 0.281f;
				if (nWide == 0)
				{
					g_aWindow[nCntWindow].nLengthX = SCREEN_WIDTH / 20;
					g_aWindow[nCntWindow].pos.x = SCREEN_WIDTH * 0.505f;
				}
				else if (nWide == 1)
				{
					g_aWindow[nCntWindow].nLengthX = SCREEN_WIDTH / 20;
					g_aWindow[nCntWindow].pos.x = SCREEN_WIDTH * 0.608f;
				}
				else if (nWide == 2)
				{
					g_aWindow[nCntWindow].nLengthX = SCREEN_WIDTH / 17;
					g_aWindow[nCntWindow].pos.x = SCREEN_WIDTH * 0.7305f;
				}
			}
			else
			{//ひら カナ 終わり 以外
				g_aWindow[nCntWindow].nLengthX = SCREEN_WIDTH / 38;
				g_aWindow[nCntWindow].nLengthY = SCREEN_HEIGHT / 16;
				g_aWindow[nCntWindow].pos.x = nWide * SCREEN_WIDTH * 0.0525f + SCREEN_WIDTH * 0.2365f;
				g_aWindow[nCntWindow].pos.y = nHeight * SCREEN_HEIGHT * 0.118f + SCREEN_HEIGHT * 0.2815f;
			}
		}
	}
}