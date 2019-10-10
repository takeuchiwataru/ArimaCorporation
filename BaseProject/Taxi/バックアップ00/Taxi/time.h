//=============================================================================
//
// �^�C�}�[���� [time.h]
// Author : �L�n�@���u
//
//=============================================================================
#ifndef _TIME_H_
#define _TIME_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
//�@�O���錾
//*****************************************************************************
class CNumber;
class CLogo;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TIME_MAX			(3)		// �^�C�}�[�̌���
#define CUTOMERTIME_MAX		(2)		// ���q�l�̎��Ԍ���

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CTime : public CScene
{
public:
	typedef enum
	{
		STATE_NONE = 0,		// �����Ȃ�
		STATE_TIMEADD,		// �������ԉ��Z
		STATE_MAX			// ����
	}STATETIME;

	typedef enum
	{
		STATECUTOMERADD_NONE = 0,	// �����Ȃ�
		STATECUTOMERADD_CUTOMERADD,	// �������ԉ��Z
		STATECUTOMERADD_MAX			// ����
	}STATECUTOMERADD;

	CTime(int nPriority = 6, OBJTYPE objType = OBJTYPE_TIME);
	~CTime();

	static CTime *Create(D3DXVECTOR3 pos);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetCutomer(void);
	void TexTime(int nTexData);
	void TexTimCutomerTimee(int nTexData);

	//============================
	// �ݒ�@�擾�@�̊֐�
	//============================
	static int GetTime(void) { return m_nTime; };
	static int GetCutomerTime(void) { return m_nCutomerTime;}
	static void AddTime(int nTime);
	static void SetCutomerTime(int nCutomerTime)
	{ m_nCutomerTime = nCutomerTime; m_bRideEnd = false; m_stateCutomer = STATECUTOMERADD_CUTOMERADD;}
	static bool GetRideEnd(void) { return m_bRideEnd; }
	static void ResetCutomerTime(void) { m_nCutomerTime = 0; }

private:
	void DebugKey(void);
	void TimeManagement(void);
	int PowerCalculation(int nData, int nOperation);
	CNumber *m_apNumber[TIME_MAX];	// �i���o�[�ւ̃|�C���^
	CNumber *m_apCutomerTiem[CUTOMERTIME_MAX];
	static STATETIME		m_stateTime;				// ���
	static STATECUTOMERADD	m_stateCutomer;
	static int				m_nTime;			// ����
	static int				m_nCutomerTime;		// ���q�l�̎���
	static bool				m_bRideEnd;			// ��Ԏ��Ԃ��߂����ꍇ�̃t���O
	int						m_nTimeCount;		// ���Ԃ̃J�E���^
	int						m_nCutmoerCount;	// ��Ԏ��Ԃ̃J�E���^
	int						m_nTimeNum;			// ���Ԃ̕\��������
	int						m_nCutmoerNum;		// ��q���Ԃ̕\��������
	int						m_nCntDirecting;	// ���o����
	D3DXVECTOR3				m_pos;				// �ʒu
	CLogo					*m_pLogo;			// ���S�ւ̃|�C���^
	bool					m_bUse;				// �g�p���Ă��邩�ǂ���
};
#endif