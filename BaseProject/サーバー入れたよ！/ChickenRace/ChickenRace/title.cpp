//=============================================================================
//
// �^�C�g���̏��� [title.cpp]
// Author : �L�n ���u
//
//=============================================================================
#include "title.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "ranking.h"
#include "ui.h"
#include "titlecamera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define EFFECT_FRAME	(300)		// �G�߂̃G�t�F�N�g�t���[��
#define SEASON_MAX		(4)			// �G�߂̃J�E���g
#define EFFECT_CREATE	(10)		// �G�t�F�N�g��������
#define EFFECT_ROS		(D3DXVECTOR3(rand() % 1200 + 10.0f, -20.0f, 0.0f))	// �ʒu(�����_��)
#define EFFECT_SIZE		(rand() % RADIUS_CONDIUIONS + 10)					// �T�C�Y(�����_��)
#define TITLE_POS		(D3DXVECTOR3(SCREEN_WIDTH /2, SCREEN_HEIGHT / 2 - 80.0f, 0.0f))	// �ʒu(�^�C�g�����S000)
#define TITLE_SIZE		(D3DXVECTOR2(480.0f, 100.0f))										// �T�C�Y(�^�C�g�����S000)
#define LOGO_POS		(D3DXVECTOR3(TITLE_POS.x + 130.0f, TITLE_POS.y + 100.0f, 0.0f))		// �ʒu(�^�C�g�����S001)
#define LOGO_SIZE		(D3DXVECTOR2(125.0f, 35.0f))										// �T�C�Y(�^�C�g�����S001)
#define PRESS_POS		(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT/ 2) + 200.0f, 0.0f))// �ʒu(Press Start Enter)
#define PRESS_SIZE		(D3DXVECTOR2(250.0f, 20.0f))										// �T�C�Y(Press Start Enter)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CTitleCamera *CTitle::m_pTitleCamera = NULL;

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CTitle::CTitle()
{
	m_nCntTex = 0;
	m_nFrame = 0;
	m_nLeafposX = 0;
	m_nLeafsize = 0;
	m_nCntCreate = 0;
	m_bOnOff = false;
	m_nTitleCounter = 0;
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTitle::~CTitle()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CTitle::Init()
{
	//===================================
	//		 Load�̓ǂݍ��ݏꏊ
	//===================================
	CFade::Load();		//�t�F�[�h�̃e�N�X�`���̓ǂݍ���
	CUi::Load();		//UI�̃e�N�X�`���̓ǂݍ���

	//===================================
	//		    UI�����̏ꏊ
	//===================================
	CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 120.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f), CUi::UI_TITLE_LOGO);	//�^�C�g�����S
	CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 270.0f, 0.0f), D3DXVECTOR2(250.0f, 50.0f), CUi::UI_PRESS_BUTTON);	//�G���^�[�w��

	//	�ϐ��̏�����
	m_nTitleCounter = 0;

	if (m_pTitleCamera == NULL)
	{
		m_pTitleCamera = new CTitleCamera;
		if (m_pTitleCamera != NULL) { m_pTitleCamera->Init(); }
	}

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CTitle::Uninit(void)
{
	//===================================
	//	�@�@UnLoad�̔j������ꏊ
	//===================================
	CFade::UnLoad();	//�t�F�[�h�̃e�N�X�`���̔j��
	CUi::UnLoad();		//UI�̃e�N�X�`���̔j��

	// �^�C�g���J�����̔j��
	if (m_pTitleCamera != NULL)
	{
		m_pTitleCamera->Uninit();
		delete m_pTitleCamera;
		m_pTitleCamera = NULL;
	}

	//�t�F�[�h�ȊO�폜
	CScene::NotFadeReleseAll();
}
//=============================================================================
// �X�V����
//=============================================================================
void CTitle::Update(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputJoyPad_0 * pXpad = CManager::GetInputJoyPad0(0);					//�W���C�p�b�g�̎擾
	CInputMouse *pCInputMouse = CManager::GetInputMouse();

	//�T�E���h�̏��
	CSound *pSound = CManager::GetSound();

	//�t�F�[�h�̃|�C���g
	CFade::FADE pFade = CFade::GetFade();

	//	�^�C�g���̃J�E���^�[���Z
	m_nTitleCounter++;

	if (pXpad->GetAllTrigger() == true || pCInputKeyBoard->GetKeyboardAny(1) == true || pCInputMouse->GetMouseTrigger(0) == true)
	{//�^�C�g������Q�[����
		//�t�F�[�h���n�܂�����
		if (pFade == CFade::FADE_NONE)
		{
			//�^�C�g���̑I���̉���
			pSound->SetVolume(CSound::SOUND_LABEL_SE_TITLEFADE, 0.5f);
			//�^�C�g���̑I���̌��艹
			pSound->PlaySound(CSound::SOUND_LABEL_SE_TITLEFADE);

			CFade::Create(CManager::MODE_GAME);
		}
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void CTitle::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	pDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
		D3DCOLOR_RGBA(157, 184, 224, 255),
		1.0f,
		0);

	//�^�C�g���J�����̐���
	if (m_pTitleCamera != NULL) { m_pTitleCamera->SetCamera(); }
	
	//�S�Ă̕`��
	CScene::DrawAll();
}