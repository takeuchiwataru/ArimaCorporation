//=============================================================================
//
// ���U���g���� [result.h]
// Author : �L�n ���u
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"
//#include "meshfield.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_MAP_OBJECT		(3500)
#define RESULT_WAIT			(120)

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CResultCamera;

class CResultUI;

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
	static void	Draw(void);					// �`�揈��

	static int GetCounter(void) { return m_nResultCounter; }

	static CResultCamera * GetResultCamera(void) { return m_pResultCamera; }

private:
	typedef struct
	{
		int					m_nType;			// ���
		int					m_nTexType;			// �e�N�X�`���̎��
		int					m_nCollision;		// �����蔻���ONOFF
		D3DXVECTOR3			m_pos;				// �ʒu
		D3DXVECTOR3			m_rot;				// ��]
		D3DXVECTOR3			m_scale;			// �T�C�Y
	}Map;

	//---------------------------------
	//�t�@�C����ǂݍ���
	//---------------------------------
	void TextLoad(int nLoadNumber);

	char *ReadLine(FILE *pFile, char *pDst);	//1�s�ǂݍ���
	char *GetLineTop(char *pStr);				//�s�̐擪���擾
	int  PopString(char *pStr, char *pDest);	//�s�̍Ō��؂�̂�

	int m_nSetObjectNum;						//�I�u�W�F�N�g��u������

	Map m_Map[MAX_MAP_OBJECT];					//�ݒu����I�u�W�F�N�g�̍\����

	static int			m_nResultCounter;	// �^�C�g���̃J�E���^�[

	static CResultCamera	*m_pResultCamera;	//���U���g�J�����̃|�C���^

	CResultUI			*m_pResultUI;		// ���U���gUI
};
#endif