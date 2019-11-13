//=============================================================================
//
// オブジェクトの処理 [scene3D.cpp]
// Author : 有馬武志
//
//=============================================================================
#include "scene.h"
#include "scene3D.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CScene3D::CScene3D(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{	
	m_pVtxBuff = NULL;
	m_pTexture = NULL;
	m_pos = D3DXVECTOR3(0.0f,0.0f,0.0f);	//位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向き
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//動き
	m_Type = TYPE_NONE;
}
//===============================================================================
//　デストラクタ
//===============================================================================
CScene3D::~CScene3D()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScene3D::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//向き
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//位置
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//動き

	// 頂点情報の作成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL
	);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//法線ベクトル
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	//頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	if (m_Type == TYPE_WALL)
	{
		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
	}
	//頂点バッファのアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CScene3D::Uninit(void)
{
	// 頂点バッファの開放
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
void CScene3D::Update(void){}

//=============================================================================
// 描画処理
//=============================================================================
void CScene3D::Draw(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxView, mtxRot, mtxTrans;				//計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	if (m_Type != TYPE_WALL && m_Type != TYPE_GROUND && m_Type != TYPE_SHADOW && m_Type != TYPE_BLOOD && m_Type != TYPE_SPOTLIGHT)
	{
		//逆行列を設定
		m_mtxWorld._11 = mtxView._11;
		m_mtxWorld._12 = mtxView._21;
		m_mtxWorld._13 = mtxView._31;
		m_mtxWorld._21 = mtxView._12;
		m_mtxWorld._22 = mtxView._22;
		m_mtxWorld._23 = mtxView._32;
		m_mtxWorld._31 = mtxView._13;
		m_mtxWorld._32 = mtxView._23;
		m_mtxWorld._33 = mtxView._33;
	}
	else if (m_Type == TYPE_WALL || m_Type == TYPE_GROUND || m_Type == TYPE_SHADOW || m_Type == TYPE_BLOOD || m_Type == TYPE_SPOTLIGHT)
	{
		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	}

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	if (m_Type == TYPE_WALL)
	{
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// 裏面をカリング
	}

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//壁の場合
	if (m_Type == TYPE_WALL)
	{
		//カリングしない
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				
	}
}
//===============================================================================
//　クリエイト
//===============================================================================
CScene3D * CScene3D::Create(void)
{
	CScene3D *pScene3D = NULL;

	//NULLチェック
	if (pScene3D == NULL)
	{//メモリの動的確保

		pScene3D = new CScene3D;

		if (pScene3D != NULL)
		{
			//オブジェクトクラスの生成
			pScene3D->Init();
		}
	}

	return pScene3D;
}
//===============================================================================
// 共有テクスチャの割り当て
//===============================================================================
void CScene3D::BindTexture(LPDIRECT3DTEXTURE9 Texture)
{
	m_pTexture = Texture;
}
//=============================================================================
// 壁の設定
//=============================================================================
void CScene3D::SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 nSize)
{
	//代入
	m_pos = pos;
	m_rot = rot * D3DX_PI;
	m_size = nSize;

	VERTEX_3D *pVtx;

	// 頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);	

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, 0.0f, 0.0f);

	// 頂点バッファのアンロック
	m_pVtxBuff->Unlock();	
}
//=============================================================================
// セット処理
//=============================================================================
void CScene3D::SetPosSize(D3DXVECTOR3 pos,D3DXVECTOR2 nSize)
{
	m_pos = pos;

	m_size = nSize;

	// 頂点情報を設定
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点設定
	pVtx[0].pos = D3DXVECTOR3(- m_size.x,m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x,m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(- m_size.x,- m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_size.x,- m_size.y, 0.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//=============================================================================
// セット処理
//=============================================================================
void CScene3D::SetGroundPosSize(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	//位置とサイズの設定
	m_pos = pos;
	m_size = size;

	// 頂点情報を設定
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点設定
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, 0.0f, m_size.y);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, 0.0f, m_size.y);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, 0.0f, -m_size.y);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, 0.0f, -m_size.y);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//===============================================================================
//  カラーの設定処理
//===============================================================================
void CScene3D::SetColor(D3DXCOLOR Color)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = Color;
	pVtx[1].col = Color;
	pVtx[2].col = Color;
	pVtx[3].col = Color;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//=============================================================================
// エフェクトの処理
//=============================================================================
void CScene3D::SetVtxEffect(float fRadius)
{
	// 頂点情報を設定
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点設定
	pVtx[0].pos = D3DXVECTOR3(-fRadius, fRadius, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(fRadius, fRadius, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-fRadius, -fRadius, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(fRadius, -fRadius, 0.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//===============================================================================
// テクスチャのアニメーション
//===============================================================================
void CScene3D::SetUV(D3DXVECTOR2 *uv)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = uv[0];
	pVtx[1].tex = uv[1];
	pVtx[2].tex = uv[2];
	pVtx[3].tex = uv[3];

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//=============================================================================
// アニメーションの処理
//=============================================================================
void CScene3D::SetTexture(int PatternAnim, int X, int Y, int nNum)
{
	VERTEX_3D *pVtx;//頂点情報へのポインタ

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

//===============================================================================
// テクスチャの法線設定
//===============================================================================
void CScene3D::SetNor(D3DXVECTOR3 Nor)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//法線ベクトル
	pVtx[0].nor = Nor;
	pVtx[1].nor = Nor;
	pVtx[2].nor = Nor;
	pVtx[3].nor = Nor;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//===============================================================================
//  回転処理
//===============================================================================
void CScene3D::SetSpin(D3DXVECTOR3 pos, float fAngle, float fLength, D3DXVECTOR3 rot)
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
