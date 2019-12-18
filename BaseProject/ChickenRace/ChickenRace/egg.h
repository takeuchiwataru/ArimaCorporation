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
#define EGG_PRIOTITY	(0)
#define EGG_SCALE		(1.0f)										//���̑傫��
#define EGG_POS			(7)											// �����m�̊Ԋu�̍L���i���₷�ƍL���Ȃ�j
#define SPEED			(0.5f)										// ���������
#define MAX_EGG			(3)											//���̍ő吔
#define MAX_EGG_TEXTURE		(3)										// �I�u�W�F�N�g�̃e�N�X�`����
#define HATCHING_TIME	(600)										// �z������܂ł̎���
#define EXPLOSION_TIME	(35)		// �������Ă��鎞��

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
	EGGSTATE GetState(void) { return m_eggState; }
	void SetState(EGGSTATE state) { m_eggState = state; }
	EGGTYPE GetType(void) { return m_eggType; }
	BULLETTYPE GetBulletType(void) { return m_bulletType; }
	int GetNumPlayer(void) { return m_nNumPlayer; }
	void SetRank(int nRank) { m_nRank = nRank; }
	int GetRank(void) { return m_nRank; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	int GetHatchingTimer(void) { return m_nHatchingTimer; }
	void SetHatchingTimer(int nTimer) { m_nHatchingTimer = nTimer; }
	void Jump(float fJump);
	void Bullet(void);
	void SetThrow(bool bThrow) { m_bThrow = bThrow; }
	float GetHeight(void) { return m_fHeight; }
	void SetDis(bool bDis) { m_bDis = bDis; }
	bool GetDis(void) { return m_bDis; }
	void SetJump(bool bJump) { m_bJump = bJump; }

private:
	void Item(void);
	void Move(void);
	float AdjustAngle(float rot);
	static D3DXVECTOR3 m_VtxMaxModel[MAX_EGG];	//���f���̍ő�l
	static D3DXVECTOR3 m_VtxMinModel[MAX_EGG];	//���f���̍ŏ��l

	BULLETTYPE			m_bulletType;			// �e�̎��
	EGGTYPE				m_eggType;				// ���̎��
	EGGSTATE			m_eggState;				// ���̏��
	D3DXVECTOR3			m_scale;				// �傫��
	D3DXVECTOR3			m_rot;					// ��]
	CShadow				*m_pShadow;				// �e�̃|�C���^
	CObjBillboad *		m_pObjBill;				// �I�u�W�F�N�g�r���{�[�h�̃|�C���^
	float				m_fHeight;
	D3DXVECTOR3			m_move;
	D3DXVECTOR3			m_pos;
	D3DXVECTOR3			m_posOld;
	D3DXVECTOR3			m_FNor;					//�n�ʂ̖@��
	bool				m_bJump;				// �W�����v���Ă��邩�ǂ���
	bool				m_bThrow;				// ���������ǂ���
	bool				m_bExplosion;			// �͈͍U���ɂȂ邩�Ȃ�Ȃ���
	bool				m_bDis;							// �������ǂ���
	float				m_fDestAngle;			// �ړI�̊p�x
	float				m_fDiffAngle;			// ����
	float				m_fLength;				// ����
	int					m_nDisTimer;					// �����܂ł̎���
	int					m_nRank;				// �������Ă�������
	int					m_nNumPlayer;			// �v���C���[���������Ă��闑��
	int					m_nHatchingTimer;		// �z������܂ł̎���
	int					m_nExplosion;			// �������Ă��鎞��
	int					m_nMap;					//��������}�b�v
};
#endif