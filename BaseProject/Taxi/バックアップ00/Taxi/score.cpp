//=============================================================================
//
// �X�R�A���� [score.cpp]
// Author : shun yokomichi
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "score.h"
#include "number.h"
#include "logo.h"
#include "totalscore.h"
#include "numberdisplay.h"
#include "combo.h"
#include "texture.h"
#include "tutorial.h"
//=============================================================================
// �}�N����`
//=============================================================================
#define CENTER_WIDTH		(SCREEN_WIDTH / 2)	// X�̒��S���W
#define TIMER_SPACE			(30.0f)				// �����Ɛ����̊Ԃ̃T�C�Y
#define TIMER_POSITION_Y	(70.0f)				// �^�C�}�[��Y���W
#define ROULETTE_COUNT		(2)					// ���[���b�g�̃J�E���g�ؑ�
#define ROULETTE_CAHNGE		(8)					// ���[���b�g����؂�ւ��t���[����
#define POWER_X				(10)				// �ׂ���((X * X) Y)���鐔�l
//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================
int				CScore::m_nScore	= 0;
int				CScore::m_nScoreNum = 1;
CScore::STATE	CScore::m_state		= STATE_NONE;
CScore::TYPE	CScore::m_type		= TYPE_NOT_USE;
//=============================================================================
// ��������
//=============================================================================
CScore *CScore::Create(D3DXVECTOR3 pos)
{
	CScore *pScore = NULL;

	//NULL�`�F�b�N
	if (pScore == NULL)
	{//���I�m��
		pScore = new CScore;

		//NULL�`�F�b�N
		if (pScore != NULL)
		{
			//�ʒu�̐ݒ�
			pScore->m_pos = pos;
			//����������
			pScore->Init();
		}
	}

	return pScore;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScore::CScore(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_nCntRoulette	= 0;
	m_nCntAdd		= 0;
	m_nCntNum		= 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScore::~CScore(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CScore::Init(void)
{
	m_nScoreNum = 1;
	m_nScore = 0;
	m_type = TYPE::TYPE_NOT_USE;
	for (int nCntTime = 0; nCntTime < SCORE_MAX; nCntTime++)
	{
		m_apNumber[nCntTime] = new CNumber;
		m_apNumber[nCntTime]->Init(D3DXVECTOR3((m_pos.x - TIMER_SPACE * nCntTime), m_pos.y, m_pos.z), 1);
		m_apNumber[nCntTime]->SetCol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
		m_apNumber[nCntTime]->SetSize(D3DXVECTOR2(15.0f, 13.0f), D3DXVECTOR2((m_pos.x - TIMER_SPACE * nCntTime), m_pos.y));
	}


	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CScore::Uninit(void)
{
	for (int nCntTime = 0; nCntTime < SCORE_MAX; nCntTime++)
	{
		m_apNumber[nCntTime]->Uninit();
		m_apNumber[nCntTime] = NULL;
	}

	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CScore::Update(void)
{
	//���݂̃��[�h�̎擾
	CManager::MODE GameMode = CManager::GetMode();

	//DebugKey();	// �f�o�b�N�p

	int nTexData = m_nScore;

	if (m_state == STATE_SET)
	{	// �ݒ��ԂȂ�
		m_type = TYPE::TYPE_USE;
		int nNum = (int)std::pow(POWER_X, m_nScoreNum);
		while ((m_nScore / nNum) != 0)
		{
			m_nScoreNum++;
			nNum = (int)std::pow(POWER_X, m_nScoreNum);		// �ׂ��悷��(����O���Ȃ�)
			if (m_nScoreNum > SCORE_MAX - 1) { m_nScoreNum = SCORE_MAX - 1; }
		}
		m_state = STATE_ROULETTE;
	}

	if (m_state == STATE_ADD_TOTAL)
	{	// ���Z���̏���
		m_nScoreNum = 1;
		m_nCntRoulette = ROULETTE_CAHNGE + 1;
		m_nCntNum = m_nScoreNum - 1;
		m_state = STATE_ROULETTE;
		m_type = TYPE::TYPE_NOT_USE;
	}

	if (m_state == STATE_RESET)
	{	// ���Z�b�g���
		m_nScoreNum = 1;
		m_nCntRoulette = 0;
		m_nCntNum = m_nScoreNum - 1;
		m_state = STATE_ROULETTE;
		m_type = TYPE::TYPE_NOT_USE;
	}

	if (m_state == STATE_ROULETTE)
	{	// ���[���b�g��Ԃ̏���
		if (m_nCntRoulette >= ROULETTE_CAHNGE)
		{
			NumberRoulette(nTexData);
		}
		else
		{
			for (int nCntTime = 0; nCntTime < m_nScoreNum; nCntTime++)
			{// �e�N�X�`���ɔ��f
				m_apNumber[nCntTime]->SetNumber(m_nCntAdd);
			}
		}
		m_nCntRoulette++;
		m_nCntAdd++;
	}

	SetType();

}

//=============================================================================
// �`�揈��
//=============================================================================
void CScore::Draw(void)
{
	for (int nCntTime = 0; nCntTime < m_nScoreNum; nCntTime++)
	{
		m_apNumber[nCntTime]->Draw();
	}
}

//=============================================================================
// �X�R�A�ݒ菈��
//=============================================================================
void CScore::SetScore(int nScore)
{
	m_nScore = nScore;
	m_state = STATE_SET;
}

//=============================================================================
// �X�R�A���Z����
//=============================================================================
void CScore::AddScore(int nScore)
{
	m_nScore += nScore;
	CNumberDisplay::Create(nScore);
	m_state = STATE_SET;
}

//=============================================================================
// �ׂ���v�Z
//=============================================================================
void CScore::pow(void)
{
	int nNum = (int)std::pow(POWER_X, m_nScoreNum);
	while ((m_nScore / nNum) != 0)
	{
		m_nScoreNum++;
		nNum = (int)std::pow(POWER_X, m_nScoreNum);		// �ׂ��悷��(����O���Ȃ�)
		if (m_nScoreNum > SCORE_MAX - 1) { m_nScoreNum = SCORE_MAX - 1; }
	}
}

//=============================================================================
// �g�[�^���X�R�A�ւ̉��Z����
//=============================================================================
void CScore::AddTotalScore(void)
{
	CTotalScore::AddTotalScore(m_nScore);
	m_state = STATE_ADD_TOTAL;
	m_nScore = 0;
}

//=============================================================================
// ���[���b�g�̂悤�ɉ񂷏���
//=============================================================================
void CScore::NumberRoulette(int nTexData)
{
	if (m_nCntRoulette == ROULETTE_COUNT) { m_nCntNum = 0; }
	if ((m_nCntRoulette % ROULETTE_CAHNGE) == 0) { m_nCntNum++; }
	if (m_nCntNum > m_nScoreNum - 1) { m_nCntNum = m_nScoreNum - 1; m_state = STATE_DESSCORE; }
	for (int nCntScore = 0; nCntScore < m_nScoreNum; nCntScore++)
	{// �e�N�X�`���ɔ��f
		if (nCntScore <= m_nCntNum)
		{
			m_apNumber[nCntScore]->SetNumber(nTexData);
			nTexData /= 10;
		}
		else { m_apNumber[nCntScore]->SetNumber(m_nCntAdd); }
	}

	// �g�[�^���X�R�A�̕\����Ԃ̏ꍇ �J�E���^�[�̃��Z�b�g
	if (m_state == STATE_DESSCORE) { m_nCntAdd = 0; m_nCntRoulette = 0; }

}

//=============================================================================
// �f�o�b�N�p
//=============================================================================
void CScore::DebugKey(void)
{
	if (CManager::GetInput()->GetKeyboardTrigger(DIK_2))
	{	// �X�R�A���Z
		AddScore(100);
		CNumberDisplay::Create(100);
		if (CCombo::GetCombo() == 0)
		{
			CCombo::Create(CCombo::TYPE_THROUGH);
		}
		else
		{
			CCombo::AddCombo(1);
		}
	}
	if (CManager::GetInput()->GetKeyboardTrigger(DIK_3))
	{
		Reset();
	}
}

//=============================================================================
// �^�C�v�̐ݒ�
//=============================================================================
void CScore::SetType()
{
	CManager::MODE mode = CManager::GetMode();
	if (m_type == TYPE::TYPE_USE)
	{
		if (mode == CManager::MODE_GAME)
		{
			CGame::GetScoreUI(1)->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			CGame::GetScoreUI(3)->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			CGame::GetScoreUI(5)->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else if (mode == CManager::MODE_TUTORIAL)
		{
			CTutorial::GetScoreUI(1)->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			CTutorial::GetScoreUI(3)->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			CTutorial::GetScoreUI(5)->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
	else if (m_type == TYPE::TYPE_NOT_USE)
	{
		if (mode == CManager::MODE_GAME)
		{
			CGame::GetScoreUI(1)->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
			CGame::GetScoreUI(3)->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
			CGame::GetScoreUI(5)->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
		}
		else if (mode == CManager::MODE_TUTORIAL)
		{
			CTutorial::GetScoreUI(1)->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
			CTutorial::GetScoreUI(3)->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
			CTutorial::GetScoreUI(5)->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
		}
	}
}