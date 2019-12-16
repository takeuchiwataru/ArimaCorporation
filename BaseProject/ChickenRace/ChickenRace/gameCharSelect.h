//=============================================================================
//
// �Q�[���i�L�����I���j�̏��� [game.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _GAMECHARSELECT_H_
#define _GAMECHARSELECT_H_

#include "main.h"
#include "game.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_HINT	(2)

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene2D;

//=====================
// ��{�N���X
//=====================
class CGameCharSelect : public CScene
{// �Q�[���i�L�����I���j
public:
	typedef enum
	{
		TEXTURE_CHAR = 0,
		TEXTURE_FRAME,
		TEXTURE_NUMBER,
		TEXTURE_ENTER,
		TEXTURE_BUTTON,
		TEXTURE_ACTION,
		TEXTURE_YOU,
		TEXTURE_READY,
		TEXTURE_TUTORIAL,
		TEXTURE_HINT,
		TEXTURE_RETURN_BG,
		TEXTURE_RETURN_TREE,
		TEXTURE_RETURN_HOME,
		TEXTURE_RETURN_CHAR,
		TEXTURE_RETURN_GRASS,
		TEXTURE_MAX
	}TEXTURE;

	CGameCharSelect();						//�R���X�g���N�^
	~CGameCharSelect();						//�f�X�g���N�^

	static HRESULT Load(void);
	static void Unload(void);
	static CGameCharSelect *Create(void);

	HRESULT Init();							//����������
	void Uninit(void);						//�I������
	void Update(void);						//�X�V����
	void Draw(void);						//�`�揈��

private:
	void Online(void);
	void Local(void);
	void Tutorial(void);

	static LPDIRECT3DTEXTURE9	m_pTexture[TEXTURE_MAX];	// �e�N�X�`���ւ̃|�C���^

	CScene2D *m_pPlayerNum[MAX_PLAYER];		// �v���C���[�ԍ�
	CScene2D *m_pPlayerBG[MAX_PLAYER];		// �v���C���[�w�i
	CScene2D *m_pSelect[MAX_PLAYER];		// �I��
	CScene2D *m_pEnter[MAX_PLAYER];			// ����
	CScene2D *m_pButton[MAX_PLAYER];		// �{�^��

	CScene2D *m_pAction;					// �A�N�V����

	CScene2D *m_pYor;						// ���Ȃ�
	CScene2D *m_pReturnFrame;				// �߂�t���[��
	CScene2D *m_pReturnButton;				// �߂�
	CScene2D *m_pReturnBG;					// �߂�w�i
	CScene2D *m_pReturnTree;				// �߂��
	CScene2D *m_pReturnHome;				// �߂鏬��
	CScene2D *m_pReturnChar;				// �߂�L����
	CScene2D *m_pReturnGrass;				// �߂鑐
	int		m_nReturnCounter;				// �߂�J�E���g

	CScene2D *m_pReady;						// ����
	bool	m_bReady;						// ����

	CScene2D *m_pTutorial;					// �`���[�g���A��
	CScene2D *m_pHint[MAX_HINT];			// �q���g
	CScene2D *m_pGo;						// Go

	int		m_nTutorialNum;					// �`���[�g���A���ԍ�
	int		m_nTutorialCounter;				// �`���[�g���A���J�E���g

	bool	m_bEntry[MAX_PLAYER];			// �G���g���[
	bool	m_bEnter[MAX_PLAYER];			// ���肵����

	int		m_nEntryCounter;				// �G���g���[�J�E���g

	CScene2D *m_pCharacter[MAX_CHARCTER];
};
#endif