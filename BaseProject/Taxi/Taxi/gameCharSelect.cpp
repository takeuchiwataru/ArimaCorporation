//=============================================================================
//
// �Q�[���i�L�����I���j�̏��� [game.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#include "gameCharSelect.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CGameCharSelect::CGameCharSelect()
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
		m_pPlayerBG[nCntPlayer] = NULL;

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

	return S_OK;
}

//=============================================================================
// �A�����[�h
//=============================================================================
void CGameCharSelect::Unload(void)
{

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
	// �v���C���[�w�i
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// �v���C���[�J�E���g
		if (m_pPlayerBG[nCntPlayer] == NULL)
		{// NULL
			m_pPlayerBG[nCntPlayer] = new CScene2D(0, CScene::OBJTYPE_2DPOLYGON);
			m_pPlayerBG[nCntPlayer]->Init();
			m_pPlayerBG[nCntPlayer]->SetPosSize(
				D3DXVECTOR3(
				(nCntPlayer % 2 == 0 ? 0.0f + (SCREEN_HEIGHT * 0.25f) : SCREEN_WIDTH - (SCREEN_HEIGHT * 0.25f)),
					(nCntPlayer / 2 == 0 ? 0.0f + (SCREEN_HEIGHT * 0.25f) : SCREEN_HEIGHT - (SCREEN_HEIGHT * 0.25f)), 0.0f),
				D3DXVECTOR2(SCREEN_HEIGHT * 0.25f, SCREEN_HEIGHT * 0.25f));
			m_pPlayerBG[nCntPlayer]->SetColor(&D3DXCOLOR((nCntPlayer % 2 == 0 ? 1.0f : 0.0f), (nCntPlayer / 2 % 2 == 1 ? 1.0f : 0.0f), (nCntPlayer % 4 == 1 ? 1.0f : 0.0f), 0.6f));
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
				(SCREEN_WIDTH * 0.5f) + ((SCREEN_WIDTH * 0.045f + (nCntChar % 4 == 0 || nCntChar % 4 == 3 ? (SCREEN_WIDTH * 0.045f) * 2.0f : 0.0f)) * (nCntChar / 2 % 2 == 0 ? -1.0f : 1.0f)),
					(SCREEN_HEIGHT * 0.8f) + ((SCREEN_WIDTH * 0.045f) * (nCntChar / 4 == 0 ? -1.0f : 1.0f)),
					0.0f),
				D3DXVECTOR2(SCREEN_WIDTH * 0.045f, SCREEN_WIDTH * 0.045f));
			//m_pCharacter[nCntChar]->SetColor(&D3DXCOLOR((nCntChar % 2 == 0 ? 1.0f : 0.0f), (nCntChar / 2 % 2 == 1 ? 1.0f : 0.0f), (nCntChar % 4 == 1 ? 1.0f : 0.0f), (nCntChar / 4 == 0 ? 1.0f : 0.8f)));
			m_pCharacter[nCntChar]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, (1.0f / MAX_CHARCTER) * nCntChar));
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
		if (m_pPlayerBG[nCntPlayer] != NULL)
		{// NULL�ȊO
			m_pPlayerBG[nCntPlayer]->Uninit();	// �I������
			m_pPlayerBG[nCntPlayer] = NULL;		// NULL��
		}
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
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputXPad * pXpad = CManager::GetXInput();					//�W���C�p�b�g�̎擾

	// �L�����I��ԍ��擾
	int *pnCharSelectNum = CGame::GetCharSelectNum();

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// �v���C���[�J�E���g


		if (pCInputKeyBoard->GetKeyboardTrigger(DIK_W) == true ||
			pXpad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_UP, nCntPlayer) == true ||
			pXpad->GetTrigger(XINPUT_GAMEPAD_DPAD_UP, nCntPlayer) == true)
		{// ��L�[
			if (pnCharSelectNum[nCntPlayer] / 4 == 1)
			{// �ړ�����
				// ���̒l
				int nNext = (pnCharSelectNum[nCntPlayer] + 4) % MAX_CHARCTER;

				for (int nCntSelect = 0; nCntSelect < MAX_PLAYER; nCntSelect++)
				{// �I���J�E���g
					if (nCntSelect != nCntPlayer)
					{// �����ȊO
						if (pnCharSelectNum[nCntSelect] == nNext)	// �l����v
							break;

						if (nCntSelect == MAX_PLAYER - 1)			// ��v���Ȃ�������X�V
							pnCharSelectNum[nCntPlayer] = nNext;
					}
				}

			}
		}
		else if (pCInputKeyBoard->GetKeyboardTrigger(DIK_S) == true ||
			pXpad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_DOWN, nCntPlayer) == true ||
			pXpad->GetTrigger(XINPUT_GAMEPAD_DPAD_DOWN, nCntPlayer) == true)
		{// ���L�[
			if (pnCharSelectNum[nCntPlayer] / 4 == 0)
			{// �ړ�����
				// ���̒l
				int nNext = (pnCharSelectNum[nCntPlayer] + 4) % MAX_CHARCTER;

				for (int nCntSelect = 0; nCntSelect < MAX_PLAYER; nCntSelect++)
				{// �I���J�E���g
					if (nCntSelect != nCntPlayer)
					{// �����ȊO
						if (pnCharSelectNum[nCntSelect] == nNext)	// �l����v
							break;

						if (nCntSelect == MAX_PLAYER - 1)			// ��v���Ȃ�������X�V
							pnCharSelectNum[nCntPlayer] = nNext;
					}
				}
			}
		}
		if (pCInputKeyBoard->GetKeyboardTrigger(DIK_A) == true ||
			pXpad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_LEFT, nCntPlayer) == true ||
			pXpad->GetTrigger(XINPUT_GAMEPAD_DPAD_LEFT, nCntPlayer) == true)
		{// ���L�[
			if (pnCharSelectNum[nCntPlayer] % 4 != 0)
			{// �ړ�����
				// ���̒l
				int nNext = pnCharSelectNum[nCntPlayer];

				for (int nCntLine = pnCharSelectNum[nCntPlayer] % 4 - 1; 0 <= nCntLine; nCntLine--)
				{// ���C���J�E���g
					nNext = (nNext - 1) % MAX_CHARCTER;

					for (int nCntSelect = 0; nCntSelect < MAX_PLAYER; nCntSelect++)
					{// �I���J�E���g
						if (nCntSelect != nCntPlayer)
						{// �����ȊO
							if (pnCharSelectNum[nCntSelect] == nNext)	// �l����v
								break;

							if (nCntSelect == MAX_PLAYER - 1)			// ��v���Ȃ�������X�V
								pnCharSelectNum[nCntPlayer] = nNext;
						}
					}

					if (pnCharSelectNum[nCntPlayer] == nNext)				// �X�V����
						break;
				}
			}
		}
		else if (pCInputKeyBoard->GetKeyboardTrigger(DIK_D) == true ||
			pXpad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_RIGHT, nCntPlayer) == true ||
			pXpad->GetTrigger(XINPUT_GAMEPAD_DPAD_RIGHT, nCntPlayer) == true)
		{// �E�L�[
			if (pnCharSelectNum[nCntPlayer] % 4 != (4 - 1))
			{// �ړ�����
				// ���̒l
				int nNext = pnCharSelectNum[nCntPlayer];

				for (int nCntLine = pnCharSelectNum[nCntPlayer] % 4 + 1; nCntLine < 4; nCntLine++)
				{// ���C���J�E���g	
					nNext = (nNext + 1) % MAX_CHARCTER;

					for (int nCntSelect = 0; nCntSelect < MAX_PLAYER; nCntSelect++)
					{// �I���J�E���g
						if (nCntSelect != nCntPlayer)
						{// �����ȊO
							if (pnCharSelectNum[nCntSelect] == nNext)	// �l����v
								break;

							if (nCntSelect == MAX_PLAYER - 1)			// ��v���Ȃ�������X�V
								pnCharSelectNum[nCntPlayer] = nNext;
						}
					}

					if (pnCharSelectNum[nCntPlayer] == nNext)				// �X�V����
						break;
				}
			}
		}
	}

	// �L�����N�^�[
	for (int nCntChar = 0; nCntChar < MAX_CHARCTER; nCntChar++)
	{// �L�����J�E���g
		if (m_pCharacter[nCntChar] != NULL)
		{// NULL�ȊO
			for (int nCntSelect = 0; nCntSelect < MAX_PLAYER; nCntSelect++)
			{
				if (pnCharSelectNum[nCntSelect] == nCntChar)
				{
					m_pCharacter[nCntChar]->SetColor(&D3DXCOLOR((nCntSelect % 2 == 0 ? 1.0f : 0.0f), (nCntSelect / 2 % 2 == 1 ? 1.0f : 0.0f), (nCntSelect % 4 == 1 ? 1.0f : 0.0f), (nCntSelect / 4 == 0 ? 1.0f : 0.8f)));
					break;
				}
				else
				{
					m_pCharacter[nCntChar]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, (1.0f / MAX_CHARCTER) * nCntSelect));
				}
			}
		}
	}

	// �L�����I��ԍ��ݒ�
	CGame::SetCharSelectNum(pnCharSelectNum);
}

//=============================================================================
// �`�揈��
//=============================================================================
void CGameCharSelect::Draw(void)
{

}