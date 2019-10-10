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
#include "texture.h"
#include "ranking.h"
#include "select.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CTutorial;
class CXInput;

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
	static MODE GetMode(void) { return m_mode; }
	static CRenderer *GetRenderer(void) { return m_pRenderer; }
	static CInputKeyBoard *GetInput(void) { return m_pInputKeyBoard; }
	static CInputMouse *GetInputMouse(void) { return m_pInputMouse; }
	static CInputJoypad *GetInputJoyPad(void) { return m_pJoyPad; }
	static CXInput *GetXInput(void) { return m_pXInput; }
	//static CCamera *GetCamera(void) { return m_pCamera; }
	static CSound *GetSound(void) { return m_pSound; }
	static CTexture *GetTexture(void) { return m_pTexture; }
	static bool GetInputFlag(void) { return m_bInput; }
	static void SetInputFlag(bool bInput) { m_bInput = bInput; }

private://�l�ł̂ݎg��
	static MODE m_mode;
	static CRenderer *m_pRenderer;
	static CSound *m_pSound;
	static CInputKeyBoard *m_pInputKeyBoard;
	static CInputMouse *m_pInputMouse;
	static CInputJoypad *m_pJoyPad;
	static CXInput	*m_pXInput;
	//static CCamera *m_pCamera;
	static CLight *m_pLight;
	static CDebugProc *m_pDebugProc;
	static CGame *m_pGame;
	static CTitle *m_pTitle;
	static CResult *m_pResult;
	static CTexture *m_pTexture;
	static CRanking *m_pRanking;
	static CSelect	*m_pSelect;
	static CTutorial * m_pTutorial;
	static bool m_bInput;				//���͏�Ԃ̎擾
};
#endif