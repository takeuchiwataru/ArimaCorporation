//=============================================================================
//
// �G�߃G�t�F�N�g�̏��� [seasoneffect.cpp]
// Author : shun yokomichi
//
//=============================================================================
#include "scene.h"
#include "seasoneffect.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"
#include "texture.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define VECTOR3_ZERO		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))		// D3DXVECTOR3�̏����l
#define VECTOR2_ZERO		(D3DXVECTOR2(0.0f, 0.0f))			// D3DXVECTOR2�̏����l
#define MOVE				(D3DXVECTOR3(4.0f, 2.5f, 0.0f))		// �ړ���
#define MOVE_CHANGE_TAIME	(70)								// �ړ��ʂ�ς��鎞��
#define ADD_ROT_X			(0.05f)								// ��]��(X��)
#define ADD_ROT_Y			(0.1f)								// ��]��(X��)
#define PRIORTYV_4			(4)									// �D�揇��(4)
#define PRIORTYV_6			(6)									// �D�揇��(6)
#define TEX_ANIM_X			(4)									// �e�N�X�`���A�j���[�V����X��
#define TEX_ANIM_Y			(1)									// �e�N�X�`���A�j���[�V����Y��
#define SLOWING_DOWN		(0.02f)								// ����
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
int CSeasonEffect::m_nCntEffect = 0;
//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CSeasonEffect::CSeasonEffect(int nPriorityv) : CScene2D(nPriorityv, CScene::OBJTYPE_EFFECT)
{
	m_pVtxBuff		= NULL;
	m_pos			= VECTOR3_ZERO;
	m_posOld		= VECTOR3_ZERO;
	m_rot			= VECTOR3_ZERO;
	m_size			= VECTOR2_ZERO;
	m_SeasonType	= SEASONTYPE_NONE;
	m_fRadius		= 0.0f;
	m_fAngle		= 0.0f;
	m_fLength		= 0.0f;
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CSeasonEffect::~CSeasonEffect()
{

}
//=============================================================================
// ����������
//=============================================================================
HRESULT CSeasonEffect::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	CScene2D::Init();
	// �e�N�X�`���̐ݒ�
	CScene2D::SetTexture((int)m_SeasonType - 1, TEX_ANIM_X, TEX_ANIM_Y, 1);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CSeasonEffect::Uninit(void)
{
	CScene2D::Uninit();
}
//=============================================================================
// �X�V����
//=============================================================================
void CSeasonEffect::Update(void)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();

	CScene2D::Update();

	//�ʒu�̒l��e������炤
	D3DXVECTOR3 pos = CScene2D::GetPosition();
	// �T�C�Y�̒l��e������炤
	D3DXVECTOR2 size = CScene2D::GetSize();
	//�ړ��ʂ̒l��e������炤
	D3DXVECTOR3 move = CScene2D::GetMove();

	m_fLength = sqrtf((size.x + size.x) * (size.y + size.y));
	// �p�x���o��
	m_fAngle = atan2f(size.x / 2, size.y / 2);
	m_rot.x += ADD_ROT_X;
	m_rot.y = ADD_ROT_Y;

	m_nCountTime++;
	if ((m_nCountTime % MOVE_CHANGE_TAIME) == 0) { move.x *= -1; }

	move.x += (0.0f - move.x) * SLOWING_DOWN;

	//�ړ��l���
	pos.x += sinf(move.x);
	pos.y += move.y;

	//�ʒu����0����Ȃ��ꍇ�̂݃Z�b�g����
	if (pos.y <= SCREEN_HEIGHT)
	{//���a��0�ȉ��̏ꍇ
		// �e�N�X�`���̐ݒ�
		CScene2D::SetTexture((int)m_SeasonType - 1, TEX_ANIM_X, TEX_ANIM_Y, 1);
		// ��]����
		CScene2D::SetSpin(pos, m_fAngle, m_fLength, m_rot);
		//�G�t�F�N�g�̈ړ���
		CScene2D::SetMove(move);
	}
	else { Uninit(); }

}
//=============================================================================
// �`�揈��
//=============================================================================
void CSeasonEffect::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// �J�����O���Ȃ�

	CScene2D::Draw();

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ���ʂ��J�����O(���ɖ߂�)
}
//===============================================================================
//�@�N���G�C�g
//===============================================================================
CSeasonEffect * CSeasonEffect::Create(int Type,D3DXVECTOR3 pos, int nRadius,int nTexType)
{
	CSeasonEffect *pSeasonEffect = NULL;

	//NULL�`�F�b�N
	if (pSeasonEffect == NULL)
	{//�������̓��I�m��
		// �v���C�x�[�g�ݒ�
		int nPriorityv = PRIORTYV_4;
		if (nRadius >= RADIUS_CONDIUIONS) { nPriorityv = PRIORTYV_6; }
		pSeasonEffect = new CSeasonEffect(nPriorityv);

		if (pSeasonEffect != NULL)
		{
			//�G�t�F�N�g�̎��
			pSeasonEffect->m_SeasonType = (SEASONTYPE)Type;
			//�I�u�W�F�N�g�N���X�̐���
			pSeasonEffect->Init();
			//���a�̐ݒ�
			pSeasonEffect->m_fRadius = (float)nRadius;
			//�e�N�X�`���̊��蓖��
			pSeasonEffect->BindTexture(*CTexture::GetTexture(nTexType));
			//�ʒu�̊��蓖��
			pSeasonEffect->SetPosSize(pos, D3DXVECTOR2(pSeasonEffect->m_fRadius, pSeasonEffect->m_fRadius));
			//�ړ��ʂ̊��蓖��
			pSeasonEffect->SetMove(MOVE);
		}
	}
	return pSeasonEffect;
}