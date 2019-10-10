//=============================================================================
//
// パーティクルビルボード処理 [particlebillboad.cpp]
// Author : 佐藤 安純
//
//=============================================================================
#include "particlebillboad.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "shadermgr.h"
#include "rotationshader.h"
#include "gamecamera.h"
#include "loadText.h"
#include "tutorial.h"

//==================================
// マクロ定義
//==================================
#define MAX_LIFE_GRAND		(30)
#define CHANGE_SIZE			(0.1f)
#define VECTOR_ZERO			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))		//ベクトルの初期化
#define FLAM				(8)
#define CHANGE_A			(0.06f)
#define CHANGE_A_STAR		(0.14f)
#define RAND_ANGLE			(314)
#define CICLE_MIN			(-200)
#define CICLE_MAX			(400)
#define LIGHT_MIN			(18.0f)
#define LIGHT_MAX			(32.0f)
#define SIZE_UP				(0.15f)
#define SPEW_MIN			(-100)
#define SPEW_MAX			(200)

//==================================
// 静的メンバ変数宣言
//==================================
LPDIRECT3DTEXTURE9 * CParticleBillboad::m_ppTexture = NULL;
int CParticleBillboad::m_nMaxTexture = 0;

//=============================================================================
// 生成処理
//=============================================================================
CParticleBillboad * CParticleBillboad::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const int nNumTex, DRAWTYPE DrawType, TYPE type, D3DXCOLOR col, int nLife, float fSpeed, int nMinHight, int nMaxHight, bool bZBuffu, int nPriority)
{
	//インスタンスの生成
	CParticleBillboad * pParticle = NULL;

	if (type == TYPE_NORMAL)
	{//通常
		pParticle = new CParticleBillboad(nPriority);
	}
	else if (type == TYPE_SPEW)
	{//噴水型
		pParticle = new CSpewParticle(nPriority);
	}
	else if (type == TYPE_GRANDSMOKE)
	{//その場で拡大
		pParticle = new CGrandSmokeParticle(nPriority);
	}
	else if (type == TYPE_LIGHT)
	{//拡大縮小
		pParticle = new CLightParticle(nPriority);
	}
	else if (type == TYPE_CICLE)
	{//円形型
		pParticle = new CCicleParticle(nPriority);
	}
	else if (type == TYPE_STAR)
	{//爆発型
		pParticle = new CStarParticle(nPriority);
	}

	if (pParticle != NULL)
	{
		pParticle->SetSpeed(fSpeed);						//スピードの設定
		pParticle->SetMinHight(nMinHight);					//高さの最小値
		pParticle->SetMaxHight(nMaxHight);					//高さの最大値
		pParticle->SetSize(D3DXVECTOR2(size.x, size.y));	//サイズの設定

		pParticle->Init();					//初期化処理

		//設定処理
		pParticle->SetPos(pos);				//位置の設定
		pParticle->SetDrawType(DrawType);	//描画タイプの設定
		pParticle->SetColor(col);			//色の設定
		pParticle->SetColorA(col.a);		//色のa値の設定
		pParticle->SetZbuffu(bZBuffu);		//Zバッファの設定
		pParticle->SetLife(nLife);			//ライフの設定

		if (m_ppTexture[nNumTex] != NULL)
		{
			pParticle->BindTexture(m_ppTexture[nNumTex]);		//テクスチャの割当て
		}
	}
	else
	{
		MessageBox(NULL, TEXT("pParticleがNULLです"), TEXT("CEffectTool::Create()"), MB_OK);
	}

	return pParticle;
}

//=========================================
// デフォルトコンストラクタ
//=========================================
CParticleBillboad::CParticleBillboad() : CScene(3, CScene::OBJTYPE_EFFECT)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
}

//=========================================
// コンストラクタ
//=========================================
CParticleBillboad::CParticleBillboad(int nPriority) : CScene(nPriority, CScene::OBJTYPE_EFFECT)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
}

//=========================================
// デストラクタ
//=========================================
CParticleBillboad::~CParticleBillboad(){}

//=============================================================================
// テクスチャの読み込み
//=============================================================================
void CParticleBillboad::LoadTexture(void)
{
	//レンダリングクラスを取得
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//テクスチャ情報の取得
	CLoadTextEffect * pLoadTextTexture = {};
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		pLoadTextTexture = CGame::GetLoadEffect();	//パーティクルテクスチャのポインタを取得
	}
	else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{
		pLoadTextTexture = CTutorial::GetLoadEffect();	//パーティクルテクスチャのポインタを取得
	}
	CLoadText::TEX_INFO TexInfo = pLoadTextTexture->GetTexInfo();	//テクスチャ情報の取得
	m_nMaxTexture = TexInfo.nMaxTex;								//テクスチャの最大数を取得


	//テクスチャをサイズ分ヒープ領域からメモリを動的確保する
	if (m_ppTexture == NULL)
	{
		m_ppTexture = new LPDIRECT3DTEXTURE9[m_nMaxTexture];
	}

	// テクスチャの生成
	for (int nCntTex = 0; nCntTex < m_nMaxTexture; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			TexInfo.ppFileName[nCntTex],
			&m_ppTexture[nCntTex]);
	}
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CParticleBillboad::UnloadTexture(void)
{
	//テクスチャの破棄
	for (int nCntTex = 0; nCntTex < m_nMaxTexture; nCntTex++)
	{
		if (m_ppTexture[nCntTex] != NULL)
		{
			m_ppTexture[nCntTex]->Release();
			m_ppTexture[nCntTex] = NULL;
		}
	}

	//テクスチャポインタの破棄
	if (m_ppTexture != NULL)
	{
		delete[] m_ppTexture;
		m_ppTexture = NULL;
	}
}

//=========================================
// 初期化処理
//=========================================
HRESULT CParticleBillboad::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_bDeth = false;

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_3D,
								D3DPOOL_MANAGED,
								&m_pVtxBuff,
								NULL);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y, 0.0f);

	//法線ベクトル
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//頂点カラー
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファのアンロックする
	m_pVtxBuff->Unlock();
	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CParticleBillboad::Uninit(void)
{
	if (m_pVtxBuff != NULL)
	{// 頂点情報の破棄
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	if (m_pTexture != NULL)
	{
		m_pTexture = NULL;
	}

	// 自分を破棄
	Release();
}

//=========================================
// 更新処理
//=========================================
void CParticleBillboad::Update(void)
{
	//クリッピング処理の追加
	CManager::MODE mode = CManager::GetMode();
	CGameCamera * pGameCamera = NULL;

	if (mode == CManager::MODE_GAME)
	{//ゲーム
		pGameCamera = CGame::GetGameCamera();
	}
	else if (mode == CManager::MODE_TUTORIAL)
	{//チュートリアル
		pGameCamera = CTutorial::GetCamera();
	}

	SetDraw(pGameCamera->ClippingPos(m_pos));
}

//=========================================
// 描画処理
//=========================================
void CParticleBillboad::Draw(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxView;	// 計算用マトリックス

	//アルファテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	if (m_DrawType == DRAWTYPE_ADD)
	{
		//αブレンディングを加算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	if (m_bZBuffu)
	{
		//デフォルトの場合FALSEになっているからTRUEにする
		pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	}

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

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

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	if (m_pVtxBuff != NULL)
	{
		//頂点バッファをデバイスのデータストリームに設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, m_pTexture);

		// ライティングを無効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		// ライティングを有効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}

	if (m_DrawType == DRAWTYPE_ADD)
	{
		//αブレンディングを元に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
	if (m_bZBuffu)
	{
		//デフォルトの場合FALSEになっているからTRUEにする
		pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	}
}

//=========================================
// 色の設定
//=========================================
void CParticleBillboad::SetColor(D3DXCOLOR col)
{
	m_col = col;

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラー
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//頂点バッファのアンロックする
	m_pVtxBuff->Unlock();
}

//=========================================
// サイズの再設定
//=========================================
void CParticleBillboad::RemakeSize(D3DXVECTOR2 size)
{
	m_size = size;

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y, 0.0f);

	//頂点バッファのアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 回転の修正処理
//=============================================================================
void CParticleBillboad::SetAngle(float * pAngle)
{
	//角度の修正
	if (*pAngle < -D3DX_PI)
	{
		*pAngle += D3DX_PI * 2.0f;
	}
	if (*pAngle > D3DX_PI)
	{
		*pAngle -= D3DX_PI * 2.0f;
	}
}

//=========================================
// UV座標の設定
//=========================================
void CParticleBillboad::SetUV(D3DXVECTOR2 MinUV, D3DXVECTOR2 MaxUV)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(MinUV.x, MaxUV.y);
	pVtx[1].tex = D3DXVECTOR2(MaxUV.x, MaxUV.y);
	pVtx[2].tex = D3DXVECTOR2(MinUV.x, MinUV.y);
	pVtx[3].tex = D3DXVECTOR2(MaxUV.x, MinUV.y);

	//頂点バッファのアンロックする
	m_pVtxBuff->Unlock();
}

//*****************************************************************************
//
// 星型パーティクルクラス
//
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CSpewParticle::CSpewParticle(int nPriority) : CParticleBillboad(nPriority) {}

//=============================================================================
// デストラクタ
//=============================================================================
CSpewParticle::~CSpewParticle() {}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSpewParticle::Init(void)
{
	CParticleBillboad::Init();

	//変数の初期化
	m_nCountFlam = 0;		//フレームカウンター
	m_nLife = 30;			//表示時間
	m_fChangeCol_a = 0.0f;	//透明度の変化量;
	m_bGravity = true;		//重力のON・OFF
	m_fGravity = 0.0f;		//重力

	float fPosX = (float)(SPEW_MIN + rand() % SPEW_MAX);
	float fPosZ = (float)(SPEW_MIN + rand() % SPEW_MAX);

	//目的の角度を求める
	float fAngle = atan2f(fPosX, fPosZ);

	//移動量を求める
	m_move.x = sinf(fAngle) * m_fSpeed;
	m_move.z = cosf(fAngle) * m_fSpeed;

	//高さ設定
	float fHight = (float)(m_nMinHight + rand() % m_nMaxHight);
	m_move.y = fHight;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSpewParticle::Uninit(void)
{
	CParticleBillboad::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CSpewParticle::Update(void)
{
	//重力を足す
	if (m_bGravity == true)
	{
		m_move.y -= m_fGravity;
	}

	//時間の加算
	m_nCountFlam++;

	if (m_nCountFlam > FLAM)	//一定のフレーム数が経ったら
	{
		m_nLife--;						//表示時間の減算

		if (m_nLife < 0)
		{
			m_colA -= m_fChangeCol_a;	//透明度を下げる

			if (m_colA < 0.0f)
			{
				m_bDeth = true;
			}
			else
			{
				D3DXCOLOR col = CParticleBillboad::GetColor();
				CParticleBillboad::SetColor(D3DXCOLOR(col.r, col.g, col.b, m_colA));
			}
		}
	}

	D3DXVECTOR3 pos = CParticleBillboad::GetPos();
	pos += m_move;
	CParticleBillboad::SetPos(pos);

	CParticleBillboad::Update();

	//死亡状態だったら破棄
	if (m_bDeth) { Uninit(); }
}

//=============================================================================
// 描画処理
//=============================================================================
void CSpewParticle::Draw(void)
{
	CParticleBillboad::Draw();
}

//*****************************************************************************
//
// 土煙パーティクルクラス
//
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CGrandSmokeParticle::CGrandSmokeParticle(int nPriority) : CParticleBillboad(nPriority) {}

//=============================================================================
// デストラクタ
//=============================================================================
CGrandSmokeParticle::~CGrandSmokeParticle() {}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGrandSmokeParticle::Init(void)
{
	CParticleBillboad::Init();

	m_nCountFlam = 0;		//フレームカウンター
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGrandSmokeParticle::Uninit(void)
{
	CParticleBillboad::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CGrandSmokeParticle::Update(void)
{
	//時間の加算
	m_nCountFlam++;

	m_nLife--;				//表示時間の減算

	if (m_nLife < 0)
	{
		m_colA -= CHANGE_A;	//透明度を下げる

		if (m_colA < 0.0f)
		{
			m_bDeth = true;
		}
		else
		{
			D3DXCOLOR col = CParticleBillboad::GetColor();
			CParticleBillboad::SetColor(D3DXCOLOR(col.r, col.g, col.b, m_colA));
		}
	}

	//サイズの更新
	D3DXVECTOR2 size = CParticleBillboad::GetSize();
	size.x += SIZE_UP;
	size.y += SIZE_UP;

	CParticleBillboad::SetSize(size);
	CParticleBillboad::Update();

	//死亡状態だったら破棄
	if (m_bDeth) { Uninit(); }
}

//=============================================================================
// 描画処理
//=============================================================================
void CGrandSmokeParticle::Draw(void)
{
	CParticleBillboad::Draw();
}

//*****************************************************************************
//
// ライトパーティクル
//
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CLightParticle::CLightParticle(int nPriority) : CParticleBillboad(nPriority) {}

//=============================================================================
// デストラクタ
//=============================================================================
CLightParticle::~CLightParticle() {}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLightParticle::Init(void)
{
	CParticleBillboad::Init();
	m_fChangeSize = CHANGE_SIZE;
	m_nLife = MAX_LIFE_GRAND;	//表示時間
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLightParticle::Uninit(void)
{
	CParticleBillboad::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CLightParticle::Update(void)
{
	//サイズの保存
	D3DXVECTOR2 size = CParticleBillboad::GetSize();

	//パーティクルのサイズを変更する
	if (size.x > LIGHT_MAX || size.x < LIGHT_MIN)
	{
		m_fChangeSize *= -1;	//符号の変更
	}

	//サイズの変更
	size.x += m_fChangeSize;
	size.y += m_fChangeSize;

	//サイズの設定
	SetSize(size);

	CParticleBillboad::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CLightParticle::Draw(void)
{
	CParticleBillboad::Draw();
}

//*****************************************************************************
//
// 円形パーティクルクラス
//
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CCicleParticle::CCicleParticle(int nPriority) : CParticleBillboad(nPriority) {}

//=============================================================================
// デストラクタ
//=============================================================================
CCicleParticle::~CCicleParticle() {}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCicleParticle::Init(void)
{
	CParticleBillboad::Init();

	m_nCountFlam = 0;						//フレームカウンター
	m_nLife = 30;							//表示時間
	m_fChangeCol_a = 0.0f;					//透明度の変化量;
	m_bGravity = true;						//重力のON・OFF
	m_fGravity = 0.0f;						//重力

	float fPosX = (float)(CICLE_MIN + rand() % CICLE_MAX);
	float fPosY = (float)(CICLE_MIN + rand() % CICLE_MAX);

	//目的の角度を求める
	float fAngle = atan2f(fPosX, fPosY);

	//移動量を求める
	m_move.x = sinf(fAngle) * m_fSpeed;
	m_move.z = cosf(fAngle) * m_fSpeed;
	m_move.y = cosf(fAngle) * m_fSpeed;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCicleParticle::Uninit(void)
{
	CParticleBillboad::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CCicleParticle::Update(void)
{
	//重力を足す
	if (m_bGravity == true)
	{
		m_move.y -= m_fGravity;
	}

	m_nLife--;						//表示時間の減算

	if (m_nLife < 0)
	{
		m_colA -= m_fChangeCol_a;	//透明度を下げる

		if (m_colA < 0.0f)
		{
			m_bDeth = true;
		}
		else
		{
			D3DXCOLOR col = CParticleBillboad::GetColor();
			CParticleBillboad::SetColor(D3DXCOLOR(col.r, col.g, col.b, m_colA));
		}
	}

	CParticleBillboad::Update();

	//死亡状態だったら破棄
	if (m_bDeth) { Uninit(); }
}

//=============================================================================
// 描画処理
//=============================================================================
void CCicleParticle::Draw(void)
{
	CParticleBillboad::Draw();
}


//*****************************************************************************
//
// 星パーティクルクラス
//
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CStarParticle::CStarParticle(int nPriority) : CParticleBillboad(nPriority)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CStarParticle::~CStarParticle()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CStarParticle::Init(void)
{
	CParticleBillboad::Init();

	m_nCountFlam = 0;			//フレームカウンター
								//向き設定
	//m_rot.x = (float)(rand() % RAND_ANGLE);
	//m_rot.z = (float)(rand() % RAND_ANGLE);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CStarParticle::Uninit(void)
{
	CParticleBillboad::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CStarParticle::Update(void)
{
	//時間の加算
	m_nCountFlam++;

	m_nLife--;				//表示時間の減算

	if (m_nLife < 0)
	{
		m_colA -= CHANGE_A_STAR;	//透明度を下げる

		if (m_colA < 0.0f)
		{
			m_bDeth = true;
		}
		else
		{
			D3DXCOLOR col = CParticleBillboad::GetColor();
			CParticleBillboad::SetColor(D3DXCOLOR(col.r, col.g, col.b, m_colA));
		}
	}

	CParticleBillboad::Update();

	//死亡状態だったら破棄
	if (m_bDeth) { Uninit(); }
}

//=============================================================================
// 描画処理
//=============================================================================
void CStarParticle::Draw(void)
{
	CParticleBillboad::Draw();
}