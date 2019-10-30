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
#define MAX_LINE	(2)
#define COUNT_DOWN	(3)

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
		TEXTURE_COUNT = 0,
		TEXTURE_RANK,
		TEXTURE_GOUL,
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

	CScene2D *m_pLine[MAX_LINE];			// ��
	CScene2D *m_pCountDown[COUNT_DOWN];		// �J�E���g�_�E��
	CScene2D *m_pRanking[MAX_PLAYER];		// �����L���O
	CScene2D *m_pItem[MAX_PLAYER][MAX_EGG];	// �A�C�e��
	CScene2D *m_pGoul[MAX_PLAYER];			// �S�[��

};
#endif