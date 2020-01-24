//=============================================================================
//
// �I�[�r�b�g���� [Orbit.h]
// Author : Ryo Sugimoto
//
//=============================================================================
#ifndef _ORBIT_H_
#define _ORBIT_H_

#include "main.h"
#include "scene.h"
#include "2DPolygon.h"
//*****************************************************************************
// �}�N���̒�`
//*****************************************************************************
#define ORBIT_PRIORITY				(2)			//�O�Ղ̗D�揇��
#define ORBIT_RESET					(10)		//�O�Ղ̊J�n�t���[��

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CEfcOrbit : public CScene
{
public:
	typedef enum
	{
		TYPE_NONE,		//�ʏ�
		TYPE_FADE,		//�t�F�[�h
		TYPE_MAX
	}TYPE;
	CEfcOrbit(int nPrioryity = ORBIT_PRIORITY) : CScene::CScene(nPrioryity)
	{
		m_pmtxWorld = NULL;
		m_bDelete = NULL;
	};
	~CEfcOrbit() {};

	static CEfcOrbit	*Create(void);
	CEfcOrbit	*Set(D3DXMATRIX &mtxWorld, D3DXVECTOR3 length0, D3DXVECTOR3 length1, bool &bDelete, D3DXCOLOR col, int nUV, int nHeight, int nReset);
	HRESULT	Init(void);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	void	ReSet(void);
	static void	ReSetAll(void);
	static void	Delete(bool bEnd);
	static void	DeleteAll(void);
	void		BindTexture(LPDIRECT3DTEXTURE9 pTex) { m_pTex = pTex; }
	C2D::DRAW_TYPE &GetDrawType(void) { return m_DrawType; }
	float		&GetfCol(void) { return m_fPlusCol; }
	TYPE		&GetType(void) { return m_Type; }

private://*****************************************************************************
	void	SetVtx(bool bAll);		//�ŐV�̒��_���v�Z���đ��
	void	SwapVtx(void);		//���_�̓���ւ�
								//�ϐ��錾//***********************************************************************
	TYPE						m_Type;
	C2D::DRAW_TYPE			m_DrawType;		// �`�悷����@(���Z�����Ȃ�)
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//���_�o�b�t�@
	LPDIRECT3DTEXTURE9		m_pTex;		//�摜
	int						m_nNumPolygon;	//�|���S����

	D3DXMATRIX		*m_pmtxWorld;	//�e�̃��[���h�}�g���b�N�X
	D3DXMATRIX		m_mtxWorld[2];	//���[���h�}�g���b�N�X
	D3DXVECTOR3		m_posold;		//�O�̈ʒu
	D3DXCOLOR		m_col;			//�F
	int				m_nUV;			//�摜��UV�ʒu
	int				m_nHeight;		//�摜�̍����̕�����
	int				m_nCross;		//���̕�����
	int				m_nReset;		//���Z�b�g���ꂽ�ꍇ�̃J�E���g�p
	int				m_nInitReset;	//���Z�b�g�̕ۑ��p
	float			m_fPlusCol;		//����䂭�F
	float			m_fUV;			//����䂭�F
	float			m_fEnd;			//�j���܂ł̌o�ߎ���
	bool			m_bEnd;			//�I������
	bool			*m_bDelete;		//�����锻��
	bool			m_bUpdate;			//�X�V�ʂ������ǂ���
};
#endif