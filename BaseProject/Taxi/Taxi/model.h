//=============================================================================
//
// ���f����p�̏��� [model.h]
// Author : �L�n���u
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//=====================
//  CModel �P�ƃN���X
//=====================
class CModel
{
public://�N�ł�������
	CModel();
	~CModel();
	HRESULT Init(char FileName[40]);
	void Uninit(void);
	void Update(void);
	void Draw(float fAlpha);
	static CModel * Create(const D3DXVECTOR3 pos, char FileName[40], D3DXVECTOR3 Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }

	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_Rot; }

	void SetFirstPos(D3DXVECTOR3 FirstPos) { m_FirstPos = FirstPos; };
	void SetParent(CModel *pModel) { m_pParent = pModel; }
	void AddPos(D3DXVECTOR3 Pos) { m_Pos = Pos + m_FirstPos; };
	void BindModel(LPD3DXMESH mesh, LPD3DXBUFFER buff, DWORD nummat) { m_pMesh = mesh; m_pBuffMat = buff; m_nNumMat = nummat; }
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTextures = pTexture; }
	D3DXMATRIX &GetMtxWorld(void) { return m_mtxWorld; }
	D3DXVECTOR3 GetPos(void) { return m_Pos; }
	D3DXVECTOR3 GetVtxMax(void) { return m_VtxMax; }
	D3DXVECTOR3 GetVtxMin(void) { return m_VtxMin; }
	D3DXVECTOR3 GetScale(void) { return m_Scale; }
	void SetColor(D3DXCOLOR col);

protected://�h���N���X�ł��g����
	void CreateXFile(char FileName[40]);						//X�t�@�C���̐���
	LPD3DXMESH					m_pMesh;						//���b�V�����ւ̃|�C���^
	LPD3DXBUFFER				m_pBuffMat;						//�}�e���A���̏��ւ̃|�C���^
	DWORD						m_nNumMat;						//�}�e���A���̏��
	LPDIRECT3DTEXTURE9			m_pTextures;					//�e�N�X�`��
	D3DXMATRIX					m_mtxWorld;						//���[���h�}�g���b�N�X
	D3DXVECTOR3					m_VtxMin,m_VtxMax;				//���f���̍ŏ��l�E�ő�l
	D3DXVECTOR3					m_Pos;							//�ʒu
	D3DXVECTOR3					m_Rot;							//����
	D3DXVECTOR3					m_FirstPos;						//�����ʒu
	D3DXVECTOR3					m_Scale;						//�g��A�k����
	CModel						*m_pParent;						//�e���f���ւ̃|�C���^

private://�l�̂ݎg����
	void SetScale(D3DXVECTOR3 Scale) { m_Scale = Scale; };
};
#endif