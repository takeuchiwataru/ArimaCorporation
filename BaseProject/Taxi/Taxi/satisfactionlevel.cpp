//=============================================================================
//
// 満足度の処理 [satisfactionlevel.cpp]
// Author : shun yokomichi
//
//=============================================================================
#include "scene.h"
#include "scene2D.h"
#include "satisfactionlevel.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "texture.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_LEVEL				(20)			// レベルの最大数
#define ADD_SIZE				(8.0f + (MAX_LEVEL / 10) + (MAX_LEVEL / 2))			// サイズ加算

//=========================
// リザルト時使用
//=========================
#define EVALUATION_COUNT		(45)		// カウント
#define LEVEL_ADD				(3)			// 加算するレベル

//=========================
// ランキング時使用
//=========================
#define CHANGE_COL_TIME			(5)		// フレーム数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
int CSatisfaction::m_nNextType = 0;
int CSatisfaction::m_nNextFont = 0;

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CSatisfaction::CSatisfaction() : CScene2D(6, CScene::OBJTYPE_LOGO)//優先優位イニシャライズ
{
	m_size = D3DXVECTOR2(0.0f, 0.0f);

	m_fLength = 0.0f;
	m_fAngle = 0.0f;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntTime = 0;
	m_bChange = false;
	m_nStateFont = 0;
	m_nStateType = 0;
	m_nLevel = 0;
	m_nCntNum = 0;
	m_nCntLevel = 0;
	m_fSize = 0.0f;
	m_type = COL_NOTCHANGE;
	m_nLevelOld = 0;
	m_Col = 0.0f;
	m_bChange = false;
}
//===============================================================================
//　デストラクタ
//===============================================================================
CSatisfaction::~CSatisfaction()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSatisfaction::Init(void)
{
	CScene2D::Init();
	CScene2D::SetColor(&D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSatisfaction::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CSatisfaction::Update(void)
{
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CManager::MODE mode = CManager::GetMode();

	//位置の割り当て
	D3DXVECTOR3 pos;
	pos = GetPosition();

	if (mode == CManager::MODE_RESULT)
	{
		ResultEvaluation();
		CScene2D::SetTexture(m_nCntLevel, MAX_LEVEL, 1, 0);
		//ロゴの位置とサイズ
		CScene2D::SetPosSizeSide(pos, m_size);
	}
	else if(mode == CManager::MODE_GAME || mode == CManager::MODE_TUTORIAL)
	{
		if (m_LevelType == LEVEL_UP)
		{
			m_fSize += 1.0f;
			if ( m_nLevelOld * ADD_SIZE < m_fSize)
			{
				m_fSize = m_nLevelOld * ADD_SIZE;
				if (m_nLevel != m_nLevelOld) { m_nLevelOld++; }
				else if (m_nLevel == m_nLevelOld) { m_LevelType = LEVEL_NONE; }
			}
			ChangeCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

		}
		else if (m_LevelType == LEVEL_DOWN)
		{
			/*m_fSize -= 1.0f;
			if (m_nLevelOld * ADD_SIZE > m_fSize && m_nLevelOld < 1)
			{
				m_fSize = m_nLevelOld * ADD_SIZE;
				if (m_nLevel != m_nLevelOld) { m_nLevelOld--; }
				else if (m_nLevel == m_nLevelOld) { m_LevelType = LEVEL_NONE; m_nLevelOld = m_nLevel; }
			}*/

			m_fSize -= 1.0f;
			if (m_nLevelOld * ADD_SIZE > m_fSize)
			{
				m_fSize = m_nLevelOld * ADD_SIZE;
				if (m_nLevel != m_nLevelOld) { m_nLevelOld--; }
				if (m_nLevel == m_nLevelOld) { m_LevelType = LEVEL_NONE; }
			}
			ChangeCol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
		}

		// Texture設定
		CScene2D::SetTexture(m_nLevelOld, MAX_LEVEL, 1, 0);
		if (m_LevelType == LEVEL_NONE) { CScene2D::SetColor(&D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)); }
		//ロゴの位置とサイズ
		CScene2D::SetPosSizeSide(pos, D3DXVECTOR2(m_fSize, m_size.y));
	}
	else
	{
		// Texture設定
		CScene2D::SetTexture(m_nLevel, MAX_LEVEL, 1, 0);
		//ロゴの位置とサイズ
		CScene2D::SetPosSizeSide(pos, m_size);
	}

}
//=============================================================================
// 描画処理
//=============================================================================
void CSatisfaction::Draw(void)
{
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputJoypad *pCInputJoyPad = CManager::GetInputJoyPad();

	CScene2D::Draw();
}

//===============================================================================
// 満足度加算
//===============================================================================
void CSatisfaction::AddLevel(int nLevel)
{
	m_LevelType = LEVEL_UP;
	m_nLevelOld = m_nLevel;
	if (m_nLevel < MAX_LEVEL - 1) { m_size.x += ADD_SIZE * nLevel; }

	m_nLevel += nLevel;

	if (m_nLevel >= MAX_LEVEL - 1) { m_nLevel = MAX_LEVEL - 1; }
}

//===============================================================================
// リザルト時に表示
//===============================================================================
void CSatisfaction::ResultEvaluation(void)
{
	if ((m_nCntTime % EVALUATION_COUNT) == 0)
	{
		m_nCntLevel += LEVEL_ADD + m_nCntNum;
		m_nCntNum++;
		if (m_nCntNum > 1) { m_nCntNum = 1; }
		if (m_nCntLevel > m_nLevel) { m_nCntLevel = m_nLevel; }

		m_size = D3DXVECTOR2(m_fSize + (ADD_SIZE * m_nCntLevel), m_size.y);
	}
	m_nCntTime++;
}

//=============================================================================
// 色(透明度)変化の処理
//=============================================================================
void CSatisfaction::ChangeCol(D3DXCOLOR col)
{
	if ((m_nCntTime % CHANGE_COL_TIME) == 0)
	{
		if (m_bChange == false) { CScene2D::SetColor(&col); m_bChange = true; }
		else if (m_bChange == true) { CScene2D::SetColor(&D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)); m_bChange = false; }
	}
	m_nCntTime++;
}

//===============================================================================
// 満足度減算
//===============================================================================
void CSatisfaction::Subtraction(int nLevel)
{
	m_LevelType = LEVEL_DOWN;
	m_nLevelOld = m_nLevel;
	if (m_nLevel > 0) { m_size.x -= ADD_SIZE * nLevel; }

	m_nLevel -= nLevel;

	if (m_nLevel <= 0) { m_nLevel = 1; }
}

//===============================================================================
//　クリエイト
//===============================================================================
CSatisfaction * CSatisfaction::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nLevel)
{
	CSatisfaction *pSatisfaction = NULL;
	CManager::MODE mode = CManager::GetMode();

	//NULLチェック
	if (pSatisfaction == NULL)
	{//メモリの動的確保

		pSatisfaction = new CSatisfaction;

		if (pSatisfaction != NULL)
		{
			//変形できる種類
			pSatisfaction->m_nLevel = nLevel;
			pSatisfaction->m_nLevelOld = nLevel;
			//オブジェクトクラスの生成
			pSatisfaction->Init();
			//テクスチャを割り当てる
			pSatisfaction->BindTexture(*CTexture::GetTexture(CTexture::TYPE_SATISFACTION));
			//位置を割り当てる
			pSatisfaction->SetPosition(pos);
			//サイズの割り当て
			if (mode == CManager::MODE_RESULT)
			{
				pSatisfaction->m_size = size;
				pSatisfaction->m_fSize = size.x;
			}
			else if (mode == CManager::MODE_GAME || mode == CManager::MODE_TUTORIAL)
			{
				pSatisfaction->m_size = D3DXVECTOR2(size.x + (ADD_SIZE * nLevel), size.y);
				pSatisfaction->m_fSize = size.x + (ADD_SIZE * nLevel);
			}
			else
			{
				pSatisfaction->m_size = D3DXVECTOR2(size.x + (ADD_SIZE * nLevel), size.y);
			}
		}
	}

	return pSatisfaction;
}
//===============================================================================
//　状態の優先順位
//===============================================================================
void CSatisfaction::SetLogoState(int nStateType, int nStateFont)
{
	//アイコンの優先順位
	if (m_nNextType <= nStateType)
	{
		m_nNextType = nStateType;
	}
	//フォントの優先順位
	if (m_nNextFont <= nStateFont)
	{
		m_nNextFont = nStateFont;
	}
}