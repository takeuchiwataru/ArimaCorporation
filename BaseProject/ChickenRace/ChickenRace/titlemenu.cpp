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
	m_pLogo = NULL;			// ���S
	m_pEgg = NULL;			// ��
	m_pChicken = NULL;		// �`�L��

	for (int nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
		m_pFlag[nCntFlag] = NULL;			// ��

	for (int nCntChicks = 0; nCntChicks < MAX_CHICKS; nCntChicks++)
		m_pChicks[nCntChicks] = NULL;		// �Ђ悱

	for (int nCntPIYO = 0; nCntPIYO < MAX_PIYO; nCntPIYO++)
		m_pPIYO[nCntPIYO] = NULL;			// �s��

	m_pChicksHome = NULL;	// �Ђ悱��

	m_nCntLogo = 0;			// ���S�J�E���g

	m_pPress = NULL;		// �v���X
	m_nCntPress = 0;		// �v���X�J�E���g

	m_pMenuBG = NULL;		// ���j���[BG

	for (int nCntMainMenu = 0; nCntMainMenu < MAX_MAINMENU; nCntMainMenu++)
	{// �J�E���g
		m_pMainMenu[nCntMainMenu] = NULL;		// ���C�����j���[
		m_pNameMenu[nCntMainMenu] = NULL;		// ���j���[��
	}
	m_nCntMainMenu = 0;							// ���C�����j���[�J�E���g

	m_nSelectNum = 0;		// �I��ԍ�

	m_bSubMenu = false;		// �T�u���j���[

	m_pEntry = NULL;		// ��t��
	m_pHint = NULL;			// ����

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
		m_pMember[nCntPlayer] = NULL;			// �l��

	m_pEnter = NULL;		// ����
	m_pSearch = NULL;		// �T�[�`
	m_pCancel = NULL;		// �L�����Z��
	m_bEntry = false;		// ��t��
	m_nCntEntry = 0;		// ��t���J�E���g
	m_bSearch = false;		// �T�[�`
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
		case TEXTURE_LOGO:
			strcpy(cName, "data\\TEXTURE\\Title\\UI\\title.png");
			break;
		case TEXTURE_EGG:
			strcpy(cName, "data\\TEXTURE\\Title\\UI\\egg.png");
			break;
		case TEXTURE_FLAG:
			strcpy(cName, "data\\TEXTURE\\Title\\UI\\flag.png");
			break;
		case TEXTURE_PIYO:
			strcpy(cName, "data\\TEXTURE\\Title\\UI\\piyo.png");
			break;
		case TEXTURE_PRESS:
			strcpy(cName, "data\\TEXTURE\\Title\\UI\\PressButton.png");
			break;
		case TEXTURE_MENU:
			strcpy(cName, "data\\TEXTURE\\Title\\UI\\modeselect_00.png");
			break;
		case TEXTURE_NAME_M:
			strcpy(cName, "data\\TEXTURE\\Title\\UI\\modeselect.png");
			break;
		case TEXTURE_NAME_S:
			strcpy(cName, "data\\TEXTURE\\Title\\UI\\modeselect_13.png");
			break;
		case TEXTURE_INPUT:
			strcpy(cName, "data\\TEXTURE\\Title\\UI\\modeselect_15.png");
			break;
		case TEXTURE_RETURN:
			strcpy(cName, "data\\TEXTURE\\Title\\UI\\modeselect_19.png");
			break;
		case TEXTURE_ENTRY:
			strcpy(cName, "data\\TEXTURE\\Title\\UI\\modeselect_16.png");
			break;
		case TEXTURE_HINT:
			strcpy(cName, "data\\TEXTURE\\Title\\UI\\modeselect_11.png");
			break;
		case TEXTURE_NUM:
			strcpy(cName, "data\\TEXTURE\\Title\\UI\\chiken_icon.png");
			break;
		case TEXTURE_SETING:
			strcpy(cName, "data\\TEXTURE\\Title\\UI\\modeselect_18.png");
			break;
		case TEXTURE_SEARCH:
			strcpy(cName, "data\\TEXTURE\\Title\\UI\\modeselect_10.png");
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
	// ���S
	if (m_pLogo == NULL)
	{// NULL
		m_pLogo = new CScene2D(6, CScene::OBJTYPE_2DPOLYGON);
		m_pLogo->Init();
		m_pLogo->SetPosSize(
			D3DXVECTOR3
			(
				SCREEN_WIDTH * 0.5f,
				SCREEN_HEIGHT * 0.34f,
				0.0f
			),
			D3DXVECTOR2(SCREEN_WIDTH * 0.4f, SCREEN_HEIGHT * 0.15f));
		m_pLogo->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pLogo->BindTexture(m_pTexture[TEXTURE_LOGO]);
		m_pLogo->SetTexture(0, 1, 2, 1);
	}

	// ��
	if (m_pEgg == NULL)
	{// NULL
		m_pEgg = new CScene2D(6, CScene::OBJTYPE_2DPOLYGON);
		m_pEgg->Init();
		m_pEgg->SetPosSize(
			D3DXVECTOR3
			(
				SCREEN_WIDTH * 0.245f,
				SCREEN_HEIGHT * 0.342f,
				0.0f
			),
			D3DXVECTOR2(SCREEN_WIDTH * 0.0325f, SCREEN_WIDTH * 0.038f));
		m_pEgg->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		m_pEgg->BindTexture(m_pTexture[TEXTURE_EGG]);
		m_pEgg->SetTexture(0, 2, 1, 1);
	}

	// �`�L��
	if (m_pChicken == NULL)
	{// NULL
		m_pChicken = new CScene2D(6, CScene::OBJTYPE_2DPOLYGON);
		m_pChicken->Init();
		m_pChicken->SetPosSize(
			D3DXVECTOR3
			(
				SCREEN_WIDTH * 0.322f,
				-SCREEN_WIDTH * 0.05f,
				0.0f
			),
			D3DXVECTOR2(SCREEN_WIDTH * 0.05f, SCREEN_WIDTH * 0.05f));
		m_pChicken->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pChicken->BindTexture(m_pTexture[TEXTURE_NUM]);
		m_pChicken->SetTexture(0, 3, 1, 1);
	}

	// ��
	for (int nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
	{// ���J�E���g
		m_pFlag[nCntFlag] = new CScene2D(6, CScene::OBJTYPE_2DPOLYGON);
		m_pFlag[nCntFlag]->Init();
		m_pFlag[nCntFlag]->SetPosSize(
			D3DXVECTOR3
			(
			(nCntFlag == 0 ? SCREEN_WIDTH * 0.308f : SCREEN_WIDTH * 0.368f),
				((nCntFlag == 0 ? SCREEN_HEIGHT * 0.25f : SCREEN_HEIGHT * 0.255f) - ((SCREEN_HEIGHT * 0.2f) + (SCREEN_WIDTH * 0.05f))),
				0.0f
			),
			D3DXVECTOR2(SCREEN_WIDTH * 0.03f, SCREEN_WIDTH * 0.05f));
		m_pFlag[nCntFlag]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pFlag[nCntFlag]->BindTexture(m_pTexture[TEXTURE_FLAG]);

		if (nCntFlag == 0)
		{
			m_pFlag[nCntFlag]->SetRot(3.14f + 0.5f, D3DXVECTOR2(SCREEN_WIDTH * 0.03f, SCREEN_WIDTH * 0.05f));
		}
		else
		{
			m_pFlag[nCntFlag]->SetRot(3.14f - 0.5f, D3DXVECTOR2(SCREEN_WIDTH * 0.03f, SCREEN_WIDTH * 0.05f));
			m_pFlag[nCntFlag]->SetUV_X_ROLL();
		}
	}

	for (int nCntChicks = 0; nCntChicks < MAX_CHICKS; nCntChicks++)
	{// �Ђ悱�J�E���g
	 // �Ђ悱
		if (m_pChicks[nCntChicks] == NULL)
		{// NULL
			m_pChicks[nCntChicks] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
			m_pChicks[nCntChicks]->Init();
			m_pChicks[nCntChicks]->SetPosSize(
				D3DXVECTOR3
				(
					SCREEN_WIDTH * 0.845f + ((SCREEN_WIDTH * 0.015f) * (nCntChicks == 0 ? -1 : 1)),
					SCREEN_HEIGHT * 0.24f,
					0.0f
				),
				D3DXVECTOR2(SCREEN_WIDTH * 0.02f, SCREEN_WIDTH * 0.02f));
			m_pChicks[nCntChicks]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_pChicks[nCntChicks]->BindTexture(m_pTexture[TEXTURE_NUM]);
			m_pChicks[nCntChicks]->SetTexture(2, 3, 1, 1);
		}
	}

	for (int nCntPIYO = 0; nCntPIYO < MAX_PIYO; nCntPIYO++)
	{
		// �s��
		if (m_pPIYO[nCntPIYO] == NULL)
		{// NULL
			m_pPIYO[nCntPIYO] = new CScene2D(6, CScene::OBJTYPE_2DPOLYGON);
			m_pPIYO[nCntPIYO]->Init();
			m_pPIYO[nCntPIYO]->SetPosSize(
				D3DXVECTOR3
				(
					SCREEN_WIDTH * 0.85f + ((nCntPIYO / 2 == 0 ? (SCREEN_WIDTH * 0.021f) : (SCREEN_WIDTH * 0.05f)) * (nCntPIYO % 2 == 0 ? -1 : 1)),
					(nCntPIYO / 2 == 0 ? (SCREEN_HEIGHT * 0.14f) : (SCREEN_HEIGHT * 0.18f)),
					0.0f
				),
				D3DXVECTOR2(SCREEN_WIDTH * 0.015f, SCREEN_WIDTH * 0.01f));
			m_pPIYO[nCntPIYO]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_pPIYO[nCntPIYO]->BindTexture(m_pTexture[TEXTURE_PIYO]);
			m_pPIYO[nCntPIYO]->SetTexture(0, 2, 1, 1);
			m_pPIYO[nCntPIYO]->SetRot(3.14f + (nCntPIYO % 2 == 0 ? 0.2f : -0.1f), D3DXVECTOR2(SCREEN_WIDTH * 0.015f, SCREEN_WIDTH * 0.01f));
		}
	}

	// �Ђ悱��
	if (m_pChicksHome == NULL)
	{// NULL
		m_pChicksHome = new CScene2D(6, CScene::OBJTYPE_2DPOLYGON);
		m_pChicksHome->Init();
		m_pChicksHome->SetPosSize(
			D3DXVECTOR3
			(
				SCREEN_WIDTH * 0.85f,
				-SCREEN_WIDTH * 0.05f,
				0.0f
			),
			D3DXVECTOR2(SCREEN_WIDTH * 0.04f, SCREEN_WIDTH * 0.04f));
		m_pChicksHome->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pChicksHome->BindTexture(m_pTexture[TEXTURE_EGG]);
		m_pChicksHome->SetTexture(1, 2, 1, 1);
	}

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
			D3DXVECTOR2(SCREEN_WIDTH * 0.14f, SCREEN_HEIGHT * 0.06f));
		m_pPress->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pPress->BindTexture(m_pTexture[TEXTURE_PRESS]);
	}

	// ���j���[BG
	if (m_pMenuBG == NULL)
	{// NULL
		m_pMenuBG = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
		m_pMenuBG->Init();
		m_pMenuBG->SetPosSize(
			D3DXVECTOR3
			(
				SCREEN_WIDTH * 0.5f,
				SCREEN_HEIGHT * (1.0f - 0.25f),
				0.0f
			),
			D3DXVECTOR2(SCREEN_WIDTH * 0.38f, SCREEN_HEIGHT * 0.25f));
		m_pMenuBG->SetColor(&D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	}

	for (int nCntMainMenu = 0; nCntMainMenu < MAX_MAINMENU; nCntMainMenu++)
	{// ���C�����j���[�J�E���g
		// ���C�����j���[
		if (m_pMainMenu[nCntMainMenu] == NULL)
		{// NULL
			m_pMainMenu[nCntMainMenu] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
			m_pMainMenu[nCntMainMenu]->Init();
			m_pMainMenu[nCntMainMenu]->SetPosSize(
				D3DXVECTOR3
				(
					SCREEN_WIDTH * 0.5f - (SCREEN_WIDTH * 0.08f * 2.5f) + ((SCREEN_WIDTH * 0.08f * 2.5f) * nCntMainMenu),
					SCREEN_HEIGHT * 0.7f,
					0.0f
				),
				D3DXVECTOR2(SCREEN_WIDTH * 0.08f, SCREEN_WIDTH * 0.08f));
			m_pMainMenu[nCntMainMenu]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_pMainMenu[nCntMainMenu]->BindTexture(m_pTexture[TEXTURE_MENU]);
			m_pMainMenu[nCntMainMenu]->SetTexture(nCntMainMenu, 3, 2, 1);
		}
		// ���j���[��
		if (m_pNameMenu[nCntMainMenu] == NULL)
		{// NULL
			m_pNameMenu[nCntMainMenu] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
			m_pNameMenu[nCntMainMenu]->Init();
			m_pNameMenu[nCntMainMenu]->SetPosSize(
				D3DXVECTOR3
				(
					SCREEN_WIDTH * 0.5f - (SCREEN_WIDTH * 0.08f * 2.5f) + ((SCREEN_WIDTH * 0.08f * 2.5f) * nCntMainMenu),
					SCREEN_HEIGHT * 0.9f,
					0.0f
				),
				D3DXVECTOR2(SCREEN_WIDTH * 0.088f, SCREEN_WIDTH * 0.025f));
			m_pNameMenu[nCntMainMenu]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			if (nCntMainMenu != MAX_MAINMENU - 1)
			{// �߂�ȊO
				m_pNameMenu[nCntMainMenu]->BindTexture(m_pTexture[TEXTURE_NAME_M]);
				m_pNameMenu[nCntMainMenu]->SetTexture(nCntMainMenu, 1, 2, 1);
			}
			else
			{// �߂�
				m_pNameMenu[nCntMainMenu]->SetPosSize(
					D3DXVECTOR3
					(
						SCREEN_WIDTH * 0.5f - (SCREEN_WIDTH * 0.08f * 2.5f) + ((SCREEN_WIDTH * 0.08f * 2.5f) * nCntMainMenu),
						SCREEN_HEIGHT * 0.9f,
						0.0f
					),
					//D3DXVECTOR2(SCREEN_WIDTH * 0.13f, SCREEN_WIDTH * 0.025f));
					D3DXVECTOR2(SCREEN_WIDTH * 0.048f, SCREEN_WIDTH * 0.021f));
				m_pNameMenu[nCntMainMenu]->BindTexture(m_pTexture[TEXTURE_RETURN]);
			}
		}
	}

	// �G���g���[
	if (m_pEntry == NULL)
	{// NULL
		m_pEntry = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
		m_pEntry->Init();
		m_pEntry->SetPosSize(
			D3DXVECTOR3
			(
				SCREEN_WIDTH * 0.25f,
				SCREEN_HEIGHT * 0.58f,
				0.0f
			),
			D3DXVECTOR2(SCREEN_WIDTH * 0.1f, SCREEN_WIDTH * 0.047f));
		m_pEntry->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		m_pEntry->BindTexture(m_pTexture[TEXTURE_ENTRY]);
	}

	// ����
	if (m_pHint == NULL)
	{// NULL
		m_pHint = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
		m_pHint->Init();
		m_pHint->SetPosSize(
			D3DXVECTOR3
			(
				SCREEN_WIDTH * 0.58f,
				SCREEN_HEIGHT * 0.58f,
				0.0f
			),
			D3DXVECTOR2(SCREEN_WIDTH * 0.15f, SCREEN_WIDTH * 0.02f));
		m_pHint->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		m_pHint->BindTexture(m_pTexture[TEXTURE_HINT]);
	}

	// �l��
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// �v���C���[�J�E���g
		if (m_pMember[nCntPlayer] == NULL)
		{// NULL
			m_pMember[nCntPlayer] = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
			m_pMember[nCntPlayer]->Init();
			m_pMember[nCntPlayer]->SetPosSize(
				D3DXVECTOR3
				(
					SCREEN_WIDTH * 0.5f - ((SCREEN_WIDTH * 0.06f) + ((SCREEN_WIDTH * 0.06f) * 2.2f)) + (((SCREEN_WIDTH * 0.06f) * 2.2f) * nCntPlayer),
					SCREEN_HEIGHT * 0.75f,
					0.0f
				),
				D3DXVECTOR2(SCREEN_WIDTH * 0.06f, SCREEN_WIDTH * 0.06f));
			m_pMember[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_pMember[nCntPlayer]->BindTexture(m_pTexture[TEXTURE_NUM]);
			m_pMember[nCntPlayer]->SetTexture((nCntPlayer == 0 ? 1 : 0), 3, 1, 1);
		}
	}

	// ����
	if (m_pEnter == NULL)
	{// NULL
		m_pEnter = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
		m_pEnter->Init();
		m_pEnter->SetPosSize(
			D3DXVECTOR3
			(
				SCREEN_WIDTH * 0.45f,
				SCREEN_HEIGHT * 0.9f,
				0.0f
			),
			D3DXVECTOR2(SCREEN_WIDTH * 0.12f, SCREEN_WIDTH * 0.025f));
		m_pEnter->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		m_pEnter->BindTexture(m_pTexture[TEXTURE_INPUT]);
		m_pEnter->SetTexture(1, 1, 2, 1);
	}

	// �L�����Z��
	if (m_pCancel == NULL)
	{// NULL
		m_pCancel = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
		m_pCancel->Init();
		m_pCancel->SetPosSize(
			D3DXVECTOR3
			(
				SCREEN_WIDTH * 0.7f,
				SCREEN_HEIGHT * 0.9f,
				0.0f
			),
			D3DXVECTOR2(SCREEN_WIDTH * 0.13f, SCREEN_WIDTH * 0.025f));
		m_pCancel->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		m_pCancel->BindTexture(m_pTexture[TEXTURE_INPUT]);
		m_pCancel->SetTexture(0, 1, 2, 1);
	}

	// �T�[�`
	if (m_pSearch == NULL)
	{// NULL
		m_pSearch = new CScene2D(5, CScene::OBJTYPE_2DPOLYGON);
		m_pSearch->Init();
		m_pSearch->SetPosSize(
			D3DXVECTOR3
			(
				SCREEN_WIDTH * 0.5f,
				SCREEN_HEIGHT * 0.7f,
				0.0f
			),
			D3DXVECTOR2(SCREEN_WIDTH * 0.2f, SCREEN_WIDTH * 0.025f));
		m_pSearch->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		m_pSearch->BindTexture(m_pTexture[TEXTURE_SEARCH]);
		m_pSearch->SetTexture(0, 1, 2, 1);
	}

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CTitleMenu::Uninit(void)
{
	// ���S
	if (m_pLogo != NULL)
	{// NULL�ȊO
		m_pLogo->Uninit();
		m_pLogo = NULL;
	}

	// ��
	if (m_pEgg != NULL)
	{// NULL�ȊO
		m_pEgg->Uninit();
		m_pEgg = NULL;
	}

	// �`�L��
	if (m_pChicken != NULL)
	{// NULL�ȊO
		m_pChicken->Uninit();
		m_pChicken = NULL;
	}

	// ��
	for (int nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
	{// ���J�E���g
		if (m_pFlag[nCntFlag] != NULL)
		{// NULL�ȊO
			m_pFlag[nCntFlag]->Uninit();
			m_pFlag[nCntFlag] = NULL;
		}
	}

	for (int nCntChicks = 0; nCntChicks < MAX_CHICKS; nCntChicks++)
	{// �Ђ悱�J�E���g
	 // �Ђ悱
		if (m_pChicks[nCntChicks] != NULL)
		{// NULL�ȊO
			m_pChicks[nCntChicks]->Uninit();
			m_pChicks[nCntChicks] = NULL;
		}
	}
	for (int nCntPIYO = 0; nCntPIYO < MAX_PIYO; nCntPIYO++)
	{// �s���J�E���g
	 // �s��
		if (m_pPIYO[nCntPIYO] != NULL)
		{// NULL�ȊO
			m_pPIYO[nCntPIYO]->Uninit();
			m_pPIYO[nCntPIYO] = NULL;
		}
	}

	// �Ђ悱��
	if (m_pChicksHome != NULL)
	{// NULL�ȊO
		m_pChicksHome->Uninit();
		m_pChicksHome = NULL;
	}

	// �v���X
	if (m_pPress != NULL)
	{// NULL�ȊO
		m_pPress->Uninit();
		m_pPress = NULL;
	}

	//	���j���[BG
	if (m_pMenuBG != NULL)
	{// NULL�ȊO
		m_pMenuBG->Uninit();
		m_pMenuBG = NULL;
	}

	for (int nCntMainMenu = 0; nCntMainMenu < MAX_MAINMENU; nCntMainMenu++)
	{// ���C�����j���[
		if (m_pMainMenu[nCntMainMenu] != NULL)
		{// NULL�ȊO
			m_pMainMenu[nCntMainMenu]->Uninit();
			m_pMainMenu[nCntMainMenu] = NULL;
		}
		if (m_pNameMenu[nCntMainMenu] != NULL)
		{// NULL�ȊO
			m_pNameMenu[nCntMainMenu]->Uninit();
			m_pNameMenu[nCntMainMenu] = NULL;
		}
	}

	// �G���g���[
	if (m_pEntry != NULL)
	{// NULL�ȊO
		m_pEntry->Uninit();
		m_pEntry = NULL;
	}
	// ����
	if (m_pHint != NULL)
	{// NULL�ȊO
		m_pHint->Uninit();
		m_pHint = NULL;
	}
	// �l��
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// �v���C���[�J�E���g
		if (m_pMember[nCntPlayer] != NULL)
		{// NULL�ȊO
			m_pMember[nCntPlayer]->Uninit();
			m_pMember[nCntPlayer] = NULL;
		}
	}
	// ����
	if (m_pEnter != NULL)
	{// NULL�ȊO
		m_pEnter->Uninit();
		m_pEnter = NULL;
	}
	// �L�����Z��
	if (m_pCancel != NULL)
	{// NULL�ȊO
		m_pCancel->Uninit();
		m_pCancel = NULL;
	}
	// �T�[�`
	if (m_pSearch != NULL)
	{// NULL�ȊO
		m_pSearch->Uninit();
		m_pSearch = NULL;
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

	// �ҏW
	EditLogo();
	EditEgg();
	EditChicken();
	EditFlag();
	EditChicks();
	EditPIYO();
	EditChicksHome();

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
			EditMenuBG();
			EditMainMenu();
			EditSubMenu();
			EditEntry();
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
			if (pCInputKeyBoard->GetKeyboardAny(1) == true || pXpad->GetAllTrigger() == true ||
				CManager::GetAging() == true)
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
					pXpad->GetTrigger(INPUT_R2) == true ||
					pXpad->GetTrigger(INPUT_START) == true ||
					CManager::GetAging() == true)
				{// ����
					if (CManager::GetAging() == true)
						m_nSelectNum = 0;

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
					pXpad->GetTrigger(INPUT_A) == true ||
					pXpad->GetTrigger(INPUT_L2) == true)
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
		if (m_nCntPress == 0 && m_nCntMainMenu == (60 - 1) && m_nCntEntry == 0)
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
					pXpad->GetTrigger(INPUT_R2) == true ||
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
					pXpad->GetTrigger(INPUT_A) == true ||
					pXpad->GetTrigger(INPUT_L2) == true)
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
			 // �T�[�`
				if (m_pSearch != NULL)
				{// NULL�ȊO
					if (bHost == true)
						m_pSearch->BindTexture(m_pTexture[TEXTURE_SETING]);
					else
						m_pSearch->BindTexture(m_pTexture[TEXTURE_SEARCH]);

					m_pSearch->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					m_pSearch->SetTexture(0, 1, 2, 1);
				}

				// ��t
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

	int nClient = CServer::GetnMaxClient();

	if (m_nCntMainMenu == 0/* && m_nCntEntry == (60 - 1)*/)
	{// �\��������Ȃ�
		if (1 < nClient)
		{// 1�l�ȏ�
			if (pXpad->GetTrigger(INPUT_B) == true ||
				pXpad->GetTrigger(INPUT_START) == true)
			{// ����
				CFade::Create(CManager::MODE_GAME);		// �Q�[����
				return;
			}
		}
		else if (nClient < 0)
		{
			if (m_nCntEntry == (60 - 1))
			{
				m_bSubMenu = true;				// �T�u���j���[�\��				

				m_bEntry = false;				// ��t�I��
				m_bSearch = false;				// �T�[�`
				CManager::OnlineSeting(false);	// �I�����C���ݒ�
				return;
			}
		}
	}

	for (int nCntPlayer = 0; nCntPlayer < nClient; nCntPlayer++)
	{
		pXpad = CManager::GetInputJoyPad0(nCntPlayer);

		if (pCInputKeyBoard->GetKeyboardTrigger(DIK_X) == true ||
			pXpad->GetTrigger(INPUT_A) == true ||
			pXpad->GetTrigger(INPUT_L2) == true)
		{// �L�����Z��
			m_bSubMenu = true;				// �T�u���j���[�\��				

			m_bEntry = false;				// ��t�I��
			m_bSearch = false;				// �T�[�`
			CManager::OnlineSeting(false);	// �I�����C���ݒ�
			return;
		}
	}
}

//=============================================================================
// ���S�ҏW
//=============================================================================
void CTitleMenu::EditLogo(void)
{
	// �擾
	int nCounter = CTitle::GetCounter();
	int nNum = nCounter;

	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.34f, 0.0f);
	D3DXVECTOR2 size = D3DXVECTOR2((SCREEN_WIDTH * 0.4f), (SCREEN_HEIGHT * 0.1f));

	if (nCounter < 60)
	{// 60�ȉ�
		if (nCounter < 30)
		{// 30�ȉ�
			size.x = (SCREEN_WIDTH * 0.4f) * (float)((float)nCounter / (float)30);
			size.y = (SCREEN_HEIGHT * 0.1f) * (float)((float)nCounter / (float)30);
		}
	}
	else if (nCounter < 80)
	{// 80�ȉ�
		nNum = nCounter - 60;
		int nNum2 = nNum % 10;

		pos.y = (SCREEN_HEIGHT * (0.34f + ((nNum / 10 == 0 ? nNum2 : 10 - nNum2) * 0.002f)));
		size.y = (SCREEN_HEIGHT * (0.1f - ((nNum / 10 == 0 ? nNum2 : 10 - nNum2) * 0.002f)));
	}
	else
	{// ���̑�
		nNum = nCounter - 80;
		int nType = 0;

		if ((nNum % 240) < 120)
			nType = 0;
		else
			nType = 1;

		if (m_pLogo != NULL)
			m_pLogo->SetTexture(nType, 1, 2, 1);
	}

	if (m_pLogo != NULL)
		m_pLogo->SetPosSize(pos, size);
}

//=============================================================================
// ���ҏW
//=============================================================================
void CTitleMenu::EditEgg(void)
{
	// �擾
	int nCounter = CTitle::GetCounter();
	int nNum = nCounter;

	if (80 <= nCounter)
	{// 80�̂�
		nNum = nCounter - 80;
		int nNum2 = nNum % 120;

		float fcol = 0.0f;
		float frot = 0.0f;

		if ((nNum % 240) < 120)
			fcol = 0.0f;
		else
		{
			fcol = 1.0f;

			if (nNum2 < 30)
				frot = (nNum2 * 0.005f);
			else if (nNum2 < 90)
				frot = (30 * 0.005f) + -((nNum2 - 30) * 0.005f);
			else
				frot = -(30 * 0.005f) + ((nNum2 - 90) * 0.005f);

			if (m_pEgg != NULL)
				m_pEgg->SetRot(3.14f + frot, D3DXVECTOR2(SCREEN_WIDTH * 0.0325f, SCREEN_WIDTH * 0.038f));
		}

		if (m_pEgg != NULL)
			m_pEgg->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, fcol));
	}
}

//=============================================================================
// �`�L���ҏW
//=============================================================================
void CTitleMenu::EditChicken(void)
{
	// �擾
	int nCounter = CTitle::GetCounter();
	int nNum = nCounter;

	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.322f, -SCREEN_WIDTH * 0.05f, 0.0f);
	D3DXVECTOR2 size = D3DXVECTOR2(SCREEN_WIDTH * 0.05f, SCREEN_WIDTH * 0.05f);
	// �ڕW�ʒu�ix:(SCREEN_WIDTH * 0.322f), y:(SCREEN_HEIGHT * 0.2f)�j

	if (nCounter < 60)
	{// 60�ȉ�
		if (30 <= nCounter)
		{// 30�ȉ�
			nNum = nCounter - 30 + 1;

			pos.y = (-SCREEN_WIDTH * 0.05f) + (((SCREEN_HEIGHT * 0.2f) + (SCREEN_WIDTH * 0.05f)) * (float)((float)nNum / (float)30));
		}
	}
	else
	{// ���̑�
		pos.y = (SCREEN_HEIGHT * 0.2f);

		if (nCounter < 80)
		{// 80�ȉ�
			nNum = nCounter - 60;
			int nNum2 = nNum % 10;

			pos.y = (SCREEN_HEIGHT * (0.2f + ((nNum / 10 == 0 ? nNum2 : 10 - nNum2) * 0.003f)));
		}
	}

	if (m_pChicken != NULL)
		m_pChicken->SetPosSize(pos, size);
}

//=============================================================================
// ���ҏW
//=============================================================================
void CTitleMenu::EditFlag(void)
{
	// �擾
	int nCounter = CTitle::GetCounter();
	int nNum = nCounter;

	float frot = 0.5f;

	D3DXVECTOR3 posL = D3DXVECTOR3(SCREEN_WIDTH * 0.308f, (SCREEN_HEIGHT * 0.25f - ((SCREEN_HEIGHT * 0.2f) + (SCREEN_WIDTH * 0.05f))), 0.0f);
	D3DXVECTOR3 posR = D3DXVECTOR3(SCREEN_WIDTH * 0.368f, (SCREEN_HEIGHT * 0.255f - ((SCREEN_HEIGHT * 0.2f) + (SCREEN_WIDTH * 0.05f))), 0.0f);
	D3DXVECTOR2 size = D3DXVECTOR2(SCREEN_WIDTH * 0.03f, SCREEN_WIDTH * 0.05f);
	// �ڕW�ʒu�ix:(SCREEN_WIDTH * 0.308f), y:(SCREEN_HEIGHT * 0.25f)�j
	// �ڕW�ʒu�ix:(SCREEN_WIDTH * 0.368f), y:(SCREEN_HEIGHT * 0.255f)�j

	if (nCounter < 60)
	{// 60�ȉ�
		if (30 <= nCounter)
		{// 30�ȉ�
			nNum = nCounter - 30 + 1;

			posL.y = (SCREEN_HEIGHT * 0.25f - ((SCREEN_HEIGHT * 0.2f) + (SCREEN_WIDTH * 0.05f))) + (((SCREEN_HEIGHT * 0.2f) + (SCREEN_WIDTH * 0.05f)) * (float)((float)nNum / (float)30));
			posR.y = (SCREEN_HEIGHT * 0.255f - ((SCREEN_HEIGHT * 0.2f) + (SCREEN_WIDTH * 0.05f))) + (((SCREEN_HEIGHT * 0.2f) + (SCREEN_WIDTH * 0.05f)) * (float)((float)nNum / (float)30));
		}
	}
	else
	{// ���̑�
		posL.y = (SCREEN_HEIGHT * 0.25f);
		posR.y = (SCREEN_HEIGHT * 0.255f);

		if (nCounter < 80)
		{// 80�ȉ�
			nNum = nCounter - 60;
			int nNum2 = nNum % 10;

			posL.y = (SCREEN_HEIGHT * (0.25f + ((nNum / 10 == 0 ? nNum2 : 10 - nNum2) * 0.003f)));
			posR.y = (SCREEN_HEIGHT * (0.255f + ((nNum / 10 == 0 ? nNum2 : 10 - nNum2) * 0.003f)));
		}
		else
		{// ���̑�
			if (90 <= nCounter)
			{// 90�ȏ�
				nNum = nCounter - 90;
				int nNum2 = nNum % 20;

				// ��]
				if ((nNum / 20) % 2 == 0)
					frot = 0.5f - (0.2f * (float)((float)nNum2 / (float)20));
				else
					frot = 0.3f + (0.2f * (float)((float)nNum2 / (float)20));
			}
		}
	}

	for (int nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
	{
		if (m_pFlag[nCntFlag] != NULL)
		{
			m_pFlag[nCntFlag]->SetPosSize((nCntFlag == 0 ? posL : posR), size);
			m_pFlag[nCntFlag]->SetRot(3.14f + (nCntFlag == 0 ? frot : -frot), D3DXVECTOR2(SCREEN_WIDTH * 0.03f, SCREEN_WIDTH * 0.05f));
		}
	}
}

//=============================================================================
// �Ђ悱�ҏW
//=============================================================================
void CTitleMenu::EditChicks(void)
{
	// �擾
	int nCounter = CTitle::GetCounter();
	int nNum = nCounter;

	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.845f, SCREEN_HEIGHT * 0.24f, 0.0f);
	D3DXVECTOR2 size = D3DXVECTOR2(SCREEN_WIDTH * 0.02f, SCREEN_WIDTH * 0.02f);
	// �ڕW�ʒu�ix:(SCREEN_WIDTH * 0.845f), y:(SCREEN_HEIGHT * 0.21f)�j

	float fAlpha = 0.0f;

	if (90 <= nCounter)
	{// 90�ȏ�
		pos.y = (SCREEN_HEIGHT * 0.21f);
		fAlpha = 1.0f;

		if (nCounter < 100)
		{// 100�ȉ�
			nNum = nCounter - 90;
			pos.y = (SCREEN_HEIGHT * 0.24f) - ((SCREEN_HEIGHT * (0.24f - 0.21f)) * (float)((float)nNum / (float)10));
		}
		else
		{// ���̑�
			nNum = nCounter - 100;
			int nNum2 = nNum % 60;

			if (60 <= (nNum % 120))
			{
				if (((nNum / 15) % 2) == 0)
					size.y = (SCREEN_WIDTH * 0.02f) + ((SCREEN_WIDTH * 0.0004f) * (nNum2 % 15));
				else
					size.y = (SCREEN_WIDTH * 0.02f) + ((SCREEN_WIDTH * 0.0004f) * 15) - ((SCREEN_WIDTH * 0.0004f) * (nNum2 % 15));
			}
		}
	}

	// �Ђ悱
	for (int nCntChicks = 0; nCntChicks < MAX_CHICKS; nCntChicks++)
	{// �Ђ悱�J�E���g
		D3DXVECTOR3 posEdit = pos;
		posEdit.x += +((SCREEN_WIDTH * 0.015f) * (nCntChicks == 0 ? -1 : 1));

		if (m_pChicks[nCntChicks] != NULL)
		{
			m_pChicks[nCntChicks]->SetPosSize(posEdit, size);
			m_pChicks[nCntChicks]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha));
		}
	}
}

//=============================================================================
// �s���ҏW
//=============================================================================
void CTitleMenu::EditPIYO(void)
{
	// �擾
	int nCounter = CTitle::GetCounter();
	int nNum = nCounter;

	if (100 <= nCounter)
	{// 100�ȏ�
		float fAlpha[MAX_PIYO] = { 0.0f };

		nNum = nCounter - 100;
		int nNum2 = nNum % 60;

		if (60 <= nNum)
		{// 60�ȏ�
			if (60 <= (nNum % 120))
			{// �\��
				if (0 <= (nNum2 / 15))
					fAlpha[2] = 1.0f;
				if (1 <= (nNum2 / 15))
					fAlpha[3] = 1.0f;
				if (2 <= (nNum2 / 15))
					fAlpha[0] = 1.0f;
				if (3 <= (nNum2 / 15))
					fAlpha[1] = 1.0f;
			}
			else
			{// �t�F�[�h�A�E�g
				for (int nCntPIYO = 0; nCntPIYO < MAX_PIYO; nCntPIYO++)
					fAlpha[nCntPIYO] = 1.0f - (1.0f * (float)((float)nNum2 / (float)60));
			}
		}

		// �Ђ悱
		for (int nCntPIYO = 0; nCntPIYO < MAX_PIYO; nCntPIYO++)
			if (m_pPIYO[nCntPIYO] != NULL)
				m_pPIYO[nCntPIYO]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha[nCntPIYO]));
	}
}

//=============================================================================
// �Ђ悱�ƕҏW
//=============================================================================
void CTitleMenu::EditChicksHome(void)
{
	// �擾
	int nCounter = CTitle::GetCounter();
	int nNum = nCounter;

	D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH * 0.85f, -SCREEN_WIDTH * 0.05f, 0.0f);
	D3DXVECTOR2 size = D3DXVECTOR2(SCREEN_WIDTH * 0.04f, SCREEN_WIDTH * 0.04f);
	// �ڕW�ʒu�ix:(SCREEN_WIDTH * 0.85f), y:(SCREEN_HEIGHT * 0.24f)�j

	if (nCounter < 60)
	{// 60�ȉ�
		if (30 <= nCounter)
		{// 30�ȉ�
			nNum = nCounter - 30 + 1;

			pos.y = (-SCREEN_WIDTH * 0.05f) + (((SCREEN_HEIGHT * 0.24f) + (SCREEN_WIDTH * 0.05f)) * (float)((float)nNum / (float)30));
		}
	}
	else
	{// ���̑�
		pos.y = (SCREEN_HEIGHT * 0.24f);

		if (nCounter < 80)
		{// 80�ȉ�
			nNum = nCounter - 60;
			int nNum2 = nNum % 10;

			pos.y = (SCREEN_HEIGHT * (0.24f + ((nNum / 10 == 0 ? nNum2 : 10 - nNum2) * 0.004f)));
		}
	}

	if (m_pChicksHome != NULL)
		m_pChicksHome->SetPosSize(pos, size);
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
// ���j���[BG�ҏW
//=============================================================================
void CTitleMenu::EditMenuBG(void)
{
	// �擾
	bool bMenu = CTitle::GetMenu();

	float fAlpha = 0.0f;		// �����x

	if (bMenu == false)
	{// ���j���[��\��
		if (m_nCntLogo != 0)
		{// �O�ȊO
		 // �����x�v�Z
			fAlpha = 1.0f * (float)((float)(m_nCntLogo % 60) / (float)(60));

			// �J�E���g
			m_nCntLogo--;
		}
		else
		{// ��\��
		 // �l����
			fAlpha = 0.0f;
		}
	}
	else
	{// ���j���[�\��
		if (m_nCntLogo != (60 - 1))
		{// �U�O�ȊO
		 // �����x�v�Z
			fAlpha = 1.0f * (float)((float)(m_nCntLogo % 60) / (float)(60));

			// �J�E���g
			m_nCntLogo++;
		}
		else
		{// �\��
			fAlpha = 1.0f;
		}
	}

	// �F�ݒ�
	if (m_pMenuBG != NULL)
		m_pMenuBG->SetColor(&D3DXCOLOR(0.0f, 0.0f, 0.0f, (0.3f * fAlpha)));
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
				{
					m_bSubMenu = true;

					for (int nCntMainMenu = 0; nCntMainMenu < MAX_MAINMENU; nCntMainMenu++)
					{
						m_pMainMenu[nCntMainMenu]->SetTexture(nCntMainMenu + 3, 3, 2, 1);

						if (nCntMainMenu != MAX_MAINMENU - 1)
						{// �߂�ȊO
							m_pNameMenu[nCntMainMenu]->BindTexture(m_pTexture[TEXTURE_NAME_S]);
							m_pNameMenu[nCntMainMenu]->SetTexture(nCntMainMenu, 1, 2, 1);
						}
						else
						{// �߂�
							m_pNameMenu[nCntMainMenu]->SetPosSize(
								D3DXVECTOR3
								(
									SCREEN_WIDTH * 0.5f - (SCREEN_WIDTH * 0.08f * 2.5f) + ((SCREEN_WIDTH * 0.08f * 2.5f) * nCntMainMenu),
									SCREEN_HEIGHT * 0.9f,
									0.0f
								),
								D3DXVECTOR2(SCREEN_WIDTH * 0.048f, SCREEN_WIDTH * 0.021f));
							m_pNameMenu[nCntMainMenu]->BindTexture(m_pTexture[TEXTURE_RETURN]);
							m_pNameMenu[nCntMainMenu]->SetTexture(0, 1, 1, 1);
						}
					}
				}
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

				if (m_pSearch != NULL)
					m_pSearch->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			}
		}

		// ���C�����j���[
		for (int nCntMainMenu = 0; nCntMainMenu < MAX_MAINMENU; nCntMainMenu++)
		{// ���C�����j���[�J�E���g
			float fcol = 0.0f;

			// �F����
			if (m_nSelectNum == nCntMainMenu)	// �I��ԍ��ƈ�v
				fcol = 1.0f;
			else
				fcol = 0.3f;

			// �F�ݒ�
			if (m_pMainMenu[nCntMainMenu] != NULL)
				m_pMainMenu[nCntMainMenu]->SetColor(&D3DXCOLOR(fcol, fcol, fcol, fAlpha));
			if (m_pNameMenu[nCntMainMenu] != NULL)
				m_pNameMenu[nCntMainMenu]->SetColor(&D3DXCOLOR(fcol, fcol, fcol, fAlpha));
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
	bool bHost = CTitle::GetHost();


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
				{
					m_bSubMenu = false;

					for (int nCntMainMenu = 0; nCntMainMenu < MAX_MAINMENU; nCntMainMenu++)
					{
						m_pMainMenu[nCntMainMenu]->SetTexture(nCntMainMenu, 3, 2, 1);

						if (nCntMainMenu != MAX_MAINMENU - 1)
						{// �߂�ȊO
							m_pNameMenu[nCntMainMenu]->BindTexture(m_pTexture[TEXTURE_NAME_M]);
							m_pNameMenu[nCntMainMenu]->SetTexture(nCntMainMenu, 1, 2, 1);
						}
						else
						{// �߂�
							m_pNameMenu[nCntMainMenu]->SetPosSize(
								D3DXVECTOR3
								(
									SCREEN_WIDTH * 0.5f - (SCREEN_WIDTH * 0.08f * 2.5f) + ((SCREEN_WIDTH * 0.08f * 2.5f) * nCntMainMenu),
									SCREEN_HEIGHT * 0.9f,
									0.0f
								),
								D3DXVECTOR2(SCREEN_WIDTH * 0.048f, SCREEN_WIDTH * 0.021f));
							m_pNameMenu[nCntMainMenu]->BindTexture(m_pTexture[TEXTURE_RETURN]);
							m_pNameMenu[nCntMainMenu]->SetTexture(0, 1, 1, 1);
						}
					}

					m_bSearch = false;				// �T�[�`
				}
				else if (m_bEntry == true)
				{
					// �I�����C���ݒ�
					if (CManager::OnlineSeting(true, bHost) == false)
					{// ���Ȃ�����
						bOnline = false;	// �I�����C������߂�
						CTitle::SetOnline(bOnline);

						m_bEntry = false;			// �G���g���[
						m_bSearch = false;			// �T�[�`

						if (m_pSearch != NULL)
							m_pSearch->SetTexture(1, 1, 2, 1);
					}
					else
					{
						m_bSearch = true;			// �T�[�`

						if (m_pSearch != NULL)
							m_pSearch->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
					}
				}
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
			float fcol = 0.0f;

			// �F����
			if (m_nSelectNum == nCntMainMenu)	// �I��ԍ��ƈ�v
				fcol = 1.0f;
			else
				fcol = 0.3f;

			// �F�ݒ�
			if (m_pMainMenu[nCntMainMenu] != NULL)
				m_pMainMenu[nCntMainMenu]->SetColor(&D3DXCOLOR(fcol, fcol, fcol, fAlpha));
			if (m_pNameMenu[nCntMainMenu] != NULL)
				m_pNameMenu[nCntMainMenu]->SetColor(&D3DXCOLOR(fcol, fcol, fcol, fAlpha));
		}
	}
}

//=============================================================================
// �G���g���[�ҏW
//=============================================================================
void CTitleMenu::EditEntry(void)
{
	// �擾
	bool bMenu = CTitle::GetMenu();
	bool bOnline = CTitle::GetOnline();

	//if (m_bSearch == true)
	{
		float fAlpha = 0.0f;		// �����x

		if (m_bSearch == false)
		{// ���j���[��\��
			if (m_nCntEntry != 0)
			{// �O�ȊO
			 // �����x�v�Z
				fAlpha = 1.0f * (float)((float)(m_nCntEntry % 60) / (float)(60));

				// �J�E���g
				m_nCntEntry--;
			}
			else
			{// ��\��
			 // �l����
				fAlpha = 0.0f;
			}
		}
		else
		{// �����x�\��
			if (m_nCntEntry != (60 - 1))
			{// �U�O�ȊO
			 // �����x�v�Z
				fAlpha = 1.0f * (float)((float)(m_nCntEntry % 60) / (float)(60));

				// �J�E���g
				m_nCntEntry++;
			}
			else
			{// �\��
				fAlpha = 1.0f;
			}
		}

		// �N���C�A���g��
		int nClient = CServer::GetnMaxClient();

		// �F�ݒ�
		if (m_pEntry != NULL)
			m_pEntry->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha));
		if (m_pHint != NULL)
			m_pHint->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha));

		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
			if (m_pMember[nCntPlayer] != NULL)
				m_pMember[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, (nCntPlayer < nClient ? 1.0f : 0.0f)));

		//for (int nCntPlayer = 0; nCntPlayer < (m_bSearch == true ? (1 < nClient ? nClient : 1) : MAX_PLAYER); nCntPlayer++)
		//	if (m_pMember[nCntPlayer] != NULL)
		//		m_pMember[nCntPlayer]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha));

		if (m_pCancel != NULL)
			m_pCancel->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha));

		if (m_pEnter != NULL)
			m_pEnter->SetColor(&D3DXCOLOR(1.0f, 1.0f, 0.0f, (1 < nClient ? 1.0f : 0.0f)));
	}
}