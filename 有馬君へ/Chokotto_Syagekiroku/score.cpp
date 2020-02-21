//=============================================================================
//
// スコア処理 [score.cpp]
// Author : 
//
//=============================================================================
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCORE_VERTEX_NUM			(4)					// 頂点の数
#define SCORE_PRIMITIVE_NUM		(2)					// プリミティブの数
#define	TEXTURE_SCORE		"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名
#define MAX_NUMBER			(8)
#define MAX_SETSCORE		(6)

//*****************************************************************************
// 構造体定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexScore(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureScore = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;		// 頂点バッファへのポインタ
SCORE g_aScore[MAX_SETSCORE];
//=============================================================================
// 初期化処理
//=============================================================================
void InitScore(void)
{
	int nCntScore;
	int nCntScore1;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (nCntScore = 0; nCntScore < MAX_SETSCORE; nCntScore++)
	{
		g_aScore[nCntScore].fCntstate = 0.0f;
		g_aScore[nCntScore].bUse = false;
	}

	//頂点BUFFERの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * SCORE_VERTEX_NUM * MAX_NUMBER * MAX_SETSCORE,		//確保するBUFFERのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	// スコアの初期化
	VERTEX_2D *pVtx;		//頂点情報へのポインタ
							//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntScore1 = 0; nCntScore1 < MAX_SETSCORE; nCntScore1++)
	{
		for (nCntScore = 0; nCntScore < MAX_NUMBER; nCntScore++)
		{
			////頂点座標の設定
			//pVtx[nCntScore * 4].pos = D3DXVECTOR3(0, 0, 0.0f);
			//pVtx[nCntScore * 4 + 1].pos = D3DXVECTOR3(0, 0, 0.0f);
			//pVtx[nCntScore * 4 + 2].pos = D3DXVECTOR3(0, 0, 0.0f);
			//pVtx[nCntScore * 4 + 3].pos = D3DXVECTOR3(0, 0, 0.0f);

			pVtx[nCntScore * 4].rhw = 1.0f;
			pVtx[nCntScore * 4 + 1].rhw = 1.0f;
			pVtx[nCntScore * 4 + 2].rhw = 1.0f;
			pVtx[nCntScore * 4 + 3].rhw = 1.0f;

			//カラー設定
			pVtx[nCntScore * 4].col = D3DXCOLOR(1.0f, 0.99f, 0.04f, 1.0f);	// A = 透明度
			pVtx[nCntScore * 4 + 1].col = D3DXCOLOR(1.0f, 0.99f, 0.04f, 1.0f);
			pVtx[nCntScore * 4 + 2].col = D3DXCOLOR(1.0f, 0.99f, 0.04f, 1.0f);
			pVtx[nCntScore * 4 + 3].col = D3DXCOLOR(1.0f, 0.99f, 0.04f, 1.0f);

			////テクスチャ座標設定
			//pVtx[nCntScore * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
			//pVtx[nCntScore * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
			//pVtx[nCntScore * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
			//pVtx[nCntScore * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		pVtx += 4 * 8;
	}

	//頂点BUFFERをアンロックする
	g_pVtxBuffScore->Unlock();


	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
								TEXTURE_SCORE,		// ファイルの名前
								&g_pTextureScore);	// テクスチャへのポインタ

	// 頂点情報の作成
	MakeVertexScore(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitScore(void)
{
	BreakScore();
	if (g_pTextureScore != NULL)
	{// テクスチャの開放
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}
	if (g_pVtxBuffScore != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateScore(void)
{
	int nCntScore[2];
	int nScore;
	VERTEX_2D *pVtx;		//頂点情報へのポインタ
							//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore[0] = 0; nCntScore[0] < MAX_SETSCORE; nCntScore[0]++)
	{
		if (g_aScore[nCntScore[0]].bUse == true)
		{
			switch (g_aScore[nCntScore[0]].state)
			{//状態に応じて処理
			case SCORESTATE_FADE:
				g_aScore[nCntScore[0]].col.a += 0.029f;
				if (g_aScore[nCntScore[0]].col.a >= 1.0f)
				{
					g_aScore[nCntScore[0]].col.a = 1.0f;
					g_aScore[nCntScore[0]].state = SCORESTATE_NONE;
				}
				break;

			case SCORESTATE_SET:
				g_aScore[nCntScore[0]].col.a += 0.029f;
				g_aScore[nCntScore[0]].pos.y += 3.0f;
				if (g_aScore[nCntScore[0]].col.a >= 1.0f)
				{
					g_aScore[nCntScore[0]].col.a = 1.0f;
					g_aScore[nCntScore[0]].state = SCORESTATE_NONE;
				}
				break;

			case SCORESTATE_DELETE:
				g_aScore[nCntScore[0]].col.a -= 0.028f;
				g_aScore[nCntScore[0]].pos.y += 3.0f;
				if (g_aScore[nCntScore[0]].col.a <= 0.0f)
				{
					g_aScore[nCntScore[0]].bUse = false;
				}
				break;

			case SCORESTATE_RANKDOWN:
				if (g_aScore[nCntScore[0]].fCntstate <= 1.0f)
				{
					g_aScore[nCntScore[0]].fCntstate += 0.028f;
					g_aScore[nCntScore[0]].pos.y += 3.0f;
				}
				else
				{
					g_aScore[nCntScore[0]].fCntstate = 0.0f;
					g_aScore[nCntScore[0]].state = SCORESTATE_NONE;
				}
				break;

			case SCORESTATE_RANKSET:
				g_aScore[nCntScore[0]].col.a += 0.029f;
				g_aScore[nCntScore[0]].pos.x -= 5.0f;
				if (g_aScore[nCntScore[0]].col.a >= 1.0f)
				{
					g_aScore[nCntScore[0]].col.a = 1.0f;
					g_aScore[nCntScore[0]].state = SCORESTATE_NONE;
				}
				break;

			case SCORESTATE_RANKDELETE:
				g_aScore[nCntScore[0]].col.a -= 0.028f;
				//g_aScore[nCntScore[0]].pos.x -= 5.0f;
				if (g_aScore[nCntScore[0]].col.a <= 0.0f)
				{
					g_aScore[nCntScore[0]].bUse = false;
				}
				break;

			default:
				break;
			}
			nScore = g_aScore[nCntScore[0]].g_nScore;
			for (nCntScore[1] = 0; nCntScore[1] < MAX_NUMBER; nCntScore[1]++)
			{//8桁分ポリゴンを生成
			 //カラー設定
				pVtx[nCntScore[1] * 4].col = D3DXCOLOR(1.0f, 0.99f, 0.04f, g_aScore[nCntScore[0]].col.a);	// A = 透明度
				pVtx[nCntScore[1] * 4 + 1].col = D3DXCOLOR(1.0f, 0.99f, 0.04f, g_aScore[nCntScore[0]].col.a);
				pVtx[nCntScore[1] * 4 + 2].col = D3DXCOLOR(1.0f, 0.99f, 0.04f, g_aScore[nCntScore[0]].col.a);
				pVtx[nCntScore[1] * 4 + 3].col = D3DXCOLOR(1.0f, 0.99f, 0.04f, g_aScore[nCntScore[0]].col.a);

				//頂点座標の設定
				pVtx[nCntScore[1] * 4].pos = D3DXVECTOR3(g_aScore[nCntScore[0]].pos.x - 20 - (nCntScore[1] * 40), g_aScore[nCntScore[0]].pos.y - 40, 0.0f);
				pVtx[nCntScore[1] * 4 + 1].pos = D3DXVECTOR3(g_aScore[nCntScore[0]].pos.x + 20 - (nCntScore[1] * 40), g_aScore[nCntScore[0]].pos.y - 40, 0.0f);
				pVtx[nCntScore[1] * 4 + 2].pos = D3DXVECTOR3(g_aScore[nCntScore[0]].pos.x - 20 - (nCntScore[1] * 40), g_aScore[nCntScore[0]].pos.y + 40, 0.0f);
				pVtx[nCntScore[1] * 4 + 3].pos = D3DXVECTOR3(g_aScore[nCntScore[0]].pos.x + 20 - (nCntScore[1] * 40), g_aScore[nCntScore[0]].pos.y + 40, 0.0f);

				//テクスチャ座標設定
				pVtx[nCntScore[1] * 4].tex = D3DXVECTOR2((nScore % 10) * 0.1f, 0.0f);
				pVtx[nCntScore[1] * 4 + 1].tex = D3DXVECTOR2((nScore % 10) * 0.1f + 0.1f, 0.0f);
				pVtx[nCntScore[1] * 4 + 2].tex = D3DXVECTOR2((nScore % 10) * 0.1f, 1.0f);
				pVtx[nCntScore[1] * 4 + 3].tex = D3DXVECTOR2((nScore % 10) * 0.1f + 0.1f, 1.0f);
				nScore /= 10;
			}
		}
		pVtx += 32;
	}
	//頂点BUFFERをアンロックする
	g_pVtxBuffScore->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawScore(void)
{
	int nCntScore[2];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
    pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureScore);
	// ポリゴンの描画
	for (nCntScore[0] = 0; nCntScore[0] < MAX_SETSCORE; nCntScore[0]++)
	{//スコア分回る
		if (g_aScore[nCntScore[0]].bUse == true)
		{//指定されたスコアが使われているなら表示
			for (nCntScore[1] = 0; nCntScore[1] < MAX_NUMBER; nCntScore[1]++)
			{
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore[1] * 4 + 32 * nCntScore[0], SCORE_PRIMITIVE_NUM);
			}
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexScore(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成


	// 頂点情報を設定

}

//=============================================================================
// スコアの加算
//=============================================================================
void AddScore(int nValue, int nNumber)
{
	int nCntScore;
	int nScore;
	Difficulty *difficulty;
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	difficulty = GetDifficulty();	//難易度取得

	//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	g_aScore[nNumber].g_nScore += nValue * difficulty[0].nDifficulty / 100;	//難易度を込めて
	if (g_aScore[nNumber].g_nScore < 0)
	{//マイナスになったら0代入
		g_aScore[nNumber].g_nScore = 0;
	}

	nScore = g_aScore[nNumber].g_nScore;

	pVtx += 32 * nNumber;
	for (nCntScore = 0; nCntScore < MAX_NUMBER; nCntScore++)
	{//8桁分ポリゴンを生成
		//テクスチャ座標設定
		pVtx[nCntScore * 4].tex = D3DXVECTOR2((nScore % 10) * 0.1f, 0.0f);
		pVtx[nCntScore * 4 + 1].tex = D3DXVECTOR2((nScore % 10) * 0.1f + 0.1f, 0.0f);
		pVtx[nCntScore * 4 + 2].tex = D3DXVECTOR2((nScore % 10) * 0.1f, 1.0f);
		pVtx[nCntScore * 4 + 3].tex = D3DXVECTOR2((nScore % 10) * 0.1f + 0.1f, 1.0f);
		nScore /= 10;
	}

	//頂点BUFFERをアンロックする
	g_pVtxBuffScore->Unlock();
}
//=============================================================================
// スコアのセット
//=============================================================================
void SetScore(D3DXVECTOR3 pos, D3DXCOLOR col, int nScore, int nNumber, SCORESTATE state)
{
	int nCntScore;
	VERTEX_2D *pVtx;		//頂点情報へのポインタ
	//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);


	g_aScore[nNumber].bUse = true;
	g_aScore[nNumber].g_nScore = nScore;
	g_aScore[nNumber].pos = pos;
	g_aScore[nNumber].col = col;
	g_aScore[nNumber].state = state;
	pVtx += 32 * nNumber;
	for (nCntScore = 0; nCntScore < MAX_NUMBER; nCntScore++)
	{//8桁分ポリゴンを生成
	//カラー設定
		pVtx[nCntScore * 4].col = D3DXCOLOR(1.0f, 0.99f, 0.04f, col.a);	// A = 透明度
		pVtx[nCntScore * 4 + 1].col = D3DXCOLOR(1.0f, 0.99f, 0.04f, col.a);
		pVtx[nCntScore * 4 + 2].col = D3DXCOLOR(1.0f, 0.99f, 0.04f, col.a);
		pVtx[nCntScore * 4 + 3].col = D3DXCOLOR(1.0f, 0.99f, 0.04f, col.a);

		//頂点座標の設定
		pVtx[nCntScore * 4].pos = D3DXVECTOR3(pos.x - 20 - (nCntScore * 40), pos.y - 40, 0.0f);
		pVtx[nCntScore * 4 + 1].pos = D3DXVECTOR3(pos.x + 20 - (nCntScore * 40), pos.y - 40, 0.0f);
		pVtx[nCntScore * 4 + 2].pos = D3DXVECTOR3(pos.x - 20 - (nCntScore * 40), pos.y + 40, 0.0f);
		pVtx[nCntScore * 4 + 3].pos = D3DXVECTOR3(pos.x + 20 - (nCntScore * 40), pos.y + 40, 0.0f);

		//テクスチャ座標設定
		pVtx[nCntScore * 4].tex = D3DXVECTOR2((nScore % 10) * 0.1f, 0.0f);
		pVtx[nCntScore * 4 + 1].tex = D3DXVECTOR2((nScore % 10) * 0.1f + 0.1f, 0.0f);
		pVtx[nCntScore * 4 + 2].tex = D3DXVECTOR2((nScore % 10) * 0.1f, 1.0f);
		pVtx[nCntScore * 4 + 3].tex = D3DXVECTOR2((nScore % 10) * 0.1f + 0.1f, 1.0f);
		nScore /= 10;
	}

	//頂点BUFFERをアンロックする
	g_pVtxBuffScore->Unlock();
}
//=============================================================================
// スコアの全破壊
//=============================================================================
void BreakScore(void)
{
	int nCntScore;

	for (nCntScore = 0; nCntScore < MAX_SETSCORE; nCntScore++)
	{
		g_aScore[nCntScore].bUse = false;
	}
}
//=============================================================================
// スコアの状態変更
//=============================================================================
void ChangeScore(int nNumber, SCORESTATE state)
{//スコアの状態変更
	g_aScore[nNumber].state = state;
}
//=========================================================================================================//
// * ランキングのため
//=========================================================================================================//
SCORE *GetGameScore(void)
{
	return &g_aScore[0];
}
