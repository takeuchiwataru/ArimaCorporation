//=============================================================================
//
// 傾斜処理 [incline.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _INCLINE_H_
#define _INCLINE_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CMeshField;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CIncline
{
public:
	CIncline();
	~CIncline();
	HRESULT Init(void);
	void Uninit(void);
	void Update(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fSpeed);

	static CIncline* Create(void);

	CMeshField *GetOnField(void) { return m_pField; };						// 現在乗っている地面の取得
	void SetOnField(CMeshField *pField) { m_pField = pField; };				// 現在乗っている地面の設定

	float GetJumpHeight(void) { return m_fJumpHeight; };					// ジャンプの高さの取得
	void SetJumpHeight(float fJumpHeight) { m_fJumpHeight = fJumpHeight; };	// ジャンプの高さの設定

	float GetSpeed(void) { return m_fSpeed; };								// 速度の取得
	void SetSpeed(float fSpeed) { m_fSpeed = fSpeed; };						// 速度の設定

	bool GetJumpFlag(void) { return m_bJump; };								// ジャンプ状態の取得
	void SetJumpFlag(bool bJump) { m_bJump = bJump; };						// ジャンプ状態の設定

	float GetAngle(void) { return m_fAngle; };								// 向きと地面の交点の角度の取得
	void SetAngle(float fAngle) { m_fAngle = fAngle; };						// 向きと地面の交点の角度の設定

	float GetClimbSpeed(void) { return m_fClimbSpeed; };					// 傾斜を上るときの速度補正の取得
	void SetClimbSpeed(float fClimbSpeed) { m_fClimbSpeed = fClimbSpeed; };	// 傾斜を上るときの速度補正の設定

	D3DXMATRIX GetRot(D3DXVECTOR3 pos, D3DXMATRIX mtxRot);					// 傾斜に沿った角度の取得

private:
	CMeshField		*m_pField;			// 乗っている地面のポインタ
	float			m_fJumpHeight;		// ジャンプの高さ
	float			m_fSpeed;			// 速度
	bool			m_bJump;			// ジャンプフラグ
	float			m_fAngle;			// 向きと地面の交点の角度
	float			m_fClimbSpeed;		// 傾斜を上るときの速度補正
	D3DXVECTOR3		m_norField;			// 地面の法線
};

#endif