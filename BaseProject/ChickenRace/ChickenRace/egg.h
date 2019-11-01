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
#define EGG_PRIOTITY	(3)
#define EGG_SCALE		(10.0f)										//���̑傫��
#define EGG_RANGE		(50.0f)										// ���ƃL�����N�^�[�̋���
#define EGG_POS			(7)											// �����m�̊Ԋu�̍L���i���₷�ƍL���Ȃ�j
#define SPEED			(2.0f)										// ���������
#define MAX_EGG			(3)											//���̍ő吔
#define HATCHING_TIME	(600)										// �z������܂ł̎���
#define EGGJUMP			(5.5f)										// ���̃W�����v��

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
	} EGGTYPE;

	typedef enum
	{// ���̏��
		EGGSTATE_NORMAL = 0,	// �ʏ�
		EGGSTATE_CHASE,			// ���Ă���
		EGGSTATE_BULLET,		// ���ł���
		EGGSTATE_MAX,			//�ő吔
	} EGGSTATE;

	typedef enum
	{// ���̏��
		BULLETTYPE_NONE = 0,
		BULLETTYPE_PLAYER,
		BULLETTYPE_ENEMY,
		BULLETTYPE_MAX,
	} BULLETTYPE;

	CEgg();
	~CEgg();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CEgg *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, EGGTYPE eggType, BULLETTYPE bulletType, int nNumPlayer);
	bool CollisionEgg(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld);
	float SetHeight(void);
	EGGSTATE GetState(void) { return m_eggState; }
	void SetState(EGGSTATE state) { m_eggState = state; }
	EGGTYPE GetType(void) { return m_eggType; }
	BULLETTYPE GetBulletType(void) { return m_bulletType; }
	int GetNumPlayer(void) { return m_nNumPlayer; }
	void SetRank(int nRank) { m_nRank = nRank; }
	int GetRank(void) { return m_nRank; }
	int GetHatchingTimer(void) { return m_nHatchingTimer; }
	void SetHatchingTimer(int nTimer) { m_nHatchingTimer = nTimer; }
	void Jump(float fJump);
	void Bullet(void);

private:
	static LPD3DXMESH	m_pMeshModel;			//���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER m_pBuffMatModel;		//�}�e���A���̏��ւ̃|�C���^
	static DWORD		m_nNumMatModel;			//�}�e���A���̏��
	static LPDIRECT3DTEXTURE9 m_pMeshTextures;	//�e�N�X�`�����
	static D3DXVECTOR3 m_VtxMaxModel;			//���f���̍ő�l
	static D3DXVECTOR3 m_VtxMinModel;			//���f���̍ŏ��l

	BULLETTYPE			m_bulletType;			// �e�̎��
	EGGTYPE				m_eggType;				// ���̎��
	EGGSTATE			m_eggState;				// ���̏��
	D3DXVECTOR3			m_scale;				// �傫��
	D3DXVECTOR3			m_rot;					// ��]
	CShadow				*m_pShadow;				// �e�̃|�C���^
	CObjBillboad *		m_pObjBill;				// �I�u�W�F�N�g�r���{�[�h�̃|�C���^
	float				m_fHeight;
	D3DXVECTOR3			m_move;
	bool				m_bJump;				// �W�����v���Ă��邩�ǂ���
	float				m_fDestAngle;			// �ړI�̊p�x
	float				m_fDiffAngle;			// ����
	int					m_nRank;				// �������Ă�������
	int					m_nNumPlayer;			// �v���C���[���������Ă��闑��
	int					m_nHatchingTimer;		// �z������܂ł̎���
};
#endif