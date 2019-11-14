//=============================================================================
//
// UI�̏��� [ui.cpp]
// Author : �L�n���u
//
//=============================================================================
#include "scene.h"
#include "scene2D.h"
#include "ui.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define UI_TEXTURE_NAME_1		"data\\TEXTURE\\Title\\UI\\title.png"			//�ǂݍ��ރe�N�X�`���t�@�C��enter
#define UI_TEXTURE_NAME_2		"data\\TEXTURE\\Title\\UI\\PressButton.png"		//�ǂݍ��ރe�N�X�`���t�@�C��
#define TITLE_SIZE_X			(500.0f)										//�^�C�g���̉��̃T�C�Y
#define TITLE_SIZE_Y			(180.0f)										//�^�C�g���̏c�̃T�C�Y
#define TITLE_SIZE_PERCENTAGE	(50.0f)											//�g�傷�銄��
#define ALPHA_LIMIT				(0.1f)											//�����x�̍Œ���x
#define ALPHA_LOWER				(0.015f)										//�����x�̉����Ă����l

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CUi::m_pTexture[MAX_TEXTURE] = {};
int CUi::m_nNextType = 0;
int CUi::m_nNextFont = 0;

//UI�̏���������
CUi::UI CUi::m_Ui = {};

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CUi::CUi() : CScene2D(5, CScene::OBJTYPE_LOGO)//�D��D�ʃC�j�V�����C�Y
{
	m_nType = 0;
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_fLength = 0.0f;
	m_fAngle = 0.0f;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntTutorial = 0;
	m_bOneCnt = false;
	m_nStateFont = 0;
	m_nStateType = 0;
	m_fAlpha = 1.0f;
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CUi::~CUi() { }

//=============================================================================
// ����������
//=============================================================================
HRESULT CUi::Init(void)
{
	CScene2D::Init();

	D3DXCOLOR Color[4];

	for (int nCount = 0; nCount < 4; nCount++)
	{//����ȊO�̏ꍇ���ׂĂ̏�����
		Color[nCount] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	CScene2D::SetColor(&Color[0]);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CUi::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CUi::Update(void)
{
	//UI�̓���
	UiMovement();
}
//=============================================================================
// �`�揈��
//=============================================================================
void CUi::Draw(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();
	CInputJoypad *pCInputJoyPad = CManager::GetInputJoyPad();

	CScene2D::Draw();
}
//===============================================================================
//�@�N���G�C�g
//===============================================================================
CUi * CUi::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, CUi::UI m_Ui)
{
	CUi *pLogo = NULL;

	//NULL�`�F�b�N
	if (pLogo == NULL)
	{//�������̓��I�m��

		pLogo = new CUi;

		if (pLogo != NULL)
		{
			//��ނ����蓖�Ă�
			pLogo->m_nType = m_Ui;
			//�I�u�W�F�N�g�N���X�̐���
			pLogo->Init();
			//�e�N�X�`�������蓖�Ă�
			pLogo->BindTexture(m_pTexture[m_Ui]);
			//�ʒu�����蓖�Ă�
			pLogo->SetPosition(pos);
			//�ʒu�����蓖�Ă�
			pLogo->m_pos = pos;
			//�T�C�Y�̊��蓖��
			pLogo->m_size = size;
		}
	}

	return pLogo;
}

//===============================================================================
// �e�N�X�`���̓ǂݍ���
//===============================================================================
HRESULT CUi::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, UI_TEXTURE_NAME_1, &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, UI_TEXTURE_NAME_2, &m_pTexture[1]);

	return S_OK;
}
//===============================================================================
// �e�N�X�`���̔j��
//===============================================================================
void CUi::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_TEXTURE; nCount++)
	{
		//�e�N�X�`���[�̔j��
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}
//===============================================================================
// UI�̓���
//===============================================================================
void CUi::UiMovement(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();

	//�ʒu�̊��蓖��
	D3DXVECTOR3 pos;
	pos = GetPosition();

	//�ړ��ʂ̊��蓖��
	D3DXVECTOR3 move;
	move = GetMove();

	//�J���[�̊��蓖�ĂƃJ���[�̏�����
	D3DXCOLOR Color[4];
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		Color[nCnt] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//===============================
	//       �e�e�N�X�`���̓���
	//===============================
	if (m_nType == UI_TITLE_LOGO)
	{
		//�g��̏���
		if (m_size.x <= TITLE_SIZE_X) { m_size.x += (TITLE_SIZE_X / TITLE_SIZE_PERCENTAGE); }
		if (m_size.y <= TITLE_SIZE_Y) { m_size.y += (TITLE_SIZE_Y / TITLE_SIZE_PERCENTAGE); }
	}
	else if (m_nType == UI_PRESS_BUTTON)
	{
		//�����x�̒l�����炵�Ă���
		m_fAlpha -= ALPHA_LOWER;

		//���̒l��؂����珉���l�ɖ߂�
		if (m_fAlpha < ALPHA_LIMIT) { m_fAlpha = 1.0f; }
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			Color[nCnt] = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha);
		}
	}

	//�ʒu���X�V(�ړ�)
	pos.x += move.x;
	pos.y += move.y;

	//UI�̐F
	CScene2D::SetColor(&Color[0]);
	//UI�̈ʒu�ƃT�C�Y
	CScene2D::SetPosSize(m_pos, m_size);
	//UI�̓���
	CScene2D::SetMove(move);
}