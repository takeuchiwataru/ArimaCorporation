//=============================================================================
//
// テキストの読み込み処理 [LoadText.cpp]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "loadText.h"
#include "game.h"
#include "manager.h"
#include "tutorial.h"

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