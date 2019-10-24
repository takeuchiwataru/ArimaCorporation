//=============================================================================
//
// ゲームカメラ処理 [gamecamera.h]
// Author : 佐藤安純　
//
//=============================================================================
#ifndef _GAMECAMERA_H_
#define _GAMECAMERA_H_

#include "main.h"
#include "camerabace.h"

//=====================================================================
//　   前方宣言
//=====================================================================
class CPlayer;

//=====================================================================
//	   ゲームカメラクラス
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