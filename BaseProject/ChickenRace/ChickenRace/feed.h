//=============================================================================
//
// �a�̏��� [feed.h]
// Author : ���R���
//
//=============================================================================
#ifndef _FEED_H_
#define _FEED_H_

#include "model3D.h"
#include "camerabace.h"
#include "shadow.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FEED_PRIOTITY				(3)			// �H�ו��̗D�揇��
#define MAX_FEED					(3)			// �H�ו��̃��f����
#define MAX_FEED_TEXTURE			(3)			// �H�ו��̃e�N�X�`����

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CObjBillboad;

//=====================
//  CModel3D�̔h���N���X
//=====================
class CFeed : public CModel3D
{
public:
	typedef enum
	{// �a�̎��
		FEEDTYPE_ATTACK = 0,	// �U��
		FEEDTYPE_ANNOY,			// �W�Q
		FEEDTYPE_SPEED,			// ����
		FEEDTYPE_MAX,			//�ő吔
	}FEEDTYPE;

	CFeed();
	~CFeed();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CFeed *Create(D3DXVECTOR3 pos, int nZone, int nType);
	bool CollisionFeed(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld);
	FEEDTYPE GetFeedType(void) { return m_feedType; }

private:
	static D3DXVECTOR3 m_VtxMaxModel[FEEDTYPE_MAX];		//���f���̍ő�l
	static D3DXVECTOR3 m_VtxMinModel[FEEDTYPE_MAX];		//���f���̍ŏ��l

	int					m_nZone;						// �]�[��
	FEEDTYPE			m_feedType;						// �a�̎��
	D3DXVECTOR3			m_scale;						// �傫��
	D3DXVECTOR3			m_rot;							// ��]
	CShadow				*m_pShadow;						// �e�̃|�C���^
	CObjBillboad *		m_pObjBill;						// �I�u�W�F�N�g�r���{�[�h�̃|�C���^
};
#endif