//=============================================================================
//
// UIの処理 [ui.cpp]
// Author : 有馬武志
//
//=============================================================================
#include "scene.h"
#include "scene2D.h"
#include "ui.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define UI_TEXTURE_NAME_1		"data\\TEXTURE\\Title\\UI\\title.png"			//読み込むテクスチャファイルenter
#define UI_TEXTURE_NAME_2		"data\\TEXTURE\\Title\\UI\\PressButton.png"		//読み込むテクスチャファイル
#define TITLE_SIZE_X			(500.0f)										//タイトルの横のサイズ
#define TITLE_SIZE_Y			(180.0f)										//タイトルの縦のサイズ
#define TITLE_SIZE_PERCENTAGE	(50.0f)											//拡大する割合
#define ALPHA_LIMIT				(0.1f)											//透明度の最低限度
#define ALPHA_LOWER				(0.015f)										//透明度の下げていく値

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CUi::m_pTexture[MAX_TEXTURE] = {};
int CUi::m_nNextType = 0;
int CUi::m_nNextFont = 0;

//UIの初期化処理
CUi::UI CUi::m_Ui = {};

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CUi::CUi() : CScene2D(5, CScene::OBJTYPE_LOGO)//優先優位イニシャライズ
{
	m_nType = 0;
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_fLength = 0.0f;
	m_fAngle = 0.0f;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntTutorial = 0;
	m_bOneCnt = false;
	m_nStateFont = 0;
	m_nStateType = 0;
	m_fAlpha = 1.0f;
}
//===============================================================================
//　デストラクタ
//===============================================================================
CUi::~CUi() { }

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CUi::Init(void)
{
	CScene2D::Init();

	D3DXCOLOR Color[4];

	for (int nCount = 0; nCount < 4; nCount++)
	{//それ以外の場合すべての初期化
		Color[nCount] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	CScene2D::SetColor(&Color[0]);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CUi::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CUi::Update(void)
{
	//UIの動き
	UiMovement();
}
//=============================================================================
// 描画処理
//=============================================================================
void CUi::Draw(void)
{
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputJoypad *pCInputJoyPad = CManager::GetInputJoyPad();

	CScene2D::Draw();
}
//===============================================================================
//　クリエイト
//===============================================================================
CUi * CUi::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, CUi::UI m_Ui)
{
	CUi *pLogo = NULL;

	//NULLチェック
	if (pLogo == NULL)
	{//メモリの動的確保

		pLogo = new CUi;

		if (pLogo != NULL)
		{
			//種類を割り当てる
			pLogo->m_nType = m_Ui;
			//オブジェクトクラスの生成
			pLogo->Init();
			//テクスチャを割り当てる
			pLogo->BindTexture(m_pTexture[m_Ui]);
			//位置を割り当てる
			pLogo->SetPosition(pos);
			//位置を割り当てる
			pLogo->m_pos = pos;
			//サイズの割り当て
			pLogo->m_size = size;
		}
	}

	return pLogo;
}

//===============================================================================
// テクスチャの読み込み
//===============================================================================
HRESULT CUi::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, UI_TEXTURE_NAME_1, &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, UI_TEXTURE_NAME_2, &m_pTexture[1]);

	return S_OK;
}
//===============================================================================
// テクスチャの破棄
//===============================================================================
void CUi::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_TEXTURE; nCount++)
	{
		//テクスチャーの破棄
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}
//===============================================================================
// UIの動き
//===============================================================================
void CUi::UiMovement(void)
{
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();

	//位置の割り当て
	D3DXVECTOR3 pos;
	pos = GetPosition();

	//移動量の割り当て
	D3DXVECTOR3 move;
	move = GetMove();

	//カラーの割り当てとカラーの初期化
	D3DXCOLOR Color[4];
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		Color[nCnt] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//===============================
	//       各テクスチャの動き
	//===============================
	if (m_nType == UI_TITLE_LOGO)
	{
		//拡大の処理
		if (m_size.x <= TITLE_SIZE_X) { m_size.x += (TITLE_SIZE_X / TITLE_SIZE_PERCENTAGE); }
		if (m_size.y <= TITLE_SIZE_Y) { m_size.y += (TITLE_SIZE_Y / TITLE_SIZE_PERCENTAGE); }
	}
	else if (m_nType == UI_PRESS_BUTTON)
	{
		//透明度の値を減らしていく
		m_fAlpha -= ALPHA_LOWER;

		//一定の値を切ったら初期値に戻す
		if (m_fAlpha < ALPHA_LIMIT) { m_fAlpha = 1.0f; }
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			Color[nCnt] = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha);
		}
	}

	//位置を更新(移動)
	pos.x += move.x;
	pos.y += move.y;

	//UIの色
	CScene2D::SetColor(&Color[0]);
	//UIの位置とサイズ
	CScene2D::SetPosSize(m_pos, m_size);
	//UIの動き
	CScene2D::SetMove(move);
}