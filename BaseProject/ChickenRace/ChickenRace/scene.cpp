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
#include "object.h"
#include "billboord.h"

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
			if (!m_pTop[nPriority]->m_pDeath)
			{
				m_pTop[nPriority]->Uninit();
			}

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
				if (pScene->m_pDeath == false)
				{
					pScene->Update();	//�X�V����
				}
			}
			else
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

			if (pScene->m_ObjType == OBJTYPE_BILLBOORD)
			{
				CBillBoord *pBillBoord = (CBillBoord*)pScene;
				if (pBillBoord->GetMapOn() == true)
					goto skip;		// �X�L�b�v�����
			}

			if (pScene->m_pDeath == true)
			{
				pScene->Delete();	//�폜
			}
			else if (pScene->m_bDraw == true)
			{
				pScene->Draw();

				if (pScene->GetObjType() == OBJTYPE_OBJECT)
				{
					nCnt++;
				}
			}
		
			skip:	// �����܂ŃX�L�b�v

			//Next�Ɏ���Scene������
			pScene = pSceneNext;
		}
	}

	CDebugProc::Print("�`�搔 : %d", nCnt);
}
void CScene::DrawMap(void)
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

			if (pScene->m_ObjType == OBJTYPE_COLMESH || pScene->m_ObjType == OBJTYPE_OBJECT || pScene->m_ObjType == OBJTYPE_BILLBOORD)
			{
				if (pScene->m_ObjType == OBJTYPE_OBJECT)
				{
					CObject *pObject = (CObject*)pScene;

					//�R�E��E��
					if (pObject->GetType() != 3 && pObject->GetType() != 4 && pObject->GetType() != 11)
						goto skip;		// �X�L�b�v�����
				}
				else if (pScene->m_ObjType == OBJTYPE_BILLBOORD)
				{
					CBillBoord *pBillBoord = (CBillBoord*)pScene;
					if (pBillBoord->GetMapOn() == false)
						goto skip;		// �X�L�b�v�����
				}

				if (pScene->m_pDeath == true)
				{
					pScene->Delete();	//�폜
				}
				else if (pScene->m_bDraw == true)
				{
					pScene->Draw();

					if (pScene->GetObjType() == OBJTYPE_OBJECT)
					{
						nCnt++;
					}
				}
			}

			skip:	// �����܂ŃX�L�b�v

			//Next�Ɏ���Scene������
			pScene = pSceneNext;
		}
	}

	CDebugProc::Print("�`�搔 : %d", nCnt);
}
void CScene::DrawSeting(bool bSelect, int nNum, bool bStart, int nEnd)
{
	CScene *pScene;
	int nCnt = 0;

	if (NUM_PRIORITY <= nEnd)
		nEnd = NUM_PRIORITY;

	if ((nEnd) <= nNum)
		return;

	for (int nPriority = nNum; nPriority < NUM_PRIORITY; nPriority++)
	{
		pScene = m_pTop[nPriority];

		while (pScene != NULL)
		{//�擪��NULL�ł͂Ȃ�������
		 //Update��Uninit����Ă��܂��ꍇ�@Next��������\�������邩��Next�Ƀf�[�^���c���Ă���
			CScene *pSceneNext = pScene->m_pNext;

			if (pScene->m_ObjType != OBJTYPE_3DPOLYGON && pScene->m_ObjType != OBJTYPE_DISPEFFECT)
			{
				if (pScene->m_pDeath == true)
				{
					pScene->Delete();	//�폜
				}
				else if (pScene->m_bDraw == true)
				{
					pScene->Draw();

					if (pScene->GetObjType() == OBJTYPE_OBJECT)
					{
						nCnt++;
					}
				}
			}
			//Next�Ɏ���Scene������
			pScene = pSceneNext;
		}

		if (bSelect == true)
		{
			if (bStart == false || (bStart == true && (nEnd - 1) <= nPriority))
				break;
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

