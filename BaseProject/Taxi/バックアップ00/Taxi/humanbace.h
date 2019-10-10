//=============================================================================
//
// �q���[�}���x�[�X�̏��� [humanbace.h]
// Author : �������� Sato_Asumi
//
//=============================================================================
#ifndef _HUMANBACE_H_
#define _HUMANBACE_H_

#include "main.h"
#include "scene.h"
#include "motion.h"
#include "loadText.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_PARTS	(7)		//�ǂݍ��ރp�[�c��

//=============================================================================
// �O���錾
//=============================================================================
class CModel;
class CShadow;

//=============================================================================
// �q���[�}���x�[�X�N���X
//=============================================================================
class CHumanBace : public CScene
{
public:
	typedef enum
	{
		MODEL_MAN = 0,
		MODEL_WOMAN,
		MODEL_MAX,
	}MODEL_TYPE;

	typedef enum
	{
		MOTION_NEUTRAL = 0,
		MOTION_TAXICOLL,
		MOTION_WALK,
		MOTION_BOW,
		MOTION_SHINOBU,
		MOTION_KICK,
		MOTION_ANGER,
		MOTION_ESCAPE,
		MOTION_MAX,
	}MOTION_TYPE;

	CHumanBace(int nPriority, CScene::OBJTYPE type);
	~CHumanBace();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(float fAlpha);
	static void LoadModel(void);
	static void UnloadModel(void);
	void SetClipping(bool bClipping) { m_bClipping = bClipping; }
	
protected:
	//�ʒu�̐ݒ�E�擾
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };
	D3DXVECTOR3 GetPos(void) { return m_pos; };

	//�����̐ݒ�E�擾
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };
	D3DXVECTOR3 GetRot(void) { return m_rot; };

	//�ړ��ʂ̐ݒ�E�擾
	void SetMove(D3DXVECTOR3 move) { m_move = move; };
	D3DXVECTOR3 GetMove(void) { return m_move; };

	void RemakeAngle(float * pAngle);
	float GetLength(D3DXVECTOR3 StartPos, D3DXVECTOR3 EndPos);
	void MoveEscape(void);

	//���f�����̎擾
	CModel ** GetpModel(void) { return m_pModel; };

	//���f���^�C�v�̐ݒ�E�擾
	void SetModelType(MODEL_TYPE type) { m_ModelType = type; };
	MODEL_TYPE GetModelType(void) { return m_ModelType; };

	void SetShadowColor_A(float col_a);
	void SetNextMotion(MOTION_TYPE type);

	//----------------------
	// �ϐ�
	//----------------------
	D3DXVECTOR3	m_pos;			//���S���W
	D3DXVECTOR3	m_move;			//�ړ�
	D3DXVECTOR3	m_rot;			//����
	D3DXMATRIX	m_mtxWorld;		//���[���h�}�g���b�N�X
	int			m_nCountTime;	//�J�E���^�[
	MODEL_TYPE  m_ModelType;	//���f���̃^�C�v
	MOTION_TYPE	m_MotionType;	//���[�V�����̎��
	bool		m_bClipping;	//�N���b�s���O�t���O
	float		m_fLength;		//�����̕ϐ�
	float		m_MotionLength;	//���[�V�����̍X�V�͈�

	//----------------------
	// �萔
	//----------------------
	const D3DXVECTOR3 VECTOR_ZERO = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

private:
	void CreateModel(void);
	void CreateMotion(void);
	void CollisionMesh(void);
	D3DXVECTOR3 GetVtxMax(void);
	D3DXVECTOR3 GetVtxMin(void);

	static LPDIRECT3DTEXTURE9			m_pTexture[MODEL_MAX];		//�e�N�X�`��
	static int							m_nMaxMotion[MODEL_MAX];	//���[�V�����̍ő吔
	static CMotion::MOTION_INFO *		m_pMotionInfo[MODEL_MAX];	//���[�V�������
	static CLoadTextMotion::MODEL_INFO 	m_ModelInfo[MODEL_MAX];		//���f�����
	CModel **							m_pModel;					//�p�[�c���f���̃|�C���^
	D3DXVECTOR3							m_vtxMax;					//���f���̒��_�ő�l
	D3DXVECTOR3							m_vtxMin;					//���f���̒��_�ŏ��l
	CShadow *							m_pShadow;					//�e�̃|�C���^
	CMotion *							m_pMotion;					//���[�V�����̃|�C���^
};
#endif