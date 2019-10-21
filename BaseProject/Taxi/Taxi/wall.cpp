//=============================================================================
//
// 壁処理 [wall.cpp]
// Author : 有馬武志
//
//=============================================================================
#include "wall.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "game.h"
#include "player.h"
#include "fade.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define WALL_TEX_1	"data\\TEXTURE\\wall\\wall000.jpg"			// テクスチャの名前
#define WALL_TEX_2	"data\\TEXTURE\\wall\\wall001.jpg"			// テクスチャの名前

//=============================================================================
// 静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9 CWall::m_pTexture[MAX_WALL_TEX] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CWall::CWall() : CScene3D(WALL_PRIOTITY, CScene::OBJTYPE_WALL)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CWall::~CWall()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CWall::Init()
{
	//初期化処理
	CScene3D::Init();

	//種類の設定
	SetTypeNumber(TYPE_WALL);

	if (m_nType == 9)
	{
		//CScene3D::SetNor(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CWall::Uninit(void)
{
	//for (int nCount = 0; nCount < MAX_WALL_TEX; nCount++)
	//{
	//	// テクスチャの破棄
	//	if (m_pTexture[nCount] != NULL)
	//	{
	//		m_pTexture[nCount]->Release();
	//		m_pTexture[nCount] = NULL;
	//	}
	//}

	CScene3D::Uninit();
}
//=============================================================================
// 更新処理
//=============================================================================
void CWall::Update(void)
{
	CScene3D::Update();
}
//=============================================================================
// 描画処理
//=============================================================================
void CWall::Draw(void)
{
	//ゲームの情報
	CManager::MODE pMode = CManager::GetMode();

	CScene3D::Draw();
}
//=============================================================================
// 生成処理
//=============================================================================
CWall * CWall::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR3 rot, int nTexType)
{
	CWall *pWall = NULL;

	//NULLチェック
	if (pWall == NULL)
	{//メモリの動的確保

		pWall = new CWall;

		if (pWall != NULL)
		{
			//種類の設定
			pWall->m_nType = nTexType;
			//初期化処理
			pWall->Init();
			//位置の設定
			pWall->m_pos = pos;
			//当たり判定用
			pWall->m_size = size;
			//テクスチャの設定
			pWall->BindTexture(m_pTexture[nTexType]);
			//位置と回転とサイズ
			pWall->SetWall(pos, rot, size);
		}
	}

	return pWall;
}
//=============================================================================
// 壁の当たり判定
//=============================================================================
bool CWall::CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fDepth)
{
	bool bTouch = false;

	D3DXVECTOR3 WallPos = GetPosition();
	D3DXVECTOR3 WallRot = GetRotation();
	D3DXVECTOR2 WallSize = GetSize();

	if (WallRot.y == (0.0f * D3DX_PI))
	{// Z向きの壁
		if (pPos->x <= WallPos.x + WallSize.x + 18.5f
			&& pPos->x >= WallPos.x - WallSize.x - 18.5f)
		{// 壁と同じz軸に存在している
			if (pPos->z <= WallPos.z + WallSize.x + 18.5f
				&& pPos->z >= WallPos.z - WallSize.x - 18.5f)
			{// 壁と同じx軸に存在している
				if (pPos->y < WallPos.y + WallSize.y && pPos->y + 100.0f >= WallPos.y)
				{// 壁よりも低い
					if (pPos->z >= WallPos.z - fDepth
						&& pPosOld->z <= WallPos.z - fDepth)
					{// 手前から壁を貫通した
						pPos->z = WallPos.z - fDepth;
						bTouch = true;
					}
					else if (pPos->z <= WallPos.z + fDepth
						&& pPosOld->z >= WallPos.z + fDepth)
					{// 奥から壁を貫通した
						pPos->z = WallPos.z + fDepth;
						bTouch = true;
					}
				}
			}
		}
	}			
	else if (WallRot.y == (0.5f * D3DX_PI))
	{// X向きの壁
		if (pPos->x <= WallPos.x + WallSize.x + 18.5f
			&& pPos->x >= WallPos.x - WallSize.x - 18.5f)
		{// 壁と同じz軸に存在している
			if (pPos->z <= WallPos.z + WallSize.x + 18.5f
				&& pPos->z >= WallPos.z - WallSize.x - 18.5f)
			{// 壁と同じx軸に存在している
				if (pPos->y < WallPos.y + WallSize.y && pPos->y + 100.0f >= WallPos.y)
				{// 壁よりも低い
					if (pPos->x >= WallPos.x - fDepth
						&& pPosOld->x <= WallPos.x - fDepth)
					{// 手前から壁を貫通した
						pPos->x = WallPos.x - fDepth;
						bTouch = true;
					}
					else if (pPos->x <= WallPos.x + fDepth
						&& pPosOld->x >= WallPos.x + fDepth)
					{// 奥から壁を貫通した
						pPos->x = WallPos.x + fDepth;
						bTouch = true;
					}
				}
			}
		}
	}

	return bTouch;
}
//=============================================================================
//	ステージ移動時に初期化するため
//=============================================================================
void CWall::DeleteWall(void)
{
	CScene *pScene;

	//プライオリティーチェック
	pScene = CScene::GetTop(WALL_PRIOTITY);

	//NULLチェック
	while (pScene != NULL)
	{
		//UpdateでUninitされてしまう場合　Nextが消える可能性があるからNextにデータを残しておく
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetDeath() == false)
		{//タイプが壁だったら
			if (pScene->GetObjType() == OBJTYPE_WALL)
			{
				pScene->Uninit();
			}
		}
		//Nextに次のSceneを入れる
		pScene = pSceneNext;
	}
}
//=============================================================================
// 読み込み処理
//=============================================================================
HRESULT CWall::Load(void)
{
	// デバイス取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャ生成
	D3DXCreateTextureFromFile(pDevice, WALL_TEX_1, &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, WALL_TEX_2, &m_pTexture[1]);
	
	return S_OK;
}

//=============================================================================
// 開放処理
//=============================================================================
void CWall::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_WALL_TEX; nCount++)
	{
		// テクスチャ破棄
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}