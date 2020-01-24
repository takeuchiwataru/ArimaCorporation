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
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_CHICK_PARTS		(11)								// 読み込むパーツ数

#define FILE_NAME_CHICK		"data\\TEXT\\Player\\Chick.txt"	// 読み込むtxtファイルの名前
#define MAX_CHICK_ANIM		(8)									// アニメーション数
#define MAX_CHICK_KEY		(8)									// キーフレーム数
#define MAX_CHICK_MOTION	(10)								// モーションの最大数

#define CHICK_PRIOTITY		(3)									// ひよこの優先度
#define CHICK_RANGE			(50.0f)								// ひよことキャラクターの距離
#define CHICK_POS			(7)									// ひよこ同士の間隔の広さ（増やすと広くなる）
#define CHICK_BOOST			(2.0f)								// 加速する量
#define MAX_CHICK			(3)									// ひよこの最大数
#define MAX_SPEED_CHICK		(8)									// ひよこの数

#define DISTIME					(100)		// 消えるまでの時間
#define ANNOY_RANGE				(70.0f)		// 減速させる範囲
#define ATTACK_TIME				(3)		// 隕石ひよこが落ちるまでの時間
//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CObjBillboad;
class CPlayer;
class CModel;
class CCylinder;

//=====================
//  CModel3Dの派生クラス
//=====================
class CChick : public CScene
{
public:
	//キー要素
	typedef struct
	{
		float fposX;
		float fposY;
		float fposZ;
		float frotX;
		float frotY;
		float frotZ;
	}KEY;

	//キー情報
	typedef struct
	{
		int nFrame;
		KEY aKey[MAX_CHICK_PARTS];
	}KEY_INFO;

	//モーション情報
	typedef struct
	{
		bool bLoop;
		int nNumKey;
		KEY_INFO aKayInfo[MAX_CHICK_MOTION];
	}MOTION_INFO;

	//モーション種類
	typedef enum
	{
		CHICK_ANIM_NEUTRAL = 0,		//ニュートラルモーション
		CHICK_ANIM_RUN,				//走る
		CHICK_MAX					//モーションの最大数
	}ChickAnim;

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
	static void Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CChick *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type, BULLETTYPE bulletType, STATE state, int nNumPlayer);
	bool CollisionChick(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld);
	float SetHeight(void);
	void NewEfc(void);
	void DeleteEfc(void);
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
	bool GetSpeedS(void) { return m_bSpeedS; }
	void SetSpeedS(bool bSpeedS) { m_bSpeedS = bSpeedS; }
	void SetDestRank(int nDestRank) { m_DestRank = nDestRank; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	float GetHeight(void) { return m_fHeight; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetAttackCol(bool bAttackCol) { m_bAttackCol = bAttackCol; }
	bool GetAttackCol(void) { return m_bAttackCol; }

	//モーションの更新関数
	void UpdateMotion(void);

	//ファイル読み込み関数
	static void FileLoad(void);								//ファイル読み込み
	static char *ReadLine(FILE *pFile, char *pDst);			//1行読み込み
	static char *GetLineTop(char *pStr);					//行の先頭を取得
	static int  PopString(char *pStr, char *pDest);			//行の最後を切り捨て

private:
	bool Move(void);
	void Attack(void);
	void AnnoyS(void);
	void AttackS(void);
	bool Item(void);
	void Speed(void);
	void SpeedS(void);
	void Annoy(void);
	void CancelMotion(ChickAnim Anim, bool bRow);
	float AdjustAngle(float rot);
	D3DXMATRIX			m_mtxWorld;						// ワールドマトリックス
	static D3DXVECTOR3	m_VtxMaxModel[MAX_CHICK];		//モデルの最大値
	static D3DXVECTOR3	m_VtxMinModel[MAX_CHICK];		//モデルの最小値
	BULLETTYPE			m_bulletType;					// 弾の種類
	TYPE				m_type;							// ひよこの種類
	STATE				m_state;						// ひよこの状態
	D3DXVECTOR3			m_scale;						// 大きさ
	D3DXVECTOR3			m_rot;							// 回転
	CShadow				*m_pShadow;						// 影のポインタ
	CObjBillboad *		m_pObjBill;						// オブジェクトビルボードのポインタ
	D3DXVECTOR3			m_move;
	D3DXVECTOR3			m_pos;
	D3DXVECTOR3			m_posOld;
	D3DXVECTOR3			m_fDestAngle;					// 目的の角度
	D3DXVECTOR3			m_fDiffAngle;					// 差分
	D3DXVECTOR3			m_FNor;							//地面の法線
	D3DXVECTOR3			m_DestPos;					// 目的の位置
	int					m_nRank;						// 向かっていく順位
	int					m_nNumPlayer;					// プレイヤー何が持っているひよこか
	int					m_nDisTimer;					// 消すまでの時間
	int					m_DestRank;						// 目的の順位
	int					m_nMap;							// 判定を取るマップ
	int					m_nExplosion;					// 爆発している時間
	int					m_nCntUpDown;
	int					m_nCntAttackTime;				// 隕石が落ちるまでの時間
	float				m_fHeight;
	float				m_fLength;						// 横幅
	float				m_fUpDown;				// スピードアップひよこの上下
	float				m_fRangePos;					// 位置保存用
	float				m_fRange;
	float				m_fDisTime;						// 消えるまでの時間(ながやま修正12/17)
	bool				m_bJump;						// ジャンプしているかどうか
	bool				m_bDis;							// 消すかどうか
	bool				m_bAttackS;
	bool				m_bExplosion;					// 範囲攻撃になるかならないか
	bool				m_bHeight;
	bool				m_bJumpOk;
	bool				m_bSpeedS;
	bool				m_bAttackCol;

	// モーション関数	新規		
	CCylinder			*m_pClyinder;
	CModel				**m_apModel;	// パーツモデルのポインタ
	ChickAnim			m_nAnimnow;						// 現在のアニメーション
	static KEY_INFO		*m_pKeyInfo[MAX_CHICK_MOTION];	// キー情報へのポインタ
	int					m_nKey;							// 現在のキーナンバー
	int					m_nCountFlame;					// フレーム数
	static int			m_nNumParts;					// パーツ数
	static int			m_aIndexParent[MAX_CHICK_PARTS];// 親のインデックス
	static KEY			m_aKayOffset[MAX_CHICK_PARTS];	// オフセット情報
	static MOTION_INFO	m_aMotionInfo[MAX_CHICK_MOTION];// モーション情報
	int					m_nMotionType;					// モーションのタイプ(int型)
	bool				m_bMotionEnd;					// モーション終了
	float				m_fCola[MAX_PLAYCOL];			//キャラ分α値保存
};
#endif