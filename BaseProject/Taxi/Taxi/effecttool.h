//=============================================================================
//
// �G�t�F�N�g�c�[������ [Effecttool.h]
// Author : �������� Sato_Asumi
//
//=============================================================================
#ifndef _EFFECTTOOL_H_
#define _EFFECTTOOL_H_

#include "main.h"
#include "particlebillboad.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_EFFECT	(3)		//�G�t�F�N�g�̍ő吔

//=============================================================================
// �O���錾
//=============================================================================
class CSaveText;

//=============================================================================
// �G�t�F�N�g�N���X
//=============================================================================
class CEffectTool
{
public:
	typedef struct
	{
		bool					bCreateEffect;	//�G�t�F�N�g�𐶐�
		bool					bLoop;			//���[�v���邩�ǂ�
		D3DXVECTOR3				pos;			//�ʒu
		D3DXVECTOR3				size;			//�T�C�Y
		int						nTexNum;		//�e�N�X�`���ԍ�
		CParticleBillboad::DRAWTYPE		DrawType;		//�`��^�C�v
		CParticleBillboad::TYPE			Type;			//�p�[�e�B�N���̃^
		D3DXCOLOR				Color;			//�F
		bool					bZbuffur;		//Z�o�b�t�@�̃I���I�t
		int						nLife;			//���C�t
		int						nInterbalTime;	//��������
		float					fChangeCol_a;	//�����x�̕ω���
		bool					bUseGravity;	//�d��
		float					fGravity;		//�d�͂̐ݒ�
		int						nNumCreate;		//������
		float					fSpeed;			//�X�s�[�h�̐ݒ�
		int						nMinHight;		//�����̍ŏ��l
		int						nMaxHight;		//�����̍ő吔
		int						nCountTime;		//���Ԃ̃J�E���^�[
	}Effect_Info;

	typedef struct
	{
		Effect_Info * EffectInfo;		//�G�t�F�N�g���
	}Effect;

	CEffectTool();
	~CEffectTool();
	static CEffectTool * Create(void);
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void LoadEffect(void);
	static void UnloadEffect(void);
	void CreateEffect(int nCntParticle, int nNumEffect, D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nPriority);
	void SetBuffZ(bool bBuffZ) { m_bBuffZ = bBuffZ; };					//Z�o�b�t�@�̗L���ݒ�

protected:
	static Effect *		m_pEffect;		//�G�t�F�N�g���

private:
	static int			m_nMaxEffect;	//�G�t�F�N�g�̍ő吔
	bool				m_bBuffZ;		//Z�o�b�t�@�̃I���I�t
};
#endif