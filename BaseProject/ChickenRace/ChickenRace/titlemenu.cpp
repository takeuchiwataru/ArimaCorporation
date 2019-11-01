//=============================================================================
//
// �^�C�g���i���j���[�j�̏��� [titlemenu.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS

#include "titlemenu.h"
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
LPDIRECT3DTEXTURE9	CTitleMenu::m_pTexture[CTitleMenu::TEXTURE_MAX] = { NULL };

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTitleMenu::CTitleMenu()
{
	m_pPress = NULL;		// �v���X
	m_nCntPress = 0;		// �v���X�J�E���g

	for (int nCntMainMenu = 0; nCntMainMenu < MAX_MAINMENU; nCntMainMenu++)
		m_pMainMenu[nCntMainMenu] = NULL;		// ���C�����j���[
	m_nCntMainMenu = 0;							// ���C�����j���[�J�E���g

	m_nSelectNum = 0;		// �I��ԍ�

	m_bSubMenu = false;		// �T�u���j���[

	m_bEntry = false;		// ��t��
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTitleMenu::~CTitleMenu() {}

//=============================================================================
// ���[�h
//=============================================================================
HRESULT CTitleMenu::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{// �e�N�X�`���J�E���g
		char cName[128] = {};

		switch (nCntTex)
		{
		case TEXTURE_PRESS:
			strcpy(cName, "data\\TEXTURE\\Title\\UI\\PressButton.png");
			break;
		case TEXTURE_MAINMENU:
			strcpy(cName, "data\\TEXTURE\\Title\\UI\\modeselect_00.png");
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
void CTitleMenu::Unload(void)
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
CTitleMenu *CTitleMenu::Create(void)
{
	CTitleMenu *pTitleMenu = NULL;

	// �������m��
	if (pTitleMenu == NULL)
		pTitleMenu = new CTitleMenu;

	if (pTitleMenu != NULL)
	{// NULL�ȊO
		pTitleMenu->Init();	// ����������
	}

	return pTitleMenu;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTitleMenu::Init()
{
	// �v���X
	if (m_pPress == NULL)
	{// NULL
		m_pPress = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
		m_pPress->Init();
		m_pPress->SetPosSize(
			D3DXVECTOR3
			(
				SCREEN_WIDTH * 0.5f,
				SCREEN_HEIGHT * 0.8f,
				0.0f
			),
			D3DXVECTOR2(SCREEN_WIDTH * 0.13f, SCREEN_HEIGHT * 0.07f));
		m_pPress->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pPress->BindTexture(m_pTexture[TEXTURE_PRESS]);
	}

	for (int nCntMainMenu = 0; nCntMainMenu < MAX_MAINMENU; nCntMainMenu++)
	{// ���C�����j���[
		if (m_pMainMenu[nCntMainMenu] == NULL)
		{// NULL
			m_pMainMenu[nCntMainMenu] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
			m_pMainMenu[nCntMainMenu]->Init();
			m_pMainMenu[nCntMainMenu]->SetPosSize(
				D3DXVECTOR3
				(
					SCREEN_WIDTH * 0.5f - (SCREEN_WIDTH * 0.11f * 2.0f) + ((SCREEN_WIDTH * 0.11f * 2.0f) * nCntMainMenu),
					SCREEN_HEIGHT * 0.8f,
					0.0f
				),
				D3DXVECTOR2(SCREEN_WIDTH * 0.1f, SCREEN_WIDTH * 0.1f));
			m_pMainMenu[nCntMainMenu]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

			if (nCntMainMenu != (MAX_MAINMENU - 1))
			{// ���ǂ�ȊO
				m_pMainMenu[nCntMainMenu]->BindTexture(m_pTexture[TEXTURE_MAINMENU]);
				m_pMainMenu[nCntMainMenu]->SetTexture(nCntMainMenu, 2, 1, 1);
			}
			else
			{
				//m_pMainMenu[nCntMainMenu]->BindTexture(m_pTexture[TEXTURE_MAINMENU]);
			}
		}
	}

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CTitleMenu::Uninit(void)
{
	// �v���X
	if (m_pPress != NULL)
	{// NULL�ȊO
		m_pPress->Uninit();
		m_pPress = NULL;
	}

	for (int nCntMainMenu = 0; nCntMainMenu < MAX_MAINMENU; nCntMainMenu++)
	{// ���C�����j���[
		if (m_pMainMenu[nCntMainMenu] != NULL)
		{// NULL�ȊO
			m_pMainMenu[nCntMainMenu]->Uninit();
			m_pMainMenu[nCntMainMenu] = NULL;
		}
	}

	//���g�̍폜
	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTitleMenu::Update(void)
{
	// �擾
	int nCounter = CTitle::GetCounter();

	if (60 < nCounter)
	{// �^�C�g���\����
		if (CFade::GetFade() == CFade::FADE_NONE)
		{// �t�F�[�h���Ă��Ȃ�
			// �R���g���[��
			if (m_bEntry == false)
			{// ��t���Ă��Ȃ�
				ControllTitle();
				ControllMainMenu();
				ControllSubMenu();
			}
			else
			{// ��t��
				ControllEntry();
			}

			// �ҏW
			EditPress();
			EditMainMenu();
			EditSubMenu();
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTitleMenu::Draw(void)
{

}

//=============================================================================
// �R���g���[���^�C�g��
//=============================================================================
void CTitleMenu::ControllTitle(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputJoyPad_0 * pXpad = CManager::GetInputJoyPad0(0);		//�W���C�p�b�g�̎擾
	CSound *pSound = CManager::GetSound();						//�T�E���h�̏��

	// �擾
	bool bMenu = CTitle::GetMenu();

	if (m_nCntMainMenu == 0)
	{// �\��������Ȃ�
		if (bMenu == false)
		{// ���j���[��\��
			if (pCInputKeyBoard->GetKeyboardAny(1) == true || pXpad->GetAllTrigger() == true)
				bMenu = true;	// ���j���[���o��
		}
	}

	// �ݒ�
	CTitle::SetMenu(bMenu);
}

//=============================================================================
// �R���g���[�����C�����j���[
//=============================================================================
void CTitleMenu::ControllMainMenu(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputJoyPad_0 * pXpad = CManager::GetInputJoyPad0(0);		//�W���C�p�b�g�̎擾
	CSound *pSound = CManager::GetSound();						//�T�E���h�̏��

	// �I��邩
	bool bEnd = false;

	// �擾
	bool bMenu = CTitle::GetMenu();
	bool bOnline = CTitle::GetOnline();

	if (m_bSubMenu == false)
	{// �T�u���j���[��\��
		if (m_nCntPress == 0 && m_nCntMainMenu == (60 - 1))
		{// �\��������Ȃ�
			if (bMenu == true)
			{// ���j���[�\��
				if (pCInputKeyBoard->GetKeyboardTrigger(DIK_A) == true ||
					pCInputKeyBoard->GetKeyboardTrigger(DIK_LEFT) == true ||
					pXpad->GetTrigger(INPUT_LEFT) == true ||
					pXpad->GetTrigger(INPUT_LS_L) == true)
				{// ���L�[
					m_nSelectNum = (m_nSelectNum + (MAX_MAINMENU - 1)) % MAX_MAINMENU;
				}
				else if (pCInputKeyBoard->GetKeyboardTrigger(DIK_D) == true ||
					pCInputKeyBoard->GetKeyboardTrigger(DIK_RIGHT) == true ||
					pXpad->GetTrigger(INPUT_RIGHT) == true ||
					pXpad->GetTrigger(INPUT_LS_R) == true)
				{// �E�L�[
					m_nSelectNum = (m_nSelectNum + (1)) % MAX_MAINMENU;
				}

				if (pCInputKeyBoard->GetKeyboardTrigger(DIK_RETURN) == true ||
					pCInputKeyBoard->GetKeyboardTrigger(DIK_Z) == true ||
					pXpad->GetTrigger(INPUT_B) == true ||
					pXpad->GetTrigger(INPUT_START) == true)
				{// ����
					switch (m_nSelectNum)
					{// �I��ԍ�
					case 0:
						bOnline = false;	// ���[�J���ŗV��
						bEnd = true;		// �I���
						break;
					case 1:
						bOnline = true;		// �I�����C���ŗV��
						break;
					case 2:
						bMenu = false;		// ���j���[�����
						break;
					}
				}

				if (pCInputKeyBoard->GetKeyboardTrigger(DIK_X) == true ||
					pXpad->GetTrigger(INPUT_A) == true)
				{// �L�����Z��
					bMenu = false;			// ���j���[�����
				}
			}

			// �ݒ�
			CTitle::SetMenu(bMenu);
			CTitle::SetOnline(bOnline);

			if (bEnd == true)
			{// �I���
				CFade::Create(CManager::MODE_GAME);
			}
		}
	}
}

//=============================================================================
// �R���g���[���T�u���j���[
//=============================================================================
void CTitleMenu::ControllSubMenu(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputJoyPad_0 * pXpad = CManager::GetInputJoyPad0(0);		//�W���C�p�b�g�̎擾
	CSound *pSound = CManager::GetSound();						//�T�E���h�̏��

	// �I��邩
	bool bEnd = false;

	// �擾
	bool bMenu = CTitle::GetMenu();
	bool bOnline = CTitle::GetOnline();
	bool bHost = CTitle::GetHost();

	if (m_bSubMenu == true)
	{// �T�u���j���[�\��
		if (m_nCntPress == 0 && m_nCntMainMenu == (60 - 1))
		{// �\��������Ȃ�
			if (bMenu == true && bOnline == true)
			{// ���j���[�\��
				if (pCInputKeyBoard->GetKeyboardTrigger(DIK_A) == true ||
					pCInputKeyBoard->GetKeyboardTrigger(DIK_LEFT) == true ||
					pXpad->GetTrigger(INPUT_LEFT) == true ||
					pXpad->GetTrigger(INPUT_LS_L) == true)
				{// ���L�[
					m_nSelectNum = (m_nSelectNum + (MAX_MAINMENU - 1)) % MAX_MAINMENU;
				}
				else if (pCInputKeyBoard->GetKeyboardTrigger(DIK_D) == true ||
					pCInputKeyBoard->GetKeyboardTrigger(DIK_RIGHT) == true ||
					pXpad->GetTrigger(INPUT_RIGHT) == true ||
					pXpad->GetTrigger(INPUT_LS_R) == true)
				{// �E�L�[
					m_nSelectNum = (m_nSelectNum + (1)) % MAX_MAINMENU;
				}

				if (pCInputKeyBoard->GetKeyboardTrigger(DIK_RETURN) == true ||
					pCInputKeyBoard->GetKeyboardTrigger(DIK_Z) == true ||
					pXpad->GetTrigger(INPUT_B) == true ||
					pXpad->GetTrigger(INPUT_START) == true)
				{// ����
					switch (m_nSelectNum)
					{// �I��ԍ�
					case 0:
						bHost = true;		// �z�X�g�ɂȂ�
						bEnd = true;		// �I���
						break;
					case 1:
						bHost = false;		// �����o�[�ɂȂ�
						bEnd = true;		// �I���
						break;
					case 2:
						bOnline = false;	// �I�����C������߂�
						break;
					}
				}

				if (pCInputKeyBoard->GetKeyboardTrigger(DIK_X) == true ||
					pXpad->GetTrigger(INPUT_A) == true)
				{// �L�����Z��
					bOnline = false;		// �I�����C������߂�
				}
			}

			// �ݒ�
			CTitle::SetMenu(bMenu);
			CTitle::SetOnline(bOnline);
			CTitle::SetHost(bHost);

			if (bEnd == true)
			{// �I���
				CManager::OnlineSeting(true, bHost);	// �I�����C���ݒ�
				m_bEntry = true;
			}
		}
	}
}

//=============================================================================
// �R���g���[���G���g���[
//=============================================================================
void CTitleMenu::ControllEntry(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputJoyPad_0 * pXpad = CManager::GetInputJoyPad0(0);		//�W���C�p�b�g�̎擾
	CSound *pSound = CManager::GetSound();						//�T�E���h�̏��

	if (m_nCntMainMenu == 0)
	{// �\��������Ȃ�
		if (1 < CServer::GetnMaxClient())
		{// 1�l�ȏ�
			if (pCInputKeyBoard->GetKeyboardTrigger(DIK_RETURN) == true ||
				pCInputKeyBoard->GetKeyboardTrigger(DIK_Z) == true ||
				pXpad->GetTrigger(INPUT_START) == true)
			{// ����
				CFade::Create(CManager::MODE_GAME);		// �Q�[����
				return;
			}
		}

		if (pCInputKeyBoard->GetKeyboardTrigger(DIK_X) == true ||
			pXpad->GetTrigger(INPUT_A) == true)
		{// �L�����Z��
			m_bSubMenu = true;				// �T�u���j���[�\��				

			m_bEntry = false;				// ��t�I��
			CManager::OnlineSeting(false);	// �I�����C���ݒ�
		}
	}
}

//=============================================================================
// �v���X�ҏW
//=============================================================================
void CTitleMenu::EditPress(void)
{
	// �擾
	bool bMenu = CTitle::GetMenu();

	float fAlpha = 0.0f;		// �����x

	if (bMenu == false)
	{// ���j���[��\��
	 // �����x�v�Z
		fAlpha = 1.0f * (float)((float)(m_nCntPress % 60) / (float)(60));

		// ����
		if (m_nCntPress / 60 == 1)
			fAlpha = 1.0f - fAlpha;

		// �J�E���g
		m_nCntPress = (m_nCntPress + 1) % 120;
	}
	else
	{// ���j���[�\��
		if (m_nCntPress != 0 && m_nCntPress != 120)
		{// ��������Ȃ�
			// �����x�v�Z
			fAlpha = 1.0f * (float)((float)(m_nCntPress % 60) / (float)(60));

			// ����
			if (m_nCntPress / 60 == 1)
				fAlpha = 1.0f - fAlpha;

			// �J�E���g
			if (m_nCntPress < 60)
				m_nCntPress--;
			else
				m_nCntPress++;
		}
		else
		{// ����
			// �l����
			m_nCntPress = 0;
		}
	}

	// �v���X
	if (m_pPress != NULL)
	{// NULL�ȊO
		// �F�ݒ�
		m_pPress->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha));
	}
}

//=============================================================================
// ���C�����j���[�ҏW
//=============================================================================
void CTitleMenu::EditMainMenu(void)
{
	// �擾
	bool bMenu = CTitle::GetMenu();
	bool bOnline = CTitle::GetOnline();

	float fAlpha = 0.0f;		// �����x

	if (m_bSubMenu == false)
	{// �T�u���j���[��\��
		if (bMenu == false || bOnline == true)
		{// ���j���[��\��
			if (m_nCntMainMenu != 0)
			{// �O�ȊO
				// �����x�v�Z
				fAlpha = 1.0f * (float)((float)(m_nCntMainMenu % 60) / (float)(60));

				// �J�E���g
				m_nCntMainMenu--;
			}
			else
			{// ��\��
				// �l����
				fAlpha = 0.0f;
				m_nSelectNum = 0;

				// �I�����C���Ȃ�T�u���j���[�\��
				if (bOnline == true)
					m_bSubMenu = true;
			}
		}
		else
		{// ���j���[�\��
			if (m_nCntMainMenu != (60 - 1))
			{// �U�O�ȊO
				// �����x�v�Z
				fAlpha = 1.0f * (float)((float)(m_nCntMainMenu % 60) / (float)(60));

				// �J�E���g
				m_nCntMainMenu++;
			}
			else
			{// �\��
				fAlpha = 1.0f;
			}
		}

		// ���C�����j���[
		for (int nCntMainMenu = 0; nCntMainMenu < MAX_MAINMENU; nCntMainMenu++)
		{// ���C�����j���[�J�E���g
			if (m_pMainMenu[nCntMainMenu] != NULL)
			{// NULL�ȊO
				float fcol = 0.0f;

				// �F����
				if (m_nSelectNum == nCntMainMenu)	// �I��ԍ��ƈ�v
					fcol = 1.0f;
				else
					fcol = 0.3f;

				// �F�ݒ�
				m_pMainMenu[nCntMainMenu]->SetColor(&D3DXCOLOR(fcol, fcol, fcol, fAlpha));
			}
		}
	}
}

//=============================================================================
// �T�u���j���[�ҏW
//=============================================================================
void CTitleMenu::EditSubMenu(void)
{
	// �擾
	bool bMenu = CTitle::GetMenu();
	bool bOnline = CTitle::GetOnline();

	if (m_bSubMenu == true)
	{// �T�u���j���[�\��
		float fAlpha = 0.0f;		// �����x

		if (bMenu == false || bOnline == false || m_bEntry == true)
		{// ���j���[��\��
			if (m_nCntMainMenu != 0)
			{// �O�ȊO
			 // �����x�v�Z
				fAlpha = 1.0f * (float)((float)(m_nCntMainMenu % 60) / (float)(60));

				// �J�E���g
				m_nCntMainMenu--;
			}
			else
			{// ��\��
				// �l����
				fAlpha = 0.0f;
				m_nSelectNum = 0;

				// �I�����C������Ȃ��Ȃ�T�u���j���[��\��
				if (bOnline == false)
					m_bSubMenu = false;
			}
		}
		else
		{// �����x�\��
			if (m_nCntMainMenu != (60 - 1))
			{// �U�O�ȊO
			 // �����x�v�Z
				fAlpha = 1.0f * (float)((float)(m_nCntMainMenu % 60) / (float)(60));

				// �J�E���g
				m_nCntMainMenu++;
			}
			else
			{// �\��
				fAlpha = 1.0f;
			}
		}

		// ���C�����j���[
		for (int nCntMainMenu = 0; nCntMainMenu < MAX_MAINMENU; nCntMainMenu++)
		{// ���C�����j���[�J�E���g
			if (m_pMainMenu[nCntMainMenu] != NULL)
			{// NULL�ȊO
				float fcol = 0.0f;

				// �F����
				if (m_nSelectNum == nCntMainMenu)	// �I��ԍ��ƈ�v
					fcol = 1.0f;
				else
					fcol = 0.3f;

				// �F�ݒ�
				m_pMainMenu[nCntMainMenu]->SetColor(&D3DXCOLOR(0.0f, 0.0f, fcol, fAlpha));
			}
		}
	}
}