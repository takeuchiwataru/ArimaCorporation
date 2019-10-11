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

//=====================================================================
//	   �Q�[���J�����N���X
//=====================================================================
class CGameCamera : public CCamera
{
public:
	CGameCamera();
	~CGameCamera();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera();
private:
	void UpdateNormal(void);
	void DrawReset(void);

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