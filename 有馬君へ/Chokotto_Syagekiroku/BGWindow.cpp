//=========================================================================================================//
//
// window���� [window.cpp]
// Author : Ryou Sugimoto
//
//=========================================================================================================//
#include "main.h"
#include "game.h"

//*********************************************************************************************************//
// �}�N����`
//*********************************************************************************************************//
#define WINDOW_VERTEX_NUM			(4)								// ���_�̐�
#define WINDOW_PRIMITIVE_NUM		(2)								// �v���~�e�B�u�̐�
#define WINDOW_MAX_TEXTURE			(5)								//TEXTURE�̐�
#define WINDOW_TextureName0		"data/TEXTURE/20.jpg"			//�^�C�g��
#define WINDOW_TextureName1		"data/TEXTURE/BG001.jpg"			//�Q�[�����
#define WINDOW_TextureName2		"data/TEXTURE/Tutorial001.jpg"		//Tutorial
#define WINDOW_TextureName3		"data/TEXTURE/pipo-fog003.png"		//Smoke0
#define WINDOW_TextureName4		"data/TEXTURE/pipo-fog002.png"		//Smoke1
#define	MAX_WINDOW			(4)		// window�̍ő吔
#define HALFEXPLOSION		(30)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	COLLAR col;			//�F
	int nLengthX;		//����X
	int nLengthY;		//����Y
	int nType;			//�摜
	WINDOWSTATE state;	//�g�p���
	float fSpeed;		//SCROLLSPEED
	float fPos[3];		//0�F���ݒl, 1�F�c�����p, 2�F�����ݒl
	bool bUse;
}BGWindow;

//*********************************************************************************************************//
// �O���[�o���ϐ�
//*********************************************************************************************************//
LPDIRECT3DTEXTURE9		g_pTextureBGWindow[WINDOW_MAX_TEXTURE] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBGWindow = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
BGWindow g_aBGWindow[MAX_WINDOW];

//=========================================================================================================//
// * �w�i�̕`�揈��1	����������
//=========================================================================================================//
void InitBGWindow(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	int nCntWindow;

	// �e�̏��̏�����
	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		//g_aWindow[nCntWindow].pos = { 0.0f, 0.0f, 0.0f };
		//g_aWindow[nCntWindow].nLengthX = 0;
		//g_aWindow[nCntWindow].nLengthY = 0;
		//g_aWindow[nCntWindow].nType = 0;
		//g_aWindow[nCntWindow].col = { 1.0f, 1.0f, 1.0f, };
		g_aBGWindow[nCntWindow].bUse = false;
	}

	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName0, &g_pTextureBGWindow[0]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName1, &g_pTextureBGWindow[1]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName2, &g_pTextureBGWindow[2]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName3, &g_pTextureBGWindow[3]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName4, &g_pTextureBGWindow[4]);

	//���_BUFFER�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * WINDOW_VERTEX_NUM * MAX_WINDOW,		//�m�ۂ���BUFFER�̃T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffBGWindow,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
	//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBGWindow->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		//���_���W�̐ݒ�
		pVtx[nCntWindow * 4].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[nCntWindow * 4 + 1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[nCntWindow * 4 + 2].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[nCntWindow * 4 + 3].pos = D3DXVECTOR3(0, 0, 0.0f);

		pVtx[nCntWindow * 4].rhw = 1.0f;
		pVtx[nCntWindow * 4 + 1].rhw = 1.0f;
		pVtx[nCntWindow * 4 + 2].rhw = 1.0f;
		pVtx[nCntWindow * 4 + 3].rhw = 1.0f;

		//�J���[�ݒ�
		pVtx[nCntWindow * 4].col = D3DCOLOR_RGBA(0, 0, 0, 255);	// A = �����x
		pVtx[nCntWindow * 4 + 1].col = D3DCOLOR_RGBA(0, 0, 0, 255);
		pVtx[nCntWindow * 4 + 2].col = D3DCOLOR_RGBA(0, 0, 0, 255);
		pVtx[nCntWindow * 4 + 3].col = D3DCOLOR_RGBA(0, 0, 0, 255);

		//�e�N�X�`�����W�ݒ�
		pVtx[nCntWindow * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nCntWindow * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[nCntWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[nCntWindow * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//���_BUFFER���A�����b�N����
	g_pVtxBuffBGWindow->Unlock();
}
//=========================================================================================================//
// * �w�i�̕`�揈��2	�I������
//=========================================================================================================//
void UninitBGWindow(void)
{//�e�N�X�`���̔j��
	int nCntTexture;

	for (nCntTexture = 0; nCntTexture < WINDOW_MAX_TEXTURE; nCntTexture++)
	{
		if (g_pTextureBGWindow[nCntTexture] != NULL)
		{
			g_pTextureBGWindow[nCntTexture]->Release();
			g_pTextureBGWindow[nCntTexture] = NULL;
		}
	}
	if (g_pVtxBuffBGWindow != NULL)
	{
		g_pVtxBuffBGWindow->Release();
		g_pVtxBuffBGWindow = NULL;
	}
}
//=========================================================================================================//
// * �w�i�̕`�揈��3	�X�V����
//=========================================================================================================//
void UpdateBGWindow(void)
{
	int nCntWindow;

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
	//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBGWindow->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aBGWindow[nCntWindow].bUse == true)
		{//�g���Ă���window�Ȃ�

			switch (g_aBGWindow[nCntWindow].state)
			{
			case WINDOWSTATE_SCROLL0: //�c��SCROLL
				g_aBGWindow[nCntWindow].fPos[1] -= g_aBGWindow[nCntWindow].fSpeed;
				break;

			case WINDOWSTATE_SCROLL1:	//�΂�SCROLL
				g_aBGWindow[nCntWindow].fPos[1] -= g_aBGWindow[nCntWindow].fSpeed;	//����
				g_aBGWindow[nCntWindow].fPos[2] += g_aBGWindow[nCntWindow].fSpeed;	//����
				break;

			default:
				break;
			}

			pVtx[nCntWindow * 4].tex = D3DXVECTOR2(g_aBGWindow[nCntWindow].fPos[2] - 1.0f, g_aBGWindow[nCntWindow].fPos[1] - g_aBGWindow[nCntWindow].fPos[0]);
			pVtx[nCntWindow * 4 + 1].tex = D3DXVECTOR2(g_aBGWindow[nCntWindow].fPos[2], g_aBGWindow[nCntWindow].fPos[1] - g_aBGWindow[nCntWindow].fPos[0]);
			pVtx[nCntWindow * 4 + 2].tex = D3DXVECTOR2(g_aBGWindow[nCntWindow].fPos[2] - 1.0f, g_aBGWindow[nCntWindow].fPos[1]);
			pVtx[nCntWindow * 4 + 3].tex = D3DXVECTOR2(g_aBGWindow[nCntWindow].fPos[2], g_aBGWindow[nCntWindow].fPos[1]);

		}
	}

	//���_BUFFER���A�����b�N����
	g_pVtxBuffBGWindow->Unlock();
}
//=========================================================================================================//
// * �w�i�̕`�揈��4	�`�揈��
//=========================================================================================================//
void DrawBGWindow(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntWindow;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffBGWindow, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_NUM);
	// �|���S���̕`��
	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aBGWindow[nCntWindow].bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBGWindow[g_aBGWindow[nCntWindow].nType]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWindow * 4, WINDOW_PRIMITIVE_NUM);
		}
	}

}
//=============================================================================
// �e�̐ݒ菈��
//=============================================================================
void SetBGWindow(D3DXVECTOR3 pos, COLLAR col,float fSpeed, float fPos, int nLengthX, int nLengthY, int nType, WINDOWSTATE state)
{
	int nCntWindow;
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

							//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBGWindow->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aBGWindow[nCntWindow].bUse == false)
		{
			g_aBGWindow[nCntWindow].pos = pos;
			g_aBGWindow[nCntWindow].nLengthX = nLengthX;
			g_aBGWindow[nCntWindow].nLengthY = nLengthY;
			g_aBGWindow[nCntWindow].nType = nType;
			g_aBGWindow[nCntWindow].state = state;
			g_aBGWindow[nCntWindow].fSpeed = fSpeed;
			g_aBGWindow[nCntWindow].fPos[0] = fPos;
			g_aBGWindow[nCntWindow].fPos[1] = 1.0f;
			g_aBGWindow[nCntWindow].fPos[2] = 1.0f;
			g_aBGWindow[nCntWindow].bUse = true;

			pVtx[nCntWindow * 4].pos = D3DXVECTOR3(g_aBGWindow[nCntWindow].pos.x - g_aBGWindow[nCntWindow].nLengthX, g_aBGWindow[nCntWindow].pos.y - g_aBGWindow[nCntWindow].nLengthY, 0.0f);
			pVtx[nCntWindow * 4 + 1].pos = D3DXVECTOR3(g_aBGWindow[nCntWindow].pos.x + g_aBGWindow[nCntWindow].nLengthX, g_aBGWindow[nCntWindow].pos.y - g_aBGWindow[nCntWindow].nLengthY, 0.0f);
			pVtx[nCntWindow * 4 + 2].pos = D3DXVECTOR3(g_aBGWindow[nCntWindow].pos.x - g_aBGWindow[nCntWindow].nLengthX, g_aBGWindow[nCntWindow].pos.y + g_aBGWindow[nCntWindow].nLengthY, 0.0f);
			pVtx[nCntWindow * 4 + 3].pos = D3DXVECTOR3(g_aBGWindow[nCntWindow].pos.x + g_aBGWindow[nCntWindow].nLengthX, g_aBGWindow[nCntWindow].pos.y + g_aBGWindow[nCntWindow].nLengthY, 0.0f);

			pVtx[nCntWindow * 4].tex = D3DXVECTOR2(0.0f, g_aBGWindow[nCntWindow].fPos[1] - g_aBGWindow[nCntWindow].fPos[0]);
			pVtx[nCntWindow * 4 + 1].tex = D3DXVECTOR2(1.0f, g_aBGWindow[nCntWindow].fPos[1] - g_aBGWindow[nCntWindow].fPos[0]);
			pVtx[nCntWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, g_aBGWindow[nCntWindow].fPos[1]);
			pVtx[nCntWindow * 4 + 3].tex = D3DXVECTOR2(1.0f, g_aBGWindow[nCntWindow].fPos[1]);

			g_aBGWindow[nCntWindow].col.colA = col.colA;
			g_aBGWindow[nCntWindow].col.colB = col.colB;
			g_aBGWindow[nCntWindow].col.colG = col.colG;
			g_aBGWindow[nCntWindow].col.colR = col.colR;

			pVtx[nCntWindow * 4].col = D3DCOLOR_RGBA(col.colR, col.colG, col.colB, col.colA);	// A = �����x
			pVtx[nCntWindow * 4 + 1].col = D3DCOLOR_RGBA(col.colR, col.colG, col.colB, col.colA);
			pVtx[nCntWindow * 4 + 2].col = D3DCOLOR_RGBA(col.colR, col.colG, col.colB, col.colA);
			pVtx[nCntWindow * 4 + 3].col = D3DCOLOR_RGBA(col.colR, col.colG, col.colB, col.colA);
			break;
		}
	}
	//���_BUFFER���A�����b�N����
	g_pVtxBuffBGWindow->Unlock();
}
//=============================================================================
// �e�̐ݒ菈��
//=============================================================================
void BreakBGWindow(void)
{//�S�E�B���h�E�j��
	int nCntWindow;

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aBGWindow[nCntWindow].bUse == true)
		{
			g_aBGWindow[nCntWindow].bUse = false;
		}
	}
}
