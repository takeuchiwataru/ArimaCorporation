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
	LPDIRECT3DTEXTURE9 GetMapTexture(void);
	LPDIRECT3DTEXTURE9 GetLineTexture(void);
	LPDIRECT3DTEXTURE9 GetLineMapTexture(void);

protected:
	LPDIRECT3DTEXTURE9	m_pTexture;				//�g�D�[���e�N�X�`���̏��
	LPDIRECT3DTEXTURE9	m_pTextureMap;			//�}�b�v�e�N�X�`���̏��
	LPDIRECT3DTEXTURE9	m_pTextureLine;			//���C���e�N�X�`���̏��
	LPDIRECT3DTEXTURE9	m_pTextureMapLine;		//�}�b�v�e�N�X�`���̏��
	LPD3DXEFFECT		m_pShader;				//�V�F�[�_�[���
};
#endif