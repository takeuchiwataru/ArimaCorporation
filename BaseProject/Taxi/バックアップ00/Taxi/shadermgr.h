//=============================================================================
//
// シェーダー管理処理 [shadermgr.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _SHADERMGR_H_
#define _SHADERMGR_H_

#include "main.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CRotationShader;

//=============================================================================
// シェーダー管理クラス
//=============================================================================
class CShaderMgr
{
public:
	CShaderMgr();
	~CShaderMgr();

	static CShaderMgr * Create();
	void Init(void);
	void Uninit(void);

	//シェーダーの取得
	CRotationShader * GetRotatonShader(void) { return m_pRotationShader; }	//ローテーション

private:
	CRotationShader * m_pRotationShader;	//回転シェーダー
};
#endif
