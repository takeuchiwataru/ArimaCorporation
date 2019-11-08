//=============================================================================
//
// タイトル（メニュー）の処理 [titlemenu.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS

#include "titlemenu.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "input.h"
#include "fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	CTitleMenu::m_pTexture[CTitleMenu::TEXTURE_MAX] = { NULL };

//=============================================================================
// コンストラクタ
//=============================================================================
CTitleMenu::CTitleMenu()
{
	m_pLogo = NULL;			// ロゴ
	m_pChicken = NULL;		// チキン

	for (int nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
		m_pFlag[nCntFlag] = NULL;			// 旗

	m_nCntLogo = 0;			// ロゴカウント

	m_pPress = NULL;		// プレス
	m_nCntPress = 0;		// プレスカウント

	m_pMenuBG = NULL;		// メニューBG

	for (int nCntMainMenu = 0; nCntMainMenu < MAX_MAINMENU; nCntMainMenu++)
	{// カウント
		m_pMainMenu[nCntMainMenu] = NULL;		// メインメニュー
		m_pNameMenu[nCntMainMenu] = NULL;		// メニュー名
	}
	m_nCntMainMenu = 0;							// メインメニューカウント

	m_nSelectNum = 0;		// 選択番号

	m_bSubMenu = false;		// サブメニュー

	m_pEntry = NULL;		// 受付中
	m_pHint = NULL;			// 説明

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
		m_pMember[nCntPlayer] = NULL;			// 人数

	m_pEnter = NULL;		// 決定
	m_pSearch = NULL;		// サーチ
	m_pCancel = NULL;		// キャンセル
	m_bEntry = false;		// 受付中
	m_nCntEntry = 0;		// 受付中カウント
	m_bSearch = false;		// サーチ
}
//=============================================================================
// デストラクタ
//=============================================================================
CTitleMenu::~CTitleMenu() {}

//=============================================================================
// ロード
//=============================================================================
HRESULT CTitleMenu::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{// テクスチャカウント
		char cName[128] = {};

		switch (nCntTex)
		{
		case TEXTURE_LOGO:
			strcpy(cName, "data\\TEXTURE\\Title\\UI\\title.png");
			break;
		case TEXTURE_FLAG:
			strcpy(cName, "data\\TEXTURE\\Title\\UI\\flag.png");
			break; 
		case TEXTURE_PRESS:
			strcpy(cName, "data\\TEXTURE\\Title\\UI\\PressButton.png");
			break;
		case TEXTURE_MENU:
			strcpy(cName, "data\\TEXTURE\\Title\\UI\\modeselect_00.png");
			break;
		case TEXTURE_NAME_M:
			strcpy(cName, "data\\TEXTURE\\Title\\UI\\modeselect.png");
			break;
		case TEXTURE_NAME_S:
			strcpy(cName, "data\\TEXTURE\\Title\\UI\\modeselect_13.png");
			break;
		case TEXTURE_INPUT:
			strcpy(cName, "data\\TEXTURE\\Title\\UI\\modeselect_15.png");
			break;
		case TEXTURE_ENTRY:
			strcpy(cName, "data\\TEXTURE\\Title\\UI\\modeselect_16.png");
			break;
		case TEXTURE_HINT:
			strcpy(cName, "data\\TEXTURE\\Title\\UI\\modeselect_11.png");
			break;
		case TEXTURE_NUM:
			strcpy(cName, "data\\TEXTURE\\Title\\UI\\chiken_icon.png");
			break;
		case TEXTURE_SEARCH:
			strcpy(cName, "data\\TEXTURE\\Title\\UI\\modeselect_10.png");
			break;
		}

		// テクスチャの生成
		if (m_pTexture[nCntTex] == NULL)
			D3DXCreateTextureFromFile(pDevice, cName, &m_pTexture[nCntTex]);
	}

	return S_OK;
}

//=============================================================================
// アンロード
//=============================================================================
void CTitleMenu::Unload(void)
{
	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{// テクスチャカウント
		//テクスチャーの破棄
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// クリエイト
//=============================================================================
CTitleMenu *CTitleMenu::Create(void)
{
	CTitleMenu *pTitleMenu = NULL;

	// メモリ確保
	if (pTitleMenu == NULL)
		pTitleMenu = new CTitleMenu;

	if (pTitleMenu != NULL)
	{// NULL以外
		pTitleMenu->Init();	// 初期化処理
	}

	return pTitleMenu;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTitleMenu::Init()
{
	// ロゴ
	if (m_pLogo == NULL)
	{// NULL
		m_pLogo = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
		m_pLogo->Init();
		m_pLogo->SetPosSize(
			D3DXVECTOR3
			(
				SCREEN_WIDTH * 0.5f,
				SCREEN_HEIGHT * 0.34f,
				0.0f
			),
			D3DXVECTOR2(SCREEN_WIDTH * 0.4f, SCREEN_HEIGHT * 0.15f));
		m_pLogo->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pLogo->BindTexture(m_pTexture[TEXTURE_LOGO]);
	}

	// チキン
	if (m_pChicken == NULL)
	{// NULL
		m_pChicken = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
		m_pChicken->Init();
		m_pChicken->SetPosSize(
			D3DXVECTOR3
			(
				SCREEN_WIDTH * 0.355f,
				-SCREEN_WIDTH * 0.05f,
				0.0f
			),
			D3DXVECTOR2(SCREEN_WIDTH * 0.05f, SCREEN_WIDTH * 0.05f));
		m_pChicken->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pChicken->BindTexture(m_pTexture[TEXTURE_NUM]);
		m_pChicken->SetTexture(0, 2, 1, 1);
	}

	// 旗
	for (int nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
	{// 旗カウント
		m_pFlag[nCntFlag] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
		m_pFlag[nCntFlag]->Init();
		m_pFlag[nCntFlag]->SetPosSize(
			D3DXVECTOR3
			(
				(nCntFlag == 0 ? SCREEN_WIDTH * 0.328f : SCREEN_WIDTH * 0.387f),
				(nCntFlag == 0 ? SCREEN_HEIGHT * 0.25f : SCREEN_HEIGHT * 0.255f),
				0.0f
			),
			D3DXVECTOR2(SCREEN_WIDTH * 0.03f, SCREEN_WIDTH * 0.05f));
		m_pFlag[nCntFlag]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		m_pFlag[nCntFlag]->BindTexture(m_pTexture[TEXTURE_FLAG]);

		if (nCntFlag == 0)
		{
			m_pFlag[nCntFlag]->SetRot(3.14f + 0.3f, D3DXVECTOR2(SCREEN_WIDTH * 0.03f, SCREEN_WIDTH * 0.05f));
		}
		else
		{
			m_pFlag[nCntFlag]->SetRot(3.14f - 0.3f, D3DXVECTOR2(SCREEN_WIDTH * 0.03f, SCREEN_WIDTH * 0.05f));
			m_pFlag[nCntFlag]->SetUV_X_ROLL();
		}
	}

	// プレス
	if (m_pPress == NULL)
	{// NULL
		m_pPress = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
		m_pPress->Init();
		m_pPress->SetPosSize(
			D3DXVECTOR3
			(
				SCREEN_WIDTH * 0.5f,
				SCREEN_HEIGHT * 0.8f,
				0.0f
			),
			D3DXVECTOR2(SCREEN_WIDTH * 0.13f, SCREEN_HEIGHT * 0.07f));
		m_pPress->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pPress->BindTexture(m_pTexture[TEXTURE_PRESS]);
	}

	// メニューBG
	if (m_pMenuBG == NULL)
	{// NULL
		m_pMenuBG = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
		m_pMenuBG->Init();
		m_pMenuBG->SetPosSize(
			D3DXVECTOR3
			(
				SCREEN_WIDTH * 0.5f,
				SCREEN_HEIGHT * (1.0f - 0.25f),
				0.0f
			),
			D3DXVECTOR2(SCREEN_WIDTH * 0.38f, SCREEN_HEIGHT * 0.25f));
		m_pMenuBG->SetColor(&D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	}

	for (int nCntMainMenu = 0; nCntMainMenu < MAX_MAINMENU; nCntMainMenu++)
	{// メインメニューカウント
		// メインメニュー
		if (m_pMainMenu[nCntMainMenu] == NULL)
		{// NULL
			m_pMainMenu[nCntMainMenu] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
			m_pMainMenu[nCntMainMenu]->Init();
			m_pMainMenu[nCntMainMenu]->SetPosSize(
				D3DXVECTOR3
				(
					SCREEN_WIDTH * 0.5f - (SCREEN_WIDTH * 0.08f * 2.5f) + ((SCREEN_WIDTH * 0.08f * 2.5f) * nCntMainMenu),
					SCREEN_HEIGHT * 0.7f,
					0.0f
				),
				D3DXVECTOR2(SCREEN_WIDTH * 0.08f, SCREEN_WIDTH * 0.08f));
			m_pMainMenu[nCntMainMenu]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_pMainMenu[nCntMainMenu]->BindTexture(m_pTexture[TEXTURE_MENU]);
			m_pMainMenu[nCntMainMenu]->SetTexture(nCntMainMenu, 3, 2, 1);
		}
		// メニュー名
		if (m_pNameMenu[nCntMainMenu] == NULL)
		{// NULL
			m_pNameMenu[nCntMainMenu] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
			m_pNameMenu[nCntMainMenu]->Init();
			m_pNameMenu[nCntMainMenu]->SetPosSize(
				D3DXVECTOR3
				(
					SCREEN_WIDTH * 0.5f - (SCREEN_WIDTH * 0.08f * 2.5f) + ((SCREEN_WIDTH * 0.08f * 2.5f) * nCntMainMenu),
					SCREEN_HEIGHT * 0.9f,
					0.0f
				),
				D3DXVECTOR2(SCREEN_WIDTH * 0.088f, SCREEN_WIDTH * 0.025f));
			m_pNameMenu[nCntMainMenu]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			if (nCntMainMenu != MAX_MAINMENU - 1)
			{// 戻る以外
				m_pNameMenu[nCntMainMenu]->BindTexture(m_pTexture[TEXTURE_NAME_M]);
				m_pNameMenu[nCntMainMenu]->SetTexture(nCntMainMenu, 1, 2, 1);
			}
			else
			{// 戻る
				m_pNameMenu[nCntMainMenu]->SetPosSize(
					D3DXVECTOR3
					(
						SCREEN_WIDTH * 0.5f - (SCREEN_WIDTH * 0.08f * 2.5f) + ((SCREEN_WIDTH * 0.08f * 2.5f) * nCntMainMenu),
						SCREEN_HEIGHT * 0.9f,
						0.0f
					),
					D3DXVECTOR2(SCREEN_WIDTH * 0.13f, SCREEN_WIDTH * 0.025f));
				m_pNameMenu[nCntMainMenu]->BindTexture(m_pTexture[TEXTURE_INPUT]);
				m_pNameMenu[nCntMainMenu]->SetTexture(0, 1, 2, 1);
			}
		}
	}

	// エントリー
	if (m_pEntry == NULL)
	{// NULL
		m_pEntry = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
		m_pEntry->Init();
		m_pEntry->SetPosSize(
			D3DXVECTOR3
			(
				SCREEN_WIDTH * 0.25f,
				SCREEN_HEIGHT * 0.58f,
				0.0f
			),
			D3DXVECTOR2(SCREEN_WIDTH * 0.1f, SCREEN_WIDTH * 0.047f));
		m_pEntry->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		m_pEntry->BindTexture(m_pTexture[TEXTURE_ENTRY]);
	}

	// 説明
	if (m_pHint == NULL)
	{// NULL
		m_pHint = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
		m_pHint->Init();
		m_pHint->SetPosSize(
			D3DXVECTOR3
			(
				SCREEN_WIDTH * 0.58f,
				SCREEN_HEIGHT * 0.58f,
				0.0f
			),
			D3DXVECTOR2(SCREEN_WIDTH * 0.15f, SCREEN_WIDTH * 0.02f));
		m_pHint->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		m_pHint->BindTexture(m_pTexture[TEXTURE_HINT]);
	}

	// 人数
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// プレイヤーカウント
		if (m_pMember[nCntPlayer] == NULL)
		{// NULL
			m_pMember[nCntPlayer] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
			m_pMember[nCntPlayer]->Init();
			m_pMember[nCntPlayer]->SetPosSize(
				D3DXVECTOR3
				(
					SCREEN_WIDTH * 0.5f - ((SCREEN_WIDTH * 0.06f) + ((SCREEN_WIDTH * 0.06f) * 2.2f)) + (((SCREEN_WIDTH * 0.06f) * 2.2f) * nCntPlayer),
					SCREEN_HEIGHT * 0.75f,
					0.0f
				),
				D3DXVECTOR2(SCREEN_WIDTH * 0.06f, SCREEN_WIDTH * 0.06f));
			m_pMember[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_pMember[nCntPlayer]->BindTexture(m_pTexture[TEXTURE_NUM]);
			m_pMember[nCntPlayer]->SetTexture((nCntPlayer == 0 ? 1 : 0), 2, 1, 1);
		}
	}

	// 決定
	if (m_pEnter == NULL)
	{// NULL
		m_pEnter = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
		m_pEnter->Init();
		m_pEnter->SetPosSize(
			D3DXVECTOR3
			(
				SCREEN_WIDTH * 0.5f,
				SCREEN_HEIGHT * 0.9f,
				0.0f
			),
			D3DXVECTOR2(SCREEN_WIDTH * 0.088f, SCREEN_WIDTH * 0.025f));
		m_pEnter->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		m_pEnter->BindTexture(m_pTexture[TEXTURE_INPUT]);
		m_pEnter->SetTexture(1, 1, 2, 1);
	}

	// キャンセル
	if (m_pCancel == NULL)
	{// NULL
		m_pCancel = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
		m_pCancel->Init();
		m_pCancel->SetPosSize(
			D3DXVECTOR3
			(
				SCREEN_WIDTH * 0.7f,
				SCREEN_HEIGHT * 0.9f,
				0.0f
			),
			D3DXVECTOR2(SCREEN_WIDTH * 0.13f, SCREEN_WIDTH * 0.025f));
		m_pCancel->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		m_pCancel->BindTexture(m_pTexture[TEXTURE_INPUT]);
		m_pCancel->SetTexture(0, 1, 2, 1);
	}

	// サーチ
	if (m_pSearch == NULL)
	{// NULL
		m_pSearch = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
		m_pSearch->Init();
		m_pSearch->SetPosSize(
			D3DXVECTOR3
			(
				SCREEN_WIDTH * 0.5f,
				SCREEN_HEIGHT * 0.7f,
				0.0f
			),
			D3DXVECTOR2(SCREEN_WIDTH * 0.2f, SCREEN_WIDTH * 0.025f));
		m_pSearch->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		m_pSearch->BindTexture(m_pTexture[TEXTURE_SEARCH]);
		m_pSearch->SetTexture(0, 1, 2, 1);
	}	

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CTitleMenu::Uninit(void)
{
	// ロゴ
	if (m_pLogo != NULL)
	{// NULL以外
		m_pLogo->Uninit();
		m_pLogo = NULL;
	}

	// チキン
	if (m_pChicken != NULL)
	{// NULL以外
		m_pChicken->Uninit();
		m_pChicken = NULL;
	}

	// 旗
	for (int nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
	{// 旗カウント
		if (m_pFlag[nCntFlag] != NULL)
		{// NULL以外
			m_pFlag[nCntFlag]->Uninit();
			m_pFlag[nCntFlag] = NULL;
		}
	}

	// プレス
	if (m_pPress != NULL)
	{// NULL以外
		m_pPress->Uninit();
		m_pPress = NULL;
	}

	//	メニューBG
	if (m_pMenuBG != NULL)
	{// NULL以外
		m_pMenuBG->Uninit();
		m_pMenuBG = NULL;
	}

	for (int nCntMainMenu = 0; nCntMainMenu < MAX_MAINMENU; nCntMainMenu++)
	{// メインメニュー
		if (m_pMainMenu[nCntMainMenu] != NULL)
		{// NULL以外
			m_pMainMenu[nCntMainMenu]->Uninit();
			m_pMainMenu[nCntMainMenu] = NULL;
		}
		if (m_pNameMenu[nCntMainMenu] != NULL)
		{// NULL以外
			m_pNameMenu[nCntMainMenu]->Uninit();
			m_pNameMenu[nCntMainMenu] = NULL;
		}
	}

	// エントリー
	if (m_pEntry != NULL)
	{// NULL以外
		m_pEntry->Uninit();
		m_pEntry = NULL;
	}
	// 説明
	if (m_pHint != NULL)
	{// NULL以外
		m_pHint->Uninit();
		m_pHint = NULL;
	}
	// 人数
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// プレイヤーカウント
		if (m_pMember[nCntPlayer] != NULL)
		{// NULL以外
			m_pMember[nCntPlayer]->Uninit();
			m_pMember[nCntPlayer] = NULL;
		}
	}
	// 決定
	if (m_pEnter != NULL)
	{// NULL以外
		m_pEnter->Uninit();
		m_pEnter = NULL;
	}
	// キャンセル
	if (m_pCancel != NULL)
	{// NULL以外
		m_pCancel->Uninit();
		m_pCancel = NULL;
	}
	// サーチ
	if (m_pSearch != NULL)
	{// NULL以外
		m_pSearch->Uninit();
		m_pSearch = NULL;
	}

	//自身の削除
	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTitleMenu::Update(void)
{
	// 取得
	int nCounter = CTitle::GetCounter();

	// 編集
	EditLogo();
	EditChicken();
	EditFlag();

	if (60 < nCounter)
	{// タイトル表示後
		if (CFade::GetFade() == CFade::FADE_NONE)
		{// フェードしていない
			// コントロール
			if (m_bEntry == false)
			{// 受付していない
				ControllTitle();
				ControllMainMenu();
				ControllSubMenu();
			}
			else
			{// 受付中
				ControllEntry();
			}

			// 編集
			EditPress();
			EditMenuBG();
			EditMainMenu();
			EditSubMenu();
			EditEntry();
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTitleMenu::Draw(void)
{

}

//=============================================================================
// コントロールタイトル
//=============================================================================
void CTitleMenu::ControllTitle(void)
{
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputJoyPad_0 * pXpad = CManager::GetInputJoyPad0(0);		//ジョイパットの取得
	CSound *pSound = CManager::GetSound();						//サウンドの情報

	// 取得
	bool bMenu = CTitle::GetMenu();

	if (m_nCntMainMenu == 0)
	{// 表示中じゃない
		if (bMenu == false)
		{// メニュー非表示
			if (pCInputKeyBoard->GetKeyboardAny(1) == true || pXpad->GetAllTrigger() == true)
				bMenu = true;	// メニューを出す
		}
	}

	// 設定
	CTitle::SetMenu(bMenu);
}

//=============================================================================
// コントロールメインメニュー
//=============================================================================
void CTitleMenu::ControllMainMenu(void)
{
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputJoyPad_0 * pXpad = CManager::GetInputJoyPad0(0);		//ジョイパットの取得
	CSound *pSound = CManager::GetSound();						//サウンドの情報

	// 終わるか
	bool bEnd = false;

	// 取得
	bool bMenu = CTitle::GetMenu();
	bool bOnline = CTitle::GetOnline();

	if (m_bSubMenu == false)
	{// サブメニュー非表示
		if (m_nCntPress == 0 && m_nCntMainMenu == (60 - 1))
		{// 表示中じゃない
			if (bMenu == true)
			{// メニュー表示
				if (pCInputKeyBoard->GetKeyboardTrigger(DIK_A) == true ||
					pCInputKeyBoard->GetKeyboardTrigger(DIK_LEFT) == true ||
					pXpad->GetTrigger(INPUT_LEFT) == true ||
					pXpad->GetTrigger(INPUT_LS_L) == true)
				{// 左キー
					m_nSelectNum = (m_nSelectNum + (MAX_MAINMENU - 1)) % MAX_MAINMENU;
				}
				else if (pCInputKeyBoard->GetKeyboardTrigger(DIK_D) == true ||
					pCInputKeyBoard->GetKeyboardTrigger(DIK_RIGHT) == true ||
					pXpad->GetTrigger(INPUT_RIGHT) == true ||
					pXpad->GetTrigger(INPUT_LS_R) == true)
				{// 右キー
					m_nSelectNum = (m_nSelectNum + (1)) % MAX_MAINMENU;
				}

				if (pCInputKeyBoard->GetKeyboardTrigger(DIK_RETURN) == true ||
					pCInputKeyBoard->GetKeyboardTrigger(DIK_Z) == true ||
					pXpad->GetTrigger(INPUT_B) == true ||
					pXpad->GetTrigger(INPUT_START) == true)
				{// 決定
					switch (m_nSelectNum)
					{// 選択番号
					case 0:
						bOnline = false;	// ローカルで遊ぶ
						bEnd = true;		// 終わり
						break;
					case 1:
						bOnline = true;		// オンラインで遊ぶ
						break;
					case 2:
						bMenu = false;		// メニューを閉じる
						break;
					}
				}

				if (pCInputKeyBoard->GetKeyboardTrigger(DIK_X) == true ||
					pXpad->GetTrigger(INPUT_A) == true)
				{// キャンセル
					bMenu = false;			// メニューを閉じる
				}
			}

			// 設定
			CTitle::SetMenu(bMenu);
			CTitle::SetOnline(bOnline);

			if (bEnd == true)
			{// 終わり
				CFade::Create(CManager::MODE_GAME);
			}
		}
	}
}

//=============================================================================
// コントロールサブメニュー
//=============================================================================
void CTitleMenu::ControllSubMenu(void)
{
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputJoyPad_0 * pXpad = CManager::GetInputJoyPad0(0);		//ジョイパットの取得
	CSound *pSound = CManager::GetSound();						//サウンドの情報

	// 終わるか
	bool bEnd = false;

	// 取得
	bool bMenu = CTitle::GetMenu();
	bool bOnline = CTitle::GetOnline();
	bool bHost = CTitle::GetHost();

	if (m_bSubMenu == true)
	{// サブメニュー表示
		if (m_nCntPress == 0 && m_nCntMainMenu == (60 - 1) && m_nCntEntry == 0)
		{// 表示中じゃない
			if (bMenu == true && bOnline == true)
			{// メニュー表示
				if (pCInputKeyBoard->GetKeyboardTrigger(DIK_A) == true ||
					pCInputKeyBoard->GetKeyboardTrigger(DIK_LEFT) == true ||
					pXpad->GetTrigger(INPUT_LEFT) == true ||
					pXpad->GetTrigger(INPUT_LS_L) == true)
				{// 左キー
					m_nSelectNum = (m_nSelectNum + (MAX_MAINMENU - 1)) % MAX_MAINMENU;
				}
				else if (pCInputKeyBoard->GetKeyboardTrigger(DIK_D) == true ||
					pCInputKeyBoard->GetKeyboardTrigger(DIK_RIGHT) == true ||
					pXpad->GetTrigger(INPUT_RIGHT) == true ||
					pXpad->GetTrigger(INPUT_LS_R) == true)
				{// 右キー
					m_nSelectNum = (m_nSelectNum + (1)) % MAX_MAINMENU;
				}

				if (pCInputKeyBoard->GetKeyboardTrigger(DIK_RETURN) == true ||
					pCInputKeyBoard->GetKeyboardTrigger(DIK_Z) == true ||
					pXpad->GetTrigger(INPUT_B) == true ||
					pXpad->GetTrigger(INPUT_START) == true)
				{// 決定
					switch (m_nSelectNum)
					{// 選択番号
					case 0:
						bHost = true;		// ホストになる
						bEnd = true;		// 終わり
						break;
					case 1:
						bHost = false;		// メンバーになる
						bEnd = true;		// 終わり
						break;
					case 2:
						bOnline = false;	// オンラインをやめる
						break;
					}
				}

				if (pCInputKeyBoard->GetKeyboardTrigger(DIK_X) == true ||
					pXpad->GetTrigger(INPUT_A) == true)
				{// キャンセル
					bOnline = false;		// オンラインをやめる
				}
			}

			// 設定
			CTitle::SetMenu(bMenu);
			CTitle::SetOnline(bOnline);
			CTitle::SetHost(bHost);

			if (bEnd == true)
			{// 終わる
				// サーチ
				if (bHost == false)
				{// メンバー
					if (m_pSearch != NULL)
					{// NULL以外
						m_pSearch->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
						m_pSearch->SetTexture(0, 1, 2, 1);
					}
				}

				// 受付
				m_bEntry = true;
			}
		}
	}
}

//=============================================================================
// コントロールエントリー
//=============================================================================
void CTitleMenu::ControllEntry(void)
{
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputJoyPad_0 * pXpad = CManager::GetInputJoyPad0(0);		//ジョイパットの取得
	CSound *pSound = CManager::GetSound();						//サウンドの情報

	if (m_nCntMainMenu == 0 && m_nCntEntry == (60 - 1))
	{// 表示中じゃない
		if (1 < CServer::GetnMaxClient())
		{// 1人以上
			if (pCInputKeyBoard->GetKeyboardTrigger(DIK_RETURN) == true ||
				pCInputKeyBoard->GetKeyboardTrigger(DIK_Z) == true ||
				pXpad->GetTrigger(INPUT_START) == true)
			{// 決定
				CFade::Create(CManager::MODE_GAME);		// ゲームへ
				return;
			}
		}

		if (pCInputKeyBoard->GetKeyboardTrigger(DIK_X) == true ||
			pXpad->GetTrigger(INPUT_A) == true)
		{// キャンセル
			m_bSubMenu = true;				// サブメニュー表示				

			m_bEntry = false;				// 受付終了
			m_bSearch = false;				// サーチ
			CManager::OnlineSeting(false);	// オンライン設定
		}
	}
}

//=============================================================================
// ロゴ編集
//=============================================================================
void CTitleMenu::EditLogo(void)
{
	// 取得
	int nCounter = CTitle::GetCounter();
	int nNum = nCounter;

	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.34f, 0.0f);
	D3DXVECTOR2 size = D3DXVECTOR2((SCREEN_WIDTH * 0.4f), (SCREEN_HEIGHT * 0.15f));

	if (nCounter < 60)
	{// 60以下
		if (nCounter < 30)
		{// 30以下
			size.x = (SCREEN_WIDTH * 0.4f) * (float)((float)nCounter / (float)30);
			size.y = (SCREEN_HEIGHT * 0.15f) * (float)((float)nCounter / (float)30);
		}
	}
	else if (nCounter < 80)
	{// 80以下
		nNum = nCounter - 60;
		int nNum2 = nNum % 10;

		pos.y = (SCREEN_HEIGHT * (0.34f + ((nNum / 10 == 0 ? nNum2 : 10 - nNum2) * 0.002f)));
		size.y = (SCREEN_HEIGHT * (0.15f - ((nNum / 10 == 0 ? nNum2 : 10 - nNum2) * 0.002f)));
	}

	if (m_pLogo != NULL)
		m_pLogo->SetPosSize(pos, size);
}

//=============================================================================
// チキン編集
//=============================================================================
void CTitleMenu::EditChicken(void)
{
	// 取得
	int nCounter = CTitle::GetCounter();
	int nNum = nCounter;

	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.355f, -SCREEN_WIDTH * 0.05f, 0.0f);
	D3DXVECTOR2 size = D3DXVECTOR2(SCREEN_WIDTH * 0.05f, SCREEN_WIDTH * 0.05f);
	// 目標位置（x:(SCREEN_HEIGHT * 0.355f), y:(SCREEN_HEIGHT * 0.2f)）

	if (nCounter < 60)
	{// 60以下
		if (30 <= nCounter)
		{// 30以下
			nNum = nCounter - 30 + 1;

			pos.y = (-SCREEN_WIDTH * 0.05f) + (((SCREEN_HEIGHT * 0.2f) + (SCREEN_WIDTH * 0.05f)) * (float)((float)nNum / (float)30));
		}
	}
	else
	{// その他
		pos.y = (SCREEN_HEIGHT * 0.2f);

		if (nCounter < 80)
		{// 80以下
			nNum = nCounter - 60;
			int nNum2 = nNum % 10;

			pos.y = (SCREEN_HEIGHT * (0.2f + ((nNum / 10 == 0 ? nNum2 : 10 - nNum2) * 0.003f)));
		}
	}

	if (m_pChicken != NULL)
		m_pChicken->SetPosSize(pos, size);
}

//=============================================================================
// 旗編集
//=============================================================================
void CTitleMenu::EditFlag(void)
{
	// 取得
	int nCounter = CTitle::GetCounter();
	int nNum = nCounter;

	float fcol = 0.0f;
	float frot = 0.0f;

	if (80 <= nCounter)
	{// 80以上
		fcol = 1.0f;

		if (nCounter < 90)
		{

		}
		else
		{
			if ((nCounter / 10) % 2 == 0)
				frot = 0.3f;
			else
				frot = 0.5f;
		}
	}

	for (int nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
	{
		if (m_pFlag[nCntFlag] != NULL)
		{
			m_pFlag[nCntFlag]->SetRot(3.14f + (nCntFlag == 0 ? frot : -frot), D3DXVECTOR2(SCREEN_WIDTH * 0.03f, SCREEN_WIDTH * 0.05f));
			m_pFlag[nCntFlag]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, fcol));
		}
	}
}

//=============================================================================
// プレス編集
//=============================================================================
void CTitleMenu::EditPress(void)
{
	// 取得
	bool bMenu = CTitle::GetMenu();

	float fAlpha = 0.0f;		// 透明度

	if (bMenu == false)
	{// メニュー非表示
	 // 透明度計算
		fAlpha = 1.0f * (float)((float)(m_nCntPress % 60) / (float)(60));

		// 調整
		if (m_nCntPress / 60 == 1)
			fAlpha = 1.0f - fAlpha;

		// カウント
		m_nCntPress = (m_nCntPress + 1) % 120;
	}
	else
	{// メニュー表示
		if (m_nCntPress != 0 && m_nCntPress != 120)
		{// 透明じゃない
			// 透明度計算
			fAlpha = 1.0f * (float)((float)(m_nCntPress % 60) / (float)(60));

			// 調整
			if (m_nCntPress / 60 == 1)
				fAlpha = 1.0f - fAlpha;

			// カウント
			if (m_nCntPress < 60)
				m_nCntPress--;
			else
				m_nCntPress++;
		}
		else
		{// 透明
			// 値調整
			m_nCntPress = 0;
		}
	}

	// プレス
	if (m_pPress != NULL)
	{// NULL以外
		// 色設定
		m_pPress->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha));
	}
}

//=============================================================================
// メニューBG編集
//=============================================================================
void CTitleMenu::EditMenuBG(void)
{
	// 取得
	bool bMenu = CTitle::GetMenu();

	float fAlpha = 0.0f;		// 透明度

	if (bMenu == false)
	{// メニュー非表示
		if (m_nCntLogo != 0)
		{// ０以外
		 // 透明度計算
			fAlpha = 1.0f * (float)((float)(m_nCntLogo % 60) / (float)(60));

			// カウント
			m_nCntLogo--;
		}
		else
		{// 非表示
			// 値調整
			fAlpha = 0.0f;
		}
	}
	else
	{// メニュー表示
		if (m_nCntLogo != (60 - 1))
		{// ６０以外
		 // 透明度計算
			fAlpha = 1.0f * (float)((float)(m_nCntLogo % 60) / (float)(60));

			// カウント
			m_nCntLogo++;
		}
		else
		{// 表示
			fAlpha = 1.0f;
		}
	}

	// 色設定
	if (m_pMenuBG != NULL)
		m_pMenuBG->SetColor(&D3DXCOLOR(0.0f, 0.0f, 0.0f, (0.3f * fAlpha)));
}

//=============================================================================
// メインメニュー編集
//=============================================================================
void CTitleMenu::EditMainMenu(void)
{
	// 取得
	bool bMenu = CTitle::GetMenu();
	bool bOnline = CTitle::GetOnline();

	float fAlpha = 0.0f;		// 透明度

	if (m_bSubMenu == false)
	{// サブメニュー非表示
		if (bMenu == false || bOnline == true)
		{// メニュー非表示
			if (m_nCntMainMenu != 0)
			{// ０以外
				// 透明度計算
				fAlpha = 1.0f * (float)((float)(m_nCntMainMenu % 60) / (float)(60));

				// カウント
				m_nCntMainMenu--;
			}
			else
			{// 非表示
				// 値調整
				fAlpha = 0.0f;
				m_nSelectNum = 0;

				// オンラインならサブメニュー表示
				if (bOnline == true)
				{
					m_bSubMenu = true;

					for (int nCntMainMenu = 0; nCntMainMenu < MAX_MAINMENU; nCntMainMenu++)
					{
						m_pMainMenu[nCntMainMenu]->SetTexture(nCntMainMenu + 3, 3, 2, 1);
					
						if (nCntMainMenu != MAX_MAINMENU - 1)
						{// 戻る以外
							m_pNameMenu[nCntMainMenu]->BindTexture(m_pTexture[TEXTURE_NAME_S]);
							m_pNameMenu[nCntMainMenu]->SetTexture(nCntMainMenu, 1, 2, 1);
						}
						else
						{// 戻る
							m_pNameMenu[nCntMainMenu]->BindTexture(m_pTexture[TEXTURE_INPUT]);
							m_pNameMenu[nCntMainMenu]->SetTexture(0, 1, 2, 1);
						}
					}
				}
			}
		}
		else
		{// メニュー表示
			if (m_nCntMainMenu != (60 - 1))
			{// ６０以外
				// 透明度計算
				fAlpha = 1.0f * (float)((float)(m_nCntMainMenu % 60) / (float)(60));

				// カウント
				m_nCntMainMenu++;
			}
			else
			{// 表示
				fAlpha = 1.0f;

				if (m_pSearch != NULL)
					m_pSearch->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			}
		}

		// メインメニュー
		for (int nCntMainMenu = 0; nCntMainMenu < MAX_MAINMENU; nCntMainMenu++)
		{// メインメニューカウント
			float fcol = 0.0f;

			// 色調整
			if (m_nSelectNum == nCntMainMenu)	// 選択番号と一致
				fcol = 1.0f;
			else
				fcol = 0.3f;

			// 色設定
			if (m_pMainMenu[nCntMainMenu] != NULL)
				m_pMainMenu[nCntMainMenu]->SetColor(&D3DXCOLOR(fcol, fcol, fcol, fAlpha));
			if (m_pNameMenu[nCntMainMenu] != NULL)
				m_pNameMenu[nCntMainMenu]->SetColor(&D3DXCOLOR(fcol, fcol, fcol, fAlpha));
		}
	}
}

//=============================================================================
// サブメニュー編集
//=============================================================================
void CTitleMenu::EditSubMenu(void)
{
	// 取得
	bool bMenu = CTitle::GetMenu();
	bool bOnline = CTitle::GetOnline();
	bool bHost = CTitle::GetHost();


	if (m_bSubMenu == true)
	{// サブメニュー表示
		float fAlpha = 0.0f;		// 透明度

		if (bMenu == false || bOnline == false || m_bEntry == true)
		{// メニュー非表示
			if (m_nCntMainMenu != 0)
			{// ０以外
			 // 透明度計算
				fAlpha = 1.0f * (float)((float)(m_nCntMainMenu % 60) / (float)(60));

				// カウント
				m_nCntMainMenu--;
			}
			else
			{// 非表示
				// 値調整
				fAlpha = 0.0f;
				m_nSelectNum = 0;

				// オンラインじゃないならサブメニュー非表示
				if (bOnline == false)
				{
					m_bSubMenu = false;

					for (int nCntMainMenu = 0; nCntMainMenu < MAX_MAINMENU; nCntMainMenu++)
					{
						m_pMainMenu[nCntMainMenu]->SetTexture(nCntMainMenu, 3, 2, 1);

						if (nCntMainMenu != MAX_MAINMENU - 1)
						{// 戻る以外
							m_pNameMenu[nCntMainMenu]->BindTexture(m_pTexture[TEXTURE_NAME_M]);
							m_pNameMenu[nCntMainMenu]->SetTexture(nCntMainMenu, 1, 2, 1);
						}
						else
						{// 戻る
							m_pNameMenu[nCntMainMenu]->BindTexture(m_pTexture[TEXTURE_INPUT]);
							m_pNameMenu[nCntMainMenu]->SetTexture(0, 1, 2, 1);
						}
					}

					m_bSearch = false;				// サーチ
				}
				else if (m_bEntry == true)
				{
					// オンライン設定
					if (CManager::OnlineSeting(true, bHost) == false)
					{// 作れなかった
						bOnline = false;	// オンラインをやめる
						CTitle::SetOnline(bOnline);

						m_bEntry = false;			// エントリー
						m_bSearch = false;			// サーチ

						if (m_pSearch != NULL)
							m_pSearch->SetTexture(1, 1, 2, 1);
					}
					else
					{
						m_bSearch = true;			// サーチ

						if (m_pSearch != NULL)
							m_pSearch->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
					}					
				}
			}
		}
		else
		{// 透明度表示
			if (m_nCntMainMenu != (60 - 1))
			{// ６０以外
			 // 透明度計算
				fAlpha = 1.0f * (float)((float)(m_nCntMainMenu % 60) / (float)(60));

				// カウント
				m_nCntMainMenu++;
			}
			else
			{// 表示
				fAlpha = 1.0f;
			}
		}

		// メインメニュー
		for (int nCntMainMenu = 0; nCntMainMenu < MAX_MAINMENU; nCntMainMenu++)
		{// メインメニューカウント
			float fcol = 0.0f;

			// 色調整
			if (m_nSelectNum == nCntMainMenu)	// 選択番号と一致
				fcol = 1.0f;
			else
				fcol = 0.3f;

			// 色設定
			if (m_pMainMenu[nCntMainMenu] != NULL)
				m_pMainMenu[nCntMainMenu]->SetColor(&D3DXCOLOR(fcol, fcol, fcol, fAlpha));
			if (m_pNameMenu[nCntMainMenu] != NULL)
				m_pNameMenu[nCntMainMenu]->SetColor(&D3DXCOLOR(fcol, fcol, fcol, fAlpha));
		}
	}
}

//=============================================================================
// エントリー編集
//=============================================================================
void CTitleMenu::EditEntry(void)
{
	// 取得
	bool bMenu = CTitle::GetMenu();
	bool bOnline = CTitle::GetOnline();

	//if (m_bSearch == true)
	{
		float fAlpha = 0.0f;		// 透明度

		if (m_bSearch == false)
		{// メニュー非表示
			if (m_nCntEntry != 0)
			{// ０以外
				// 透明度計算
				fAlpha = 1.0f * (float)((float)(m_nCntEntry % 60) / (float)(60));

				// カウント
				m_nCntEntry--;
			}
			else
			{// 非表示
				// 値調整
				fAlpha = 0.0f;
			}
		}
		else
		{// 透明度表示
			if (m_nCntEntry != (60 - 1))
			{// ６０以外
			 // 透明度計算
				fAlpha = 1.0f * (float)((float)(m_nCntEntry % 60) / (float)(60));

				// カウント
				m_nCntEntry++;
			}
			else
			{// 表示
				fAlpha = 1.0f;
			}
		}

		// クライアント数
		int nClient = CServer::GetnMaxClient();

		// 色設定
		if (m_pEntry != NULL)
			m_pEntry->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha));
		if (m_pHint != NULL)
			m_pHint->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha));

		for (int nCntPlayer = 0; nCntPlayer < (1 < nClient ? nClient : 1); nCntPlayer++)
			if (m_pMember[nCntPlayer] != NULL)
				m_pMember[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha));

		if (m_pCancel != NULL)
			m_pCancel->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha));

		if (m_pEnter != NULL)
			m_pEnter->SetColor(&D3DXCOLOR(1.0f, 1.0f, 0.0f, (1 < nClient ? 1.0f : 0.0f)));
	}
}