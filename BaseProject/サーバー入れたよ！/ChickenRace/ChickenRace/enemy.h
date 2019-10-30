//=============================================================================
//
// �G�̏����i���j [enemy.h]
// Author : ���R���
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "model3D.h"
#include "camerabace.h"
#include "shadow.h"
#include "egg.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ENEMY_PRIOTITY				(3)

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CObjBillboad;

//=====================
//  CModel3D�̔h���N���X
//=====================
class CEnemy : public CModel3D
{
public:
	typedef enum
	{// �a�̎��
		ENEMYSTATE_NORMAL = 0,	// �ʏ�
		ENEMYSTATE_SPEEDUP,		// ����
		ENEMYSTATE_SPEEDDOWN,	// ����
		ENEMYSTATE_DAMAGE,		// �U���H�炤
		ENEMYSTATE_MAX,			//�ő吔
	} ENEMYSTATE;

	CEnemy();
	~CEnemy();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CEnemy *Create(D3DXVECTOR3 pos);
	void CollisionEgg(void);
	void CollisionFeed(void);
	void EggAppear(CFeed *pFeed);
	void ChaseEgg(void);
	void BulletEgg(void);

private:
	static LPD3DXMESH	m_pMeshModel;		//���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER m_pBuffMatModel;	//�}�e���A���̏��ւ̃|�C���^
	static DWORD		m_nNumMatModel;	//�}�e���A���̏��
	static LPDIRECT3DTEXTURE9 m_pMeshTextures;			//�e�N�X�`�����
	static D3DXVECTOR3 m_VtxMaxModel;		//���f���̍ő�l
	static D3DXVECTOR3 m_VtxMinModel;		//���f���̍ŏ��l

	D3DXVECTOR3			m_scale;						// �傫��
	D3DXVECTOR3			m_rot;							// ��]
	D3DXVECTOR3			m_OldEggRot[MAX_FRAME];			// �O��̈ʒu
	D3DXVECTOR3			m_move;							// �ړ���
	D3DXVECTOR3			m_pos;							// �ʒu
	D3DXVECTOR3			m_posOld;						// �O��̈ʒu
	CShadow				*m_pShadow;						// �e�̃|�C���^
	CObjBillboad *		m_pObjBill;						// �I�u�W�F�N�g�r���{�[�h�̃|�C���^
	CEgg				*m_pEgg[MAX_EGG];	//���̃|�C���^
	int					m_nCntMove;
	int					m_nCntDamage;
	ENEMYSTATE			m_State;
	float				m_fSpeed;
	bool				m_bSpeedDown;
	bool				m_abJump[MAX_FRAME];
	bool				m_bJumpSave;
	bool				m_bDamage;
	int					m_nNumEgg;
	int					m_nCountSpeed;		// ���Ԃ̉��Z
	int					m_nCntFrame;			// ���̂��Ă��鏈���Ɏg��
};
#endif