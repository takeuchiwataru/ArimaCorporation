//=========================================================================================================//
//
// [�e����]->�e�ړ�:�������� [bullet.cpp]
// Author : Ryou Sugimoto
//
//=========================================================================================================//
#include "main.h"
#include "game.h"

//*********************************************************************************************************//
// �}�N����`
//*********************************************************************************************************//
#define EXPLOSION_VERTEX_NUM		(4)					// ���_�̐�
#define EXPLOSION_PRIMITIVE_NUM		(2)					// �v���~�e�B�u�̐�
#define EXPLOSION_MAX_TEX			(4)
#define EXPLOSION_TextureName		"data/TEXTURE/Explosion003.png"
#define EXPLOSION_TextureName1		"data/TEXTURE/pipo-btleffect108f.png"
#define EXPLOSION_TextureName2		"data/TEXTURE/warp1_640.png"
#define EXPLOSION_TextureName3		"data/TEXTURE/ranking000.png"
#define	MAX_EXPLOSION				(32)		// �e�̍ő吔
#define HALFEXPLOSION				(30)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 col;	//�F
	int nLengthX;		//X�̒���
	int nLengthY;		//Y�̒���
	int nCntAnim;		//Animation�̃J�E���g
	int nPtnAnim;		//�w�肳�ꂽ���W�ŕ\��
	int nPtnHeight;		//�摜�̏c�̕�����
	int nPtnWide;		//�摜�̉��̕�����
	int nPtnSpeed;		//Animation���x
	int nType;			//�摜�̎w��
	ANIMATION anim;		//�A�j���[�V�����̎g����
	bool bUse;
}Explosin;

//*********************************************************************************************************//
// �O���[�o���ϐ�
//*********************************************************************************************************//
LPDIRECT3DTEXTURE9			g_pTextureExplosion[EXPLOSION_MAX_TEX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffExplosion = NULL;						// ���_�o�b�t�@�ւ̃|�C���^
Explosin					g_aExplosion[MAX_EXPLOSION];					//�ő��Animation��

//=========================================================================================================//
// * �w�i�̕`�揈��1	����������
//=========================================================================================================//
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	int nCntBullet;

	// �e�̏��̏�����
	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{
		//g_aExplosion[nCntBullet].pos = { 0.0f, 0.0f, 0.0f };
		//g_aExplosion[nCntBullet].col = { 1.0f, 1.0f, 1.0f, };
		//g_aExplosion[nCntBullet].nCntAnim = -1;
		//g_aExplosion[nCntBullet].nPtnAnim = -1;
		//g_aExplosion[nCntBullet].nType = 0;
		g_aExplosion[nCntBullet].bUse = false;
	}

	//TEXTURE�ǂݍ���
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TextureName, &g_pTextureExplosion[0]);
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TextureName1, &g_pTextureExplosion[1]);
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TextureName2, &g_pTextureExplosion[2]);
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TextureName3, &g_pTextureExplosion[3]);

	//���_BUFFER�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * EXPLOSION_VERTEX_NUM * MAX_EXPLOSION,		//�m�ۂ���BUFFER�̃T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
							//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{
		//���_���W�̐ݒ�
		//pVtx[nCntBullet * 4].pos = D3DXVECTOR3(0, 0, 0.0f);
		//pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(0, 0, 0.0f);
		//pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(0, 0, 0.0f);
		//pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(0, 0, 0.0f);

		pVtx[nCntBullet * 4].rhw = 1.0f;
		pVtx[nCntBullet * 4 + 1].rhw = 1.0f;
		pVtx[nCntBullet * 4 + 2].rhw = 1.0f;
		pVtx[nCntBullet * 4 + 3].rhw = 1.0f;

		////�J���[�ݒ�
		//pVtx[nCntBullet * 4].col = D3DCOLOR_RGBA(0,0,0, 255);	// A = �����x
		//pVtx[nCntBullet * 4 + 1].col = D3DCOLOR_RGBA(0,0,0, 255);
		//pVtx[nCntBullet * 4 + 2].col = D3DCOLOR_RGBA(0,0,0, 255);
		//pVtx[nCntBullet * 4 + 3].col = D3DCOLOR_RGBA(0,0,0, 255);

		//�e�N�X�`�����W�ݒ�
		//pVtx[nCntBullet * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
		//pVtx[nCntBullet * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		//pVtx[nCntBullet * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		//pVtx[nCntBullet * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//���_BUFFER���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
}
//=========================================================================================================//
// * �w�i�̕`�揈��2	�I������
//=========================================================================================================//
void UninitExplosion(void)
{//�e�N�X�`���̔j��
	int nCntTexture;

	for (nCntTexture = 0; nCntTexture < EXPLOSION_MAX_TEX; nCntTexture++)
	{
		if (g_pTextureExplosion[nCntTexture] != NULL)
		{
			g_pTextureExplosion[nCntTexture]->Release();
			g_pTextureExplosion[nCntTexture] = NULL;
		}
	}
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}
//=========================================================================================================//
// * �w�i�̕`�揈��3	�X�V����
//=========================================================================================================//
void UpdateExplosion(void)
{
	int nCntBullet;
	PLAYER *pPlayer;
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X���擾����
	pDevice = GetDevice();

	//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{

		if (g_aExplosion[nCntBullet].bUse == true)
		{//�e��O�ɔ�΂�
		 //MessageBox(0, "�{�������H", "�I��", MB_YESNO);
			g_aExplosion[nCntBullet].nCntAnim++;

			if (g_aExplosion[nCntBullet].nType == 1)
			{
				pPlayer = GetPlayer();

				pVtx[nCntBullet * 4].pos = D3DXVECTOR3(pPlayer[0].pos.x - g_aExplosion[nCntBullet].nLengthX, pPlayer[0].pos.y + PLAYER_POSY3 - g_aExplosion[nCntBullet].nLengthY, 0.0f);
				pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(pPlayer[0].pos.x + g_aExplosion[nCntBullet].nLengthX, pPlayer[0].pos.y + PLAYER_POSY3 - g_aExplosion[nCntBullet].nLengthY, 0.0f);
				pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(pPlayer[0].pos.x - g_aExplosion[nCntBullet].nLengthX, pPlayer[0].pos.y + PLAYER_POSY3 + g_aExplosion[nCntBullet].nLengthY, 0.0f);
				pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(pPlayer[0].pos.x + g_aExplosion[nCntBullet].nLengthX, pPlayer[0].pos.y + PLAYER_POSY3 + g_aExplosion[nCntBullet].nLengthY, 0.0f);
			}
			else
			{
				pVtx[nCntBullet * 4].pos = D3DXVECTOR3(g_aExplosion[nCntBullet].pos.x - g_aExplosion[nCntBullet].nLengthX, g_aExplosion[nCntBullet].pos.y - g_aExplosion[nCntBullet].nLengthY, 0.0f);
				pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(g_aExplosion[nCntBullet].pos.x + g_aExplosion[nCntBullet].nLengthX, g_aExplosion[nCntBullet].pos.y - g_aExplosion[nCntBullet].nLengthY, 0.0f);
				pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(g_aExplosion[nCntBullet].pos.x - g_aExplosion[nCntBullet].nLengthX, g_aExplosion[nCntBullet].pos.y + g_aExplosion[nCntBullet].nLengthY, 0.0f);
				pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(g_aExplosion[nCntBullet].pos.x + g_aExplosion[nCntBullet].nLengthX, g_aExplosion[nCntBullet].pos.y + g_aExplosion[nCntBullet].nLengthY, 0.0f);

			}
			pVtx[nCntBullet * 4].tex = D3DXVECTOR2((g_aExplosion[nCntBullet].nPtnAnim % g_aExplosion[nCntBullet].nPtnWide) * (1.0f / g_aExplosion[nCntBullet].nPtnWide), (g_aExplosion[nCntBullet].nPtnAnim / g_aExplosion[nCntBullet].nPtnWide) * (1.0f / g_aExplosion[nCntBullet].nPtnHeight));
			pVtx[nCntBullet * 4 + 1].tex = D3DXVECTOR2((g_aExplosion[nCntBullet].nPtnAnim % g_aExplosion[nCntBullet].nPtnWide) * (1.0f / g_aExplosion[nCntBullet].nPtnWide) + (1.0f / g_aExplosion[nCntBullet].nPtnWide), (g_aExplosion[nCntBullet].nPtnAnim / g_aExplosion[nCntBullet].nPtnWide) * (1.0f / g_aExplosion[nCntBullet].nPtnHeight));
			pVtx[nCntBullet * 4 + 2].tex = D3DXVECTOR2((g_aExplosion[nCntBullet].nPtnAnim % g_aExplosion[nCntBullet].nPtnWide) * (1.0f / g_aExplosion[nCntBullet].nPtnWide), (g_aExplosion[nCntBullet].nPtnAnim / g_aExplosion[nCntBullet].nPtnWide) * (1.0f / g_aExplosion[nCntBullet].nPtnHeight) + (1.0f / g_aExplosion[nCntBullet].nPtnHeight));
			pVtx[nCntBullet * 4 + 3].tex = D3DXVECTOR2((g_aExplosion[nCntBullet].nPtnAnim % g_aExplosion[nCntBullet].nPtnWide) * (1.0f / g_aExplosion[nCntBullet].nPtnWide) + (1.0f / g_aExplosion[nCntBullet].nPtnWide), (g_aExplosion[nCntBullet].nPtnAnim / g_aExplosion[nCntBullet].nPtnWide) * (1.0f / g_aExplosion[nCntBullet].nPtnHeight) + (1.0f / g_aExplosion[nCntBullet].nPtnHeight));
			if (g_aExplosion[nCntBullet].nCntAnim % g_aExplosion[nCntBullet].nPtnSpeed == 0)
			{
				g_aExplosion[nCntBullet].nPtnAnim++;
				if (g_aExplosion[nCntBullet].nPtnAnim > g_aExplosion[nCntBullet].nPtnHeight * g_aExplosion[nCntBullet].nPtnWide && g_aExplosion[nCntBullet].anim != ANIMATION_LOOP)
				{//�S��1��\�������������
					g_aExplosion[nCntBullet].bUse = false;
				}
			}
		}
	}
	//���_BUFFER���A�����b�N����
	g_pVtxBuffExplosion->Unlock();

}
//=========================================================================================================//
// * �w�i�̕`�揈��4	�`�揈��
//=========================================================================================================//
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{
		if (g_aExplosion[nCntBullet].bUse == true)
		{
			// �e�N�X�`���̐ݒ� �|���S���̕`��
			pDevice->SetTexture(0, g_pTextureExplosion[g_aExplosion[nCntBullet].nType]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, EXPLOSION_PRIMITIVE_NUM);
		}
	}

}
//=============================================================================
// �e�̐ݒ菈��
//=============================================================================
void SetExplosion(D3DXVECTOR3 pos, COLLAR col , int nLengthX, int nLengthY, int nPtnWide, int nPtnHeight, int nPtnSpeed, int nType, ANIMATION anim)
{
	int nCntBullet;
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{
		if (g_aExplosion[nCntBullet].bUse == false)
		{
			g_aExplosion[nCntBullet].pos = pos;
			g_aExplosion[nCntBullet].nCntAnim = -1;
			g_aExplosion[nCntBullet].nPtnAnim = -1;
			g_aExplosion[nCntBullet].nPtnHeight = nPtnHeight;
			g_aExplosion[nCntBullet].nPtnWide = nPtnWide;
			g_aExplosion[nCntBullet].nPtnSpeed = nPtnSpeed;
			g_aExplosion[nCntBullet].nLengthX = nLengthX;
			g_aExplosion[nCntBullet].nLengthY = nLengthY;
			g_aExplosion[nCntBullet].nType = nType;
			g_aExplosion[nCntBullet].anim = anim;
			g_aExplosion[nCntBullet].bUse = true;

			pVtx[nCntBullet * 4].pos = D3DXVECTOR3(g_aExplosion[nCntBullet].pos.x - g_aExplosion[nCntBullet].nLengthX, g_aExplosion[nCntBullet].pos.y - g_aExplosion[nCntBullet].nLengthY, 0.0f);
			pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(g_aExplosion[nCntBullet].pos.x + g_aExplosion[nCntBullet].nLengthX, g_aExplosion[nCntBullet].pos.y - g_aExplosion[nCntBullet].nLengthY, 0.0f);
			pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(g_aExplosion[nCntBullet].pos.x - g_aExplosion[nCntBullet].nLengthX, g_aExplosion[nCntBullet].pos.y + g_aExplosion[nCntBullet].nLengthY, 0.0f);
			pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(g_aExplosion[nCntBullet].pos.x + g_aExplosion[nCntBullet].nLengthX, g_aExplosion[nCntBullet].pos.y + g_aExplosion[nCntBullet].nLengthY, 0.0f);

			pVtx[nCntBullet * 4].col = D3DCOLOR_RGBA(col.colR, col.colG, col.colB, 200);	// A = �����x
			pVtx[nCntBullet * 4 + 1].col = D3DCOLOR_RGBA(col.colR, col.colG, col.colB, 200);
			pVtx[nCntBullet * 4 + 2].col = D3DCOLOR_RGBA(col.colR, col.colG, col.colB, 200);
			pVtx[nCntBullet * 4 + 3].col = D3DCOLOR_RGBA(col.colR, col.colG, col.colB, 200);
			break;
		}
	}
	//���_BUFFER���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
}
//=========================================================================================================//
// * �SAnimation�j������
//=========================================================================================================//
void BreakExplosion(void)
{
	int nCntAnim;

	for (nCntAnim = 0; nCntAnim < MAX_EXPLOSION; nCntAnim++)
	{
		g_aExplosion[nCntAnim].bUse = false;
	}
}
