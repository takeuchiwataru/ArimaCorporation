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
			m_pCharacter[nCntChar]->SetColor(&D3DXCOLOR((nCntChar % 2 == 0 ? 1.0f : 0.0f), (nCntChar / 2 % 2 == 1 ? 1.0f : 0.0f), (nCntChar % 4 == 1 ? 1.0f : 0.0f), (nCntChar / 4 == 0 ? 1.0f : 0.8f)));
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
}

//=============================================================================
// �X�V����
//=============================================================================
void CGameCharSelect::Update(void)
{
	
}

//=============================================================================
// �`�揈��
//=============================================================================
void CGameCharSelect::Draw(void)
{

}