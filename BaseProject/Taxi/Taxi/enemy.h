//=============================================================================
//
// 敵の処理（仮） [enemy.h]
// Author : 長山拓実
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "model3D.h"
#include "camerabace.h"
#include "shadow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMY_PRIOTITY				(3)

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CObjBillboad;

//=====================
//  CModel3Dの派生クラス
//=====================
class CEnemy : public CModel3D
{
public:
	typedef enum
	{// 餌の種類
		ENEMYSTATE_NORMAL = 0,	// 通常
		ENEMYSTATE_SPEEDUP,		// 加速
		ENEMYSTATE_SPEEDDOWN,	// 減速
		ENEMYSTATE_DAMAGE,		// 攻撃食らう
		ENEMYSTATE_MAX,			//最大数
	} ENEMYSTATE;

	CEnemy();
	~CEnemy();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CEnemy *Create(D3DXVECTOR3 pos);
	void CollisionEgg(void);

private:
	static LPD3DXMESH	m_pMeshModel;		//メッシュ情報へのポインタ
	static LPD3DXBUFFER m_pBuffMatModel;	//マテリアルの情報へのポインタ
	static DWORD		m_nNumMatModel;	//マテリアルの情報数
	static LPDIRECT3DTEXTURE9 m_pMeshTextures;			//テクスチャ情報
	static D3DXVECTOR3 m_VtxMaxModel;		//モデルの最大値
	static D3DXVECTOR3 m_VtxMinModel;		//モデルの最小値

	D3DXVECTOR3			m_scale;						// 大きさ
	D3DXVECTOR3			m_rot;							// 回転
	D3DXVECTOR3			m_move;							// 移動量
	D3DXVECTOR3			m_pos;							// 位置
	D3DXVECTOR3			m_posOld;						// 前回の位置
	CShadow				*m_pShadow;						// 影のポインタ
	CObjBillboad *		m_pObjBill;						// オブジェクトビルボードのポインタ
	int					m_nCntMove;
	int					m_nCntDamage;
	ENEMYSTATE			m_State;
	float				m_fSpeed;
	bool				m_bSpeedDown;
};
#endif