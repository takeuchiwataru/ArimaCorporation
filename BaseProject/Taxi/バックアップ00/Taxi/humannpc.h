//=============================================================================
//
// �lNPC�̏��� [humannpc.h]
// Author : �������� Sato_Asumi
//
//=============================================================================
#ifndef _CHUMANNPC_H_
#define _CHUMANNPC_H_

#include "main.h"
#include "humanbace.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_PARTS	(7)		//�ǂݍ��ރp�[�c��

//=============================================================================
// �lNPC�N���X
//=============================================================================
class CHumanNPC : public CHumanBace
{
public:
	CHumanNPC();
	~CHumanNPC();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CHumanNPC * Create(D3DXVECTOR3 pos, MODEL_TYPE type);

private:
	void Escape(void);	//�����鏈��

	bool m_bEscape;		//���������ǂ���
};
#endif;