//=============================================================================
//
// フェード処理 [fade.cpp]
// Author : 
//
//=============================================================================
#include "fade.h"
#include "main.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Fade_TextureName	"data/TEXTURE/Load000.jpg"				//FADEのTEXTURE名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexFade(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureFade = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;	// 頂点バッファへのポインタ
FADE					g_fade;					// フェード状態
MODE					g_modeNext;				// 次のモード
D3DXCOLOR				g_colorFade;			// フェード色
int						g_nTypeFade;			//画像と更新内容

//=============================================================================
// 初期化処理
//=============================================================================
void InitFade(MODE modeNext)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_fade = FADE_IN;	//初期真っ黒画面
	g_modeNext = modeNext;
	g_colorFade = D3DXCOLOR{ 0.0f,0.0f,0.0f,1.0f };
	g_nTypeFade = 0;

	D3DXCreateTextureFromFile(pDevice, Fade_TextureName, &g_pTextureFade);	//タイトル

	// 値の初期化
	//頂点BUFFERの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,		//確保するBUFFERのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ
							//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//X,Y,0.0f
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラー設定
	pVtx[0].col = g_colorFade;	// A = 透明度
	pVtx[1].col = g_colorFade;
	pVtx[2].col = g_colorFade;
	pVtx[3].col = g_colorFade;

	//テクスチャ座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点BUFFERをアンロックする
	g_pVtxBuffFade->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitFade(void)
{
	if (g_pTextureFade != NULL)
	{
		g_pTextureFade->Release();
		g_pTextureFade = NULL;
	}
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateFade(void)
{
	if (g_fade != FADE_NONE)
	{//フェード状態
		VERTEX_2D *pVtx;		//頂点情報へのポインタ
		//頂点BUFFERをロックし、頂点情報へのポインタを取得
		g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

		if (g_fade == FADE_IN)
		{//明るくな～る
			g_colorFade.a -= 0.03f;
			if (g_colorFade.a <= 0.0f)
			{//完全に透明になったら
				g_colorFade.a = 0.0f;
				g_fade = FADE_NONE;
			}
		}
		else if(g_fade == FADE_OUT)
		{//暗くな～る
			g_colorFade.a += 0.03f;
			if (g_colorFade.a >= 1.0f)
			{//完全に不透明になったら
				g_colorFade.a = 1.0f;
				SetMode(g_modeNext);

				if (g_nTypeFade == 0)
				{//そのまま晴れ
					g_fade = FADE_IN;
				}
				else
				{
					g_fade = FADE_ENTER;
					LoadBreakWindow();
					SetFadeWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.9f, 0.0f), D3DXCOLOR{ 1.0f,0.99f,0.04f, 1.0f }, 150, 30, 0, 0, WINDOWSTATE_FLASH, WINDOWUSE_LOAD);
				}
			}
		}
		else if (g_fade == FADE_ENTER && GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_Z) == true)
		{
			g_fade = FADE_IN;
			LoadBreakWindow();
		}
		//カラー設定
		pVtx[0].col = g_colorFade;	// A = 透明度
		pVtx[1].col = g_colorFade;
		pVtx[2].col = g_colorFade;
		pVtx[3].col = g_colorFade;

		//頂点BUFFERをアンロックする
		g_pVtxBuffFade->Unlock();
	}
}

//=============================================================================
// タイトル画面
//=============================================================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureFade);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexFade(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成


	// 頂点バッファの情報を設定

}

//=============================================================================
// フェードの設定
//=============================================================================
void SetFade(MODE modeNext, int nTypeFade)
{
	if (g_fade == FADE_NONE)
	{
		g_nTypeFade = nTypeFade;
		g_fade = FADE_OUT;		//暗くな～る
		g_modeNext = modeNext;	//切り替えの予約
		if (nTypeFade == 0)
		{
			g_colorFade = D3DXCOLOR{ 0.0f,0.0f,0.0f,0.0f };
		}
		else
		{
		g_colorFade = D3DXCOLOR{ 1.0f,1.0f,1.0f,0.0f };
		SetFadeWindow(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.9f, 0.0f), D3DXCOLOR{ 0.0f,0.0f,0.0f, 0.0f }, 150, 30, 1, 0, WINDOWSTATE_FADEIN, WINDOWUSE_LOAD);
		}
	}
}

//=============================================================================
// フェードの取得
//=============================================================================
FADE *GetFade(void)
{
	return &g_fade;
}

