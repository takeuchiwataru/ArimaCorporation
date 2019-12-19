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
#include "model3D.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_CHICK_PARTS		(11)								// �ǂݍ��ރp�[�c��

#define FILE_NAME_CHICK		"data\\TEXT\\Player\\Chick.txt"	// �ǂݍ���txt�t�@�C���̖��O
#define MAX_CHICK_ANIM		(8)									// �A�j���[�V������
#define MAX_CHICK_KEY		(8)									// �L�[�t���[����
#define MAX_CHICK_MOTION	(10)								// ���[�V�����̍ő吔

#define CHICK_PRIOTITY		(3)									// �Ђ悱�̗D��x
#define CHICK_RANGE			(50.0f)								// �Ђ悱�ƃL�����N�^�[�̋���
#define CHICK_POS			(7)									// �Ђ悱���m�̊Ԋu�̍L���i���₷�ƍL���Ȃ�j
#define CHICK_BOOST			(2.0f)								// ���������
#define MAX_CHICK			(3)									// �Ђ悱�̍ő吔
#define MAX_SPEED_CHICK		(8)									// �Ђ悱�̐�

#define DISTIME					(100)		// ������܂ł̎���
#define ANNOY_RANGE				(70.0f)		// ����������͈�
#define ATTACK_TIME				(3)		// 覐΂Ђ悱��������܂ł̎���
//*****************************************************************************
// �O���錾
//*****************************************************************************
class CObjBillboad;
class CPlayer;
class CModel;
class CCylinder;

//=====================
//  CModel3D�̔h���N���X
//=====================
class CChick : public CScene
{
public:
	//�L�[�v�f
	typedef struct
	{
		float fposX;
		float fposY;
		float fposZ;
		float frotX;
		float frotY;
		float frotZ;
	}KEY;

	//�L�[���
	typedef struct
	{
		int nFrame;
		KEY aKey[MAX_CHICK_PARTS];
	}KEY_INFO;

	//���[�V�������
	typedef struct
	{
		bool bLoop;
		int nNumKey;
		KEY_INFO aKayInfo[MAX_CHICK_MOTION];
	}MOTION_INFO;

	//���[�V�������
	typedef enum
	{
		CHICK_ANIM_NEUTRAL = 0,		//�j���[�g�������[�V����
		CHICK_ANIM_RUN,				//����
		CHICK_MAX					//���[�V�����̍ő吔
	}ChickAnim;

	typedef enum
	{// �Ђ悱�̎��
		TYPE_ATTACK = 0,	// �U��
		TYPE_ANNOY,			// �W�Q
		TYPE_SPEED,			// ����
		TYPE_ATTACK_S,		// �����U��
		TYPE_ANNOY_S,		// �����W�Q
		TYPE_SPEED_S,		// ��������
		TYPE_MAX,			// �ő吔
	} TYPE;

	typedef enum
	{// �Ђ悱�̏��
		STATE_NORMAL = 0,	// �ʏ�
		STATE_CHASE,		// ���Ă���
		STATE_BULLET,		// ���ł���
		STATE_MAX,			// �ő吔
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
	static void Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CChick *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type, BULLETTYPE bulletType, STATE state, int nNumPlayer);
	bool CollisionChick(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld);
	float SetHeight(void);
	void NewEfc(void);
	void DeleteEfc(void);
	STATE GetState(void) { return m_state; }
	void SetState(STATE state) { m_state = state; }
	TYPE GetType(void) { return m_type; }
	BULLETTYPE GetBulletType(void) { return m_bulletType; }
	int GetNumPlayer(void) { return m_nNumPlayer; }
	void SetRank(int nRank) { m_nRank = nRank; }
	int GetRank(void) { return m_nRank; }
	void SetDis(bool bDis) { m_bDis = bDis; }
	bool GetDis(void) { return m_bDis; }
	void Jump(float fJump);
	void Bullet(void);
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	bool GetAttackS(void) { return m_bAttackS; }
	void SetAttackS(bool bAttackS) { m_bAttackS = bAttackS; }
	bool GetSpeedS(void) { return m_bSpeedS; }
	void SetSpeedS(bool bSpeedS) { m_bSpeedS = bSpeedS; }
	void SetDestRank(int nDestRank) { m_DestRank = nDestRank; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	float GetHeight(void) { return m_fHeight; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetAttackCol(bool bAttackCol) { m_bAttackCol = bAttackCol; }
	bool GetAttackCol(void) { return m_bAttackCol; }

	//���[�V�����̍X�V�֐�
	void UpdateMotion(void);

	//�t�@�C���ǂݍ��݊֐�
	static void FileLoad(void);								//�t�@�C���ǂݍ���
	static char *ReadLine(FILE *pFile, char *pDst);			//1�s�ǂݍ���
	static char *GetLineTop(char *pStr);					//�s�̐擪���擾
	static int  PopString(char *pStr, char *pDest);			//�s�̍Ō��؂�̂�

private:
	bool Move(void);
	void Attack(void);
	void AnnoyS(void);
	void AttackS(void);
	bool Item(void);
	void Speed(void);
	void SpeedS(void);
	void Annoy(void);
	void CancelMotion(ChickAnim Anim, bool bRow);
	float AdjustAngle(float rot);
	D3DXMATRIX			m_mtxWorld;						// ���[���h�}�g���b�N�X
	static D3DXVECTOR3	m_VtxMaxModel[MAX_CHICK];		//���f���̍ő�l
	static D3DXVECTOR3	m_VtxMinModel[MAX_CHICK];		//���f���̍ŏ��l
	BULLETTYPE			m_bulletType;					// �e�̎��
	TYPE				m_type;							// �Ђ悱�̎��
	STATE				m_state;						// �Ђ悱�̏��
	D3DXVECTOR3			m_scale;						// �傫��
	D3DXVECTOR3			m_rot;							// ��]
	CShadow				*m_pShadow;						// �e�̃|�C���^
	CObjBillboad *		m_pObjBill;						// �I�u�W�F�N�g�r���{�[�h�̃|�C���^
	D3DXVECTOR3			m_move;
	D3DXVECTOR3			m_pos;
	D3DXVECTOR3			m_posOld;
	D3DXVECTOR3			m_fDestAngle;					// �ړI�̊p�x
	D3DXVECTOR3			m_fDiffAngle;					// ����
	D3DXVECTOR3			m_FNor;							//�n�ʂ̖@��
	D3DXVECTOR3			m_DestPos;					// �ړI�̈ʒu
	int					m_nRank;						// �������Ă�������
	int					m_nNumPlayer;					// �v���C���[���������Ă���Ђ悱��
	int					m_nDisTimer;					// �����܂ł̎���
	int					m_DestRank;						// �ړI�̏���
	int					m_nMap;							// ��������}�b�v
	int					m_nExplosion;					// �������Ă��鎞��
	int					m_nCntUpDown;
	int					m_nCntAttackTime;				// 覐΂�������܂ł̎���
	float				m_fHeight;
	float				m_fLength;						// ����
	float				m_fUpDown;				// �X�s�[�h�A�b�v�Ђ悱�̏㉺
	float				m_fRangePos;					// �ʒu�ۑ��p
	float				m_fRange;
	float				m_fDisTime;						// ������܂ł̎���(�Ȃ���܏C��12/17)
	bool				m_bJump;						// �W�����v���Ă��邩�ǂ���
	bool				m_bDis;							// �������ǂ���
	bool				m_bAttackS;
	bool				m_bExplosion;					// �͈͍U���ɂȂ邩�Ȃ�Ȃ���
	bool				m_bHeight;
	bool				m_bJumpOk;
	bool				m_bSpeedS;
	bool				m_bAttackCol;

	// ���[�V�����֐�	�V�K		
	CCylinder			*m_pClyinder;
	CModel				**m_apModel;	// �p�[�c���f���̃|�C���^
	ChickAnim			m_nAnimnow;						// ���݂̃A�j���[�V����
	static KEY_INFO		*m_pKeyInfo[MAX_CHICK_MOTION];	// �L�[���ւ̃|�C���^
	int					m_nKey;							// ���݂̃L�[�i���o�[
	int					m_nCountFlame;					// �t���[����
	static int			m_nNumParts;					// �p�[�c��
	static int			m_aIndexParent[MAX_CHICK_PARTS];// �e�̃C���f�b�N�X
	static KEY			m_aKayOffset[MAX_CHICK_PARTS];	// �I�t�Z�b�g���
	static MOTION_INFO	m_aMotionInfo[MAX_CHICK_MOTION];// ���[�V�������
	int					m_nMotionType;					// ���[�V�����̃^�C�v(int�^)
	bool				m_bMotionEnd;					// ���[�V�����I��
	float				m_fCola[MAX_PLAYCOL];			//�L���������l�ۑ�
};
#endif