//=============================================================================
//
// ���b�V������ [meshf.h]
// Author : �L�n���u
//
//=============================================================================
#ifndef _MESH_H_
#define _MESH_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_MESH_TEXTURE			(3)							//�g�p����e�N�X�`���̎��
#define ANGLE						(1.0f / (CORSS/2))			//�w�肵��������PI���o����
#define MESH_PRIOTITY				(2)							//���b�V���t�B�[���h�̗D�揇��

//=====================
//  CScene�̔h���N���X
//=====================
class CMesh : public CScene
{
public://�N�ł�������
	CMesh(int nPriority = 3, CScene::OBJTYPE objType = CScene::OBJTYPE_MESH);
	~CMesh();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CMesh *Create();
	void BindTexture(LPDIRECT3DTEXTURE9 Texture);
	void TextSave(void);
	void TextLoad(void);
	void SetPosition(D3DXVECTOR3 pos);
	void SetMeshInfo(int nMeshX, int nMeshZ, float fMeshTexX, float fMeshTexY, float fMeshDepth, float fMeshWidth);
	void SetMeshType(int nType) { m_nType = nType; }
	D3DXVECTOR3 &GetPosition(void) { return m_pos; }
	void SetVtx(float fVtx0, float fVtxMeshX, float fVtxMeshZ, float fVtxMeshXMeshZ) { m_fVtx0 = fVtx0; m_fVtxMeshX = fVtxMeshX; m_fVtxMeshZ = fVtxMeshZ; m_fVtxMeshXMeshZ = fVtxMeshXMeshZ; }
	void Nor(void);
	bool GetHeight(D3DXVECTOR3 &pos);
	void SetHeight(D3DXVECTOR3 pos, float fValue, float fRange);

private://�l�ł̂ݎg��
	LPDIRECT3DTEXTURE9		m_pTexture;							// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;							// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;							// �C���f�b�N�X�ւ̃|�C���^
	D3DXVECTOR3				m_pos;								// �ʒu
	D3DXVECTOR3				m_rot;								// ����
	D3DXMATRIX				m_mtxWorld, m_local;				// ���[���h�}�g���b�N�X
	int						m_nType;							// ���b�V���t�B�[���h���V�����_�[�̎��
	int						m_nMeshX;							// ���b�V����X������
	int						m_nMeshZ;							// ���b�V����Z������
	float					m_fMeshTexX;						// ���b�V����XUV���W
	float					m_fMeshTexY;						// ���b�V����YUV���W
	float					m_fMeshDepth;						// ���b�V���̉��s
	float					m_fMeshWidth;						// ���b�V���̉���
	float					m_fVtx0;							// pVtx[0]�Ԗڂ̍�����ς���
	float					m_fVtxMeshX;						// pVtx[MeshX + 1]�Ԗڂ̍�����ς���
	float					m_fVtxMeshZ;						// pVtx[MeshZ + 1]�Ԗڂ̍�����ς���
	float					m_fVtxMeshXMeshZ;					// pVtx[MeshXZ + 1]�Ԗڂ̍�����ς���
};
#endif