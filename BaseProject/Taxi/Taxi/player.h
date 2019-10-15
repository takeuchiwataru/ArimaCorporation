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

//=============================================================================
// 前方宣言
//=============================================================================
class CModel;
class CLoadTextPlayer;
class CTire;
class CLoadEffect;
class CCombo;
class CSound;

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

	//テキスト情報
	typedef struct
	{
		float fAccel;			//加速値
		float fBraks;			//減速値
		float fAddRot;			//加える回転値
		float fWindy;			//風圧
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
	static CPlayer * Create(const D3DXVECTOR3 pos);
	static void LoadModel(void);
	static void UnloadModel(void);

	static D3DXVECTOR3 GetPos(void) { return m_pos; };
	static void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };

	D3DXVECTOR3 GetOldPos(void) { return m_OldPos; };

	D3DXVECTOR3 GetMove(void) { return m_move; };
	void SetMove(D3DXVECTOR3 move) { m_move = move; };

	D3DXVECTOR3 GetRot(void) { return m_rot; };
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };

	PLAYER_INFO GetPlayerInfo(void) { return m_PlayerInfo; };
	PLAYER_INFO * GetPlayerInfoPoint(void) { return &m_PlayerInfo; };
	STATE_SPEED GetStateSpeed(void) { return m_StateSpeed; };
	STATE_HANDLE GetStateHandle(void) { return m_StateHandle; };
	STATE GetState(void) { return m_state; };

	void CollisitionWall(void);
	void LoadText(void);

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
	void CreateTire(void);
	void UpdateShake(void);
	void SetState(STATE state);
	void SetStateSpeed(STATE_SPEED state);
	void SetStateHandle(STATE_HANDLE state) { m_StateHandle = state; };
	void CollisionObject(void);
	void CollisionFeed(void);
	void CarCalculate(D3DXVECTOR3 * TirePos);
	void UpdateStateJump(void);
	void PlaySoundObj(int nType, CSound * pSound);

	static CModel *		m_pModel;		//パーツモデルのポインタ
	static int				m_nMaxModel;	//読み込むモデルの最大数
	static int				m_nMaxParts;	//読み込むパーツの最大数

	//メンバ変数
	static int	m_nMaxMotion;							// モーションの最大数
	static CMotion::MOTION_INFO * m_pMotionInfo;		// モーション情報
	static LPDIRECT3DTEXTURE9     m_pTexture;			// テクスチャ
	static D3DXVECTOR3			  m_pos;				// 中心座標
	D3DXVECTOR3					  m_move;				// 移動
	D3DXVECTOR3					  m_MoveMotion;			// モーションの移動量
	D3DXVECTOR3					  m_OldPos;				// 前回の座標
	D3DXVECTOR3					  m_OldDiffuse;			// 前回の差分
	D3DXVECTOR3					  m_rot;				// 向き
	D3DXVECTOR3					  m_vtxMaxModel;		// モデルの頂点最大値
	D3DXVECTOR3					  m_vtxMinModel;		// モデルの頂点最小値
	D3DXMATRIX					  m_mtxWorld;			// ワールドマトリックス
	CMotion *					  m_pMotion;			// モーションのポインタ
	STATE						  m_state;				// 状態設定
	STATE_SPEED					  m_StateSpeed;			// スピードの状態
	STATE_HANDLE				  m_StateHandle;		// ハンドルの状態
	PLAYER_INFO					  m_PlayerInfo;			// プレイヤーの情報
	CLoadTextPlayer *			  m_pText;				// プレイヤーの情報読み込み
	CTire *						  m_pTire[MAX_TIRE];	// タイヤのポインタ
	bool						  m_bJump;				//  ジャンプフラグ
	bool						  m_bControl;			// コントローラーの使用状態
	int							  m_nCountJumpTime;		// ジャンプ状態の時間をカウントする
	float						  m_fvtxMaxY;			// モデル頂点の最大値（Y）
	float						  m_fMaxSpeed;			// スピードの最大値
	float						  m_fMass;				// 質量
	int							  m_nCountTime;			// 時間の加算
	bool						  m_bCrcleCarIn;		// 車が範囲内に入っているかどうか
	CLoadEffect *				  m_pLoadEffect;		// ロードエフェクトのポインタ変数
	int							  m_nCntCombo;
	int							  m_nCntShake;			//揺れの時間をカウントする
	bool						  m_bShake;				//揺れのオンオフ
	bool						  m_bDirive;			//前進、後退の操作フラグ
	int							  m_nCntFlag;
};
#endif
