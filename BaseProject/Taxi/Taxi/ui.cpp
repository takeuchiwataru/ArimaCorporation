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
#define UI_TEXTURE_NAME_1		"data\\TEXTURE\\Title\\UI\\�^�C�g����.png"			//�ǂݍ��ރe�N�X�`���t�@�C��enter
#define UI_TEXTURE_NAME_2		"data\\TEXTURE\\Title\\UI\\PressButton.png"				//�ǂݍ��ރe�N�X�`���t�@�C��

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

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CUi::CUi() : CScene2D(5, CScene::OBJTYPE_LOGO)//�D��D�ʃC�j�V�����C�Y
{
	m_nType = 0;
	m_size = D3DXVECTOR2(0.0f,0.0f);
	m_fLength = 0.0f;
	m_fAngle = 0.0f;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntTutorial = 0;
	m_bOneCnt = false;
	m_nStateFont = 0;
	m_nStateType = 0;
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
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();

	//�ʒu�̊��蓖��
	D3DXVECTOR3 pos;
	pos = GetPosition();

	//�ړ��ʂ̊��蓖��
	D3DXVECTOR3 move;
	move = GetMove();



	//�ʒu���X�V(�ړ�)
	pos.x += move.x;
	pos.y += move.y;


	//���S�̈ʒu�ƃT�C�Y
	CScene2D::SetPosSize(m_pos, m_size);
	//���S�̈ʒu�ƃT�C�Y
	CScene2D::SetMove(move);
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
CUi * CUi::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,int nType)
{
	CUi *pLogo = NULL;

	//NULL�`�F�b�N
	if (pLogo == NULL)
	{//�������̓��I�m��

		pLogo = new CUi;

		if (pLogo != NULL)
		{
			//��ނ����蓖�Ă�
			pLogo->m_nType = nType;
			//�I�u�W�F�N�g�N���X�̐���
			pLogo->Init();
			//�e�N�X�`�������蓖�Ă�
			pLogo->BindTexture(m_pTexture[nType]);
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
	D3DXCreateTextureFromFile(pDevice, UI_TEXTURE_NAME_1,&m_pTexture[0]);
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