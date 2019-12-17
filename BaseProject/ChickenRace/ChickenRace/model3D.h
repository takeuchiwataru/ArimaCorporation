//=============================================================================
//
// ���f���̏��� [model3D.h]
// Author : �L�n���u
//
//=============================================================================
#ifndef _MODEL3D_H_
#define _MODEL3D_H_

#include "main.h"
#include "scene.h"
#include "toonshader.h"
#include "camerabace.h"
#include "model3D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_PLAYCOL	(4)

//=====================
//  CScene�̔h���N���X
//=====================
class CModel3D : public CScene
{
public:
	typedef enum
	{// �I�u�W�F�N�g�̎��
		MODEL_TYPE_TREE = 0,		//��
		MODEL_TYPE_SAKU,			//��
		MODEL_TYPE_BOX,				//��
		MODEL_TYPE_MAP_FIRST,		//1�Ԗ�
		MODEL_TYPE_MAP_SECOND,		//2�Ԗ�
		MODEL_TYPE_MAP_MOUNTAIN,	//�R
		MODEL_TYPE_MAP_SKY,			//��
		MODEL_TYPE_CHICKEN_HOUSE,	//�{�̉�
		MODEL_TYPE_L_SIGN,			//���̊Ŕ�
		MODEL_TYPE_R_SIGN,			//�E�̊Ŕ�
		MODEL_TYPE_LR_SIGN,			//�����̊Ŕ�
		MODEL_TYPE_BRIDGE,			//��
		MODEL_TYPE_STATUE,			//����
		MODEL_TYPE_PODIUM,			//�\����
		MODEL_TYPE_BIGSIGN,			//�傫�ȊŔ�
		MODEL_TYPE_STONE,			//�傫�Ȋ�
		MODEL_TYPE_GOAL,			//�S�[��
		MODEL_TYPE_CLIFF_SIGN,		//�R�Ŕ�
		MODEL_TYPE_FEED_SIGN,		//�a�Ŕ�
		MODEL_TYPE_WATER_SIGN,		//���Ŕ�
		MODEL_TYPE_FEED_K,			//�U���a
		MODEL_TYPE_FEED_B,			//�W�Q�a
		MODEL_TYPE_FEED_S,			//���x�a
		MODEL_TYPE_EGG,				//��
		MODEL_TYPE_CHICK,			//�q���R
		MODEL_TYPE_DRIT,			//�y���G�t�F�N�g
		MODEL_TYPE_GRASS,			//���G�t�F�N�g
		MODEL_TYPE_MAX,				//�ő吔
	}MODEL_TYPE;

	typedef enum
	{// �e�N�X�`���̎��
		TEXTURE_TYPE_TREE = 0,		//��
		TEXTURE_TYPE_SAKU,			//��
		TEXTURE_TYPE_BOX,			//��
		TEXTURE_TYPE_MAP_FIRST,		//1�Ԗ�
		TEXTURE_TYPE_MAP_SECOND,	//2�Ԗ�
		TEXTURE_TYPE_MAP_MOUNTAIN,	//�R
		TEXTURE_TYPE_MAP_SKY,		//��
		TEXTURE_TYPE_CHICKEN_HOUSE,	//�{�̉�
		TEXTURE_TYPE_L_SIGN,		//���̊Ŕ�
		TEXTURE_TYPE_R_SIGN,		//�E�̊Ŕ�
		TEXTURE_TYPE_LR_SIGN,		//�����̊Ŕ�
		TEXTURE_TYPE_BRIDGE,		//��
		TEXTURE_TYPE_STATUE,		//����
		TEXTURE_TYPE_PODIUM,		//�\����
		TEXTURE_TYPE_BIGSIGN,		//�傫�ȊŔ�
		TEXTURE_TYPE_STONE,			//�傫�Ȋ�
		TEXTURE_TYPE_GOAL,			//�S�[��
		TEXTURE_TYPE_CLIFF_SIGN,	//�R�Ŕ�
		TEXTURE_TYPE_FEED_SIGN,		//�a�Ŕ�
		TEXTURE_TYPE_WATER_SIGN,	//���Ŕ�
		TEXTURE_TYPE_FEED_K,		//�U���a
		TEXTURE_TYPE_FEED_B,		//�W�Q�a
		TEXTURE_TYPE_FEED_S,		//���x�a
		TEXTURE_TYPE_EGG_K,			//�U����
		TEXTURE_TYPE_EGG_B,			//�W�Q��
		TEXTURE_TYPE_EGG_S,			//���x��
		TEXTURE_TYPE_CHICK_K,		//�U���q���R
		TEXTURE_TYPE_CHICK_B,		//�W�Q�q���R
		TEXTURE_TYPE_CHICK_S,		//���x�q���R
		TEXTURE_TYPE_CHICK_K_S,		//�����U���q���R
		TEXTURE_TYPE_CHICK_B_S,		//�����W�Q�q���R
		TEXTURE_TYPE_CHICK_S_S,		//�������x�q���R
		TEXTURE_TYPE_DRIT,			//�y���G�t�F�N�g
		TEXTURE_TYPE_WATER,			//���G�t�F�N�g
		TEXTURE_TYPE_GRASS,			//���G�t�F�N�g
		TEXTURE_TYPE_MAX,			//�ő吔
	}TEXTURE_TYPE;

	CModel3D(int nPriority = 3, CScene::OBJTYPE objType = CScene::OBJTYPE_3DMODEL);
	~CModel3D();
	HRESULT				Init(void);
	void				Uninit(void);
	void				Update(void);
	void				Draw(void);
	static void			ModelShaderDeleter(void);
	static LPD3DXMESH	&MeshLoad(MODEL_TYPE modeltype);
	static void			UnLoad(void);
	D3DXVECTOR3			GetPosition(void) { return m_Pos; }
	D3DXVECTOR3			&GetRot(void) { return m_Rot; }
	D3DXVECTOR3			GetMove(void) { return m_Move; }
	D3DXVECTOR3			GetScale(void) { return m_Scale; }
	D3DXMATRIX			GetMtx(void) { return m_mtxWorldObject; }
	static D3DXVECTOR3	&GetVtxMin(int nNumber) { return m_VtxMinModel[nNumber]; }
	static D3DXVECTOR3	&GetVtxMax(int nNumber) { return m_VtxMaxModel[nNumber]; }
	D3DXVECTOR3			&GetposR(void) { return m_Pos; }
	D3DXVECTOR3			&GetrotR(void) { return m_Rot; }

	static CModel3D		*Create(void);
	void				SetMove(D3DXVECTOR3 m_move) { m_Move = m_move; }
	void				SetScale(D3DXVECTOR3 Scale) { m_Scale = Scale; }
	void				SetPosition(D3DXVECTOR3 pos) { m_Pos = pos; }
	void				SetRot(D3DXVECTOR3 rot) { m_Rot = rot; };
	bool				Collision(D3DXVECTOR3 pos, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin, D3DXVECTOR3 move);
	void				Setcol(D3DXCOLOR col) { m_col = col; m_bcolChange = true; }

	D3DXCOLOR			&GetCol(void) { return m_col; }
	MODEL_TYPE			&GetModelType(void) { return m_modeltype; }
	bool				&GetbColR(void) { return m_bcolChange; }
	D3DXVECTOR3			&GetScaleR(void) { return m_Scale; }
	float				*GetfCola(void) { return &m_fCola[0]; }
	static CToonShader	*&GetToonShader(void) { return m_pToonShader; }

protected:
	void				SetModelType(int nModelType) { m_nModelType = nModelType; }
	void				SetTextureType(int nTextureType) { m_nTextureType = nTextureType; }

private:
	static const char			*m_apModelFile[];								// ���f���̕������i�[����
	static const char			*m_apTextureFile[];								// �e�N�X�`���̕������i�[����
	static LPD3DXMESH			m_pMeshModel[MODEL_TYPE_MAX];					// ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER			m_pBuffMatModel[MODEL_TYPE_MAX];				// �}�e���A���̏��ւ̃|�C���^
	static DWORD				m_nNumMatModel[MODEL_TYPE_MAX];					// �}�e���A���̏��
	static CToonShader			*m_pToonShader;									// �V�F�[�_�[�̃|�C���^
	static LPDIRECT3DTEXTURE9	m_pMeshTextures[TEXTURE_TYPE_MAX];				// �e�N�X�`���̏��
	static D3DMATERIAL9			*m_pMeshMaterials[MODEL_TYPE_MAX];				// ���b�V���}�e���A���̏��
	static LPDIRECT3DTEXTURE9	*m_pShaderMeshTextures[MODEL_TYPE_MAX];			// �V�F�[�_�[�p
	static D3DXVECTOR3			m_VtxMinModel[MODEL_TYPE_MAX];
	static D3DXVECTOR3			m_VtxMaxModel[MODEL_TYPE_MAX];					// ���f���̍ŏ��l�E�ő�l
	D3DXMATRIX					m_mtxWorldObject;								// ���[���h�}�g���b�N�X
	D3DXMATRIX					m_mtxRot;										// ��]�}�g���b�N�X(�ۑ��p)
	D3DXQUATERNION				m_quat;											// �N�H�[�^�j�I��
	D3DXVECTOR3					m_VecAxis;										// ��]��
	float						m_fValueRot;									// ��]�p(��]��)
	D3DXVECTOR3					m_Pos;											// �ʒu
	D3DXVECTOR3					m_Rot;											// ����
	D3DXVECTOR3					m_Scale;										// �T�C�Y
	D3DXVECTOR3					m_Move;											// ����
	bool						m_bcolChange;									// �J���[�ύX
	D3DXCOLOR					m_col;											// �J���[
	float						m_fLength;										// ����
	bool						m_bTexMat;										// �V�F�[�_�[�Ɏg���e�N�X�`���ƃ}�e���A��
	int							m_nModelType;									// ���f���̎��
	int							m_nTextureType;									// �e�N�X�`���̎��
	D3DXVECTOR3					m_MapView;										// �}�b�v�ɔ��f������J����
	MODEL_TYPE					m_modeltype;									// ���f�����
	float						m_fCola[MAX_PLAYCOL];							//�L���������l�ۑ�
};
#endif