//=============================================================================
//
// ���b�V������ [mesh.h]
// Author : Ryo Sugimoto
//
//=============================================================================
#ifndef _NORMESH_H_
#define _NORMESH_H_

#include "main.h"
#include "scene.h"
//*****************************************************************************
// �}�N���̒�`
//*****************************************************************************

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CNorMesh
{
public:
	typedef enum
	{
		TYPE_FIELD,		//�n��
		TYPE_WALL,		//��
		TYPE_CEILING,	//�V��
		TYPE_ORBIT,		//�I�[�r�b�g
		TYPE_CYLINDER,	//�V�����_�[
		TYPE_MAX
	}TYPE;
	CNorMesh() {};
	~CNorMesh() {};

	static bool	Create(CNorMesh *&pNorMesh);
	void	Set(TYPE type, D3DXVECTOR3 pos, int nVertical, int nCorss, bool bMesh);	//�Z�b�g����
	void	SetNor(void);	//�@���̃Z�b�g����
	void	SetMtx(void);

	HRESULT Init(void);
	void	Uninit(void);
	void	Draw(void);
	void		BindTexture(LPDIRECT3DTEXTURE9 pTex) { m_pTex = pTex; }

	D3DXMATRIX  &GetMtx(void)	{ return m_mtxWorld; }
	D3DXVECTOR3 &GetRot(void)	{ return m_rot; }
	D3DXVECTOR3 &GetPos(void)	{ return m_pos;}
	int &GetnCross(void)		{ return m_nCross; }
	int &GetnVertical(void)		{ return m_nVertical; }
	int &GetnNumVertex(void) { return m_nNumVertex; }
	LPDIRECT3DVERTEXBUFFER9 &GetpVtxBuff(void) { return m_pVtxBuff; }
protected://*****************************************************************************
	LPDIRECT3DVERTEXBUFFER9 &GetVtxBuff(void) { return m_pVtxBuff; }
private://*****************************************************************************
	void PlusNor(VERTEX_3D *pVtx, VERTEX_3D *pVtxL, VERTEX_3D *pVtxR);	//�@���̃Z�b�g����
		//�ϐ��錾//***********************************************************************
	TYPE					m_Type;			//�^�C�v
	D3DXMATRIX				m_mtxWorld;		//���[���h�}�g���b�N�X
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//���_�o�b�t�@
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;		//�C���f�b�N�X�o�b�t�@�̃|�C���^
	LPDIRECT3DTEXTURE9		m_pTex;		//�摜
	D3DXVECTOR3				m_pos;			//�ʒu
	D3DXVECTOR3				m_rot;			//����
	int						m_nVertical;	//�c�̕�����
	int						m_nCross;		//���̕�����
	int						m_nNumVertex;	//���_��
	int						m_nNumIdx;		//�C���f�b�N�X��
	int						m_nNumPolygon;	//�|���S����
	bool					m_bMesh;		//�����Ă���ʂ��������O����(����肩�E��肩)
};
#endif
