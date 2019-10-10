//=============================================================================
//
// シェーダー基本処理 [baceshader.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _BACESHADER_H_
#define _BACESHADER_H_

#include "main.h"

//=============================================================================
// シェーダー基本クラス
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
	LPD3DXEFFECT		m_pShader;	//シェーダー情報
};
#endif