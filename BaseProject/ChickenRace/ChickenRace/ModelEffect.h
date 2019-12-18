//=============================================================================
//
// ���f�����o���� [ModelEffect.h]
// Author : Ryo Sugimoto
//
//=============================================================================
#ifndef _MODELEFFECT_H_
#define _MODELEFFECT_H_

#include "model3D.h"
#include "2DPolygon.h"
//*****************************************************************************
// �}�N���̒�`
//*****************************************************************************
#define SMOKE_TIME	(30.0f)
#define WATER_TIME	(30.0f)
#define WATER_G		(0.1f)
//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CModelEffect : public CModel3D
{
public:
	typedef enum
	{
		TYPE_SMOKE,		//�y��
		TYPE_GRASS,		//��
		TYPE_WATER,		//�����Ԃ�
		TYPE_WATER_S,	//�����Ԃ���
		TYPE_MAX
	}TYPE;
	typedef enum
	{
		STATE_NORMAL,	//�ʏ�
		STATE_LOCK,		//�Ǐ]
		STATE_MAX
	}STATE;
	CModelEffect(int nPriority = 4) : CModel3D::CModel3D(nPriority) {};
	~CModelEffect() {};

	static	CModelEffect	*Create(D3DXVECTOR3 *pos, D3DXVECTOR3 &move, TYPE type, STATE state = STATE_NORMAL);
	void	Set(D3DXVECTOR3 *&pos, D3DXVECTOR3 &move, TYPE &type, STATE &state);
	HRESULT Init(void);
	void	Update(void);
	void	Draw(void);
private://*****************************************************************************
		//�ϐ��錾//***********************************************************************
	D3DXVECTOR3		*m_pPos;
	D3DXVECTOR3		m_move;
	D3DXVECTOR3		m_MoveRot;
	C2D::DRAW_TYPE	m_DrawType;		// �`�悷����@(���Z�����Ȃ�)

	TYPE	m_Type;			//�^�C�v
	STATE	m_State;			//���
	float	m_fCntState;	//��ԊǗ��p�ϐ�
};
#endif
