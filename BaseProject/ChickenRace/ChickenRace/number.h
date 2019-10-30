//=============================================================================
//
// �������� [number.h]
// Author : �L�n�@���u
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "scene2D.h"

#define MAX_TEX_NUM		(2)

//=============================================================================
// �N���X��`
//=============================================================================
class CNumber
{
public:
	CNumber();
	~CNumber();

	HRESULT Init(D3DXVECTOR3 pos, int nType);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetNumber(int nNumber);
	void SetCol(D3DXCOLOR col);
	void SetSize(D3DXVECTOR2 size, D3DXVECTOR2 pos);

private:
	LPDIRECT3DVERTEXBUFFER9   m_pVtxBuff;				// ���_�o�b�t�@�ւ̃|�C���^
	int                       m_nType;					// �e�N�X�`���̔ԍ�
};

#endif