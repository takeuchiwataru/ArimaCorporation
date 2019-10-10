//=============================================================================
//
// �t���p�l���̏��� [LDCpanel.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _LCDPANEL_H_
#define _LCDPANEL_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "scene3D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_LCD_TEXTURE		(11)			// �e�N�X�`����

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CObject;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CLCDpanel : public CScene3D
{
public:
	CLCDpanel();
	~CLCDpanel();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fScale);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static HRESULT Load(void);
	static void Unload(void);

	static CLCDpanel* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fScale = 1.0f);	// ��������

	float GetScale(void) { return m_fScale; };
	void SetScale(float fScale);

private:
	static LPDIRECT3DTEXTURE9		m_pTexture[MAX_LCD_TEXTURE];		// �e�N�X�`���ւ̃|�C���^
	int								m_nCntDisp;			// �\����ʐؑ֗p�J�E���^
	int								m_nCurTex;			// ���ݕ\�����Ă���e�N�X�`���ԍ�
	CObject							*m_pObject;			// �t����➑̂̃��f��
	float							m_fScale;			// �X�P�[���̔{��
};
#endif