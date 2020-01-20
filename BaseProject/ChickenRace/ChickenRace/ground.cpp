//=============================================================================
//
// ��̏��� [ground.cpp]
// Author : �L�n�@���u
//
//=============================================================================
#include "ground.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "mesh.h"
#include "scene3D.h"
#include "game.h"
#include "player.h"
#include "sound.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define GROUND_TEX_1		"data\\TEXTURE\\mesh\\Water03.jpg"				// �t�@�C���̖��O
#define GROUND_TEX_2		"data\\TEXTURE\\mesh\\Water_Highlight.png"		// �t�@�C���̖��O
#define GROUND_MOVE_SPEED	(0.0008f)										// �e�N�X�`���̈ړ����x�̒����p

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 CGround::m_pTexture[MAX_GROUND_TEX] = {};
LPDIRECT3DVERTEXBUFFER9	CGround::m_pVtxBuff = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CGround::CGround() : CScene3D(GROUND_PRIOTITY, CScene::OBJTYPE_GROUND)
{
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_fTexMoveSpeed = 0.0f;
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGround::~CGround()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CGround::Init()
{
	CScene3D::Init();
	//GetDrawType() = C2D::DRAW_TYPE_ADD;
	//��ނ̐ݒ�
	SetTypeNumber(TYPE_GROUND);
	m_fTexMoveSpeed = GROUND_MOVE_SPEED;
	m_nCnt = 0;
	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CGround::Uninit(void)
{
	for (int nCount = 0; nCount < MAX_GROUND_TEX; nCount++)
	{
		// �e�N�X�`���̔j��
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}

	CScene3D::Uninit();
}
//=============================================================================
// �X�V����
//=============================================================================
void CGround::Update(void)
{
	CScene3D::Update();

	//�ϐ�
	D3DXVECTOR2 UV[4];

	m_nCnt++;

	if (m_move.x != 0.0f)
	{// �e�N�X�`���ݒ�@���E�ɗ����
		UV[0] = D3DXVECTOR2(0.0f - (m_nCnt * (m_move.x * m_fTexMoveSpeed)), 0.0f);
		UV[1] = D3DXVECTOR2(1.0f - (m_nCnt * (m_move.x * m_fTexMoveSpeed)), 0.0f);
		UV[2] = D3DXVECTOR2(0.0f - (m_nCnt * (m_move.x * m_fTexMoveSpeed)), 1.0f);
		UV[3] = D3DXVECTOR2(1.0f - (m_nCnt * (m_move.x * m_fTexMoveSpeed)), 1.0f);
	}
	if (m_move.z != 0.0f)
	{// �e�N�X�`���ݒ� �㉺�ɗ����
		UV[0] = D3DXVECTOR2(0.0f + (m_nCnt * (m_move.z * m_fTexMoveSpeed)), 1.0f);
		UV[1] = D3DXVECTOR2(0.0f + (m_nCnt * (m_move.z * m_fTexMoveSpeed)), 0.0f);
		UV[2] = D3DXVECTOR2(1.0f + (m_nCnt * (m_move.z * m_fTexMoveSpeed)), 1.0f);
		UV[3] = D3DXVECTOR2(1.0f + (m_nCnt * (m_move.z * m_fTexMoveSpeed)), 0.0f);
	}
	if (m_move.z == 0.0f && m_move.x == 0.0f)
	{// �e�N�X�`���ݒ� �����Ȃ�
		UV[0] = D3DXVECTOR2(0.0f, 0.0f);
		UV[1] = D3DXVECTOR2(1.0f, 0.0f);
		UV[2] = D3DXVECTOR2(0.0f, 1.0f);
		UV[3] = D3DXVECTOR2(1.0f, 1.0f);
	}

	//�w�i��UV
	CScene3D::SetUV(&UV[0]);
}

//=============================================================================
// �`�揈��
//=============================================================================
void CGround::Draw(void)
{
	CScene3D::Draw();
}
//===============================================================================
//�@�N���G�C�g
//===============================================================================
CGround * CGround::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR2 size, TYPE Type)
{
	CGround *pGround = NULL;

	//NULL�`�F�b�N
	if (pGround == NULL)
	{//�������̓��I�m��

		pGround = new CGround;

		if (pGround != NULL)
		{
			//����������
			pGround->Init();
			//�����蔻��p
			pGround->m_size = size;
			//���
			pGround->m_Type = Type;
			//��ނ̐ݒ�
			pGround->SetTexType(Type);
			//�ʒu�Ƒ傫���̐ݒ�
			pGround->m_pos = pos;
			pGround->SetGroundPosSize(pos, size);
			// �ړ��ʂ̐ݒ�
			pGround->m_move = move;
			//�e�N�X�`���̐ݒ�
			pGround->BindTexture(m_pTexture[Type]);
		}
	}

	return pGround;
}
//=============================================================================
// �ǂݍ��ݏ���
//=============================================================================
HRESULT CGround::Load(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`������
	D3DXCreateTextureFromFile(pDevice, GROUND_TEX_1, &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, GROUND_TEX_2, &m_pTexture[1]);

	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CGround::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_GROUND_TEX; nCount++)
	{
		// �e�N�X�`���j��
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}