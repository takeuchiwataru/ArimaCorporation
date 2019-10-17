//=============================================================================
//
// �Z���N�g�̏��� [select.cpp]
// Author : shun yokomichi
//
//=============================================================================
#include "select.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SELECT_LOGO_POS			(D3DXVECTOR3(SCREEN_WIDTH / 2, 70.0f, 0.0f))					// �ʒu(�^�C�g�����S000)
#define SELECT_LOGO_SIZE		(D3DXVECTOR2(200.0f, 60.0f))									// �T�C�Y(�^�C�g�����S000)
#define GAME_POS				(D3DXVECTOR3(SCREEN_WIDTH / 2, 310.0f, 0.0f))					// �Q�[�����(TEXTURE)�̈ʒu
#define GAME_SIZE				(D3DXVECTOR2(500.0f, 250.0f))									// �Q�[�����(TEXTURE)�̃T�C�Y
#define QUIT_POS				(D3DXVECTOR3(GAME_POS.x, GAME_POS.y + 330.0f, 0.0f))			// Quit�̈ʒu
#define QUIT_SIZE				(D3DXVECTOR2(530.0f, 70.0f))									// Quit�̃T�C�Y
#define TOTAL_POS				(D3DXVECTOR3(320.0f, GAME_POS.y + 250.0f, 0.0f))				// �g�[�^���̈ʒu
#define TOTAL_SIZE				(D3DXVECTOR2(200.0f, 60.0f))									// �g�[�^���̃T�C�Y
#define SATYIFACTION_POS		(D3DXVECTOR3(250.0f - 100.0f, TOTAL_POS.y + 60.0f, 0.0f))	// �����x�̈ʒu
#define SATYIFACTION_SIZE		(D3DXVECTOR2(15.0f / 4, 25.0f))									// �����x�̃T�C�Y
#define SCORE_POS				(D3DXVECTOR3(250.0f + 800.0f, TOTAL_POS.y + 60.0f, 0.0f))	// ���݂̍ō��X�R�A�̈ʒu
#define MONEY_POS				(D3DXVECTOR3(250.0f + 530.0f, TOTAL_POS.y + 60.0f, 0.0f))	// \�}�[�N�̈ʒu
#define MONEY_SIZE				(D3DXVECTOR2(25.0f, 23.0f))										// \�}�[�N�̃T�C�Y
#define CIRCLE_POS				(D3DXVECTOR3(SCREEN_WIDTH / 2, 520.0f, 0.0f))					// �T�[�N���̈ʒu
#define CIRCLE_SIZE				(D3DXVECTOR2(100.0f, 100.0f))									// �T�[�N���̃T�C�Y
#define FREAM_POS				(D3DXVECTOR3(QUIT_POS.x, QUIT_POS.y, 0.0f))						// �t���[���̈ʒu
#define FREAM_SIZE000			(D3DXVECTOR2(GAME_SIZE.x + 25.0f , GAME_SIZE.y + 20.0f))		// �t���[���̃T�C�Y(�Q�[�����)
#define FREAM_SIZE001			(D3DXVECTOR2(QUIT_SIZE.x + 20.0f , QUIT_SIZE.y + 18.0f))		// �t���[���̃T�C�Y(Quit)

//-------------------------------------------
// �t�@�C����
//-------------------------------------------
#define TOTALSCORE_TXT_NAME		("data/TEXT/Ranking/ranking.bin")
#define TOTALSCORE_TXT_NAME2	("data/TEXT/Ranking/ranking001.bin")
#define SATISFACTION_TXT_NAME	("data/TEXT/Ranking/SatisfactionLevel.bin")
#define SATISFACTION_TXT_NAME2	("data/TEXT/Ranking/SatisfactionLevel001.bin")

//-------------------------------------------
// ���̑�
//-------------------------------------------
#define ROT_MOVE				(0.5f)				// ��]���x
#define ANIM_COUNT				(120)				// �A�j���[�V�����J�E���^�[
#define PATTERN_NUM				(4)					// �p�^�[���̐�
#define MENU_POS				(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100.0f + (nCntMenu * 300.0f), 0.0f))	// continu �` Quit�܂ňʒu
#define DEFAULT_POS_LEFT		(D3DXVECTOR3(0.0f, MENU_POS.y, 0.0f))						// �f�t�H���g�̈ʒu(��)�̏ꍇ
#define DEFAULT_POS_RIGHT		(D3DXVECTOR3(SCREEN_WIDTH, MENU_POS.y, 0.0f))				// �f�t�H���g�̈ʒu(�E)�̏ꍇ
#define END_POS_LEFT			(D3DXVECTOR3(-200.0f, MENU_POS.y, 0.0f))					// �I����̈ʒu(��)�̏ꍇ
#define END_POS_RIGHT			(D3DXVECTOR3(SCREEN_WIDTH + 200.0f, MENU_POS.y, 0.0f))		// �I����̈ʒu(�E)�̏ꍇ
#define MENU_SIZE				(D3DXVECTOR2(550.0f, 150.0f))								// continu �` Quit�܂ŃT�C�Y
#define MOVE					(27.0f)														// ���E�̈ړ���

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
int CSelect::m_nStageNumber = 0;

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CSelect::CSelect()
{
	m_pFream = NULL;
	m_pBg = NULL;
	m_bPress = false;
	m_bSame = false;
	m_fAddRot = D3DX_PI;
	m_nCntAnim = 0;
	m_nCntPattern = 0;
	m_nStageNum = 1;
	m_nStageNumber = 0;
	m_bEnter = false;
	m_bSelectFlag = false;
	m_bLevelFlag = false;
	m_bMove = false;
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSelect::~CSelect()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CSelect::Init()
{


	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CSelect::Uninit(void)
{

}
//=============================================================================
// �X�V����
//=============================================================================
void CSelect::Update(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputXPad * pXpad = CManager::GetXInput();					//�W���C�p�b�g�̎擾
	CInputMouse *pCInputMouse = CManager::GetInputMouse();

	
}
//=============================================================================
// �`�揈��
//=============================================================================
void CSelect::Draw(void)
{

}

//=============================================================================
// �X�e�[�W�I��
//=============================================================================
void CSelect::SlectStage(void)
{
	//�T�E���h�̏��
	CSound *pSound = CManager::GetSound();

	
}