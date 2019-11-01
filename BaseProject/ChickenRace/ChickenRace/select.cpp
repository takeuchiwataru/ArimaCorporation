//=============================================================================
//
// セレクトの処理 [select.cpp]
// Author : shun yokomichi
//
//=============================================================================
#include "select.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SELECT_LOGO_POS			(D3DXVECTOR3(SCREEN_WIDTH / 2, 70.0f, 0.0f))					// 位置(タイトルロゴ000)
#define SELECT_LOGO_SIZE		(D3DXVECTOR2(200.0f, 60.0f))									// サイズ(タイトルロゴ000)
#define GAME_POS				(D3DXVECTOR3(SCREEN_WIDTH / 2, 310.0f, 0.0f))					// ゲーム画面(TEXTURE)の位置
#define GAME_SIZE				(D3DXVECTOR2(500.0f, 250.0f))									// ゲーム画面(TEXTURE)のサイズ
#define QUIT_POS				(D3DXVECTOR3(GAME_POS.x, GAME_POS.y + 330.0f, 0.0f))			// Quitの位置
#define QUIT_SIZE				(D3DXVECTOR2(530.0f, 70.0f))									// Quitのサイズ
#define TOTAL_POS				(D3DXVECTOR3(320.0f, GAME_POS.y + 250.0f, 0.0f))				// トータルの位置
#define TOTAL_SIZE				(D3DXVECTOR2(200.0f, 60.0f))									// トータルのサイズ
#define SATYIFACTION_POS		(D3DXVECTOR3(250.0f - 100.0f, TOTAL_POS.y + 60.0f, 0.0f))	// 満足度の位置
#define SATYIFACTION_SIZE		(D3DXVECTOR2(15.0f / 4, 25.0f))									// 満足度のサイズ
#define SCORE_POS				(D3DXVECTOR3(250.0f + 800.0f, TOTAL_POS.y + 60.0f, 0.0f))	// 現在の最高スコアの位置
#define MONEY_POS				(D3DXVECTOR3(250.0f + 530.0f, TOTAL_POS.y + 60.0f, 0.0f))	// \マークの位置
#define MONEY_SIZE				(D3DXVECTOR2(25.0f, 23.0f))										// \マークのサイズ
#define CIRCLE_POS				(D3DXVECTOR3(SCREEN_WIDTH / 2, 520.0f, 0.0f))					// サークルの位置
#define CIRCLE_SIZE				(D3DXVECTOR2(100.0f, 100.0f))									// サークルのサイズ
#define FREAM_POS				(D3DXVECTOR3(QUIT_POS.x, QUIT_POS.y, 0.0f))						// フレームの位置
#define FREAM_SIZE000			(D3DXVECTOR2(GAME_SIZE.x + 25.0f , GAME_SIZE.y + 20.0f))		// フレームのサイズ(ゲーム画面)
#define FREAM_SIZE001			(D3DXVECTOR2(QUIT_SIZE.x + 20.0f , QUIT_SIZE.y + 18.0f))		// フレームのサイズ(Quit)

//-------------------------------------------
// ファイル名
//-------------------------------------------
#define TOTALSCORE_TXT_NAME		("data/TEXT/Ranking/ranking.bin")
#define TOTALSCORE_TXT_NAME2	("data/TEXT/Ranking/ranking001.bin")
#define SATISFACTION_TXT_NAME	("data/TEXT/Ranking/SatisfactionLevel.bin")
#define SATISFACTION_TXT_NAME2	("data/TEXT/Ranking/SatisfactionLevel001.bin")

//-------------------------------------------
// その他
//-------------------------------------------
#define ROT_MOVE				(0.5f)				// 回転速度
#define ANIM_COUNT				(120)				// アニメーションカウンター
#define PATTERN_NUM				(4)					// パターンの数
#define MENU_POS				(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100.0f + (nCntMenu * 300.0f), 0.0f))	// continu ～ Quitまで位置
#define DEFAULT_POS_LEFT		(D3DXVECTOR3(0.0f, MENU_POS.y, 0.0f))						// デフォルトの位置(左)の場合
#define DEFAULT_POS_RIGHT		(D3DXVECTOR3(SCREEN_WIDTH, MENU_POS.y, 0.0f))				// デフォルトの位置(右)の場合
#define END_POS_LEFT			(D3DXVECTOR3(-200.0f, MENU_POS.y, 0.0f))					// 選択後の位置(左)の場合
#define END_POS_RIGHT			(D3DXVECTOR3(SCREEN_WIDTH + 200.0f, MENU_POS.y, 0.0f))		// 選択後の位置(右)の場合
#define MENU_SIZE				(D3DXVECTOR2(550.0f, 150.0f))								// continu ～ Quitまでサイズ
#define MOVE					(27.0f)														// 左右の移動量

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
int CSelect::m_nStageNumber = 0;

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CSelect::CSelect()
{
	m_pFream = NULL;
	m_pBg = NULL;
	m_bPress = false;
	m_bSame = false;
	m_fAddRot = D3DX_PI;
	m_nCntAnim = 0;
	m_nCntPattern = 0;
	m_nStageNum = 1;
	m_nStageNumber = 0;
	m_bEnter = false;
	m_bSelectFlag = false;
	m_bLevelFlag = false;
	m_bMove = false;
}
//=============================================================================
// デストラクタ
//=============================================================================
CSelect::~CSelect()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSelect::Init()
{


	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CSelect::Uninit(void)
{

}
//=============================================================================
// 更新処理
//=============================================================================
void CSelect::Update(void)
{
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputXPad * pXpad = CManager::GetXInput();					//ジョイパットの取得
	CInputMouse *pCInputMouse = CManager::GetInputMouse();

	
}
//=============================================================================
// 描画処理
//=============================================================================
void CSelect::Draw(void)
{

}

//=============================================================================
// ステージ選択
//=============================================================================
void CSelect::SlectStage(void)
{
	//サウンドの情報
	CSound *pSound = CManager::GetSound();

	
}