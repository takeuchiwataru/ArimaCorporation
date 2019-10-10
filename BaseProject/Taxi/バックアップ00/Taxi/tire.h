//=============================================================================
//
// タイヤの処理 [model.h]
// Author : 佐藤安純　SatoAsumi
//
//=============================================================================
#ifndef _TIRE_H_
#define _TIRE_H_

#include "main.h"
#include "model.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CIncline;
class CPlayer;

//=====================
//  CTire 単独クラス
//=====================
class CTire : public CModel
{
public://誰でも扱える
	CTire();
	~CTire();
	HRESULT Init(void);
	void Uninit(void);
	void Update(int nCntTire, float fGravity);
	void Draw(float fAlpha);
	static CTire * Create(const D3DXVECTOR3 pos);
	static void LoadTexture(void);
	static void UnloadTexture(void);

	D3DXVECTOR3 GetWorldPos(void) { return m_WorldPos; };	//タイヤのワールド座標の取得
	bool GetLand(void) { return m_bLand; };					//地面に乗っているかどうか

	//定数
	const float TIRE_HIGHT = 18.0f;

private:
	void RemakeAngle(float * pAngle);
	void UpdateField(CPlayer * pPlayer);
	void SetIncline(CPlayer * pPlayer);

	static LPDIRECT3DTEXTURE9	m_pTexture;		// テクスチャのポインタ
	static float				m_fHight;		// タイヤの高さ
	static bool					m_bJump;		// ジャンプ状態かどうか
	static bool					m_bPass;		// 処理判定フラグ
	D3DXVECTOR3					m_move;			// 移動量
	D3DXVECTOR3					m_WorldPos;		// ワールド座標
	D3DXVECTOR3					m_WorldRot;		// ワールド回転
	CIncline *					m_pIncline;		// 傾斜処理のポインタ変数
	bool						m_bLand;		// 地面に乗っている状態かどうか
	float						m_fvtxMaxY;		// モデル頂点の最大値
};	
#endif