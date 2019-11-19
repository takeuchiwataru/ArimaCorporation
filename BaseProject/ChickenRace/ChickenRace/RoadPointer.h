//=============================================================================
//
// メッシュ処理 [mesh.h]
// Author : Ryo Sugimoto
//
//=============================================================================
#ifndef _ROADPOINTER_H_
#define _ROADPOINTER_H_

#include "main.h"
#include "scene.h"
class CPlayer;
//*****************************************************************************
// マクロの定義
//*****************************************************************************
#define MAX_RACER	(8)
#define MAX_RPOINT	(2)
#define MAX_TOP		(4)
//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CRoad_Pointer
{
public:
	typedef enum
	{
		ROAD_SHORT,	//短い一つ目の道
		ROAD_ITEM,	//アイテム二つ目の道
		ROAD_MAX
	}ROAD;
	typedef enum
	{
		POINT_LEFT,		//左端
		POINT_RIGHT,	//右端
		POINT_MAX
	}POINT;
	typedef enum
	{
		TRAILS_NONE,		//どちらでもないコース
		TRAILS_IN,		//インコース
		TRAILS_OUT,		//アウトコース
		TRAILS_MAX
	}TRAILS;
	CRoad_Pointer() {};
	~CRoad_Pointer() {};

	static bool	Create(CRoad_Pointer *&pRoadPointer);
	HRESULT Init(void);
	void	Uninit(void);

	void	Save(FILE *pFile);
	void	Load(FILE *pFile, CRoad_Pointer **&pPoint);
	void	SetRoad(int &nNumRoad, bool &bRoad);
	void	Connect(CRoad_Pointer *pPoint);
	CRoad_Pointer	*Release(void);
	void	Scale(POINT point, float fValue);
	float			GetfRotY(void);
	CRoad_Pointer	*GetNext(int nNumber, int &nMap, int nRank);
	CRoad_Pointer	*GetPrev(int nNumber, int &nMap);

	D3DXVECTOR3		&Getpos(void) { return m_pos; };
	D3DXVECTOR3		&GetPoint(int nNumber) { return m_Point[nNumber]; };
	int				&GetnNumRoad(void) { return m_nNumRoad; };

	CRoad_Pointer	*&GetpNextPointer(int nNumber) { return m_pNextPointer[nNumber]; };
	CRoad_Pointer	*&GetpRoadPointer(int nNumber) { return m_pRoadPointer[nNumber]; };

	static CRoad_Pointer	*Plus(CRoad_Pointer *pPointer, D3DXVECTOR3 pos, D3DXVECTOR3 pos2);
	static void				RankPoint(CPlayer *pPlayer, bool &bGoal);
	static bool				BeyondPoint(CPlayer *&pPlayer, bool bRank, bool &bGoal);

	static	float			NextRot(D3DXVECTOR3 &pos, CRoad_Pointer *&pmyPoint, float &fRoad, int &nMap, int nNumber);
	static	float			BentRot(D3DXVECTOR3 &pos, CRoad_Pointer *&pPoint, float &fRot, float &fDistance, float &fRoad);
	static	float			BentRotX(D3DXVECTOR3 &pos, CRoad_Pointer *&pPoint, float &fRot, float &fDistance);

private://*****************************************************************************
	static bool	Beyond(CPlayer *&pPlayer, CRoad_Pointer *&pmyPoint, int &nNumber, bool &bRank, bool &bGoal);
	static bool	Reverse(CPlayer *&pPlayer, CRoad_Pointer *&pmyPoint, int &nNumber);
	//変数宣言//***********************************************************************
	int				m_nNumber;					//ポイント生成番号
	int				m_nNumRoad;					//ポイント生成番号
	D3DXVECTOR3		m_pos;						//中心
	D3DXVECTOR3		m_Point[POINT_MAX];			//端の位置
	TRAILS			m_Trails[POINT_MAX];		//インアウト判定用
	float			m_fNextDistance[ROAD_MAX];	//次のポイントまでの距離
	CRoad_Pointer	*m_pNextPointer[ROAD_MAX];	//次のポイント	生成するやつ
	CRoad_Pointer	*m_pRoadPointer[ROAD_MAX];				//前のポインタ
};
//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CRoad_Manager
{
public:
	typedef enum
	{
		MAP_Half0,	//マップ半分
		MAP_Bridge,	//橋
		MAP_Half1,	//マップ半分
		MAP_MAX
	}MAP;
	CRoad_Manager() {};
	~CRoad_Manager() {};

	static bool	Create(CRoad_Manager *&pRoadManager);
	HRESULT Init(void);
	void	Uninit(void);

	void	Save(void);
	void	LoadMap(void);
	void	Load(int &nNumber, MAP map);
	void	SetDistance(int &nNumber, int &nNumPoint, float fDistance);
	void	SetRank(void);
	void	SetRoad(void);
	int		GetRank(int &nNumber) { return m_nRanking[nNumber]; };
	int		&GetnNumAll(void) { return m_nNumAll[m_nWKRank][m_nWKData]; };
	CRoad_Pointer *GetTop(int nRank = 0, int nNumber = 0) { return m_pTop[nRank][nNumber]; };
	CRoad_Pointer *GetCur(int nNumber = 0) { return m_pCur[0][nNumber]; };
	void SetCur(CRoad_Pointer *pPoint) { m_pCur[m_nWKRank][m_nWKData] = pPoint; };
	CRoad_Pointer *GetWKTop(void) { return m_pWKTop; };
	CRoad_Pointer *&GetpWKTop(void) { return m_pWKTop; };
	bool			&GetbWKData(void) { return m_bWKData; };
	int				&GetnWKData(void) { return m_nWKData; };

	void SetTop(CRoad_Pointer *pPoint) { m_pTop[m_nWKRank][m_nWKData] = pPoint; };
	static CRoad_Manager *&GetManager(void) { return m_pRoadManager; };
private://*****************************************************************************
		//変数宣言//***********************************************************************
	static CRoad_Manager	*m_pRoadManager;
	CRoad_Pointer	*m_pTop[MAX_RPOINT][MAX_TOP];		//最初のポイント
	CRoad_Pointer	*m_pCur[MAX_RPOINT][MAX_TOP];		//最初のポイント

	CRoad_Pointer	*m_pWKTop;				//最初のポイント
	bool			m_bWKData;				//作業中判定
	int				m_nWKData;				//作業中番号
	int				m_nWKRank;				//作業中番号

	int				m_nNumAll[MAX_RPOINT][MAX_TOP];		//ポイント番号
	float			m_fDistance[MAX_RACER];	//ポイントまでのそれぞれの距離
	int				m_nNumPoint[MAX_RACER];	//ポイントまでのそれぞれの距離
	int				m_nRanking[MAX_RACER];	//順位
};
#endif
