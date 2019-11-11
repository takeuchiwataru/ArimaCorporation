//=============================================================================
//
// マネージャー処理 [manager.h]
// Author : 有馬武志
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"
#include "renderer.h"
#include "sound.h"
#include "input.h"
#include "camerabace.h"
#include "light.h"
#include "debugproc.h"
#include "scene2D.h"
#include "game.h"
#include "title.h"
#include "result.h"
#include "ranking.h"
#include "select.h"
#include "Server.h"

//*****************************************************************************
// マクロ宣言
//*****************************************************************************
#define INIT_VECTOR		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CTutorial;

//=====================
//  基本クラス
//=====================
class CManager
{
public://誰でも扱える
	typedef enum
	{
		MODE_TITLE = 0,		//タイトル
		MODE_TUTORIAL,		//チュートリアル
		MODE_SELECT,		// セレクト
		MODE_GAME,			//ゲーム
		MODE_RESULT,		//リザルト
		MODE_RANKING,		//ランキング
		MODE_MAX			//最大数
	}MODE;

	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindows);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void SetMode(MODE mode);
	static MODE GetMode(void) { return m_mode; }
	static CRenderer *GetRenderer(void) { return m_pRenderer; }
	static CInputKeyBoard *GetInput(void) { return m_pInputKeyBoard; }
	static CInputMouse *GetInputMouse(void) { return m_pInputMouse; }
	static CInputJoypad *GetInputJoyPad(void) { return m_pJoyPad; }
	static CInputXPad *GetXInput(void) { return m_pXPad; }
	//static CCamera *GetCamera(void) { return m_pCamera; }
	static CSound *GetSound(void) { return m_pSound; }
	static bool GetInputFlag(void) { return m_bInput; }
	static void SetInputFlag(bool bInput) { m_bInput = bInput; }
	static CInputJoyPad_0 *&GetInputJoyPad0(int nID) { return m_pJoyPad0[nID]; }
	static int			&GetnInput(void) { return m_nInput; }
	static CServer		*&GetServer(void) { return m_pServer; }
	static CClient		*&GetClient(void) { return m_pClient; }

	static bool OnlineSeting(bool bOpen, bool bHost = true);

private://個人でのみ使う
	static MODE m_mode;
	static CRenderer *m_pRenderer;
	static CSound *m_pSound;
	static CInputKeyBoard *m_pInputKeyBoard;
	static CInputMouse *m_pInputMouse;
	static CInputJoypad *m_pJoyPad;
	static CInputXPad	*m_pXPad;
	//static CCamera *m_pCamera;
	static CLight *m_pLight;
	static CDebugProc *m_pDebugProc;
	static CGame *m_pGame;
	static CTitle *m_pTitle;
	static CResult *m_pResult;
	static CRanking *m_pRanking;
	static CSelect	*m_pSelect;
	static CTutorial * m_pTutorial;
	static bool m_bInput;				//入力状態の取得
	static CInputJoyPad_0	*m_pJoyPad0[JOYPAD_MAX];	//ジョイパッドのポインタ
	static int				m_nInput;					//インプットの更新チェック
	static CServer			*m_pServer;		//サーバー
	static CClient			*m_pClient;		//クライアント
};
#endif