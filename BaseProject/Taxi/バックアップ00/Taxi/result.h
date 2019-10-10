//=============================================================================
//
// ���U���g���� [result.h]
// Author : �L�n ���u
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"
#include "meshfield.h"
#include "object.h"
#include "model.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_RANKING				(5)		// �����x�̐�

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CPlayer;
class CObject;
class CModel;
class CMeshField;
class CPolygon3D;
class CBillBoord;
class CEffect;
class CWall;
class CItem;
class CShadow;
class CReport;
class CResultCamera;
class CLoadTextMotion;
class CResultPlayer;
class CTotalScore;

//=====================
// ��{�N���X
//=====================
class CResult
{
public:

	CResult();								// �R���X�g���N�^
	~CResult();								// �f�X�g���N�^
	HRESULT Init();							// ����������
	void	Uninit(void);					// �I������
	void	Update(void);					// �X�V����
	void	Draw(void);						// �`�揈��

	//�t�@�C����ǂݍ���
	void TextLoad(int nLoadNumber);
	void MeshTextLoad(int nLoadNumber);

	char *ReadLine(FILE *pFile, char *pDst);	//1�s�ǂݍ���
	char *GetLineTop(char *pStr);				//�s�̐擪���擾
	int  PopString(char *pStr, char *pDest);	//�s�̍Ō��؂�̂�

	// �g�[�^���X�R�A�Ɩ����x�̎擾
	static void SetGrades(int nTotalScore, int nLevel) { m_nGetTotalScoer = nTotalScore; m_nGetLevel = nLevel; }

	static CLoadTextMotion * GetPlayerMotion(void) { return m_pPlayerMotion; }	//�v���C���[�̃��[�V�����̎擾
	static CResultPlayer * GetPlayer(void) { return m_pPlayer; }
	static CResultCamera * GetCamera(void) { return m_pCamera; }
	static int GetFirstScore(void) { return m_aTotalScore[0]; }

private:
	//�e�L�X�g�̒l���Ǘ�����ꏊ
	typedef struct
	{
		int					m_nType;			// ���
		int					m_nTexType;			// �e�N�X�`���̎��
		int					m_nCollision;		// �����蔻���ONOFF
		D3DXVECTOR3			m_pos;				// �ʒu
		D3DXVECTOR3			m_rot;				// ��]
		D3DXVECTOR3			m_scale;			// �T�C�Y
	}Map;

	//�e�L�X�g�̒l���Ǘ�����ꏊ
	typedef struct
	{
		int					m_nTexType;						// �e�N�X�`���̎��
		int					m_nWidthDivide;					// ���̕�����������
		int					m_nDepthDivide;					// ���̕�����������
		float				m_fWidthLength;					// ���̒���
		float				m_fDepthLength;					// ���̒���
		float				m_fTexXUV;						// XUV���W
		float				m_fTexYUV;						// YUV���W
		float				m_fVtxHeight_No0;				// ���_�ԍ�0�𑀍�
		float				m_fVtxHeight_No1;				// ���_�ԍ�1�𑀍�
		float				m_fVtxHeight_No2;				// ���_�ԍ�2�𑀍�
		float				m_fVtxHeight_No3;				// ���_�ԍ�3�𑀍�
		D3DXVECTOR3			m_pos;							// �ʒu
	}Mesh;

	//�e�L�X�g�̒l���Ǘ�����ꏊ
	typedef struct
	{
		D3DXVECTOR3			m_pos;				// �ʒu
		D3DXVECTOR3			m_rot;				// ��]
		float				m_fWidthDivide;		// ����
		float				m_fHightDivide;		// �c��
		int					m_nTexType;			// �e�N�X�`���̎��
	}Wall;

	//�e�L�X�g�̒l���Ǘ�����ꏊ
	typedef struct
	{
		int					m_nType;			// ���
		int					m_nTexType;			// �e�N�X�`���̎��
		int					m_nCollision;		// �����蔻���ONOFF
		D3DXVECTOR3			m_pos;				// �ʒu
		D3DXVECTOR3			m_rot;				// ��]
		D3DXVECTOR3			m_scale;			// �T�C�Y
		int					m_nRoute;			// ���[�g�̐ݒ�
	}Npc;

	//�e�L�X�g�̒l���Ǘ�����ꏊ
	typedef struct
	{
		int					nNum;				// �ԍ�
		D3DXVECTOR3			pos;				// �ʒu
	}Point;

	void LoadScore(void);

	static CMeshField		*m_pMeshField;		// ���b�V���t�B�[���h�� �|�C���^
	static CObject			*m_pObject;			// �I�u�W�F�N�g�� �|�C���^
	static CReport			*m_pReport;			// �񍐂̃|�C���^
	static CResultCamera	*m_pCamera;			// ���U���g�J�����ւ̃|�C���^
	static int				m_nGetTotalScoer;	// �g�[�^���X�R�A�擾�p
	static int				m_nGetLevel;		// �����x���x���擾�p
	static CLoadTextMotion	*m_pPlayerMotion;	//�v���C���[�̃��[�V�����ǂݍ���
	static CResultPlayer	*m_pPlayer;
	static int			m_aTotalScore[MAX_RANKING];

	int						m_nCntTime;			// �J�E���^�[
	int						m_nSetCnt;
	bool					m_bPress;			// �����������ǂ���
	int m_nSetObjectNum;					//�I�u�W�F�N�g��u������
	int m_nSetMeshFieldNum;					//���b�V���t�B�[���h��u������
	int	m_nSetWallNum;						//�ǂ̒u������
	Map m_Map[MAX_MAP_OBJECT];					//�ݒu����I�u�W�F�N�g�̍\����
	Mesh m_Mesh[MAX_MAP_MESH];				//�ݒu���郁�b�V���t�B�[���h�̍\����
	Wall m_aWall[MAX_MAP_WALL];				//�ݒu����ǂ̍\����

};
#endif