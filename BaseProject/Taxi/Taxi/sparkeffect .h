//=============================================================================
//
// �ΉԂ̃G�t�F�N�g���� [sparkeffect.h]
// Author : �������� Sato_Asumi
//
//=============================================================================
#ifndef _SPARKEFFECT_H_
#define _SPARKEFFECT_H_

#include "main.h"
#include "particlebillboad.h"

//=============================================================================
// �Q�[���̋G�߃G�t�F�N�g�N���X
//=============================================================================
class CSparkEffect : CParticleBillboad
{
public:
	CSparkEffect();
	~CSparkEffect();

	static CSparkEffect * Create(D3DXVECTOR3 Pos);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void LoadTex(void);
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;		//�e�N�X�`��
	float		m_fCol_A;		//�F�̃��l
	float		m_fCol_G;		//�F�̂��l
	float		m_fCol_B;		//�F�̂��l
	float		m_fChangeCol_G;	//�F�̕ω���
	float		m_fChangeCol_B;	//�F�̕ω���
	int			m_nLife;		//���C�t
	D3DXVECTOR3 m_move;			//�ړ���
};
#endif
