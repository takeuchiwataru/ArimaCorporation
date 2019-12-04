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

	static void Load(void);
	static void Unload(void);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nType);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetNumber(int nNumber);
	void SetCol(D3DXCOLOR col);
	void SetSize(D3DXVECTOR2 size, D3DXVECTOR2 pos);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;				// �e�N�X�`�����ւ̃|�C���^

	LPDIRECT3DVERTEXBUFFER9   m_pVtxBuff;				// ���_�o�b�t�@�ւ̃|�C���^
	int                       m_nType;					// �e�N�X�`���̔ԍ�
};

#endif