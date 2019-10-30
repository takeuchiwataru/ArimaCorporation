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
		CAMERA_COURSE,
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

private:
	void UpdateCourse(void);
	void UpdatePlayer(void);
	void DrawReset(void);

	CAMERA		m_cameraType;
	CPlayer		*m_pPlayer;
};
#endif