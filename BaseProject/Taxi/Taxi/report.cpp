//=============================================================================
//
// �񍐂̏��� [report.cpp]
// Author : �L�n���u
//
//=============================================================================
#include "scene.h"
#include "scene2D.h"
#include "report.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "sound.h"
#include "game.h"
#include "logo.h"
#include "texture.h"
#include "satisfactionlevel.h"
#include "totalscore.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RESULT_LOGO_POS		(D3DXVECTOR3(240.0f, 80.0f, 0.0f))			// ���U���g���S(�ʒu)
#define RESULT_LOGO_SIZE	(D3DXVECTOR2(150.0f, 50.0f))				// ���U���g���S(�T�C�Y)
#define TOTAL_LOGO_POS		(D3DXVECTOR3(270.0f, 220.0f, 0.0f))			// �g�[�^�����S(�ʒu)
#define TOTAL_LOGO_SIZE		(D3DXVECTOR2(220.0f, 25.0f))				// �g�[�^�����S(�T�C�Y)
#define TOTALSCORE_POS		(D3DXVECTOR3(400.0f, 310.0f, 0.0f))			// �g�[�^���X�R�A(�ʒu)
#define CLAS_POS			(D3DXVECTOR3(140.0f, 460.0f, 0.0f))			// �����x���S(�ʒu)
#define CLAS_SIZE			(D3DXVECTOR2(100.0f, 30.0f))				// �����x���S(�T�C�Y)
#define CLAS_LEVEL_POS		(D3DXVECTOR3(30.0f,  540.0f, 0.0f))			// �����x(�ʒu)
#define CLAS_LEVEL_SIZE		(D3DXVECTOR2(15.0f, 35.0f))					// �����x(�T�C�Y)
#define PRESS_BUTTON_POS	(D3DXVECTOR3(140.0f, 670.0f, 0.0f))			// ����{�^��(�ʒu)
#define PRESS_BUTTON_SIZE	(D3DXVECTOR2(100.0f, 30.0f))				// ����{�^��(�T�C�Y)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CReport::CReport() : CScene(4)//�D��D�ʃC�j�V�����C�Y
{
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = 0.0f;
	m_fAngle = 0.0f;
	m_nCntLogo = 0;
	m_pSatisfaction = NULL;
	m_pTotalScore = NULL;
	m_type = TYPE_NONE;
	m_nType = 0;
	for (int nCntLogo = 0; nCntLogo < REPORT_LOGO_NUM; nCntLogo++)
	{
		m_apLogo[nCntLogo] = NULL;
	}
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CReport::~CReport()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CReport::Init(void)
{
	// ���S����
	m_apLogo[0] = CLogo::Create(RESULT_LOGO_POS, RESULT_LOGO_SIZE, CTexture::TYPE_RESLUT_LOGO000, 0, CLogo::TYPE_LOGO);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CReport::Uninit(void)
{
	// �����x�̔j��
	if (m_pSatisfaction != NULL)
	{
		m_pSatisfaction->Uninit();
		m_pSatisfaction = NULL;
	}

	// �g�[�^���X�R�A�̔j��
	if (m_pTotalScore != NULL)
	{
		m_pTotalScore->Uninit();
		m_pTotalScore = NULL;
	}

	// ���S�̔j��
	for (int nCntLogo = 0; nCntLogo < REPORT_LOGO_NUM; nCntLogo++)
	{
		if (m_apLogo[nCntLogo] != NULL)
		{
			m_apLogo[nCntLogo]->Uninit();
			m_apLogo[nCntLogo] = NULL;
		}
	}

	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CReport::Update(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputMouse *pCInputMouse = CManager::GetInputMouse();
	//�T�E���h�̏��
	CSound *pSound = CManager::GetSound();

	//���݂̃��[�h�̎擾
	CManager::MODE GameMode = CManager::GetMode();

	SetDisplay();

}
//=============================================================================
// �`�揈��
//=============================================================================
void CReport::Draw(void)
{
	// NULL�`�F�b�N���ĕ`��
	if (m_pSatisfaction != NULL)
	{
		m_pSatisfaction->Draw();
	}
	if (m_pTotalScore != NULL)
	{
		m_pTotalScore->Draw();
	}
	for (int nCntLogo = 0; nCntLogo < REPORT_LOGO_NUM; nCntLogo++)
	{
		if (m_apLogo[nCntLogo] != NULL)
		{
			m_apLogo[nCntLogo]->Draw();
		}
	}
}
//===============================================================================
//�@�\������
//===============================================================================
void CReport::SetDisplay()
{
	if (m_type == TYPE_ALL_DISPLAY)
	{	// �S�ĕ\��
		produceAll();

		if (m_pTotalScore != NULL) { m_pTotalScore->RouletteNone();	}	// ���[���b�g�����Ȃ�(�X�R�A���o��)
	}
	else
	{	// 1���\��
		produce();
	}
}
//===============================================================================
//�@��������
//===============================================================================
void CReport::produce()
{
	if (m_apLogo[1] == NULL && m_nCntLogo == 0 && m_type == TYPE_TOTALLOGO)
	{
		m_nCntLogo++;
		m_apLogo[1] = CLogo::Create(TOTAL_LOGO_POS, TOTAL_LOGO_SIZE, CTexture::TYPE_RESLUT_LOGO001, 0, CLogo::TYPE_LOGO);
	}
	else if (m_pTotalScore == NULL && m_nCntLogo == 1 && m_type == TYPE_TOTALSCORE)
	{
		m_nCntLogo++;
		m_pTotalScore = CTotalScore::Create(TOTALSCORE_POS);
		m_pTotalScore->AddTotalScore(m_nTotalScore);
		CLogo::Create(D3DXVECTOR3(TOTALSCORE_POS.x - 320.0f, TOTALSCORE_POS.y, 0.0f), D3DXVECTOR2(30.0f, 30.0f), CTexture::TYPE_MONEY_TEN, 0, CLogo::TYPE_LOGO);
	}
	else if (m_apLogo[2] == NULL && m_nCntLogo == 2 && m_type == TYPE_CLAS)
	{
		m_nCntLogo++;
		m_apLogo[2] = CLogo::Create(CLAS_POS, CLAS_SIZE, CTexture::TYPE_RESULT_LOGO002, 0, CLogo::TYPE_LOGO);
	}
	else if (m_pSatisfaction == NULL && m_nCntLogo == 3 && m_type == TYPE_CLASLEVEL)
	{
		m_nCntLogo++;
		m_pSatisfaction = CSatisfaction::Create(CLAS_LEVEL_POS, CLAS_LEVEL_SIZE, m_nLevel);
	}
	else if (m_apLogo[3] == NULL && m_nCntLogo == 4 && m_type == TYPE_OK_KEY)
	{
		m_nCntLogo++;
		m_apLogo[3] = CLogo::Create(PRESS_BUTTON_POS, PRESS_BUTTON_SIZE, CTexture::TYPE_RESULT_BUTTON, 0, CLogo::TYPE_LOGO);
	}
}
//===============================================================================
//�@�S�Đ�������
//===============================================================================
void CReport::produceAll()
{
	for (int nCntProduce = m_nType; nCntProduce < TYPE_MAX - 1; nCntProduce++, m_type = (REPORTTYPE)nCntProduce)
	{
		produce();
	}
}
//===============================================================================
//�@�N���G�C�g
//===============================================================================
CReport * CReport::Create(REPORTTYPE type, int nTotalScore, int nLevel)
{
	CReport *pReport = NULL;

	//NULL�`�F�b�N
	if (pReport == NULL)
	{//�������̓��I�m��

		pReport = new CReport;

		if (pReport != NULL)
		{
			//��ނ����蓖�Ă�
			pReport->m_type = type;
			// �g�[�^���X�R�A�����蓖�Ă�
			pReport->m_nTotalScore = nTotalScore;
			// �����x�̃��x�������蓖�Ă�
			pReport->m_nLevel = nLevel;
			//�I�u�W�F�N�g�N���X�̐���
			pReport->Init();
		}
	}
	return pReport;
}
