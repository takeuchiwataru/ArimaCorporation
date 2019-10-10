//=============================================================================
//
// �R�C���ǉ��̏��� [addcoin.cpp]
// Author : �L�n���u
//
//=============================================================================
#include "addcoin.h"
#include "scene2D.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "billboord.h"
#include "game.h"
#include "player.h"
#include "arrow.h"
#include "texture.h"
#include "tutorial.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define VECTORE_ZERO		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
#define LIFE				(35)
#define SPEED				(10.0f)
#define POS					(D3DXVECTOR3(700.0f, 400.0f, 0.0f))
#define AIM_POS				(D3DXVECTOR3(1000.0f, 200.0f, 0.0f))
#define MAX_PATTERN			(4)	
#define UV					(0.25f)
#define LOAD_TEX			("data\\TEXTURE\\game\\Score\\Coin.png")
#define SIZE				(D3DXVECTOR2(25.0f, 25.0f))

//*****************************************************************************
// �O���錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CAddCoin::m_pTexture = NULL;

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CAddCoin::CAddCoin(int nPriorityv) : CScene2D(nPriorityv, CScene::OBJTYPE_LOGO){}

//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CAddCoin::~CAddCoin(){}

//===============================================================================
//�@�N���G�C�g
//===============================================================================
CAddCoin * CAddCoin::Create(void)
{
	CAddCoin *pLogo = NULL;

	//NULL�`�F�b�N
	if (pLogo == NULL)
	{//�������̓��I�m��
		pLogo = new CAddCoin(6);

		if (pLogo != NULL)
		{
			pLogo->Init();
		}
	}

	return pLogo;
}

//===============================================================================
//�@�e�N�X�`���ǂݍ��ݏ���
//===============================================================================
void CAddCoin::LoadTex(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, LOAD_TEX, &m_pTexture);
}

//===============================================================================
//�@�e�N�X�`���̔j������
//===============================================================================
void CAddCoin::UnloadTex(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CAddCoin::Init(void)
{
	//�ϐ��̏�����
	m_nPattern = 0;
	m_nCountTime = 0;
	m_nLife = LIFE;

	//�ʒu�ݒ�
	float fRand = (float)((rand() % 50) + 10);
	D3DXVECTOR3 pos = D3DXVECTOR3(POS.x + fRand, POS.y + fRand, 0.0f);

	CScene2D::Init();
	CScene2D::BindTexture(m_pTexture);		//�e�N�X�`���̊��蓖��
	CScene2D::SetPosSize(pos, SIZE);
	CScene2D::SetUVWidth(UV, m_nPattern);

	//���������߂�
	D3DXVECTOR3 Distance = pos - AIM_POS;
	m_fAngle = atan2f(Distance.x, Distance.y);
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CAddCoin::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CAddCoin::Update(void)
{
	//�e�N�X�`����UV�X�V
	m_nCountTime++;

	if ((m_nCountTime % 6) == 0)
	{
		m_nPattern++;

		if ((m_nPattern % MAX_PATTERN) == 0) { m_nPattern = 0; }

		//UV�l�̐ݒ�
		CScene2D::SetUVWidth(UV, m_nPattern);
	}

	//�ʒu�̎擾
	D3DXVECTOR3 pos;
	pos = GetPosition();

	//�ړ��ʂ̍X�V
	m_move.x = sinf(m_fAngle) * -SPEED;
	m_move.y = cosf(m_fAngle) * -SPEED;

	//�ʒu�̍X�V
	pos += m_move;
	CScene2D::SetPosSize(pos, SIZE);

	CScene2D::Update();

	//�\�����Ԃ̍X�V
	m_nLife--;

	//�j������
	if (m_nLife < 0) { Uninit(); }
}
//=============================================================================
// �`�揈��
//=============================================================================
void CAddCoin::Draw(void)
{
	CScene2D::Draw();
}
