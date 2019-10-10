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
class CCustomer;
class CStartSignal;

//=====================================================================
//	   ゲームカメラクラス
//=====================================================================
class CGameCamera : public CCamera
{
public:
	typedef enum
	{//カメラの状態
		MODE_NORMAL = 0,	//通常状態
		MODE_PUTON,			//乗車時
		MODE_TAKEDOEN,		//降車時
		MODE_START,			//スタートカメラ
	}MODECAMERA;

	typedef enum MyEnum
	{	// ゲーム開始時演出
		DIRECTING_NONE = 0,	// 何もない
		DIRECTING_000,		// 最初
		DIRECTING_001,		// 2番目
		DIRECTING_002,		// 3番目
		DIRECTING_END,		// 終了
	}DIRECTING;
	
	CGameCamera();
	~CGameCamera();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera();
	void SetModeCamera(MODECAMERA mode);
	MODECAMERA GetModeCamera(void) { return m_mode; };
	void SetCustomerInfo(CCustomer * pCustomer) { m_pCustomer = pCustomer; };
	void UninitSignal(void);
	DIRECTING GetDirecting(void) { return m_Directing; }
private:
	void UpdateNormal(void);
	void UpdatePutOn(void);
	void UpdateTakeDown(void);
	void UpdateStart(void);
	void DrawReset(void);
	void StartSkip(void);

	MODECAMERA m_mode;			//カメラの状態
	MODECAMERA m_oldmode;		//前回の状態
	DIRECTING  m_Directing;		//演出の状態
	CCustomer * m_pCustomer;	//お客さんのポインタ
	CStartSignal* m_pStartSignal;	// スタート表示のポインタ
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