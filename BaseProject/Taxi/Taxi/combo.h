//=============================================================================
//
// �R���{���� [combo.h]
// Author : shun yokomichi
//
//=============================================================================
#ifndef _COMBO_H_
#define _COMBO_H_

#include "scene2D.h"
#include "texture.h"
//=============================================================================
// �O���錾
//=============================================================================
class CNumber;
class CLogo;
//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_COMBO		(2)		// �R���{�̌���
#define COMBO_DISTIME	(180)	// �\������

//=============================================================================
// �N���X��`
//=============================================================================
class CCombo : public CScene2D
{
public:
	typedef enum
	{
		TYPE_THROUGH = 0,	// ����Ⴂ��
		TYPE_JUMP,			// �W�����v��
		TYPE_DRIFT,			// �h���t�g��
		TYPR_MAX
	}TYPE;

	CCombo();
	~CCombo();

	static CCombo *Create(TYPE type);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void AddCombo(int nCombo) { m_nComboNum += nCombo; }	// �R���{�̉��Z
	void ResetCombo(void) { m_nComboNum = 0; m_nCntFrame = 0; }		// �R���{�̃��Z�b�g
	static int GetCombo(void) { return m_nComboNum; }
	int GetFream(void) {return m_nCntFrame;}					// �R���{���̃t���[���擾
	void SetType(TYPE type) { CScene2D::BindTexture(*CTexture::GetTexture(CTexture::TYPE_THROUGH + (int)type)); }
	TYPE GetType(void) { return m_type; }
private:
	int PowerCalculation(int nData);		// �ׂ���̌v�Z
	void NumMove(void);						// �R���{���̐����ړ�

	static int					m_nComboNum;				// �R���{�̑���
	int							m_nComboOld;
	int							m_nCntFrame;				// �R���{���̃t���[��
	int							m_DisNumber;				// �\�����鐔(�R���{���p)
	int							m_nDiSTime;					// �\������
	D3DXVECTOR2					m_NumPos;					// �R���{���̌��݈ʒu
	D3DXVECTOR2					m_size;
	CNumber						*m_apNumber[MAX_COMBO];
	CLogo						*m_pLogo;
	TYPE						m_type;
};
#endif