//=============================================================================
//
// �r���{�[�h�̏��� [billboord.h]
// Author : �L�n���u
//
//=============================================================================
#ifndef _BILLBOORD_H_
#define _BILLBOORD_H_

#include "main.h"
#include "scene.h"
#include "scene3D.h"
#include "logo.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//=====================
//  CScene�̔h���N���X
//=====================
class CBillBoord : public CScene3D
{
public://�N�ł�������
	typedef enum
	{
		BILLBOORD_NONE = 0,			//�������Ȃ����
		BILLBOORD_EXCLAMATION,		//�������[�h
		BILLBOORD_QUESTION,			//�^�⃂�[�h
		BILLBOORD_ATTACK,			//�U�����[�h
		BILLBOORD_MAX				//�ő吔
	}BILLBOORD;

	CBillBoord();
	~CBillBoord();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CBillBoord *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,int nTexType);
	void SetState(BILLBOORD state) { m_State = state; }
	void ResetBillboord(void);

private://�l�ł̂ݎg��
	static LPDIRECT3DTEXTURE9	m_pTexture[MAX_BILLBOORD_TEX];	// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;						// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3					m_pos;							// �ʒu
	D3DXVECTOR2					m_size;							// �T�C�Y
	BILLBOORD					m_State;						// ���
	float						m_bOnOff;
	float						m_fLength;
	float						m_fAngle;
	int							m_nCntShake;
};
#endif