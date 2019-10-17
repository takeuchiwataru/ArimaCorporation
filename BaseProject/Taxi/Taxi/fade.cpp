//=============================================================================
//
// フェードの処理 [fade.cpp]
// Author : 有馬武志
//
//=============================================================================
#include "scene.h"
#include "scene2D.h"
#include "fade.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_NAME_1					"data\\TEXTURE\\.png"		//読み込むテクスチャファイル
#define FADE_TIME						(0.02f)						//フェードの時間

//ロード画像
#define LOAD_POS	(D3DXVECTOR3(1150.0f, 680.0f, 0.0f))
#define LOAD_SIZE	(D3DXVECTOR2(100.0f, 25.0f))

//アニメーション画像
#define ANIM_POS	(D3DXVECTOR3(1025.0f, 665.0f, 0.0f))
#define ANIM_SIZE	(D3DXVECTOR2(30.0f, 30.0f))
#define ANIM_UV		(0.2f)
#define MAX_PATTERN	(5)
#define ANIM_COUNT	(7)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CFade::m_pTexture[MAX_TEXTURE] = {};
CFade::FADE CFade::m_fade = CFade::FADE_NONE;
int CFade::m_nModeNext = CManager::MODE_TITLE;
CFade::MODEFADE CFade::m_modegame = CFade::MODEFADE_NONE;
CGame *CFade::m_pGame = NULL;
//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CFade::CFade() : CScene2D(7, CScene::OBJTYPE_FADE)//優先優位イニシャライズ
{
	// 値の初期化
	m_fade = FADE_NONE;									//フェードイン状態
	m_colorFade = D3DXCOLOR(0.0f,0.0f, 0.0f, 0.0f);		//黒い不透明にしておく
	m_Size = D3DXVECTOR2(0.0f, 0.0f);
	m_pNowLoading = NULL;
	m_pAnim = NULL;
	m_nAnimNum = 0;
	m_nCountAnim = 0;
}
//===============================================================================
//　デストラクタ
//===============================================================================
CFade::~CFade(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CFade::Init()
{	
	CScene2D::Init();

	//位置の宣言
	D3DXVECTOR3 VtxPos[4];

	//カラーの宣言と初期化
	D3DXCOLOR Color[4];
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		Color[nCnt] = m_colorFade;
	}

	//テクスチャ設定 縦スクロール用
	VtxPos[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	VtxPos[1] = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	VtxPos[2] = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	VtxPos[3] = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//フェードの状態を初期化
	m_modegame = MODEFADE_NONE;

	//オブジェクトの種類の設定
	CScene::SetObjType(CScene::OBJTYPE_FADE);

	//カラーの割り当て
	CScene2D::SetColor(&Color[0]);

	//位置の設定
	CScene2D::SetVtxPos(&VtxPos[0]);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CFade::Uninit(void)
{
	CScene2D::Uninit();

	//ロード画像
	if (m_pNowLoading != NULL)
	{
		m_pNowLoading->Uninit();
		m_pNowLoading = NULL;
	}

	//アニメーション画像
	if (m_pAnim != NULL)
	{
		m_pAnim->Uninit();
		m_pAnim = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CFade::Update(void)
{
	//カラーの宣言と初期化
	D3DXCOLOR Color[4];
	bool bDelete = false;

	if (m_fade != FADE_NONE)
	{
		if (m_fade == FADE_IN)				//フェードイン
		{
			m_colorFade.a -= FADE_TIME;		//画面を透明にしていく
			if (m_colorFade.a <= 0.0f)
			{
				m_colorFade.a = 0.0f;

				m_fade = FADE_NONE;			//何もしていない
				bDelete = true;
			}
		}
		else if (m_fade == FADE_OUT)
		{
			m_colorFade.a += FADE_TIME;		//画面を不透明にしていく
			if (m_colorFade.a >= 1.0f)
			{
				m_colorFade.a = 1.0f;

				m_fade = FADE_IN;			//フェードイン

				if (m_modegame == MODEFADE_MODE)
				{
					//モード設定
					CManager::SetMode((CManager::MODE)m_nModeNext);
				}
				else if (m_modegame == MODEFADE_GAME)
				{
					CGame::SetGameModeNext((CGame::GAMEMODE)m_nModeNext);
				}

				//UpdateAllを戻す！
				CScene::SetUpdatePri(0);
			}
		}

		//頂点カラーの設定
		Color[0] = m_colorFade;
		Color[1] = m_colorFade;
		Color[2] = m_colorFade;
		Color[3] = m_colorFade;

		//カラーの割り当て
		CScene2D::SetColor(&Color[0]);
	}

	//ロード画像の色を割当て
	if (m_pNowLoading != NULL) { m_pNowLoading->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, m_colorFade.a)); }

	//アニメーション画像の更新
	if (m_pAnim != NULL)
	{
		//色の設定
		m_pAnim->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, m_colorFade.a));

		//時間の加算
		m_nCountAnim++;

		if ((m_nCountAnim % ANIM_COUNT) == 0)
		{
			//アニメーション数の可算
			m_nAnimNum++;

			//アニメーション数を初期化
			if ((m_nAnimNum % MAX_PATTERN) == 0) { m_nAnimNum = 0; }

			//アニメーション設定
			m_pAnim->SetUVWidth(ANIM_UV, m_nAnimNum);
		}
	}

	if (bDelete == true)
	{//フェードの終了処理
		Uninit();
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void CFade::Draw(void)
{
	CScene2D::Draw();
}
//===============================================================================
//　クリエイト
//===============================================================================
CFade * CFade::Create(CManager::MODE modeNext)
{
	CFade *pFade = NULL;

	if (m_fade == FADE_NONE)
	{
		//NULLチェック
		if (pFade == NULL)
		{//メモリの動的確保

			pFade = new CFade;

			if (pFade != NULL)
			{
				//オブジェクトクラスの生成
				pFade->Init();

				m_nModeNext = modeNext;

				//フェードのモード
				m_modegame = MODEFADE_MODE;

				m_fade = FADE_OUT;
			}
		}
	}

	return pFade;
}
CFade * CFade::Create(CGame::GAMEMODE modeNext)
{
	CFade *pFade = NULL;

	if (m_fade == FADE_NONE)
	{
		//NULLチェック
		if (pFade == NULL)
		{//メモリの動的確保

			pFade = new CFade;

			if (pFade != NULL)
			{
				//オブジェクトクラスの生成
				pFade->Init();

				m_nModeNext = modeNext;

				//フェードのモード
				m_modegame = MODEFADE_GAME;

				m_fade = FADE_OUT;
			}
		}
	}

	return pFade;
}

//===============================================================================
// テクスチャの読み込み
//===============================================================================
HRESULT CFade::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_1,
		&m_pTexture[0]
	);

	return S_OK;
}
//===============================================================================
// テクスチャの破棄
//===============================================================================
void CFade::UnLoad(void)
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