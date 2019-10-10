//=============================================================================
//
// �\�������̏��� [numberdisplay.h]
// Author : shun yokomichi
//
//=============================================================================
#ifndef _NUMBERDISPLAY_H_
#define _NUMBERDISPLAY_H_

#include "main.h"
#include "scene2D.h"

//*****************************************************************************
//�@�O���錾
//*****************************************************************************
class CNumber;
class CLogo;

//*****************************************************************************
//�@�}�N����`
//*****************************************************************************
#define ADDSCORE_DISPLAY	(8)		// ���Z����X�R�A�̌���

//*****************************************************************************
// �X�R�A�����̕\��
//*****************************************************************************
class CNumberDisplay : public CScene
{
public:
	CNumberDisplay();
	~CNumberDisplay() {};

	static CNumberDisplay *Create(int nNumber);
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetNumber(int nNumber) { m_nNumber = nNumber; }
	void Reset(void) { Uninit(); }
	int PowerCalculation(int nData);
private:
	int m_nNumCount;
	int m_nNumber;
	int m_nCntDelet;
	int m_nUp;
	CNumber *m_apNumber[ADDSCORE_DISPLAY];	// �i���o�[�ւ̃|�C���^
	CLogo	*m_pLogo;
	CLogo	*m_pPlus;
};


#endif

