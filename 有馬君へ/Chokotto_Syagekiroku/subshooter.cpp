//=========================================================================================================//
//
// �T�u�V���[�^�[ ���� [subshooter.cpp]
// Author : Ryou Sugimoto
//
//=========================================================================================================//
#include "main.h"
#include "game.h"

//*********************************************************************************************************//
// �}�N����`
//*********************************************************************************************************//
#define SUB_VERTEX_NUM			(4)					// ���_�̐�
#define SUB_PRIMITIVE_NUM		(2)					// �v���~�e�B�u�̐�
#define SUB__TextureName		"data/TEXTURE/migawari001.png"
#define	MAX_SUB_BULLET			(4)		// �e�̍ő吔
#define MAX_WIDE			(35)
#define MAX_HEIGHT			(25)
#define HALF_POLYGON		(15)
#define RAND_REFLECT		(1)
#define RAND_NOREFLECT		(2)
#define RAND_NOREFLECT2		(1.2f)
#define STANDING_X			(10)			//����X�l	�O�W��
#define STANDING_X2			(25)			//����X�l2	�O�W��
#define STANDING_Y			(40)			//����Y�l	�O�W��
#define STANDING_Y2			(15)			//����Y�l2	�O�W��
#define STANDING2_X			(25)			//����X�l	�g�U
#define STANDING2_X2		(45)			//����X2�l	�g�U
#define STANDING2_Y			(45)			//����Y�l	�g�U
#define STANDING2_Y2		(30)			//����Y2�l	�g�U
#define TIME_LATE			(8)				//���ˑ��x
#define SUB_BULLET_SPD		(2.0f)			//�e�̃X�s�[�h
#define MODE_CHANGE			(10)			//���񂳂������̈ړ�SPEED
#define HALF_SUB_BULLETX		(8)				//���˂���e�̔���
#define HALF_SUB_BULLETY		(10)			//���˂���e�̔���

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	D3DXVECTOR3 PosMove;	//�ړ������ۑ�
	int nType;
	bool bUse;
}Shooter;

typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
}SubShot;

//*********************************************************************************************************//
// �O���[�o���ϐ�
//*********************************************************************************************************//
LPDIRECT3DTEXTURE9		g_pTextureShooter = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShooter = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
Shooter g_aShooter[MAX_SUB_BULLET];
bool g_bPos;

//=========================================================================================================//
// * �w�i�̕`�揈��1	����������
//=========================================================================================================//
void InitShooter(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	int nCntBullet;

	// �e�̏��̏�����
	for (nCntBullet = 0; nCntBullet < MAX_SUB_BULLET; nCntBullet++)
	{
		g_aShooter[nCntBullet].pos = { 0.0f, 0.0f, 0.0f };
		g_aShooter[nCntBullet].move = { 0.0f, 0.0f, 0.0f };
		g_aShooter[nCntBullet].nType = 0;
		g_aShooter[nCntBullet].PosMove = { 0.0f, 0.0f, 0.0f };
		g_aShooter[nCntBullet].bUse = false;
	}
	g_bPos = false;

	D3DXCreateTextureFromFile(pDevice, SUB__TextureName, &g_pTextureShooter);

	//���_BUFFER�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * SUB_VERTEX_NUM * MAX_SUB_BULLET,		//�m�ۂ���BUFFER�̃T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffShooter,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
							//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffShooter->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntBullet = 0; nCntBullet < MAX_SUB_BULLET; nCntBullet++)
	{
		//���_���W�̐ݒ�
		pVtx[nCntBullet * 4].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(0, 0, 0.0f);

		pVtx[nCntBullet * 4].rhw = 1.0f;
		pVtx[nCntBullet * 4 + 1].rhw = 1.0f;
		pVtx[nCntBullet * 4 + 2].rhw = 1.0f;
		pVtx[nCntBullet * 4 + 3].rhw = 1.0f;

		pVtx[nCntBullet * 4].col = D3DCOLOR_RGBA(255, 255, 255, 180);	// A = �����x
		pVtx[nCntBullet * 4 + 1].col = D3DCOLOR_RGBA(255, 255, 255, 180);
		pVtx[nCntBullet * 4 + 2].col = D3DCOLOR_RGBA(255, 255, 255, 180);
		pVtx[nCntBullet * 4 + 3].col = D3DCOLOR_RGBA(255, 255, 255, 180);

		//�e�N�X�`�����W�ݒ�
		//pVtx[nCntBullet * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
		//pVtx[nCntBullet * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		//pVtx[nCntBullet * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		//pVtx[nCntBullet * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//���_BUFFER���A�����b�N����
	g_pVtxBuffShooter->Unlock();
}
//=========================================================================================================//
// * �w�i�̕`�揈��2	�I������
//=========================================================================================================//
void UninitShooter(void)
{//�e�N�X�`���̔j��
	if (g_pTextureShooter != NULL)
	{
		g_pTextureShooter->Release();
		g_pTextureShooter = NULL;
	}
	if (g_pVtxBuffShooter != NULL)
	{
		g_pVtxBuffShooter->Release();
		g_pVtxBuffShooter = NULL;
	}
}
//=========================================================================================================//
// * �w�i�̕`�揈��3	�X�V����
//=========================================================================================================//
void UpdateShooter(D3DXVECTOR3 pos , int nSet)
{
	int nCntBullet;
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;
	static int g_nShotCount = 0;
	static SubShot g_aSubShot[MAX_SUB_BULLET];
	static D3DXVECTOR3 moveSet[MAX_SUB_BULLET];	//�ړ���
	static int nMoveCount[2];
	static bool bSet = false;
	float nRand[2];

	//�f�o�C�X���擾����
	pDevice = GetDevice();


	//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffShooter->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntBullet = 0; nCntBullet < MAX_SUB_BULLET; nCntBullet++)
	{
		g_nShotCount++;
		if (g_aShooter[nCntBullet].bUse == true)
		{//�e��O�ɔ�΂�
		 //MessageBox(0, "�{�������H", "�I��", MB_YESNO);
			if (nSet == 0)
			{//�Z�b�g1�@���肮�邮��
				if (g_bPos == false)
				{
					g_aShooter[0].pos = { STANDING2_X, -STANDING2_Y, 0.0f };
					g_aShooter[1].pos = { -STANDING2_X, -STANDING2_Y, 0.0f };
					g_aShooter[2].pos = { STANDING2_X2, -STANDING2_Y2, 0.0f };
					g_aShooter[3].pos = { -STANDING2_X2, -STANDING2_Y2, 0.0f };

					g_aShooter[0].PosMove = D3DXVECTOR3{ -2.0f , -3.0f,0.0f };
					g_aShooter[1].PosMove = D3DXVECTOR3{ 2.0f , 3.0f,0.0f };
					g_aShooter[2].PosMove = D3DXVECTOR3{ -3.0f , -2.0f,0.0f };
					g_aShooter[3].PosMove = D3DXVECTOR3{ 3.0f , 2.0f,0.0f };

					g_bPos = true;
					nMoveCount[0] = 0;
				}
				g_aShooter[nCntBullet].move += g_aShooter[nCntBullet].PosMove;
				g_aShooter[nCntBullet].pos += g_aShooter[nCntBullet].move;
				g_aShooter[nCntBullet].move.x += 0.0f - g_aShooter[nCntBullet].move.x * 0.15f;
				g_aShooter[nCntBullet].move.y += 0.0f - g_aShooter[nCntBullet].move.y * 0.15f;
			}
			else if(nSet == 1)
			{//�O��
				if (nMoveCount[0] == MODE_CHANGE)
				{//���ꂼ�ꐔ���ʂ�΃��[�v
					g_aShooter[0].pos = { STANDING_X, STANDING_Y, 0.0f };
					g_aShooter[1].pos = { -STANDING_X, STANDING_Y, 0.0f };
					g_aShooter[2].pos = { STANDING_X2, STANDING_Y2, 0.0f };
					g_aShooter[3].pos = { -STANDING_X2, STANDING_Y2, 0.0f };

					moveSet[0] = { 0.0f, 0.0f, 0.0f };
					moveSet[1] = { 0.0f, 0.0f, 0.0f };
					moveSet[2] = { 0.0f, 0.0f, 0.0f };
					moveSet[3] = { 0.0f, 0.0f, 0.0f };

					bSet = true;
					g_bPos = false;
					nMoveCount[1] = 0;
				}
				else if (nMoveCount[0] < 1 && bSet == false)
				{//�ړ���������
					moveSet[0] = { (-g_aShooter[nCntBullet].pos.x + STANDING_X) / MODE_CHANGE, (-g_aShooter[nCntBullet].pos.y + STANDING_Y) / MODE_CHANGE, 0.0f };
					moveSet[1] = { (-g_aShooter[nCntBullet].pos.x - STANDING_X) / MODE_CHANGE, (-g_aShooter[nCntBullet].pos.y + STANDING_Y) / MODE_CHANGE, 0.0f };
					moveSet[2] = { (-g_aShooter[nCntBullet].pos.x + STANDING_X2) / MODE_CHANGE, (-g_aShooter[nCntBullet].pos.y + STANDING_Y2) / MODE_CHANGE, 0.0f };
					moveSet[3] = { (-g_aShooter[nCntBullet].pos.x - STANDING_X2) / MODE_CHANGE, (-g_aShooter[nCntBullet].pos.y + STANDING_Y2) / MODE_CHANGE, 0.0f };
				}
				nMoveCount[0]++;
				g_aShooter[nCntBullet].pos += moveSet[nCntBullet];
			}
			else if (nSet == 2)
			{//�g�U
				if (nMoveCount[1] == MODE_CHANGE)
				{//���ꂼ�ꐔ���ʂ�΃��[�v
					g_aShooter[0].pos = { STANDING2_X, -STANDING2_Y, 0.0f };
					g_aShooter[1].pos = { -STANDING2_X, -STANDING2_Y, 0.0f };
					g_aShooter[2].pos = { STANDING2_X2, -STANDING2_Y2, 0.0f };
					g_aShooter[3].pos = { -STANDING2_X2, -STANDING2_Y2, 0.0f };

					moveSet[0] = { 0.0f, 0.0f, 0.0f };
					moveSet[1] = { 0.0f, 0.0f, 0.0f };
					moveSet[2] = { 0.0f, 0.0f, 0.0f };
					moveSet[3] = { 0.0f, 0.0f, 0.0f };

					bSet = false;
				}
				else if (nMoveCount[1] < 1 && bSet == true)
				{//�ړ���������
					moveSet[0] = { (-g_aShooter[nCntBullet].pos.x + STANDING2_X) / MODE_CHANGE, (-g_aShooter[nCntBullet].pos.y - STANDING2_Y) / MODE_CHANGE, 0.0f };
					moveSet[1] = { (-g_aShooter[nCntBullet].pos.x - STANDING2_X) / MODE_CHANGE, (-g_aShooter[nCntBullet].pos.y - STANDING2_Y) / MODE_CHANGE, 0.0f };
					moveSet[2] = { (-g_aShooter[nCntBullet].pos.x + STANDING2_X2) / MODE_CHANGE, (-g_aShooter[nCntBullet].pos.y - STANDING2_Y2) / MODE_CHANGE, 0.0f };
					moveSet[3] = { (-g_aShooter[nCntBullet].pos.x - STANDING2_X2) / MODE_CHANGE, (-g_aShooter[nCntBullet].pos.y - STANDING2_Y2) / MODE_CHANGE, 0.0f };
				}
				nMoveCount[1]++;
				g_aShooter[nCntBullet].pos += moveSet[nCntBullet];
			}
				if (g_nShotCount > TIME_LATE && nSet < 2)
				{//0.2�b��1��
					g_aSubShot[nCntBullet].pos = D3DXVECTOR3{ pos.x - g_aShooter[nCntBullet].pos.x ,pos.y - g_aShooter[nCntBullet].pos.y ,0.0f };
					g_nShotCount = 0;
					SetBullet(D3DXVECTOR3{ pos.x - g_aShooter[nCntBullet].pos.x ,pos.y - g_aShooter[nCntBullet].pos.y ,0.0f }, D3DX_PI, HALF_SUB_BULLETX, HALF_SUB_BULLETY, (nCntBullet * 2) + 52, BULLET_ATKTYPE0, SUB_BULLET_SPD, BULLETTYPE_PLAYER, 0, BULLETSTATE_NONE, 0.0f, 0.0f, 0, 0);
				}
				else if (g_nShotCount > TIME_LATE && nSet == 2)
				{
					g_aSubShot[nCntBullet].pos = D3DXVECTOR3{ pos.x - g_aShooter[nCntBullet].pos.x ,pos.y - g_aShooter[nCntBullet].pos.y ,0.0f };
					g_nShotCount = 0;
					if (g_aShooter[nCntBullet].pos.x + pos.x > pos.x + 30)
					{
						SetBullet(g_aSubShot[nCntBullet].pos, -2.5f, HALF_SUB_BULLETX, HALF_SUB_BULLETY, (nCntBullet * 2) + 52, BULLET_ATKTYPE0, SUB_BULLET_SPD, BULLETTYPE_PLAYER, 0, BULLETSTATE_NONE, 0.0f, 0.0f, 0, 0);
					}
					else if (g_aShooter[nCntBullet].pos.x + pos.x > pos.x)
					{
						SetBullet(g_aSubShot[nCntBullet].pos, -3.0f, HALF_SUB_BULLETX, HALF_SUB_BULLETY, (nCntBullet * 2) + 52, BULLET_ATKTYPE0, SUB_BULLET_SPD, BULLETTYPE_PLAYER, 0, BULLETSTATE_NONE, 0.0f, 0.0f, 0, 0);
					}
					else if (g_aShooter[nCntBullet].pos.x + pos.x + 30> pos.x)
					{
						SetBullet(g_aSubShot[nCntBullet].pos, 3.0f, HALF_SUB_BULLETX, HALF_SUB_BULLETY, (nCntBullet * 2) + 52, BULLET_ATKTYPE0, SUB_BULLET_SPD, BULLETTYPE_PLAYER, 0, BULLETSTATE_NONE, 0.0f, 0.0f, 0, 0);
					}
					else
					{
						SetBullet(g_aSubShot[nCntBullet].pos, 2.5f, HALF_SUB_BULLETX, HALF_SUB_BULLETY, (nCntBullet * 2) + 52, BULLET_ATKTYPE0, SUB_BULLET_SPD, BULLETTYPE_PLAYER, 0, BULLETSTATE_NONE, 0.0f, 0.0f, 0, 0);
					}
				}

			nRand[0] = rand() % RAND_REFLECT + 1.0f;
			nRand[1] = rand() % RAND_NOREFLECT + 1.0f;
			if (nRand[1] == 2)
			{
				nRand[1] = RAND_NOREFLECT2;
			}
			if (g_aShooter[nCntBullet].pos.y > MAX_HEIGHT - 15)
			{
				g_aShooter[nCntBullet].PosMove.y = -nRand[0];
				g_aShooter[nCntBullet].PosMove.x *= nRand[1];
			}
			if (g_aShooter[nCntBullet].pos.y < -MAX_HEIGHT)
			{
				g_aShooter[nCntBullet].PosMove.y = nRand[0];
				g_aShooter[nCntBullet].PosMove.x *= nRand[1];
			}
			if (g_aShooter[nCntBullet].pos.x > MAX_WIDE)
			{
				g_aShooter[nCntBullet].PosMove.x = -nRand[0];
				g_aShooter[nCntBullet].PosMove.y *= nRand[1];
			}
			if (g_aShooter[nCntBullet].pos.x < -MAX_WIDE)
			{
				g_aShooter[nCntBullet].PosMove.x = nRand[0];
				g_aShooter[nCntBullet].PosMove.y *= nRand[1];
			}

			pVtx[nCntBullet * 4].pos = D3DXVECTOR3(pos.x - g_aShooter[nCntBullet].pos.x - HALF_POLYGON, pos.y - g_aShooter[nCntBullet].pos.y, 0.0f);
			pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(pos.x - g_aShooter[nCntBullet].pos.x + HALF_POLYGON, pos.y - g_aShooter[nCntBullet].pos.y, 0.0f);
			pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(pos.x - g_aShooter[nCntBullet].pos.x - HALF_POLYGON, pos.y - g_aShooter[nCntBullet].pos.y + HALF_POLYGON * 2, 0.0f);
			pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(pos.x - g_aShooter[nCntBullet].pos.x + HALF_POLYGON, pos.y - g_aShooter[nCntBullet].pos.y + HALF_POLYGON * 2, 0.0f);
		}
	}
	//���_BUFFER���A�����b�N����
	g_pVtxBuffShooter->Unlock();
}
//=========================================================================================================//
// * �w�i�̕`�揈��4	�`�揈��
//=========================================================================================================//
void DrawShooter(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffShooter, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureShooter);
	
	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_NUM);
	// �|���S���̕`��
	for (nCntBullet = 0; nCntBullet < MAX_SUB_BULLET; nCntBullet++)
	{
		if (g_aShooter[nCntBullet].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, SUB_PRIMITIVE_NUM);
		}
	}

}
//=============================================================================
// �e�̐ݒ菈��
//=============================================================================
void SetShooter(D3DXVECTOR3 move, int nType)
{
	int nCntBullet;
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

		//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffShooter->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_SUB_BULLET; nCntBullet++)
	{
		if (g_aShooter[nCntBullet].bUse == false)
		{
			g_aShooter[nCntBullet].move = move;
			g_aShooter[nCntBullet].bUse = true;

			pVtx[nCntBullet * 4].pos = D3DXVECTOR3(g_aShooter[nCntBullet].pos.x - HALF_POLYGON, g_aShooter[nCntBullet].pos.y - HALF_POLYGON, 0.0f);
			pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(g_aShooter[nCntBullet].pos.x + HALF_POLYGON, g_aShooter[nCntBullet].pos.y - HALF_POLYGON, 0.0f);
			pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(g_aShooter[nCntBullet].pos.x - HALF_POLYGON, g_aShooter[nCntBullet].pos.y + HALF_POLYGON, 0.0f);
			pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(g_aShooter[nCntBullet].pos.x + HALF_POLYGON, g_aShooter[nCntBullet].pos.y + HALF_POLYGON, 0.0f);

			g_aShooter[nCntBullet].nType = nType;
			pVtx[nCntBullet * 4].tex = D3DXVECTOR2((nType % 4) * 0.25f, (nType / 4) * 0.5f);
			pVtx[nCntBullet * 4 + 1].tex = D3DXVECTOR2((nType % 4) * 0.25f + 0.25f, (nType / 4) * 0.5f);
			pVtx[nCntBullet * 4 + 2].tex = D3DXVECTOR2((nType % 4) * 0.25f, (nType / 4) * 0.5f + 0.5f);
			pVtx[nCntBullet * 4 + 3].tex = D3DXVECTOR2((nType % 4) * 0.25f + 0.25f, (nType / 4) * 0.5f + 0.5f);
			break;
		}
	}
	//���_BUFFER���A�����b�N����
	g_pVtxBuffShooter->Unlock();

}
//=============================================================================
// �e�̐ݒ菈��
//=============================================================================
void BreakShooter(void)
{
	int nCntBullet;
	PLAYER *pPlayer;

	pPlayer = GetPlayer();

	for (nCntBullet = 0; nCntBullet < MAX_SUB_BULLET; nCntBullet++)
	{
		if (g_aShooter[nCntBullet].bUse == true)
		{
			SetExplosion(D3DXVECTOR3{ g_aShooter[nCntBullet].pos.x + pPlayer[0].pos.x, pPlayer[0].pos.y + PLAYER_POSY3 - g_aShooter[nCntBullet].pos.y, 0.0f }, COLLAR{ 255,255,255 }, HALF_EXPLOSION, HALF_EXPLOSION, 5, 4, 2, 0, ANIMATION_NORMAL);
			g_aShooter[nCntBullet].bUse = false;
		}
	}
}