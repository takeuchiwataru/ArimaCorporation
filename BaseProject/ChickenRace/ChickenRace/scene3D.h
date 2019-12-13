//=============================================================================
//
// ���C������ [scene3D.h]
// Author : �L�n���u
//
//=============================================================================
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

#include "main.h"
#include "scene.h"
#include "2DPolygon.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_BILLBOORD_TEX		(2)

//=====================
//  CScene�̔h���N���X
//=====================
class CScene3D : public CScene
{
public://�N�ł�������
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

private://�l�ł̂ݎg��
	C2D::DRAW_TYPE			m_DrawType;		// �`�悷����@(���Z�����Ȃ�)
	LPDIRECT3DTEXTURE9		m_pTexture;						//�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;						//���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3				m_pos;							//�ʒu
	D3DXVECTOR3				m_rot;							//����
	D3DXVECTOR2				m_size;							//�T�C�Y
	D3DXVECTOR3				m_Move;							//����
	D3DXMATRIX				m_mtxWorld;						//���[���h�}�g���b�N�X
	TYPE					m_Type;							//�r���{�[�h�F�P���G�t�F�N�g�F�Q��
	int						m_TexType;						// �e�N�X�`���̃^�C�v
};
//=============================================================================
// �N���X��`
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
	//==�ϐ�===========================================================================
	D3DXVECTOR3	m_Initpos;		//�����ʒu
	D3DXCOLOR	m_col;			//�F
	TYPE		m_Type;			//���
	float		m_fCntState;	//��ԊǗ��p
};
//=============================================================================
// �N���X��`
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
	//==�ϐ�===========================================================================
	ANIM	m_Anim;
	int		m_nPtn, m_nPtnX, m_nPtnY;	//�A�j���[�V�������
	int		m_nSpeed, m_nCntAnim;	//�A�j���[�V�����Ǘ��p
};
#endif