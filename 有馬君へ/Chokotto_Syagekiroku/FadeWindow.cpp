//=========================================================================================================//
//
// window���� [window.cpp]
// Author : Ryou Sugimoto
//
//=========================================================================================================//
#include "main.h"
#include "game.h"
#include "input.h"

//*********************************************************************************************************//
// �}�N����`
//*********************************************************************************************************//
#define WINDOW_VERTEX_NUM			(4)					// ���_�̐�
#define WINDOW_PRIMITIVE_NUM		(2)					// �v���~�e�B�u�̐�
#define WINDOW_MAX_TEXTURE			(2)
#define WINDOW_TextureName0		"data/TEXTURE/press_enter.png"		//�^�C�g�� �G���^�[�L�[
#define WINDOW_TextureName1		"data/TEXTURE/nowloading.png"			//NOW LOADING
#define	MAX_WINDOW			(24)		// window�̍ő吔
#define HALFEXPLOSION		(30)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXCOLOR col;
	int nLengthX;
	int nLengthY;
	int nType;
	float fSteep;			//�p�x
	WINDOWSTATE state;
	WINDOWUSE use;
	int nCounterState[2];	//�G�l�~�[�̏�ԊǗ��p
	bool bUse;
}Window;

//*********************************************************************************************************//
// �O���[�o���ϐ�
//*********************************************************************************************************//
LPDIRECT3DTEXTURE9		g_pTextureFadeWindow[WINDOW_MAX_TEXTURE] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFadeWindow = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
Window g_aFadeWindow[MAX_WINDOW];

//=========================================================================================================//
// * �w�i�̕`�揈��1	����������
//=========================================================================================================//
void InitFadeWindow(void)
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
		g_aFadeWindow[nCntWindow].bUse = false;
	}

	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName0, &g_pTextureFadeWindow[0]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName1, &g_pTextureFadeWindow[1]);

	//���_BUFFER�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * WINDOW_VERTEX_NUM * MAX_WINDOW,		//�m�ۂ���BUFFER�̃T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffFadeWindow,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
							//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffFadeWindow->Lock(0, 0, (void**)&pVtx, 0);
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
	g_pVtxBuffFadeWindow->Unlock();
}
//=========================================================================================================//
// * �w�i�̕`�揈��2	�I������
//=========================================================================================================//
void UninitFadeWindow(void)
{//�e�N�X�`���̔j��
	int nCntTexture;

	for (nCntTexture = 0; nCntTexture < WINDOW_MAX_TEXTURE; nCntTexture++)
	{
		if (g_pTextureFadeWindow[nCntTexture] != NULL)
		{
			g_pTextureFadeWindow[nCntTexture]->Release();
			g_pTextureFadeWindow[nCntTexture] = NULL;
		}
	}
	if (g_pVtxBuffFadeWindow != NULL)
	{
		g_pVtxBuffFadeWindow->Release();
		g_pVtxBuffFadeWindow = NULL;
	}
}
//=========================================================================================================//
// * �w�i�̕`�揈��3	�X�V����
//=========================================================================================================//
void UpdateFadeWindow(void)
{
	int nCntWindow;
	static float fStealth = 0.02f;
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
	PLAYER *pPlayer;

	pPlayer = GetPlayer();

	//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffFadeWindow->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aFadeWindow[nCntWindow].bUse == true && g_aFadeWindow[nCntWindow].state != WINDOWSTATE_NORMAL)
		{
			switch (g_aFadeWindow[nCntWindow].state)
			{
			case WINDOWSTATE_FADEIN:	//Stealth�̑�����Ȃ�
				g_aFadeWindow[nCntWindow].col.a += 0.3f;
				if (g_aFadeWindow[nCntWindow].col.a >= 1.0f)
				{
					g_aFadeWindow[nCntWindow].col.a = 1.0f;
					g_aFadeWindow[nCntWindow].state = WINDOWSTATE_NORMAL;
				}
				break;
			case WINDOWSTATE_FADEOUT://�ݒ肳�ꂽ�����x����Â��Ȃ�0�ŏ�����
				g_aFadeWindow[nCntWindow].col.a -= 0.01f;
				if (g_aFadeWindow[nCntWindow].col.a <= 0.0f)
				{
					g_aFadeWindow[nCntWindow].col.a = 0.0f;
					g_aFadeWindow[nCntWindow].bUse = false;
				}
				break;

			case WINDOWSTATE_FLASH://�_�łŕ\��
				g_aFadeWindow[nCntWindow].nCounterState[0]++;
				if (g_aFadeWindow[nCntWindow].nCounterState[1] % 2 == 0 && g_aFadeWindow[nCntWindow].nCounterState[0] % 45 == 0)
				{
					g_aFadeWindow[nCntWindow].col.a = 1.0f;
					g_aFadeWindow[nCntWindow].nCounterState[1]++;
				}
				else if (g_aFadeWindow[nCntWindow].nCounterState[0] % 45 == 0)
				{
					g_aFadeWindow[nCntWindow].col.a = 0.0f;
					g_aFadeWindow[nCntWindow].nCounterState[1]++;
				}
				break;
			}

			pVtx[nCntWindow * 4].pos = D3DXVECTOR3(g_aFadeWindow[nCntWindow].pos.x + (sinf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthY) + (cosf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthX), g_aFadeWindow[nCntWindow].pos.y - (sinf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthX) + (cosf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthY), 0.0f);
			pVtx[nCntWindow * 4 + 1].pos = D3DXVECTOR3(g_aFadeWindow[nCntWindow].pos.x + (sinf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthY) - (cosf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthX), g_aFadeWindow[nCntWindow].pos.y + (sinf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthX) + (cosf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthY), 0.0f);
			pVtx[nCntWindow * 4 + 2].pos = D3DXVECTOR3(g_aFadeWindow[nCntWindow].pos.x - (sinf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthY) + (cosf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthX), g_aFadeWindow[nCntWindow].pos.y - (sinf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthX) - (cosf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthY), 0.0f);
			pVtx[nCntWindow * 4 + 3].pos = D3DXVECTOR3(g_aFadeWindow[nCntWindow].pos.x - (sinf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthY) - (cosf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthX), g_aFadeWindow[nCntWindow].pos.y + (sinf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthX) - (cosf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthY), 0.0f);

			pVtx[nCntWindow * 4].col = g_aFadeWindow[nCntWindow].col;	// A = �����x
			pVtx[nCntWindow * 4 + 1].col = g_aFadeWindow[nCntWindow].col;
			pVtx[nCntWindow * 4 + 2].col = g_aFadeWindow[nCntWindow].col;
			pVtx[nCntWindow * 4 + 3].col = g_aFadeWindow[nCntWindow].col;
		}
	}
	//���_BUFFER���A�����b�N����
	g_pVtxBuffFadeWindow->Unlock();
}
//=========================================================================================================//
// * �w�i�̕`�揈��4	�`�揈��
//=========================================================================================================//
void DrawFadeWindow(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntWindow;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffFadeWindow, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_NUM);
	// �|���S���̕`��
	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aFadeWindow[nCntWindow].bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureFadeWindow[g_aFadeWindow[nCntWindow].nType]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWindow * 4, WINDOW_PRIMITIVE_NUM);
		}
	}

}
//=============================================================================
// �e�̐ݒ菈��
//=============================================================================
void SetFadeWindow(D3DXVECTOR3 pos, D3DXCOLOR col, int nLengthX, int nLengthY, int nType, int nCntUse, WINDOWSTATE state, WINDOWUSE use)
{
	int nCntWindow;
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

							//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffFadeWindow->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntWindow = 1; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (use == WINDOWUSE_GUARD)
		{
			nCntWindow = 0;
			if (g_aFadeWindow[nCntWindow].bUse == true)
			{
				break;
			}
		}
		if (g_aFadeWindow[nCntWindow].bUse == false)
		{
			g_aFadeWindow[nCntWindow].pos = pos;
			g_aFadeWindow[nCntWindow].nLengthX = nLengthX;
			g_aFadeWindow[nCntWindow].nLengthY = nLengthY;
			g_aFadeWindow[nCntWindow].nType = nType;
			g_aFadeWindow[nCntWindow].state = state;
			g_aFadeWindow[nCntWindow].use = use;
			g_aFadeWindow[nCntWindow].nCounterState[0] = nCntUse;
			g_aFadeWindow[nCntWindow].nCounterState[1] = 1;
			g_aFadeWindow[nCntWindow].bUse = true;

			g_aFadeWindow[nCntWindow].fSteep = D3DX_PI;
			/*pVtx[nCntWindow * 4].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x - g_aWindow[nCntWindow].nLengthX, g_aWindow[nCntWindow].pos.y - g_aWindow[nCntWindow].nLengthY, 0.0f);
			pVtx[nCntWindow * 4 + 1].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x + g_aWindow[nCntWindow].nLengthX, g_aWindow[nCntWindow].pos.y - g_aWindow[nCntWindow].nLengthY, 0.0f);
			pVtx[nCntWindow * 4 + 2].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x - g_aWindow[nCntWindow].nLengthX, g_aWindow[nCntWindow].pos.y + g_aWindow[nCntWindow].nLengthY, 0.0f);
			pVtx[nCntWindow * 4 + 3].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x + g_aWindow[nCntWindow].nLengthX, g_aWindow[nCntWindow].pos.y + g_aWindow[nCntWindow].nLengthY, 0.0f);*/

			pVtx[nCntWindow * 4].pos = D3DXVECTOR3(g_aFadeWindow[nCntWindow].pos.x + (sinf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthY) + (cosf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthX), g_aFadeWindow[nCntWindow].pos.y - (sinf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthX) + (cosf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthY), 0.0f);
			pVtx[nCntWindow * 4 + 1].pos = D3DXVECTOR3(g_aFadeWindow[nCntWindow].pos.x + (sinf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthY) - (cosf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthX), g_aFadeWindow[nCntWindow].pos.y + (sinf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthX) + (cosf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthY), 0.0f);
			pVtx[nCntWindow * 4 + 2].pos = D3DXVECTOR3(g_aFadeWindow[nCntWindow].pos.x - (sinf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthY) + (cosf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthX), g_aFadeWindow[nCntWindow].pos.y - (sinf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthX) - (cosf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthY), 0.0f);
			pVtx[nCntWindow * 4 + 3].pos = D3DXVECTOR3(g_aFadeWindow[nCntWindow].pos.x - (sinf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthY) - (cosf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthX), g_aFadeWindow[nCntWindow].pos.y + (sinf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthX) - (cosf(g_aFadeWindow[nCntWindow].fSteep) * g_aFadeWindow[nCntWindow].nLengthY), 0.0f);

			g_aFadeWindow[nCntWindow].col = col;
			pVtx[nCntWindow * 4].col = col;	// A = �����x
			pVtx[nCntWindow * 4 + 1].col = col;
			pVtx[nCntWindow * 4 + 2].col = col;
			pVtx[nCntWindow * 4 + 3].col = col;
			break;
		}
	}
	//���_BUFFER���A�����b�N����
	g_pVtxBuffFadeWindow->Unlock();
}
//=============================================================================
// LOAD�|���S���j�󏈗�
//=============================================================================
void LoadBreakWindow(void)
{
	int nCntWindow;

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aFadeWindow[nCntWindow].use == WINDOWUSE_LOAD && g_aFadeWindow[nCntWindow].bUse == true)
		{
			g_aFadeWindow[nCntWindow].bUse = false;
			break;
		}
	}
}