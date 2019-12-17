//=============================================================================
//
// ��ʉ��o���� [DispEffect.h]
// Author : Ryo Sugimoto
//
//=============================================================================
#ifndef _DISPEFFECT_H_
#define _DISPEFFECT_H_

#include "main.h"
#include "scene.h"
#include "2DPolygon.h"
class CPlayer;
//*****************************************************************************
// �}�N���̒�`
//*****************************************************************************
#define DISP_ANIM_MAX	(32)		//�����ɑ��݂ł���A�j���[�V������
#define DISP_SSET		(5)			//�����܂�ɂ͂��������̉�����
#define DISP_SSIZE		(90.0f)		//�����܂�̈�̊�{�T�C�Y
#define DISP_STIME		(20.0f)		//�����܂�̈�̏o������
#define DISP_BTIME		(60.0f)		//�������o�̏I������

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CDispEffect : public CScene
{
public:
	typedef enum
	{
		EFFECT_SWAMP,	//�����܂�
		EFFECT_BOOST,	//����
		EFFECT_MAX,
	}EFFECT;
	typedef enum
	{
		TEX_SWAMP, TEX_SWAMP_ONE,	//�����܂�
		TEX_BOOST, 					//����
		TEX_FootSteps, TEX_Shadow,	//����
		TEX_Pin, TEX_Light,			//����
		TEX_Warning,				//�x��
		TEX_White, TEX_Water,
		TEX_MAX,
	}TEX;
	CDispEffect(int nPriority = 5, CScene::OBJTYPE objType = CScene::OBJTYPE_DISPEFFECT) : CScene(nPriority, objType) {};
	~CDispEffect() {};
	static void Load(void);
	static void UnLoad(void);

	static LPDIRECT3DTEXTURE9 &GetpTexAll(TEX tex) { return m_pTexAll[tex]; }
	static CDispEffect	*Create(CPlayer *pPlayer);
	HRESULT Init(void);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	void	SetEffect(EFFECT Effect);
private://*****************************************************************************
	void	Set(CPlayer *pPlayer);
	void	AnimUp(void);
	void	GetSize(D3DXVECTOR3 &pos, float &fSizeX, float &fSizeY);
	void	SetDispSwmp(void);
	void	SetDispBoost(void);

	void	ChangeEffect(void);
	C2DAnim	*&SetAnim(bool bSet = true);
	void	SetSwmp(C2DAnim *&pAnim, float fRot = -99.9f);
	//�ϐ��錾//***********************************************************************
	static LPDIRECT3DTEXTURE9	m_pTexAll[TEX_MAX];			// �e�N�X�`���ԍ�

	LPDIRECT3DTEXTURE9		m_pTex;			// �e�N�X�`���ԍ�
	CPlayer *m_pPlayer;
	EFFECT	m_Effect;
	float	m_fCntState;

	C2DAnim	*m_pDispAnim[EFFECT_MAX];	//�S�̃G�t�F�N�g
	C2DAnim	*m_pAnim[DISP_ANIM_MAX];	//�ʃG�t�F�N�g
	EFFECT	m_EAnim[DISP_ANIM_MAX];		//�ʂ̃G�t�F�N�g�^�C�v
};
#endif
