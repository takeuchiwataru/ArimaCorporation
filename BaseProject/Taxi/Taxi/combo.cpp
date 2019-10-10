//=============================================================================
//
// コンボ処理 [combo.cpp]
// Author : shun yokomichi
//
//=============================================================================
#include "combo.h"
#include "manager.h"
#include "number.h"
#include "logo.h"
#include "input.h"
#include "game.h"
#include "texture.h"
//=============================================================================
// マクロ定義
//=============================================================================
#define POS				(D3DXVECTOR3(1060.0f, 220.0f, 0.0f))										// コンボの位置
#define SIZE			(D3DXVECTOR2(90.0f, 23.0f))													// コンボのサイズ
#define NUMBER_POS		(D3DXVECTOR3(((POS.x + 160.0f) - 45.0f * nCntNum), POS.y + 40.0f, POS.z))	// 数字の位置
#define NUMBER_SIZE		(D3DXVECTOR2(40.0f, 35.0f))													// 数字のサイズ
#define LOGO_POS		(D3DXVECTOR3(POS.x + 30.0f, POS.y + 40.0f, POS.z))							// ロゴの位置
#define LOGO_SIZE		(D3DXVECTOR2(60.0f, 15.0f))													// ロゴのサイズ
#define NUMBER_POS_START (D3DXVECTOR2(((POS.x + 250.0f)), POS.y + 40.0f))							// 数字の出現位置
#define SAID_COMBO		(2)																			// コンボ数が指定以上の場合表示する
#define POWER_X			(10)																		// べき乗の計算時
#define NUM_MOVE		(22.0f)																		// コンボ時の数字移動量
#define DISPLAY_TIME	(120)																		// 表示時間
//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
int				   CCombo::m_nComboNum = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
CCombo::CCombo() : CScene2D(5, CScene::OBJTYPE_COMBO)//優先優位イニシャライズ
{
	m_nCntFrame = 0;
	m_nComboOld = 0;
	m_DisNumber = 0;
	m_nDiSTime = 0;
	m_NumPos = D3DXVECTOR2(0.0f, 0.0f);
	for (int nCntNum = 0; nCntNum < MAX_COMBO; nCntNum++)
	{
		m_apNumber[nCntNum] = NULL;
	}
	m_pLogo = NULL;
}
//=============================================================================
// デストラクタ
//=============================================================================
CCombo::~CCombo(){}

//=============================================================================
// 生成処理
//=============================================================================
CCombo * CCombo::Create(TYPE type)
{
	CCombo *pCombo = NULL;

	//NULLチェック
	if (pCombo == NULL)
	{//メモリの動的確保
		pCombo = new CCombo;
		if (pCombo != NULL)
		{
			//オブジェクトクラスの生成
			pCombo->Init();
			//テクスチャを割り当てる
			pCombo->BindTexture(*CTexture::GetTexture((int)CTexture::TYPE_THROUGH + (int)type));
			//位置を割り当てる
			pCombo->SetPosition(POS);
			//サイズの割り当て
			pCombo->m_size = SIZE;
			pCombo->m_type = type;
		}
	}
	return pCombo;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCombo::Init(void)
{
	m_nComboNum = 1;
	m_nComboOld = m_nComboNum;
	m_nCntFrame = COMBO_DISTIME;
	CScene2D::Init();

	if (m_pLogo == NULL)
	{
		m_pLogo = CLogo::Create(LOGO_POS, LOGO_SIZE, CTexture::TYPE_COMBO, 0, CLogo::TYPE_LOGO);
	}
	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CCombo::Uninit(void)
{
	m_nComboNum = 0;

	for (int nCntNum = 0; nCntNum < MAX_COMBO; nCntNum++)
	{
		if (m_apNumber[nCntNum] != NULL)
		{
			m_apNumber[nCntNum]->Uninit();
			m_apNumber[nCntNum] = NULL;
		}
	}

	if (m_pLogo != NULL)
	{
		m_pLogo->Uninit();
		m_pLogo = NULL;
	}

	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CCombo::Update(void)
{
	m_nCntFrame--;
	m_nDiSTime++;
	m_DisNumber = PowerCalculation(m_nComboNum);	// べき乗計算
	if (m_nComboNum == SAID_COMBO)
	{	// 2コンボ目から数字表示
		for (int nCntNum = 0; nCntNum < MAX_COMBO; nCntNum++)
		{
			if (m_apNumber[nCntNum] == NULL)
			{
				m_apNumber[nCntNum] = new CNumber;
				m_apNumber[nCntNum]->Init(D3DXVECTOR3(NUMBER_POS_START.x, NUMBER_POS_START.y, 0.0f), 0);
				m_apNumber[nCntNum]->SetSize(NUMBER_SIZE, NUMBER_POS_START);
				m_apNumber[nCntNum]->SetCol(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
				m_NumPos = NUMBER_POS_START;
			}
		}
	}

	NumMove();		// コンボ数の移動処理
	int nTexData = m_nComboNum;
	for (int nCntNum = 0; nCntNum < m_DisNumber; nCntNum++)
	{
		if (m_apNumber[nCntNum] != NULL)
		{	// Textureの設定
			m_apNumber[nCntNum]->SetNumber(nTexData);
			nTexData /= 10;
			m_apNumber[nCntNum]->SetSize(NUMBER_SIZE, D3DXVECTOR2(m_NumPos.x - 45.0f * nCntNum, m_NumPos.y));
		}
	}

	if (m_nComboOld != m_nComboNum)
	{	// コンボが加算された場合
		m_nCntFrame = COMBO_DISTIME;
		m_nComboOld = m_nComboNum;
		m_NumPos = NUMBER_POS_START;
		m_nDiSTime = 0;
	}

	//ロゴの位置とサイズ
	CScene2D::SetPosSize(POS, SIZE);
}

//=============================================================================
// 描画処理
//=============================================================================
void CCombo::Draw(void)
{
	for (int nCntNum = 0; nCntNum < m_DisNumber; nCntNum++)
	{
		if (m_apNumber[nCntNum] != NULL)
		{
			m_apNumber[nCntNum]->Draw();
		}
	}

	if (m_pLogo != NULL)
	{
		m_pLogo->Draw();
	}

	CScene2D::Draw();

}

//=============================================================================
// べき乗の計算
//=============================================================================
int CCombo::PowerCalculation(int nData)
{
	int nNum = 1;
	int nPower = (int)std::pow(POWER_X, nNum);
	int nDataNum = nData;
	while ((nDataNum / nPower) != 0)
	{
		nNum++;
		nPower = (int)std::pow(POWER_X, nNum);		// べき乗する(二乗や三条など)
		if (nNum >= MAX_COMBO) { nNum = MAX_COMBO; }
	}
	return nNum;
}

//=============================================================================
// コンボ数の移動処理
//=============================================================================
void CCombo::NumMove(void)
{
	m_NumPos.x -= NUM_MOVE;
	if (m_NumPos.x < (POS.x + 160.0f))
	{
		m_NumPos.x = (POS.x + 160.0f);
	}
}