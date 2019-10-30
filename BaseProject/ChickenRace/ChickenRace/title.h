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

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CTitleCamera;

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
	static void	Draw(void);						// �`�揈��

private:
	int					m_nFrame;			// �G�߂̃J�E���^�[
	int					m_nCntTex;			// �e�N�X�`���̃J�E���^�[
	int					m_nCntCreate;		// ��������
	int					m_nLeafposX;		// �����t�� �������W
	int					m_nLeafsize;		// �����t�� �T�C�Y
	int					m_nTitleCounter;	// �^�C�g���̃J�E���^�[
	bool				m_bOnOff;

	static CTitleCamera	*m_pTitleCamera;	//�^�C�g���J�����̃|�C���^
};
#endif