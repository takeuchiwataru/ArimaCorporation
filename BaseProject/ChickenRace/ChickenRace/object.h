//=============================================================================
//
// �I�u�W�F�N�g���� [object.h]
// Author : �L�n���u
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "model3D.h"
#include "camerabace.h"
#include "shadow.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_OBJECT					(7)				// �I�u�W�F�N�g�̃��f����
#define MAX_OBJECT_TEXTURE			(7)				// �I�u�W�F�N�g�̃e�N�X�`����
#define OBJECT_PRIOTITY				(3)				// �I�u�W�F�N�g�̗D�揇��
#define PLAYER_HEIGHT			(80.0f)				// �v���C���[�̔w�̍���
#define PLAYER_DEPTH			(20.0f)				// �v���C���[�̕������p

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CLoadEffect;
class CObjBillboad;

//=====================
//  CModel3D�̔h���N���X
//=====================
class CObject : public CModel3D
{
public://�N�ł�������
	CObject(int nPriority = OBJECT_PRIOTITY);
	~CObject();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CObject *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, float move, int nTexTypeint, int nObjectType, int nCollision);
	void DeleteObject(void);
	int GetType(void) { return m_nType; };
	D3DXVECTOR3 Fountain(D3DXVECTOR3 pos, D3DXVECTOR3 move);
	bool CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);

private://�l�ł̂ݎg��
	static D3DXVECTOR3 m_LoadVtxMaxModel[MAX_OBJECT];				//���f���̍ő�l
	static D3DXVECTOR3 m_LoadVtxMinModel[MAX_OBJECT];				//���f���̍ŏ��l

	D3DXVECTOR3			m_scale;									// �T�C�Y
	D3DXVECTOR3			m_Spin;										// ��]
	int					m_nCount;									// �J�E���^�[
	int					m_nCollision;								// �����蔻���ONOFF
	int					m_nType;									// ���
	int					m_nTex;										// �e�N�X�`���ԍ�
	D3DXVECTOR3			m_ModelMove;								// �ړ��̎d��
	int					m_nStageCount;								// �X�e�[�W�̈ړ����ɏ������Ԃ�������
	bool				m_bStageNext;								// �X�e�[�W�J�ڒ�
	CShadow				*m_pShadow;									// �e�̃|�C���^
	CObjBillboad *		m_pObjBill;									// �I�u�W�F�N�g�r���{�[�h�̃|�C���^
};
#endif