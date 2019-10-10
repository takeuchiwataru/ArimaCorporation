//=============================================================================
//
// ���S�̏��� [logo.cpp]
// Author : �L�n���u
//
//=============================================================================
#include "scene.h"
#include "scene2D.h"
#include "logo.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "billboord.h"
#include "game.h"
#include "player.h"
#include "arrow.h"
#include "texture.h"
#include "tutorial.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define VECTORE_ZERO		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
#define COUNT_COLA			(30)					// �������J�E���g
#define COUNT_FRAME			(80 + COUNT_COLA)		// �\���t���[��
#define COLA_SPEED			(m_nCntColA * 0.015f)	// �����x�̑��x

//--------------------------------
// �M�A�ؑ֊֌W
//--------------------------------
#define MOVE_GEAR_POS		(D3DXVECTOR3(pos.x, m_posOld.y + (10.0f * (int)pPlayer->GetState()), pos.z))	// �ړ����̖ړI�n
#define MOVE_GEAR_ADD		(1.0f)																			// �ړ��ʂ̉��Z�l

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
int CLogo::m_nNextType = 0;
int CLogo::m_nNextFont = 0;

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CLogo::CLogo(int nPriorityv) : CScene2D(nPriorityv, CScene::OBJTYPE_LOGO)//�D��D�ʃC�j�V�����C�Y
{
	m_nType = 0;
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_fLength = 0.0f;
	m_fAngle = 0.0f;
	m_rot = VECTORE_ZERO;
	m_moveLimit = VECTORE_ZERO;
	m_nCntColA = 0;
	m_bColA = false;
	m_nStateFont = 0;
	m_nStateType = 0;
	m_nCount	 = 0;
	m_bMoveFlag = false;
	m_bFlag = false;
	m_bDataFlag = false;
	m_nCntData = 0;
	m_nCntTime = 0;
	m_bVibration = false;
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CLogo::~CLogo()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CLogo::Init(void)
{
	CScene2D::Init();
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CLogo::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CLogo::Update(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();

	//�ʒu�̊��蓖��
	D3DXVECTOR3 pos;
	pos = GetPosition();

	//���S�̈ʒu�ƃT�C�Y
	CScene2D::SetPosSize(pos, m_size);

	// �F�̐ݒ�
	SetCol();
	// Texture�ݒ�
	TexAnim();
}
//=============================================================================
// �`�揈��
//=============================================================================
void CLogo::Draw(void)
{
	CScene2D::Draw();
}
//===============================================================================
//�@�N���G�C�g
//===============================================================================
CLogo * CLogo::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nType, int nTransformType, TYPE type)
{
	CLogo *pLogo = NULL;

	//NULL�`�F�b�N
	if (pLogo == NULL)
	{//�������̓��I�m��
		int nPriorityv = 5;
		if (CTexture::TYPE_MONEY_TEN == nType || CTexture::TYPE_GAMETIMELOGO == nType ||  CTexture::TYPE_TUTORIAL_STOP == nType) { nPriorityv = 6; }
		else if (CTexture::TYPE_PAUSE_ICON <= nType && CTexture::TYPE_PAUSE_OPERATION >= nType ) { nPriorityv = 7; }
		pLogo = new CLogo(nPriorityv);

		if (pLogo != NULL)
		{
			//��ނ����蓖�Ă�
			pLogo->m_nType = nType;
			//�ό`�ł�����
			pLogo->m_nTransformType = nTransformType;
			//�I�u�W�F�N�g�N���X�̐���
			pLogo->Init();
			//�e�N�X�`�������蓖�Ă�
			if (CLogo::TYPE_LOGO == type) { pLogo->BindTexture(*CTexture::GetTexture(nType)); }
			else if (CLogo::TYPE_MULTIRENDERING == type) { pLogo->BindTexture(CArrow::GetTex()); }
			//�ʒu�����蓖�Ă�
			pLogo->SetPosition(pos);
			pLogo->m_posOld = pos;
			//�T�C�Y�̊��蓖��
			pLogo->m_size = size;
		}
	}

	return pLogo;
}
//===============================================================================
//�@��Ԃ̗D�揇��
//===============================================================================
void CLogo::SetLogoState(int nStateType, int nStateFont)
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
//===============================================================================
//�@Logo���ŐF�̐ݒ���܂Ƃ߂��֐�
//===============================================================================
void CLogo::SetCol()
{
	if (m_nType == (int)CTexture::TYPE_TITLE_PRESS)
	{	// Press Strat Enter�̏ꍇ
		m_nCntColA++;
		if ((m_nCntColA % COUNT_FRAME) == 0)
		{
			CScene2D::SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_nCntColA = 0;
		}
		else if (m_nCntColA >= COUNT_COLA)
		{
			CScene2D::SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}

	if ((m_nType == (int)CTexture::TYPE_RESULT_BUTTON) || (m_nType == (int)CTexture::TYPE_TUTORIAL_SKIP_KEYBOAD) || (m_nType == (int)CTexture::TYPE_TUTORIAL_SKIP_PAD) ||
		(m_nType == (int)CTexture::TYPE_TUTORIAL_ENTER_KEYBOAD) || (m_nType == (int)CTexture::TYPE_TUTORIAL_ENTER_PAD))
	{	// �����x�̕ω�(���傶��ɕς���)
		float fColA = COLA_SPEED;
		m_nCntColA++;
		if (m_bColA == false)
		{
			CScene2D::SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + fColA));
			if (fColA >= 1.0f) { m_nCntColA = 0;  m_bColA = true; }
		}
		else if (m_bColA == true)
		{
			CScene2D::SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - fColA));
			if (fColA >= 1.0f) { m_nCntColA = 0;  m_bColA = false; }
		}
	}

	if ((m_nType == (int)CTexture::TYPE_COMMAND_000))
	{
		float fColA = m_nCntColA * 0.045f;
		m_nCntColA++;
		if (m_bColA == false)
		{
			CScene2D::SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f + fColA));
			if (fColA >= 1.0f) { m_nCntColA = 0;  m_bColA = true; }
		}
		else if (m_bColA == true)
		{
			CScene2D::SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - fColA));
			if (fColA >= 1.0f) { m_nCntColA = 0;  m_bColA = false; }
		}
	}
}
//===============================================================================
//�@Logo����TextureAnimation���܂Ƃ߂��֐�
//===============================================================================
void CLogo::TexAnim()
{
	if (m_nType == (int)CTexture::TYPE_GEARCHANGE)
	{	//�M�A�ؑւ̏ꍇ
		CManager::MODE mode = CManager::GetMode();
		CPlayer * pPlayer = NULL;
		D3DXVECTOR3 pos = CScene2D::GetPosition();

		if (mode == CManager::MODE_GAME)
		{//�Q�[��
			pPlayer = CGame::GetPlayer();
		}
		if (mode == CManager::MODE_TUTORIAL)
		{//�`���[�g���A��
			pPlayer = CTutorial::GetPlayer();
		}

		if (m_bDataFlag == true)
		{
			pos = Move(pos, 3.0f);
		}

		if (m_nCntData != (int)pPlayer->GetState())
		{
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_STAGESELECT);
			m_nCntData = (int)pPlayer->GetState();
			m_bDataFlag = true;
		}
		else if (m_bDataFlag == true)
		{
			m_nCount++;
			if ((m_nCount % 15) == 0)
			{
				m_bDataFlag = false;
				pos = m_posOld;
				m_nCount = 0;
			}
		}

		CScene2D::SetPosition(pos);
		CScene2D::SetTexture(m_nCntData, 2, 1, 1);
	}
	else if (m_nType == (int)CTexture::TYPE_MONEY_TEN)
	{	// \�}�[�N & .�̏ꍇ
		CScene2D::SetTexture(m_nTransformType, 2, 1, 1);
	}
	else if (m_nType == (int)CTexture::TYPE_BOARDING)
	{	// ��Ԃ���Ԃ�Texture
		int nType = 0;
		bool bPutin = false;
		if (CManager::GetMode() == CManager::MODE_GAME) { bPutin = CGame::GetPlayer()->GetPutin(); }
		else if (CManager::GetMode() == CManager::MODE_TUTORIAL) { bPutin = CTutorial::GetPlayer()->GetPutin(); }

		if (bPutin == false) { nType = 0; CScene2D::SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f)); }		// ��Ԃ̏ꍇ
		else if (bPutin == true) { nType = 1; CScene2D::SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));} // ��Ԃ̏ꍇ

		CScene2D::SetTexture(nType, 2, 1, 1);
	}
}

//===============================================================================
//�@Logo���ňړ��̐ݒ���܂Ƃ߂��֐�
//===============================================================================
D3DXVECTOR3 CLogo::Move(D3DXVECTOR3 pos, float fMove)
{
	D3DXVECTOR3 move = VECTORE_ZERO;
	if (CTexture::TYPE_IPHONE == m_nType)
	{	// iPhone�Ȃ�
		move.y = fMove;
		if (fMove < 0 && m_nCntTime < 60)
		{
			//�U�������Đ�����
			if (m_bVibration == false)
			{ 
				CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_VIBRATION);
				m_bVibration = true;
			}

			if ((m_nCntTime % 2) == 0)
			{
				m_bFlag = m_bFlag ? false : true;
			}

			if (m_bFlag == false) { move.x = 3.0f; }
			else if (m_bFlag == true) { move.x = -3.0f; }
			m_nCntTime++;
		}
		else if (m_nCntTime >= 60)
		{
			if (m_bVibration)
			{//�����~�߂�
				CManager::GetSound()->StopSound(CSound::SOUND_LABEL_SE_VIBRATION);
				m_bVibration = false;
			}
		}
	}
	else if (m_nType == (int)CTexture::TYPE_GEARCHANGE)
	{
		if ((m_nCntTime % 2) == 0)
		{
			m_bFlag = m_bFlag ? false : true;
		}

		if (m_bFlag == false) { move.x = fMove; }
		else if (m_bFlag == true) { move.x = -fMove; }
		m_nCntTime++;
	}
	else { move.x += fMove; }

	//�ʒu���X�V(�ړ�)
	pos.x += move.x;
	pos.y += move.y;

	return pos;
}

//===============================================================================
//�@�ړ������̐ݒ�
//===============================================================================
D3DXVECTOR3 CLogo::MoveLimit(D3DXVECTOR3 pos, D3DXVECTOR3 PosLimit, int nOperation, float fMove)
{
	pos = Move(pos, fMove);

	if (nOperation == 0)
	{	// X�����w�肵���l���E�ɍs���Ȃ�
		if (pos.x >= PosLimit.x)
		{
			pos.x = PosLimit.x;
			m_bMoveFlag = m_bMoveFlag ? false : true;
		}
	}
	else if (nOperation == 1)
	{	// X�����w�肵���l��荶�ɍs���Ȃ�
		if (pos.x <= PosLimit.x)
		{
			pos.x = PosLimit.x;
			m_bMoveFlag = m_bMoveFlag ? false : true;
		}
	}
	else if (nOperation == 2)
	{	// Y�����w�肵���l��艺�ɍs���Ȃ�
		if (pos.y <= PosLimit.y)
		{
			pos.y = PosLimit.y;
			m_bMoveFlag = m_bMoveFlag ? false : true;
		}
	}
	else if (nOperation == 3)
	{	// Y�����w�肵���l����ɍs���Ȃ�
		if (pos.y >= PosLimit.y)
		{
			pos.y = PosLimit.y;
			m_bMoveFlag = m_bMoveFlag ? false : true;
		}
	}
	return pos;
}

//===============================================================================
//�@Logo���Ŋg��k�����܂Ƃ߂��֐�
//===============================================================================
void CLogo::Zoom(D3DXVECTOR3 pos, D3DXVECTOR2 EndSize, D3DXVECTOR2 AddSize, bool bZoom)
{
	D3DXVECTOR2 ZoomSize = EndSize;
	if (bZoom == false)
	{	// �k���̏ꍇ
		m_size.x -= AddSize.x;
		m_size.y -= AddSize.y;
		if (m_size.x <= ZoomSize.x) { m_size.x = ZoomSize.x; }
		if (m_size.y <= ZoomSize.y) { m_size.y = ZoomSize.y; }
		CScene2D::SetPosSize(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR2(m_size.x, m_size.y));
	}
	else if (bZoom == true)
	{
		m_size.x += AddSize.x;
		m_size.y += AddSize.y;
		if (m_size.x >= ZoomSize.x) { m_size.x = ZoomSize.x; }
		if (m_size.y >= ZoomSize.y) { m_size.y = ZoomSize.y; }
		CScene2D::SetPosSize(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR2(m_size.x, m_size.y));
	}
}