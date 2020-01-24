//=============================================================================
//
// オブジェクト処理 [object.h]
// Author : 有馬武志
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "model3D.h"
#include "camerabace.h"
#include "shadow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_OBJECT					(7)				// オブジェクトのモデル数
#define MAX_OBJECT_TEXTURE			(7)				// オブジェクトのテクスチャ数
#define OBJECT_PRIOTITY				(3)				// オブジェクトの優先順位
#define PLAYER_HEIGHT			(80.0f)				// プレイヤーの背の高さ
#define PLAYER_DEPTH			(20.0f)				// プレイヤーの幅調整用

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CLoadEffect;
class CObjBillboad;

//=====================
//  CModel3Dの派生クラス
//=====================
class CObject : public CModel3D
{
public://誰でも扱える
	CObject(int nPriority = OBJECT_PRIOTITY);
	~CObject();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CObject *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, float move, int nTexTypeint, int nObjectType, int nCollision);
	void DeleteObject(void);
	int GetType(void) { return m_nType; };
	D3DXVECTOR3 Fountain(D3DXVECTOR3 pos, D3DXVECTOR3 move);
	bool CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);

private://個人でのみ使う
	static D3DXVECTOR3 m_LoadVtxMaxModel[MAX_OBJECT];				//モデルの最大値
	static D3DXVECTOR3 m_LoadVtxMinModel[MAX_OBJECT];				//モデルの最小値

	D3DXVECTOR3			m_scale;									// サイズ
	D3DXVECTOR3			m_Spin;										// 回転
	int					m_nCount;									// カウンター
	int					m_nCollision;								// 当たり判定のONOFF
	int					m_nType;									// 種類
	int					m_nTex;										// テクスチャ番号
	D3DXVECTOR3			m_ModelMove;								// 移動の仕方
	int					m_nStageCount;								// ステージの移動時に少し時間をかける
	bool				m_bStageNext;								// ステージ遷移中
	CShadow				*m_pShadow;									// 影のポインタ
	CObjBillboad *		m_pObjBill;									// オブジェクトビルボードのポインタ

	float				m_fRoll;									// 回転
};
#endif