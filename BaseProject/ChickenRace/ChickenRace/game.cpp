//=============================================================================
//
// ゲームの処理 [game.cpp]
// Author : 有馬　武志
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS	//Fire Wall突破
#include <string.h>				//ストリング使用のため
#include "game.h"
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "player.h"
#include "input.h"
#include "fade.h"
#include "pause.h"
#include "sound.h"
#include "billboord.h"
#include "meshfield.h"
#include "object.h"
#include "fade.h"
#include "wall.h"
#include "shadow.h"
#include "loadText.h"
#include "gamecamera.h"
#include "select.h"
#include "feed.h"
#include "egg.h"
#include "gameCharSelect.h"
#include "gamePlay.h"
#include "chick.h"
#include "ColMesh.h"
#include "particle.h"
#include "time.h"
#include "model.h"
#include "number.h"
#include "Character.h"
#include "toonshader.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXT_OBJECTNAME1		"data\\TEXT\\ゲームマップ\\objecy.txt"			// 読み込むテキストファイル
#define TEXT_MESHFIELDNAME1		"data\\TEXT\\ゲームマップ\\meshfield.txt"		// 読み込むテキストファイル
#define TEXT_WALLNAME1			"data\\TEXT\\ゲームマップ\\wall.txt"			// 読み込むテキストファイル
#define TEXT_FEEDNAME			"data\\TEXT\\ゲームマップ\\feed.txt"			// 読み込むテキストファイル
#define TEXT_PLAYER_MOTION		"data\\TEXT\\Player\\Player.txt"				// プレイヤーのモーションファイル
#define VECTOR_ZERO				(D3DXVECTOR3(0.0f, 0.0f, 0.0f))

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CGameCharSelect *CGame::m_pGameCharSelect = NULL;	// ゲーム（キャラ選択）
CGamePlay *CGame::m_pGamePlay = NULL;				// ゲーム（プレイ）

CPlayer *CGame::m_pPlayer[MAX_MEMBER] = { NULL };
CPause *CGame::m_pPause = NULL;
//CLoadTextMotion * CGame::m_pPlayerMotion = NULL;
CGameCamera * CGame::m_pCuorseCamera = NULL;
CGameCamera * CGame::m_pGameCamera[MAX_PLAYER] = { NULL };
bool CGame::m_bHelp = false;
bool CGame::m_bPause = false;
CGame::GAMEMODE CGame::m_gameMode = CGame::GAMEMODE_NONE;
CGame::GAMEMODE CGame::m_gameModeNext = CGame::GAMEMODE_NONE;
CGame::GAMESTATE CGame::m_gameState = CGame::GAMESTATE_NONE;
int	CGame::m_nCntSetStage = 0;
int CGame::m_nGameCounter = 0;

//ウォークスルー用
bool CGame::m_bDrawUI = false;

int CGame::m_nMaxPlayer = 0;						// プレイヤー数
int CGame::m_nCharSelectNum[MAX_MEMBER] = { 0 };	// キャラ選択番号
int CGame::m_nControllerNum[MAX_PLAYER] = { 0 };	// コントローラー番号
int CGame::m_nRanking[MAX_MEMBER] = { 0 };			// ランキング
int CGame::m_nRankingSort[MAX_MEMBER] = { 0 };		// ランキング
bool CGame::m_bGoul[MAX_PLAYER] = { false };		// ゴール

int CGame::m_nCameraNumber = 0;						// 現在使用しているカメラ番号

int CGame::m_nTime[MAX_MEMBER] = { 0 };				// タイム

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CGame::CGame()
{
	m_gameMode = GAMEMODE_NONE;
	m_gameModeNext = GAMEMODE_NONE;
	m_gameState = GAMESTATE_NONE;
	m_nCounterGameState = 0;
	m_NowGameState = GAMESTATE_NONE;
	m_nCntSetStage = 0;
	m_bPause = false;
}
//=============================================================================
// デストラクタ
//=============================================================================
CGame::~CGame() {}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGame::Init()
{
	//====================================================================
	//					テクスチャモデルの読み込み場所
	//====================================================================

	CFade::Load();				//フェードのテクスチャの読み込み
	CMeshField::Load();			//メッシュフィールドのテクスチャの読み込み
	CWall::Load();				//壁のテクスチャの読み込み
	CShadow::Load();			//影のテクスチャ読み込み
	CObject::Load();			//オブジェクトのテクスチャの読み込み
	CFeed::Load();				//食べ物のテクスチャの読み込み
	CEgg::Load();				//卵のテクスチャの読み込み
	CChick::Load();				//ひよこのテクスチャの読み込み
	CParticle::Load();			// パーティクルのテクスチャのロード
	m_pPause->Load();			//ポーズのテクスチャの読み込み
	CToonShader::Load();

	CGameCharSelect::Load();	// ゲーム（キャラ選択）
	CGamePlay::Load();			// ゲーム（プレイ）

	CPlayer::Load();			//モデルの読み込み
	CModel::Load();				//キャラモデルの読み込み

	CTime::Load();
	CNumber::Load();

	//====================================================================
	//						 必要な変数の初期化
	//====================================================================

	m_gameMode = GAMEMODE_CHARSELECT;			// ゲームモード
	m_gameModeNext = m_gameMode;		// 次のゲームモード
	m_gameState = GAMESTATE_NORMAL;		//通常状態に
	m_nCntSetStage = 0;					//どこのステージから開始するか
	m_bPause = false;					//ポーズを初期化
	m_nGameCounter = 0;					//カウンターの初期化

	m_nCameraNumber = 0;				// 現在使用しているカメラ番号

	m_nMaxPlayer = 0;					// プレイヤー数
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		m_nControllerNum[nCntPlayer] = nCntPlayer;		// コントローラー番号
		m_bGoul[nCntPlayer] = false;					// ゴール
	}

	for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
	{
		m_nCharSelectNum[nCntMember] = nCntMember;		// キャラ選択番号
		m_nRanking[nCntMember] = nCntMember;			// ランキング
		m_nRankingSort[nCntMember] = nCntMember;
		m_nTime[nCntMember] = 0;
	}
														// デバッグ用
	if (m_gameMode == GAMEMODE_COURSE_VIEW)
		m_nMaxPlayer = 1;
	else if (m_gameMode == GAMEMODE_PLAY)
		m_nMaxPlayer = 1;

	SetGameMode(m_gameMode);			// ゲームモード設定

	// カウントをなくす
	//if (m_gameMode == GAMEMODE_PLAY) 
	//	m_nGameCounter = START_SET_TIME;

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CGame::Uninit(void)
{
	//===================================
	//	　　UnLoadの破棄する場所
	//===================================
	CModel3D::ModelShaderDeleter();
	CModel3D::UnLoad();
	CMeshField::UnLoad();			//メッシュフィールドテクスチャの破棄
	CFade::UnLoad();				//フェードのテクスチャの破棄
	CParticle::UnLoad();			// パーティクルのテクスチャのロード
	CObject::UnLoad();				//オブジェクトのテクスチャの破棄
	CWall::UnLoad();				//壁のテクスチャの破
	CShadow::UnLoad();				//影のテクスチャの破棄
	CFeed::UnLoad();				//餌のテクスチャの破棄
	CEgg::UnLoad();					//卵のテクスチャの破棄
	CChick::UnLoad();				//ひよこのテクスチャの破棄

	CGameCharSelect::Unload();		// ゲーム（キャラ選択）
	CGamePlay::Unload();			// ゲーム（プレイ）

	//モデルの破棄
	CPlayer::Unload();

	CTime::Unload();
	CNumber::Unload();

	//ポーズ削除
	if (m_pPause != NULL)
	{// NULL以外
		m_pPause->Uninit();
		m_pPause = NULL;
	}

	// ゲーム（キャラ選択）
	if (m_pGameCharSelect != NULL)
	{// NULL以外
		m_pGameCharSelect->Uninit();
		m_pGameCharSelect = NULL;
	}
	// ゲーム（プレイ）
	if (m_pGamePlay != NULL)
	{// NULL以外
		m_pGamePlay->Uninit();
		m_pGamePlay = NULL;
	}

	////プレイヤーのモーションの破棄
	//if (m_pPlayerMotion != NULL)
	//{
	//	m_pPlayerMotion->Uninit();
	//	delete m_pPlayerMotion;
	//	m_pPlayerMotion = NULL;
	//}

	//カメラの破棄
	if (m_pCuorseCamera != NULL)
	{
		m_pCuorseCamera->Uninit();
		delete m_pCuorseCamera;
		m_pCuorseCamera = NULL;
	}

	for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
	{// メンバーカウント
		if (m_pPlayer[nCntMember] != NULL)
		{// NULL以外
			m_pPlayer[nCntMember]->Uninit();
			m_pPlayer[nCntMember] = NULL;
		}
	}

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// プレイヤーカウント
		if (m_pGameCamera[nCntPlayer] != NULL)
		{// NULL以外
			m_pGameCamera[nCntPlayer]->Uninit();
			delete m_pGameCamera[nCntPlayer];
			m_pGameCamera[nCntPlayer] = NULL;
		}
	}

	//マップの破棄
	CCOL_MESH_MANAGER::EndMap();

	//フェード以外の破棄
	CScene::NotFadeReleseAll();
}
//=============================================================================
// 更新処理
//=============================================================================
void CGame::Update(void)
{
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputJoyPad_0 * pXpad = CManager::GetInputJoyPad0(m_nControllerNum[0]);		//ジョイパットの取得
	CSound *pSound = CManager::GetSound();						//サウンドの情報
	CFade::FADE fade = CFade::GetFade();

	// 取得
	bool bOnine = CTitle::GetOnline();

	if (m_gameMode != m_gameModeNext)
	{// ゲームモードが変わった
		SetGameMode(m_gameModeNext);
		return;
	}

	switch (m_gameMode)
	{// ゲームモード
	case GAMEMODE_CHARSELECT:
		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
			if (m_pGameCamera[nCntPlayer] != NULL) { m_pGameCamera[nCntPlayer]->SetPlayer((m_pPlayer[m_nCharSelectNum[nCntPlayer]] != NULL ? m_pPlayer[m_nCharSelectNum[nCntPlayer]] : NULL)); }

		break;
	case GAMEMODE_COURSESELECT:
		//if (pCInputKeyBoard->GetKeyboardTrigger(DIK_RETURN) == true)
		//	CFade::Create(GAMEMODE_PLAY);

		break;
	case GAMEMODE_COURSE_VIEW:
		for (int nCntPlayer = 0; nCntPlayer < MAX_MEMBER; nCntPlayer++)
			if (m_pPlayer[nCntPlayer] != NULL)
				m_pPlayer[nCntPlayer]->SetControl(false);

		fade = CFade::GetFade(); 
		if (fade == CFade::FADE_NONE)
		{// フェードしていない
			if (m_bPause == false)
			{//ポーズしていない
				if (pCInputKeyBoard->GetKeyboardTrigger(DIK_RETURN) == true ||
					pCInputKeyBoard->GetKeyboardTrigger(DIK_Z) == true ||
					pXpad->GetTrigger(INPUT_START) == true)
					CFade::Create(CGame::GAMEMODE_PLAY);
			}
		}
		break;
	case GAMEMODE_PLAY:
		//現在の状態を保存
		m_NowGameState = GetGameState();

		switch (m_NowGameState)
		{// ゲーム状態
		case GAMESTATE_NORMAL:	//通常の状態
			if (m_nGameCounter < START_SET_TIME)
			{
				for (int nCntPlayer = 0; nCntPlayer < MAX_MEMBER; nCntPlayer++)
					if (m_pPlayer[nCntPlayer] != NULL)
						m_pPlayer[nCntPlayer]->SetControl(false);
			}
			else if (m_nGameCounter == START_SET_TIME)
			{
				for (int nCntPlayer = 0; nCntPlayer < MAX_MEMBER; nCntPlayer++)
					if (m_pPlayer[nCntPlayer] != NULL)
						m_pPlayer[nCntPlayer]->SetControl(true);
			}

			if (m_bPause == false)
				Ranking();		// ランキング

			break;
		case GAMESTATE_END:			//ゲーム終了状態
			m_nCounterGameState++;

			if (180 < m_nCounterGameState)
			{//画面（モード）の設定
				CFade::Create(CManager::MODE_RESULT);
			}
			break;
		}

		break;
	}

	fade = CFade::GetFade();
	if (fade == CFade::FADE_NONE)
	{// フェードしていない
		if (m_gameMode == GAMEMODE_COURSE_VIEW || (m_gameMode == GAMEMODE_PLAY && (START_SET_TIME) < m_nGameCounter))
		{// レース中
			if (bOnine == false)
			{// オンラインじゃない
			 //ポーズの処理
				if (pCInputKeyBoard->GetKeyboardTrigger(DIK_P) == true ||
					pXpad->GetTrigger(INPUT_START) == true)
				{//Pキーが押されたら
					m_bPause = m_bPause ? false : true;

					switch (m_bPause)
					{// ポーズ
					case true:
						if (m_pPause == NULL)
						{
							//ポーズを開く音
							//pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_OPEN);

							//ポーズの生成
							m_pPause = CPause::Create();

							//ポーズとフェードだけ回す
							CScene::SetUpdatePri(7);
						}
						break;
					case false:
						if (m_pPause != NULL)
						{
							//ポーズを閉じる音
							//pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_CLOSE);

							//ポーズを削除
							m_pPause->Uninit();
							m_pPause = NULL;

							//アップデート順番をすべて回す
							CScene::SetUpdatePri(0);
						}
					}
				}
			}
		}
	}

	if (m_bPause == false)
	{//開く音
		//カメラの更新処理
		if (m_pCuorseCamera != NULL) { m_pCuorseCamera->Update(); }

		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
			if (m_pGameCamera[nCntPlayer] != NULL) { m_pGameCamera[nCntPlayer]->Update(); }

		//	カウンター進める
		m_nGameCounter++;
	}

	//CDebugProc::Print("MaxPlayer:%d\n", m_nMaxPlayer);	// プレイヤー数
	//CDebugProc::Print("m_bPause:%d\n", m_bPause);		// ポーズ
}

//=============================================================================
// 描画処理
//=============================================================================
void CGame::Draw(void)
{
	// 取得
	bool bOnine = CTitle::GetOnline();

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_nCameraNumber = 0;

	if (m_gameMode == GAMEMODE_CHARSELECT)
	{// その他
		D3DVIEWPORT9 viewport;
		pDevice->GetViewport(&viewport);	// ビューポート取得

											// バックバッファ＆Ｚバッファのクリア
		pDevice->Clear(0,
			NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(157, 184, 224, 255),
			1.0f,
			0);

		m_nCameraNumber = -1;

		if (m_pCuorseCamera != NULL) { m_pCuorseCamera->SetCamera(); }

		//全ての描画
		CScene::DrawAll();

		pDevice->SetViewport(&viewport);	// ビューポート設定

		//２Dの描画
		CScene::DrawSeting(true);

		for (int nCntPlayer = 0; nCntPlayer < m_nMaxPlayer; nCntPlayer++)
		{// プレイヤーカウント
		 // バックバッファ＆Ｚバッファのクリア
			pDevice->Clear(0,
				NULL,
				(D3DCLEAR_ZBUFFER),
				D3DCOLOR_RGBA(157, 184, 224, 255),
				1.0f,
				0);

			m_nCameraNumber = nCntPlayer;

			//カメラの設定
			if (m_pGameCamera[nCntPlayer] != NULL) { m_pGameCamera[nCntPlayer]->SetCamera(); }

			//全ての描画
			CScene::DrawSeting(true, 0, true, 5);
		}

		pDevice->SetViewport(&viewport);	// ビューポート設定

											//２Dの描画
		CScene::DrawSeting(true, 6, true);
	}
	else if (m_gameMode == GAMEMODE_COURSE_VIEW)
	{// その他
	 // バックバッファ＆Ｚバッファのクリア
		pDevice->Clear(0,
			NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(157, 184, 224, 255),
			1.0f,
			0);

		m_nCameraNumber = -1;

		if (m_pCuorseCamera != NULL) { m_pCuorseCamera->SetCamera(); }

		//全ての描画
		CScene::DrawAll();
	}
	else if (m_gameMode == GAMEMODE_PLAY)
	{// プレイ
		D3DVIEWPORT9 viewport;
		pDevice->GetViewport(&viewport);	// ビューポート取得

											// バックバッファ＆Ｚバッファのクリア
		pDevice->Clear(0,
			NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(157, 184, 224, 255),
			1.0f,
			0);

		if (bOnine == true)
		{// オンライン
			int nClient = CClient::GetnID();
			m_nCameraNumber = nClient;
			if (m_pGameCamera[nClient] != NULL) { m_pGameCamera[nClient]->SetCamera(); }

			//全ての描画
			CScene::DrawAll();
		}
		else
		{// ローカル
			for (int nCntPlayer = 0; nCntPlayer < m_nMaxPlayer; nCntPlayer++)
			{// プレイヤーカウント
			 // バックバッファ＆Ｚバッファのクリア
				pDevice->Clear(0,
					NULL,
					(D3DCLEAR_ZBUFFER),
					D3DCOLOR_RGBA(157, 184, 224, 255),
					1.0f,
					0);

				m_nCameraNumber = nCntPlayer;

				//カメラの設定
				if (m_pGameCamera[nCntPlayer] != NULL) { m_pGameCamera[nCntPlayer]->SetCamera(); }

				//全ての描画
				CScene::DrawAll();
			}
		}

		pDevice->SetViewport(&viewport);	// ビューポート設定

		//2Dの描画
		CScene::DrawSeting(true, 5, true, 6);

		// バックバッファ＆Ｚバッファのクリア
		pDevice->Clear(0,
			NULL,
			(D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(157, 184, 224, 255),
			1.0f,
			0);

		m_nCameraNumber = -1;

		if (m_pCuorseCamera != NULL) { m_pCuorseCamera->SetCamera(); }

		CScene::DrawMap();

		pDevice->SetViewport(&viewport);	// ビューポート設定

		//2Dの描画
		CScene::DrawSeting(true, 7, true);
	}
	else
	{// その他
	 // バックバッファ＆Ｚバッファのクリア
		pDevice->Clear(0,
			NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
			D3DCOLOR_RGBA(157, 184, 224, 255),
			1.0f,
			0);

		//全ての描画
		CScene::DrawAll();
	}
}

//=============================================================================
// ポーズを押したかどうか
//=============================================================================
void CGame::SetPause(bool bPause)
{
	m_bPause = bPause;

	switch (m_bPause)
	{
	case true:
		if (m_pPause == NULL)
		{
			//ポーズの生成
			m_pPause = CPause::Create();

			CScene::SetUpdatePri(7);
		}
		break;
	case false:
		if (m_pPause != NULL)
		{
			m_pPause->Uninit();
			m_pPause = NULL;

			CScene::SetUpdatePri(0);
		}
	}
}

//=============================================================================
// ゲームモード設定
//=============================================================================
void CGame::SetGameMode(GAMEMODE gameMode)
{
	CSound *pSound = CManager::GetSound();						//サウンドの情報

	switch (m_gameMode)
	{// ゲームモード
	case GAMEMODE_CHARSELECT:		// キャラ選択
		if (m_pGameCharSelect != NULL)
		{// NULL以外
			m_pGameCharSelect->Uninit();
			m_pGameCharSelect = NULL;
		}

		if (m_pCuorseCamera != NULL)
		{
			m_pCuorseCamera->Uninit();
			delete m_pCuorseCamera;
			m_pCuorseCamera = NULL;
		}

		break;
	case GAMEMODE_COURSESELECT:		// コース選択		

		break;
	case GAMEMODE_COURSE_VIEW:		// コース見る		
		if (m_pCuorseCamera != NULL)
		{
			m_pCuorseCamera->Uninit();
			delete m_pCuorseCamera;
			m_pCuorseCamera = NULL;
		}

		break;
	case GAMEMODE_PLAY:				// プレイ
		if (m_pGamePlay != NULL)
		{// NULL以外
			m_pGamePlay->Uninit();
			m_pGamePlay = NULL;
		}

		if (m_pCuorseCamera != NULL)
		{
			m_pCuorseCamera->Uninit();
			delete m_pCuorseCamera;
			m_pCuorseCamera = NULL;
		}

		//フェード以外の破棄
		CScene::NotFadeReleseAll();
		break;
	}

	SetPlayer(false, m_gameMode);

	// ゲームモードを変更
	m_gameMode = gameMode;

	switch (m_gameMode)
	{// ゲームモード変更後
	case GAMEMODE_CHARSELECT:		// キャラ選択
		pSound->StopSound();
		//キャラクターセレクトの曲
		pSound->SetVolume(CSound::SOUND_LABEL_BGM_CHARACTERSERECT, 0.3f);
		pSound->PlaySound(CSound::SOUND_LABEL_BGM_CHARACTERSERECT);

		if (m_pGameCharSelect == NULL)
			m_pGameCharSelect = CGameCharSelect::Create();

		for (int nCntPlayer = 0; nCntPlayer < MAX_MEMBER; nCntPlayer++)
			m_nCharSelectNum[nCntPlayer] = nCntPlayer;			// キャラ選択番号

																//ゲームカメラの生成
		if (m_pCuorseCamera == NULL)
		{// NULL
			m_pCuorseCamera = new CGameCamera;

			if (m_pCuorseCamera != NULL)
			{// NULL以外
			 // 初期化処理
				m_pCuorseCamera->Init();
				// タイプ設定処理
				m_pCuorseCamera->SetType(CGameCamera::CAMERA_CHARSELECT);
			}
		}

		if (m_nCntSetStage == 0)
			SetStage();

		break;
	case GAMEMODE_COURSESELECT:		// コース選択

		break;
	case GAMEMODE_COURSE_VIEW:		// コース見る
		pSound->StopSound();
		//タイトルの曲
		pSound->SetVolume(CSound::SOUND_LABEL_BGM_GAME, 0.4f);
		pSound->PlaySound(CSound::SOUND_LABEL_BGM_GAME);

		//ゲームカメラの生成
		if (m_pCuorseCamera == NULL)
		{// NULL
			m_pCuorseCamera = new CGameCamera;

			if (m_pCuorseCamera != NULL)
			{// NULL以外
			 // 初期化処理
				m_pCuorseCamera->Init();
				// タイプ設定処理
				m_pCuorseCamera->SetType(CGameCamera::CAMERA_COURSE);
			}
		}

		if (m_nCntSetStage == 0)
			SetStage();

		break;
	case GAMEMODE_PLAY:				// プレイ
		pSound->StopSound();
		//タイトルの曲
		pSound->SetVolume(CSound::SOUND_LABEL_BGM_GAME, 0.4f);
		pSound->PlaySound(CSound::SOUND_LABEL_BGM_GAME);

		if (m_pGamePlay == NULL)
			m_pGamePlay = CGamePlay::Create();

		// 取得
		bool bOnine = CTitle::GetOnline();

		//ゲームカメラの生成
		if (m_pCuorseCamera == NULL)
		{// NULL
			m_pCuorseCamera = new CGameCamera;

			if (m_pCuorseCamera != NULL)
			{// NULL以外
			 // 初期化処理
				m_pCuorseCamera->Init();
				// タイプ設定処理
				m_pCuorseCamera->SetType(CGameCamera::CAMERA_NONE);
				m_pCuorseCamera->SetCameraPosR(D3DXVECTOR3(-11004.0f, -3000.0f, -799.0f));
				m_pCuorseCamera->SetCameraPosV(D3DXVECTOR3(-11000.0f, 54000.0f, -800.0f));
				m_pCuorseCamera->GetfCameraAngle() = 25.0f;
				// ビューポート
				if (m_nMaxPlayer == 1 || m_nMaxPlayer == 2 || bOnine == true)
				{
					m_pCuorseCamera->SetViewPort(
						(DWORD)((SCREEN_WIDTH * 1.0f) - (SCREEN_HEIGHT * 0.25f)),
						(DWORD)((SCREEN_HEIGHT * 0.5f) - (SCREEN_HEIGHT * 0.25f)),
						(DWORD)((SCREEN_HEIGHT * 0.25f)),
						(DWORD)((SCREEN_HEIGHT * 0.5f))
					);
				}
				else if (m_nMaxPlayer == 3)
				{
					m_pCuorseCamera->SetViewPort(
						(DWORD)((SCREEN_WIDTH * 0.75f) - (SCREEN_HEIGHT * 0.125f)),
						(DWORD)((SCREEN_HEIGHT * 0.75f) - (SCREEN_HEIGHT * 0.25f)),
						(DWORD)((SCREEN_HEIGHT * 0.25f)),
						(DWORD)((SCREEN_HEIGHT * 0.5f))
					);
				}
				else
				{
					m_pCuorseCamera->SetViewPort(
						(DWORD)((SCREEN_WIDTH * 0.5f) - (SCREEN_HEIGHT * 0.125f)),
						(DWORD)((SCREEN_HEIGHT * 0.5f) - (SCREEN_HEIGHT * 0.25f)),
						(DWORD)((SCREEN_HEIGHT * 0.25f)),
						(DWORD)((SCREEN_HEIGHT * 0.5f))
					);
				}
			}
		}

		if (m_nCntSetStage == 0)
			SetStage();

		break;
	}

	SetPlayer(true, m_gameMode);

	m_nGameCounter = 0;
}

//=============================================================================
// ステージ設定
//=============================================================================
void CGame::SetStage(void)
{
	//サウンドのポインタ
	CSound *pSound = CManager::GetSound();

	if (m_nCntSetStage == 0)
	{
		//=====================================================================
		//	ステージ1
		//=====================================================================

		//マップを読み込む
		TextLoad(6);
		WallTextLoad(6);
		MeshTextLoad(6);
		FeedTextLoad();

		//===================================
		//		Create
		//===================================

		CCOL_MESH_MANAGER::LoadMap();

		for (int nCount = 0; nCount < m_nSetObjectNum; nCount++)
		{
			//オブジェクトの生成
			CObject::Create(m_Map[nCount].m_pos, m_Map[nCount].m_rot, m_Map[nCount].m_scale, 0.0f, m_Map[nCount].m_nTexType, m_Map[nCount].m_nType, m_Map[nCount].m_nCollision);
		}
		for (int nCount = 0; nCount < m_nSetFeedNum; nCount++)
		{
			//食べ物の生成		
			CFeed::Create(m_aFeed[nCount].m_pos, m_aFeed[nCount].m_nZone, m_aFeed[nCount].m_nType);
		}

		m_nCntSetStage = 1;
	}
}

//=============================================================================
// プレイヤー設定
//=============================================================================
void CGame::SetPlayer(bool bCreate, int nMode)
{
	// 取得
	bool bOnine = CTitle::GetOnline();

	if (bCreate == false)
	{
		switch (nMode)
		{
		case GAMEMODE_CHARSELECT:		// キャラ選択
			// キャラ入れ替え
			for (int nCntMember = m_nMaxPlayer; nCntMember < MAX_MEMBER; nCntMember++)
			{// メンバーカウント
				bool bCheck = false;
				do
				{
					bCheck = false;
					for (int nCntPlayer = 0; nCntPlayer < MAX_MEMBER; nCntPlayer++)
					{// プレイヤーカウント
						if (nCntMember != nCntPlayer)
						{
							if (m_nCharSelectNum[nCntMember] == m_nCharSelectNum[nCntPlayer])
							{
								m_nCharSelectNum[nCntMember] = (m_nCharSelectNum[nCntMember] + 1) % MAX_MEMBER;
								bCheck = true;
								break;
							}
						}
					}
				} while (bCheck != false);
			}
		case GAMEMODE_COURSESELECT:		// コース選択
		case GAMEMODE_COURSE_VIEW:		// コースビュー選択
		case GAMEMODE_PLAY:				// プレイ選択
			for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
			{// メンバーカウント
				if (m_pPlayer[nCntMember] != NULL)
				{// NULL以外
					m_pPlayer[nCntMember]->Uninit();
					m_pPlayer[nCntMember] = NULL;
				}
			}

			for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
			{// プレイヤーカウント
				if (m_pGameCamera[nCntPlayer] != NULL)
				{// NULL以外
					m_pGameCamera[nCntPlayer]->Uninit();
					delete m_pGameCamera[nCntPlayer];
					m_pGameCamera[nCntPlayer] = NULL;
				}
			}
			break;
		}
	}
	else
	{
		float frot = (-D3DX_PI * 0.5f);

		switch (nMode)
		{
		case GAMEMODE_CHARSELECT:		// キャラ選択
			for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
			{// メンバーカウント
			 //プレイヤーの生成
				if (m_pPlayer[nCntMember] == NULL)
					m_pPlayer[nCntMember] = CPlayer::Create(
						D3DXVECTOR3(
						((-250.0f + (50.0f * (nCntMember / 4))) + 2950.0f),
							(-90.0f),
							(210.0f - (((70.0f * 2.0f) / 3.0f) * (nCntMember % 4)))),
						D3DXVECTOR3(0.0f, frot, 0.0f),
						nCntMember, 0, m_nCharSelectNum[nCntMember], CPlayer::PLAYERTYPE_SELECT);
			}

			for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
			{// プレイヤーカウント
				if (m_pGameCamera[nCntPlayer] == NULL)
				{// NULL
					m_pGameCamera[nCntPlayer] = new CGameCamera;

					if (m_pGameCamera[nCntPlayer] != NULL)
					{// NULL以外
					 // 初期化処理
						m_pGameCamera[nCntPlayer]->Init();
						// タイプ設定処理
						m_pGameCamera[nCntPlayer]->SetType(CGameCamera::CAMERA_CHARUP);

						// ビューポート設定
						m_pGameCamera[nCntPlayer]->SetViewPort(
							(DWORD)(nCntPlayer % 2 == 0 ? 0.0f + (SCREEN_HEIGHT * ((0.5f - 0.47f) * 0.5f)) : (SCREEN_WIDTH - (SCREEN_HEIGHT * 0.47f)) - (SCREEN_HEIGHT * ((0.5f - 0.47f) * 0.5f))),
							(DWORD)(nCntPlayer / 2 == 0 ? 0.0f + (SCREEN_HEIGHT * ((0.5f - 0.47f) * 0.5f)) : (SCREEN_HEIGHT - (SCREEN_HEIGHT * 0.47f)) - (SCREEN_HEIGHT * ((0.5f - 0.47f) * 0.5f))),
							(DWORD)(SCREEN_HEIGHT * 0.47f),
							(DWORD)(SCREEN_HEIGHT * 0.47f)
						);
					}
				}
			}
			break;
		case GAMEMODE_COURSESELECT:		// コース選択
			break;
		case GAMEMODE_COURSE_VIEW:		// コースビュー選択
		case GAMEMODE_PLAY:				// プレイ選択
			for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
			{// メンバーカウント
			 //プレイヤーの生成
				if (m_pPlayer[nCntMember] == NULL)
					m_pPlayer[nCntMember] = CPlayer::Create(
						D3DXVECTOR3(-250.0f + (100.0f * (nCntMember / 4)), -90.0f, -100.0f + (((70.0f * 2.0f) / 3.0f) * (nCntMember % 4)) + (35.0f * (nCntMember / 4))),
						D3DXVECTOR3(0.0f, frot, 0.0f),
						nCntMember, (nCntMember < m_nMaxPlayer ? m_nControllerNum[nCntMember] : 0), m_nCharSelectNum[nCntMember], (nCntMember < m_nMaxPlayer ? CPlayer::PLAYERTYPE_PLAYER : CPlayer::PLAYERTYPE_ENEMY));

				if (nMode == GAMEMODE_PLAY)
				{
					if (nCntMember < m_nMaxPlayer)
					{// プレイヤー		
						if (m_pPlayer[nCntMember] != NULL)
						{// NULL以外
						 //ゲームカメラの生成
							if (m_pGameCamera[nCntMember] == NULL)
							{// NULL
								m_pGameCamera[nCntMember] = new CGameCamera;

								if (m_pGameCamera[nCntMember] != NULL)
								{// NULL以外
								 // 初期化処理
									m_pGameCamera[nCntMember]->Init();
									// タイプ設定処理
									m_pGameCamera[nCntMember]->SetType(CGameCamera::CAMERA_PLAYER);
									// 追従プレイヤー設定
									m_pGameCamera[nCntMember]->SetPlayer(m_pPlayer[nCntMember]);

									// ビューポート設定
									if (bOnine == true)
									{// オンライン
										m_pGameCamera[nCntMember]->SetViewPort(
											(DWORD)(0.0f),
											(DWORD)(0.0f),
											(DWORD)((SCREEN_WIDTH * 1.0f)),
											(DWORD)((SCREEN_HEIGHT * 1.0f))
										);
									}
									else
									{// ローカル
										m_pGameCamera[nCntMember]->SetViewPort(
											// 横
											//(DWORD)((SCREEN_WIDTH * 0.5f) * ((nCntMember) % 2)), 
											//(DWORD)((SCREEN_HEIGHT * 0.5f) * ((nCntMember) / 2)), 
											//(DWORD)((SCREEN_WIDTH * ((m_nMaxPlayer - 1) == 0 ? 1.0f : 0.5f))),
											//(DWORD)((SCREEN_HEIGHT * ((m_nMaxPlayer - 1) / 2 == 0 ? 1.0f : 0.5f)))
											// 縦
											//(DWORD)((SCREEN_WIDTH * 0.5f) * ((nCntMember) / 2)),
											//(DWORD)((SCREEN_HEIGHT * 0.5f) * ((nCntMember) % 2)),
											//(DWORD)((SCREEN_WIDTH * ((m_nMaxPlayer - 1) / 2 == 0 ? 1.0f : 0.5f))),
											//(DWORD)((SCREEN_HEIGHT * ((m_nMaxPlayer - 1) == 0 ? 1.0f : 0.5f)))
											// 縦->横
											(DWORD)((SCREEN_WIDTH * 0.5f) * ((m_nMaxPlayer - 1) / 2 == 0 ? ((nCntMember) / 2) : ((nCntMember) % 2))),
											(DWORD)((SCREEN_HEIGHT * 0.5f) * ((m_nMaxPlayer - 1) / 2 == 0 ? ((nCntMember) % 2) : ((nCntMember) / 2))),
											(DWORD)((SCREEN_WIDTH * ((m_nMaxPlayer - 1) / 2 == 0 ? 1.0f : 0.5f))),
											(DWORD)((SCREEN_HEIGHT * ((m_nMaxPlayer - 1) == 0 ? 1.0f : 0.5f)))
										);
									}

									// カメラ判定
									m_pGameCamera[nCntMember]->SetColl(true);
								}
							}
						}
					}
				}
			}
			break;
		}
	}
}

//===============================================================================
// ランキング
//===============================================================================
void CGame::Ranking(void)
{
	CSound *pSound = CManager::GetSound();						//サウンドの情報

	int	nGoulNum = 0;						// ゴール数

	for (int nCount = 0; nCount < MAX_MEMBER; nCount++)
	{// プレイヤーカウント
	 // ランキング
		m_nRanking[nCount] = CRoad_Manager::GetManager()->GetRank(nCount) - 1;

		if (m_pPlayer[nCount] != NULL)
		{// NULL以外
			if (nCount < m_nMaxPlayer)
			{
				if (m_pPlayer[nCount]->GetPlayerType() == CPlayer::PLAYERTYPE_PLAYER &&
					m_pPlayer[nCount]->GetGoal() == true)
				{// プレイヤーがゴールした
					m_bGoul[nCount] = true;		// ゴール
					nGoulNum++;					// ゴール人数
				}
			}

			if (START_SET_TIME <= m_nGameCounter)
				if (m_pPlayer[nCount]->GetGoal() == false)
					m_nTime[nCount]++;
		}
		//CDebugProc::Print("プレイヤー番号 [%d] : 順位 [%d]\n", nCount, m_nRanking[nCount]);
	}

	if (nGoulNum == m_nMaxPlayer)
	{// 全てゴールした
		SetGameState(GAMESTATE_END);

		for (int nCount = 0; nCount < MAX_MEMBER; nCount++)
		{// プレイヤーカウント
			for (int nCntCheck = 0; nCntCheck < MAX_MEMBER; nCntCheck++)
				if (m_nRanking[nCntCheck] == nCount)
				{
					m_nRankingSort[nCount] = nCntCheck;
					break;
				}
		}

		return;
	}
}

//===============================================================================
// ファイルからロード
//===============================================================================
void CGame::TextLoad(int nLoadNumber)
{
	//ファイル用変数
	FILE *pFile;		//ファイルポインタ
	char *pStrcur;		//現在の先頭の文字列
	char aLine[256];	//文字列
	char aStr[256];		//一時保存文字列
	int nIndex = 0;		//現在のインデックス
	int nWord = 0;		//ポップで返された値を保持

						//ファイルポインタの初期化処理
	pFile = NULL;

	//ファイルを開く 読み込み
	pFile = fopen(TEXT_OBJECTNAME1, "r");

	//NULLチェック
	if (pFile != NULL)
	{
		//文字列の先頭を設定
		pStrcur = ReadLine(pFile, &aLine[0]);
		//文字列を取り出す
		strcpy(aStr, pStrcur);

		//文字列のデータ 比較する文字列 比較する文字数
		if (memcmp(pStrcur, "OBJECT_SETNUM = ", strlen("OBJECT_SETNUM = ")) == 0)
		{
			//頭出し
			pStrcur += strlen("OBJECT_SETNUM = ");
			//文字列の先頭を設定
			strcpy(aStr, pStrcur);
			//文字列抜き出し
			m_nSetObjectNum = atoi(pStrcur);
		}

		//オブジェクトの数分回す
		for (int nCntObject = 0; nCntObject < m_nSetObjectNum; nCntObject++)
		{
			//文字列の先頭を設定
			pStrcur = ReadLine(pFile, &aLine[0]);
			//文字列を取り出す
			strcpy(aStr, pStrcur);

			if (memcmp(pStrcur, "OBJECT_START", strlen("OBJECT_START")) == 0)
			{
				while (1)
				{
					//文字列の先頭を設定
					pStrcur = ReadLine(pFile, &aLine[0]);

					//種類のを読み込み
					if (memcmp(pStrcur, "TYPE = ", strlen("TYPE = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("TYPE = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						m_Map[nCntObject].m_nType = atoi(pStrcur);
					}
					//種類のを読み込み
					if (memcmp(pStrcur, "TEXTURETYPE = ", strlen("TEXTURETYPE = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("TEXTURETYPE = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						m_Map[nCntObject].m_nTexType = atoi(pStrcur);
					}
					//当たり判定を読み込み
					if (memcmp(pStrcur, "COLLISION = ", strlen("COLLISION = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("COLLISION = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						m_Map[nCntObject].m_nCollision = atoi(pStrcur);
					}
					//POSを読み込み
					if (memcmp(pStrcur, "POS = ", strlen("POS = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("POS = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);

						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						m_Map[nCntObject].m_pos.x = (float)atof(pStrcur);
						//文字数分進める
						pStrcur += nWord;

						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						m_Map[nCntObject].m_pos.y = (float)atof(pStrcur);
						//文字数分進める
						pStrcur += nWord;

						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						m_Map[nCntObject].m_pos.z = (float)atof(pStrcur);

					}
					//ROTを読み込み
					if (memcmp(pStrcur, "ROT = ", strlen("ROT = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("ROT = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);

						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						m_Map[nCntObject].m_rot.x = (float)atof(pStrcur);

						//文字数分進める
						pStrcur += nWord;
						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						m_Map[nCntObject].m_rot.y = (float)atof(pStrcur);

						//文字数分進める
						pStrcur += nWord;
						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						m_Map[nCntObject].m_rot.z = (float)atof(pStrcur);
					}
					//SIZを読み込み
					if (memcmp(pStrcur, "SIZ = ", strlen("SIZ = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("SIZ = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);

						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						m_Map[nCntObject].m_scale.x = (float)atof(pStrcur);

						//文字数分進める
						pStrcur += nWord;
						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						m_Map[nCntObject].m_scale.y = (float)atof(pStrcur);

						//文字数分進める
						pStrcur += nWord;
						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						m_Map[nCntObject].m_scale.z = (float)atof(pStrcur);
					}
					else if (memcmp(pStrcur, "OBJECT_END", strlen("OBJECT_END")) == 0)
					{
						break;
					}
				}
			}
		}
	}
}
//===============================================================================
// メッシュフィールドをファイルからロード
//===============================================================================
void CGame::MeshTextLoad(int nLoadNumber)
{
	//ファイル用変数
	FILE *pFile;		//ファイルポインタ
	char *pStrcur;		//現在の先頭の文字列
	char aLine[256];	//文字列
	char aStr[256];		//一時保存文字列
	int nIndex = 0;		//現在のインデックス
	int nWord = 0;		//ポップで返された値を保持

						//ファイルポインタの初期化処理
	pFile = NULL;

	//ファイルを開く 読み込み
	pFile = fopen(TEXT_MESHFIELDNAME1, "r");

	//NULLチェック
	if (pFile != NULL)
	{
		//文字列の先頭を設定
		pStrcur = ReadLine(pFile, &aLine[0]);
		//文字列を取り出す
		strcpy(aStr, pStrcur);

		//文字列のデータ 比較する文字列 比較する文字数
		if (memcmp(pStrcur, "MESHFIELD_SETNUM = ", strlen("MESHFIELD_SETNUM = ")) == 0)
		{
			//頭出し
			pStrcur += strlen("MESHFIELD_SETNUM = ");
			//文字列の先頭を設定
			strcpy(aStr, pStrcur);
			//文字列抜き出し
			m_nSetMeshFieldNum = atoi(pStrcur);
		}

		//オブジェクトの数分回す
		for (int nCntObject = 0; nCntObject < m_nSetMeshFieldNum; nCntObject++)
		{
			//文字列の先頭を設定
			pStrcur = ReadLine(pFile, &aLine[0]);
			//文字列を取り出す
			strcpy(aStr, pStrcur);

			if (memcmp(pStrcur, "MESHFIELD_START", strlen("MESHFIELD_START")) == 0)
			{
				while (1)
				{
					//文字列の先頭を設定
					pStrcur = ReadLine(pFile, &aLine[0]);

					//種類のを読み込み
					if (memcmp(pStrcur, "TEXTURETYPE = ", strlen("TEXTURETYPE = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("TEXTURETYPE = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						m_Mesh[nCntObject].m_nTexType = atoi(pStrcur);
					}
					//横の分割数を読み込み
					if (memcmp(pStrcur, "X_DIVIDE = ", strlen("X_DIVIDE = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("X_DIVIDE = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						m_Mesh[nCntObject].m_nWidthDivide = atoi(pStrcur);
					}
					//縦の分割数を読み込み
					if (memcmp(pStrcur, "Z_DIVIDE = ", strlen("Z_DIVIDE = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("Z_DIVIDE = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						m_Mesh[nCntObject].m_nDepthDivide = atoi(pStrcur);
					}
					//横のテクスチャ座標を読み込み
					if (memcmp(pStrcur, "X_TEXUV = ", strlen("X_TEXUV = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("X_TEXUV = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						m_Mesh[nCntObject].m_fTexXUV = (float)atoi(pStrcur);
					}
					//縦のテクスチャ座標を読み込み
					if (memcmp(pStrcur, "Y_TEXUV = ", strlen("Y_TEXUV = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("Y_TEXUV = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						m_Mesh[nCntObject].m_fTexYUV = (float)atoi(pStrcur);
					}
					//横の長さを読み込み
					if (memcmp(pStrcur, "X_LENGTH = ", strlen("X_LENGTH = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("X_LENGTH = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						m_Mesh[nCntObject].m_fWidthLength = (float)atoi(pStrcur);
					}
					//縦の長さを読み込み
					if (memcmp(pStrcur, "Z_LENGTH = ", strlen("Z_LENGTH = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("Z_LENGTH = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						m_Mesh[nCntObject].m_fDepthLength = (float)atoi(pStrcur);
					}
					//１頂点の高さ
					if (memcmp(pStrcur, "VTX0_HEIGHT = ", strlen("VTX0_HEIGHT = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("VTX0_HEIGHT = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						m_Mesh[nCntObject].m_fVtxHeight_No0 = (float)atoi(pStrcur);
					}
					//２頂点の高さ
					if (memcmp(pStrcur, "VTX1_HEIGHT = ", strlen("VTX1_HEIGHT = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("VTX1_HEIGHT = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						m_Mesh[nCntObject].m_fVtxHeight_No1 = (float)atoi(pStrcur);
					}
					//３頂点の高さ
					if (memcmp(pStrcur, "VTX2_HEIGHT = ", strlen("VTX2_HEIGHT = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("VTX2_HEIGHT = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						m_Mesh[nCntObject].m_fVtxHeight_No2 = (float)atoi(pStrcur);
					}
					//４頂点の高さ
					if (memcmp(pStrcur, "VTX3_HEIGHT = ", strlen("VTX3_HEIGHT = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("VTX3_HEIGHT = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						m_Mesh[nCntObject].m_fVtxHeight_No3 = (float)atoi(pStrcur);
					}
					//１頂点の高さ
					if (memcmp(pStrcur, "VTX0_SIDE = ", strlen("VTX0_SIDE = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("VTX0_SIDE = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						m_Mesh[nCntObject].m_fVtxSide_No0 = (float)atoi(pStrcur);
					}
					//２頂点の高さ
					if (memcmp(pStrcur, "VTX1_SIDE = ", strlen("VTX1_SIDE = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("VTX1_SIDE = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						m_Mesh[nCntObject].m_fVtxSide_No1 = (float)atoi(pStrcur);
					}
					//３頂点の高さ
					if (memcmp(pStrcur, "VTX2_SIDE = ", strlen("VTX2_SIDE = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("VTX2_SIDE = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						m_Mesh[nCntObject].m_fVtxSide_No2 = (float)atoi(pStrcur);
					}
					//４頂点の高さ
					if (memcmp(pStrcur, "VTX3_SIDE = ", strlen("VTX3_SIDE = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("VTX3_SIDE = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						m_Mesh[nCntObject].m_fVtxSide_No3 = (float)atoi(pStrcur);
					}
					//POSを読み込み
					if (memcmp(pStrcur, "POS = ", strlen("POS = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("POS = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);

						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						m_Mesh[nCntObject].m_pos.x = (float)atof(pStrcur);
						//文字数分進める
						pStrcur += nWord;

						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						m_Mesh[nCntObject].m_pos.y = (float)atof(pStrcur);
						//文字数分進める
						pStrcur += nWord;

						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						m_Mesh[nCntObject].m_pos.z = (float)atof(pStrcur);

					}
					else if (memcmp(pStrcur, "MESHFIELD_END", strlen("MESHFIELD_END")) == 0)
					{
						break;
					}
				}
			}
		}
	}
}

//===============================================================================
// 壁をファイルからロード
//===============================================================================
void CGame::WallTextLoad(int nLoadNumber)
{
	//ファイル用変数
	FILE *pFile;		//ファイルポインタ
	char *pStrcur;		//現在の先頭の文字列
	char aLine[256];	//文字列
	char aStr[256];		//一時保存文字列
	int nIndex = 0;		//現在のインデックス
	int nWord = 0;		//ポップで返された値を保持

						//ファイルポインタの初期化処理
	pFile = NULL;

	//ファイルを開く 読み込み
	pFile = fopen(TEXT_WALLNAME1, "r");

	//NULLチェック
	if (pFile != NULL)
	{
		//文字列の先頭を設定
		pStrcur = ReadLine(pFile, &aLine[0]);
		//文字列を取り出す
		strcpy(aStr, pStrcur);

		//文字列のデータ 比較する文字列 比較する文字数
		if (memcmp(pStrcur, "WALL_SETNUM = ", strlen("WALL_SETNUM = ")) == 0)
		{
			//頭出し
			pStrcur += strlen("WALL_SETNUM = ");
			//文字列の先頭を設定
			strcpy(aStr, pStrcur);
			//文字列抜き出し
			m_nSetWallNum = atoi(pStrcur);
		}

		//オブジェクトの数分回す
		for (int nCntObject = 0; nCntObject < m_nSetWallNum; nCntObject++)
		{
			//文字列の先頭を設定
			pStrcur = ReadLine(pFile, &aLine[0]);
			//文字列を取り出す
			strcpy(aStr, pStrcur);

			if (memcmp(pStrcur, "WALL_START", strlen("WALL_START")) == 0)
			{
				while (1)
				{
					//文字列の先頭を設定
					pStrcur = ReadLine(pFile, &aLine[0]);

					//POSを読み込み
					if (memcmp(pStrcur, "POS = ", strlen("POS = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("POS = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);

						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						m_aWall[nCntObject].m_pos.x = (float)atof(pStrcur);
						//文字数分進める
						pStrcur += nWord;

						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						m_aWall[nCntObject].m_pos.y = (float)atof(pStrcur);
						//文字数分進める
						pStrcur += nWord;

						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						m_aWall[nCntObject].m_pos.z = (float)atof(pStrcur);

					}
					//ROTを読み込み
					if (memcmp(pStrcur, "ROT = ", strlen("ROT = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("ROT = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);

						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						m_aWall[nCntObject].m_rot.x = (float)atof(pStrcur);
						//文字数分進める
						pStrcur += nWord;

						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						m_aWall[nCntObject].m_rot.y = (float)atof(pStrcur);
						//文字数分進める
						pStrcur += nWord;

						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						m_aWall[nCntObject].m_rot.z = (float)atof(pStrcur);

					}
					//WIDTHを読み込み
					if (memcmp(pStrcur, "WIDTH = ", strlen("WIDTH = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("WIDTH = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);

						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						m_aWall[nCntObject].m_fWidthDivide = (float)atof(pStrcur);
					}
					//HIGHTを読み込み
					if (memcmp(pStrcur, "HIGHT = ", strlen("HIGHT = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("HIGHT = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);

						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						m_aWall[nCntObject].m_fHightDivide = (float)atof(pStrcur);
					}
					//HIGHTを読み込み
					if (memcmp(pStrcur, "TEXTURE = ", strlen("TEXTURE = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("TEXTURE = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);

						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						m_aWall[nCntObject].m_nTexType = (int)atof(pStrcur);
					}
					else if (memcmp(pStrcur, "WALL_END", strlen("WALL_END")) == 0)
					{
						break;
					}
				}
			}
		}
	}
}
//===============================================================================
// 食べ物をファイルからロード
//===============================================================================
void CGame::FeedTextLoad(void)
{
	//ファイル用変数
	FILE *pFile;		//ファイルポインタ
	char *pStrcur;		//現在の先頭の文字列
	char aLine[256];	//文字列
	char aStr[256];		//一時保存文字列
	int nIndex = 0;		//現在のインデックス
	int nWord = 0;		//ポップで返された値を保持

						//ファイルを開く 読み込み
	pFile = fopen(TEXT_FEEDNAME, "r");

	//NULLチェック
	if (pFile != NULL)
	{
		//文字列の先頭を設定
		pStrcur = ReadLine(pFile, &aLine[0]);
		//文字列を取り出す
		strcpy(aStr, pStrcur);

		//文字列のデータ 比較する文字列 比較する文字数
		if (memcmp(pStrcur, "FEED_SETNUM = ", strlen("FEED_SETNUM = ")) == 0)
		{
			//頭出し
			pStrcur += strlen("FEED_SETNUM = ");
			//文字列の先頭を設定
			strcpy(aStr, pStrcur);
			//文字列抜き出し
			m_nSetFeedNum = atoi(pStrcur);
		}

		//オブジェクトの数分回す
		for (int nCntObject = 0; nCntObject < m_nSetFeedNum; nCntObject++)
		{
			//文字列の先頭を設定
			pStrcur = ReadLine(pFile, &aLine[0]);
			//文字列を取り出す
			strcpy(aStr, pStrcur);

			if (memcmp(pStrcur, "FEED_START", strlen("FEED_START")) == 0)
			{
				while (1)
				{
					//文字列の先頭を設定
					pStrcur = ReadLine(pFile, &aLine[0]);

					//POSを読み込み
					if (memcmp(pStrcur, "POS = ", strlen("POS = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("POS = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);

						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						m_aFeed[nCntObject].m_pos.x = (float)atof(pStrcur);
						//文字数分進める
						pStrcur += nWord;

						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						m_aFeed[nCntObject].m_pos.y = (float)atof(pStrcur);
						//文字数分進める
						pStrcur += nWord;

						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						m_aFeed[nCntObject].m_pos.z = (float)atof(pStrcur);

					}
					//ZONEを読み込み
					if (memcmp(pStrcur, "ZONE = ", strlen("ZONE = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("ZONE = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						m_aFeed[nCntObject].m_nZone = atoi(pStrcur);
					}
					//TYPEを読み込み
					if (memcmp(pStrcur, "TYPE = ", strlen("TYPE = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("TYPE = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						m_aFeed[nCntObject].m_nType = atoi(pStrcur);
					}
					else if (memcmp(pStrcur, "FEED_END", strlen("FEED_END")) == 0)
					{
						break;
					}
				}
			}
		}
	}

	//m_pMarkFeed->SetFeedNum(CMarkFeed::GetFeedNum() + m_nSetFeedNum);
}

//=============================================================================
//　ファイル読み込み無効文を排除
//=============================================================================
char *CGame::ReadLine(FILE *pFile, char *pDst)
{
	while (1)
	{
		//１行分読み込み
		fgets(&pDst[0], 256, pFile);

		//文字列のデータ 比較する文字列 比較する文字数
		if (memcmp(pDst, "#", strlen("#")) == 0)
		{
			pDst += strlen("\n");
		}
		//文字列のデータ 比較する文字列 比較する文字数
		else if (memcmp(pDst, "\t", strlen("\t")) == 0)
		{
			pDst += strlen("\t");
			while (1)
			{
				if (memcmp(pDst, "\t", strlen("\t")) == 0)
				{
					pDst += strlen("\t");
				}
				else
				{
					break;
				}
			}
			break;
		}
		//文字列のデータ 比較する文字列 比較する文字数
		else if (memcmp(pDst, " ", strlen(" ")) == 0)
		{
			pDst += strlen(" ");
			while (1)
			{
				if (memcmp(pDst, " ", strlen(" ")) == 0)
				{
					pDst += strlen(" ");
				}
				else
				{
					break;
				}
				break;
			}
		}
		//文字列のデータ 比較する文字列 比較する文字数
		else if (memcmp(pDst, "\n", strlen("\n")) == 0)
		{
			pDst += strlen("\n");
		}
		else
		{
			break;
		}
	}
	return pDst;
}

//=============================================================================
//　ファイル読み込み先頭を排除
//=============================================================================
char *CGame::GetLineTop(char * pStr)
{
	while (1)
	{
		//文字列のデータ 比較する文字列 比較する文字数
		if (memcmp(pStr, " ", strlen(" ")) == 0)
		{
			pStr += strlen(" ");
			while (1)
			{
				if (memcmp(pStr, " ", strlen(" ")) == 0)
				{
					pStr += strlen(" ");
				}
				else
				{
					break;
				}
				break;
			}
		}
		//文字列のデータ 比較する文字列 比較する文字数
		else if (memcmp(pStr, "\t", strlen("\t")) == 0)
		{
			pStr += strlen("\t");
			while (1)
			{
				if (memcmp(pStr, "\t", strlen("\t")) == 0)
				{
					pStr += strlen("\t");
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			break;
		}
	}
	return pStr;
}

//=============================================================================
//　文字数を返す
//=============================================================================
int CGame::PopString(char * pStr, char * pDest)
{
	int nWord = 0;

	while (1)
	{	//頭出し
		pStr += 1;
		nWord += 1;
		if (memcmp(pStr, " ", strlen(" ")) == 0)
		{	//頭出し
			pStr = "\0";
			nWord += 1;
			break;
		}
		if (memcmp(pStr, "\t", strlen("\t")) == 0)
		{	//頭出し
			pStr = "\0";
			nWord += (int)strlen("\t");
			break;
		}
		//文字列のデータ 比較する文字列 比較する文字数
		else if (memcmp(pStr, "\n", strlen("\n")) == 0)
		{
			//頭出し
			nWord += (int)strlen("\n");
			break;
		}
	}
	strcpy(pDest, pStr);
	//文字列の数を返す
	return nWord;
}