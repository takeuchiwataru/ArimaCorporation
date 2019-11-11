//=============================================================================
//
// ゲーム（キャラ選択）の処理 [game.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS	//Fire Wall突破
#include <string.h>				//ストリング使用のため
#include "gameCharSelect.h"
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
LPDIRECT3DTEXTURE9	CGameCharSelect::m_pTexture[CGameCharSelect::TEXTURE_MAX] = { NULL };

//=============================================================================
// コンストラクタ
//=============================================================================
CGameCharSelect::CGameCharSelect()
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// プレイヤーカウント
		m_pPlayerNum[nCntPlayer] = NULL;
		m_pPlayerBG[nCntPlayer] = NULL;
		m_pSelect[nCntPlayer] = NULL;
		m_pEnter[nCntPlayer] = NULL;

		m_bEntry[nCntPlayer] = false;
		m_bEnter[nCntPlayer] = false;
	}

	m_pYor = NULL;

	m_nEntryCounter = 0;				// エントリーカウント

	for (int nCntChar = 0; nCntChar < MAX_CHARCTER; nCntChar++)
		m_pCharacter[nCntChar] = NULL;
}
//=============================================================================
// デストラクタ
//=============================================================================
CGameCharSelect::~CGameCharSelect() {}

//=============================================================================
// ロード
//=============================================================================
HRESULT CGameCharSelect::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{// テクスチャカウント
		char cName[128] = {};

		switch (nCntTex)
		{
		case TEXTURE_CHAR:
			strcpy(cName, "data/TEXTURE/game/charselect/char.png");
			break;
		case TEXTURE_FRAME:
			strcpy(cName, "data/TEXTURE/game/charselect/select_frame.png");
			break;
		case TEXTURE_NUMBER:
			strcpy(cName, "data/TEXTURE/game/charselect/icon.png");
			break;
		case TEXTURE_ENTER:
			strcpy(cName, "data/TEXTURE/game/charselect/characterselect.png");
			break;
		case TEXTURE_YOU:
			strcpy(cName, "data/TEXTURE/game/charselect/modeselect_17.png");
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
void CGameCharSelect::Unload(void)
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
CGameCharSelect *CGameCharSelect::Create(void)
{
	CGameCharSelect *pGameCharSelect = NULL;

	// メモリ確保
	if (pGameCharSelect == NULL)
		pGameCharSelect = new CGameCharSelect;

	if (pGameCharSelect != NULL)
	{// NULL以外
		pGameCharSelect->Init();	// 初期化処理
	}

	return pGameCharSelect;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGameCharSelect::Init()
{
	// 取得
	bool bOnine = CTitle::GetOnline();

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// プレイヤーカウント	
		// プレイヤー番号
		if (m_pPlayerNum[nCntPlayer] == NULL)
		{// NULL
			m_pPlayerNum[nCntPlayer] = new CScene2D(1, CScene::OBJTYPE_2DPOLYGON);
			m_pPlayerNum[nCntPlayer]->Init();
			m_pPlayerNum[nCntPlayer]->SetPosSize(
				D3DXVECTOR3(
					(nCntPlayer % 2 == 0 ? 0.0f + (SCREEN_HEIGHT * 0.25f) : SCREEN_WIDTH - (SCREEN_HEIGHT * 0.25f)) - (SCREEN_HEIGHT * 0.18f),
					(nCntPlayer / 2 == 0 ? 0.0f + (SCREEN_HEIGHT * 0.25f) : SCREEN_HEIGHT - (SCREEN_HEIGHT * 0.25f)) - (SCREEN_HEIGHT * 0.2f),
					0.0f),
				D3DXVECTOR2(SCREEN_HEIGHT * 0.1f, SCREEN_HEIGHT * 0.05f));
			m_pPlayerNum[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_pPlayerNum[nCntPlayer]->BindTexture(m_pTexture[TEXTURE_NUMBER]);
			m_pPlayerNum[nCntPlayer]->SetTexture(nCntPlayer, 5, 2, 1);
		}
		// プレイヤー背景
		if (m_pPlayerBG[nCntPlayer] == NULL)
		{// NULL
			m_pPlayerBG[nCntPlayer] = new CScene2D(0, CScene::OBJTYPE_2DPOLYGON);
			m_pPlayerBG[nCntPlayer]->Init();
			m_pPlayerBG[nCntPlayer]->SetPosSize(
				D3DXVECTOR3(
					(nCntPlayer % 2 == 0 ? 0.0f + (SCREEN_HEIGHT * 0.25f) : SCREEN_WIDTH - (SCREEN_HEIGHT * 0.25f)),
					(nCntPlayer / 2 == 0 ? 0.0f + (SCREEN_HEIGHT * 0.25f) : SCREEN_HEIGHT - (SCREEN_HEIGHT * 0.25f)),
					0.0f),
				D3DXVECTOR2(SCREEN_HEIGHT * 0.25f, SCREEN_HEIGHT * 0.25f));
			//m_pPlayerBG[nCntPlayer]->SetColor(&D3DXCOLOR((nCntPlayer % 2 == 0 ? 1.0f : 0.0f), (nCntPlayer / 2 % 2 == 1 ? 1.0f : 0.0f), (nCntPlayer % 4 == 1 ? 1.0f : 0.0f), 0.6f));
			m_pPlayerBG[nCntPlayer]->BindTexture(m_pTexture[TEXTURE_FRAME]);
		}
		// 選択
		if (m_pSelect[nCntPlayer] == NULL)
		{// NULL
			m_pSelect[nCntPlayer] = new CScene2D(1, CScene::OBJTYPE_2DPOLYGON);
			m_pSelect[nCntPlayer]->Init();
			m_pSelect[nCntPlayer]->SetPosSize(
				D3DXVECTOR3(
				(SCREEN_WIDTH * 0.5f) + ((SCREEN_WIDTH * 0.055f + (nCntPlayer % 4 == 0 || nCntPlayer % 4 == 3 ? (SCREEN_WIDTH * 0.055f) * 2.0f : 0.0f)) * (nCntPlayer / 2 % 2 == 0 ? -1.0f : 1.0f)),
					(SCREEN_HEIGHT * 0.7f) + ((SCREEN_WIDTH * 0.055f) * (nCntPlayer / 4 == 0 ? -1.0f : 1.0f)),
					0.0f),
				D3DXVECTOR2(SCREEN_WIDTH * 0.04f, SCREEN_WIDTH * 0.04f));
			m_pSelect[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_pSelect[nCntPlayer]->BindTexture(m_pTexture[TEXTURE_NUMBER]);
			m_pSelect[nCntPlayer]->SetTexture(nCntPlayer, 5, 1, 1);
		}
		// 決定
		if (m_pEnter[nCntPlayer] == NULL)
		{// NULL
			m_pEnter[nCntPlayer] = new CScene2D(1, CScene::OBJTYPE_2DPOLYGON);
			m_pEnter[nCntPlayer]->Init();
			m_pEnter[nCntPlayer]->SetPosSize(
				D3DXVECTOR3(
				(nCntPlayer % 2 == 0 ? 0.0f + (SCREEN_HEIGHT * 0.25f) : SCREEN_WIDTH - (SCREEN_HEIGHT * 0.25f)),
					(nCntPlayer / 2 == 0 ? 0.0f + (SCREEN_HEIGHT * 0.25f) : SCREEN_HEIGHT - (SCREEN_HEIGHT * 0.25f)) + (SCREEN_HEIGHT * 0.18f),
					0.0f),
				D3DXVECTOR2(SCREEN_HEIGHT * 0.18f, SCREEN_HEIGHT * 0.05f));
			m_pEnter[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_pEnter[nCntPlayer]->BindTexture(m_pTexture[TEXTURE_ENTER]);
			m_pEnter[nCntPlayer]->SetTexture(1, 1, 2, 2);

		}
	}

	// キャラクター
	for (int nCntChar = 0; nCntChar < MAX_CHARCTER; nCntChar++)
	{// キャラカウント
		if (m_pCharacter[nCntChar] == NULL)
		{// NULL
			m_pCharacter[nCntChar] = new CScene2D(0, CScene::OBJTYPE_2DPOLYGON);
			m_pCharacter[nCntChar]->Init();
			m_pCharacter[nCntChar]->SetPosSize(
				D3DXVECTOR3(
					(SCREEN_WIDTH * 0.5f) + ((SCREEN_WIDTH * 0.053f + (nCntChar % 4 == 0 || nCntChar % 4 == 3 ? (SCREEN_WIDTH * 0.053f) * 2.0f : 0.0f)) * (nCntChar / 2 % 2 == 0 ? -1.0f : 1.0f)),
					(SCREEN_HEIGHT * 0.8f) + ((SCREEN_WIDTH * 0.053f) * (nCntChar / 4 == 0 ? -1.0f : 1.0f)),
					0.0f),
				D3DXVECTOR2(SCREEN_WIDTH * 0.05f, SCREEN_WIDTH * 0.05f));
			m_pCharacter[nCntChar]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_pCharacter[nCntChar]->BindTexture(m_pTexture[TEXTURE_CHAR]);
		}
	}

	if (bOnine == true)
	{// オンライン
		int nClient = CServer::GetnMaxClient();		// クライアント数
		int nID = CClient::GetnID();			// クライアントID

		// あなた
		if (m_pYor == NULL)
		{// NULL
			m_pYor = new CScene2D(0, CScene::OBJTYPE_2DPOLYGON);
			m_pYor->Init();
			m_pYor->SetPosSize(
				D3DXVECTOR3(
					(nID % 2 == 0 ? 0.0f + (SCREEN_HEIGHT * 0.25f) : SCREEN_WIDTH - (SCREEN_HEIGHT * 0.25f)) + (SCREEN_HEIGHT * 0.07f),
					(nID / 2 == 0 ? 0.0f + (SCREEN_HEIGHT * 0.25f) : SCREEN_HEIGHT - (SCREEN_HEIGHT * 0.25f)) - (SCREEN_HEIGHT * 0.18f),
					0.0f),
				D3DXVECTOR2(SCREEN_HEIGHT * 0.12f, SCREEN_HEIGHT * 0.04f));
			m_pYor->BindTexture(m_pTexture[TEXTURE_YOU]);
		}

		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
		{// プレイヤーカウント				
			// プレイヤー番号
			if (m_pPlayerNum[nCntPlayer] != NULL)
			{// NULL以外
				if (nCntPlayer < nClient)
					m_pPlayerNum[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			// 決定
			if (m_pEnter[nCntPlayer] != NULL)
			{// NULL以外
				m_pEnter[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
				m_pEnter[nCntPlayer]->SetTexture(0, 1, 2, 2);
			}

			// 参加
			if (nCntPlayer < nClient)
				m_bEntry[nCntPlayer] = true;
		}
		
		// プレイヤー最大数設定
		CGame::SetMaxPlayer(nClient);
	}

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CGameCharSelect::Uninit(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// プレイヤーカウント	
		if (m_pPlayerNum[nCntPlayer] != NULL)
		{// NULL以外
			m_pPlayerNum[nCntPlayer]->Uninit();	// 終了処理
			m_pPlayerNum[nCntPlayer] = NULL;	// NULLへ
		}
		if (m_pPlayerBG[nCntPlayer] != NULL)
		{// NULL以外
			m_pPlayerBG[nCntPlayer]->Uninit();	// 終了処理
			m_pPlayerBG[nCntPlayer] = NULL;		// NULLへ
		}
		if (m_pSelect[nCntPlayer] != NULL)
		{// NULL以外
			m_pSelect[nCntPlayer]->Uninit();	// 終了処理
			m_pSelect[nCntPlayer] = NULL;		// NULLへ
		}
		if (m_pEnter[nCntPlayer] != NULL)
		{// NULL以外
			m_pEnter[nCntPlayer]->Uninit();		// 終了処理
			m_pEnter[nCntPlayer] = NULL;		// NULLへ
		}
	}

	if (m_pYor != NULL)
	{// NULL以外
		m_pYor->Uninit();	// 終了処理
		m_pYor = NULL;		// NULLへ
	}

	// キャラクター
	for (int nCntChar = 0; nCntChar < MAX_CHARCTER; nCntChar++)
	{// キャラカウント
		if (m_pCharacter[nCntChar] != NULL)
		{// NULL以外
			m_pCharacter[nCntChar]->Uninit();	// 終了処理
			m_pCharacter[nCntChar] = NULL;		// NULLへ
		}
	}

	//自身の削除
	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CGameCharSelect::Update(void)
{
	// 取得
	bool bOnine = CTitle::GetOnline();

	if (bOnine == true)	
		Online();		// オンライン
	else
		Local();		// ローカル
}

//=============================================================================
// 描画処理
//=============================================================================
void CGameCharSelect::Draw(void)
{

}

//=============================================================================
// オンライン
//=============================================================================
void CGameCharSelect::Online(void)
{
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputJoyPad_0 * pXpad = CManager::GetInputJoyPad0(0);		//ジョイパットの取得
	CSound *pSound = CManager::GetSound();						//サウンドの情報

	// プレイヤー最大数取得
	int nMaxPlayer = CGame::GetMaxPlayer();
	// キャラ選択番号取得
	int *pnCharSelectNum = CGame::GetCharSelectNum();

	int nPlayerNum = nMaxPlayer;

	if (CFade::GetFade() == CFade::FADE_NONE)
	{// 使用していない
		for (int nCntPlayer = 0; nCntPlayer < nMaxPlayer; nCntPlayer++)
		{// プレイヤーカウント
			pXpad = CManager::GetInputJoyPad0(nCntPlayer);		//ジョイパットの取得

			if (m_bEnter[nCntPlayer] == false)
			{// 決定していない
				if (pCInputKeyBoard->GetKeyboardTrigger(DIK_W) == true ||
					pXpad->GetTrigger(INPUT_LS_U) == true ||
					pXpad->GetTrigger(INPUT_UP) == true)
				{// 上キー
					if (pnCharSelectNum[nCntPlayer] / 4 == 1)
					{// 移動制限
					 // 次の値
						int nNext = (pnCharSelectNum[nCntPlayer] + 4) % MAX_CHARCTER;

						for (int nCntSelect = 0; nCntSelect < nMaxPlayer; nCntSelect++)
						{// 選択カウント
							if (nCntSelect != nCntPlayer)
							{// 自分以外
								if (pnCharSelectNum[nCntSelect] == nNext)	// 値が一致
									break;
							}

							if (nCntSelect == (nMaxPlayer - 1))			// 一致しなかったら更新
								pnCharSelectNum[nCntPlayer] = nNext;
						}

					}
				}
				else if (pCInputKeyBoard->GetKeyboardTrigger(DIK_S) == true ||
					pXpad->GetTrigger(INPUT_LS_D) == true ||
					pXpad->GetTrigger(INPUT_DOWN) == true)
				{// 下キー
					if (pnCharSelectNum[nCntPlayer] / 4 == 0)
					{// 移動制限
					 // 次の値
						int nNext = (pnCharSelectNum[nCntPlayer] + 4) % MAX_CHARCTER;

						for (int nCntSelect = 0; nCntSelect < nMaxPlayer; nCntSelect++)
						{// 選択カウント
							if (nCntSelect != nCntPlayer)
							{// 自分以外
								if (pnCharSelectNum[nCntSelect] == nNext)	// 値が一致
									break;
							}

							if (nCntSelect == (nMaxPlayer - 1))			// 一致しなかったら更新
								pnCharSelectNum[nCntPlayer] = nNext;
						}
					}
				}
				if (pCInputKeyBoard->GetKeyboardTrigger(DIK_A) == true ||
					pXpad->GetTrigger(INPUT_LS_L) == true ||
					pXpad->GetTrigger(INPUT_LEFT) == true)
				{// 左キー
					if (pnCharSelectNum[nCntPlayer] % 4 != 0)
					{// 移動制限
					 // 次の値
							int nNext = pnCharSelectNum[nCntPlayer];

						for (int nCntLine = pnCharSelectNum[nCntPlayer] % 4 - 1; 0 <= nCntLine; nCntLine--)
						{// ラインカウント
							nNext = (nNext - 1) % MAX_CHARCTER;

							for (int nCntSelect = 0; nCntSelect < nMaxPlayer; nCntSelect++)
							{// 選択カウント
								if (nCntSelect != nCntPlayer)
								{// 自分以外
									if (pnCharSelectNum[nCntSelect] == nNext)	// 値が一致
										break;
								}

								if (nCntSelect == (nMaxPlayer - 1))			// 一致しなかったら更新
									pnCharSelectNum[nCntPlayer] = nNext;
							}

							if (pnCharSelectNum[nCntPlayer] == nNext)				// 更新した
								break;
						}
					}
				}
				else if (pCInputKeyBoard->GetKeyboardTrigger(DIK_D) == true ||
					pXpad->GetTrigger(INPUT_LS_R) == true ||
					pXpad->GetTrigger(INPUT_RIGHT) == true)
				{// 右キー
					if (pnCharSelectNum[nCntPlayer] % 4 != (4 - 1))
					{// 移動制限
					 // 次の値
						int nNext = pnCharSelectNum[nCntPlayer];

						for (int nCntLine = pnCharSelectNum[nCntPlayer] % 4 + 1; nCntLine < 4; nCntLine++)
						{// ラインカウント	
							nNext = (nNext + 1) % MAX_CHARCTER;

							for (int nCntSelect = 0; nCntSelect < nMaxPlayer; nCntSelect++)
							{// 選択カウント
								if (nCntSelect != nCntPlayer)
								{// 自分以外
									if (pnCharSelectNum[nCntSelect] == nNext)	// 値が一致
										break;
								}

								if (nCntSelect == (nMaxPlayer - 1))			// 一致しなかったら更新
									pnCharSelectNum[nCntPlayer] = nNext;
							}

							if (pnCharSelectNum[nCntPlayer] == nNext)				// 更新した
								break;
						}
					}
				}
			}

			if (pXpad->GetTrigger(INPUT_B) == true)
			{// 決定キー
				if (m_bEntry[nCntPlayer] == true)
				{
					m_bEnter[nCntPlayer] = (m_bEnter[nCntPlayer] ^ 1 ? true : false);

					//決定音の設定
					pSound->SetVolume(CSound::SOUND_LABEL_SE_CHARACTERSERECT, 1.3f);
					pSound->PlaySound(CSound::SOUND_LABEL_SE_CHARACTERSERECT);
				}
			}
		}
	}

	int nChackEnter = 0;
	for (int nCntPlayer = 0; nCntPlayer < nMaxPlayer; nCntPlayer++)
	{// プレイヤーカウント
		// 選択
		if (m_pSelect[nCntPlayer] != NULL)
		{// NULL以外
			m_pSelect[nCntPlayer]->SetPosSize(
				D3DXVECTOR3(
				(SCREEN_WIDTH * 0.5f) + ((SCREEN_WIDTH * 0.053f + (pnCharSelectNum[nCntPlayer] % 4 == 0 || pnCharSelectNum[nCntPlayer] % 4 == 3 ? (SCREEN_WIDTH * 0.053f) * 2.0f : 0.0f)) * (pnCharSelectNum[nCntPlayer] / 2 % 2 == 0 ? -1.0f : 1.0f)),
					(SCREEN_HEIGHT * 0.7f) + ((SCREEN_WIDTH * 0.053f) * (pnCharSelectNum[nCntPlayer] / 4 == 0 ? -1.0f : 1.0f)),
					0.0f),
				D3DXVECTOR2(SCREEN_WIDTH * 0.04f, SCREEN_WIDTH * 0.04f));
			m_pSelect[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}

		// 決定数チャック
		if (m_bEnter[nCntPlayer] == true)
		{// 決定
			nChackEnter++;

			// 決定
			if (m_pEnter[nCntPlayer] != NULL)
			{
				m_pEnter[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
		else
		{// 選択中
		 // 決定
			if (m_pEnter[nCntPlayer] != NULL)
			{
				m_pEnter[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			}
		}

		// 全員決定
		if (nChackEnter == nMaxPlayer)
			CFade::Create(CGame::GAMEMODE_COURSE_VIEW);
	}

	// プレイヤー最大数設定
	CGame::SetMaxPlayer(nMaxPlayer);
	// キャラ選択番号設定
	CGame::SetCharSelectNum(pnCharSelectNum);

	m_nEntryCounter++;		// カウント	
}

//=============================================================================
// ローカル
//=============================================================================
void CGameCharSelect::Local(void)
{
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputJoyPad_0 * pXpad = CManager::GetInputJoyPad0(0);		//ジョイパットの取得
	CSound *pSound = CManager::GetSound();						//サウンドの情報

	// プレイヤー最大数取得
	int nMaxPlayer = CGame::GetMaxPlayer();
	// キャラ選択番号取得
	int *pnCharSelectNum = CGame::GetCharSelectNum();
	// コントローラー番号取得
	int *pnControllerNum = CGame::GetControllerNum();

	int nPlayerNum = nMaxPlayer;

	if (CFade::GetFade() == CFade::FADE_NONE)
	{// 使用していない
		for (int nCntPlayer = 0; nCntPlayer < nMaxPlayer; nCntPlayer++)
		{// プレイヤーカウント
			pXpad = CManager::GetInputJoyPad0(pnControllerNum[nCntPlayer]);		//ジョイパットの取得

			if (m_bEnter[nCntPlayer] == false)
			{// 決定していない
				if (pCInputKeyBoard->GetKeyboardTrigger(DIK_W) == true ||
					pCInputKeyBoard->GetKeyboardTrigger(DIK_UP) == true || 
					pXpad->GetTrigger(INPUT_LS_U) == true ||
					pXpad->GetTrigger(INPUT_UP) == true)
				{// 上キー
					if (pnCharSelectNum[nCntPlayer] / 4 == 1)
					{// 移動制限
					 // 次の値
						int nNext = (pnCharSelectNum[nCntPlayer] + 4) % MAX_CHARCTER;

						for (int nCntSelect = 0; nCntSelect < nMaxPlayer; nCntSelect++)
						{// 選択カウント
							if (nCntSelect != nCntPlayer)
							{// 自分以外
								if (pnCharSelectNum[nCntSelect] == nNext)	// 値が一致
									break;
							}

							if (nCntSelect == (nMaxPlayer - 1))			// 一致しなかったら更新
								pnCharSelectNum[nCntPlayer] = nNext;
						}

					}
				}
				else if (pCInputKeyBoard->GetKeyboardTrigger(DIK_S) == true ||
					pCInputKeyBoard->GetKeyboardTrigger(DIK_DOWN) == true || 
					pXpad->GetTrigger(INPUT_LS_D) == true ||
					pXpad->GetTrigger(INPUT_DOWN) == true)
				{// 下キー
					if (pnCharSelectNum[nCntPlayer] / 4 == 0)
					{// 移動制限
					 // 次の値
						int nNext = (pnCharSelectNum[nCntPlayer] + 4) % MAX_CHARCTER;

						for (int nCntSelect = 0; nCntSelect < nMaxPlayer; nCntSelect++)
						{// 選択カウント
							if (nCntSelect != nCntPlayer)
							{// 自分以外
								if (pnCharSelectNum[nCntSelect] == nNext)	// 値が一致
									break;
							}

							if (nCntSelect == (nMaxPlayer - 1))			// 一致しなかったら更新
								pnCharSelectNum[nCntPlayer] = nNext;
						}
					}
				}
				if (pCInputKeyBoard->GetKeyboardTrigger(DIK_A) == true ||
					pCInputKeyBoard->GetKeyboardTrigger(DIK_LEFT) == true ||
					pXpad->GetTrigger(INPUT_LS_L) == true ||
					pXpad->GetTrigger(INPUT_LEFT) == true)
				{// 左キー
					if (pnCharSelectNum[nCntPlayer] % 4 != 0)
					{// 移動制限
					 // 次の値
						int nNext = pnCharSelectNum[nCntPlayer];

						for (int nCntLine = pnCharSelectNum[nCntPlayer] % 4 - 1; 0 <= nCntLine; nCntLine--)
						{// ラインカウント
							nNext = (nNext - 1) % MAX_CHARCTER;

							for (int nCntSelect = 0; nCntSelect < nMaxPlayer; nCntSelect++)
							{// 選択カウント
								if (nCntSelect != nCntPlayer)
								{// 自分以外
									if (pnCharSelectNum[nCntSelect] == nNext)	// 値が一致
										break;
								}

								if (nCntSelect == (nMaxPlayer - 1))			// 一致しなかったら更新
									pnCharSelectNum[nCntPlayer] = nNext;
							}

							if (pnCharSelectNum[nCntPlayer] == nNext)				// 更新した
								break;
						}
					}
				}
				else if (pCInputKeyBoard->GetKeyboardTrigger(DIK_D) == true ||
					pCInputKeyBoard->GetKeyboardTrigger(DIK_RIGHT) == true || 
					pXpad->GetTrigger(INPUT_LS_R) == true ||
					pXpad->GetTrigger(INPUT_RIGHT) == true)
				{// 右キー
					if (pnCharSelectNum[nCntPlayer] % 4 != (4 - 1))
					{// 移動制限
					 // 次の値
						int nNext = pnCharSelectNum[nCntPlayer];

						for (int nCntLine = pnCharSelectNum[nCntPlayer] % 4 + 1; nCntLine < 4; nCntLine++)
						{// ラインカウント	
							nNext = (nNext + 1) % MAX_CHARCTER;

							for (int nCntSelect = 0; nCntSelect < nMaxPlayer; nCntSelect++)
							{// 選択カウント
								if (nCntSelect != nCntPlayer)
								{// 自分以外
									if (pnCharSelectNum[nCntSelect] == nNext)	// 値が一致
										break;
								}

								if (nCntSelect == (nMaxPlayer - 1))			// 一致しなかったら更新
									pnCharSelectNum[nCntPlayer] = nNext;
							}

							if (pnCharSelectNum[nCntPlayer] == nNext)				// 更新した
								break;
						}
					}
				}
			}

			if (pXpad->GetTrigger(INPUT_B) == true)
			{// 決定キー
				if (m_bEntry[nCntPlayer] == true)
				{
					m_bEnter[nCntPlayer] = (m_bEnter[nCntPlayer] ^ 1 ? true : false);

					//決定音の設定
					pSound->SetVolume(CSound::SOUND_LABEL_SE_CHARACTERSERECT, 1.3f);
					pSound->PlaySound(CSound::SOUND_LABEL_SE_CHARACTERSERECT);
				}
			}
		}

		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
		{// プレイヤーカウント
			bool bSet = false;	// コントローラーチェック用

								// 使用していないコントローラーがチェック
			for (int nCntCheck = 0; nCntCheck < nMaxPlayer; nCntCheck++)
				if (pnControllerNum[nCntCheck] == nCntPlayer)
					bSet = true;

			if (bSet == false)
			{// 使用していない
				pXpad = CManager::GetInputJoyPad0(nCntPlayer);		//ジョイパットの取得

				if (pXpad->GetTrigger(INPUT_B) == true)
				{// 決定キー
					if (m_bEntry[nPlayerNum] == false)
					{
						m_bEntry[nPlayerNum] = true;
						pnControllerNum[nPlayerNum] = nCntPlayer;
						nPlayerNum = nPlayerNum + (nPlayerNum < (MAX_PLAYER - 1) ? 1 : 0);
					}
				}
			}
		}

		// キーボード
		if (pCInputKeyBoard->GetKeyboardTrigger(DIK_Z) == true || pCInputKeyBoard->GetKeyboardTrigger(DIK_RETURN) == true)
		{
			if (m_bEntry[0] == true)
			{
				m_bEnter[0] = (m_bEnter[0] ^ 1 ? true : false);

				//決定音の設定
				pSound->SetVolume(CSound::SOUND_LABEL_SE_CHARACTERSERECT, 1.3f);
				pSound->PlaySound(CSound::SOUND_LABEL_SE_CHARACTERSERECT);
			}
			else
			{
				m_bEntry[0] = true;
			}
		}

		// デバッグ（各決定）
		if (pCInputKeyBoard->GetKeyboardTrigger(DIK_1) == true)
		{
			if (m_bEntry[0] == true)
			{
				m_bEnter[0] = (m_bEnter[0] ^ 1 ? true : false);

				//決定音の設定
				pSound->SetVolume(CSound::SOUND_LABEL_SE_CHARACTERSERECT, 1.3f);
				pSound->PlaySound(CSound::SOUND_LABEL_SE_CHARACTERSERECT);
			}
			else
			{
				m_bEntry[0] = true;
			}
		}
		if (pCInputKeyBoard->GetKeyboardTrigger(DIK_2) == true)
		{
			if (m_bEntry[1] == true)
			{
				m_bEnter[1] = (m_bEnter[1] ^ 1 ? true : false);

				//決定音の設定
				pSound->SetVolume(CSound::SOUND_LABEL_SE_CHARACTERSERECT, 1.3f);
				pSound->PlaySound(CSound::SOUND_LABEL_SE_CHARACTERSERECT);
			}
			else
			{
				m_bEntry[1] = true;
			}
		}
		if (pCInputKeyBoard->GetKeyboardTrigger(DIK_3) == true)
		{
			if (m_bEntry[2] == true)
			{
				m_bEnter[2] = (m_bEnter[2] ^ 1 ? true : false);

				//決定音の設定
				pSound->SetVolume(CSound::SOUND_LABEL_SE_CHARACTERSERECT, 1.3f);
				pSound->PlaySound(CSound::SOUND_LABEL_SE_CHARACTERSERECT);
			}
			else
			{
				m_bEntry[2] = true;
			}
		}
		if (pCInputKeyBoard->GetKeyboardTrigger(DIK_4) == true)
		{
			if (m_bEntry[3] == true)
			{
				m_bEnter[3] = (m_bEnter[3] ^ 1 ? true : false);

				//決定音の設定
				pSound->SetVolume(CSound::SOUND_LABEL_SE_CHARACTERSERECT, 1.3f);
				pSound->PlaySound(CSound::SOUND_LABEL_SE_CHARACTERSERECT);
			}
			else
			{
				m_bEntry[3] = true;
			}
		}
	}

	int nChackEnter = 0;
	for (int nCntPlayer = 0; nCntPlayer < nMaxPlayer; nCntPlayer++)
	{// プレイヤーカウント
	 // プレイヤー番号
		if (m_pPlayerNum[nCntPlayer] != NULL)
		{// NULL以外
			m_pPlayerNum[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		// 選択
		if (m_pSelect[nCntPlayer] != NULL)
		{// NULL以外
			m_pSelect[nCntPlayer]->SetPosSize(
				D3DXVECTOR3(
				(SCREEN_WIDTH * 0.5f) + ((SCREEN_WIDTH * 0.053f + (pnCharSelectNum[nCntPlayer] % 4 == 0 || pnCharSelectNum[nCntPlayer] % 4 == 3 ? (SCREEN_WIDTH * 0.053f) * 2.0f : 0.0f)) * (pnCharSelectNum[nCntPlayer] / 2 % 2 == 0 ? -1.0f : 1.0f)),
					(SCREEN_HEIGHT * 0.7f) + ((SCREEN_WIDTH * 0.053f) * (pnCharSelectNum[nCntPlayer] / 4 == 0 ? -1.0f : 1.0f)),
					0.0f),
				D3DXVECTOR2(SCREEN_WIDTH * 0.04f, SCREEN_WIDTH * 0.04f));
			m_pSelect[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}

		// 決定数チャック
		if (m_bEnter[nCntPlayer] == true)
		{// 決定
			nChackEnter++;

			// 決定
			if (m_pEnter[nCntPlayer] != NULL)
			{
				m_pEnter[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_pEnter[nCntPlayer]->SetTexture(0, 1, 2, 2);
			}
		}
		else
		{// 選択中
		 // 決定
			if (m_pEnter[nCntPlayer] != NULL)
			{
				m_pEnter[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			}
		}

		// 全員決定
		if (nChackEnter == nMaxPlayer)
			CFade::Create(CGame::GAMEMODE_COURSE_VIEW);
	}

	int nEntryNum = 0;
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// プレイヤーカウント
		if (m_bEntry[nCntPlayer] == true)
			nEntryNum++;
	}

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// プレイヤーカウント
	 // 決定
		if (nEntryNum <= nCntPlayer)
		{
			if (m_pEnter[nCntPlayer] != NULL)
			{
				int nCount = (m_nEntryCounter % 60);
				if (nCount < 20)
				{
					if (nCount < 10)
						m_pEnter[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - ((1.0f / 10.0f) * nCount)));
					else
						m_pEnter[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, ((1.0f / 10.0f) * (nCount - 10))));
				}
				else
				{
					m_pEnter[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
				m_pEnter[nCntPlayer]->SetTexture(1, 1, 2, 2);
			}
		}
	}

	if (nMaxPlayer != nEntryNum)
	{// プレイヤー数が変わった
		nMaxPlayer = nEntryNum;		// プレイヤー数を更新

		for (int nCntPlayer = (nMaxPlayer - 1); 0 <= nCntPlayer; nCntPlayer--)
		{// プレイヤーカウント
			while (1)
			{// 同じ番号をなくす
				bool bCheck = false;
				for (int nCntCheck = (nMaxPlayer - 1); 0 <= nCntCheck; nCntCheck--)
				{// チェックカウント
					if (nCntPlayer != nCntCheck)
					{// 自分以外
						if (pnCharSelectNum[nCntPlayer] == pnCharSelectNum[nCntCheck])
						{// 同じ番号があった
							bCheck = true;
							break;
						}
					}
				}

				// 番号を変更 or break
				if (bCheck == true)
					pnCharSelectNum[nCntPlayer] = (pnCharSelectNum[nCntPlayer] + 1) % MAX_CHARCTER;
				else
					break;
			}

			while (1)
			{// 同じ番号をなくす
				bool bCheck = false;
				for (int nCntCheck = (nMaxPlayer - 1); 0 <= nCntCheck; nCntCheck--)
				{// チェックカウント
					if (nCntPlayer != nCntCheck)
					{// 自分以外
						if (pnControllerNum[nCntPlayer] == pnControllerNum[nCntCheck])
						{// 同じ番号があった
							bCheck = true;
							break;
						}
					}
				}

				// 番号を変更 or break
				if (bCheck == true)
					pnControllerNum[nCntPlayer] = (pnControllerNum[nCntPlayer] + 1) % MAX_PLAYER;
				else
					break;
			}
		}
	}

	// プレイヤー最大数設定
	CGame::SetMaxPlayer(nMaxPlayer);
	// キャラ選択番号設定
	CGame::SetCharSelectNum(pnCharSelectNum);
	// コントローラー設定
	CGame::SetControllerNum(pnControllerNum);

	m_nEntryCounter++;		// カウント
}