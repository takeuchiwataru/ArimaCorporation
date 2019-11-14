//=============================================================================
//
// ���C������ [renderer.h]
// Author : �L�n���u
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "main.h"

//=====================
//	   ��{�N���X
//=====================
class CRenderer
{
public://�N�ł�������
	CRenderer();
	~CRenderer();
	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }//NULL�`�F�b�N�Ƃ��Ȃ�����1�����̎�

private://�l�ł̂ݎg��
#ifdef _DEBUG
	void DrawFPS(void);
#endif
	LPDIRECT3D9				m_pD3D;					// Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9		m_pD3DDevice;			// Device�I�u�W�F�N�g(�`��ɕK�v)
	bool					m_bOnOff;
#ifdef _DEBUG
	//LPD3DXFONT				m_pFont;				// �t�H���g�ւ̃|�C���^
#endif
};


#endif