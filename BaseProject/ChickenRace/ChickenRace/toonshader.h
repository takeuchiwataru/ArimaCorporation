//=============================================================================
//
// �g�D�[���V�F�[�_�[��{���� [ToonShader.h]
// Author : �L�n ���u
//
//=============================================================================
#ifndef _TOONSHADER_H_
#define _TOONSHADER_H_

#include "main.h"

//=============================================================================
// �V�F�[�_�[��{�N���X
//=============================================================================
class CToonShader
{
public:
	CToonShader();
	~CToonShader();

	void Init(char * FileName);
	void Uninit(void);
	LPD3DXEFFECT GetShader(void);
	LPDIRECT3DTEXTURE9 GetTexture(void);
	LPDIRECT3DTEXTURE9 GetLineTexture(void);

protected:
	LPDIRECT3DTEXTURE9	m_pTexture;		//�e�N�X�`���̏��
	LPDIRECT3DTEXTURE9	m_pTextureLine;	//�e�N�X�`���̏��
	LPD3DXEFFECT		m_pShader;		//�V�F�[�_�[���
};
#endif