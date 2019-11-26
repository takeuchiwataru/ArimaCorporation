//=============================================================================
//
// �p�[�e�B�N���̏��� [particle.h]
// Author : ���R���
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"
#include "billboord.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_PARTICLE3D		(256)							// �p�[�e�B�N���̐�
#define MAX_EFFECT_TEX		(2)								// �G�t�F�N�g�̃e�N�X�`����

//========================================
// �N���X�̒�`
//========================================
//=====================
// �p�[�e�B�N���N���X
//=====================
class CParticle : public CBillBoord
{
public:
	CParticle();	// �R���X�g���N�^
	~CParticle();	// �f�X�g���N�^

	typedef enum
	{// �L�[�v�f
		TYPE_NORMAL = 0,	// �ʏ���
		TYPE_DOWN,			// �������
		PARTICLESTATE_MAX			// �v���C���[�̎�ނ̑���
	} PARTICLE_TYPE;

	HRESULT Init(void);	// 3D�p�[�e�B�N������������
	void Uninit(void);	// 3D�p�[�e�B�N���I������
	void Update(void);	// 3D�p�[�e�B�N���X�V����
	void Draw(void);	// 3D�p�[�e�B�N���`�揈��
	static HRESULT Load(void);
	static void UnLoad(void);

	static CParticle *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, D3DXVECTOR2 size, int nLife, PARTICLE_TYPE type);	// �I�u�W�F�N�g�̐���


private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_EFFECT_TEX];

	int m_nLife;
	D3DXCOLOR m_col;
	D3DXVECTOR2 m_Size;
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_rot;
	PARTICLE_TYPE m_ParticleType;
};

#endif