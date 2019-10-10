//=============================================================================
//
// �����_���[���� [scene.cpp]
// Author : �L�n���u
//
//=============================================================================
#include "scene.h"
#include "scene2D.h"
#include "renderer.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

////-------------------------------------------------------------------------------
////	                    �`�ÓI�����o�ϐ��錾�`
////-------------------------------------------------------------------------------
//--- �����l��K������� ---//
CScene *CScene::m_pTop[NUM_PRIORITY] = {};
CScene *CScene::m_pCur[NUM_PRIORITY] = {};
int CScene::m_nNumAll = 0;
int CScene::m_nUpdatePri = 0;
bool CScene::m_bOnOff = false;

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CScene::CScene(int nPriority,OBJTYPE objType)
{
	//�ŏ���������΂���
	if (m_pTop[nPriority] == NULL)
	{//�g�b�v��CScene�����
		m_pTop[nPriority] = this;
	}
	else
	{//���݂̎���this������
		m_pCur[nPriority]->m_pNext = this;
	}

	m_ObjType = objType;

	m_pNext = NULL;

	//�O�Ɍ��݂�����
	m_pPrev = m_pCur[nPriority];

	//���݂�this������
	m_pCur[nPriority] = this;

	//�D�揇�ʂ�����
	m_nPriority = nPriority;

	//���S�t���O�̏�����
	m_pDeath = false;

	//�`��t���O�̏�����
	m_bDraw = true;
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CScene::~CScene()
{

}
//===============================================================================
//	�S�ẴN���[��
//===============================================================================
void CScene::ReleseAll(void)
{
	for (int nPriority = 0; nPriority < NUM_PRIORITY; nPriority++)
	{
		while (m_pTop[nPriority] != NULL)
		{//���݂�NULL�o�Ȃ�������
			if (m_pTop[nPriority]->m_pDeath == true)
			{
				m_pTop[nPriority]->Uninit();
			}

			//�폜
			m_pTop[nPriority]->Delete();
		}
	}
}
//===============================================================================
//	fade�����������Ȃ�
//===============================================================================
void CScene::NotFadeReleseAll(void)
{
	for (int nPriority = 0; nPriority < NUM_PRIORITY - 1; nPriority++)
	{
		while (m_pTop[nPriority] != NULL)
		{//���݂�NULL�o�Ȃ�������
			m_pTop[nPriority]->Uninit();

			//�폜
			m_pTop[nPriority]->Delete();
		}
	}
}
//===============================================================================
//	�S�ẴA�b�v�f�[�g
//===============================================================================
void CScene::UpdateAll(void)
{
	//���𐔂���ϐ�
	int nPriNum = 0;
	CScene *pScene;

	for (int nPriority = m_nUpdatePri; nPriority < NUM_PRIORITY; nPriority++)
	{
		pScene = m_pTop[nPriority];

		while (pScene != NULL)
		{
			//Update��Uninit����Ă��܂��ꍇ�@Next��������\�������邩��Next�Ƀf�[�^���c���Ă���
			CScene *pSceneNext = pScene->m_pNext;

			if (pScene->m_pDeath == false)
			{
				pScene->Update();	//�X�V����
			}
			if (pScene->m_pDeath == true)
			{
				pScene->Delete();	//�폜
			}

			//Next�Ɏ���Scene������
			pScene = pSceneNext;
		}
	}
}
//===============================================================================
//	�S�Ă̕`��
//===============================================================================
void CScene::DrawAll(void)
{
	CScene *pScene;
	int nCnt = 0;

	for (int nPriority = 0; nPriority < NUM_PRIORITY; nPriority++)
	{
		pScene = m_pTop[nPriority];

		while (pScene != NULL)
		{//�擪��NULL�ł͂Ȃ�������
			//Update��Uninit����Ă��܂��ꍇ�@Next��������\�������邩��Next�Ƀf�[�^���c���Ă���
			CScene *pSceneNext = pScene->m_pNext;

			if (pScene->m_bDraw == true)
			{
				pScene->Draw();

				if (pScene->GetObjType() == OBJTYPE_OBJECT)
				{
					nCnt++;
				}
			}
	
			//Next�Ɏ���Scene������
			pScene = pSceneNext;
		}
	}

	CDebugProc::Print("�`�搔 : %d", nCnt);
}
//===============================================================================
//	�P�̂̍폜 ���S�t���O�����Ă�
//===============================================================================
void CScene::Release(void)
{
	//���S�t���O�����Ă�
	m_pDeath = true;
}
//===============================================================================
// 
//===============================================================================
void CScene::SetObjType(OBJTYPE ObjType)
{
	m_ObjType = ObjType;
}
//
//
//
CScene *& CScene::GetTop(int nPriority)
{
	return m_pTop[nPriority];
}
//===============================================================================
// 
//===============================================================================
CScene::OBJTYPE CScene::GetObjType(void)
{
	return m_ObjType;
}
//===============================================================================
// 
//===============================================================================
CScene * CScene::GetScene(int nPriNumber)
{
	return m_pTop[nPriNumber];
}
//===============================================================================
//
//===============================================================================
void CScene::SetUpdatePri(int nUpdatePri)
{
	m_nUpdatePri = nUpdatePri;
}
//===============================================================================
//	���S�t���O�����������̂��m�F������ւ���
//===============================================================================
void CScene::Delete(void)
{
	//���S�t���O�������Ă邩���m�F
	if (this != NULL)
	{
		int nPriority = m_nPriority;

		if (m_pNext != NULL)
		{
			m_pNext->m_pPrev = m_pPrev;
		}
		if (m_pPrev != NULL)
		{
			m_pPrev->m_pNext = m_pNext;
		}

		if (m_pTop[nPriority] == this)
		{
			m_pTop[nPriority] = m_pNext;
		}
		if (m_pCur[nPriority] == this)
		{
			m_pCur[nPriority] = m_pPrev;
		}

		//���g�̍폜
		delete this;

		m_nNumAll--;
	}
}

