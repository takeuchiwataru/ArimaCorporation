//=============================================================================
//
// �G�t�F�N�g�}�l�[�W���[���� [effectmgr.h]
// Author : �������� Sato_Asumi
//
//=============================================================================
#ifndef _EFFECTMANAGER_H_
#define _EFFECTMANAGER_H_

#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TEX_NUM	(1)

//=============================================================================
// �G�t�F�N�g�}�l�[�W���[�N���X
//=============================================================================
class CEffectMgr
{
public:
	typedef enum
	{//�G�t�F�N�g�̎��
		TYPE_SPRING,
		TYPE_SUMMER,
		TYPE_FALL_YELLOW,
		TYPE_FALL_RED,
		TYPE_SNOW,
		TYPE_MAX,
	}TYPE;

	CEffectMgr();
	~CEffectMgr();

	static CEffectMgr * Create(D3DXVECTOR3 pos, TYPE type);
	void Init(void);
	void Uninit(void);
	void Update(void);
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }

	//----------------
	// Load & Unload
	//----------------
	static void LoadTex(void);
	static void UnloadTex(void);

private:
	void SetType(TYPE type) { m_Type = type; }

	//�X�^�e�B�b�N�ϐ�
	static LPDIRECT3DTEXTURE9 m_pTexture;

	//�����o�ϐ�
	TYPE		m_Type;		//�^�C�v
	D3DXVECTOR3	m_pos;		//�ʒu
	int			m_nCounter;	//�J�E���^�[
};
#endif