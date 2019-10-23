//=============================================================================
//
// �Q�[���i�v���C�j�̏��� [game.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
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
#define RANKING_SIZE_1P		(0.13f)					// 1P���̃����L���O�̑傫��
#define RANKING_SIZE_4P		(0.07f)					// 4P���̃����L���O�̑傫��
#define ITEM_SIZE_1P		(0.11f)					// 1P���̃A�C�e���̑傫��
#define ITEM_SIZE_4P		(0.06f)					// 4P���̃A�C�e���̑傫��

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
	{// �v���C���[�J�E���g
		m_pRanking[nCntPlayer] = NULL;

		for (int nCntItem = 0; nCntItem < MAX_EGG; nCntItem++)
		{// �A�C�e���J�E���g
			m_pItem[nCntPlayer][nCntItem] = NULL;
		}
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
		case TEXTURE_RANK:
			strcpy(cName, "data/TEXTURE/game/play/rank.png");
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
					(SCREEN_WIDTH * ((nMaxPlayer - 1) / 2 == 0 ? 0.5f : (nCntPlayer % 2 == 0 ? 0.25f : 0.75f)))		+ (((SCREEN_WIDTH * ((nMaxPlayer - 1) / 2 == 0 ? 0.5f : 0.25f))	- 
					(SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? RANKING_SIZE_1P : RANKING_SIZE_4P))) * ((nMaxPlayer - 1) / 2 == 0 ? -1.0f : (nCntPlayer % 2 == 0 ? -1.0f : 1.0f))),
					(SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? 0.5f : ((nMaxPlayer - 1) / 2 == 0 ? (nCntPlayer % 2 == 0 ? 0.25f : 0.75f) : (nCntPlayer / 2 == 0 ? 0.25f : 0.75f))))	+ ((SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? 0.5f : 0.25f)) -
					(SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? RANKING_SIZE_1P : RANKING_SIZE_4P))),
					0.0f),
				D3DXVECTOR2(
					SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? RANKING_SIZE_1P : RANKING_SIZE_4P),
					SCREEN_HEIGHT * ((nMaxPlayer - 1) == 0 ? RANKING_SIZE_1P : RANKING_SIZE_4P)));
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

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// �v���C���[�J�E���g
		if (m_pRanking[nCntPlayer] != NULL)
		{// NULL�ȊO
			m_pRanking[nCntPlayer]->SetTexture(pnRanking[nCntPlayer], 8, 1, 1);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CGamePlay::Draw(void)
{

}