//=============================================================================
//
// �w�i�̏��� [background.h]
// Author : shun yokomichi
//
//=============================================================================
#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#include "scene2D.h"
#include "texture.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
class CBackGround : public CScene2D
{
public://�N�ł�������
	CBackGround(int nPriorityv = 1);
	~CBackGround();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CBackGround *Create(CTexture::TEXTYPE type);

private:
	int							m_nStateType;				//��Ԃ�Ⴄ�Ƃ��̃^�C�v
	int							m_nStateFont;				//��Ԃ̕���
	D3DXVECTOR2					m_size;						//�T�C�Y
	D3DXVECTOR3					m_rot;						//����
};



#endif