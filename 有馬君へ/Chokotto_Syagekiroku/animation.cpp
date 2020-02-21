//=========================================================================================================//
//
// �A�j���[�V�������� [animation.cpp]
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
#define EXPLOSION_MAX_TEX			(1)
#define EXPLOSION_TextureName		"data/TEXTURE/pipo-btleffect084.png"
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
}Animation;

//*********************************************************************************************************//
// �O���[�o���ϐ�
//*********************************************************************************************************//
LPDIRECT3DTEXTURE9			g_pTextureAnimation[EXPLOSION_MAX_TEX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffAnimation = NULL;						// ���_�o�b�t�@�ւ̃|�C���^
Animation					g_aAnimation[MAX_EXPLOSION];					//�ő��Animation��

//=========================================================================================================//
// * �w�i�̕`�揈��1	����������
//=========================================================================================================//
void InitAnimation(void)
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
		g_aAnimation[nCntBullet].bUse = false;
	}

	//TEXTURE�ǂݍ���
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TextureName, &g_pTextureAnimation[0]);

	//���_BUFFER�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * EXPLOSION_VERTEX_NUM * MAX_EXPLOSION,		//�m�ۂ���BUFFER�̃T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffAnimation,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
							//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffAnimation->Lock(0, 0, (void**)&pVtx, 0);
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
	g_pVtxBuffAnimation->Unlock();
}
//=========================================================================================================//
// * �w�i�̕`�揈��2	�I������
//=========================================================================================================//
void UninitAnimation(void)
{//�e�N�X�`���̔j��
	int nCntTexture;

	for (nCntTexture = 0; nCntTexture < EXPLOSION_MAX_TEX; nCntTexture++)
	{
		if (g_pTextureAnimation[nCntTexture] != NULL)
		{
			g_pTextureAnimation[nCntTexture]->Release();
			g_pTextureAnimation[nCntTexture] = NULL;
		}
	}
	if (g_pVtxBuffAnimation != NULL)
	{
		g_pVtxBuffAnimation->Release();
		g_pVtxBuffAnimation = NULL;
	}
}
//=========================================================================================================//
// * �w�i�̕`�揈��3	�X�V����
//=========================================================================================================//
void UpdateAnimation(void)
{
	int nCntBullet;
	PLAYER *pPlayer;
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X���擾����
	pDevice = GetDevice();

	//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffAnimation->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{

		if (g_aAnimation[nCntBullet].bUse == true)
		{//�e��O�ɔ�΂�
		 //MessageBox(0, "�{�������H", "�I��", MB_YESNO);
			g_aAnimation[nCntBullet].nCntAnim++;

			if (g_aAnimation[nCntBullet].nType == 1)
			{
				pPlayer = GetPlayer();

				pVtx[nCntBullet * 4].pos = D3DXVECTOR3(pPlayer[0].pos.x - g_aAnimation[nCntBullet].nLengthX, pPlayer[0].pos.y + PLAYER_POSY3 - g_aAnimation[nCntBullet].nLengthY, 0.0f);
				pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(pPlayer[0].pos.x + g_aAnimation[nCntBullet].nLengthX, pPlayer[0].pos.y + PLAYER_POSY3 - g_aAnimation[nCntBullet].nLengthY, 0.0f);
				pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(pPlayer[0].pos.x - g_aAnimation[nCntBullet].nLengthX, pPlayer[0].pos.y + PLAYER_POSY3 + g_aAnimation[nCntBullet].nLengthY, 0.0f);
				pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(pPlayer[0].pos.x + g_aAnimation[nCntBullet].nLengthX, pPlayer[0].pos.y + PLAYER_POSY3 + g_aAnimation[nCntBullet].nLengthY, 0.0f);
			}
			else
			{
				pVtx[nCntBullet * 4].pos = D3DXVECTOR3(g_aAnimation[nCntBullet].pos.x - g_aAnimation[nCntBullet].nLengthX, g_aAnimation[nCntBullet].pos.y - g_aAnimation[nCntBullet].nLengthY, 0.0f);
				pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(g_aAnimation[nCntBullet].pos.x + g_aAnimation[nCntBullet].nLengthX, g_aAnimation[nCntBullet].pos.y - g_aAnimation[nCntBullet].nLengthY, 0.0f);
				pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(g_aAnimation[nCntBullet].pos.x - g_aAnimation[nCntBullet].nLengthX, g_aAnimation[nCntBullet].pos.y + g_aAnimation[nCntBullet].nLengthY, 0.0f);
				pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(g_aAnimation[nCntBullet].pos.x + g_aAnimation[nCntBullet].nLengthX, g_aAnimation[nCntBullet].pos.y + g_aAnimation[nCntBullet].nLengthY, 0.0f);

			}
			pVtx[nCntBullet * 4].tex = D3DXVECTOR2((g_aAnimation[nCntBullet].nPtnAnim % g_aAnimation[nCntBullet].nPtnWide) * (1.0f / g_aAnimation[nCntBullet].nPtnWide), (g_aAnimation[nCntBullet].nPtnAnim / g_aAnimation[nCntBullet].nPtnWide) * (1.0f / g_aAnimation[nCntBullet].nPtnHeight));
			pVtx[nCntBullet * 4 + 1].tex = D3DXVECTOR2((g_aAnimation[nCntBullet].nPtnAnim % g_aAnimation[nCntBullet].nPtnWide) * (1.0f / g_aAnimation[nCntBullet].nPtnWide) + (1.0f / g_aAnimation[nCntBullet].nPtnWide), (g_aAnimation[nCntBullet].nPtnAnim / g_aAnimation[nCntBullet].nPtnWide) * (1.0f / g_aAnimation[nCntBullet].nPtnHeight));
			pVtx[nCntBullet * 4 + 2].tex = D3DXVECTOR2((g_aAnimation[nCntBullet].nPtnAnim % g_aAnimation[nCntBullet].nPtnWide) * (1.0f / g_aAnimation[nCntBullet].nPtnWide), (g_aAnimation[nCntBullet].nPtnAnim / g_aAnimation[nCntBullet].nPtnWide) * (1.0f / g_aAnimation[nCntBullet].nPtnHeight) + (1.0f / g_aAnimation[nCntBullet].nPtnHeight));
			pVtx[nCntBullet * 4 + 3].tex = D3DXVECTOR2((g_aAnimation[nCntBullet].nPtnAnim % g_aAnimation[nCntBullet].nPtnWide) * (1.0f / g_aAnimation[nCntBullet].nPtnWide) + (1.0f / g_aAnimation[nCntBullet].nPtnWide), (g_aAnimation[nCntBullet].nPtnAnim / g_aAnimation[nCntBullet].nPtnWide) * (1.0f / g_aAnimation[nCntBullet].nPtnHeight) + (1.0f / g_aAnimation[nCntBullet].nPtnHeight));
			if (g_aAnimation[nCntBullet].nCntAnim % g_aAnimation[nCntBullet].nPtnSpeed == 0)
			{
				g_aAnimation[nCntBullet].nPtnAnim++;
				if (g_aAnimation[nCntBullet].nPtnAnim > g_aAnimation[nCntBullet].nPtnHeight * g_aAnimation[nCntBullet].nPtnWide && g_aAnimation[nCntBullet].anim != ANIMATION_LOOP)
				{//�S��1��\�������������
					g_aAnimation[nCntBullet].bUse = false;
				}
			}
		}
	}
	//���_BUFFER���A�����b�N����
	g_pVtxBuffAnimation->Unlock();

}
//=========================================================================================================//
// * �w�i�̕`�揈��4	�`�揈��
//=========================================================================================================//
void DrawAnimation(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffAnimation, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{
		if (g_aAnimation[nCntBullet].bUse == true)
		{
			// �e�N�X�`���̐ݒ� �|���S���̕`��
			pDevice->SetTexture(0, g_pTextureAnimation[g_aAnimation[nCntBullet].nType]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, EXPLOSION_PRIMITIVE_NUM);
		}
	}

}
//=============================================================================
// �e�̐ݒ菈��
//=============================================================================
void SetAnimation(D3DXVECTOR3 pos, COLLAR col , int nLengthX, int nLengthY, int nPtnWide, int nPtnHeight, int nPtnSpeed, int nType, ANIMATION anim)
{
	int nCntBullet;
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffAnimation->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{
		if (g_aAnimation[nCntBullet].bUse == false)
		{
			g_aAnimation[nCntBullet].pos = pos;
			g_aAnimation[nCntBullet].nCntAnim = -1;
			g_aAnimation[nCntBullet].nPtnAnim = -1;
			g_aAnimation[nCntBullet].nPtnHeight = nPtnHeight;
			g_aAnimation[nCntBullet].nPtnWide = nPtnWide;
			g_aAnimation[nCntBullet].nPtnSpeed = nPtnSpeed;
			g_aAnimation[nCntBullet].nLengthX = nLengthX;
			g_aAnimation[nCntBullet].nLengthY = nLengthY;
			g_aAnimation[nCntBullet].nType = nType;
			g_aAnimation[nCntBullet].anim = anim;
			g_aAnimation[nCntBullet].bUse = true;

			pVtx[nCntBullet * 4].pos = D3DXVECTOR3(g_aAnimation[nCntBullet].pos.x - g_aAnimation[nCntBullet].nLengthX, g_aAnimation[nCntBullet].pos.y - g_aAnimation[nCntBullet].nLengthY, 0.0f);
			pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(g_aAnimation[nCntBullet].pos.x + g_aAnimation[nCntBullet].nLengthX, g_aAnimation[nCntBullet].pos.y - g_aAnimation[nCntBullet].nLengthY, 0.0f);
			pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(g_aAnimation[nCntBullet].pos.x - g_aAnimation[nCntBullet].nLengthX, g_aAnimation[nCntBullet].pos.y + g_aAnimation[nCntBullet].nLengthY, 0.0f);
			pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(g_aAnimation[nCntBullet].pos.x + g_aAnimation[nCntBullet].nLengthX, g_aAnimation[nCntBullet].pos.y + g_aAnimation[nCntBullet].nLengthY, 0.0f);

			pVtx[nCntBullet * 4].col = D3DCOLOR_RGBA(col.colR, col.colG, col.colB, 200);	// A = �����x
			pVtx[nCntBullet * 4 + 1].col = D3DCOLOR_RGBA(col.colR, col.colG, col.colB, 200);
			pVtx[nCntBullet * 4 + 2].col = D3DCOLOR_RGBA(col.colR, col.colG, col.colB, 200);
			pVtx[nCntBullet * 4 + 3].col = D3DCOLOR_RGBA(col.colR, col.colG, col.colB, 200);
			break;
		}
	}
	//���_BUFFER���A�����b�N����
	g_pVtxBuffAnimation->Unlock();
}
//=========================================================================================================//
// * �SAnimation�j������
//=========================================================================================================//
void BreakAnimation(void)
{
	int nCntAnim;

	for (nCntAnim = 0; nCntAnim < MAX_EXPLOSION; nCntAnim++)
	{
		g_aAnimation[nCntAnim].bUse = false;
	}
}
