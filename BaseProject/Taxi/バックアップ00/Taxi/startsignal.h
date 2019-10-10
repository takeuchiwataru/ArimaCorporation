//************************************************************
//
//	�X�^�[�g���}�̏���[startsignal.h]
//	Author : Shun Yokomichi
//
//************************************************************
#ifndef _STARTSIGNAL_H_
#define _STARTSIGNAL_H_

#include "main.h"
#include "manager.h"
#include "renderer.h"

//************************************************************
// �}�N����`
//************************************************************
#define MAX_SIGNAL		(2)		// �X�^�[�g���}�̐�
//************************************************************
// �O���錾
//************************************************************
class CLogo;

//************************************************************
// �N���X
//************************************************************
class CStartSignal
{
public:
	typedef enum
	{
		SIGNAL_HERE = 0,
		SIGNAL_WE,
		SIGNAL_GO,
		SIGNAL_NONE,
	}SIGNAL;

	CStartSignal();
	~CStartSignal();
	static CStartSignal *Create(D3DXVECTOR2 size, SIGNAL type);
	HRESULT Init();
	void Uninit();
	void Updata();
	void Draw();

	bool GetEnd(void) { return m_bEnd; }
private:
	void SignalUpdata(bool bFrag, int nType);
	void StartUpdata(void);

	CLogo			*m_pSignal[MAX_SIGNAL];
	CLogo			*m_pStart;

	SIGNAL			m_SignalType;		// ���}�̏��
	D3DXVECTOR2		m_size;				// �T�C�Y
	float			m_fColA;			// �����x�̐F
	bool			m_bEnd;				// �I���̂��m�点
	bool			m_bColA;			
	int				m_nCntDisplay;		// �\������
};

#endif // !_STARTSIGNAL_H_
