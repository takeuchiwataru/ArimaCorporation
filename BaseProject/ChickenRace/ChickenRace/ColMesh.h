//=============================================================================
//
// ����t�����b�V������ [ColMesh.h]
// Author : Ryo Sugimoto
//
//=============================================================================
#ifndef _COLMESH_H_
#define _COLMESH_H_

#define _CRT_SECURE_NO_WARNINGS	//Fire Wall�˔j
#include <stdio.h>				//�C���N���h�t�@�C��
#include "main.h"
#include "scene.h"
#include "NorMesh.h"
class CCOL_MESH_MANAGER;
class CPlayer;
//*****************************************************************************
// �}�N���̒�`
//*****************************************************************************
#define COLMESH_PRIORITY	(1)	//�D�揇��
#define COLMESH_MODE		(0)	//0�Ȃ�c�[��
#define MAX_COL_MESH		(64)
#define INIT_LENGTH			(100.0f)

//*****************************************************************************
// �N���X�̒�`
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
		EFFECT_GRASS,	//�_�[�g
		EFFECT_SWAMP,	//�����܂�
		EFFECT_RIVER,	//��
		EFFECT_BOOST,	//������
		EFFECT_DROP,	//����������
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

	bool	MeshField(CPlayer *&pPlayer);
	bool	GetHeight(D3DXVECTOR3 pos, float &fHeight);
	bool	FieldCheck(D3DXVECTOR3 &FposUL, D3DXVECTOR3 &FposUR, D3DXVECTOR3 &FposDL, D3DXVECTOR3 &FposDR, D3DXVECTOR3& pos);
	float	FieldCollision(D3DXVECTOR3 &FposUL, D3DXVECTOR3 &FposUR, D3DXVECTOR3 &FposDL, D3DXVECTOR3 &FposDR, D3DXVECTOR3 WKpos, D3DXVECTOR3& pos, D3DXVECTOR3 &FNor);
	void	MeshWall(CPlayer *&pPlayer, int &nCntHit, bool bReflection);
	int		WallCollision(D3DXVECTOR3 Wpos0, D3DXVECTOR3 Wpos1, D3DXVECTOR3 WUpos0, D3DXVECTOR3 WUpos1, CPlayer *&pPlayer, int &nCntHit, bool &bReflection);
	static int	AngleCheck(float fAngle0, float fAngle1);
	static bool	CrossCheck(D3DXVECTOR3 &Wpos0, D3DXVECTOR3 &Wpos1, D3DXVECTOR3 &pos, D3DXVECTOR3 &posold);
	TYPE		&GetType(void) { return m_Type; }
	EFFECT		&GetEffect(void) { return m_Effect; }
	int			&GetnNumVtx(void) { return m_nNumVtx; }
	int			&GetnNumWidth(void) { return m_nNumWidth; }
	int			&GetnNumVertical(void) { return m_nNumVertical; }
	D3DXVECTOR3 *&GetVtx(void) { return m_pVtx; }
	D3DXVECTOR3 &Getpos(void) { return m_pos; }

private://*****************************************************************************
		//�ϐ��錾//***********************************************************************
	TYPE			m_Type;				//���b�V���^�C�v
	EFFECT			m_Effect;			//����
	D3DXVECTOR3		m_pos;				//�ʒu
	int				m_nNumVtx;			//���_��
	int				m_nNumVertical;		//�c
	int				m_nNumWidth;		//��
	D3DXVECTOR3		*m_pVtx;			//���_
	bool			*m_bSelect;			//�I��
	bool			m_bField;			//�n�ʂ��ǂ�

	CNorMesh		*m_pMesh;
};
//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CCOL_MESH_MANAGER : public CScene
{
public:
	typedef enum
	{
		//TYPE_MADE,
		TYPE_HALF0,		//�n�߂̔���
		TYPE_BRIDGE,	//��
		TYPE_HALF1,		//�I���̔���
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
	void	ResetCollar(void);
	static bool	Collision(CPlayer *pPlayer);
	static float	CCOL_MESH_MANAGER::GetHeight(D3DXVECTOR3 &pos, int &nMap);

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
	static void	WNumCollision(CPlayer *&pPlayer);
	static bool	FNumCollision(CPlayer *&pPlayer);
	//�ϐ��錾//***********************************************************************
	static CCOL_MESH_MANAGER *m_pColMesh[TYPE_MAX];

	TYPE		m_Type;
	int			m_nNumField;
	int			m_nNumWall;
	CCOL_MESH	**m_pField;
	CCOL_MESH	**m_pWall;
};
#endif