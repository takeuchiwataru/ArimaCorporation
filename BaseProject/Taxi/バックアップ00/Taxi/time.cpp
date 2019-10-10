//=============================================================================
//
// �^�C�}�[���� [time.cpp]
// Author : keisuke komatsu
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "time.h"
#include "number.h"
#include "game.h"
#include "numberdisplay.h"
#include "fade.h"
#include "logo.h"
#include "gamecamera.h"
#include "tutorial.h"
#include "player.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define CENTER_WIDTH		(SCREEN_WIDTH / 2)				// X�̒��S���W
#define TIMER_SPACE			(50.0f)							// �����Ɛ����̊Ԃ̃T�C�Y(�Q�[������)
#define TIMER_POSITION_Y	(70.0f)							// �^�C�}�[��Y���W(�Q�[������)
#define MAX_TIME			(999)							// �ő吧������(�Q�[������)
#define TIME_SPACE			(40.0f)							// �����Ɛ����̊Ԃ̃T�C�Y(���q�l�̎���)
#define TIME_POSITION_Y		((SCREEN_HEIGHT / 2) - 80.0f)	// �^�C�}�[��Y���W(���q�l�̎���)
#define TIME_POSITION_X		((SCREEN_WIDTH / 2) + 180.0f)	// �^�C�}�[��X���W(���q�l�̎���)
#define GAME_TIME			(25)							// �Q�[�����̎���
#define POWER_X				(10)
#define DIRECTING_FRAME		(2)							// Time Up �\���̃t���[������
//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================
int						CTime::m_nTime = 0;
int						CTime::m_nCutomerTime = 0;
bool					CTime::m_bRideEnd = false;
CTime::STATETIME		CTime::m_stateTime = STATE_NONE;
CTime::STATECUTOMERADD	CTime::m_stateCutomer = STATECUTOMERADD_NONE;

//=============================================================================
// ��������
//=============================================================================
CTime *CTime::Create(D3DXVECTOR3 pos)
{
	CTime *pTime = NULL;

	//NULL�`�F�b�N
	if (pTime == NULL)
	{//���I�m��
		pTime = new CTime;

		//NULL�`�F�b�N
		if (pTime != NULL)
		{
			//�ʒu�̐ݒ�
			pTime->m_pos = pos;
			//����������
			pTime->Init();
		}
	}

	return pTime;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTime::CTime(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_nTimeCount = 0;
	m_nCutomerTime = 0;
	m_nTimeNum = 1;
	m_nCutmoerCount = -1;
	m_nCutmoerNum = 0;
	m_pLogo = NULL;
	m_nCntDirecting = 0;
	m_bUse = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTime::~CTime(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTime::Init(void)
{
	m_bUse = false;
	int nTexData = 0;
	m_nTime = GAME_TIME;
	m_nTimeNum = PowerCalculation(m_nTime, 0);

	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		for (int nCntTime = 0; nCntTime < TIME_MAX; nCntTime++)
		{	// �^�C�}�[�����ݒ�
			m_apNumber[nCntTime] = new CNumber;
			m_apNumber[nCntTime]->Init(D3DXVECTOR3((m_pos.x - TIMER_SPACE * nCntTime), m_pos.y, m_pos.z), 0);
		}
		// �������Ԃ̃��S�̐���
		m_pLogo = CLogo::Create(D3DXVECTOR3(100.0f, 150.0f, 0.0f), D3DXVECTOR2(75.0f, 40.0f), CTexture::TYPE_GAMETIMELOGO, 0, CLogo::TYPE_LOGO);

		// �����̃e�N�X�`���ݒ�
		TexTime(nTexData);
	}

	for (int nCntCutTiem = 0; nCntCutTiem < CUTOMERTIME_MAX; nCntCutTiem++)
	{	// ���q�l�̎��Ԃ̏�����
		m_apCutomerTiem[nCntCutTiem] = NULL;
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTime::Uninit(void)
{
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		for (int nCntTime = 0; nCntTime < TIME_MAX; nCntTime++)
		{	// �^�C�}�[�̔j��
			if (m_apNumber[nCntTime] != NULL)
			{
				m_apNumber[nCntTime]->Uninit();
				m_apNumber[nCntTime] = NULL;
			}
		}
	}

	for (int nCntCutTiem = 0; nCntCutTiem < CUTOMERTIME_MAX; nCntCutTiem++)
	{	// ���q�l�̎��Ԃ̔j��
		if (m_apCutomerTiem[nCntCutTiem] != NULL)
		{
			m_apCutomerTiem[nCntCutTiem]->Uninit();
			m_apCutomerTiem[nCntCutTiem] = NULL;
		}
	}

	// ���S�̔j��
	if (m_pLogo != NULL)
	{
		m_pLogo->Uninit();
		m_pLogo = NULL;
	}

	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTime::Update(void)
{
	//���݂̃��[�h�̎擾
	CManager::MODE GameMode = CManager::GetMode();
	DebugKey();		// �f�o�b�N�p
	CGameCamera * pGameCamera = NULL;
	CSound *pSound = CManager::GetSound();

	if ((GameMode == CManager::MODE_GAME))
	{//��������
		//�Q�[��
		pGameCamera = CGame::GetGameCamera();

		CGameCamera::MODECAMERA mode = pGameCamera->GetModeCamera();

		if (mode == CGameCamera::MODE_START) { return; }
		m_nTimeCount++;
		m_nCutmoerCount++;

		//TimeManagement();	// ���Ԃ̊Ǘ�
		if (m_nTime == 0)
		{	// �Q�[���I��
			if (m_bUse == false)
			{
				pSound->StopSound();
				m_bUse = true;
			}

			/*if (m_nCntDirecting == 0 && GameMode == CManager::MODE_GAME)
			{	// Time Up �\��
				CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR2(300.0f, 100.0f), CTexture::TYPE_TIME_UP, 0, CLogo::TYPE_LOGO);
				pSound->SetVolume(CSound::SOUND_LABEL_BGM_SELECT, 0.7f);
				pSound->PlaySound(CSound::SOUND_LABEL_BGM_SELECT);
				pSound->SetVolume(CSound::SOUND_LABEL_TIME_UP, 3.0f);
				pSound->PlaySound(CSound::SOUND_LABEL_TIME_UP);
			}

			if (GameMode == CManager::MODE_GAME && m_nCntDirecting == DIRECTING_FRAME)
			{	// ���U���g��
				CFade::Create(CManager::MODE_RESULT);
			}*/

			if (CManager::MODE_TUTORIAL != GameMode)
			{
				CGame::GetPlayer()->SetControl(false);
			}
		}
		else if (m_nTime >= MAX_TIME)
		{	// �������Ԃ��ő�ȏ�̏ꍇ
			m_nTime = MAX_TIME;
		}
		SetCutomer();		// ���q�l�̎��Ԃ̐����ƊǗ�

		int nTexData = 0;

		// �����̃e�N�X�`���ݒ�
		TexTime(nTexData);
		TexTimCutomerTimee(nTexData);

		if (m_pLogo != NULL) { m_pLogo->Update(); }
	}
	else if (GameMode == CManager::MODE_TUTORIAL)
	{
		pGameCamera = CTutorial::GetCamera();
		m_nCutmoerCount++;
		TimeManagement();	// ���Ԃ̊Ǘ�
		SetCutomer();		// ���q�l�̎��Ԃ̐����ƊǗ�

		int nTexData = 0;
		TexTimCutomerTimee(nTexData);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTime::Draw(void)
{
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		for (int nCntTime = 0; nCntTime < m_nTimeNum; nCntTime++)
		{
			if (m_apNumber[nCntTime] != NULL)
			{
				m_apNumber[nCntTime]->Draw();
			}
		}
	}

	for (int nCntCutTiem = 0; nCntCutTiem < m_nCutmoerNum; nCntCutTiem++)
	{
		if (m_apCutomerTiem[nCntCutTiem] != NULL)
		{
			m_apCutomerTiem[nCntCutTiem]->Draw();
		}
	}

	if (m_pLogo != NULL) { m_pLogo->Draw(); }
}

//=============================================================================
// ���q�l�̎��Ԃ̊Ǘ�(�����Ɣj��)
//=============================================================================
void CTime::SetCutomer(void)
{
	if (m_nCutomerTime <= 0)
	{	// �J�E���g��0�̏ꍇ
		m_nCutomerTime = 0;
		m_bRideEnd = true;
		for (int nCntCutTiem = 0; nCntCutTiem < CUTOMERTIME_MAX; nCntCutTiem++)
		{
			if (m_apCutomerTiem[nCntCutTiem] != NULL)
			{	// ���q�l�̎��Ԃ�����Ȃ�I��������
				m_apCutomerTiem[nCntCutTiem]->Uninit();
				m_apCutomerTiem[nCntCutTiem] = NULL;
			}
		}
	}
	else
	{
		for (int nCntCutTiem = 0; nCntCutTiem < CUTOMERTIME_MAX; nCntCutTiem++)
		{
			if (m_apCutomerTiem[nCntCutTiem] == NULL)
			{	// �Ȃ��ꍇ�͐���������
				m_apCutomerTiem[nCntCutTiem] = new CNumber;
				m_apCutomerTiem[nCntCutTiem]->Init(D3DXVECTOR3((TIME_POSITION_X - TIME_SPACE * nCntCutTiem), 450.0f, 0.0f), 0);
				m_apCutomerTiem[nCntCutTiem]->SetSize(D3DXVECTOR2(30.0f, 26.0f), D3DXVECTOR2((TIME_POSITION_X - TIME_SPACE * nCntCutTiem), 450.0f));
			}
		}
	}
}

//=============================================================================
// �^�C�}�[��Texture�Ǘ�
//=============================================================================
void CTime::TexTime(int nTexData)
{
	nTexData = m_nTime;

	for (int nCntTime = 0; nCntTime < m_nTimeNum; nCntTime++)
	{// �e�N�X�`���ɔ��f

		if (m_apNumber[nCntTime] != NULL)
		{
			m_apNumber[nCntTime]->SetNumber(nTexData);

			nTexData /= 10;
			// �F�̐ݒ�
			if (m_nTime <= 10)
			{ // 10�b�ȉ� �F��Ԃ�
				m_apNumber[nCntTime]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
				m_pLogo->SetColor(&D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}
			else
			{
				m_apNumber[nCntTime]->SetCol(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
				m_pLogo->SetColor(&D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
			}
		}
	}
}

//=============================================================================
// �^�C�}�[��Texture�Ǘ�
//=============================================================================
void CTime::TexTimCutomerTimee(int nTexData)
{
	nTexData = m_nCutomerTime;
	for (int nCntCutTiem = 0; nCntCutTiem < m_nCutmoerNum; nCntCutTiem++)
	{// �e�N�X�`���ɔ��f
		if (m_apCutomerTiem[nCntCutTiem] != NULL)
		{
			m_apCutomerTiem[nCntCutTiem]->SetNumber(nTexData);
			nTexData /= 10;
			// �F�̐ݒ�
			if (m_nCutomerTime <= 10)
			{ // 10�b�ȉ� �F��Ԃ�
				m_apCutomerTiem[nCntCutTiem]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}
			else
			{
				m_apCutomerTiem[nCntCutTiem]->SetCol(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
			}
		}
	}
}

//=============================================================================
// �^�C�����Z����
//=============================================================================
void CTime::AddTime(int nTime)
{
	if (m_nTime > 0)
	{
		m_nTime += nTime;
	}
	m_stateTime = STATE_TIMEADD;
}
//=============================================================================
// �ׂ���̌v�Z
//=============================================================================
int CTime::PowerCalculation(int nData, int nOperation)
{
	int nNum = 1;
	int nPower = (int)std::pow(POWER_X, nNum);
	int nDataNum = nData;
	while ((nDataNum / nPower) != 0)
	{
		nNum++;
		nPower = (int)std::pow(POWER_X, nNum);		// �ׂ��悷��(����O���Ȃ�)
		if (nOperation == 0)
		{	// ��������
			if (nNum >= TIME_MAX) { nNum = TIME_MAX; }
		}
		else if (nOperation == 1)
		{	// ��Ԏ���
			if (nNum > CUTOMERTIME_MAX) { nNum = CUTOMERTIME_MAX - 1; }
		}
	}
	return nNum;
}

//=============================================================================
// ���Ԃ̊Ǘ�
//=============================================================================
void CTime::TimeManagement(void)
{
	if (m_stateTime == STATE_TIMEADD)
	{	// �������ԉ��Z
		m_nTimeNum = PowerCalculation(m_nTime, 0);
		m_stateTime = STATE_NONE;
	}
	if (m_stateCutomer == STATECUTOMERADD_CUTOMERADD)
	{	// ��Ԏ��ԉ��Z
		m_nCutmoerNum = PowerCalculation(m_nCutomerTime, 0);
		m_stateCutomer = STATECUTOMERADD_NONE;
	}
	if (m_nTimeCount % 60 == 0)
	{// 1�b���ƂɌ��Z(��������)
		m_nTime--;
		if (m_nTime < 0) { m_nTime = 0;
		m_nCntDirecting++; }
		m_nTimeNum = PowerCalculation(m_nTime, 0);
	}
	if (m_nCutmoerCount % 60 == 0)
	{	//1�b���ƂɌ��Z(��Ԏ���)
		if (CManager::GetMode() == CManager::MODE_TUTORIAL && CTutorial::GetType() == CTutorial::TYPE_MIDDLE3 ||
			CTutorial::GetType() == CTutorial::TYPE_MIDDLE4 || CTutorial::GetType() == CTutorial::TYPE_MIDDLE5)
		{
			return;
		}
		m_nCutomerTime--;
		m_nCutmoerNum = PowerCalculation(m_nCutomerTime, 0);
	}
}
//=============================================================================
// �f�o�b�N�p
//=============================================================================
void CTime::DebugKey(void)
{
	// �f�o�b�N�p
	if (CManager::GetInput()->GetKeyboardTrigger(DIK_1))
	{	// �������ԉ��Z
		AddTime(30);
	}
	if (CManager::GetInput()->GetKeyboardTrigger(DIK_4))
	{	// ���q�l�̎��Ԃ̉��Z
		SetCutomerTime(5);
	}
	if (CManager::GetInput()->GetKeyboardTrigger(DIK_7))
	{	// ���U���g��ʂ�
		CGame::SetGameState(CGame::GAMESTATE_CLEAR);
	}
}