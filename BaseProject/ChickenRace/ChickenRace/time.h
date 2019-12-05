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
class CScene2D;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TIME_MAX			(5)		// �^�C�}�[�̌���
#define CUTOMERTIME_MAX		(2)		// ���q�l�̎��Ԍ���
#define MAX_COLON			(2)

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

	typedef enum
	{
		TEXTURE_COLON = 0,
		TEXTURE_MAX
	}TEXTURE;

	CTime(int nPriority = 5, OBJTYPE objType = OBJTYPE_TIME);
	~CTime();

	static HRESULT Load(void);
	static void Unload(void);
	static CTime *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void TexTime(int nTexData, bool bEdit = false);

	//============================
	// �ݒ�@�擾�@�̊֐�
	//============================
	static int GetTime(void) { return m_nTime; };
	static int GetCutomerTime(void) { return m_nCutomerTime; }
	static void AddTime(int nTime);
	static void SetCutomerTime(int nCutomerTime)
	{
		m_nCutomerTime = nCutomerTime; m_bRideEnd = false; m_stateCutomer = STATECUTOMERADD_CUTOMERADD;
	}
	static bool GetRideEnd(void) { return m_bRideEnd; }
	static void ResetCutomerTime(void) { m_nCutomerTime = 0; }

	void Setpos(D3DXVECTOR3 pos);
	void Setcol(D3DXCOLOR col);

private:
	static LPDIRECT3DTEXTURE9	m_pTexture[TEXTURE_MAX];	// �e�N�X�`���ւ̃|�C���^

	CNumber *m_apNumber[TIME_MAX];	// �i���o�[�ւ̃|�C���^
	CScene2D *m_pColon[MAX_COLON];
	static STATETIME		m_stateTime;				// ���
	static STATECUTOMERADD	m_stateCutomer;
	static int				m_nTime;			// ����
	static int				m_nCutomerTime;		// ���q�l�̎���
	static bool				m_bRideEnd;			// ��Ԏ��Ԃ��߂����ꍇ�̃t���O
	int						m_nTimeCount;		// ���Ԃ̃J�E���^
	int						m_nCutmoerCount;	// ��Ԏ��Ԃ̃J�E���^
	int						m_nCutmoerNum;		// ��q���Ԃ̕\��������
	int						m_nCntDirecting;	// ���o����
	D3DXVECTOR3				m_pos;				// �ʒu
	D3DXVECTOR3				m_size;				// �ʒu
	bool					m_bUse;				// �g�p���Ă��邩�ǂ���
};
#endif