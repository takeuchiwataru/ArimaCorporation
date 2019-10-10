//=============================================================================
//
// �����x�̏��� [satisfactionlevel.h]
// Author : shun yokomichi
//
//=============================================================================
#ifndef _SATISFACTION_H_
#define _SATISFACTION_H_

#include "main.h"
#include "billboord.h"
#include "scene2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//=====================
//  CScene2D�̔h���N���X
//=====================
class CSatisfaction : public CScene2D
{
public://�N�ł�������
	typedef enum
	{	// �F�̕ω�
		COL_NOTCHANGE = 0,	// �ʏ�F
		COL_CHANGE,			// �F��ς���
		TYPE_MAX
	}COL_TYPE;

	typedef enum 
	{
		LEVEL_NONE = 0,		// �����Ȃ�
		LEVEL_UP,			// �オ��
		LEVEL_DOWN,			// ������
		LEVEL_MAX			// ����
	}LEVEL_TYPE;

	CSatisfaction();
	~CSatisfaction();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CSatisfaction *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nLevel);
	static void SetLogoState(int nStateType, int nStateFont);
	void SetColAType(COL_TYPE type) { m_type = type; }		// �^�C�v�̐ݒ�
	void AddLevel(int nLevel);		// �����x���Z
	void Subtraction(int nLevel);	// �����x���Z
	int GetLevel(void) { return m_nLevel; }	// �����x�̎擾
	void ChangeCol(D3DXCOLOR col);			// �F�̕ω�

private:
	void ResultEvaluation(void);		// ���U���g���̌��ʕ�
	static int					m_nNextType;				//��Ԃ�Ⴄ�Ƃ��̃^�C�v
	static int					m_nNextFont;				//��Ԃ̕���
	int							m_nStateType;				//��Ԃ�Ⴄ�Ƃ��̃^�C�v
	int							m_nStateFont;				//��Ԃ̕���
	D3DXVECTOR2					m_size;						//�T�C�Y
	float						m_fLength;					//����
	float						m_fAngle;					//�p�x
	D3DXVECTOR3					m_rot;						//����
	int							m_nCntTime;
	int							m_nCntNum;					// ���̐��J�E���g
	int							m_nCntLevel;
	bool						m_bChange;
	int							m_nLevel;					// �����x�̃��x��
	float						m_fSize;
	COL_TYPE					m_type;
	LEVEL_TYPE					m_LevelType;
	int							m_nLevelOld;
	float						m_Col;
	bool						m_bColFlag;
};

#endif