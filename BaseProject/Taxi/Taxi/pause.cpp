//=============================================================================
//
// ポーズの処理 [pause.cpp]
// Author : 有馬武志
//
//=============================================================================
#include "scene.h"
#include "pause.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "fade.h"
#include "texture.h"
#include "logo.h"
#include "background.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_PAUSE				(128)							//ポーズの最大数
#define PAUSE_POS_X				(0)								//ポーズの左上X座標
#define PAUSE_POS_Y				(0)								//ポーズの左上Y座標
#define PAUSE_WIDTH				(SCREEN_WIDTH)					//ポーズの幅
#define PAUSE_HEIGHT			(SCREEN_HEIGHT)					//ポーズの高さ
#define PAUSE_TEXTURE_UV_U		(1.0f)							//テクスチャアニメーションU範囲
#define PAUSE_TEXTURE_UV_V		(1.0f)							//テクスチャアニメーションV範囲
#define PAUSE_SPEED				(0.003f)						//ポーズスピード

//===============================================
// 位置とサイズ(初期位置)
//===============================================
#define LOGO_POS				(D3DXVECTOR3(PAUSE_WIDTH / 2, 100.0f, 0.0f))				// ロゴの位置
#define LOGO_SIZE				(D3DXVECTOR2(200.0f, 60.0f))								// ロゴのサイズ
#define LOGO_ADD_SIZE_X			(11.0f)														// サイズの加算(X)
#define LOGO_ADD_SIZE_Y			(3.0f)														// サイズの加算(Y)
#define OPERATION_POS			(D3DXVECTOR3(1020.0f, 450.0f, 0.0f))						// 操作方法の位置
#define OPERATION_SIZE			(D3DXVECTOR2(230.0f, 230.0f))								// 操作方法のサイズ
#define OPERATION_ADD_SIZE_X	(12.0f)														// サイズの加算(X)
#define OPERATION_ADD_SIZE_Y	(15.0f)														// サイズの加算(Y)
#define SIZE_SMALL				(D3DXVECTOR2(0.0f, 0.0f))									// 操作方法のサイズ(小さい)

//===============================================
// 選択時アイコン関係
//===============================================
#define ICON_POS				(D3DXVECTOR3(340.0f - 250.0f, 300.0f, 0.0f))				// 選択時アイコンの位置
#define ICON_SIZE				(D3DXVECTOR2(40.0f, 40.0f))									// 選択時アイコンのサイズ
#define ADD_MOVE_NOMAL			(0.5f)														// 選択時アイコンの移動量(通常時)
#define ADD_MOVE_SELECT			(6.0f)														// 選択時アイコンの移動量(選択時)
#define CURRENT_POS				(D3DXVECTOR3(SelectPos.x, m_apLogo[m_SelectNum]->GetPosition().y, SelectPos.z))	// 選択時アイコンの現在位置
//===============================================
// メニュー関係
//===============================================
#define MENU_POS				(D3DXVECTOR3(300.0f, 250.0f + (nCntMenu * 170.0f), 0.0f))	// continu ～ Quitまで位置
#define DEFAULT_POS_LEFT		(D3DXVECTOR3(0.0f, MENU_POS.y, 0.0f))						// デフォルトの位置(左)の場合
#define DEFAULT_POS_RIGHT		(D3DXVECTOR3(SCREEN_WIDTH / 2, MENU_POS.y, 0.0f))			// デフォルトの位置(右)の場合
#define END_POS_LEFT			(D3DXVECTOR3(-200.0f, MENU_POS.y, 0.0f))					// 選択後の位置(左)の場合
#define END_POS_RIGHT			(D3DXVECTOR3(SCREEN_WIDTH + 200.0f, MENU_POS.y, 0.0f))		// 選択後の位置(右)の場合
#define MENU_SIZE				(D3DXVECTOR2(200.0f, 60.0f))								// continu ～ Quitまでサイズ
#define MOVE					(27.0f)														// 左右の移動量
//===============================================
// ポーズを閉じる時のカウンタ
//===============================================
#define PAUSE_END_FRAME			(30)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
bool			   CPause::m_bPause = false;
bool			   CPause::m_bSerectEnd = false;

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CPause::CPause(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_SelectColor = 0.0f;
	m_nCntState = 0;
	m_SelectNum = 0;
	m_nSize = D3DXVECTOR2(0.0f, 0.0f);
	m_pBg = NULL;
	m_pSelectIcon = NULL;
	m_state = PAUSE_NORMAL;
	m_OperationNum = 0;
	m_nCntPauseEnd = 0;
	for (int nCntLogo = 0; nCntLogo < MAX_PAUSE_POLYGON; nCntLogo++)
	{
		m_apLogo[nCntLogo] = NULL;
	}
	m_nPauseCounter = 0;
}
//===============================================================================
//　デストラクタ
//===============================================================================
CPause::~CPause()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPause::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_pos = D3DXVECTOR3((PAUSE_POS_X + PAUSE_WIDTH) / 2, (PAUSE_POS_Y + PAUSE_HEIGHT) / 2, 0.0f);
	m_SelectNum = 0;
	m_SelectColor = 1.0f;

	if (m_pBg == NULL) { m_pBg = CBackGround::Create(CTexture::TYPE_PAUSE_BG); }

	// メニューの生成
	for (int nCntMenu = 0; nCntMenu < MAX_PAUSE_POLYGON - 2; nCntMenu++)
	{
		if (m_apLogo[nCntMenu] == NULL)
		{
			if ((nCntMenu % 2) == 0)
			{
				m_apLogo[nCntMenu] = CLogo::Create(DEFAULT_POS_RIGHT, MENU_SIZE, CTexture::TYPE_PAUSE_CONTINUE + nCntMenu, 0, CLogo::TYPE_LOGO);
			}
			else { m_apLogo[nCntMenu] = CLogo::Create(DEFAULT_POS_LEFT, MENU_SIZE, CTexture::TYPE_PAUSE_CONTINUE + nCntMenu, 0, CLogo::TYPE_LOGO); }

			if (nCntMenu == 0) { m_apLogo[nCntMenu]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)); }
			else { m_apLogo[nCntMenu]->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f)); }
		}
	}

	// ロゴの生成
	if (m_apLogo[3] == NULL)
	{
		m_apLogo[3] = CLogo::Create(LOGO_POS, SIZE_SMALL, CTexture::TYPE_PAUSE_LOGO, 0, CLogo::TYPE_LOGO);
	}

	// 操作方法の生成
	if (m_apLogo[4] == NULL)
	{
		m_apLogo[4] = CLogo::Create(OPERATION_POS, SIZE_SMALL, CTexture::TYPE_PAUSE_OPERATION, m_OperationNum, CLogo::TYPE_LOGO);
		m_apLogo[4]->SetTexture(m_OperationNum, 2, 1, 1);
	}

	// 選択時のアイコン生成
	if (m_pSelectIcon == NULL)
	{
		m_pSelectIcon = CLogo::Create(ICON_POS, ICON_SIZE, CTexture::TYPE_PAUSE_ICON, 0, CLogo::TYPE_LOGO);
	}

	m_bPause = false;
	m_nPauseCounter = 0;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPause::Uninit(void)
{
	//選んでる状態を戻す
	m_bSerectEnd = false;

	if (m_pBg != NULL)
	{	// 背景の破棄
		m_pBg->Uninit();
		m_pBg = NULL;
	}

	if (m_pSelectIcon != NULL)
	{	// 選択時アイコンの破棄
		m_pSelectIcon->Uninit();
		m_pSelectIcon = NULL;

	}
	for (int nCntLogo = 0; nCntLogo < MAX_PAUSE_POLYGON; nCntLogo++)
	{
		if (m_apLogo[nCntLogo] != NULL)
		{	// ロゴの破棄
			m_apLogo[nCntLogo]->Uninit();
			m_apLogo[nCntLogo] = NULL;
		}
	}

	//自身の削除
	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPause::Update(void)
{
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CXInput *pCInputJoypad = CManager::GetXInput();
	CInputMouse *pCInputMouse = CManager::GetInputMouse();

	//サウンドの情報
	CSound *pSound = CManager::GetSound();

	//フェードのポイント
	CFade::FADE pFade = CFade::GetFade();

	//	Pauseのカウンターの加算
	m_nPauseCounter++;

	// 選択時アイコンの位置取得
	D3DXVECTOR3 SelectPos = m_pSelectIcon->GetPosition();
	D3DXVECTOR3 MenuPos[MAX_PAUSE_POLYGON - 2] = {};
	int nCntNum = 0;														// 回数のカウント
	for (int nCntMenu = 0; nCntMenu < MAX_PAUSE_POLYGON - 2; nCntMenu++)
	{	// メニューの位置取得
		if (m_apLogo[nCntMenu] != NULL) { MenuPos[nCntMenu] = m_apLogo[nCntMenu]->GetPosition(); }
	}

	if (m_state != PAUSE_SELECT)
	{
		if (m_nPauseCounter >=30)
		{
			if ((pCInputKeyBoard->GetKeyboardTrigger(DIK_UP) == true || pCInputKeyBoard->GetKeyboardTrigger(DIK_W) == true || pCInputJoypad->GetRepeat(CXInput::XIJS_BUTTON_0) == true || pCInputJoypad->GetRepeat(CXInput::XIJS_BUTTON_16)) && m_bSerectEnd == false)
			{//矢印キーの上を押したとき
				if (m_apLogo[m_SelectNum] != NULL) { m_apLogo[m_SelectNum]->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f)); }
				//ポーズの選択肢移動音
				pSound->SetVolume(CSound::SOUND_LABEL_SE_PAUSE_MOVE, 0.8f);
				pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_MOVE);

				m_SelectNum = (m_SelectNum + 2) % 3;
				m_SelectColor = 1.0f;
				if (m_apLogo[m_SelectNum] != NULL) { m_apLogo[m_SelectNum]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, m_SelectColor)); }
			}
			if ((pCInputKeyBoard->GetKeyboardTrigger(DIK_DOWN) == true || pCInputKeyBoard->GetKeyboardTrigger(DIK_S) == true || pCInputJoypad->GetRepeat(CXInput::XIJS_BUTTON_1) == true || pCInputJoypad->GetRepeat(CXInput::XIJS_BUTTON_17)) && m_bSerectEnd == false)
			{//矢印キーの下を押したとき
				if (m_apLogo[m_SelectNum] != NULL) { m_apLogo[m_SelectNum]->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f)); }
				//ポーズの選択し移動音
				pSound->SetVolume(CSound::SOUND_LABEL_SE_PAUSE_MOVE, 0.8f);
				pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_MOVE);

				m_SelectNum = (m_SelectNum + 1) % 3;
				m_SelectColor = 1.0f;
				if (m_apLogo[m_SelectNum] != NULL) { m_apLogo[m_SelectNum]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, m_SelectColor)); }
			}

			if (pCInputKeyBoard->GetKeyboardTrigger(DIK_RIGHT) == true && m_bSerectEnd == false || pCInputKeyBoard->GetKeyboardTrigger(DIK_D) == true && m_bSerectEnd == false
				|| pCInputKeyBoard->GetKeyboardTrigger(DIK_LEFT) == true && m_bSerectEnd == false || pCInputKeyBoard->GetKeyboardTrigger(DIK_A) == true)
			{//矢印キーの右を押したとき
				//ポーズの選択肢移動音
				pSound->SetVolume(CSound::SOUND_LABEL_SE_PAUSE_MOVE, 0.8f);
				pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_MOVE);
				m_OperationNum = (m_OperationNum + 1) % 2;
				if (m_apLogo[4] != NULL) { m_apLogo[4]->SetTexture(m_OperationNum, 2, 1, 1); }
			}
		}
	}
	// サイズの変更
	if (m_apLogo[3] != NULL && m_apLogo[4] != NULL)
	{

		if (m_state != PAUSE_SELECT)
		{
			m_apLogo[3]->Zoom(m_apLogo[3]->GetPosition(), LOGO_SIZE, D3DXVECTOR2(LOGO_ADD_SIZE_X, LOGO_ADD_SIZE_Y), true);
			m_apLogo[4]->Zoom(m_apLogo[4]->GetPosition(), OPERATION_SIZE, D3DXVECTOR2(OPERATION_ADD_SIZE_X, OPERATION_ADD_SIZE_Y), true);
		}
		else
		{
			m_apLogo[3]->Zoom(m_apLogo[3]->GetPosition(), SIZE_SMALL, D3DXVECTOR2(LOGO_ADD_SIZE_X, LOGO_ADD_SIZE_Y), false);
			m_apLogo[4]->Zoom(m_apLogo[4]->GetPosition(), SIZE_SMALL, D3DXVECTOR2(OPERATION_ADD_SIZE_X, OPERATION_ADD_SIZE_Y), false);

		}
	}

	// 色の切替
	ColChange();
	if (m_nPauseCounter >= 30)
	{
		if ((pCInputKeyBoard->GetKeyboardTrigger(DIK_RETURN) == true || pCInputMouse->GetMouseTrigger(0) == true || pCInputJoypad->GetTrigger(CXInput::XIJS_BUTTON_11) == true || pCInputJoypad->GetTrigger(CXInput::XIJS_BUTTON_10) == true) && m_bSerectEnd == false)
		{
			if (pFade == CFade::FADE_NONE &&m_state != PAUSE_SELECT)
			{
				SelectNumOperation();		// 選択後の処理
				//ポーズの選択の決定音
				pSound->SetVolume(CSound::SOUND_LABEL_SE_PAUSE_SELECT, 1.1f);
				pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_SELECT);
			}
			m_state = PAUSE_SELECT;			// 選択状態にする
		}
	}
	PauseEnd();		// コンティニュー選択時の処理

	// 選択時アイコンの移動
	SelectPos = SelectMove(SelectPos);

	// メニュの移動
	for (int nCntMenu = 0; nCntMenu < MAX_PAUSE_POLYGON - 2; nCntMenu++)
	{
		if (m_apLogo[nCntMenu] != NULL)
		{
			if (m_state != PAUSE_SELECT)
			{	// 画面表示時の移動
				if ((nCntMenu % 2) == 0)
				{
					MenuPos[nCntMenu] = m_apLogo[nCntMenu]->MoveLimit(MenuPos[nCntMenu], MENU_POS, 1, -MOVE);
				}
				else { MenuPos[nCntMenu] = m_apLogo[nCntMenu]->MoveLimit(MenuPos[nCntMenu], MENU_POS, 0, MOVE); }
			}
			else
			{	// 回数のカウント
				if ((nCntMenu % 2) == 0 && m_SelectNum != nCntMenu) { nCntNum++;}
				else  if (m_SelectNum != nCntMenu) { nCntNum++; }

				if (m_SelectNum != nCntMenu)
				{	// 選択番号がカウントと違う場合 左右のどちらかに動かす
					if ((nCntNum % 2) == 0){ MenuPos[nCntMenu] = m_apLogo[nCntMenu]->MoveLimit(MenuPos[nCntMenu], END_POS_LEFT, 1, -MOVE); }
					else { MenuPos[nCntMenu] = m_apLogo[nCntMenu]->MoveLimit(MenuPos[nCntMenu], END_POS_RIGHT, 0, MOVE); }
				}
			}
		}
	}

	// 選択時アイコンの更新
	if (m_pSelectIcon != NULL) { m_pSelectIcon->SetPosition(CURRENT_POS); m_pSelectIcon->Update(); }
	for (int nCntLogo = 0; nCntLogo < MAX_PAUSE_POLYGON; nCntLogo++)
	{	// ロゴの更新
		if (m_apLogo[nCntLogo] != NULL)
		{
			if (nCntLogo < 3) { m_apLogo[nCntLogo]->SetPosition(MenuPos[nCntLogo]); }
			else if (nCntLogo = 4) { m_apLogo[nCntLogo]->SetTexture(m_OperationNum, 2, 1, 1);}
			m_apLogo[nCntLogo]->Update();
		}
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void CPause::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (m_pBg != NULL) { m_pBg->Draw(); }	// 背景の描画

	if (m_pSelectIcon != NULL) { m_pSelectIcon->Draw(); }	// 選択時アイコンの更新

	for (int nCntLogo = 0; nCntLogo < MAX_PAUSE_POLYGON; nCntLogo++)
	{	// ロゴの描画
		if (m_apLogo[nCntLogo] != NULL) { m_apLogo[nCntLogo]->Draw(); }
	}
}
//===============================================================================
//　クリエイト
//===============================================================================
CPause * CPause::Create()
{
	CPause *pPause = NULL;

	//NULLチェック
	if (pPause == NULL)
	{//メモリの動的確保

		pPause = new CPause;

		if (pPause != NULL)
		{
			//オブジェクトクラスの生成
			pPause->Init();
		}
	}

	return pPause;
}

//===============================================================================
//　選択時アイコンの移動
//===============================================================================
D3DXVECTOR3 CPause::SelectMove(D3DXVECTOR3 SelectPos)
{
	// ローカル変数
	bool bMoveFlag = false;
	if (m_pSelectIcon != NULL) { bMoveFlag = m_pSelectIcon->GetMoveFlag(); }
	float fAddMove = 0.0f;

	if (m_state == PAUSE_NORMAL) { fAddMove = ADD_MOVE_NOMAL; }
	else if (m_state == PAUSE_SELECT) { fAddMove = ADD_MOVE_SELECT; }

	if (m_pSelectIcon != NULL && bMoveFlag == false)
	{	// 右に移動
		SelectPos = m_pSelectIcon->MoveLimit(CURRENT_POS, D3DXVECTOR3(ICON_POS.x + 20.0f, CURRENT_POS.y, CURRENT_POS.z), 0, fAddMove);
	}

	else if (m_pSelectIcon != NULL && bMoveFlag == true)
	{	// 左に移動
		SelectPos = m_pSelectIcon->MoveLimit(CURRENT_POS, D3DXVECTOR3(ICON_POS.x - 20.0f, CURRENT_POS.y, CURRENT_POS.z), 1, -fAddMove);
	}
	return SelectPos;
}
//===============================================================================
//　色の変化
//===============================================================================
void CPause::ColChange(void)
{
	//点滅処理
	if (m_apLogo[m_SelectNum] != NULL) { m_apLogo[m_SelectNum]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, m_SelectColor)); }
	//選択しているカラーの値を減らしていく
	m_SelectColor -= 0.009f;

	if (m_SelectColor < 0.5f)
	{//信託しているカラーが0.5を切ったらカラーの値が増えてく
		m_SelectColor = 1.0f;
	}
}

//===============================================================================
//　コンティニュー選択時　ポーズのカウンタを回す
//===============================================================================
void CPause::PauseEnd(void)
{
	if (m_state == PAUSE_SELECT && m_SelectNum == 0)
	{	// コンティニューを選択した場合 カウントを回してポーズを終了
		m_nCntPauseEnd++;
		m_bPause = true;
		if ((m_nCntPauseEnd % PAUSE_END_FRAME) == 0)
		{	//ポーズを閉じる
			CGame::SetPause(false);
			m_bPause = false;
		}
	}
}

//===============================================================================
//　ポーズの選択後の処理を管理
//===============================================================================
void CPause::SelectNumOperation(void)
{
	if (m_SelectNum == 1)
	{
		//ゲームの最初に戻る
		CFade::Create(CManager::MODE_GAME);

		//ポーズで選んだ状態
		m_bSerectEnd = true;
		m_bPause = true;
	}
	else if (m_SelectNum == 2)
	{
		//ゲームのタイトルに戻る
		CFade::Create(CManager::MODE_TITLE);

		//ポーズで選んだ状態
		m_bSerectEnd = true;
		m_bPause = true;
	}
}