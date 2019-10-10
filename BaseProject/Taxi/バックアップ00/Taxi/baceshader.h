//=============================================================================
//
// �V�F�[�_�[��{���� [baceshader.h]
// Author : �������� Sato_Asumi
//
//=============================================================================
#ifndef _BACESHADER_H_
#define _BACESHADER_H_

#include "main.h"

//=============================================================================
// �V�F�[�_�[��{�N���X
//=============================================================================
class CBaceShader
{
public:
	CBaceShader();
	~CBaceShader();

	void Init(char * FileName);
	void Uninit(void);
	LPD3DXEFFECT GetShader(void) { return m_pShader; }

protected:
	LPD3DXEFFECT		m_pShader;	//�V�F�[�_�[���
};
#endif