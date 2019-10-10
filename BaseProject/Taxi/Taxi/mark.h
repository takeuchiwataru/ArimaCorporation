//=============================================================================
//
// �q�̓���ɏo��}�[�N�̏��� [mark.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _MARK_H_
#define _MARK_H_

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
class CMark : public CScene
{
public:
	CMark();
	~CMark();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMark* Create(D3DXVECTOR3 offset);	// ��������
	static void LoadModel(void);
	static void UnloadModel(void);

	D3DXCOLOR GetColor(void) { return m_col; };		// �F�̎擾
	void SetColor(D3DXCOLOR col);					// �F�̐ݒ�
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }	// �ʒu�̐ݒ�


private:
	void DrawModel(void);						//���f���̕`�揈��
	static LPD3DXMESH		m_pLoadMesh;		//���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER		m_pLoadBuffMat;		//�}�e���A���̏��ւ̃|�C���^
	static DWORD			m_nLoadNumMat;		//�}�e���A���̏��

	LPDIRECT3DTEXTURE9		m_pTexture;			// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// ���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX				m_mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXVECTOR3				m_pos;				// �ʒu
	D3DXVECTOR3				m_rot;				// ����
	D3DXCOLOR				m_col;				// �F
	D3DXVECTOR2				m_size;				// �傫��
	float					m_fCntPos;			// �ʒu��ς��邽�߂̃J�E���^
};

#endif