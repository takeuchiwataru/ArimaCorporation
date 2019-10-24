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
#define MESHFILE_NAME			"data\\TEXT\\�X�e�[�W6\\meshfield.txt"	//�ǂݍ���txt�t�@�C���̖��O
#define MESHFIELD_PRIOTITY		(2)

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
	void TextSave(void);
	static CMeshField *Create(D3DXVECTOR3 pos, int nMeshX, int nMeshZ, float fMeshXUV, float fMeshYUV, float fMeshWidth, float fMeshDepth, float fVtx0, float fVtxMeshX, float fVtxMeshZ, float fVtxMeshXMeshZ, float fVtxSide0, float fVtxSide1, float fVtxSide2, float fVtxSide3, int nTexTypeint, int nMeshType);
	static void CreateMesh(void);

private://�l�ł̂ݎg��
	static LPDIRECT3DTEXTURE9		m_pTexture[MAX_MESH_TEXTURE];	//�e�N�X�`���ւ̃|�C���^
	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;						//���_�o�b�t�@�ւ̃|�C���^
	static LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;						//�C���f�b�N�X�ւ̃|�C���^
	int								m_nCnt;							//�J�E���^�[
	int								m_nType;						//���b�V���̎��
	float							m_fVtx0;						//pVtx[0]�Ԗڂ̍�����ς���
	float							m_fVtxMeshX;					//pVtx[MeshX + 1]�Ԗڂ̍�����ς���
	D3DXVECTOR3						m_pos;
	
	//�t�@�C���������ݗp�ϐ�
	int								m_nWidthDivide;					//���̕�����������
	int								m_nDepthDivide;					//���̕�����������
	float							m_fTextXUV;						//���̃e�N�X�`�����W
	float							m_fTextYUV;						//�c�̃e�N�X�`�����W
	float							m_fWidthLength;					//���̒���
	float							m_fDepthLength;					//���̒���
	int								m_nSelectNum;					//�I��ł������
	int								m_nTexType;						//�e�N�X�`���̎��
	int								m_nVtxNumber;					//�I�����Ă��钸�_�̔ԍ�
	float							m_fVtxHeight_No0;				//���_�ԍ�0�𑀍�
	float							m_fVtxHeight_No1;				//���_�ԍ�1�𑀍�
	float							m_fVtxHeight_No2;				//���_�ԍ�2�𑀍�
	float							m_fVtxHeight_No3;				//���_�ԍ�3�𑀍�
	float							m_fVtxSide_No0;					//���_�ԍ�0�𑀍�
	float							m_fVtxSide_No1;					//���_�ԍ�1�𑀍�
	float							m_fVtxSide_No2;					//���_�ԍ�2�𑀍�
	float							m_fVtxSide_No3;					//���_�ԍ�3�𑀍�
};
#endif