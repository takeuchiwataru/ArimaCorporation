//=============================================================================
//
// ���b�V���V�����_�[���� [meshCylinder.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CMeshCylinder : public CScene
{
public://�N�ł�������
	CMeshCylinder();
	~CMeshCylinder();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMeshCylinder *Create(D3DXVECTOR3 pos, int nLap, int nStage, float fRadius, float fHeight, D3DXCOLOR col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

	void MakeVertex(int nLap, int nStage, float fRadius, float fHeight);	// ���_���̍쐬
	void ReMakeVertex(void);												// ���_������蒼��

	void AlongField(bool bAlong, float fFloatHeight = 0.0f);	// �n�ʂɉ��킹�邩�ǂ���

	int GetNumVtx(void) { return m_nNumVtx; };					// ���_���̎擾
	int GetNumPolygon(void) { return m_nNumPolygon; };			// �|���S�����̎擾

	int GetLap(void) { return m_nLap; };						// ����̌��̎擾
	void SetLap(int nLap) { m_nLap = nLap; };					// ����̌��̐ݒ�

	float GetRadius(void) { return m_fRadius; };				// ���a�̎擾
	void SetRadius(float fRadius) { m_fRadius = fRadius; };		// ���a�̐ݒ�

	float GetHeight(void) { return m_fHeight; };				// �����̎擾
	void SetHeight(float fHeight) { m_fHeight = fHeight; };		// �����̐ݒ�

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };			// �ʒu�̐ݒ�


	D3DXCOLOR GetColor(void) { return m_col; };					// �F�̎擾
	void SetColor(D3DXCOLOR col);								// �F�̐ݒ�

private://�l�ł̂ݎg��
	float GetHeightAlongField(D3DXVECTOR3 pos);		// �n�ʂɉ���������

	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;			// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuff;			// �C���f�b�N�X�ւ̃|�C���^
	D3DXMATRIX					m_mtxWorld;			// ���[���h�}�g���b�N�X
	int							m_nNumPolygon;		// �|���S����
	int							m_nNumVtx;			// ���_��
	D3DXVECTOR3					m_pos;				// �ʒu
	int							m_nLap;				// ����̌�
	int							m_nStage;			// �i��
	float						m_fRadius;			// ���a
	float						m_fHeight;			// ����
	D3DXCOLOR					m_col;				// �F
	bool						m_bAlong;			// �n�ʂɉ��킹�邩�ǂ���
	float						m_fFloatHeight;		// �n�ʂ��畂�����l
};
#endif