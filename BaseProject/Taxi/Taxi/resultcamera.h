//=============================================================================
//
// リザルトカメラ処理 [resultcamera.h]
// Author : shun yokomichi
//
//=============================================================================
#ifndef _RESULTCAMERA_H_
#define _RESULTCAMERA_H_

#include "main.h"
#include "camerabace.h"

//=====================================================================
//　   前方宣言
//=====================================================================

//=====================================================================
//　   リザルトカメラクラス
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