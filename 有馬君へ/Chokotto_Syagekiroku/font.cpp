//=========================================================================================================//
//
// �w�i���� [bg.cpp]
// Author : Ryou Sugimoto
//
//=========================================================================================================//
#define _CRT_SECURE_NO_WARNINGS
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//*********************************************************************************************************//
// �}�N����`
//*********************************************************************************************************//
#define MAX_FONT			(16)
#define SETFONT_WIDE		(SCREEN_WIDTH * 0.35)
#define SETFONT_HEIGHT		(SCREEN_HEIGHT * 0.4)
#define Font_TextureName	"data/TEXTURE/50on.jpg"		//50���\

//*********************************************************************************************************//
// �O���[�o���ϐ�
//*********************************************************************************************************//
LPDIRECT3DTEXTURE9 g_pTextureFont = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFont = NULL;	//���_BUFFER�ւ̃|�C���^
LPD3DXFONT			g_pFont0 = NULL;			// �t�H���g�ւ̃|�C���^
Font				g_Font[MAX_FONT];
int					g_nSetFont = 0;
//=========================================================================================================//
// ����������
//=========================================================================================================//
void InitFont(void)
{
	int nCntFont;

	for (nCntFont = 0; nCntFont < MAX_FONT; nCntFont++)
	{
		//g_Font[nCntFont].rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
		g_Font[nCntFont].bUse = false;
	}

	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	D3DXCreateTextureFromFile(pDevice, Font_TextureName, &g_pTextureFont);	//�^�C�g��

	//���_BUFFER�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,		//�m�ۂ���BUFFER�̃T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffFont,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
							//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffFont->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f - SETFONT_WIDE, SCREEN_HEIGHT * 0.5f - SETFONT_HEIGHT, 0.0f);		//X,Y,0.0f
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f + SETFONT_WIDE, SCREEN_HEIGHT * 0.5f - SETFONT_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f - SETFONT_WIDE, SCREEN_HEIGHT * 0.5f + SETFONT_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f + SETFONT_WIDE, SCREEN_HEIGHT * 0.5f + SETFONT_HEIGHT, 0.0f);

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
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.5f);

	//���_BUFFER���A�����b�N����
	g_pVtxBuffFont->Unlock();

	D3DXCreateFont(pDevice, 40, 12, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Font", &g_pFont0);
}
//=========================================================================================================//
// �I������
//=========================================================================================================//
void UninitFont(void)
{
	if (g_pFont0 != NULL)
	{// �f�o�b�O�\���p�t�H���g�̊J��
		g_pFont0->Release();
		g_pFont0 = NULL;
	}
}
//=========================================================================================================//
// �X�V����
//=========================================================================================================//
void UpdateFont(void)
{
	int nCntFont;

	for (nCntFont = 0; nCntFont < MAX_FONT; nCntFont++)
	{
		if (g_Font[nCntFont].bUse == true)
		{//�������\����ԂȂ�
			switch (g_Font[nCntFont].state)
			{
			case FONTSTATE_RANKDOWN://������
				g_Font[nCntFont].fCntState += 0.028f;
				g_Font[nCntFont].rect.top += 3;
				g_Font[nCntFont].rect.bottom += 3;
				if (g_Font[nCntFont].fCntState >= 1.0f)
				{
					g_Font[nCntFont].fCntState = 0.0f;
					g_Font[nCntFont].state = FONTSTATE_NONE;
				}
				break;

			case FONTSTATE_DELETE://������Ȃ��������
				g_Font[nCntFont].col.a -= 0.028f;
				g_Font[nCntFont].rect.top += 3;
				g_Font[nCntFont].rect.bottom += 3;
				if (g_Font[nCntFont].col.a <= 0.0f)
				{
					g_Font[nCntFont].bUse = false;
				}
				break;

			case FONTSTATE_RANKSET:
				g_Font[nCntFont].col.a += 0.029f;
				g_Font[nCntFont].rect.left -= 5;
				g_Font[nCntFont].rect.right -= 5;
				if (g_Font[nCntFont].col.a >= 1.0f)
				{
					g_Font[nCntFont].col.a = 1.0f;
					g_Font[nCntFont].state = FONTSTATE_NONE;
				}
				break;

			case FONTSTATE_RANKDELETE:
				g_Font[nCntFont].col.a -= 0.029f;
				g_Font[nCntFont].rect.left -= 5;
				g_Font[nCntFont].rect.right -= 5;
				if (g_Font[nCntFont].col.a <= 0.0f)
				{
					g_Font[nCntFont].col.a = 0.0f;
					g_Font[nCntFont].bUse = false;
				}
				break;

			default:
				break;
			}
		}
	}
}

//=========================================================================================================//
// �`�揈��
//=========================================================================================================//
void DrawFont(void)
{
	int nCntFont;
	
	if (g_nSetFont != 0)
	{//50���\�\��
		VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
		//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffFont->Lock(0, 0, (void**)&pVtx, 0);

		//�e�N�X�`�����W�ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, (g_nSetFont - 1.0f) * 0.5f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, (g_nSetFont - 1.0f) * 0.5f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, g_nSetFont * 0.5f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, g_nSetFont * 0.5f);

		//���_BUFFER���A�����b�N����
		g_pVtxBuffFont->Unlock();

		LPDIRECT3DDEVICE9 pDevice;
		//�f�o�C�X���擾����
		pDevice = GetDevice();


		//���_BUFFER���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffFont, 0, sizeof(VERTEX_2D));

		pDevice->SetFVF(FVF_VERTEX_2D);	// ���_�t�H�[�}�b�g�̐ݒ�

										//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureFont);
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

	// �e�L�X�g�`��
	// hDC      : �f�o�C�X�R���e�L�X�g�̃n���h��
	// lpString : �`�悷�镶����ւ̃|�C���^
	// nCount   : ������̕�����
	// lpRect   : �����`�̈�
	// uFormat  : �e�L�X�g�̐��`���@���w��
	for (nCntFont = 0; nCntFont < MAX_FONT; nCntFont++)
	{
		if (g_Font[nCntFont].bUse == true)
		{// ��������� �\��
			g_pFont0->DrawText(NULL, &g_Font[nCntFont].aStr[0], -1, &g_Font[nCntFont].rect, DT_LEFT, g_Font[nCntFont].col);
		}
	}
}
//=========================================================================================================//
// �����������  Set!!
//=========================================================================================================//
void SetFont(RECT rect, D3DXCOLOR col, char *aStr, int nNumber, FONTSTATE state)
{//�����Z�b�g
	g_Font[nNumber].bUse = true;
	g_Font[nNumber].state = state;
	g_Font[nNumber].rect = rect;
	strcpy(&g_Font[nNumber].aStr[0], &aStr[0]);
	g_Font[nNumber].col = col;
	g_Font[nNumber].fCntState = 0.0f;
}
//=========================================================================================================//
// �����������  Change!!
//=========================================================================================================//
void ChangeFont(bool bUse, char *aStr, int nNumber, FONTSTATE state)
{//�Z�b�g���ꂽ���̂�ύX
	g_Font[nNumber].bUse = bUse;
	g_Font[nNumber].state = state;
	if(aStr[0] != NULL) { strcpy(&g_Font[nNumber].aStr[0], &aStr[0]); }//NULL����Ȃ���΍X�V
}
//=========================================================================================================//
// ������S�Ĕj��
//=========================================================================================================//
void BreakFont(void)
{//�Z�b�g���ꂽ���������ׂĔj��
	int nCntFont;

	for (nCntFont = 0; nCntFont < MAX_FONT; nCntFont++)
	{
		g_Font[nCntFont].bUse = false;
	}
}
//=========================================================================================================//
// �Z�b�g����Ă���50���̎擾
//=========================================================================================================//
int *GetSetFont(void)
{
	return &g_nSetFont;
}