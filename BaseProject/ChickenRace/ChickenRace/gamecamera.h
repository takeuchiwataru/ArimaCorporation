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
class CPlayer;

//=====================================================================
//	   �Q�[���J�����N���X
//=====================================================================
class CGameCamera : public CCamera
{
public:
	typedef enum
	{
		CAMERA_NONE = 0,
		CAMERA_CHARSELECT,
		CAMERA_CHARUP,
		CAMERA_COURSE,
		CAMERA_GOUL,
		CAMERA_PLAYER,
		CAMERA_MAX
	}CAMERA;

	CGameCamera();
	~CGameCamera();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera();

	void SetType(CAMERA camera) { m_cameraType = camera; }
	void SetPlayer(CPlayer *pPlayer) { m_pPlayer = pPlayer; }
	void UseBoost(void) { m_fPlusDis = 1.5f; }
	float &GetfRot(void) { return m_fRot; };
	void SetTypeReset(void);

private:
	void UpdateCharSelect(void);
	void UpdateCharUp(void);
	void UpdateCourse(void);
	void UpdateGoul(bool bSet = false);
	void UpdatePlayer(bool bSet = false);
	void DrawReset(void);

	CAMERA		m_cameraType;
	CPlayer		*m_pPlayer;
	float		m_fDistance;
	float		m_fPlusDis;
	float		m_fRot;			//�v�Z��̊p�x
	float		m_fGAngle;		//�ڎw����p

	int			m_nTopPlayer;
};
#endif