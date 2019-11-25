//=============================================================================
//
// 2DPolygon処理 [2DPolygon.cpp]
// Author : Ryo Sugimoto
//
//=============================================================================
#include "main.h"
#include "scene.h"
#include "2DPolygon.h"
#include "renderer.h"
#include "manager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SLIDE_SPEED	(10.0f)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//==================================================================================================//
//    * 生成関数 *
//==================================================================================================//
C2D	*C2D::Create(void)
{
	C2D *p2D = NULL;	p2D = new C2D;
	if (p2D != NULL) { p2D->Init(); }
	return p2D;
}
//==================================================================================================//
//    * 入力関数 *
//==================================================================================================//
void C2D::Set(D3DXVECTOR3 pos, float fLengthX, float fLengthY, float fRot, int nUV, int nUV_X, int nUV_Y, D3DXCOLOR col, DRAW_TYPE Drawtype)
{
		m_DrawType = Drawtype;
		m_pos = pos;
		m_fLengthX = fLengthX;
		m_fLengthY = fLengthY;
		
		//Vtxの変更
		VERTEX_2D *pVtx;		//頂点情報へのポインタ

								//頂点BUFFERをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(m_pos.x + (sinf(fRot) * m_fLengthY) + (cosf(fRot) * m_fLengthX), m_pos.y - (sinf(fRot) * m_fLengthX) + (cosf(fRot) * m_fLengthY), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + (sinf(fRot) * m_fLengthY) - (cosf(fRot) * m_fLengthX), m_pos.y + (sinf(fRot) * m_fLengthX) + (cosf(fRot) * m_fLengthY), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - (sinf(fRot) * m_fLengthY) + (cosf(fRot) * m_fLengthX), m_pos.y - (sinf(fRot) * m_fLengthX) - (cosf(fRot) * m_fLengthY), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x - (sinf(fRot) * m_fLengthY) - (cosf(fRot) * m_fLengthX), m_pos.y + (sinf(fRot) * m_fLengthX) - (cosf(fRot) * m_fLengthY), 0.0f);

		//カラー設定
		pVtx[0].col = col;	// A = 透明度
		pVtx[1].col = col;
		pVtx[2].col = col;
		pVtx[3].col = col;

		//テクスチャ座標設定
		pVtx[0].tex = D3DXVECTOR2((nUV % nUV_X) * (1.0f / nUV_X), (nUV / nUV_X) * (1.0f / nUV_Y));
		pVtx[1].tex = D3DXVECTOR2((nUV % nUV_X) * (1.0f / nUV_X) + (1.0f / nUV_X), (nUV / nUV_X) * (1.0f / nUV_Y));
		pVtx[2].tex = D3DXVECTOR2((nUV % nUV_X) * (1.0f / nUV_X), (nUV / nUV_X) * (1.0f / nUV_Y) + (1.0f / nUV_Y));
		pVtx[3].tex = D3DXVECTOR2((nUV % nUV_X) * (1.0f / nUV_X) + (1.0f / nUV_X), (nUV / nUV_X) * (1.0f / nUV_Y) + (1.0f / nUV_Y));

		//頂点BUFFERをアンロックする
		m_pVtxBuff->Unlock();
}
//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
HRESULT  C2D::Init(void)
{
	// ポリゴンの位置を設定
	m_pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0);
	m_fLengthX = 50;
	m_fLengthY = 50;
	m_pTex = NULL;
	CManager::GetRenderer();

	LPDIRECT3DDEVICE9		pD3DDevice = CManager::GetRenderer()->GetDevice();
	// 頂点バッファの生成
	pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,		//確保するBUFFERのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				//頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	float fRot = 0.0f;

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

							//頂点BUFFERをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点BUFFERをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void C2D::Uninit(void)
{
	if (m_pVtxBuff != NULL)
	{// 頂点バッファの破棄
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	delete this;
}

//=============================================================================
// ポリゴンの更新処理
//=============================================================================
void C2D::Update(void)
{

}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void C2D::Draw(void)
{
	LPDIRECT3DDEVICE9	pD3DDevice = CManager::GetRenderer()->GetDevice();
	int					nNumDraw = 1;
	DrawPrepare(m_DrawType, pD3DDevice);
	//頂点BUFFERをデバイスのデータストリームに設定
	pD3DDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	pD3DDevice->SetFVF(FVF_VERTEX_2D);	// 頂点フォーマットの設定

										//テクスチャの設定
	//テクスチャの設定
	pD3DDevice->SetTexture(0, m_pTex);

	if (m_DrawType == DRAW_TYPE_ADD) { nNumDraw = 1; }
	//ポリゴンの描画
	for (int nCount = 0; nCount < nNumDraw; nCount++)
	{//指定回数分
		pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

	pD3DDevice->SetTexture(0, NULL);
	DrawPrepare(DRAW_TYPE_NORMAL, pD3DDevice);
}
//==================================================================================================//
//    * 描画準備関数 *
//==================================================================================================//
bool C2D::DrawPrepare(DRAW_TYPE DrawType, LPDIRECT3DDEVICE9 &pD3DDevice)
{
	if (DrawType == DRAW_TYPE_NO) { return true; }

	if (DrawType >= DRAW_TYPE_ZNORMAL)
	{// Zテスト解除
		pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	}
	else
	{// Zテスト始動
		pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	}
	switch (DrawType % 10)
	{//ほんとは3行あることを忘れてはいけない...
	case DRAW_TYPE_NORMAL:// 通常
		pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		break;
	case DRAW_TYPE_ADD:// αブレンディングを加算合成に設定
		pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
	case DRAW_TYPE_SUBTRACT:// 減算
		pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
	case DRAW_TYPE_MULTIPLY:
		pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO | D3DBLEND_ONE);
		pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTCOLOR);
		break;
	case DRAW_TYPE_OVERLAY:
		pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO | D3DBLEND_ONE);
		pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
	case DRAW_TYPE_AER:
		pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		break;
	}

	return false;
}
//=============================================================================
// 頂点情報の設定
//=============================================================================
void C2D::SetPosition(float fRot)
{
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + (sinf(fRot) * m_fLengthY) + (cosf(fRot) * m_fLengthX), m_pos.y - (sinf(fRot) * m_fLengthX) + (cosf(fRot) * m_fLengthY), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (sinf(fRot) * m_fLengthY) - (cosf(fRot) * m_fLengthX), m_pos.y + (sinf(fRot) * m_fLengthX) + (cosf(fRot) * m_fLengthY), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - (sinf(fRot) * m_fLengthY) + (cosf(fRot) * m_fLengthX), m_pos.y - (sinf(fRot) * m_fLengthX) - (cosf(fRot) * m_fLengthY), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x - (sinf(fRot) * m_fLengthY) - (cosf(fRot) * m_fLengthX), m_pos.y + (sinf(fRot) * m_fLengthX) - (cosf(fRot) * m_fLengthY), 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}
//=============================================================================
// 頂点倍率の設定
//=============================================================================
void C2D::SetPosition2(float fSize)
{
	VERTEX_2D *pVtx;
	float fLengthX = m_fLengthX * fSize;
	float fLengthY = m_fLengthY * fSize;
	float fRot = D3DX_PI;

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + (sinf(fRot) * fLengthY) + (cosf(fRot) * fLengthX), m_pos.y - (sinf(fRot) * fLengthX) + (cosf(fRot) * fLengthY), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (sinf(fRot) * fLengthY) - (cosf(fRot) * fLengthX), m_pos.y + (sinf(fRot) * fLengthX) + (cosf(fRot) * fLengthY), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - (sinf(fRot) * fLengthY) + (cosf(fRot) * fLengthX), m_pos.y - (sinf(fRot) * fLengthX) - (cosf(fRot) * fLengthY), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x - (sinf(fRot) * fLengthY) - (cosf(fRot) * fLengthX), m_pos.y + (sinf(fRot) * fLengthX) - (cosf(fRot) * fLengthY), 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}
//==================================================================================================//
//    * UV変更 *
//==================================================================================================//
void C2D::SetUV(int nUV, int nUV_X, int nUV_Y)
{
	//Vtxの変更
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

							//頂点BUFFERをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標設定
	pVtx[0].tex = D3DXVECTOR2((nUV % nUV_X) * (1.0f / nUV_X), (nUV / nUV_X) * (1.0f / nUV_Y));
	pVtx[1].tex = D3DXVECTOR2((nUV % nUV_X) * (1.0f / nUV_X) + (1.0f / nUV_X), (nUV / nUV_X) * (1.0f / nUV_Y));
	pVtx[2].tex = D3DXVECTOR2((nUV % nUV_X) * (1.0f / nUV_X), (nUV / nUV_X) * (1.0f / nUV_Y) + (1.0f / nUV_Y));
	pVtx[3].tex = D3DXVECTOR2((nUV % nUV_X) * (1.0f / nUV_X) + (1.0f / nUV_X), (nUV / nUV_X) * (1.0f / nUV_Y) + (1.0f / nUV_Y));

	pVtx[0].tex += D3DXVECTOR2(0.01f, 0.01f);
	pVtx[1].tex += D3DXVECTOR2(-0.01f, 0.01f);
	pVtx[2].tex += D3DXVECTOR2(0.01f, -0.01f);
	pVtx[3].tex += D3DXVECTOR2(-0.01f, -0.01f);

	//頂点BUFFERをアンロックする
	m_pVtxBuff->Unlock();
}
//==================================================================================================//
//    * UV変更 *
//==================================================================================================//
void C2D::SetUV_Camera(float fUV_X, float fUV_Y, float fDiameter)
{
	//Vtxの変更
	VERTEX_2D *pVtx;		//頂点情報へのポインタ
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							//頂点BUFFERをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標設定
	pVtx[0].tex = D3DXVECTOR2((pos.x * fDiameter) + fUV_X, (pos.z * (-fDiameter * 1.65f)) + fUV_Y);
	pVtx[1].tex = D3DXVECTOR2((pos.x * fDiameter) + 1.0f + fUV_X, (pos.z * (-fDiameter * 1.65f)) + fUV_Y);
	pVtx[2].tex = D3DXVECTOR2((pos.x * fDiameter) + fUV_X, (pos.z * (-fDiameter * 1.65f)) + 1.0f + fUV_Y);
	pVtx[3].tex = D3DXVECTOR2((pos.x * fDiameter) + 1.0f + fUV_X, (pos.z * (-fDiameter * 1.65f)) + 1.0f + fUV_Y);


	//頂点BUFFERをアンロックする
	m_pVtxBuff->Unlock();
}
//==================================================================================================//
//    * UVをゲージ型に変更 *
//==================================================================================================//
void	C2D::SetGageUV(float fMaxLengthX)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ
	//頂点BUFFERをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((m_fLengthX / fMaxLengthX), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((m_fLengthX / fMaxLengthX), 1.0f);


	//頂点BUFFERをアンロックする
	m_pVtxBuff->Unlock();
}
//==================================================================================================//
//    * ゲージ型に変更 *
//==================================================================================================//
void C2D::SetGage(float fLengthX, int nDirection)
{
	//Vtxの変更
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	m_fLengthX = fLengthX;

	//頂点BUFFERをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fRot = D3DX_PI - (D3DX_PI / 2.0f) * nDirection;
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + (sinf(fRot) * m_fLengthY), m_pos.y + (cosf(fRot) * m_fLengthY), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (sinf(fRot) * m_fLengthY) - (cosf(fRot) * fLengthX), m_pos.y + (sinf(fRot) * fLengthX) + (cosf(fRot) * m_fLengthY), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - (sinf(fRot) * m_fLengthY), m_pos.y - (cosf(fRot) * m_fLengthY), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x - (sinf(fRot) * m_fLengthY) - (cosf(fRot) * fLengthX), m_pos.y + (sinf(fRot) * fLengthX) - (cosf(fRot) * m_fLengthY), 0.0f);

	//頂点BUFFERをアンロックする
	m_pVtxBuff->Unlock();
}
//=============================================================================
// 頂点カラーの設定
//=============================================================================
void C2D::SetColor(D3DXCOLOR col)
{
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}
//=============================================================================
// 頂点カラーの加算処理
//=============================================================================
void	C2D::PlusColor(D3DXCOLOR col)
{
	VERTEX_2D *pVtx;
	D3DXCOLOR WKcol;
	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	for (int nCount = 0; nCount < 4; nCount++)
	{
		WKcol = pVtx[nCount].col;
		WKcol += col;
		if (WKcol.r > 1.0f) { WKcol.r = 1.0f; } else if (WKcol.r < 0.0f) { WKcol.r = 0.0f; }
		if (WKcol.g > 1.0f) { WKcol.g = 1.0f; } else if (WKcol.g < 0.0f) { WKcol.g = 0.0f; }
		if (WKcol.b > 1.0f) { WKcol.b = 1.0f; } else if (WKcol.b < 0.0f) { WKcol.b = 0.0f; }
		if (WKcol.a > 1.0f) { WKcol.a = 1.0f; } else if (WKcol.a < 0.0f) { WKcol.a = 0.0f; }

		pVtx[nCount].col = WKcol;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}
//=============================================================================
// 頂点カラー透明度(左右)の設定
//=============================================================================
void	C2D::SetSideAlpha(float fCol, float fMinCol, int nID)
{
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	D3DXCOLOR col = pVtx[0 + nID].col;
	col.a += fCol;
	if (col.a >= 1.0f) { col.a = 1.0f; }
	if (col.a <= fMinCol) { col.a = fMinCol; }

	pVtx[0 + nID].col = col;
	pVtx[2 + nID].col = col;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}
//=============================================================================
// 頂点カラーの設定
//=============================================================================
void C2D::PlusUV(D3DXVECTOR2 uv)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	for (int nCount = 0; nCount < 4; nCount++) { pVtx[nCount].tex += uv; }

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//==================================================================================================//
//    * 生成関数 *
//==================================================================================================//
C2DAnim	*C2DAnim::Create(void)
{
	C2DAnim *p2D = NULL;	p2D = new C2DAnim;
	if (p2D != NULL) { p2D->Init(); }
	return p2D;
}
//=============================================================================
// アニメーションポリゴンの設定
//=============================================================================
void	C2DAnim::Set(D3DXVECTOR3 pos, float fLengthX, float flengthY, float fRot, int nPtnAnim, int nPtnWide, int nPtnHeight, int nPtnSpeed,
	ANIMATION animation, D3DXCOLOR col, DRAW_TYPE Drawtype)
{
	C2D::Set(pos, fLengthX, flengthY, fRot, nPtnAnim, nPtnWide, nPtnHeight, col, Drawtype);
	m_animation = animation;
	m_nCntAnim = 0;
	m_nPtnAnim = nPtnAnim;
	m_nPtnWide = nPtnWide;
	m_nPtnHeight = nPtnHeight;
	m_nPtnSpeed = nPtnSpeed;
	m_col = col;
	m_fStateSpd = 1.0f;
	m_fCntSize = 0.0f;
}
//=============================================================================
// アニメーションポリゴンの状態設定
//=============================================================================
void	C2DAnim::SetState(STATE state, float fCntState)
{
	m_state = state;
	if (fCntState == 99.9f)
	{//引数が設定されていないなら
		fCntState = 1.0f / (float)((m_nPtnHeight * m_nPtnWide) * m_nPtnSpeed);
	}
	m_fCntState = fCntState;
	if (m_state == STATE_FADEIN) { m_fCntSize = 0.3f; }
	if (m_state == STATE_FADEIN_D) { m_fCntSize = -0.3f; }
}
//=============================================================================
// アニメーションポリゴンの更新処理
//=============================================================================
void	C2DAnim::Update(void)
{
	switch (m_state)
	{
	case STATE_NORMAL:
		m_fCntSize += (0.05f - m_fCntSize) * 0.05f;
		SetPosition2(1.0f + m_fCntSize);
		break;
	case STATE_FADEIN:
		m_col.a += m_fCntState * m_fStateSpd;
		if (m_col.a > 1.0f) { m_col.a = 1.0f; m_state = STATE_NORMAL; }
		SetColor(m_col);
		m_fCntSize += (0.0f - m_fCntSize) * 0.25f;
		SetPosition2(1.0f + m_fCntSize);
		break;
	case STATE_FADEOUT:
		m_col.a -= m_fCntState * m_fStateSpd;
		if (m_col.a < 0.0f) { m_col.a = 0.0f; }
		SetColor(m_col);
		m_fCntSize += (0.1f - m_fCntSize) * 0.02f;
		SetPosition2(1.0f + m_fCntSize);
		break;
	case 	STATE_FADEIN_D:
		m_col.a += m_fCntState * m_fStateSpd;
		if (m_col.a > 1.0f) { m_col.a = 1.0f; m_state = STATE_FADEOUT_D; }
		SetColor(m_col);
		m_fCntSize += (1.0f - m_fCntSize) * 0.25f;
		SetPosition2(1.0f + m_fCntSize);
		break;
	case	STATE_FADEOUT_D:
		m_col.a -= m_fCntState * m_fStateSpd * 0.5f;
		if (m_col.a < 0.0f) { m_animation = ANIMATION_END; return; }
		SetColor(m_col);
		m_fCntSize += (0.0f - m_fCntSize) * 0.005f;
		SetPosition2(1.0f + m_fCntSize);
		break;
	}
	m_nCntAnim++;
	if (m_nCntAnim % m_nPtnSpeed == 0)
	{//UV変更
		m_nPtnAnim++;
		if (m_nPtnAnim >= m_nPtnWide * m_nPtnHeight)
		{//画像の範囲外
			if (m_animation == ANIMATION_LOOP) { m_nPtnAnim = 0; }
			else { m_animation = ANIMATION_END; return; }
		}//アニメーション更新
		SetUV(m_nPtnAnim, m_nPtnWide, m_nPtnHeight);
	}
}

//=============================================================================
// カメラ付き添いポリゴンの設定処理
//=============================================================================
void C2DCameraBG::Set(D3DXVECTOR3 pos, float fLengthX, float flengthY, float fRot, float fUV_X, float fUV_Y, float fDiameter, D3DXCOLOR col)
{
	C2D::Set(pos, fLengthX, flengthY, fRot, 0, 1, 1, col);
	m_fUV_X = 0.0f;
	m_fUV_Y = 0.0f;
	m_fUV_XPlus = fUV_X;
	m_fUV_YPlus = fUV_Y;
	m_fDiameter = fDiameter;
}
//=============================================================================
// カメラ付き添いポリゴンの更新処理
//=============================================================================
void	C2DCameraBG::Update(void)
{
	m_fUV_X += m_fUV_XPlus;
	m_fUV_Y += m_fUV_YPlus;
	SetUV_Camera(m_fUV_X, m_fUV_Y, m_fDiameter);
}

//==================================================================================================//
//    * 生成関数 *
//==================================================================================================//
C2DPolygon	*C2DPolygon::Create(void)
{
	C2DPolygon *p2D = NULL;	p2D = new C2DPolygon;
	if (p2D != NULL) { p2D->Init(); }
	return p2D;
}
//==================================================================================================//
//    * 入力関数 *
//==================================================================================================//
void C2DPolygon::Set(D3DXVECTOR3 pos, float fLengthX, float fLengthY, float fRot, int nUV, int nUV_X, int nUV_Y, float fCntState, STATE state, D3DXCOLOR col)
{
	m_col = col;
	m_fCntState = fCntState;
	m_state = state;
	C2D::Set(pos, fLengthX, fLengthY, fRot, nUV, nUV_X, nUV_Y, col);
}
//=============================================================================
// 通常ポリゴンの更新処理
//=============================================================================
void	C2DPolygon::Update(void)
{
	if (m_state != STATE_NORMAL)
	{
		float fWKstate;

		switch (m_state)
		{
		case STATE_FADEOUT:
			m_col.a -= m_fCntState / FRAME_SPEED;
			if (m_col.a <= 0.0f) { m_col.a = 0.0f; m_state = STATE_NORMAL; }
			SetColor(m_col);
			break;
		case STATE_FADEIN:
			m_col.a += m_fCntState / FRAME_SPEED;
			if (m_col.a >= 1.0f) { m_col.a = 1.0f; m_state = STATE_NORMAL; }
			SetColor(m_col);
			break;
		case STATE_FADE:
			m_col.a += m_fCntState / FRAME_SPEED;
			if (m_col.a >= 1.0f) { m_col.a = 1.0f; m_state = STATE_FADEOUT; }
			SetColor(m_col);
			break;
		case STATE_SLIDEIN:
			GetPosition().x -= (1.0f - m_col.a) * 2.0f;
			SetPosition(D3DX_PI);
			m_col.a += m_fCntState;
			if (m_col.a >= 1.0f) { m_col.a = 1.0f; m_state = STATE_NORMAL; }
			SetColor(m_col);
			break;
		case STATE_SLIDEOUT:
			GetPosition() += D3DXVECTOR3(powf(SLIDE_SPEED * m_col.a, 2), 0.0f, 0.0f);
			SetPosition(D3DX_PI);
			m_col.a -= m_fCntState;
			if (m_col.a <= 0.0f) { m_col.a = 0.0f; m_state = STATE_NORMAL; }
			SetColor(m_col);
			break;
		case STATE_SLIDEIN2:
			GetPosition() += D3DXVECTOR3(powf(SLIDE_SPEED * (1.0f - m_col.a), 2), powf(SLIDE_SPEED * (1.0f - m_col.a), 2), 0.0f);
			SetPosition(D3DX_PI);
			m_col.a += m_fCntState;
			if (m_col.a >= 1.0f) { m_col.a = 1.0f; m_state = STATE_NORMAL; }
			SetColor(m_col);
			break;
		case STATE_SLIDEOUT2:
			GetPosition() += D3DXVECTOR3(powf(SLIDE_SPEED * m_col.a, 2), powf(SLIDE_SPEED * m_col.a, 2), 0.0f);
			SetPosition(D3DX_PI);
			m_col.a -= m_fCntState;
			if (m_col.a <= 0.0f) { m_col.a = 0.0f; m_state = STATE_NORMAL; }
			SetColor(m_col);
			break;
		case STATE_BIG:
			if (m_fCntState < 125.0f)
			{
				m_fCntState += 4.0f;
				if (m_fCntState >= 125.0f) { m_fCntState = 125.0f; }
				SetPosition2(m_fCntState * 0.01f);
			}
			break;
		case STATE_MEDIUM:
			if (m_fCntState > 100.0f)
			{
				m_fCntState -= 4.0f;
				if (m_fCntState <= 100.0f) { m_fCntState = 100.0f; }
				SetPosition2(m_fCntState * 0.01f);
			}
			else if (m_fCntState < 100.0f)
			{
				m_fCntState += 4.0f;
				if (m_fCntState >= 100.0f) { m_fCntState = 100.0f; }
				SetPosition2(m_fCntState * 0.01f);
			}
			break;
		case STATE_SMALL:
			if (m_fCntState > 75.0f)
			{
				m_fCntState -= 4.0f;
				if (m_fCntState <= 75.0f) { m_fCntState = 75.0f; }
				SetPosition2(m_fCntState * 0.01f);
			}
			break;
		case STATE_FLASH:
			m_fCntState = m_fCntState + 1.0f;
			fWKstate = m_fCntState * 0.01f;
			SetColor(D3DXCOLOR(fWKstate, fWKstate, fWKstate, 1.0f));
			if (m_fCntState >= 100.0f) { m_state = STATE_FLASH2; m_fCntState = 100.0f; }
			break;
		case STATE_FLASH2:
			m_fCntState = m_fCntState - 1.5f;
			fWKstate = m_fCntState * 0.01f;
			SetColor(D3DXCOLOR(fWKstate, fWKstate, fWKstate, 1.0f));
			if (m_fCntState <= 25.0f) { m_state = STATE_FLASH; m_fCntState = 25.0f; }
			break;
		}
	}
}