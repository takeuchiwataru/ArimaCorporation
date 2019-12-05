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
#define EGG_PRIOTITY	(0)
#define EGG_SCALE		(1.0f)										//卵の大きさ
#define EGG_POS			(7)											// 卵同士の間隔の広さ（増やすと広くなる）
#define SPEED			(0.5f)										// 加速する量
#define MAX_EGG			(3)											//卵の最大数
#define MAX_EGG_TEXTURE		(3)										// オブジェクトのテクスチャ数
#define HATCHING_TIME	(600)										// 孵化するまでの時間
#define EXPLOSION_TIME	(30)		// 爆発している時間

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
	} EGGTYPE;

	typedef enum
	{// 卵の状態
		EGGSTATE_NORMAL = 0,	// 通常
		EGGSTATE_CHASE,			// ついていく
		EGGSTATE_BULLET,		// 飛んでいく
		EGGSTATE_MAX,			//最大数
	} EGGSTATE;

	typedef enum
	{// 卵の状態
		BULLETTYPE_NONE = 0,
		BULLETTYPE_PLAYER,
		BULLETTYPE_ENEMY,
		BULLETTYPE_MAX,
	} BULLETTYPE;

	CEgg();
	~CEgg();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CEgg *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, EGGTYPE eggType, BULLETTYPE bulletType, int nNumPlayer);
	bool CollisionEgg(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld);
	EGGSTATE GetState(void) { return m_eggState; }
	void SetState(EGGSTATE state) { m_eggState = state; }
	EGGTYPE GetType(void) { return m_eggType; }
	BULLETTYPE GetBulletType(void) { return m_bulletType; }
	int GetNumPlayer(void) { return m_nNumPlayer; }
	void SetRank(int nRank) { m_nRank = nRank; }
	int GetRank(void) { return m_nRank; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	int GetHatchingTimer(void) { return m_nHatchingTimer; }
	void SetHatchingTimer(int nTimer) { m_nHatchingTimer = nTimer; }
	void Jump(float fJump);
	void Bullet(void);
	void SetThrow(bool bThrow) { m_bThrow = bThrow; }
	float GetHeight(void) { return m_fHeight; }

private:
	void Item(void);
	void Move(void);
	float AdjustAngle(float rot);
	static D3DXVECTOR3 m_VtxMaxModel[MAX_EGG];	//モデルの最大値
	static D3DXVECTOR3 m_VtxMinModel[MAX_EGG];	//モデルの最小値

	BULLETTYPE			m_bulletType;			// 弾の種類
	EGGTYPE				m_eggType;				// 卵の種類
	EGGSTATE			m_eggState;				// 卵の状態
	D3DXVECTOR3			m_scale;				// 大きさ
	D3DXVECTOR3			m_rot;					// 回転
	CShadow				*m_pShadow;				// 影のポインタ
	CObjBillboad *		m_pObjBill;				// オブジェクトビルボードのポインタ
	float				m_fHeight;
	D3DXVECTOR3			m_move;
	D3DXVECTOR3			m_pos;
	D3DXVECTOR3			m_posOld;
	D3DXVECTOR3			m_FNor;					//地面の法線
	bool				m_bJump;				// ジャンプしているかどうか
	bool				m_bThrow;				// 投げたかどうか
	bool				m_bExplosion;			// 範囲攻撃になるかならないか
	float				m_fDestAngle;			// 目的の角度
	float				m_fDiffAngle;			// 差分
	float				m_fLength;				// 横幅
	int					m_nRank;				// 向かっていく順位
	int					m_nNumPlayer;			// プレイヤー何が持っている卵か
	int					m_nHatchingTimer;		// 孵化するまでの時間
	int					m_nExplosion;			// 爆発している時間
	int					m_nMap;					//判定を取るマップ
};
#endif