//=============================================================================
//
// メイン処理 [scene3D.h]
// Author : 有馬武志
//
//=============================================================================
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

#include "main.h"
#include "scene.h"
#include "2DPolygon.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_BILLBOORD_TEX		(2)

//=====================
//  CSceneの派生クラス
//=====================
class CScene3D : public CScene
{
public://誰でも扱える
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_BILLBOORD,
		TYPE_WALL,
		TYPE_GROUND,
		TYPE_EFFECT,
		TYPE_BLOOD,
		TYPE_SHADOW,
		TYPE_SPOTLIGHT,
		TYPE_MAX
	}TYPE;

	CScene3D(int nPriority = 3, CScene::OBJTYPE objType = CScene::OBJTYPE_3DPOLYGON);
	~CScene3D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindTexture(LPDIRECT3DTEXTURE9 Texture);
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetRotation(void) { return m_rot; }
	D3DXVECTOR2 GetSize(void) { return m_size; }
	D3DXVECTOR3 GetMove(void) { return m_Move; }
	D3DXMATRIX GetMatrix(void) { return m_mtxWorld; }
	int  GetTexType(void) { return m_TexType; }
	C2D::DRAW_TYPE  &GetDrawType(void) { return m_DrawType; }

	static CScene3D *Create();
	void SetTypeNumber(TYPE Type) { m_Type = Type; }
	void SetTexType(int Type) { m_TexType = Type; }
	void SetTexture(int PatternAnim, int X, int Y, int nNum);
	void SetMove(D3DXVECTOR3 move) { m_Move = move; }
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetMatrix(D3DXMATRIX mtx) { m_mtxWorld = mtx; }
	void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 nSize);
	void SetPosSize(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void SetGroundPosSize(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void SetColor(D3DXCOLOR Color);
	void SetUV(D3DXVECTOR2 *uv);
	void SetNor(D3DXVECTOR3 Nor);
	void SetVtxEffect(float fRadius);
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetSpin(D3DXVECTOR3 pos, float fAngle, float fLength, D3DXVECTOR3 rot);
	void Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	D3DXVECTOR3 &GetposR(void) { return m_pos; }
	D3DXVECTOR3 &GetrotR(void) { return m_rot; }

private://個人でのみ使う
	C2D::DRAW_TYPE			m_DrawType;		// 描画する方法(加算合成など)
	LPDIRECT3DTEXTURE9		m_pTexture;						//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;						//頂点バッファへのポインタ
	D3DXVECTOR3				m_pos;							//位置
	D3DXVECTOR3				m_rot;							//向き
	D3DXVECTOR2				m_size;							//サイズ
	D3DXVECTOR3				m_Move;							//動き
	D3DXMATRIX				m_mtxWorld;						//ワールドマトリックス
	TYPE					m_Type;							//ビルボード：１かエフェクト：２か
	int						m_TexType;						// テクスチャのタイプ
};
//=============================================================================
// クラス定義
//=============================================================================
class C3DPolygon : public CScene3D
{
public:
	typedef enum
	{
		TYPE_Pin,
		TYPE_Light,
		TYPE_Shadow,
		TYPE_FootSteps,
		TYPE_Wind,
		TYPE_MAX
	}TYPE;
	C3DPolygon(int nPriority = 0) : CScene3D(nPriority, OBJTYPE_3DPOLYGON) {};
	~C3DPolygon() {};

	static C3DPolygon *Create(TYPE Type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPriority = 1);
	HRESULT Init(void);
	void Update(void);
	void Draw(void);
	void SetShadow(D3DXVECTOR3 pos);
	void SetColor(D3DXCOLOR col) { m_col = col; CScene3D::SetColor(col); };
	D3DXCOLOR &GetcolR(void) { return m_col; }

protected:
	void	Setting(TYPE &Type, D3DXVECTOR3 &pos, D3DXVECTOR3 &rot);
private:
	void	Set(TYPE &Type, float &fCntState, D3DXCOLOR &col);
	void	SetRotY(void);
	void	GoStraight(float fPlus);
	//==変数===========================================================================
	D3DXVECTOR3	m_Initpos;		//初期位置
	D3DXCOLOR	m_col;			//色
	TYPE		m_Type;			//種類
	float		m_fCntState;	//状態管理用
};
//=============================================================================
// クラス定義
//=============================================================================
class C3DAnim : public C3DPolygon
{
public:
	typedef enum
	{
		ANIM_LOOP = -1,
		ANIM_END,
		ANIM_ONE,
		ANIM_MAX
	}ANIM;
	C3DAnim(int nPriority = 5) : C3DPolygon(nPriority) {};
	~C3DAnim() {};

	static C3DAnim *Create(TYPE Type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPriority);
	HRESULT Init(void);
	void Update(void);

private:
	bool	EndCheck(void);
	void	Setting(TYPE &Type);
	//==変数===========================================================================
	ANIM	m_Anim;
	int		m_nPtn, m_nPtnX, m_nPtnY;	//アニメーション情報
	int		m_nSpeed, m_nCntAnim;	//アニメーション管理用
};
#endif