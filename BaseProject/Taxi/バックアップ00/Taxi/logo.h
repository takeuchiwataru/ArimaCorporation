//=============================================================================
//
// ���S�̏��� [logo.h]
// Author : �L�n���u
//
//=============================================================================
#ifndef _LOGO_H_
#define _LOGO_H_

#include "main.h"
#include "billboord.h"
#include "scene2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//=====================
//  CScene2D�̔h���N���X
//=====================
class CLogo : public CScene2D
{
public://�N�ł�������
	typedef enum
	{
		TYPE_LOGO = 0,			// �ʏ�̃��S
		TYPE_MULTIRENDERING,	// �}���`�����_�����O
		TYPE_MAX
	}TYPE;

	CLogo(int nPriorityv = 5);
	~CLogo();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CLogo *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nType, int nTransformType, TYPE type);
	static void SetLogoState(int nStateType, int nStateFont);
	D3DXVECTOR3 MoveLimit(D3DXVECTOR3 pos, D3DXVECTOR3 PosLimit, int nOperation, float fMove);
	bool GetMoveFlag(void) { return m_bMoveFlag; }
	void Zoom(D3DXVECTOR3 pos, D3DXVECTOR2 EndSize, D3DXVECTOR2 AddSize, bool bZoom);
	D3DXVECTOR2 SetSize(D3DXVECTOR2 size) { m_size = size; return m_size; }


private:
	void TexAnim(void);
	void SetCol(void);
	// �g��k���̏���

	D3DXVECTOR3 Move(D3DXVECTOR3 pos, float fMove);

	static int					m_nNextType;				//��Ԃ�Ⴄ�Ƃ��̃^�C�v
	static int					m_nNextFont;				//��Ԃ̕���
	int							m_nStateType;				//��Ԃ�Ⴄ�Ƃ��̃^�C�v
	int							m_nStateFont;				//��Ԃ̕���
	int							m_nType;					//���
	int							m_nPolygonType;				//���(�摜)
	int							m_nTransformType;			//�摜�̌`��ς����^�C�v
	int							m_nCount;					//�J�E���^�[
	D3DXVECTOR3					m_posOld;					//�ʒu�ۑ�
	D3DXVECTOR2					m_size;						//�T�C�Y
	float						m_fLength;					//����
	float						m_fAngle;					//�p�x
	D3DXVECTOR3					m_rot;						//����
	D3DXVECTOR3					m_moveLimit;				// �ړ�����
	int							m_nCntColA;					// �����x�̃J�E���^�[
	bool						m_bColA;					// �����x�̃t���O
	bool						m_bMoveFlag;				// �ړ��������̃t���O
	bool						m_bFlag;					//�t���O
	bool						m_bDataFlag;				//�I���t���O
	int							m_nCntData;					// �J�E���g
	int							m_nCntTime;
	bool						m_bVibration;				//�U�����Đ��������ǂ���
};

#endif