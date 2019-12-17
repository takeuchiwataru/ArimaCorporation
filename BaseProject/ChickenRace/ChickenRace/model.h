//=============================================================================
//
// ���f����p�̏��� [model.h]
// Author : �L�n���u
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"
#include "toonshader.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_MODEL			(25)
#define MAX_MODEL_TEXTURE	(4)

//=====================
//  CModel �P�ƃN���X
//=====================
class CModel
{
public://�N�ł�������

	typedef enum
	{// �I�u�W�F�N�g�̎��
		PARTS_CHICKEN_BODY = 0,		//��
		PARTS_CHICKEN_HEAD,			//��
		PARTS_CHICKEN_ASS,			//�K��
		PARTS_CHICKEN_WINGSR_0,		//�E�H��
		PARTS_CHICKEN_WINGSR_1,		//�E�H��
		PARTS_CHICKEN_WINGSL_0,		//���H��
		PARTS_CHICKEN_WINGSL_1,		//���H��
		PARTS_CHICKEN_LEGR,			//�E��
		PARTS_CHICKEN_FOOTR,		//�E��
		PARTS_CHICKEN_LEGL,			//����
		PARTS_CHICKEN_FOOTL,		//����

		PARTS_CHICKEN_11,			//�n�b�s�[�{�[�C
		PARTS_CHICKEN_12,			//�n�b�g�X�q
		PARTS_CHICKEN_13,			//�t�F�U�[�{�[��
		PARTS_CHICKEN_14,			//�C���X�q
		PARTS_CHICKEN_15,			//��
		PARTS_CHICKEN_16,			//�w�����b�g
		PARTS_CHICKEN_17,			//�x���[�X
		PARTS_CHICKEN_18,			//�s�G���X�q

		PARTS_CHICK_BODY,			//��
		PARTS_CHICK_HEAD,			//��
		PARTS_CHICK_LEGR,			//�E��
		PARTS_CHICK_FOOTR,			//�E��
		PARTS_CHICK_LEGL,			//����
		PARTS_CHICK_FOOTL,			//����

		PARTS_WOOD_LEG,				//��
		PARTS_WOOD_BODY_R,			//�E���g
		PARTS_WOOD_LONG_R,			//�����E�r
		PARTS_WOOD_SHORT_R,			//�Z���E�r
		PARTS_WOOD_LEAF_R,			//�E�̑�
		PARTS_WOOD_BODY_L,			//�����g
		PARTS_WOOD_LONG_L,			//�������r
		PARTS_WOOD_SHORT_L,			//�Z�����r
		PARTS_WOOD_LEAF_L,			//���̑�
		PARTS_MAX,					//�ő吔
	}PARTS_TYPE;

	typedef enum
	{// �I�u�W�F�N�g�̎��
		TYPE_CHICKEN = 0,			//��
		TYPE_CHICK,					//�Ђ悱
		TYPE_WOOD,					//��
		TYPE_MAX,					//�ő吔
	}TYPE;

	typedef enum
	{// �e�N�X�`���̎��
		TEX_HAPPYBOY = 0,			//�n�b�s�{�[�C
		TEX_HAT,					//�n�b�g�X�q
		TEX_FEATHERBONE,			//�t�F�U�[�{�[��
		TEX_PIRATE,					//�C��
		TEX_KABUTO,					//��
		TEX_HELMET,					//�w�����b�g
		TEX_BERET,					//�x���[
		TEX_CLOWN,					//�s�G��
		TEX_CHICKEN,				//�j���g��
		TEX_CHICK_R,				//�q���R��
		TEX_CHICK_B,				//�q���R��
		TEX_CHICK_Y,				//�q���R��
		TEX_MAX,					//�ő吔
	}TEXTURE;


	CModel();
	~CModel();
	HRESULT Init();
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
	void BindModel(LPD3DXMESH mesh, LPD3DXBUFFER buff, DWORD nummat) { *m_pMesh = mesh; *m_pBuffMat = buff; *m_nNumMat = nummat; }
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTextures = pTexture; }
	D3DXMATRIX &GetMtxWorld(void) { return m_mtxWorld; }
	D3DXVECTOR3 GetPos(void) { return m_Pos; }

	static D3DXVECTOR3 GetVtxMax(int nNumber) { return m_VtxMax[nNumber]; }
	static D3DXVECTOR3 GetVtxMin(int nNumber) { return m_VtxMin[nNumber]; }
	static int &GetnModelMax(TYPE type) { return m_nModelMax[type]; }
	static LPD3DXBUFFER &GetpBuffMat(int nNumber) { return m_pBuffMat[nNumber]; }
	static LPD3DXMESH &GetpMesh(int nNumber) { return m_pMesh[nNumber]; }
	static DWORD &GetnNumMat(int nNumber) { return m_nNumMat[nNumber]; }
	static LPDIRECT3DTEXTURE9 &GetTexAll(int nNumber) { return m_pTexAll[nNumber]; }

	D3DXVECTOR3 GetScale(void) { return m_Scale; }
	void SetColor(D3DXCOLOR col);
	static void Load(void);
	static void SetParts(void);
	static void ParentModel(CModel **&apModel, TYPE type, int nSelectParts = 0);
	static void PartsTypeUnLoad(void);
	void SetType(PARTS_TYPE partstype) { m_Type = partstype; }

private://�h���N���X�ł��g����
	void CreateXFile(char FileName[40]);									//X�t�@�C���̐���
	static D3DMATERIAL9			*m_pMeshMaterials[PARTS_MAX];
	static LPDIRECT3DTEXTURE9	*m_pShaderMeshTextures[PARTS_MAX];			//�V�F�[�_�[�p
	static LPDIRECT3DTEXTURE9	*m_pMeshTextures[MAX_MODEL_TEXTURE];		// �V�F�[�_�[�p
	static LPD3DXBUFFER			m_pBuffMat[PARTS_MAX];						//�}�e���A���̏��ւ̃|�C���^
	static LPD3DXMESH			m_pMesh[PARTS_MAX];							//���b�V�����ւ̃|�C���^
	static DWORD				m_nNumMat[PARTS_MAX];						//�}�e���A���̏��
	static D3DXVECTOR3			m_VtxMin[PARTS_MAX], m_VtxMax[PARTS_MAX];							//���f���̍ŏ��l�E�ő�l
	static PARTS_TYPE			*m_partstype[TYPE_MAX];
	static int					m_nModelMax[TYPE_MAX];								// ���f���̎��
	static LPDIRECT3DTEXTURE9	m_pTexAll[TEX_MAX];							//�e�N�X�`��

	PARTS_TYPE					m_Type;										// ���f���̎��
	LPDIRECT3DTEXTURE9			m_pTextures;								//�e�N�X�`��
	D3DXMATRIX					m_mtxWorld;									//���[���h�}�g���b�N�X
	D3DXVECTOR3					m_Pos;										//�ʒu
	D3DXVECTOR3					m_Rot;										//����
	D3DXVECTOR3					m_FirstPos;									//�����ʒu
	D3DXVECTOR3					m_Scale;									//�g��A�k����
	CModel						*m_pParent;									//�e���f���ւ̃|�C���^
	bool						m_bTexMat;									//�V�F�[�_�[�Ɏg���e�N�X�`���ƃ}�e���A��

protected://�l�̂ݎg����

};
#endif