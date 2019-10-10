//=============================================================================
//
// 表示数字の処理 [numberdisplay.cpp]
// Author : shun yokomichi
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "numberdisplay.h"
#include "number.h"
#include "game.h"
#include "logo.h"
//=============================================================================
// マクロ定義
//=============================================================================
#define CENTER_WIDTH		(SCREEN_WIDTH / 2)							// Xの中心座標
#define TIMER_SPACE			(40.0f)										// 数字と数字の間のサイズ
#define TIMER_POSITION_Y	((SCREEN_HEIGHT / 2) - 0.0f)				// タイマーのY座標
#define TIMER_POSITION_X	((SCREEN_WIDTH / 2) + 150.0f)				// タイマーのX座標
#define MOVE_Y				(3)
#define POWER_X				(10)	

//=============================================================================
//	静的メンバ変数
//=============================================================================

//=============================================================================
//	コンストラクタ
//=============================================================================
CNumberDisplay::CNumberDisplay() 
{
	m_pLogo = NULL;
	m_pPlus = NULL;
}

//=============================================================================
//	生成処理
//=============================================================================
CNumberDisplay *CNumberDisplay::Create(int nNumber)
{
	CNumberDisplay *pNumberDisplay = NULL;

	//NULLチェック
	if (pNumberDisplay == NULL)
	{//メモリの動的確保

		pNumberDisplay = new CNumberDisplay;

		if (pNumberDisplay != NULL)
		{
			// 数字の割り当て
			pNumberDisplay->SetNumber(nNumber);
			// 種類の割り当て
			//表示数字の生成
			pNumberDisplay->Init();
		}
	}
	return pNumberDisplay;
}

//=============================================================================
//	初期化
//=============================================================================
HRESULT CNumberDisplay::Init()
{
	m_nCntDelet = 30;
	m_nUp = 0;
	m_nNumCount = 0;
	int nTexData = m_nNumber;
	m_nNumCount = PowerCalculation(nTexData);

	for (int nCntSaid = 0; nCntSaid < ADDSCORE_DISPLAY; nCntSaid++)
	{
		m_apNumber[nCntSaid] = new CNumber;
		m_apNumber[nCntSaid]->Init(D3DXVECTOR3((TIMER_POSITION_X - TIMER_SPACE * nCntSaid), TIMER_POSITION_Y, 0.0f), 0);
		m_apNumber[nCntSaid]->SetCol(D3DXCOLOR(1.0f, 1.0f, .0f, 1.0f));
		m_apNumber[nCntSaid]->SetNumber(nTexData);
		nTexData /= 10;
		m_apNumber[nCntSaid]->SetSize(D3DXVECTOR2(30.0f, 26.0f), D3DXVECTOR2((TIMER_POSITION_X - TIMER_SPACE * nCntSaid), TIMER_POSITION_Y - m_nUp));
	}

	if (m_pLogo == NULL)
	{	// ￥マーク
		m_pLogo = CLogo::Create(D3DXVECTOR3(TIMER_POSITION_X - TIMER_SPACE * m_nNumCount, TIMER_POSITION_Y, 0.0f), D3DXVECTOR2(20.0f, 20.0f), CTexture::TYPE_MONEY_TEN, 0, CLogo::TYPE_LOGO);
		m_pLogo->SetColor(&D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	}

	if (m_pPlus == NULL)
	{
		m_pPlus = CLogo::Create(D3DXVECTOR3(TIMER_POSITION_X - TIMER_SPACE * (m_nNumCount + 1), TIMER_POSITION_Y, 0.0f), D3DXVECTOR2(20.0f, 20.0f), CTexture::TYPE_PLUS_MARK, 0, CLogo::TYPE_LOGO);
		m_pPlus->SetColor(&D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
	
	return S_OK;
}

//=============================================================================
//	終了処理
//=============================================================================
void CNumberDisplay::Uninit()
{
	for (int nCntSaid = 0; nCntSaid < ADDSCORE_DISPLAY; nCntSaid++)
	{
		if (m_apNumber[nCntSaid] != NULL)
		{
			m_apNumber[nCntSaid]->Uninit();
			m_apNumber[nCntSaid] = NULL;
		}
	}

	if (m_pLogo != NULL)
	{	// ￥マーク
		m_pLogo->Uninit();
		m_pLogo = NULL;
	}

	if (m_pPlus != NULL)
	{
		m_pPlus->Uninit();
		m_pPlus = NULL;
	}

	Release();
}

//=============================================================================
//	更新処理
//=============================================================================
void CNumberDisplay::Update()
{
	m_nCntDelet--;
	m_nUp += MOVE_Y;
	int nTexData = m_nNumber;

	m_nNumCount = PowerCalculation(nTexData);

	for (int nCntSaid = 0; nCntSaid < m_nNumCount; nCntSaid++)
	{// テクスチャに反映
		m_apNumber[nCntSaid]->SetNumber(nTexData);
		nTexData /= 10;
		m_apNumber[nCntSaid]->SetSize(D3DXVECTOR2(30.0f, 26.0f), D3DXVECTOR2((TIMER_POSITION_X - TIMER_SPACE * nCntSaid), TIMER_POSITION_Y - m_nUp));
	}

	if (m_pLogo != NULL)
	{	// ￥マーク
		D3DXVECTOR3 pos = m_pLogo->GetPosition();
		pos.y -= MOVE_Y;
		m_pLogo->SetPosition(D3DXVECTOR3(TIMER_POSITION_X - TIMER_SPACE * m_nNumCount, pos.y, 0.0f));
	}

	if(m_pPlus != NULL)
	{
		D3DXVECTOR3 pos = m_pPlus->GetPosition();
		pos.y -= MOVE_Y;
		m_pPlus->SetPosition(D3DXVECTOR3(TIMER_POSITION_X - TIMER_SPACE * (m_nNumCount + 1), pos.y, 0.0f));
	}

	if (m_nCntDelet == 0)
	{	// 0の場合
		Uninit();
	}
}

//=============================================================================
//	描画処理
//=============================================================================
void CNumberDisplay::Draw()
{
	for (int nCntSaid = 0; nCntSaid < m_nNumCount; nCntSaid++)
	{
		m_apNumber[nCntSaid]->Draw();
	}
}

//=============================================================================
// べき乗の計算
//=============================================================================
int CNumberDisplay::PowerCalculation(int nData)
{
	int nNum = 1;
	int nPower = (int)std::pow(POWER_X, nNum);
	int nDataNum = nData;
	while ((nDataNum / nPower) != 0)
	{
		nNum++;
		nPower = (int)std::pow(POWER_X, nNum);		// べき乗する(二乗や三条など)
		if (nNum >= ADDSCORE_DISPLAY) { nNum = ADDSCORE_DISPLAY; }
	}
	return nNum;
}
