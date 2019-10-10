//=============================================================================
//
// �e�[�������v���� [taillamp.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _TAILLAMP_H_
#define _TAILLAMP_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CTailLamp : public CScene
{
public:
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_BACK,			// �o�b�N
		TYPE_BREAKING,		// �u���[�L
		TYPE_HAZARD_LAMP,	// �n�U�[�h�����v
		TYPE_MAX
	}TYPE;

	CTailLamp();
	~CTailLamp();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CTailLamp* Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXMATRIX *pMtxParent);	// ��������

	D3DXMATRIX *GetMtxParent(void) { return m_pMtxParent; };								// �e�̃}�g���b�N�X�̎擾
	void SetMtxParent(D3DXMATRIX *pMtxParent) { m_pMtxParent = pMtxParent; };				// �e�̃}�g���b�N�X�̐ݒ�

	D3DXCOLOR GetColor(void) { return m_col; };												// �F�̎擾
	void SetColor(D3DXCOLOR col);															// �F�̐ݒ�

	TYPE GetType(void) { return m_type; };													// ��ނ̎擾
	void SetType(TYPE type);																// ��ނ̐ݒ�

private:
	LPDIRECT3DTEXTURE9		m_pTexture;			// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// ���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX				m_mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXMATRIX				*m_pMtxParent;		// �e�̃��[���h�}�g���b�N�X
	D3DXVECTOR3				m_pos;				// �ʒu
	D3DXVECTOR3				m_rot;				// ����
	D3DXCOLOR				m_col;				// �F
	D3DXVECTOR2				m_size;				// �傫��
	TYPE					m_type;				// �����v�̎��
	int						m_nCntFlash;		// �����v�̓_�ŗp�J�E���^
};

#endif