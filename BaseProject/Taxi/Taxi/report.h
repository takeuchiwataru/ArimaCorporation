//=============================================================================
//
// �񍐂̏��� [report.h]
// Author : shun yokomichi
//
//=============================================================================
#ifndef _REPORT_H_
#define _REPORT_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define REPORT_LOGO_NUM		(4)

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CSatisfaction;
class CTotalScore;
class CLogo;

//=====================
//  CScene2D�̔h���N���X
//=====================
class CReport : public CScene
{
public://�N�ł�������
	typedef enum
	{	// ��ʕ\���̎��
		TYPE_NONE = 0,		// �����Ȃ�
		TYPE_TOTALLOGO,		// �g�[�^���X�R�A�̃��S�\��
		TYPE_TOTALSCORE,	// �g�[�^���X�R�A�̕\��
		TYPE_CLAS,			// �����x�̃��S�\��
		TYPE_CLASLEVEL,		// �����x�̕]���\��
		TYPE_OK_KEY,		// ����{�^���\��
		TYPE_ALL_DISPLAY,	// �{�^���������ꂽ��S�ĕ\��
		TYPE_MAX			// ����
	}REPORTTYPE;

	CReport();
	~CReport();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CReport *Create(REPORTTYPE type, int nTotalScore, int nLevel);

	void SetReporh(REPORTTYPE type) { m_nType = m_type;  m_type = type; }

private:
	void SetDisplay();
	void produce();
	void produceAll();

	CSatisfaction	*m_pSatisfaction;				//�����x�̃|�C���^
	CTotalScore		*m_pTotalScore;					//�g�[�^���X�R�A�̃|�C���^
	CLogo			*m_apLogo[REPORT_LOGO_NUM];		//���S�ւ̃|�C���^

	int				m_nTotalScore;					// �g�[�^���X�R�A�ۑ��p
	int				m_nLevel;						// �����x�̃��x���ۑ��p
	int				m_nCntLogo;						//���S�̃J�E���g
	D3DXVECTOR2		m_size;							//�T�C�Y
	D3DXVECTOR3		m_rot;							//����
	D3DXCOLOR		m_col;							//�J���[
	float			m_fLength;						//����
	float			m_fAngle;						//�p�x
	REPORTTYPE		m_type;							//���
	int				m_nType;						//��ޕۑ�

};
#endif