//=============================================================================
//
// 液晶パネルの処理 [LDCpanel.h]
// Author : Takuto Ishida
//
//=============================================================================
#include "LCDpanel.h"
#include "manager.h"
#include "renderer.h"
#include "object.h"

//==================================
// マクロ定義
//==================================
#define LCD_INTERVAL		(120)		// 表示切替の間隔

//==================================
// 静的メンバ変数宣言
//==================================
LPDIRECT3DTEXTURE9 CLCDpanel::m_pTexture[MAX_LCD_TEXTURE] = {};

//==================================
// 生成処理
//==================================
CLCDpanel* CLCDpanel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fScale)
{
	CLCDpanel *pLCDpanel;

	pLCDpanel = new CLCDpanel;	// メモリを確保

	if (pLCDpanel != NULL)
	{// メモリ確保成功
		pLCDpanel->Init(pos, rot, fScale);
	}

	return pLCDpanel;
}

//===============================================================================
// テクスチャの読み込み
//===============================================================================
HRESULT CLCDpanel::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	const char *m_pTextureName[MAX_LCD_TEXTURE] =
	{// テクスチャへのパス
		{ "data\\TEXTURE\\game\\Destination\\Place\\ClockTower.jpg" },
		{ "data\\TEXTURE\\game\\Destination\\Place\\Fountain.jpg" },
		{ "data\\TEXTURE\\game\\Destination\\Place\\Hospital.jpg" },
		{ "data\\TEXTURE\\game\\Destination\\Place\\Main Street & TV Tower.jpg" },
		{ "data\\TEXTURE\\game\\Destination\\Place\\Norvesa.jpg" },
		{ "data\\TEXTURE\\game\\Destination\\Place\\place000.jpg" },
		{ "data\\TEXTURE\\game\\Destination\\Place\\Red Brick.jpg" },
		{ "data\\TEXTURE\\game\\Destination\\Place\\Sappro.jpg" },
		{ "data\\TEXTURE\\game\\Destination\\Place\\Susukino_Street.jpg" },
		{ "data\\TEXTURE\\game\\Destination\\Place\\Tanukikoji.jpg" },
		{ "data\\TEXTURE\\game\\Destination\\Place\\Tapiocar.jpg" },
	};

	for (int nCntTexture = 0; nCntTexture < MAX_LCD_TEXTURE; nCntTexture++)
	{// テクスチャの生成
		D3DXCreateTextureFromFile(pDevice, m_pTextureName[nCntTexture], &m_pTexture[nCntTexture]);
	}

	return S_OK;
}

//===============================================================================
// テクスチャの破棄
//===============================================================================
void CLCDpanel::Unload(void)
{
	for (int nCount = 0; nCount < MAX_LCD_TEXTURE; nCount++)
	{
		if (m_pTexture[nCount] != NULL)
		{//テクスチャの破棄
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//=========================================
// コンストラクタ
//=========================================
CLCDpanel::CLCDpanel()
{
	m_nCntDisp = 0;
	m_nCurTex = 0;
	m_pObject = NULL;
}

//=========================================
// デストラクタ
//=========================================
CLCDpanel::~CLCDpanel()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CLCDpanel::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fScale)
{
	CScene3D::Init();

	// テクスチャの設定
	CScene3D::BindTexture(m_pTexture[m_nCurTex]);

	// 位置とサイズの設定
	CScene3D::SetPosSize(pos + D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXVECTOR2(50.0f, 30.0f) * fScale);

	// 向きの設定
	CScene3D::SetRotation(rot);

	if (NULL == m_pObject)
	{// モデルの生成
		m_pObject = CObject::Create(D3DXVECTOR3(0.0f,0.0f,0.0f), GetRotation(), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 0.0f, 0, 37, CModel3D::MOVETYPE_NONE, 1);
		m_pObject->SetPosition(pos);
		m_pObject->SetScale(D3DXVECTOR3(3.5f, 3.8f, 4.0f) * fScale);
	}

	CScene3D::SetTypeNumber(CScene3D::TYPE_BLOOD);
	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CLCDpanel::Uninit(void)
{
	CScene3D::Uninit();
}

//=========================================
// 更新処理
//=========================================
void CLCDpanel::Update(void)
{
	// 切り替えるまでの時間をカウントアップ
	m_nCntDisp = (m_nCntDisp + 1) % LCD_INTERVAL;

	if (0 == m_nCntDisp)
	{// テクスチャを切り替える
		m_nCurTex = (m_nCurTex + 1) % MAX_LCD_TEXTURE;
		CScene3D::BindTexture(m_pTexture[m_nCurTex]);
	}
}

//=========================================
// 描画処理
//=========================================
void CLCDpanel::Draw(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 描画処理
	CScene3D::Draw();

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// モデルの描画
	m_pObject->Draw();
}

//=========================================
// スケール倍率の設定
//=========================================
void CLCDpanel::SetScale(float fScale)
{
	// 値の更新
	m_fScale = fScale;

	// サイズの設定
	D3DXVECTOR3 pos = GetPosition();
	CScene3D::SetPosSize(D3DXVECTOR3(pos.x, pos.y * m_fScale, pos.z), D3DXVECTOR2(50.0f, 30.0f) * m_fScale);
	m_pObject->SetScale(D3DXVECTOR3(3.5f, 3.8f, 4.0f) * m_fScale);
}