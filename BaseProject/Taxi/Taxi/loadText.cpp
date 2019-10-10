//=============================================================================
//
// テキストの読み込み処理 [LoadText.cpp]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "loadText.h"
#include "game.h"
#include "customer.h"
#include "humannpc.h"
#include "particlebillboad.h"
#include "manager.h"
#include "tutorial.h"
#include "objbillboad.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define VECTOR_ZERO (D3DXVECTOR3(0.0f, 0.0f, 0.0f))		//ゼロベクトル

//=============================================================================
// 生成処理
//=============================================================================
CLoadText * CLoadText::Create(void)
{
	//インスタンスの生成
	CLoadText * pLoadText = NULL;

	pLoadText->Init();
	return pLoadText;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CLoadText::CLoadText()
{
	m_pStrCur = NULL;
	m_aLine[256] = {};
	m_aStrCur[256] = {};
}

//=============================================================================
// デストラクタ
//=============================================================================
CLoadText::~CLoadText(){}

//=============================================================================
// 列の読み込み
//=============================================================================
char * CLoadText::ReadLine(FILE * pFile, char * pDest)
{
	//変数宣言
	char * pStrCur = NULL;

	while (1)
	{//行を飛ばす
		fgets(pDest, 256, pFile);					//一行分抜き出す
		pStrCur = pDest;							//先頭の文字列を保管する

		if (false == ((*pStrCur != '\0') && (*pStrCur != '\n') && (*pStrCur != '#'))) { continue; }

		while (1)
		{//列に有効な文字がないがチェックする
			if ((*pStrCur != '\n') && (*pStrCur != '\t') && (*pStrCur != ' ') && (*pStrCur != '#') && (*pStrCur != '\0'))
			{
				break;
			}

			pStrCur++;	//一文字ずつ進める
		}
		break;
	}

	return pStrCur;
}

//=============================================================================
// 先頭ポインタの文字を進める
//=============================================================================
char * CLoadText::AddChar(char * pStrCur, char * aStrCur, int nAddChar)
{
	pStrCur += nAddChar;						//先頭アドレスを文字数分進める
	strcpy(aStrCur, pStrCur);					//文字列を保管する

	return pStrCur;
}

//=============================================================================
// 文字列を抜き出す
//=============================================================================
int CLoadText::PopString(char * pStrCur, char * aStrCur)
{
	//変数宣言
	int nCounterWord = 0;	//進んだ文字をカウントする
	while (1)
	{
		aStrCur++;
		nCounterWord++;

		if ((*aStrCur == ' ') || (*aStrCur == '\t') || (*aStrCur == '#') || (*aStrCur == '\n'))
		{
			aStrCur[0] = '\0';
			nCounterWord++;
			break;
		}
	}

	return nCounterWord;
}

//=============================================================================
// ヌル文字の追加
//=============================================================================
void CLoadText::AddNull(char * pLine)
{
	//変数宣言
	char * cTop = &pLine[0];
	int nCounter = 0;

	while (1)
	{
		cTop++;
		nCounter++;

		if ((*cTop == '\n') || (*cTop == '\t') || (*cTop == ' ') || (*cTop == '#'))
		{
			pLine[nCounter] = '\0';	//無効文字を \0( ヌル文字 ) に置き換える
			break;
		}
	}
}

//*****************************************************************************
//
// モーション情報読み込みクラス
//
//*****************************************************************************

//=============================================================================
// 生成処理
//=============================================================================
CLoadTextMotion * CLoadTextMotion::Create(char * pFileName)
{
	//インスタンスの生成
	CLoadTextMotion * pLoadTextMotion = new CLoadTextMotion;

	pLoadTextMotion->SetLoadTextFileName(pFileName);	//読み込むテキストファイル名の保管
	pLoadTextMotion->Init();							//初期化処理

	return pLoadTextMotion;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CLoadTextMotion::CLoadTextMotion(){}

//=============================================================================
// デストラクタ
//=============================================================================
CLoadTextMotion::~CLoadTextMotion(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLoadTextMotion::Init(void)
{
	//変数の初期化
	m_pStrCur = NULL;			//文字列の先頭へのポインタ
	m_aLine[256] = {};			//文字列読み込み用（一行分）
	m_aStrCur[256] = {};		//文字列抜き出し用
	m_pMotionInfo = NULL;
	m_nMaxMotion = 0;
	m_ModelInfo.paFileName = NULL;
	m_ModelInfo.nMaxModel = 0;
	m_ModelInfo.nMaxParts = 0;
	m_ModelInfo.pParent = NULL;
	m_ModelInfo.pOfSetPos = NULL;
	m_ModelInfo.pOfsetRot = NULL;

	FILE *pFile;

	pFile = fopen(m_pLoadTextFileName, "r");	//テキストの読み込み

	if (pFile != NULL)
	{
		m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす

		while (memcmp(m_aLine, "END_SCRIPT", 10) != 0)
		{
			m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす

			if (memcmp(m_aLine, "NUM_MODEL = ", 12) == 0)		//読み込むモデル数
			{
				m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 12);
				m_ModelInfo.nMaxModel = atoi(m_pStrCur);

				//ファイル名格納用変数を動的確保する
				m_ModelInfo.paFileName = new char *[m_ModelInfo.nMaxModel];			//モデル数分確保する

				for (int nCntModel = 0; nCntModel < m_ModelInfo.nMaxModel; nCntModel++)
				{
					m_ModelInfo.paFileName[nCntModel] = new char[256];	//文字数分確保する
				}
				for (int nCntModel = 0; nCntModel < m_ModelInfo.nMaxModel; nCntModel++)
				{
					m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす

					if (memcmp(m_aLine, "MODEL_FILENAME = ", 17) != 0) { continue; }

					//Xファイル名を読み込む
					m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 17);
					strcpy(m_ModelInfo.paFileName[nCntModel], m_pStrCur);
					AddNull(m_ModelInfo.paFileName[nCntModel]);				//ファイル名の最後にNULLを追加する
				}
			}
			else if (memcmp(m_aLine, "CHARACTERSET", 12) == 0)	//キャラクターのオフセットの読み込み
			{
				while (memcmp(m_aLine, "END_CHARACTERSET", 16) != 0)
				{
					m_pStrCur = ReadLine(pFile, &m_aLine[0]);
					strcpy(m_aStrCur, m_pStrCur);

					if (memcmp(m_aStrCur, "NUM_PARTS = ", 12) == 0)	//使用パーツ数の読み込み
					{
						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 12);
						m_ModelInfo.nMaxParts = atoi(m_pStrCur);

						//各座標を動的確保する
						m_ModelInfo.pOfSetPos = new D3DXVECTOR3[m_ModelInfo.nMaxParts];
						m_ModelInfo.pOfsetRot = new D3DXVECTOR3[m_ModelInfo.nMaxParts];
						m_ModelInfo.pParent = new int[m_ModelInfo.nMaxParts];
					}
					else if (memcmp(m_aStrCur, "PARTSSET", 8) == 0)	//オフセットの読み込み
					{
						int nIndex = 0;	//インデックス格納用変数

						while (memcmp(m_aStrCur, "END_PARTSSET", 12) != 0)
						{
							m_pStrCur = ReadLine(pFile, &m_aLine[0]);
							strcpy(m_aStrCur, m_pStrCur);

							if (memcmp(m_aStrCur, "INDEX = ", 8) == 0)		//インデックス数の読み込み
							{
								m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 8);
								nIndex = atoi(m_pStrCur);
							}
							else if (memcmp(m_aStrCur, "PARENT = ", 9) == 0)	//親モデル番号の読み込み
							{
								m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 9);
								m_ModelInfo.pParent[nIndex] = atoi(m_pStrCur);
							}
							else if (memcmp(m_aStrCur, "POS = ", 6) == 0)		//座標の読み込み
							{
								m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 6);
								int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
								m_ModelInfo.pOfSetPos[nIndex].x = (float)atof(m_aStrCur);

								m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
								nWord = PopString(m_pStrCur, &m_aStrCur[0]);
								m_ModelInfo.pOfSetPos[nIndex].y = (float)atof(m_aStrCur);

								m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
								nWord = PopString(m_pStrCur, &m_aStrCur[0]);
								m_ModelInfo.pOfSetPos[nIndex].z = (float)atof(m_aStrCur);
							}
							else if (memcmp(m_aStrCur, "ROT = ", 6) == 0)
							{
								m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 6);
								int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
								m_ModelInfo.pOfsetRot[nIndex].x = (float)atof(m_aStrCur);	//オフセットを格納する

								m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
								nWord = PopString(m_pStrCur, &m_aStrCur[0]);
								m_ModelInfo.pOfsetRot[nIndex].y = (float)atof(m_aStrCur);

								m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
								nWord = PopString(m_pStrCur, &m_aStrCur[0]);
								m_ModelInfo.pOfsetRot[nIndex].z = (float)atof(m_aStrCur);
							}
						}
					}
				}
			}
			else if (memcmp(m_aLine, "MAX_MOTION = ", 13) == 0)	//モーションの最大数を取得
			{
				m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 13);
				m_nMaxMotion = atoi(m_pStrCur);

				//モーションデータの動的確保
				if (m_pMotionInfo == NULL) { m_pMotionInfo = new CMotion::MOTION_INFO[m_nMaxMotion]; }

				//モーションデータの読み込み
				for (int nCntMotion = 0; nCntMotion < m_nMaxMotion; nCntMotion++)
				{
					while (memcmp(m_aStrCur, "MOTIONSET", 9) != 0)
					{
						m_pStrCur = ReadLine(pFile, &m_aLine[0]);
						strcpy(m_aStrCur, m_pStrCur);
					}

					while (memcmp(m_aLine, "END_MOTIONSET", 13) != 0)
					{
						m_pStrCur = ReadLine(pFile, &m_aLine[0]);
						strcpy(m_aStrCur, m_pStrCur);

						if (memcmp(m_aStrCur, "LOOP = ", 7) == 0)				//ループ読み込み
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 7);
							m_pMotionInfo[nCntMotion].m_bLoop = (atoi(m_pStrCur) != 0);
						}
						else if (memcmp(m_aStrCur, "NUM_KEY = ", 10) == 0)	//キーの総数の読み込み
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 10);
							m_pMotionInfo[nCntMotion].nNumKey = atoi(m_pStrCur);

							//キー情報を動的確保する
							m_pMotionInfo[nCntMotion].m_pKeyInfo = new CMotion::KEY_INFO[m_pMotionInfo[nCntMotion].nNumKey];

							for (int nCntKey = 0; nCntKey < m_pMotionInfo[nCntMotion].nNumKey; nCntKey++)
							{
								//キーを動的確保する
								m_pMotionInfo[nCntMotion].m_pKeyInfo[nCntKey].aKey = new CMotion::KEY[m_ModelInfo.nMaxParts];	//パーツ数分確保する

								while (memcmp(m_aStrCur, "KEYSET", 6) != 0)
								{
									m_pStrCur = ReadLine(pFile, &m_aLine[0]);
									strcpy(m_aStrCur, m_pStrCur);
								}

								while (memcmp(m_aStrCur, "END_KEYSET", 10) != 0)	//キー情報の読み込み
								{
									m_pStrCur = ReadLine(pFile, &m_aLine[0]);
									strcpy(m_aStrCur, m_pStrCur);

									if (memcmp(m_aStrCur, "FRAME = ", 8) != 0) { continue; }	//フレーム数の読み込み
									m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 8);
									m_pMotionInfo[nCntMotion].m_pKeyInfo[nCntKey].nFrame = atoi(m_pStrCur);

									for (int nCntKeyParts = 0; nCntKeyParts < m_ModelInfo.nMaxParts; nCntKeyParts++)
									{
										while (memcmp(m_aStrCur, "KEY", 3) != 0)
										{
											m_pStrCur = ReadLine(pFile, &m_aLine[0]);
											strcpy(m_aStrCur, m_pStrCur);
										}

										while (memcmp(m_aStrCur, "END_KEY", 7) != 0)	//各パーツの位置、回転量を読み込む
										{
											m_pStrCur = ReadLine(pFile, &m_aLine[0]);
											strcpy(m_aStrCur, m_pStrCur);

											if (memcmp(m_aStrCur, "POS = ", 6) == 0)	//位置の読み込み
											{
												m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 6);
												int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
												m_pMotionInfo[nCntMotion].m_pKeyInfo[nCntKey].aKey[nCntKeyParts].fPosX = (float)atof(m_aStrCur);

												m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
												nWord = PopString(m_pStrCur, &m_aStrCur[0]);
												m_pMotionInfo[nCntMotion].m_pKeyInfo[nCntKey].aKey[nCntKeyParts].fPosY = (float)atof(m_aStrCur);

												m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
												nWord = PopString(m_pStrCur, &m_aStrCur[0]);
												m_pMotionInfo[nCntMotion].m_pKeyInfo[nCntKey].aKey[nCntKeyParts].fPosZ = (float)atof(m_aStrCur);
											}
											else if (memcmp(m_aStrCur, "ROT = ", 6) == 0)	//回転量の読み込み
											{
												m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 6);
												int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
												m_pMotionInfo[nCntMotion].m_pKeyInfo[nCntKey].aKey[nCntKeyParts].fRotX = (float)atof(m_aStrCur);

												m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
												nWord = PopString(m_pStrCur, &m_aStrCur[0]);
												m_pMotionInfo[nCntMotion].m_pKeyInfo[nCntKey].aKey[nCntKeyParts].fRotY = (float)atof(m_aStrCur);

												m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
												nWord = PopString(m_pStrCur, &m_aStrCur[0]);
												m_pMotionInfo[nCntMotion].m_pKeyInfo[nCntKey].aKey[nCntKeyParts].fRotZ = (float)atof(m_aStrCur);
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		fclose(pFile);	//読み込みを終了する
	}
	else
	{
		MessageBox(NULL, TEXT("pFileがNULLです"), TEXT("CLoadTextLoadMotion::Init()"), MB_OK);
	}

	//初期化する
	m_pStrCur = NULL;
	m_aLine[0] = '\0';
	m_aStrCur[0] = '\0';
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLoadTextMotion::Uninit(void)
{
	//モデル情報のポインタを破棄する
	if (m_ModelInfo.pOfSetPos != NULL)	//位置
	{
		delete[] m_ModelInfo.pOfSetPos;
		m_ModelInfo.pOfSetPos = NULL;
	}
	if (m_ModelInfo.pOfsetRot != NULL)	//向き
	{
		delete[] m_ModelInfo.pOfsetRot;
		m_ModelInfo.pOfsetRot = NULL;
	}
	if (m_ModelInfo.pParent != NULL)	//親情報
	{
		delete[] m_ModelInfo.pParent;
		m_ModelInfo.pParent = NULL;
	}
	for (int nCntModel = 0; nCntModel < m_ModelInfo.nMaxModel; nCntModel++)	//ファイル名
	{
		if (m_ModelInfo.paFileName[nCntModel] == NULL) { continue; }

			delete[] m_ModelInfo.paFileName[nCntModel];
			m_ModelInfo.paFileName[nCntModel] = NULL;
	}
	if (m_ModelInfo.paFileName != NULL)	//ファイルポインタ
	{
		delete[] m_ModelInfo.paFileName;
		m_ModelInfo.paFileName = NULL;
	}

	//パーツごとのキー情報を破棄する
	for (int nCntMotion = 0; nCntMotion < m_nMaxMotion; nCntMotion++)
	{
		for (int nCntKey = 0; nCntKey < m_pMotionInfo[nCntMotion].nNumKey; nCntKey++)
		{
			if (m_pMotionInfo[nCntMotion].m_pKeyInfo[nCntKey].aKey == NULL) { continue; }

			delete[] m_pMotionInfo[nCntMotion].m_pKeyInfo[nCntKey].aKey;
			m_pMotionInfo[nCntMotion].m_pKeyInfo[nCntKey].aKey = NULL;
		}
	}

	//キー情報を破棄する
	for (int nCntParts = 0; nCntParts < m_nMaxMotion; nCntParts++)
	{
		if (m_pMotionInfo[nCntParts].m_pKeyInfo == NULL) { continue; }

		delete[] m_pMotionInfo[nCntParts].m_pKeyInfo;
		m_pMotionInfo[nCntParts].m_pKeyInfo = NULL;
	}

	//モーションデータの破棄
	if (m_pMotionInfo != NULL)
	{
		delete[] m_pMotionInfo;
		m_pMotionInfo = NULL;
	}
}

//*****************************************************************************
//
// プレイヤーの情報読み込みクラス
//
//*****************************************************************************

//=============================================================================
// 生成処理
//=============================================================================
CLoadTextPlayer * CLoadTextPlayer::Create(char * pFileName)
{
	//インスタンスの生成
	CLoadTextPlayer * pLoadTextPlayer = new CLoadTextPlayer;

	pLoadTextPlayer->SetLoadTextFileName(pFileName);	//読み込むテキストファイル名の保管
	pLoadTextPlayer->Init();							//初期化処理

	return pLoadTextPlayer;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CLoadTextPlayer::CLoadTextPlayer(){}

//=============================================================================
// デストラクタ
//=============================================================================
CLoadTextPlayer::~CLoadTextPlayer(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLoadTextPlayer::Init(void)
{
	//変数の初期化
	m_pStrCur = NULL;			//文字列の先頭へのポインタ
	m_aLine[256] = {};			//文字列読み込み用（一行分）
	m_aStrCur[256] = {};		//文字列抜き出し用
	//CPlayer::PLAYER_INFO * pPlayerInfo = NULL;
	CManager::MODE mode = CManager::GetMode();

	//プレイヤー情報の初期化
	m_PlayerInfo.fAccel = 1.0f;				//加速値
	m_PlayerInfo.fBraks = 0.00f;			//減速値
	m_PlayerInfo.fAddRot = 0.00f;			//加える回転値
	m_PlayerInfo.fDistance = 0.0f;			//距離
	m_PlayerInfo.fWindy = 0.0f;				//風圧
	m_PlayerInfo.FirstPos = VECTOR_ZERO;	//初期位置

	//プレイヤーの情報を取得する
	//if (mode == CManager::MODE_GAME)
	//{//ゲーム
	//	pPlayerInfo = CGame::GetPlayer()->GetPlayerInfoPoint();	//プレイヤーの情報を取得
	//}
	//else if (mode == CManager::MODE_TUTORIAL)
	//{//チュートリアル
	//	pPlayerInfo = CTutorial::GetPlayer()->GetPlayerInfoPoint();	//プレイヤーの情報を取得
	//}

	FILE *pFile;

	pFile = fopen(m_pLoadTextFileName, "r");	//テキストの読み込み

	if (pFile != NULL)
	{
		m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす

		while (memcmp(m_aLine, "END_SCRIPT", 10) != 0)
		{
			m_pStrCur = ReadLine(pFile, &m_aLine[0]);		//無効文字を飛ばす

			if (memcmp(m_aLine, "POS = ", 6) == 0)
			{
				m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 6);
				int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
				m_PlayerInfo.FirstPos.x = (float)atof(m_aStrCur);

				m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
				nWord = PopString(m_pStrCur, &m_aStrCur[0]);
				m_PlayerInfo.FirstPos.y = (float)atof(m_aStrCur);

				m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
				m_PlayerInfo.FirstPos.z = (float)atof(m_aStrCur);
			}
			else if (memcmp(m_aLine, "ACCEL = ", 8) == 0)		//読み込むモデル数
			{
				m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 8);
				m_PlayerInfo.fAccel = (float)atof(m_aStrCur);
			}
			else if (memcmp(m_aLine, "BRAKS = ", 8) == 0)
			{
				m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 8);
				m_PlayerInfo.fBraks = (float)atof(m_aStrCur);
			}
			else if (memcmp(m_aLine, "ADDROT = ", 9) == 0)
			{
				m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 9);
				m_PlayerInfo.fAddRot = (float)atof(m_aStrCur);
			}
			else if (memcmp(m_aLine, "WINDY = ", 8) == 0)
			{
				m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 8);
				m_PlayerInfo.fWindy = (float)atof(m_aStrCur);
			}
		}
		fclose(pFile);	//読み込みを終了する
	}
	else
	{
		MessageBox(NULL, TEXT("pFileがNULLです"), TEXT("CLoadTextLoadMotion::Init()"), MB_OK);
	}

	//初期化する
	m_pStrCur = NULL;
	m_aLine[0] = '\0';
	m_aStrCur[0] = '\0';
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLoadTextPlayer::Uninit(void){}

//*****************************************************************************
//
// お客さんの情報読み込みクラス
//
//*****************************************************************************

//=============================================================================
// 生成処理
//=============================================================================
void CLoadTextCustomer::Create(char * pFileName)
{
	//インスタンスの生成
	CLoadTextCustomer * pLoadTextPlayer = new CLoadTextCustomer;

	pLoadTextPlayer->SetLoadTextFileName(pFileName);	//読み込むテキストファイル名の保管

	//初期化処理
	if (S_OK == pLoadTextPlayer->Init())
	{
		pLoadTextPlayer->Uninit();
		delete pLoadTextPlayer;
		pLoadTextPlayer = NULL;
	}
	else
	{
		MessageBox(NULL, TEXT("テキストの読み込みに失敗しました"), TEXT("CLoadTextCustomer::Init()"), MB_OK);
	}
}

//=============================================================================
// コンストラクタ
//=============================================================================
CLoadTextCustomer::CLoadTextCustomer() {}

//=============================================================================
// デストラクタ
//=============================================================================
CLoadTextCustomer::~CLoadTextCustomer() {}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLoadTextCustomer::Init(void)
{
	//変数の初期化
	m_pStrCur = NULL;					//文字列の先頭へのポインタ
	m_aLine[256] = {};					//文字列読み込み用（一行分）
	m_aStrCur[256] = {};				//文字列抜き出し用
	D3DXVECTOR3 Pos = VECTOR_ZERO;		//位置
	D3DXVECTOR3 GoalPos = VECTOR_ZERO;	//目的地
	int nTex = -1;						//目的地テクスチャ
	int nAddTime = 0;					//追加時間
	int nLimit = 0;						//乗車制限時間
	int nType = -1;						//モデルタイプの設定

	//テキストの読み込み
	FILE * pFile;
	pFile = fopen(m_pLoadTextFileName, "r");

	if(pFile != NULL)
	{
		m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす

		while (memcmp(m_aLine, "END_SCRIPT", 10) != 0)
		{
			m_pStrCur = ReadLine(pFile, &m_aLine[0]);		//無効文字を飛ばす

			if (memcmp(m_aLine, "CUSTOMERSET", 11) == 0)	//キャラクターのオフセットの読み込み
			{
				while (memcmp(m_aLine, "END_CUSTOMERSET", 15) != 0)
				{
					m_pStrCur = ReadLine(pFile, &m_aLine[0]);
					strcpy(m_aStrCur, m_pStrCur);

					if (memcmp(m_pStrCur, "POS = ", 6) == 0)
					{
						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 6);
						int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Pos.x = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Pos.y = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						Pos.z = (float)atof(m_aStrCur);
					}
					else if (memcmp(m_pStrCur, "GOAL = ", 7) == 0)		//読み込むモデル数
					{
						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 7);
						int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						GoalPos.x = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						GoalPos.y = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						GoalPos.z = (float)atof(m_aStrCur);
					}
					else if (memcmp(m_pStrCur, "GOAL_TEX = ", 11) == 0)
					{
						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 11);
						nTex = atoi(m_aStrCur);
					}
					else if (memcmp(m_pStrCur, "ADDTIME = ", 10) == 0)
					{
						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 10);
						nAddTime = atoi(m_aStrCur);
					}
					else if (memcmp(m_pStrCur, "LIMIT = ", 8) == 0)
					{
						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 8);
						nLimit = atoi(m_aStrCur);
					}
					else if (memcmp(m_pStrCur, "TYPE = ", 7) == 0)
					{
						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 7);
						nType = atoi(m_aStrCur);
					}
				}

				//お客さんの生成
				CCustomer::Create(Pos, GoalPos, nTex, nAddTime, nLimit, (CHumanBace::MODEL_TYPE)nType);
			}
		}
		fclose(pFile);	//読み込みを終了する
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLoadTextCustomer::Uninit(void) {}

//*****************************************************************************
//
// 人のNPC情報読み込みクラス
//
//*****************************************************************************

//=============================================================================
// 生成処理
//=============================================================================
void CLoadTextHumanNPC::Create(char * pFileName)
{
	//インスタンスの生成
	CLoadTextHumanNPC * pLoadTextBill = new CLoadTextHumanNPC;

	pLoadTextBill->SetLoadTextFileName(pFileName);	//読み込むテキストファイル名の保管

													//初期化処理
	if (S_OK == pLoadTextBill->Init())
	{
		pLoadTextBill->Uninit();
		delete pLoadTextBill;
		pLoadTextBill = NULL;
	}
	else
	{
		MessageBox(NULL, TEXT("テキストの読み込みに失敗しました"), TEXT("CLoadTextCustomer::Init()"), MB_OK);
	}
}

//=============================================================================
// コンストラクタ
//=============================================================================
CLoadTextHumanNPC::CLoadTextHumanNPC() {}

//=============================================================================
// デストラクタ
//=============================================================================
CLoadTextHumanNPC::~CLoadTextHumanNPC() {}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLoadTextHumanNPC::Init(void)
{
	//変数の初期化
	m_pStrCur = NULL;					//文字列の先頭へのポインタ
	m_aLine[256] = {};					//文字列読み込み用（一行分）
	m_aStrCur[256] = {};				//文字列抜き出し用
	D3DXVECTOR3 Pos = VECTOR_ZERO;		//位置
	int nType = -1;						//モデルタイプの設定

										//テキストの読み込み
	FILE * pFile;
	pFile = fopen(m_pLoadTextFileName, "r");

	if (pFile != NULL)
	{
		m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす

		while (memcmp(m_aLine, "END_SCRIPT", 10) != 0)
		{
			m_pStrCur = ReadLine(pFile, &m_aLine[0]);		//無効文字を飛ばす

			if (memcmp(m_aLine, "NPCSET", 5) == 0)	//キャラクターのオフセットの読み込み
			{
				while (memcmp(m_aLine, "END_NPCSET", 9) != 0)
				{
					m_pStrCur = ReadLine(pFile, &m_aLine[0]);
					strcpy(m_aStrCur, m_pStrCur);

					if (memcmp(m_pStrCur, "POS = ", 6) == 0)
					{//位置の取得
						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 6);
						int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Pos.x = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Pos.y = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						Pos.z = (float)atof(m_aStrCur);
					}
					else if (memcmp(m_pStrCur, "TYPE = ", 7) == 0)
					{//タイプの取得
						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 7);
						nType = atoi(m_aStrCur);
					}
				}

				//人NPCの生成
				CHumanNPC::Create(Pos, (CHumanBace::MODEL_TYPE)nType);
			}
		}
		fclose(pFile);	//読み込みを終了する
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLoadTextHumanNPC::Uninit(void) {}

//*****************************************************************************
//
// ビルボード読み込みクラス
//
//*****************************************************************************

//=============================================================================
// 生成処理
//=============================================================================
void CLoadTextObjBill::Create(char * pFileName)
{
	//インスタンスの生成
	CLoadTextObjBill * pLoadTextBill = new CLoadTextObjBill;

	pLoadTextBill->SetLoadTextFileName(pFileName);	//読み込むテキストファイル名の保管

	//初期化処理
	if (S_OK == pLoadTextBill->Init())
	{
		pLoadTextBill->Uninit();
		delete pLoadTextBill;
		pLoadTextBill = NULL;
	}
	else
	{
		MessageBox(NULL, TEXT("テキストの読み込みに失敗しました"), TEXT("CLoadTextCustomer::Init()"), MB_OK);
	}
}

//=============================================================================
// コンストラクタ
//=============================================================================
CLoadTextObjBill::CLoadTextObjBill() {}

//=============================================================================
// デストラクタ
//=============================================================================
CLoadTextObjBill::~CLoadTextObjBill() {}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLoadTextObjBill::Init(void)
{
	//変数の初期化
	m_pStrCur = NULL;					//文字列の先頭へのポインタ
	m_aLine[256] = {};					//文字列読み込み用（一行分）
	m_aStrCur[256] = {};				//文字列抜き出し用
	D3DXVECTOR3 Pos = VECTOR_ZERO;		//位置
	int nType = -1;						//モデルタイプの設定

										//テキストの読み込み
	FILE * pFile;
	pFile = fopen(m_pLoadTextFileName, "r");

	if (pFile != NULL)
	{
		m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす

		while (memcmp(m_aLine, "END_SCRIPT", 10) != 0)
		{
			m_pStrCur = ReadLine(pFile, &m_aLine[0]);		//無効文字を飛ばす

			if (memcmp(m_aLine, "OBJECT_START", 12) == 0)	//キャラクターのオフセットの読み込み
			{
				while (memcmp(m_aLine, "OBJECT_END", 10) != 0)
				{
					m_pStrCur = ReadLine(pFile, &m_aLine[0]);
					strcpy(m_aStrCur, m_pStrCur);

					if (memcmp(m_pStrCur, "POS = ", 6) == 0)
					{//位置の取得
						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 6);
						int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Pos.x = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Pos.y = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						Pos.z = (float)atof(m_aStrCur);
					}
					else if (memcmp(m_pStrCur, "TYPE = ", 7) == 0)
					{//タイプの取得
						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 7);
						nType = atoi(m_aStrCur);
					}
				}

				//オブジェクトビルボードの生成
				CObjBillboad::Create(Pos, nType);
			}
		}
		fclose(pFile);	//読み込みを終了する
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLoadTextObjBill::Uninit(void) {}

//*****************************************************************************
//
// エフェクト読み込みクラス
//
//*****************************************************************************

//=============================================================================
// 生成処理
//=============================================================================
CLoadTextEffect * CLoadTextEffect::Create(char * pFileName)
{
	//インスタンスの生成
	CLoadTextEffect * pLoadTextEffect = new CLoadTextEffect;

	pLoadTextEffect->SetLoadTextFileName(pFileName);	//読み込むファイル名を保管する
	pLoadTextEffect->Init();

	return pLoadTextEffect;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CLoadTextEffect::CLoadTextEffect() {}

//=============================================================================
// デストラクタ
//=============================================================================
CLoadTextEffect::~CLoadTextEffect() {}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLoadTextEffect::Init(void)
{
	//変数宣言
	int	 nCountParticle = 0;				//カウンター
	int	 nCountEffect = 0;					//エフェクトカウンター

											//変数の初期化
	m_pStrCur = NULL;
	m_aLine[256] = {};
	m_aStrCur[256] = {};
	m_TexInfo.ppFileName = NULL;
	m_nMaxEffect = 0;
	m_pEffect = NULL;

	//ファイル宣言
	FILE *pFile;

	pFile = fopen(m_pLoadTextFileName, "r");	//テキストの読み込み

	if (pFile != NULL)
	{
		m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす

		while (memcmp(m_aLine, "END_SCRIPT", 10) != 0)
		{
			m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす
			strcpy(m_aStrCur, m_pStrCur);

			//----------------------------------
			//テクスチャの最大数・情報の読み込み
			//----------------------------------
			if (memcmp(m_aStrCur, "MAX_TEX = ", 10) == 0)
			{
				m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 10);
				m_TexInfo.nMaxTex = atoi(m_pStrCur);

				//テクスチャファイル名を動的確保する
				if (m_TexInfo.ppFileName == NULL) { m_TexInfo.ppFileName = new char *[m_TexInfo.nMaxTex]; }

				if (m_TexInfo.ppFileName != NULL)
				{
					for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++)
					{//文字数分メモリを動的確保する
						m_TexInfo.ppFileName[nCntTex] = new char[256];
					}
				}

				for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++)
				{
					if (m_TexInfo.ppFileName[nCntTex] == NULL)
					{
						MessageBox(NULL, TEXT("テクスチャファイルポインタがNULLです"), TEXT("CLoadTextEffect::Init()"), MB_OK);
						break;
					}

					m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす
					strcpy(m_aStrCur, m_pStrCur);

					if (memcmp(m_aStrCur, "TEXTURE = ", 10) != 0) { continue; }

					//テクスチャのファイル名を取得
					m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 10);
					strcpy(m_TexInfo.ppFileName[nCntTex], m_pStrCur);
					AddNull(m_TexInfo.ppFileName[nCntTex]);			//ファイル名の最後にNULLを追加する
				}

				m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす
				strcpy(m_aStrCur, m_pStrCur);
			}

			//----------------------------------
			//エフェクトの最大数・情報の初期化
			//----------------------------------
			if (memcmp(m_aStrCur, "MAX_EFFECT = ", 13) == 0)
			{
				m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 13);
				m_nMaxEffect = atoi(m_pStrCur);

				//メモリの動的確保
				if (m_pEffect == NULL) { m_pEffect = new CEffectTool::Effect[m_nMaxEffect]; }	//エフェクトの個数分メモリを確保する

																							//変数の初期化
				for (int nCntEffect = 0; nCntEffect < m_nMaxEffect; nCntEffect++)
				{
					if (m_pEffect != NULL)
					{
						m_pEffect[nCntEffect].EffectInfo = new CEffectTool::Effect_Info[MAX_EFFECT];
					}

					if (m_pEffect[nCntEffect].EffectInfo != NULL)
					{
						for (int nCntParticle = 0; nCntParticle < MAX_EFFECT; nCntParticle++)
						{
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].bCreateEffect = false;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].bLoop = false;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].bUseGravity = true;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].bZbuffur = true;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].DrawType = CParticleBillboad::DRAWTYPE_ADD;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].fChangeCol_a = 0.15f;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].fGravity = 0.01f;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].fSpeed = 2.0f;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].nInterbalTime = 10;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].nCountTime = 0;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].nLife = 50;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].nMaxHight = 3;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].nMinHight = 2;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].nNumCreate = 1;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].nTexNum = 0;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].size = D3DXVECTOR3(20.0f, 20.0f, 0.0f);
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].Type = CParticleBillboad::TYPE_SPEW;
						}
					}
				}
			}

			//-------------------------------------------
			//エフェクトの設定
			//-------------------------------------------
			if (memcmp(m_aStrCur, "SETEFFECT", 9) == 0)
			{
				while (memcmp(m_aLine, "END_EFFECT", 10) != 0)
				{
					m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす
					strcpy(m_aStrCur, m_pStrCur);

					if (memcmp(m_aStrCur, "SETPARTICLE", 11) != 0) { continue; }
					while (memcmp(m_aStrCur, "END_PARTICLE", 12) != 0)
					{
						if (memcmp(m_aStrCur, "LOOP = ", 7) == 0)
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 7);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].bLoop = (atoi(m_pStrCur) != 0);	//ループ状態の設定

							m_pEffect[nCountEffect].EffectInfo[nCountParticle].bCreateEffect = true;			//生成状態の設定
						}
						else if (memcmp(m_aStrCur, "SIZE = ", 7) == 0)						//サイズの設定
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 7);
							int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].size.x = (float)atof(m_aStrCur);

							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
							nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].size.y = (float)atof(m_aStrCur);

							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].size.z = (float)atof(m_aStrCur);
						}
						else if (memcmp(m_aStrCur, "TEX = ", 6) == 0)						//テクスチャ番号の設定
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 6);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].nTexNum = atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "DRAWTYPE = ", 11) == 0)					//描画タイプの設定
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 11);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].DrawType = (CParticleBillboad::DRAWTYPE)atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "TYPE = ", 7) == 0)						//タイプの設定
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 7);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].Type = (CParticleBillboad::TYPE)atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "COLOR = ", 8) == 0)						//色の設定
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 8);
							int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].Color.r = (float)atof(m_aStrCur);

							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
							nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].Color.g = (float)atof(m_aStrCur);

							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
							nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].Color.b = (float)atof(m_aStrCur);

							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].Color.a = (float)atof(m_aStrCur);
						}
						else if (memcmp(m_aStrCur, "ZBUFFER = ", 10) == 0)
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 10);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].bZbuffur = (atoi(m_pStrCur) != 0);	//Zバッファの設定
						}
						else if (memcmp(m_aStrCur, "LIFE = ", 7) == 0)								//表示時間の設定
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 7);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].nLife = atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "CHANGECOLOR = ", 14) == 0)						//透明度の変化量
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 14);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].fChangeCol_a = (float)atof(m_aStrCur);
						}
						else if (memcmp(m_aStrCur, "USEGRAVITY = ", 13) == 0)
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 13);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].bUseGravity = (atoi(m_pStrCur) != 0);	//重力の状態設定
						}
						else if (memcmp(m_aStrCur, "TIME = ", 7) == 0)									//生成時間の設定
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 7);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].nInterbalTime = atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "NUMCREATE = ", 12) == 0)								//生成量の設定
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 12);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].nNumCreate = atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "SPEED = ", 8) == 0)									//スピードの設定
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 8);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].fSpeed = (float)atof(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "MINHIGHT = ", 11) == 0)								//高さの最小値の設定
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 11);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].nMinHight = atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "MAXHIGHT = ", 11) == 0)								//高さの最大値の設置
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 11);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].nMaxHight = atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "GRAVITY = ", 10) == 0)								//重力の設定
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 10);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].fGravity = (float)atof(m_pStrCur);
						}

						m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす
						strcpy(m_aStrCur, m_pStrCur);
					}

					//カウンターを進める
					nCountParticle++;
				}

				nCountEffect++;			//エフェクトカウンターを進める
				nCountParticle = 0;		//パーティクルカウンターの初期化
			}
		}
		fclose(pFile);	//ファイル読み込みを終わる
	}
	else
	{
		MessageBox(NULL, TEXT("pFileがNULLです"), TEXT("CLoadTextObject::Init()"), MB_OK);
	}

	//初期化する
	m_pStrCur = NULL;
	m_aLine[0] = '\0';
	m_aStrCur[0] = '\0';
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLoadTextEffect::Uninit(void)
{
	//エフェクトの情報破棄
	for (int nCntEffect = 0; nCntEffect < m_nMaxEffect; nCntEffect++)
	{
		if (m_pEffect[nCntEffect].EffectInfo != NULL)
		{
			delete[] m_pEffect[nCntEffect].EffectInfo;
			m_pEffect[nCntEffect].EffectInfo = NULL;
		}
	}
	if (m_pEffect != NULL)
	{
		delete[] m_pEffect;
		m_pEffect = NULL;
	}

	//テクスチャ情報の破棄
	for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++)
	{
		if (m_TexInfo.ppFileName[nCntTex] != NULL)
		{
			delete[] m_TexInfo.ppFileName[nCntTex];
			m_TexInfo.ppFileName[nCntTex] = NULL;
		}
	}

	//テクスチャのファイルポインタの破棄
	if (m_TexInfo.ppFileName != NULL)
	{
		delete[] m_TexInfo.ppFileName;
		m_TexInfo.ppFileName = NULL;
	}
}