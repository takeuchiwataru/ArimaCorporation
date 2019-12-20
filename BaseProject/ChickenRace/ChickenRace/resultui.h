//=============================================================================
//
// ���U���gUI�̏��� [resultui.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _RESULTUI_H_
#define _RESULTUI_H_

#include "main.h"
#include "game.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_RANKING	(3)

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene2D;
class CTime;

//=====================
// ��{�N���X
//=====================
class CResultUI : public CScene
{// ���U���gUI
public:
	typedef enum
	{
		TEXTURE_RANK = 0,
		TEXTURE_CHAR,
		TEXTURE_ICON,
		TEXTURE_PRESS,
		TEXTURE_MAX
	}TEXTURE;

	CResultUI();								//�R���X�g���N�^
	~CResultUI();								//�f�X�g���N�^

	static HRESULT Load(void);
	static void Unload(void);
	static CResultUI *Create(void);

	HRESULT Init();								//����������
	void Uninit(void);							//�I������
	void Update(void);							//�X�V����
	void Draw(void);							//�`�揈��

private:
	void Title(void);
	void Game(void);
	void Result(void);

	void LoadRanking(void);
	void SaveRanking(void);
	void SortRanking(void);

	static LPDIRECT3DTEXTURE9	m_pTexture[TEXTURE_MAX];	// �e�N�X�`���ւ̃|�C���^

	CScene2D			*m_pRankBG[MAX_MEMBER];	// �����NBG
	CScene2D			*m_pRank[MAX_MEMBER];	// �����N
	CScene2D			*m_pChar[MAX_MEMBER];	// �L����
	CScene2D			*m_pPlayer[MAX_MEMBER];	// �v���C���[
	CTime				*m_pTime[MAX_MEMBER];	// �^�C��
	CScene2D			*m_pPress;				// �v���X
	CScene2D			*m_pFade;				// �t�F�[�h

	int					m_nCharacter[MAX_RANKING];// �L�����N�^�[
	int					m_nRanking[MAX_RANKING];// �����L���O
};
#endif