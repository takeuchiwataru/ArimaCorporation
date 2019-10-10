//=============================================================================
//
// �R�C���ǉ��̏��� [addcoin.h]
// Author : �L�n���u
//
//=============================================================================
#ifndef _ADDCOIN_H_
#define _ADDCOIN_H_

#include "main.h"
#include "scene2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//=====================
//  CScene2D�̔h���N���X
//=====================
class CAddCoin : public CScene2D
{
public://�N�ł�������
	CAddCoin(int nPriorityv = 6);
	~CAddCoin();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CAddCoin *Create(void);

	static void LoadTex(void);
	static void UnloadTex(void);
private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	D3DXVECTOR3 m_pos;		//�ʒu
	D3DXVECTOR3 m_move;		//�ړ���
	float m_fAngle;			//�p�x
	int	  m_nCountTime;		//�J�E���^�[
	int	  m_nPattern;		//�p�^�[��
	int   m_nLife;			//�\������
};
#endif