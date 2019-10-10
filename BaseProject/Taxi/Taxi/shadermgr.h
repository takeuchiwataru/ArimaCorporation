//=============================================================================
//
// �V�F�[�_�[�Ǘ����� [shadermgr.h]
// Author : �������� Sato_Asumi
//
//=============================================================================
#ifndef _SHADERMGR_H_
#define _SHADERMGR_H_

#include "main.h"

//=============================================================================
// �O���錾
//=============================================================================
class CRotationShader;

//=============================================================================
// �V�F�[�_�[�Ǘ��N���X
//=============================================================================
class CShaderMgr
{
public:
	CShaderMgr();
	~CShaderMgr();

	static CShaderMgr * Create();
	void Init(void);
	void Uninit(void);

	//�V�F�[�_�[�̎擾
	CRotationShader * GetRotatonShader(void) { return m_pRotationShader; }	//���[�e�[�V����

private:
	CRotationShader * m_pRotationShader;	//��]�V�F�[�_�[
};
#endif
