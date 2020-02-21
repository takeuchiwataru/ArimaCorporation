//=============================================================================
//
// �X�R�A���� [score.cpp]
// Author : 
//
//=============================================================================
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCORE_VERTEX_NUM			(4)					// ���_�̐�
#define SCORE_PRIMITIVE_NUM		(2)					// �v���~�e�B�u�̐�
#define	TEXTURE_SCORE		"data/TEXTURE/number000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define MAX_NUMBER			(8)
#define MAX_SETSCORE		(6)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexScore(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureScore = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
SCORE g_aScore[MAX_SETSCORE];
//=============================================================================
// ����������
//=============================================================================
void InitScore(void)
{
	int nCntScore;
	int nCntScore1;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (nCntScore = 0; nCntScore < MAX_SETSCORE; nCntScore++)
	{
		g_aScore[nCntScore].fCntstate = 0.0f;
		g_aScore[nCntScore].bUse = false;
	}

	//���_BUFFER�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * SCORE_VERTEX_NUM * MAX_NUMBER * MAX_SETSCORE,		//�m�ۂ���BUFFER�̃T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	// �X�R�A�̏�����
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
							//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntScore1 = 0; nCntScore1 < MAX_SETSCORE; nCntScore1++)
	{
		for (nCntScore = 0; nCntScore < MAX_NUMBER; nCntScore++)
		{
			////���_���W�̐ݒ�
			//pVtx[nCntScore * 4].pos = D3DXVECTOR3(0, 0, 0.0f);
			//pVtx[nCntScore * 4 + 1].pos = D3DXVECTOR3(0, 0, 0.0f);
			//pVtx[nCntScore * 4 + 2].pos = D3DXVECTOR3(0, 0, 0.0f);
			//pVtx[nCntScore * 4 + 3].pos = D3DXVECTOR3(0, 0, 0.0f);

			pVtx[nCntScore * 4].rhw = 1.0f;
			pVtx[nCntScore * 4 + 1].rhw = 1.0f;
			pVtx[nCntScore * 4 + 2].rhw = 1.0f;
			pVtx[nCntScore * 4 + 3].rhw = 1.0f;

			//�J���[�ݒ�
			pVtx[nCntScore * 4].col = D3DXCOLOR(1.0f, 0.99f, 0.04f, 1.0f);	// A = �����x
			pVtx[nCntScore * 4 + 1].col = D3DXCOLOR(1.0f, 0.99f, 0.04f, 1.0f);
			pVtx[nCntScore * 4 + 2].col = D3DXCOLOR(1.0f, 0.99f, 0.04f, 1.0f);
			pVtx[nCntScore * 4 + 3].col = D3DXCOLOR(1.0f, 0.99f, 0.04f, 1.0f);

			////�e�N�X�`�����W�ݒ�
			//pVtx[nCntScore * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
			//pVtx[nCntScore * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
			//pVtx[nCntScore * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
			//pVtx[nCntScore * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		pVtx += 4 * 8;
	}

	//���_BUFFER���A�����b�N����
	g_pVtxBuffScore->Unlock();


	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
								TEXTURE_SCORE,		// �t�@�C���̖��O
								&g_pTextureScore);	// �e�N�X�`���ւ̃|�C���^

	// ���_���̍쐬
	MakeVertexScore(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitScore(void)
{
	BreakScore();
	if (g_pTextureScore != NULL)
	{// �e�N�X�`���̊J��
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}
	if (g_pVtxBuffScore != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateScore(void)
{
	int nCntScore[2];
	int nScore;
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
							//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore[0] = 0; nCntScore[0] < MAX_SETSCORE; nCntScore[0]++)
	{
		if (g_aScore[nCntScore[0]].bUse == true)
		{
			switch (g_aScore[nCntScore[0]].state)
			{//��Ԃɉ����ď���
			case SCORESTATE_FADE:
				g_aScore[nCntScore[0]].col.a += 0.029f;
				if (g_aScore[nCntScore[0]].col.a >= 1.0f)
				{
					g_aScore[nCntScore[0]].col.a = 1.0f;
					g_aScore[nCntScore[0]].state = SCORESTATE_NONE;
				}
				break;

			case SCORESTATE_SET:
				g_aScore[nCntScore[0]].col.a += 0.029f;
				g_aScore[nCntScore[0]].pos.y += 3.0f;
				if (g_aScore[nCntScore[0]].col.a >= 1.0f)
				{
					g_aScore[nCntScore[0]].col.a = 1.0f;
					g_aScore[nCntScore[0]].state = SCORESTATE_NONE;
				}
				break;

			case SCORESTATE_DELETE:
				g_aScore[nCntScore[0]].col.a -= 0.028f;
				g_aScore[nCntScore[0]].pos.y += 3.0f;
				if (g_aScore[nCntScore[0]].col.a <= 0.0f)
				{
					g_aScore[nCntScore[0]].bUse = false;
				}
				break;

			case SCORESTATE_RANKDOWN:
				if (g_aScore[nCntScore[0]].fCntstate <= 1.0f)
				{
					g_aScore[nCntScore[0]].fCntstate += 0.028f;
					g_aScore[nCntScore[0]].pos.y += 3.0f;
				}
				else
				{
					g_aScore[nCntScore[0]].fCntstate = 0.0f;
					g_aScore[nCntScore[0]].state = SCORESTATE_NONE;
				}
				break;

			case SCORESTATE_RANKSET:
				g_aScore[nCntScore[0]].col.a += 0.029f;
				g_aScore[nCntScore[0]].pos.x -= 5.0f;
				if (g_aScore[nCntScore[0]].col.a >= 1.0f)
				{
					g_aScore[nCntScore[0]].col.a = 1.0f;
					g_aScore[nCntScore[0]].state = SCORESTATE_NONE;
				}
				break;

			case SCORESTATE_RANKDELETE:
				g_aScore[nCntScore[0]].col.a -= 0.028f;
				//g_aScore[nCntScore[0]].pos.x -= 5.0f;
				if (g_aScore[nCntScore[0]].col.a <= 0.0f)
				{
					g_aScore[nCntScore[0]].bUse = false;
				}
				break;

			default:
				break;
			}
			nScore = g_aScore[nCntScore[0]].g_nScore;
			for (nCntScore[1] = 0; nCntScore[1] < MAX_NUMBER; nCntScore[1]++)
			{//8�����|���S���𐶐�
			 //�J���[�ݒ�
				pVtx[nCntScore[1] * 4].col = D3DXCOLOR(1.0f, 0.99f, 0.04f, g_aScore[nCntScore[0]].col.a);	// A = �����x
				pVtx[nCntScore[1] * 4 + 1].col = D3DXCOLOR(1.0f, 0.99f, 0.04f, g_aScore[nCntScore[0]].col.a);
				pVtx[nCntScore[1] * 4 + 2].col = D3DXCOLOR(1.0f, 0.99f, 0.04f, g_aScore[nCntScore[0]].col.a);
				pVtx[nCntScore[1] * 4 + 3].col = D3DXCOLOR(1.0f, 0.99f, 0.04f, g_aScore[nCntScore[0]].col.a);

				//���_���W�̐ݒ�
				pVtx[nCntScore[1] * 4].pos = D3DXVECTOR3(g_aScore[nCntScore[0]].pos.x - 20 - (nCntScore[1] * 40), g_aScore[nCntScore[0]].pos.y - 40, 0.0f);
				pVtx[nCntScore[1] * 4 + 1].pos = D3DXVECTOR3(g_aScore[nCntScore[0]].pos.x + 20 - (nCntScore[1] * 40), g_aScore[nCntScore[0]].pos.y - 40, 0.0f);
				pVtx[nCntScore[1] * 4 + 2].pos = D3DXVECTOR3(g_aScore[nCntScore[0]].pos.x - 20 - (nCntScore[1] * 40), g_aScore[nCntScore[0]].pos.y + 40, 0.0f);
				pVtx[nCntScore[1] * 4 + 3].pos = D3DXVECTOR3(g_aScore[nCntScore[0]].pos.x + 20 - (nCntScore[1] * 40), g_aScore[nCntScore[0]].pos.y + 40, 0.0f);

				//�e�N�X�`�����W�ݒ�
				pVtx[nCntScore[1] * 4].tex = D3DXVECTOR2((nScore % 10) * 0.1f, 0.0f);
				pVtx[nCntScore[1] * 4 + 1].tex = D3DXVECTOR2((nScore % 10) * 0.1f + 0.1f, 0.0f);
				pVtx[nCntScore[1] * 4 + 2].tex = D3DXVECTOR2((nScore % 10) * 0.1f, 1.0f);
				pVtx[nCntScore[1] * 4 + 3].tex = D3DXVECTOR2((nScore % 10) * 0.1f + 0.1f, 1.0f);
				nScore /= 10;
			}
		}
		pVtx += 32;
	}
	//���_BUFFER���A�����b�N����
	g_pVtxBuffScore->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawScore(void)
{
	int nCntScore[2];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
    pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureScore);
	// �|���S���̕`��
	for (nCntScore[0] = 0; nCntScore[0] < MAX_SETSCORE; nCntScore[0]++)
	{//�X�R�A�����
		if (g_aScore[nCntScore[0]].bUse == true)
		{//�w�肳�ꂽ�X�R�A���g���Ă���Ȃ�\��
			for (nCntScore[1] = 0; nCntScore[1] < MAX_NUMBER; nCntScore[1]++)
			{
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore[1] * 4 + 32 * nCntScore[0], SCORE_PRIMITIVE_NUM);
			}
		}
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexScore(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�𐶐�


	// ���_����ݒ�

}

//=============================================================================
// �X�R�A�̉��Z
//=============================================================================
void AddScore(int nValue, int nNumber)
{
	int nCntScore;
	int nScore;
	Difficulty *difficulty;
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	difficulty = GetDifficulty();	//��Փx�擾

	//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	g_aScore[nNumber].g_nScore += nValue * difficulty[0].nDifficulty / 100;	//��Փx�����߂�
	if (g_aScore[nNumber].g_nScore < 0)
	{//�}�C�i�X�ɂȂ�����0���
		g_aScore[nNumber].g_nScore = 0;
	}

	nScore = g_aScore[nNumber].g_nScore;

	pVtx += 32 * nNumber;
	for (nCntScore = 0; nCntScore < MAX_NUMBER; nCntScore++)
	{//8�����|���S���𐶐�
		//�e�N�X�`�����W�ݒ�
		pVtx[nCntScore * 4].tex = D3DXVECTOR2((nScore % 10) * 0.1f, 0.0f);
		pVtx[nCntScore * 4 + 1].tex = D3DXVECTOR2((nScore % 10) * 0.1f + 0.1f, 0.0f);
		pVtx[nCntScore * 4 + 2].tex = D3DXVECTOR2((nScore % 10) * 0.1f, 1.0f);
		pVtx[nCntScore * 4 + 3].tex = D3DXVECTOR2((nScore % 10) * 0.1f + 0.1f, 1.0f);
		nScore /= 10;
	}

	//���_BUFFER���A�����b�N����
	g_pVtxBuffScore->Unlock();
}
//=============================================================================
// �X�R�A�̃Z�b�g
//=============================================================================
void SetScore(D3DXVECTOR3 pos, D3DXCOLOR col, int nScore, int nNumber, SCORESTATE state)
{
	int nCntScore;
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
	//���_BUFFER�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);


	g_aScore[nNumber].bUse = true;
	g_aScore[nNumber].g_nScore = nScore;
	g_aScore[nNumber].pos = pos;
	g_aScore[nNumber].col = col;
	g_aScore[nNumber].state = state;
	pVtx += 32 * nNumber;
	for (nCntScore = 0; nCntScore < MAX_NUMBER; nCntScore++)
	{//8�����|���S���𐶐�
	//�J���[�ݒ�
		pVtx[nCntScore * 4].col = D3DXCOLOR(1.0f, 0.99f, 0.04f, col.a);	// A = �����x
		pVtx[nCntScore * 4 + 1].col = D3DXCOLOR(1.0f, 0.99f, 0.04f, col.a);
		pVtx[nCntScore * 4 + 2].col = D3DXCOLOR(1.0f, 0.99f, 0.04f, col.a);
		pVtx[nCntScore * 4 + 3].col = D3DXCOLOR(1.0f, 0.99f, 0.04f, col.a);

		//���_���W�̐ݒ�
		pVtx[nCntScore * 4].pos = D3DXVECTOR3(pos.x - 20 - (nCntScore * 40), pos.y - 40, 0.0f);
		pVtx[nCntScore * 4 + 1].pos = D3DXVECTOR3(pos.x + 20 - (nCntScore * 40), pos.y - 40, 0.0f);
		pVtx[nCntScore * 4 + 2].pos = D3DXVECTOR3(pos.x - 20 - (nCntScore * 40), pos.y + 40, 0.0f);
		pVtx[nCntScore * 4 + 3].pos = D3DXVECTOR3(pos.x + 20 - (nCntScore * 40), pos.y + 40, 0.0f);

		//�e�N�X�`�����W�ݒ�
		pVtx[nCntScore * 4].tex = D3DXVECTOR2((nScore % 10) * 0.1f, 0.0f);
		pVtx[nCntScore * 4 + 1].tex = D3DXVECTOR2((nScore % 10) * 0.1f + 0.1f, 0.0f);
		pVtx[nCntScore * 4 + 2].tex = D3DXVECTOR2((nScore % 10) * 0.1f, 1.0f);
		pVtx[nCntScore * 4 + 3].tex = D3DXVECTOR2((nScore % 10) * 0.1f + 0.1f, 1.0f);
		nScore /= 10;
	}

	//���_BUFFER���A�����b�N����
	g_pVtxBuffScore->Unlock();
}
//=============================================================================
// �X�R�A�̑S�j��
//=============================================================================
void BreakScore(void)
{
	int nCntScore;

	for (nCntScore = 0; nCntScore < MAX_SETSCORE; nCntScore++)
	{
		g_aScore[nCntScore].bUse = false;
	}
}
//=============================================================================
// �X�R�A�̏�ԕύX
//=============================================================================
void ChangeScore(int nNumber, SCORESTATE state)
{//�X�R�A�̏�ԕύX
	g_aScore[nNumber].state = state;
}
//=========================================================================================================//
// * �����L���O�̂���
//=========================================================================================================//
SCORE *GetGameScore(void)
{
	return &g_aScore[0];
}
