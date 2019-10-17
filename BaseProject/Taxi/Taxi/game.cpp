//=============================================================================
//
// ゲームの処理 [game.cpp]
// Author : 有馬　武志
//
//=============================================================================
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

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXT_OBJECTNAME1		"data\\TEXT\\ステージ6\\objecy.txt"			// 読み込むテキストファイル
#define TEXT_MESHFIELDNAME1		"data\\TEXT\\ステージ6\\meshfield.txt"		// 読み込むテキストファイル
#define TEXT_WALLNAME1			"data\\TEXT\\ステージ6\\wall.txt"			// 読み込むテキストファイル
#define TEXT_PLAYER_MOTION		"data\\TEXT\\Player\\Player.txt"			// プレイヤーのモーションファイル
#define TEXT_BOY_MOTION			"data\\TEXT\\お客さん\\motion_boy.txt"		// 男性モーションファイル
#define TEXT_GIRL_MOTION		"data\\TEXT\\お客さん\\motion_girl.txt"		// 女性モーションファイル
#define TEXT_CUSTOMER_SUMMER	"data\\TEXT\\お客さん\\Summer2.txt"			// お客さんの情報（夏）
#define TEXT_CUSTOMER_HARD		"data\\TEXT\\お客さん\\Summer.txt"			// お客さんの情報 (ハード)
#define TEXT_HUMAN_NPC			"data\\TEXT\\HumanNPC.txt"					// 人NPCの情報
#define VECTOR_ZERO				(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
#define TEXT_POINTNAME1			"data\\TEXT\\ステージ6\\point.txt"			// 車のポイント情報
#define TEXT_ROUTENAME1			"data\\TEXT\\ステージ6\\route.txt"			// 車のルート情報
#define TEXT_EFFECT				"data\\TEXT\\particle.txt"					// エフェクト情報
#define TEXT_EFFECT_TOOL		"data\\TEXT\\Particle.txt"					// エフェクト情報
#define TEXT_OBJBILL			"data\\TEXT\\ステージ6\\objbillboad.txt"	// オブジェクトビルボード情報

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CGameCharSelect *CGame::m_pGameCharSelect = NULL;	// ゲーム（キャラ選択）

CPlayer *CGame::m_pPlayer = NULL;
CPause *CGame::m_pPause = NULL;
CLoadTextMotion * CGame::m_pPlayerMotion = NULL;
CGameCamera * CGame::m_pGameCamera = NULL;
bool CGame::m_bHelp = false;
bool CGame::m_bPause = false;
CGame::GAMEMODE CGame::m_gameMode = CGame::GAMEMODE_NONE;
CGame::GAMEMODE CGame::m_gameModeNext = CGame::GAMEMODE_NONE; 
CGame::GAMESTATE CGame::m_gameState = CGame::GAMESTATE_NONE;
int	CGame::m_nCntSetStage = 0;
int CGame::m_nGameCounter = 0;

//ウォークスルー用
bool CGame::m_bDrawUI = false;

int CGame::m_nCharSelectNum[MAX_PLAYER] = { 0 };	// キャラ選択番号

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
	CGame::GetPlayer() = NULL;
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
	CBillBoord::Load();			//ビルボードテクスチャの読み込み
	CWall::Load();				//壁のテクスチャの読み込み
	CShadow::Load();			//影のテクスチャ読み込み
	CObject::Load();			//オブジェクトのテクスチャの読み込み
	CFeed::Load();				//食べ物のテクスチャの読み込み
	CEgg::Load();				//卵のテクスチャの読み込み
	m_pPause->Load();			//ポーズのテクスチャの読み込み

	//====================================================================
	//						 必要な変数の初期化
	//====================================================================

	m_gameMode = GAMEMODE_CHARSELECT;	// ゲームモード
	m_gameModeNext = m_gameMode;		// 次のゲームモード
	m_gameState = GAMESTATE_NORMAL;		//通常状態に
	m_nCntSetStage = 0;					//どこのステージから開始するか
	m_bPause = false;					//ポーズを初期化
	m_nGameCounter = 0;					//カウンターの初期化

	SetGameMode(m_gameMode);

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CGame::Uninit(void)
{
	//ポーズ削除
	if (m_pPause != NULL)
	{
		m_pPause->Uninit();
		m_pPause = NULL;
	}

	//===================================
	//	　　UnLoadの破棄する場所
	//===================================

	CMeshField::UnLoad();			//メッシュフィールドテクスチャの破棄
	CFade::UnLoad();				//フェードのテクスチャの破棄
	CBillBoord::UnLoad();			//ビルボードテクスチャの破棄
	CObject::UnLoad();				//オブジェクトのテクスチャの破棄
	CWall::UnLoad();				//壁のテクスチャの破
	CShadow::UnLoad();				//影のテクスチャの破棄
	CFeed::UnLoad();				//餌のテクスチャの破棄
	CEgg::UnLoad();					//卵のテクスチャの破棄

	if (m_pGameCharSelect != NULL)
	{// NULL以外
		m_pGameCharSelect->Uninit();
		m_pGameCharSelect = NULL;
	}

	//フェード以外の破棄
	CScene::NotFadeReleseAll();

	//プレイヤーのモーションの破棄
	if (m_pPlayerMotion != NULL)
	{
		m_pPlayerMotion->Uninit();
		delete m_pPlayerMotion;
		m_pPlayerMotion = NULL;
	}

	//プレイヤーモデルの破棄
	CPlayer::UnloadModel();

	//カメラの破棄
	if (m_pGameCamera != NULL)
	{
		m_pGameCamera->Uninit();
		delete m_pGameCamera;
		m_pGameCamera = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void CGame::Update(void)
{
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputXPad * pXpad = CManager::GetXInput();					//ジョイパットの取得

	if (m_gameMode != m_gameModeNext)
	{
		SetGameMode(m_gameModeNext);
		return;
	}

	switch (m_gameMode)
	{
	case GAMEMODE_CHARSELECT:
		if (pCInputKeyBoard->GetKeyboardTrigger(DIK_RETURN) == true)
		//if (m_nGameCounter == 10)
			CFade::Create(GAMEMODE_PLAY);
		break;
	case GAMEMODE_COURSESELECT:
		if (pCInputKeyBoard->GetKeyboardTrigger(DIK_RETURN) == true)
			CFade::Create(GAMEMODE_PLAY);
		break;
	case GAMEMODE_PLAY:
		//サウンドの情報
		CSound *pSound = CManager::GetSound();

		if (pCInputKeyBoard->GetKeyboardTrigger(DIK_RETURN) == true)
		{
			//m_bDrawUI = m_bDrawUI ? false : true;

			//ポーズの選択の決定音
			CFade::Create(CManager::MODE_RESULT);
		}

		if (m_pPause == false)
		{//開く音
			//現在の状態を保存
			m_NowGameState = GetGameState();

			//カメラの更新処理
			if (m_pGameCamera != NULL) { m_pGameCamera->Update(); }

			switch (m_NowGameState)
			{
			case GAMESTATE_NORMAL:	//通常の状態

				break;
			case GAMESTATE_CLEAR:	//ゲームをクリアした状態
				m_nCounterGameState++;

				if (m_nCounterGameState >= 60)
				{//画面（モード）の設定
					CFade::Create(CManager::MODE_RESULT);
				}
				break;
			case GAMESTATE_OVER:	//ゲームで負けたときの状態
				m_nCounterGameState++;

				if (m_nCounterGameState >= 60)
				{//画面（モード）の設定
					CFade::Create(CManager::MODE_RESULT);
				}
				break;
			}
		}

		//ポーズの処理
		if (pCInputKeyBoard->GetKeyboardTrigger(DIK_P) == true)
		{//Pキーが押されたら

			m_bPause = m_bPause ? false : true;

			switch (m_bPause)
			{
			case true:
				if (m_pPause == NULL)
				{
					//ポーズを開く音
					pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_OPEN);

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
					pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_CLOSE);

					//ポーズを削除
					m_pPause->Uninit();
					m_pPause = NULL;

					//アップデート順番をすべて回す
					CScene::SetUpdatePri(0);
				}
			}
		}
		break;
	}

	//	カウンター進める
	m_nGameCounter++;
}

//=============================================================================
// 描画処理
//=============================================================================
void CGame::Draw(void)
{
	//カメラの設定
	if (m_pGameCamera != NULL) { m_pGameCamera->SetCamera(); }
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
	switch (m_gameMode)
	{// ゲームモード
	case GAMEMODE_CHARSELECT:		// キャラ選択
		if (m_pGameCharSelect != NULL)
		{// NULL以外
			m_pGameCharSelect->Uninit();
			m_pGameCharSelect = NULL;
		}

		break;
	case GAMEMODE_COURSESELECT:		// コース選択

		break;
	case GAMEMODE_PLAY:				// プレイ
		//フェード以外の破棄
		CScene::NotFadeReleseAll();
		break;
	}

	// ゲームモードを変更
	m_gameMode = gameMode;

	switch (m_gameMode)
	{// ゲームモード変更後
	case GAMEMODE_CHARSELECT:		// キャラ選択
		if (m_pGameCharSelect == NULL)
		{// NULL
			m_pGameCharSelect = CGameCharSelect::Create();
		}

		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
			m_nCharSelectNum[nCntPlayer] = nCntPlayer;			// キャラ選択番号

		break;
	case GAMEMODE_COURSESELECT:		// コース選択

		break;
	case GAMEMODE_PLAY:				// プレイ
		SetStage();
		break;
	}

	m_nGameCounter = 0;
}

//=============================================================================
// ステージ設定
//=============================================================================
void CGame::SetStage(void)
{
	//サウンドのポインタ
	CSound *pSound = CManager::GetSound();

	//if (m_nCntSetStage == 0)
	{
		//=====================================================================
		//	ステージ1
		//=====================================================================

		//マップを読み込む
		TextLoad(6);
		WallTextLoad(6);
		MeshTextLoad(6);

		//===================================
		//		Create
		//===================================

		if (m_pPlayerMotion == NULL) { m_pPlayerMotion = CLoadTextMotion::Create(TEXT_PLAYER_MOTION); }	//プレイヤーのモーション読み込み
		CPlayer::LoadModel();	//モデルの読み込み

		//プレイヤーの生成
		m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		//ゲームカメラの生成
		if (m_pGameCamera == NULL)
		{
			m_pGameCamera = new CGameCamera;

			if (m_pGameCamera != NULL) { m_pGameCamera->Init(); }
		}

		for (int nCount = 0; nCount < m_nSetObjectNum; nCount++)
		{
			//オブジェクトの生成
			CObject::Create(m_Map[nCount].m_pos, m_Map[nCount].m_rot, m_Map[nCount].m_scale, 0.0f, m_Map[nCount].m_nTexType, m_Map[nCount].m_nType, CModel3D::MOVETYPE_NOT, m_Map[nCount].m_nCollision);
		}
		for (int nCount = 0; nCount < m_nSetMeshFieldNum; nCount++)
		{
			//フィールドの生成
			CMeshField::Create(m_Mesh[nCount].m_pos, m_Mesh[nCount].m_nWidthDivide, m_Mesh[nCount].m_nDepthDivide, m_Mesh[nCount].m_fTexXUV, m_Mesh[nCount].m_fTexYUV,
				m_Mesh[nCount].m_fWidthLength, m_Mesh[nCount].m_fDepthLength,
				m_Mesh[nCount].m_fVtxHeight_No0, m_Mesh[nCount].m_fVtxHeight_No1, m_Mesh[nCount].m_fVtxHeight_No2, m_Mesh[nCount].m_fVtxHeight_No3,
				m_Mesh[nCount].m_fVtxSide_No0, m_Mesh[nCount].m_fVtxSide_No1, m_Mesh[nCount].m_fVtxSide_No2, m_Mesh[nCount].m_fVtxSide_No3,
				m_Mesh[nCount].m_nTexType, 0);
		}
		for (int nCount = 0; nCount < m_nSetWallNum; nCount++)
		{
			//壁の生成
			CWall::Create(m_aWall[nCount].m_pos, D3DXVECTOR2(m_aWall[nCount].m_fWidthDivide, m_aWall[nCount].m_fHightDivide), m_aWall[nCount].m_rot, m_aWall[nCount].m_nTexType);
		}

		CFeed::Create(D3DXVECTOR3(0.0f, 1.0f, 700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), CFeed::FEEDTYPE_ATTACK);
		CFeed::Create(D3DXVECTOR3(100.0f, 1.0f, 700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), CFeed::FEEDTYPE_ANNOY);
		CFeed::Create(D3DXVECTOR3(200.0f, 1.0f, 700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), CFeed::FEEDTYPE_SPEED);

		m_nCntSetStage = 1;
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