//=============================================================================
//
// ���C������ [title.h]
// Author : �L�n ���u
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_MAP_OBJECT		(3500)

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CTitleCamera;

class CTitleMenu;

//=====================
// ��{�N���X
//=====================
class CTitle
{
public:
	CTitle();								// �R���X�g���N�^
	~CTitle();								// �f�X�g���N�^
	HRESULT Init();							// ����������
	void	Uninit(void);					// �I������
	void	Update(void);					// �X�V����
	static void	Draw(void);					// �`�揈��

	static int GetCounter(void) { return m_nTitleCounter; }

	// ���j���[
	static void SetMenu(bool bMenu)		{ m_bMenu = bMenu; }
	static bool GetMenu(void)			{ return m_bMenu; }
	// �I�����C��
	static void SetOnline(bool bOnline) { m_bOnline = bOnline; }
	static bool GetOnline(void)			{ return m_bOnline; }
	// �z�X�g
	static void SetHost(bool bHost)		{ m_bHost = bHost; }
	static bool GetHost(void)			{ return m_bHost; }

	static CTitleCamera * GetTitleCamera(void) { return m_pTitleCamera; }

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

	int					m_nFrame;			// �G�߂̃J�E���^�[
	int					m_nCntTex;			// �e�N�X�`���̃J�E���^�[
	int					m_nCntCreate;		// ��������
	int					m_nLeafposX;		// �����t�� �������W
	int					m_nLeafsize;		// �����t�� �T�C�Y
	static int			m_nTitleCounter;	// �^�C�g���̃J�E���^�[
	bool				m_bOnOff;

	static CTitleCamera	*m_pTitleCamera;	//�^�C�g���J�����̃|�C���^

	static bool			m_bMenu;			// ���j���[
	static bool			m_bOnline;			// �I�����C��
	static bool			m_bHost;			// �z�X�g

	CTitleMenu			*m_pTitleMenu;		// �^�C�g�����j���[
};
#endif