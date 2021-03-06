//=============================================================================
//
// タイトルの処理 [title.cpp]
// Author : 有馬 武志
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS	//Fire Wall突破
#include <string.h>				//ストリング使用のため
#include "title.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "ui.h"
#include "titlecamera.h"
#include "object.h"

#include "titlemenu.h"
#include "player.h"
#include "model.h"
#include "feed.h"
#include "resultui.h"
#include "particle.h"
#include "ground.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define EFFECT_FRAME	(300)		// 季節のエフェクトフレーム
#define SEASON_MAX		(4)			// 季節のカウント
#define EFFECT_CREATE	(10)		// エフェクト生成時間
#define TITLE_POS		(D3DXVECTOR3(SCREEN_WIDTH /2, SCREEN_HEIGHT / 2 - 80.0f, 0.0f))	// 位置(タイトルロゴ000)
#define TITLE_SIZE		(D3DXVECTOR2(480.0f, 100.0f))										// サイズ(タイトルロゴ000)
#define LOGO_POS		(D3DXVECTOR3(TITLE_POS.x + 130.0f, TITLE_POS.y + 100.0f, 0.0f))		// 位置(タイトルロゴ001)
#define LOGO_SIZE		(D3DXVECTOR2(125.0f, 35.0f))										// サイズ(タイトルロゴ001)
#define PRESS_POS		(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT/ 2) + 200.0f, 0.0f))// 位置(Press Start Enter)
#define PRESS_SIZE		(D3DXVECTOR2(250.0f, 20.0f))										// サイズ(Press Start Enter)

#define TEXT_OBJECTNAME1		"data\\TEXT\\ゲームマップ\\objecy.txt"			// 読み込むテキストファイル

#define TEXT_FEEDNAME			"data\\TEXT\\ゲームマップ\\feed.txt"			// 読み込むテキストファイル

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
int				CTitle::m_nTitleCounter = 0;		// タイトルのカウンター

CTitleCamera	*CTitle::m_pTitleCamera = NULL;
bool			CTitle::m_bMenu = false;			// メニュー
bool			CTitle::m_bOnline = false;			// オンライン
bool			CTitle::m_bHost = false;			// ホスト

CPlayer			*CTitle::m_pPlayer[MAX_MEMBER] = { NULL };
int				CTitle::m_nRanking[MAX_MEMBER] = { 0 };			// ランキング

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CTitle::CTitle()
{
	m_nSetObjectNum = 0;

	for (int nCount = 0; nCount < MAX_MAP_OBJECT; nCount++)
		m_Map[nCount] = {};

	m_nCntTex = 0;
	m_nFrame = 0;
	m_nLeafposX = 0;
	m_nLeafsize = 0;
	m_nCntCreate = 0;
	m_bOnOff = false;
	m_nTitleCounter = 0;

	m_bMenu = false;			// メニュー
	m_bOnline = false;			// オンライン
	m_bHost = false;			// ホスト

	m_pTitleMenu = NULL;		// タイトルメニュー

	m_pResultUI = NULL;			// UIメニュー

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
	CFade::Load();				//フェードのテクスチャの読み込み
	CUi::Load();				//UIのテクスチャの読み込み
	CTitleMenu::Load();
	CObject::Load();			//オブジェクトのテクスチャの読み込み
	CToonShader::Load();
	CResultUI::Load();
	CParticle::Load();			// パーティクルのテクスチャのロード
	CGround::Load();

	CChick::Load();				//ひよこのテクスチャの読み込み
	CPlayer::Load();			//モデルの読み込み
	CModel::Load();				//キャラモデルの読み込み

								//マップを読み込む
	TextLoad(6);

	//	変数の初期化
	m_nTitleCounter = 0;

	if (m_pTitleCamera == NULL)
	{
		m_pTitleCamera = new CTitleCamera;
		if (m_pTitleCamera != NULL)
		{
			m_pTitleCamera->Init();
		}
	}

	// リザルト
	if (m_pResultUI == NULL)
	{
		m_pResultUI = CResultUI::Create();
	}

	// タイトルメニュー
	if (m_pTitleMenu == NULL)
	{
		m_pTitleMenu = CTitleMenu::Create();
	}

	SetStage();

	CManager::OnlineSeting(false);	// オンライン設定

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
	CTitleMenu::Unload();
	CObject::UnLoad();				//オブジェクトのテクスチャの破棄
	CResultUI::Unload();
	CParticle::UnLoad();			// パーティクルのテクスチャのロード
	CGround::UnLoad();

	//モデルの破棄
	CChick::UnLoad();				//ひよこのテクスチャの破棄
	CPlayer::Unload();			//モデルの読み込み

	CModel3D::UnLoad();
	CModel3D::ModelShaderDeleter();

	// タイトルカメラの破棄
	if (m_pTitleCamera != NULL)
	{
		m_pTitleCamera->Uninit();
		delete m_pTitleCamera;
		m_pTitleCamera = NULL;
	}

	// リザルト
	if (m_pResultUI != NULL)
	{
		m_pResultUI->Uninit();
		m_pResultUI = NULL;
	}

	// タイトルメニュー
	if (m_pTitleMenu != NULL)
	{
		m_pTitleMenu->Uninit();
		m_pTitleMenu = NULL;
	}

	for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
	{// メンバーカウント
		if (m_pPlayer[nCntMember] != NULL)
		{// NULL以外
			m_pPlayer[nCntMember]->Uninit();
			m_pPlayer[nCntMember] = NULL;
		}
	}

	//マップの破棄
	CCOL_MESH_MANAGER::EndMap();

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
	CInputJoyPad_0 * pXpad = CManager::GetInputJoyPad0(0);					//ジョイパットの取得
	CInputMouse *pCInputMouse = CManager::GetInputMouse();

	//サウンドの情報
	CSound *pSound = CManager::GetSound();

	//フェードのポイント
	CFade::FADE pFade = CFade::GetFade();

	Ranking();
	SetTitleMode();		// タイトルモード

						//	タイトルのカウンター加算
	m_nTitleCounter++;

	if (m_pTitleCamera != NULL) { m_pTitleCamera->Updata(); }
}
//=============================================================================
// 描画処理
//=============================================================================
void CTitle::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// バックバッファ＆Ｚバッファのクリア
	pDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(157, 184, 224, 255),
		1.0f,
		0);

	//タイトルカメラの生成
	if (m_pTitleCamera != NULL) { m_pTitleCamera->SetCamera(); }

	//全ての描画
	CScene::DrawAll();
}

//=============================================================================
// ステージ設定
//=============================================================================
void CTitle::SetStage(void)
{
	//マップを読み込む
	TextLoad(6);
	FeedTextLoad();

	//山の外側の海
	CGround::Create(D3DXVECTOR3(13500.0, -9000.0, 8000.0), D3DXVECTOR3(-0.6f, 0.0f, 0.0f), D3DXVECTOR2(5700.0f, 8300.0f), CGround::TYPE_WATER);
	CGround::Create(D3DXVECTOR3(13500.0, -8950.0, 8000.0), D3DXVECTOR3(0.3f, 0.0f, 0.0f), D3DXVECTOR2(5700.0f, 8300.0f), CGround::TYPE_WATER_HIGHLIGHT);

	//川の処理
	CGround::Create(D3DXVECTOR3(-12000.0f, -4000.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.4f), D3DXVECTOR2(5000.0f, 9000.0f), CGround::TYPE_WATER);
	CGround::Create(D3DXVECTOR3(-12000.0f, -3950.0f, 0.0f), D3DXVECTOR3(-0.2f, 0.0f, 0.0f), D3DXVECTOR2(5000.0f, 9000.0f), CGround::TYPE_WATER_HIGHLIGHT);

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

	for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
	{// メンバーカウント
		float frot = (-D3DX_PI * 0.5f);
		//プレイヤーの生成
		if (m_pPlayer[nCntMember] == NULL)
			m_pPlayer[nCntMember] = CPlayer::Create(
				D3DXVECTOR3(-250.0f + (100.0f * (nCntMember / 4)), -90.0f, -100.0f + (((70.0f * 2.0f) / 3.0f) * (nCntMember % 4)) + (35.0f * (nCntMember / 4))),
				D3DXVECTOR3(0.0f, frot, 0.0f),
				nCntMember, nCntMember, nCntMember, CPlayer::PLAYERTYPE_ENEMY);
		if (m_pPlayer[nCntMember] != NULL)
			m_pPlayer[nCntMember]->SetControl(false);
	}
}

//=============================================================================
// タイトルモード設定
//=============================================================================
void CTitle::SetTitleMode(void)
{
	int nCounter = m_nTitleCounter % RESET_TITLEMODE;

	if (nCounter == RACEINIT_TITLEMODE)
	{// コース
		for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
		{// メンバーカウント
			float frot = (-D3DX_PI * 0.5f);
			if (m_pPlayer[nCntMember] != NULL)
			{// NULL以外
				m_pPlayer[nCntMember]->SetControl(false);

				m_pPlayer[nCntMember]->SetPos(
					D3DXVECTOR3(-250.0f + (100.0f * (nCntMember / 4)), -90.0f, -100.0f + (((70.0f * 2.0f) / 3.0f) * (nCntMember % 4)) + (35.0f * (nCntMember / 4))));
				m_pPlayer[nCntMember]->SetRot(
					D3DXVECTOR3(0.0f, frot, 0.0f));
				m_pPlayer[nCntMember]->SetPlayerType(CPlayer::PLAYERTYPE_ENEMY);
			}
		}

		if (m_pTitleCamera != NULL)
			m_pTitleCamera->SetType(CTitleCamera::CAMERA_COURSE);
	}
	else if (nCounter == RACEMOVE_TITLEMODE)
	{// 動く
		for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
		{// メンバーカウント

			if (m_pPlayer[nCntMember] != NULL)
			{// NULL以外
				m_pPlayer[nCntMember]->SetControl(true);
			}
		}
	}
	else if (nCounter == CAMERA_P_TITLEMODE_1)
	{// カメラをプレイヤーへ
		if (m_pTitleCamera != NULL)
		{
			m_pTitleCamera->SetType(CTitleCamera::CAMERA_PLAYER);
			m_pTitleCamera->SetPlayer(m_pPlayer[(CServer::Rand() % MAX_MEMBER)]);
			m_pTitleCamera->SetTypeReset();
		}
	}
	else if (nCounter == CAMERA_UP_TITLEMODE)
	{// 上から
		if (m_pTitleCamera != NULL)
		{
			m_pTitleCamera->SetType(CTitleCamera::CAMERA_GOUL);
			m_pTitleCamera->SetTypeReset();
		}
	}
	else if (nCounter == CAMERA_P_TITLEMODE_2)
	{// カメラをプレイヤーへ
		if (m_pTitleCamera != NULL)
		{
			m_pTitleCamera->SetType(CTitleCamera::CAMERA_PLAYER);
			m_pTitleCamera->SetTypeReset();
		}
	}
	else if (nCounter == (HOME_TITLEMODE - RANKING_TITLEMODE))
	{// 

	}
	else if (nCounter == HOME_TITLEMODE)
	{// 鳥小屋
		for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
		{// メンバーカウント
			if (m_pPlayer[nCntMember] != NULL)
			{// NULL以外
				m_pPlayer[nCntMember]->Uninit();
				m_pPlayer[nCntMember] = NULL;
			}
		}

		CScene *pScene = NULL;
		CScene *pSceneNext = NULL;

		pScene = CScene::GetTop(0);

		//NULLチェック
		while (pScene != NULL)
		{
			//UpdateでUninitされてしまう場合　Nextが消える可能性があるからNextにデータを残しておく
			CScene *pSceneNext = pScene->GetNext();

			if (pScene->GetDeath() == false)
			{//タイプが壁だったら
				if (pScene->GetObjType() == CScene::OBJTYPE_CHICK || pScene->GetObjType() == CScene::OBJTYPE_EGG)
				{
					//何もしていない状態
					pScene->Uninit();
				}
			}
			//Nextに次のSceneを入れる
			pScene = pSceneNext;
		}

		for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
		{// メンバーカウント
			float frot = (-D3DX_PI * 0.5f);
			if (m_pPlayer[nCntMember] == NULL)
				m_pPlayer[nCntMember] = CPlayer::Create(
					D3DXVECTOR3(((-250.0f + (50.0f * (nCntMember / 4))) + 2950.0f), (-90.0f), (210.0f - (((70.0f * 2.0f) / 3.0f) * (nCntMember % 4)))),
					D3DXVECTOR3(0.0f, frot, 0.0f),
					nCntMember, nCntMember, nCntMember, CPlayer::PLAYERTYPE_SELECT);
			if (m_pPlayer[nCntMember] != NULL)
			{
				m_pPlayer[nCntMember]->SetControl(false);
				m_pTitleCamera->SetTypeReset();
			}
		}

		if (m_pTitleCamera != NULL)
			m_pTitleCamera->SetType(CTitleCamera::CAMERA_CHARSELECT);
	}
	else if (nCounter == CHARUP_TITLEMODE)												
	{// キャラアップ
		if (m_pTitleCamera != NULL)
		{
			m_pTitleCamera->SetType(CTitleCamera::CAMERA_CHARUP);
			m_pTitleCamera->SetPlayer(m_pPlayer[(CServer::Rand() % MAX_MEMBER)]);
		}
	}
}

//===============================================================================
// ランキング
//===============================================================================
void CTitle::Ranking(void)
{
	CSound *pSound = CManager::GetSound();						//サウンドの情報

	int	nGoulNum = 0;						// ゴール数

	for (int nCount = 0; nCount < MAX_MEMBER; nCount++)
	{// プレイヤーカウント
	 // ランキング
		m_nRanking[nCount] = CRoad_Manager::GetManager()->GetRank(nCount) - 1;
	}
}

//===============================================================================
// ファイルからロード
//===============================================================================
void CTitle::TextLoad(int nLoadNumber)
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
// 食べ物をファイルからロード
//===============================================================================
void CTitle::FeedTextLoad(void)
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
char *CTitle::ReadLine(FILE *pFile, char *pDst)
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
char *CTitle::GetLineTop(char * pStr)
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
int CTitle::PopString(char * pStr, char * pDest)
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