//=============================================================================
//
// 卵の処理 [egg.h]
// Author : 長山拓実
//
//=============================================================================
#ifndef _EGG_H_
#define _EGG_H_

#include "model3D.h"
#include "camerabace.h"
#include "shadow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define EGG_PRIOTITY				(3)

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CObjBillboad;
class CPlayer;

//=====================
//  CModel3Dの派生クラス
//=====================
class CEgg : public CModel3D
{
public:
	typedef enum
	{// 卵の種類
		EGGTYPE_ATTACK = 0,		// 攻撃
		EGGTYPE_ANNOY,			// 妨害
		EGGTYPE_SPEED,			// 加速
		EGGTYPE_MAX,			//最大数
	}EGGTYPE;

	CEgg();
	~CEgg();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CEgg *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, EGGTYPE eggType);
	bool CollisionEgg(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld);
	float SetHeight(void);
	void Jump(void);

private:
	static LPD3DXMESH	m_pMeshModel;			//メッシュ情報へのポインタ
	static LPD3DXBUFFER m_pBuffMatModel;		//マテリアルの情報へのポインタ
	static DWORD		m_nNumMatModel;			//マテリアルの情報数
	static LPDIRECT3DTEXTURE9 m_pMeshTextures;	//テクスチャ情報
	static D3DXVECTOR3 m_VtxMaxModel;			//モデルの最大値
	static D3DXVECTOR3 m_VtxMinModel;			//モデルの最小値

	EGGTYPE				m_eggType;						// 卵の種類
	D3DXVECTOR3			m_scale;						// 大きさ
	D3DXVECTOR3			m_rot;							// 回転
	CShadow				*m_pShadow;						// 影のポインタ
	CObjBillboad *		m_pObjBill;						// オブジェクトビルボードのポインタ
	float				m_fHeight;
	D3DXVECTOR3			m_move;
	bool				m_bJump;				// ジャンプしているかどうか
};
#endif