//=========================================================================================================//
//
// [�e����]->�e�ړ����� [bullet.cpp]
// Author : Ryou Sugimoto
//
//=========================================================================================================//
#include "main.h"
#include "game.h"				//�������ĂԂ���
#include "enemy.h"				//�G�̏ꏊ�m�F�̂���
#include "sound.h"

//*********************************************************************************************************//
// �}�N����`
//*********************************************************************************************************//
#define BULLET_VERTEX_NUM			(4)					// ���_�̐�
#define BULLET_PRIMITIVE_NUM		(2)					// �v���~�e�B�u�̐�
#define BULLET_TextureName		"data/TEXTURE/danmaku002.png"
#define BULLET_TextureName1		"data/TEXTURE/KNIFE000.png"
#define BULLET_MAX_Texture			(2)					//�摜�̎��
#define	MAX_BULLET			(3840)				// �e�̍ő吔
#define HIT_PLUS			(3.0f)				//�GHit����̒e�̑傫���̔{��
#define HIT_SAFE			(0.55f)				//���@Hit����̒e�̑傫���̔{��
#define KNIFE_STEEP			(D3DX_PI * -0.55f)	//�i�C�t�̏����p�x
#define KNIFE_STEEPENEMY	(D3DX_PI * -0.45f)	//�i�C�t�̏����p�x
#define BULLET_SLIP			(0.15f)				//�e�̊���
#define BULLET_DELETE		(200)
#define BULLET_DELETE1		(1)

//*********************************************************************************************************//
// �O���[�o���ϐ�
//*********************************************************************************************************//
LPDIRECT3DTEXTURE9		g_pTextureBullet[2] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	// ���_�o�b�t�@�ւ̃|�C���^		

//=========================================================================================================//
// * �w�i�̕`�揈��1	����������
//=========================================================================================================//
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	int nCntBullet;

	Bullet *g_aBullet;
	g_aBullet = GetBullet();

	// �e�̏��̏�����
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//g_aBullet[nCntBullet].pos = { 0.0f, 0.0f, 0.0f };
		//g_aBullet[nCntBullet].steep = 0.0f;
		//g_aBullet[nCntBullet].nType = 0;
		//g_aBullet[nCntBullet].nAtkType = 0;
		//g_aBullet[nCntBullet].nShot = 0;
		//g_aBullet[nCntBullet].nLengthX = 0;
		//g_aBullet[nCntBullet].nLengthY = 0;
		g_aBullet[nCntBullet].bUse = false;
	}

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, BULLET_TextureName, &g_pTextureBullet[0]);
	D3DXCreateTextureFromFile(pDevice, BULLET_TextureName1, &g_pTextureBullet[1]);

	//���_BUFFER�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * BULLET_VERTEX_NUM * MAX_BULLET,		//�m�ۂ���BUFFER�̃T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
							//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//���_���W�̐ݒ�
		/*pVtx[nCntBullet * 4].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(0, 0, 0.0f);*/

		pVtx[nCntBullet * 4].rhw = 1.0f;
		pVtx[nCntBullet * 4 + 1].rhw = 1.0f;
		pVtx[nCntBullet * 4 + 2].rhw = 1.0f;
		pVtx[nCntBullet * 4 + 3].rhw = 1.0f;

		//�J���[�ݒ�
		pVtx[nCntBullet * 4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// A = �����x
		pVtx[nCntBullet * 4 + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[nCntBullet * 4 + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[nCntBullet * 4 + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�ݒ�
		/*pVtx[nCntBullet * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nCntBullet * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[nCntBullet * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[nCntBullet * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);*/
	}

	//���_BUFFER���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}
//=========================================================================================================//
// * �w�i�̕`�揈��2	�I������
//=========================================================================================================//
void UninitBullet(void)
{//�e�N�X�`���̔j��
	int nCntTexture;

	for (nCntTexture = 0; nCntTexture < BULLET_MAX_Texture; nCntTexture++)
	{
		if (g_pTextureBullet[nCntTexture] != NULL)
		{
			g_pTextureBullet[nCntTexture]->Release();
			g_pTextureBullet[nCntTexture] = NULL;
		}
	}
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}
//=========================================================================================================//
// * �w�i�̕`�揈��3	�X�V����
//=========================================================================================================//
void UpdateBullet(void)
{
	static int nCntTime = 100;
	static int nCntGuard = 100;
	int nCntBullet;			//�e������`
	int nCntEnemy;			//�G������񂶂�`
	int nScore;

	bool bHit;
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
	ENEMY *pEnemy = GetEnemy();

	Bullet *g_aBullet;
	PLAYER *pPlayer;
	g_aBullet = GetBullet();
	pPlayer = GetPlayer();
	nCntTime++;
	nCntGuard++;

	//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{

		if (g_aBullet[nCntBullet].bUse == true)
		{//�e��O�ɔ�΂�
		 //MessageBox(0, "�{�������H", "�I��", MB_YESNO);
			g_aBullet[nCntBullet].nCntState[2]++;
			g_aBullet[nCntBullet].fSetSteep[0] += g_aBullet[nCntBullet].fSetSteep[1];
			switch (g_aBullet[nCntBullet].state)
			{
			case BULLETSTATE_NONE:			//���ʂɔ��
				g_aBullet[nCntBullet].move += D3DXVECTOR3(sinf(g_aBullet[nCntBullet].steep), cosf(g_aBullet[nCntBullet].steep), 0.0f) * g_aBullet[nCntBullet].fSpeed;
				break;
			case BULLETSTATE_START:			//�J�n������
				g_aBullet[nCntBullet].move += D3DXVECTOR3(sinf(g_aBullet[nCntBullet].steep), cosf(g_aBullet[nCntBullet].steep), 0.0f) * (g_aBullet[nCntBullet].fSpeed + g_aBullet[nCntBullet].fSpdPlus);
				g_aBullet[nCntBullet].state = BULLETSTATE_NONE;
				break;
			case BULLETSTATE_ACCELERATOR:	//������������
				g_aBullet[nCntBullet].fSpeed += g_aBullet[nCntBullet].fSpdPlus;
				g_aBullet[nCntBullet].move += D3DXVECTOR3(sinf(g_aBullet[nCntBullet].steep), cosf(g_aBullet[nCntBullet].steep), 0.0f) * g_aBullet[nCntBullet].fSpeed;
				break;
			case BULLETSTATE_STOP_S:			//�J�n���~�܂�
				g_aBullet[nCntBullet].nCntState[0]--;
				if (g_aBullet[nCntBullet].nCntState[0] < 1) { g_aBullet[nCntBullet].state = BULLETSTATE_START; }
				break;
			case BULLETSTATE_STOP_A:			//�J�n���~�܂�
				g_aBullet[nCntBullet].nCntState[0]--;
				if (g_aBullet[nCntBullet].nCntState[0] < 1) { g_aBullet[nCntBullet].state = BULLETSTATE_ACCELERATOR; }
				break;
			case BULLETSTATE_STOP_B:			//�J�n���~�܂�
				g_aBullet[nCntBullet].nCntState[0]--;
				if (g_aBullet[nCntBullet].nCntState[0] < 1) { g_aBullet[nCntBullet].state = BULLETSTATE_BACK; }
				break;
			case BULLETSTATE_BACK:				//-�ɂȂ��Ă���X�s�[�h�𐧌��t����+�ɖ߂��Ă���
				if (g_aBullet[nCntBullet].fSpeed < 0.5f) { g_aBullet[nCntBullet].fSpeed += g_aBullet[nCntBullet].fSpdPlus; }
				g_aBullet[nCntBullet].move += D3DXVECTOR3(sinf(g_aBullet[nCntBullet].steep), cosf(g_aBullet[nCntBullet].steep), 0.0f) * g_aBullet[nCntBullet].fSpeed;
				break;
			case BULLETSTATE_STRAIGHT_B:
				if (g_aBullet[nCntBullet].fSpeed < 0.5f) { g_aBullet[nCntBullet].fSpeed += g_aBullet[nCntBullet].fSpdPlus; }
			case BULLETSTATE_STRAIGHT:
				g_aBullet[nCntBullet].fCurve *= 0.95f;
				g_aBullet[nCntBullet].move += D3DXVECTOR3(sinf(g_aBullet[nCntBullet].steep), cosf(g_aBullet[nCntBullet].steep), 0.0f) * g_aBullet[nCntBullet].fSpeed;
				break;
			case BULLETSTATE_DELETE:		//�����Ă���
				g_aBullet[nCntBullet].move += D3DXVECTOR3(sinf(g_aBullet[nCntBullet].steep), cosf(g_aBullet[nCntBullet].steep), 0.0f) * g_aBullet[nCntBullet].fSpeed;
			case BULLETSTATE_FLASH:			//���̏�ŏ�����
				g_aBullet[nCntBullet].fcolA -= 0.03f;
				pVtx[nCntBullet * 4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aBullet[nCntBullet].fcolA);	// A = �����x
				pVtx[nCntBullet * 4 + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aBullet[nCntBullet].fcolA);
				pVtx[nCntBullet * 4 + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aBullet[nCntBullet].fcolA);
				pVtx[nCntBullet * 4 + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aBullet[nCntBullet].fcolA);
				if (g_aBullet[nCntBullet].fcolA <= 0.0f) { g_aBullet[nCntBullet].bUse = false; }
				break;
			case BULLETSTATE_SET_A:			//�΂������u��
				g_aBullet[nCntBullet].move += D3DXVECTOR3(sinf(g_aBullet[nCntBullet].steep), cosf(g_aBullet[nCntBullet].steep), 0.0f) * g_aBullet[nCntBullet].fSpeed;
				if (g_aBullet[nCntBullet].nCntState[2] % 2 == 0)
				{
					SetAttack(0, 2, 8, 8, g_aBullet[nCntBullet].nType % 16 + 16, g_aBullet[nCntBullet].nCntEnemy, g_aBullet[nCntBullet].nCntState[1], g_aBullet[nCntBullet].fSetSteep[0], 0.4f, 0.0001f, -0.005f, g_aBullet[nCntBullet].pos, 30.0f, BULLETSTATE_STOP_A);
				}
				break;
			case BULLETSTATE_SET_A1:			//�΂������u��
				g_aBullet[nCntBullet].move += D3DXVECTOR3(sinf(g_aBullet[nCntBullet].steep), cosf(g_aBullet[nCntBullet].steep), 0.0f) * g_aBullet[nCntBullet].fSpeed;
				if (g_aBullet[nCntBullet].nCntState[2] % 10 == 0)
				{
					SetAttack(0, 2, 10, 10, g_aBullet[nCntBullet].nType % 16 + 16, g_aBullet[nCntBullet].nCntEnemy, g_aBullet[nCntBullet].nCntState[1], g_aBullet[nCntBullet].fSetSteep[0], 0.4f, 0.0001f, -0.005f, g_aBullet[nCntBullet].pos, 30.0f, BULLETSTATE_STOP_A);
				}
				break;
			case BULLETSTATE_SET_B:			//BACK�̒e��u��
				g_aBullet[nCntBullet].move += D3DXVECTOR3(sinf(g_aBullet[nCntBullet].steep), cosf(g_aBullet[nCntBullet].steep), 0.0f) * g_aBullet[nCntBullet].fSpeed;
				if (g_aBullet[nCntBullet].nCntState[2] % 3 == 0)
				{
					SetAttack(0, 5, 8, 8, g_aBullet[nCntBullet].nType % 16 + 48, g_aBullet[nCntBullet].nCntEnemy, g_aBullet[nCntBullet].nCntState[1], g_aBullet[nCntBullet].fSetSteep[0], -0.5f, 0.005f, 0.0f, g_aBullet[nCntBullet].pos, 30.0f, BULLETSTATE_STOP_B);
				}
				break;
			case BULLETSTATE_SET_B1:		//BACK�̒e��u��
				g_aBullet[nCntBullet].move += D3DXVECTOR3(sinf(g_aBullet[nCntBullet].steep), cosf(g_aBullet[nCntBullet].steep), 0.0f) * g_aBullet[nCntBullet].fSpeed;
				if (g_aBullet[nCntBullet].nCntState[2] % 10 == 0)
				{
					SetAttack(0, 5, 8, 8, g_aBullet[nCntBullet].nType % 16 + 48, g_aBullet[nCntBullet].nCntEnemy, g_aBullet[nCntBullet].nCntState[1], g_aBullet[nCntBullet].fSetSteep[0], -0.5f, 0.005f, 0.0f, g_aBullet[nCntBullet].pos, 30.0f, BULLETSTATE_STOP_B);
				}
				break;
			case BULLETSTATE_SET_B2:		//BACK�̒e��u��
				g_aBullet[nCntBullet].move += D3DXVECTOR3(sinf(g_aBullet[nCntBullet].steep), cosf(g_aBullet[nCntBullet].steep), 0.0f) * g_aBullet[nCntBullet].fSpeed;
				if (g_aBullet[nCntBullet].nCntState[2] % 30 == 0)
				{
					SetAttack(0, 5, 8, 8, g_aBullet[nCntBullet].nType % 16 + 48, g_aBullet[nCntBullet].nCntEnemy, g_aBullet[nCntBullet].nCntState[1], g_aBullet[nCntBullet].fSetSteep[0], -0.5f, 0.005f, 0.0f, g_aBullet[nCntBullet].pos, 30.0f, BULLETSTATE_STOP_B);
				}
				break;
			}
			g_aBullet[nCntBullet].steep += g_aBullet[nCntBullet].fCurve;

			if (g_aBullet[nCntBullet].nAtkType >= BULLET_ATKTYPE99)
			{//�i�C�t�Ȃ�
				g_aBullet[nCntBullet].nAtkType++;
				if (g_aBullet[nCntBullet].nAtkType > KNIFE_TIME)
				{//�i�C�t���Ԑ؂�->false
					g_aBullet[nCntBullet].bUse = false;
				}
				if (g_aBullet[nCntBullet].nShot == BULLETTYPE_PLAYER)
				{
					g_aBullet[nCntBullet].steep -= 0.471f;
					pVtx[nCntBullet * 4].pos = D3DXVECTOR3(pPlayer[0].pos.x + (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY) + (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX), pPlayer[0].pos.y + PLAYER_POSY3 - (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX) + (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY), 0.0f);
					pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(pPlayer[0].pos.x + (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY) - (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX), pPlayer[0].pos.y + PLAYER_POSY3 + (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX) + (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY), 0.0f);
					pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(pPlayer[0].pos.x - (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY) + (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX), pPlayer[0].pos.y + PLAYER_POSY3 - (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX) - (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY), 0.0f);
					pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(pPlayer[0].pos.x - (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY) - (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX), pPlayer[0].pos.y + PLAYER_POSY3 + (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX) - (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY), 0.0f);
				}
				else
				{
					g_aBullet[nCntBullet].steep += 0.471f;
					pVtx[nCntBullet * 4].pos = D3DXVECTOR3(pEnemy[g_aBullet[nCntBullet].nCntEnemy].pos.x + (sinf(g_aBullet[nCntBullet].steep + pEnemy[g_aBullet[nCntBullet].nCntEnemy].fsteep) * g_aBullet[nCntBullet].nLengthY) + (cosf(g_aBullet[nCntBullet].steep + pEnemy[g_aBullet[nCntBullet].nCntEnemy].fsteep) * g_aBullet[nCntBullet].nLengthX), pEnemy[g_aBullet[nCntBullet].nCntEnemy].pos.y - (sinf(g_aBullet[nCntBullet].steep + pEnemy[g_aBullet[nCntBullet].nCntEnemy].fsteep) * g_aBullet[nCntBullet].nLengthX) + (cosf(g_aBullet[nCntBullet].steep + pEnemy[g_aBullet[nCntBullet].nCntEnemy].fsteep) * g_aBullet[nCntBullet].nLengthY), 0.0f);
					pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(pEnemy[g_aBullet[nCntBullet].nCntEnemy].pos.x + (sinf(g_aBullet[nCntBullet].steep + pEnemy[g_aBullet[nCntBullet].nCntEnemy].fsteep) * g_aBullet[nCntBullet].nLengthY) - (cosf(g_aBullet[nCntBullet].steep + pEnemy[g_aBullet[nCntBullet].nCntEnemy].fsteep) * g_aBullet[nCntBullet].nLengthX), pEnemy[g_aBullet[nCntBullet].nCntEnemy].pos.y + (sinf(g_aBullet[nCntBullet].steep + pEnemy[g_aBullet[nCntBullet].nCntEnemy].fsteep) * g_aBullet[nCntBullet].nLengthX) + (cosf(g_aBullet[nCntBullet].steep + pEnemy[g_aBullet[nCntBullet].nCntEnemy].fsteep) * g_aBullet[nCntBullet].nLengthY), 0.0f);
					pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(pEnemy[g_aBullet[nCntBullet].nCntEnemy].pos.x - (sinf(g_aBullet[nCntBullet].steep + pEnemy[g_aBullet[nCntBullet].nCntEnemy].fsteep) * g_aBullet[nCntBullet].nLengthY) + (cosf(g_aBullet[nCntBullet].steep + pEnemy[g_aBullet[nCntBullet].nCntEnemy].fsteep) * g_aBullet[nCntBullet].nLengthX), pEnemy[g_aBullet[nCntBullet].nCntEnemy].pos.y - (sinf(g_aBullet[nCntBullet].steep + pEnemy[g_aBullet[nCntBullet].nCntEnemy].fsteep) * g_aBullet[nCntBullet].nLengthX) - (cosf(g_aBullet[nCntBullet].steep + pEnemy[g_aBullet[nCntBullet].nCntEnemy].fsteep) * g_aBullet[nCntBullet].nLengthY), 0.0f);
					pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(pEnemy[g_aBullet[nCntBullet].nCntEnemy].pos.x - (sinf(g_aBullet[nCntBullet].steep + pEnemy[g_aBullet[nCntBullet].nCntEnemy].fsteep) * g_aBullet[nCntBullet].nLengthY) - (cosf(g_aBullet[nCntBullet].steep + pEnemy[g_aBullet[nCntBullet].nCntEnemy].fsteep) * g_aBullet[nCntBullet].nLengthX), pEnemy[g_aBullet[nCntBullet].nCntEnemy].pos.y + (sinf(g_aBullet[nCntBullet].steep + pEnemy[g_aBullet[nCntBullet].nCntEnemy].fsteep) * g_aBullet[nCntBullet].nLengthX) - (cosf(g_aBullet[nCntBullet].steep + pEnemy[g_aBullet[nCntBullet].nCntEnemy].fsteep) * g_aBullet[nCntBullet].nLengthY), 0.0f);
				}
			}
			else
			{//�ʏ�e�Ȃ�����Ă�����ɔ��
				g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;
				pVtx[nCntBullet * 4].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY) + (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX), g_aBullet[nCntBullet].pos.y - (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX) + (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY), 0.0f);
				pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY) - (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX), g_aBullet[nCntBullet].pos.y + (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX) + (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY), 0.0f);
				pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY) + (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX), g_aBullet[nCntBullet].pos.y - (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX) - (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY), 0.0f);
				pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY) - (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX), g_aBullet[nCntBullet].pos.y + (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX) - (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY), 0.0f);
				//�ړ��ʒቺ(��C��R���̂���)
				g_aBullet[nCntBullet].move.x += (0.0f - g_aBullet[nCntBullet].move.x) * BULLET_SLIP;
				g_aBullet[nCntBullet].move.y += (0.0f - g_aBullet[nCntBullet].move.y) * BULLET_SLIP;
			}

			if (g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].nLengthY <= -BULLET_DELETE || g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].nLengthY >= SCREEN_HEIGHT + BULLET_DELETE || g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].nLengthX <= -BULLET_DELETE || g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].nLengthX >= SCREEN_GAMEX + BULLET_DELETE)
			{//��ʊO+200�̒e������
				g_aBullet[nCntBullet].bUse = false;
			}
			if (g_aBullet[nCntBullet].state >= BULLETSTATE_SET_A)
			{//�Z�b�g����e�̏ꍇ && ���̋����o�����G���U���\��ԂȂ�
				if (g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].nLengthY <= -BULLET_DELETE1 || g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].nLengthY >= SCREEN_HEIGHT + BULLET_DELETE1 || g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].nLengthX <= -BULLET_DELETE1 || g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].nLengthX >= SCREEN_GAMEX + BULLET_DELETE1)
				{//��ʊO�ɂȂ�����
					StartBullet(g_aBullet[nCntBullet].nCntEnemy, g_aBullet[nCntBullet].nCntState[1]);
					g_aBullet[nCntBullet].bUse = false;
					if (nCntTime > 30)
					{
						PlaySound(SOUND_LABEL_KIRA);
						nCntTime = 0;
					}
				}
			}

			if (g_aBullet[nCntBullet].nShot == BULLETTYPE_PLAYER)
			{//���@���������e�Ȃ�
				pEnemy = GetEnemy();
				for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
				{//�G�̐��������
					if (pEnemy[nCntEnemy].bUse == true && pEnemy[nCntEnemy].state != ENEMYSTATE_TIME)
					{//�G������I
						bHit = HitAim(g_aBullet[nCntBullet], pEnemy[nCntEnemy].pos, pEnemy[nCntEnemy].nLength, HIT_PLUS);
						if (bHit == true)
						{//�e���������Ă�
							nScore = 10;
							if (g_aBullet[nCntBullet].nAtkType == BULLET_ATKTYPE0)
							{//�ʏ�e
								ExplosionEffect(g_aBullet[nCntBullet].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR{ 1.0f, 0.98f, 0.04f, 0.5f }, 55, 50, 0.1f, 0.5f, 0.1f, 0.5f);
								HitEnemy(nCntEnemy, 1, g_aBullet[nCntBullet].nAtkType);
								g_aBullet[nCntBullet].bUse = false;	//�e�̏���
							}
							else if (g_aBullet[nCntBullet].nAtkType == BULLET_ATKTYPE1)
							{//����
								ExplosionEffect(pEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR{ 1.0f, 0.98f, 0.04f, 0.5f }, 55, 50, 0.1f, 0.5f, 0.1f, 0.5f);
								HitEnemy(nCntEnemy, 1, g_aBullet[nCntBullet].nAtkType);
								g_aBullet[nCntBullet].nAtkType = BULLET_ATKTYPE50;
								nScore = 30;
							}
							else if(g_aBullet[nCntBullet].nAtkType >= BULLET_ATKTYPE99)
							{//�i�C�t
								PlaySound(SOUND_LABEL_HIT_KNIFE000);
								ExplosionEffect(pEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR{ 1.0f, 0.05f, 0.05f, 0.5f }, 55, 50, 0.1f, 0.5f, 0.1f, 0.5f);
								ExplosionEffect(pEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR{ 1.0f, 0.05f, 0.05f, 0.5f }, 55, 50, 0.1f, 0.5f, 0.1f, 0.5f);
								ExplosionEffect(pEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR{ 1.0f, 0.05f, 0.05f, 0.5f }, 55, 50, 0.1f, 0.5f, 0.1f, 0.5f);
								ExplosionEffect(pEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR{ 1.0f, 0.05f, 0.05f, 0.5f }, 55, 50, 0.1f, 0.5f, 0.1f, 0.5f);
								ExplosionEffect(pEnemy[nCntEnemy].pos, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), D3DXCOLOR{ 1.0f, 0.05f, 0.05f, 0.5f }, 55, 50, 0.1f, 0.5f, 0.1f, 0.5f);
								HitEnemy(nCntEnemy, 3, g_aBullet[nCntBullet].nAtkType);			//�G�Ƀ_���[�W
								nScore = 750;
							}
							else if (g_aBullet[nCntBullet].nAtkType == BULLET_ATKTYPE50)
							{//�����e
								ExplosionEffect(pEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR{ 1.0f, 0.98f, 0.04f, 0.5f }, 55, 50, 0.1f, 0.5f, 0.1f, 0.5f);
							}
							AddScore(nScore, 0);
						}//�q�b�g����
					}//�G���ݔ���
				}//for����
			}
			else
			{//�G�����������Ȃ�
				if (pPlayer[0].bUse == true && g_aBullet[nCntBullet].fcolA == 1.0f)
				{//���@������I
					if (pPlayer[0].bGuard == false && pPlayer[0].nStealth > 254)
					{// �K�[�h����&&���G��ԂłȂ�!
						bHit = HitAim(g_aBullet[nCntBullet], D3DXVECTOR3{ pPlayer[0].pos.x, pPlayer[0].pos.y + PLAYER_POSY3, 0.0f }, 0, HIT_SAFE);
						if (bHit == true)
						{//�e���������Ă�
							
							ExplosionEffect(D3DXVECTOR3{ pPlayer[0].pos.x, pPlayer[0].pos.y + PLAYER_POSY3, 0.0f }, pPlayer[0].move, D3DXCOLOR{ 1.0f, 0.98f, 0.04f, 0.8f }, 50, 100, 0.1f, 8.0f, 0.3f, 100.0f);
							ExplosionEffect(D3DXVECTOR3{ pPlayer[0].pos.x, pPlayer[0].pos.y + PLAYER_POSY3, 0.0f }, pPlayer[0].move, D3DXCOLOR{ 1.0f, 0.98f, 0.04f, 0.75f }, 100, 100, 0.5f, 13.0f, 0.1f, 3.0f);
							ExplosionEffect(D3DXVECTOR3{ pPlayer[0].pos.x, pPlayer[0].pos.y + PLAYER_POSY3, 0.0f }, pPlayer[0].move, D3DXCOLOR{ 0.49f, 1.0f, 0.97f, 0.75f }, 100, 100, 0.5f, 13.0f, 0.1f, 3.0f);
							ExplosionEffect(D3DXVECTOR3{ pPlayer[0].pos.x, pPlayer[0].pos.y + PLAYER_POSY3, 0.0f }, pPlayer[0].move, D3DXCOLOR{ 1.0f, 0.05f, 0.05f, 0.75f }, 100, 100, 0.5f, 13.0f, 0.1f, 3.0f);
							ExplosionEffect(D3DXVECTOR3{ pPlayer[0].pos.x, pPlayer[0].pos.y + PLAYER_POSY3, 0.0f }, pPlayer[0].move, D3DXCOLOR{ 1.0f, 0.05f, 0.05f, 0.75f }, 100, 100, 0.5f, 13.0f, 0.1f, 3.0f);
							ExplosionEffect(D3DXVECTOR3{ pPlayer[0].pos.x, pPlayer[0].pos.y + PLAYER_POSY3, 0.0f }, pPlayer[0].move, D3DXCOLOR{ 1.0f, 0.05f, 0.05f, 0.75f }, 100, 100, 0.5f, 13.0f, 0.1f, 3.0f);

							SetExplosion(D3DXVECTOR3{ pPlayer[0].pos.x, pPlayer[0].pos.y + PLAYER_POSY3, 0.0f }, COLLAR{ 255,252,10 }, HALF_EXPLOSION * 10, HALF_EXPLOSION * 10, 5, 4, 2, 0, ANIMATION_NORMAL);
							PlaySound(SOUND_LABEL_BOMB001);
							BreakShooter();
							ChangeGuard(2);
							pPlayer[0].bUse = false;
							AddScore(-5000, 0);
							if (g_aBullet[nCntBullet].state >= BULLETSTATE_SET_A) { StartBullet(g_aBullet[nCntBullet].nCntEnemy, g_aBullet[nCntBullet].nCntState[1]); }
						}//�q�b�g����
					}
					else if(pPlayer[0].bGuard == true)
					{
						bHit = HitAim(g_aBullet[nCntBullet], D3DXVECTOR3{ pPlayer[0].pos.x, pPlayer[0].pos.y + PLAYER_POSY3, 0.0f }, 32, HIT_PLUS);
						if (bHit == true)
						{//�K�[�h����
							ChangeGuard(0);
							g_aBullet[nCntBullet].bUse = false;
							AddScore(300, 0);
							SetExplosion(g_aBullet[nCntBullet].pos, COLLAR{ 255, 255, 255, 200 }, 10, 10, 5, 4, 2, 0, ANIMATION_NORMAL);
							if(g_aBullet[nCntBullet].state >= BULLETSTATE_SET_A){ StartBullet(g_aBullet[nCntBullet].nCntEnemy, g_aBullet[nCntBullet].nCntState[1]); }
							if (nCntGuard > 0) 
							{ 
								PlaySound(SOUND_LABEL_GUARD000);
								nCntGuard = 0;
							}
						}
					}
				}//���@���ݔ���
			}
		}
	}
	//���_BUFFER���A�����b�N����
	g_pVtxBuffBullet->Unlock();

}
//=========================================================================================================//
// * �e�̓����蔻��
//=========================================================================================================//
bool HitAim(Bullet pBullet, D3DXVECTOR3 aim, int nLength, float fPlusHit)
{
	float fLengthX;			//�e�̒��S����ڕW�̒��S�܂ł̋���X
	float fLengthY;			//�e�̒��S����ڕW�̒��S�܂ł̋���Y
	float fLengthAim;		//�ڕW�܂ł̋���(��)
	float fAimSteep = 1.0;	//�ڕW�܂ł̊p�x -> ������
	float fBullSteep;		//�e�̊p�x
	float fLengthBX;		//�e�̗L���̕�X
	float fLengthBY;		//�e�̗L���̕�Y
	float fLengthB;			//�e�̗L���̕�(���v)
	float fSteep[2] = { 1.0f, 1.0f };	//X���̃}�C�i�X�̕ۑ��p -> ������

	float fPercentY = 1.0f;		//�ȉ~����Y�̔{��
	float fPercentX = 1.0f;		//�ȉ~����X�̔{��

	fLengthX = aim.x - pBullet.pos.x;	//��X����
	fLengthY = aim.y - pBullet.pos.y;	//��Y����
	if (fLengthX < 0.0f)
	{//X�̒l���}�C�i�X�Ȃ�v���X��
		fLengthX *= -1;	//X���]
		fSteep[0] *= -1;	//X�����}�C�i�X��ۑ�
	}
	if (fLengthY < 0.0f)
	{//Y�̒l���}�C�i�X�Ȃ�v���X��
		fLengthY *= -1;		//Y���]
		fSteep[1] *= -1;	//Y�����}�C�i�X��ۑ�
	}
	

	//����(��)��2��̏�Ԃő��
	fLengthAim = (fLengthX * fLengthX) + (fLengthY * fLengthY);
	fLengthAim -= nLength * nLength;

	//���ŖڕW�ւ̊p�x->sin, cos�ɕϊ� 100����1.57�Ƃ���
	fAimSteep = fLengthY / (fLengthX + fLengthY) * (D3DX_PI / 2) * fSteep[1];
	fAimSteep = (D3DX_PI / 2 + fAimSteep) * fSteep[0];
	//fAimSteep = atan2f(fLengthX * fSteep[0], fLengthY * fSteep[1]);

	//�e�̊p�x�ƖڕW�̊p�x�̍���
	fBullSteep = pBullet.steep;
	if (fBullSteep < 0)
	{//�␳�̕K�v���Ȃ��ꍇ
		fBullSteep = D3DX_PI + fBullSteep;
		fAimSteep -= fBullSteep;
	}
	else
	{//�e�̊p�x���ŏ��l�ɖ߂�
		fBullSteep = D3DX_PI - fBullSteep;
		fAimSteep += fBullSteep;
	}

	if (pBullet.nLengthY > pBullet.nLengthX || pBullet.nLengthY < pBullet.nLengthX)
	{//�|���S����XY�̔䗦�ɉ����Ċp�x��蒼��
		if (pBullet.nLengthY > pBullet.nLengthX)
		{//Y���������ꍇ�@Y�̔䗦�ۑ�
			fPercentX = sinf(fAimSteep) * pBullet.nLengthY / pBullet.nLengthX;
			fPercentY = cosf(fAimSteep) * 1.0f;
		}
		if (pBullet.nLengthY < pBullet.nLengthX)
		{//X���������ꍇ�@X�̔䗦�ۑ�
			fPercentX = sinf(fAimSteep) * 1.0f;
			fPercentY = cosf(fAimSteep) * pBullet.nLengthX / pBullet.nLengthY;
		}

		fAimSteep = 1;	//�p�x������
		fSteep[0] = 1;	//�}�C�i�X�ۑ��p��������
		if (fPercentX < 0.0f)
		{//X���̃}�C�i�X�ۑ�
			fPercentX *= -1;
			fSteep[0] = -1;
		}
		if (fPercentY < 0.0f)
		{//Y���̃}�C�i�X�ۑ�
			fPercentY *= -1;
			fAimSteep = -1;
		}
		//���ŖڕW�ւ̊p�x->sin, cos�ɕϊ� 100����1.57�Ƃ��� �ȉ~����
		fAimSteep = fPercentY / (fPercentX + fPercentY) * (D3DX_PI / 2) * fAimSteep;
		fAimSteep = (D3DX_PI / 2 + fAimSteep) * fSteep[0];
	}

	fLengthBY = cosf(fAimSteep) * pBullet.nLengthY;
	fLengthBX = sinf(fAimSteep) * pBullet.nLengthX;

	fLengthB = (fLengthBX + fLengthBY) * (fLengthBX + fLengthBY);	//�e�̗L���ȕ�(���v)����

	if (fLengthAim < fLengthB * fPlusHit)
	{//����@2�悳�ꂽ��ԂȂ̂�
		return true;
	}
	else
	{
		return false;
	}
}
//=========================================================================================================//
// * �e�̕`�揈��4	�`�揈��
//=========================================================================================================//
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet;

	Bullet *g_aBullet;
	g_aBullet = GetBullet();

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ���u�����f�B���O�����Z�����ɐݒ�
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, 2);
	
	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_NUM);
	// �|���S���̕`��
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			if (g_aBullet[nCntBullet].nAtkType < BULLET_ATKTYPE99) { pDevice->SetTexture(0, g_pTextureBullet[0]); }
			else { pDevice->SetTexture(0, g_pTextureBullet[1]); }
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, BULLET_PRIMITIVE_NUM);
		}
	}

	// ���u�����f�B���O�����ɖ߂�
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
//=============================================================================
// �e�̐ݒ菈��
//=============================================================================
void SetBullet(D3DXVECTOR3 pos, float steep, int nLengthX, int nLengthY, int nType, int nAtkType, float fSpeed, BULLETTYPE nShot, int nCntEnemy, BULLETSTATE state, float fSpdPlus, float fCurve, int nCntstate0, int nCntstate1)
{
	int nCntBullet;			//�e������́`
	int nClear = 255;		//�����ݒ�̂���
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	ENEMY *pEnemy = GetEnemy();

	Bullet *g_aBullet;
	g_aBullet = GetBullet();

	//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{
			//�܂Ƃ߂ď�����
			g_aBullet[nCntBullet] = { pos, D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }, state,{ nCntstate0, nCntstate1, 0 }, nLengthX, nLengthY, nType, nAtkType, nShot, nCntEnemy, steep, {pEnemy[nCntEnemy].attack[nCntstate1].fSetAngle[0],pEnemy[nCntEnemy].attack[nCntstate1].fSetAngle[1] }, fCurve, fSpeed, fSpdPlus, 1.0f, true };
			//g_aBullet[nCntBullet].pos = pos;
			//g_aBullet[nCntBullet].move = D3DXVECTOR3{ 0.0f, 0.0f, 0.0f };
			//g_aBullet[nCntBullet].state = state;
			//g_aBullet[nCntBullet].nCntState[0] = nCntstate0;
			//g_aBullet[nCntBullet].nCntState[1] = nCntstate1;
			//g_aBullet[nCntBullet].nCntState[2] = 0;
			//g_aBullet[nCntBullet].nLengthX = nLengthX;
			//g_aBullet[nCntBullet].nLengthY = nLengthY;
			//g_aBullet[nCntBullet].nType = nType;
			//g_aBullet[nCntBullet].nAtkType = nAtkType;
			//g_aBullet[nCntBullet].nShot = nShot;
			//g_aBullet[nCntBullet].nCntEnemy = nCntEnemy;
			//g_aBullet[nCntBullet].steep = steep;
			//g_aBullet[nCntBullet].fSetSteep[0] = pEnemy[nCntEnemy].attack[nCntstate1].fSetAngle[0];
			//g_aBullet[nCntBullet].fSetSteep[1] = pEnemy[nCntEnemy].attack[nCntstate1].fSetAngle[1];
			//g_aBullet[nCntBullet].fSpeed = fSpeed;
			//g_aBullet[nCntBullet].fSpdPlus = fSpdPlus;
			//g_aBullet[nCntBullet].fcolA = 1.0f;
			//g_aBullet[nCntBullet].bUse = true;

			if (g_aBullet[nCntBullet].nAtkType == BULLET_ATKTYPE99)
			{//�i�C�t
				if (nShot == BULLETTYPE_PLAYER) { g_aBullet[nCntBullet].steep = KNIFE_STEEP; }
				if (nShot == BULLETTYPE_ENEMY)  { g_aBullet[nCntBullet].steep = KNIFE_STEEPENEMY + pEnemy[nCntEnemy].fsteep; }
				pVtx[nCntBullet * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCntBullet * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[nCntBullet * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[nCntBullet * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
			else
			{//�ʏ�e
				g_aBullet[nCntBullet].steep = steep;		//�e�̊p�x�����ƂɃ|���S���̊p�x�𒲐�
				pVtx[nCntBullet * 4].tex = D3DXVECTOR2((nType % 16) * 0.0625f, (nType / 16) * 0.1f);
				pVtx[nCntBullet * 4 + 1].tex = D3DXVECTOR2((nType % 16) * 0.0625f + 0.0625f, (nType / 16) * 0.1f);
				pVtx[nCntBullet * 4 + 2].tex = D3DXVECTOR2((nType % 16) * 0.0625f, (nType / 16) * 0.1f + 0.1f);
				pVtx[nCntBullet * 4 + 3].tex = D3DXVECTOR2((nType % 16) * 0.0625f + 0.0625f, (nType / 16) * 0.1f + 0.1f);
			}
			pVtx[nCntBullet * 4].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY) + (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX), g_aBullet[nCntBullet].pos.y - (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX) + (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY), 0.0f);
			pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY) - (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX), g_aBullet[nCntBullet].pos.y + (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX) + (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY), 0.0f);
			pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY) + (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX), g_aBullet[nCntBullet].pos.y - (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX) - (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY), 0.0f);
			pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY) - (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX), g_aBullet[nCntBullet].pos.y + (sinf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthX) - (cosf(g_aBullet[nCntBullet].steep) * g_aBullet[nCntBullet].nLengthY), 0.0f);

			g_aBullet[nCntBullet].nShot = nShot;

			nClear = 255;
			if (g_aBullet[nCntBullet].nShot == 0 && g_aBullet[nCntBullet].nAtkType < BULLET_ATKTYPE99)
			{
				nClear = 180;
				if (g_aBullet[nCntBullet].nAtkType == BULLET_ATKTYPE1 || g_aBullet[nCntBullet].nAtkType == BULLET_ATKTYPE50)
				{
					nClear = 80;
				}
			}
			if (g_aBullet[nCntBullet].state == BULLETSTATE_FLASH) { nClear = 200; }
			
			pVtx[nCntBullet * 4].col = D3DCOLOR_RGBA(255, 255, 255, nClear);	// A = �����x
			pVtx[nCntBullet * 4 + 1].col = D3DCOLOR_RGBA(255, 255, 255, nClear);
			pVtx[nCntBullet * 4 + 2].col = D3DCOLOR_RGBA(255, 255, 255, nClear);
			pVtx[nCntBullet * 4 + 3].col = D3DCOLOR_RGBA(255, 255, 255, nClear);
			break;
		}
	}
	//���_BUFFER���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}
//=============================================================================
// �e�̐ݒ菈��
//=============================================================================
void StartBullet(int nCntEnemy, int nCntAtk)
{
	int nCntBullet;
	Bullet *g_aBullet = GetBullet();

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].nCntEnemy == nCntEnemy && g_aBullet[nCntBullet].nCntState[1] == nCntAtk && g_aBullet[nCntBullet].bUse == true)
		{
			g_aBullet[nCntBullet].nCntState[0] = 0;
		}
	}
}
//=============================================================================
// �e�̐ݒ菈��
//=============================================================================
void BreakBullet(void)
{
	int nCntBullet;
	Bullet *g_aBullet = GetBullet();

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].bUse = false;
	}
}
//=============================================================================
// �e�̐ݒ菈��
//=============================================================================
void BreakBullet(int nCntEnemy)
{
	int nCntBullet;
	Bullet *g_aBullet;
	g_aBullet = GetBullet();

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{//�w�肵���G�l�~�[�̒e��j��
		if(g_aBullet[nCntBullet].nCntEnemy == nCntEnemy)
		{ 
			g_aBullet[nCntBullet].state = BULLETSTATE_DELETE;
			//ExplosionEffect(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].move, D3DXCOLOR{ 1.0f, 0.05f, 0.05f, 0.75f }, 100, 100, 0.1f, g_aBullet[nCntBullet].nLengthX * 0.05f, 0.1f, 1.0f);
		}
		//�G�t�F�N�g�~������
	}
}
//=============================================================================
// �e�̎擾
//=============================================================================
Bullet *GetBullet(void)
{
	static Bullet bullet[MAX_BULLET];//�e�̍ő吔

	return &bullet[0];
}