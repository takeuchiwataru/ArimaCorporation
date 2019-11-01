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
		TEXTURE_PRESS = 0,
		TEXTURE_MAINMENU,
		TEXTURE_MAX
	}TEXTURE;

	CTitleMenu();							//�R���X�g���N�^
	~CTitleMenu();							//�f�X�g���N�^

	static HRESULT Load(void);
	static void Unload(void);
	static CTitleMenu *Create(void);

	HRESULT Init();							//����������
	void Uninit(void);						//�I������
	void Update(void);						//�X�V����
	void Draw(void);						//�`�揈��

private:
	void ControllTitle(void);
	void ControllMainMenu(void);
	void ControllSubMenu(void);
	void ControllEntry(void);

	void EditPress(void);
	void EditMainMenu(void);
	void EditSubMenu(void);


	static LPDIRECT3DTEXTURE9	m_pTexture[TEXTURE_MAX];	// �e�N�X�`���ւ̃|�C���^

	CScene2D			*m_pPress;			// �v���X
	int					m_nCntPress;		// �v���X�J�E���g

	CScene2D			*m_pMainMenu[MAX_MAINMENU];		// ���C�����j���[
	int					m_nCntMainMenu;					// ���C�����j���[�J�E���g

	int					m_nSelectNum;		// �I��ԍ�

	bool				m_bSubMenu;			// �T�u���j���[

	bool				m_bEntry;			// ��t��
};
#endif