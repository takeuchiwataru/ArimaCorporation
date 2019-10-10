//=============================================================================
//
// ���q����̏��� [costomer.h]
// Author : �������� Sato_Asumi
//
//=============================================================================
#ifndef _CPSTOMER_H_
#define _CPSTOMER_H_

#include "main.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "motion.h"
#include "humanbace.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_PARTS	(7)		//�ǂݍ��ރp�[�c��

//=============================================================================
// �O���錾
//=============================================================================
class CMeshCylinder;
class CDestination;
class CArrow;
class CMark;
class CPlayer;
class CGameCamera;

//=============================================================================
// ���q����N���X
//=============================================================================
class CCustomer : public CHumanBace
{
public:
	typedef enum
	{//���q����̏��
		STATE_PUTIN = 0,
		STATE_MOVE,
		STATE_DOWN,
		STATE_MISS,
		STATE_DOWNMOVE,
		STATE_END,
		STATE_ESCAPE,
		STATE_RETURN,
		STATE_CALLTAXI,
		STATE_MAX,
	}STATE;

	//�F�̏�Ԃ̎��
	CCustomer();
	~CCustomer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CCustomer * Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 GoalPos, const int nGoalTex, int nAddTime, int nRideTime, MODEL_TYPE type);
	D3DXVECTOR3 GetOldPos(void) { return m_OldPos; };
	D3DXVECTOR3 GetPos(void) { return m_pos; };
	D3DXVECTOR3 GetRot(void) { return m_rot; };
	STATE GetState(void) { return m_state; };

	static bool GetPutIn(void) { return m_bPutIn; }

private:
	//�ϐ��錾
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void SetGoalTex(int nTex) { m_nGoalTex = nTex; };
	void SetAddTime(int nTime) { m_nAddTime = nTime; };
	void SetRideTime(int nTime) { m_nRideTime = nTime; };
	void SetFirstScore(void);
	void UpdatePutin(CPlayer * pPlayer, CGameCamera * pGameCamera);
	void UpdateDown(CGameCamera * pGameCamera);
	void UpdateDownMove(void);
	void UpdateMove(D3DXVECTOR3 PlayerPos);
	void UpdateMiss(CPlayer * pPlayer, CGameCamera * pGameCamera);
	void UpdateEscape(void);
	void UpdateCallTaxi(D3DXVECTOR3 PlayerPos);
	void UpdateEnd(CPlayer * pPlayer);
	void UninitMeshCylinder(void);
	void UpdateReturn(void);
	void CustomerResult(void);
	D3DXCOLOR SetRank(void);
	void SetState(STATE state);

	D3DXVECTOR3		m_MoveMotion;			//���[�V�����̈ړ���
	D3DXVECTOR3		m_OldPos;				//�O��̍��W
	D3DXVECTOR3		m_OldDiffuse;			//�O��̍���
	D3DXVECTOR3		m_GoalPos;				//�S�[���n�_
	D3DXVECTOR3		m_FirstPos;				//�����ʒu
	D3DXCOLOR		m_Color;				//�F
	STATE			m_state;				//��Ԑݒ�
	STATE			m_OldState;				//�O��̏�Ԃ��擾����
	float			m_fAngle;				//�p�x
	int				m_nGoalTex;				//�ړI�n�e�N�X�`���ԍ�
	int				m_nAddTime;				//���Ԃ̉Z��
	int				m_nRideTime;			//��������
	int				m_nCountRideTime;		//��Ԏ��ԃJ�E���^�[
	int				m_nFirstScore;			//�����̉��Z��
	bool			m_bDrawMeshCylinder;	//�V�����_�[�̕`��t���O
	bool			m_bGotOn;				//��������ǂ���
	bool			m_bResult;				//���ʂ����߂����ǂ���
	CMeshCylinder *	m_pMeshCylinder;		//���b�V���V�����_�[�̃|�C���^
	CDestination *	m_pDestination;			// �ړI�n�ʐ^�ւ̃|�C���^
	CArrow *		m_pArrow;				//���ւ̃|�C���^
	CMark *			m_pMark;				//�}�[�N�̃|�C���^

	static bool		m_bPutIn;				//	��Ԃ��Ă��邩�ǂ���
};
#endif