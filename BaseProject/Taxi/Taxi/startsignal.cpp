//************************************************************
//
//	スタート合図の処理[startsignal.h]
//	Author : Shun Yokomichi
//
//************************************************************
#include "startsignal.h"
#include "logo.h"
#include "gamecamera.h"

//************************************************************
//	マクロ定義
//************************************************************
#define SIGNAL_POS		(D3DXVECTOR3(480.0f + (320 * nCntSignal), SCREEN_HEIGHT / 2, 0.0f))		// 初期位置
#define PURPOSE_POS		(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f))						// 目的地の位置
#define ADD_MOVE		(8.0f)																			// 移動の加算量
#define ADD_SIZE		(3.0f)																			// サイズの加算量
#define COL_A_REDUCED	(0.01f)																			// 透明度の減算量
#define COL_DEFAULT		(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fColA))											// デフォルトの色
//************************************************************
//	コンストラクタ
//************************************************************
CStartSignal::CStartSignal() 
{
	m_pStart = NULL;	// スタート合図
	m_SignalType = SIGNAL_NONE;	// 合図の状態
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_fColA = 1.0f;
	m_bEnd = false;
	m_bColA = false;
	m_nCntDisplay = 0;
	for (int nCntSignal = 0; nCntSignal < MAX_SIGNAL; nCntSignal++)
	{
		m_pSignal[nCntSignal] = NULL;
	}
}
//************************************************************
//	デストラクタ
//************************************************************
CStartSignal::~CStartSignal() {}

//************************************************************
//	初期化
//************************************************************
HRESULT CStartSignal::Init() 
{
	for (int nCntSignal = 0; nCntSignal < MAX_SIGNAL; nCntSignal++)
	{
		if (m_pSignal[nCntSignal] == NULL)
		{
			m_pSignal[nCntSignal] = CLogo::Create(SIGNAL_POS, m_size,CTexture::TYPE_START_SIGNAL000, 0, CLogo::TYPE_LOGO );
			m_pSignal[nCntSignal]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}
	}

	return S_OK;
}

//************************************************************
//	終了処理
//************************************************************
void CStartSignal::Uninit() 
{
	// スタート合図の破棄
	for (int nCntSignal = 0; nCntSignal < MAX_SIGNAL; nCntSignal++)
	{
		if (m_pSignal[nCntSignal] != NULL)
		{
			m_pSignal[nCntSignal]->Uninit();
			m_pSignal[nCntSignal] = NULL;
		}
	}

	// 開始時のロゴ
	m_pStart = NULL;
}

//************************************************************
//	更新処理
//************************************************************
void CStartSignal::Updata() 
{
	bool bFalse = false;
	if (m_pSignal[0] != NULL) {
		bFalse = m_pSignal[0]->GetMoveFlag();}

	switch (m_SignalType)
	{
	case SIGNAL_HERE:	SignalUpdata(bFalse, (int)m_SignalType);	break;
	case SIGNAL_WE:		SignalUpdata(bFalse, (int)m_SignalType);	break;
	case SIGNAL_GO:		StartUpdata();								break;
	default:			break;
	}
}

//************************************************************
// 描画処理
//************************************************************
void CStartSignal::Draw() 
{
	for (int nCntSignal = 0; nCntSignal < MAX_SIGNAL; nCntSignal++)
	{
		if (m_pSignal[nCntSignal] != NULL)
		{
			m_pSignal[nCntSignal]->Draw();
		}
	}

	if (m_pStart != NULL) { m_pStart->Draw(); }
}

//************************************************************
//	生成処理
//************************************************************
CStartSignal *CStartSignal::Create(D3DXVECTOR2 size, SIGNAL type)
{
	CStartSignal *pStartSignal = NULL;
	CManager::MODE mode = CManager::GetMode();

	//NULLチェック
	if (pStartSignal == NULL)
	{//メモリの動的確保

		pStartSignal = new CStartSignal;

		if (pStartSignal != NULL)
		{
			//サイズの割り当て
			pStartSignal->m_size = size;
			// 合図の状態
			pStartSignal->m_SignalType = type;
			//オブジェクトクラスの生成
			pStartSignal->Init();
		}
	}
	return pStartSignal;
}

//************************************************************
//	スタート合図の処理
//************************************************************
void CStartSignal::SignalUpdata(bool bFrag, int nType)
{
	bool bChange = false;
	if (bFrag == true && nType == 0) { bChange = true; }
	if (bFrag == false && nType == 1) { bChange = true;}

	if (bChange == true) { m_nCntDisplay++; }
	for (int nCntSignal = 0; nCntSignal < MAX_SIGNAL; nCntSignal++)
	{
		if (m_pSignal[nCntSignal] != NULL)
		{
			if (bChange == true)
			{	// 初期位置に戻す & Textureの変更
				if ((m_nCntDisplay % 20) == 0)
				{
					m_pSignal[nCntSignal]->SetPosition(SIGNAL_POS);
					m_pSignal[nCntSignal]->BindTexture(*CTexture::GetTexture(CTexture::TYPE_START_SIGNAL000 + nType + 1));
					if (bFrag == true && nType == 0) { m_SignalType = SIGNAL_WE; }
					if (bFrag == false && nType == 1) {m_SignalType = SIGNAL_GO; }
				}
				else { m_pSignal[nCntSignal]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)); }
			}
			else if (bChange == false)
			{	// 位置移動
				float fMove = ADD_MOVE;
				m_pSignal[nCntSignal]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
				if (nCntSignal == 1) { fMove = ADD_MOVE * -1; }
				D3DXVECTOR3 pos = m_pSignal[nCntSignal]->GetPosition();
				pos = m_pSignal[nCntSignal]->MoveLimit(pos, PURPOSE_POS, nCntSignal, fMove);
				m_pSignal[nCntSignal]->SetPosition(pos);
				m_pSignal[nCntSignal]->Update();
			}
		}
	}

	for (int nCntSignal = 0; nCntSignal < MAX_SIGNAL; nCntSignal++)
	{	// falseの場合は終了処理
		if ((m_nCntDisplay % 20) == 0)
		{
			if (bFrag == false && nType == 1 && m_pSignal[nCntSignal] != NULL) { m_pSignal[nCntSignal]->Uninit(); m_pSignal[nCntSignal] = NULL; }
		}
	}

}

//************************************************************
// ゲーム開始時の合図
//************************************************************
void CStartSignal::StartUpdata()
{
	CGameCamera * pGameCamera = CGame::GetGameCamera();

	if (m_pStart == NULL)
	{
		m_pStart = CLogo::Create(PURPOSE_POS, m_size, CTexture::TYPE_START_SIGNAL002, 0, CLogo::TYPE_LOGO);
	}
	if (m_pStart != NULL)
	{
		m_fColA -= COL_A_REDUCED;
		m_size.x += ADD_SIZE;
		m_size.y += ADD_SIZE / 2;
		m_pStart->SetPosSize(PURPOSE_POS, m_size);
		m_pStart->SetColor(&COL_DEFAULT);
		if (m_fColA <= 0.3f) { m_pStart->Uninit(); m_bEnd = true; m_SignalType = SIGNAL_NONE; m_pStart = NULL; }
	}
}