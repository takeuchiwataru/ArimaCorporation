//=============================================================================
//
// ���b�V������ [mesh.h]
// Author : Ryo Sugimoto
//
//=============================================================================
#ifndef _ROADPOINTER_H_
#define _ROADPOINTER_H_

#include "main.h"
#include "scene.h"
#include "ColMesh.h"
class CPlayer;
//*****************************************************************************
// �}�N���̒�`
//*****************************************************************************
#define MAX_RACER	(8)
#define MAX_RPOINT	(2)
#define MAX_TOP		(4)
//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CRoad_Pointer
{
public:
	typedef enum
	{
		ROAD_SHORT,	//�Z����ڂ̓�
		ROAD_ITEM,	//�A�C�e����ڂ̓�
		ROAD_MAX
	}ROAD;
	typedef enum
	{
		POINT_LEFT,		//���[
		POINT_RIGHT,	//�E�[
		POINT_MAX
	}POINT;
	typedef enum
	{
		TRAILS_NONE,		//�ǂ���ł��Ȃ��R�[�X
		TRAILS_IN,		//�C���R�[�X
		TRAILS_OUT,		//�A�E�g�R�[�X
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
	CRoad_Pointer	*&SetKiller(D3DXVECTOR3 &pos, int &nMap);
	CRoad_Pointer	*&SearchKiller(D3DXVECTOR3 &pos, float &fDistance, CRoad_Pointer *&pPoint);

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

	static	float			NextRot(D3DXVECTOR3 &pos, CRoad_Pointer *&pmyPoint, float &fRoad, int &nMap, int nNumber, CCOL_MESH::EFFECT &effect);
	static	float			BentRot(D3DXVECTOR3 &pos, CRoad_Pointer *&pPoint, float &fRot, float &fDistance, float &fRoad);
	static	float			BentRotX(D3DXVECTOR3 &pos, CRoad_Pointer *&pPoint, float &fRot, float &fDistance);

private://*****************************************************************************
	static bool	Beyond(CPlayer *&pPlayer, CRoad_Pointer *&pmyPoint, int &nNumber, bool &bRank, bool &bGoal);
	static bool	Reverse(CPlayer *&pPlayer, CRoad_Pointer *&pmyPoint, int &nNumber);
	//�ϐ��錾//***********************************************************************
	int				m_nNumber;					//�|�C���g�����ԍ�
	int				m_nNumRoad;					//�|�C���g�����ԍ�
	D3DXVECTOR3		m_pos;						//���S
	D3DXVECTOR3		m_Point[POINT_MAX];			//�[�̈ʒu
	TRAILS			m_Trails[POINT_MAX];		//�C���A�E�g����p
	float			m_fNextDistance[ROAD_MAX];	//���̃|�C���g�܂ł̋���
	CRoad_Pointer	*m_pNextPointer[ROAD_MAX];	//���̃|�C���g	����������
	CRoad_Pointer	*m_pRoadPointer[ROAD_MAX];				//�O�̃|�C���^
};
//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CRoad_Manager
{
public:
	typedef enum
	{
		MAP_Half0,	//�}�b�v����
		MAP_Bridge,	//��
		MAP_Half1,	//�}�b�v����
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
	void	SetDistance(int &nNumber, int &nNumPoint, float fDistance, bool &bGoal);
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
		//�ϐ��錾//***********************************************************************
	static CRoad_Manager	*m_pRoadManager;
	CRoad_Pointer	*m_pTop[MAX_RPOINT][MAX_TOP];		//�ŏ��̃|�C���g
	CRoad_Pointer	*m_pCur[MAX_RPOINT][MAX_TOP];		//�ŏ��̃|�C���g

	CRoad_Pointer	*m_pWKTop;				//�ŏ��̃|�C���g
	bool			m_bWKData;				//��ƒ�����
	int				m_nWKData;				//��ƒ��ԍ�
	int				m_nWKRank;				//��ƒ��ԍ�

	int				m_nNumAll[MAX_RPOINT][MAX_TOP];		//�|�C���g�ԍ�
	float			m_fDistance[MAX_RACER];	//�|�C���g�܂ł̂��ꂼ��̋���
	int				m_nNumPoint[MAX_RACER];	//�|�C���g�܂ł̂��ꂼ��̋���
	int				m_nRanking[MAX_RACER];	//����
};
#endif
