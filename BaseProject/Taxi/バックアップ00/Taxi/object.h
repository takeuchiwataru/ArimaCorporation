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
#define MAX_OBJECT					(38)				// �I�u�W�F�N�g�̃��f����
#define MAX_OBJECT_TEXTURE			(41)			// �I�u�W�F�N�g�̃e�N�X�`����
#define OBJECT_PRIOTITY				(3)

//�A�b�v�f�[�g��ʂ��Ȃ��^�C�v
#define UPDATE_TYPE_NUM			(20)		// �A�b�v�f�[�g���Ȃ��^�C�v�̍ő吔
#define TYPE_TREE00				(2)			// �c���[U
#define TYPE_TREE01				(3)			// �c���[D
#define TYPE_BILL00				(6)			// �r��00
#define TYPE_BILL01				(7)			// �r��01
#define TYPE_BILL02				(8)			// �r��02
#define TYPE_TVBILL				(13)		// �e���r��
#define TYPE_TANUKI				(18)		// �����̃^�C�v�ԍ�
#define TYPE_OCLOCK				(22)		// ���v��
#define TYPE_REDBILL			(23)		// �ԃ����K
#define TYPE_CORN2				(24)		// �R�[���i�t�B�[���h�̎���ɂ�����́j
#define TYPE_STATION			(25)		// �D�y�w
#define TYPE_ESTA				(26)		// �G�X�^
#define TYPE_DAIMAL				(27)		// ���
#define TYPE_APIA				(28)		// �A�s�A�h�[��
#define TYPE_TOWER				(29)		// JR�^���[
#define TYPE_FOUNTAIN			(30)		// ����
#define TYPE_FERRISWGEEL		(34)		// �ϗ���
#define TYPE_TAPIOCA			(35)		// �^�s�I�J�[
#define TYPE_HOSPITAL			(36)		// �a�@
#define TYPE_LCDPANEL			(37)		// �t���p�l��

//�����̂�
#define TYPE_TREE02				(14)		// ��
#define TYPE_FLOWER				(17)		// �Ԓd

//�����I�u�W�F�N�g�^�C�v
#define HIGHT_OBJ_NUM			(4)
#define TYPE_STREETLIGHT		(9)			// �X��
#define TYPE_TRAFFICLIGHT00		(10)		// �M���@00
#define TYPE_TRAFFICLIGHT01		(11)		// �M���@02
#define TYPE_ROAD				(31)		// �w���Ŕ�

//�������I�u�W�F�N�g�^�C�v
#define SMALL_OBJ_NUM			(10)		// �������I�u�W�F�N�g�̎��
#define TYPE_DRINKMACHINE		(0)			// �����̔��@
#define TYPE_GRASS				(1)			// �Ő�
#define TYPE_CARDBORD			(4)			// �_���{�[��
#define TYPE_CORN				(5)			// �R�[��
#define TYPE_BENCH				(12)		// �x���`
#define TYPE_PHONEBOX			(15)		// �d�b�{�b�N�X
#define TYPE_LEAF				(16)		// ���d
#define TYPE_SIGNBOARD			(20)		// �Ŕ�
#define TYPE_BEERBOX			(21)		// �r�[���P�[�X
#define TYPE_FENCE				(32)		// �t�F���X

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CEffectMgr;
class CLoadEffect;
class CObjBillboad;

//=====================
//  CModel3D�̔h���N���X
//=====================
class CObject : public CModel3D
{
public://�N�ł�������
	CObject();
	~CObject();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CObject *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, float move, int nTexTypeint, int nObjectType, MOVETYPE nMovetype, int nCollision);
	void DeleteObject(void);
	int GetType(void) { return m_nType; };
	D3DXVECTOR3 Fountain(D3DXVECTOR3 pos, D3DXVECTOR3 move);
	bool CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);

private://�l�ł̂ݎg��
	void CreateEffectMgr(D3DXVECTOR3 pos);
	void FountainEffect(void);
	void LeafEffect(float fLength);

	static LPD3DXMESH	m_pMeshModel[MAX_OBJECT];					//���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER m_pBuffMatModel[MAX_OBJECT];				//�}�e���A���̏��ւ̃|�C���^
	static DWORD		m_nNumMatModel[MAX_OBJECT];					//�}�e���A���̏��
	static LPDIRECT3DTEXTURE9 m_pMeshTextures[MAX_OBJECT_TEXTURE];	//�e�N�X�`�����
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
	CEffectMgr *		m_pEffectMgr;								// �G�t�F�N�g�}�l�[�W���[
	CLoadEffect *		m_pLoadEffect;								// ���[�h�G�t�F�N�g
	CObjBillboad *		m_pObjBill;									// �I�u�W�F�N�g�r���{�[�h�̃|�C���^
};
#endif