//=============================================================================
//
// 回転シェーダー処理 [rotationshader.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _ROTATIONSHADER_H_
#define _ROTATIONSHADER_H_

#include "main.h"
#include "baceshader.h"

//=============================================================================
// シェーダー基本クラス
//=============================================================================
class CRotationShader : public CBaceShader
{
public:
	CRotationShader();
	~CRotationShader();

	static CRotationShader * Create(void);
	void Init(void);
	void Uninit(void);
	void SetBegin(void);
	void SetMatrix(D3DXMATRIX * pMtxRot, D3DXMATRIX * pMtxWorld, D3DXMATRIX * pMtxView, D3DXMATRIX * pMtxProjection);
	void SetBeginPass(void);
	void EndShader(void);
	void SetCol_a(float col_a);
};
#endif