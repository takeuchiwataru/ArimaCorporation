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
#include "model3D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CHICK_PRIOTITY				(3)
#define CHICK_RANGE		(50.0f)										// ひよことキャラクターの距離
#define CHICK_POS			(7)											// ひよこ同士の間隔の広さ（増やすと広くなる）
#define CHICK_BOOST			(2.0f)										// 加速する量
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
		TYPE_ATTACK = 0,	// 攻撃
		TYPE_ANNOY,			// 妨害
		TYPE_SPEED,			// 加速
		TYPE_ATTACK_S,		// 強い攻撃
		TYPE_ANNOY_S,		// 強い妨害
		TYPE_SPEED_S,		// 強い加速
		TYPE_MAX,			// 最大数
	} TYPE;

	typedef enum
	{// ひよこの状態
		STATE_NORMAL = 0,	// 通常
		STATE_CHASE,		// ついていく
		STATE_BULLET,		// 飛んでいく
		STATE_MAX,			// 最大数
	} STATE;

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
	static CChick *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type, BULLETTYPE bulletType, STATE state, int nNumPlayer);
	bool CollisionChick(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld);
	float SetHeight(void);
	STATE GetState(void) { return m_state; }
	void SetState(STATE state) { m_state = state; }
	TYPE GetType(void) { return m_type; }
	BULLETTYPE GetBulletType(void) { return m_bulletType; }
	int GetNumPlayer(void) { return m_nNumPlayer; }
	void SetRank(int nRank) { m_nRank = nRank; }
	int GetRank(void) { return m_nRank; }
	void SetDis(bool bDis) { m_bDis = bDis; }
	bool GetDis(void) { return m_bDis; }
	void Jump(float fJump);
	void Bullet(void);
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	bool GetAttackS(void) { return m_bAttackS; }
	void SetAttackS(bool bAttackS) { m_bAttackS = bAttackS; }
	void SetDestRank(int nDestRank) { m_DestRank = nDestRank; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	float GetHeight(void) { return m_fHeight; }

private:
	void Move(void);
	void Attack(void);
	void AnnoyS(void);
	void AttackS(void);
	void Item(void);
	float AdjustAngle(float rot);
	static D3DXVECTOR3 m_VtxMaxModel[MAX_CHICK];   //モデルの最大値
	static D3DXVECTOR3 m_VtxMinModel[MAX_CHICK];   //モデルの最小値
	BULLETTYPE			m_bulletType;			// 弾の種類
	TYPE				m_type;				// ひよこの種類
	STATE				m_state;				// ひよこの状態
	D3DXVECTOR3			m_scale;				// 大きさ
	D3DXVECTOR3			m_rot;					// 回転
	CShadow				*m_pShadow;				// 影のポインタ
	CObjBillboad *		m_pObjBill;				// オブジェクトビルボードのポインタ
	D3DXVECTOR3			m_move;
	D3DXVECTOR3			m_pos;
	D3DXVECTOR3			m_posOld;
	D3DXVECTOR3			m_fDestAngle;			// 目的の角度
	D3DXVECTOR3			m_fDiffAngle;			// 差分
	D3DXVECTOR3			m_FNor;					//地面の法線
	int					m_nRank;				// 向かっていく順位
	int					m_nNumPlayer;			// プレイヤー何が持っているひよこか
	int					m_nDisTimer;			// 消すまでの時間
	int					m_DestRank;				// 目的の順位
	int					m_nMap;					//判定を取るマップ
	int					m_nExplosion;			// 爆発している時間
	float				m_fHeight;
	float				m_fLength;				// 横幅
	bool				m_bJump;				// ジャンプしているかどうか
	bool				m_bDis;					// 消すかどうか
	bool				m_bAttackS;
	bool				m_bExplosion;			// 範囲攻撃になるかならないか
};
#endif