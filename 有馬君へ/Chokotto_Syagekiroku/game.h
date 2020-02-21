#pragma once
//=========================================================================================================//
//
// �|���S���F�v���g�^�C�v�錾 [Scan.h]
// Author : Ryo Sugimoto
//
//=========================================================================================================//
#ifndef _POLYGON_H_
#define _POLYGON_H_

#include"main.h"

//*********************************************************************************************************//
// �}�N����`
//*********************************************************************************************************//
#define PLAYER_POSX			(SCREEN_GAMEX / 2 - 22)					//�摜��X
#define PLAYER_POSY			(SCREEN_HEIGHT / 4 * 3)					//�摜��Y
#define PLAYER_POSX2			(SCREEN_GAMEX / 2 + 22)					//�摜��X2
#define PLAYER_POSY2			(SCREEN_HEIGHT / 4 * 3 + 44)			//�摜��Y2
#define PLAYER_POSX3			((PLAYER_POSX2 - PLAYER_POSX) / 2)	//�摜��X�̕� / 2
#define PLAYER_POSY3			((PLAYER_POSY2 - PLAYER_POSY) / 2)	//�摜��Y�̕� / 2
#define POSPLAYERX				((PLAYER_POSX + PLAYER_POSX2) / 2)	//X�̒��S
#define POSPLAYERY				((PLAYER_POSY + PLAYER_POSY2) / 2)	//Y�̒��S

#define BULLET_ATKTYPE0			(0)										//�ʏ�e
#define BULLET_ATKTYPE1			(1)										//����
#define BULLET_ATKTYPE50		(50)									//����
#define BULLET_ATKTYPE99		(99)									//�i�C�t�I
#define KNIFE_TIME				(BULLET_ATKTYPE99 + 6)					//�i�C�t�̗L������

#define HALF_EXPLOSION			(30)									//�����̕�����
#define GUARD_BREAK				(481)			//GUARD�����܂ł̎���

#define MAX_SCORE		(5 * 4)		//5��*��Փx��
#define RANK_SCORE		(5)			//�����L���O�Ɏg���Ă���X�R�A

#define	MAX_BULLET			(3840)				// �e�̍ő吔
#define BULLET_SLIP			(0.15f)				//�e�̊���
#define BULLET_DELETE		(200)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	GAMESTATE_NONE = 0,
	GAMESTATE_NORMAL,
	GAMESTATE_END,
	GAMESTATE_PAUSE

}GAMESTATE;

typedef enum
{
	BULLETTYPE_PLAYER = 0,
	BULLETTYPE_ENEMY,
	BULLETTYPE_MAX
}BULLETTYPE;

typedef struct
{
	GAMESTATE now;		//���݂̏��
	MODE CHANGE;		//�ς��郂�[�h
	int nCntGameState;	//�J�E���g�p

}GAME;

typedef struct
{
	D3DXVECTOR3 pos;	//�|���S���̈ʒu
	D3DXVECTOR3 move;	//�|���S���̈ړ���
	int nStealth;		//�������̖��G���ԂɎg�p
	int nMode;			//�������ύX�p�@���[�J���ł��Ɣ��莞�߂��Ȃ��Ȃ�
	int nGuardTime;		//�K�[�h�̃��L���X�g�^�C��
	bool bGuard;		//Guard
	bool bCharge;		//GUARD��Charge���
	bool bKnifeLimit;	//�i�C�t����p
	bool bUse;			//���� or ���S

}PLAYER;

typedef enum
{
	BULLETSTATE_NONE = 0,		//�����Ȃ�
	BULLETSTATE_NOT,			//�g�p���Ă��Ȃ�
	BULLETSTATE_START,			//�J�n�̂݉���
	BULLETSTATE_ACCELERATOR,	//������������
	BULLETSTATE_STOP_S,			//�J�n�~�܂� int�ϐ��ɂ����� START��
	BULLETSTATE_STOP_A,			//�J�n�~�܂� int�ϐ��ɂ����� ACCELERATOR��
	BULLETSTATE_STOP_B,			//�J�n�~�܂� int�ϐ��ɂ����� BACK��
	BULLETSTATE_BACK,			//���֐i�� �ی`�̒e�ł����g���Ȃ�I
	BULLETSTATE_STRAIGHT,		//�J�[�u�̒l�������Ă���
	BULLETSTATE_STRAIGHT_B,		//�J�[�u�̒l�������Ă��� BACK��
	BULLETSTATE_PAUSE,			//�r����~ int�ϐ��ɂ��J�n+����
	BULLETSTATE_DELETE,			//�����Ă���
	BULLETSTATE_FLASH,			//�U�����Ɍ���
	BULLETSTATE_SET_A,			//����������u���Ȃ���i�� �u���ꂽ�� = STOP
	BULLETSTATE_SET_B,			//����������u���Ȃ���i�� �u���ꂽ�� = STOP
	BULLETSTATE_SET_S,			//����������u���Ȃ���i�� �u���ꂽ�� = STOP
	BULLETSTATE_SET_A1,			//����������u���Ȃ���i�� �u���ꂽ�� = STOP
	BULLETSTATE_SET_B1,			//����������u���Ȃ���i�� �u���ꂽ�� = STOP
	BULLETSTATE_SET_S1,			//����������u���Ȃ���i�� �u���ꂽ�� = STOP
	BULLETSTATE_SET_B2,			//����������u���Ȃ���i�� �u���ꂽ�� = STOP

}BULLETSTATE;
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ�
	BULLETSTATE state;	//�e�̏��
	int nCntState[3];	//�e�̏�ԗp STOP���̎��Ԃ��ۑ�
	int nLengthX;		//��X
	int nLengthY;		//��Y
	int nType;			//�摜�̎��
	int nAtkType;		//�U�����@	0:�ʏ�e 1:���� 99:�i�C�t
	int nShot;			//�N����������
	int nCntEnemy;		//�G�����ăi�C�t�U�肽����ł��I
	float steep;		//�X��
	float fSetSteep[2];	//�Z�b�g���̊p�x
	float fCurve;		//�J�[�u
	float fSpeed;		//�e�̃X�s�[�h
	float fSpdPlus;		//�X�s�[�h�̉����l
	float fcolA;		//�����x
	bool bUse;			//�g�por�s�g�p
}Bullet;

typedef struct
{//��Փx�̐ݒ�
	int nDifficulty;	//��Փx
	int nNumber;		//��Փx���Ƃ̃i���o�[

}Difficulty;

typedef struct
{//�Q�[���̓G�o�����̃J�E���g
	int nCntGameOver[2];	//�Q�[���I�[�o�[[2]
	int nCntData;			//�G�o��
	int nCntClear;			//Clear�J�E���g
	int nEnd;				//���S�J�E���g

}GameCounter;

typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 move;		// �ړ��l�̎c���
	D3DXCOLOR col;			// �F
	float fSteep;			//�p�x
	float fSpeed;
	float fRadius;			// ���a(�傫��)
	bool bUse;				// �g�p���Ă��邩�ǂ���

} EFFECT;

//*********************************************************************************************************//
// �v���g�^�C�v�錾�@Game
//*********************************************************************************************************//
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
GAME *GetGameState(void);
void SetGameState(GAMESTATE game, MODE mode, int nCntGameState);

//*********************************************************************************************************//
// �v���g�^�C�v�錾�@Chapter
//*********************************************************************************************************//
void ChapterEnemy(void);
void EASYMode(void);
void NORMALMode(void);
void HARDMode(void);
void LUNATICMode(void);
void SetDifficulty(Difficulty diffculty);
Difficulty *GetDifficulty(void);
void SetGamecounter(GameCounter gamecounter);
GameCounter *GetGamecounter(void);
void SetFieldBGM(void);

//*********************************************************************************************************//
// �v���g�^�C�v�錾�@�|���S��
//*********************************************************************************************************//
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(void);

//*****************************************************************************
// �v���g�^�C�v�錾�@�e
//*****************************************************************************
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, float steep, int nLengthX, int nLengthY, int nType, int nAtkType, float fSpeed, BULLETTYPE nShot, int nCntEnemy, BULLETSTATE state, float fSpdPlus, float fCurve, int nCntstate0, int nCntstate1);
void StartBullet(int nCntEnemy, int nCntAtk);
void BreakBullet(void);
void BreakBullet(int nCntEnemy);
bool HitAim(Bullet pBullet, D3DXVECTOR3 aim, int nLength, float fPlusHit);
Bullet *GetBullet(void);

//*****************************************************************************
// �v���g�^�C�v�錾�@�T�u�@�V���[�^�[
//*****************************************************************************
void InitShooter(void);
void UninitShooter(void);
void UpdateShooter(D3DXVECTOR3 pos, int nSet);
void DrawShooter(void);
void SetShooter(D3DXVECTOR3 move, int nType);
void BreakShooter(void);

//*********************************************************************************************************//
// �v���g�^�C�v�錾�@�w�i
//*********************************************************************************************************//
void InitBG(void);
void UninitBG(void);
void UpdateBG();
void DrawBG(int nBG);

//*****************************************************************************
// �v���g�^�C�v�錾  �X�R�A
//*****************************************************************************
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void AddScore(int nValue, int nNumber);
void SetScore(D3DXVECTOR3 pos, D3DXCOLOR col, int nScore, int nNumber, SCORESTATE state);
void BreakScore(void);
SCORE *GetGameScore(void);
void ChangeScore(int nNumber, SCORESTATE state);

//*****************************************************************************
// �v���g�^�C�v�錾  window
//*****************************************************************************
void InitGameWindow(void);
void UninitGameWindow(void);
void UpdateGameWindow(void);
void DrawGameWindow(void);
void SetGameWindow(D3DXVECTOR3 pos, D3DXCOLOR col, int nLengthX, int nLengthY, int nType, WINDOWSTATE state, WINDOWUSE use);
void BreakGameWindow(void);
void BackGameWindow(int nNumber);
void ChangeGameWindow(int nNumber, WINDOWSTATE state);

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);

void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSteep, float fSpeed, float fRadius);
void ExplosionEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nSRand, int nLRand, float fSRand0, float fSRand1, float fLRand0, float fLRand1);
void SetBulletEffect(Bullet bullet);
EFFECT *GetEffect(void);
Bullet *GetBulletEffect(void);

#endif