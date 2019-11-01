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
	m_pPress = NULL;		// プレス
	m_nCntPress = 0;		// プレスカウント

	for (int nCntMainMenu = 0; nCntMainMenu < MAX_MAINMENU; nCntMainMenu++)
		m_pMainMenu[nCntMainMenu] = NULL;		// メインメニュー
	m_nCntMainMenu = 0;							// メインメニューカウント

	m_nSelectNum = 0;		// 選択番号

	m_bSubMenu = false;		// サブメニュー

	m_bEntry = false;		// 受付中
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
		case TEXTURE_PRESS:
			strcpy(cName, "data\\TEXTURE\\Title\\UI\\PressButton.png");
			break;
		case TEXTURE_MAINMENU:
			strcpy(cName, "data\\TEXTURE\\Title\\UI\\modeselect_00.png");
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

	for (int nCntMainMenu = 0; nCntMainMenu < MAX_MAINMENU; nCntMainMenu++)
	{// メインメニュー
		if (m_pMainMenu[nCntMainMenu] == NULL)
		{// NULL
			m_pMainMenu[nCntMainMenu] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
			m_pMainMenu[nCntMainMenu]->Init();
			m_pMainMenu[nCntMainMenu]->SetPosSize(
				D3DXVECTOR3
				(
					SCREEN_WIDTH * 0.5f - (SCREEN_WIDTH * 0.11f * 2.0f) + ((SCREEN_WIDTH * 0.11f * 2.0f) * nCntMainMenu),
					SCREEN_HEIGHT * 0.8f,
					0.0f
				),
				D3DXVECTOR2(SCREEN_WIDTH * 0.1f, SCREEN_WIDTH * 0.1f));
			m_pMainMenu[nCntMainMenu]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

			if (nCntMainMenu != (MAX_MAINMENU - 1))
			{// もどる以外
				m_pMainMenu[nCntMainMenu]->BindTexture(m_pTexture[TEXTURE_MAINMENU]);
				m_pMainMenu[nCntMainMenu]->SetTexture(nCntMainMenu, 2, 1, 1);
			}
			else
			{
				//m_pMainMenu[nCntMainMenu]->BindTexture(m_pTexture[TEXTURE_MAINMENU]);
			}
		}
	}

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CTitleMenu::Uninit(void)
{
	// プレス
	if (m_pPress != NULL)
	{// NULL以外
		m_pPress->Uninit();
		m_pPress = NULL;
	}

	for (int nCntMainMenu = 0; nCntMainMenu < MAX_MAINMENU; nCntMainMenu++)
	{// メインメニュー
		if (m_pMainMenu[nCntMainMenu] != NULL)
		{// NULL以外
			m_pMainMenu[nCntMainMenu]->Uninit();
			m_pMainMenu[nCntMainMenu] = NULL;
		}
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
			EditMainMenu();
			EditSubMenu();
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
		if (m_nCntPress == 0 && m_nCntMainMenu == (60 - 1))
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
				CManager::OnlineSeting(true, bHost);	// オンライン設定
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

	if (m_nCntMainMenu == 0)
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
			CManager::OnlineSeting(false);	// オンライン設定
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
					m_bSubMenu = true;
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
			}
		}

		// メインメニュー
		for (int nCntMainMenu = 0; nCntMainMenu < MAX_MAINMENU; nCntMainMenu++)
		{// メインメニューカウント
			if (m_pMainMenu[nCntMainMenu] != NULL)
			{// NULL以外
				float fcol = 0.0f;

				// 色調整
				if (m_nSelectNum == nCntMainMenu)	// 選択番号と一致
					fcol = 1.0f;
				else
					fcol = 0.3f;

				// 色設定
				m_pMainMenu[nCntMainMenu]->SetColor(&D3DXCOLOR(fcol, fcol, fcol, fAlpha));
			}
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
					m_bSubMenu = false;
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
			if (m_pMainMenu[nCntMainMenu] != NULL)
			{// NULL以外
				float fcol = 0.0f;

				// 色調整
				if (m_nSelectNum == nCntMainMenu)	// 選択番号と一致
					fcol = 1.0f;
				else
					fcol = 0.3f;

				// 色設定
				m_pMainMenu[nCntMainMenu]->SetColor(&D3DXCOLOR(0.0f, 0.0f, fcol, fAlpha));
			}
		}
	}
}