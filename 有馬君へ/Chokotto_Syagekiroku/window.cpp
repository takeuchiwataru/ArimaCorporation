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
#define WINDOW_MAX_TEXTURE			(26)
#define WINDOW_TextureName0		"data/TEXTURE/press_enter.png"		//�^�C�g�� �G���^�[�L�[
#define WINDOW_TextureName1		"data/TEXTURE/now loading"			//NOW LOADING
#define WINDOW_TextureName2		"data/TEXTURE/SELECTWINDOW.png"		//Title ��Փx�I��
#define WINDOW_TextureName3		"data/TEXTURE/gameover.jpg"			//�Q�[���I�[�o�[0
#define WINDOW_TextureName4		"data/TEXTURE/gameover000.png"		//�Q�[���I�[�o�[1
#define WINDOW_TextureName5		"data/TEXTURE/gameclear_logo.png"	//�Q�[���N���A���S
#define WINDOW_TextureName6		"data/TEXTURE/GameClear000.jpg"		//�Q�[���N���ABG
#define WINDOW_TextureName7		"data/TEXTURE/Player002.png"		//�t�i�b�V�[�I�c�@
#define WINDOW_TextureName8		"data/TEXTURE/START.png"			//Title�@START���S
#define WINDOW_TextureName9		"data/TEXTURE/TUTORIAL001.png"		//Title�@TUTORIAL���S
#define WINDOW_TextureName10	"data/TEXTURE/END.png"				//Title�@�d�m�c���S
#define WINDOW_TextureName11	"data/TEXTURE/Easy.png"				//Title�@�d�`�r�x���S
#define WINDOW_TextureName12	"data/TEXTURE/Normal.png"			//Title�@NORMAL���S
#define WINDOW_TextureName13	"data/TEXTURE/Hard.png"				//Title�@�g�`�q�c���S
#define WINDOW_TextureName14	"data/TEXTURE/Lunatic.png"			//Title�@LUNATIC���S
#define WINDOW_TextureName15	"data/TEXTURE/pause100.png"			//PAUSE�g
#define WINDOW_TextureName16	"data/TEXTURE/pause000.png"			//PAUSE���S
#define WINDOW_TextureName17	"data/TEXTURE/pause001.png"			//PAUSE���S
#define WINDOW_TextureName18	"data/TEXTURE/pause002.png"			//PAUSE���S
#define WINDOW_TextureName19	"data/TEXTURE/Guard000.png"			//GUARD�@�g�p���
#define WINDOW_TextureName20	"data/TEXTURE/Guard001.png"			//GUARD�@�`���[�W�ς�
#define WINDOW_TextureName21	"data/TEXTURE/FontWindow.png"		//FONT�̂��߂̘g
#define WINDOW_TextureName22	"data/TEXTURE/BOSS003.png"			//Game�{�X
#define WINDOW_TextureName23	"data/TEXTURE/BOSS_Change004.png"	//Game�{�XMODEChange��
#define WINDOW_TextureName24	""									//�}�e���A���̐F
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
LPDIRECT3DTEXTURE9		g_pTextureWindow[WINDOW_MAX_TEXTURE] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWindow = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
Window g_aWindow[MAX_WINDOW];

//=========================================================================================================//
// * �w�i�̕`�揈��1	����������
//=========================================================================================================//
void InitWindow(void)
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
		g_aWindow[nCntWindow].bUse = false;
	}

	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName0, &g_pTextureWindow[0]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName1, &g_pTextureWindow[1]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName2, &g_pTextureWindow[2]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName3, &g_pTextureWindow[3]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName4, &g_pTextureWindow[4]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName5, &g_pTextureWindow[5]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName6, &g_pTextureWindow[6]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName7, &g_pTextureWindow[7]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName8, &g_pTextureWindow[8]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName9, &g_pTextureWindow[9]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName10, &g_pTextureWindow[10]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName11, &g_pTextureWindow[11]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName12, &g_pTextureWindow[12]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName13, &g_pTextureWindow[13]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName14, &g_pTextureWindow[14]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName15, &g_pTextureWindow[15]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName16, &g_pTextureWindow[16]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName17, &g_pTextureWindow[17]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName18, &g_pTextureWindow[18]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName19, &g_pTextureWindow[19]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName20, &g_pTextureWindow[20]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName21, &g_pTextureWindow[21]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName22, &g_pTextureWindow[22]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName23, &g_pTextureWindow[23]);
	D3DXCreateTextureFromFile(pDevice, WINDOW_TextureName24, &g_pTextureWindow[24]);

	//���_BUFFER�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * WINDOW_VERTEX_NUM * MAX_WINDOW,		//�m�ۂ���BUFFER�̃T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffWindow,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
							//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWindow->Lock(0, 0, (void**)&pVtx, 0);
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
	g_pVtxBuffWindow->Unlock();
}
//=========================================================================================================//
// * �w�i�̕`�揈��2	�I������
//=========================================================================================================//
void UninitWindow(void)
{//�e�N�X�`���̔j��
	int nCntTexture;

	for (nCntTexture = 0; nCntTexture < WINDOW_MAX_TEXTURE; nCntTexture++)
	{
		if (g_pTextureWindow[nCntTexture] != NULL)
		{
			g_pTextureWindow[nCntTexture]->Release();
			g_pTextureWindow[nCntTexture] = NULL;
		}
	}
	if (g_pVtxBuffWindow != NULL)
	{
		g_pVtxBuffWindow->Release();
		g_pVtxBuffWindow = NULL;
	}
}
//=========================================================================================================//
// * �w�i�̕`�揈��3	�X�V����
//=========================================================================================================//
void UpdateWindow(void)
{
	int nCntWindow;
	static float fStealth = 0.02f;
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
	PLAYER *pPlayer;

	pPlayer = GetPlayer();

							//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWindow->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aWindow[nCntWindow].bUse == true && g_aWindow[nCntWindow].state != WINDOWSTATE_NORMAL)
		{
			switch (g_aWindow[nCntWindow].state)
			{
			case WINDOWSTATE_STEALTH://�ݒ肳�ꂽ�����x���疾�邭�Ȃ�255�Œʏ�ɂȂ�
				g_aWindow[nCntWindow].col.a += 0.012f;
				if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_UP) == true || GetKeyboardTrigger(DIK_DOWN) == true || GetKeyboardTrigger(DIK_Z) == true || g_aWindow[nCntWindow].col.a >= 1.0f)
				{//�L�[���͂ŃX�L�b�v
					g_aWindow[nCntWindow].col.a = 1.0f;
					g_aWindow[nCntWindow].state = WINDOWSTATE_NORMAL;
				}
				break;
			case WINDOWSTATE_FADEIN:	//Stealth�̑�����Ȃ�
				g_aWindow[nCntWindow].col.a += 0.012f;
				if (g_aWindow[nCntWindow].col.a >= 1.0f)
				{
					g_aWindow[nCntWindow].col.a = 1.0f;
					g_aWindow[nCntWindow].state = WINDOWSTATE_NORMAL;
				}
				break;
			case WINDOWSTATE_FADEOUT://�ݒ肳�ꂽ�����x����Â��Ȃ�0�ŏ�����
				g_aWindow[nCntWindow].col.a -= 0.01f;
				if (g_aWindow[nCntWindow].col.a <= 0.0f)
				{
					g_aWindow[nCntWindow].col.a = 0.0f;
					g_aWindow[nCntWindow].bUse = false;
				}
				break;

			case WINDOWSTATE_FLASH://�_�łŕ\��
				g_aWindow[nCntWindow].nCounterState[0]++;
				if (g_aWindow[nCntWindow].nCounterState[1] % 2 == 0 && g_aWindow[nCntWindow].nCounterState[0] % 45 == 0)
				{
					g_aWindow[nCntWindow].col.a = 1.0f;
					g_aWindow[nCntWindow].nCounterState[1]++;
				}
				else if (g_aWindow[nCntWindow].nCounterState[0] % 45 == 0)
				{
					g_aWindow[nCntWindow].col.a = 0.0f;
					g_aWindow[nCntWindow].nCounterState[1]++;
				}
				break;

			case WINDOWSTATE_SELECTON://SELECT�����Ŏg�p����A�I������Ă���Ƃ�
				g_aWindow[nCntWindow].col.r += fStealth;
				g_aWindow[nCntWindow].col.g += fStealth;
				g_aWindow[nCntWindow].col.b += fStealth;
				if (g_aWindow[nCntWindow].col.r >= 1.0f || g_aWindow[nCntWindow].col.r <= 0.22f)
				{//���邳���ő�, �ŏ��ɂȂ����甽�]
					fStealth *= -1.0f;
				}
				break;

			case WINDOWSTATE_SELECTOFF://SELECT�����Ŏg�p����A�I������Ă��Ȃ��Ƃ�
				g_aWindow[nCntWindow].col.r = 0.23f;
				g_aWindow[nCntWindow].col.g = 0.23f;
				g_aWindow[nCntWindow].col.b = 0.23f;
				break;

			case WINDOWSTATE_GUARDON:	//GUARD�g�p���
				g_aWindow[nCntWindow].pos = D3DXVECTOR3(pPlayer[0].pos.x, pPlayer[0].pos.y + PLAYER_POSY3, 0.0f);
				g_aWindow[nCntWindow].fSteep -= 0.05f;
				if (g_aWindow[nCntWindow].nCounterState[0] != 0)
				{
					g_aWindow[nCntWindow].nCounterState[0]--;
					if (g_aWindow[nCntWindow].nCounterState[0] == 0)
					{
						g_aWindow[nCntWindow].col.r = 1.0f;
						g_aWindow[nCntWindow].col.g = 1.0f;
						g_aWindow[nCntWindow].col.b = 1.0f;
					}
				}
				break;

			case WINDOWSTATE_GUARDOFF:
				g_aWindow[nCntWindow].fSteep -= 0.02f;
				g_aWindow[nCntWindow].pos = D3DXVECTOR3(pPlayer[0].pos.x, pPlayer[0].pos.y + PLAYER_POSY3, 0.0f);
				if (g_aWindow[nCntWindow].col.a != 0.4f)
				{
					g_aWindow[nCntWindow].col.a += 0.02f;
					if (g_aWindow[nCntWindow].col.a > 0.399f)
					{
						g_aWindow[nCntWindow].col.a = 0.4f;
					}
				}
				break;
			case WINDOWSTATE_ENEMYCHANGE_FLOW:
				if (g_aWindow[nCntWindow].nCounterState[0] == 0)
				{//������E�ɗ���Ȃ���\��
					g_aWindow[nCntWindow].col.a += 0.05f;
					g_aWindow[nCntWindow].pos.x += 20.0f;
					if (g_aWindow[nCntWindow].col.a >= 1.0f)
					{
						g_aWindow[nCntWindow].col.a = 1.0f;
						g_aWindow[nCntWindow].nCounterState[0] = 1;
					}
				}
				else
				{//���Ԍo�߂�
					g_aWindow[nCntWindow].nCounterState[0]++;
					if (g_aWindow[nCntWindow].nCounterState[0] > 60)
					{//����Ɏ��Ԍo�߂�
						g_aWindow[nCntWindow].col.a -= 0.029f;
						g_aWindow[nCntWindow].pos.x += 8.0f;
						if (g_aWindow[nCntWindow].col.a <= 0.0f)
						{//�E�ɗ���Ȃ��������
							g_aWindow[nCntWindow].col.a = 0.0f;
							g_aWindow[nCntWindow].bUse = false;
						}
					}
				}
				break;
			case WINDOWSTATE_ENEMYCHANGE_ZOOM:
				if (g_aWindow[nCntWindow].nCounterState[0] == 0)
				{//������E�ɗ���Ȃ���\��
					g_aWindow[nCntWindow].col.a += 0.05f;
					g_aWindow[nCntWindow].pos.x += 20.0f;
					if (g_aWindow[nCntWindow].col.a >= 1.0f)
					{
						g_aWindow[nCntWindow].col.a = 1.0f;
						g_aWindow[nCntWindow].nCounterState[0] = 1;
					}
				}
				else
				{//���Ԍo�߂�
					g_aWindow[nCntWindow].nCounterState[0]++;
					if (g_aWindow[nCntWindow].nCounterState[0] > 60)
					{//����Ɏ��Ԍo�߂�
						g_aWindow[nCntWindow].col.a -= 0.029f;
						g_aWindow[nCntWindow].nLengthX += 3;
						g_aWindow[nCntWindow].nLengthY += 3;
						if (g_aWindow[nCntWindow].col.a <= 0.0f)
						{//�Y�[�����Ȃ��������
							g_aWindow[nCntWindow].col.a = 0.0f;
							g_aWindow[nCntWindow].bUse = false;
						}
					}
				}
				break;
			}

			pVtx[nCntWindow * 4].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x + (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthY) + (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthX), g_aWindow[nCntWindow].pos.y - (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthX) + (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthY), 0.0f);
			pVtx[nCntWindow * 4 + 1].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x + (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthY) - (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthX), g_aWindow[nCntWindow].pos.y + (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthX) + (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthY), 0.0f);
			pVtx[nCntWindow * 4 + 2].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x - (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthY) + (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthX), g_aWindow[nCntWindow].pos.y - (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthX) - (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthY), 0.0f);
			pVtx[nCntWindow * 4 + 3].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x - (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthY) - (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthX), g_aWindow[nCntWindow].pos.y + (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthX) - (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthY), 0.0f);

			pVtx[nCntWindow * 4].col = g_aWindow[nCntWindow].col;	// A = �����x
			pVtx[nCntWindow * 4 + 1].col = g_aWindow[nCntWindow].col;
			pVtx[nCntWindow * 4 + 2].col = g_aWindow[nCntWindow].col;
			pVtx[nCntWindow * 4 + 3].col = g_aWindow[nCntWindow].col;
		}
	}
	//���_BUFFER���A�����b�N����
	g_pVtxBuffWindow->Unlock();
}
//=========================================================================================================//
// * �w�i�̕`�揈��4	�`�揈��
//=========================================================================================================//
void DrawWindow(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntWindow;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffWindow, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_NUM);
	// �|���S���̕`��
	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aWindow[nCntWindow].bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureWindow[g_aWindow[nCntWindow].nType]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWindow * 4, WINDOW_PRIMITIVE_NUM);
		}
	}

}
//=============================================================================
// �e�̐ݒ菈��
//=============================================================================
void SetWindow(D3DXVECTOR3 pos, D3DXCOLOR col, int nLengthX, int nLengthY, int nType, int nCntUse, int nEnemy, WINDOWSTATE state, WINDOWUSE use)
{
	int nCntWindow;
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

							//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWindow->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntWindow = 1; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (use == WINDOWUSE_GUARD)
		{
			nCntWindow = 0;
			if (g_aWindow[nCntWindow].bUse == true)
			{
				break;
			}
		}
		if (g_aWindow[nCntWindow].bUse == false)
		{
			g_aWindow[nCntWindow].pos = pos;
			g_aWindow[nCntWindow].nLengthX = nLengthX;
			g_aWindow[nCntWindow].nLengthY = nLengthY;
			g_aWindow[nCntWindow].nType = nType;
			g_aWindow[nCntWindow].state = state;
			g_aWindow[nCntWindow].use = use;
			g_aWindow[nCntWindow].nCounterState[0] = nCntUse;
			g_aWindow[nCntWindow].nCounterState[1] = 1;
			g_aWindow[nCntWindow].bUse = true;

			g_aWindow[nCntWindow].fSteep = D3DX_PI;
			/*pVtx[nCntWindow * 4].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x - g_aWindow[nCntWindow].nLengthX, g_aWindow[nCntWindow].pos.y - g_aWindow[nCntWindow].nLengthY, 0.0f);
			pVtx[nCntWindow * 4 + 1].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x + g_aWindow[nCntWindow].nLengthX, g_aWindow[nCntWindow].pos.y - g_aWindow[nCntWindow].nLengthY, 0.0f);
			pVtx[nCntWindow * 4 + 2].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x - g_aWindow[nCntWindow].nLengthX, g_aWindow[nCntWindow].pos.y + g_aWindow[nCntWindow].nLengthY, 0.0f);
			pVtx[nCntWindow * 4 + 3].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x + g_aWindow[nCntWindow].nLengthX, g_aWindow[nCntWindow].pos.y + g_aWindow[nCntWindow].nLengthY, 0.0f);*/

			pVtx[nCntWindow * 4].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x + (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthY) + (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthX), g_aWindow[nCntWindow].pos.y - (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthX) + (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthY), 0.0f);
			pVtx[nCntWindow * 4 + 1].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x + (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthY) - (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthX), g_aWindow[nCntWindow].pos.y + (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthX) + (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthY), 0.0f);
			pVtx[nCntWindow * 4 + 2].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x - (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthY) + (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthX), g_aWindow[nCntWindow].pos.y - (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthX) - (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthY), 0.0f);
			pVtx[nCntWindow * 4 + 3].pos = D3DXVECTOR3(g_aWindow[nCntWindow].pos.x - (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthY) - (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthX), g_aWindow[nCntWindow].pos.y + (sinf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthX) - (cosf(g_aWindow[nCntWindow].fSteep) * g_aWindow[nCntWindow].nLengthY), 0.0f);

			g_aWindow[nCntWindow].col = col;
			pVtx[nCntWindow * 4].col = col;	// A = �����x
			pVtx[nCntWindow * 4 + 1].col = col;
			pVtx[nCntWindow * 4 + 2].col = col;
			pVtx[nCntWindow * 4 + 3].col = col;
			if (use == WINDOWUSE_ENEMY)
			{//�e�N�X�`�����W�ݒ�
				pVtx[nCntWindow * 4].tex = D3DXVECTOR2((nEnemy % 2) * 0.5f, (nEnemy / 2) * 0.2f);
				pVtx[nCntWindow * 4 + 1].tex = D3DXVECTOR2((nEnemy % 2) * 0.5f + 0.5f, (nEnemy / 2) * 0.2f);
				pVtx[nCntWindow * 4 + 2].tex = D3DXVECTOR2((nEnemy % 2) * 0.5f, (nEnemy / 2) * 0.2f + 0.2f);
				pVtx[nCntWindow * 4 + 3].tex = D3DXVECTOR2((nEnemy % 2) * 0.5f + 0.5f, (nEnemy / 2) * 0.2f + 0.2f);
			}
			else
			{//�e�N�X�`�����W�ݒ�
				pVtx[nCntWindow * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCntWindow * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[nCntWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[nCntWindow * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
			break;
		}
	}
	//���_BUFFER���A�����b�N����
	g_pVtxBuffWindow->Unlock();
}
//=============================================================================
// window�̔j��
//=============================================================================
void BreakWindow(void)
{//�S�E�B���h�E�j��
	int nCntWindow;

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aWindow[nCntWindow].bUse == true)
		{
			g_aWindow[nCntWindow].bUse = false;
		}
	}
}
//=============================================================================
// �c�@�j��
//=============================================================================
bool BreakAirport(void)
{//�c�@���c���Ă����true
	bool bRespawn = false;
	int nCntWindow;

	for (nCntWindow = MAX_WINDOW; nCntWindow > 0; nCntWindow--)
	{
		if (g_aWindow[nCntWindow - 1].bUse == true && g_aWindow[nCntWindow - 1].use == WINDOWUSE_RESIDUALAIRPORT && g_aWindow[nCntWindow - 1].state == WINDOWSTATE_NORMAL)
		{
			g_aWindow[nCntWindow - 1].state = WINDOWSTATE_FADEOUT;
			bRespawn = true;
			break;
		}
	}

	return bRespawn;
}
//=============================================================================
// �I������
//=============================================================================
void SelectWindow(int nNumber)
{
	int nCntWindow;

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aWindow[nCntWindow].use == WINDOWUSE_SELECT && g_aWindow[nCntWindow].bUse == true)
		{
			if (g_aWindow[nCntWindow].nCounterState[0] == nNumber)
			{
				g_aWindow[nCntWindow].state = WINDOWSTATE_SELECTON;
			}
			else
			{
				g_aWindow[nCntWindow].state = WINDOWSTATE_SELECTOFF;
			}
		}
	}
}
//=============================================================================
// �w�肵��window���j��
//=============================================================================
void BackWindow(int nNumber)
{
	int nCntWindow;			//window�𐔂���
	int nBreak = nNumber;	//nNumber����������ǂ��Ȃ邩�킩���...

	for (nCntWindow = MAX_WINDOW; nCntWindow > 0; nCntWindow--)
	{//�ő�ł��S���I�������I��
		if (g_aWindow[nCntWindow - 1].bUse == true)
		{//��납�猩��true�ɂȂ��Ƃ����E��
			g_aWindow[nCntWindow - 1].bUse = false;
			nBreak--;
			if (nBreak < 1)
			{//0�ȉ��ɂȂ�����I���
				break;
			}
		}
	}
}
//=============================================================================
// �w�肵��window���j��
//=============================================================================
void ChangeGuard(int nUse)
{
	int nCntWindow;			//window�𐔂���

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{//�ő�ł��S���I�������I��
		if (g_aWindow[nCntWindow].bUse == true && g_aWindow[nCntWindow].use == WINDOWUSE_GUARD)
		{//GUARD�̃|���S���̏�ԕύX
			if (nUse == 0)
			{//GUARD����
				g_aWindow[nCntWindow].col.r = 1.0f;
				g_aWindow[nCntWindow].col.g = 0.99f;
				g_aWindow[nCntWindow].col.b = 0.03f;

				g_aWindow[nCntWindow].nCounterState[0] = 5;
			}
			else if (nUse == 1)
			{
				g_aWindow[nCntWindow].state = WINDOWSTATE_GUARDON;
				g_aWindow[nCntWindow].nLengthX = 32;
				g_aWindow[nCntWindow].nLengthY = 32;
				g_aWindow[nCntWindow].col.a = 0.85f;
				g_aWindow[nCntWindow].nType = 19;
			}
			else if(nUse == 2)
			{
				g_aWindow[nCntWindow].bUse = false;
			}
		}
	}
}
//=============================================================================
// 50���\���̘g�ړ�����
//=============================================================================
void FontWindowMove(int nWide, int nHeight)
{
	int nCntWindow;

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW; nCntWindow++)
	{
		if (g_aWindow[nCntWindow].use == WINDOWUSE_FONT)
		{
			if (nHeight == 5)
			{//�Ђ� �J�i �I���̏ꍇ
				g_aWindow[nCntWindow].nLengthY = SCREEN_HEIGHT / 30;
				g_aWindow[nCntWindow].pos.y = 4.9f * SCREEN_HEIGHT * 0.118f + SCREEN_HEIGHT * 0.281f;
				if (nWide == 0)
				{
					g_aWindow[nCntWindow].nLengthX = SCREEN_WIDTH / 20;
					g_aWindow[nCntWindow].pos.x = SCREEN_WIDTH * 0.505f;
				}
				else if (nWide == 1)
				{
					g_aWindow[nCntWindow].nLengthX = SCREEN_WIDTH / 20;
					g_aWindow[nCntWindow].pos.x = SCREEN_WIDTH * 0.608f;
				}
				else if (nWide == 2)
				{
					g_aWindow[nCntWindow].nLengthX = SCREEN_WIDTH / 17;
					g_aWindow[nCntWindow].pos.x = SCREEN_WIDTH * 0.7305f;
				}
			}
			else
			{//�Ђ� �J�i �I��� �ȊO
				g_aWindow[nCntWindow].nLengthX = SCREEN_WIDTH / 38;
				g_aWindow[nCntWindow].nLengthY = SCREEN_HEIGHT / 16;
				g_aWindow[nCntWindow].pos.x = nWide * SCREEN_WIDTH * 0.0525f + SCREEN_WIDTH * 0.2365f;
				g_aWindow[nCntWindow].pos.y = nHeight * SCREEN_HEIGHT * 0.118f + SCREEN_HEIGHT * 0.2815f;
			}
		}
	}
}