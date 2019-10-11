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
private:
	void UpdateNormal(void);
	void DrawReset(void);

	D3DXVECTOR3 m_DestPosV;		//目的の位置
	D3DXVECTOR3 m_Move;			//移動量
	float		m_fOldRotY;		//前回の向き
	float		m_fAngle;		//角度
	float		m_fHoldAngle;	//保存する角度
	float		m_fAddDirecting;
	bool		m_bSet;			//設定したかどうか
	bool		m_bSetAngle;	//角度を設定したかどうか
};
#endif