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
#include "RoadPointer.h"
#include "ColMesh.h"
#include "game.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_PARTS	(11)											//読み込むパーツ数
#define MAX_LIFE	(15)										//ライフ
#define MAX_TIRE	(4)											//タイヤの最大数
#define MAX_EGG		(3)											//卵の最大数
#define MAX_FRAME	(60)

#define DAMAGE_TIME	(60)										// ダメージを食らっている時間
#define SPEEDDOWN_TIME	(300)									// 減速している時間
#define FALL_CHICK_RANGE		(400)							// ひよこが降る範囲
#define CHICK_FALL_NUM			(5)								// 落ちてくるひよこの数
#define MAX_EGG		(3)											//卵の最大数

#define FILE_NAME_PRISONER		"data\\TEXT\\Player\\Player.txt"//読み込むtxtファイルの名前
#define MAX_PLAYERANIM			(8)								//アニメーション数
#define MAX_PLAYERKEY			(8)								//キーフレーム数
#define MAX_MOTION				(10)							//モーションの最大数


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
		KEY aKey[MAX_PARTS];
	}KEY_INFO;

	//モーション情報
	typedef struct
	{
		bool bLoop;
		int nNumKey;
		KEY_INFO aKayInfo[MAX_MOTION];
	}MOTION_INFO;

	//モーション種類
	typedef enum
	{
		PLAYERANIM_NEUTRAL = 0,		//ニュートラルモーション
		PLAYERANIM_RUN,				//走る
		PLALYER_MAX					//モーションの最大数
	}PlayerAnim;

	typedef enum
	{
		PLAYERTYPE_PLAYER = 0,
		PLAYERTYPE_ENEMY,
		PLAYERTYPE_MAX
	}PLAYERTYPE;

	//走行状態
	typedef enum
	{
		STATE_SPEED_ACCEL = 0,
		STATE_SPEED_BRAKS,
		STATE_SPEED_DRIFT,
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
		PLAYERSTATE_SPEEDUP_S,		// 自動加速
		PLAYERSTATE_SPEEDDOWN,	// 減速
		PLAYERSTATE_SPEEDDOWN_S,	// 全員減速
		PLAYERSTATE_DAMAGE,		// 攻撃食らう
		PLAYERSTATE_MAX,			//最大数
	} PLAYERSTATE;

	typedef enum
	{
		BULLET_EGG_ATTACK = 0,		// 卵（攻撃）
		BULLET_EGG_ANNOY,			// 卵（妨害）
		BULLET_EGG_SPEED,			// 卵（速度）
		BULLET_CHICK_ATTACK,		// ひよこ（攻撃）
		BULLET_CHICK_ANNOY,			// ひよこ（妨害）
		BULLET_CHICK_SPEED,			// ひよこ（速度）
		BULLET_MAX,
	}BULLET;

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
	static CPlayer * Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, int nPlayerNum = 0, int nControllerNum = 0, PLAYERTYPE playerType = PLAYERTYPE_PLAYER);
	static void LoadModel(void);
	static void UnloadModel(void);

	PLAYERTYPE GetPlayerType(void) { return m_PlayerType; }

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
	PLAYERSTATE GetPlayerState(void) { return m_State; }

	void CollisitionWall(void);

	void SetControl(bool bControl) { m_bControl = bControl; };
	bool GetControl(void) { return m_bControl; };
	int GetCntChick(void) { return m_nCntChick; }
	void SetCntChick(int nCntChick) { m_nCntChick = nCntChick; }
	CChick **GetAnnoyChick(void) { return m_pAnnoyChick; }
	void FallChicks(D3DXVECTOR3 pos);
	void AnnoyChicks(void);

	void SetJump(bool bJump) { m_bJump = bJump; }
	bool GetJump(void) { return m_bJump; };
	void SetCrcleCarIn(bool bCrcleCarIn) { m_bCrcleCarIn = bCrcleCarIn; }
	bool GetCrcleCarIn(void) { return m_bCrcleCarIn; }
	bool GetShake(void) { return m_bShake; }
	bool GetDrive(void) { return m_bDirive; }
	void SetDrive(bool bDrive) { m_bDirive = bDrive; }

	CCOL_MESH::EFFECT &GgetFEffect(void) { return m_FEffect; }
	CRoad_Pointer	*&GetpPoint(void) { return m_pPoint; }
	CRoad_Pointer	*&GetpEnmPoint(void) { return m_pEnmPoint; }
	D3DXVECTOR3		&GetFNor(void) { return m_FNor; };
	D3DXVECTOR3		&Getpos(void) { return m_pos; };
	D3DXVECTOR3		&Getposold(void) { return m_OldPos; };
	D3DXVECTOR3		&Getmove(void) { return m_move; };
	bool			&GetbJump(void) { return m_bJump; };
	float			&GetfLength(void) { return m_fLength; };
	float			&GetfTilt(void) { return m_fCTilt; };
	float			&GetfRotOld(void) { return m_fRotOld; };
	int				&GetnMap(void) { return m_nMap; };
	int				&GetnPlayerNum(void) { return m_nPlayerNum; }
	bool			&GetbDivided(void) { return m_bDivided; };

	int GetItemNum(void) { return m_nNumItem; }
	int GetItemType(int nNum) { return m_bulletType[nNum]; }

	bool GetGoal(void) { return m_bGoal; }

	//モーションの更新関数
	void UpdateMotion(void);

	//ファイル読み込み関数
	void FileLoad(void);								//ファイル読み込み
	char *ReadLine(FILE *pFile, char *pDst);			//1行読み込み
	char *GetLineTop(char *pStr);						//行の先頭を取得
	int  PopString(char *pStr, char *pDest);			//行の最後を切り捨て

private:
	static LPD3DXMESH	m_pMesh[MAX_PARTS];				//メッシュ情報へのポインタ
	static LPD3DXBUFFER	m_pBuffMat[MAX_PARTS];			//マテリアルの情報へのポインタ
	static DWORD		m_nNumMat[MAX_PARTS];			//マテリアルの情報数

	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void RemakeAngle(float * pAngle);
	void RemakeCarRot(float * pAngle);
	float GetLength(D3DXVECTOR3 StartPos, D3DXVECTOR3 EndPos);
	void UpdateMove(void);
	void ControlKey(void);
	void DebugProc(void);

	void UpdateAI(void);
	void UseItem(void);
	bool UseATK(int &nRank);
	bool UseDEF(int &nRank);
	bool UseSPD(int &nRank);
	float GetDistance(int nRank);
	void UpdateKiller(void);
	void UpdateFEffect(void);
	void WarpNext(void);

	void UpdateField(void);
	void SetStateSpeed(STATE_SPEED state);
	void SetStateHandle(STATE_HANDLE state)
	{
		if (m_StateHandle != state) { m_StateHandle = state; if (m_StateSpeed == STATE_SPEED_DRIFT) m_nDriftCounter = 0; }
	};
	void CollisionObject(void);
	void CollisionFeed(void);
	void CollisionEgg(void);
	void CollisionChick(void);
	void CarCalculate(D3DXVECTOR3 * TirePos);
	void UpdateStateJump(void);
	void PlaySoundObj(int nType, CSound * pSound);
	void EggAppear(CFeed *pFeed);
	void ChickAppear(void);
	void ChaseEgg(void);
	void BulletEgg(void);
	void CollisionCharacter(void);
	void ChaseAnnoyS(void);


	CModel						*m_apModel[MAX_PARTS];			//パーツモデルのポインタ
	PlayerAnim					m_nAnimnow;						//現在のアニメーション

	//static CModel *		m_pModel;			//パーツモデルのポインタ
	//static int				m_nMaxModel;	//読み込むモデルの最大数
	//static int				m_nMaxParts;	//読み込むパーツの最大数

	//メンバ変数
	static int					  m_nMaxMotion;					// モーションの最大数
	static CMotion::MOTION_INFO * m_pMotionInfo;				// モーション情報
	static LPDIRECT3DTEXTURE9     m_pTexture;					// テクスチャ
	PLAYERTYPE					  m_PlayerType;					// プレイヤータイプ
	D3DXVECTOR3				      m_pos;						// 中心座標
	D3DXVECTOR3					  m_move;						// 移動
	D3DXVECTOR3					  m_MoveMotion;					// モーションの移動量
	D3DXVECTOR3					  m_OldPos;						// 前回の座標
	D3DXVECTOR3					  m_OldDiffuse;					// 前回の差分
	D3DXVECTOR3					  m_rot;						// 向き
	D3DXVECTOR3					  m_OldEggRot[MAX_FRAME];		// 前回の位置
	D3DXVECTOR3					  m_vtxMaxModel;				// モデルの頂点最大値
	D3DXVECTOR3					  m_vtxMinModel;				// モデルの頂点最小値
	D3DXMATRIX					  m_mtxWorld;					// ワールドマトリックス
	CMotion *					  m_pMotion;					// モーションのポインタ
	STATE_SPEED					  m_StateSpeed;					// スピードの状態
	STATE_HANDLE				  m_StateHandle;				// ハンドルの状態
	PLAYER_INFO					  m_PlayerInfo;					// プレイヤーの情報
	CLoadTextPlayer *			  m_pText;						// プレイヤーの情報読み込み
	CEgg						  *m_pEgg[MAX_EGG];				//卵のポインタ
	CChick						  *m_pChick[MAX_EGG];			//ひよこのポインタ
	static CChick						  *m_pAnnoyChick[MAX_MEMBER];	//ひよこのポインタ
	bool						  m_bJump;						//  ジャンプフラグ
	bool						  m_bControl;					// コントローラーの使用状態
	int							  m_nCountJumpTime;				// ジャンプ状態の時間をカウントする
	float						  m_fvtxMaxY;					// モデル頂点の最大値（Y）
	float						  m_fMaxSpeed;					// スピードの最大値
	float						  m_fMass;						// 質量
	float						  m_fSpeed;						// 速さ
	int							  m_nCountTime;					// 時間の加算
	int							  m_nCountSpeed;				// 時間の加算
	bool						  m_bCrcleCarIn;				// 車が範囲内に入っているかどうか
	CLoadEffect *				  m_pLoadEffect;				// ロードエフェクトのポインタ変数
	int							  m_nCntCombo;
	int							  m_nCntShake;					//揺れの時間をカウントする
	bool						  m_bShake;						//揺れのオンオフ
	bool						  m_bDirive;					//前進、後退の操作フラグ
	int							  m_nCntFlag;
	int							  m_nNumEgg;
	int							  m_nNumChick;
	int							  m_nCntFrame;					// 卵のついてくる処理に使う
	int							  m_nPlayerRank;				// 順位保存用
	int							  m_nCntChick;
	int							  m_nDestRank;					// 狙う順位
	bool						  m_abJump[MAX_FRAME];
	bool						  m_bJumpSave;
	bool						  m_bAnnoyS;
	int							  m_nNumItem;
	int							  m_nAnnoySTimer;
	BULLET						  m_bulletType[MAX_EGG];

	int							  m_nDriftCounter;		// ドリフトカウント

	bool						  m_bDamage;
	PLAYERSTATE					  m_State;
	int							  m_nCntDamage;
	int							  m_nPlayerNum;			// プレイヤー番号
	int							  m_nControllerNum;		// コントローラー番号

	CCOL_MESH::EFFECT			  m_FEffect;				//地面効果
	CRoad_Pointer				  *m_pPoint;			//次のポイント
	CRoad_Pointer				  *m_pEnmPoint;			//敵が見る次のポイント
	D3DXVECTOR3					  m_FNor;				//地面の法線
	float						  m_fPosY;				//別加算位置Y
	bool						  m_bDivided;			//分かれ道かどうか
	int							  m_nMap;				//判定を取るマップ
	float						  m_fLength;			//横幅
	float						  m_fRoad;				//IN_OUTの％
	float						  m_fTilt;				//坂
	float						  m_fCTilt;				//カメラ用坂
	float						  m_fRotOld;			//前のRotY

	// モーション関数	新規
	KEY_INFO						*m_pKeyInfo[MAX_MOTION];	//キー情報へのポインタ
	int								m_nKey;						//現在のキーナンバー
	int								m_nCountFlame;				//フレーム数
	int								m_nNumParts;				//パーツ数
	int								m_aIndexParent[MAX_PARTS];	//親のインデックス
	KEY								m_aKayOffset[MAX_PARTS];	//オフセット情報
	MOTION_INFO						m_aMotionInfo[MAX_MOTION];	//モーション情報
	int								m_nMotionType;				//モーションのタイプ(int型)
	bool							m_bMotionEnd;				//モーション終了
	D3DXVECTOR3						m_OffSetPos[MAX_PARTS];		//パーツごとの最初の位置

	CBillBoord					  *m_pPlayerNum;		// プレイヤー番号（追従）
	bool						  m_bGoal;				// ゴール

	CBillBoord					  *m_pPlayerpos;		// プレイヤー
	float						  m_fAddRot;			// 加算角度
};
#endif
