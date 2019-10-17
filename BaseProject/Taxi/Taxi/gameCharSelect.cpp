//=============================================================================
//
// ゲーム（キャラ選択）の処理 [game.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#include "gameCharSelect.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CGameCharSelect::CGameCharSelect()
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
		m_pPlayerBG[nCntPlayer] = NULL;

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

	return S_OK;
}

//=============================================================================
// アンロード
//=============================================================================
void CGameCharSelect::Unload(void)
{

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
	// プレイヤー背景
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// プレイヤーカウント
		if (m_pPlayerBG[nCntPlayer] == NULL)
		{// NULL
			m_pPlayerBG[nCntPlayer] = new CScene2D(0, CScene::OBJTYPE_2DPOLYGON);
			m_pPlayerBG[nCntPlayer]->Init();
			m_pPlayerBG[nCntPlayer]->SetPosSize(
				D3DXVECTOR3(
				(nCntPlayer % 2 == 0 ? 0.0f + (SCREEN_HEIGHT * 0.25f) : SCREEN_WIDTH - (SCREEN_HEIGHT * 0.25f)),
					(nCntPlayer / 2 == 0 ? 0.0f + (SCREEN_HEIGHT * 0.25f) : SCREEN_HEIGHT - (SCREEN_HEIGHT * 0.25f)), 0.0f),
				D3DXVECTOR2(SCREEN_HEIGHT * 0.25f, SCREEN_HEIGHT * 0.25f));
			m_pPlayerBG[nCntPlayer]->SetColor(&D3DXCOLOR((nCntPlayer % 2 == 0 ? 1.0f : 0.0f), (nCntPlayer / 2 % 2 == 1 ? 1.0f : 0.0f), (nCntPlayer % 4 == 1 ? 1.0f : 0.0f), 0.6f));
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
				(SCREEN_WIDTH * 0.5f) + ((SCREEN_WIDTH * 0.045f + (nCntChar % 4 == 0 || nCntChar % 4 == 3 ? (SCREEN_WIDTH * 0.045f) * 2.0f : 0.0f)) * (nCntChar / 2 % 2 == 0 ? -1.0f : 1.0f)),
					(SCREEN_HEIGHT * 0.8f) + ((SCREEN_WIDTH * 0.045f) * (nCntChar / 4 == 0 ? -1.0f : 1.0f)),
					0.0f),
				D3DXVECTOR2(SCREEN_WIDTH * 0.045f, SCREEN_WIDTH * 0.045f));
			//m_pCharacter[nCntChar]->SetColor(&D3DXCOLOR((nCntChar % 2 == 0 ? 1.0f : 0.0f), (nCntChar / 2 % 2 == 1 ? 1.0f : 0.0f), (nCntChar % 4 == 1 ? 1.0f : 0.0f), (nCntChar / 4 == 0 ? 1.0f : 0.8f)));
			m_pCharacter[nCntChar]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, (1.0f / MAX_CHARCTER) * nCntChar));
		}
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
		if (m_pPlayerBG[nCntPlayer] != NULL)
		{// NULL以外
			m_pPlayerBG[nCntPlayer]->Uninit();	// 終了処理
			m_pPlayerBG[nCntPlayer] = NULL;		// NULLへ
		}
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
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputXPad * pXpad = CManager::GetXInput();					//ジョイパットの取得

	// キャラ選択番号取得
	int *pnCharSelectNum = CGame::GetCharSelectNum();

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// プレイヤーカウント


		if (pCInputKeyBoard->GetKeyboardTrigger(DIK_W) == true ||
			pXpad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_UP, nCntPlayer) == true ||
			pXpad->GetTrigger(XINPUT_GAMEPAD_DPAD_UP, nCntPlayer) == true)
		{// 上キー
			if (pnCharSelectNum[nCntPlayer] / 4 == 1)
			{// 移動制限
				// 次の値
				int nNext = (pnCharSelectNum[nCntPlayer] + 4) % MAX_CHARCTER;

				for (int nCntSelect = 0; nCntSelect < MAX_PLAYER; nCntSelect++)
				{// 選択カウント
					if (nCntSelect != nCntPlayer)
					{// 自分以外
						if (pnCharSelectNum[nCntSelect] == nNext)	// 値が一致
							break;

						if (nCntSelect == MAX_PLAYER - 1)			// 一致しなかったら更新
							pnCharSelectNum[nCntPlayer] = nNext;
					}
				}

			}
		}
		else if (pCInputKeyBoard->GetKeyboardTrigger(DIK_S) == true ||
			pXpad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_DOWN, nCntPlayer) == true ||
			pXpad->GetTrigger(XINPUT_GAMEPAD_DPAD_DOWN, nCntPlayer) == true)
		{// 下キー
			if (pnCharSelectNum[nCntPlayer] / 4 == 0)
			{// 移動制限
				// 次の値
				int nNext = (pnCharSelectNum[nCntPlayer] + 4) % MAX_CHARCTER;

				for (int nCntSelect = 0; nCntSelect < MAX_PLAYER; nCntSelect++)
				{// 選択カウント
					if (nCntSelect != nCntPlayer)
					{// 自分以外
						if (pnCharSelectNum[nCntSelect] == nNext)	// 値が一致
							break;

						if (nCntSelect == MAX_PLAYER - 1)			// 一致しなかったら更新
							pnCharSelectNum[nCntPlayer] = nNext;
					}
				}
			}
		}
		if (pCInputKeyBoard->GetKeyboardTrigger(DIK_A) == true ||
			pXpad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_LEFT, nCntPlayer) == true ||
			pXpad->GetTrigger(XINPUT_GAMEPAD_DPAD_LEFT, nCntPlayer) == true)
		{// 左キー
			if (pnCharSelectNum[nCntPlayer] % 4 != 0)
			{// 移動制限
				// 次の値
				int nNext = pnCharSelectNum[nCntPlayer];

				for (int nCntLine = pnCharSelectNum[nCntPlayer] % 4 - 1; 0 <= nCntLine; nCntLine--)
				{// ラインカウント
					nNext = (nNext - 1) % MAX_CHARCTER;

					for (int nCntSelect = 0; nCntSelect < MAX_PLAYER; nCntSelect++)
					{// 選択カウント
						if (nCntSelect != nCntPlayer)
						{// 自分以外
							if (pnCharSelectNum[nCntSelect] == nNext)	// 値が一致
								break;

							if (nCntSelect == MAX_PLAYER - 1)			// 一致しなかったら更新
								pnCharSelectNum[nCntPlayer] = nNext;
						}
					}

					if (pnCharSelectNum[nCntPlayer] == nNext)				// 更新した
						break;
				}
			}
		}
		else if (pCInputKeyBoard->GetKeyboardTrigger(DIK_D) == true ||
			pXpad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_RIGHT, nCntPlayer) == true ||
			pXpad->GetTrigger(XINPUT_GAMEPAD_DPAD_RIGHT, nCntPlayer) == true)
		{// 右キー
			if (pnCharSelectNum[nCntPlayer] % 4 != (4 - 1))
			{// 移動制限
				// 次の値
				int nNext = pnCharSelectNum[nCntPlayer];

				for (int nCntLine = pnCharSelectNum[nCntPlayer] % 4 + 1; nCntLine < 4; nCntLine++)
				{// ラインカウント	
					nNext = (nNext + 1) % MAX_CHARCTER;

					for (int nCntSelect = 0; nCntSelect < MAX_PLAYER; nCntSelect++)
					{// 選択カウント
						if (nCntSelect != nCntPlayer)
						{// 自分以外
							if (pnCharSelectNum[nCntSelect] == nNext)	// 値が一致
								break;

							if (nCntSelect == MAX_PLAYER - 1)			// 一致しなかったら更新
								pnCharSelectNum[nCntPlayer] = nNext;
						}
					}

					if (pnCharSelectNum[nCntPlayer] == nNext)				// 更新した
						break;
				}
			}
		}
	}

	// キャラクター
	for (int nCntChar = 0; nCntChar < MAX_CHARCTER; nCntChar++)
	{// キャラカウント
		if (m_pCharacter[nCntChar] != NULL)
		{// NULL以外
			for (int nCntSelect = 0; nCntSelect < MAX_PLAYER; nCntSelect++)
			{
				if (pnCharSelectNum[nCntSelect] == nCntChar)
				{
					m_pCharacter[nCntChar]->SetColor(&D3DXCOLOR((nCntSelect % 2 == 0 ? 1.0f : 0.0f), (nCntSelect / 2 % 2 == 1 ? 1.0f : 0.0f), (nCntSelect % 4 == 1 ? 1.0f : 0.0f), (nCntSelect / 4 == 0 ? 1.0f : 0.8f)));
					break;
				}
				else
				{
					m_pCharacter[nCntChar]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, (1.0f / MAX_CHARCTER) * nCntSelect));
				}
			}
		}
	}

	// キャラ選択番号設定
	CGame::SetCharSelectNum(pnCharSelectNum);
}

//=============================================================================
// 描画処理
//=============================================================================
void CGameCharSelect::Draw(void)
{

}