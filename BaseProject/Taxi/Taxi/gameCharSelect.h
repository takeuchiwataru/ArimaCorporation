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
	CScene2D *m_pPlayerBG[MAX_PLAYER];
	CScene2D *m_pCharacter[MAX_CHARCTER];

};
#endif