//=============================================================================
//
// タイトルの処理 [title.cpp]
// Author : shun yokomichi
//
//=============================================================================
#include "title.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "ranking.h"
#include "ui.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define EFFECT_FRAME	(300)		// 季節のエフェクトフレーム
#define SEASON_MAX		(4)			// 季節のカウント
#define EFFECT_CREATE	(10)		// エフェクト生成時間
#define EFFECT_ROS		(D3DXVECTOR3(rand() % 1200 + 10.0f, -20.0f, 0.0f))	// 位置(ランダム)
#define EFFECT_SIZE		(rand() % RADIUS_CONDIUIONS + 10)					// サイズ(ランダム)
#define TITLE_POS		(D3DXVECTOR3(SCREEN_WIDTH /2, SCREEN_HEIGHT / 2 - 80.0f, 0.0f))	// 位置(タイトルロゴ000)
#define TITLE_SIZE		(D3DXVECTOR2(480.0f, 100.0f))										// サイズ(タイトルロゴ000)
#define LOGO_POS		(D3DXVECTOR3(TITLE_POS.x + 130.0f, TITLE_POS.y + 100.0f, 0.0f))		// 位置(タイトルロゴ001)
#define LOGO_SIZE		(D3DXVECTOR2(125.0f, 35.0f))										// サイズ(タイトルロゴ001)
#define PRESS_POS		(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT/ 2) + 200.0f, 0.0f))// 位置(Press Start Enter)
#define PRESS_SIZE		(D3DXVECTOR2(250.0f, 20.0f))										// サイズ(Press Start Enter)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CTitle::CTitle()
{
	m_nCntTex = 0;
	m_nFrame = 0;
	m_nLeafposX = 0;
	m_nLeafsize = 0;
	m_nCntCreate = 0;
	m_bOnOff = false;
	m_nTitleCounter = 0;
}
//=============================================================================
// デストラクタ
//=============================================================================
CTitle::~CTitle()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTitle::Init()
{
	//===================================
	//		 Loadの読み込み場所
	//===================================
	CFade::Load();		//フェードのテクスチャの読み込み
	CUi::Load();		//UIのテクスチャの読み込み

	//===================================
	//		    UI生成の場所
	//===================================
	CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 120.0f, 0.0f), D3DXVECTOR2(500.0f, 180.0f), 0);	//タイトルロゴ
	CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 270.0f, 0.0f), D3DXVECTOR2(250.0f, 50.0f), 1);	//エンター指示

	//	変数の初期化
	m_nTitleCounter = 0;

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CTitle::Uninit(void)
{
	//===================================
	//	　　UnLoadの破棄する場所
	//===================================
	CFade::UnLoad();	//フェードのテクスチャの破棄
	CUi::UnLoad();		//UIのテクスチャの破棄

	//フェード以外削除
	CScene::NotFadeReleseAll();
}
//=============================================================================
// 更新処理
//=============================================================================
void CTitle::Update(void)
{
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputXPad * pXpad = CManager::GetXInput();					//ジョイパットの取得
	CInputMouse *pCInputMouse = CManager::GetInputMouse();

	//サウンドの情報
	CSound *pSound = CManager::GetSound();

	//フェードのポイント
	CFade::FADE pFade = CFade::GetFade();

	//	タイトルのカウンター加算
	m_nTitleCounter++;

	if (pXpad->GetALL(1, 0) == true || pCInputKeyBoard->GetKeyboardAny(1) == true || pCInputMouse->GetMouseTrigger(0) == true)
	{//タイトルからゲームへ
	 //フェードが始まったら
		if (pFade == CFade::FADE_NONE)
		{
			//ポーズの選択の決定音
			pSound->PlaySound(CSound::SOUND_LABEL_SE_TITLE_ENTER);
			CFade::Create(CManager::MODE_GAME);
		}
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void CTitle::Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	CManager::GetRenderer()->GetDevice()->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
		D3DCOLOR_RGBA(157, 184, 224, 255),
		1.0f,
		0);

	//全ての描画
	CScene::DrawAll();
}