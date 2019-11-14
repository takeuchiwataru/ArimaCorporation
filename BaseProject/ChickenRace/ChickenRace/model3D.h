//=============================================================================
//
// モデルの処理 [model3D.h]
// Author : 有馬武志
//
//=============================================================================
#ifndef _MODEL3D_H_
#define _MODEL3D_H_

#include "main.h"
#include "scene.h"
#include "toonshader.h"
#include "camerabace.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//=====================
//  CSceneの派生クラス
//=====================
class CModel3D : public CScene
{
public://誰でも扱える
	typedef enum
	{
		MOVETYPE_NONE = 0,
		MOVETYPE_NOT,
		MOVETYPE_CONT,
		MOVETYPE_X_MOVE,
		MOVETYPE_Y_MOVE,
		MOVETYPE_Z_MOVE,
		MOVETYPE_ROTATION_X,
		MOVETYPE_ROTATION_Y,
		MOVETYPE_ROTATION_Z,
		MOVETYPE_HOUSE,
		MOVETYPE_MAX
	}MOVETYPE;

	typedef enum
	{// ボックスの角８つ
		VTX_CORNER_LDF = 0,		// 左下前
		VTX_CORNER_LUF,			// 左上前
		VTX_CORNER_RDF,			// 右下前
		VTX_CORNER_RUF,			// 右上前
		VTX_CORNER_RDB,			// 右下奥
		VTX_CORNER_RUB,			// 右上奥
		VTX_CORNER_LDB,			// 左下奥
		VTX_CORNER_LUB,			// 左上奥
		VTX_CORNER_MAX,
	}VTX_CORNER;

	CModel3D(int nPriority = 3, CScene::OBJTYPE objType = CScene::OBJTYPE_3DMODEL);
	~CModel3D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetPosition(void) { return m_Pos; }
	D3DXVECTOR3 &GetRot(void) { return m_Rot; }
	MOVETYPE GetMoveType(void) { return m_nMoveType; }
	D3DXVECTOR3 GetMove(void) { return m_Move; }
	D3DXVECTOR3 GetScale(void) { return m_Scale; }
	D3DXMATRIX GetMtx(void) { return m_mtxWorldObject; }
	void BindModel(LPD3DXMESH pMeshObject, LPD3DXBUFFER	pBuffMatObject, DWORD nNumMatObject, LPDIRECT3DTEXTURE9	pMeshTextures, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin);
	D3DXVECTOR3 GetVtxMin(void) { return m_VtxMinModel; }
	D3DXVECTOR3 GetVtxMax(void) { return m_VtxMaxModel; }
	static CModel3D *Create(void);
	void SetMove(D3DXVECTOR3 m_move) { m_Move = m_move; }
	void SetScale(D3DXVECTOR3 Scale) { m_Scale = Scale; }
	void SetPosition(D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; };
	void SetMoveType(MOVETYPE MoveType) { m_nMoveType = MoveType; }
	void RecalculationVtx(void);							// 頂点の最大値と最小値を再計算
	D3DXVECTOR3 BlowOff(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fHeavy);
	bool Collision(D3DXVECTOR3 pos, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin, D3DXVECTOR3 move);
	void Landing(D3DXVECTOR3 posOld);						// 地面に着いたとき

	bool GetBoolBlowOff(void) { return m_bBlowOff; };
	void SetBoolBlowOff(bool bBlowOff) { m_bBlowOff = bBlowOff; };

	float GetHeavy(void) { return m_fMass; };
	void SetHeavy(float fMass) { m_fMass = fMass; };
	void Setcol(D3DXCOLOR col) { m_col = col; m_bcolChange = true; }// カラー設定

	void InitRot(void);

	bool GetBoolRoll(void) { return m_bRoll; }
	void SetBoolRoll(bool bRoll) { m_bRoll = bRoll; }

	bool GetBoolBlow(void) { return m_bBlow; }
	void SetBoolBlow(bool bBlow) { m_bBlow = bBlow; }

	void ScaleVtxCornerPos(D3DXVECTOR3 scale);

	bool GetStateRoll(void) { return m_bStateRoll; }
	void SetStateRoll(bool bStateRoll) { m_bStateRoll = bStateRoll; }

	float GetLength(void) { return m_fLength; }

protected:
	void SetUpdate(bool bUpadate) { m_bUpdate = bUpadate; };
	void SetSmallObj(bool bSmallObj) { m_bSmallObjDraw = bSmallObj; };
	void SetHightObj(bool bHightObj) { m_bHightObjDraw = bHightObj; };
	void SetOnlyLength(bool bOnlyLength) { m_bOnlyLengthDraw = bOnlyLength; };
	bool GetDelete(void) { return m_bDelete; };
	void SetDelete(bool bDelete) { m_bDelete = bDelete; };
	void SetType(int nType) { m_nType = nType; }

private://個人でのみ使う
	float CrossingAngle(D3DXVECTOR3 vec0, D3DXVECTOR3 vec1);	// ベクトルのなす角を計算
	void RotCalculation(void);									// 衝突時の角度計算
	bool LengthJudgment(void);									//距離判定

	D3DMATERIAL9			*m_pMeshMaterials = NULL;

	LPD3DXMESH				m_pMeshObject;						//メッシュ情報へのポインタ
	LPD3DXBUFFER			m_pBuffMatObject;					//マテリアルの情報へのポインタ
	DWORD					m_nNumMatObject;					//マテリアルの情報数
	LPDIRECT3DTEXTURE9		m_pMeshTextures;
	LPDIRECT3DTEXTURE9		*m_pShaderMeshTextures;				//シェーダー用
	D3DXMATRIX				m_mtxWorldObject;					//ワールドマトリックス
	D3DXMATRIX				m_mtxRot;							//回転マトリックス(保存用)
	D3DXQUATERNION			m_quat;								//クォータニオン
	D3DXVECTOR3				m_VecAxis;							//回転軸
	float					m_fValueRot;						//回転角(回転量)
	D3DXVECTOR3				m_VtxMinModel, m_VtxMaxModel;		//モデルの最小値・最大値
	D3DXVECTOR3				m_Pos;								//位置
	D3DXVECTOR3				m_Rot;								//向き
	D3DXVECTOR3				m_Scale;							//サイズ
	D3DXVECTOR3				m_Move;								//動き
	MOVETYPE				m_nMoveType;						//動きの種類
	bool					m_bBlowOff;							// 吹っ飛ばされているかどうか
	float					m_fMass;							// 重さ
	float					m_fRotY;							// 衝突時の回転方向
	D3DXVECTOR3				m_aVtxCornerPos[VTX_CORNER_MAX];	// 当たり判定用ボックスの角８つの頂点座標
	D3DXVECTOR3				m_aVtxPosStorage[VTX_CORNER_MAX];	// 当たり判定用ボックスの角８つの頂点座標の保存用
	D3DXVECTOR3				m_blowOff;							// 吹っ飛ぶ距離
	int						m_nIdxCornerPosMinY;				// ボックスの角８つの頂点座標の中で一番低い
	int						m_nCountTime;						// 時間の可算
	bool					m_bUpdate;							// アップデートを通すかどうか
	bool					m_bUpdateBlow;						// 吹っ飛ばす処理を行うかどうか
	bool					m_bBlow;							// 吹っ飛ばすかどうか
	bool					m_bDelete;							// 破棄するかどうか
	bool					m_bcolChange;						// カラー変更
	bool					m_bSmallObjDraw;					// 小さいオブジェクトの描画フラグ
	bool					m_bHightObjDraw;					// 高いオブジェクトの描画フラグ
	D3DXCOLOR				m_col;								// カラー
	bool					m_bRoll;							// 転がるかどうか
	bool					m_bStateRoll;						// 転がっているかどうか
	bool					m_bOnlyLengthDraw;					// 描画距離だけ求める
	D3DXVECTOR3				m_scale;							// スケール変更の倍率
	float					m_fLength;							// 距離
	bool					m_bTexMat;							// シェーダーに使うテクスチャとマテリアル
	int						m_nType;							//モデルの種類
	CToonShader				*m_pToonShader;						//シェーダーのポインタ
	D3DXVECTOR3				m_MapView;							//マップに反映させるカメラ
};
#endif