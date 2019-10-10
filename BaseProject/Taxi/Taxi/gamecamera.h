//=============================================================================
//
// �Q�[���J�������� [gamecamera.h]
// Author : ���������@
//
//=============================================================================
#ifndef _GAMECAMERA_H_
#define _GAMECAMERA_H_

#include "main.h"
#include "camerabace.h"

//=====================================================================
//�@   �O���錾
//=====================================================================
class CCustomer;
class CStartSignal;

//=====================================================================
//	   �Q�[���J�����N���X
//=====================================================================
class CGameCamera : public CCamera
{
public:
	typedef enum
	{//�J�����̏��
		MODE_NORMAL = 0,	//�ʏ���
		MODE_PUTON,			//��Ԏ�
		MODE_TAKEDOEN,		//�~�Ԏ�
		MODE_START,			//�X�^�[�g�J����
	}MODECAMERA;

	typedef enum MyEnum
	{	// �Q�[���J�n�����o
		DIRECTING_NONE = 0,	// �����Ȃ�
		DIRECTING_000,		// �ŏ�
		DIRECTING_001,		// 2�Ԗ�
		DIRECTING_002,		// 3�Ԗ�
		DIRECTING_END,		// �I��
	}DIRECTING;
	
	CGameCamera();
	~CGameCamera();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera();
	void SetModeCamera(MODECAMERA mode);
	MODECAMERA GetModeCamera(void) { return m_mode; };
	void SetCustomerInfo(CCustomer * pCustomer) { m_pCustomer = pCustomer; };
	void UninitSignal(void);
	DIRECTING GetDirecting(void) { return m_Directing; }
private:
	void UpdateNormal(void);
	void UpdatePutOn(void);
	void UpdateTakeDown(void);
	void UpdateStart(void);
	void DrawReset(void);
	void StartSkip(void);

	MODECAMERA m_mode;			//�J�����̏��
	MODECAMERA m_oldmode;		//�O��̏��
	DIRECTING  m_Directing;		//���o�̏��
	CCustomer * m_pCustomer;	//���q����̃|�C���^
	CStartSignal* m_pStartSignal;	// �X�^�[�g�\���̃|�C���^
	D3DXVECTOR3 m_DestPosV;		//�ړI�̈ʒu
	D3DXVECTOR3 m_Move;			//�ړ���
	float		m_fOldRotY;		//�O��̌���
	float		m_fAngle;		//�p�x
	float		m_fHoldAngle;	//�ۑ�����p�x
	float		m_fAddDirecting;
	bool		m_bSet;			//�ݒ肵�����ǂ���
	bool		m_bSetAngle;	//�p�x��ݒ肵�����ǂ���
};
#endif