//=============================================================================
//
// ��̏��� [ground.h]
// Author :�L�n�@���u
//
//=============================================================================
#ifndef _GROUND_H_
#define _GROUND_H_

#include "main.h"
#include "scene3D.h"

#define	MAX_GROUND_TEX		(2)
#define GROUND_PRIOTITY		(3)

//=============================================================================
// �N���X��`
//=============================================================================
class CGround : public CScene3D
{
public:
	//�e�N�X�`���̎��
	typedef enum
	{
		TYPE_WATER = 0,
		TYPE_WATER_HIGHLIGHT,
		TYPE_MAX,
	}TYPE;

	CGround();
	~CGround();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CGround *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move,D3DXVECTOR2 size, TYPE Type);

private:
	static LPDIRECT3DTEXTURE9		m_pTexture[MAX_GROUND_TEX];	// �e�N�X�`���ւ̃|�C���^
	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;					// ���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX						m_mtxWorld;					// ���[���h�}�g���b�N�X
	D3DXVECTOR3						m_pos;						// �ʒu
	D3DXVECTOR3						m_rot;						// ��]
	D3DXVECTOR3						m_move;						// �ړ���
	D3DXVECTOR2						m_size;						// �傫��
	int								m_nCnt;						// �J�E���^�[
	TYPE							m_Type;						// ���̎��
	float							m_fTexMoveSpeed;			// �e�N�X�`���̈ړ����x
};
#endif