//=============================================================================
//
// 目的地の処理 [destination.cpp]
// Author : shun yokomichi
//
//=============================================================================
#include "scene.h"
#include "scene2D.h"
#include "destination.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "billboord.h"
#include "game.h"
#include "tutorial.h"
#include "player.h"
#include "texture.h"
#include "logo.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define DESTINATION_POS_START		(D3DXVECTOR3(-150.0f, 300.0f, 0.0f))						// 表示位置
#define DESTINATION_POS_END			(D3DXVECTOR3(180.0f, 300.0f, 0.0f))						// 表示位置
#define DESTINATION_SIZE			(D3DXVECTOR2(150.0f, 100.0f))							// サイズ
#define SAID_TIME					(180)					// 表示する時間
#define ADD_MOVE					(15.0f)

//-----------------------------------
// 目的地名のマクロ
//-----------------------------------
#define NAME_POS_START			(D3DXVECTOR3(-150.0f, DESTINATION_POS_END.y + 140.0f, DESTINATION_POS_END.z))
#define NAME_POS_END			(D3DXVECTOR3(DESTINATION_POS_END.x, DESTINATION_POS_END.y + 140.0f, DESTINATION_POS_END.z))
#define NAME_SIZE				( D3DXVECTOR2(150.0f, 25.0f))

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
int CDestination::m_nNextType = 0;
int CDestination::m_nNextFont = 0;

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CDestination::CDestination() : CScene2D(5, CScene::OBJTYPE_LOGO)//優先優位イニシャライズ
{
	m_size = D3DXVECTOR2(0.0f, 0.0f);

	m_fLength = 0.0f;
	m_fAngle = 0.0f;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntTime = 0;
	m_nStateFont = 0;
	m_nStateType = 0;
	m_bDelet = false;
	m_pLogo = NULL;
}
//===============================================================================
//　デストラクタ
//===============================================================================
CDestination::~CDestination()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CDestination::Init(void)
{
	CScene2D::Init();

	if (m_pLogo == NULL)
	{
		m_pLogo =
		CLogo::Create(NAME_POS_END, NAME_SIZE, CTexture::TYPE_NAME_SAPPRO + m_nDestination, 0, CLogo::TYPE_LOGO);
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CDestination::Uninit(void)
{
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
void CDestination::Update(void)
{
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();

	//位置の割り当て
	D3DXVECTOR3 pos;
	pos = GetPosition();

	if (m_nCntTime >= 150)
	{
		pos.x -= ADD_MOVE;
	}

	if (m_pLogo != NULL)
	{
		if (m_nCntTime >= 150)
		{
			D3DXVECTOR3 pos = m_pLogo->GetPosition();
			m_pLogo->SetPosition(m_pLogo->MoveLimit(pos, NAME_POS_END, 0, -ADD_MOVE));
		}
		m_pLogo->Update();
	}

	if (pos.x <= DESTINATION_POS_START.x)
	{
		pos.x = DESTINATION_POS_START.x;
	}

	bool bPutin = false;
	CManager::MODE mode = CManager::GetMode();
	if (mode == CManager::MODE_GAME) { bPutin = CGame::GetPlayer()->GetPutin(); }
	else if (mode == CManager::MODE_TUTORIAL) { bPutin = CTutorial::GetPlayer()->GetPutin(); }
	if (bPutin == true || CManager::GetMode() == CManager::MODE_TUTORIAL)
	{
		m_nCntTime++;
	}

	if (m_nCntTime >= SAID_TIME)
	{	// 表示しない
		SetDelet(true);
	}
	//ロゴの位置とサイズ
	CScene2D::SetPosSize(pos, m_size);
}
//=============================================================================
// 描画処理
//=============================================================================
void CDestination::Draw(void)
{
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputJoypad *pCInputJoyPad = CManager::GetInputJoyPad();

	if (m_pLogo != NULL)
	{
		m_pLogo->Draw();
	}

	CScene2D::Draw();
}
//===============================================================================
//　クリエイト
//===============================================================================
CDestination * CDestination::Create(int nDestination)
{
	CDestination *pDestination = NULL;

	//NULLチェック
	if (pDestination == NULL)
	{//メモリの動的確保

		pDestination = new CDestination;

		if (pDestination != NULL)
		{
			//Textureの種類
			pDestination->m_nDestination = nDestination;
			if (nDestination >= 2) { nDestination--; }
			//オブジェクトクラスの生成
			pDestination->Init();
			//テクスチャを割り当てる
			pDestination->BindTexture(*CTexture::GetTexture(CTexture::TYPE_PLACE_SAPPRO + nDestination));
			//位置を割り当てる
			pDestination->SetPosition(DESTINATION_POS_END);
			//サイズの割り当て
			pDestination->m_size = DESTINATION_SIZE;
		}
	}

	return pDestination;
}
//===============================================================================
//　状態の優先順位
//===============================================================================
void CDestination::SetLogoState(int nStateType, int nStateFont)
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