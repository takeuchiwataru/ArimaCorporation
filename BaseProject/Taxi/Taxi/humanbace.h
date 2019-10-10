//=============================================================================
//
// ヒューマンベースの処理 [humanbace.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _HUMANBACE_H_
#define _HUMANBACE_H_

#include "main.h"
#include "scene.h"
#include "motion.h"
#include "loadText.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_PARTS	(7)		//読み込むパーツ数

//=============================================================================
// 前方宣言
//=============================================================================
class CModel;
class CShadow;

//=============================================================================
// ヒューマンベースクラス
//=============================================================================
class CHumanBace : public CScene
{
public:
	typedef enum
	{
		MODEL_MAN = 0,
		MODEL_WOMAN,
		MODEL_MAX,
	}MODEL_TYPE;

	typedef enum
	{
		MOTION_NEUTRAL = 0,
		MOTION_TAXICOLL,
		MOTION_WALK,
		MOTION_BOW,
		MOTION_SHINOBU,
		MOTION_KICK,
		MOTION_ANGER,
		MOTION_ESCAPE,
		MOTION_MAX,
	}MOTION_TYPE;

	CHumanBace(int nPriority, CScene::OBJTYPE type);
	~CHumanBace();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(float fAlpha);
	static void LoadModel(void);
	static void UnloadModel(void);
	void SetClipping(bool bClipping) { m_bClipping = bClipping; }
	
protected:
	//位置の設定・取得
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };
	D3DXVECTOR3 GetPos(void) { return m_pos; };

	//向きの設定・取得
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };
	D3DXVECTOR3 GetRot(void) { return m_rot; };

	//移動量の設定・取得
	void SetMove(D3DXVECTOR3 move) { m_move = move; };
	D3DXVECTOR3 GetMove(void) { return m_move; };

	void RemakeAngle(float * pAngle);
	float GetLength(D3DXVECTOR3 StartPos, D3DXVECTOR3 EndPos);
	void MoveEscape(void);

	//モデル情報の取得
	CModel ** GetpModel(void) { return m_pModel; };

	//モデルタイプの設定・取得
	void SetModelType(MODEL_TYPE type) { m_ModelType = type; };
	MODEL_TYPE GetModelType(void) { return m_ModelType; };

	void SetShadowColor_A(float col_a);
	void SetNextMotion(MOTION_TYPE type);

	//----------------------
	// 変数
	//----------------------
	D3DXVECTOR3	m_pos;			//中心座標
	D3DXVECTOR3	m_move;			//移動
	D3DXVECTOR3	m_rot;			//向き
	D3DXMATRIX	m_mtxWorld;		//ワールドマトリックス
	int			m_nCountTime;	//カウンター
	MODEL_TYPE  m_ModelType;	//モデルのタイプ
	MOTION_TYPE	m_MotionType;	//モーションの種類
	bool		m_bClipping;	//クリッピングフラグ
	float		m_fLength;		//距離の変数
	float		m_MotionLength;	//モーションの更新範囲

	//----------------------
	// 定数
	//----------------------
	const D3DXVECTOR3 VECTOR_ZERO = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

private:
	void CreateModel(void);
	void CreateMotion(void);
	void CollisionMesh(void);
	D3DXVECTOR3 GetVtxMax(void);
	D3DXVECTOR3 GetVtxMin(void);

	static LPDIRECT3DTEXTURE9			m_pTexture[MODEL_MAX];		//テクスチャ
	static int							m_nMaxMotion[MODEL_MAX];	//モーションの最大数
	static CMotion::MOTION_INFO *		m_pMotionInfo[MODEL_MAX];	//モーション情報
	static CLoadTextMotion::MODEL_INFO 	m_ModelInfo[MODEL_MAX];		//モデル情報
	CModel **							m_pModel;					//パーツモデルのポインタ
	D3DXVECTOR3							m_vtxMax;					//モデルの頂点最大値
	D3DXVECTOR3							m_vtxMin;					//モデルの頂点最小値
	CShadow *							m_pShadow;					//影のポインタ
	CMotion *							m_pMotion;					//モーションのポインタ
};
#endif