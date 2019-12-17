//=============================================================================
//
// �t�F�[�h�̏��� [fade.cpp]
// Author : �L�n���u
//
//=============================================================================
#include "scene.h"
#include "scene2D.h"
#include "fade.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_NAME_1					"data\\TEXTURE\\.png"		//�ǂݍ��ރe�N�X�`���t�@�C��
#define FADE_TIME						(0.02f)						//�t�F�[�h�̎���

//���[�h�摜
#define LOAD_POS	(D3DXVECTOR3(1150.0f, 680.0f, 0.0f))
#define LOAD_SIZE	(D3DXVECTOR2(100.0f, 25.0f))

//�A�j���[�V�����摜
#define ANIM_POS	(D3DXVECTOR3(1025.0f, 665.0f, 0.0f))
#define ANIM_SIZE	(D3DXVECTOR2(30.0f, 30.0f))
#define ANIM_UV		(0.2f)
#define MAX_PATTERN	(5)
#define ANIM_COUNT	(7)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 CFade::m_pTexture[MAX_TEXTURE] = {};
CFade::FADE CFade::m_fade = CFade::FADE_NONE;
int CFade::m_nModeNext = CManager::MODE_TITLE;
CFade::MODEFADE CFade::m_modegame = CFade::MODEFADE_NONE;
CGame *CFade::m_pGame = NULL;
//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CFade::CFade() : CScene2D(7, CScene::OBJTYPE_FADE)//�D��D�ʃC�j�V�����C�Y
{
	// �l�̏�����
	m_fade = FADE_NONE;									//�t�F�[�h�C�����
	m_colorFade = D3DXCOLOR(0.0f,0.0f, 0.0f, 0.0f);		//�����s�����ɂ��Ă���
	m_Size = D3DXVECTOR2(0.0f, 0.0f);
	m_pNowLoading = NULL;
	m_pAnim = NULL;
	m_nAnimNum = 0;
	m_nCountAnim = 0;
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CFade::~CFade(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CFade::Init()
{	
	CScene2D::Init();

	//�ʒu�̐錾
	D3DXVECTOR3 VtxPos[4];

	//�J���[�̐錾�Ə�����
	D3DXCOLOR Color[4];
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		Color[nCnt] = m_colorFade;
	}

	//�e�N�X�`���ݒ� �c�X�N���[���p
	VtxPos[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	VtxPos[1] = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	VtxPos[2] = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	VtxPos[3] = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//�t�F�[�h�̏�Ԃ�������
	m_modegame = MODEFADE_NONE;

	//�I�u�W�F�N�g�̎�ނ̐ݒ�
	CScene::SetObjType(CScene::OBJTYPE_FADE);

	//�J���[�̊��蓖��
	CScene2D::SetColor(&Color[0]);

	//�ʒu�̐ݒ�
	CScene2D::SetVtxPos(&VtxPos[0]);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CFade::Uninit(void)
{
	CScene2D::Uninit();

	//���[�h�摜
	if (m_pNowLoading != NULL)
	{
		m_pNowLoading->Uninit();
		m_pNowLoading = NULL;
	}

	//�A�j���[�V�����摜
	if (m_pAnim != NULL)
	{
		m_pAnim->Uninit();
		m_pAnim = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CFade::Update(void)
{
	//�J���[�̐錾�Ə�����
	D3DXCOLOR Color[4];
	bool bDelete = false;

	if (m_fade != FADE_NONE)
	{
		if (m_fade == FADE_IN)				//�t�F�[�h�C��
		{
			m_colorFade.a -= FADE_TIME;		//��ʂ𓧖��ɂ��Ă���
			if (m_colorFade.a <= 0.0f)
			{
				m_colorFade.a = 0.0f;

				m_fade = FADE_NONE;			//�������Ă��Ȃ�
				bDelete = true;
			}
		}
		else if (m_fade == FADE_OUT)
		{
			m_colorFade.a += FADE_TIME;		//��ʂ�s�����ɂ��Ă���
			if (m_colorFade.a >= 1.0f)
			{
				m_colorFade.a = 1.0f;

				m_fade = FADE_IN;			//�t�F�[�h�C��

				if (m_modegame == MODEFADE_MODE)
				{
					CManager::AgingCouneter();
					//���[�h�ݒ�
					CManager::SetMode((CManager::MODE)m_nModeNext);
				}
				else if (m_modegame == MODEFADE_GAME)
				{
					CGame::SetGameModeNext((CGame::GAMEMODE)m_nModeNext);
				}

				//UpdateAll��߂��I
				CScene::SetUpdatePri(0);
			}
		}

		//���_�J���[�̐ݒ�
		Color[0] = m_colorFade;
		Color[1] = m_colorFade;
		Color[2] = m_colorFade;
		Color[3] = m_colorFade;

		//�J���[�̊��蓖��
		CScene2D::SetColor(&Color[0]);
	}

	//���[�h�摜�̐F��������
	if (m_pNowLoading != NULL) { m_pNowLoading->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, m_colorFade.a)); }

	//�A�j���[�V�����摜�̍X�V
	if (m_pAnim != NULL)
	{
		//�F�̐ݒ�
		m_pAnim->SetColor(&D3DXCOLOR(1.0f, 1.0f, 1.0f, m_colorFade.a));

		//���Ԃ̉��Z
		m_nCountAnim++;

		if ((m_nCountAnim % ANIM_COUNT) == 0)
		{
			//�A�j���[�V�������̉Z
			m_nAnimNum++;

			//�A�j���[�V��������������
			if ((m_nAnimNum % MAX_PATTERN) == 0) { m_nAnimNum = 0; }

			//�A�j���[�V�����ݒ�
			m_pAnim->SetUVWidth(ANIM_UV, m_nAnimNum);
		}
	}

	if (bDelete == true)
	{//�t�F�[�h�̏I������
		Uninit();
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void CFade::Draw(void)
{
	CScene2D::Draw();
}
//===============================================================================
//�@�N���G�C�g
//===============================================================================
CFade * CFade::Create(CManager::MODE modeNext)
{
	CFade *pFade = NULL;

	if (m_fade == FADE_NONE)
	{
		//NULL�`�F�b�N
		if (pFade == NULL)
		{//�������̓��I�m��

			pFade = new CFade;

			if (pFade != NULL)
			{
				//�I�u�W�F�N�g�N���X�̐���
				pFade->Init();

				m_nModeNext = modeNext;

				//�t�F�[�h�̃��[�h
				m_modegame = MODEFADE_MODE;

				m_fade = FADE_OUT;
			}
		}
	}

	return pFade;
}
CFade * CFade::Create(CGame::GAMEMODE modeNext)
{
	CFade *pFade = NULL;

	if (m_fade == FADE_NONE)
	{
		//NULL�`�F�b�N
		if (pFade == NULL)
		{//�������̓��I�m��

			pFade = new CFade;

			if (pFade != NULL)
			{
				//�I�u�W�F�N�g�N���X�̐���
				pFade->Init();

				m_nModeNext = modeNext;

				//�t�F�[�h�̃��[�h
				m_modegame = MODEFADE_GAME;

				m_fade = FADE_OUT;
			}
		}
	}

	return pFade;
}

//===============================================================================
// �e�N�X�`���̓ǂݍ���
//===============================================================================
HRESULT CFade::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NAME_1,
		&m_pTexture[0]
	);

	return S_OK;
}
//===============================================================================
// �e�N�X�`���̔j��
//===============================================================================
void CFade::UnLoad(void)
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