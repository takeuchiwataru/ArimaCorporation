//=============================================================================
//
// �Ђ悱�̏��� [chick.h]
// Author : ���R���
//
//=============================================================================
#ifndef _CHICK_H_
#define _CHICK_H_

#include "model3D.h"
#include "camerabace.h"
#include "shadow.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CHICK_PRIOTITY				(3)
#define CHICK_RANGE		(50.0f)										// �Ђ悱�ƃL�����N�^�[�̋���
#define CHICK_POS			(7)											// �Ђ悱���m�̊Ԋu�̍L���i���₷�ƍL���Ȃ�j
#define CHICK_BOOST			(2.0f)										// ���������
#define MAX_CHICK		(3)												// �Ђ悱�̍ő吔

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CObjBillboad;
class CPlayer;

//=====================
//  CModel3D�̔h���N���X
//=====================
class CChick : public CModel3D
{
public:
	typedef enum
	{// �Ђ悱�̎��
		TYPE_ATTACK = 0,	// �U��
		TYPE_ANNOY,			// �W�Q
		TYPE_SPEED,			// ����
		TYPE_ATTACK_S,		// �����U��
		TYPE_ANNOY_S,		// �����W�Q
		TYPE_SPEED_S,		// ��������
		TYPE_MAX,			//�ő吔
	} TYPE;

	typedef enum
	{// �Ђ悱�̏��
		STATE_NORMAL = 0,	// �ʏ�
		STATE_CHASE,			// ���Ă���
		STATE_BULLET,		// ���ł���
		STATE_MAX,			//�ő吔
	} STATE;

	typedef enum
	{// �Ђ悱�̏��
		BULLETTYPE_NONE = 0,
		BULLETTYPE_PLAYER,
		BULLETTYPE_ENEMY,
		BULLETTYPE_MAX,
	} BULLETTYPE;

	CChick();
	~CChick();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CChick *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type, BULLETTYPE bulletType, STATE state, int nNumPlayer);
	bool CollisionChick(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld);
	float SetHeight(void);
	STATE GetState(void) { return m_state; }
	void SetState(STATE state) { m_state = state; }
	TYPE GetType(void) { return m_type; }
	BULLETTYPE GetBulletType(void) { return m_bulletType; }
	int GetNumPlayer(void) { return m_nNumPlayer; }
	void SetRank(int nRank) { m_nRank = nRank; }
	int GetRank(void) { return m_nRank; }
	void SetDis(bool bDis) { m_bDis = bDis; }
	bool GetDis(void) { return m_bDis; }
	void Jump(void);
	void Bullet(void);
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	bool GetAttackS(void) { return m_bAttackS; }
	void SetAttackS(bool bAttackS) { m_bAttackS = bAttackS; }
	void SetDestRank(int nDestRank) { m_DestRank = nDestRank; }

private:
	void Move(void);
	void Attack(void);
	void AnnoyS(void);
	void AttackS(void);
	void Item(void);
	void AdjustAngle(float rot);
	static LPD3DXMESH	m_pMeshModel;			//���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER m_pBuffMatModel;		//�}�e���A���̏��ւ̃|�C���^
	static DWORD		m_nNumMatModel;			//�}�e���A���̏��
	static LPDIRECT3DTEXTURE9 m_pMeshTextures;	//�e�N�X�`�����
	static D3DXVECTOR3 m_VtxMaxModel;			//���f���̍ő�l
	static D3DXVECTOR3 m_VtxMinModel;			//���f���̍ŏ��l
	static int			m_nChickTimer;
	BULLETTYPE			m_bulletType;			// �e�̎��
	TYPE				m_type;				// �Ђ悱�̎��
	STATE				m_state;				// �Ђ悱�̏��
	D3DXVECTOR3			m_scale;				// �傫��
	D3DXVECTOR3			m_rot;					// ��]
	CShadow				*m_pShadow;				// �e�̃|�C���^
	CObjBillboad *		m_pObjBill;				// �I�u�W�F�N�g�r���{�[�h�̃|�C���^
	float				m_fHeight;
	D3DXVECTOR3			m_move;
	D3DXVECTOR3			m_pos;
	bool				m_bJump;				// �W�����v���Ă��邩�ǂ���
	bool				m_bDis;					// �������ǂ���
	bool				m_bAttackS;
	D3DXVECTOR3			m_fDestAngle;			// �ړI�̊p�x
	D3DXVECTOR3			m_fDiffAngle;			// ����
	int					m_nRank;				// �������Ă�������
	int					m_nNumPlayer;			// �v���C���[���������Ă���Ђ悱��
	int					m_nDisTimer;			// �����܂ł̎���
	int					m_DestRank;			// �ړI�̏���
};
#endif