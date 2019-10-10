//=============================================================================
//
// �]���̏��� [reputation.cpp]
// Author : shun yokomichi
//
//=============================================================================
#include "scene.h"
#include "scene2D.h"
#include "reputation.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "game.h"
#include "logo.h"
#include "texture.h"
#include "satisfactionlevel.h"
#include "tutorial.h"
#include "time.h"
#//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define DESTINATION_POS			(D3DXVECTOR3(180.0f, 300.0f, 0.0f))			// �\���ʒu
#define DESTINATION_SIZE		(D3DXVECTOR2(150.0f, 100.0f))				// �T�C�Y
#define BACK_POS_START			(D3DXVECTOR3(170.0f, 900.0f, 0.0f))			// �\���ʒu(�w�i)
#define BACK_POS				(D3DXVECTOR3(170.0f, 570.0f, 0.0f))			// �\���ʒu(�w�i)
#define BACK_SIZE				(D3DXVECTOR2(180.0f, 150.0f))				// �T�C�Y(�w�i)
#define FRAME_INTERVAL			(54.0f * (m_nCntNum - nCntTex - 1))			// �Ԋu(�t���[��)
#define FRAME_MOVE				(10.0f)										// �ړ����x(�t���[��)
#define FRAME_POS				(D3DXVECTOR3(BACK_POS.x, 560.0f, 0.0f))		// �\���ʒu(�t���[��)
#define FRAME_SIZE				(D3DXVECTOR2(150.0f, 25.0f))				// �T�C�Y(�t���[��)
#define DISPLAY_TIME			(35)										// 1��ɕ\�����鎞��
#define DISPLAY_NUM				(4)											// �\�������
#define COMMENT_POS				(D3DXVECTOR3(BACK_POS.x, FRAME_POS.y + 10.0f, 0.0f))	// �\���ʒu(�R�����g)
#define COMMENT_SIZE			(D3DXVECTOR2(100.0f, 20.0f))				// �T�C�Y�ʒu(�R�����g)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
int CReputation::m_nNextType = 0;
int CReputation::m_nNextFont = 0;

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CReputation::CReputation() : CScene2D(5, CScene::OBJTYPE_LOGO)//�D��D�ʃC�j�V�����C�Y
{
	m_size = D3DXVECTOR2(0.0f, 0.0f);

	m_fLength = 0.0f;
	m_fAngle = 0.0f;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntTime = 0;
	m_nStateFont = 0;
	m_nStateType = 0;
	m_pBack = NULL;
	m_nCntNum = 0;
	m_nTexType = 1;
	m_bUninitFlag = false;
	m_nCntUninitTime = 0;
	for (int nCntTex = 0; nCntTex < MAX_FRAME; nCntTex++)
	{
		m_apFrame[nCntTex] = NULL;
		m_apComment[nCntTex] = NULL;
		m_nCommentNum[nCntTex] = -1;
	}
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CReputation::~CReputation()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CReputation::Init(void)
{
	CScene2D::Init();

	// �w�i�̐���
	if (m_pBack == NULL)
	{
		m_pBack = CLogo::Create(BACK_POS_START, BACK_SIZE, CTexture::TYPE_IPHONE, 0, CLogo::TYPE_LOGO);
		m_pBack->SetTexture(m_nTexType, 2, 1, 1);
		m_pBack->SetMove(D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	}
	Satisfaction();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CReputation::Uninit(void)
{
	// �w�i�̔j��
	if (m_pBack != NULL) { m_pBack->Uninit(); m_pBack = NULL; }

	// �t���[���̔j��
	for (int nCntTex = 0; nCntTex < MAX_FRAME; nCntTex++)
	{
		if (m_apFrame[nCntTex] != NULL) { m_apFrame[nCntTex]->Uninit(); m_apFrame[nCntTex] = NULL; }
	}
	// �R�����g�̔j��
	for (int nCntTex = 0; nCntTex < MAX_FRAME; nCntTex++)
	{
		if (m_apComment[nCntTex] != NULL) { m_apComment[nCntTex]->Uninit(); m_apComment[nCntTex] = NULL; }
	}

	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CReputation::Update(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();

	//�ʒu�̊��蓖��
	D3DXVECTOR3 pos;
	pos = GetPosition();

	//�ړ��ʂ̊��蓖��
	D3DXVECTOR3 move;
	move = GetMove();

	m_nCntTime++;

	if (m_nTexType == 0) { FrameDisplay();} // �t���[���\��

	//���S�̈ʒu�ƃT�C�Y
	//CScene2D::SetPosSize(pos, m_size);

	if (m_pBack != NULL)
	{
		D3DXVECTOR3 pos = m_pBack->GetPosition();
		if (m_bUninitFlag == false)
		{
			pos = m_pBack->MoveLimit(m_pBack->GetPosition(),BACK_POS, 2, -6.0f);
			if (pos.y == BACK_POS.y) { m_nTexType = 0; }
		}
		else if (m_bUninitFlag == true)
		{
			pos = m_pBack->MoveLimit(m_pBack->GetPosition(), BACK_POS_START, 3, 6.0f);
			if (pos.y == BACK_POS_START.y) { SetLevel(); return; }
		}
		m_pBack->SetPosition(pos);
		m_pBack->SetTexture(m_nTexType, 2, 1, 1);
		m_pBack->Update();
	}
	for (int nCntTex = 0; nCntTex < MAX_FRAME; nCntTex++)
	{
		if (m_apFrame[nCntTex] != NULL) { m_apFrame[nCntTex]->Update(); }
		if (m_apComment[nCntTex] != NULL) { m_apComment[nCntTex]->Update(); }
	}

	if (m_nCntNum >= DISPLAY_NUM - 1)
	{	// �\�����Ȃ�
		m_nCntUninitTime++;
		if ((m_nCntUninitTime % 60) == 0)
		{
			// �R�����g�̔j��
			for (int nCntTex = 0; nCntTex < MAX_FRAME; nCntTex++)
			{
				if (m_apComment[nCntTex] != NULL) { m_apComment[nCntTex]->Uninit(); m_apComment[nCntTex] = NULL; }
				if (m_apFrame[nCntTex] != NULL) { m_apFrame[nCntTex]->Uninit(); m_apFrame[nCntTex] = NULL; }
			}
			m_bUninitFlag = true;
			m_nTexType = 1;
		}
	}

	if (CTime::GetTime == 0) { SetLevel(); }
}
//=============================================================================
// �`�揈��
//=============================================================================
void CReputation::Draw(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputJoypad *pCInputJoyPad = CManager::GetInputJoyPad();
	if (m_pBack != NULL) { m_pBack->Draw(); }
	for (int nCntTex = 0; nCntTex < MAX_FRAME; nCntTex++)
	{
		if (m_apFrame[nCntTex] != NULL) { m_apFrame[nCntTex]->Draw(); }
		if (m_apComment[nCntTex] != NULL) { m_apComment[nCntTex]->Draw(); }
	}

	CScene2D::Draw();

}
//=============================================================================
// �t���[���\��
//=============================================================================
void CReputation::FrameDisplay(void)
{
	if ((m_nCntTime % DISPLAY_TIME) == 0)
	{	// �\�����Ԃ��߂�����
		if (m_apFrame[m_nCntNum] == NULL && m_nCntNum < DISPLAY_NUM)
		{	// ����
			m_apFrame[m_nCntNum] = CLogo::Create(D3DXVECTOR3(0.0f, FRAME_POS.y, FRAME_POS.z), FRAME_SIZE, CTexture::TYPE_COMMENT_FREAME, 0, CLogo::TYPE_LOGO);
			m_apComment[m_nCntNum] = CLogo::Create(D3DXVECTOR3(0.0f, COMMENT_POS.y, COMMENT_POS.z), COMMENT_SIZE, CTexture::TYPE_COMMENT000 + (int)m_Type, 0, CLogo::TYPE_LOGO);
			int nTexNum = 0;
			int nNum = 0;
			do
			{
				nNum = 0;
				nTexNum = rand() % 4 + 0;
				if (m_nCommentNum[0] != nTexNum) { nNum++;}
				if (m_nCommentNum[1] != nTexNum) { nNum++;}
				if (m_nCommentNum[2] != nTexNum) { nNum++;}

			} while (nNum <= 2);
			m_nCommentNum[m_nCntNum] = nTexNum;
			m_apComment[m_nCntNum]->SetTexture(nTexNum, 1, 4, 1);
			m_nCntNum++;	// ���Z
		}
	}

	for (int nCntTex = 0; nCntTex < MAX_FRAME; nCntTex++)
	{
		if (m_apFrame[nCntTex] != NULL && m_apComment[nCntTex] != NULL)
		{
			D3DXVECTOR3 pos = m_apFrame[nCntTex]->GetPosition();	// �ʒu�擾
			D3DXVECTOR3 posComment = m_apComment[nCntTex]->GetPosition();	// �ʒu�擾
			pos.x += FRAME_MOVE;
			posComment.x += FRAME_MOVE;
			if (pos.x >= FRAME_POS.x) { pos.x = FRAME_POS.x; }
			if (posComment.x >= COMMENT_POS.x) { posComment.x = COMMENT_POS.x; }
			m_apFrame[nCntTex]->SetPosition(D3DXVECTOR3(pos.x, FRAME_POS.y + FRAME_INTERVAL, FRAME_POS.z));
			m_apComment[nCntTex]->SetPosition(D3DXVECTOR3(posComment.x, COMMENT_POS.y + FRAME_INTERVAL, COMMENT_POS.z));
		}
	}
}
//=============================================================================
// �]������
//=============================================================================
void CReputation::Satisfaction(void)
{
	// ���ɍ����]����( �]����MAX20)
	if (m_nSatisfaction <= -1)
	{//	�@���}���s�@�]���@-1
		m_Type = REPUTATION_FAIL;
	}
	else if (m_nSatisfaction <= 0)
	{//	0�b�ȉ��@
		m_Type = REPUTATION_ONE;
	}
	else if (m_nSatisfaction <= 1 && m_nSatisfaction > 0)
	{//�@0�b�ȏ�@3�b�ȉ��@
		m_Type = REPUTATION_TWO;
	}
	else if (m_nSatisfaction <= 2 && m_nSatisfaction > 1)
	{//�@3�b�ȏ�@6�b�ȓ��@
		m_Type = REPUTATION_THREE;
	}
	else if (m_nSatisfaction <= 3 && m_nSatisfaction > 2)
	{//	6�b�ȏ� 10�b�ȓ��@�@
		m_Type = REPUTATION_FOUR;
	}
}

//=============================================================================
// ���̉��Z
//=============================================================================
void CReputation::SetLevel(void)
{
	CSatisfaction *pSatisfaction = CGame::GetSatisfaction();
	CManager::MODE mode = CManager::GetMode();

	if (mode == CManager::MODE_GAME)
	{//�Q�[��
		pSatisfaction = CGame::GetSatisfaction();
	}
	else if (mode == CManager::MODE_TUTORIAL)
	{//�`���[�g���A��
		pSatisfaction = CTutorial::GetSatisfaction();
	}

	// ���ɍ����]����( �]����MAX20)
	if (m_Type == REPUTATION_FAIL) { pSatisfaction->Subtraction(7); }
	if (m_Type == REPUTATION_ONE) { pSatisfaction->Subtraction(4); }
	if (m_Type == REPUTATION_TWO) { pSatisfaction->AddLevel(2); }
	if (m_Type == REPUTATION_THREE) { pSatisfaction->AddLevel(3); }
	if (m_Type == REPUTATION_FOUR) { pSatisfaction->AddLevel(4); }

	Uninit();
	
	return;
}
//===============================================================================
//�@�N���G�C�g
//===============================================================================
CReputation * CReputation::Create(int nSatisfaction)
{
	CReputation *pDestination = NULL;

	//NULL�`�F�b�N
	if (pDestination == NULL)
	{//�������̓��I�m��

		pDestination = new CReputation;

		if (pDestination != NULL)
		{
			//Texture�̎��
			pDestination->m_nSatisfaction = nSatisfaction;
			//�I�u�W�F�N�g�N���X�̐���
			pDestination->Init();
			//�ʒu�����蓖�Ă�
			pDestination->SetPosition(DESTINATION_POS);
			//�T�C�Y�̊��蓖��
			pDestination->m_size = DESTINATION_SIZE;
		}
	}

	return pDestination;
}
//===============================================================================
//�@��Ԃ̗D�揇��
//===============================================================================
void CReputation::SetLogoState(int nStateType, int nStateFont)
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
