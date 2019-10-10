//=============================================================================
//
// タイマー処理 [time.cpp]
// Author : keisuke komatsu
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "time.h"
#include "number.h"
#include "game.h"
#include "numberdisplay.h"
#include "fade.h"
#include "logo.h"
#include "gamecamera.h"
#include "tutorial.h"
#include "player.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define CENTER_WIDTH		(SCREEN_WIDTH / 2)				// Xの中心座標
#define TIMER_SPACE			(50.0f)							// 数字と数字の間のサイズ(ゲーム時間)
#define TIMER_POSITION_Y	(70.0f)							// タイマーのY座標(ゲーム時間)
#define MAX_TIME			(999)							// 最大制限時間(ゲーム時間)
#define TIME_SPACE			(40.0f)							// 数字と数字の間のサイズ(お客様の時間)
#define TIME_POSITION_Y		((SCREEN_HEIGHT / 2) - 80.0f)	// タイマーのY座標(お客様の時間)
#define TIME_POSITION_X		((SCREEN_WIDTH / 2) + 180.0f)	// タイマーのX座標(お客様の時間)
#define GAME_TIME			(25)							// ゲーム内の時間
#define POWER_X				(10)
#define DIRECTING_FRAME		(2)							// Time Up 表示のフレーム時間
//=============================================================================
//	静的メンバ変数
//=============================================================================
int						CTime::m_nTime = 0;
int						CTime::m_nCutomerTime = 0;
bool					CTime::m_bRideEnd = false;
CTime::STATETIME		CTime::m_stateTime = STATE_NONE;
CTime::STATECUTOMERADD	CTime::m_stateCutomer = STATECUTOMERADD_NONE;

//=============================================================================
// 生成処理
//=============================================================================
CTime *CTime::Create(D3DXVECTOR3 pos)
{
	CTime *pTime = NULL;

	//NULLチェック
	if (pTime == NULL)
	{//動的確保
		pTime = new CTime;

		//NULLチェック
		if (pTime != NULL)
		{
			//位置の設定
			pTime->m_pos = pos;
			//初期化処理
			pTime->Init();
		}
	}

	return pTime;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CTime::CTime(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_nTimeCount = 0;
	m_nCutomerTime = 0;
	m_nTimeNum = 1;
	m_nCutmoerCount = -1;
	m_nCutmoerNum = 0;
	m_pLogo = NULL;
	m_nCntDirecting = 0;
	m_bUse = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CTime::~CTime(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTime::Init(void)
{
	m_bUse = false;
	int nTexData = 0;
	m_nTime = GAME_TIME;
	m_nTimeNum = PowerCalculation(m_nTime, 0);

	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		for (int nCntTime = 0; nCntTime < TIME_MAX; nCntTime++)
		{	// タイマー初期設定
			m_apNumber[nCntTime] = new CNumber;
			m_apNumber[nCntTime]->Init(D3DXVECTOR3((m_pos.x - TIMER_SPACE * nCntTime), m_pos.y, m_pos.z), 0);
		}
		// 制限時間のロゴの生成
		m_pLogo = CLogo::Create(D3DXVECTOR3(100.0f, 150.0f, 0.0f), D3DXVECTOR2(75.0f, 40.0f), CTexture::TYPE_GAMETIMELOGO, 0, CLogo::TYPE_LOGO);

		// 数字のテクスチャ設定
		TexTime(nTexData);
	}

	for (int nCntCutTiem = 0; nCntCutTiem < CUTOMERTIME_MAX; nCntCutTiem++)
	{	// お客様の時間の初期化
		m_apCutomerTiem[nCntCutTiem] = NULL;
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTime::Uninit(void)
{
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		for (int nCntTime = 0; nCntTime < TIME_MAX; nCntTime++)
		{	// タイマーの破棄
			if (m_apNumber[nCntTime] != NULL)
			{
				m_apNumber[nCntTime]->Uninit();
				m_apNumber[nCntTime] = NULL;
			}
		}
	}

	for (int nCntCutTiem = 0; nCntCutTiem < CUTOMERTIME_MAX; nCntCutTiem++)
	{	// お客様の時間の破棄
		if (m_apCutomerTiem[nCntCutTiem] != NULL)
		{
			m_apCutomerTiem[nCntCutTiem]->Uninit();
			m_apCutomerTiem[nCntCutTiem] = NULL;
		}
	}

	// ロゴの破棄
	if (m_pLogo != NULL)
	{
		m_pLogo->Uninit();
		m_pLogo = NULL;
	}

	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTime::Update(void)
{
	//現在のモードの取得
	CManager::MODE GameMode = CManager::GetMode();
	DebugKey();		// デバック用
	CGameCamera * pGameCamera = NULL;
	CSound *pSound = CManager::GetSound();

	if ((GameMode == CManager::MODE_GAME))
	{//制限時間
		//ゲーム
		pGameCamera = CGame::GetGameCamera();

		CGameCamera::MODECAMERA mode = pGameCamera->GetModeCamera();

		if (mode == CGameCamera::MODE_START) { return; }
		m_nTimeCount++;
		m_nCutmoerCount++;

		//TimeManagement();	// 時間の管理
		if (m_nTime == 0)
		{	// ゲーム終了
			if (m_bUse == false)
			{
				pSound->StopSound();
				m_bUse = true;
			}

			/*if (m_nCntDirecting == 0 && GameMode == CManager::MODE_GAME)
			{	// Time Up 表示
				CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR2(300.0f, 100.0f), CTexture::TYPE_TIME_UP, 0, CLogo::TYPE_LOGO);
				pSound->SetVolume(CSound::SOUND_LABEL_BGM_SELECT, 0.7f);
				pSound->PlaySound(CSound::SOUND_LABEL_BGM_SELECT);
				pSound->SetVolume(CSound::SOUND_LABEL_TIME_UP, 3.0f);
				pSound->PlaySound(CSound::SOUND_LABEL_TIME_UP);
			}

			if (GameMode == CManager::MODE_GAME && m_nCntDirecting == DIRECTING_FRAME)
			{	// リザルトへ
				CFade::Create(CManager::MODE_RESULT);
			}*/

			if (CManager::MODE_TUTORIAL != GameMode)
			{
				CGame::GetPlayer()->SetControl(false);
			}
		}
		else if (m_nTime >= MAX_TIME)
		{	// 制限時間が最大以上の場合
			m_nTime = MAX_TIME;
		}
		SetCutomer();		// お客様の時間の生成と管理

		int nTexData = 0;

		// 数字のテクスチャ設定
		TexTime(nTexData);
		TexTimCutomerTimee(nTexData);

		if (m_pLogo != NULL) { m_pLogo->Update(); }
	}
	else if (GameMode == CManager::MODE_TUTORIAL)
	{
		pGameCamera = CTutorial::GetCamera();
		m_nCutmoerCount++;
		TimeManagement();	// 時間の管理
		SetCutomer();		// お客様の時間の生成と管理

		int nTexData = 0;
		TexTimCutomerTimee(nTexData);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTime::Draw(void)
{
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		for (int nCntTime = 0; nCntTime < m_nTimeNum; nCntTime++)
		{
			if (m_apNumber[nCntTime] != NULL)
			{
				m_apNumber[nCntTime]->Draw();
			}
		}
	}

	for (int nCntCutTiem = 0; nCntCutTiem < m_nCutmoerNum; nCntCutTiem++)
	{
		if (m_apCutomerTiem[nCntCutTiem] != NULL)
		{
			m_apCutomerTiem[nCntCutTiem]->Draw();
		}
	}

	if (m_pLogo != NULL) { m_pLogo->Draw(); }
}

//=============================================================================
// お客様の時間の管理(生成と破棄)
//=============================================================================
void CTime::SetCutomer(void)
{
	if (m_nCutomerTime <= 0)
	{	// カウントが0の場合
		m_nCutomerTime = 0;
		m_bRideEnd = true;
		for (int nCntCutTiem = 0; nCntCutTiem < CUTOMERTIME_MAX; nCntCutTiem++)
		{
			if (m_apCutomerTiem[nCntCutTiem] != NULL)
			{	// お客様の時間があるなら終了処理へ
				m_apCutomerTiem[nCntCutTiem]->Uninit();
				m_apCutomerTiem[nCntCutTiem] = NULL;
			}
		}
	}
	else
	{
		for (int nCntCutTiem = 0; nCntCutTiem < CUTOMERTIME_MAX; nCntCutTiem++)
		{
			if (m_apCutomerTiem[nCntCutTiem] == NULL)
			{	// ない場合は生成処理へ
				m_apCutomerTiem[nCntCutTiem] = new CNumber;
				m_apCutomerTiem[nCntCutTiem]->Init(D3DXVECTOR3((TIME_POSITION_X - TIME_SPACE * nCntCutTiem), 450.0f, 0.0f), 0);
				m_apCutomerTiem[nCntCutTiem]->SetSize(D3DXVECTOR2(30.0f, 26.0f), D3DXVECTOR2((TIME_POSITION_X - TIME_SPACE * nCntCutTiem), 450.0f));
			}
		}
	}
}

//=============================================================================
// タイマーのTexture管理
//=============================================================================
void CTime::TexTime(int nTexData)
{
	nTexData = m_nTime;

	for (int nCntTime = 0; nCntTime < m_nTimeNum; nCntTime++)
	{// テクスチャに反映

		if (m_apNumber[nCntTime] != NULL)
		{
			m_apNumber[nCntTime]->SetNumber(nTexData);

			nTexData /= 10;
			// 色の設定
			if (m_nTime <= 10)
			{ // 10秒以下 色を赤に
				m_apNumber[nCntTime]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
				m_pLogo->SetColor(&D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}
			else
			{
				m_apNumber[nCntTime]->SetCol(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
				m_pLogo->SetColor(&D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
			}
		}
	}
}

//=============================================================================
// タイマーのTexture管理
//=============================================================================
void CTime::TexTimCutomerTimee(int nTexData)
{
	nTexData = m_nCutomerTime;
	for (int nCntCutTiem = 0; nCntCutTiem < m_nCutmoerNum; nCntCutTiem++)
	{// テクスチャに反映
		if (m_apCutomerTiem[nCntCutTiem] != NULL)
		{
			m_apCutomerTiem[nCntCutTiem]->SetNumber(nTexData);
			nTexData /= 10;
			// 色の設定
			if (m_nCutomerTime <= 10)
			{ // 10秒以下 色を赤に
				m_apCutomerTiem[nCntCutTiem]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}
			else
			{
				m_apCutomerTiem[nCntCutTiem]->SetCol(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
			}
		}
	}
}

//=============================================================================
// タイム加算処理
//=============================================================================
void CTime::AddTime(int nTime)
{
	if (m_nTime > 0)
	{
		m_nTime += nTime;
	}
	m_stateTime = STATE_TIMEADD;
}
//=============================================================================
// べき乗の計算
//=============================================================================
int CTime::PowerCalculation(int nData, int nOperation)
{
	int nNum = 1;
	int nPower = (int)std::pow(POWER_X, nNum);
	int nDataNum = nData;
	while ((nDataNum / nPower) != 0)
	{
		nNum++;
		nPower = (int)std::pow(POWER_X, nNum);		// べき乗する(二乗や三条など)
		if (nOperation == 0)
		{	// 制限時間
			if (nNum >= TIME_MAX) { nNum = TIME_MAX; }
		}
		else if (nOperation == 1)
		{	// 乗車時間
			if (nNum > CUTOMERTIME_MAX) { nNum = CUTOMERTIME_MAX - 1; }
		}
	}
	return nNum;
}

//=============================================================================
// 時間の管理
//=============================================================================
void CTime::TimeManagement(void)
{
	if (m_stateTime == STATE_TIMEADD)
	{	// 制限時間加算
		m_nTimeNum = PowerCalculation(m_nTime, 0);
		m_stateTime = STATE_NONE;
	}
	if (m_stateCutomer == STATECUTOMERADD_CUTOMERADD)
	{	// 乗車時間加算
		m_nCutmoerNum = PowerCalculation(m_nCutomerTime, 0);
		m_stateCutomer = STATECUTOMERADD_NONE;
	}
	if (m_nTimeCount % 60 == 0)
	{// 1秒ごとに減算(制限時間)
		m_nTime--;
		if (m_nTime < 0) { m_nTime = 0;
		m_nCntDirecting++; }
		m_nTimeNum = PowerCalculation(m_nTime, 0);
	}
	if (m_nCutmoerCount % 60 == 0)
	{	//1秒ごとに減算(乗車時間)
		if (CManager::GetMode() == CManager::MODE_TUTORIAL && CTutorial::GetType() == CTutorial::TYPE_MIDDLE3 ||
			CTutorial::GetType() == CTutorial::TYPE_MIDDLE4 || CTutorial::GetType() == CTutorial::TYPE_MIDDLE5)
		{
			return;
		}
		m_nCutomerTime--;
		m_nCutmoerNum = PowerCalculation(m_nCutomerTime, 0);
	}
}
//=============================================================================
// デバック用
//=============================================================================
void CTime::DebugKey(void)
{
	// デバック用
	if (CManager::GetInput()->GetKeyboardTrigger(DIK_1))
	{	// 制限時間加算
		AddTime(30);
	}
	if (CManager::GetInput()->GetKeyboardTrigger(DIK_4))
	{	// お客様の時間の加算
		SetCutomerTime(5);
	}
	if (CManager::GetInput()->GetKeyboardTrigger(DIK_7))
	{	// リザルト画面へ
		CGame::SetGameState(CGame::GAMESTATE_CLEAR);
	}
}