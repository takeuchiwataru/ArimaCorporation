//=============================================================================
//
// ひよこの処理 [chick.h]
// Author : 長山拓実
//
//=============================================================================
#ifndef _CHICK_H_
#define _CHICK_H_

#include "model3D.h"
#include "camerabace.h"
#include "shadow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CHICK_PRIOTITY				(3)
#define CHICK_RANGE		(50.0f)										// ひよことキャラクターの距離
#define CHICK_POS			(7)											// ひよこ同士の間隔の広さ（増やすと広くなる）
#define SPEED			(2.0f)										// 加速する量
#define MAX_CHICK		(3)												// ひよこの最大数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CObjBillboad;
class CPlayer;

//=====================
//  CModel3Dの派生クラス
//=====================
class CChick : public CModel3D
{
public:
	typedef enum
	{// ひよこの種類
		TYPE_ATTACK = 0,		// 攻撃
		TYPE_ANNOY,			// 妨害
		TYPE_SPEED,			// 加速
		TYPE_MAX,			//最大数
	} TYPE;

	typedef enum
	{// ひよこの状態
		STATE_NORMAL = 0,	// 通常
		STATE_CHASE,			// ついていく
		STATE_BULLET,		// 飛んでいく
		STATE_MAX,			//最大数
	} CHICKSTATE;

	typedef enum
	{// ひよこの状態
		BULLETTYPE_NONE = 0,
		BULLETTYPE_PLAYER,
		BULLETTYPE_ENEMY,
		BULLETTYPE_MAX,
	} BULLETTYPE;

	CChick();
	~CChick();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CChick *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type, BULLETTYPE bulletType, int nNumPlayer);
	bool CollisionChick(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld);
	float SetHeight(void);
	CHICKSTATE GetState(void) { return m_state; }
	void SetState(CHICKSTATE state) { m_state = state; }
	TYPE GetType(void) { return m_type; }
	BULLETTYPE GetBulletType(void) { return m_bulletType; }
	int GetNumPlayer(void) { return m_nNumPlayer; }
	void SetRank(int nRank) { m_nRank = nRank; }
	int GetRank(void) { return m_nRank; }
	void Jump(void);
	void Bullet(void);

private:
	static LPD3DXMESH	m_pMeshModel;			//メッシュ情報へのポインタ
	static LPD3DXBUFFER m_pBuffMatModel;		//マテリアルの情報へのポインタ
	static DWORD		m_nNumMatModel;			//マテリアルの情報数
	static LPDIRECT3DTEXTURE9 m_pMeshTextures;	//テクスチャ情報
	static D3DXVECTOR3 m_VtxMaxModel;			//モデルの最大値
	static D3DXVECTOR3 m_VtxMinModel;			//モデルの最小値

	BULLETTYPE			m_bulletType;			// 弾の種類
	TYPE				m_type;				// ひよこの種類
	CHICKSTATE			m_state;				// ひよこの状態
	D3DXVECTOR3			m_scale;				// 大きさ
	D3DXVECTOR3			m_rot;					// 回転
	CShadow				*m_pShadow;				// 影のポインタ
	CObjBillboad *		m_pObjBill;				// オブジェクトビルボードのポインタ
	float				m_fHeight;
	D3DXVECTOR3			m_move;
	bool				m_bJump;				// ジャンプしているかどうか
	float				m_fDestAngle;			// 目的の角度
	float				m_fDiffAngle;			// 差分
	int					m_nRank;				// 向かっていく順位
	int					m_nNumPlayer;			// プレイヤー何が持っているひよこか
};
#endif