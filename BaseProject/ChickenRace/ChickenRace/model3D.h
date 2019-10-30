//=============================================================================
//
// ���f���̏��� [model3D.h]
// Author : �L�n���u
//
//=============================================================================
#ifndef _MODEL3D_H_
#define _MODEL3D_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//=====================
//  CScene�̔h���N���X
//=====================
class CModel3D : public CScene
{
public://�N�ł�������
	typedef enum
	{
		MOVETYPE_NONE = 0,
		MOVETYPE_NOT,
		MOVETYPE_CONT,
		MOVETYPE_X_MOVE,
		MOVETYPE_Y_MOVE,
		MOVETYPE_Z_MOVE,
		MOVETYPE_ROTATION_X,
		MOVETYPE_ROTATION_Y,
		MOVETYPE_ROTATION_Z,
		MOVETYPE_HOUSE,
		MOVETYPE_MAX
	}MOVETYPE;

	typedef enum
	{// �{�b�N�X�̊p�W��
		VTX_CORNER_LDF = 0,		// �����O
		VTX_CORNER_LUF,			// ����O
		VTX_CORNER_RDF,			// �E���O
		VTX_CORNER_RUF,			// �E��O
		VTX_CORNER_RDB,			// �E����
		VTX_CORNER_RUB,			// �E�㉜
		VTX_CORNER_LDB,			// ������
		VTX_CORNER_LUB,			// ���㉜
		VTX_CORNER_MAX,
	}VTX_CORNER;

	CModel3D(int nPriority = 3, CScene::OBJTYPE objType = CScene::OBJTYPE_3DMODEL);
	~CModel3D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetPosition(void) { return m_Pos; }
	D3DXVECTOR3 &GetRot(void) { return m_Rot; }
	MOVETYPE GetMoveType(void) { return m_nMoveType; }
	D3DXVECTOR3 GetMove(void) { return m_Move; }
	D3DXVECTOR3 GetScale(void) { return m_Scale; }
	D3DXMATRIX GetMtx(void) { return m_mtxWorldObject; }
	void BindModel(LPD3DXMESH pMeshObject, LPD3DXBUFFER	pBuffMatObject, DWORD nNumMatObject, LPDIRECT3DTEXTURE9	pMeshTextures, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin);
	D3DXVECTOR3 GetVtxMin(void) { return m_VtxMinModel; }
	D3DXVECTOR3 GetVtxMax(void) { return m_VtxMaxModel; }
	static CModel3D *Create(void);
	void SetMove(D3DXVECTOR3 m_move) { m_Move = m_move; }
	void SetScale(D3DXVECTOR3 Scale) { m_Scale = Scale; }
	void SetPosition(D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; };
	void SetMoveType(MOVETYPE MoveType) { m_nMoveType = MoveType; }
	void RecalculationVtx(void);							// ���_�̍ő�l�ƍŏ��l���Čv�Z
	D3DXVECTOR3 BlowOff(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fHeavy);
	bool Collision(D3DXVECTOR3 pos, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin, D3DXVECTOR3 move);
	void Landing(D3DXVECTOR3 posOld);						// �n�ʂɒ������Ƃ�

	bool GetBoolBlowOff(void) { return m_bBlowOff; };
	void SetBoolBlowOff(bool bBlowOff) { m_bBlowOff = bBlowOff; };

	float GetHeavy(void) { return m_fMass; };
	void SetHeavy(float fMass) { m_fMass = fMass; };
	void Setcol(D3DXCOLOR col) { m_col = col; m_bcolChange = true; }// �J���[�ݒ�

	void InitRot(void);

	bool GetBoolRoll(void) { return m_bRoll; }
	void SetBoolRoll(bool bRoll) { m_bRoll = bRoll; }

	bool GetBoolBlow(void) { return m_bBlow; }
	void SetBoolBlow(bool bBlow) { m_bBlow = bBlow; }

	void ScaleVtxCornerPos(D3DXVECTOR3 scale);

	bool GetStateRoll(void) { return m_bStateRoll; }
	void SetStateRoll(bool bStateRoll) { m_bStateRoll = bStateRoll; }

	float GetLength(void) { return m_fLength; }

protected:
	void SetUpdate(bool bUpadate) { m_bUpdate = bUpadate; };
	void SetSmallObj(bool bSmallObj) { m_bSmallObjDraw = bSmallObj; };
	void SetHightObj(bool bHightObj) { m_bHightObjDraw = bHightObj; };
	void SetOnlyLength(bool bOnlyLength) { m_bOnlyLengthDraw = bOnlyLength; };
	bool GetDelete(void) { return m_bDelete; };
	void SetDelete(bool bDelete) { m_bDelete = bDelete; };

private://�l�ł̂ݎg��
	float CrossingAngle(D3DXVECTOR3 vec0, D3DXVECTOR3 vec1);	// �x�N�g���̂Ȃ��p���v�Z
	void RotCalculation(void);									// �Փˎ��̊p�x�v�Z
	bool LengthJudgment(void);									//��������

	LPD3DXMESH				m_pMeshObject;						//���b�V�����ւ̃|�C���^
	LPD3DXBUFFER			m_pBuffMatObject;					//�}�e���A���̏��ւ̃|�C���^
	DWORD					m_nNumMatObject;					//�}�e���A���̏��
	LPDIRECT3DTEXTURE9		m_pMeshTextures;
	D3DXMATRIX				m_mtxWorldObject;					//���[���h�}�g���b�N�X
	D3DXMATRIX				m_mtxRot;							//��]�}�g���b�N�X(�ۑ��p)
	D3DXQUATERNION			m_quat;								//�N�H�[�^�j�I��
	D3DXVECTOR3				m_VecAxis;							//��]��
	float					m_fValueRot;						//��]�p(��]��)
	D3DXVECTOR3				m_VtxMinModel, m_VtxMaxModel;		//���f���̍ŏ��l�E�ő�l
	D3DXVECTOR3				m_Pos;								//�ʒu
	D3DXVECTOR3				m_Rot;								//����
	D3DXVECTOR3				m_Scale;							//�T�C�Y
	D3DXVECTOR3				m_Move;								//����
	MOVETYPE				m_nMoveType;						//�����̎��
	bool					m_bBlowOff;							// ������΂���Ă��邩�ǂ���
	float					m_fMass;							// �d��
	float					m_fRotY;							// �Փˎ��̉�]����
	D3DXVECTOR3				m_aVtxCornerPos[VTX_CORNER_MAX];	// �����蔻��p�{�b�N�X�̊p�W�̒��_���W
	D3DXVECTOR3				m_aVtxPosStorage[VTX_CORNER_MAX];	// �����蔻��p�{�b�N�X�̊p�W�̒��_���W�̕ۑ��p
	D3DXVECTOR3				m_blowOff;							// ������ԋ���
	int						m_nIdxCornerPosMinY;				// �{�b�N�X�̊p�W�̒��_���W�̒��ň�ԒႢ
	int						m_nCountTime;						// ���Ԃ̉Z
	bool					m_bUpdate;							// �A�b�v�f�[�g��ʂ����ǂ���
	bool					m_bUpdateBlow;						// ������΂��������s�����ǂ���
	bool					m_bBlow;							// ������΂����ǂ���
	bool					m_bDelete;							// �j�����邩�ǂ���
	bool					m_bcolChange;						// �J���[�ύX
	bool					m_bSmallObjDraw;					// �������I�u�W�F�N�g�̕`��t���O
	bool					m_bHightObjDraw;					// �����I�u�W�F�N�g�̕`��t���O
	D3DXCOLOR				m_col;								// �J���[
	bool					m_bRoll;							// �]���邩�ǂ���
	bool					m_bStateRoll;						// �]�����Ă��邩�ǂ���
	bool					m_bOnlyLengthDraw;					// �`�拗���������߂�
	D3DXVECTOR3				m_scale;							// �X�P�[���ύX�̔{��
	float					m_fLength;							// ����
};
#endif