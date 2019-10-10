//=============================================================================
//
// �ړI�n�̏��� [destination.h]
// Author : shun yokomichi
//
//=============================================================================
#ifndef _DESTINATION_H_
#define _DESTINATION_H_

#include "main.h"
#include "billboord.h"
#include "scene2D.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CLogo;

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//=====================
//  CScene2D�̔h���N���X
//=====================
class CDestination : public CScene2D
{
public://�N�ł�������
	typedef enum
	{	// �ړI�n�̎��
		DESTINATION_SAPPRO = 0,		// �D�y
		DESTINATION_MAIN_STREET,	// ��ʂ�
		DESTINATION_TV_TOWER,		// �e���r��
		DESTINATION_CLOCKTOWER,		// ���v��
		DESTINATION_TANUKIKOJI,		// �K���H
		DESTINATION_NORVESA,		// �m���x�T
		DESTINATION_RED_BRICK,		// �ԃ����K
		DESTINATION_SUSUKI_SRREET,	// �������̒ʂ�
		DESTINATION_FOUNTAIN,		// ����
		DESTINATION_TAPIOCAR ,		// �^�s�I�J�\
		DESTINATION_HOSPITAL,		// �a�@
		DESTINATION_MAX
	}GOALTYPE;

	CDestination();
	~CDestination();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CDestination *Create(int nDestination);
	static void SetLogoState(int nStateType, int nStateFont);

	void SetDelet(bool bDelet) { m_bDelet = bDelet; }
	bool GetDelet(void) { return m_bDelet; }
private:
	static int					m_nNextType;				//��Ԃ�Ⴄ�Ƃ��̃^�C�v
	static int					m_nNextFont;				//��Ԃ̕���
	int							m_nStateType;				//��Ԃ�Ⴄ�Ƃ��̃^�C�v
	int							m_nStateFont;				//��Ԃ̕���
	int							m_nDestination;				//�ړI�n�̃^�C�v
	D3DXVECTOR2					m_size;						//�T�C�Y
	float						m_fLength;					//����
	float						m_fAngle;					//�p�x
	D3DXVECTOR3					m_rot;						//����
	int							m_nCntTime;
	bool						m_bDelet;					//�\�����������ǂ���
	CLogo						*m_pLogo;					// ���S�ւ̃|�C���^
};

#endif