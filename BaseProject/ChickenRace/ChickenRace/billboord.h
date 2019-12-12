//=============================================================================
//
// �r���{�[�h�̏��� [billboord.h]
// Author : �L�n���u
//
//=============================================================================
#ifndef _BILLBOORD_H_
#define _BILLBOORD_H_

#include "main.h"
#include "scene.h"
#include "scene3D.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

//=====================
//  CScene�̔h���N���X
//=====================
class CBillBoord : public CScene3D
{
public://�N�ł�������
	CBillBoord();
	~CBillBoord();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CBillBoord *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, bool bMap = false);

	bool GetMapOn(void) { return m_bMap; }

private://�l�ł̂ݎg��
	D3DXVECTOR3					m_pos;							// �ʒu
	D3DXVECTOR2					m_size;							// �T�C�Y

	bool						m_bMap;							// �}�b�v
};
#endif