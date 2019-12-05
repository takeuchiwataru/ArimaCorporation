//=============================================================================
//
// UI�̏��� [ui.h]
// Author : �L�n���u
//
//=============================================================================
#ifndef _UI_H_
#define _UI_H_

#include "main.h"
#include "scene2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//=====================
//  CScene2D�̔h���N���X
//=====================
class CUi : public CScene2D
{
public://�N�ł�������
	typedef enum
	{
		UI_TITLE_LOGO = 0,		//�^�C�g�����S
		UI_PRESS_BUTTON,		//�v���X�{�^���w��
		UI_MAX					//�ő吔
	}UI;

	CUi();
	~CUi();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CUi *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, CUi::UI m_Ui);
	void UiMovement(void);

private:
	static LPDIRECT3DTEXTURE9	m_pTexture[MAX_TEXTURE];	//���L�e�N�X�`���̃|�C���^
	static int					m_nNextType;				//��Ԃ�Ⴄ�Ƃ��̃^�C�v
	static int					m_nNextFont;				//��Ԃ̕���
	static UI					m_Ui;						//UI�̎��
	float						m_fAlpha;					//�����x
	int							m_nStateType;				//��Ԃ�Ⴄ�Ƃ��̃^�C�v
	int							m_nStateFont;				//��Ԃ̕���
	int							m_nType;					//���
	int							m_nTransformType;			//�摜�̌`��ς����^�C�v
	D3DXVECTOR2					m_size;						//�T�C�Y
	float						m_fLength;					//����
	float						m_fAngle;					//�p�x
	D3DXVECTOR3					m_rot;						//����
	D3DXVECTOR3					m_pos;						//�ʒu
	int							m_nCntTutorial;
	bool						m_bOneCnt;
};
#endif