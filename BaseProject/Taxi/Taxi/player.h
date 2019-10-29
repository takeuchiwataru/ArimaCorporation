//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _MOTIONPLAYER_H_
#define _MOTIONPLAYER_H_

#include "main.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "motion.h"
#include "scene.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_PARTS	(7)		//読み込むパーツ数
#define MAX_LIFE	(15)	//ライフ
#define MAX_TIRE	(4)		//タイヤの最大数
#define MAX_EGG		(3)		//卵の最大数
#define MAX_FRAME	(60)

#define DAMAGE_TIME	(60)		// ダメージを食らっている時間
#define SPEEDUP_TIME	(60)	// 加速している時間
#define SPEEDDOWN_TIME	(300)	// 減速している時間
#define MAX_EGG		(3)		//卵の最大数

//=============================================================================
// 前方宣言
//=============================================================================
class CModel;
class CLoadTextPlayer;
class CLoadEffect;
class CCombo;
class CSound;
class CFeed;
class CEgg;
class CChick;
class CBillBoord;

//=============================================================================
// プレイヤークラス
//=============================================================================
class CPlayer : public CScene
{
public:
	//ギアの状態
	typedef enum
	{
		STATE_DRIVE = 0,
		STATE_REVERSE,
		STATE_MAX,
	}STATE;

	//走行状態
	typedef enum
	{
		STATE_SPEED_ACCEL = 0,
		STATE_SPEED_BRAKS,
		STATE_SPEED_DOWN,
		STATE_SPEED_STOP,
	}STATE_SPEED;

	//ハンドルの状態
	typedef enum
	{
		HANDLE_LEFT = 0,
		HANDLE_RIGHT,
		HANDLE_MAX,
	}STATE_HANDLE;

	typedef enum
	{// プレイヤーの状態
		PLAYERSTATE_NORMAL = 0,	// 通常
		PLAYERSTATE_SPEEDUP,		// 加速
		PLAYERSTATE_SPEEDDOWN,	// 減速
		PLAYERSTATE_DAMAGE,		// 攻撃食らう
		PLAYERSTATE_MAX,			//最大数
	} PLAYERSTATE;

	//テキスト情報
	typedef struct
	{
		float fAccel;			//加速値（前進）
		float fBraks;			//加速値（後進）
		float fDown;			//減速値
		float fAddRot;			//加える回転値
		float fDistance;		//移動距離
		float nCountTime;		//時間の計算
		D3DXVECTOR3 FirstPos;	//初期位置
	}PLAYER_INFO;

	//色の状態の種類
	CPlayer();
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CPlayer * Create(const D3DXVECTOR3 pos, int nPlayerNum = 0, int nControllerNum = 0);
	static void LoadModel(void);
	static void UnloadModel(void);

	D3DXVECTOR3 GetPos(void) { return m_pos; };
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };

	D3DXVECTOR3 GetOldPos(void) { return m_OldPos; };

	D3DXVECTOR3 GetMove(void) { return m_move; };
	void SetMove(D3DXVECTOR3 move) { m_move = move; };

	D3DXVECTOR3 GetRot(void) { return m_rot; };
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };

	PLAYER_INFO GetPlayerInfo(void) { return m_PlayerInfo; };
	PLAYER_INFO * GetPlayerInfoPoint(void) { return &m_PlayerInfo; };
	STATE_SPEED GetStateSpeed(void) { return m_StateSpeed; };
	STATE_HANDLE GetStateHandle(void) { return m_StateHandle; };
	STATE GetState(void) { return m_MoveState; };

	void CollisitionWall(void);
	void CollisionEgg(void);

	void SetControl(bool bControl) { m_bControl = bControl; };
	bool GetControl(void) { return m_bControl; };

	void SetJump(bool bJump) { m_bJump = bJump; }
	bool GetJump(void) { return m_bJump; };
	void SetCrcleCarIn(bool bCrcleCarIn) { m_bCrcleCarIn = bCrcleCarIn; }
	bool GetCrcleCarIn(void) { return m_bCrcleCarIn; }
	bool GetShake(void) { return m_bShake; }
	bool GetDrive(void) { return m_bDirive; }
	void SetDrive(bool bDrive) { m_bDirive = bDrive; }

private:
	//変数宣言
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void RemakeAngle(float * pAngle);
	void RemakeCarRot(float * pAngle);
	float GetLength(D3DXVECTOR3 StartPos, D3DXVECTOR3 EndPos);
	void UpdateMove(void);
	void ControlKey(void);
	void DebugProc(void);
	void UpdateShake(void);
	void UpdateField(void);
	void SetState(STATE state);
	void SetStateSpeed(STATE_SPEED state);
	void SetStateHandle(STATE_HANDLE state) { m_StateHandle = state; };
	void CollisionObject(void);
	void CollisionFeed(void);
	void CarCalculate(D3DXVECTOR3 * TirePos);
	void UpdateStateJump(void);
	void PlaySoundObj(int nType, CSound * pSound);
	void EggAppear(CFeed *pFeed);
	void ChickAppear(void);
	void ChaseEgg(void);
	void BulletEgg(void);

	static CModel *		m_pModel;			//パーツモデルのポインタ
	static int				m_nMaxModel;	//読み込むモデルの最大数
	static int				m_nMaxParts;	//読み込むパーツの最大数

											//メンバ変数
	static int					  m_nMaxMotion;			// モーションの最大数
	static CMotion::MOTION_INFO * m_pMotionInfo;		// モーション情報
	static LPDIRECT3DTEXTURE9     m_pTexture;			// テクスチャ
	D3DXVECTOR3				      m_pos;				// 中心座標
	D3DXVECTOR3					  m_move;				// 移動
	D3DXVECTOR3					  m_MoveMotion;			// モーションの移動量
	D3DXVECTOR3					  m_OldPos;				// 前回の座標
	D3DXVECTOR3					  m_OldDiffuse;			// 前回の差分
	D3DXVECTOR3					  m_rot;				// 向き
	D3DXVECTOR3					  m_OldEggRot[MAX_FRAME];	// 前回の位置
	D3DXVECTOR3					  m_vtxMaxModel;		// モデルの頂点最大値
	D3DXVECTOR3					  m_vtxMinModel;		// モデルの頂点最小値
	D3DXMATRIX					  m_mtxWorld;			// ワールドマトリックス
	CMotion *					  m_pMotion;			// モーションのポインタ
	STATE						  m_MoveState;			// 状態設定
	STATE_SPEED					  m_StateSpeed;			// スピードの状態
	STATE_HANDLE				  m_StateHandle;		// ハンドルの状態
	PLAYER_INFO					  m_PlayerInfo;			// プレイヤーの情報
	CLoadTextPlayer *			  m_pText;				// プレイヤーの情報読み込み
	CEgg						  *m_pEgg[MAX_EGG];		//卵のポインタ
	CChick						  *m_pChick[MAX_EGG];	//ひよこのポインタ
	bool						  m_bJump;				//  ジャンプフラグ
	bool						  m_bControl;			// コントローラーの使用状態
	int							  m_nCountJumpTime;		// ジャンプ状態の時間をカウントする
	float						  m_fvtxMaxY;			// モデル頂点の最大値（Y）
	float						  m_fMaxSpeed;			// スピードの最大値
	float						  m_fMass;				// 質量
	float						  m_fSpeed;				// 速さ
	int							  m_nCountTime;			// 時間の加算
	int							  m_nCountSpeed;		// 時間の加算
	bool						  m_bCrcleCarIn;		// 車が範囲内に入っているかどうか
	CLoadEffect *				  m_pLoadEffect;		// ロードエフェクトのポインタ変数
	int							  m_nCntCombo;
	int							  m_nCntShake;			//揺れの時間をカウントする
	bool						  m_bShake;				//揺れのオンオフ
	bool						  m_bDirive;			//前進、後退の操作フラグ
	int							  m_nCntFlag;
	int							  m_nNumEgg;
	int							  m_nNumChick;
	int							  m_nCntFrame;			// 卵のついてくる処理に使う
	bool						  m_abJump[MAX_FRAME];
	bool						  m_bJumpSave;

	bool						  m_bDamage;
	PLAYERSTATE					  m_State;
	int							  m_nCntDamage;
	int							  m_nPlayerNum;			// プレイヤー番号
	int							  m_nControllerNum;		// コントローラー番号

	CBillBoord					  *m_pPlayerNum;		// プレイヤー番号（追従）
};
#endif
