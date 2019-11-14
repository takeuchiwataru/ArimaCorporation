//=============================================================================
//
// 判定付加メッシュ処理 [ColMesh.h]
// Author : Ryo Sugimoto
//
//=============================================================================
#ifndef _COLMESH_H_
#define _COLMESH_H_

#define _CRT_SECURE_NO_WARNINGS	//Fire Wall突破
#include <stdio.h>				//インクルドファイル
#include "main.h"
#include "scene.h"
#include "NorMesh.h"
class CCOL_MESH_MANAGER;
//*****************************************************************************
// マクロの定義
//*****************************************************************************
#define COLMESH_PRIORITY	(1)	//優先順位
#define COLMESH_MODE		(0)	//0ならツール
#define MAX_COL_MESH		(64)
#define INIT_LENGTH			(100.0f)

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CCOL_MESH
{
public:
	typedef enum
	{
		LINE_UP,
		LINE_DOWN,
		LINE_ALL,
		LINE_MAX,
	}LINE;
	typedef enum
	{
		TYPE_NORMAL,
		TYPE_EFFECT,
		TYPE_FLOW,
		TYPE_MAX,
	}TYPE;
	typedef enum
	{
		EFFECT_NORMAL,
		EFFECT_GRASS,	//ダート
		EFFECT_SWAMP,	//水たまり
		EFFECT_RIVER,	//川
		EFFECT_BOOST,	//加速床
		EFFECT_MAX,
	}EFFECT;
	CCOL_MESH() {};
	~CCOL_MESH() {};

	static bool	Create(CCOL_MESH *&pColMesh);
	HRESULT Init(void);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	void	Delete(void);
	void	Set(bool bField, D3DXVECTOR3 pos);
	void	SetMesh(void);
	int		AddWidth(int nValue);
	int		AddVertical(int nValue);
	int		DeleteLineV(int nWidth);
	int		DeleteLineW(int nVertical);

	void	SpinRot(float fAddRot, float fScale);
	void	Delete(int nNumber);
	void	Move(D3DXVECTOR3 move);
	void	SetWall(D3DXVECTOR3 &pos, D3DXVECTOR3 &Vtx, int &nNumWidth);
	void	Select(int nNumber);
	void	LineSelectX(int nNumX, int nNumZ, LINE line);
	void	LineSelectZ(int nNumX, int nNumZ, LINE line);
	void	ResetSelect(void);
	
	void	SetPoint(bool bWK);

	void	Save(FILE *pFile);
	void	Load(FILE *pFile);

	bool	MeshField(D3DXVECTOR3& mypos, D3DXVECTOR3& pos, D3DXVECTOR3 &posold, D3DXVECTOR3 &move, D3DXVECTOR3 &FNor, bool &bJump);
	bool	FieldCheck(D3DXVECTOR3 &FposUL, D3DXVECTOR3 &FposUR, D3DXVECTOR3 &FposDL, D3DXVECTOR3 &FposDR, D3DXVECTOR3& pos);
	float	FieldCollision(D3DXVECTOR3 &FposUL, D3DXVECTOR3 &FposUR, D3DXVECTOR3 &FposDL, D3DXVECTOR3 &FposDR, D3DXVECTOR3 WKpos, D3DXVECTOR3& pos, D3DXVECTOR3 &FNor);
	void	MeshWall(D3DXVECTOR3& mypos, D3DXVECTOR3& pos, D3DXVECTOR3 &posold, D3DXVECTOR3 &move, float& fLength, int &nCntHit, bool &bReflection);
	int		WallCollision(D3DXVECTOR3 Wpos0, D3DXVECTOR3 Wpos1, D3DXVECTOR3 WUpos0, D3DXVECTOR3 WUpos1, D3DXVECTOR3& pos, D3DXVECTOR3 posold, D3DXVECTOR3 &move, float& fLength, int &nCntHit, bool &bReflection);
	static int		AngleCheck(float fAngle0, float fAngle1);

	TYPE		&GetType(void) { return m_Type; }
	EFFECT		&GetEffect(void) { return m_Effect; }
	int			&GetnNumVtx(void) { return m_nNumVtx; }
	int			&GetnNumWidth(void) { return m_nNumWidth; }
	int			&GetnNumVertical(void) { return m_nNumVertical; }
	D3DXVECTOR3 *&GetVtx(void) { return m_pVtx; }
	D3DXVECTOR3 &Getpos(void) { return m_pos; }

private://*****************************************************************************
		//変数宣言//***********************************************************************
	TYPE			m_Type;				//メッシュタイプ
	EFFECT			m_Effect;			//効果
	D3DXVECTOR3		m_pos;				//位置
	int				m_nNumVtx;			//頂点数
	int				m_nNumVertical;		//縦
	int				m_nNumWidth;		//横
	D3DXVECTOR3		*m_pVtx;			//頂点
	bool			*m_bSelect;			//選択
	bool			m_bField;			//地面か壁か

	CNorMesh		*m_pMesh;
};
//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CCOL_MESH_MANAGER : public CScene
{
public:
	typedef enum
	{
		TYPE_MADE,
		TYPE_HALF0,		//始めの半分
		TYPE_BRIDGE,	//橋
		TYPE_HALF1,		//終わりの半分
		TYPE_MAX,
	}TYPE;
	CCOL_MESH_MANAGER(int nPrioryity = COLMESH_PRIORITY) : CScene::CScene(nPrioryity, CScene::OBJTYPE_COLMESH) {};
	~CCOL_MESH_MANAGER() {};

	static void LoadMap(void);
	static void EndMap(void);
	static void Load(TYPE type);
	static void UnLoad(void);

	static bool	Create(CCOL_MESH_MANAGER *&pColManager);
	HRESULT	Init(void);
	void	Update(void);
	void	Draw(void);
	void	ColDraw(void);
	void	Uninit(void);
	void	Save(void);
	void	Loadtxt(TYPE type);
	void	Set(TYPE type);
	static void	WNumCollision(int nNumber, D3DXVECTOR3& mypos, D3DXVECTOR3& pos, D3DXVECTOR3 &posold, D3DXVECTOR3 &move, float &fLength);
	static bool	Collision(D3DXVECTOR3& pos, D3DXVECTOR3 &posold, D3DXVECTOR3 &move, float &fLength, D3DXVECTOR3 &FNor, bool &bJump, int &nMap, bool bWHit);
	static bool	FNumCollision(int nNumber, D3DXVECTOR3& mypos, D3DXVECTOR3& pos, D3DXVECTOR3 &posold, D3DXVECTOR3 &move, D3DXVECTOR3 &FNor, bool &bJump);

	void	WCollision(void);

	int	AddField(D3DXVECTOR3 pos);
	int	AddWall(D3DXVECTOR3 pos);
	void	DeleteField(int nNumber);
	void	DeleteWall(int nNumber);

	CCOL_MESH	**&GetpField(void) { return m_pField; }
	CCOL_MESH	**&GetpWall(void) { return m_pWall; }
	int			&GetnNumField(void) { return m_nNumField; }
	int			&GetnNumWall(void) { return m_nNumWall; }
private://*****************************************************************************
		//変数宣言//***********************************************************************
	static CCOL_MESH_MANAGER *m_pColMesh[TYPE_MAX];

	TYPE		m_Type;
	int			m_nNumField;
	int			m_nNumWall;
	CCOL_MESH	**m_pField;
	CCOL_MESH	**m_pWall;
};
#endif