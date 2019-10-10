//=============================================================================
//
// �G�߃G�t�F�N�g�̏��� [seasoneffect.h]
// Author : shun yokomichi
//
//=============================================================================
#ifndef _SEASONEFFECT_H_
#define _SEASONEFFECT_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RADIUS_CONDIUIONS	(20)								// ���a�̑傫���̏����l
//=====================
//  CScene2D�̔h���N���X
//=====================
class CSeasonEffect : public CScene2D
{
public://�N�ł�������
	typedef enum
	{
		SEASONTYPE_NONE = 0,		//�����Ȃ����
		SEASONTYPE_SPRING,			// �t
		SEASONTYPE_SUMMER,			// ��
		SEASONTYPE_FALL,			// �H
		SEASONTYPE_WINTRE,			// �~
		SEASONTYPE_MAX				//�G�t�F�N�g�̍ő吔
	}SEASONTYPE;

	CSeasonEffect(int nPriorityv = 4);
	~CSeasonEffect();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CSeasonEffect *Create(int Type,D3DXVECTOR3 pos,int nRadius,int nTexType);

private://�l�ł̂ݎg��
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;						// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3					m_pos;							// �ʒu
	D3DXVECTOR3					m_posOld;						// �ߋ��̈ʒu
	D3DXVECTOR2					m_size;							// �T�C�Y
	D3DXVECTOR3					m_rot;							// ����
	float						m_fRadius;						// ���a(�傫��)
	float						m_fAngle;						// �p�x
	float						m_fLength;						// ����
	int							m_nType;						// ���
	SEASONTYPE					m_SeasonType;					// �G�߂̎��
	int							m_nCntEffectState;				// ��ԃJ�E���^
	int							m_nCountTime;					// �b���J�E���g
	static int					m_nCntEffect;
};
#endif