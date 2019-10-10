//=============================================================================
//
// �]���̏��� [reputation.h]
// Author : shun yokomichi
//
//=============================================================================
#ifndef _REPUTATION_H_
#define _REPUTATION_H_

#include "main.h"
#include "scene2D.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CLogo;
class CScene2D;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_FRAME	(3)		// �R�����g�̍ő吔

//=====================
//  CScene2D�̔h���N���X
//=====================
class CReputation : public CScene2D
{
public://�N�ł�������
	typedef enum
	{	// �]���̎��(5�i�K + ���s)
		REPUTATION_ONE = 0,	// �]��1(�Ⴂ)
		REPUTATION_TWO,		// �]��2(���ʂ�菭���Ⴂ)
		REPUTATION_THREE,	// �]��3(����)
		REPUTATION_FOUR,	// �]��4(���ʗp��������)
		REPUTATION_FIVE,	// �]��5(����)
		REPUTATION_FAIL,	// ���s
		REPUTATION_MAX		// ����
	}REPUTATIONTYPE;

	CReputation();
	~CReputation();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CReputation *Create(int nSatisfaction);
	static void SetLogoState(int nStateType, int nStateFont);

	void FrameDisplay(void);
	void Satisfaction(void);
private:
	void SetLevel(void);

	static int					m_nNextType;				//��Ԃ�Ⴄ�Ƃ��̃^�C�v
	static int					m_nNextFont;				//��Ԃ̕���
	int							m_nStateType;				//��Ԃ�Ⴄ�Ƃ��̃^�C�v
	int							m_nStateFont;				//��Ԃ̕���
	int							m_nSatisfaction;			//�����x
	D3DXVECTOR2					m_size;						//�T�C�Y
	float						m_fLength;					//����
	float						m_fAngle;					//�p�x
	D3DXVECTOR3					m_rot;						//����
	int							m_nCntTime;
	int							m_nCntNum;					// �R�����g�̉�
	int							m_nNumTexAnim[MAX_FRAME - 1];// �]���̋L�^
	REPUTATIONTYPE				m_Type;						// �R�����g�̎��
	CLogo *						m_pBack;					// ���S�ւ̃|�C���^(���w�i)
	CLogo *						m_apFrame[MAX_FRAME];		// ���S�ւ̃|�C���^(�R�����g�t���[��)
	CLogo *						m_apComment[MAX_FRAME];		// ���S�ւ̃|�C���^(�R�����g)
	int							m_nCommentNum[MAX_FRAME];
	int							m_nTexType;
	bool						m_bUninitFlag;
	int							m_nCntUninitTime;
};

#endif