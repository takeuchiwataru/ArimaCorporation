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
#include "manager.h"
//*****************************************************************************
// �}�N���̒�`
//*****************************************************************************
#define Cylinder_PRIORITY	(4)
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
	void	SetCylinder(float fLengthY, float fLengthXZU, float fLengthXZD
		, D3DXCOLOR col = INIT_COL, D3DXVECTOR2 UV0 = D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2 UV1 = D3DXVECTOR2(0.1f, 1.0f));	//�Z�b�g����
	void	SetNor(void);	//�@���̃Z�b�g����
	void	SetMtx(void);

	HRESULT Init(void);
	void	Uninit(void);
	void	Draw(void);
	void		BindTexture(LPDIRECT3DTEXTURE9 pTex) { m_pTex = pTex; }

	D3DXMATRIX  &GetMtx(void) { return m_mtxWorld; }
	D3DXVECTOR3 &GetRot(void) { return m_rot; }
	D3DXVECTOR3 &GetPos(void) { return m_pos; }
	int &GetnCross(void) { return m_nCross; }
	int &GetnVertical(void) { return m_nVertical; }
	int &GetnNumVertex(void) { return m_nNumVertex; }
	int &GetnNumDraw(void) { return m_nNumDraw; }
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
	int						m_nNumDraw;		//�`��֐�
	bool					m_bMesh;		//�����Ă���ʂ��������O����(����肩�E��肩)
};
//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CCylinder : public CNorMesh, public CScene
{
public:
	typedef enum
	{
		TYPE_BOOST,		//�u�[�X�g
		TYPE_RARE,		//���A�s���R
		TYPE_EATK,		//���U��
		TYPE_HATK,		//�q���R�U��
		TYPE_HATK_S,	//���q���R�U��
		TYPE_HATK_SC,	//���q���R�U�� �L�����x���p
		TYPE_HATK_SH,	//���q���R�U�� �͈͌x���p
		TYPE_HDEF,		//�q���R�W�Q
		TYPE_MAX
	}TYPE;
	typedef enum
	{
		STATE_NORMAL,	//�ʏ�
		STATE_SPREAD,	//�L����
		STATE_FADE,		//�����Ă���
		STATE_TIME,		//���Ԍo�߂Ńt�F�[�h
		STATE_END,		//�I���
		STATE_MAX
	}STATE;
	CCylinder(int nPriority = Cylinder_PRIORITY) : CScene(nPriority) {};
	~CCylinder() {};

	static CCylinder	*Create(D3DXVECTOR3 &pos, D3DXVECTOR3 &rot, TYPE type);
	void	Set(D3DXVECTOR3 &pos, D3DXVECTOR3 &rot, TYPE type);	//�Z�b�g����
	void	SetState(STATE state);	//��Ԑݒ菈��

	HRESULT Init(void);
	void	Update(void);
	void	Uninit(void);
	void	Draw(void);

private://*****************************************************************************
		//�ϐ��錾//***********************************************************************
	TYPE	m_Type;
	STATE	m_State;		//�I������
	D3DXVECTOR3 *m_pPos;	//�Q�Ƃ���ʒu
	D3DXVECTOR3 *m_pRot;	//�Q�Ƃ���p�x
	D3DXVECTOR3 m_AddPos;	//���Z�ʒu
	D3DXVECTOR3 m_AddRot;	//���Z�p�x
	D3DXCOLOR	m_col;		//�����x

	float	m_fUV_X;		//X�ɂ��炷UV�̒l
	float	m_fPlusA;		//�����x������
	float	m_fCntState;	//��ԊǗ��p
	float	m_fCntTime;	//���ԊǗ��p
	float	m_fLengthY;		//����Y
	float	m_fLengthXZ[2];	//����XZ	0:��	1:��

	int		m_nPtn;
	int		m_nSpeed;
	int		m_nCntSpd;
};
#endif
