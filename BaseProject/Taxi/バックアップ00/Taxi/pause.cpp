//=============================================================================
//
// �|�[�Y�̏��� [pause.cpp]
// Author : �L�n���u
//
//=============================================================================
#include "scene.h"
#include "pause.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "fade.h"
#include "texture.h"
#include "logo.h"
#include "background.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_PAUSE				(128)							//�|�[�Y�̍ő吔
#define PAUSE_POS_X				(0)								//�|�[�Y�̍���X���W
#define PAUSE_POS_Y				(0)								//�|�[�Y�̍���Y���W
#define PAUSE_WIDTH				(SCREEN_WIDTH)					//�|�[�Y�̕�
#define PAUSE_HEIGHT			(SCREEN_HEIGHT)					//�|�[�Y�̍���
#define PAUSE_TEXTURE_UV_U		(1.0f)							//�e�N�X�`���A�j���[�V����U�͈�
#define PAUSE_TEXTURE_UV_V		(1.0f)							//�e�N�X�`���A�j���[�V����V�͈�
#define PAUSE_SPEED				(0.003f)						//�|�[�Y�X�s�[�h

//===============================================
// �ʒu�ƃT�C�Y(�����ʒu)
//===============================================
#define LOGO_POS				(D3DXVECTOR3(PAUSE_WIDTH / 2, 100.0f, 0.0f))				// ���S�̈ʒu
#define LOGO_SIZE				(D3DXVECTOR2(200.0f, 60.0f))								// ���S�̃T�C�Y
#define LOGO_ADD_SIZE_X			(11.0f)														// �T�C�Y�̉��Z(X)
#define LOGO_ADD_SIZE_Y			(3.0f)														// �T�C�Y�̉��Z(Y)
#define OPERATION_POS			(D3DXVECTOR3(1020.0f, 450.0f, 0.0f))						// ������@�̈ʒu
#define OPERATION_SIZE			(D3DXVECTOR2(230.0f, 230.0f))								// ������@�̃T�C�Y
#define OPERATION_ADD_SIZE_X	(12.0f)														// �T�C�Y�̉��Z(X)
#define OPERATION_ADD_SIZE_Y	(15.0f)														// �T�C�Y�̉��Z(Y)
#define SIZE_SMALL				(D3DXVECTOR2(0.0f, 0.0f))									// ������@�̃T�C�Y(������)

//===============================================
// �I�����A�C�R���֌W
//===============================================
#define ICON_POS				(D3DXVECTOR3(340.0f - 250.0f, 300.0f, 0.0f))				// �I�����A�C�R���̈ʒu
#define ICON_SIZE				(D3DXVECTOR2(40.0f, 40.0f))									// �I�����A�C�R���̃T�C�Y
#define ADD_MOVE_NOMAL			(0.5f)														// �I�����A�C�R���̈ړ���(�ʏ펞)
#define ADD_MOVE_SELECT			(6.0f)														// �I�����A�C�R���̈ړ���(�I����)
#define CURRENT_POS				(D3DXVECTOR3(SelectPos.x, m_apLogo[m_SelectNum]->GetPosition().y, SelectPos.z))	// �I�����A�C�R���̌��݈ʒu
//===============================================
// ���j���[�֌W
//===============================================
#define MENU_POS				(D3DXVECTOR3(300.0f, 250.0f + (nCntMenu * 170.0f), 0.0f))	// continu �` Quit�܂ňʒu
#define DEFAULT_POS_LEFT		(D3DXVECTOR3(0.0f, MENU_POS.y, 0.0f))						// �f�t�H���g�̈ʒu(��)�̏ꍇ
#define DEFAULT_POS_RIGHT		(D3DXVECTOR3(SCREEN_WIDTH / 2, MENU_POS.y, 0.0f))			// �f�t�H���g�̈ʒu(�E)�̏ꍇ
#define END_POS_LEFT			(D3DXVECTOR3(-200.0f, MENU_POS.y, 0.0f))					// �I����̈ʒu(��)�̏ꍇ
#define END_POS_RIGHT			(D3DXVECTOR3(SCREEN_WIDTH + 200.0f, MENU_POS.y, 0.0f))		// �I����̈ʒu(�E)�̏ꍇ
#define MENU_SIZE				(D3DXVECTOR2(200.0f, 60.0f))								// continu �` Quit�܂ŃT�C�Y
#define MOVE					(27.0f)														// ���E�̈ړ���
//===============================================
// �|�[�Y����鎞�̃J�E���^
//===============================================
#define PAUSE_END_FRAME			(30)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
bool			   CPause::m_bPause = false;
bool			   CPause::m_bSerectEnd = false;

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CPause::CPause(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_SelectColor = 0.0f;
	m_nCntState = 0;
	m_SelectNum = 0;
	m_nSize = D3DXVECTOR2(0.0f, 0.0f);
	m_pBg = NULL;
	m_pSelectIcon = NULL;
	m_state = PAUSE_NORMAL;
	m_OperationNum = 0;
	m_nCntPauseEnd = 0;
	for (int nCntLogo = 0; nCntLogo < MAX_PAUSE_POLYGON; nCntLogo++)
	{
		m_apLogo[nCntLogo] = NULL;
	}
	m_nPauseCounter = 0;
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CPause::~CPause()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CPause::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_pos = D3DXVECTOR3((PAUSE_POS_X + PAUSE_WIDTH) / 2, (PAUSE_POS_Y + PAUSE_HEIGHT) / 2, 0.0f);
	m_SelectNum = 0;
	m_SelectColor = 1.0f;

	if (m_pBg == NULL) { m_pBg = CBackGround::Create(CTexture::TYPE_PAUSE_BG); }

	// ���j���[�̐���
	for (int nCntMenu = 0; nCntMenu < MAX_PAUSE_POLYGON - 2; nCntMenu++)
	{
		if (m_apLogo[nCntMenu] == NULL)
		{
			if ((nCntMenu % 2) == 0)
			{
				m_apLogo[nCntMenu] = CLogo::Create(DEFAULT_POS_RIGHT, MENU_SIZE, CTexture::TYPE_PAUSE_CONTINUE + nCntMenu, 0, CLogo::TYPE_LOGO);
			}
			else { m_apLogo[nCntMenu] = CLogo::Create(DEFAULT_POS_LEFT, MENU_SIZE, CTexture::TYPE_PAUSE_CONTINUE + nCntMenu, 0, CLogo::TYPE_LOGO); }

			if (nCntMenu == 0) { m_apLogo[nCntMenu]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)); }
			else { m_apLogo[nCntMenu]->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f)); }
		}
	}

	// ���S�̐���
	if (m_apLogo[3] == NULL)
	{
		m_apLogo[3] = CLogo::Create(LOGO_POS, SIZE_SMALL, CTexture::TYPE_PAUSE_LOGO, 0, CLogo::TYPE_LOGO);
	}

	// ������@�̐���
	if (m_apLogo[4] == NULL)
	{
		m_apLogo[4] = CLogo::Create(OPERATION_POS, SIZE_SMALL, CTexture::TYPE_PAUSE_OPERATION, m_OperationNum, CLogo::TYPE_LOGO);
		m_apLogo[4]->SetTexture(m_OperationNum, 2, 1, 1);
	}

	// �I�����̃A�C�R������
	if (m_pSelectIcon == NULL)
	{
		m_pSelectIcon = CLogo::Create(ICON_POS, ICON_SIZE, CTexture::TYPE_PAUSE_ICON, 0, CLogo::TYPE_LOGO);
	}

	m_bPause = false;
	m_nPauseCounter = 0;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPause::Uninit(void)
{
	//�I��ł��Ԃ�߂�
	m_bSerectEnd = false;

	if (m_pBg != NULL)
	{	// �w�i�̔j��
		m_pBg->Uninit();
		m_pBg = NULL;
	}

	if (m_pSelectIcon != NULL)
	{	// �I�����A�C�R���̔j��
		m_pSelectIcon->Uninit();
		m_pSelectIcon = NULL;

	}
	for (int nCntLogo = 0; nCntLogo < MAX_PAUSE_POLYGON; nCntLogo++)
	{
		if (m_apLogo[nCntLogo] != NULL)
		{	// ���S�̔j��
			m_apLogo[nCntLogo]->Uninit();
			m_apLogo[nCntLogo] = NULL;
		}
	}

	//���g�̍폜
	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPause::Update(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CXInput *pCInputJoypad = CManager::GetXInput();
	CInputMouse *pCInputMouse = CManager::GetInputMouse();

	//�T�E���h�̏��
	CSound *pSound = CManager::GetSound();

	//�t�F�[�h�̃|�C���g
	CFade::FADE pFade = CFade::GetFade();

	//	Pause�̃J�E���^�[�̉��Z
	m_nPauseCounter++;

	// �I�����A�C�R���̈ʒu�擾
	D3DXVECTOR3 SelectPos = m_pSelectIcon->GetPosition();
	D3DXVECTOR3 MenuPos[MAX_PAUSE_POLYGON - 2] = {};
	int nCntNum = 0;														// �񐔂̃J�E���g
	for (int nCntMenu = 0; nCntMenu < MAX_PAUSE_POLYGON - 2; nCntMenu++)
	{	// ���j���[�̈ʒu�擾
		if (m_apLogo[nCntMenu] != NULL) { MenuPos[nCntMenu] = m_apLogo[nCntMenu]->GetPosition(); }
	}

	if (m_state != PAUSE_SELECT)
	{
		if (m_nPauseCounter >=30)
		{
			if ((pCInputKeyBoard->GetKeyboardTrigger(DIK_UP) == true || pCInputKeyBoard->GetKeyboardTrigger(DIK_W) == true || pCInputJoypad->GetRepeat(CXInput::XIJS_BUTTON_0) == true || pCInputJoypad->GetRepeat(CXInput::XIJS_BUTTON_16)) && m_bSerectEnd == false)
			{//���L�[�̏���������Ƃ�
				if (m_apLogo[m_SelectNum] != NULL) { m_apLogo[m_SelectNum]->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f)); }
				//�|�[�Y�̑I�����ړ���
				pSound->SetVolume(CSound::SOUND_LABEL_SE_PAUSE_MOVE, 0.8f);
				pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_MOVE);

				m_SelectNum = (m_SelectNum + 2) % 3;
				m_SelectColor = 1.0f;
				if (m_apLogo[m_SelectNum] != NULL) { m_apLogo[m_SelectNum]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, m_SelectColor)); }
			}
			if ((pCInputKeyBoard->GetKeyboardTrigger(DIK_DOWN) == true || pCInputKeyBoard->GetKeyboardTrigger(DIK_S) == true || pCInputJoypad->GetRepeat(CXInput::XIJS_BUTTON_1) == true || pCInputJoypad->GetRepeat(CXInput::XIJS_BUTTON_17)) && m_bSerectEnd == false)
			{//���L�[�̉����������Ƃ�
				if (m_apLogo[m_SelectNum] != NULL) { m_apLogo[m_SelectNum]->SetColor(&D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f)); }
				//�|�[�Y�̑I�����ړ���
				pSound->SetVolume(CSound::SOUND_LABEL_SE_PAUSE_MOVE, 0.8f);
				pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_MOVE);

				m_SelectNum = (m_SelectNum + 1) % 3;
				m_SelectColor = 1.0f;
				if (m_apLogo[m_SelectNum] != NULL) { m_apLogo[m_SelectNum]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, m_SelectColor)); }
			}

			if (pCInputKeyBoard->GetKeyboardTrigger(DIK_RIGHT) == true && m_bSerectEnd == false || pCInputKeyBoard->GetKeyboardTrigger(DIK_D) == true && m_bSerectEnd == false
				|| pCInputKeyBoard->GetKeyboardTrigger(DIK_LEFT) == true && m_bSerectEnd == false || pCInputKeyBoard->GetKeyboardTrigger(DIK_A) == true)
			{//���L�[�̉E���������Ƃ�
				//�|�[�Y�̑I�����ړ���
				pSound->SetVolume(CSound::SOUND_LABEL_SE_PAUSE_MOVE, 0.8f);
				pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_MOVE);
				m_OperationNum = (m_OperationNum + 1) % 2;
				if (m_apLogo[4] != NULL) { m_apLogo[4]->SetTexture(m_OperationNum, 2, 1, 1); }
			}
		}
	}
	// �T�C�Y�̕ύX
	if (m_apLogo[3] != NULL && m_apLogo[4] != NULL)
	{

		if (m_state != PAUSE_SELECT)
		{
			m_apLogo[3]->Zoom(m_apLogo[3]->GetPosition(), LOGO_SIZE, D3DXVECTOR2(LOGO_ADD_SIZE_X, LOGO_ADD_SIZE_Y), true);
			m_apLogo[4]->Zoom(m_apLogo[4]->GetPosition(), OPERATION_SIZE, D3DXVECTOR2(OPERATION_ADD_SIZE_X, OPERATION_ADD_SIZE_Y), true);
		}
		else
		{
			m_apLogo[3]->Zoom(m_apLogo[3]->GetPosition(), SIZE_SMALL, D3DXVECTOR2(LOGO_ADD_SIZE_X, LOGO_ADD_SIZE_Y), false);
			m_apLogo[4]->Zoom(m_apLogo[4]->GetPosition(), SIZE_SMALL, D3DXVECTOR2(OPERATION_ADD_SIZE_X, OPERATION_ADD_SIZE_Y), false);

		}
	}

	// �F�̐ؑ�
	ColChange();
	if (m_nPauseCounter >= 30)
	{
		if ((pCInputKeyBoard->GetKeyboardTrigger(DIK_RETURN) == true || pCInputMouse->GetMouseTrigger(0) == true || pCInputJoypad->GetTrigger(CXInput::XIJS_BUTTON_11) == true || pCInputJoypad->GetTrigger(CXInput::XIJS_BUTTON_10) == true) && m_bSerectEnd == false)
		{
			if (pFade == CFade::FADE_NONE &&m_state != PAUSE_SELECT)
			{
				SelectNumOperation();		// �I����̏���
				//�|�[�Y�̑I���̌��艹
				pSound->SetVolume(CSound::SOUND_LABEL_SE_PAUSE_SELECT, 1.1f);
				pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_SELECT);
			}
			m_state = PAUSE_SELECT;			// �I����Ԃɂ���
		}
	}
	PauseEnd();		// �R���e�B�j���[�I�����̏���

	// �I�����A�C�R���̈ړ�
	SelectPos = SelectMove(SelectPos);

	// ���j���̈ړ�
	for (int nCntMenu = 0; nCntMenu < MAX_PAUSE_POLYGON - 2; nCntMenu++)
	{
		if (m_apLogo[nCntMenu] != NULL)
		{
			if (m_state != PAUSE_SELECT)
			{	// ��ʕ\�����̈ړ�
				if ((nCntMenu % 2) == 0)
				{
					MenuPos[nCntMenu] = m_apLogo[nCntMenu]->MoveLimit(MenuPos[nCntMenu], MENU_POS, 1, -MOVE);
				}
				else { MenuPos[nCntMenu] = m_apLogo[nCntMenu]->MoveLimit(MenuPos[nCntMenu], MENU_POS, 0, MOVE); }
			}
			else
			{	// �񐔂̃J�E���g
				if ((nCntMenu % 2) == 0 && m_SelectNum != nCntMenu) { nCntNum++;}
				else  if (m_SelectNum != nCntMenu) { nCntNum++; }

				if (m_SelectNum != nCntMenu)
				{	// �I��ԍ����J�E���g�ƈႤ�ꍇ ���E�̂ǂ��炩�ɓ�����
					if ((nCntNum % 2) == 0){ MenuPos[nCntMenu] = m_apLogo[nCntMenu]->MoveLimit(MenuPos[nCntMenu], END_POS_LEFT, 1, -MOVE); }
					else { MenuPos[nCntMenu] = m_apLogo[nCntMenu]->MoveLimit(MenuPos[nCntMenu], END_POS_RIGHT, 0, MOVE); }
				}
			}
		}
	}

	// �I�����A�C�R���̍X�V
	if (m_pSelectIcon != NULL) { m_pSelectIcon->SetPosition(CURRENT_POS); m_pSelectIcon->Update(); }
	for (int nCntLogo = 0; nCntLogo < MAX_PAUSE_POLYGON; nCntLogo++)
	{	// ���S�̍X�V
		if (m_apLogo[nCntLogo] != NULL)
		{
			if (nCntLogo < 3) { m_apLogo[nCntLogo]->SetPosition(MenuPos[nCntLogo]); }
			else if (nCntLogo = 4) { m_apLogo[nCntLogo]->SetTexture(m_OperationNum, 2, 1, 1);}
			m_apLogo[nCntLogo]->Update();
		}
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void CPause::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (m_pBg != NULL) { m_pBg->Draw(); }	// �w�i�̕`��

	if (m_pSelectIcon != NULL) { m_pSelectIcon->Draw(); }	// �I�����A�C�R���̍X�V

	for (int nCntLogo = 0; nCntLogo < MAX_PAUSE_POLYGON; nCntLogo++)
	{	// ���S�̕`��
		if (m_apLogo[nCntLogo] != NULL) { m_apLogo[nCntLogo]->Draw(); }
	}
}
//===============================================================================
//�@�N���G�C�g
//===============================================================================
CPause * CPause::Create()
{
	CPause *pPause = NULL;

	//NULL�`�F�b�N
	if (pPause == NULL)
	{//�������̓��I�m��

		pPause = new CPause;

		if (pPause != NULL)
		{
			//�I�u�W�F�N�g�N���X�̐���
			pPause->Init();
		}
	}

	return pPause;
}

//===============================================================================
//�@�I�����A�C�R���̈ړ�
//===============================================================================
D3DXVECTOR3 CPause::SelectMove(D3DXVECTOR3 SelectPos)
{
	// ���[�J���ϐ�
	bool bMoveFlag = false;
	if (m_pSelectIcon != NULL) { bMoveFlag = m_pSelectIcon->GetMoveFlag(); }
	float fAddMove = 0.0f;

	if (m_state == PAUSE_NORMAL) { fAddMove = ADD_MOVE_NOMAL; }
	else if (m_state == PAUSE_SELECT) { fAddMove = ADD_MOVE_SELECT; }

	if (m_pSelectIcon != NULL && bMoveFlag == false)
	{	// �E�Ɉړ�
		SelectPos = m_pSelectIcon->MoveLimit(CURRENT_POS, D3DXVECTOR3(ICON_POS.x + 20.0f, CURRENT_POS.y, CURRENT_POS.z), 0, fAddMove);
	}

	else if (m_pSelectIcon != NULL && bMoveFlag == true)
	{	// ���Ɉړ�
		SelectPos = m_pSelectIcon->MoveLimit(CURRENT_POS, D3DXVECTOR3(ICON_POS.x - 20.0f, CURRENT_POS.y, CURRENT_POS.z), 1, -fAddMove);
	}
	return SelectPos;
}
//===============================================================================
//�@�F�̕ω�
//===============================================================================
void CPause::ColChange(void)
{
	//�_�ŏ���
	if (m_apLogo[m_SelectNum] != NULL) { m_apLogo[m_SelectNum]->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, m_SelectColor)); }
	//�I�����Ă���J���[�̒l�����炵�Ă���
	m_SelectColor -= 0.009f;

	if (m_SelectColor < 0.5f)
	{//�M�����Ă���J���[��0.5��؂�����J���[�̒l�������Ă�
		m_SelectColor = 1.0f;
	}
}

//===============================================================================
//�@�R���e�B�j���[�I�����@�|�[�Y�̃J�E���^����
//===============================================================================
void CPause::PauseEnd(void)
{
	if (m_state == PAUSE_SELECT && m_SelectNum == 0)
	{	// �R���e�B�j���[��I�������ꍇ �J�E���g���񂵂ă|�[�Y���I��
		m_nCntPauseEnd++;
		m_bPause = true;
		if ((m_nCntPauseEnd % PAUSE_END_FRAME) == 0)
		{	//�|�[�Y�����
			CGame::SetPause(false);
			m_bPause = false;
		}
	}
}

//===============================================================================
//�@�|�[�Y�̑I����̏������Ǘ�
//===============================================================================
void CPause::SelectNumOperation(void)
{
	if (m_SelectNum == 1)
	{
		//�Q�[���̍ŏ��ɖ߂�
		CFade::Create(CManager::MODE_GAME);

		//�|�[�Y�őI�񂾏��
		m_bSerectEnd = true;
		m_bPause = true;
	}
	else if (m_SelectNum == 2)
	{
		//�Q�[���̃^�C�g���ɖ߂�
		CFade::Create(CManager::MODE_TITLE);

		//�|�[�Y�őI�񂾏��
		m_bSerectEnd = true;
		m_bPause = true;
	}
}