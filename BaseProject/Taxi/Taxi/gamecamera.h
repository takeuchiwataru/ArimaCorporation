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
	CGameCamera();
	~CGameCamera();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera();

	void SetPlayer(CPlayer *pPlayer) { m_pPlayer = pPlayer; }

private:
	void UpdateNormal(void);
	void DrawReset(void);
	
	CPlayer		*m_pPlayer;
};
#endif