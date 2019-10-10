//=============================================================================
//
// ���̃G�t�F�N�g���� [smokeeffect.h]
// Author : �������� Sato_Asumi
//
//=============================================================================
#ifndef _SMOKEEFFECT_H_
#define _SMOKEEFFECT_H_

#include "main.h"
#include "particlebillboad.h"

//=============================================================================
// �Q�[���̋G�߃G�t�F�N�g�N���X
//=============================================================================
class CSmokeEffect : CParticleBillboad
{
public:
	CSmokeEffect();
	~CSmokeEffect();

	static CSmokeEffect * Create(D3DXVECTOR3 Pos);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void LoadTex(void);
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;		//�e�N�X�`��
	D3DXVECTOR2 m_size;			//�T�C�Y
	float		m_fCol_A;		//�F�̃��l
	int			m_nLife;		//���C�t
};
#endif
