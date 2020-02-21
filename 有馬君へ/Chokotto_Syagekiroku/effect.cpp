//=============================================================================
//
// エフェクトの処理 [effect.cpp]
// Author : 
//
//=============================================================================
#include "game.h"
#include <stdlib.h>				//ランド用
#include <time.h>				//同じくランド用

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_EFFECT		"data/TEXTURE/effect000.jpg"	// 読み込むテクスチャファイル名
#define	MAX_EFFECT			(5024)							// エフェクトの最大数
#define EFFECT_SLIP			(0.8f)

//*****************************************************************************
// 構造体定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureEffect = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	// 頂点バッファへのポインタ

//=============================================================================
// 初期化処理
//=============================================================================
void InitEffect(void)
{
	int nCntEffect;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	EFFECT *g_aEffect = GetEffect();

	for(int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].bUse = false;
	}

	D3DXCreateTextureFromFile(pDevice, TEXTURE_EFFECT, &g_pTextureEffect);

	// 頂点情報の作成
	VERTEX_2D *pVtx;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//頂点座標の設定
		//pVtx[nCntEnemy * 4].pos = D3DXVECTOR3(0, 0, 0.0f);
		//pVtx[nCntEnemy * 4 + 1].pos = D3DXVECTOR3(0, 0, 0.0f);
		//pVtx[nCntEnemy * 4 + 2].pos = D3DXVECTOR3(0, 0, 0.0f);
		//pVtx[nCntEnemy * 4 + 3].pos = D3DXVECTOR3(0, 0, 0.0f);

		pVtx[nCntEffect * 4].rhw = 1.0f;
		pVtx[nCntEffect * 4 + 1].rhw = 1.0f;
		pVtx[nCntEffect * 4 + 2].rhw = 1.0f;
		pVtx[nCntEffect * 4 + 3].rhw = 1.0f;

		//テクスチャ座標設定
		pVtx[nCntEffect * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nCntEffect * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[nCntEffect * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[nCntEffect * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEffect(void)
{
	// テクスチャの開放
		if (g_pTextureEffect != NULL)
		{
			g_pTextureEffect->Release();
			g_pTextureEffect = NULL;
		}

	// 頂点バッファの開放
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEffect(void)
{
	int nCntEffect;	//エフェクトを数える
	VERTEX_2D *pVtx;		//頂点情報へのポインタ
	EFFECT *g_aEffect = GetEffect();
							//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{//エフェクトの最大まで回る
		if (g_aEffect[nCntEffect].bUse == true)
		{//使っているエフェクトなら

			g_aEffect[nCntEffect].pos += D3DXVECTOR3(sinf(g_aEffect[nCntEffect].fSteep), cosf(g_aEffect[nCntEffect].fSteep), 0.0f) * g_aEffect[nCntEffect].fSpeed;
			g_aEffect[nCntEffect].move.x += (0.0f - g_aEffect[nCntEffect].move.x) * EFFECT_SLIP;
			g_aEffect[nCntEffect].move.y += (0.0f - g_aEffect[nCntEffect].move.y) * EFFECT_SLIP;
			g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;
			//毎回薄くする
			g_aEffect[nCntEffect].col.a -= 0.03f;

			if (g_aEffect[nCntEffect].col.a <= 0.0f)
			{//透明度が0になったら
				g_aEffect[nCntEffect].bUse = false;
			}

			pVtx[nCntEffect * 4].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.g, g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);	// A = 透明度
			pVtx[nCntEffect * 4 + 1].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.g, g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);
			pVtx[nCntEffect * 4 + 2].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.g, g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);
			pVtx[nCntEffect * 4 + 3].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.g, g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);

			pVtx[nCntEffect * 4].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);
		}
	}//for括弧

	 //頂点BUFFERをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	EFFECT *g_aEffect = GetEffect();
	int nCntEffect;

	// 頂点バッファをデータストリームに設定
    pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureEffect);

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ポリゴンの描画
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//trueなら表示
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
		}
	}

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

//=============================================================================
// 頂点の作成
//=============================================================================
void MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
 

	// 頂点バッファの情報を設定

}


//=============================================================================
// エフェクト設定
//=============================================================================
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSteep, float fSpeed, float fRadius)
{
	int nCntEffect;
	VERTEX_2D *pVtx;		//頂点情報へのポインタ
	EFFECT *g_aEffect = GetEffect();

							//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{
			//D3DXVECTOR3 pos;		// 位置
			//D3DXVECTOR3 move;		// 移動値の残り火
			//D3DXCOLOR col;			// 色
			//float fSteep;			//角度
			//float fSpeed;
			//float fRadius;			// 半径(大きさ)
			//bool bUse;				// 使用しているかどうか
			g_aEffect[nCntEffect] = { pos, move, col, fSteep, fSpeed, fRadius, true };

			//g_aEffect[nCntEffect].move = move;
			//g_aEffect[nCntEffect].pos = pos;
			//g_aEffect[nCntEffect].fSteep = fSteep;
			//g_aEffect[nCntEffect].fSpeed = fSpeed;
			//g_aEffect[nCntEffect].fRadius = fRadius;
			//g_aEffect[nCntEffect].bUse = true;

			pVtx[nCntEffect * 4].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);

			//g_aEffect[nCntEffect].col = col;
			pVtx[nCntEffect * 4].col = D3DXCOLOR(col.r, col.g, col.b, col.a);	// A = 透明度
			pVtx[nCntEffect * 4 + 1].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
			pVtx[nCntEffect * 4 + 2].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
			pVtx[nCntEffect * 4 + 3].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
			break;
		}
	}
	//頂点BUFFERをアンロックする
	g_pVtxBuffEffect->Unlock();
}
//=============================================================================
// ヒット時のエフェクト設定
//=============================================================================
void ExplosionEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nSRand, int nLRand, float fSRand0, float fSRand1, float fLRand0, float fLRand1)
{
	static int nCntEffect;
	int nSetEffect;
	int nRand[3];
	float fRand[3];

	nSetEffect = 0;


	//srand((unsigned int)time(0));	//ランドのため
	while(1)
	{//エフェクト全部回る
		nRand[0] = rand() % 628;
		nRand[1] = rand() % nSRand;
		nRand[2] = rand() % nLRand;

		fRand[0] = nRand[0] * 0.01f;					//角度ランダム
		fRand[1] = nRand[1] * fSRand0 + fSRand1;		//SPEEDランダム
		fRand[2] = nRand[2] * fLRand0 + fLRand1;		//大きさランダム

		SetEffect(pos, move, col, fRand[0], fRand[1], fRand[2]);


		nSetEffect++;
		if (nSetEffect > 50)
		{//100個出たら終了
			break;
		}

	}
}
//=============================================================================
// 敵の取得
//=============================================================================
EFFECT *GetEffect(void)
{
	static EFFECT effect[MAX_EFFECT];//弾の最大数

	return &effect[0];
}
