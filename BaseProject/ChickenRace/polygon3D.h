//=============================================================================
//
// ���C������ [scene3D.h]
// Author : �L�n���u
//
//=============================================================================
#ifndef _POLYGON3D_H_
#define _POLYGON3D_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//=====================
//  CScene�̔h���N���X
//=====================
class CPolygon3D : public CScene
{
public://�N�ł�������
	CPolygon3D();
	~CPolygon3D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindTexture(LPDIRECT3DTEXTURE9 Texture);
	D3DXVECTOR3 GetPosition(void) { return m_Pos; }
	D3DXVECTOR3 GetMove(void) { return m_Move; }
	static D3DXVECTOR3 *GetNor(void) { return &m_nor[0]; }
	static CPolygon3D *Create();
	float GetHeight(D3DXVECTOR3 pos);
	void GetNor(D3DXVECTOR3 *nor);
	void SetMove(D3DXVECTOR3 m_move) { m_Move = m_move; }
	void SetPosition(D3DXVECTOR3 pos) { m_Pos = pos; }

private://�l�ł̂ݎg��
	LPDIRECT3DTEXTURE9		m_pTexture;						//�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;						//���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3				m_Pos;							//�ʒu
	D3DXVECTOR3				m_Rot;							//����
	D3DXVECTOR3				m_Move;							//����
	static D3DXVECTOR3		m_nor[4];
	D3DXMATRIX				m_mtxWorldPolygon;				//���[���h�}�g���b�N�X
	int						m_nType;						//���
	int						m_nIdxShadow;
	bool					m_bUse;							//���
};
#endif