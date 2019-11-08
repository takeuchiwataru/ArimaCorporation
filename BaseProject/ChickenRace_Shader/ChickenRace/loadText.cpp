//=============================================================================
//
// �e�L�X�g�̓ǂݍ��ݏ��� [LoadText.cpp]
// Author : �������� Sato_Asumi
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "loadText.h"
#include "game.h"
#include "manager.h"
#include "tutorial.h"

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
CLoadText::~CLoadText() {}

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
CLoadTextMotion::CLoadTextMotion() {}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLoadTextMotion::~CLoadTextMotion() {}

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
CLoadTextPlayer::CLoadTextPlayer() {}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLoadTextPlayer::~CLoadTextPlayer() {}

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
	m_PlayerInfo.fAccel = 1.0f;				//�����l�i�O�i�j
	m_PlayerInfo.fBraks = 0.00f;			//�����l�i��i�j
	m_PlayerInfo.fDown = 0.0f;				//�����l
	m_PlayerInfo.fAddRot = 0.00f;			//�������]�l
	m_PlayerInfo.fDistance = 0.0f;			//����
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

	pFile = fopen("data/TEXT/Player/PlayerState.txt", "r");	//�e�L�X�g�̓ǂݍ���

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
			else if (memcmp(m_aLine, "DOWN = ", 7) == 0)
			{
				m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 8);
				m_PlayerInfo.fDown = (float)atof(m_aStrCur);
			}
			else if (memcmp(m_aLine, "ADDROT = ", 9) == 0)
			{
				m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 9);
				m_PlayerInfo.fAddRot = (float)atof(m_aStrCur);
			}
		}
		fclose(pFile);	//�ǂݍ��݂��I������
	}
	else
	{
		MessageBox(NULL, TEXT("pFile��NULL�ł�"), TEXT("CLoadTextPlayer::Init()"), MB_OK);
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
void CLoadTextPlayer::Uninit(void) {}