//=============================================================================
//
// メイン処理 [renderer.h]
// Author : 有馬武志
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "main.h"

//=====================
//	   基本クラス
//=====================
class CRenderer
{
public://誰でも扱える
	CRenderer();
	~CRenderer();
	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }//NULLチェックとかない＆＆1つだけの時

private://個人でのみ使う
	LPDIRECT3D9				m_pD3D;					// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9		m_pD3DDevice;			// Deviceオブジェクト(描画に必要)
	bool					m_bOnOff;
};


#endif