//=============================================================================
//
// オブジェクト処理 [object.h]
// Author : 有馬武志
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "model3D.h"
#include "camerabace.h"
#include "shadow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_OBJECT					(38)				// オブジェクトのモデル数
#define MAX_OBJECT_TEXTURE			(41)			// オブジェクトのテクスチャ数
#define OBJECT_PRIOTITY				(3)

//アップデートを通さないタイプ
#define UPDATE_TYPE_NUM			(20)		// アップデートしないタイプの最大数
#define TYPE_TREE00				(2)			// ツリーU
#define TYPE_TREE01				(3)			// ツリーD
#define TYPE_BILL00				(6)			// ビル00
#define TYPE_BILL01				(7)			// ビル01
#define TYPE_BILL02				(8)			// ビル02
#define TYPE_TVBILL				(13)		// テレビ塔
#define TYPE_TANUKI				(18)		// 屋根のタイプ番号
#define TYPE_OCLOCK				(22)		// 時計台
#define TYPE_REDBILL			(23)		// 赤レンガ
#define TYPE_CORN2				(24)		// コーン（フィールドの周りにあるもの）
#define TYPE_STATION			(25)		// 札幌駅
#define TYPE_ESTA				(26)		// エスタ
#define TYPE_DAIMAL				(27)		// 大丸
#define TYPE_APIA				(28)		// アピアドーム
#define TYPE_TOWER				(29)		// JRタワー
#define TYPE_FOUNTAIN			(30)		// 噴水
#define TYPE_FERRISWGEEL		(34)		// 観覧車
#define TYPE_TAPIOCA			(35)		// タピオカー
#define TYPE_HOSPITAL			(36)		// 病院
#define TYPE_LCDPANEL			(37)		// 液晶パネル

//距離のみ
#define TYPE_TREE02				(14)		// 木
#define TYPE_FLOWER				(17)		// 花壇

//高いオブジェクトタイプ
#define HIGHT_OBJ_NUM			(4)
#define TYPE_STREETLIGHT		(9)			// 街灯
#define TYPE_TRAFFICLIGHT00		(10)		// 信号機00
#define TYPE_TRAFFICLIGHT01		(11)		// 信号機02
#define TYPE_ROAD				(31)		// 指示看板

//小さいオブジェクトタイプ
#define SMALL_OBJ_NUM			(10)		// 小さいオブジェクトの種類
#define TYPE_DRINKMACHINE		(0)			// 自動販売機
#define TYPE_GRASS				(1)			// 芝生
#define TYPE_CARDBORD			(4)			// ダンボール
#define TYPE_CORN				(5)			// コーン
#define TYPE_BENCH				(12)		// ベンチ
#define TYPE_PHONEBOX			(15)		// 電話ボックス
#define TYPE_LEAF				(16)		// 草壇
#define TYPE_SIGNBOARD			(20)		// 看板
#define TYPE_BEERBOX			(21)		// ビールケース
#define TYPE_FENCE				(32)		// フェンス

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CEffectMgr;
class CLoadEffect;
class CObjBillboad;

//=====================
//  CModel3Dの派生クラス
//=====================
class CObject : public CModel3D
{
public://誰でも扱える
	CObject();
	~CObject();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CObject *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, float move, int nTexTypeint, int nObjectType, MOVETYPE nMovetype, int nCollision);
	void DeleteObject(void);
	int GetType(void) { return m_nType; };
	D3DXVECTOR3 Fountain(D3DXVECTOR3 pos, D3DXVECTOR3 move);
	bool CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);

private://個人でのみ使う
	void CreateEffectMgr(D3DXVECTOR3 pos);
	void FountainEffect(void);
	void LeafEffect(float fLength);

	static LPD3DXMESH	m_pMeshModel[MAX_OBJECT];					//メッシュ情報へのポインタ
	static LPD3DXBUFFER m_pBuffMatModel[MAX_OBJECT];				//マテリアルの情報へのポインタ
	static DWORD		m_nNumMatModel[MAX_OBJECT];					//マテリアルの情報数
	static LPDIRECT3DTEXTURE9 m_pMeshTextures[MAX_OBJECT_TEXTURE];	//テクスチャ情報
	static D3DXVECTOR3 m_LoadVtxMaxModel[MAX_OBJECT];				//モデルの最大値
	static D3DXVECTOR3 m_LoadVtxMinModel[MAX_OBJECT];				//モデルの最小値

	D3DXVECTOR3			m_scale;									// サイズ
	D3DXVECTOR3			m_Spin;										// 回転
	int					m_nCount;									// カウンター
	int					m_nCollision;								// 当たり判定のONOFF
	int					m_nType;									// 種類
	int					m_nTex;										// テクスチャ番号
	D3DXVECTOR3			m_ModelMove;								// 移動の仕方
	int					m_nStageCount;								// ステージの移動時に少し時間をかける
	bool				m_bStageNext;								// ステージ遷移中
	CShadow				*m_pShadow;									// 影のポインタ
	CEffectMgr *		m_pEffectMgr;								// エフェクトマネージャー
	CLoadEffect *		m_pLoadEffect;								// ロードエフェクト
	CObjBillboad *		m_pObjBill;									// オブジェクトビルボードのポインタ
};
#endif