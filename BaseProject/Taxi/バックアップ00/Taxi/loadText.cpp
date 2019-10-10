//=============================================================================
//
// �e�L�X�g�̓ǂݍ��ݏ��� [LoadText.cpp]
// Author : �������� Sato_Asumi
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
// �}�N����`
//=============================================================================
#define VECTOR_ZERO (D3DXVECTOR3(0.0f, 0.0f, 0.0f))		//�[���x�N�g��

//=============================================================================
// ��������
//=============================================================================
CLoadText * CLoadText::Create(void)
{
	//�C���X�^���X�̐���
	CLoadText * pLoadText = NULL;

	pLoadText->Init();
	return pLoadText;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLoadText::CLoadText()
{
	m_pStrCur = NULL;
	m_aLine[256] = {};
	m_aStrCur[256] = {};
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLoadText::~CLoadText(){}

//=============================================================================
// ��̓ǂݍ���
//=============================================================================
char * CLoadText::ReadLine(FILE * pFile, char * pDest)
{
	//�ϐ��錾
	char * pStrCur = NULL;

	while (1)
	{//�s���΂�
		fgets(pDest, 256, pFile);					//��s�������o��
		pStrCur = pDest;							//�擪�̕������ۊǂ���

		if (false == ((*pStrCur != '\0') && (*pStrCur != '\n') && (*pStrCur != '#'))) { continue; }

		while (1)
		{//��ɗL���ȕ������Ȃ����`�F�b�N����
			if ((*pStrCur != '\n') && (*pStrCur != '\t') && (*pStrCur != ' ') && (*pStrCur != '#') && (*pStrCur != '\0'))
			{
				break;
			}

			pStrCur++;	//�ꕶ�����i�߂�
		}
		break;
	}

	return pStrCur;
}

//=============================================================================
// �擪�|�C���^�̕�����i�߂�
//=============================================================================
char * CLoadText::AddChar(char * pStrCur, char * aStrCur, int nAddChar)
{
	pStrCur += nAddChar;						//�擪�A�h���X�𕶎������i�߂�
	strcpy(aStrCur, pStrCur);					//�������ۊǂ���

	return pStrCur;
}

//=============================================================================
// ������𔲂��o��
//=============================================================================
int CLoadText::PopString(char * pStrCur, char * aStrCur)
{
	//�ϐ��錾
	int nCounterWord = 0;	//�i�񂾕������J�E���g����
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
// �k�������̒ǉ�
//=============================================================================
void CLoadText::AddNull(char * pLine)
{
	//�ϐ��錾
	char * cTop = &pLine[0];
	int nCounter = 0;

	while (1)
	{
		cTop++;
		nCounter++;

		if ((*cTop == '\n') || (*cTop == '\t') || (*cTop == ' ') || (*cTop == '#'))
		{
			pLine[nCounter] = '\0';	//���������� \0( �k������ ) �ɒu��������
			break;
		}
	}
}

//*****************************************************************************
//
// ���[�V�������ǂݍ��݃N���X
//
//*****************************************************************************

//=============================================================================
// ��������
//=============================================================================
CLoadTextMotion * CLoadTextMotion::Create(char * pFileName)
{
	//�C���X�^���X�̐���
	CLoadTextMotion * pLoadTextMotion = new CLoadTextMotion;

	pLoadTextMotion->SetLoadTextFileName(pFileName);	//�ǂݍ��ރe�L�X�g�t�@�C�����̕ۊ�
	pLoadTextMotion->Init();							//����������

	return pLoadTextMotion;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLoadTextMotion::CLoadTextMotion(){}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLoadTextMotion::~CLoadTextMotion(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLoadTextMotion::Init(void)
{
	//�ϐ��̏�����
	m_pStrCur = NULL;			//������̐擪�ւ̃|�C���^
	m_aLine[256] = {};			//������ǂݍ��ݗp�i��s���j
	m_aStrCur[256] = {};		//�����񔲂��o���p
	m_pMotionInfo = NULL;
	m_nMaxMotion = 0;
	m_ModelInfo.paFileName = NULL;
	m_ModelInfo.nMaxModel = 0;
	m_ModelInfo.nMaxParts = 0;
	m_ModelInfo.pParent = NULL;
	m_ModelInfo.pOfSetPos = NULL;
	m_ModelInfo.pOfsetRot = NULL;

	FILE *pFile;

	pFile = fopen(m_pLoadTextFileName, "r");	//�e�L�X�g�̓ǂݍ���

	if (pFile != NULL)
	{
		m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�

		while (memcmp(m_aLine, "END_SCRIPT", 10) != 0)
		{
			m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�

			if (memcmp(m_aLine, "NUM_MODEL = ", 12) == 0)		//�ǂݍ��ރ��f����
			{
				m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 12);
				m_ModelInfo.nMaxModel = atoi(m_pStrCur);

				//�t�@�C�����i�[�p�ϐ��𓮓I�m�ۂ���
				m_ModelInfo.paFileName = new char *[m_ModelInfo.nMaxModel];			//���f�������m�ۂ���

				for (int nCntModel = 0; nCntModel < m_ModelInfo.nMaxModel; nCntModel++)
				{
					m_ModelInfo.paFileName[nCntModel] = new char[256];	//���������m�ۂ���
				}
				for (int nCntModel = 0; nCntModel < m_ModelInfo.nMaxModel; nCntModel++)
				{
					m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�

					if (memcmp(m_aLine, "MODEL_FILENAME = ", 17) != 0) { continue; }

					//X�t�@�C������ǂݍ���
					m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 17);
					strcpy(m_ModelInfo.paFileName[nCntModel], m_pStrCur);
					AddNull(m_ModelInfo.paFileName[nCntModel]);				//�t�@�C�����̍Ō��NULL��ǉ�����
				}
			}
			else if (memcmp(m_aLine, "CHARACTERSET", 12) == 0)	//�L�����N�^�[�̃I�t�Z�b�g�̓ǂݍ���
			{
				while (memcmp(m_aLine, "END_CHARACTERSET", 16) != 0)
				{
					m_pStrCur = ReadLine(pFile, &m_aLine[0]);
					strcpy(m_aStrCur, m_pStrCur);

					if (memcmp(m_aStrCur, "NUM_PARTS = ", 12) == 0)	//�g�p�p�[�c���̓ǂݍ���
					{
						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 12);
						m_ModelInfo.nMaxParts = atoi(m_pStrCur);

						//�e���W�𓮓I�m�ۂ���
						m_ModelInfo.pOfSetPos = new D3DXVECTOR3[m_ModelInfo.nMaxParts];
						m_ModelInfo.pOfsetRot = new D3DXVECTOR3[m_ModelInfo.nMaxParts];
						m_ModelInfo.pParent = new int[m_ModelInfo.nMaxParts];
					}
					else if (memcmp(m_aStrCur, "PARTSSET", 8) == 0)	//�I�t�Z�b�g�̓ǂݍ���
					{
						int nIndex = 0;	//�C���f�b�N�X�i�[�p�ϐ�

						while (memcmp(m_aStrCur, "END_PARTSSET", 12) != 0)
						{
							m_pStrCur = ReadLine(pFile, &m_aLine[0]);
							strcpy(m_aStrCur, m_pStrCur);

							if (memcmp(m_aStrCur, "INDEX = ", 8) == 0)		//�C���f�b�N�X���̓ǂݍ���
							{
								m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 8);
								nIndex = atoi(m_pStrCur);
							}
							else if (memcmp(m_aStrCur, "PARENT = ", 9) == 0)	//�e���f���ԍ��̓ǂݍ���
							{
								m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 9);
								m_ModelInfo.pParent[nIndex] = atoi(m_pStrCur);
							}
							else if (memcmp(m_aStrCur, "POS = ", 6) == 0)		//���W�̓ǂݍ���
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
								m_ModelInfo.pOfsetRot[nIndex].x = (float)atof(m_aStrCur);	//�I�t�Z�b�g���i�[����

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
			else if (memcmp(m_aLine, "MAX_MOTION = ", 13) == 0)	//���[�V�����̍ő吔���擾
			{
				m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 13);
				m_nMaxMotion = atoi(m_pStrCur);

				//���[�V�����f�[�^�̓��I�m��
				if (m_pMotionInfo == NULL) { m_pMotionInfo = new CMotion::MOTION_INFO[m_nMaxMotion]; }

				//���[�V�����f�[�^�̓ǂݍ���
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

						if (memcmp(m_aStrCur, "LOOP = ", 7) == 0)				//���[�v�ǂݍ���
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 7);
							m_pMotionInfo[nCntMotion].m_bLoop = (atoi(m_pStrCur) != 0);
						}
						else if (memcmp(m_aStrCur, "NUM_KEY = ", 10) == 0)	//�L�[�̑����̓ǂݍ���
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 10);
							m_pMotionInfo[nCntMotion].nNumKey = atoi(m_pStrCur);

							//�L�[���𓮓I�m�ۂ���
							m_pMotionInfo[nCntMotion].m_pKeyInfo = new CMotion::KEY_INFO[m_pMotionInfo[nCntMotion].nNumKey];

							for (int nCntKey = 0; nCntKey < m_pMotionInfo[nCntMotion].nNumKey; nCntKey++)
							{
								//�L�[�𓮓I�m�ۂ���
								m_pMotionInfo[nCntMotion].m_pKeyInfo[nCntKey].aKey = new CMotion::KEY[m_ModelInfo.nMaxParts];	//�p�[�c�����m�ۂ���

								while (memcmp(m_aStrCur, "KEYSET", 6) != 0)
								{
									m_pStrCur = ReadLine(pFile, &m_aLine[0]);
									strcpy(m_aStrCur, m_pStrCur);
								}

								while (memcmp(m_aStrCur, "END_KEYSET", 10) != 0)	//�L�[���̓ǂݍ���
								{
									m_pStrCur = ReadLine(pFile, &m_aLine[0]);
									strcpy(m_aStrCur, m_pStrCur);

									if (memcmp(m_aStrCur, "FRAME = ", 8) != 0) { continue; }	//�t���[�����̓ǂݍ���
									m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 8);
									m_pMotionInfo[nCntMotion].m_pKeyInfo[nCntKey].nFrame = atoi(m_pStrCur);

									for (int nCntKeyParts = 0; nCntKeyParts < m_ModelInfo.nMaxParts; nCntKeyParts++)
									{
										while (memcmp(m_aStrCur, "KEY", 3) != 0)
										{
											m_pStrCur = ReadLine(pFile, &m_aLine[0]);
											strcpy(m_aStrCur, m_pStrCur);
										}

										while (memcmp(m_aStrCur, "END_KEY", 7) != 0)	//�e�p�[�c�̈ʒu�A��]�ʂ�ǂݍ���
										{
											m_pStrCur = ReadLine(pFile, &m_aLine[0]);
											strcpy(m_aStrCur, m_pStrCur);

											if (memcmp(m_aStrCur, "POS = ", 6) == 0)	//�ʒu�̓ǂݍ���
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
											else if (memcmp(m_aStrCur, "ROT = ", 6) == 0)	//��]�ʂ̓ǂݍ���
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
		fclose(pFile);	//�ǂݍ��݂��I������
	}
	else
	{
		MessageBox(NULL, TEXT("pFile��NULL�ł�"), TEXT("CLoadTextLoadMotion::Init()"), MB_OK);
	}

	//����������
	m_pStrCur = NULL;
	m_aLine[0] = '\0';
	m_aStrCur[0] = '\0';
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CLoadTextMotion::Uninit(void)
{
	//���f�����̃|�C���^��j������
	if (m_ModelInfo.pOfSetPos != NULL)	//�ʒu
	{
		delete[] m_ModelInfo.pOfSetPos;
		m_ModelInfo.pOfSetPos = NULL;
	}
	if (m_ModelInfo.pOfsetRot != NULL)	//����
	{
		delete[] m_ModelInfo.pOfsetRot;
		m_ModelInfo.pOfsetRot = NULL;
	}
	if (m_ModelInfo.pParent != NULL)	//�e���
	{
		delete[] m_ModelInfo.pParent;
		m_ModelInfo.pParent = NULL;
	}
	for (int nCntModel = 0; nCntModel < m_ModelInfo.nMaxModel; nCntModel++)	//�t�@�C����
	{
		if (m_ModelInfo.paFileName[nCntModel] == NULL) { continue; }

			delete[] m_ModelInfo.paFileName[nCntModel];
			m_ModelInfo.paFileName[nCntModel] = NULL;
	}
	if (m_ModelInfo.paFileName != NULL)	//�t�@�C���|�C���^
	{
		delete[] m_ModelInfo.paFileName;
		m_ModelInfo.paFileName = NULL;
	}

	//�p�[�c���Ƃ̃L�[����j������
	for (int nCntMotion = 0; nCntMotion < m_nMaxMotion; nCntMotion++)
	{
		for (int nCntKey = 0; nCntKey < m_pMotionInfo[nCntMotion].nNumKey; nCntKey++)
		{
			if (m_pMotionInfo[nCntMotion].m_pKeyInfo[nCntKey].aKey == NULL) { continue; }

			delete[] m_pMotionInfo[nCntMotion].m_pKeyInfo[nCntKey].aKey;
			m_pMotionInfo[nCntMotion].m_pKeyInfo[nCntKey].aKey = NULL;
		}
	}

	//�L�[����j������
	for (int nCntParts = 0; nCntParts < m_nMaxMotion; nCntParts++)
	{
		if (m_pMotionInfo[nCntParts].m_pKeyInfo == NULL) { continue; }

		delete[] m_pMotionInfo[nCntParts].m_pKeyInfo;
		m_pMotionInfo[nCntParts].m_pKeyInfo = NULL;
	}

	//���[�V�����f�[�^�̔j��
	if (m_pMotionInfo != NULL)
	{
		delete[] m_pMotionInfo;
		m_pMotionInfo = NULL;
	}
}

//*****************************************************************************
//
// �v���C���[�̏��ǂݍ��݃N���X
//
//*****************************************************************************

//=============================================================================
// ��������
//=============================================================================
CLoadTextPlayer * CLoadTextPlayer::Create(char * pFileName)
{
	//�C���X�^���X�̐���
	CLoadTextPlayer * pLoadTextPlayer = new CLoadTextPlayer;

	pLoadTextPlayer->SetLoadTextFileName(pFileName);	//�ǂݍ��ރe�L�X�g�t�@�C�����̕ۊ�
	pLoadTextPlayer->Init();							//����������

	return pLoadTextPlayer;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLoadTextPlayer::CLoadTextPlayer(){}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLoadTextPlayer::~CLoadTextPlayer(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLoadTextPlayer::Init(void)
{
	//�ϐ��̏�����
	m_pStrCur = NULL;			//������̐擪�ւ̃|�C���^
	m_aLine[256] = {};			//������ǂݍ��ݗp�i��s���j
	m_aStrCur[256] = {};		//�����񔲂��o���p
	//CPlayer::PLAYER_INFO * pPlayerInfo = NULL;
	CManager::MODE mode = CManager::GetMode();

	//�v���C���[���̏�����
	m_PlayerInfo.fAccel = 1.0f;				//�����l
	m_PlayerInfo.fBraks = 0.00f;			//�����l
	m_PlayerInfo.fAddRot = 0.00f;			//�������]�l
	m_PlayerInfo.fDistance = 0.0f;			//����
	m_PlayerInfo.fWindy = 0.0f;				//����
	m_PlayerInfo.FirstPos = VECTOR_ZERO;	//�����ʒu

	//�v���C���[�̏����擾����
	//if (mode == CManager::MODE_GAME)
	//{//�Q�[��
	//	pPlayerInfo = CGame::GetPlayer()->GetPlayerInfoPoint();	//�v���C���[�̏����擾
	//}
	//else if (mode == CManager::MODE_TUTORIAL)
	//{//�`���[�g���A��
	//	pPlayerInfo = CTutorial::GetPlayer()->GetPlayerInfoPoint();	//�v���C���[�̏����擾
	//}

	FILE *pFile;

	pFile = fopen(m_pLoadTextFileName, "r");	//�e�L�X�g�̓ǂݍ���

	if (pFile != NULL)
	{
		m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�

		while (memcmp(m_aLine, "END_SCRIPT", 10) != 0)
		{
			m_pStrCur = ReadLine(pFile, &m_aLine[0]);		//�����������΂�

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
			else if (memcmp(m_aLine, "ACCEL = ", 8) == 0)		//�ǂݍ��ރ��f����
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
		fclose(pFile);	//�ǂݍ��݂��I������
	}
	else
	{
		MessageBox(NULL, TEXT("pFile��NULL�ł�"), TEXT("CLoadTextLoadMotion::Init()"), MB_OK);
	}

	//����������
	m_pStrCur = NULL;
	m_aLine[0] = '\0';
	m_aStrCur[0] = '\0';
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CLoadTextPlayer::Uninit(void){}

//*****************************************************************************
//
// ���q����̏��ǂݍ��݃N���X
//
//*****************************************************************************

//=============================================================================
// ��������
//=============================================================================
void CLoadTextCustomer::Create(char * pFileName)
{
	//�C���X�^���X�̐���
	CLoadTextCustomer * pLoadTextPlayer = new CLoadTextCustomer;

	pLoadTextPlayer->SetLoadTextFileName(pFileName);	//�ǂݍ��ރe�L�X�g�t�@�C�����̕ۊ�

	//����������
	if (S_OK == pLoadTextPlayer->Init())
	{
		pLoadTextPlayer->Uninit();
		delete pLoadTextPlayer;
		pLoadTextPlayer = NULL;
	}
	else
	{
		MessageBox(NULL, TEXT("�e�L�X�g�̓ǂݍ��݂Ɏ��s���܂���"), TEXT("CLoadTextCustomer::Init()"), MB_OK);
	}
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLoadTextCustomer::CLoadTextCustomer() {}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLoadTextCustomer::~CLoadTextCustomer() {}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLoadTextCustomer::Init(void)
{
	//�ϐ��̏�����
	m_pStrCur = NULL;					//������̐擪�ւ̃|�C���^
	m_aLine[256] = {};					//������ǂݍ��ݗp�i��s���j
	m_aStrCur[256] = {};				//�����񔲂��o���p
	D3DXVECTOR3 Pos = VECTOR_ZERO;		//�ʒu
	D3DXVECTOR3 GoalPos = VECTOR_ZERO;	//�ړI�n
	int nTex = -1;						//�ړI�n�e�N�X�`��
	int nAddTime = 0;					//�ǉ�����
	int nLimit = 0;						//��Ԑ�������
	int nType = -1;						//���f���^�C�v�̐ݒ�

	//�e�L�X�g�̓ǂݍ���
	FILE * pFile;
	pFile = fopen(m_pLoadTextFileName, "r");

	if(pFile != NULL)
	{
		m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�

		while (memcmp(m_aLine, "END_SCRIPT", 10) != 0)
		{
			m_pStrCur = ReadLine(pFile, &m_aLine[0]);		//�����������΂�

			if (memcmp(m_aLine, "CUSTOMERSET", 11) == 0)	//�L�����N�^�[�̃I�t�Z�b�g�̓ǂݍ���
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
					else if (memcmp(m_pStrCur, "GOAL = ", 7) == 0)		//�ǂݍ��ރ��f����
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

				//���q����̐���
				CCustomer::Create(Pos, GoalPos, nTex, nAddTime, nLimit, (CHumanBace::MODEL_TYPE)nType);
			}
		}
		fclose(pFile);	//�ǂݍ��݂��I������
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CLoadTextCustomer::Uninit(void) {}

//*****************************************************************************
//
// �l��NPC���ǂݍ��݃N���X
//
//*****************************************************************************

//=============================================================================
// ��������
//=============================================================================
void CLoadTextHumanNPC::Create(char * pFileName)
{
	//�C���X�^���X�̐���
	CLoadTextHumanNPC * pLoadTextBill = new CLoadTextHumanNPC;

	pLoadTextBill->SetLoadTextFileName(pFileName);	//�ǂݍ��ރe�L�X�g�t�@�C�����̕ۊ�

													//����������
	if (S_OK == pLoadTextBill->Init())
	{
		pLoadTextBill->Uninit();
		delete pLoadTextBill;
		pLoadTextBill = NULL;
	}
	else
	{
		MessageBox(NULL, TEXT("�e�L�X�g�̓ǂݍ��݂Ɏ��s���܂���"), TEXT("CLoadTextCustomer::Init()"), MB_OK);
	}
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLoadTextHumanNPC::CLoadTextHumanNPC() {}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLoadTextHumanNPC::~CLoadTextHumanNPC() {}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLoadTextHumanNPC::Init(void)
{
	//�ϐ��̏�����
	m_pStrCur = NULL;					//������̐擪�ւ̃|�C���^
	m_aLine[256] = {};					//������ǂݍ��ݗp�i��s���j
	m_aStrCur[256] = {};				//�����񔲂��o���p
	D3DXVECTOR3 Pos = VECTOR_ZERO;		//�ʒu
	int nType = -1;						//���f���^�C�v�̐ݒ�

										//�e�L�X�g�̓ǂݍ���
	FILE * pFile;
	pFile = fopen(m_pLoadTextFileName, "r");

	if (pFile != NULL)
	{
		m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�

		while (memcmp(m_aLine, "END_SCRIPT", 10) != 0)
		{
			m_pStrCur = ReadLine(pFile, &m_aLine[0]);		//�����������΂�

			if (memcmp(m_aLine, "NPCSET", 5) == 0)	//�L�����N�^�[�̃I�t�Z�b�g�̓ǂݍ���
			{
				while (memcmp(m_aLine, "END_NPCSET", 9) != 0)
				{
					m_pStrCur = ReadLine(pFile, &m_aLine[0]);
					strcpy(m_aStrCur, m_pStrCur);

					if (memcmp(m_pStrCur, "POS = ", 6) == 0)
					{//�ʒu�̎擾
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
					{//�^�C�v�̎擾
						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 7);
						nType = atoi(m_aStrCur);
					}
				}

				//�lNPC�̐���
				CHumanNPC::Create(Pos, (CHumanBace::MODEL_TYPE)nType);
			}
		}
		fclose(pFile);	//�ǂݍ��݂��I������
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CLoadTextHumanNPC::Uninit(void) {}

//*****************************************************************************
//
// �r���{�[�h�ǂݍ��݃N���X
//
//*****************************************************************************

//=============================================================================
// ��������
//=============================================================================
void CLoadTextObjBill::Create(char * pFileName)
{
	//�C���X�^���X�̐���
	CLoadTextObjBill * pLoadTextBill = new CLoadTextObjBill;

	pLoadTextBill->SetLoadTextFileName(pFileName);	//�ǂݍ��ރe�L�X�g�t�@�C�����̕ۊ�

	//����������
	if (S_OK == pLoadTextBill->Init())
	{
		pLoadTextBill->Uninit();
		delete pLoadTextBill;
		pLoadTextBill = NULL;
	}
	else
	{
		MessageBox(NULL, TEXT("�e�L�X�g�̓ǂݍ��݂Ɏ��s���܂���"), TEXT("CLoadTextCustomer::Init()"), MB_OK);
	}
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLoadTextObjBill::CLoadTextObjBill() {}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLoadTextObjBill::~CLoadTextObjBill() {}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLoadTextObjBill::Init(void)
{
	//�ϐ��̏�����
	m_pStrCur = NULL;					//������̐擪�ւ̃|�C���^
	m_aLine[256] = {};					//������ǂݍ��ݗp�i��s���j
	m_aStrCur[256] = {};				//�����񔲂��o���p
	D3DXVECTOR3 Pos = VECTOR_ZERO;		//�ʒu
	int nType = -1;						//���f���^�C�v�̐ݒ�

										//�e�L�X�g�̓ǂݍ���
	FILE * pFile;
	pFile = fopen(m_pLoadTextFileName, "r");

	if (pFile != NULL)
	{
		m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�

		while (memcmp(m_aLine, "END_SCRIPT", 10) != 0)
		{
			m_pStrCur = ReadLine(pFile, &m_aLine[0]);		//�����������΂�

			if (memcmp(m_aLine, "OBJECT_START", 12) == 0)	//�L�����N�^�[�̃I�t�Z�b�g�̓ǂݍ���
			{
				while (memcmp(m_aLine, "OBJECT_END", 10) != 0)
				{
					m_pStrCur = ReadLine(pFile, &m_aLine[0]);
					strcpy(m_aStrCur, m_pStrCur);

					if (memcmp(m_pStrCur, "POS = ", 6) == 0)
					{//�ʒu�̎擾
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
					{//�^�C�v�̎擾
						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 7);
						nType = atoi(m_aStrCur);
					}
				}

				//�I�u�W�F�N�g�r���{�[�h�̐���
				CObjBillboad::Create(Pos, nType);
			}
		}
		fclose(pFile);	//�ǂݍ��݂��I������
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CLoadTextObjBill::Uninit(void) {}

//*****************************************************************************
//
// �G�t�F�N�g�ǂݍ��݃N���X
//
//*****************************************************************************

//=============================================================================
// ��������
//=============================================================================
CLoadTextEffect * CLoadTextEffect::Create(char * pFileName)
{
	//�C���X�^���X�̐���
	CLoadTextEffect * pLoadTextEffect = new CLoadTextEffect;

	pLoadTextEffect->SetLoadTextFileName(pFileName);	//�ǂݍ��ރt�@�C������ۊǂ���
	pLoadTextEffect->Init();

	return pLoadTextEffect;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLoadTextEffect::CLoadTextEffect() {}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLoadTextEffect::~CLoadTextEffect() {}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLoadTextEffect::Init(void)
{
	//�ϐ��錾
	int	 nCountParticle = 0;				//�J�E���^�[
	int	 nCountEffect = 0;					//�G�t�F�N�g�J�E���^�[

											//�ϐ��̏�����
	m_pStrCur = NULL;
	m_aLine[256] = {};
	m_aStrCur[256] = {};
	m_TexInfo.ppFileName = NULL;
	m_nMaxEffect = 0;
	m_pEffect = NULL;

	//�t�@�C���錾
	FILE *pFile;

	pFile = fopen(m_pLoadTextFileName, "r");	//�e�L�X�g�̓ǂݍ���

	if (pFile != NULL)
	{
		m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�

		while (memcmp(m_aLine, "END_SCRIPT", 10) != 0)
		{
			m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�
			strcpy(m_aStrCur, m_pStrCur);

			//----------------------------------
			//�e�N�X�`���̍ő吔�E���̓ǂݍ���
			//----------------------------------
			if (memcmp(m_aStrCur, "MAX_TEX = ", 10) == 0)
			{
				m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 10);
				m_TexInfo.nMaxTex = atoi(m_pStrCur);

				//�e�N�X�`���t�@�C�����𓮓I�m�ۂ���
				if (m_TexInfo.ppFileName == NULL) { m_TexInfo.ppFileName = new char *[m_TexInfo.nMaxTex]; }

				if (m_TexInfo.ppFileName != NULL)
				{
					for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++)
					{//���������������𓮓I�m�ۂ���
						m_TexInfo.ppFileName[nCntTex] = new char[256];
					}
				}

				for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++)
				{
					if (m_TexInfo.ppFileName[nCntTex] == NULL)
					{
						MessageBox(NULL, TEXT("�e�N�X�`���t�@�C���|�C���^��NULL�ł�"), TEXT("CLoadTextEffect::Init()"), MB_OK);
						break;
					}

					m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�
					strcpy(m_aStrCur, m_pStrCur);

					if (memcmp(m_aStrCur, "TEXTURE = ", 10) != 0) { continue; }

					//�e�N�X�`���̃t�@�C�������擾
					m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 10);
					strcpy(m_TexInfo.ppFileName[nCntTex], m_pStrCur);
					AddNull(m_TexInfo.ppFileName[nCntTex]);			//�t�@�C�����̍Ō��NULL��ǉ�����
				}

				m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�
				strcpy(m_aStrCur, m_pStrCur);
			}

			//----------------------------------
			//�G�t�F�N�g�̍ő吔�E���̏�����
			//----------------------------------
			if (memcmp(m_aStrCur, "MAX_EFFECT = ", 13) == 0)
			{
				m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 13);
				m_nMaxEffect = atoi(m_pStrCur);

				//�������̓��I�m��
				if (m_pEffect == NULL) { m_pEffect = new CEffectTool::Effect[m_nMaxEffect]; }	//�G�t�F�N�g�̌������������m�ۂ���

																							//�ϐ��̏�����
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
			//�G�t�F�N�g�̐ݒ�
			//-------------------------------------------
			if (memcmp(m_aStrCur, "SETEFFECT", 9) == 0)
			{
				while (memcmp(m_aLine, "END_EFFECT", 10) != 0)
				{
					m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�
					strcpy(m_aStrCur, m_pStrCur);

					if (memcmp(m_aStrCur, "SETPARTICLE", 11) != 0) { continue; }
					while (memcmp(m_aStrCur, "END_PARTICLE", 12) != 0)
					{
						if (memcmp(m_aStrCur, "LOOP = ", 7) == 0)
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 7);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].bLoop = (atoi(m_pStrCur) != 0);	//���[�v��Ԃ̐ݒ�

							m_pEffect[nCountEffect].EffectInfo[nCountParticle].bCreateEffect = true;			//������Ԃ̐ݒ�
						}
						else if (memcmp(m_aStrCur, "SIZE = ", 7) == 0)						//�T�C�Y�̐ݒ�
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
						else if (memcmp(m_aStrCur, "TEX = ", 6) == 0)						//�e�N�X�`���ԍ��̐ݒ�
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 6);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].nTexNum = atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "DRAWTYPE = ", 11) == 0)					//�`��^�C�v�̐ݒ�
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 11);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].DrawType = (CParticleBillboad::DRAWTYPE)atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "TYPE = ", 7) == 0)						//�^�C�v�̐ݒ�
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 7);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].Type = (CParticleBillboad::TYPE)atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "COLOR = ", 8) == 0)						//�F�̐ݒ�
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
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].bZbuffur = (atoi(m_pStrCur) != 0);	//Z�o�b�t�@�̐ݒ�
						}
						else if (memcmp(m_aStrCur, "LIFE = ", 7) == 0)								//�\�����Ԃ̐ݒ�
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 7);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].nLife = atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "CHANGECOLOR = ", 14) == 0)						//�����x�̕ω���
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 14);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].fChangeCol_a = (float)atof(m_aStrCur);
						}
						else if (memcmp(m_aStrCur, "USEGRAVITY = ", 13) == 0)
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 13);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].bUseGravity = (atoi(m_pStrCur) != 0);	//�d�͂̏�Ԑݒ�
						}
						else if (memcmp(m_aStrCur, "TIME = ", 7) == 0)									//�������Ԃ̐ݒ�
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 7);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].nInterbalTime = atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "NUMCREATE = ", 12) == 0)								//�����ʂ̐ݒ�
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 12);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].nNumCreate = atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "SPEED = ", 8) == 0)									//�X�s�[�h�̐ݒ�
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 8);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].fSpeed = (float)atof(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "MINHIGHT = ", 11) == 0)								//�����̍ŏ��l�̐ݒ�
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 11);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].nMinHight = atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "MAXHIGHT = ", 11) == 0)								//�����̍ő�l�̐ݒu
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 11);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].nMaxHight = atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "GRAVITY = ", 10) == 0)								//�d�͂̐ݒ�
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 10);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].fGravity = (float)atof(m_pStrCur);
						}

						m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�
						strcpy(m_aStrCur, m_pStrCur);
					}

					//�J�E���^�[��i�߂�
					nCountParticle++;
				}

				nCountEffect++;			//�G�t�F�N�g�J�E���^�[��i�߂�
				nCountParticle = 0;		//�p�[�e�B�N���J�E���^�[�̏�����
			}
		}
		fclose(pFile);	//�t�@�C���ǂݍ��݂��I���
	}
	else
	{
		MessageBox(NULL, TEXT("pFile��NULL�ł�"), TEXT("CLoadTextObject::Init()"), MB_OK);
	}

	//����������
	m_pStrCur = NULL;
	m_aLine[0] = '\0';
	m_aStrCur[0] = '\0';
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CLoadTextEffect::Uninit(void)
{
	//�G�t�F�N�g�̏��j��
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

	//�e�N�X�`�����̔j��
	for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++)
	{
		if (m_TexInfo.ppFileName[nCntTex] != NULL)
		{
			delete[] m_TexInfo.ppFileName[nCntTex];
			m_TexInfo.ppFileName[nCntTex] = NULL;
		}
	}

	//�e�N�X�`���̃t�@�C���|�C���^�̔j��
	if (m_TexInfo.ppFileName != NULL)
	{
		delete[] m_TexInfo.ppFileName;
		m_TexInfo.ppFileName = NULL;
	}
}