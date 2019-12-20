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
#include "time.h"
#include "gamecamera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define COUNTDOWN_SIZE		(SCREEN_HEIGHT * 0.12f)										// �J�E���g�_�E���̑傫��
#define TUTORIAL_SIZE		(D3DXVECTOR2(SCREEN_WIDTH * 0.14f, SCREEN_HEIGHT * 0.14f))	// �`���[�g���A���̑傫��
#define GO_SIZE				(D3DXVECTOR2(SCREEN_WIDTH * 0.12f, SCREEN_WIDTH * 0.06f))	// GO�̑傫��
#define RANKING_SIZE_1P_X	(0.13f)														// 1P���̃����L���O�̑傫��X
#define RANKING_SIZE_1P_Y	(0.09f)														// 1P���̃����L���O�̑傫��Y
#define RANKING_SIZE_4P_X	(0.07f)														// 4P���̃����L���O�̑傫��X
#define RANKING_SIZE_4P_Y	(0.05f)														// 4P���̃����L���O�̑傫��Y
#define ITEM_SIZE_1P		(0.11f)														// 1P���̃A�C�e���̑傫��
#define ITEM_SIZE_4P		(0.06f)														// 4P���̃A�C�e���̑傫��
#define GOUL_SIZE_2P		(D3DXVECTOR2(SCREEN_WIDTH * 0.20f, SCREEN_WIDTH * 0.07f))	// 2P���̃S�[���̑傫��
#define GOUL_SIZE_4P		(D3DXVECTOR2(SCREEN_WIDTH * 0.15f, SCREEN_WIDTH * 0.05f))	// 4P���̃S�[���̑傫��
#define VIEW_SIZE_2P		(D3DXVECTOR2(SCREEN_WIDTH * 0.08f, SCREEN_WIDTH * 0.03f))	// 2P���̊ϐ풆�̑傫��
#define VIEW_SIZE_4P		(D3DXVECTOR2(SCREEN_WIDTH * 0.04f, SCREEN_WIDTH * 0.015f))	// 4P���̊ϐ풆�̑傫��
#define CAMERAHINT_SIZE_2P	(D3DXVECTOR2(SCREEN_WIDTH * 0.1f, SCREEN_WIDTH * 0.05f))	// 2P���̃J���������̑傫��
#define CAMERAHINT_SIZE_4P	(D3DXVECTOR2(SCREEN_WIDTH * 0.06f, SCREEN_WIDTH * 0.03f))	// 4P���̃J���������̑傫��

#define CLOSE_FADE			(10)														// �N���[�Y�t�F�[�h����

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
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
		m_pFade[nCntPlayer] = NULL;

	for (int nCntLine = 0; nCntLine < MAX_LINE; nCntLine++)
	{// ���J�E���g
		m_pLine[nCntLine] = NULL;					// ��
	}

	for (int nCntDown = 0; nCntDown < COUNT_DOWN; nCntDown++)
	{// �_�E���J�E���g
		m_pCountDown[nCntDown] = NULL;				// �J�E���g�_�E��
	}
	m_pTutorial = NULL;								// �`���[�g���A��

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// �v���C���[�J�E���g
		m_pRanking[nCntPlayer] = NULL;				// �����L���O

		for (int nCntItem = 0; nCntItem < MAX_EGG; nCntItem++)
		{// �A�C�e���t���[���J�E���g
			m_pItemFrame[nCntPlayer][nCntItem] = NULL;		// �A�C�e���t���[���t���[��
			m_pItem[nCntPlayer][nCntItem] = NULL;			// �A�C�e���t���[��
			m_pItemClose[nCntPlayer][nCntItem] = NULL;		// �A�C�e���~
		}
		m_nCloseCounter[nCntPlayer] = 0;			// �~�J�E���g

		m_pGoul[nCntPlayer] = NULL;					// �����L���O
		m_pTime[nCntPlayer] = NULL;					// �^�C��
		m_pView[nCntPlayer] = NULL;					// �ϐ풆
		m_pCameraHint[nCntPlayer] = NULL;			// �J��������

		m_nGoulCounter[nCntPlayer] = 0;				// �S�[���J�E���g
		m_nCameraNumber[nCntPlayer] = nCntPlayer;	// �J�����ԍ�
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
		case TEXTURE_TUTORIAL:
			strcpy(cName, "data/TEXTURE/tutorial/Tutorial.jpg");
			break;
		case TEXTURE_GO:
			strcpy(cName, "data/TEXTURE/game/play/go.png");
			break;
		case TEXTURE_RANK:
			strcpy(cName, "data/TEXTURE/game/play/rank.png");
			break;
		case TEXTURE_ITEMFRAME:
			strcpy(cName, "data/TEXTURE/game/play/itemfram.png");
			break;
		case TEXTURE_ITEM:
			strcpy(cName, "data/TEXTURE/game/play/egg_chickslot.png");
			break;
		case TEXTURE_ITEMCLOSE:
			strcpy(cName, "data/TEXTURE/game/play/batu.png");
			break;
		case TEXTURE_GOUL:
			strcpy(cName, "data/TEXTURE/game/play/finish.png");
			break;
		case TEXTURE_VIEW:
			strcpy(cName, "data/TEXTURE/game/play/game00.png");
			break;
		case TEXTURE_CAMERAHINT:
			strcpy(cName, "data/TEXTURE/game/play/camera_arrows.png");
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
	// �擾
	bool bOnine = CTitle::GetOnline();
	int nClient = 0;

	if (bOnine == true)
		nClient = CClient::GetnID();

	// �v���C���[�ő吔�擾
	int nMaxPlayer = CGame::GetMaxPlayer();

	for (int nCntPlayer = 0; nCntPlayer < nMaxPlayer; nCntPlayer++)
	{// �v���C���[�J�E���g
	 // �t�F�[�h
		if (m_pFade[nCntPlayer] == NULL)
		{// NULL
			m_pFade[nCntPlayer] = new CScene2D(6, CScene::OBJTYPE_2DPOLYGON);
			m_pFade[nCntPlayer]->Init();
			// �e�r���[�|�[�g�̒��S�_����v�Z�i���S�_ -> �w��̈ʒu�ցj
			if (bOnine == true)
			{// �I�����C��
				m_pFade[nCntPlayer]->SetPosSize(
					D3DXVECTOR3(
						SCREEN_WIDTH * 0.5f,
						SCREEN_HEIGHT * 0.5f,
						0.0f),
					D3DXVECTOR2(
						SCREEN_WIDTH * 0.5f,
						SCREEN_HEIGHT * 0.5f));
			}
			else
			{// �I�����C������Ȃ�
				m_pFade[nCntPlayer]->SetPosSize(
					D3DXVECTOR3(
					((nMaxPlayer - 1) / 2 == 0 ? (SCREEN_WIDTH * 0.5f) : (SCREEN_WIDTH * 0.25f) + ((SCREEN_WIDTH * 0.5f) * (nCntPlayer % 2))),
						((nMaxPlayer - 1) == 0 ? (SCREEN_HEIGHT * 0.5f) : (SCREEN_HEIGHT * 0.25f) + ((SCREEN_HEIGHT * 0.5f) * (((nMaxPlayer - 1) / 2 == 0 ? nCntPlayer : nCntPlayer / 2)))),
						0.0f),
					D3DXVECTOR2(
					((SCREEN_WIDTH * ((nMaxPlayer - 1) / 2 == 0 ? 0.5f : 0.25f))),
						((SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? 0.5f : 0.25f)))));
			}

			m_pFade[nCntPlayer]->SetColor(&D3DXCOLOR(((nCntPlayer) % 2 == 0 ? 1.0f : 0.0f), ((nCntPlayer) / 2 == 1 ? 1.0f : 0.0f), ((nCntPlayer) == 1 ? 1.0f : 0.0f), 0.0f));
		}
	}

	if (bOnine == false)
	{// �I�����C������Ȃ�
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
	}

	for (int nCntDown = 0; nCntDown < COUNT_DOWN; nCntDown++)
	{// �_�E���J�E���g
		if (m_pCountDown[nCntDown] == NULL)
		{// NULL
			m_pCountDown[nCntDown] = new CScene2D(7, CScene::OBJTYPE_2DPOLYGON);
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

	if (m_pTutorial == NULL)
	{// NULL
		m_pTutorial = new CScene2D(7, CScene::OBJTYPE_2DPOLYGON);
		m_pTutorial->Init();
		if (bOnine == true)
		{// �I�����C��
			m_pTutorial->SetPosSize(
			D3DXVECTOR3(
				(SCREEN_WIDTH * 0.5f),
				(TUTORIAL_SIZE.y),
				0.0f),
				TUTORIAL_SIZE);
		}
		else
		{// �I�����C������Ȃ�
			m_pTutorial->SetPosSize(
				D3DXVECTOR3(
				(SCREEN_WIDTH * 0.5f),
					((nMaxPlayer - 1) == 0 ? (TUTORIAL_SIZE.y) : (SCREEN_HEIGHT * 0.5f)),
					0.0f),
				TUTORIAL_SIZE);
		}
		m_pTutorial->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f));
		m_pTutorial->BindTexture(m_pTexture[(TEXTURE)(TEXTURE_TUTORIAL)]);
		m_pTutorial->SetTexture(2, 3, 1, 1);
	}

	for (int nCntPlayer = 0; nCntPlayer < nMaxPlayer; nCntPlayer++)
	{// �v���C���[�J�E���g
	 // �����L���O
		if (m_pRanking[nCntPlayer] == NULL)
		{// NULL
			m_pRanking[nCntPlayer] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
			m_pRanking[nCntPlayer]->Init();
			// �e�r���[�|�[�g�̒��S�_����v�Z�i���S�_ -> �w��̈ʒu�ցj
			if (bOnine == true)
			{// �I�����C��
				m_pRanking[nCntPlayer]->SetPosSize(
					D3DXVECTOR3(
						(SCREEN_WIDTH * 0.5f) + (((SCREEN_WIDTH * 0.5f) - (SCREEN_HEIGHT * RANKING_SIZE_1P_X)) * -1.0f),
						(SCREEN_HEIGHT * 0.5f) + ((SCREEN_HEIGHT * 0.5f) - (SCREEN_HEIGHT * RANKING_SIZE_1P_Y)),
						0.0f),
					D3DXVECTOR2(
						SCREEN_HEIGHT * RANKING_SIZE_1P_X,
						SCREEN_HEIGHT * RANKING_SIZE_1P_Y * 0.8f));
			}
			else
			{// �I�����C������Ȃ�
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
			}
			m_pRanking[nCntPlayer]->BindTexture(m_pTexture[TEXTURE_RANK]);
			m_pRanking[nCntPlayer]->SetTexture(nCntPlayer, 8, 1, 1);

			if (bOnine == true && nClient != nCntPlayer)
				m_pRanking[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}

		// �A�C�e��
		for (int nCntItem = 0; nCntItem < MAX_EGG; nCntItem++)
		{// �A�C�e���J�E���g
			// �A�C�e���t���[��
			if (m_pItemFrame[nCntPlayer][nCntItem] == NULL)
			{// NULL
				m_pItemFrame[nCntPlayer][nCntItem] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
				m_pItemFrame[nCntPlayer][nCntItem]->Init();
				// �e�r���[�|�[�g�̒��S�_����v�Z�i���S�_ -> �w��̈ʒu�ցj
				if (bOnine == true)
				{// �I�����C��
					m_pItemFrame[nCntPlayer][nCntItem]->SetPosSize(
						D3DXVECTOR3(
						(SCREEN_WIDTH * 0.5f) + (((SCREEN_WIDTH * 0.5f) - (SCREEN_HEIGHT * ITEM_SIZE_1P)) * -1.0f),
							(SCREEN_HEIGHT * 0.5f) - ((SCREEN_HEIGHT * 0.5f) - (SCREEN_HEIGHT * ITEM_SIZE_1P)) + (((SCREEN_HEIGHT * ITEM_SIZE_1P) * 2.1f) * nCntItem),
							0.0f),
						D3DXVECTOR2(
							SCREEN_HEIGHT * ITEM_SIZE_1P,
							SCREEN_HEIGHT * ITEM_SIZE_1P));
					m_pItemFrame[nCntPlayer][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
				}
				else
				{// �I�����C������Ȃ�
					m_pItemFrame[nCntPlayer][nCntItem]->SetPosSize(
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
					m_pItemFrame[nCntPlayer][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
				}
				m_pItemFrame[nCntPlayer][nCntItem]->BindTexture(m_pTexture[TEXTURE_ITEMFRAME]);
				m_pItemFrame[nCntPlayer][nCntItem]->SetTexture(3, 4, 1, 1);

				if (bOnine == true && nClient != nCntPlayer)
					m_pItemFrame[nCntPlayer][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			}
			// �A�C�e��
			if (m_pItem[nCntPlayer][nCntItem] == NULL)
			{// NULL
				m_pItem[nCntPlayer][nCntItem] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
				m_pItem[nCntPlayer][nCntItem]->Init();
				// �e�r���[�|�[�g�̒��S�_����v�Z�i���S�_ -> �w��̈ʒu�ցj
				if (bOnine == true)
				{// �I�����C��
					m_pItem[nCntPlayer][nCntItem]->SetPosSize(
						D3DXVECTOR3(
						(SCREEN_WIDTH * 0.5f) + (((SCREEN_WIDTH * 0.5f) - (SCREEN_HEIGHT * ITEM_SIZE_1P)) * -1.0f),
							(SCREEN_HEIGHT * 0.5f) - ((SCREEN_HEIGHT * 0.5f) - (SCREEN_HEIGHT * ITEM_SIZE_1P)) + (((SCREEN_HEIGHT * ITEM_SIZE_1P) * 2.1f) * nCntItem),
							0.0f),
						D3DXVECTOR2(
							SCREEN_HEIGHT * ITEM_SIZE_1P,
							SCREEN_HEIGHT * ITEM_SIZE_1P));
					m_pItem[nCntPlayer][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
				}
				else
				{// �I�����C������Ȃ�
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
					m_pItem[nCntPlayer][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
				}
				m_pItem[nCntPlayer][nCntItem]->BindTexture(m_pTexture[TEXTURE_ITEM]);

				if (bOnine == true && nClient != nCntPlayer)
					m_pItem[nCntPlayer][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			}
			// �A�C�e���~
			if (m_pItemClose[nCntPlayer][nCntItem] == NULL)
			{// NULL
				m_pItemClose[nCntPlayer][nCntItem] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
				m_pItemClose[nCntPlayer][nCntItem]->Init();
				// �e�r���[�|�[�g�̒��S�_����v�Z�i���S�_ -> �w��̈ʒu�ցj
				if (bOnine == true)
				{// �I�����C��
					m_pItemClose[nCntPlayer][nCntItem]->SetPosSize(
						D3DXVECTOR3(
						(SCREEN_WIDTH * 0.5f) + (((SCREEN_WIDTH * 0.5f) - (SCREEN_HEIGHT * ITEM_SIZE_1P)) * -1.0f),
							(SCREEN_HEIGHT * 0.5f) - ((SCREEN_HEIGHT * 0.5f) - (SCREEN_HEIGHT * ITEM_SIZE_1P)) + (((SCREEN_HEIGHT * ITEM_SIZE_1P) * 2.1f) * nCntItem),
							0.0f),
						D3DXVECTOR2(
							SCREEN_HEIGHT * ITEM_SIZE_1P,
							SCREEN_HEIGHT * ITEM_SIZE_1P));
					m_pItemClose[nCntPlayer][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
				}
				else
				{// �I�����C������Ȃ�
					m_pItemClose[nCntPlayer][nCntItem]->SetPosSize(
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
					m_pItemClose[nCntPlayer][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
				}
				m_pItemClose[nCntPlayer][nCntItem]->BindTexture(m_pTexture[TEXTURE_ITEMCLOSE]);

				if (bOnine == true && nClient != nCntPlayer)
					m_pItemClose[nCntPlayer][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			}
		}

		if (m_pGoul[nCntPlayer] == NULL)
		{// NULL�ȊO
			m_pGoul[nCntPlayer] = new CScene2D(6, CScene::OBJTYPE_2DPOLYGON);
			m_pGoul[nCntPlayer]->Init();
			if (bOnine == true)
			{// �I�����C��
				m_pGoul[nCntPlayer]->SetPosSize(
					D3DXVECTOR3(
					(SCREEN_WIDTH * 0.5f),
						(SCREEN_HEIGHT * 0.5f),
						0.0f),
					GOUL_SIZE_2P);
			}
			else
			{// �I�����C������Ȃ�
				m_pGoul[nCntPlayer]->SetPosSize(
					D3DXVECTOR3(
					(SCREEN_WIDTH * ((nMaxPlayer - 1) / 2 == 0 ? 0.5f : (nCntPlayer % 2 == 0 ? 0.25f : 0.75f))),
						(SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? 0.5f : ((nMaxPlayer - 1) / 2 == 0 ? (nCntPlayer % 2 == 0 ? 0.25f : 0.75f) : (nCntPlayer / 2 == 0 ? 0.25f : 0.75f)))),
						0.0f),
						((nMaxPlayer - 1) == 0 ? GOUL_SIZE_2P : GOUL_SIZE_4P));
			}
			m_pGoul[nCntPlayer]->BindTexture(m_pTexture[TEXTURE_GOUL]);
			//m_pGoul[nCntPlayer]->SetTexture(nCntDown, 3, 1, 1);
			m_pGoul[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}

		// �^�C��
		if (m_pTime[nCntPlayer] == NULL)
		{// NULL
			D3DXVECTOR3 pos, size;

			if (bOnine == true)
			{// �I�����C��
				pos = D3DXVECTOR3(
					(SCREEN_WIDTH * 0.5f) + (((SCREEN_WIDTH * 0.5f) - (SCREEN_HEIGHT * RANKING_SIZE_1P_X)) * -1.0f) + (((SCREEN_HEIGHT * 0.02f) * 2.0f) * 10.0f),
					(SCREEN_HEIGHT * 0.5f) + ((SCREEN_HEIGHT * 0.5f) - (SCREEN_HEIGHT * RANKING_SIZE_1P_Y)) + (SCREEN_HEIGHT * 0.04f),
					0.0f);

				size = D3DXVECTOR3(
					(SCREEN_HEIGHT * 0.02f),
					(SCREEN_HEIGHT * 0.04f),
					0.0f);
			}
			else
			{// �I�����C������Ȃ�
				pos = D3DXVECTOR3(
					(SCREEN_WIDTH * ((nMaxPlayer - 1) / 2 == 0 ? 0.5f : (nCntPlayer % 2 == 0 ? 0.25f : 0.75f))) + (((SCREEN_WIDTH * ((nMaxPlayer - 1) / 2 == 0 ? 0.5f : 0.25f)) -
					(SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? RANKING_SIZE_1P_X : RANKING_SIZE_4P_X))) * ((nMaxPlayer - 1) / 2 == 0 ? -1.0f : (nCntPlayer % 2 == 0 ? -1.0f : 1.0f)))
					+ ((2 < (nMaxPlayer) && nCntPlayer % 2 == 1) ? -(((SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? 0.02f : 0.01f)) * 2.0f) * 4.0f) : (((SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? 0.02f : 0.01f)) * 2.0f) * 10.0f)),
					(SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? 0.5f : ((nMaxPlayer - 1) / 2 == 0 ? (nCntPlayer % 2 == 0 ? 0.25f : 0.75f) : (nCntPlayer / 2 == 0 ? 0.25f : 0.75f)))) + ((SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? 0.5f : 0.25f)) -
					(SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? RANKING_SIZE_1P_Y : RANKING_SIZE_4P_Y)))
					+ (SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? 0.04f : 0.02f)),
					0.0f);

				size = D3DXVECTOR3(
					SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? 0.02f : 0.01f),
					SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? 0.04f : 0.02f),
					0.0f);
			}

			m_pTime[nCntPlayer] = CTime::Create(pos, size);
			m_pTime[nCntPlayer]->TexTime(0, true);

			if (bOnine == true && nClient != nCntPlayer)
				m_pTime[nCntPlayer]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}

		if (m_pView[nCntPlayer] == NULL)
		{// NULL�ȊO
			m_pView[nCntPlayer] = new CScene2D(6, CScene::OBJTYPE_2DPOLYGON);
			m_pView[nCntPlayer]->Init();
			if (bOnine == true)
			{// �I�����C��
				m_pView[nCntPlayer]->SetPosSize(
					D3DXVECTOR3(
					(SCREEN_WIDTH * 0.5f),
						(SCREEN_HEIGHT * 0.5f) - (SCREEN_HEIGHT * 0.25f),
						0.0f),
					VIEW_SIZE_2P);
			}
			else
			{// �I�����C������Ȃ�
				m_pView[nCntPlayer]->SetPosSize(
					D3DXVECTOR3(
					(SCREEN_WIDTH * ((nMaxPlayer - 1) / 2 == 0 ? 0.5f : (nCntPlayer % 2 == 0 ? 0.25f : 0.75f))),
						(SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? 0.5f : ((nMaxPlayer - 1) / 2 == 0 ? (nCntPlayer % 2 == 0 ? 0.25f : 0.75f) : (nCntPlayer / 2 == 0 ? 0.25f : 0.75f)))) -
						(SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? 0.25f : 0.125f)),
						0.0f),
						((nMaxPlayer - 1) == 0 ? VIEW_SIZE_2P : VIEW_SIZE_4P));
			}
			m_pView[nCntPlayer]->BindTexture(m_pTexture[TEXTURE_VIEW]);
			m_pView[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}

		if (m_pCameraHint[nCntPlayer] == NULL)
		{// NULL�ȊO
			m_pCameraHint[nCntPlayer] = new CScene2D(6, CScene::OBJTYPE_2DPOLYGON);
			m_pCameraHint[nCntPlayer]->Init();
			if (bOnine == true)
			{// �I�����C��
				m_pCameraHint[nCntPlayer]->SetPosSize(
					D3DXVECTOR3(
					(SCREEN_WIDTH * 0.5f),
						(SCREEN_HEIGHT * 0.5f) + (SCREEN_HEIGHT * 0.25f),
						0.0f),
					CAMERAHINT_SIZE_2P);
			}
			else
			{// �I�����C������Ȃ�
				m_pCameraHint[nCntPlayer]->SetPosSize(
					D3DXVECTOR3(
					(SCREEN_WIDTH * ((nMaxPlayer - 1) / 2 == 0 ? 0.5f : (nCntPlayer % 2 == 0 ? 0.25f : 0.75f))),
						(SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? 0.5f : ((nMaxPlayer - 1) / 2 == 0 ? (nCntPlayer % 2 == 0 ? 0.25f : 0.75f) : (nCntPlayer / 2 == 0 ? 0.25f : 0.75f)))) +
						((SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? 0.5f : 0.25f)) - ((nMaxPlayer - 1) == 0 ? CAMERAHINT_SIZE_2P.y : CAMERAHINT_SIZE_4P.y)),
						0.0f),
						((nMaxPlayer - 1) == 0 ? CAMERAHINT_SIZE_2P : CAMERAHINT_SIZE_4P));
			}
			m_pCameraHint[nCntPlayer]->BindTexture(m_pTexture[TEXTURE_CAMERAHINT]);
			m_pCameraHint[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
	}

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CGamePlay::Uninit(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// �v���C���[�J�E���g
		if (m_pFade[nCntPlayer] != NULL)
		{// NULL�ȊO
			m_pFade[nCntPlayer]->Uninit();
			m_pFade[nCntPlayer] = NULL;
		}
	}

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
	if (m_pTutorial != NULL)
	{// NULL�ȊO
		m_pTutorial->Uninit();
		m_pTutorial = NULL;
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
			if (m_pItemFrame[nCntPlayer][nCntItem] != NULL)
			{// NULL�ȊO
				m_pItemFrame[nCntPlayer][nCntItem]->Uninit();
				m_pItemFrame[nCntPlayer][nCntItem] = NULL;
			}
			if (m_pItem[nCntPlayer][nCntItem] != NULL)
			{// NULL�ȊO
				m_pItem[nCntPlayer][nCntItem]->Uninit();
				m_pItem[nCntPlayer][nCntItem] = NULL;
			}
			if (m_pItemClose[nCntPlayer][nCntItem] != NULL)
			{// NULL�ȊO
				m_pItemClose[nCntPlayer][nCntItem]->Uninit();
				m_pItemClose[nCntPlayer][nCntItem] = NULL;
			}
		}

		if (m_pGoul[nCntPlayer] != NULL)
		{// NULL�ȊO
			m_pGoul[nCntPlayer]->Uninit();
			m_pGoul[nCntPlayer] = NULL;
		}

		if (m_pTime[nCntPlayer] != NULL)
		{// NULL�ȊO
			m_pTime[nCntPlayer]->Uninit();
			m_pTime[nCntPlayer] = NULL;
		}

		if (m_pView[nCntPlayer] != NULL)
		{// NULL�ȊO
			m_pView[nCntPlayer]->Uninit();
			m_pView[nCntPlayer] = NULL;
		}

		if (m_pCameraHint[nCntPlayer] != NULL)
		{// NULL�ȊO
			m_pCameraHint[nCntPlayer]->Uninit();
			m_pCameraHint[nCntPlayer] = NULL;
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
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputJoyPad_0 * pXpad = CManager::GetInputJoyPad0(0);		//�W���C�p�b�g�̎擾

																// �v���C���[
	CPlayer **pPlayer = NULL;
	switch (CManager::GetMode())
	{
	case CManager::MODE_TITLE:
		pPlayer = CTitle::GetPlayer();
		break;
	case CManager::MODE_GAME:
		pPlayer = CGame::GetPlayer();
		break;
	}
	// �擾
	bool bOnine = CTitle::GetOnline();
	int nClient = 0;

	if (bOnine == true)
		nClient = CClient::GetnID();

	// �v���C���[�ő吔�擾
	int nMaxPlayer = CGame::GetMaxPlayer();
	// �R���g���[���[�ԍ��擾
	int *pnControllerNum = CGame::GetControllerNum();

	CGameCamera *pCamera = NULL;

	// �����L���O
	int *pnRanking = CGame::GetRanking();
	// �Q�[���J�E���^�[
	int nGameCounter = CGame::GetGameCounter();
	int nCounter = nGameCounter - (START_SET_TIME - START_COUNT_TIME);
	bool *pbGoul = CGame::GetGoul();

	int *nTimer = CGame::GetTimer();

	CSound *pSound = CManager::GetSound();

	// �J�E���g�_�E��
	if ((START_SET_TIME - START_COUNT_TIME) <= nGameCounter && nGameCounter < START_SET_TIME)
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

		if (m_pTutorial != NULL)
		{// NULL�ȊO
			if (nCounter / 60 == 0)
			{
				m_pTutorial->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f - (0.8f * (float)((float)(nCounter) / (float)(60)))));
			}
		}
	}
	else
	{
		if (START_SET_TIME <= nGameCounter)
		{
			if (nGameCounter == START_SET_TIME)
			{
				for (int nCntDown = 0; nCntDown < COUNT_DOWN; nCntDown++)
				{// �_�E���J�E���g
					if (m_pCountDown[nCntDown] != NULL)
					{// NULL�ȊO
						m_pCountDown[nCntDown]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
					}
				}

				if (m_pCountDown[1] != NULL)
				{// NULL�ȊO
					m_pCountDown[1]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					m_pCountDown[1]->BindTexture(m_pTexture[TEXTURE_GO]);
					m_pCountDown[1]->SetTexture(0, 1, 1, 1);
					m_pCountDown[1]->SetPosSize(m_pCountDown[1]->GetPosition(), D3DXVECTOR2(0.0f, 0.0f));
				}
			}

			int nCount = nGameCounter - START_SET_TIME;
			if (nCount < 90)
			{
				if (nCount < 10)
				{
					if (m_pCountDown[1] != NULL)
						m_pCountDown[1]->SetPosSize(m_pCountDown[1]->GetPosition(),
						((GO_SIZE) * (float)((float)((nCount + 1)) / (float)10)));
				}
				else
				{
					if (60 <= nCount)
					{
						nCount = nCount - 60;
						if (nCount < 30)
						{
							if (m_pCountDown[1] != NULL)
								m_pCountDown[1]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - ((1.0f) * (float)((float)((nCount + 1)) / (float)30))));
						}
					}
				}
			}
		}
	}

	int nPlayerNum = 0;
	for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
	{// �v���C���[�J�E���g
		if (pPlayer[nCntMember] != NULL)
		{// NULL�ȊO
			if (pPlayer[nCntMember]->GetPlayerType() == CPlayer::PLAYERTYPE_PLAYER)
			{
				if (pbGoul[nPlayerNum] == false)
				{
					if (m_pFade[nPlayerNum] != NULL)
					{// NULL�ȊO
						if (pPlayer[nCntMember]->GetDrop() == true)
						{
							int nCounter = pPlayer[nCntMember]->GetDropCounter();
							float fcol_a = 1.0f;
							float fDiff = 1.0f;

							int nNum = (((nCounter + (nCounter < MAX_FALL_FADE + MAX_FALL_WAIT ? 0 : -MAX_FALL_WAIT)) % MAX_FALL_FADE) + 1);

							if (nCounter < MAX_FALL_FADE)
								fDiff = (float)((float)((nCounter % MAX_FALL_FADE)) / (float)MAX_FALL_FADE);
							else if (nCounter < MAX_FALL_FADE + MAX_FALL_WAIT)
								fDiff = 1.0f;
							else
								fDiff = 1.0f - (float)((float)(((nCounter - MAX_FALL_WAIT) % MAX_FALL_FADE)) / (float)MAX_FALL_FADE);

							fcol_a *= fDiff;

							m_pFade[nPlayerNum]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, fcol_a));

							if (bOnine == true && nClient != nPlayerNum)
								m_pFade[nPlayerNum]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
						}
						else
						{
							m_pFade[nPlayerNum]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
						}
					}

					for (int nCntItem = 0; nCntItem < MAX_EGG; nCntItem++)
					{// �A�C�e���J�E���g
						int nNum = pPlayer[nCntMember]->GetItemNum();
						int nType = pPlayer[nCntMember]->GetItemType(nCntItem);

						// �A�C�e���t���[��
						if (m_pItemFrame[nPlayerNum][nCntItem] != NULL)
						{// NULL�ȊO
							if ((bOnine == true && nClient == nPlayerNum) || bOnine == false)
							{// �I�����C���Ŏ����Ȃ�A�I�����C������Ȃ�
								if (nCntItem < nNum)
								{
									m_pItemFrame[nPlayerNum][nCntItem]->SetTexture((nType % 3), 4, 1, 1);
									m_pItemFrame[nPlayerNum][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
								}
								else
								{
									m_pItemFrame[nPlayerNum][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
									m_pItemFrame[nPlayerNum][nCntItem]->SetTexture(3, 4, 1, 1);
								}
							}
							else
							{
								m_pItemFrame[nPlayerNum][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
							}
						}

						// �A�C�e��
						if (m_pItem[nPlayerNum][nCntItem] != NULL)
						{// NULL�ȊO
							if ((bOnine == true && nClient == nPlayerNum) || bOnine == false)
							{// �I�����C���Ŏ����Ȃ�A�I�����C������Ȃ�
								if (nCntItem < nNum)
								{
									m_pItem[nPlayerNum][nCntItem]->SetTexture(((nType % 3) * 3) + (nType / 3), 3, 3, 1);
									m_pItem[nPlayerNum][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
								}
								else
								{
									m_pItem[nPlayerNum][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
								}
							}
							else
							{
								m_pItem[nPlayerNum][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
							}
						}

						// �A�C�e���~
						if (m_pItemClose[nPlayerNum][nCntItem] != NULL)
						{
							if (pPlayer[nCntMember]->GetDamage() == true)
							{	if (m_nCloseCounter[nPlayerNum] < CLOSE_FADE) m_nCloseCounter[nPlayerNum]++;	}
							else
							{	if (0 < m_nCloseCounter[nPlayerNum]) m_nCloseCounter[nPlayerNum]--;		}

							m_pItemClose[nPlayerNum][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)((float)(m_nCloseCounter[nPlayerNum]) / (float)(CLOSE_FADE))));

							if (bOnine == true && nClient != nPlayerNum)
								m_pItemClose[nPlayerNum][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
						}
					}
				}
				else
				{
					// �t�F�[�h�A�E�g
					for (int nCntItem = 0; nCntItem < MAX_EGG; nCntItem++)
					{// �A�C�e���J�E���g
						int nNum = pPlayer[nCntMember]->GetItemNum();

						if (60 <= m_nGoulCounter[nPlayerNum] && m_nGoulCounter[nPlayerNum] < 120)
						{
							int nFrame = m_nGoulCounter[nPlayerNum] - 60;

							// �A�C�e���t���[��
							if (m_pItemFrame[nPlayerNum][nCntItem] != NULL)
							{// NULL�ȊO
								int nCount = (nFrame % 20) + 1;
								if (nCntItem < nNum)
								{
									if (nFrame / 20 == 0)
										m_pItemFrame[nPlayerNum][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (float)((float)nCount / (float)20)));
								}
								else
								{
									m_pItemFrame[nPlayerNum][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
								}

								if (bOnine == true && nClient != nPlayerNum)
									m_pItemFrame[nPlayerNum][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

							}
							// �A�C�e��
							if (m_pItem[nPlayerNum][nCntItem] != NULL)
							{// NULL�ȊO
								int nCount = (nFrame % 20) + 1;
								if (nCntItem < nNum)
								{
									if (nFrame / 20 == 0)
										m_pItem[nPlayerNum][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (float)((float)nCount / (float)20)));
								}
								else
								{
									m_pItemFrame[nPlayerNum][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
								}

								if (bOnine == true && nClient != nPlayerNum)
									m_pItem[nPlayerNum][nCntItem]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
							}
						}
					}
				}

				// �^�C��
				if (m_pTime[nPlayerNum] != NULL)
				{// NULL�ȊO
					m_pTime[nPlayerNum]->TexTime(nTimer[nCntMember], true);

					if (pbGoul[nPlayerNum] == true)
						m_pTime[nPlayerNum]->Setcol(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
					else
						m_pTime[nPlayerNum]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

					if (bOnine == true && nClient != nPlayerNum)
						m_pTime[nPlayerNum]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
				}

				nPlayerNum++;
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
				if (bOnine == true)
				{// �I�����C��
					if (nClient == nCntPlayer)
						m_pGoul[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
				else
				{// �I�����C������Ȃ�
					m_pGoul[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
			}

			if (60 <= m_nGoulCounter[nCntPlayer] && m_nGoulCounter[nCntPlayer] < 120)
			{// �S�[����ړ��A�t�F�[�h�A�E�g
				int nFrame = m_nGoulCounter[nCntPlayer] - 60;

				if (m_pGoul[nCntPlayer] != NULL)
				{// NULL�ȊO
					if (bOnine == true && nClient == nCntPlayer)
					{// �I�����C��
						m_pGoul[nCntPlayer]->SetPosSize(
							D3DXVECTOR3(
								(SCREEN_WIDTH * 0.5f),
								(SCREEN_HEIGHT * 0.5f) -
								(((SCREEN_HEIGHT * 0.5f) - (GOUL_SIZE_2P.y * (1.0f - (0.5f * (float)((float)nFrame / (float)60))))) * (float)((float)nFrame / (float)60)),
								0.0f),
								GOUL_SIZE_2P * (1.0f - (0.5f * (float)((float)nFrame / (float)60))));
					}
					else
					{// �I�����C������Ȃ�
						m_pGoul[nCntPlayer]->SetPosSize(
							D3DXVECTOR3(
								(SCREEN_WIDTH * ((nMaxPlayer - 1) / 2 == 0 ? 0.5f : (nCntPlayer % 2 == 0 ? 0.25f : 0.75f))),
								(SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? 0.5f : ((nMaxPlayer - 1) / 2 == 0 ? (nCntPlayer % 2 == 0 ? 0.25f : 0.75f) : (nCntPlayer / 2 == 0 ? 0.25f : 0.75f)))) -
								((((nMaxPlayer - 1) == 0 ? (SCREEN_HEIGHT * 0.5f) : (SCREEN_HEIGHT * 0.25f)) - (((nMaxPlayer - 1) / 2 == 0 ? GOUL_SIZE_2P.y : GOUL_SIZE_4P.y) * (1.0f - (0.5f * (float)((float)nFrame / (float)60))))) * (float)((float)nFrame / (float)60)),
								0.0f),
								((nMaxPlayer - 1) / 2 == 0 ? GOUL_SIZE_2P : GOUL_SIZE_4P) * (1.0f - (0.5f * (float)((float)nFrame / (float)60))));
					}
				}

				if (m_pFade[nCntPlayer] != NULL)
				{// NULL�ȊO
					int nNum = (nFrame % 20) + 1;
					if (nFrame / 20 == 1)
						m_pFade[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					else
						m_pFade[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, (nFrame / 20 == 0 ? (float)((float)nNum / (float)20) : 1.0f - (float)((float)nNum / (float)20))));

					if (bOnine == true && nClient != nCntPlayer)
						m_pFade[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
				}

				if (m_pView[nCntPlayer] != NULL)
				{// NULL�ȊO
					int nNum = (nFrame % 20) + 1;
					if (40 <= nFrame)
						m_pView[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f * (float)((float)nNum / (float)20)));

					if (bOnine == true && nClient != nCntPlayer)
						m_pView[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
				}

				if (m_pCameraHint[nCntPlayer] != NULL)
				{// NULL�ȊO
					int nNum = (nFrame % 20) + 1;
					if (40 <= nFrame)
						m_pCameraHint[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)((float)nNum / (float)20)));

					if (bOnine == true && nClient != nCntPlayer)
						m_pCameraHint[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
				}
			}

			if (90 <= m_nGoulCounter[nCntPlayer])
			{// �t�F�[�h��
				pXpad = CManager::GetInputJoyPad0(pnControllerNum[nCntPlayer]);
				bool bEdit = false;

				if (CFade::GetFade() == CFade::FADE_NONE)
				{
					if (pXpad->GetTrigger(INPUT_LS_L) == true ||
						pXpad->GetTrigger(INPUT_LEFT) == true ||
						(bOnine == false && pCInputKeyBoard->GetKeyboardTrigger(DIK_A) == true) ||
						(bOnine == false && pCInputKeyBoard->GetKeyboardTrigger(DIK_LEFT) == true))
					{// ���L�[
						m_nCameraNumber[nCntPlayer] = ((m_nCameraNumber[nCntPlayer] + ((nMaxPlayer + 1) - 1)) % (nMaxPlayer + 1));
						bEdit = true;
					}
					else if (pXpad->GetTrigger(INPUT_LS_R) == true ||
						pXpad->GetTrigger(INPUT_RIGHT) == true ||
						(bOnine == false && pCInputKeyBoard->GetKeyboardTrigger(DIK_D) == true) ||
						(bOnine == false && pCInputKeyBoard->GetKeyboardTrigger(DIK_RIGHT) == true))
					{// �E�L�[
						m_nCameraNumber[nCntPlayer] = ((m_nCameraNumber[nCntPlayer] + (1)) % (nMaxPlayer + 1));
						bEdit = true;
					}
				}

				if (bEdit == true)
				{// �ύX����
				 // �J�����擾
					pCamera = CGame::GetGameCamera(nCntPlayer);

					if (pCamera != NULL)
					{// NULL�ȊO
						if (m_nCameraNumber[nCntPlayer] < nMaxPlayer)
						{// �v���C���[�J����
							pCamera->SetPlayer(pPlayer[m_nCameraNumber[nCntPlayer]]);
							pCamera->SetType(CGameCamera::CAMERA_PLAYER);
							pCamera->SetTypeReset();
						}
						else
						{// �P�ʃJ����
							pCamera->SetType(CGameCamera::CAMERA_GOUL);
							pCamera->SetTypeReset();
						}
					}
				}
			}

			m_nGoulCounter[nCntPlayer]++;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CGamePlay::Draw(void)
{

}