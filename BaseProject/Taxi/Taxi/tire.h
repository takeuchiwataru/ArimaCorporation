//=============================================================================
//
// �^�C���̏��� [model.h]
// Author : ���������@SatoAsumi
//
//=============================================================================
#ifndef _TIRE_H_
#define _TIRE_H_

#include "main.h"
#include "model.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CIncline;
class CPlayer;

//=====================
//  CTire �P�ƃN���X
//=====================
class CTire : public CModel
{
public://�N�ł�������
	CTire();
	~CTire();
	HRESULT Init(void);
	void Uninit(void);
	void Update(int nCntTire, float fGravity);
	void Draw(float fAlpha);
	static CTire * Create(const D3DXVECTOR3 pos);
	static void LoadTexture(void);
	static void UnloadTexture(void);

	D3DXVECTOR3 GetWorldPos(void) { return m_WorldPos; };	//�^�C���̃��[���h���W�̎擾
	bool GetLand(void) { return m_bLand; };					//�n�ʂɏ���Ă��邩�ǂ���

	//�萔
	const float TIRE_HIGHT = 18.0f;

private:
	void RemakeAngle(float * pAngle);
	void UpdateField(CPlayer * pPlayer);
	void SetIncline(CPlayer * pPlayer);

	static LPDIRECT3DTEXTURE9	m_pTexture;		// �e�N�X�`���̃|�C���^
	static float				m_fHight;		// �^�C���̍���
	static bool					m_bJump;		// �W�����v��Ԃ��ǂ���
	static bool					m_bPass;		// ��������t���O
	D3DXVECTOR3					m_move;			// �ړ���
	D3DXVECTOR3					m_WorldPos;		// ���[���h���W
	D3DXVECTOR3					m_WorldRot;		// ���[���h��]
	CIncline *					m_pIncline;		// �X�Ώ����̃|�C���^�ϐ�
	bool						m_bLand;		// �n�ʂɏ���Ă����Ԃ��ǂ���
	float						m_fvtxMaxY;		// ���f�����_�̍ő�l
};	
#endif