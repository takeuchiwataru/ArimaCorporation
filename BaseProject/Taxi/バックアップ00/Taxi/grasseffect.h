//=============================================================================
//
// ���̃G�t�F�N�g���� [grasseffect.h]
// Author : �������� Sato_Asumi
//
//=============================================================================
#ifndef _GRASSEFFECT_H_
#define _GRASSEFFECT_H_

#include "main.h"
#include "particlebillboad.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_PATTERN		(4)

//=============================================================================
// �Q�[���̋G�߃G�t�F�N�g�N���X
//=============================================================================
class CGrassEffect : CParticleBillboad
{
public:
	CGrassEffect();
	~CGrassEffect();

	static CGrassEffect * Create(D3DXVECTOR3 Pos, int nNumPattern);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void LoadTex(void);
	static void Unload(void);

private:
	void SetNumPattern(int NumPattern) { m_nNumPattern = NumPattern; }
	static LPDIRECT3DTEXTURE9 m_pTexture;		//�e�N�X�`��
	D3DXVECTOR2 m_size;			//�T�C�Y
	D3DXVECTOR3 m_move;			//�ړ���
	float		m_fCol_A;		//�F�̃��l
	int			m_nLife;		//���C�t
	int			m_nNumPattern;	//�p�^�[���ԍ�
};
#endif
