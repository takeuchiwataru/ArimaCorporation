//=============================================================================
//
// �L�����N�^�[���� [Character.h]
// Author : Ryo Sugimoto
//
//=============================================================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "main.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "player.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define CHAR_PRIORITY	(1)
#define WOOD_DIS		(45.0f)				//�؂Ɩ؂̊Ԃ̋����i�����j
#define WOOD_LEN		(8.0f)				//�؂̒���
//=============================================================================
// �O���錾
//=============================================================================

//=============================================================================
// �v���C���[�N���X
//=============================================================================
class CCharcter : public CScene
{
public:
	//���[�V�������
	typedef enum
	{
		ANIM_NEUTRAL = 0,		//�j���[�g�������[�V����
		ANIM_MAX					//���[�V�����̍ő吔
	}Anim;

	//�F�̏�Ԃ̎��
	CCharcter(int nPriority = CHAR_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_WINDWOOD) : CScene(nPriority, objType) {};
	~CCharcter() {};
	static void Load(void);
	static void Unload(void);
	static void	FileLoad(void);
	static CCharcter	*Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fScale);
	static void	CollisionAll(CPlayer *pPlayer);

	void				Setting(D3DXVECTOR3 &pos, D3DXVECTOR3 &rot, float &fScale);
	HRESULT				Init(void);
	void				Uninit(void);
	void				Update(void);
	void				Draw(void);
	bool				Collision(CPlayer *&pPlayer, D3DXVECTOR3 &Mypos);
	void				CollisionWood(CPlayer *&pPlayer);

	void UpdateMotion(void);

	static bool	DrawCheck(D3DXVECTOR3 &pos, float *fCol, float &fCola);
	static void	ResetCheck(D3DXVECTOR3 &pos, float *fCol);
	float	&GetfCntState(void) { return m_fCntState; }
private:

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	float		m_fCntState;
	bool		m_bDraw;

	// ���[�V�����֐�	�V�K
	static CPlayer::KEY_INFO	  *m_pKeyInfo[MAX_MOTION];		// �L�[���ւ̃|�C���^
	static int					  m_nNumParts;					// �p�[�c��
	static int					  m_aIndexParent[MAX_PARTS];	// �e�̃C���f�b�N�X
	static CPlayer::KEY			  m_aKayOffset[MAX_PARTS];		// �I�t�Z�b�g���
	static CPlayer::MOTION_INFO	  m_aMotionInfo[MAX_MOTION];	// ���[�V�������
	static LPDIRECT3DTEXTURE9	  m_pTexture;					// �e�N�X�`��	
	static int					  m_nNumModel;					//���f����
	static char					  m_aFileNameModel[MAX_PARTS][256];

	CModel						  **m_apModel;					// �p�[�c���f���̃|�C���^
	int							  m_nKey;						// ���݂̃L�[�i���o�[
	int							  m_nCountFlame;				// �t���[����
	int							  m_nMotionType;				// ���[�V�����̃^�C�v(int�^)
	bool						  m_bMotionEnd;					// ���[�V�����I��
	float						  m_fScale;

	float						  m_fCola[MAX_PLAYCOL];			//�L���������l�ۑ�
	C3DPolygon					  *m_p3D;
};
#endif
