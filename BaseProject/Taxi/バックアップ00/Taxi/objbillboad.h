//=============================================================================
//
// �I�u�W�F�N�g�r���{�[�h���� [objbillboad.h]
// Author : ���� ����	Sato Asumi
//
//=============================================================================
#ifndef _OBJBILLBOAD_H_
#define _OBJBILLBOAD_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define OBJBILL_TEX	(2)

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CObjBillboad : public CScene
{
public:
	typedef enum
	{
		TYPE_KONE = 0,
		TYPE_WOOD,
		TYPE_MAX,
	}TYPE;

	CObjBillboad();
	~CObjBillboad();
	static CObjBillboad * Create(D3DXVECTOR3 pos, int nObjType);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetModelInfo(D3DXVECTOR3 Scale, int ModelTex, int nCollision);

	//---------------------
	// Load & Unload
	//---------------------
	static void LoadTexture(void);
	static void UnloadTexture(void);

private:
	void SetTexNum(int nObjType);
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };
	static LPDIRECT3DTEXTURE9 m_pTexture[OBJBILL_TEX];
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// ���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX				m_mtxWorld;			// ���[���h�}�g���b�N�X
	int m_nTexNum;								//�r���{�[�h�̃e�N�X�`���ԍ�
	int m_nCollision;							//���f���̂����蔻��
	int m_nModelNumTex;							//���f���̃e�N�X�`���ԍ�
	int m_nObjType;								//�I�u�W�F�N�g�̃^�C�v
	D3DXVECTOR3 m_pos;							//�ʒu
	D3DXVECTOR2 m_size;							//�r���{�[�h�̃T�C�Y
	D3DXVECTOR3 m_ModelSize;					//���f���̃T�C�Y
	D3DXVECTOR3 m_ModelPos;						//���f���̈ʒu
	float m_fDrawLength;						//�`��͈�
	float m_fModelLength;						//���f���\���͈�
	bool m_bSetModel;							//���f����\�����邩�ǂ���
};
#endif