//=============================================================================
//
// �}�l�[�W���[���� [manager.h]
// Author : �L�n���u
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
#include "select.h"
#include "Server.h"

//*****************************************************************************
// �}�N���錾
//*****************************************************************************
#define INIT_VECTOR		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
#define INIT_COL		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CTutorial;

//=====================
//  ��{�N���X
//=====================
class CManager
{
public://�N�ł�������
	typedef enum
	{
		MODE_TITLE = 0,		//�^�C�g��
		MODE_TUTORIAL,		//�`���[�g���A��
		MODE_SELECT,		// �Z���N�g
		MODE_GAME,			//�Q�[��
		MODE_RESULT,		//���U���g
		MODE_RANKING,		//�����L���O
		MODE_MAX			//�ő吔
	}MODE;

	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindows);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void SetMode(MODE mode);
	static bool GetAging(void) { return m_bAging; }
	static void AgingCouneter(void) { m_nAgingCounter++; }
	static int GetAgingCouneter(void) { return m_nAgingCounter; }
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

private://�l�ł̂ݎg��
	static MODE m_mode;
	static bool m_bAging;
	static int	m_nAgingCounter;
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
	static CSelect	*m_pSelect;
	static CTutorial * m_pTutorial;
	static bool m_bInput;				//���͏�Ԃ̎擾
	static CInputJoyPad_0	*m_pJoyPad0[JOYPAD_MAX];	//�W���C�p�b�h�̃|�C���^
	static int				m_nInput;					//�C���v�b�g�̍X�V�`�F�b�N
	static CServer			*m_pServer;		//�T�[�o�[
	static CClient			*m_pClient;		//�N���C�A���g
};
#endif