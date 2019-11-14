//=============================================================================
//
// レンダラー処理 [scene.cpp]
// Author : 有馬武志
//
//=============================================================================
#include "scene.h"
#include "scene2D.h"
#include "renderer.h"
#include "debugproc.h"
#include "object.h"
#include "billboord.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

////-------------------------------------------------------------------------------
////	                    ～静的メンバ変数宣言～
////-------------------------------------------------------------------------------
//--- 初期値を必ず入れる ---//
CScene *CScene::m_pTop[NUM_PRIORITY] = {};
CScene *CScene::m_pCur[NUM_PRIORITY] = {};
int CScene::m_nNumAll = 0;
int CScene::m_nUpdatePri = 0;
bool CScene::m_bOnOff = false;

//===============================================================================
//　デフォルトコンストラクタ
//===============================================================================
CScene::CScene(int nPriority,OBJTYPE objType)
{
	//最初だけ入ればいい
	if (m_pTop[nPriority] == NULL)
	{//トップにCScene入れる
		m_pTop[nPriority] = this;
	}
	else
	{//現在の次にthisを入れる
		m_pCur[nPriority]->m_pNext = this;
	}

	m_ObjType = objType;

	m_pNext = NULL;

	//前に現在を入れる
	m_pPrev = m_pCur[nPriority];

	//現在にthisを入れる
	m_pCur[nPriority] = this;

	//優先順位を入れる
	m_nPriority = nPriority;

	//死亡フラグの初期化
	m_pDeath = false;

	//描画フラグの初期化
	m_bDraw = true;
}
//===============================================================================
//　デストラクタ
//===============================================================================
CScene::~CScene()
{

}
//===============================================================================
//	全てのクリーン
//===============================================================================
void CScene::ReleseAll(void)
{
	for (int nPriority = 0; nPriority < NUM_PRIORITY; nPriority++)
	{
		while (m_pTop[nPriority] != NULL)
		{//現在がNULL出ない限り回る
			if (m_pTop[nPriority]->m_pDeath == true)
			{
				m_pTop[nPriority]->Uninit();
			}

			//削除
			m_pTop[nPriority]->Delete();
		}
	}
}
//===============================================================================
//	fadeだけを消さない
//===============================================================================
void CScene::NotFadeReleseAll(void)
{
	for (int nPriority = 0; nPriority < NUM_PRIORITY - 1; nPriority++)
	{
		while (m_pTop[nPriority] != NULL)
		{//現在がNULL出ない限り回る
			m_pTop[nPriority]->Uninit();

			//削除
			m_pTop[nPriority]->Delete();
		}
	}
}
//===============================================================================
//	全てのアップデート
//===============================================================================
void CScene::UpdateAll(void)
{
	//個数を数える変数
	int nPriNum = 0;
	CScene *pScene;

	for (int nPriority = m_nUpdatePri; nPriority < NUM_PRIORITY; nPriority++)
	{
		pScene = m_pTop[nPriority];

		while (pScene != NULL)
		{
			//UpdateでUninitされてしまう場合　Nextが消える可能性があるからNextにデータを残しておく
			CScene *pSceneNext = pScene->m_pNext;

			if (pScene->m_pDeath == false)
			{
				pScene->Update();	//更新処理
			}
			if (pScene->m_pDeath == true)
			{
				pScene->Delete();	//削除
			}

			//Nextに次のSceneを入れる
			pScene = pSceneNext;
		}
	}
}
//===============================================================================
//	全ての描画
//===============================================================================
void CScene::DrawAll(void)
{
	CScene *pScene;
	int nCnt = 0;

	for (int nPriority = 0; nPriority < NUM_PRIORITY; nPriority++)
	{
		pScene = m_pTop[nPriority];

		while (pScene != NULL)
		{//先頭がNULLではない限り回る
			//UpdateでUninitされてしまう場合　Nextが消える可能性があるからNextにデータを残しておく
			CScene *pSceneNext = pScene->m_pNext;

			if (pScene->m_ObjType == OBJTYPE_BILLBOORD)
			{
				CBillBoord *pBillBoord = (CBillBoord*)pScene;
				if (pBillBoord->GetMapOn() == true)
					goto skip;		// スキップするよ
			}

			if (pScene->m_pDeath == true)
			{
				pScene->Delete();	//削除
			}
			else if (pScene->m_bDraw == true)
			{
				pScene->Draw();

				if (pScene->GetObjType() == OBJTYPE_OBJECT)
				{
					nCnt++;
				}
			}
		
			skip:	// ここまでスキップ

			//Nextに次のSceneを入れる
			pScene = pSceneNext;
		}
	}

	CDebugProc::Print("描画数 : %d", nCnt);
}
void CScene::DrawMap(void)
{
	CScene *pScene;
	int nCnt = 0;

	for (int nPriority = 0; nPriority < NUM_PRIORITY; nPriority++)
	{
		pScene = m_pTop[nPriority];

		while (pScene != NULL)
		{//先頭がNULLではない限り回る
			//UpdateでUninitされてしまう場合　Nextが消える可能性があるからNextにデータを残しておく
			CScene *pSceneNext = pScene->m_pNext;

			if (pScene->m_ObjType == OBJTYPE_COLMESH || pScene->m_ObjType == OBJTYPE_OBJECT || pScene->m_ObjType == OBJTYPE_BILLBOORD)
			{
				if (pScene->m_ObjType == OBJTYPE_OBJECT)
				{
					CObject *pObject = (CObject*)pScene;
					if (pObject->GetType() != 3 && pObject->GetType() != 4)
						goto skip;		// スキップするよ
				}
				else if (pScene->m_ObjType == OBJTYPE_BILLBOORD)
				{
					CBillBoord *pBillBoord = (CBillBoord*)pScene;
					if (pBillBoord->GetMapOn() == false)
						goto skip;		// スキップするよ
				}

				if (pScene->m_pDeath == true)
				{
					pScene->Delete();	//削除
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

			skip:	// ここまでスキップ

			//Nextに次のSceneを入れる
			pScene = pSceneNext;
		}
	}

	CDebugProc::Print("描画数 : %d", nCnt);
}
void CScene::Draw2D(void)
{
	CScene *pScene;
	int nCnt = 0;

	for (int nPriority = 5; nPriority < NUM_PRIORITY; nPriority++)
	{
		pScene = m_pTop[nPriority];

		while (pScene != NULL)
		{//先頭がNULLではない限り回る
		 //UpdateでUninitされてしまう場合　Nextが消える可能性があるからNextにデータを残しておく
			CScene *pSceneNext = pScene->m_pNext;

			if (pScene->m_pDeath == true)
			{
				pScene->Delete();	//削除
			}
			else if (pScene->m_bDraw == true)
			{
				pScene->Draw();

				if (pScene->GetObjType() == OBJTYPE_OBJECT)
				{
					nCnt++;
				}
			}

			//Nextに次のSceneを入れる
			pScene = pSceneNext;
		}
	}

	CDebugProc::Print("描画数 : %d", nCnt);
}
//===============================================================================
//	単体の削除 死亡フラグをたてる
//===============================================================================
void CScene::Release(void)
{
	//死亡フラグをたてる
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
//	死亡フラグが立ったものを確認し入れ替える
//===============================================================================
void CScene::Delete(void)
{
	//死亡フラグが立ってるかを確認
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

		//自身の削除
		delete this;

		m_nNumAll--;
	}
}

