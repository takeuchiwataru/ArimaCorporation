//=============================================================================
//
// �Q�[���i�v���C�j�̏��� [game.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _GAMEPLAY_H_
#define _GAMEPLAY_H_

#include "main.h"
#include "game.h"
#include "scene.h"
#include "player.h"

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
class CGamePlay : public CScene
{// �Q�[���i�v���C�j
public:
	typedef enum
	{
		TEXTURE_RANK = 0,
		TEXTURE_MAX
	}TEXTURE;

	CGamePlay();							//�R���X�g���N�^
	~CGamePlay();							//�f�X�g���N�^

	static HRESULT Load(void);
	static void Unload(void);
	static CGamePlay *Create(void);

	HRESULT Init();							//����������
	void Uninit(void);						//�I������
	void Update(void);						//�X�V����
	void Draw(void);						//�`�揈��

private:
	static LPDIRECT3DTEXTURE9	m_pTexture[TEXTURE_MAX];	// �e�N�X�`���ւ̃|�C���^

	CScene2D *m_pRanking[MAX_PLAYER];		// �����L���O
	CScene2D *m_pItem[MAX_PLAYER][MAX_EGG];	// �A�C�e��

};
#endif