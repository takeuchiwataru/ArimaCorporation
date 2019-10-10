//=============================================================================
//
// �؂̃G�t�F�N�g���� [3Dparticle.cpp]
// Author : �������� Sato_Asumi
//
//=============================================================================
#include "woodeffect.h"
#include "player.h"
#include "game.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define SIZE	(D3DXVECTOR3(5.0f, 0.0f, 5.0f))
#define LIFE	(60 * 8)							//�\������
#define LENGTH	(3000.0f)							//����

//=============================================================================
// ��������
//=============================================================================
CWoodEffect * CWoodEffect::Create(D3DXVECTOR3 Pos)
{
	CWoodEffect * pWoodEffect = new CWoodEffect;

	pWoodEffect->Init();			//����������
	pWoodEffect->Set(Pos, SIZE);	//�ʒu�̐ݒ�

	return pWoodEffect;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CWoodEffect::CWoodEffect(){}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CWoodEffect::~CWoodEffect(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CWoodEffect::Init(void)
{
	C3DParticle::Init();

	//�ϐ��̏�����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fAngle = (float)((rand() % 6) - 3); 
	m_fSpeed = (float)((rand() % 5) + 5) * 0.1f;
	m_move.x = sinf(m_fAngle);
	m_move.z = cosf(m_fAngle);
	m_fMoveRot = (float)((rand() % 3) + 3) * 0.01f;
	m_nLife = LIFE;
	m_fCol_a = 1.0f;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CWoodEffect::Uninit(void)
{
	C3DParticle::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CWoodEffect::Update(void)
{
	C3DParticle::Update();

	//�`���Ԃ��擾����
	bool bDraw = GetDraw();

	//�ʒu�̎擾
	D3DXVECTOR3 pos = C3DParticle::GetPos();

	if (bDraw)
	{
		//�v���C���[�Ƃ̋��������߂�
		D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();
		D3DXVECTOR3 Distance = pos - PlayerPos;
		float fLength = sqrtf((Distance.x * Distance.x) + (Distance.z * Distance.z));

		if (fLength > LENGTH)
		{ //��苗���ȏ�Ȃ�
			bDraw = false;
			SetDraw(bDraw);
		}
	}

	//�����̍X�V
	D3DXVECTOR3 rot = C3DParticle::GetRot();
	rot.z += m_fMoveRot;
	rot.x += m_fMoveRot;
	rot.y += 0.01f;

	//�ʒu�̍X�V
	pos.y -= m_fSpeed;

	//�ړ��ʂ̐ݒ�
	pos.x += m_move.x;
	pos.z += m_move.z;

	//�����E�ʒu�̐ݒ�
	C3DParticle::SetRot(rot);
	C3DParticle::SetPos(pos);

	//�\�����Ԃ����炷
	m_nLife--;

	//�\�����Ԃ�0�ȉ��Ŕj��
	if (m_nLife < 0) { Uninit(); }
}

//=============================================================================
// �`�揈��
//=============================================================================
void CWoodEffect::Draw(void)
{
	C3DParticle::Draw();
}
