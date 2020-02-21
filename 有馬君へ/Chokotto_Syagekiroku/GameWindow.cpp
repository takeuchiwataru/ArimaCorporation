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
#define WINDOW_MAX_TEXTURE			(5)
#define WINDOW_TextureName0		"data/TEXTURE/taitol001.jpg"		//GAME �X�R�A��
#define WINDOW_TextureName1		"data/TEXTURE/TUTORIAL000.jpg"		//�`���[�g���A��
#define WINDOW_TextureName2		"data/TEXTURE/difficult000.png"		//Result�@��Փx���S
#define WINDOW_TextureName3		"data/TEXTURE/RankingWindow000.png"	//Result�@SCORE�g
#define WINDOW_TextureName4		"data/TEXTURE/RankingNumber000.png"	//Result�@����
#define	MAX_WINDOW			(16)		// window�̍ő吔
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
	WINDOWSTATE state;
	WINDOWUSE use;
	int nCounterState[2];	//�G�l�~�[�̏�ԊǗ��p
	bool bUse;
}Window;

//*********************************************************************************************************//
// �O���[�o���ϐ�
//*********************************************************************************************************//
LPDIRECT3DTEXTURE9		g_pTextureGameWindow[WINDOW_MAX_TEXTURE] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameWindow = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
Window					g_aGameWindow[MAX_WINDOW];
int						g_nRank;

//=========================================================================================================//
// * �w�i�̕`�揈��1	����������
//=========================================================================================================//
void InitGameWindow(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	int nCntWindow;
	g_nRank = 0;

	// �e�̏��̏�����
	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		//g_aWindow[nCntWindow].pos = { 0.0f, 0.0f, 0.0f };
		//g_aWindow[nCntWindow].nLengthX = 0;
		//g_aWindow[nCntWindow].nLengthY = 0;
		//g_aWindow[nCntWindow].nType = 0;
		//g_aWindow[nCntWindow].col = { 1.0f, 1.0f, 1.0f, };
		g_aGameWindow[nCntWindow].bUse = false;
	}

	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName0, &g_pTextureGameWindow[0]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName1, &g_pTextureGameWindow[1]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName2, &g_pTextureGameWindow[2]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName3, &g_pTextureGameWindow[3]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName4, &g_pTextureGameWindow[4]);

	//���_BUFFER�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * WINDOW_VERTEX_NUM * MAX_WINDOW,		//�m�ۂ���BUFFER�̃T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffGameWindow,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
	//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGameWindow->Lock(0, 0, (void**)&pVtx, 0);
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
	g_pVtxBuffGameWindow->Unlock();
}
//=========================================================================================================//
// * �w�i�̕`�揈��2	�I������
//=========================================================================================================//
void UninitGameWindow(void)
{//�e�N�X�`���̔j��
	int nCntTexture;

	for (nCntTexture = 0; nCntTexture < WINDOW_MAX_TEXTURE; nCntTexture++)
	{
		if (g_pTextureGameWindow[nCntTexture] != NULL)
		{
			g_pTextureGameWindow[nCntTexture]->Release();
			g_pTextureGameWindow[nCntTexture] = NULL;
		}
	}
	if (g_pVtxBuffGameWindow != NULL)
	{
		g_pVtxBuffGameWindow->Release();
		g_pVtxBuffGameWindow = NULL;
	}
}
//=========================================================================================================//
// * �w�i�̕`�揈��3	�X�V����
//=========================================================================================================//
void UpdateGameWindow(void)
{
	int nCntWindow;

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
	//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGameWindow->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{//�S�E�B���h�E�����
		if (g_aGameWindow[nCntWindow].bUse == true)
		{//�g���Ă�����
			switch (g_aGameWindow[nCntWindow].state)
			{
			case WINDOWSTATE_RANKSET:
				g_aGameWindow[nCntWindow].col.a += 0.029f;
				g_aGameWindow[nCntWindow].pos.x -= 5.0f;
				if (g_aGameWindow[nCntWindow].col.a >= 1.0f)
				{
					g_aGameWindow[nCntWindow].col.a = 1.0f;
					g_aGameWindow[nCntWindow].state = WINDOWSTATE_NORMAL;
				}
				break;
			case WINDOWSTATE_RANKDELETE:
				g_aGameWindow[nCntWindow].col.a -= 0.029f;
				g_aGameWindow[nCntWindow].pos.x -= 5.0f;
				if (g_aGameWindow[nCntWindow].col.a <= 0.0f)
				{
					g_aGameWindow[nCntWindow].col.a = 0.0f;
					g_aGameWindow[nCntWindow].bUse = false;
				}
				break;
			}

			//���_���W�̐ݒ�
			pVtx[nCntWindow * 4].pos = D3DXVECTOR3(g_aGameWindow[nCntWindow].pos.x - g_aGameWindow[nCntWindow].nLengthX, g_aGameWindow[nCntWindow].pos.y - g_aGameWindow[nCntWindow].nLengthY, 0.0f);
			pVtx[nCntWindow * 4 + 1].pos = D3DXVECTOR3(g_aGameWindow[nCntWindow].pos.x + g_aGameWindow[nCntWindow].nLengthX, g_aGameWindow[nCntWindow].pos.y - g_aGameWindow[nCntWindow].nLengthY, 0.0f);
			pVtx[nCntWindow * 4 + 2].pos = D3DXVECTOR3(g_aGameWindow[nCntWindow].pos.x - g_aGameWindow[nCntWindow].nLengthX, g_aGameWindow[nCntWindow].pos.y + g_aGameWindow[nCntWindow].nLengthY, 0.0f);
			pVtx[nCntWindow * 4 + 3].pos = D3DXVECTOR3(g_aGameWindow[nCntWindow].pos.x + g_aGameWindow[nCntWindow].nLengthX, g_aGameWindow[nCntWindow].pos.y + g_aGameWindow[nCntWindow].nLengthY, 0.0f);
			//�J���[�ݒ�
			pVtx[nCntWindow * 4].col = g_aGameWindow[nCntWindow].col;	// A = �����x
			pVtx[nCntWindow * 4 + 1].col = g_aGameWindow[nCntWindow].col;
			pVtx[nCntWindow * 4 + 2].col = g_aGameWindow[nCntWindow].col;
			pVtx[nCntWindow * 4 + 3].col = g_aGameWindow[nCntWindow].col;
		}
	}
	//���_BUFFER���A�����b�N����
	g_pVtxBuffGameWindow->Unlock();
}
//=========================================================================================================//
// * �w�i�̕`�揈��4	�`�揈��
//=========================================================================================================//
void DrawGameWindow(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntWindow;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffGameWindow, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_NUM);
	// �|���S���̕`��
	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aGameWindow[nCntWindow].bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureGameWindow[g_aGameWindow[nCntWindow].nType]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWindow * 4, WINDOW_PRIMITIVE_NUM);
		}
	}

}
//=============================================================================
// �e�̐ݒ菈��
//=============================================================================
void SetGameWindow(D3DXVECTOR3 pos, D3DXCOLOR col, int nLengthX, int nLengthY, int nType, WINDOWSTATE state, WINDOWUSE use)
{
	int nCntWindow;
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

							//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGameWindow->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aGameWindow[nCntWindow].bUse == false)
		{
			g_aGameWindow[nCntWindow].pos = pos;
			g_aGameWindow[nCntWindow].nLengthX = nLengthX;
			g_aGameWindow[nCntWindow].nLengthY = nLengthY;
			g_aGameWindow[nCntWindow].nType = nType;
			g_aGameWindow[nCntWindow].state = state;
			g_aGameWindow[nCntWindow].use = use;
			g_aGameWindow[nCntWindow].nCounterState[0] = 0;
			g_aGameWindow[nCntWindow].nCounterState[1] = 1;
			g_aGameWindow[nCntWindow].bUse = true;

			pVtx[nCntWindow * 4].pos = D3DXVECTOR3(g_aGameWindow[nCntWindow].pos.x - g_aGameWindow[nCntWindow].nLengthX, g_aGameWindow[nCntWindow].pos.y - g_aGameWindow[nCntWindow].nLengthY, 0.0f);
			pVtx[nCntWindow * 4 + 1].pos = D3DXVECTOR3(g_aGameWindow[nCntWindow].pos.x + g_aGameWindow[nCntWindow].nLengthX, g_aGameWindow[nCntWindow].pos.y - g_aGameWindow[nCntWindow].nLengthY, 0.0f);
			pVtx[nCntWindow * 4 + 2].pos = D3DXVECTOR3(g_aGameWindow[nCntWindow].pos.x - g_aGameWindow[nCntWindow].nLengthX, g_aGameWindow[nCntWindow].pos.y + g_aGameWindow[nCntWindow].nLengthY, 0.0f);
			pVtx[nCntWindow * 4 + 3].pos = D3DXVECTOR3(g_aGameWindow[nCntWindow].pos.x + g_aGameWindow[nCntWindow].nLengthX, g_aGameWindow[nCntWindow].pos.y + g_aGameWindow[nCntWindow].nLengthY, 0.0f);

			g_aGameWindow[nCntWindow].col = col;
			pVtx[nCntWindow * 4].col = g_aGameWindow[nCntWindow].col;// A = �����x
			pVtx[nCntWindow * 4 + 1].col = g_aGameWindow[nCntWindow].col;
			pVtx[nCntWindow * 4 + 2].col = g_aGameWindow[nCntWindow].col;
			pVtx[nCntWindow * 4 + 3].col = g_aGameWindow[nCntWindow].col;

			if (g_aGameWindow[nCntWindow].use == WINDOWUSE_RANKING)
			{//��Փx���S
				Difficulty *pDifficulty = GetDifficulty();

				//�e�N�X�`�����W�ݒ�
				pVtx[nCntWindow * 4].tex = D3DXVECTOR2(0.0f, pDifficulty[0].nNumber * 0.25f);
				pVtx[nCntWindow * 4 + 1].tex = D3DXVECTOR2(1.0f, pDifficulty[0].nNumber * 0.25f);
				pVtx[nCntWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, pDifficulty[0].nNumber * 0.25f + 0.25f);
				pVtx[nCntWindow * 4 + 3].tex = D3DXVECTOR2(1.0f, pDifficulty[0].nNumber * 0.25f + 0.25f);
			}
			else if (g_aGameWindow[nCntWindow].use == WINDOWUSE_RANKNUMBER)
			{//����
			 //�e�N�X�`�����W�ݒ�
				pVtx[nCntWindow * 4].tex = D3DXVECTOR2(0.0f, g_nRank * 0.2f);
				pVtx[nCntWindow * 4 + 1].tex = D3DXVECTOR2(1.0f, g_nRank * 0.2f);
				pVtx[nCntWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, g_nRank * 0.2f + 0.2f);
				pVtx[nCntWindow * 4 + 3].tex = D3DXVECTOR2(1.0f, g_nRank * 0.2f + 0.2f);
				g_nRank = (g_nRank + 1) % 5;
			}
			break;
		}
	}
	//���_BUFFER���A�����b�N����
	g_pVtxBuffGameWindow->Unlock();
}
//=============================================================================
// window�j�󏈗�
//=============================================================================
void BreakGameWindow(void)
{//�S�E�B���h�E�j��
	int nCntWindow;

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aGameWindow[nCntWindow].bUse == true)
		{
			g_aGameWindow[nCntWindow].bUse = false;
		}
	}
}
//=============================================================================
// �w�肵��window���j��
//=============================================================================
void BackGameWindow(int nNumber)
{
	int nCntWindow;			//window�𐔂���
	int nBreak = nNumber;	//nNumber����������ǂ��Ȃ邩�킩���...

	for (nCntWindow = MAX_WINDOW; nCntWindow > 0; nCntWindow--)
	{//�ő�ł��S���I�������I��
		if (g_aGameWindow[nCntWindow - 1].bUse == true)
		{//��납�猩��true�ɂȂ��Ƃ����E��
			g_aGameWindow[nCntWindow - 1].bUse = false;
			nBreak--;
			if (nBreak < 1)
			{//0�ȉ��ɂȂ�����I���
				break;
			}
		}
	}
}
//=============================================================================
// �Swindow��ԕύX����
//=============================================================================
void BreakGameWindow(WINDOWSTATE state)
{//�S�E�B���h�E�j��
	int nCntWindow;

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aGameWindow[nCntWindow].bUse == true)
		{
			g_aGameWindow[nCntWindow].state = state ;
		}
	}
}
//=============================================================================
// �w�肵��window�̕ύX
//=============================================================================
void ChangeGameWindow(int nNumber, WINDOWSTATE state)
{
	g_aGameWindow[nNumber].state = state;
}