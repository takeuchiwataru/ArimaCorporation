//=========================================================================================================//
//
// ���@ ���� [player.cpp]
// Author : Ryou Sugimoto
//
//=========================================================================================================//
#include "main.h"
#include "input.h"
#include "game.h"
#include "sound.h"
#include <stdlib.h>				//�����h�p
#include <time.h>				//�����������h�p

//*********************************************************************************************************//
// �O���[�o���ϐ�
//*********************************************************************************************************//
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;	//���_BUFFER�ւ̃|�C���^
PLAYER g_Player;
int nSet = 0;
//*********************************************************************************************************//
// �}�N����`
//*********************************************************************************************************//
#define PLAYER_TextureName		"data/TEXTURE/Player001.png"	//TEXTURE�̖��O
#define VERTEX_NUM				(4)				// ���_�̐�
#define PRIMITIVE_NUM			(2)				// �v���~�e�B�u�̐�
#define RECAST_TIME				(4)				//�e�̔��ˑ��x

#define MOVEPOLYGON				(1.0f)			//�|���S���̐i�ޑ���
#define SLIPPLAYER				(0.12f)
#define WIDETEXTURE				(1.0f)			//TEXTURE�̕�
#define HEIGHTTEXTURE			(1.0f)			//TEXTURE�̍���
#define BULLET_SPEED			(2.0f)			//�e��SPEED
#define BULLET0_LENGTHX			(8)				//�ʏ�eX
#define BULLET0_LENGTHY			(8)				//�ʏ�eY
#define BULLET1_LENGTHX			(10)			//���˒eX
#define BULLET1_LENGTHY			(13)			//���˒eY
#define BULLET2_LENGTHX			(12)			//�Q���rX
#define BULLET2_LENGTHY			(50)			//�Q���rY
#define KNIFE_LENGTHX			(13)			//�i�C�tX
#define KNIFE_LENGTHY			(80)			//�i�C�tY
#define GUARD_TIME				(60)			//GUARD�̗L������

//=========================================================================================================//
// * �v���C���[�̕`�揈��1	����������
//=========================================================================================================//
void InitPlayer(void)
{
	srand((unsigned int)time(0));
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	g_Player.pos = D3DXVECTOR3{ POSPLAYERX, POSPLAYERY, 0.0f };		//�|���S���̒��S
	g_Player.move = D3DXVECTOR3{ 0,0,0.0f };	//�|���S���̈ړ�
	g_Player.nStealth = 255;
	g_Player.nMode = 0;
	g_Player.nGuardTime = 0;
	g_Player.bGuard = false;
	g_Player.bCharge = false;
	g_Player.bKnifeLimit = false;
	g_Player.bUse = false;
	nSet = 0;

	D3DXCreateTextureFromFile(pDevice, PLAYER_TextureName, &g_pTexturePlayer);

	//���_BUFFER�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_NUM,		//�m�ۂ���BUFFER�̃T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
							//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - PLAYER_POSX3, g_Player.pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x + PLAYER_POSX3, g_Player.pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x - PLAYER_POSX3, g_Player.pos.y + PLAYER_POSY3 * 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + PLAYER_POSX3, g_Player.pos.y + PLAYER_POSY3 * 2, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//�J���[�ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);	// A = �����x
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(WIDETEXTURE, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, HEIGHTTEXTURE);
	pVtx[3].tex = D3DXVECTOR2(WIDETEXTURE, HEIGHTTEXTURE);

	//���_BUFFER���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
}
//=========================================================================================================//
// * �v���C���[�̕`�揈��2	�I������
//=========================================================================================================//
void UninitPlayer(void)
{//�e�N�X�`���̔j��
	g_Player.bUse = false;
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}
//=========================================================================================================//
// * �v���C���[�̕`�揈��3	�X�V����
//=========================================================================================================//
void UpdatePlayer(void)
{
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
	static bool g_bChannel;
	static int g_nRecast = 0;
	static int nKnifeTime = 0;
	static int nBoostTime = 0;
	static float g_fChannel;
	int nSubShot;
	static bool bStartShooter = false;
	int nRand;

	float fRun = MOVEPOLYGON;
	float fSlip = SLIPPLAYER;
	int nLate = RECAST_TIME;
	int nCount;

	fSlip = SLIPPLAYER;

	if (g_Player.bUse == true)
	{
		if (g_Player.nGuardTime != GUARD_BREAK && g_Player.bGuard == false)
		{//Guard���Ԃ�60F�܂�
			g_Player.nGuardTime++;
			if (g_Player.nGuardTime == GUARD_BREAK)
			{//�`���[�W����
				PlaySound(SOUND_LABEL_BOTTON000);
				SetExplosion(D3DXVECTOR3{ g_Player.pos.x, g_Player.pos.y + PLAYER_POSY3, 0.0f }, COLLAR{ 255,255,200 }, 100, 100, 10, 1, 2, 1, ANIMATION_NORMAL);
				SetWindow(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y + PLAYER_POSY3, 0.0f), D3DXCOLOR{ 1.0f,1.0f,1.0f, 0.0f }, 32, 32, 20, 0, 0, WINDOWSTATE_GUARDOFF, WINDOWUSE_GUARD);
			}
		}
		else if (g_Player.bGuard == true)
		{//�K�[�h��
			fSlip *= 0.8f;			//����I
			g_Player.nGuardTime--;	//�K�[�h�J�E���gDOWN
			if (g_Player.nGuardTime == GUARD_BREAK - GUARD_TIME)
			{//GUARD����
				g_Player.bGuard = false;	//�K�[�h������
				g_Player.nGuardTime = 0;	//������
				ChangeGuard(2);				//�K�[�h�̌����ڍ폜
			}
		}

		g_nRecast++;	//�e�̘A�ː����J�E���g�p
		GetKeyboardQuick(DIK_A, false);
		if (g_Player.nStealth < 255)
		{
			g_Player.nStealth++;
		}
		if (g_Player.bKnifeLimit == false)
		{
			if (bStartShooter == false)
			{//�����V���[�^�[�̐�
				nSubShot = 0;
				g_fChannel = 0.01f;
				g_bChannel = true;
				for (nCount = 0; nCount < nSubShot; nCount++)
				{
					nRand = rand() % 64;
					SetShooter(D3DXVECTOR3(cosf(2.7f) * (-BULLET_SPEED * 0.5f), cosf(g_fChannel) * (BULLET_SPEED * 0.5f), 0.0f), nRand);
				}
				bStartShooter = true;
			}
			if (g_bChannel == true)
			{//�}�C�i�X�I �p�x
				g_fChannel -= 0.02f;
				//MessageBox(0, "a", "a", MB_OK);
			}
			else
			{//�v���X�I�@�p�x
				g_fChannel += 0.02f;
			}
			if (g_fChannel >= 3.14f)
			{//�O���˃��[�h�p
				g_fChannel = 3.14f;
				g_bChannel = true;
			}
			else if (g_fChannel <= 2.5f)
			{//�O���˃��[�h�p�@�͈́i���j1.57���^��
				g_fChannel = 2.5f;
				g_bChannel = false;
			}//�e����
			if (GetKeyboardTrigger(DIK_Z) == true || GetKeyboardTrigger(DIK_N) == true)
			{//������(���[�h)�ύX
				g_Player.nMode = (g_Player.nMode + 1) % 3;
			}
			if (GetKeyboardTrigger(DIK_X) == true || GetKeyboardTrigger(DIK_M) == true)
			{//�y�b�g�̐���@3�p�^�[��
				nSet = (nSet + 1) % 3;
			}

			if (g_Player.nMode == 2)
			{//�Q���rMODE�Ȃ�U�����xMAX
				nLate = 2;
			}
			else
			{//�ʏ�Ȃ�
				nLate = RECAST_TIME;
			}
			if (g_nRecast % nLate == 0 && g_Player.bGuard == false && nKnifeTime == 0)
			{//�e����
				PlaySound(SOUND_LABEL_SHOT000);
				if (g_Player.nMode == 2)
				{//MODE3�Ȃ�
					SetBullet(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y - 30.0f, 0.0f), D3DX_PI, BULLET2_LENGTHX, BULLET2_LENGTHY, 3, BULLET_ATKTYPE1, BULLET_SPEED * 3, BULLETTYPE_PLAYER, 0, BULLETSTATE_NONE, 0.0f, 0.0f, 0, 0);
					g_Player.move.y += cosf(0.0f) * fRun * 0.15f;	//����
					fSlip *= 0.8f;
				}
				else if (g_Player.nMode == 1)
				{//MODE2�Ȃ�
					SetBullet(g_Player.pos, D3DX_PI, BULLET0_LENGTHX, BULLET0_LENGTHY, 67, BULLET_ATKTYPE0, BULLET_SPEED, BULLETTYPE_PLAYER, 0, BULLETSTATE_NONE, 0.0f, 0.0f, 0, 0);
					SetBullet(g_Player.pos, g_fChannel, BULLET1_LENGTHX, BULLET1_LENGTHY, 19, BULLET_ATKTYPE0, BULLET_SPEED * 0.8f, BULLETTYPE_PLAYER, 0, BULLETSTATE_NONE, 0.0f, 0.0f, 0, 0);
					SetBullet(g_Player.pos, -g_fChannel, BULLET1_LENGTHX, BULLET1_LENGTHY, 19, BULLET_ATKTYPE0, BULLET_SPEED * 0.8f, BULLETTYPE_PLAYER, 0, BULLETSTATE_NONE, 0.0f, 0.0f, 0, 0);
				}
				else
				{//MODE1�Ȃ�
					SetBullet(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y, 0.0f), D3DX_PI, BULLET0_LENGTHX, BULLET0_LENGTHY, 51, BULLET_ATKTYPE0, BULLET_SPEED, BULLETTYPE_PLAYER, 0, BULLETSTATE_NONE, 0.0f, 0.0f, 0, 0);
					SetBullet(D3DXVECTOR3(g_Player.pos.x + PLAYER_POSX3 + BULLET0_LENGTHX, g_Player.pos.y + PLAYER_POSY3, 0.0f), D3DX_PI, BULLET0_LENGTHX, BULLET0_LENGTHY, 51, BULLET_ATKTYPE0, BULLET_SPEED, BULLETTYPE_PLAYER, 0, BULLETSTATE_NONE, 0.0f, 0.0f, 0, 0);
					SetBullet(D3DXVECTOR3(g_Player.pos.x - PLAYER_POSX3 - BULLET0_LENGTHX, g_Player.pos.y + PLAYER_POSY3, 0.0f), D3DX_PI, BULLET0_LENGTHX, BULLET0_LENGTHY, 51, BULLET_ATKTYPE0, BULLET_SPEED, BULLETTYPE_PLAYER, 0, BULLETSTATE_NONE, 0.0f, 0.0f, 0, 0);
				}
			}
		}
		if (g_Player.nGuardTime == GUARD_BREAK || g_Player.bGuard == true)
		{//�K�[�h�ɓ����܂ł�F
			if (GetKeyboardTrigger(DIK_LCONTROL) == true || GetKeyboardTrigger(DIK_RCONTROL) == true && g_Player.bGuard == false)
			{//Guard���
				g_Player.bGuard = true;		//�K�[�h��Ԃ�
				ChangeGuard(1);		//GUARD���� (������)
			}
		}
		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{//�K�E�̃i�C�t�����I�A�ł��Ă�_(:3�v��)_
			PlaySound(SOUND_LABEL_KNIFE000);
			SetBullet(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y + PLAYER_POSY3, 0.0f), D3DX_PI, KNIFE_LENGTHX, KNIFE_LENGTHY, 0, BULLET_ATKTYPE99, 0.0f, BULLETTYPE_PLAYER, 0, BULLETSTATE_NONE, 0.0f, 0.0f, 0, 0);
			nKnifeTime = 10;
		}
		else if (nKnifeTime > 0)
		{
			nKnifeTime--;
			fRun = fRun / 5.0f;
		}

		if (g_Player.nMode != 2 && g_Player.bGuard == false)
		{//Mode3�Ȃ瓮���Ȃ� &&�@�i�C�t���ł��Ȃ�
			if (GetKeyboardQuick(DIK_D, true) == true || GetKeyboardQuick(DIK_RIGHT, true) == true ||
				GetKeyboardQuick(DIK_A, true) == true || GetKeyboardQuick(DIK_LEFT, true) == true ||
				GetKeyboardQuick(DIK_W, true) == true || GetKeyboardQuick(DIK_UP, true) == true ||
				GetKeyboardQuick(DIK_S, true) == true || GetKeyboardQuick(DIK_DOWN, true) == true)
			{//�X�e�b�v�@�L�����Z���ł���͎̂d�l����I
				nBoostTime = 6;	//����5F
				if (g_Player.nStealth > 250)
				{//�X�e�b�v��5F�̖��G����
					g_Player.nStealth = 250;
				}
			}
			if (nBoostTime > 0)
			{
				fRun *= 8;
				nBoostTime--;
				if (nBoostTime == 0)
				{
					fRun = MOVEPOLYGON;
					g_Player.move *= 0.3f;
				}
			}

			if (GetKeyboardPress(DIK_LSHIFT) == true || GetKeyboardPress(DIK_RSHIFT) == true)
			{//�X���[
				fRun = fRun / 2.5f;
			}

			if (g_Player.bGuard == false)
			{
				if (GetKeyboardPress(DIK_D) == true || GetKeyboardPress(DIK_RIGHT) == true)
				{//D, ���ŉE�ړ�
					if (GetKeyboardPress(DIK_W) == true || GetKeyboardPress(DIK_UP) == true)
					{//�΂߂ɐ؂�ւ��@�E��
						g_Player.move.x += sinf(D3DX_PI * 0.75f) * fRun;
						g_Player.move.y += cosf(D3DX_PI * 0.75f) * fRun;
					}
					else if (GetKeyboardPress(DIK_S) == true || GetKeyboardPress(DIK_DOWN) == true)
					{//�΂߂ɐ؂�ւ��@�E��
						g_Player.move.x += sinf(D3DX_PI / 4) * fRun;
						g_Player.move.y += cosf(D3DX_PI / 4) * fRun;
					}
					else
					{//����Ȃ��@�E
						g_Player.move.x += sinf(D3DX_PI / 2) * fRun;
					}
				}
				else if (GetKeyboardPress(DIK_A) == true || GetKeyboardPress(DIK_LEFT) == true)
				{//A, ���ō��ړ�
					if (GetKeyboardPress(DIK_W) == true || GetKeyboardPress(DIK_UP) == true)
					{//�΂߂ɐ؂�ւ��@����
						g_Player.move.x += sinf(D3DX_PI * -0.75f) * fRun;
						g_Player.move.y += cosf(D3DX_PI * -0.75f) * fRun;
					}
					else if (GetKeyboardPress(DIK_S) == true || GetKeyboardPress(DIK_DOWN) == true)
					{//�΂߂ɐ؂�ւ��@����
						g_Player.move.x += sinf(D3DX_PI / -4) * fRun;
						g_Player.move.y += cosf(D3DX_PI / -4) * fRun;
					}
					else
					{//����Ȃ��@��
						g_Player.move.x += sinf(D3DX_PI / -2) * fRun;
					}
				}
				else if (GetKeyboardPress(DIK_W) == true || GetKeyboardPress(DIK_UP) == true)
				{//��ړ�
					g_Player.move.y += cosf(0.0f) * -fRun;
				}
				else if (GetKeyboardPress(DIK_S) == true || GetKeyboardPress(DIK_DOWN) == true)
				{//���ړ�
					g_Player.move.y += cosf(0.0f) * fRun;
				}
			}//Guard����
		}//MODE2����
		g_Player.pos += g_Player.move;//���@�̈ړ�
		if (g_Player.pos.y <= 0)
		{//�オ�o�Ă���Ζ߂�
			g_Player.pos.y = 0;
		}
		if (g_Player.pos.y + (PLAYER_POSY3 * 2) >= SCREEN_HEIGHT)
		{//��������
			g_Player.pos.y = SCREEN_HEIGHT - (PLAYER_POSY3 * 2);
		}
		if (g_Player.pos.x + PLAYER_POSX3 >= SCREEN_GAMEX)
		{//�������E
			g_Player.pos.x = SCREEN_GAMEX - PLAYER_POSX3;
		}
		if (g_Player.pos.x - PLAYER_POSX3 <= 0)
		{//��������
			g_Player.pos.x = PLAYER_POSX3;
		}

		UpdateShooter(g_Player.pos, nSet);

		//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);
		//�摜�ړ�
		pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - 180.0f, g_Player.pos.y - 180.0f + 20.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x + 180.0f, g_Player.pos.y - 180.0f + 20.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x - 180.0f, g_Player.pos.y + 180.0f + 20.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + 180.0f, g_Player.pos.y + 180.0f + 20.0f, 0.0f);

		//�ړ��ʒቺ(��C��R���̂���)
		g_Player.move.x += (0.0f - g_Player.move.x) * fSlip;
		g_Player.move.y += (0.0f - g_Player.move.y) * fSlip;

		if (g_Player.nMode == 2)
		{//�Q���r�����Ă�Ƃ��͐ԁI
			pVtx[0].col = D3DCOLOR_RGBA(100, 100, 100, g_Player.nStealth);	// A = �����x
			pVtx[1].col = D3DCOLOR_RGBA(100, 100, 100, g_Player.nStealth);
			pVtx[2].col = D3DCOLOR_RGBA(100, 100, 100, g_Player.nStealth);
			pVtx[3].col = D3DCOLOR_RGBA(100, 100, 100, g_Player.nStealth);
		}
		else
		{//��{�}�e���A����
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_Player.nStealth);	// A = �����x
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_Player.nStealth);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_Player.nStealth);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_Player.nStealth);
		}
		//���_BUFFER���A�����b�N����
		g_pVtxBuffPlayer->Unlock();
	}
}
//=========================================================================================================//
// * �v���C���[�̕`�揈��4	�`�揈��
//=========================================================================================================//
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X���擾����
	pDevice = GetDevice();

	//���_BUFFER���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);	// ���_�t�H�[�}�b�g�̐ݒ�

									//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturePlayer);

	//�|���S���̕`��
	if (g_Player.bUse == true)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_NUM);
	}

}
//=========================================================================================================//
// * �v���C���[�̏��������
//=========================================================================================================//
PLAYER *GetPlayer(void)
{
	return &g_Player;
}