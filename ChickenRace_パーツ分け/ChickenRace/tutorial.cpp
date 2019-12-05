//=============================================================================
//
// チュートリアルの処理 [tutorial.cpp]
// Author : 佐藤 安純
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS	//Fire Wall突破
#include <stdio.h>				//インクルドファイル

#include "tutorial.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "meshfield.h"
#include "gamecamera.h"
#include "player.h"
#include "loadText.h"
#include "object.h"
#include "loadText.h"
#include "time.h"
#include "wall.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define VECTOR_ZERO				(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
#define TEXT_MESHFIELDNAME1		"data\\TEXT\\ステージ6\\meshfield.txt"	// メッシュフィールドテキストファイル
#define TEXT_OBJECTNAME1		"data\\TEXT\\ステージ6\\objecy.txt"		// オブジェクトのテキストファイル
#define TEXT_PLAYER_MOTION		"data\\TEXT\\Player\\Player.txt"		// プレイヤーのモーションファイル
#define TEX_LINE				"data\\TEXTURE\\Tutorial\\Line.png"		// 文章テクスチャファイル名
#define TEXT_EFFECT_TOOL		"data\\TEXT\\Particle.txt"				// エフェクト情報
#define TEXT_BOY_MOTION			"data\\TEXT\\お客さん\\motion_boy.txt"	// 男性モーションファイル
#define TEXT_GIRL_MOTION		"data\\TEXT\\お客さん\\motion_girl.txt"	// 女性モーションファイル
#define TEXT_OBJBILL			"data\\TEXT\\ステージ6\\objbillboad.txt"	// オブジェクトビルボード情報
#define TEXT_WALLNAME1			"data\\TEXT\\ステージ6\\wall.txt"			// 読み込むテキストファイル
#define TEXT_CUSTOMER_SUMMER	"data\\TEXT\\お客さん\\Summer.txt"			// お客さんの情報（夏）
#define TIME					(60 * 5)								// 表示時間
#define ACCLE_TIME				(100)									// アクセル時間
#define BREAK_TIME				(50)									// ブレーキ時間
#define HANDLE_TIME				(50)									// 表示時間
#define END_TIME				(60 * 2)								// 表示時間
#define KEY_TIME				(30)									// キーの切り替え時間									

//フレーム
#define FLAM_POS	(D3DXVECTOR3(640.0f, 610.0f, 0.0f))
#define FLAM_SIZE	(D3DXVECTOR2(560.0f, 120.0f))

//ロゴ
#define LOGO_POS	(D3DXVECTOR3(240.0f, 520.0f, 0.0f))
#define LOGO_SIZE	(D3DXVECTOR2(135.0f, 67.5f))

//アイコン
#define ICON_POS	(D3DXVECTOR3(260.0f, 630.0f, 0.0f))
#define ICON_SIZE	(D3DXVECTOR2(48.0f, 48.0f))

//スペースキー
#define SPACE_POS	(D3DXVECTOR3(1020.0f, 660.0f, 0.0f))
#define SPACE_SIZE	(D3DXVECTOR2(90.0f, 15.0f))

//Xボタン
#define X_POS	(D3DXVECTOR3(1050.0f, 660.0f, 0.0f))
#define X_SIZE	(D3DXVECTOR2(30.0f, 20.0f))

//コントローラーボタン
#define CONTROL_POS	(D3DXVECTOR3(640.0f, 650.0f, 0.0f))
#define CONTROL_SIZE	(D3DXVECTOR2(25.0f, 25.0f))

//コントローラーボタン
#define STICK_POS	(D3DXVECTOR3(640.0f, 650.0f, 0.0f))
#define STICK_SIZE	(D3DXVECTOR2(40.0f, 40.0f))

//ギア
#define GEAR_POS	(D3DXVECTOR3(1200.0f, 400.0f, 0.0f))
#define GEAR_SIZE	(D3DXVECTOR2(60.0f, 60.0f))

//スキップボタン
#define SKIP_POS	(D3DXVECTOR3(635.0f, 35.0f, 0.0f))
#define SKIP_SIZE	(D3DXVECTOR2(140.0f, 30.0f))

//文章
#define LINE_POS	(D3DXVECTOR3(640.0f, 600.0f, 0.0f))
#define LINE_SIZE	(D3DXVECTOR2(300.0f, 100.0f))
#define UV			(0.04545f)

//イメージ画像
#define IMAGE_POS	(D3DXVECTOR3(200.0f, 315.0f, 0.0f))
#define IMAGE_SIZE	(D3DXVECTOR2(195.0f, 145.0f))
#define IMAGE_ADDSIZE	(D3DXVECTOR2(4.0f, 3.0f))

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CMeshField * CTutorial::m_pMeshField = NULL;
CGameCamera * CTutorial::m_pCamera = NULL;
CPlayer * CTutorial::m_pPlayer = NULL;
CLoadTextMotion * CTutorial::m_pPlayerMotion = NULL;
CGround * CTutorial::m_pGround = NULL;
CLoadTextEffect * CTutorial::m_pLoadEffect = NULL;
CLoadTextMotion * CTutorial::m_pBoyMotion = NULL;
CLoadTextMotion * CTutorial::m_pGirlMotion = NULL;
CLogo * CTutorial::m_pScoreUI[MAX_SCORE_UI_TUTORIAL] = {};
bool CTutorial::m_bCustomer = false;
CTutorial::TYPE CTutorial::m_type = TYPE_START;

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CTutorial::CTutorial(){}

//=============================================================================
// デストラクタ
//=============================================================================
CTutorial::~CTutorial(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTutorial::Init()
{
	//変数の初期化
	m_pSpace = NULL;
	m_pImage = NULL;
	m_pSkip = NULL;
	m_pBotunn = NULL;
	m_bInput = CManager::GetInputFlag();
	m_nCountTimeKey = 0;
	m_nAinNum = 0;
	
	//--------------------
	// Load
	//--------------------
	CFade::Load();					//フェードテクスチャ
	CMeshField::Load();				//メッシュフィールドテクスチャ
	CObject::Load();				//オブジェクトテクスチャ
	CShadow::Load();				//影のテクスチャ
	CWall::Load();					//壁の読み込み

	if (m_pPlayerMotion == NULL) { m_pPlayerMotion = CLoadTextMotion::Create(TEXT_PLAYER_MOTION); }	//プレイヤーのモーション読み込み
	//CPlayer::LoadModel();		//モデルの読み込み

	if (m_pBoyMotion == NULL) { m_pBoyMotion = CLoadTextMotion::Create(TEXT_BOY_MOTION); }		//男性モーション読み込み
	if (m_pGirlMotion == NULL) { m_pGirlMotion = CLoadTextMotion::Create(TEXT_GIRL_MOTION); }	//女性モーション読み込み

	//--------------------
	// Create
	//--------------------
	ObjectCreate();		//オブジェクトの生成

	//壁の読み込み
	WallTexLoad();

	MeshFildCreate();	//地面の生成

	//プレイヤーの生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, -300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//カメラの生成
	if (m_pCamera == NULL)
	{
		m_pCamera = new CGameCamera;
		if (m_pCamera != NULL) { m_pCamera->Init(); }
	}

	//変数の初期化
	m_type = TYPE_START;
	m_nCountTime = 0;
	m_bCustomer = false;

	//文章ポリゴンの生成
	CreateLine();

	//前進、後退ボタンを操作できないようにする
	m_pPlayer->SetDrive(false);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTutorial::Uninit(void)
{
	//-------------------
	// Unload
	//-------------------
	CFade::UnLoad();					//フェードテクスチャ
	CMeshField::UnLoad();				//メッシュフィールドテクスチャ
	//CPlayer::UnloadModel();				//プレイヤーモデル
	CObject::UnLoad();					//オブジェクトのテクスチャの破棄
	CShadow::UnLoad();					//影テクスチャ
	CWall::UnLoad();					//壁のテクスチャ破棄
	//-------------------
	// Uninit
	//-------------------
	//プレイヤーのモーションの破棄
	if (m_pPlayerMotion != NULL)
	{
		m_pPlayerMotion->Uninit();
		delete m_pPlayerMotion;
		m_pPlayerMotion = NULL;
	}

	//男性のモーション破棄
	if (m_pBoyMotion != NULL)
	{
		m_pBoyMotion->Uninit();
		delete m_pBoyMotion;
		m_pBoyMotion = NULL;
	}

	//女性モーションの破棄
	if (m_pGirlMotion != NULL)
	{
		m_pGirlMotion->Uninit();
		delete m_pGirlMotion;
		m_pGirlMotion = NULL;
	}

	if (m_pPlayer != NULL) { m_pPlayer = NULL; }	//プレイヤーの破棄
	if (m_pGround != NULL) { m_pGround = NULL; }	//川の破棄

	//カメラ
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	UninitBottun();
	UninitControlBottun();

	//フェード以外削除
	CScene::NotFadeReleseAll();

	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	if (m_pLine != NULL) { m_pLine = NULL; }					//文章ポリゴンの破棄
}

//=============================================================================
// 更新処理
//=============================================================================
void CTutorial::Update(void)
{
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputXPad *pXpad = CManager::GetXInput();

	if (pXpad->GetTrigger(XINPUT_GAMEPAD_START, 0) == true ||
		pCInputKeyBoard->GetKeyboardTrigger(DIK_SPACE) == true)
	{//タイトルからゲームへ
	 //フェードが始まったら
		UpdateFade();
	}

	//カメラの更新
	if (m_pCamera != NULL) { m_pCamera->Update(); }
}

//=============================================================================
// 描画処理
//=============================================================================
void CTutorial::Draw(void)
{
	//カメラの設定
	if (m_pCamera != NULL) { m_pCamera->SetCamera(); }
}

//=============================================================================
// フェードの更新処理
//=============================================================================
void CTutorial::UpdateFade(void)
{
	//サウンドの情報
	CSound *pSound = CManager::GetSound();

	//フェードのポイント
	CFade::FADE pFade = CFade::GetFade();

	if (pFade == CFade::FADE_NONE)
	{
		//ポーズの選択の決定音
		//pSound->PlaySound(CSound::SOUND_LABEL_SE_TITLE_ENTER);
		CFade::Create(CManager::MODE_GAME);
	}
}

//=============================================================================
// ボタンの破棄
//=============================================================================
void CTutorial::UninitBottun(void)
{

}

//=============================================================================
// コントローラーボタンの破棄
//=============================================================================
void CTutorial::UninitControlBottun(void)
{

}

//=============================================================================
// ボタンの生成
//=============================================================================
void CTutorial::CreateBottun(void)
{

}

//=============================================================================
// スキップキーの生成
//=============================================================================
void CTutorial::CreateSkip(bool bInput)
{

}

//=============================================================================
// アニメーション更新処理
//=============================================================================
void CTutorial::UpdateAnim(void)
{
	
}

//=============================================================================
//  文章の切り替え処理
//=============================================================================
void CTutorial::ChangeLine(void)
{
	//状態の加算
	m_type = (TYPE)(m_type + 1);

	//UVの設定
	if (m_pLine != NULL) { m_pLine->SetUV(UV, m_type); }
}

//=============================================================================
// 文章の生成
//=============================================================================
void CTutorial::CreateLine(void)
{
	//変数の初期化
	m_pTexture = NULL;
	m_pLine = NULL;

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{//	フラグの初期化
		m_bLine[nCnt] = false;
	}

	//テクスチャの生成
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), TEX_LINE, &m_pTexture);

	//ポリゴンの生成
	if (m_pLine == NULL)
	{
		m_pLine = new CScene2D(6, CScene::OBJTYPE_LOGO);

		if (m_pLine != NULL)
		{
			m_pLine->Init();							//初期化処理
			m_pLine->SetPosSize(LINE_POS, LINE_SIZE);	//位置とサイズの設定
			m_pLine->BindTexture(m_pTexture);			//テクスチャの設定
			m_pLine->SetUV(UV, m_type);					//UVの設定
		}
	}
}

//===============================================================================
// メッシュフィールドをファイルからロード
//===============================================================================
void CTutorial::MeshFildCreate(void)
{
	//ファイル用変数
	FILE *pFile;		//ファイルポインタ
	char *pStrcur;		//現在の先頭の文字列
	char aLine[256];	//文字列
	char aStr[256];		//一時保存文字列
	int nIndex = 0;		//現在のインデックス
	int nWord = 0;		//ポップで返された値を保持

	//読み込んだデータの保管変数
	int nSetMeshFieldNum = 0;		//フィールドの最大数
	int nTexType = -1;				//テクスチャのタイプ番号
	int nWidthDivide = 0;			//分割数
	int nDepthDivide = 0;			//分割数
	float fWidthTexUV = 0.0f;		//テクスチャの横UV
	float fHightTexUV = 0.0f;		//テクスチャの縦UV
	float fWidrhLength = 0.0f;		//横幅の長さ
	float fDepthLength = 0.0f;		//奥行きの長さ
	float fVtxHeight_No0 = 0.0f;
	float fVtxHeight_No1 = 0.0f;
	float fVtxHeight_No2 = 0.0f;
	float fVtxHeight_No3 = 0.0f;
	D3DXVECTOR3 Pos = VECTOR_ZERO;

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
			nSetMeshFieldNum = atoi(pStrcur);
		}

		//オブジェクトの数分回す
		for (int nCntObject = 0; nCntObject < nSetMeshFieldNum; nCntObject++)
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
						nTexType = atoi(pStrcur);
					}
					//横の分割数を読み込み
					if (memcmp(pStrcur, "X_DIVIDE = ", strlen("X_DIVIDE = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("X_DIVIDE = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						nWidthDivide = atoi(pStrcur);
					}
					//縦の分割数を読み込み
					if (memcmp(pStrcur, "Z_DIVIDE = ", strlen("Z_DIVIDE = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("Z_DIVIDE = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						nDepthDivide = atoi(pStrcur);
					}
					//横のテクスチャ座標を読み込み
					if (memcmp(pStrcur, "X_TEXUV = ", strlen("X_TEXUV = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("X_TEXUV = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						fWidthTexUV = (float)atoi(pStrcur);
					}
					//縦のテクスチャ座標を読み込み
					if (memcmp(pStrcur, "Y_TEXUV = ", strlen("Y_TEXUV = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("Y_TEXUV = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						fHightTexUV = (float)atoi(pStrcur);
					}
					//横の長さを読み込み
					if (memcmp(pStrcur, "X_LENGTH = ", strlen("X_LENGTH = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("X_LENGTH = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						fWidrhLength = (float)atoi(pStrcur);
					}
					//縦の長さを読み込み
					if (memcmp(pStrcur, "Z_LENGTH = ", strlen("Z_LENGTH = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("Z_LENGTH = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						fDepthLength = (float)atoi(pStrcur);
					}
					//１頂点の高さ
					if (memcmp(pStrcur, "VTX0_HEIGHT = ", strlen("VTX0_HEIGHT = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("VTX0_HEIGHT = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						fVtxHeight_No0 = (float)atoi(pStrcur);
					}
					//２頂点の高さ
					if (memcmp(pStrcur, "VTX1_HEIGHT = ", strlen("VTX1_HEIGHT = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("VTX1_HEIGHT = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						fVtxHeight_No1 = (float)atoi(pStrcur);
					}
					//３頂点の高さ
					if (memcmp(pStrcur, "VTX2_HEIGHT = ", strlen("VTX2_HEIGHT = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("VTX2_HEIGHT = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						fVtxHeight_No2 = (float)atoi(pStrcur);
					}
					//４頂点の高さ
					if (memcmp(pStrcur, "VTX3_HEIGHT = ", strlen("VTX3_HEIGHT = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("VTX3_HEIGHT = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						fVtxHeight_No3 = (float)atoi(pStrcur);
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
						Pos.x = (float)atof(pStrcur);
						//文字数分進める
						pStrcur += nWord;

						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						Pos.y = (float)atof(pStrcur);
						//文字数分進める
						pStrcur += nWord;

						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						Pos.z = (float)atof(pStrcur);

					}
					else if (memcmp(pStrcur, "MESHFIELD_END", strlen("MESHFIELD_END")) == 0)
					{
						/*CMeshField::Create(Pos, nWidthDivide, nDepthDivide, fWidthTexUV, fHightTexUV,
										  fWidrhLength, fDepthLength, fVtxHeight_No0, fVtxHeight_No1, fVtxHeight_No2, fVtxHeight_No3, nTexType, 0);
						*/break;
					}
				}
			}
		}
	}
}

//=============================================================================
// オブジェクトの生成
//=============================================================================
void CTutorial::ObjectCreate(void)
{
	//ファイル用変数
	FILE *pFile;		//ファイルポインタ
	char *pStrcur;		//現在の先頭の文字列
	char aLine[256];	//文字列
	char aStr[256];		//一時保存文字列
	int nIndex = 0;		//現在のインデックス
	int nWord = 0;		//ポップで返された値を保持

	//読み込み用変数
	int nSetObjNum = -1;				//オブジェクトの最大数
	int nType = -1;						//タイプ番号
	int nTexType = -1;					//テクスチャタイプ
	int nCollision = 0;					//コリジョンの数
	D3DXVECTOR3 pos = VECTOR_ZERO;		//位置
	D3DXVECTOR3 rot = VECTOR_ZERO;		//向き
	D3DXVECTOR3 scale = VECTOR_ZERO;	//スケール

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
			nSetObjNum = atoi(pStrcur);
		}

		//オブジェクトの数分回す
		for (int nCntObject = 0; nCntObject < nSetObjNum; nCntObject++)
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
						nType = atoi(pStrcur);
					}
					//種類のを読み込み
					if (memcmp(pStrcur, "TEXTURETYPE = ", strlen("TEXTURETYPE = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("TEXTURETYPE = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						nTexType = atoi(pStrcur);
					}
					//当たり判定を読み込み
					if (memcmp(pStrcur, "COLLISION = ", strlen("COLLISION = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("COLLISION = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						nCollision = atoi(pStrcur);
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
						pos.x = (float)atof(pStrcur);
						//文字数分進める
						pStrcur += nWord;

						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						pos.y = (float)atof(pStrcur);
						//文字数分進める
						pStrcur += nWord;

						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						pos.z = (float)atof(pStrcur);

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
						rot.x = (float)atof(pStrcur);

						//文字数分進める
						pStrcur += nWord;
						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						rot.y = (float)atof(pStrcur);

						//文字数分進める
						pStrcur += nWord;
						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						rot.z = (float)atof(pStrcur);
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
						scale.x = (float)atof(pStrcur);

						//文字数分進める
						pStrcur += nWord;
						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						scale.y = (float)atof(pStrcur);

						//文字数分進める
						pStrcur += nWord;
						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						scale.z = (float)atof(pStrcur);
					}
					else if (memcmp(pStrcur, "OBJECT_END", strlen("OBJECT_END")) == 0)
					{
						CObject::Create(pos, rot, scale, 0.0f, nTexType,nType, nCollision);
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
char * CTutorial::ReadLine(FILE *pFile, char *pDst)
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
char *CTutorial::GetLineTop(char * pStr)
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
int CTutorial::PopString(char * pStr, char * pDest)
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

//=============================================================================
// 壁のテクスチャの読み込み
//=============================================================================
void CTutorial::WallTexLoad(void)
{
	//変数宣言
	int nMaxWall = 0;	//壁の最大数

						//ファイル用変数
	FILE *pFile;		//ファイルポインタ
	char *pStrcur;		//現在の先頭の文字列
	char aLine[256];	//文字列
	char aStr[256];		//一時保存文字列
	int nIndex = 0;		//現在のインデックス
	int nWord = 0;		//ポップで返された値を保持

						//保存用の変数
	D3DXVECTOR3 Pos = VECTOR_ZERO;
	D3DXVECTOR3 Rot = VECTOR_ZERO;
	float fWidthDivide = 0.0f;
	float fHightDivide = 0.0f;
	int nTexType = 0;

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
			nMaxWall = atoi(pStrcur);
		}

		//オブジェクトの数分回す
		for (int nCntObject = 0; nCntObject < nMaxWall; nCntObject++)
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
						Pos.x = (float)atof(pStrcur);
						//文字数分進める
						pStrcur += nWord;

						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						Pos.y = (float)atof(pStrcur);
						//文字数分進める
						pStrcur += nWord;

						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						Pos.z = (float)atof(pStrcur);

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
						Rot.x = (float)atof(pStrcur);
						//文字数分進める
						pStrcur += nWord;

						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						Rot.y = (float)atof(pStrcur);
						//文字数分進める
						pStrcur += nWord;

						//文字数を返してもらう
						nWord = PopString(pStrcur, &aStr[0]);
						//文字列変換
						Rot.z = (float)atof(pStrcur);

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
						fWidthDivide = (float)atof(pStrcur);
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
						fHightDivide = (float)atof(pStrcur);
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
						nTexType = (int)atof(pStrcur);
					}
					else if (memcmp(pStrcur, "WALL_END", strlen("WALL_END")) == 0)
					{//壁の生成
						CWall::Create(Pos, D3DXVECTOR2(fWidthDivide, fHightDivide), Rot, nTexType);
						break;
					}
				}
			}
		}
	}
}