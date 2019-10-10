//=============================================================================
//
// �x�@�̃����v���� [policelanmp.h]
// Author : �L�n���u
//
//=============================================================================
#ifndef _POLICELAMP_H_
#define _POLICELAMP_H_

#include "main.h"
#include "scene3D.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define POLICELAMP_PRIOTITY		(5)
#define	MAX_POLICELAMP_TEX		(1)

//=============================================================================
// �N���X��`
//=============================================================================
class CPoliceLamp : public CScene3D
{
public:
	CPoliceLamp();
	~CPoliceLamp();

	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CPoliceLamp *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);

private:
	static LPDIRECT3DTEXTURE9	m_pTexture[MAX_POLICELAMP_TEX];	// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;					// ���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX					m_mtxWorld;					// ���[���h�}�g���b�N�X
	int							m_nType;					// �ǂ̕���
};
#endif
