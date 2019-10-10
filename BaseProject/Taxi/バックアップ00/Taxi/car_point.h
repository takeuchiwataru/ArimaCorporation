//=============================================================================
//
// �Ԃ̓����|�C���g�̏��� [car_point.h]
// Author : YUTARO�@ABE
//
//=============================================================================
#ifndef _CAR_POINT_H_
#define _CAR_POINT_H_

#include "model3D.h"
#include "camerabace.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_CARPOINT						(1)			// �Ԃ̓����|�C���g�̃��f����
#define MAX_CARPOINT_TEXTURE				(1)			// �Ԃ̓����|�C���g�̃e�N�X�`����
#define CARPOINT_PRIOTITY					(2)

//=====================
//  CModel3D�̔h���N���X
//=====================
class CCarPoint : public CModel3D
{
public://�N�ł�������
	CCarPoint();
	~CCarPoint();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CCarPoint *Create(int nNum,D3DXVECTOR3 pos);

	int GetObjNumber(void) { return m_nObjNumber; }
;
private://�l�ł̂ݎg��
	static LPD3DXMESH	m_pMeshModel[MAX_CARPOINT];					//���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER m_pBuffMatModel[MAX_CARPOINT];				//�}�e���A���̏��ւ̃|�C���^
	static DWORD		m_nNumMatModel[MAX_CARPOINT];				//�}�e���A���̏��
	static LPDIRECT3DTEXTURE9 m_pMeshTextures[MAX_CARPOINT_TEXTURE];
	static D3DXVECTOR3 m_LoadVtxMaxModel[MAX_CARPOINT];				//���f���̍ő�l
	static D3DXVECTOR3 m_LoadVtxMinModel[MAX_CARPOINT];				//���f���̍ŏ��l
	int					m_nObjNumber;								// �I�u�W�F�N�g�̔ԍ�
	bool				m_bLook;									// ���邩�ǂ���
};
#endif