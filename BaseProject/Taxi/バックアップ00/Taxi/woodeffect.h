//=============================================================================
//
// �؂̃G�t�F�N�g���� [woodeffect.h]
// Author : �������� Sato_Asumi
//
//=============================================================================
#ifndef _WOODEFFECT_H_
#define _WOODEFFECT_H_

#include "main.h"
#include "3Dparticle.h"

//=============================================================================
// �Q�[���̋G�߃G�t�F�N�g�N���X
//=============================================================================
class CWoodEffect : C3DParticle
{
public:
	CWoodEffect();
	~CWoodEffect();

	static CWoodEffect * Create(D3DXVECTOR3 Pos);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture) { C3DParticle::BindTexture(pTexture); }
	void SetDivision(D3DXVECTOR2 Division) { C3DParticle::SetDivision(Division); }
	void SetUV(int nWidth, int nHight) { C3DParticle::SetUV(nWidth, nHight); }

private:
	D3DXVECTOR3 m_move;			//�ړ���
	float		m_fAngle;		//�p�x
	float		m_fAimAngle;	//�ړI�̊p�x
	float		m_fSpeed;		//���x
	float		m_fMoveRot;		//�ړ��p�x
	int			m_nLife;		//�\������
	float		m_fCol_a;		//�J���[�̃��l
};
#endif
