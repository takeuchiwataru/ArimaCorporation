//=============================================================================
//
// �G�t�F�N�g�̏��� effect.h]
// Author : �L�n���u
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"
#include "scene.h"
#include "scene3D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define EFFECT_PRIORTY		(4)
//=====================
//  CScene3D�̔h���N���X
//=====================
class CEffect : public CScene3D
{
public://�N�ł�������
	typedef enum
	{
		EFFECTTYPE_NONE = 0,		//�����Ȃ����
		EFFECTTYPE_SPLASHES,		//�����Ԃ��̃G�t�F�N�g
		EFFECTTYPE_ERUPTION,		//�����G�t�F�N�g
		EFFECTTYPE_SMOKE,			//�����G�t�F�N�g
		EFFECTTYPE_SNOW,			//�y�V��z��G�t�F�N�g
		EFFECTTYPE_RAIN,			//�y�V��z�J�G�t�F�N�g
		EFFECTTYPE_FIRE,			//���G�t�F�N�g
		EFFECTTYPE_POWDER,			//�΂̕��G�t�F�N�g
		EFFECTTYPE_MAX				//�G�t�F�N�g�̍ő吔
	}EFFECTTYPE;

	CEffect();
	~CEffect();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CEffect *Create(EFFECTTYPE Type,D3DXVECTOR3 pos,int nRadius, int nLife,int nTexType);
	void SetParticleEffect(EFFECTTYPE Type, int nRadius);

private://�l�ł̂ݎg��
	static LPDIRECT3DTEXTURE9	m_pTexture[MAX_BILLBOORD_TEX];	// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;						// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3					m_pos;							// �ʒu
	D3DXVECTOR3					m_posOld;						// �ߋ��̈ʒu
	D3DXVECTOR2					m_size;							// �T�C�Y
	float						m_fRadius;						// ���a(�傫��)
	int							m_nLife;						// �\������(����)
	int							m_nType;						// ���
	EFFECTTYPE					m_EffectType;					// �G�t�F�N�g�̎��
	int							m_nCntEffectState;				// ��ԃJ�E���^
	int							m_nCountTime;					// �b���J�E���g
	static int							m_nCntEffect;
};
#endif