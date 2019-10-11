//=============================================================================
//
// ロゴの処理 [logo.cpp]
// Author : 有馬武志
//
//=============================================================================
#include "scene.h"
#include "scene2D.h"
#include "logo.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "billboord.h"
#include "game.h"
#include "player.h"
#include "texture.h"
#include "tutorial.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define VECTORE_ZERO		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
#define COUNT_COLA			(30)					// 透明時カウント
#define COUNT_FRAME			(80 + COUNT_COLA)		// 表示フレーム
#define COLA_SPEED			(m_nCntColA * 0.015f)	// 透明度の速度

//--------------------------------
// ギア切替関係
//--------------------------------
#define MOVE_GEAR_POS		(D3DXVECTOR3(pos.x, m_posOld.y + (10.0f * (int)pPlayer->GetState()), pos.z))	// 移動時の目的地
#define MOVE_GEAR_ADD		(1.0f)																			// 移動量の加算値

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
int CLogo::m_nNextType = 0;
int CLogo::m_nNextFont = 0;

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CLogo::CLogo(int nPriorityv) : CScene2D(nPriorityv, CScene::OBJTYPE_LOGO)//優先優位イニシャライズ
{
	m_nType = 0;
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_fLength = 0.0f;
	m_fAngle = 0.0f;
	m_rot = VECTORE_ZERO;
	m_moveLimit = VECTORE_ZERO;
	m_nCntColA = 0;
	m_bColA = false;
	m_nStateFont = 0;
	m_nStateType = 0;
	m_nCount	 = 0;
	m_bMoveFlag = false;
	m_bFlag = false;
	m_bDataFlag = false;
	m_nCntData = 0;
	m_nCntTime = 0;
	m_bVibration = false;
}
//===============================================================================
//　デストラクタ
//===============================================================================
CLogo::~CLogo()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLogo::Init(void)
{
	CScene2D::Init();
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLogo::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CLogo::Update(void)
{
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();

	//位置の割り当て
	D3DXVECTOR3 pos;
	pos = GetPosition();

	//ロゴの位置とサイズ
	CScene2D::SetPosSize(pos, m_size);

	// 色の設定
	SetCol();
	// Texture設定
	TexAnim();
}
//=============================================================================
// 描画処理
//=============================================================================
void CLogo::Draw(void)
{
	CScene2D::Draw();
}
//===============================================================================
//　クリエイト
//===============================================================================
CLogo * CLogo::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nType, int nTransformType, TYPE type)
{
	CLogo *pLogo = NULL;

	//NULLチェック
	if (pLogo == NULL)
	{//メモリの動的確保
		int nPriorityv = 5;
		if (CTexture::TYPE_MONEY_TEN == nType || CTexture::TYPE_GAMETIMELOGO == nType ||  CTexture::TYPE_TUTORIAL_STOP == nType) { nPriorityv = 6; }
		else if (CTexture::TYPE_PAUSE_ICON <= nType && CTexture::TYPE_PAUSE_OPERATION >= nType ) { nPriorityv = 7; }
		pLogo = new CLogo(nPriorityv);

		if (pLogo != NULL)
		{
			//種類を割り当てる
			pLogo->m_nType = nType;
			//変形できる種類
			pLogo->m_nTransformType = nTransformType;
			//オブジェクトクラスの生成
			pLogo->Init();
			//テクスチャを割り当てる
			if (CLogo::TYPE_LOGO == type) { pLogo->BindTexture(*CTexture::GetTexture(nType)); }
			//位置を割り当てる
			pLogo->SetPosition(pos);
			pLogo->m_posOld = pos;
			//サイズの割り当て
			pLogo->m_size = size;
		}
	}

	return pLogo;
}
//===============================================================================
//　状態の優先順位
//===============================================================================
void CLogo::SetLogoState(int nStateType, int nStateFont)
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
//===============================================================================
//　Logo内で色の設定をまとめた関数
//===============================================================================
void CLogo::SetCol()
{
	if (m_nType == (int)CTexture::TYPE_TITLE_PRESS)
	{	// Press Strat Enterの場合
		m_nCntColA++;
		if ((m_nCntColA % COUNT_FRAME) == 0)
		{
			CScene2D::SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_nCntColA = 0;
		}
		else if (m_nCntColA >= COUNT_COLA)
		{
			CScene2D::SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}

	if ((m_nType == (int)CTexture::TYPE_RESULT_BUTTON) || (m_nType == (int)CTexture::TYPE_TUTORIAL_SKIP_KEYBOAD) || (m_nType == (int)CTexture::TYPE_TUTORIAL_SKIP_PAD) ||
		(m_nType == (int)CTexture::TYPE_TUTORIAL_ENTER_KEYBOAD) || (m_nType == (int)CTexture::TYPE_TUTORIAL_ENTER_PAD))
	{	// 透明度の変化(じょじょに変える)
		float fColA = COLA_SPEED;
		m_nCntColA++;
		if (m_bColA == false)
		{
			CScene2D::SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + fColA));
			if (fColA >= 1.0f) { m_nCntColA = 0;  m_bColA = true; }
		}
		else if (m_bColA == true)
		{
			CScene2D::SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - fColA));
			if (fColA >= 1.0f) { m_nCntColA = 0;  m_bColA = false; }
		}
	}

	if ((m_nType == (int)CTexture::TYPE_COMMAND_000))
	{
		float fColA = m_nCntColA * 0.045f;
		m_nCntColA++;
		if (m_bColA == false)
		{
			CScene2D::SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + fColA));
			if (fColA >= 1.0f) { m_nCntColA = 0;  m_bColA = true; }
		}
		else if (m_bColA == true)
		{
			CScene2D::SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - fColA));
			if (fColA >= 1.0f) { m_nCntColA = 0;  m_bColA = false; }
		}
	}
}
//===============================================================================
//　Logo内でTextureAnimationをまとめた関数
//===============================================================================
void CLogo::TexAnim()
{
	if (m_nType == (int)CTexture::TYPE_GEARCHANGE)
	{	//ギア切替の場合
		CManager::MODE mode = CManager::GetMode();
		CPlayer * pPlayer = NULL;
		D3DXVECTOR3 pos = CScene2D::GetPosition();

		if (mode == CManager::MODE_GAME)
		{//ゲーム
			pPlayer = CGame::GetPlayer();
		}
		if (mode == CManager::MODE_TUTORIAL)
		{//チュートリアル
			pPlayer = CTutorial::GetPlayer();
		}

		if (m_bDataFlag == true)
		{
			pos = Move(pos, 3.0f);
		}

		if (m_nCntData != (int)pPlayer->GetState())
		{
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_STAGESELECT);
			m_nCntData = (int)pPlayer->GetState();
			m_bDataFlag = true;
		}
		else if (m_bDataFlag == true)
		{
			m_nCount++;
			if ((m_nCount % 15) == 0)
			{
				m_bDataFlag = false;
				pos = m_posOld;
				m_nCount = 0;
			}
		}

		CScene2D::SetPosition(pos);
		CScene2D::SetTexture(m_nCntData, 2, 1, 1);
	}
	else if (m_nType == (int)CTexture::TYPE_MONEY_TEN)
	{	// \マーク & .の場合
		CScene2D::SetTexture(m_nTransformType, 2, 1, 1);
	}
	else if (m_nType == (int)CTexture::TYPE_BOARDING)
	{	// 空車か乗車のTexture
		int nType = 0;
		bool bPutin = false;

		if (bPutin == false) { nType = 0; CScene2D::SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f)); }		// 空車の場合
		else if (bPutin == true) { nType = 1; CScene2D::SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));} // 乗車の場合

		CScene2D::SetTexture(nType, 2, 1, 1);
	}
}

//===============================================================================
//　Logo内で移動の設定をまとめた関数
//===============================================================================
D3DXVECTOR3 CLogo::Move(D3DXVECTOR3 pos, float fMove)
{
	D3DXVECTOR3 move = VECTORE_ZERO;
	if (CTexture::TYPE_IPHONE == m_nType)
	{	// iPhoneなら
		move.y = fMove;
		if (fMove < 0 && m_nCntTime < 60)
		{
			//振動音を再生する
			if (m_bVibration == false)
			{ 
				CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_VIBRATION);
				m_bVibration = true;
			}

			if ((m_nCntTime % 2) == 0)
			{
				m_bFlag = m_bFlag ? false : true;
			}

			if (m_bFlag == false) { move.x = 3.0f; }
			else if (m_bFlag == true) { move.x = -3.0f; }
			m_nCntTime++;
		}
		else if (m_nCntTime >= 60)
		{
			if (m_bVibration)
			{//音を止める
				CManager::GetSound()->StopSound(CSound::SOUND_LABEL_SE_VIBRATION);
				m_bVibration = false;
			}
		}
	}
	else if (m_nType == (int)CTexture::TYPE_GEARCHANGE)
	{
		if ((m_nCntTime % 2) == 0)
		{
			m_bFlag = m_bFlag ? false : true;
		}

		if (m_bFlag == false) { move.x = fMove; }
		else if (m_bFlag == true) { move.x = -fMove; }
		m_nCntTime++;
	}
	else { move.x += fMove; }

	//位置を更新(移動)
	pos.x += move.x;
	pos.y += move.y;

	return pos;
}

//===============================================================================
//　移動制限の設定
//===============================================================================
D3DXVECTOR3 CLogo::MoveLimit(D3DXVECTOR3 pos, D3DXVECTOR3 PosLimit, int nOperation, float fMove)
{
	pos = Move(pos, fMove);

	if (nOperation == 0)
	{	// X軸が指定した値より右に行かない
		if (pos.x >= PosLimit.x)
		{
			pos.x = PosLimit.x;
			m_bMoveFlag = m_bMoveFlag ? false : true;
		}
	}
	else if (nOperation == 1)
	{	// X軸が指定した値より左に行かない
		if (pos.x <= PosLimit.x)
		{
			pos.x = PosLimit.x;
			m_bMoveFlag = m_bMoveFlag ? false : true;
		}
	}
	else if (nOperation == 2)
	{	// Y軸が指定した値より下に行かない
		if (pos.y <= PosLimit.y)
		{
			pos.y = PosLimit.y;
			m_bMoveFlag = m_bMoveFlag ? false : true;
		}
	}
	else if (nOperation == 3)
	{	// Y軸が指定した値より上に行かない
		if (pos.y >= PosLimit.y)
		{
			pos.y = PosLimit.y;
			m_bMoveFlag = m_bMoveFlag ? false : true;
		}
	}
	return pos;
}

//===============================================================================
//　Logo内で拡大縮小をまとめた関数
//===============================================================================
void CLogo::Zoom(D3DXVECTOR3 pos, D3DXVECTOR2 EndSize, D3DXVECTOR2 AddSize, bool bZoom)
{
	D3DXVECTOR2 ZoomSize = EndSize;
	if (bZoom == false)
	{	// 縮小の場合
		m_size.x -= AddSize.x;
		m_size.y -= AddSize.y;
		if (m_size.x <= ZoomSize.x) { m_size.x = ZoomSize.x; }
		if (m_size.y <= ZoomSize.y) { m_size.y = ZoomSize.y; }
		CScene2D::SetPosSize(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR2(m_size.x, m_size.y));
	}
	else if (bZoom == true)
	{
		m_size.x += AddSize.x;
		m_size.y += AddSize.y;
		if (m_size.x >= ZoomSize.x) { m_size.x = ZoomSize.x; }
		if (m_size.y >= ZoomSize.y) { m_size.y = ZoomSize.y; }
		CScene2D::SetPosSize(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR2(m_size.x, m_size.y));
	}
}