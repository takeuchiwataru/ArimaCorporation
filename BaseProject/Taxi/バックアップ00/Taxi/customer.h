//=============================================================================
//
// お客さんの処理 [costomer.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _CPSTOMER_H_
#define _CPSTOMER_H_

#include "main.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "motion.h"
#include "humanbace.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_PARTS	(7)		//読み込むパーツ数

//=============================================================================
// 前方宣言
//=============================================================================
class CMeshCylinder;
class CDestination;
class CArrow;
class CMark;
class CPlayer;
class CGameCamera;

//=============================================================================
// お客さんクラス
//=============================================================================
class CCustomer : public CHumanBace
{
public:
	typedef enum
	{//お客さんの状態
		STATE_PUTIN = 0,
		STATE_MOVE,
		STATE_DOWN,
		STATE_MISS,
		STATE_DOWNMOVE,
		STATE_END,
		STATE_ESCAPE,
		STATE_RETURN,
		STATE_CALLTAXI,
		STATE_MAX,
	}STATE;

	//色の状態の種類
	CCustomer();
	~CCustomer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CCustomer * Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 GoalPos, const int nGoalTex, int nAddTime, int nRideTime, MODEL_TYPE type);
	D3DXVECTOR3 GetOldPos(void) { return m_OldPos; };
	D3DXVECTOR3 GetPos(void) { return m_pos; };
	D3DXVECTOR3 GetRot(void) { return m_rot; };
	STATE GetState(void) { return m_state; };

	static bool GetPutIn(void) { return m_bPutIn; }

private:
	//変数宣言
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void SetGoalTex(int nTex) { m_nGoalTex = nTex; };
	void SetAddTime(int nTime) { m_nAddTime = nTime; };
	void SetRideTime(int nTime) { m_nRideTime = nTime; };
	void SetFirstScore(void);
	void UpdatePutin(CPlayer * pPlayer, CGameCamera * pGameCamera);
	void UpdateDown(CGameCamera * pGameCamera);
	void UpdateDownMove(void);
	void UpdateMove(D3DXVECTOR3 PlayerPos);
	void UpdateMiss(CPlayer * pPlayer, CGameCamera * pGameCamera);
	void UpdateEscape(void);
	void UpdateCallTaxi(D3DXVECTOR3 PlayerPos);
	void UpdateEnd(CPlayer * pPlayer);
	void UninitMeshCylinder(void);
	void UpdateReturn(void);
	void CustomerResult(void);
	D3DXCOLOR SetRank(void);
	void SetState(STATE state);

	D3DXVECTOR3		m_MoveMotion;			//モーションの移動量
	D3DXVECTOR3		m_OldPos;				//前回の座標
	D3DXVECTOR3		m_OldDiffuse;			//前回の差分
	D3DXVECTOR3		m_GoalPos;				//ゴール地点
	D3DXVECTOR3		m_FirstPos;				//初期位置
	D3DXCOLOR		m_Color;				//色
	STATE			m_state;				//状態設定
	STATE			m_OldState;				//前回の状態を取得する
	float			m_fAngle;				//角度
	int				m_nGoalTex;				//目的地テクスチャ番号
	int				m_nAddTime;				//時間の可算量
	int				m_nRideTime;			//制限時間
	int				m_nCountRideTime;		//乗車時間カウンター
	int				m_nFirstScore;			//お金の加算量
	bool			m_bDrawMeshCylinder;	//シリンダーの描画フラグ
	bool			m_bGotOn;				//乗ったかどうか
	bool			m_bResult;				//結果を求めたかどうか
	CMeshCylinder *	m_pMeshCylinder;		//メッシュシリンダーのポインタ
	CDestination *	m_pDestination;			// 目的地写真へのポインタ
	CArrow *		m_pArrow;				//矢印へのポインタ
	CMark *			m_pMark;				//マークのポインタ

	static bool		m_bPutIn;				//	乗車しているかどうか
};
#endif