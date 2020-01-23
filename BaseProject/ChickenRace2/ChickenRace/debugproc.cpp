//=============================================================================
//
// デバック用フォント処理 [debugproc.cpp]
// Author : 有馬武志
//
//=============================================================================
#include "debugproc.h"
#include "manager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
char CDebugProc::m_aStn[MAX_FONT] = "";
LPD3DXFONT  CDebugProc::m_pFont = NULL;

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CDebugProc::CDebugProc()
{

}
//===============================================================================
//　デストラクタ
//===============================================================================
CDebugProc::~CDebugProc()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
void CDebugProc::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// デバッグ情報表示用フォントの生成
	D3DXCreateFont(pDevice, 25, 0, 0, 0, FALSE, MONO_FONT,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Arial Black", &m_pFont);
}
//=============================================================================
// 終了処理
//=============================================================================
void CDebugProc::Uninit(void)
{
	// デバッグ情報表示用フォントの破棄
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
}
//=============================================================================
// プリントの処理
//=============================================================================
void CDebugProc::Print(char *fmt, ...)
{
#ifdef _DEBUG
	va_list argp;
	char cString[256];
	cString[0] = '\0';

	int nBit1;
	int nBit2;

	//文字のスタート
	va_start(argp, fmt);

	//入る文字をprintfに入れる
	vsprintf(cString, fmt, argp);

	//バイトに変換
	nBit1 = (int)strlen(m_aStn);		//現在の文字
	nBit2 = (int)strlen(cString);	//新しく入る文字

	//現在と新がMAX_FONTを超えていない場合
	if (nBit1 + nBit2 <= MAX_FONT)
	{
		//改行する場合
		strcat(m_aStn, cString);
	}

	//文字の終了
	va_end(argp);
#endif

}
//=============================================================================
// 更新処理
//=============================================================================
void CDebugProc::Draw(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH,SCREEN_HEIGHT };

	// テキスト描画
	m_pFont->DrawText
	(
		NULL,
		&m_aStn[0],							//入る文字 
		-1,									//表示する文字のバイト数
		&rect,								//表示範囲
		DT_LEFT,							//表示範囲の調整
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)	//色
	);

	//文字をない状態にする
	m_aStn[0] = '\0';
}