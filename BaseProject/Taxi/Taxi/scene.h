//=============================================================================
//
// ���C������ [scene.h]
// Author : �L�n���u
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_PRIORITY	(8)			//�D�揇�ʂ̍ő吔

//=====================
//	   ��{�N���X
//=====================
class CScene
{
public://�N�ł�������
	typedef enum
	{
		OBJTYPE_NONE = 0,		//�g���Ă��Ȃ����
		OBJTYPE_BILLBOORD,		//�r���{�[�h
		OBJTYPE_EFFECT,			//�G�t�F�N�g
		OBJTYPE_2DPOLYGON,		//2D�摜
		OBJTYPE_3DPOLYGON,		//3D�摜
		OBJTYPE_3DMODEL,		//3D���f��
		OBJTYPE_OBJECT,			//3D�I�u�W�F�N�g
		OBJTYPE_SHADOWMODEL,	//�X�e���V���V���h�E
		OBJTYPE_PAUSE,			//�|�[�Y
		OBJTYPE_SERECT,			//�Z���N�g
		OBJTYPE_FADE,			//�t�F�[�h
		OBJTYPE_PLAYER,			//�v���C���[
		OBJTYPE_ENEMY,			//�G
		OBJTYPE_WALL,			//��
		OBJTYPE_GROUND,			//�n��
		OBJTYPE_MESH,			//���b�V��
		OBJTYPE_MESHORBIT,		//�O��
		OBJTYPE_MESHFILED,		//���b�V���t�B�[���h
		OBJTYPE_SHADOW,			//�e
		OBJTYPE_LOGO,			//UI�n
		OBJTYPE_TIME,			//����
		OBJTYPE_SCORE,			//�X�R�A
		OBJTYPE_TOTALSCORE,		//�g�[�^���X�R�A
		OBJTYPE_CUSTOMER,		//���q����
		OBJTYPE_COMBO,			//�R���{��
		OBJTYPE_CAR,			//NPC��
		OBJTYPE_CARPOINT,		//�Ԃ̓����|�C���g
		OBJTYPE_HUMANNPC,		//�lNPC
		OBJTYPE_CYLINDER,		//�V�����_�[
		OBJTYPE_MAX,			//�ő吔
	}OBJTYPE;

	CScene(int nPriority = 3, OBJTYPE objType = OBJTYPE_NONE);
	virtual ~CScene();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	static void ReleseAll(void);
	static void NotFadeReleseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	void SetObjType(OBJTYPE ObjType);
	static CScene *&GetTop(int nPriority);
	CScene *&GetNext(void) { return m_pNext; }
	bool &GetDeath(void) { return m_pDeath; }
	OBJTYPE GetObjType(void);
	static CScene *GetScene(int nPriNumber);
	static void SetUpdatePri(int nUpdatePri);

	//�`��t���O
	void SetDraw(bool bDraw) { m_bDraw = bDraw; };
	bool GetDraw(void) { return m_bDraw; };

private://�l�ł̂ݎg��
	static CScene   *m_pTop[NUM_PRIORITY];	//�擪�̃I�u�W�F�N�g�ւ̃|�C���^
	static CScene   *m_pCur[NUM_PRIORITY];	//���݂̃I�u�W�F�N�g�ւ̃|�C���^
	CScene			*m_pPrev;				//�O�̃I�u�W�F�N�g�ւ̃|�C���^
	CScene			*m_pNext;				//���̃I�u�W�F�N�g�ւ̃|�C���^
	OBJTYPE			m_ObjType;				//�I�u�W�F�N�g�̎��
	static int		m_nNumAll;				//�G�̑���
	int				m_nID;					//�ԍ�
	int				m_nPriority;			//�D�揇�ʂ̔ԍ�
	bool			m_pDeath;				//���S�t���O
	bool			m_bDraw; ;				//�`��t���O
	static int      m_nUpdatePri;
	static bool		m_bOnOff;

protected:
	void Release(void);
	void Delete(void);
};
#endif