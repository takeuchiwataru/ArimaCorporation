//=============================================================================
//
// �G�t�F�N�g�̏��� [effect.cpp]
// Author : 
//
//=============================================================================
#include "game.h"
#include <stdlib.h>				//�����h�p
#include <time.h>				//�����������h�p

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_EFFECT		"data/TEXTURE/effect000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	MAX_EFFECT			(5024)							// �G�t�F�N�g�̍ő吔
#define EFFECT_SLIP			(0.8f)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureEffect = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

//=============================================================================
// ����������
//=============================================================================
void InitEffect(void)
{
	int nCntEffect;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	EFFECT *g_aEffect = GetEffect();

	for(int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].bUse = false;
	}

	D3DXCreateTextureFromFile(pDevice, TEXTURE_EFFECT, &g_pTextureEffect);

	// ���_���̍쐬
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//���_���W�̐ݒ�
		//pVtx[nCntEnemy * 4].pos = D3DXVECTOR3(0, 0, 0.0f);
		//pVtx[nCntEnemy * 4 + 1].pos = D3DXVECTOR3(0, 0, 0.0f);
		//pVtx[nCntEnemy * 4 + 2].pos = D3DXVECTOR3(0, 0, 0.0f);
		//pVtx[nCntEnemy * 4 + 3].pos = D3DXVECTOR3(0, 0, 0.0f);

		pVtx[nCntEffect * 4].rhw = 1.0f;
		pVtx[nCntEffect * 4 + 1].rhw = 1.0f;
		pVtx[nCntEffect * 4 + 2].rhw = 1.0f;
		pVtx[nCntEffect * 4 + 3].rhw = 1.0f;

		//�e�N�X�`�����W�ݒ�
		pVtx[nCntEffect * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nCntEffect * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[nCntEffect * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[nCntEffect * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}

//=============================================================================
// �I������
//=============================================================================
void UninitEffect(void)
{
	// �e�N�X�`���̊J��
		if (g_pTextureEffect != NULL)
		{
			g_pTextureEffect->Release();
			g_pTextureEffect = NULL;
		}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEffect(void)
{
	int nCntEffect;	//�G�t�F�N�g�𐔂���
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
	EFFECT *g_aEffect = GetEffect();
							//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{//�G�t�F�N�g�̍ő�܂ŉ��
		if (g_aEffect[nCntEffect].bUse == true)
		{//�g���Ă���G�t�F�N�g�Ȃ�

			g_aEffect[nCntEffect].pos += D3DXVECTOR3(sinf(g_aEffect[nCntEffect].fSteep), cosf(g_aEffect[nCntEffect].fSteep), 0.0f) * g_aEffect[nCntEffect].fSpeed;
			g_aEffect[nCntEffect].move.x += (0.0f - g_aEffect[nCntEffect].move.x) * EFFECT_SLIP;
			g_aEffect[nCntEffect].move.y += (0.0f - g_aEffect[nCntEffect].move.y) * EFFECT_SLIP;
			g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;
			//���񔖂�����
			g_aEffect[nCntEffect].col.a -= 0.03f;

			if (g_aEffect[nCntEffect].col.a <= 0.0f)
			{//�����x��0�ɂȂ�����
				g_aEffect[nCntEffect].bUse = false;
			}

			pVtx[nCntEffect * 4].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.g, g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);	// A = �����x
			pVtx[nCntEffect * 4 + 1].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.g, g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);
			pVtx[nCntEffect * 4 + 2].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.g, g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);
			pVtx[nCntEffect * 4 + 3].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.g, g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);

			pVtx[nCntEffect * 4].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);
		}
	}//for����

	 //���_BUFFER���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	EFFECT *g_aEffect = GetEffect();
	int nCntEffect;

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
    pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureEffect);

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �|���S���̕`��
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//true�Ȃ�\��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
		}
	}

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
void MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�𐶐�
 

	// ���_�o�b�t�@�̏���ݒ�

}


//=============================================================================
// �G�t�F�N�g�ݒ�
//=============================================================================
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSteep, float fSpeed, float fRadius)
{
	int nCntEffect;
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
	EFFECT *g_aEffect = GetEffect();

							//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{
			//D3DXVECTOR3 pos;		// �ʒu
			//D3DXVECTOR3 move;		// �ړ��l�̎c���
			//D3DXCOLOR col;			// �F
			//float fSteep;			//�p�x
			//float fSpeed;
			//float fRadius;			// ���a(�傫��)
			//bool bUse;				// �g�p���Ă��邩�ǂ���
			g_aEffect[nCntEffect] = { pos, move, col, fSteep, fSpeed, fRadius, true };

			//g_aEffect[nCntEffect].move = move;
			//g_aEffect[nCntEffect].pos = pos;
			//g_aEffect[nCntEffect].fSteep = fSteep;
			//g_aEffect[nCntEffect].fSpeed = fSpeed;
			//g_aEffect[nCntEffect].fRadius = fRadius;
			//g_aEffect[nCntEffect].bUse = true;

			pVtx[nCntEffect * 4].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);

			//g_aEffect[nCntEffect].col = col;
			pVtx[nCntEffect * 4].col = D3DXCOLOR(col.r, col.g, col.b, col.a);	// A = �����x
			pVtx[nCntEffect * 4 + 1].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
			pVtx[nCntEffect * 4 + 2].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
			pVtx[nCntEffect * 4 + 3].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
			break;
		}
	}
	//���_BUFFER���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}
//=============================================================================
// �q�b�g���̃G�t�F�N�g�ݒ�
//=============================================================================
void ExplosionEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nSRand, int nLRand, float fSRand0, float fSRand1, float fLRand0, float fLRand1)
{
	static int nCntEffect;
	int nSetEffect;
	int nRand[3];
	float fRand[3];

	nSetEffect = 0;


	//srand((unsigned int)time(0));	//�����h�̂���
	while(1)
	{//�G�t�F�N�g�S�����
		nRand[0] = rand() % 628;
		nRand[1] = rand() % nSRand;
		nRand[2] = rand() % nLRand;

		fRand[0] = nRand[0] * 0.01f;					//�p�x�����_��
		fRand[1] = nRand[1] * fSRand0 + fSRand1;		//SPEED�����_��
		fRand[2] = nRand[2] * fLRand0 + fLRand1;		//�傫�������_��

		SetEffect(pos, move, col, fRand[0], fRand[1], fRand[2]);


		nSetEffect++;
		if (nSetEffect > 50)
		{//100�o����I��
			break;
		}

	}
}
//=============================================================================
// �G�̎擾
//=============================================================================
EFFECT *GetEffect(void)
{
	static EFFECT effect[MAX_EFFECT];//�e�̍ő吔

	return &effect[0];
}
