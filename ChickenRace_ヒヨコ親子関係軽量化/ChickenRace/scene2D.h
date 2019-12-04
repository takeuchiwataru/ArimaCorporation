//=============================================================================
//
// メイン処理 [scene2D.h]
// Author : 有馬武志
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TEXTURE	(30)

//=====================
//  CSceneの派生クラス
//=====================
class CScene2D : public CScene
{
public://誰でも扱える
	CScene2D(int nPriorityv = 5, CScene::OBJTYPE objType = CScene::OBJTYPE_2DPOLYGON);
	~CScene2D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindTexture(LPDIRECT3DTEXTURE9 Texture);
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetMove(void) { return m_Move; }
	D3DXVECTOR2 GetSize(void) { return m_nSize; }
	static CScene2D *Create(void);
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetPosSize(D3DXVECTOR3 pos, D3DXVECTOR2 nSize);
	void SetPosDiamond(D3DXVECTOR3 pos, D3DXVECTOR2 nSize, D3DXVECTOR4 VtxAngle);
	void SetRot(float fAngle, D3DXVECTOR2 size);
	void SetVtxPos(D3DXVECTOR3 *VxtPos);
	void SetColor(D3DXCOLOR *Color);
	void SetMove(D3DXVECTOR3 move) { m_Move = move; }
	void SetSpin(D3DXVECTOR3 pos, float fAngle, float fLength, D3DXVECTOR3 rot);
	void SetTexture(int PatternAnim, int X, int Y, int nNum);
	void SetPosSizeSide(D3DXVECTOR3 pos, D3DXVECTOR2 nSize);
	void SetUV(D3DXVECTOR2 LU, D3DXVECTOR2 RU, D3DXVECTOR2 LD, D3DXVECTOR2 RD);
	void SetUV(float fPattern, int nNum);
	void SetUV_X_ROLL(void);
	void SetUVWidth(float fPattern, int nNum);

private://個人でのみ使う
	LPDIRECT3DTEXTURE9		m_pTexture;						// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;						// 頂点バッファへのポインタ
	D3DXVECTOR3				m_pos;							// 位置
	D3DXVECTOR3				m_Move;							// 動き
	D3DXVECTOR3				m_rot;							// 向き
	float					m_fLength;						// 長さ
	float					m_fAngle;						// 角度
	float					m_fRadius;						// 半径
	D3DXVECTOR2				m_nSize;						// サイズ
};
#endif