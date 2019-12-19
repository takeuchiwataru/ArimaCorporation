//=============================================================================
//
// ���U���gUI�̏��� [resultui.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS

#include "resultui.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "input.h"
#include "fade.h"
#include "time.h"

#include "number.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RANKING_TEXT	"data/TEXT/RANKING/score.bin"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	CResultUI::m_pTexture[CResultUI::TEXTURE_MAX] = { NULL };

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CResultUI::CResultUI()
{
	for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
	{
		m_pRankBG[nCntMember] = NULL;		// �����NBG
		m_pRank[nCntMember] = NULL;			// �����N
		m_pChar[nCntMember] = NULL;			// �L����
		m_pPlayer[nCntMember] = NULL;		// �v���C���[
		m_pTime[nCntMember] = NULL;			// �^�C��
	}
	m_pPress = NULL;						// �v���X
	m_pFade = NULL;							// �t�F�[�h

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{
		m_nCharacter[nCntRanking] = 0;
		m_nRanking[nCntRanking] = 0;
	}
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CResultUI::~CResultUI() {}

//=============================================================================
// ���[�h
//=============================================================================
HRESULT CResultUI::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{// �e�N�X�`���J�E���g
		char cName[128] = {};

		switch (nCntTex)
		{
		case TEXTURE_RANK:
			strcpy(cName, "data/TEXTURE/game/play/rank.png");
			break;
		case TEXTURE_CHAR:
			strcpy(cName, "data/TEXTURE/game/play/icon.png");
			break;
		case TEXTURE_ICON:
			strcpy(cName, "data/TEXTURE/game/charselect/icon.png");
			break;
		case TEXTURE_PRESS:
			strcpy(cName, "data/TEXTURE/Title/UI/PressButton.png");
			break;
		}

		// �e�N�X�`���̐���
		if (m_pTexture[nCntTex] == NULL)
			D3DXCreateTextureFromFile(pDevice, cName, &m_pTexture[nCntTex]);
	}

	CTime::Load();
	CNumber::Load();

	return S_OK;
}

//=============================================================================
// �A�����[�h
//=============================================================================
void CResultUI::Unload(void)
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

	CTime::Unload();
	CNumber::Unload();
}

//=============================================================================
// �N���G�C�g
//=============================================================================
CResultUI *CResultUI::Create(void)
{
	CResultUI *pResultUI = NULL;

	// �������m��
	if (pResultUI == NULL)
		pResultUI = new CResultUI;

	if (pResultUI != NULL)
	{// NULL�ȊO
		pResultUI->Init();	// ����������
	}

	return pResultUI;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CResultUI::Init()
{
	CManager::MODE mode = CManager::GetMode();
	int *pnCharSelectNum = NULL;
	int nMaxPlayer = 0;
	int *pnRankingSort = NULL;
	int *nTimer = NULL;

	LoadRanking();
	if (mode == CManager::MODE_RESULT)
	{
		//for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
		//	m_nRanking[nCntRanking] = MAX_TIME;
		SortRanking();
		SaveRanking();
	}

	if (mode == CManager::MODE_RESULT)
	{
		pnCharSelectNum = CGame::GetCharSelectNum();
		nMaxPlayer = CGame::GetMaxPlayer();
		pnRankingSort = CGame::GetRankingSort();
		nTimer = CGame::GetTimer();
	}

	for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
	{// �����o�[�J�E���g
		if (mode == CManager::MODE_RESULT || ((mode == CManager::MODE_TITLE || (mode == CManager::MODE_GAME && CGame::GetGameMode() == CGame::GAMEMODE_COURSE_VIEW)) && nCntMember < MAX_RANKING))
		{
			if (mode == CManager::MODE_RESULT || ((mode == CManager::MODE_TITLE || (mode == CManager::MODE_GAME && CGame::GetGameMode() == CGame::GAMEMODE_COURSE_VIEW)) && nCntMember == 0))
			{
				// �����NBG
				if (m_pRankBG[nCntMember] == NULL)
				{// NULL
					m_pRankBG[nCntMember] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
					m_pRankBG[nCntMember]->Init();
					if (mode != CManager::MODE_RESULT)
					{
						m_pRankBG[nCntMember]->SetPosSize(
							D3DXVECTOR3
							(
							(SCREEN_WIDTH * 0.5f) + (SCREEN_WIDTH),
								(SCREEN_HEIGHT * 0.05f),
								0.0f
							),
							D3DXVECTOR2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.05f));
					}
					else
					{
						m_pRankBG[nCntMember]->SetPosSize(
							D3DXVECTOR3
							(
							(SCREEN_WIDTH * 0.5f) - (SCREEN_WIDTH * 0.17f) - (SCREEN_WIDTH),
								(SCREEN_HEIGHT * 0.12f) + (((SCREEN_HEIGHT * 0.05f) * 2.2f) * nCntMember),
								0.0f
							),
							D3DXVECTOR2(SCREEN_WIDTH * 0.29f, SCREEN_HEIGHT * 0.05f));
					}
					m_pRankBG[nCntMember]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
				}
			}

			// �����N
			if (m_pRank[nCntMember] == NULL)
			{// NULL
				m_pRank[nCntMember] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
				m_pRank[nCntMember]->Init();
				if (mode != CManager::MODE_RESULT)
				{
					m_pRank[nCntMember]->SetPosSize(
						D3DXVECTOR3
						(
						(SCREEN_HEIGHT * 0.07f) + ((SCREEN_HEIGHT * 0.6f) * nCntMember) + (SCREEN_WIDTH),
							(SCREEN_HEIGHT * 0.05f),
							0.0f
						),
						D3DXVECTOR2(SCREEN_HEIGHT * 0.07f, SCREEN_HEIGHT * 0.04f));
				}
				else
				{
					m_pRank[nCntMember]->SetPosSize(
						D3DXVECTOR3
						(
						(SCREEN_WIDTH * 0.5f) - (SCREEN_WIDTH * 0.4f) - (SCREEN_WIDTH),
							(SCREEN_HEIGHT * 0.12f) + (((SCREEN_HEIGHT * 0.05f) * 2.2f) * nCntMember),
							0.0f
						),
						D3DXVECTOR2(SCREEN_HEIGHT * 0.08f, SCREEN_HEIGHT * 0.05f));
				}
				m_pRank[nCntMember]->BindTexture(m_pTexture[TEXTURE_RANK]);
				m_pRank[nCntMember]->SetTexture(nCntMember, 8, 1, 1);
			}

			// �L����
			if (m_pChar[nCntMember] == NULL)
			{// NULL
				m_pChar[nCntMember] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
				m_pChar[nCntMember]->Init();
				if (mode != CManager::MODE_RESULT)
				{
					m_pChar[nCntMember]->SetPosSize(
						D3DXVECTOR3
						(
						(SCREEN_HEIGHT * 0.07f) + (SCREEN_HEIGHT * 0.10f) + ((SCREEN_HEIGHT * 0.6f) * nCntMember) + (SCREEN_WIDTH),
							(SCREEN_HEIGHT * 0.05f),
							0.0f
						),
						D3DXVECTOR2(SCREEN_HEIGHT * 0.04f, SCREEN_HEIGHT * 0.04f));
				}
				else
				{
					m_pChar[nCntMember]->SetPosSize(
						D3DXVECTOR3
						(
						(SCREEN_WIDTH * 0.5f) - (SCREEN_WIDTH * 0.3f) - (SCREEN_WIDTH),
							(SCREEN_HEIGHT * 0.12f) + (((SCREEN_HEIGHT * 0.05f) * 2.2f) * nCntMember),
							0.0f
						),
						D3DXVECTOR2(SCREEN_HEIGHT * 0.05f, SCREEN_HEIGHT * 0.05f));
				}
				m_pChar[nCntMember]->BindTexture(m_pTexture[TEXTURE_CHAR]);

				if (mode != CManager::MODE_RESULT)
					m_pChar[nCntMember]->SetTexture(m_nCharacter[nCntMember], 8, 1, 1);
				else
					m_pChar[nCntMember]->SetTexture(pnCharSelectNum[pnRankingSort[nCntMember]], 8, 1, 1);
			}

			if (mode == CManager::MODE_RESULT)
			{
				// �v���C���[
				if (m_pPlayer[nCntMember] == NULL)
				{// NULL
					m_pPlayer[nCntMember] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
					m_pPlayer[nCntMember]->Init();
					m_pPlayer[nCntMember]->SetPosSize(
						D3DXVECTOR3
						(
						(SCREEN_WIDTH * 0.5f) - (SCREEN_WIDTH * 0.21f) - (SCREEN_WIDTH),
							(SCREEN_HEIGHT * 0.12f) - (SCREEN_HEIGHT * 0.012f) + (((SCREEN_HEIGHT * 0.05f) * 2.2f) * nCntMember),
							0.0f
						),
						D3DXVECTOR2(SCREEN_HEIGHT * 0.08f, SCREEN_HEIGHT * 0.05f));
					m_pPlayer[nCntMember]->BindTexture(m_pTexture[TEXTURE_ICON]);
					m_pPlayer[nCntMember]->SetTexture((pnRankingSort[nCntMember] < nMaxPlayer ? pnRankingSort[nCntMember] : 4), 5, 2, 1);
				}
			}

			// �^�C��
			if (m_pTime[nCntMember] == NULL)
			{// NULL
				if (mode != CManager::MODE_RESULT)
				{
					m_pTime[nCntMember] = CTime::Create(
						D3DXVECTOR3
						(
						(SCREEN_HEIGHT * 0.07f) + (SCREEN_HEIGHT * 0.45f) + ((SCREEN_HEIGHT * 0.6f) * nCntMember) + (SCREEN_WIDTH),
							(SCREEN_HEIGHT * 0.05f),
							0.0f
						),
						D3DXVECTOR3((SCREEN_HEIGHT * 0.02f), (SCREEN_HEIGHT * 0.04f), 0.0f));
				}
				else
				{
					m_pTime[nCntMember] = CTime::Create(
						D3DXVECTOR3
						(
						(SCREEN_WIDTH * 0.5f) + (SCREEN_WIDTH * 0.08f) - (SCREEN_WIDTH),
							(SCREEN_HEIGHT * 0.12f) + (((SCREEN_HEIGHT * 0.05f) * 2.2f) * nCntMember),
							0.0f
						),
						D3DXVECTOR3((SCREEN_HEIGHT * 0.03f), (SCREEN_HEIGHT * 0.05f), 0.0f));
				}

				if (mode != CManager::MODE_RESULT)
				{
					m_pTime[nCntMember]->TexTime(m_nRanking[nCntMember], true);
				}
				else
				{
					m_pTime[nCntMember]->TexTime(nTimer[pnRankingSort[nCntMember]], true);

					if (pnRankingSort[nCntMember] < nMaxPlayer)
						m_pTime[nCntMember]->Setcol(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
				}
			}
		}
	}

	if (mode == CManager::MODE_RESULT)
	{
		// �v���X
		if (m_pPress == NULL)
		{// NULL
			m_pPress = new CScene2D(6, CScene::OBJTYPE_2DPOLYGON);
			m_pPress->Init();
			m_pPress->SetPosSize(
				D3DXVECTOR3
				(
				(SCREEN_WIDTH * 0.5f) + (SCREEN_WIDTH * 0.31f),
					(SCREEN_HEIGHT * 0.12f) + (SCREEN_HEIGHT * 0.8f),
					0.0f
				),
				D3DXVECTOR2(SCREEN_HEIGHT * 0.2f, SCREEN_HEIGHT * 0.05f));
			m_pPress->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_pPress->BindTexture(m_pTexture[TEXTURE_PRESS]);
		}

		// �t�F�[�h
		if (m_pFade == NULL)
		{// NULL
			m_pFade = new CScene2D(6, CScene::OBJTYPE_2DPOLYGON);
			m_pFade->Init();
			m_pFade->SetPosSize(
				D3DXVECTOR3
				(
				(SCREEN_WIDTH * 0.5f),
					(SCREEN_HEIGHT * 0.5f),
					0.0f
				),
				D3DXVECTOR2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f));
			m_pFade->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
	}

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CResultUI::Uninit(void)
{
	for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
	{// �����o�[�J�E���g
	 // �����NBG
		if (m_pRankBG[nCntMember] != NULL)
		{// NULL�ȊO
			m_pRankBG[nCntMember]->Uninit();
			m_pRankBG[nCntMember] = NULL;
		}

		// �����N
		if (m_pRank[nCntMember] != NULL)
		{// NULL�ȊO
			m_pRank[nCntMember]->Uninit();
			m_pRank[nCntMember] = NULL;
		}

		// �L����
		if (m_pChar[nCntMember] != NULL)
		{// NULL�ȊO
			m_pChar[nCntMember]->Uninit();
			m_pChar[nCntMember] = NULL;
		}

		// �v���C���[
		if (m_pPlayer[nCntMember] != NULL)
		{// NULL�ȊO
			m_pPlayer[nCntMember]->Uninit();
			m_pPlayer[nCntMember] = NULL;
		}

		// �^�C��
		if (m_pTime[nCntMember] != NULL)
		{// NULL�ȊO
			m_pTime[nCntMember]->Uninit();
			m_pTime[nCntMember] = NULL;
		}
	}

	// �v���X
	if (m_pPress != NULL)
	{// NULL�ȊO
		m_pPress->Uninit();
		m_pPress = NULL;
	}

	// �t�F�[�h
	if (m_pFade != NULL)
	{// NULL�ȊO
		m_pFade->Uninit();
		m_pFade = NULL;
	}

	//���g�̍폜
	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CResultUI::Update(void)
{
	switch (CManager::GetMode())
	{
	case CManager::MODE_TITLE:
		Title();
		break;
	case CManager::MODE_GAME:
		Game();
		break;
	default:
		Result();
		break;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CResultUI::Draw(void)
{

}

//=============================================================================
// �^�C�g������
//=============================================================================
void CResultUI::Title(void)
{
	int nCounter = CTitle::GetCounter() % RESET_TITLEMODE;

	if ((HOME_TITLEMODE - RANKING_TITLEMODE) <= nCounter && nCounter < HOME_TITLEMODE)
	{
		int nFrame = nCounter - (HOME_TITLEMODE - RANKING_TITLEMODE);

		for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
		{// �����o�[�J�E���g
			if (nFrame < 30 || (RANKING_TITLEMODE - 30) <= nFrame)
			{
				float fDiff = 0.0f;

				if (nFrame < 30)
				{
					fDiff = (SCREEN_WIDTH) * (1.0f - (float)((float)((nFrame % 30) + 1) / (float)30));
				}
				else
				{
					fDiff = (-SCREEN_WIDTH) * (float)((float)((nFrame % 30) + 1) / (float)30);
				}

				// �����NBG
				if (m_pRankBG[nCntMember] != NULL)
				{// NULL�ȊO
					m_pRankBG[nCntMember]->SetPosSize(
						D3DXVECTOR3
						(
						(SCREEN_WIDTH * 0.5f) + (fDiff),
							(SCREEN_HEIGHT * 0.05f),
							0.0f
						),
						D3DXVECTOR2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.05f));
				}

				// �����N
				if (m_pRank[nCntMember] != NULL)
				{// NULL�ȊO
					m_pRank[nCntMember]->SetPosSize(
						D3DXVECTOR3
						(
						(SCREEN_HEIGHT * 0.07f) + ((SCREEN_HEIGHT * 0.6f) * nCntMember) + (fDiff),
							(SCREEN_HEIGHT * 0.05f),
							0.0f
						),
						D3DXVECTOR2(SCREEN_HEIGHT * 0.07f, SCREEN_HEIGHT * 0.04f));
				}

				// �L����
				if (m_pChar[nCntMember] != NULL)
				{// NULL�ȊO
					m_pChar[nCntMember]->SetPosSize(
						D3DXVECTOR3
						(
						(SCREEN_HEIGHT * 0.07f) + (SCREEN_HEIGHT * 0.10f) + ((SCREEN_HEIGHT * 0.6f) * nCntMember) + (fDiff),
							(SCREEN_HEIGHT * 0.05f),
							0.0f
						),
						D3DXVECTOR2(SCREEN_HEIGHT * 0.04f, SCREEN_HEIGHT * 0.04f));
				}

				// �^�C��
				if (m_pTime[nCntMember] != NULL)
				{// NULL�ȊO
					m_pTime[nCntMember]->Setpos(
						D3DXVECTOR3
						(
						(SCREEN_HEIGHT * 0.07f) + (SCREEN_HEIGHT * 0.45f) + ((SCREEN_HEIGHT * 0.6f) * nCntMember) + (fDiff),
							(SCREEN_HEIGHT * 0.05f),
							0.0f
						));
				}
			}
		}
	}
}

//=============================================================================
// �Q�[������
//=============================================================================
void CResultUI::Game(void)
{
	for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
	{// �����o�[�J�E���g
		float fDiff = 0.0f;// (-SCREEN_WIDTH);

		// �����NBG
		if (m_pRankBG[nCntMember] != NULL)
		{// NULL�ȊO
			m_pRankBG[nCntMember]->SetPosSize(
				D3DXVECTOR3
				(
				(SCREEN_WIDTH * 0.5f) + (fDiff),
					(SCREEN_HEIGHT * 0.05f),
					0.0f
				),
				D3DXVECTOR2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.05f));
		}

		// �����N
		if (m_pRank[nCntMember] != NULL)
		{// NULL�ȊO
			m_pRank[nCntMember]->SetPosSize(
				D3DXVECTOR3
				(
				(SCREEN_HEIGHT * 0.07f) + ((SCREEN_HEIGHT * 0.6f) * nCntMember) + (fDiff),
					(SCREEN_HEIGHT * 0.05f),
					0.0f
				),
				D3DXVECTOR2(SCREEN_HEIGHT * 0.07f, SCREEN_HEIGHT * 0.04f));
		}

		// �L����
		if (m_pChar[nCntMember] != NULL)
		{// NULL�ȊO
			m_pChar[nCntMember]->SetPosSize(
				D3DXVECTOR3
				(
				(SCREEN_HEIGHT * 0.07f) + (SCREEN_HEIGHT * 0.10f) + ((SCREEN_HEIGHT * 0.6f) * nCntMember) + (fDiff),
					(SCREEN_HEIGHT * 0.05f),
					0.0f
				),
				D3DXVECTOR2(SCREEN_HEIGHT * 0.04f, SCREEN_HEIGHT * 0.04f));
		}

		// �^�C��
		if (m_pTime[nCntMember] != NULL)
		{// NULL�ȊO
			m_pTime[nCntMember]->Setpos(
				D3DXVECTOR3
				(
				(SCREEN_HEIGHT * 0.07f) + (SCREEN_HEIGHT * 0.45f) + ((SCREEN_HEIGHT * 0.6f) * nCntMember) + (fDiff),
					(SCREEN_HEIGHT * 0.05f),
					0.0f
				));
		}
	}
}

//=============================================================================
// ���U���g����
//=============================================================================
void CResultUI::Result(void)
{
	int nCounter = CResult::GetCounter();

	if (RESULT_RANK_APP < nCounter)
	{
		int nFrame = nCounter - RESULT_RANK_APP;

		for (int nCntMember = (MAX_MEMBER - 1); 0 <= nCntMember; nCntMember--)
		{// �����o�[�J�E���g
			if (((MAX_MEMBER - 1) - nCntMember) == (nFrame / 10))
			{
				float fDiff = -(SCREEN_WIDTH) * (1.0f - (float)((float)((nFrame % 10) + 1) / (float)10));

				// �����NBG
				if (m_pRankBG[nCntMember] != NULL)
				{// NULL�ȊO
					m_pRankBG[nCntMember]->SetPosSize(
						D3DXVECTOR3
						(
						(SCREEN_WIDTH * 0.5f) - (SCREEN_WIDTH * 0.17f) + fDiff,
							(SCREEN_HEIGHT * 0.12f) + (((SCREEN_HEIGHT * 0.05f) * 2.2f) * nCntMember),
							0.0f
						),
						D3DXVECTOR2(SCREEN_WIDTH * 0.29f, SCREEN_HEIGHT * 0.05f));
				}

				// �����N
				if (m_pRank[nCntMember] != NULL)
				{// NULL�ȊO
					m_pRank[nCntMember]->SetPosSize(
						D3DXVECTOR3
						(
						(SCREEN_WIDTH * 0.5f) - (SCREEN_WIDTH * 0.4f) + fDiff,
							(SCREEN_HEIGHT * 0.12f) + (((SCREEN_HEIGHT * 0.05f) * 2.2f) * nCntMember),
							0.0f
						),
						D3DXVECTOR2(SCREEN_HEIGHT * 0.08f, SCREEN_HEIGHT * 0.05f));
				}

				// �L����
				if (m_pChar[nCntMember] != NULL)
				{// NULL�ȊO
					m_pChar[nCntMember]->SetPosSize(
						D3DXVECTOR3
						(
						(SCREEN_WIDTH * 0.5f) - (SCREEN_WIDTH * 0.3f) + fDiff,
							(SCREEN_HEIGHT * 0.12f) + (((SCREEN_HEIGHT * 0.05f) * 2.2f) * nCntMember),
							0.0f
						),
						D3DXVECTOR2(SCREEN_HEIGHT * 0.05f, SCREEN_HEIGHT * 0.05f));
				}

				// �v���C���[
				if (m_pPlayer[nCntMember] != NULL)
				{// NULL�ȊO
					m_pPlayer[nCntMember]->SetPosSize(
						D3DXVECTOR3
						(
						(SCREEN_WIDTH * 0.5f) - (SCREEN_WIDTH * 0.21f) + fDiff,
							(SCREEN_HEIGHT * 0.12f) - (SCREEN_HEIGHT * 0.012f) + (((SCREEN_HEIGHT * 0.05f) * 2.2f) * nCntMember),
							0.0f
						),
						D3DXVECTOR2(SCREEN_HEIGHT * 0.08f, SCREEN_HEIGHT * 0.05f));
				}

				// �^�C��
				if (m_pTime[nCntMember] != NULL)
				{// NULL�ȊO
					m_pTime[nCntMember]->Setpos(D3DXVECTOR3
					(
						(SCREEN_WIDTH * 0.5f) + (SCREEN_WIDTH * 0.08f) + fDiff,
						(SCREEN_HEIGHT * 0.12f) + (((SCREEN_HEIGHT * 0.05f) * 2.2f) * nCntMember),
						0.0f
					));
				}
			}
		}
	}

	if (RESULT_WAIT < nCounter)
	{
		int nFrame = nCounter - RESULT_WAIT;

		// �v���X
		if (m_pPress != NULL)
		{// NULL�ȊO
			nFrame %= 120;
			int nNum = nFrame % 60;

			m_pPress->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, (nFrame < 60 ? (float)((float)nNum / (float)60) : 1.0f - (float)((float)nNum / (float)60))));
		}
	}

	if ((RESULT_RANK_APP - 15) <= nCounter && nCounter < (RESULT_RANK_APP + 15))
	{
		int nFrame = nCounter - (RESULT_RANK_APP - 15);
		int nNum = (nFrame % 10) + 1;

		if (m_pFade != NULL)
		{// NULL�ȊO
			if (nFrame / 10 != 1)
				m_pFade->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, (nFrame < 10 ? (float)((float)nNum / (float)10) : 1.0f - (float)((float)nNum / (float)10))));
		}
	}
}

//=============================================================================
// ���[�h����
//=============================================================================
void CResultUI::LoadRanking(void)
{
	// �t�@�C���|�C���^
	FILE *pFile = NULL;
	pFile = fopen(RANKING_TEXT, "rb");

	if (pFile != NULL)
	{// NULL�ȊO
		fread(m_nCharacter, sizeof(int), MAX_RANKING, pFile);
		fread(m_nRanking, sizeof(int), MAX_RANKING, pFile);
		fclose(pFile);
	}
}

//=============================================================================
// �Z�[�u����
//=============================================================================
void CResultUI::SaveRanking(void)
{
	// �t�@�C���|�C���^
	FILE *pFile = NULL;
	pFile = fopen(RANKING_TEXT, "wb");

	if (pFile != NULL)
	{// NULL�ȊO
		fwrite(m_nCharacter, sizeof(int), MAX_RANKING, pFile);
		fwrite(m_nRanking, sizeof(int), MAX_RANKING, pFile);
		fclose(pFile);
	}
}

//=============================================================================
// �\�[�g����
//=============================================================================
void CResultUI::SortRanking(void)
{
	int *pnCharSelectNum = CGame::GetCharSelectNum();
	int nMaxPlayer = CGame::GetMaxPlayer();
	int *pnRankingSort = CGame::GetRankingSort();
	int *nTimer = CGame::GetTimer();

	int nCharacter[(MAX_PLAYER + MAX_RANKING)] = { 0 };
	int nRanking[(MAX_PLAYER + MAX_RANKING)] = { 0 };

	for (int nCntMember = 0; nCntMember < (MAX_PLAYER + MAX_RANKING); nCntMember++)
		nRanking[nCntMember] = MAX_TIME;

	// �l�ݒ�i�����[�X�j
	int nNumber = 0;
	for (int nCntMember = 0; nCntMember < (MAX_MEMBER); nCntMember++)
	{
		if (pnRankingSort[nCntMember] < nMaxPlayer)
		{
			nCharacter[nNumber] = pnCharSelectNum[pnRankingSort[nCntMember]];
			if (nTimer[pnRankingSort[nCntMember]] == 0)
				nRanking[nNumber] = MAX_TIME;
			else
				nRanking[nNumber] = nTimer[pnRankingSort[nCntMember]];
			nNumber++;
		}
	}

	// �l�ݒ�i�g�b�v���R�[�h�j
	for (int nCntRanking = 0; nCntRanking < (MAX_RANKING); nCntRanking++)
	{
		nCharacter[nNumber + nCntRanking] = m_nCharacter[nCntRanking];
		nRanking[nNumber + nCntRanking] = m_nRanking[nCntRanking];
	}

	// �\�[�g
	for (int nCntSort = 0; nCntSort < (MAX_PLAYER + MAX_RANKING) - 1; nCntSort++)
	{
		for (int nCntCheck = nCntSort; nCntCheck < (MAX_PLAYER + MAX_RANKING); nCntCheck++)
		{
			if (nRanking[nCntCheck] < nRanking[nCntSort])
			{
				int nData = 0;
				nData = nCharacter[nCntSort];
				nCharacter[nCntSort] = nCharacter[nCntCheck];
				nCharacter[nCntCheck] = nData;

				nData = nRanking[nCntSort];
				nRanking[nCntSort] = nRanking[nCntCheck];
				nRanking[nCntCheck] = nData;
			}
		}
	}

	// �ݒ�
	for (int nCntRanking = 0; nCntRanking < (MAX_RANKING); nCntRanking++)
	{
		m_nCharacter[nCntRanking] = nCharacter[nCntRanking];
		m_nRanking[nCntRanking] = nRanking[nCntRanking];
	}
}