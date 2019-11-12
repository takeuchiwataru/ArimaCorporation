//=============================================================================
//
// �O�Ղ̏��� [meshorbit.h]
// Author : �L�n���u
//
//=============================================================================
#ifndef _MESHORBIT_H_
#define _MESHORBIT_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_MESHORBIT_TEXTURE		(1)				//�e�N�X�`���̍ő吔
#define MESHORBIT_X					(20)			//x�������̃}�X��
#define MESHORBIT_Z					(1)				//z�������̃}�X��
#define MESH_ORBITWIDTH				(50.0f)			//��
#define MESH_ORBITDEPTH				(50.0f)			//���s
#define MAX_OFFSET					(2)				//�I�t�Z�b�g�̍ő吔

//=====================
//  CScene�̔h���N���X
//=====================
class CMeshOrbit : public CScene
{
public://�N�ł�������
	CMeshOrbit();
	~CMeshOrbit();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CMeshOrbit *Create();
	void BindTexture(LPDIRECT3DTEXTURE9 Texture);
	void SetPosition(D3DXVECTOR3 pos);
	void SetMatRarent(D3DXMATRIX *pMtx) { m_pMtxParent = pMtx; }
	void SetOrbitOn(bool bOrbitOnOff) { m_bOrbitOnOff = bOrbitOnOff; }

private://�l�ł̂ݎg��
	static LPDIRECT3DTEXTURE9	m_pTexture[MAX_MESHORBIT_TEXTURE];		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;									// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;									// �C���f�b�N�X�ւ̃|�C���^
	D3DXVECTOR3				m_pos;										// �ʒu
	D3DXVECTOR3				m_rot;										// ����
	D3DXMATRIX				m_mtxWorld, m_local;						// ���[���h�}�g���b�N�X
	D3DXMATRIX				*m_pMtxParent;								// �e�̃}�g���b�N�X�̃|�C���^
	D3DXVECTOR3				m_aPosVertex[MESHORBIT_X];					// �v�Z�������_���W
	D3DXVECTOR3				m_aOffSet[MAX_OFFSET];						// ���[�̃I�t�Z�b�g
	int						m_nType;									// ���
	bool					m_bOrbitOnOff;								// �O�Ղ̕`��

};
#endif