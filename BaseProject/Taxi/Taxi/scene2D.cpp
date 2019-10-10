//=============================================================================
//
// UIなどの親の処理 [scene2D.cpp]
// Author : 有馬武志
//
//=============================================================================
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_SIZE		(50)					//テクスチャサイズ
#define TEXTURE_ANIM_UV_U	(0.125f)				//テクスチャアニメーションU範囲
#define TEXTURE_ANIM_UV_V	(1.0f)					//テクスチャアニメーションV範囲

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CScene2D::CScene2D(int nPriority,CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	//値をクリア
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fAngle = 0;
	m_fLength = 0;
	m_fRadius = 0;
	m_nSize = D3DXVECTOR2(0.0f, 0.0f);
}
//===============================================================================
//　デストラクタ
//===============================================================================
CScene2D::~CScene2D()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScene2D::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL
	);

	// 頂点情報を設定
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//頂点カラー設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CScene2D::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	if (m_pTexture != NULL) { m_pTexture = NULL; }

	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CScene2D::Update(void)
{

}
//=============================================================================
// 描画処理
//=============================================================================
void CScene2D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (m_pVtxBuff != NULL)
	{
		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource
		(
			0,
			m_pVtxBuff,
			0,
			sizeof(VERTEX_2D)
		);

		//頂点フォーマット
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, m_pTexture);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}
//===============================================================================
//　クリエイト
//===============================================================================
CScene2D * CScene2D::Create()
{
	CScene2D *pScene2D = NULL;

	//NULLチェック
	if (pScene2D == NULL)
	{//メモリの動的確保

		pScene2D = new CScene2D;

		if (pScene2D != NULL)
		{
			//オブジェクトクラスの生成
			pScene2D->Init();
		}
	}

	return pScene2D;
}
//===============================================================================
// 共有テクスチャの割り当て
//===============================================================================
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 Texture)
{
	m_pTexture = Texture;
}
//=============================================================================
// セット処理
//=============================================================================
void CScene2D::SetPosSize(D3DXVECTOR3 pos, D3DXVECTOR2 nSize)
{
	//pos値代入
	m_pos = pos;

	m_nSize = nSize;

	// 頂点情報を設定
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_nSize.x, m_pos.y - m_nSize.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_nSize.x, m_pos.y - m_nSize.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_nSize.x, m_pos.y + m_nSize.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_nSize.x, m_pos.y + m_nSize.y, 0.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//=============================================================================
// セット処理(中心点横)
//=============================================================================
void CScene2D::SetPosSizeSide(D3DXVECTOR3 pos, D3DXVECTOR2 nSize)
{
	//pos値代入
	m_pos = pos;

	m_nSize = nSize;

	// 頂点情報を設定
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

						//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y - m_nSize.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_nSize.x, m_pos.y - m_nSize.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_nSize.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_nSize.x, m_pos.y + m_nSize.y, 0.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//=============================================================================
// ひし形のポリゴン
//=============================================================================
void CScene2D::SetPosDiamond(D3DXVECTOR3 pos, D3DXVECTOR2 nSize, D3DXVECTOR4 VtxAngle)
{
	//pos値代入
	m_pos = pos;

	m_nSize = nSize;

	// 頂点情報を設定
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_nSize.x + VtxAngle.x, m_pos.y - m_nSize.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_nSize.x + VtxAngle.y, m_pos.y - m_nSize.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_nSize.x + VtxAngle.z, m_pos.y + m_nSize.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_nSize.x + VtxAngle.w, m_pos.y + m_nSize.y, 0.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//===============================================================================
// 向きを入れる
//===============================================================================
void CScene2D::SetRot(float fAngle, D3DXVECTOR2 size)
{
	// 頂点情報を設定
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + sinf(fAngle) * size.y + cosf(fAngle) * size.x, m_pos.y - sinf(fAngle) * size.x + cosf(fAngle) * size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + sinf(fAngle) * size.y - cosf(fAngle) * size.x, m_pos.y + sinf(fAngle) * size.x + cosf(fAngle) * size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - sinf(fAngle) * size.y + cosf(fAngle) * size.x, m_pos.y - sinf(fAngle) * size.x - cosf(fAngle) * size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x - sinf(fAngle) * size.y - cosf(fAngle) * size.x, m_pos.y + sinf(fAngle) * size.x - cosf(fAngle) * size.y, 0.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//===============================================================================
// 大きさを変える
//===============================================================================
void CScene2D::SetVtxPos(D3DXVECTOR3 *VxtPos)
{
	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = VxtPos[0];
	pVtx[1].pos = VxtPos[1];
	pVtx[2].pos = VxtPos[2];
	pVtx[3].pos = VxtPos[3];

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//===============================================================================
// 色を頂点事に変える
//===============================================================================
void CScene2D::SetColor(D3DXCOLOR *Color)
{
	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = *Color;
	pVtx[1].col = *Color;
	pVtx[2].col = *Color;
	pVtx[3].col = *Color;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//===============================================================================
//  回転処理
//===============================================================================
void CScene2D::SetSpin(D3DXVECTOR3 pos, float fAngle, float fLength, D3DXVECTOR3 rot)
{
	m_pos = pos;

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//拡縮回転処理
	pVtx[0].pos = D3DXVECTOR3((m_pos.x + sinf(-D3DX_PI + fAngle + rot.x) * fLength),
		(m_pos.y + cosf(-D3DX_PI + fAngle + rot.y) * fLength), 0.0f);

	pVtx[1].pos = D3DXVECTOR3((m_pos.x + sinf(D3DX_PI - fAngle + rot.x) * fLength),
		(m_pos.y + cosf(D3DX_PI - fAngle + rot.y) * fLength), 0.0f);

	pVtx[2].pos = D3DXVECTOR3((m_pos.x + sinf(-fAngle + rot.x) * fLength),
		(m_pos.y + cosf(-fAngle + rot.y) * fLength), 0.0f);

	pVtx[3].pos = D3DXVECTOR3((m_pos.x + sinf(fAngle + rot.x) * fLength),
		(m_pos.y + cosf(fAngle + rot.y) * fLength), 0.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// アニメーションの処理
//=============================================================================
void CScene2D::SetTexture(int PatternAnim, int X, int Y, int nNum)
{
	VERTEX_2D *pVtx;//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2((PatternAnim % X) * (1.0f / X) * (1 * nNum), (PatternAnim / X) * (1.0f / Y));
	pVtx[1].tex = D3DXVECTOR2((PatternAnim % X) * (1.0f / X) + (1.0f / X), (PatternAnim / X) * (1.0f / Y));
	pVtx[2].tex = D3DXVECTOR2((PatternAnim % X) * (1.0f / X) * (1 * nNum), (PatternAnim / X) * (1.0f / Y) + (1.0f / Y));
	pVtx[3].tex = D3DXVECTOR2((PatternAnim % X) * (1.0f / X) + (1.0f / X), (PatternAnim / X) * (1.0f / Y) + (1.0f / Y));

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// UVの設定
//=============================================================================
void CScene2D::SetUV(float fPattern, int nNum)
{
	VERTEX_2D *pVtx;//頂点情報へのポインタ

					//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, fPattern * nNum);
	pVtx[1].tex = D3DXVECTOR2(1.0f, fPattern * nNum);
	pVtx[2].tex = D3DXVECTOR2(0.0f, (fPattern * nNum) + (fPattern));
	pVtx[3].tex = D3DXVECTOR2(1.0f, (fPattern * nNum) + (fPattern));

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// UVの横の設定
//=============================================================================
void CScene2D::SetUVWidth(float fPattern, int nNum)
{
	VERTEX_2D *pVtx;//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(fPattern * nNum, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((fPattern * nNum) + (fPattern), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(fPattern * nNum, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((fPattern * nNum) + (fPattern), 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}