//=============================================================================
//
// �^�C�g���i���j���[�j�̏��� [titlemenu.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _TITLEMENU_H_
#define _TITLEMENU_H_

#include "main.h"
#include "game.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_MAINMENU	(3)
#define MAX_SUBMENU		(3)
#define MAX_FLAG		(2)
#define MAX_CHICKS		(2)
#define MAX_PIYO		(4)

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene2D;

//=====================
// ��{�N���X
//=====================
class CTitleMenu : public CScene
{// �^�C�g���i���j���[�j
public:
	typedef enum
	{
		TEXTURE_LOGO = 0,
		TEXTURE_EGG,
		TEXTURE_FLAG,
		TEXTURE_PIYO,
		TEXTURE_PRESS,
		TEXTURE_MENU,
		TEXTURE_NAME_M,
		TEXTURE_NAME_S,
		TEXTURE_INPUT,
		TEXTURE_RETURN,
		TEXTURE_ENTRY,
		TEXTURE_HINT,
		TEXTURE_NUM,
		TEXTURE_SETING,
		TEXTURE_SEARCH,
		TEXTURE_MAX
	}TEXTURE;

	CTitleMenu();								//�R���X�g���N�^
	~CTitleMenu();								//�f�X�g���N�^

	static HRESULT Load(void);
	static void Unload(void);
	static CTitleMenu *Create(void);

	HRESULT Init();								//����������
	void Uninit(void);							//�I������
	void Update(void);							//�X�V����
	void Draw(void);							//�`�揈��

private:
	void ControllTitle(void);
	void ControllMainMenu(void);
	void ControllSubMenu(void);
	void ControllEntry(void);

	void EditLogo(void);
	void EditEgg(void);
	void EditChicken(void);
	void EditFlag(void);
	void EditChicks(void);
	void EditPIYO(void);
	void EditChicksHome(void);
	void EditPress(void);
	void EditMenuBG(void);
	void EditMainMenu(void);
	void EditSubMenu(void);
	void EditEntry(void);

	static LPDIRECT3DTEXTURE9	m_pTexture[TEXTURE_MAX];	// �e�N�X�`���ւ̃|�C���^

	CScene2D			*m_pLogo;				// ���S
	CScene2D			*m_pEgg;				// ��
	CScene2D			*m_pChicken;			// �`�L��
	CScene2D			*m_pFlag[MAX_FLAG];		// ��
	CScene2D			*m_pChicks[MAX_CHICKS];	// �Ђ悱
	CScene2D			*m_pPIYO[MAX_PIYO];		// �s��
	CScene2D			*m_pChicksHome;			// �Ђ悱��
	int					m_nCntLogo;				// ���S�J�E���g

	CScene2D			*m_pPress;				// �v���X
	int					m_nCntPress;			// �v���X�J�E���g

	CScene2D			*m_pMenuBG;				// ���j���[BG

	CScene2D			*m_pMainMenu[MAX_MAINMENU];		// ���C�����j���[
	CScene2D			*m_pNameMenu[MAX_MAINMENU];		// ���j���[��
	int					m_nCntMainMenu;					// ���C�����j���[�J�E���g

	int					m_nSelectNum;			// �I��ԍ�

	bool				m_bSubMenu;				// �T�u���j���[

	CScene2D			*m_pEntry;				// ��t��
	CScene2D			*m_pHint;				// ����
	CScene2D			*m_pMember[MAX_PLAYER];	// �l��
	CScene2D			*m_pEnter;				// ����
	CScene2D			*m_pCancel;				// �L�����Z��
	CScene2D			*m_pSearch;				// �T�[�`
	bool				m_bEntry;				// ��t��
	int					m_nCntEntry;			// ��t���J�E���g
	bool				m_bSearch;				// �T�[�`
};
#endif