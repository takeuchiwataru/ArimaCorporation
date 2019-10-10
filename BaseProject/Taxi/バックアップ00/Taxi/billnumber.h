//=============================================================================
//
// �r���{�[�h�������� [billnumber.h]
// Author : �L�n�@���u
//
//=============================================================================
#ifndef _BILLNUMBER_H_
#define _BILLNUMBER_H_

#include "scene2D.h"

//=============================================================================
// �N���X��`
//=============================================================================
class CBillNumber
{
public:
	CBillNumber();
	~CBillNumber();

	static HRESULT Load(void);	//	�ǂݍ���
	static void Unload(void);	//	�J��

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetBillNumber(int nNumber);
	int GetNumber(void) { return m_nNumber; }
	void SetColor(D3DXCOLOR *Color);

private:
	static LPDIRECT3DTEXTURE9	m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX					m_mtxWorld;		//���[���h�}�g���b�N�X
	D3DXVECTOR3					m_pos;			//�ʒu
	int							m_nNumber;		//����������
};

#endif