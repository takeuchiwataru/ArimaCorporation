//=============================================================================
//
// �t�F�[�h�̏��� [fade.h]
// Author : �L�n���u
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "scene2D.h"
#include "main.h"
#include "game.h"
#include "manager.h"

//*****************************************************************************
// �}�N����`�@�Ƌ���
//*****************************************************************************
#define	MAX_TYPE	(10)

//=====================
//  CScene2D�̔h���N���X
//=====================
class CFade : public CScene2D
{
public://�N�ł�������
	typedef enum
	{
		FADE_NONE = 0,		//�������Ă��Ȃ����
		FADE_IN,			//�t�F�[�h�C�����
		FADE_OUT,			//�t�F�[�h�A�E�g���
		FADE_MAX			//��Ԃ̍ő吔
	} FADE;

	typedef enum
	{
		MODEFADE_NONE = 0,	//�������Ă��Ȃ����
		MODEFADE_NOLMAL,	//�ʏ���
		MODEFADE_GAME,		//�Q�[���̃t�F�[�h
		MODEFADE_MODE,		//���[�h�̃t�F�[�h
		MODEFADE_MAX		//�t�F�[�h�̍ő吔
	} MODEFADE;

	CFade();
	~CFade();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static FADE GetFade(void) { return m_fade; }
	static CFade *Create(CManager::MODE modeNext);
	static CFade *Create(CGame::GAMEMODE modeNext);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_TEXTURE];	//���L�e�N�X�`���̃|�C���^
	static FADE				  m_fade;					//�t�F�[�h���
	static int				  m_nModeNext;				//���̃��[�h
	static MODEFADE			  m_modegame;				//�t�F�[�h�̃��[�h
	D3DXCOLOR				  m_colorFade;				//�t�F�[�h�F
	D3DXVECTOR2				  m_Size;					//�T�C�Y
	static CGame			  *m_pGame;
	CScene2D *				  m_pNowLoading;			//���[�f�B���O
	CScene2D *				  m_pAnim;					//�A�j���[�V����
	int						  m_nAnimNum;				//�A�j���[�V�����̔ԍ�
	int						  m_nCountAnim;				//�A�j���[�V�����J�E���^�[
};
#endif