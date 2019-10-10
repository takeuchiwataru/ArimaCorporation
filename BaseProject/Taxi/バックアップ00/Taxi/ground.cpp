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
#define GROUND_TEX_1		"data\\TEXTURE\\�C.png"	// �t�@�C���̖��O
#define GROUND_MOVE_SPEED	(0.002f)				// �e�N�X�`���̈ړ����x�̒����p

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
	m_bSound = false;
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

	//��ނ̐ݒ�
	SetTypeNumber(TYPE_GROUND);
	m_fTexMoveSpeed = GROUND_MOVE_SPEED;

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
	//�`���Ԃ̎擾
	bool bDraw = CScene::GetDraw();

	//�`���Ԃł͂Ȃ��Ȃ�X�V���Ȃ�
	if (!bDraw) { return; }

	CScene3D::Update();

	//�ϐ�
	D3DXVECTOR2 UV[4];

	m_nCnt++;

	if (m_rot.x != 0.0f)
	{
		UV[0] = D3DXVECTOR2(0.0f - (m_nCnt * 0.025f), 1.0f);
		UV[1] = D3DXVECTOR2(0.0f - (m_nCnt * 0.025f), 0.0f);
		UV[2] = D3DXVECTOR2(1.0f - (m_nCnt * 0.025f), 1.0f);
		UV[3] = D3DXVECTOR2(1.0f - (m_nCnt * 0.025f), 0.0f);

		CScene3D::SetNor(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	}
	else if (m_rot.z != 0.0f)
	{
		UV[0] = D3DXVECTOR2(0.0f + (m_nCnt * 0.025f), 0.0f);
		UV[1] = D3DXVECTOR2(1.0f + (m_nCnt * 0.025f), 0.0f);
		UV[2] = D3DXVECTOR2(0.0f + (m_nCnt * 0.025f), 1.0f);
		UV[3] = D3DXVECTOR2(1.0f + (m_nCnt * 0.025f), 1.0f);
	}
	else
	{
		if (m_move.x != 0.0f)
		{// �e�N�X�`���ݒ�@���E�ɗ����
			UV[0] = D3DXVECTOR2(0.0f - (m_nCnt * (m_move.x * m_fTexMoveSpeed)), 0.0f);
			UV[1] = D3DXVECTOR2(5.0f - (m_nCnt * (m_move.x * m_fTexMoveSpeed)), 0.0f);
			UV[2] = D3DXVECTOR2(0.0f - (m_nCnt * (m_move.x * m_fTexMoveSpeed)), 1.0f);
			UV[3] = D3DXVECTOR2(5.0f - (m_nCnt * (m_move.x * m_fTexMoveSpeed)), 1.0f);
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
	}

	//�w�i��UV
	CScene3D::SetUV(&UV[0]);
}

//=============================================================================
// �`�揈��
//=============================================================================
void CGround::Draw(void)
{
	//CScene3D::Draw();

	CScene3D::Draw();
}
//===============================================================================
//�@�N���G�C�g
//===============================================================================
CGround * CGround::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXVECTOR2 size, int nType)
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
			//��ނ̐ݒ�
			pGround->m_nType = nType;
			pGround->SetTexType(nType);
			//�ʒu�Ƒ傫���̐ݒ�
			pGround->SetGroundPosSize(pos, size);
			pGround->m_pos = pos;
			// �ړ��ʂ̐ݒ�
			pGround->SetMove(move);
			pGround->m_move = move;
			// �p�x�̐ݒ�
			pGround->SetRotation(rot * D3DX_PI);
			pGround->m_rot = rot;
			//�e�N�X�`���̐ݒ�
			pGround->BindTexture(m_pTexture[nType]);
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

//=============================================================================
// ��̗���ɂ��ړ��ʂ̕ω�
//=============================================================================
D3DXVECTOR3 CGround::RiverMove(void)
{
	return m_move * 0.5f;
}

//=============================================================================
// ��̉��̍Đ�
//=============================================================================
void CGround::Sound(bool bPlay)
{
	if (bPlay)
	{// �����Đ�����
		if (!m_bSound)
		{// �����Đ����Ă��Ȃ��Ƃ�
			CSound *pSound = CManager::GetSound();
			pSound->PlaySound(CSound::SOUND_LABEL_SE_RIVER);
			m_bSound = true;
		}
	}
	else
	{// ��������
		if (m_bSound)
		{// �����Đ����Ă���Ƃ�
			CManager::GetSound()->StopSound(CSound::SOUND_LABEL_SE_RIVER);
			m_bSound = false;
		}
	}
}