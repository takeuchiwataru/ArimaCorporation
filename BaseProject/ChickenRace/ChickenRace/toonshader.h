//=============================================================================
//
// トゥーンシェーダー基本処理 [ToonShader.h]
// Author : 有馬 武志
//
//=============================================================================
#ifndef _TOONSHADER_H_
#define _TOONSHADER_H_

#include "main.h"

//=============================================================================
// シェーダー基本クラス
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
	LPDIRECT3DTEXTURE9	m_pTexture;		//テクスチャの情報
	LPDIRECT3DTEXTURE9	m_pTextureLine;	//テクスチャの情報
	LPD3DXEFFECT		m_pShader;		//シェーダー情報
};
#endif