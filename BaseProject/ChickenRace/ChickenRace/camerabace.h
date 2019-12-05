//=============================================================================
//
// �J�����x�[�X���� [camerabace.h]
// Author : ��������
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"
//=====================
//	   �}�N��
//=====================
#define OVER_DIS		(1000.0f)
#define IGNOR_SIZE		(2000.0f)

//=====================
//	   �J�����N���X
//=====================
class CCamera
{
public:

	CCamera();
	~CCamera();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera();
	float GetRot(void) { return m_rot.y; }
	void SetCameraPosV(D3DXVECTOR3 posV) { m_posV = posV; }
	void SetCameraPosR(D3DXVECTOR3 posR) { m_posR = posR; }
	D3DXVECTOR3 GetCameraPosV(void) { return m_posV; }
	D3DXVECTOR3 GetCameraPosR(void) { return m_posR; }
	bool Clipping(D3DXVECTOR3 &pos, D3DXVECTOR3 VtxMin, D3DXVECTOR3 VtxMax);				//�J�����Ɏʂ��Ă��邩�ǂ����̔���
	bool ClippingPos(D3DXVECTOR3 ObjPos);									//���S�_���J�����Ɏʂ��Ă��邩�ǂ����̔���
	D3DXMATRIX GetViewMatrix(void) { return m_mtxView; }				//�r���[�}�g���b�N�X�̎擾
	D3DXMATRIX GetProjectionMatrix(void) { return m_mtxProjection; }	//�v���W�F�N�V�����}�g���b�N�X�̎擾
	float &GetfCameraAngle(void) { return m_fCameraAngle; }	//�v���W�F�N�V�����}�g���b�N�X�̎擾

	// �r���[�|�[�g�ݒ�
	void SetViewPort(DWORD X, DWORD Y, DWORD Width, DWORD Height);

	void SetColl(bool bColl) { m_bCollCamera = bColl; }

protected:
	D3DXVECTOR3		m_posV;				//���_
	D3DXVECTOR3		m_posR;				//�����_
	D3DXVECTOR3		m_posVDest;			//�ړI�̎��_
	D3DXVECTOR3		m_posRDest;			//�ړI�̒����_
	D3DXVECTOR3		m_rot;				//�J�����̌���������
	float			m_fCameraAngle;		//�p�x
	float			m_fRotDest;			//�ړI�̊p�x
	D3DXVECTOR3		m_vecU;				//������x�N�g��

	void RemakeAngle(float * pAngle);
	
	//�萔
	const D3DXVECTOR3 VECTOR_ZERO = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

private:
	D3DXMATRIX		m_mtxProjection;	//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX		m_mtxView;			//�r���[�}�g���b�N�X
	D3DVIEWPORT9	m_viewport;			// �r���[�|�[�g
	bool			m_bCollCamera;		// �J��������

};

#endif