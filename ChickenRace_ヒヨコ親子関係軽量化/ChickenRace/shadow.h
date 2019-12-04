//=============================================================================
//
// �e�̏��� [shadow.h]
// Author : �L�n���u
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"
#include "scene.h"
#include "scene3D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SHADOW_PRIOTITY		(3)

//=====================
//  CScene3D�̔h���N���X
//=====================
class CShadow : public CScene3D
{
public://�N�ł�������
	CShadow();
	~CShadow();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CShadow *Create(D3DXVECTOR3 pos, float fWidth, float fDepth,int nTexType);
	void SetPositionShadow(D3DXVECTOR3 pos);
	float GetShadowHeight(void) { return m_fMeshHeight; }

private://�l�ł̂ݎg��
	static LPDIRECT3DTEXTURE9	m_pTexture[MAX_BILLBOORD_TEX];	// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;						// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3					m_pos;							// �ʒu
	float						m_fWidth;						// ����
	float						m_fDepth;						// ���s	
	static float				m_fMeshHeight;					// �e����݂����b�V���̍���
	
};
#endif