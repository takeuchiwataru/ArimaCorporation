//=============================================================================
//
// �����x�̏��� [satisfactionlevel.cpp]
// Author : shun yokomichi
//
//=============================================================================
#include "scene.h"
#include "scene2D.h"
#include "satisfactionlevel.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "texture.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_LEVEL				(20)			// ���x���̍ő吔
#define ADD_SIZE				(8.0f + (MAX_LEVEL / 10) + (MAX_LEVEL / 2))			// �T�C�Y���Z

//=========================
// ���U���g���g�p
//=========================
#define EVALUATION_COUNT		(45)		// �J�E���g
#define LEVEL_ADD				(3)			// ���Z���郌�x��

//=========================
// �����L���O���g�p
//=========================
#define CHANGE_COL_TIME			(5)		// �t���[����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
int CSatisfaction::m_nNextType = 0;
int CSatisfaction::m_nNextFont = 0;

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CSatisfaction::CSatisfaction() : CScene2D(6, CScene::OBJTYPE_LOGO)//�D��D�ʃC�j�V�����C�Y
{
	m_size = D3DXVECTOR2(0.0f, 0.0f);

	m_fLength = 0.0f;
	m_fAngle = 0.0f;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntTime = 0;
	m_bChange = false;
	m_nStateFont = 0;
	m_nStateType = 0;
	m_nLevel = 0;
	m_nCntNum = 0;
	m_nCntLevel = 0;
	m_fSize = 0.0f;
	m_type = COL_NOTCHANGE;
	m_nLevelOld = 0;
	m_Col = 0.0f;
	m_bChange = false;
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CSatisfaction::~CSatisfaction()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CSatisfaction::Init(void)
{
	CScene2D::Init();
	CScene2D::SetColor(&D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CSatisfaction::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CSatisfaction::Update(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CManager::MODE mode = CManager::GetMode();

	//�ʒu�̊��蓖��
	D3DXVECTOR3 pos;
	pos = GetPosition();

	if (mode == CManager::MODE_RESULT)
	{
		ResultEvaluation();
		CScene2D::SetTexture(m_nCntLevel, MAX_LEVEL, 1, 0);
		//���S�̈ʒu�ƃT�C�Y
		CScene2D::SetPosSizeSide(pos, m_size);
	}
	else if(mode == CManager::MODE_GAME || mode == CManager::MODE_TUTORIAL)
	{
		if (m_LevelType == LEVEL_UP)
		{
			m_fSize += 1.0f;
			if ( m_nLevelOld * ADD_SIZE < m_fSize)
			{
				m_fSize = m_nLevelOld * ADD_SIZE;
				if (m_nLevel != m_nLevelOld) { m_nLevelOld++; }
				else if (m_nLevel == m_nLevelOld) { m_LevelType = LEVEL_NONE; }
			}
			ChangeCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

		}
		else if (m_LevelType == LEVEL_DOWN)
		{
			/*m_fSize -= 1.0f;
			if (m_nLevelOld * ADD_SIZE > m_fSize && m_nLevelOld < 1)
			{
				m_fSize = m_nLevelOld * ADD_SIZE;
				if (m_nLevel != m_nLevelOld) { m_nLevelOld--; }
				else if (m_nLevel == m_nLevelOld) { m_LevelType = LEVEL_NONE; m_nLevelOld = m_nLevel; }
			}*/

			m_fSize -= 1.0f;
			if (m_nLevelOld * ADD_SIZE > m_fSize)
			{
				m_fSize = m_nLevelOld * ADD_SIZE;
				if (m_nLevel != m_nLevelOld) { m_nLevelOld--; }
				if (m_nLevel == m_nLevelOld) { m_LevelType = LEVEL_NONE; }
			}
			ChangeCol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
		}

		// Texture�ݒ�
		CScene2D::SetTexture(m_nLevelOld, MAX_LEVEL, 1, 0);
		if (m_LevelType == LEVEL_NONE) { CScene2D::SetColor(&D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)); }
		//���S�̈ʒu�ƃT�C�Y
		CScene2D::SetPosSizeSide(pos, D3DXVECTOR2(m_fSize, m_size.y));
	}
	else
	{
		// Texture�ݒ�
		CScene2D::SetTexture(m_nLevel, MAX_LEVEL, 1, 0);
		//���S�̈ʒu�ƃT�C�Y
		CScene2D::SetPosSizeSide(pos, m_size);
	}

}
//=============================================================================
// �`�揈��
//=============================================================================
void CSatisfaction::Draw(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputJoypad *pCInputJoyPad = CManager::GetInputJoyPad();

	CScene2D::Draw();
}

//===============================================================================
// �����x���Z
//===============================================================================
void CSatisfaction::AddLevel(int nLevel)
{
	m_LevelType = LEVEL_UP;
	m_nLevelOld = m_nLevel;
	if (m_nLevel < MAX_LEVEL - 1) { m_size.x += ADD_SIZE * nLevel; }

	m_nLevel += nLevel;

	if (m_nLevel >= MAX_LEVEL - 1) { m_nLevel = MAX_LEVEL - 1; }
}

//===============================================================================
// ���U���g���ɕ\��
//===============================================================================
void CSatisfaction::ResultEvaluation(void)
{
	if ((m_nCntTime % EVALUATION_COUNT) == 0)
	{
		m_nCntLevel += LEVEL_ADD + m_nCntNum;
		m_nCntNum++;
		if (m_nCntNum > 1) { m_nCntNum = 1; }
		if (m_nCntLevel > m_nLevel) { m_nCntLevel = m_nLevel; }

		m_size = D3DXVECTOR2(m_fSize + (ADD_SIZE * m_nCntLevel), m_size.y);
	}
	m_nCntTime++;
}

//=============================================================================
// �F(�����x)�ω��̏���
//=============================================================================
void CSatisfaction::ChangeCol(D3DXCOLOR col)
{
	if ((m_nCntTime % CHANGE_COL_TIME) == 0)
	{
		if (m_bChange == false) { CScene2D::SetColor(&col); m_bChange = true; }
		else if (m_bChange == true) { CScene2D::SetColor(&D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)); m_bChange = false; }
	}
	m_nCntTime++;
}

//===============================================================================
// �����x���Z
//===============================================================================
void CSatisfaction::Subtraction(int nLevel)
{
	m_LevelType = LEVEL_DOWN;
	m_nLevelOld = m_nLevel;
	if (m_nLevel > 0) { m_size.x -= ADD_SIZE * nLevel; }

	m_nLevel -= nLevel;

	if (m_nLevel <= 0) { m_nLevel = 1; }
}

//===============================================================================
//�@�N���G�C�g
//===============================================================================
CSatisfaction * CSatisfaction::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nLevel)
{
	CSatisfaction *pSatisfaction = NULL;
	CManager::MODE mode = CManager::GetMode();

	//NULL�`�F�b�N
	if (pSatisfaction == NULL)
	{//�������̓��I�m��

		pSatisfaction = new CSatisfaction;

		if (pSatisfaction != NULL)
		{
			//�ό`�ł�����
			pSatisfaction->m_nLevel = nLevel;
			pSatisfaction->m_nLevelOld = nLevel;
			//�I�u�W�F�N�g�N���X�̐���
			pSatisfaction->Init();
			//�e�N�X�`�������蓖�Ă�
			pSatisfaction->BindTexture(*CTexture::GetTexture(CTexture::TYPE_SATISFACTION));
			//�ʒu�����蓖�Ă�
			pSatisfaction->SetPosition(pos);
			//�T�C�Y�̊��蓖��
			if (mode == CManager::MODE_RESULT)
			{
				pSatisfaction->m_size = size;
				pSatisfaction->m_fSize = size.x;
			}
			else if (mode == CManager::MODE_GAME || mode == CManager::MODE_TUTORIAL)
			{
				pSatisfaction->m_size = D3DXVECTOR2(size.x + (ADD_SIZE * nLevel), size.y);
				pSatisfaction->m_fSize = size.x + (ADD_SIZE * nLevel);
			}
			else
			{
				pSatisfaction->m_size = D3DXVECTOR2(size.x + (ADD_SIZE * nLevel), size.y);
			}
		}
	}

	return pSatisfaction;
}
//===============================================================================
//�@��Ԃ̗D�揇��
//===============================================================================
void CSatisfaction::SetLogoState(int nStateType, int nStateFont)
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