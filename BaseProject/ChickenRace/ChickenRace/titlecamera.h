//=============================================================================
//
// タイトルカメラ処理 [titlecamera.h]
// Author : 有馬 武志
//
//=============================================================================
#ifndef _TITLECAMERA_H_
#define _TITLECAMERA_H_

#include "main.h"
#include "camerabace.h"

//=====================================================================
//　   前方宣言
//=====================================================================
class CPlayer;

//=====================================================================
//　   リザルトカメラクラス
//=====================================================================
class CTitleCamera : public CCamera
{
public:
	typedef enum
	{
		CAMERA_NONE = 0,
		CAMERA_NORMAL,
		CAMERA_CHARSELECT,
		CAMERA_CHARUP,
		CAMERA_COURSE,
		CAMERA_GOUL,
		CAMERA_PLAYER,
		CAMERA_MAX
	}CAMERA;

	CTitleCamera();
	~CTitleCamera();

	HRESULT Init(void);
	void Uninit(void);
	void Updata(void);
	void SetCamera();

	void SetType(CAMERA camera) { m_cameraType = camera; m_nTypeCounter = 0; }
	void SetPlayer(CPlayer *pPlayer) { m_pPlayer = pPlayer; }
	void UseBoost(void) { m_fPlusDis = 1.5f; }
	void SetTypeReset(void);

private:
	void UpdateNormal(void);
	void UpdateCharSelect(void);
	void UpdateCharUp(void);
	void UpdateCourse(void);
	void UpdateGoul(bool bSet = false);
	void UpdatePlayer(bool bSet = false);
	void DrawReset(void);

	CAMERA		m_cameraType;
	CPlayer		*m_pPlayer;
	float		m_fDistance;
	float		m_fBackTime;
	float		m_fCntTime;
	float		m_fPlusDis;

	int			m_nTypeCounter;

};
#endif 