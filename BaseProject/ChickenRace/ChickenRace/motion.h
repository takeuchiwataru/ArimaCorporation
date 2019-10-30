//=============================================================================
//
// ���[�V�������� [motion.h]
// Author : �������� Sato_Asumi
//
//=============================================================================
#ifndef _MOTION_H_
#define _MOTION_H_

#include "main.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "input.h"

//=============================================================================
// �O���錾
//=============================================================================
class CModel;

//=============================================================================
// 3D�|���S���N���X
//=============================================================================
class CMotion
{
public:
	//�L�[�v�f
	typedef struct
	{
		float fPosX;	//X���W
		float fPosY;	//Y���W
		float fPosZ;	//Z���W
		float fRotX;	//X����
		float fRotY;	//Y����
		float fRotZ;	//Z����
	}KEY;

	//�L�[���
	typedef struct
	{
		int nFrame;	//�t���[����
		KEY * aKey;	//�p�[�c���̃L�[
	}KEY_INFO;

	//���[�V�������
	typedef struct
	{
		bool m_bLoop;				//���[�v�L�肩�ǂ���
		int nNumKey;				//�L�[�̑���
		KEY_INFO * m_pKeyInfo;		//�L�[���
	}MOTION_INFO;

	CMotion();
	~CMotion();
	HRESULT Init(void);
	void Uninit(void);
	int Update(void);
	void Draw(void);
	static CMotion * Create(void);

	void SetMotion(MOTION_INFO * pMotionInfo) { m_pMotionInfo = pMotionInfo; };		//���[�V��������ݒ肷��
	void SetMaxMotion(int nMaxMotion) { m_nMaxMotion = nMaxMotion; };				//���[�V�����̍ő吔���Z�b�g����
	MOTION_INFO * GetMotionInfo(void) { return m_pMotionInfo; };					//���[�V�����f�[�^�̎擾
	int GetKey(void) { return m_nKey; };											//���݂̃L�[���擾
	int GetnCountMotion(void) { return m_nCountMotion; };							//���݂̃t���[�����̎擾
	void SetModel(CModel ** pModel) { m_pModel = pModel; };							//���f�����̎擾
	void SetMaxModel(int nMaxModel) { m_nMaxModel = nMaxModel; };					//���f���̍ő吔�擾
	D3DXVECTOR3 GetMove(void) { return m_move; };									//������Ԃ�
	void SetBlend(bool bBlend) { m_bBlend = bBlend; };								//���[�V�����̃u�����h��Ԃ̐ݒ�

	int GetNumMotion(void) { return m_nNumMotion; };								//���݂̃��[�V�������擾
	void SetNumMotion(int NumMotion);												//���[�V�����̐ݒ�

private:
	KEY * SetKey(int nCntModel);
	void SetMotionDifference(KEY * pNextKey, KEY * pKey, int nCntModel);
	CModel **		m_pModel;			//�p�[�c���f���̃|�C���^
	int				m_nMaxModel;		//�ǂݍ��ރ��f���̍ő吔
	D3DXVECTOR3		m_move;				//�ړ�
	D3DXVECTOR3		m_rot;				//����
	int				m_nMaxMotion;		//���[�V�����̍ő吔
	MOTION_INFO *	m_pMotionInfo;		//���[�V�������
	int				m_nKey;				//���݂̃L�[NO
	int				m_nCountMotion;		//���[�V�����J�E���^�[
	int				m_nNumMotion;		//���݉��̃��[�V������
	int				m_nNextNumMotion;	//���̃��[�V����
	bool			m_bBlend;			//���̃��[�V�����ɂȂ��邩�ǂ���
};
#endif