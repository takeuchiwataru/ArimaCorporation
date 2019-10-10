//*****************************************************************************
//
//		�e�N�X�`���̏���[texture.h]
//		Auther : shun yokomichi
//
//*****************************************************************************
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene2D.h"	// 2D�|���S��

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
//		�}�N����`
//*****************************************************************************

//*****************************************************************************
//		�N���X��`(�|���S��)
//*****************************************************************************
class CTexture
{
public:
	typedef enum
	{
		//================
		// �Q�[����
		//================
		TYPE_GEARCHANGE = 0,	// �M�A�ؑ�
		TYPE_GAMETIMELOGO,		// ��������(���S)
		TYPE_TOTALSCORELOGO,	// �g�[�^���X�R�A(���S)
		TYPE_SCORELOGO,			// �X�R�A(���S)
		TYPE_FREAME,			// �X�R�A�֌W�̃t���[��
		TYPE_MONEY_TEN,			// \�}�[�N & .�}�[�N
		TYPE_PLUS_MARK,			// +�̃}�[�N
		TYPE_TIME_UP,			// �^�C���A�b�v�̃��S
		TYPE_BOARDING,			// ��Ԃ̃��S

		//================
		// �R���{�֌W
		//================
		TYPE_COMBO,				// �R���{(���S)
		TYPE_THROUGH,			// �R���{000
		TYPE_JUMP,				// �R���{001
		TYPE_DRIFT,				// �R���{002

		//=================
		// �����x
		//=================
		TYPE_SATISFACTION,		// �����x�̐�
		TYPE_COMMENT_FREAME,	// �R�����g�̃t���[��
		TYPE_IPHONE,			// iPhone
		TYPE_COMMENT000,		// �R�����g000
		TYPE_COMMENT001,		// �R�����g001
		TYPE_COMMENT002,		// �R�����g002
		TYPE_COMMENT003,		// �R�����g003
		TYPE_COMMENT004,		// �R�����g004
		TYPE_COMMENT005,		// �R�����g005
		TYPE_GOOD,				// �X�}�C��
		TYPE_BAD,				// �_�E��

		//=================
		// �ړI�n�ʐ^
		//=================
		TYPE_PLACE_SAPPRO,		// �D�y
		TYPE_PLACE_MAIN_STREET_OR_TV_TOWER,	// ��ʂ� or �e���r��
		TYPE_PLACE_CLOCKTOWER,	// ���v��
		TYPE_PLACE_TANUKIKOJI,	// �K���H
		TYPE_PLACE_NORVESA,		// �m���x�T
		TYPE_PLACE_RED_BRICK,	// �ԃ����K
		TYPE_PLACE_SUSUKI_SRREET,// �������̒ʂ�
		TYPE_PLACE_FOUNTAIN,	// ����
		TYPE_PLACE_TAPIOCAR,	// �^�s�I�J�\
		TYPE_PLACE_HOSPITAL,	// �a�@

		//=================
		// �ړI�n��
		//=================
		TYPE_NAME_SAPPRO,		// �D�y
		TYPE_NAME_MAIN_STREET,	// ��ʂ�
		TYPE_NAME_MAIN_TV_TOWER,// �e���r��
		TYPE_NAME_CLOCKTOWER,	// ���v��
		TYPE_NAME_TANUKIKOJI,	// �K���H
		TYPE_NAME_NORVESA,		// �m���x�T
		TYPE_NAME_RED_BRICK,	// �ԃ����K
		TYPE_NAME_SUSUKI_SRREET,// �������̒ʂ�
		TYPE_NAME_FOUNTAIN,		// ����
		TYPE_NAME_TAPIOCAR,		// �^�s�I�J�\
		TYPE_NAME_HOSPITAL,		// �a�@

		//=================
		// ���߃��S
		//=================
		TYPE_COMMAND_000,		// �}�[�N���ŎԂ��~�߂�
		TYPE_COMMAND_001,		// �u���[�L

		//=================
		// ����
		//=================
		TYPE_TIME_NUMBER,		// ���Ԃ̐���
		TYPE_SCORE_NUMBER,		// �X�R�A�̐���

		//=================
		// 2D�w�i
		//=================
		TYPE_BACKGROUND000,		// �^�C�g����ʔw�i
		TYPE_BACKGROUND001,		// �Z���N�g or �����L���O��ʔw�i

		//=================
		// �^�C�g��
		//=================
		TYPE_TITLELOGO000,		// �^�C�g���̃��S(CrazyTaxi)
		TYPE_TITLELOGO001,		// �^�C�g���̃��S(in Japan)
		TYPE_TITLE_PRESS,		// Press Start Button�̃��S
		TYPE_SEASONEFFECT,		// �l�G�̃G�t�F�N�g�p

		//=================
		// ���U���g
		//=================
		TYPE_RESLUT_LOGO000,	// ���U���g�̃��S(RESULT)
		TYPE_RESLUT_LOGO001,	// ���U���g�̃��S(�g�[�^���X�R�A�\��)
		TYPE_RESULT_LOGO002,	// ���U���g�̃��S(�����x�̕\��)
		TYPE_RESULT_BUTTON,		// ���U���g�̃��S(����{�^��)

		//=================
		// �����L���O
		//=================
		TYPE_RANKING_LOGO000,	// �����L���O�̃��S(Ranking)
		TYPE_RANKING_RANK,		// ���ʂ̃��S
		TYPE_RANKING_CLASS,		// �N���X�̃��S
		TYPE_RANKING_TOTALLOGO,	// �g�[�^���X�R�A�̃��S

		//=================
		// �Z���N�g
		//=================
		TYPE_SELECT_LOGO,		// �Z���N�g�̃��S
		TYPE_SELECT_CIRCLE,		// �Z���N�g���̃T�[�N��
		TYPE_SELECT_HIGH_SCORE,	// �n�C�X�R�A
		TYPE_SELECT_GAME,		// �Q�[����ʂ̉摜
		TYPE_SELECT_QUIT,		// �^�C�g���ɂ��ǂ�
		TYPE_SELECT_BUTTON,		// Prsee Button
		TYPE_SELECT_HUMAN000,
		TYPE_SELECT_HUMAN001,
		TYPE_SELECT_HUMAN002,
		TYPE_SELECT_HUMAN003,
		TYPE_SELECT_EASY,
		TYPE_SELECT_HARD,
		TYPE_SELECT_BACK,


		//=================
		// �|�[�Y
		//=================
		TYPE_PAUSE_ICON,		// �I�����A�C�R��
		TYPE_PAUSE_PRESS_BUTTON,// PRESS BUTTON
		TYPE_PAUSE_BG,			// �|�[�Y���̔w�i
		TYPE_PAUSE_CONTINUE,	// �R���e�B�j���[
		TYPE_PAUSE_RETRY,		// ���g���C
		TYPE_PAUSE_QUIT,		// �^�C�g���ɂ��ǂ�
		TYPE_PAUSE_LOGO,		// �|�[�Y�̃��S
		TYPE_PAUSE_OPERATION,	// ������@

		//=================
		// �`���[�g���A��
		//=================
		TYPE_TUTORIAL_FLAM,				// �t���[��
		TYPE_TUTORIAL_LOGO,				// �`���[�g���A�����S
		TYPE_TUTORIAL_SKIP_KEYBOAD,		// �X�L�b�v�L�[(�L�[�{�[�h)
		TYPE_TUTORIAL_SKIP_PAD,			// �X�L�b�v�L�[(�Q�[���p�b�h)
		TYPE_TUTORIAL_ICON,				// �A�C�R��
		TYPE_TUTORIAL_ENTER_KEYBOAD,	// �G���^�[�L�[(�L�[�{�[�h)
		TYPE_TUTORIAL_ENTER_PAD,		// �G���^�[�L�[(�Q�[���p�b�h)
		TYPE_TUTORIAL_STOP,				// �X�g�b�v�摜
		TYPE_TUTORIAL_GO,				// �ړI�̉摜
		TYPE_TUTORIAL_R1,				// R�P�{�^��
		TYPE_TUTORIAL_L1,				// L1�{�^��
		TYPE_TUTORIAL_A_PAD,			// A�{�^��
		TYPE_TUTORIAL_B_PAD,			// B�{�^��
		TYPE_TUTORIAL_LEFT,				// ���ɓ|��
		TYPE_TUTORIAL_RIGHT,			// �E�ɓ|��
		TYPE_TUTORIAL_A_KEY,			// A�{�^��
		TYPE_TUTORIAL_D_KEY,			// D�{�^��
		TYPE_TUTORIAL_K_KEY,			// K�{�^��
		TYPE_TUTORIAL_L_KEY,			// L�{�^��
		TYPE_TUTORIAL_S_KEY,			// S�{�^��
		TYPE_TUTORIAL_W_KEY,			// W�{�^��

		//=================
		// �X�^�[�g���}
		//=================
		TYPE_START_SIGNAL000,	// �X�^�[�g���}(HERE)
		TYPE_START_SIGNAL001,	// �X�^�[�g���}(WE)
		TYPE_START_SIGNAL002,	// �X�^�[�g���}(GO!)

		//=================
		// �t�F�[�hl
		//=================
		TYPE_FADE_NOWLOADING,
		TYPE_FADE_ANIM,

		//=================
		// ���̑�
		//=================
		TYPE_BG_FREAM,			// �����x��g�[�^���X�R�A�����₷�����邽�߂̘g
		TYPE_MAX				// ����
	}TEXTYPE;

	CTexture();
	~CTexture();

	static HRESULT Load(void);	//	�ǂݍ��ފ֐�(�e�N�X�`��)
	static void Unload(void);	// �J������֐�(�e�N�X�`��)
	static LPDIRECT3DTEXTURE9 *GetTexture(int nIdx) { return &m_apTexture[nIdx]; }

private:
	// �ÓI�����o�ϐ�
	static LPDIRECT3DTEXTURE9 *m_apTexture;
	static const char *m_apFilename[];
};
#endif