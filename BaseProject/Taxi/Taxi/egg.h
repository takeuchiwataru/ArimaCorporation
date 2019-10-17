//=============================================================================
//
// ���̏��� [egg.h]
// Author : ���R���
//
//=============================================================================
#ifndef _EGG_H_
#define _EGG_H_

#include "model3D.h"
#include "camerabace.h"
#include "shadow.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define EGG_PRIOTITY				(3)

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CObjBillboad;
class CPlayer;

//=====================
//  CModel3D�̔h���N���X
//=====================
class CEgg : public CModel3D
{
public:
	typedef enum
	{// ���̎��
		EGGTYPE_ATTACK = 0,		// �U��
		EGGTYPE_ANNOY,			// �W�Q
		EGGTYPE_SPEED,			// ����
		EGGTYPE_MAX,			//�ő吔
	}EGGTYPE;

	CEgg();
	~CEgg();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CEgg *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, EGGTYPE eggType);
	bool CollisionEgg(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld);
	float SetHeight(void);
	void Jump(void);

private:
	static LPD3DXMESH	m_pMeshModel;			//���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER m_pBuffMatModel;		//�}�e���A���̏��ւ̃|�C���^
	static DWORD		m_nNumMatModel;			//�}�e���A���̏��
	static LPDIRECT3DTEXTURE9 m_pMeshTextures;	//�e�N�X�`�����
	static D3DXVECTOR3 m_VtxMaxModel;			//���f���̍ő�l
	static D3DXVECTOR3 m_VtxMinModel;			//���f���̍ŏ��l

	EGGTYPE				m_eggType;						// ���̎��
	D3DXVECTOR3			m_scale;						// �傫��
	D3DXVECTOR3			m_rot;							// ��]
	CShadow				*m_pShadow;						// �e�̃|�C���^
	CObjBillboad *		m_pObjBill;						// �I�u�W�F�N�g�r���{�[�h�̃|�C���^
	float				m_fHeight;
	D3DXVECTOR3			m_move;
	bool				m_bJump;				// �W�����v���Ă��邩�ǂ���
};
#endif