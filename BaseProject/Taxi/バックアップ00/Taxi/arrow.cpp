//=============================================================================
//
// 矢印の処理 [arrow.cpp]
// Author : shun yokomichi
//
//=============================================================================
#include "scene.h"
#include "model3D.h"
#include "manager.h"
#include "arrow.h"
#include "logo.h"
#include "scene3D.h"
#include "gamecamera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_NAME_1	"data\\MODEL\\矢印\\Arrow.x"			//読み込むテクスチャファイル
#define AREA_RADIUS		(m_fRadius + 6000.0f)					//半径判定の大きさ
#define MAX_SCALE		(2.0f)									//スケールの最大値
#define MIN_SCALE		(1.0f)									//スケールの最小値
#define ADD_SCALE		(0.05f)									//スケールの加算値
#define COL_CAHANGE		(0.01f)									//色の変化量
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPD3DXMESH CArrow::m_pMeshModel = NULL;							//メッシュ情報へのポインタ
LPD3DXBUFFER CArrow::m_pBuffMatModel = NULL;					//マテリアルの情報へのポインタ
DWORD CArrow::m_nNumMatModel = NULL;							//マテリアルの情報数
LPDIRECT3DTEXTURE9      CArrow::m_pTextureMT = NULL;
D3DXVECTOR3 CArrow::m_LoadVtxMax = {};
D3DXVECTOR3 CArrow::m_LoadVtxMin = {};


//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CArrow::CArrow() : CModel3D(5, CScene::OBJTYPE_3DMODEL)
{
	m_pLogo = NULL;
	m_pCommand = NULL;
	m_scale = D3DXVECTOR3(0.1f, 0.1f, 0.1f);
	m_nSizeScale = MIN_SCALE;
	m_pScaleType = SCZLE_SMALL;

}
//===============================================================================
//　デストラクタ
//===============================================================================
CArrow::~CArrow(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CArrow::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	LPDIRECT3DSURFACE9 pRenderOrg;
	LPDIRECT3DSURFACE9 pBuffOrg;

	//3DモデルのInit
	CModel3D::Init();

	// 位置の初期化
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 rot = D3DXVECTOR3(-0.1f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	m_colChange = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

	//位置の代入
	CModel3D::SetPosition(pos);
	CModel3D::SetRot(rot);

	// Textureの作成
	pDevice->CreateTexture(SCREEN_WIDTH, SCREEN_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTextureMT, NULL);

	// レンダラーの取得
	m_pTextureMT->GetSurfaceLevel(0, &m_pRenderMT);

	// バッファの生成
	pDevice->CreateDepthStencilSurface(SCREEN_WIDTH, SCREEN_HEIGHT, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pBuffMT, NULL);

	pDevice->GetRenderTarget(0, &pRenderOrg);		// 現在のレンダラーターゲットを保存
	pDevice->GetDepthStencilSurface(&pBuffOrg);		// 現在のバッファを保存
	pDevice->SetRenderTarget(0, m_pRenderMT);		// レンダラーの設定
	pDevice->SetDepthStencilSurface(m_pBuffMT);		// バッファの設定
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// 元に戻す
	pDevice->SetRenderTarget(0, pRenderOrg);		// レンダラー
	pDevice->SetDepthStencilSurface(pBuffOrg);		// バッファ

	// ビューポートの初期化
	m_viewportMT.X = 0;						// Xの設定
	m_viewportMT.Y = 0;						// Yの設定
	m_viewportMT.Width = SCREEN_WIDTH;		// 幅の設定
	m_viewportMT.Height = SCREEN_HEIGHT;	// 高さの設定
	m_viewportMT.MinZ = 0.0f;				// 手前
	m_viewportMT.MaxZ = 1.0f;				// 奥

	m_pLogo = CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 270.0f, 0.0f), D3DXVECTOR2(400.0f, 400.0f), 1, 0, CLogo::TYPE_MULTIRENDERING);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CArrow::Uninit(void)
{
	// ロゴの破棄
	if (m_pLogo != NULL) { m_pLogo->Uninit(); m_pLogo = NULL; }
	if (m_pCommand != NULL) { m_pCommand->Uninit(); m_pCommand = NULL; }
	//3DモデルのUninit
	CModel3D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CArrow::Update(void)
{
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputJoypad *pCInputJoyPad = CManager::GetInputJoyPad();

	//3DモデルのUpdate
	CModel3D::Update();

	//位置の初期化
	D3DXVECTOR3 pos = CModel3D::GetPosition();

	// 移動量の初期化
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//スケール
	D3DXVECTOR3 scale = CModel3D::GetScale();

	//向きの代入
	D3DXVECTOR3 &rot = CModel3D::GetRot();

	scale = AreaDetermine(pos, scale);
	rot = SetRot(pos, rot);

	// 各種情報の代入
	CModel3D::SetScale(scale);
	CModel3D::SetMove(move);
	CModel3D::SetRot(rot);
	CModel3D::Setcol(m_col);
}
//=============================================================================
// 描画処理
//=============================================================================
void CArrow::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	LPDIRECT3DSURFACE9 pRenderOrg;
	LPDIRECT3DSURFACE9 pBuffOrg;
	D3DXMATRIX pmtxProjectionOrg, pmtxViewOrg;

	pDevice->GetRenderTarget(0, &pRenderOrg);						// 現在のレンダラーターゲットを保存
	pDevice->GetDepthStencilSurface(&pBuffOrg);						// 現在のバッファを保存
	pDevice->GetViewport(&m_viewportMT);

	pDevice->GetTransform(D3DTS_VIEW, &pmtxViewOrg);				// ビューマトリックスの取得
	pDevice->GetTransform(D3DTS_PROJECTION, &pmtxProjectionOrg);	// プロジェクションマトリックスの取得

	pDevice->SetRenderTarget(0, m_pRenderMT);		// レンダラーの設定
	pDevice->SetDepthStencilSurface(m_pBuffMT);		// バッファの設定

	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// ビューポートの設定
	pDevice->SetViewport(&m_viewportMT);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&pmtxViewOrg);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(&pmtxViewOrg, &D3DXVECTOR3(0.0f, 90.0f, -100.0f), &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&pmtxProjectionOrg);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&pmtxProjectionOrg,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH /
		(float)SCREEN_HEIGHT,
		10.0f,
		200.0f);

	//頂点法線の自動正規化
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	CModel3D::Draw();	// 描画処理

	//頂点法線の自動正規化
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);

	// 元に戻す
	pDevice->SetRenderTarget(0, pRenderOrg);		// レンダラー
	pDevice->SetDepthStencilSurface(pBuffOrg);		// バッファ

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &pmtxViewOrg);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &pmtxProjectionOrg);

	if (m_pCommand != NULL) { m_pCommand->Draw(); }

}
//=============================================================================
// エリアの判定
//=============================================================================
D3DXVECTOR3 CArrow::AreaDetermine(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	// 現在の距離
	float fArea = ((pos.x - m_goalpos.x) * (pos.x - m_goalpos.x)) + ((pos.z - m_goalpos.z) * (pos.z - m_goalpos.z));

	if (fArea <= AREA_RADIUS * AREA_RADIUS)
	{	// エリアの範囲内なら
		if (fArea <= m_fRadius * m_fRadius)
		{
			// ロゴの廃棄 & ゴール位置の矢印
			if (m_pLogo != NULL)
			{
				m_pLogo->Uninit();
				m_pLogo = NULL;
				if (m_pCommand == NULL) { m_pCommand = CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 100.0f, 0.0f), D3DXVECTOR2(200.0f, 30.0f), CTexture::TYPE_COMMAND_000, 0, CLogo::TYPE_LOGO);}
				//CModel3D::SetPosition(D3DXVECTOR3(m_goalpos.x, m_goalpos.y + 100.0f, m_goalpos.z));
				scale = D3DXVECTOR3(MIN_SCALE, MIN_SCALE, MIN_SCALE);
				m_pScaleType = SCZLE_NOMAL;
			}
		}
		else
		{
			m_colChange.r += COL_CAHANGE;
			m_colChange.g -= COL_CAHANGE;

			if (m_colChange.r >= 1.0f) { m_colChange.r = 1.0f; }
			if (m_colChange.g <= 0.0f) { m_colChange.g = 0.0f; }

			// ロゴの生成
			if (m_pCommand != NULL) { m_pCommand->Uninit(); m_pCommand = NULL; }
			if (m_pLogo == NULL)
			{
				m_pLogo = CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 270.0f, 0.0f), D3DXVECTOR2(400.0f, 400.0f), 1, 0, CLogo::TYPE_MULTIRENDERING);
				m_pScaleType = SCZLE_SMALL;
			}

			if (m_pScaleType == SCZLE_SMALL)
			{	// スケールサイズが小さい場合
				m_nSizeScale += ADD_SCALE;
				scale = D3DXVECTOR3(m_nSizeScale, m_nSizeScale, m_nSizeScale);
				if (m_nSizeScale >= MAX_SCALE) { m_pScaleType = SCZLE_BIG; }		// 種類を代入
			}
			else if (m_pScaleType == SCZLE_BIG)
			{	// スケールサイズが大きい場合
				m_nSizeScale -= ADD_SCALE;
				scale = D3DXVECTOR3(m_nSizeScale, m_nSizeScale, m_nSizeScale);
				if (m_nSizeScale <= MIN_SCALE) { m_pScaleType = SCZLE_SMALL; }		// 種類を代入
			}
			m_col = D3DXCOLOR(m_colChange.r, m_colChange.g, 0.0f, 1.0f);			// 色の代入
		}
	}
	else { scale = D3DXVECTOR3(MIN_SCALE, MIN_SCALE, MIN_SCALE);  	m_col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);}

	return scale;
}

//=============================================================================
// 向き設定
//=============================================================================
D3DXVECTOR3 CArrow::SetRot(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	if (m_pScaleType != SCZLE_NOMAL)
	{	// ロゴの場合(向きをゴールに向ける)
		rot.x = -0.1f;
		rot.y = atan2f(m_goalpos.x - pos.x, m_goalpos.z - pos.z) + D3DX_PI;
	}
	else { rot.x = D3DX_PI * -0.5f; }

	// 現在方向調整
	if (rot.x > D3DX_PI) { rot.x -= D3DX_PI * 2.0f; }	// X
	if (rot.x < -D3DX_PI) { rot.x += D3DX_PI * 2.0f; }	// X
	if (rot.y > D3DX_PI) { rot.y -= D3DX_PI * 2.0f; }	// Y
	if (rot.y < -D3DX_PI) { rot.y += D3DX_PI * 2.0f; }	// Y
	if (rot.z > D3DX_PI) { rot.z -= D3DX_PI * 2.0f; }	// Z
	if (rot.z < -D3DX_PI) { rot.z += D3DX_PI * 2.0f; }	// Z

	return rot;
}

//===============================================================================
//　クリエイト
//===============================================================================
CArrow * CArrow::Create(int nArrowType, D3DXVECTOR3 pos, D3DXVECTOR3 GoalPos, float fRadius)
{
	CArrow *pArrow = NULL;

	// NULLチェック
	if (pArrow == NULL)
	{// メモリの動的確保

		pArrow = new CArrow;

		if (pArrow != NULL)
		{
			// 種類の設定
			pArrow->BindModel(m_pMeshModel, m_pBuffMatModel, m_nNumMatModel, NULL, m_LoadVtxMax, m_LoadVtxMin);

			// オブジェクトごとの設定用タイプ
			pArrow->m_nArrow = nArrowType;

			// オブジェクトクラスの生成
			pArrow->Init();

			// 位置を代入
			pArrow->SetPosition(pos);
			pArrow->m_goalpos = GoalPos;

			// 半径の代入
			pArrow->m_fRadius = fRadius;
			pArrow->Setcol(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		}
	}

	return pArrow;
}//===============================================================================
// Xファイルの読み込み
//===============================================================================
HRESULT CArrow::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//マテリアルデータへのポインタ
	D3DXMATERIAL *pMat;

	// Xファイルの読み込み
	D3DXLoadMeshFromX(MODEL_NAME_1, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel, NULL, &m_nNumMatModel, &m_pMeshModel);
	//マテリアル情報からテクスチャの取得
	pMat = (D3DXMATERIAL*)m_pBuffMatModel->GetBufferPointer();

	int nNumVtx;		//頂点数
	DWORD sizeFVF;		//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファへのポインタ

	//モデルの最大値・最小値を取得する
	m_LoadVtxMax = D3DXVECTOR3(-10000, -10000, -10000);	//最大値
	m_LoadVtxMin = D3DXVECTOR3(10000, 10000, 10000);	//最小値

																			//頂点数を取得
	nNumVtx = m_pMeshModel->GetNumVertices();

	//頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(m_pMeshModel->GetFVF());

	//頂点バッファのロック
	m_pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		//頂点座標の代入

		//最大値
		if (vtx.x > m_LoadVtxMax.x)
		{
			m_LoadVtxMax.x = vtx.x;
		}
		if (vtx.y > m_LoadVtxMax.y)
		{
			m_LoadVtxMax.y = vtx.y;
		}
		if (vtx.z > m_LoadVtxMax.z)
		{
			m_LoadVtxMax.z = vtx.z;
		}
		//最小値
		if (vtx.x < m_LoadVtxMin.x)
		{
			m_LoadVtxMin.x = vtx.x;
		}
		if (vtx.y < m_LoadVtxMin.y)
		{
			m_LoadVtxMin.y = vtx.y;
		}
		if (vtx.z < m_LoadVtxMin.z)
		{
			m_LoadVtxMin.z = vtx.z;
		}

		//サイズ文のポインタを進める
		pVtxBuff += sizeFVF;
	}

	//頂点バッファのアンロック
	m_pMeshModel->UnlockVertexBuffer();

	return S_OK;
}

//===============================================================================
// テクスチャの破棄
//===============================================================================
void CArrow::UnLoad(void)
{
	// メッシュの開放
	if (m_pMeshModel != NULL)
	{
		m_pMeshModel->Release();
		m_pMeshModel = NULL;
	}
	// マテリアルの開放
	if (m_pBuffMatModel != NULL)
	{
		m_pBuffMatModel->Release();
		m_pBuffMatModel = NULL;
	}
}