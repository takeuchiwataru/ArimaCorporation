//=============================================================================
//
// ���U���g�J�������� [resultcamera.h]
// Author : shun yokomichi
//
//=============================================================================
#ifndef _RESULTCAMERA_H_
#define _RESULTCAMERA_H_

#include "main.h"
#include "camerabace.h"

//=====================================================================
//�@   �O���錾
//=====================================================================

//=====================================================================
//�@   ���U���g�J�����N���X
//=====================================================================
class CResultCamera : public CCamera
{
public:
	CResultCamera();
	~CResultCamera();

	HRESULT Init(void);
	void Uninit(void);
	void Updata(void);
	void SetCamera();

private:
};
#endif 