//=============================================================================
//
// キャラクター処理 [Character.cpp]
// Author : Ryo Sugimoto
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "Character.h"
#include "model.h"
#include "renderer.h"
#include "manager.h"
#include "gamecamera.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FILE_NAME_WOOD	"data\\TEXT\\Player\\WindWood.txt"// 読み込むtxtファイルの名前
#define FILE_TEXTURE		("data\\MODEL\\WindWood\\tree000.jpg")	//テクスチャの読み込み

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CPlayer::KEY_INFO		*CCharcter::m_pKeyInfo[MAX_MOTION];		// キー情報へのポインタ
int						CCharcter::m_nNumParts;					// パーツ数
int						CCharcter::m_aIndexParent[MAX_PARTS];	// 親のインデックス
CPlayer::KEY			CCharcter::m_aKayOffset[MAX_PARTS];		// オフセット情報
CPlayer::MOTION_INFO	CCharcter::m_aMotionInfo[MAX_MOTION];	// モーション情報
LPDIRECT3DTEXTURE9		CCharcter::m_pTexture = NULL;
int						CCharcter::m_nNumModel;					//モデル数
char					CCharcter::m_aFileNameModel[MAX_PARTS][256];

//=============================================================================
// ファイルの読み込み
//=============================================================================
void CCharcter::Load(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice,
		FILE_TEXTURE,
		&m_pTexture);

	//モデルのオフセットと読み込み
	FileLoad();
	CModel::SetParts();	//ニワトリ親子の読み込み
}

//=============================================================================
// ファイルの破棄
//=============================================================================
void CCharcter::Unload(void)
{
	CModel::PartsTypeUnLoad();

	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//==================================================================================================//
//    * 生成関数 *
//==================================================================================================//
CCharcter	*CCharcter::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CCharcter *pChar = NULL;
	pChar = new CCharcter;
	if (pChar != NULL)
	{
		pChar->Init();
		pChar->Setting(pos, rot);
	}

	return pChar;
}
//==================================================================================================//
//    * 設定関数 *
//==================================================================================================//
void	CCharcter::Setting(D3DXVECTOR3 &pos, D3DXVECTOR3 &rot)
{
	ResetCheck(m_pos, &m_fCola[0]);
	CModel::ParentModel(m_apModel, CModel::TYPE_WOOD);
	int &nMaxModel = CModel::GetnModelMax(CModel::TYPE_WOOD);
	for (int nCountIndex = 0; nCountIndex < nMaxModel; nCountIndex++)
	{
		m_apModel[nCountIndex]->BindTexture(m_pTexture);
		if (m_aIndexParent[nCountIndex] == -1)
		{
			//モデルの親を指定
			m_apModel[nCountIndex]->SetParent(NULL);
		}
		else
		{
			//モデルの親を指定
			m_apModel[nCountIndex]->SetParent(m_apModel[m_aIndexParent[nCountIndex]]);
		}

		m_apModel[nCountIndex]->SetPos(D3DXVECTOR3(m_pos.x + m_aKayOffset[nCountIndex].fposX,
			m_pos.y + m_aKayOffset[nCountIndex].fposY,
			m_pos.z + m_aKayOffset[nCountIndex].fposZ));
	}

	m_pos = pos;	m_rot = rot;
}
//==================================================================================================//
//    * 初期化関数 *
//==================================================================================================//
HRESULT	CCharcter::Init(void)
{
	//変数の初期化
	m_pos = INIT_VECTOR;					//位置
	m_rot = INIT_VECTOR;					//向き
	m_nCountFlame = 0;
	m_nKey = 0;
	m_nMotionType = 0;
	m_apModel = NULL;
	m_fCntState = 0.0f;
	m_p3D = NULL;
	m_bDraw = false;

	for (int nCount = 0; nCount < MAX_PLAYCOL; nCount++) { m_fCola[nCount] = 0.0f; }
	return S_OK;
}
//==================================================================================================//
//    * 終了関数 *
//==================================================================================================//
void	CCharcter::Uninit(void)
{
	if (m_p3D != NULL) { m_p3D->Uninit(); m_p3D = NULL; }
	int &nMaxModel = CModel::GetnModelMax(CModel::TYPE_WOOD);
	for (int nCount = 0; nCount < nMaxModel; nCount++)
	{
		if (m_apModel[nCount] != NULL)
		{
			//3DモデルのUninit
			m_apModel[nCount]->Uninit();
			delete m_apModel[nCount];
			m_apModel[nCount] = NULL;
		}
	}
	delete[] m_apModel;
	m_apModel = NULL;

	//死亡フラグを立てる
	Release();
}
//==================================================================================================//
//    * 更新関数 *
//==================================================================================================//
void	CCharcter::Update(void)
{
	if (m_bDraw) { UpdateMotion(); }
	m_bDraw = false;
}
//==================================================================================================//
//    * 描画関数 *
//==================================================================================================//
void	CCharcter::Draw(void)
{
	float fCola;
	if (!DrawCheck(m_pos, &m_fCola[0], fCola)) { return; }
	m_bDraw = true;

	//レンダリングクラスを取得
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX		  mtxRot, mtxTrans, mtx;			// 計算用マトリックス

														// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtx);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y + D3DX_PI, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxTrans);

	//拡大処理
	mtx._22 = 1.5f;
	mtx._44 = (1.0f / 0.375f);
	mtx._41 *= mtx._44;
	mtx._42 *= mtx._44;
	mtx._43 *= mtx._44;

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	int &nMaxModel = CModel::GetnModelMax(CModel::TYPE_WOOD);
	for (int nCount = 0; nCount < nMaxModel; nCount++)
	{//モデルの描画
		if (m_apModel[nCount] != NULL)
		{
			//描画する
			m_apModel[nCount]->Draw(fCola);
		}
	}

}
//=============================================================================
// 描画判定
//=============================================================================
bool	CCharcter::DrawCheck(D3DXVECTOR3 &pos, float *fCol, float &fCola)
{
	fCola = 1.0f;
	int nNumber = CGame::GetCameraNumber();
	CGameCamera *pCamera = CGame::GetGameCamera(nNumber);
	if (pCamera == NULL) { return true; }

	if (pCamera->Clipping(pos, pos, pos))
	{
		fCol[nNumber] -= 0.05f;
		if (fCol[nNumber] < 0.0f) { fCol[nNumber] = 0.0f; return false; }
	}
	else
	{
		fCol[nNumber] += 0.05f;
		if (fCol[nNumber] > 1.0f) { fCol[nNumber] = 1.0f; }
	}
	fCola = fCol[nNumber];
	return true;
}
//=============================================================================
// 描画判定の初期化
//=============================================================================
void	CCharcter::ResetCheck(D3DXVECTOR3 &pos, float *fCol)
{
	CGameCamera *pCamera;
	for (int nCnt = 0; nCnt < MAX_PLAYCOL; nCnt++)
	{
		pCamera = CGame::GetGameCamera(nCnt);
		if (pCamera == NULL) { fCol[nCnt] = 1.0f; continue; }
		if (pCamera->Clipping(pos, pos, pos))
		{
			fCol[nCnt] = 1.0f;
		}
		else
		{
			fCol[nCnt] = 0.0f;
		}
	}
}
//=============================================================================
// 全ての当たり判定
//=============================================================================
void	CCharcter::CollisionAll(CPlayer *pPlayer)
{
	CScene *pScene, *pNext;

	pScene = CScene::GetTop(CHAR_PRIORITY);
	while (pScene != NULL)
	{
		pNext = pScene->GetNext();
		if (pScene->GetObjType() == OBJTYPE_WINDWOOD)
		{//木なら
			((CCharcter*)pScene)->CollisionWood(pPlayer);
		}
		pScene = pNext;
	}
}
//=============================================================================
// 当たり判定
//=============================================================================
void	CCharcter::Collision(CPlayer *&pPlayer, D3DXVECTOR3 &Mypos)
{
	D3DXVECTOR3 &pos = pPlayer->GetPos();
	float fLenght = sqrtf(powf(pos.x - Mypos.x, 2.0f) + powf(pos.z - Mypos.z, 2.0f));
	float fRange = PLAYER_LENGTH + WOOD_LEN;
	if (fLenght < fRange)
	{// 範囲内
	 // 角度計算
		float fAngle = atan2f(pos.x - Mypos.x, pos.z - Mypos.z);

		// 位置調整
		pos = D3DXVECTOR3(Mypos.x + sinf(fAngle) * fRange, pos.y, Mypos.z + cosf(fAngle) * fRange);

		//弾く
		pPlayer->Strike(NULL, Mypos, INIT_VECTOR);
	}
}
//=============================================================================
// 木の当たり判定
//=============================================================================
void	CCharcter::CollisionWood(CPlayer *&pPlayer)
{
	D3DXVECTOR3 pos;
	float fRot;
	for (int nCount = 0; nCount < 2; nCount++)
	{
		fRot = m_rot.y - D3DX_PI * 0.5f + D3DX_PI * (float)nCount;
		pos = m_pos + D3DXVECTOR3(sinf(fRot), 0.0f, cosf(fRot)) * WOOD_DIS;
		Collision(pPlayer, pos);
	}
}
//=============================================================================
// モーション
//=============================================================================
void CCharcter::UpdateMotion(void)
{
	//モーション
	CPlayer::KEY *pKey, *pNextKey;
	float fRateMotion;
	float fDiffMotion;
	D3DXVECTOR3 rotmotion;
	D3DXVECTOR3 posmotion;

	//キーが最大数を上回らないように
	if (m_aMotionInfo[m_nMotionType].nNumKey <= m_nKey)
	{
		m_nKey = 0;
	}

	//モーション更新
	for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
	{
		if (m_apModel[nCntParts] != NULL)
		{
			//現在のキーを取得
			pKey = &m_pKeyInfo[m_nMotionType][m_nKey].aKey[nCntParts];
			//次のキーを取得
			pNextKey = &m_pKeyInfo[m_nMotionType][(m_nKey + 1) % m_aMotionInfo[m_nMotionType].nNumKey].aKey[nCntParts];

			//現在のキーから次のキーへの再生フレーム数におけるモーションカウンターの相対値を算出
			fRateMotion = (float)m_nCountFlame / (float)m_pKeyInfo[m_nMotionType][m_nKey].nFrame;

			//ROT
			//現在のキーと次のキーの各要素の差分を算出
			fDiffMotion = pNextKey->frotX - pKey->frotX;
			//相対値を差分を使って各要素の値を算出
			rotmotion.x = pKey->frotX + (fDiffMotion * fRateMotion);

			//POS
			//現在のキーと次のキーの各要素の差分を算出
			fDiffMotion = pNextKey->fposX - pKey->fposX;
			//相対値を差分を使って各要素の値を算出
			posmotion.x = pKey->fposX + (fDiffMotion * fRateMotion);


			//現在のキーと次のキーの各要素の差分を算出
			fDiffMotion = pNextKey->frotY - pKey->frotY;
			//相対値を差分を使って各要素の値を算出
			rotmotion.y = pKey->frotY + (fDiffMotion * fRateMotion);
			//POS
			//現在のキーと次のキーの各要素の差分を算出
			fDiffMotion = pNextKey->fposY - pKey->fposY;
			//相対値を差分を使って各要素の値を算出
			posmotion.y = pKey->fposY + (fDiffMotion * fRateMotion);


			//現在のキーと次のキーの各要素の差分を算出
			fDiffMotion = pNextKey->frotZ - pKey->frotZ;
			//相対値を差分を使って各要素の値を算出
			rotmotion.z = pKey->frotZ + (fDiffMotion * fRateMotion);
			//POS
			//現在のキーと次のキーの各要素の差分を算出
			fDiffMotion = pNextKey->fposZ - pKey->fposZ;
			//相対値を差分を使って各要素の値を算出
			posmotion.z = pKey->fposZ + (fDiffMotion * fRateMotion);


			//パーツを動かす
			m_apModel[nCntParts]->SetRot(rotmotion);

			//POS
			m_apModel[nCntParts]->SetPos(D3DXVECTOR3(m_aKayOffset[nCntParts].fposX + posmotion.x,
				m_aKayOffset[nCntParts].fposY + posmotion.y,
				m_aKayOffset[nCntParts].fposZ + posmotion.z));
		}
	}

	//ループの判定
	switch (m_aMotionInfo[m_nMotionType].bLoop)
	{
	case true:
		//ループする
		//フレームを進める
		m_nCountFlame++;
		//キーの更新
		if (m_nCountFlame >= m_pKeyInfo[m_nMotionType][m_nKey].nFrame)
		{
			if (m_aMotionInfo[m_nMotionType].nNumKey - 1 == m_nKey)
			{
				m_nKey = 0;
			}
			else
			{
				m_nKey += 1;
			}
			m_nCountFlame = 0;
		}

		break;
	case false:
		//ループしない
		if (m_aMotionInfo[m_nMotionType].nNumKey - 1 > m_nKey)
		{//フレームを進める
			m_nCountFlame++;
		}
		else if (m_aMotionInfo[m_nMotionType].nNumKey - 1 == m_nKey)
		{
			//if (m_nAnimnow == PLAYERANIM_ATTACK)
			//{//攻撃モーション
			//	m_nAttackDelay++;
			//	if (m_nAttackDelay > 20)
			//	{
			//		m_bAttack = false;
			//		m_nAttackDelay = 0;
			//	}
			//}
			m_bMotionEnd = true;
		}
		//キーの更新
		if (m_nCountFlame >= m_pKeyInfo[m_nMotionType][m_nKey].nFrame)
		{
			if (m_aMotionInfo[m_nMotionType].nNumKey > m_nKey)
			{
				m_nKey += 1;
			}
			m_nCountFlame = 0;
		}
		break;
	}

#ifdef  _DEBUG
	/*CDebugProc::Print(" Numキー  : (%d)\n", m_nKey);
	CDebugProc::Print(" m_nCountFlame  : (%d)\n", m_nCountFlame);*/

#endif

}

//=============================================================================
// ファイル読み込み
//=============================================================================
void CCharcter::FileLoad(void)
{
	//デバイスを取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//ファイル読み込み用変数
	FILE *pFile;		//ファイルポインタ
	char *pStrcur;		//現在の先頭の文字列
	char aLine[256];	//文字列
	char aStr[256];		//一時保存文字列
	int nIndex = 0;		//現在のインデックス
	int nWord = 0;		//ポップで返された値を保持

#if 1
						//ファイルを開く 読み込み
	pFile = fopen(FILE_NAME_WOOD, "r");
	//NULLチェック
	if (pFile != NULL)
	{
		for (int nCntMotion = 0; nCntMotion < MAX_MOTION; )
		{
			//文字列の先頭を設定
			pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);
			//文字列を取り出す
			strcpy(aStr, pStrcur);

			//文字列のデータ 比較する文字列 比較する文字数
			if (memcmp(pStrcur, "NUM_MODEL = ", strlen("NUM_MODEL = ")) == 0)
			{
				//頭出し
				pStrcur += strlen("NUM_MODEL = ");
				//文字列の先頭を設定
				strcpy(aStr, pStrcur);
				//文字列抜き出し
				m_nNumModel = atoi(pStrcur);

				for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
				{
					int nNumber = nCntModel + CModel::PARTS_WOOD_LEG;
					LPD3DXBUFFER &m_pBuffMat = CModel::GetpBuffMat(nNumber);
					DWORD &m_nNumMat = CModel::GetnNumMat(nNumber);
					LPD3DXMESH &m_pMesh = CModel::GetpMesh(nNumber);

					//文字列の先頭を設定
					pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);
					//文字列を取り戻す
					strcpy(aStr, pStrcur);
					if (memcmp(pStrcur, "MODEL_FILENAME = ", strlen("MODEL_FILENAME = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("MODEL_FILENAME = ");

						//文字列を設定
						strcpy(aStr, pStrcur);

						//必要な文字列の最後の文字までの文字数を数える
						int nNullNum = CPlayer::PopString(pStrcur, &aStr[0]);

						//文字列を取り戻す
						strcpy(aStr, pStrcur);

						//最後の文字にNULL文字を入れる
						aStr[nNullNum - 1] = '\0';

						//対象の文字列から抜き出し
						strcpy(&m_aFileNameModel[nCntModel][0], aStr);

						//Xファイルの読み込み
						D3DXLoadMeshFromX(&m_aFileNameModel[nCntModel][0],
							D3DXMESH_SYSTEMMEM,
							pDevice,
							NULL,
							&m_pBuffMat,
							NULL,
							&m_nNumMat,
							&m_pMesh);
						aStr[nNullNum - 1] = '\0';

					}
				}
				//文字列の先頭を設定
				pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);
				//文字列を取り出す
				strcpy(aStr, pStrcur);
			}


			//文字列のデータ 比較する文字列 比較する文字数
			if (memcmp(pStrcur, "CHARACTERSET", 12) == 0)
			{
				while (1)
				{
					//文字列の先頭を設定
					pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);
					//文字列を取り出す
					strcpy(aStr, pStrcur);
					if (memcmp(pStrcur, "NUM_PARTS = ", strlen("NUM_PARTS = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("NUM_PARTS = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						m_nNumParts = atoi(pStrcur);
					}
					if (memcmp(pStrcur, "PARTSSET", strlen("PARTSSET")) == 0)
					{
						while (1)
						{
							//文字列の先頭を設定
							pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);

							//INDEXを読み込み
							if (memcmp(pStrcur, "INDEX = ", strlen("INDEX = ")) == 0)
							{
								//頭出し
								pStrcur += strlen("INDEX = ");
								//文字列の先頭を設定
								strcpy(aStr, pStrcur);
								//文字列抜き出し
								nIndex = atoi(pStrcur);
							}
							//PARENTを読み込み
							if (memcmp(pStrcur, "PARENT = ", strlen("PARENT = ")) == 0)
							{
								//頭出し
								pStrcur += strlen("PARENT = ");
								//文字列の先頭を設定
								strcpy(aStr, pStrcur);
								//文字列抜き出し
								m_aIndexParent[nIndex] = atoi(pStrcur);
							}
							//POSを読み込み
							if (memcmp(pStrcur, "POS = ", strlen("POS = ")) == 0)
							{
								//頭出し
								pStrcur += strlen("POS = ");
								//文字列の先頭を設定
								strcpy(aStr, pStrcur);

								//文字数を返してもらう
								nWord = CPlayer::PopString(pStrcur, &aStr[0]);
								//文字列変換
								m_aKayOffset[nIndex].fposX = (float)atof(pStrcur);
								//文字数分進める
								pStrcur += nWord;

								//文字数を返してもらう
								nWord = CPlayer::PopString(pStrcur, &aStr[0]);
								//文字列変換
								m_aKayOffset[nIndex].fposY = (float)atof(pStrcur);
								//文字数分進める
								pStrcur += nWord;

								//文字数を返してもらう
								nWord = CPlayer::PopString(pStrcur, &aStr[0]);
								//文字列変換
								m_aKayOffset[nIndex].fposZ = (float)atof(pStrcur);

							}
							//ROTを読み込み
							if (memcmp(pStrcur, "ROT = ", strlen("ROT = ")) == 0)
							{
								//頭出し
								pStrcur += strlen("ROT = ");
								//文字列の先頭を設定
								strcpy(aStr, pStrcur);

								//文字数を返してもらう
								nWord = CPlayer::PopString(pStrcur, &aStr[0]);
								//文字列変換
								m_aKayOffset[nIndex].frotX = (float)atof(pStrcur);

								//文字数分進める
								pStrcur += nWord;
								//文字数を返してもらう
								nWord = CPlayer::PopString(pStrcur, &aStr[0]);
								//文字列変換
								m_aKayOffset[nIndex].frotY = (float)atof(pStrcur);

								//文字数分進める
								pStrcur += nWord;
								//文字数を返してもらう
								nWord = CPlayer::PopString(pStrcur, &aStr[0]);
								//文字列変換
								m_aKayOffset[nIndex].frotZ = (float)atof(pStrcur);
							}
							//パーツセット終了
							else if (memcmp(pStrcur, "END_PARTSSET", strlen("END_PARTSSET")) == 0)
							{
								break;
							}
						}
					}
					//キャラクターセット終了
					else if (memcmp(pStrcur, "END_CHARACTERSET", strlen("END_CHARACTERSET")) == 0)
					{
						break;
					}
				}
				//文字列の先頭を設定
				pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);
				//文字列を取り出す
				strcpy(aStr, pStrcur);
			}

			//モーション読み込み
			if (memcmp(pStrcur, "MOTIONSET", strlen("MOTIONSET")) == 0)
			{
				//頭出し
				pStrcur += strlen("MOTIONSET");

				while (1)
				{
					//文字列の先頭を設定
					pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);
					//文字列を取り出す
					strcpy(aStr, pStrcur);

					if (memcmp(pStrcur, "LOOP = ", strlen("LOOP = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("LOOP = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);

						switch (atoi(pStrcur))
						{
						case 0:
							//文字列抜き出し
							m_aMotionInfo[nCntMotion].bLoop = false;
							break;
						case 1:
							//文字列抜き出し
							m_aMotionInfo[nCntMotion].bLoop = true;
							break;
						}
						//文字列の先頭を設定
						pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);
					}
					if (memcmp(pStrcur, "NUM_KEY = ", strlen("NUM_KEY = ")) == 0)
					{
						//頭出し
						pStrcur += strlen("NUM_KEY = ");
						//文字列の先頭を設定
						strcpy(aStr, pStrcur);
						//文字列抜き出し
						m_aMotionInfo[nCntMotion].nNumKey = atoi(pStrcur);

						//文字列の先頭を設定
						pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);
						//文字列を取り出す
						strcpy(aStr, pStrcur);
					}

					//キーの設定
					for (int nCntKey = 0; nCntKey < m_aMotionInfo[nCntMotion].nNumKey;)
					{
						if (memcmp(pStrcur, "KEYSET", strlen("KEYSET")) == 0)
						{
							//頭出し
							pStrcur += strlen("KEYSET");
							//文字列の先頭を設定
							strcpy(aStr, pStrcur);
							//文字列の先頭を設定
							pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);

							if (memcmp(pStrcur, "FRAME = ", strlen("FRAME = ")) == 0)
							{
								//頭出し
								pStrcur += strlen("FRAME = ");

								m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].nFrame = atoi(pStrcur);

								//文字列の先頭を設定
								strcpy(aStr, pStrcur);
								//文字列の先頭を設定
								pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);
							}

							//パーツ分回す
							for (int nCntParts = 0; nCntParts < m_nNumParts;)
							{
								if (memcmp(pStrcur, "KEY", strlen("KEY")) == 0)
								{
									//文字列の先頭を設定
									pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);

									if (memcmp(pStrcur, "POS = ", strlen("POS = ")) == 0)
									{
										//頭出し
										pStrcur += strlen("POS = ");
										//文字列の先頭を設定
										strcpy(aStr, pStrcur);

										//文字数を返してもらう
										nWord = CPlayer::PopString(pStrcur, &aStr[0]);
										//POS.X代入
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].fposX = (float)atof(pStrcur);
										//文字数分進める
										pStrcur += nWord;

										//文字数を返してもらう
										nWord = CPlayer::PopString(pStrcur, &aStr[0]);
										//POS.Y代入
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].fposY = (float)atof(pStrcur);
										//文字数分進める
										pStrcur += nWord;

										//文字数を返してもらう
										nWord = CPlayer::PopString(pStrcur, &aStr[0]);
										//POS.Z代入
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].fposZ = (float)atof(pStrcur);
										//文字列の先頭を設定
										pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);
									}
									//ROTを読み込み
									if (memcmp(pStrcur, "ROT = ", strlen("ROT = ")) == 0)
									{
										//頭出し
										pStrcur += strlen("ROT = ");
										//文字列の先頭を設定
										strcpy(aStr, pStrcur);

										//文字数を返してもらう
										nWord = CPlayer::PopString(pStrcur, &aStr[0]);
										//RotX
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].frotX = (float)atof(pStrcur);
										//文字数分進める
										pStrcur += nWord;

										//文字数を返してもらう
										nWord = CPlayer::PopString(pStrcur, &aStr[0]);
										//RotY
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].frotY = (float)atof(pStrcur);
										//文字数分進める
										pStrcur += nWord;

										//文字数を返してもらう
										nWord = CPlayer::PopString(pStrcur, &aStr[0]);
										//RotZ
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].frotZ = (float)atof(pStrcur);

										//文字列の先頭を設定
										pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);
									}
									if (memcmp(pStrcur, "END_KEY", strlen("END_KEY")) == 0)
									{
										//頭出し
										pStrcur += strlen("END_KEY");
										//文字列の先頭を設定
										strcpy(aStr, pStrcur);
										//文字列の先頭を設定
										pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);
										//パーツのカウントを進める
										nCntParts++;
									}
								}
								else
								{
									//文字列の先頭を設定
									pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);
								}
							}
							if (memcmp(pStrcur, "END_KEYSET", strlen("END_KEYSET")) == 0)
							{
								//文字列の先頭を設定
								pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);
								//カウントを進める
								nCntKey++;
							}
						}
						else
						{
							//文字列の先頭を設定
							pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);
						}

					}
					if (memcmp(pStrcur, "END_MOTIONSET", strlen("END_MOTIONSET")) == 0)
					{
						//モーションの情報をセット
						m_pKeyInfo[nCntMotion] = &m_aMotionInfo[nCntMotion].aKayInfo[0];
						nCntMotion++;
						break;
					}
				}
			}
			//スクリプトの終わり
			if (memcmp(pStrcur, "END_SCRIPT	", strlen("END_SCRIPT")) == 0)
			{
				break;
			}
		}
	}
	else
	{	//ファイルが開けなかった
		printf("ファイルが開けませんでした\n");
	}
	//ファイルを閉じる
	fclose(pFile);
#endif

}
