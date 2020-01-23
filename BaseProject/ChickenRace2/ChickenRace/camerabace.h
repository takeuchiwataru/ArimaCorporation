//=============================================================================
//
// カメラベース処理 [camerabace.h]
// Author : 佐藤安純
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"
//=====================
//	   マクロ
//=====================
#define OVER_DIS		(1000.0f)
#define IGNOR_SIZE		(2000.0f)

//=====================
//	   カメラクラス
//=====================
class CCamera
{
public:

	CCamera();
	~CCamera();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera();
	float GetRot(void) { return m_rot.y; }
	void SetCameraPosV(D3DXVECTOR3 posV) { m_posV = posV; }
	void SetCameraPosR(D3DXVECTOR3 posR) { m_posR = posR; }
	D3DXVECTOR3 GetCameraPosV(void) { return m_posV; }
	D3DXVECTOR3 GetCameraPosR(void) { return m_posR; }
	bool Clipping(D3DXVECTOR3 &pos, D3DXVECTOR3 VtxMin, D3DXVECTOR3 VtxMax);				//カメラに写っているかどうかの判定
	bool ClippingPos(D3DXVECTOR3 ObjPos);									//中心点がカメラに写っているかどうかの判定
	D3DXMATRIX GetViewMatrix(void) { return m_mtxView; }				//ビューマトリックスの取得
	D3DXMATRIX GetProjectionMatrix(void) { return m_mtxProjection; }	//プロジェクションマトリックスの取得
	float &GetfCameraAngle(void) { return m_fCameraAngle; }	//プロジェクションマトリックスの取得

	// ビューポート設定
	void SetViewPort(DWORD X, DWORD Y, DWORD Width, DWORD Height);

	void SetColl(bool bColl) { m_bCollCamera = bColl; }

protected:
	D3DXVECTOR3		m_posV;				//視点
	D3DXVECTOR3		m_posR;				//注視点
	D3DXVECTOR3		m_posVDest;			//目的の視点
	D3DXVECTOR3		m_posRDest;			//目的の注視点
	D3DXVECTOR3		m_rot;				//カメラの向きを入れる
	float			m_fCameraAngle;		//角度
	float			m_fRotDest;			//目的の角度
	D3DXVECTOR3		m_vecU;				//上方向ベクトル

	void RemakeAngle(float * pAngle);
	
	//定数
	const D3DXVECTOR3 VECTOR_ZERO = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

private:
	D3DXMATRIX		m_mtxProjection;	//プロジェクションマトリックス
	D3DXMATRIX		m_mtxView;			//ビューマトリックス
	D3DVIEWPORT9	m_viewport;			// ビューポート
	bool			m_bCollCamera;		// カメラ判定

};

#endif