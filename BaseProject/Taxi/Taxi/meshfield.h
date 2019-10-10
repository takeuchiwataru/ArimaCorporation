//=============================================================================
//
// ���b�V������ [meshfield.h]
// Author : �L�n���u
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "mesh.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MESHFILE_NAME			"data\\TEXT\\MeshCreate.txt"	//�ǂݍ���txt�t�@�C���̖��O
//#define MESH_DATA				(10)

//=====================
//  CMesh�̔h���N���X
//=====================
class CMeshField : public CMesh
{
public://�N�ł�������
	CMeshField();
	~CMeshField();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CMeshField *Create(D3DXVECTOR3 pos, int nMeshX, int nMeshZ, float fMeshXUV, float fMeshYUV, float fMeshWidth, float fMeshDepth,float fVtx0, float fVtxMeshX, float fVtxMeshZ, float fVtxMeshXMeshZ, int nTexTypeint,int nMeshType);
	/*static*/ int GetMeshType(void) { return m_nMeshType; }
	void DeleteMeshField(void);

private://�l�ł̂ݎg��
	//typedef struct
	//{
	//	D3DXVECTOR3			InitPos;					//Pos�̏����l
	//	int					nDivision_X;				//������X
	//	int					nDivision_Z;				//������Z
	//	float				fDepth;						//���s
	//	float				fWidth;						//����
	//	int					nTexType;					//�e�N�X�`����Type�̐ݒ�
	//	int					nMeshType;					//���b�V���t�B�[���h�����b�V���V�����_�[�̎��
	//}MeshField;

	static LPDIRECT3DTEXTURE9		m_pTexture[MAX_MESH_TEXTURE];	// �e�N�X�`���ւ̃|�C���^
	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;						// ���_�o�b�t�@�ւ̃|�C���^
	static LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;						// �C���f�b�N�X�ւ̃|�C���^
	int								m_nCnt;
	int								m_nType;						// ���b�V���t�B�[���h���V�����_�[
	int								m_nMeshType;					// ���b�V���̎��
	float							m_fVtx0;						// pVtx[0]�Ԗڂ̍�����ς���
	float							m_fVtxMeshX;					// pVtx[MeshX + 1]�Ԗڂ̍�����ς���
	D3DXVECTOR3						m_pos;							// �ʒu

};
#endif