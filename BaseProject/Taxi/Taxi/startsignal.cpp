//************************************************************
//
//	�X�^�[�g���}�̏���[startsignal.h]
//	Author : Shun Yokomichi
//
//************************************************************
#include "startsignal.h"
#include "logo.h"
#include "gamecamera.h"

//************************************************************
//	�}�N����`
//************************************************************
#define SIGNAL_POS		(D3DXVECTOR3(480.0f + (320 * nCntSignal), SCREEN_HEIGHT / 2, 0.0f))		// �����ʒu
#define PURPOSE_POS		(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f))						// �ړI�n�̈ʒu
#define ADD_MOVE		(8.0f)																			// �ړ��̉��Z��
#define ADD_SIZE		(3.0f)																			// �T�C�Y�̉��Z��
#define COL_A_REDUCED	(0.01f)																			// �����x�̌��Z��
#define COL_DEFAULT		(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fColA))											// �f�t�H���g�̐F
//************************************************************
//	�R���X�g���N�^
//************************************************************
CStartSignal::CStartSignal() 
{
	m_pStart = NULL;	// �X�^�[�g���}
	m_SignalType = SIGNAL_NONE;	// ���}�̏��
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_fColA = 1.0f;
	m_bEnd = false;
	m_bColA = false;
	m_nCntDisplay = 0;
	for (int nCntSignal = 0; nCntSignal < MAX_SIGNAL; nCntSignal++)
	{
		m_pSignal[nCntSignal] = NULL;
	}
}
//************************************************************
//	�f�X�g���N�^
//************************************************************
CStartSignal::~CStartSignal() {}

//************************************************************
//	������
//************************************************************
HRESULT CStartSignal::Init() 
{
	for (int nCntSignal = 0; nCntSignal < MAX_SIGNAL; nCntSignal++)
	{
		if (m_pSignal[nCntSignal] == NULL)
		{
			m_pSignal[nCntSignal] = CLogo::Create(SIGNAL_POS, m_size,CTexture::TYPE_START_SIGNAL000, 0, CLogo::TYPE_LOGO );
			m_pSignal[nCntSignal]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}
	}

	return S_OK;
}

//************************************************************
//	�I������
//************************************************************
void CStartSignal::Uninit() 
{
	// �X�^�[�g���}�̔j��
	for (int nCntSignal = 0; nCntSignal < MAX_SIGNAL; nCntSignal++)
	{
		if (m_pSignal[nCntSignal] != NULL)
		{
			m_pSignal[nCntSignal]->Uninit();
			m_pSignal[nCntSignal] = NULL;
		}
	}

	// �J�n���̃��S
	m_pStart = NULL;
}

//************************************************************
//	�X�V����
//************************************************************
void CStartSignal::Updata() 
{
	bool bFalse = false;
	if (m_pSignal[0] != NULL) {
		bFalse = m_pSignal[0]->GetMoveFlag();}

	switch (m_SignalType)
	{
	case SIGNAL_HERE:	SignalUpdata(bFalse, (int)m_SignalType);	break;
	case SIGNAL_WE:		SignalUpdata(bFalse, (int)m_SignalType);	break;
	case SIGNAL_GO:		StartUpdata();								break;
	default:			break;
	}
}

//************************************************************
// �`�揈��
//************************************************************
void CStartSignal::Draw() 
{
	for (int nCntSignal = 0; nCntSignal < MAX_SIGNAL; nCntSignal++)
	{
		if (m_pSignal[nCntSignal] != NULL)
		{
			m_pSignal[nCntSignal]->Draw();
		}
	}

	if (m_pStart != NULL) { m_pStart->Draw(); }
}

//************************************************************
//	��������
//************************************************************
CStartSignal *CStartSignal::Create(D3DXVECTOR2 size, SIGNAL type)
{
	CStartSignal *pStartSignal = NULL;
	CManager::MODE mode = CManager::GetMode();

	//NULL�`�F�b�N
	if (pStartSignal == NULL)
	{//�������̓��I�m��

		pStartSignal = new CStartSignal;

		if (pStartSignal != NULL)
		{
			//�T�C�Y�̊��蓖��
			pStartSignal->m_size = size;
			// ���}�̏��
			pStartSignal->m_SignalType = type;
			//�I�u�W�F�N�g�N���X�̐���
			pStartSignal->Init();
		}
	}
	return pStartSignal;
}

//************************************************************
//	�X�^�[�g���}�̏���
//************************************************************
void CStartSignal::SignalUpdata(bool bFrag, int nType)
{
	bool bChange = false;
	if (bFrag == true && nType == 0) { bChange = true; }
	if (bFrag == false && nType == 1) { bChange = true;}

	if (bChange == true) { m_nCntDisplay++; }
	for (int nCntSignal = 0; nCntSignal < MAX_SIGNAL; nCntSignal++)
	{
		if (m_pSignal[nCntSignal] != NULL)
		{
			if (bChange == true)
			{	// �����ʒu�ɖ߂� & Texture�̕ύX
				if ((m_nCntDisplay % 20) == 0)
				{
					m_pSignal[nCntSignal]->SetPosition(SIGNAL_POS);
					m_pSignal[nCntSignal]->BindTexture(*CTexture::GetTexture(CTexture::TYPE_START_SIGNAL000 + nType + 1));
					if (bFrag == true && nType == 0) { m_SignalType = SIGNAL_WE; }
					if (bFrag == false && nType == 1) {m_SignalType = SIGNAL_GO; }
				}
				else { m_pSignal[nCntSignal]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)); }
			}
			else if (bChange == false)
			{	// �ʒu�ړ�
				float fMove = ADD_MOVE;
				m_pSignal[nCntSignal]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
				if (nCntSignal == 1) { fMove = ADD_MOVE * -1; }
				D3DXVECTOR3 pos = m_pSignal[nCntSignal]->GetPosition();
				pos = m_pSignal[nCntSignal]->MoveLimit(pos, PURPOSE_POS, nCntSignal, fMove);
				m_pSignal[nCntSignal]->SetPosition(pos);
				m_pSignal[nCntSignal]->Update();
			}
		}
	}

	for (int nCntSignal = 0; nCntSignal < MAX_SIGNAL; nCntSignal++)
	{	// false�̏ꍇ�͏I������
		if ((m_nCntDisplay % 20) == 0)
		{
			if (bFrag == false && nType == 1 && m_pSignal[nCntSignal] != NULL) { m_pSignal[nCntSignal]->Uninit(); m_pSignal[nCntSignal] = NULL; }
		}
	}

}

//************************************************************
// �Q�[���J�n���̍��}
//************************************************************
void CStartSignal::StartUpdata()
{
	CGameCamera * pGameCamera = CGame::GetGameCamera();

	if (m_pStart == NULL)
	{
		m_pStart = CLogo::Create(PURPOSE_POS, m_size, CTexture::TYPE_START_SIGNAL002, 0, CLogo::TYPE_LOGO);
	}
	if (m_pStart != NULL)
	{
		m_fColA -= COL_A_REDUCED;
		m_size.x += ADD_SIZE;
		m_size.y += ADD_SIZE / 2;
		m_pStart->SetPosSize(PURPOSE_POS, m_size);
		m_pStart->SetColor(&COL_DEFAULT);
		if (m_fColA <= 0.3f) { m_pStart->Uninit(); m_bEnd = true; m_SignalType = SIGNAL_NONE; m_pStart = NULL; }
	}
}