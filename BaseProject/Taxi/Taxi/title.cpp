//=============================================================================
//
// �^�C�g���̏��� [title.cpp]
// Author : shun yokomichi
//
//=============================================================================
#include "title.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "background.h"
#include "texture.h"
#include "logo.h"
#include "ranking.h"
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
	srand((unsigned int)time(0));		// �����_���̂��
	//===================================
	//		 Load�̓ǂݍ��ݏꏊ
	//===================================
	//�t�F�[�h�̃e�N�X�`���̓ǂݍ���
	CFade::Load();

	CBackGround::Create(CTexture::TYPE_BACKGROUND000);

	// �^�C�g�����S�̐���
	CLogo::Create(TITLE_POS, TITLE_SIZE, CTexture::TYPE_TITLELOGO000, 0, CLogo::TYPE_LOGO);
	CLogo::Create(LOGO_POS, LOGO_SIZE, CTexture::TYPE_TITLELOGO001, 0, CLogo::TYPE_LOGO);

	// Press Start Enter�̐���
	CLogo::Create(PRESS_POS, PRESS_SIZE, CTexture::TYPE_TITLE_PRESS, 0, CLogo::TYPE_LOGO);

	//	�ϐ��̏�����
	m_nTitleCounter = 0;

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
	//�t�F�[�h�̃e�N�X�`���̔j��
	CFade::UnLoad();

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
	CXInput *pCInputJoypad = CManager::GetXInput();
	CInputMouse *pCInputMouse = CManager::GetInputMouse();

	//�T�E���h�̏��
	CSound *pSound = CManager::GetSound();

	//�t�F�[�h�̃|�C���g
	CFade::FADE pFade = CFade::GetFade();


	//	�^�C�g���̃J�E���^�[���Z
	m_nTitleCounter++;

	/*if (m_nTitleCounter > 480)
	{
		if (pFade == CFade::FADE_NONE)
		{
			CRanking::Reset();
			//�|�[�Y�̑I���̌��艹
			pSound->PlaySound(CSound::SOUND_LABEL_SE_TITLE_ENTER);
			CFade::Create(CManager::MODE_RANKING);
		}
	}*/

	if (pCInputJoypad->GetAnyButton() == true || pCInputKeyBoard->GetKeyboardAny(1) == true || pCInputMouse->GetMouseTrigger(0) == true)
	{//�^�C�g������Q�[����
	 //�t�F�[�h���n�܂�����
		if (pFade == CFade::FADE_NONE)
		{
			//�|�[�Y�̑I���̌��艹
			pSound->PlaySound(CSound::SOUND_LABEL_SE_TITLE_ENTER);
			CFade::Create(CManager::MODE_GAME);
		}
	}

	//SeasoneEffectUpdata();	// �G�߃G�t�F�N�g�̍X�V
}
//=============================================================================
// �`�揈��
//=============================================================================
void CTitle::Draw(void)
{

}