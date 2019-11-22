//=============================================================================
//
// ���f�����o���� [ModelEffect.h]
// Author : Ryo Sugimoto
//
//=============================================================================
#ifndef _MODELEFFECT_H_
#define _MODELEFFECT_H_

#include "model3D.h"
//*****************************************************************************
// �}�N���̒�`
//*****************************************************************************

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CModelEffect : public CModel3D
{
public:
	typedef enum
	{
		TYPE_SMOKE,		//�y��
		TYPE_MAX
	}TYPE;
	typedef enum
	{
		STATE_NORMAL,	//�ʏ�
		STATE_LOCK,		//�Ǐ]
		STATE_MAX
	}STATE;
	CModelEffect() {};
	~CModelEffect() {};

	static	CModelEffect	*Create(D3DXVECTOR3 *pos, TYPE type, STATE state = STATE_NORMAL);
	void	Set(D3DXVECTOR3 *&pos, TYPE &type, STATE &state);
	HRESULT Init(void);
	void	Update(void);
	void	Draw(void);
private://*****************************************************************************
		//�ϐ��錾//***********************************************************************
	D3DXVECTOR3		*m_pPos;
	D3DXVECTOR3		m_move;
	TYPE	m_Type;			//�^�C�v
	STATE	m_State;			//���
	float	m_fCntState;	//��ԊǗ��p�ϐ�
};
#endif
