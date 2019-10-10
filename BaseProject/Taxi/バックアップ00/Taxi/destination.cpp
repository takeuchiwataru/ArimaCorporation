//=============================================================================
//
// �ړI�n�̏��� [destination.cpp]
// Author : shun yokomichi
//
//=============================================================================
#include "scene.h"
#include "scene2D.h"
#include "destination.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "billboord.h"
#include "game.h"
#include "tutorial.h"
#include "player.h"
#include "texture.h"
#include "logo.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define DESTINATION_POS_START		(D3DXVECTOR3(-150.0f, 300.0f, 0.0f))						// �\���ʒu
#define DESTINATION_POS_END			(D3DXVECTOR3(180.0f, 300.0f, 0.0f))						// �\���ʒu
#define DESTINATION_SIZE			(D3DXVECTOR2(150.0f, 100.0f))							// �T�C�Y
#define SAID_TIME					(180)					// �\�����鎞��
#define ADD_MOVE					(15.0f)

//-----------------------------------
// �ړI�n���̃}�N��
//-----------------------------------
#define NAME_POS_START			(D3DXVECTOR3(-150.0f, DESTINATION_POS_END.y + 140.0f, DESTINATION_POS_END.z))
#define NAME_POS_END			(D3DXVECTOR3(DESTINATION_POS_END.x, DESTINATION_POS_END.y + 140.0f, DESTINATION_POS_END.z))
#define NAME_SIZE				( D3DXVECTOR2(150.0f, 25.0f))

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
int CDestination::m_nNextType = 0;
int CDestination::m_nNextFont = 0;

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CDestination::CDestination() : CScene2D(5, CScene::OBJTYPE_LOGO)//�D��D�ʃC�j�V�����C�Y
{
	m_size = D3DXVECTOR2(0.0f, 0.0f);

	m_fLength = 0.0f;
	m_fAngle = 0.0f;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntTime = 0;
	m_nStateFont = 0;
	m_nStateType = 0;
	m_bDelet = false;
	m_pLogo = NULL;
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CDestination::~CDestination()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CDestination::Init(void)
{
	CScene2D::Init();

	if (m_pLogo == NULL)
	{
		m_pLogo =
		CLogo::Create(NAME_POS_END, NAME_SIZE, CTexture::TYPE_NAME_SAPPRO + m_nDestination, 0, CLogo::TYPE_LOGO);
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CDestination::Uninit(void)
{
	if (m_pLogo != NULL)
	{
		m_pLogo->Uninit();
		m_pLogo = NULL;
	}

	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CDestination::Update(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();

	//�ʒu�̊��蓖��
	D3DXVECTOR3 pos;
	pos = GetPosition();

	if (m_nCntTime >= 150)
	{
		pos.x -= ADD_MOVE;
	}

	if (m_pLogo != NULL)
	{
		if (m_nCntTime >= 150)
		{
			D3DXVECTOR3 pos = m_pLogo->GetPosition();
			m_pLogo->SetPosition(m_pLogo->MoveLimit(pos, NAME_POS_END, 0, -ADD_MOVE));
		}
		m_pLogo->Update();
	}

	if (pos.x <= DESTINATION_POS_START.x)
	{
		pos.x = DESTINATION_POS_START.x;
	}

	bool bPutin = false;
	CManager::MODE mode = CManager::GetMode();
	if (mode == CManager::MODE_GAME) { bPutin = CGame::GetPlayer()->GetPutin(); }
	else if (mode == CManager::MODE_TUTORIAL) { bPutin = CTutorial::GetPlayer()->GetPutin(); }
	if (bPutin == true || CManager::GetMode() == CManager::MODE_TUTORIAL)
	{
		m_nCntTime++;
	}

	if (m_nCntTime >= SAID_TIME)
	{	// �\�����Ȃ�
		SetDelet(true);
	}
	//���S�̈ʒu�ƃT�C�Y
	CScene2D::SetPosSize(pos, m_size);
}
//=============================================================================
// �`�揈��
//=============================================================================
void CDestination::Draw(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputJoypad *pCInputJoyPad = CManager::GetInputJoyPad();

	if (m_pLogo != NULL)
	{
		m_pLogo->Draw();
	}

	CScene2D::Draw();
}
//===============================================================================
//�@�N���G�C�g
//===============================================================================
CDestination * CDestination::Create(int nDestination)
{
	CDestination *pDestination = NULL;

	//NULL�`�F�b�N
	if (pDestination == NULL)
	{//�������̓��I�m��

		pDestination = new CDestination;

		if (pDestination != NULL)
		{
			//Texture�̎��
			pDestination->m_nDestination = nDestination;
			if (nDestination >= 2) { nDestination--; }
			//�I�u�W�F�N�g�N���X�̐���
			pDestination->Init();
			//�e�N�X�`�������蓖�Ă�
			pDestination->BindTexture(*CTexture::GetTexture(CTexture::TYPE_PLACE_SAPPRO + nDestination));
			//�ʒu�����蓖�Ă�
			pDestination->SetPosition(DESTINATION_POS_END);
			//�T�C�Y�̊��蓖��
			pDestination->m_size = DESTINATION_SIZE;
		}
	}

	return pDestination;
}
//===============================================================================
//�@��Ԃ̗D�揇��
//===============================================================================
void CDestination::SetLogoState(int nStateType, int nStateFont)
{
	//�A�C�R���̗D�揇��
	if (m_nNextType <= nStateType)
	{
		m_nNextType = nStateType;
	}
	//�t�H���g�̗D�揇��
	if (m_nNextFont <= nStateFont)
	{
		m_nNextFont = nStateFont;
	}
}