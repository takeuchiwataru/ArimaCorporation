#pragma once
//=========================================================================================================//
//
// DirectX���^���� [main.h]
// Author : Ryo Sugimoto
//
//=========================================================================================================//
#ifndef _MAIN_H_
#define _MAIN_H_

//*********************************************************************************************************//
// �C���N���[�h�t�@�C��
//*********************************************************************************************************//
#include <windows.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800)	//�r���h���̌x���Ώ��p�}�N��
#include "dinput.h"					//���͏����ɕK�v
#include "xaudio2.h"				//�T�E���h�����ŕK�v

//*********************************************************************************************************//
// ���C�u�����̃����N
//*********************************************************************************************************//
#pragma comment(lib,"d3d9.lib")		//�`�揈���ɕK�v
#pragma comment(lib,"d3dx9.lib")	//[d3d9.lib]�̊g�����C�u����
#pragma comment(lib,"dxguid.lib")	//DirectX�R���|�[�l���g(���i)
#pragma comment(lib,"winmm.lib")	//�V�X�e�������擾�ɕK�v
#pragma comment(lib,"dinput8.lib")

//*********************************************************************************************************//
// �}�N����`
//*********************************************************************************************************//
#define SCREEN_WIDTH	(1280)		// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(720)		// �E�C���h�E�̍���
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define SCREEN_GAMEX			(800)

//*********************************************************************************************************//
// �\���̒�`
//*********************************************************************************************************//
typedef enum
{//���[�h�̏��
	MODE_TITLE = 0,	//�^�C�g��
	MODE_RANKING,	//�����L���O
	MODE_WATCH,		//�Q�[���ϐ���
	MODE_TUTORIAL,	//�`���[�g���A��
	MODE_GAME,		//�Q�[�����C��
	MODE_RESOLT,	//���U���g
	MODE_MAX

}MODE;

typedef enum
{//window�̕\�����
	WINDOWSTATE_NORMAL = 0,			//�����X�V���Ȃ�
	WINDOWSTATE_STEALTH,			//���񂾂�s������->�ʏ�ɂȂ�
	WINDOWSTATE_FADEIN,				//
	WINDOWSTATE_FADEOUT,			//
	WINDOWSTATE_FLASH,				//�_��
	WINDOWSTATE_FLASH2,				//���_��
	WINDOWSTATE_SCROLL0,			//�c�X�N���[��
	WINDOWSTATE_SCROLL1,			//�΃X�N���[�� ������
	WINDOWSTATE_SELECTON,			//�I������Ă�����
	WINDOWSTATE_SELECTOFF,			//�I������Ă��Ȃ����
	WINDOWSTATE_GUARDON,			//�K�[�h���
	WINDOWSTATE_GUARDOFF,			//�K�[�h���Ă��Ȃ����
	WINDOWSTATE_RANKSET,			//�����L���O���E���獶�֗���ăZ�b�g
	WINDOWSTATE_RANKDELETE,			//�����L���O���E���獶�֗����DELETE
	WINDOWSTATE_ENEMYCHANGE_FLOW,	//BOSS���[�h�ؑ֎� ���S
	WINDOWSTATE_ENEMYCHANGE_ZOOM,	//BOSS���[�h�ؑ֎� �G�l�~�[
	WINDOWSTATE_MAX,

}WINDOWSTATE;

typedef enum
{//window�̎g�p���
	WINDOWUSE_NORMAL = 0,
	WINDOWUSE_RESIDUALAIRPORT,	//�c�@
	WINDOWUSE_SELECT,			//�I��
	WINDOWUSE_GUARD,			//GUARD
	WINDOWUSE_LOAD,				//�ǂݍ��݉�ʂ�
	WINDOWUSE_FONT,				//�������͎� ����
	WINDOWUSE_RANKING,			//RANKING�̎��Ɏg�p�@��Փx�̃��S
	WINDOWUSE_RANKNUMBER,		//RANKING�̎��Ɏg�p�@����
	WINDOWUSE_ENEMY,			//BOSS�G�l�~�[�̏ꍇ
	WINDOWUSE_MAX,

}WINDOWUSE;

typedef enum
{//�A�j���[�V�����̎g�p���
	ANIMATION_NORMAL = 0,
	ANIMATION_LOOP,
	ANIMATION_MAX,

}ANIMATION;

typedef enum
{//�X�R�A�̏��
	SCORESTATE_NONE = 0,
	SCORESTATE_FADE,
	SCORESTATE_DELETE,
	SCORESTATE_RANKDOWN,
	SCORESTATE_SET,
	SCORESTATE_RANKSET,			//�����L���O���E���獶�֗���ăZ�b�g
	SCORESTATE_RANKDELETE,		//�����L���O���E���獶�֗����DELETE

}SCORESTATE;
typedef struct
{//�X�R�A�̍\����
	D3DXVECTOR3 pos;			//�ꏊ
	D3DXCOLOR	col;			//�J���[
	int			g_nScore;		// �X�R�A
	SCORESTATE	state;			//���
	float		fCntstate;		//��ԊǗ��p
	char		aStr[32];		//���O
	bool		bUse;			//�g���Ă邩�g���ĂȂ���

}SCORE;

typedef struct
{
	D3DXVECTOR3 pos;			//���_���W
	float		rhw;			//1.0f�ŌŒ�
	D3DCOLOR	col;			//���_�J���[
	D3DXVECTOR2 tex;			//�e�N�X�`�����W

}VERTEX_2D;

typedef enum
{//�����̏��
	FONTSTATE_NONE = 0,
	FONTSTATE_DELETE,
	FONTSTATE_RANKDOWN,
	FONTSTATE_RANKSET,		//�����L���O���̃Z�b�g
	FONTSTATE_RANKDELETE,	//�����L���O���̃f���[�g

}FONTSTATE;
typedef struct
{
	FONTSTATE	state;		//�����̏��
	D3DXCOLOR	col;		//�J���[
	RECT		rect;		//�����̕\���͈�
	float		fCntState;	//��ԊǗ��p
	char		aStr[25];	//�����̗�
	bool		bUse;		//�g�por���g�p

}Font;

typedef struct
{//���U���g�Ŏg��
	int nCntScore;		//for�ŉ񂳂Ȃ����߃��[�J�����Ə������ɍ���
	int nCntResult;		//���ԃJ�E���g
	int nRankin;		//�n�C�X�R�A���̏��ʕۑ� ���O���͎��g�p
	int nWideFont;		//50���\�̉��I�𕔕�
	int nHightFont;		//50���\�̏c�I�𕔕�
	char aStr[25];		//���O���͂̂���

}Result;

typedef struct
{
	int colR;
	int colG;
	int colB;
	int colA;
}COLLAR;

//*********************************************************************************************************//
// �v���g�^�C�v�錾
//*********************************************************************************************************//
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
MODE *GetMode(void);
HWND *GethWnd(void);

//*********************************************************************************************************//
// �v���g�^�C�v�錾
//*********************************************************************************************************//
void InitFont(void);
void UninitFont(void);
void UpdateFont(void);
void DrawFont(void);
void SetFont(RECT rect, D3DXCOLOR col, char *aStr, int nNumber, FONTSTATE state);
void ChangeFont(bool bUse, char *aStr, int nNumber, FONTSTATE state);
void BreakFont(void);
int *GetSetFont(void);

//*********************************************************************************************************//
// �v���g�^�C�v�錾 �����L���O
//*********************************************************************************************************//
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);

//*********************************************************************************************************//
// �v���g�^�C�v�錾 �����L���O
//*********************************************************************************************************//
void InitWatch(void);
void UninitWatch(void);
void UpdateWatch(void);
void DrawWatch(void);

//*********************************************************************************************************//
// �v���g�^�C�v�錾 �`���[�g���A��
//*********************************************************************************************************//
void InitTutorial(void);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);

//*********************************************************************************************************//
// �v���g�^�C�v�錾 ���U���g
//*********************************************************************************************************//
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
Result *GetResult(void);
SCORE *GetScore(void);
void InputName(int nMode);
void InputName1(void);
void InputName2(void);
void InputName3(void);

//*********************************************************************************************************//
// �v���g�^�C�v�錾�@�w�i
//*********************************************************************************************************//
void InitWindow(void);
void UninitWindow(void);
void UpdateWindow(void);
void DrawWindow(void);
void SetWindow(D3DXVECTOR3 pos, D3DXCOLOR col, int nLengthX, int nLengthY, int nType, int nCntUse, int nEnemy, WINDOWSTATE state, WINDOWUSE use);
void BreakWindow(void);
bool BreakAirport(void);
void SelectWindow(int nNumber);
void BackWindow(int nNumber);
void ChangeGuard(int nUse);
void FontWindowMove(int nWide, int nHeight);

//*********************************************************************************************************//
// �v���g�^�C�v�錾�@�w�i
//*********************************************************************************************************//
void InitBGWindow(void);
void UninitBGWindow(void);
void UpdateBGWindow(void);
void DrawBGWindow(void);
void SetBGWindow(D3DXVECTOR3 pos, COLLAR col, float fSpeed, float fPos, int nLengthX, int nLengthY, int nType, WINDOWSTATE state);
void BreakBGWindow(void);

//*********************************************************************************************************//
// �v���g�^�C�v�錾�@�t�F�[�h�̃|���S���\������
//*********************************************************************************************************//
void InitFadeWindow(void);
void UninitFadeWindow(void);
void UpdateFadeWindow(void);
void DrawFadeWindow(void);
void SetFadeWindow(D3DXVECTOR3 pos, D3DXCOLOR col, int nLengthX, int nLengthY, int nType, int nCntUse, WINDOWSTATE state, WINDOWUSE use);
void LoadBreakWindow(void);

//*****************************************************************************
// �v���g�^�C�v�錾�@���� �Ă�Animation�S��
//*****************************************************************************
void InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
void SetExplosion(D3DXVECTOR3 pos, COLLAR col, int nLengthX, int nLengthY, int nPtnWide, int nPtnHeight, int nPtnSpeed, int nType, ANIMATION anim);
void BreakExplosion(void);

//*****************************************************************************
// �v���g�^�C�v�錾�@���� �Ă�Animation�S��
//*****************************************************************************
void InitAnimation(void);
void UninitAnimation(void);
void UpdateAnimation(void);
void DrawAnimation(void);
void SetAnimation(D3DXVECTOR3 pos, COLLAR col, int nLengthX, int nLengthY, int nPtnWide, int nPtnHeight, int nPtnSpeed, int nType, ANIMATION anim);
void BreakAnimation(void);
#endif