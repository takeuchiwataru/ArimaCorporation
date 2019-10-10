//=============================================================================
//
// ���[�h�G�t�F�N�g���� [loadEffect.h]
// Author : �������� Sato_Asumi
//
//=============================================================================
#ifndef _LOADEFFECT_H_
#define _LOADEFFECT_H_

#include "main.h"
#include "effecttool.h"

//=============================================================================
// �O���錾
//=============================================================================
class CSaveText;

//=============================================================================
// �ǂݍ��񂾃G�t�F�N�g�̐����N���X
//=============================================================================
class CLoadEffect : public CEffectTool
{
public:
	CLoadEffect();
	~CLoadEffect();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CLoadEffect * Create(int nNumEffect, D3DXVECTOR3 pos, int nPriority);

	void SetNumEffect(int nNumEffect) { m_nNumEffect = nNumEffect; };	//�G�t�F�N�g�i���o�[�̐ݒ�
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };						//�ʒu�̐ݒ�
	void SetSize(D3DXVECTOR3 size) { m_size = size; };					//�T�C�Y�̐ݒ�
	void SetColor(D3DXCOLOR col) { m_color = col; };					//�F�̐ݒ�
	void SetRemake(bool bRemake) { m_bRemake = bRemake; };				//�Đݒ肷�邩�ǂ���
	void SetPriority(int nPriority) { m_nPriority = nPriority; };		//�D�揇�ʂ̐ݒ�
	D3DXMATRIX * GetMtxWorld(void) { return &m_mtxWorld; };				//�}�g���b�N�X�̃|�C���^���擾
	D3DXVECTOR3 GetPos(void) { return m_pos; };							//�ʒu�̎擾

private:
	D3DXMATRIX 		m_mtxWorld;		//���[���h�}�g���b�N�X�̃|�C���^
	D3DXVECTOR3		m_pos;			//�ʒu���
	D3DXVECTOR3		m_size;			//�傫��
	D3DXCOLOR		m_color;		//�F
	int				m_nNumEffect;	//�G�t�F�N�g�̃i���o�[
	int				m_nPriority;	//�D�揇�ʂ̎擾
	bool			m_bRemake;		//�Đݒ肷�邩�ǂ���
};
#endif
