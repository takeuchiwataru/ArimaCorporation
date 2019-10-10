//=============================================================================
//
// 評判の処理 [reputation.cpp]
// Author : shun yokomichi
//
//=============================================================================
#include "scene.h"
#include "scene2D.h"
#include "reputation.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "game.h"
#include "logo.h"
#include "texture.h"
#include "satisfactionlevel.h"
#include "tutorial.h"
#include "time.h"
#//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define DESTINATION_POS			(D3DXVECTOR3(180.0f, 300.0f, 0.0f))			// 表示位置
#define DESTINATION_SIZE		(D3DXVECTOR2(150.0f, 100.0f))				// サイズ
#define BACK_POS_START			(D3DXVECTOR3(170.0f, 900.0f, 0.0f))			// 表示位置(背景)
#define BACK_POS				(D3DXVECTOR3(170.0f, 570.0f, 0.0f))			// 表示位置(背景)
#define BACK_SIZE				(D3DXVECTOR2(180.0f, 150.0f))				// サイズ(背景)
#define FRAME_INTERVAL			(54.0f * (m_nCntNum - nCntTex - 1))			// 間隔(フレーム)
#define FRAME_MOVE				(10.0f)										// 移動速度(フレーム)
#define FRAME_POS				(D3DXVECTOR3(BACK_POS.x, 560.0f, 0.0f))		// 表示位置(フレーム)
#define FRAME_SIZE				(D3DXVECTOR2(150.0f, 25.0f))				// サイズ(フレーム)
#define DISPLAY_TIME			(35)										// 1回に表示する時間
#define DISPLAY_NUM				(4)											// 表示する回数
#define COMMENT_POS				(D3DXVECTOR3(BACK_POS.x, FRAME_POS.y + 10.0f, 0.0f))	// 表示位置(コメント)
#define COMMENT_SIZE			(D3DXVECTOR2(100.0f, 20.0f))				// サイズ位置(コメント)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
int CReputation::m_nNextType = 0;
int CReputation::m_nNextFont = 0;

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CReputation::CReputation() : CScene2D(5, CScene::OBJTYPE_LOGO)//優先優位イニシャライズ
{
	m_size = D3DXVECTOR2(0.0f, 0.0f);

	m_fLength = 0.0f;
	m_fAngle = 0.0f;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntTime = 0;
	m_nStateFont = 0;
	m_nStateType = 0;
	m_pBack = NULL;
	m_nCntNum = 0;
	m_nTexType = 1;
	m_bUninitFlag = false;
	m_nCntUninitTime = 0;
	for (int nCntTex = 0; nCntTex < MAX_FRAME; nCntTex++)
	{
		m_apFrame[nCntTex] = NULL;
		m_apComment[nCntTex] = NULL;
		m_nCommentNum[nCntTex] = -1;
	}
}
//===============================================================================
//　デストラクタ
//===============================================================================
CReputation::~CReputation()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CReputation::Init(void)
{
	CScene2D::Init();

	// 背景の生成
	if (m_pBack == NULL)
	{
		m_pBack = CLogo::Create(BACK_POS_START, BACK_SIZE, CTexture::TYPE_IPHONE, 0, CLogo::TYPE_LOGO);
		m_pBack->SetTexture(m_nTexType, 2, 1, 1);
		m_pBack->SetMove(D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	}
	Satisfaction();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CReputation::Uninit(void)
{
	// 背景の破棄
	if (m_pBack != NULL) { m_pBack->Uninit(); m_pBack = NULL; }

	// フレームの破棄
	for (int nCntTex = 0; nCntTex < MAX_FRAME; nCntTex++)
	{
		if (m_apFrame[nCntTex] != NULL) { m_apFrame[nCntTex]->Uninit(); m_apFrame[nCntTex] = NULL; }
	}
	// コメントの破棄
	for (int nCntTex = 0; nCntTex < MAX_FRAME; nCntTex++)
	{
		if (m_apComment[nCntTex] != NULL) { m_apComment[nCntTex]->Uninit(); m_apComment[nCntTex] = NULL; }
	}

	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CReputation::Update(void)
{
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();

	//位置の割り当て
	D3DXVECTOR3 pos;
	pos = GetPosition();

	//移動量の割り当て
	D3DXVECTOR3 move;
	move = GetMove();

	m_nCntTime++;

	if (m_nTexType == 0) { FrameDisplay();} // フレーム表示

	//ロゴの位置とサイズ
	//CScene2D::SetPosSize(pos, m_size);

	if (m_pBack != NULL)
	{
		D3DXVECTOR3 pos = m_pBack->GetPosition();
		if (m_bUninitFlag == false)
		{
			pos = m_pBack->MoveLimit(m_pBack->GetPosition(),BACK_POS, 2, -6.0f);
			if (pos.y == BACK_POS.y) { m_nTexType = 0; }
		}
		else if (m_bUninitFlag == true)
		{
			pos = m_pBack->MoveLimit(m_pBack->GetPosition(), BACK_POS_START, 3, 6.0f);
			if (pos.y == BACK_POS_START.y) { SetLevel(); return; }
		}
		m_pBack->SetPosition(pos);
		m_pBack->SetTexture(m_nTexType, 2, 1, 1);
		m_pBack->Update();
	}
	for (int nCntTex = 0; nCntTex < MAX_FRAME; nCntTex++)
	{
		if (m_apFrame[nCntTex] != NULL) { m_apFrame[nCntTex]->Update(); }
		if (m_apComment[nCntTex] != NULL) { m_apComment[nCntTex]->Update(); }
	}

	if (m_nCntNum >= DISPLAY_NUM - 1)
	{	// 表示しない
		m_nCntUninitTime++;
		if ((m_nCntUninitTime % 60) == 0)
		{
			// コメントの破棄
			for (int nCntTex = 0; nCntTex < MAX_FRAME; nCntTex++)
			{
				if (m_apComment[nCntTex] != NULL) { m_apComment[nCntTex]->Uninit(); m_apComment[nCntTex] = NULL; }
				if (m_apFrame[nCntTex] != NULL) { m_apFrame[nCntTex]->Uninit(); m_apFrame[nCntTex] = NULL; }
			}
			m_bUninitFlag = true;
			m_nTexType = 1;
		}
	}

	if (CTime::GetTime == 0) { SetLevel(); }
}
//=============================================================================
// 描画処理
//=============================================================================
void CReputation::Draw(void)
{
	//入力情報
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputJoypad *pCInputJoyPad = CManager::GetInputJoyPad();
	if (m_pBack != NULL) { m_pBack->Draw(); }
	for (int nCntTex = 0; nCntTex < MAX_FRAME; nCntTex++)
	{
		if (m_apFrame[nCntTex] != NULL) { m_apFrame[nCntTex]->Draw(); }
		if (m_apComment[nCntTex] != NULL) { m_apComment[nCntTex]->Draw(); }
	}

	CScene2D::Draw();

}
//=============================================================================
// フレーム表示
//=============================================================================
void CReputation::FrameDisplay(void)
{
	if ((m_nCntTime % DISPLAY_TIME) == 0)
	{	// 表示時間が過ぎたら
		if (m_apFrame[m_nCntNum] == NULL && m_nCntNum < DISPLAY_NUM)
		{	// 生成
			m_apFrame[m_nCntNum] = CLogo::Create(D3DXVECTOR3(0.0f, FRAME_POS.y, FRAME_POS.z), FRAME_SIZE, CTexture::TYPE_COMMENT_FREAME, 0, CLogo::TYPE_LOGO);
			m_apComment[m_nCntNum] = CLogo::Create(D3DXVECTOR3(0.0f, COMMENT_POS.y, COMMENT_POS.z), COMMENT_SIZE, CTexture::TYPE_COMMENT000 + (int)m_Type, 0, CLogo::TYPE_LOGO);
			int nTexNum = 0;
			int nNum = 0;
			do
			{
				nNum = 0;
				nTexNum = rand() % 4 + 0;
				if (m_nCommentNum[0] != nTexNum) { nNum++;}
				if (m_nCommentNum[1] != nTexNum) { nNum++;}
				if (m_nCommentNum[2] != nTexNum) { nNum++;}

			} while (nNum <= 2);
			m_nCommentNum[m_nCntNum] = nTexNum;
			m_apComment[m_nCntNum]->SetTexture(nTexNum, 1, 4, 1);
			m_nCntNum++;	// 加算
		}
	}

	for (int nCntTex = 0; nCntTex < MAX_FRAME; nCntTex++)
	{
		if (m_apFrame[nCntTex] != NULL && m_apComment[nCntTex] != NULL)
		{
			D3DXVECTOR3 pos = m_apFrame[nCntTex]->GetPosition();	// 位置取得
			D3DXVECTOR3 posComment = m_apComment[nCntTex]->GetPosition();	// 位置取得
			pos.x += FRAME_MOVE;
			posComment.x += FRAME_MOVE;
			if (pos.x >= FRAME_POS.x) { pos.x = FRAME_POS.x; }
			if (posComment.x >= COMMENT_POS.x) { posComment.x = COMMENT_POS.x; }
			m_apFrame[nCntTex]->SetPosition(D3DXVECTOR3(pos.x, FRAME_POS.y + FRAME_INTERVAL, FRAME_POS.z));
			m_apComment[nCntTex]->SetPosition(D3DXVECTOR3(posComment.x, COMMENT_POS.y + FRAME_INTERVAL, COMMENT_POS.z));
		}
	}
}
//=============================================================================
// 評価決め
//=============================================================================
void CReputation::Satisfaction(void)
{
	// 順に高い評価に( 評価はMAX20)
	if (m_nSatisfaction <= -1)
	{//	　送迎失敗　評価　-1
		m_Type = REPUTATION_FAIL;
	}
	else if (m_nSatisfaction <= 0)
	{//	0秒以下　
		m_Type = REPUTATION_ONE;
	}
	else if (m_nSatisfaction <= 1 && m_nSatisfaction > 0)
	{//　0秒以上　3秒以下　
		m_Type = REPUTATION_TWO;
	}
	else if (m_nSatisfaction <= 2 && m_nSatisfaction > 1)
	{//　3秒以上　6秒以内　
		m_Type = REPUTATION_THREE;
	}
	else if (m_nSatisfaction <= 3 && m_nSatisfaction > 2)
	{//	6秒以上 10秒以内　　
		m_Type = REPUTATION_FOUR;
	}
}

//=============================================================================
// 星の加算
//=============================================================================
void CReputation::SetLevel(void)
{
	CSatisfaction *pSatisfaction = CGame::GetSatisfaction();
	CManager::MODE mode = CManager::GetMode();

	if (mode == CManager::MODE_GAME)
	{//ゲーム
		pSatisfaction = CGame::GetSatisfaction();
	}
	else if (mode == CManager::MODE_TUTORIAL)
	{//チュートリアル
		pSatisfaction = CTutorial::GetSatisfaction();
	}

	// 順に高い評価に( 評価はMAX20)
	if (m_Type == REPUTATION_FAIL) { pSatisfaction->Subtraction(7); }
	if (m_Type == REPUTATION_ONE) { pSatisfaction->Subtraction(4); }
	if (m_Type == REPUTATION_TWO) { pSatisfaction->AddLevel(2); }
	if (m_Type == REPUTATION_THREE) { pSatisfaction->AddLevel(3); }
	if (m_Type == REPUTATION_FOUR) { pSatisfaction->AddLevel(4); }

	Uninit();
	
	return;
}
//===============================================================================
//　クリエイト
//===============================================================================
CReputation * CReputation::Create(int nSatisfaction)
{
	CReputation *pDestination = NULL;

	//NULLチェック
	if (pDestination == NULL)
	{//メモリの動的確保

		pDestination = new CReputation;

		if (pDestination != NULL)
		{
			//Textureの種類
			pDestination->m_nSatisfaction = nSatisfaction;
			//オブジェクトクラスの生成
			pDestination->Init();
			//位置を割り当てる
			pDestination->SetPosition(DESTINATION_POS);
			//サイズの割り当て
			pDestination->m_size = DESTINATION_SIZE;
		}
	}

	return pDestination;
}
//===============================================================================
//　状態の優先順位
//===============================================================================
void CReputation::SetLogoState(int nStateType, int nStateFont)
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
