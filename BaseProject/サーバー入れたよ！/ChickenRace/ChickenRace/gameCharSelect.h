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
	static LPDIRECT3DTEXTURE9	m_pTexture[TEXTURE_MAX];	// �e�N�X�`���ւ̃|�C���^

	CScene2D *m_pPlayerNum[MAX_PLAYER];		// �v���C���[�ԍ�
	CScene2D *m_pPlayerBG[MAX_PLAYER];		// �v���C���[�w�i
	CScene2D *m_pSelect[MAX_PLAYER];		// �I��
	CScene2D *m_pEnter[MAX_PLAYER];			// ����

	bool	m_bEntry[MAX_PLAYER];			// �G���g���[
	bool	m_bEnter[MAX_PLAYER];			// ���肵����

	int		m_nEntryCounter;				// �G���g���[�J�E���g

	CScene2D *m_pCharacter[MAX_CHARCTER];	
};
#endif