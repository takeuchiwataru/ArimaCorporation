//=============================================================================
//
// �Q�[���i�L�����I���j�̏��� [game.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS	//Fire Wall�˔j
#include <string.h>				//�X�g�����O�g�p�̂���
#include "gameCharSelect.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "input.h"
#include "fade.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	CGameCharSelect::m_pTexture[CGameCharSelect::TEXTURE_MAX] = { NULL };

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CGameCharSelect::CGameCharSelect()
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// �v���C���[�J�E���g
		m_pPlayerNum[nCntPlayer] = NULL;
		m_pPlayerBG[nCntPlayer] = NULL;
		m_pSelect[nCntPlayer] = NULL;
		m_pEnter[nCntPlayer] = NULL;

		m_bEntry[nCntPlayer] = false;
		m_bEnter[nCntPlayer] = false;
	}

	m_pYor = NULL;						// ���Ȃ�
	m_pReturn = NULL;					// �߂�
	m_pReturnBG = NULL;					// �߂�w�i
	m_pReturnChar = NULL;				// �߂�L����

	m_nReturnCounter = 0;				// �߂�J�E���g
	m_nEntryCounter = 0;				// �G���g���[�J�E���g

	for (int nCntChar = 0; nCntChar < MAX_CHARCTER; nCntChar++)
		m_pCharacter[nCntChar] = NULL;
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGameCharSelect::~CGameCharSelect() {}

//=============================================================================
// ���[�h
//=============================================================================
HRESULT CGameCharSelect::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{// �e�N�X�`���J�E���g
		char cName[128] = {};

		switch (nCntTex)
		{
		case TEXTURE_CHAR:
			strcpy(cName, "data/TEXTURE/game/charselect/char.png");
			break;
		case TEXTURE_FRAME:
			strcpy(cName, "data/TEXTURE/game/charselect/select_frame.png");
			break;
		case TEXTURE_NUMBER:
			strcpy(cName, "data/TEXTURE/game/charselect/icon.png");
			break;
		case TEXTURE_ENTER:
			strcpy(cName, "data/TEXTURE/game/charselect/characterselect.png");
			break;
		case TEXTURE_YOU:
			strcpy(cName, "data/TEXTURE/game/charselect/modeselect_17.png");
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
void CGameCharSelect::Unload(void)
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
CGameCharSelect *CGameCharSelect::Create(void)
{
	CGameCharSelect *pGameCharSelect = NULL;

	// �������m��
	if (pGameCharSelect == NULL)
		pGameCharSelect = new CGameCharSelect;

	if (pGameCharSelect != NULL)
	{// NULL�ȊO
		pGameCharSelect->Init();	// ����������
	}

	return pGameCharSelect;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CGameCharSelect::Init()
{
	// �擾
	bool bOnine = CTitle::GetOnline();

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// �v���C���[�J�E���g	
		// �v���C���[�ԍ�
		if (m_pPlayerNum[nCntPlayer] == NULL)
		{// NULL
			m_pPlayerNum[nCntPlayer] = new CScene2D(1, CScene::OBJTYPE_2DPOLYGON);
			m_pPlayerNum[nCntPlayer]->Init();
			m_pPlayerNum[nCntPlayer]->SetPosSize(
				D3DXVECTOR3(
					(nCntPlayer % 2 == 0 ? 0.0f + (SCREEN_HEIGHT * 0.25f) : SCREEN_WIDTH - (SCREEN_HEIGHT * 0.25f)) - (SCREEN_HEIGHT * 0.18f),
					(nCntPlayer / 2 == 0 ? 0.0f + (SCREEN_HEIGHT * 0.25f) : SCREEN_HEIGHT - (SCREEN_HEIGHT * 0.25f)) - (SCREEN_HEIGHT * 0.2f),
					0.0f),
				D3DXVECTOR2(SCREEN_HEIGHT * 0.1f, SCREEN_HEIGHT * 0.05f));
			m_pPlayerNum[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_pPlayerNum[nCntPlayer]->BindTexture(m_pTexture[TEXTURE_NUMBER]);
			m_pPlayerNum[nCntPlayer]->SetTexture(nCntPlayer, 5, 2, 1);
		}
		// �v���C���[�w�i
		if (m_pPlayerBG[nCntPlayer] == NULL)
		{// NULL
			m_pPlayerBG[nCntPlayer] = new CScene2D(0, CScene::OBJTYPE_2DPOLYGON);
			m_pPlayerBG[nCntPlayer]->Init();
			m_pPlayerBG[nCntPlayer]->SetPosSize(
				D3DXVECTOR3(
					(nCntPlayer % 2 == 0 ? 0.0f + (SCREEN_HEIGHT * 0.25f) : SCREEN_WIDTH - (SCREEN_HEIGHT * 0.25f)),
					(nCntPlayer / 2 == 0 ? 0.0f + (SCREEN_HEIGHT * 0.25f) : SCREEN_HEIGHT - (SCREEN_HEIGHT * 0.25f)),
					0.0f),
				D3DXVECTOR2(SCREEN_HEIGHT * 0.25f, SCREEN_HEIGHT * 0.25f));
			//m_pPlayerBG[nCntPlayer]->SetColor(&D3DXCOLOR((nCntPlayer % 2 == 0 ? 1.0f : 0.0f), (nCntPlayer / 2 % 2 == 1 ? 1.0f : 0.0f), (nCntPlayer % 4 == 1 ? 1.0f : 0.0f), 0.6f));
			m_pPlayerBG[nCntPlayer]->BindTexture(m_pTexture[TEXTURE_FRAME]);
		}
		// �I��
		if (m_pSelect[nCntPlayer] == NULL)
		{// NULL
			m_pSelect[nCntPlayer] = new CScene2D(1, CScene::OBJTYPE_2DPOLYGON);
			m_pSelect[nCntPlayer]->Init();
			m_pSelect[nCntPlayer]->SetPosSize(
				D3DXVECTOR3(
					(SCREEN_WIDTH * 0.5f) + ((SCREEN_WIDTH * 0.055f + (nCntPlayer % 4 == 0 || nCntPlayer % 4 == 3 ? (SCREEN_WIDTH * 0.055f) * 2.0f : 0.0f)) * (nCntPlayer / 2 % 2 == 0 ? -1.0f : 1.0f)),
					(SCREEN_HEIGHT * 0.7f) + ((SCREEN_WIDTH * 0.055f) * (nCntPlayer / 4 == 0 ? -1.0f : 1.0f)),
					0.0f),
				D3DXVECTOR2(SCREEN_WIDTH * 0.04f, SCREEN_WIDTH * 0.04f));
			m_pSelect[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_pSelect[nCntPlayer]->BindTexture(m_pTexture[TEXTURE_NUMBER]);
			m_pSelect[nCntPlayer]->SetTexture(nCntPlayer, 5, 1, 1);
		}
		// ����
		if (m_pEnter[nCntPlayer] == NULL)
		{// NULL
			m_pEnter[nCntPlayer] = new CScene2D(1, CScene::OBJTYPE_2DPOLYGON);
			m_pEnter[nCntPlayer]->Init();
			m_pEnter[nCntPlayer]->SetPosSize(
				D3DXVECTOR3(
				(nCntPlayer % 2 == 0 ? 0.0f + (SCREEN_HEIGHT * 0.25f) : SCREEN_WIDTH - (SCREEN_HEIGHT * 0.25f)),
					(nCntPlayer / 2 == 0 ? 0.0f + (SCREEN_HEIGHT * 0.25f) : SCREEN_HEIGHT - (SCREEN_HEIGHT * 0.25f)) + (SCREEN_HEIGHT * 0.18f),
					0.0f),
				D3DXVECTOR2(SCREEN_HEIGHT * 0.18f, SCREEN_HEIGHT * 0.05f));
			m_pEnter[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_pEnter[nCntPlayer]->BindTexture(m_pTexture[TEXTURE_ENTER]);
			m_pEnter[nCntPlayer]->SetTexture(1, 1, 2, 2);

		}
	}

	// �L�����N�^�[
	for (int nCntChar = 0; nCntChar < MAX_CHARCTER; nCntChar++)
	{// �L�����J�E���g
		if (m_pCharacter[nCntChar] == NULL)
		{// NULL
			m_pCharacter[nCntChar] = new CScene2D(0, CScene::OBJTYPE_2DPOLYGON);
			m_pCharacter[nCntChar]->Init();
			m_pCharacter[nCntChar]->SetPosSize(
				D3DXVECTOR3(
					(SCREEN_WIDTH * 0.5f) + ((SCREEN_WIDTH * 0.053f + (nCntChar % 4 == 0 || nCntChar % 4 == 3 ? (SCREEN_WIDTH * 0.053f) * 2.0f : 0.0f)) * (nCntChar / 2 % 2 == 0 ? -1.0f : 1.0f)),
					(SCREEN_HEIGHT * 0.8f) + ((SCREEN_WIDTH * 0.053f) * (nCntChar / 4 == 0 ? -1.0f : 1.0f)),
					0.0f),
				D3DXVECTOR2(SCREEN_WIDTH * 0.05f, SCREEN_WIDTH * 0.05f));
			m_pCharacter[nCntChar]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_pCharacter[nCntChar]->BindTexture(m_pTexture[TEXTURE_CHAR]);
		}
	}

	if (bOnine == true)
	{// �I�����C��
		int nClient = CServer::GetnMaxClient();		// �N���C�A���g��
		int nID = CClient::GetnID();			// �N���C�A���gID

		// ���Ȃ�
		if (m_pYor == NULL)
		{// NULL
			m_pYor = new CScene2D(0, CScene::OBJTYPE_2DPOLYGON);
			m_pYor->Init();
			m_pYor->SetPosSize(
				D3DXVECTOR3(
					(nID % 2 == 0 ? 0.0f + (SCREEN_HEIGHT * 0.25f) : SCREEN_WIDTH - (SCREEN_HEIGHT * 0.25f)) + (SCREEN_HEIGHT * 0.07f),
					(nID / 2 == 0 ? 0.0f + (SCREEN_HEIGHT * 0.25f) : SCREEN_HEIGHT - (SCREEN_HEIGHT * 0.25f)) - (SCREEN_HEIGHT * 0.18f),
					0.0f),
				D3DXVECTOR2(SCREEN_HEIGHT * 0.12f, SCREEN_HEIGHT * 0.04f));
			m_pYor->BindTexture(m_pTexture[TEXTURE_YOU]);
		}

		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
		{// �v���C���[�J�E���g				
			// �v���C���[�ԍ�
			if (m_pPlayerNum[nCntPlayer] != NULL)
			{// NULL�ȊO
				if (nCntPlayer < nClient)
					m_pPlayerNum[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			// ����
			if (m_pEnter[nCntPlayer] != NULL)
			{// NULL�ȊO
				m_pEnter[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
				m_pEnter[nCntPlayer]->SetTexture(0, 1, 2, 2);
			}

			// �Q��
			if (nCntPlayer < nClient)
				m_bEntry[nCntPlayer] = true;
		}
		
		// �v���C���[�ő吔�ݒ�
		CGame::SetMaxPlayer(nClient);
	}
	else
	{
		// ���ǂ�
		if (m_pReturn == NULL)
		{// NULL
			m_pReturn = new CScene2D(0, CScene::OBJTYPE_2DPOLYGON);
			m_pReturn->Init();
			m_pReturn->SetPosSize(
				D3DXVECTOR3(
					(SCREEN_WIDTH * 0.5f) + -(SCREEN_WIDTH * 0.12f),
					(SCREEN_HEIGHT * 0.05f),
					0.0f),
				D3DXVECTOR2(SCREEN_HEIGHT * 0.12f, SCREEN_HEIGHT * 0.032f));
		}

		// ���ǂ�w�i
		if (m_pReturnBG == NULL)
		{// NULL
			m_pReturnBG = new CScene2D(0, CScene::OBJTYPE_2DPOLYGON);
			m_pReturnBG->Init();
			m_pReturnBG->SetPosSize(
				D3DXVECTOR3(
					(SCREEN_WIDTH * 0.5f) + (SCREEN_WIDTH * 0.07f),
					(SCREEN_HEIGHT * 0.05f),
					0.0f),
				D3DXVECTOR2(SCREEN_HEIGHT * 0.2f, SCREEN_HEIGHT * 0.032f));
			m_pReturnBG->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}

		// ���ǂ�L����
		if (m_pReturnChar == NULL)
		{// NULL
			m_pReturnChar = new CScene2D(0, CScene::OBJTYPE_2DPOLYGON);
			m_pReturnChar->Init();
			m_pReturnChar->SetPosSize(
				D3DXVECTOR3(
					(SCREEN_WIDTH * 0.5f) + (SCREEN_WIDTH * 0.07f) + -(SCREEN_WIDTH * 0.09f),
					(SCREEN_HEIGHT * 0.05f),
					0.0f),
				D3DXVECTOR2(SCREEN_HEIGHT * 0.03f, SCREEN_HEIGHT * 0.03f));
			m_pReturnChar->SetColor(&D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
		}
	}

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CGameCharSelect::Uninit(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// �v���C���[�J�E���g	
		if (m_pPlayerNum[nCntPlayer] != NULL)
		{// NULL�ȊO
			m_pPlayerNum[nCntPlayer]->Uninit();	// �I������
			m_pPlayerNum[nCntPlayer] = NULL;	// NULL��
		}
		if (m_pPlayerBG[nCntPlayer] != NULL)
		{// NULL�ȊO
			m_pPlayerBG[nCntPlayer]->Uninit();	// �I������
			m_pPlayerBG[nCntPlayer] = NULL;		// NULL��
		}
		if (m_pSelect[nCntPlayer] != NULL)
		{// NULL�ȊO
			m_pSelect[nCntPlayer]->Uninit();	// �I������
			m_pSelect[nCntPlayer] = NULL;		// NULL��
		}
		if (m_pEnter[nCntPlayer] != NULL)
		{// NULL�ȊO
			m_pEnter[nCntPlayer]->Uninit();		// �I������
			m_pEnter[nCntPlayer] = NULL;		// NULL��
		}
	}

	// ���Ȃ�
	if (m_pYor != NULL)
	{// NULL�ȊO
		m_pYor->Uninit();		// �I������
		m_pYor = NULL;			// NULL��
	}

	// ���ǂ�
	if (m_pReturn != NULL)
	{// NULL�ȊO
		m_pReturn->Uninit();	// �I������
		m_pReturn = NULL;		// NULL��
	}
	// ���ǂ�w�i
	if (m_pReturnBG != NULL)
	{// NULL�ȊO
		m_pReturnBG->Uninit();	// �I������
		m_pReturnBG = NULL;		// NULL��
	}
	// ���ǂ�L����
	if (m_pReturnChar != NULL)
	{// NULL�ȊO
		m_pReturnChar->Uninit();	// �I������
		m_pReturnChar = NULL;		// NULL��
	}

	// �L�����N�^�[
	for (int nCntChar = 0; nCntChar < MAX_CHARCTER; nCntChar++)
	{// �L�����J�E���g
		if (m_pCharacter[nCntChar] != NULL)
		{// NULL�ȊO
			m_pCharacter[nCntChar]->Uninit();	// �I������
			m_pCharacter[nCntChar] = NULL;		// NULL��
		}
	}

	//���g�̍폜
	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CGameCharSelect::Update(void)
{
	// �擾
	bool bOnine = CTitle::GetOnline();

	if (bOnine == true)	
		Online();		// �I�����C��
	else
		Local();		// ���[�J��
}

//=============================================================================
// �`�揈��
//=============================================================================
void CGameCharSelect::Draw(void)
{

}

//=============================================================================
// �I�����C��
//=============================================================================
void CGameCharSelect::Online(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputJoyPad_0 * pXpad = CManager::GetInputJoyPad0(0);		//�W���C�p�b�g�̎擾
	CSound *pSound = CManager::GetSound();						//�T�E���h�̏��

	// �v���C���[�ő吔�擾
	int nMaxPlayer = CGame::GetMaxPlayer();
	// �L�����I��ԍ��擾
	int *pnCharSelectNum = CGame::GetCharSelectNum();

	int nPlayerNum = nMaxPlayer;

	if (CFade::GetFade() == CFade::FADE_NONE)
	{// �g�p���Ă��Ȃ�
		for (int nCntPlayer = 0; nCntPlayer < nMaxPlayer; nCntPlayer++)
		{// �v���C���[�J�E���g
			pXpad = CManager::GetInputJoyPad0(nCntPlayer);		//�W���C�p�b�g�̎擾

			if (m_bEnter[nCntPlayer] == false)
			{// ���肵�Ă��Ȃ�
				if (pCInputKeyBoard->GetKeyboardTrigger(DIK_W) == true ||
					pXpad->GetTrigger(INPUT_LS_U) == true ||
					pXpad->GetTrigger(INPUT_UP) == true)
				{// ��L�[
					if (pnCharSelectNum[nCntPlayer] / 4 == 1)
					{// �ړ�����
					 // ���̒l
						int nNext = (pnCharSelectNum[nCntPlayer] + 4) % MAX_CHARCTER;

						for (int nCntSelect = 0; nCntSelect < nMaxPlayer; nCntSelect++)
						{// �I���J�E���g
							if (nCntSelect != nCntPlayer)
							{// �����ȊO
								if (pnCharSelectNum[nCntSelect] == nNext)	// �l����v
									break;
							}

							if (nCntSelect == (nMaxPlayer - 1))			// ��v���Ȃ�������X�V
								pnCharSelectNum[nCntPlayer] = nNext;
						}

					}
				}
				else if (pCInputKeyBoard->GetKeyboardTrigger(DIK_S) == true ||
					pXpad->GetTrigger(INPUT_LS_D) == true ||
					pXpad->GetTrigger(INPUT_DOWN) == true)
				{// ���L�[
					if (pnCharSelectNum[nCntPlayer] / 4 == 0)
					{// �ړ�����
					 // ���̒l
						int nNext = (pnCharSelectNum[nCntPlayer] + 4) % MAX_CHARCTER;

						for (int nCntSelect = 0; nCntSelect < nMaxPlayer; nCntSelect++)
						{// �I���J�E���g
							if (nCntSelect != nCntPlayer)
							{// �����ȊO
								if (pnCharSelectNum[nCntSelect] == nNext)	// �l����v
									break;
							}

							if (nCntSelect == (nMaxPlayer - 1))			// ��v���Ȃ�������X�V
								pnCharSelectNum[nCntPlayer] = nNext;
						}
					}
				}
				if (pCInputKeyBoard->GetKeyboardTrigger(DIK_A) == true ||
					pXpad->GetTrigger(INPUT_LS_L) == true ||
					pXpad->GetTrigger(INPUT_LEFT) == true)
				{// ���L�[
					if (pnCharSelectNum[nCntPlayer] % 4 != 0)
					{// �ړ�����
					 // ���̒l
							int nNext = pnCharSelectNum[nCntPlayer];

						for (int nCntLine = pnCharSelectNum[nCntPlayer] % 4 - 1; 0 <= nCntLine; nCntLine--)
						{// ���C���J�E���g
							nNext = (nNext - 1) % MAX_CHARCTER;

							for (int nCntSelect = 0; nCntSelect < nMaxPlayer; nCntSelect++)
							{// �I���J�E���g
								if (nCntSelect != nCntPlayer)
								{// �����ȊO
									if (pnCharSelectNum[nCntSelect] == nNext)	// �l����v
										break;
								}

								if (nCntSelect == (nMaxPlayer - 1))			// ��v���Ȃ�������X�V
									pnCharSelectNum[nCntPlayer] = nNext;
							}

							if (pnCharSelectNum[nCntPlayer] == nNext)				// �X�V����
								break;
						}
					}
				}
				else if (pCInputKeyBoard->GetKeyboardTrigger(DIK_D) == true ||
					pXpad->GetTrigger(INPUT_LS_R) == true ||
					pXpad->GetTrigger(INPUT_RIGHT) == true)
				{// �E�L�[
					if (pnCharSelectNum[nCntPlayer] % 4 != (4 - 1))
					{// �ړ�����
					 // ���̒l
						int nNext = pnCharSelectNum[nCntPlayer];

						for (int nCntLine = pnCharSelectNum[nCntPlayer] % 4 + 1; nCntLine < 4; nCntLine++)
						{// ���C���J�E���g	
							nNext = (nNext + 1) % MAX_CHARCTER;

							for (int nCntSelect = 0; nCntSelect < nMaxPlayer; nCntSelect++)
							{// �I���J�E���g
								if (nCntSelect != nCntPlayer)
								{// �����ȊO
									if (pnCharSelectNum[nCntSelect] == nNext)	// �l����v
										break;
								}

								if (nCntSelect == (nMaxPlayer - 1))			// ��v���Ȃ�������X�V
									pnCharSelectNum[nCntPlayer] = nNext;
							}

							if (pnCharSelectNum[nCntPlayer] == nNext)				// �X�V����
								break;
						}
					}
				}
			}

			if (pXpad->GetTrigger(INPUT_B) == true)
			{// ����L�[
				if (m_bEntry[nCntPlayer] == true)
				{
					m_bEnter[nCntPlayer] = (m_bEnter[nCntPlayer] ^ 1 ? true : false);

					//���艹�̐ݒ�
					pSound->SetVolume(CSound::SOUND_LABEL_SE_CHARACTERSERECT, 1.3f);
					pSound->PlaySound(CSound::SOUND_LABEL_SE_CHARACTERSERECT);
				}
			}
		}
	}

	int nChackEnter = 0;
	for (int nCntPlayer = 0; nCntPlayer < nMaxPlayer; nCntPlayer++)
	{// �v���C���[�J�E���g
		// �I��
		if (m_pSelect[nCntPlayer] != NULL)
		{// NULL�ȊO
			m_pSelect[nCntPlayer]->SetPosSize(
				D3DXVECTOR3(
					(SCREEN_WIDTH * 0.5f) + ((SCREEN_WIDTH * 0.053f + (pnCharSelectNum[nCntPlayer] % 4 == 0 || pnCharSelectNum[nCntPlayer] % 4 == 3 ? (SCREEN_WIDTH * 0.053f) * 2.0f : 0.0f)) * (pnCharSelectNum[nCntPlayer] / 2 % 2 == 0 ? -1.0f : 1.0f)),
					(SCREEN_HEIGHT * 0.7f) + ((SCREEN_WIDTH * 0.053f) * (pnCharSelectNum[nCntPlayer] / 4 == 0 ? -1.0f : 1.0f)),
					0.0f),
				D3DXVECTOR2(SCREEN_WIDTH * 0.04f, SCREEN_WIDTH * 0.04f));
			m_pSelect[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}

		// ���萔�`���b�N
		if (m_bEnter[nCntPlayer] == true)
		{// ����
			nChackEnter++;

			// ����
			if (m_pEnter[nCntPlayer] != NULL)
			{
				m_pEnter[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
		else
		{// �I��
		 // ����
			if (m_pEnter[nCntPlayer] != NULL)
			{
				m_pEnter[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			}
		}

		// �S������
		if (nChackEnter == nMaxPlayer)
			CFade::Create(CGame::GAMEMODE_COURSE_VIEW);
	}

	// �v���C���[�ő吔�ݒ�
	CGame::SetMaxPlayer(nMaxPlayer);
	// �L�����I��ԍ��ݒ�
	CGame::SetCharSelectNum(pnCharSelectNum);

	m_nEntryCounter++;		// �J�E���g	
}

//=============================================================================
// ���[�J��
//=============================================================================
void CGameCharSelect::Local(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputJoyPad_0 * pXpad = CManager::GetInputJoyPad0(0);		//�W���C�p�b�g�̎擾
	CSound *pSound = CManager::GetSound();						//�T�E���h�̏��

	// �v���C���[�ő吔�擾
	int nMaxPlayer = CGame::GetMaxPlayer();
	// �L�����I��ԍ��擾
	int *pnCharSelectNum = CGame::GetCharSelectNum();
	// �R���g���[���[�ԍ��擾
	int *pnControllerNum = CGame::GetControllerNum();

	int nPlayerNum = nMaxPlayer;

	if (CFade::GetFade() == CFade::FADE_NONE)
	{// �g�p���Ă��Ȃ�
		for (int nCntPlayer = 0; nCntPlayer < nMaxPlayer; nCntPlayer++)
		{// �v���C���[�J�E���g
			pXpad = CManager::GetInputJoyPad0(pnControllerNum[nCntPlayer]);		//�W���C�p�b�g�̎擾

			if (m_bEnter[nCntPlayer] == false)
			{// ���肵�Ă��Ȃ�
				if (pCInputKeyBoard->GetKeyboardTrigger(DIK_W) == true ||
					pCInputKeyBoard->GetKeyboardTrigger(DIK_UP) == true ||
					pXpad->GetTrigger(INPUT_LS_U) == true ||
					pXpad->GetTrigger(INPUT_UP) == true)
				{// ��L�[
					if (pnCharSelectNum[nCntPlayer] / 4 == 1)
					{// �ړ�����
					 // ���̒l
						int nNext = (pnCharSelectNum[nCntPlayer] + 4) % MAX_CHARCTER;

						for (int nCntSelect = 0; nCntSelect < nMaxPlayer; nCntSelect++)
						{// �I���J�E���g
							if (nCntSelect != nCntPlayer)
							{// �����ȊO
								if (pnCharSelectNum[nCntSelect] == nNext)	// �l����v
									break;
							}

							if (nCntSelect == (nMaxPlayer - 1))			// ��v���Ȃ�������X�V
								pnCharSelectNum[nCntPlayer] = nNext;
						}

					}
				}
				else if (pCInputKeyBoard->GetKeyboardTrigger(DIK_S) == true ||
					pCInputKeyBoard->GetKeyboardTrigger(DIK_DOWN) == true ||
					pXpad->GetTrigger(INPUT_LS_D) == true ||
					pXpad->GetTrigger(INPUT_DOWN) == true)
				{// ���L�[
					if (pnCharSelectNum[nCntPlayer] / 4 == 0)
					{// �ړ�����
					 // ���̒l
						int nNext = (pnCharSelectNum[nCntPlayer] + 4) % MAX_CHARCTER;

						for (int nCntSelect = 0; nCntSelect < nMaxPlayer; nCntSelect++)
						{// �I���J�E���g
							if (nCntSelect != nCntPlayer)
							{// �����ȊO
								if (pnCharSelectNum[nCntSelect] == nNext)	// �l����v
									break;
							}

							if (nCntSelect == (nMaxPlayer - 1))			// ��v���Ȃ�������X�V
								pnCharSelectNum[nCntPlayer] = nNext;
						}
					}
				}
				if (pCInputKeyBoard->GetKeyboardTrigger(DIK_A) == true ||
					pCInputKeyBoard->GetKeyboardTrigger(DIK_LEFT) == true ||
					pXpad->GetTrigger(INPUT_LS_L) == true ||
					pXpad->GetTrigger(INPUT_LEFT) == true)
				{// ���L�[
					if (pnCharSelectNum[nCntPlayer] % 4 != 0)
					{// �ړ�����
					 // ���̒l
						int nNext = pnCharSelectNum[nCntPlayer];

						for (int nCntLine = pnCharSelectNum[nCntPlayer] % 4 - 1; 0 <= nCntLine; nCntLine--)
						{// ���C���J�E���g
							nNext = (nNext - 1) % MAX_CHARCTER;

							for (int nCntSelect = 0; nCntSelect < nMaxPlayer; nCntSelect++)
							{// �I���J�E���g
								if (nCntSelect != nCntPlayer)
								{// �����ȊO
									if (pnCharSelectNum[nCntSelect] == nNext)	// �l����v
										break;
								}

								if (nCntSelect == (nMaxPlayer - 1))			// ��v���Ȃ�������X�V
									pnCharSelectNum[nCntPlayer] = nNext;
							}

							if (pnCharSelectNum[nCntPlayer] == nNext)				// �X�V����
								break;
						}
					}
				}
				else if (pCInputKeyBoard->GetKeyboardTrigger(DIK_D) == true ||
					pCInputKeyBoard->GetKeyboardTrigger(DIK_RIGHT) == true ||
					pXpad->GetTrigger(INPUT_LS_R) == true ||
					pXpad->GetTrigger(INPUT_RIGHT) == true)
				{// �E�L�[
					if (pnCharSelectNum[nCntPlayer] % 4 != (4 - 1))
					{// �ړ�����
					 // ���̒l
						int nNext = pnCharSelectNum[nCntPlayer];

						for (int nCntLine = pnCharSelectNum[nCntPlayer] % 4 + 1; nCntLine < 4; nCntLine++)
						{// ���C���J�E���g	
							nNext = (nNext + 1) % MAX_CHARCTER;

							for (int nCntSelect = 0; nCntSelect < nMaxPlayer; nCntSelect++)
							{// �I���J�E���g
								if (nCntSelect != nCntPlayer)
								{// �����ȊO
									if (pnCharSelectNum[nCntSelect] == nNext)	// �l����v
										break;
								}

								if (nCntSelect == (nMaxPlayer - 1))			// ��v���Ȃ�������X�V
									pnCharSelectNum[nCntPlayer] = nNext;
							}

							if (pnCharSelectNum[nCntPlayer] == nNext)				// �X�V����
								break;
						}
					}
				}
			}

			if (pXpad->GetTrigger(INPUT_START) == true)
			{// ����L�[
				if (m_bEntry[nCntPlayer] == true)
				{
					m_bEnter[nCntPlayer] = (m_bEnter[nCntPlayer] ^ 1 ? true : false);

					//���艹�̐ݒ�
					pSound->SetVolume(CSound::SOUND_LABEL_SE_CHARACTERSERECT, 1.3f);
					pSound->PlaySound(CSound::SOUND_LABEL_SE_CHARACTERSERECT);
				}
			}
		}

		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
		{// �v���C���[�J�E���g
			bool bSet = false;	// �R���g���[���[�`�F�b�N�p

			// �g�p���Ă��Ȃ��R���g���[���[���`�F�b�N
			for (int nCntCheck = 0; nCntCheck < nMaxPlayer; nCntCheck++)
				if (pnControllerNum[nCntCheck] == nCntPlayer)
					bSet = true;

			if (bSet == false)
			{// �g�p���Ă��Ȃ�
				pXpad = CManager::GetInputJoyPad0(nCntPlayer);		//�W���C�p�b�g�̎擾

				if (pXpad->GetTrigger(INPUT_START) == true)
				{// ����L�[
					if (m_bEntry[nPlayerNum] == false)
					{
						m_bEntry[nPlayerNum] = true;
						pnControllerNum[nPlayerNum] = nCntPlayer;
						nPlayerNum = nPlayerNum + (nPlayerNum < (MAX_PLAYER - 1) ? 1 : 0);
					}
				}
			}
		}

		// �L�[�{�[�h
		if (pCInputKeyBoard->GetKeyboardTrigger(DIK_Z) == true || pCInputKeyBoard->GetKeyboardTrigger(DIK_RETURN) == true)
		{
			if (m_bEntry[0] == true)
			{
				m_bEnter[0] = (m_bEnter[0] ^ 1 ? true : false);

				//���艹�̐ݒ�
				pSound->SetVolume(CSound::SOUND_LABEL_SE_CHARACTERSERECT, 1.3f);
				pSound->PlaySound(CSound::SOUND_LABEL_SE_CHARACTERSERECT);
			}
			else
			{
				m_bEntry[0] = true;
			}
		}

		// �f�o�b�O�i�e����j
		if (pCInputKeyBoard->GetKeyboardTrigger(DIK_1) == true)
		{
			if (m_bEntry[0] == true)
			{
				m_bEnter[0] = (m_bEnter[0] ^ 1 ? true : false);

				//���艹�̐ݒ�
				pSound->SetVolume(CSound::SOUND_LABEL_SE_CHARACTERSERECT, 1.3f);
				pSound->PlaySound(CSound::SOUND_LABEL_SE_CHARACTERSERECT);
			}
			else
			{
				m_bEntry[0] = true;
			}
		}
		if (pCInputKeyBoard->GetKeyboardTrigger(DIK_2) == true)
		{
			if (m_bEntry[1] == true)
			{
				m_bEnter[1] = (m_bEnter[1] ^ 1 ? true : false);

				//���艹�̐ݒ�
				pSound->SetVolume(CSound::SOUND_LABEL_SE_CHARACTERSERECT, 1.3f);
				pSound->PlaySound(CSound::SOUND_LABEL_SE_CHARACTERSERECT);
			}
			else
			{
				m_bEntry[1] = true;
			}
		}
		if (pCInputKeyBoard->GetKeyboardTrigger(DIK_3) == true)
		{
			if (m_bEntry[2] == true)
			{
				m_bEnter[2] = (m_bEnter[2] ^ 1 ? true : false);

				//���艹�̐ݒ�
				pSound->SetVolume(CSound::SOUND_LABEL_SE_CHARACTERSERECT, 1.3f);
				pSound->PlaySound(CSound::SOUND_LABEL_SE_CHARACTERSERECT);
			}
			else
			{
				m_bEntry[2] = true;
			}
		}
		if (pCInputKeyBoard->GetKeyboardTrigger(DIK_4) == true)
		{
			if (m_bEntry[3] == true)
			{
				m_bEnter[3] = (m_bEnter[3] ^ 1 ? true : false);

				//���艹�̐ݒ�
				pSound->SetVolume(CSound::SOUND_LABEL_SE_CHARACTERSERECT, 1.3f);
				pSound->PlaySound(CSound::SOUND_LABEL_SE_CHARACTERSERECT);
			}
			else
			{
				m_bEntry[3] = true;
			}
		}

		pXpad = CManager::GetInputJoyPad0(pnControllerNum[0]);		//�W���C�p�b�g�̎擾

		if (pCInputKeyBoard->GetKeyboardPress(DIK_X) == true || pXpad->GetPress(INPUT_A) == true)
		{
			if (m_nReturnCounter < 90)
				m_nReturnCounter++;
			else
				CFade::Create(CManager::MODE_TITLE);
		}
		else
		{
			if (0 < m_nReturnCounter)
				m_nReturnCounter -= 2;
			else
				m_nReturnCounter = 0;
		}
	}

	int nChackEnter = 0;
	for (int nCntPlayer = 0; nCntPlayer < nMaxPlayer; nCntPlayer++)
	{// �v���C���[�J�E���g
	 // �v���C���[�ԍ�
		if (m_pPlayerNum[nCntPlayer] != NULL)
		{// NULL�ȊO
			m_pPlayerNum[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		// �I��
		if (m_pSelect[nCntPlayer] != NULL)
		{// NULL�ȊO
			m_pSelect[nCntPlayer]->SetPosSize(
				D3DXVECTOR3(
					(SCREEN_WIDTH * 0.5f) + ((SCREEN_WIDTH * 0.053f + (pnCharSelectNum[nCntPlayer] % 4 == 0 || pnCharSelectNum[nCntPlayer] % 4 == 3 ? (SCREEN_WIDTH * 0.053f) * 2.0f : 0.0f)) * (pnCharSelectNum[nCntPlayer] / 2 % 2 == 0 ? -1.0f : 1.0f)),
					(SCREEN_HEIGHT * 0.7f) + ((SCREEN_WIDTH * 0.053f) * (pnCharSelectNum[nCntPlayer] / 4 == 0 ? -1.0f : 1.0f)),
					0.0f),
				D3DXVECTOR2(SCREEN_WIDTH * 0.04f, SCREEN_WIDTH * 0.04f));
			m_pSelect[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}

		// ���萔�`���b�N
		if (m_bEnter[nCntPlayer] == true)
		{// ����
			nChackEnter++;

			// ����
			if (m_pEnter[nCntPlayer] != NULL)
			{
				m_pEnter[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_pEnter[nCntPlayer]->SetTexture(0, 1, 2, 2);
			}
		}
		else
		{// �I��
		 // ����
			if (m_pEnter[nCntPlayer] != NULL)
			{
				m_pEnter[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			}
		}

		// �S������
		if (nChackEnter == nMaxPlayer)
			CFade::Create(CGame::GAMEMODE_COURSE_VIEW);
	}

	int nEntryNum = 0;
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// �v���C���[�J�E���g
		if (m_bEntry[nCntPlayer] == true)
			nEntryNum++;
	}

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// �v���C���[�J�E���g
	 // ����
		if (nEntryNum <= nCntPlayer)
		{
			if (m_pEnter[nCntPlayer] != NULL)
			{
				int nCount = (m_nEntryCounter % 60);
				if (nCount < 20)
				{
					if (nCount < 10)
						m_pEnter[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - ((1.0f / 10.0f) * nCount)));
					else
						m_pEnter[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, ((1.0f / 10.0f) * (nCount - 10))));
				}
				else
				{
					m_pEnter[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
				m_pEnter[nCntPlayer]->SetTexture(1, 1, 2, 2);
			}
		}
	}

	if (nMaxPlayer != nEntryNum)
	{// �v���C���[�����ς����
		nMaxPlayer = nEntryNum;		// �v���C���[�����X�V

		for (int nCntPlayer = (nMaxPlayer - 1); 0 <= nCntPlayer; nCntPlayer--)
		{// �v���C���[�J�E���g
			while (1)
			{// �����ԍ����Ȃ���
				bool bCheck = false;
				for (int nCntCheck = (nMaxPlayer - 1); 0 <= nCntCheck; nCntCheck--)
				{// �`�F�b�N�J�E���g
					if (nCntPlayer != nCntCheck)
					{// �����ȊO
						if (pnCharSelectNum[nCntPlayer] == pnCharSelectNum[nCntCheck])
						{// �����ԍ���������
							bCheck = true;
							break;
						}
					}
				}

				// �ԍ���ύX or break
				if (bCheck == true)
					pnCharSelectNum[nCntPlayer] = (pnCharSelectNum[nCntPlayer] + 1) % MAX_CHARCTER;
				else
					break;
			}

			while (1)
			{// �����ԍ����Ȃ���
				bool bCheck = false;
				for (int nCntCheck = (nMaxPlayer - 1); 0 <= nCntCheck; nCntCheck--)
				{// �`�F�b�N�J�E���g
					if (nCntPlayer != nCntCheck)
					{// �����ȊO
						if (pnControllerNum[nCntPlayer] == pnControllerNum[nCntCheck])
						{// �����ԍ���������
							bCheck = true;
							break;
						}
					}
				}

				// �ԍ���ύX or break
				if (bCheck == true)
					pnControllerNum[nCntPlayer] = (pnControllerNum[nCntPlayer] + 1) % MAX_PLAYER;
				else
					break;
			}
		}
	}

	// ���ǂ�L����
	if (m_pReturnBG != NULL)
	{// NULL�ȊO
		m_pReturnBG->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, (m_nReturnCounter < 45 ? (1.0f * (float)((float)m_nReturnCounter / (float)45)) : 1.0f)));
	}
	// ���ǂ�L����
	if (m_pReturnChar != NULL)
	{// NULL�ȊO
		m_pReturnChar->SetPosSize(
			D3DXVECTOR3(
				((SCREEN_WIDTH * 0.5f) + (SCREEN_WIDTH * 0.07f) + -(SCREEN_WIDTH * 0.09f)) + (((SCREEN_WIDTH * 0.09f) * 2.0f) * (float)((float)m_nReturnCounter / (float)90)),
				(SCREEN_HEIGHT * 0.05f),
				0.0f),
			D3DXVECTOR2(SCREEN_HEIGHT * 0.03f, SCREEN_HEIGHT * 0.03f));
		m_pReturnChar->SetColor(&D3DXCOLOR(1.0f, 0.0f, 0.0f, (m_nReturnCounter < 45 ? (1.0f * (float)((float)m_nReturnCounter / (float)45)) : 1.0f)));
	}

	// �v���C���[�ő吔�ݒ�
	CGame::SetMaxPlayer(nMaxPlayer);
	// �L�����I��ԍ��ݒ�
	CGame::SetCharSelectNum(pnCharSelectNum);
	// �R���g���[���[�ݒ�
	CGame::SetControllerNum(pnControllerNum);

	m_nEntryCounter++;		// �J�E���g
}