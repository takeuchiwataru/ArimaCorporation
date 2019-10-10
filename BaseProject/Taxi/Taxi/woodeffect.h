//=============================================================================
//
// 木のエフェクト処理 [woodeffect.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _WOODEFFECT_H_
#define _WOODEFFECT_H_

#include "main.h"
#include "3Dparticle.h"

//=============================================================================
// ゲームの季節エフェクトクラス
//=============================================================================
class CWoodEffect : C3DParticle
{
public:
	CWoodEffect();
	~CWoodEffect();

	static CWoodEffect * Create(D3DXVECTOR3 Pos);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture) { C3DParticle::BindTexture(pTexture); }
	void SetDivision(D3DXVECTOR2 Division) { C3DParticle::SetDivision(Division); }
	void SetUV(int nWidth, int nHight) { C3DParticle::SetUV(nWidth, nHight); }

private:
	D3DXVECTOR3 m_move;			//移動量
	float		m_fAngle;		//角度
	float		m_fAimAngle;	//目的の角度
	float		m_fSpeed;		//速度
	float		m_fMoveRot;		//移動角度
	int			m_nLife;		//表示時間
	float		m_fCol_a;		//カラーのα値
};
#endif
