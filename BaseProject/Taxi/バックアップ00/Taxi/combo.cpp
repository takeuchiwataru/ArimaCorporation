//=============================================================================
//
// �R���{���� [combo.cpp]
// Author : shun yokomichi
//
//=============================================================================
#include "combo.h"
#include "manager.h"
#include "number.h"
#include "logo.h"
#include "input.h"
#include "game.h"
#include "texture.h"
//=============================================================================
// �}�N����`
//=============================================================================
#define POS				(D3DXVECTOR3(1060.0f, 220.0f, 0.0f))										// �R���{�̈ʒu
#define SIZE			(D3DXVECTOR2(90.0f, 23.0f))													// �R���{�̃T�C�Y
#define NUMBER_POS		(D3DXVECTOR3(((POS.x + 160.0f) - 45.0f * nCntNum), POS.y + 40.0f, POS.z))	// �����̈ʒu
#define NUMBER_SIZE		(D3DXVECTOR2(40.0f, 35.0f))													// �����̃T�C�Y
#define LOGO_POS		(D3DXVECTOR3(POS.x + 30.0f, POS.y + 40.0f, POS.z))							// ���S�̈ʒu
#define LOGO_SIZE		(D3DXVECTOR2(60.0f, 15.0f))													// ���S�̃T�C�Y
#define NUMBER_POS_START (D3DXVECTOR2(((POS.x + 250.0f)), POS.y + 40.0f))							// �����̏o���ʒu
#define SAID_COMBO		(2)																			// �R���{�����w��ȏ�̏ꍇ�\������
#define POWER_X			(10)																		// �ׂ���̌v�Z��
#define NUM_MOVE		(22.0f)																		// �R���{���̐����ړ���
#define DISPLAY_TIME	(120)																		// �\������
//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
int				   CCombo::m_nComboNum = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCombo::CCombo() : CScene2D(5, CScene::OBJTYPE_COMBO)//�D��D�ʃC�j�V�����C�Y
{
	m_nCntFrame = 0;
	m_nComboOld = 0;
	m_DisNumber = 0;
	m_nDiSTime = 0;
	m_NumPos = D3DXVECTOR2(0.0f, 0.0f);
	for (int nCntNum = 0; nCntNum < MAX_COMBO; nCntNum++)
	{
		m_apNumber[nCntNum] = NULL;
	}
	m_pLogo = NULL;
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCombo::~CCombo(){}

//=============================================================================
// ��������
//=============================================================================
CCombo * CCombo::Create(TYPE type)
{
	CCombo *pCombo = NULL;

	//NULL�`�F�b�N
	if (pCombo == NULL)
	{//�������̓��I�m��
		pCombo = new CCombo;
		if (pCombo != NULL)
		{
			//�I�u�W�F�N�g�N���X�̐���
			pCombo->Init();
			//�e�N�X�`�������蓖�Ă�
			pCombo->BindTexture(*CTexture::GetTexture((int)CTexture::TYPE_THROUGH + (int)type));
			//�ʒu�����蓖�Ă�
			pCombo->SetPosition(POS);
			//�T�C�Y�̊��蓖��
			pCombo->m_size = SIZE;
			pCombo->m_type = type;
		}
	}
	return pCombo;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCombo::Init(void)
{
	m_nComboNum = 1;
	m_nComboOld = m_nComboNum;
	m_nCntFrame = COMBO_DISTIME;
	CScene2D::Init();

	if (m_pLogo == NULL)
	{
		m_pLogo = CLogo::Create(LOGO_POS, LOGO_SIZE, CTexture::TYPE_COMBO, 0, CLogo::TYPE_LOGO);
	}
	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CCombo::Uninit(void)
{
	m_nComboNum = 0;

	for (int nCntNum = 0; nCntNum < MAX_COMBO; nCntNum++)
	{
		if (m_apNumber[nCntNum] != NULL)
		{
			m_apNumber[nCntNum]->Uninit();
			m_apNumber[nCntNum] = NULL;
		}
	}

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
void CCombo::Update(void)
{
	m_nCntFrame--;
	m_nDiSTime++;
	m_DisNumber = PowerCalculation(m_nComboNum);	// �ׂ���v�Z
	if (m_nComboNum == SAID_COMBO)
	{	// 2�R���{�ڂ��琔���\��
		for (int nCntNum = 0; nCntNum < MAX_COMBO; nCntNum++)
		{
			if (m_apNumber[nCntNum] == NULL)
			{
				m_apNumber[nCntNum] = new CNumber;
				m_apNumber[nCntNum]->Init(D3DXVECTOR3(NUMBER_POS_START.x, NUMBER_POS_START.y, 0.0f), 0);
				m_apNumber[nCntNum]->SetSize(NUMBER_SIZE, NUMBER_POS_START);
				m_apNumber[nCntNum]->SetCol(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
				m_NumPos = NUMBER_POS_START;
			}
		}
	}

	NumMove();		// �R���{���̈ړ�����
	int nTexData = m_nComboNum;
	for (int nCntNum = 0; nCntNum < m_DisNumber; nCntNum++)
	{
		if (m_apNumber[nCntNum] != NULL)
		{	// Texture�̐ݒ�
			m_apNumber[nCntNum]->SetNumber(nTexData);
			nTexData /= 10;
			m_apNumber[nCntNum]->SetSize(NUMBER_SIZE, D3DXVECTOR2(m_NumPos.x - 45.0f * nCntNum, m_NumPos.y));
		}
	}

	if (m_nComboOld != m_nComboNum)
	{	// �R���{�����Z���ꂽ�ꍇ
		m_nCntFrame = COMBO_DISTIME;
		m_nComboOld = m_nComboNum;
		m_NumPos = NUMBER_POS_START;
		m_nDiSTime = 0;
	}

	//���S�̈ʒu�ƃT�C�Y
	CScene2D::SetPosSize(POS, SIZE);
}

//=============================================================================
// �`�揈��
//=============================================================================
void CCombo::Draw(void)
{
	for (int nCntNum = 0; nCntNum < m_DisNumber; nCntNum++)
	{
		if (m_apNumber[nCntNum] != NULL)
		{
			m_apNumber[nCntNum]->Draw();
		}
	}

	if (m_pLogo != NULL)
	{
		m_pLogo->Draw();
	}

	CScene2D::Draw();

}

//=============================================================================
// �ׂ���̌v�Z
//=============================================================================
int CCombo::PowerCalculation(int nData)
{
	int nNum = 1;
	int nPower = (int)std::pow(POWER_X, nNum);
	int nDataNum = nData;
	while ((nDataNum / nPower) != 0)
	{
		nNum++;
		nPower = (int)std::pow(POWER_X, nNum);		// �ׂ��悷��(����O���Ȃ�)
		if (nNum >= MAX_COMBO) { nNum = MAX_COMBO; }
	}
	return nNum;
}

//=============================================================================
// �R���{���̈ړ�����
//=============================================================================
void CCombo::NumMove(void)
{
	m_NumPos.x -= NUM_MOVE;
	if (m_NumPos.x < (POS.x + 160.0f))
	{
		m_NumPos.x = (POS.x + 160.0f);
	}
}