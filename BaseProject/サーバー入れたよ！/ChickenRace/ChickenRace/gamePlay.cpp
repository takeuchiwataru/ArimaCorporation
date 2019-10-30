//=============================================================================
//
// �Q�[���i�v���C�j�̏��� [game.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS	//Fire Wall�˔j
#include <string.h>				//�X�g�����O�g�p�̂���
#include "gamePlay.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "input.h"
#include "fade.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define COUNTDOWN_SIZE		(SCREEN_HEIGHT * 0.12f)										// �J�E���g�_�E���̑傫��
#define RANKING_SIZE_1P_X	(0.13f)														// 1P���̃����L���O�̑傫��X
#define RANKING_SIZE_1P_Y	(0.09f)														// 1P���̃����L���O�̑傫��Y
#define RANKING_SIZE_4P_X	(0.07f)														// 4P���̃����L���O�̑傫��X
#define RANKING_SIZE_4P_Y	(0.05f)														// 4P���̃����L���O�̑傫��Y
#define ITEM_SIZE_1P		(0.11f)														// 1P���̃A�C�e���̑傫��
#define ITEM_SIZE_4P		(0.06f)														// 4P���̃A�C�e���̑傫��
#define GOUL_SIZE_2P		(D3DXVECTOR2(SCREEN_WIDTH * 0.20f, SCREEN_WIDTH * 0.07f))	// 2P���̃S�[���̑傫��
#define GOUL_SIZE_4P		(D3DXVECTOR2(SCREEN_WIDTH * 0.15f, SCREEN_WIDTH * 0.05f))	// 4P���̃S�[���̑傫��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	CGamePlay::m_pTexture[CGamePlay::TEXTURE_MAX] = { NULL };

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CGamePlay::CGamePlay()
{
	for (int nCntLine = 0; nCntLine < MAX_LINE; nCntLine++)
	{// ���J�E���g
		m_pLine[nCntLine] = NULL;					// ��
	}

	for (int nCntDown = 0; nCntDown < COUNT_DOWN; nCntDown++)
	{// �_�E���J�E���g
		m_pCountDown[nCntDown] = NULL;				// �J�E���g�_�E��
	}

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// �v���C���[�J�E���g
		m_pRanking[nCntPlayer] = NULL;				// �����L���O

		for (int nCntItem = 0; nCntItem < MAX_EGG; nCntItem++)
		{// �A�C�e���J�E���g
			m_pItem[nCntPlayer][nCntItem] = NULL;	// �A�C�e��
		}

		m_pGoul[nCntPlayer] = NULL;					// �����L���O
	}
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGamePlay::~CGamePlay() {}

//=============================================================================
// ���[�h
//=============================================================================
HRESULT CGamePlay::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{// �e�N�X�`���J�E���g
		char cName[128] = {};

		switch (nCntTex)
		{
		case TEXTURE_COUNT:
			strcpy(cName, "data/TEXTURE/game/play/count.png");
			break;
		case TEXTURE_RANK:
			strcpy(cName, "data/TEXTURE/game/play/rank.png");
			break;
		case TEXTURE_GOUL:
			strcpy(cName, "data/TEXTURE/game/play/finish.png");
			break;
		}

		// �e�N�X�`���̐���
		if (m_pTexture[nCntTex] == NULL)
			D3DXCreateTextureFromFile(pDevice, cName, &m_pTexture[nCntTex]);
	}

	return S_OK;
}

//=============================================================================
// �A�����[�h
//=============================================================================
void CGamePlay::Unload(void)
{
	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{// �e�N�X�`���J�E���g
		//�e�N�X�`���[�̔j��
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// �N���G�C�g
//=============================================================================
CGamePlay *CGamePlay::Create(void)
{
	CGamePlay *pGamePlay = NULL;

	// �������m��
	if (pGamePlay == NULL)
		pGamePlay = new CGamePlay;

	if (pGamePlay != NULL)
	{// NULL�ȊO
		pGamePlay->Init();	// ����������
	}

	return pGamePlay;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CGamePlay::Init()
{
	// �v���C���[�ő吔�擾
	int nMaxPlayer = CGame::GetMaxPlayer();

	if (1 < nMaxPlayer)
	{// 2�l�ȏ�
		for (int nCntLine = 0; nCntLine < ((nMaxPlayer - 1) / 2 == 0 ? 1 : MAX_LINE); nCntLine++)
		{// ���J�E���g
			if (m_pLine[nCntLine] == NULL)
			{// NULL�ȊO
				D3DXVECTOR2 size = D3DXVECTOR2(0.0f, 0.0f);

				if (nCntLine == 0)
					size = D3DXVECTOR2((SCREEN_WIDTH * 0.5f), (SCREEN_WIDTH * 0.0006f));
				else
					size = D3DXVECTOR2((SCREEN_WIDTH * 0.0006f), (SCREEN_HEIGHT * 0.5f));

				m_pLine[nCntLine] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
				m_pLine[nCntLine]->Init();
				m_pLine[nCntLine]->SetPosSize(
					D3DXVECTOR3((SCREEN_WIDTH * 0.5f), (SCREEN_HEIGHT * 0.5f), 0.0f),
					size);
				m_pLine[nCntLine]->SetColor(&D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
			}
		}
	}

	for (int nCntDown = 0; nCntDown < COUNT_DOWN; nCntDown++)
	{// �_�E���J�E���g
		if (m_pCountDown[nCntDown] == NULL)
		{// NULL
			m_pCountDown[nCntDown] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
			m_pCountDown[nCntDown]->Init();
			m_pCountDown[nCntDown]->SetPosSize(
				D3DXVECTOR3(
					((SCREEN_WIDTH * 0.5f) - (COUNTDOWN_SIZE * 2.1f)) + ((COUNTDOWN_SIZE * 2.1f) * nCntDown),
					(SCREEN_HEIGHT * 0.5f),
					0.0f), 
				D3DXVECTOR2(
					COUNTDOWN_SIZE,
					COUNTDOWN_SIZE));
			m_pCountDown[nCntDown]->BindTexture(m_pTexture[(TEXTURE)(TEXTURE_COUNT)]);
			m_pCountDown[nCntDown]->SetTexture(nCntDown, 3, 1, 1);
			m_pCountDown[nCntDown]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
	}

	for (int nCntPlayer = 0; nCntPlayer < nMaxPlayer; nCntPlayer++)
	{// �v���C���[�J�E���g
		// �����L���O
		if (m_pRanking[nCntPlayer] == NULL)
		{// NULL
			m_pRanking[nCntPlayer] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
			m_pRanking[nCntPlayer]->Init();
			// �e�r���[�|�[�g�̒��S�_����v�Z�i���S�_ -> �w��̈ʒu�ցj
			m_pRanking[nCntPlayer]->SetPosSize(
				//D3DXVECTOR3(
				//	(SCREEN_WIDTH * ((nMaxPlayer - 1) == 0 ? 0.5f : (nCntPlayer % 2 == 0 ? 0.25f : 0.75f)))			+ (((SCREEN_WIDTH * ((nMaxPlayer - 1) == 0 ? 0.5f : 0.25f))		- 
				//	(SCREEN_HEIGHT * ((nMaxPlayer - 1) / 2 == 0 ? RANKING_SIZE_2P : RANKING_SIZE_4P))) * (nCntPlayer % 2 == 0 ? -1.0f : 1.0f)),
				//	(SCREEN_HEIGHT * ((nMaxPlayer - 1) / 2 == 0 ? 0.5f : (nCntPlayer / 2 == 0 ? 0.25f : 0.75f)))	+ ((SCREEN_HEIGHT * ((nMaxPlayer - 1) / 2 == 0 ? 0.5f : 0.25f)) - 
				//	(SCREEN_HEIGHT * ((nMaxPlayer - 1) / 2 == 0 ? RANKING_SIZE_2P : RANKING_SIZE_4P))),
				//	0.0f),
				//D3DXVECTOR2(
				//	SCREEN_HEIGHT * ((nMaxPlayer - 1) / 2 == 0 ? RANKING_SIZE_2P : RANKING_SIZE_4P),
				//	SCREEN_HEIGHT * ((nMaxPlayer - 1) / 2 == 0 ? RANKING_SIZE_2P : RANKING_SIZE_4P)));
				D3DXVECTOR3(
					(SCREEN_WIDTH * ((nMaxPlayer - 1) / 2 == 0 ? 0.5f : (nCntPlayer % 2 == 0 ? 0.25f : 0.75f))) + (((SCREEN_WIDTH * ((nMaxPlayer - 1) / 2 == 0 ? 0.5f : 0.25f)) -
					(SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? RANKING_SIZE_1P_X : RANKING_SIZE_4P_X))) * ((nMaxPlayer - 1) / 2 == 0 ? -1.0f : (nCntPlayer % 2 == 0 ? -1.0f : 1.0f))),
					(SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? 0.5f : ((nMaxPlayer - 1) / 2 == 0 ? (nCntPlayer % 2 == 0 ? 0.25f : 0.75f) : (nCntPlayer / 2 == 0 ? 0.25f : 0.75f)))) + ((SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? 0.5f : 0.25f)) -
					(SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? RANKING_SIZE_1P_Y : RANKING_SIZE_4P_Y))),
					0.0f),
				D3DXVECTOR2(
					SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? RANKING_SIZE_1P_X : RANKING_SIZE_4P_X),
					SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? RANKING_SIZE_1P_Y * 0.8f : RANKING_SIZE_4P_Y * 0.8f)));
			m_pRanking[nCntPlayer]->BindTexture(m_pTexture[TEXTURE_RANK]);
			m_pRanking[nCntPlayer]->SetTexture(nCntPlayer, 8, 1, 1);
		}

		// �A�C�e��
		for (int nCntItem = 0; nCntItem < MAX_EGG; nCntItem++)
		{// �A�C�e���J�E���g
			if (m_pItem[nCntPlayer][nCntItem] == NULL)
			{// NULL
				m_pItem[nCntPlayer][nCntItem] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
				m_pItem[nCntPlayer][nCntItem]->Init();
				// �e�r���[�|�[�g�̒��S�_����v�Z�i���S�_ -> �w��̈ʒu�ցj
				m_pItem[nCntPlayer][nCntItem]->SetPosSize(
					//D3DXVECTOR3(
					//	(SCREEN_WIDTH * ((nMaxPlayer - 1) == 0 ? 0.5f : (nCntPlayer % 2 == 0 ? 0.25f : 0.75f)))			+ (((SCREEN_WIDTH * ((nMaxPlayer - 1) == 0 ? 0.5f : 0.25f))		- 
					//	(SCREEN_HEIGHT * ((nMaxPlayer - 1) / 2 == 0 ? ITEM_SIZE_2P : ITEM_SIZE_4P))) * (nCntPlayer % 2 == 0 ? -1.0f : 1.0f)),
					//	(SCREEN_HEIGHT * ((nMaxPlayer - 1) / 2 == 0 ? 0.5f : (nCntPlayer / 2 == 0 ? 0.25f : 0.75f)))	- ((SCREEN_HEIGHT * ((nMaxPlayer - 1) / 2 == 0 ? 0.5f : 0.25f)) - 
					//	(SCREEN_HEIGHT * ((nMaxPlayer - 1) / 2 == 0 ? ITEM_SIZE_2P : ITEM_SIZE_4P))) +
					//	(((SCREEN_HEIGHT * ((nMaxPlayer - 1) / 2 == 0 ? ITEM_SIZE_2P : ITEM_SIZE_4P)) * 2.1f) * nCntItem),
					//	0.0f),
					//D3DXVECTOR2(
					//	SCREEN_HEIGHT * ((nMaxPlayer - 1) / 2 == 0 ? ITEM_SIZE_2P : ITEM_SIZE_4P),
					//	SCREEN_HEIGHT * ((nMaxPlayer - 1) / 2 == 0 ? ITEM_SIZE_2P : ITEM_SIZE_4P)));
					D3DXVECTOR3(
						(SCREEN_WIDTH * ((nMaxPlayer - 1) / 2 == 0 ? 0.5f : (nCntPlayer % 2 == 0 ? 0.25f : 0.75f))) + (((SCREEN_WIDTH * ((nMaxPlayer - 1) / 2 == 0 ? 0.5f : 0.25f)) -
						(SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? ITEM_SIZE_1P : ITEM_SIZE_4P))) * ((nMaxPlayer - 1) / 2 == 0 ? -1.0f : (nCntPlayer % 2 == 0 ? -1.0f : 1.0f))),
						(SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? 0.5f : ((nMaxPlayer - 1) / 2 == 0 ? (nCntPlayer % 2 == 0 ? 0.25f : 0.75f) : (nCntPlayer / 2 == 0 ? 0.25f : 0.75f)))) - ((SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? 0.5f : 0.25f)) -
						(SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? ITEM_SIZE_1P : ITEM_SIZE_4P))) +
						(((SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? ITEM_SIZE_1P : ITEM_SIZE_4P)) * 2.1f) * nCntItem),
						0.0f),
					D3DXVECTOR2(
						SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? ITEM_SIZE_1P : ITEM_SIZE_4P),
						SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? ITEM_SIZE_1P : ITEM_SIZE_4P)));
				m_pItem[nCntPlayer][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
			}
		}

		if (m_pGoul[nCntPlayer] == NULL)
		{// NULL�ȊO
			m_pGoul[nCntPlayer] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
			m_pGoul[nCntPlayer]->Init();
			m_pGoul[nCntPlayer]->SetPosSize(
				D3DXVECTOR3(
					(SCREEN_WIDTH * ((nMaxPlayer - 1) / 2 == 0 ? 0.5f : (nCntPlayer % 2 == 0 ? 0.25f : 0.75f))),
					(SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? 0.5f : ((nMaxPlayer - 1) / 2 == 0 ? (nCntPlayer % 2 == 0 ? 0.25f : 0.75f) : (nCntPlayer / 2 == 0 ? 0.25f : 0.75f)))),
					0.0f),
					((nMaxPlayer - 1) / 2 == 0 ? GOUL_SIZE_2P : GOUL_SIZE_4P));
			m_pGoul[nCntPlayer]->BindTexture(m_pTexture[TEXTURE_GOUL]);
			//m_pGoul[nCntPlayer]->SetTexture(nCntDown, 3, 1, 1);
			m_pGoul[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
	}

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CGamePlay::Uninit(void)
{
	for (int nCntLine = 0; nCntLine < MAX_LINE; nCntLine++)
	{// ���J�E���g
		if (m_pLine[nCntLine] != NULL)
		{// NULL�ȊO
			m_pLine[nCntLine]->Uninit();
			m_pLine[nCntLine] = NULL;
		}
	}

	for (int nCntDown = 0; nCntDown < COUNT_DOWN; nCntDown++)
	{// �_�E���J�E���g
		if (m_pCountDown[nCntDown] != NULL)
		{// NULL�ȊO
			m_pCountDown[nCntDown]->Uninit();
			m_pCountDown[nCntDown] = NULL;
		}
	}

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// �v���C���[�J�E���g
		if (m_pRanking[nCntPlayer] != NULL)
		{// NULL�ȊO
			m_pRanking[nCntPlayer]->Uninit();
			m_pRanking[nCntPlayer] = NULL;
		}

		for (int nCntItem = 0; nCntItem < MAX_EGG; nCntItem++)
		{// �A�C�e���J�E���g
			if (m_pItem[nCntPlayer][nCntItem] != NULL)
			{// NULL�ȊO
				m_pItem[nCntPlayer][nCntItem]->Uninit();
				m_pItem[nCntPlayer][nCntItem] = NULL;
			}
		}

		if (m_pGoul[nCntPlayer] != NULL)
		{// NULL�ȊO
			m_pGoul[nCntPlayer]->Uninit();
			m_pGoul[nCntPlayer] = NULL;
		}
	}

	//���g�̍폜
	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CGamePlay::Update(void)
{
	int *pnRanking = CGame::GetRanking();
	int nGameCounter = CGame::GetGameCounter();
	int nCounter = nGameCounter - (300 - 180);
	bool *pbGoul = CGame::GetGoul();

	CSound *pSound = CManager::GetSound();

	// �J�E���g�_�E��
	if ((300 - 180) <= nGameCounter && nGameCounter < 300)
	{
		if (m_pCountDown[(nCounter / 60)] != NULL)
		{// NULL�ȊO
			m_pCountDown[(nCounter / 60)]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			if (nCounter / 60 == 0 && nCounter % 60 == 0)
			{
				//�Q�[���J�n�̃J�E���g�_�E��
				pSound->SetVolume(CSound::SOUND_LABEL_SE_STARTCOUNT, 1.0f);
				pSound->PlaySound(CSound::SOUND_LABEL_SE_STARTCOUNT);
			}
		}
	}
	else
	{
		for (int nCntDown = 0; nCntDown < COUNT_DOWN; nCntDown++)
		{// �_�E���J�E���g
			if (m_pCountDown[nCntDown] != NULL)
			{// NULL�ȊO
				m_pCountDown[nCntDown]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			}
		}
	}

	// �����L���O
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// �v���C���[�J�E���g
		if (m_pRanking[nCntPlayer] != NULL)
		{// NULL�ȊO
			m_pRanking[nCntPlayer]->SetTexture(pnRanking[nCntPlayer], 8, 1, 1);
		}

		if (pbGoul[nCntPlayer] == true)
		{
			if (m_pGoul[nCntPlayer] != NULL)
			{// NULL�ȊO
				m_pGoul[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CGamePlay::Draw(void)
{

}