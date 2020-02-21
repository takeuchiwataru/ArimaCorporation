//=============================================================================
//
// �G���� [enemy.h]
// Author :
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_ENEMY	(32)	// �G�̍ő吔

//*********************************************************************************************************//
// �}�N����`
//*********************************************************************************************************//
#define GAME_WIDE			(800)		//�Q�[����ʁi���C���j�̕�
#define ENEMY_HP1			(5)			//�G���G��HP
#define ENEMY_HP2			(50)		//���{�X��HP
#define ENEMY_HP3			(1000)		//�{�X��HP 
#define ENEMY_ATTACK		(30)		//�G���G�̍U��SPEED
#define ENEMY_ATTACK1		(8)			//�G���G�̍U��SPEED
#define ENEMY_ATTACK2		(2)			//�G���G�̍U��SPEED
#define ENEMY_ATKTIME0		(0)			//�G���G�͋x�ݎ��ԂȂ�
#define ENEMY_ATKTIME1		(135)		//�x�ݎ��Ԃ܂ł̍U������
#define ENEMY_ATKTIME2		(150)		//�x�ݎ��Ԃ܂ł̍U������
#define ENEMY_BREAKTIME0	(0)			//�G���G�͋x�ݎ��ԂȂ�
#define ENEMY_BREAKTIME1	(90)		//�G�̋x�e����
#define ENEMY_BREAKTIME2	(90)		//�G�̋x�e����
#define SHOTCOL_STAGE1		(8)			//�G�̒e�̐F�@�X�e�[�W1

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	ENEMYSTATE_NORMAL = 0,	//�G�̒ʏ���
	ENEMYSTATE_DAMAGE,		//�G�̃_���[�W���
	ENEMYSTATE_TIME,		//���G��� && �U�����Ȃ�
	ENEMYSTATE_TIME1,		//���G��� && �U�����Ȃ�
	ENEMYSTATE_Title,
	ENEMYSTATE_MAX			//�G�̏�Ԃ̍��v

}ENEMYSTATE;

typedef enum
{
	ENEMYMOVE_NORMAL = 0,	//���������Ȃ� �G���I
	ENEMYMOVE_NONE,			//�����Ȃ�
	ENEMYMOVE_RETURN,		//�^�񒆂ɖ߂� �U���͂���
	ENEMYMOVE_MOVE,			//�{�X�̈ړ�
	ENEMYMOVE_STOP,			//�{�X�̈ꎞ�I�X�g�b�v
	ENEMYMOVE_MAX			//�G�̏�Ԃ̍��v

}ENEMYMOVE;

typedef enum
{
	BULLETAIM_NORMAL = 0,	//�e�̒ʏ���
	BULLETAIM_TIME,			//�e�̋x�e���
	BULLETAIM_STOP,			//�^�C�~���O�����̂��ߏ��߂������炷
	BULLETAIM_MAX			//�e�̏�Ԃ̍��v

}BULLETAIM;

typedef enum
{
	ENEMYTYPE_NORMAL = 0,	//�G���G
	ENEMYTYPE_SMALL,		//���{�X
	ENEMYTYPE_MEDIUM,		//���{�X
	ENEMYTYPE_BIG,			//�{�X
	ENEMYTYPE_MAX,			//�G��ނ̍��v
	ENEMYTYPE_TITLE2 = 6,
	ENEMYTYPE_TITLE = 23

}ENEMYTYPE;

typedef enum
{
	ENEMYMODE_NONE = 0,			//�G�̒ʏ��ԁ@�ω��Ȃ�
	ENEMYMODE_NONE_1,			//�{�X���̎�芪��
	ENEMYMODE_MBOSS_EASY,		//���{�X EASY
	ENEMYMODE_MBOSS_NORMAL,		//���{�X NORMAL
	ENEMYMODE_MBOSS_HARD,		//���{�X HARD
	ENEMYMODE_MBOSS_LUNATIC,	//���{�X Lunatic
	ENEMYMODE_LBOSS_EASY_1,		//�{�X EASY1
	ENEMYMODE_LBOSS_NORMAL_1,	//�{�X NORMAL1
	ENEMYMODE_LBOSS_HARD_1,		//�{�X HARD1
	ENEMYMODE_LBOSS_LUNATIC_1,	//�{�X Lunatic1
	ENEMYMODE_LBOSS_EASY_2,		//�{�X EASY2
	ENEMYMODE_LBOSS_NORMAL_2,	//�{�X NORMAL2
	ENEMYMODE_LBOSS_HARD_2,		//�{�X HARD2
	ENEMYMODE_LBOSS_LUNATIC_2,	//�{�X Lunatic2
	ENEMYMODE_LBOSS_EASY_3,		//�{�X EASY2
	ENEMYMODE_LBOSS_NORMAL_3,	//�{�X NORMAL2
	ENEMYMODE_LBOSS_HARD_3,		//�{�X HARD2
	ENEMYMODE_LBOSS_LUNATIC_3,	//�{�X Lunatic2
	ENEMYMODE_MAX				//�G�̏�Ԃ̍��v

}ENEMYMODE;

typedef struct
{
	BULLETSTATE state;	//�e�̏��
	BULLETAIM aim;		//�e�̔��ˏ��
	int nCntState[2];	//��~���ԓ��ۑ�
	int nTypeAttcak;	//�U���̎��
	int nCol;			//�e�̐F
	int nAttackTime[2];	//0�F�x�e����, 1�F�J������
	int nLength[2];		//�e�̒���XY
	int nBulletNum;		//�e�̗�
	int nCntTime;		//���ԃJ�E���g
	int nRecast;		//�U���̃��L���X�g�^�C��
	int nAtkPlus;		//�^�C�~���O���炵�p�@�����U����I�񂾏ꍇ��
	float fSpdPlus;		//�����l
	float fAngle;		//���˂���p�x
	float fAnglePlus;	//�p�x��plus
	float fCurve;		//�e��plus����p�x
	float fSpeed;		//�e�̃X�s�[�h
	float fPos;			//���ˏꏊ���痣��鋗��
	float fSetAngle[2];	//�Z�b�g���邽�߂̊p�x

}ATTACK;

typedef struct
{
	ENEMYSTATE state;	//�G�l�~�[�̏��
	ENEMYMOVE movestate;//�G�l�~�[�̏�� �ύX�p
	ENEMYMODE mode;		//�G�̕ω����
	ENEMYTYPE type;		//�G�̎��	0:���@1:���@2:��
	ATTACK attack[5];	//�����̎��
	int nCounterState;	//�G�l�~�[�̏�ԊǗ��p
	int nCounterMove;	//�G�l�~�[�̈ړ��Ǘ��p
	D3DXVECTOR3 pos;	//�����ʒu
	D3DXVECTOR3 move;	//�ړ��ʕۑ�(�����̂���)
	float fsteep;		//�����p�x
	float fChannel;		//�����]���̗�, ����( + or - )
	float fSpeed;		//�ړ�SPEED
	float fAngle;		//Animation�p�̌X��
	int nAttack;		//�U���p�^�[��
	int nReplus;		//�U���^�C�~���O�p
	int nTex;			//�摜�w��
	int nHP;			//�G�l�~�[��HP
	int nCollar;		//�G�l�~�[�̔�_�����̐F�̕ω��p
	int nLength;		//�G�̔����̒���
	bool bAngle;		//�p�x+-�̕ۑ�
	bool bUse;			//�g�p or ���g�p

}ENEMY;

typedef struct
{
	int nEnemyCounter;			//�{�X�܂߃J�E���g
	bool g_bBossKill;			//�{�X�̌��j����̂���

}IfBoss;	//�{�X�̏o������

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, ENEMYTYPE nLarge, ENEMYMODE mode, float steep, float fChannel, float fSpeed, int nReplus, int nType, int nHP, ATTACK attcak0, ATTACK attcak1, ATTACK attcak2, ATTACK attcak3, ATTACK attcak4);
void SetAttack(int nAtk, int nNumBullet, int nLengthX, int nLengthY, int nCol, int nCntEnemy, int nEnemyAtk, float fSteep, float fSpeed, float fSpdPlus, float fCurve, D3DXVECTOR3 pos, float fPos, BULLETSTATE state);
void BreakEnemy(void);
void DamageBoss(int nCntEnemy, int nDamage);
void DamageBossTitle(int nDamage);
void AttackEnemy(int nCntEnemy, int nCntRecast, float fChannel360, float fChannel90, float fPlayerAngle);
void HitEnemy(int nCntEnemmy, int nDamageEnemy, int nAtkType);
void BossChange(int nCntEnemy);
void DeadEnemy(void);
IfBoss *GetBoss(void);
ENEMY *GetEnemy(void);

#endif
