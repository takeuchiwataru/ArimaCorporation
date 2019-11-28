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
		m_pRank[nCntMember] = NULL;			// �����N
		m_pChar[nCntMember] = NULL;			// �L����
		m_pPlayer[nCntMember] = NULL;		// �v���C���[
		m_pTime[nCntMember] = NULL;			// �^�C��
	}
	m_pPress = NULL;						// �v���X
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
			strcpy(cName, "data/TEXTURE/game/charselect/char.png");
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
	int nMaxPlayer = CGame::GetMaxPlayer();
	int *pnRankingSort = CGame::GetRankingSort();
	int *nTimer = CGame::GetTimer();

	for (int nCntMember = 0; nCntMember < MAX_MEMBER; nCntMember++)
	{// �����o�[�J�E���g
		// �����N
		if (m_pRank[nCntMember] == NULL)
		{// NULL
			m_pRank[nCntMember] = new CScene2D(6, CScene::OBJTYPE_2DPOLYGON);
			m_pRank[nCntMember]->Init();
			m_pRank[nCntMember]->SetPosSize(
				D3DXVECTOR3
				(
				(SCREEN_WIDTH * 0.5f) - (SCREEN_WIDTH * 0.4f),
					(SCREEN_HEIGHT * 0.12f) + (((SCREEN_HEIGHT * 0.05f) * 2.2f) * nCntMember),
					0.0f
				),
				D3DXVECTOR2(SCREEN_HEIGHT * 0.08f, SCREEN_HEIGHT * 0.05f));
			m_pRank[nCntMember]->BindTexture(m_pTexture[TEXTURE_RANK]);
			m_pRank[nCntMember]->SetTexture(nCntMember, 8, 1, 1);
		}

		// �L����
		if (m_pChar[nCntMember] == NULL)
		{// NULL
			m_pChar[nCntMember] = new CScene2D(6, CScene::OBJTYPE_2DPOLYGON);
			m_pChar[nCntMember]->Init();
			m_pChar[nCntMember]->SetPosSize(
				D3DXVECTOR3
				(
				(SCREEN_WIDTH * 0.5f) - (SCREEN_WIDTH * 0.3f),
					(SCREEN_HEIGHT * 0.12f) + (((SCREEN_HEIGHT * 0.05f) * 2.2f) * nCntMember),
					0.0f
				),
				D3DXVECTOR2(SCREEN_HEIGHT * 0.05f, SCREEN_HEIGHT * 0.05f));
			m_pChar[nCntMember]->BindTexture(m_pTexture[TEXTURE_CHAR]);
		}

		// �v���C���[
		if (m_pPlayer[nCntMember] == NULL)
		{// NULL
			m_pPlayer[nCntMember] = new CScene2D(6, CScene::OBJTYPE_2DPOLYGON);
			m_pPlayer[nCntMember]->Init();
			m_pPlayer[nCntMember]->SetPosSize(
				D3DXVECTOR3
				(
					(SCREEN_WIDTH * 0.5f) - (SCREEN_WIDTH * 0.21f),
					(SCREEN_HEIGHT * 0.12f) + (((SCREEN_HEIGHT * 0.05f) * 2.2f) * nCntMember),
					0.0f
				),
				D3DXVECTOR2(SCREEN_HEIGHT * 0.08f, SCREEN_HEIGHT * 0.05f));
			m_pPlayer[nCntMember]->BindTexture(m_pTexture[TEXTURE_ICON]);
			m_pPlayer[nCntMember]->SetTexture((pnRankingSort[nCntMember] < nMaxPlayer ? pnRankingSort[nCntMember] : 4), 5, 2, 1);
		}

		// �^�C��
		if (m_pTime[nCntMember] == NULL)
		{// NULL
			m_pTime[nCntMember] = CTime::Create(
				D3DXVECTOR3
				(
					(SCREEN_WIDTH * 0.5f) + (SCREEN_WIDTH * 0.08f),
					(SCREEN_HEIGHT * 0.12f) + (((SCREEN_HEIGHT * 0.05f) * 2.2f) * nCntMember),
					0.0f
				),
				D3DXVECTOR3((SCREEN_HEIGHT * 0.03f), (SCREEN_HEIGHT * 0.05f), 0.0f));
			m_pTime[nCntMember]->TexTime(nTimer[pnRankingSort[nCntMember]], true);

			if (pnRankingSort[nCntMember] < nMaxPlayer)
				m_pTime[nCntMember]->Setcol(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		}
	}

	// �v���X
	if (m_pPress == NULL)
	{// NULL
		m_pPress = new CScene2D(6, CScene::OBJTYPE_2DPOLYGON);
		m_pPress->Init();
		m_pPress->SetPosSize(
			D3DXVECTOR3
			(
				(SCREEN_WIDTH * 0.5f) + (SCREEN_WIDTH * 0.35f),
				(SCREEN_HEIGHT * 0.12f) + (SCREEN_HEIGHT * 0.8f),
				0.0f
			),
			D3DXVECTOR2(SCREEN_HEIGHT * 0.2f, SCREEN_HEIGHT * 0.05f));
		m_pPress->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		m_pPress->BindTexture(m_pTexture[TEXTURE_PRESS]);
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

	//���g�̍폜
	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CResultUI::Update(void)
{
	int nCounter = CResult::GetCounter();

	if (RESULT_WAIT < nCounter)
	{
		nCounter -= RESULT_WAIT;

		// �v���X
		if (m_pPress != NULL)
		{// NULL�ȊO
			nCounter %= 120;
			int nNum = nCounter % 60;

			m_pPress->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, (nCounter < 60 ? (float)((float)nNum / (float)60) : 1.0f - (float)((float)nNum / (float)60))));
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CResultUI::Draw(void)
{

}