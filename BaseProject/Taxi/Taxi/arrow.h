//=============================================================================
//
// �ړI�n�̖��̏��� [arrow.h]
// Author : shun yokomichi
//
//=============================================================================
#ifndef _ARROW_H_
#define _ARROW_H_

#include "main.h"
#include "model3D.h"
//*****************************************************************************
// �O���錾
//*****************************************************************************
class CLogo;
//*****************************************************************************
// �}�N����`
//*****************************************************************************

//=====================
//  Model3D�̔h���N���X
//=====================
class CArrow : public CModel3D
{
public://�N�ł�������
	typedef enum
	{	// �ړI�n�̎��
		TYPE_DESTINATION = 0,	// �ړI�n�ē�
		TYPE_GOAL,				// �S�[����
		TYPE_MAX
	}ARROWTYPE;

	typedef enum
	{
		SCZLE_SMALL = 0,
		SCZLE_BIG,
		SCZLE_NOMAL,
		SCZLE_MAX
	}SCZLE_TYPE;

	CArrow();
	~CArrow();
	static HRESULT Load(void);
	static void UnLoad(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CArrow *Create(int nArrowType, D3DXVECTOR3 Pos, D3DXVECTOR3 GoalPos, float fRadius);
	void SetGoalPos(D3DXVECTOR3 GoalPos) { m_goalpos = GoalPos; }
	static LPDIRECT3DTEXTURE9 GetTex() { return m_pTextureMT; }


private:
	D3DXVECTOR3 SetRot(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	D3DXVECTOR3 AreaDetermine(D3DXVECTOR3 pos, D3DXVECTOR3 scale);

	static LPD3DXMESH			m_pMeshModel;				//���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER			m_pBuffMatModel;			//�}�e���A���̏��ւ̃|�C���^
	static DWORD				m_nNumMatModel;				//�}�e���A���̏��
	static D3DXVECTOR3			m_LoadVtxMax;
	static D3DXVECTOR3			m_LoadVtxMin;
	static int					m_nNextType;				//��Ԃ�Ⴄ�Ƃ��̃^�C�v
	static int					m_nNextFont;				//��Ԃ̕���
	int							m_nStateType;				//��Ԃ�Ⴄ�Ƃ��̃^�C�v
	int							m_nStateFont;				//��Ԃ̕���
	int							m_nArrow;					//���̃^�C�v
	float						m_fLength;					//����
	float						m_fAngle;					//�p�x
	float						m_fRadius;					// �S�[���̔��a
	float						m_nSizeScale;				//�X�P�[��
	D3DXVECTOR3					m_rot;						//����
	D3DXVECTOR3					m_goalpos;					//�ړI�n�̈ʒu
	D3DXVECTOR3					m_scale;					//�X�P�[��
	CLogo *						m_pLogo;					//���S�ւ̃|�C���^
	CLogo *						m_pCommand;					//���S�ւ̃|�C���^
	SCZLE_TYPE					m_pScaleType;				// �X�P�[���̎��

	static LPDIRECT3DTEXTURE9 m_pTextureMT;					// �����_�����O�p��Texturte
	LPDIRECT3DSURFACE9		m_pRenderMT;					// Texture�Ƀ����_�����O���邽�߂̃p�C�v
	LPDIRECT3DSURFACE9		m_pBuffMT;						// Texture�Ɏg�p����o�b�t�@
	D3DVIEWPORT9			m_viewportMT;					// Texture�Ɏg�p����r���[�|�[�g
	D3DXCOLOR				m_col;							// �J���[
	D3DXCOLOR				m_colChange;					// �F�ω��p

};

#endif
