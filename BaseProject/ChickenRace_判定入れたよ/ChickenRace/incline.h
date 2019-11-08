//=============================================================================
//
// �X�Ώ��� [incline.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _INCLINE_H_
#define _INCLINE_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CMeshField;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CIncline
{
public:
	CIncline();
	~CIncline();
	HRESULT Init(void);
	void Uninit(void);
	void Update(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSpeed);

	static CIncline* Create(void);

	CMeshField *GetOnField(void) { return m_pField; };						// ���ݏ���Ă���n�ʂ̎擾
	void SetOnField(CMeshField *pField) { m_pField = pField; };				// ���ݏ���Ă���n�ʂ̐ݒ�

	float GetJumpHeight(void) { return m_fJumpHeight; };					// �W�����v�̍����̎擾
	void SetJumpHeight(float fJumpHeight) { m_fJumpHeight = fJumpHeight; };	// �W�����v�̍����̐ݒ�

	float GetSpeed(void) { return m_fSpeed; };								// ���x�̎擾
	void SetSpeed(float fSpeed) { m_fSpeed = fSpeed; };						// ���x�̐ݒ�

	bool GetJumpFlag(void) { return m_bJump; };								// �W�����v��Ԃ̎擾
	void SetJumpFlag(bool bJump) { m_bJump = bJump; };						// �W�����v��Ԃ̐ݒ�

	float GetAngle(void) { return m_fAngle; };								// �����ƒn�ʂ̌�_�̊p�x�̎擾
	void SetAngle(float fAngle) { m_fAngle = fAngle; };						// �����ƒn�ʂ̌�_�̊p�x�̐ݒ�

	float GetClimbSpeed(void) { return m_fClimbSpeed; };					// �X�΂����Ƃ��̑��x�␳�̎擾
	void SetClimbSpeed(float fClimbSpeed) { m_fClimbSpeed = fClimbSpeed; };	// �X�΂����Ƃ��̑��x�␳�̐ݒ�

	D3DXMATRIX GetRot(D3DXVECTOR3 pos, D3DXMATRIX mtxRot);					// �X�΂ɉ������p�x�̎擾

private:
	CMeshField		*m_pField;			// ����Ă���n�ʂ̃|�C���^
	float			m_fJumpHeight;		// �W�����v�̍���
	float			m_fSpeed;			// ���x
	bool			m_bJump;			// �W�����v�t���O
	float			m_fAngle;			// �����ƒn�ʂ̌�_�̊p�x
	float			m_fClimbSpeed;		// �X�΂����Ƃ��̑��x�␳
	D3DXVECTOR3		m_norField;			// �n�ʂ̖@��
};

#endif