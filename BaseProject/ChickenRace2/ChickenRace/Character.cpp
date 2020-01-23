//=============================================================================
//
// �L�����N�^�[���� [Character.cpp]
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
// �}�N����`
//*****************************************************************************
#define FILE_NAME_WOOD	"data\\TEXT\\Player\\WindWood.txt"// �ǂݍ���txt�t�@�C���̖��O
#define FILE_TEXTURE		("data\\MODEL\\WindWood\\tree000.jpg")	//�e�N�X�`���̓ǂݍ���

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CPlayer::KEY_INFO		*CCharcter::m_pKeyInfo[MAX_MOTION];		// �L�[���ւ̃|�C���^
int						CCharcter::m_nNumParts;					// �p�[�c��
int						CCharcter::m_aIndexParent[MAX_PARTS];	// �e�̃C���f�b�N�X
CPlayer::KEY			CCharcter::m_aKayOffset[MAX_PARTS];		// �I�t�Z�b�g���
CPlayer::MOTION_INFO	CCharcter::m_aMotionInfo[MAX_MOTION];	// ���[�V�������
LPDIRECT3DTEXTURE9		CCharcter::m_pTexture = NULL;
int						CCharcter::m_nNumModel;					//���f����
char					CCharcter::m_aFileNameModel[MAX_PARTS][256];

//=============================================================================
// �t�@�C���̓ǂݍ���
//=============================================================================
void CCharcter::Load(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice,
		FILE_TEXTURE,
		&m_pTexture);

	//���f���̃I�t�Z�b�g�Ɠǂݍ���
	FileLoad();
	CModel::SetParts();	//�j���g���e�q�̓ǂݍ���
}

//=============================================================================
// �t�@�C���̔j��
//=============================================================================
void CCharcter::Unload(void)
{
	CModel::PartsTypeUnLoad();

	//�e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//==================================================================================================//
//    * �����֐� *
//==================================================================================================//
CCharcter	*CCharcter::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fScale)
{
	CCharcter *pChar = NULL;
	pChar = new CCharcter;
	if (pChar != NULL)
	{
		pChar->Init();
		pChar->Setting(pos, rot, fScale);
	}

	return pChar;
}
//==================================================================================================//
//    * �ݒ�֐� *
//==================================================================================================//
void	CCharcter::Setting(D3DXVECTOR3 &pos, D3DXVECTOR3 &rot, float &fScale)
{
	CModel::ParentModel(m_apModel, CModel::TYPE_WOOD);
	int &nMaxModel = CModel::GetnModelMax(CModel::TYPE_WOOD);
	for (int nCountIndex = 0; nCountIndex < nMaxModel; nCountIndex++)
	{
		m_apModel[nCountIndex]->BindTexture(m_pTexture);
		if (m_aIndexParent[nCountIndex] == -1)
		{
			//���f���̐e���w��
			m_apModel[nCountIndex]->SetParent(NULL);
		}
		else
		{
			//���f���̐e���w��
			m_apModel[nCountIndex]->SetParent(m_apModel[m_aIndexParent[nCountIndex]]);
		}

		m_apModel[nCountIndex]->SetPos(D3DXVECTOR3(m_pos.x + m_aKayOffset[nCountIndex].fposX,
			m_pos.y + m_aKayOffset[nCountIndex].fposY,
			m_pos.z + m_aKayOffset[nCountIndex].fposZ));
	}

	m_pos = pos + D3DXVECTOR3(0.0f, -15.0f, 0.0f);
	m_rot = rot;
	ResetCheck(m_pos, &m_fCola[0]);

	m_fScale = 0.375f * fScale;
}
//==================================================================================================//
//    * �������֐� *
//==================================================================================================//
HRESULT	CCharcter::Init(void)
{
	//�ϐ��̏�����
	m_pos = INIT_VECTOR;					//�ʒu
	m_rot = INIT_VECTOR;					//����
	m_nCountFlame = 0;
	m_nKey = 0;
	m_nMotionType = 0;
	m_apModel = NULL;
	m_fCntState = 0.0f;
	m_p3D = NULL;
	m_bDraw = false;
	m_fScale = 0.375f;

	for (int nCount = 0; nCount < MAX_PLAYCOL; nCount++) { m_fCola[nCount] = 0.0f; }
	return S_OK;
}
//==================================================================================================//
//    * �I���֐� *
//==================================================================================================//
void	CCharcter::Uninit(void)
{
	if (m_p3D != NULL) { m_p3D->Uninit(); m_p3D = NULL; }
	int &nMaxModel = CModel::GetnModelMax(CModel::TYPE_WOOD);
	for (int nCount = 0; nCount < nMaxModel; nCount++)
	{
		if (m_apModel[nCount] != NULL)
		{
			//3D���f����Uninit
			m_apModel[nCount]->Uninit();
			delete m_apModel[nCount];
			m_apModel[nCount] = NULL;
		}
	}
	delete[] m_apModel;
	m_apModel = NULL;

	//���S�t���O�𗧂Ă�
	Release();
}
//==================================================================================================//
//    * �X�V�֐� *
//==================================================================================================//
void	CCharcter::Update(void)
{
	if (m_bDraw) { UpdateMotion(); }
	m_bDraw = false;
}
//==================================================================================================//
//    * �`��֐� *
//==================================================================================================//
void	CCharcter::Draw(void)
{
	float fCola;
	if (!DrawCheck(m_pos, &m_fCola[0], fCola)) { return; }
	m_bDraw = true;

	//�����_�����O�N���X���擾
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX		  mtxRot, mtxTrans, mtx;			// �v�Z�p�}�g���b�N�X

														// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtx);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y + D3DX_PI, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxTrans);

	//�g�又��
	mtx._22 = 1.5f;
	mtx._44 = (1.0f / m_fScale);
	mtx._41 *= mtx._44;
	mtx._42 *= mtx._44;
	mtx._43 *= mtx._44;

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	int &nMaxModel = CModel::GetnModelMax(CModel::TYPE_WOOD);
	for (int nCount = 0; nCount < nMaxModel; nCount++)
	{//���f���̕`��
		if (m_apModel[nCount] != NULL)
		{
			//�`�悷��
			m_apModel[nCount]->Draw(fCola);
		}
	}

}
//=============================================================================
// �`�攻��
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
// �`�攻��̏�����
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
// �S�Ă̓����蔻��
//=============================================================================
void	CCharcter::CollisionAll(CPlayer *pPlayer)
{
	CScene *pScene, *pNext;

	pScene = CScene::GetTop(CHAR_PRIORITY);
	while (pScene != NULL)
	{
		pNext = pScene->GetNext();
		if (pScene->GetObjType() == OBJTYPE_WINDWOOD)
		{//�؂Ȃ�
			((CCharcter*)pScene)->CollisionWood(pPlayer);
		}
		pScene = pNext;
	}
}
//=============================================================================
// �����蔻��
//=============================================================================
bool	CCharcter::Collision(CPlayer *&pPlayer, D3DXVECTOR3 &Mypos)
{
	D3DXVECTOR3 &pos = pPlayer->GetPos();
	float fLenght = sqrtf(powf(pos.x - Mypos.x, 2.0f) + powf(pos.z - Mypos.z, 2.0f));
	float fRange = PLAYER_LENGTH + WOOD_LEN * (m_fScale / 0.375f);
	if (fLenght < fRange)
	{// �͈͓�
	 // �p�x�v�Z
		float fAngle = atan2f(pos.x - Mypos.x, pos.z - Mypos.z);

		// �ʒu����
		pos = D3DXVECTOR3(Mypos.x + sinf(fAngle) * fRange, pos.y, Mypos.z + cosf(fAngle) * fRange);

		//�e��
		pPlayer->Strike(NULL, Mypos, INIT_VECTOR);

		return true;
	}
	return false;
}
//=============================================================================
// �؂̓����蔻��
//=============================================================================
void	CCharcter::CollisionWood(CPlayer *&pPlayer)
{
	D3DXVECTOR3 pos = pPlayer->Getpos();
	float fDis = (WOOD_DIS - 5.0f) * (m_fScale / 0.375f);
	float fValue = pPlayer->GetfInduction();
	float fRot;

	float fLenght = sqrtf(powf(pos.x - m_pos.x, 2.0f) + powf(pos.z - m_pos.z, 2.0f));
	if (fLenght < 500.0f)
	{//���ȏ�߂��Ȃ�
		D3DXVECTOR3 WKpos;
		for (int nCount = 0; nCount < 2; nCount++)
		{
			fRot = m_rot.y - D3DX_PI * 0.5f + D3DX_PI * (float)nCount;
			WKpos = m_pos + D3DXVECTOR3(sinf(fRot), 0.0f, cosf(fRot)) * fDis;
			if (Collision(pPlayer, WKpos)) { return; }
		}
		if (pPlayer->GetInduction() < CPlayer::INDUCTION_WIND) { return; }
		float fRot = atan2f(m_pos.x - pos.x, m_pos.z - pos.z);
		float &fRotY = pPlayer->Getrot().y;
		float fWK;
		fWK = fRot - fRotY;
		if (fWK < 0.0f) { fWK *= -1.0f; }

		if (fWK < D3DX_PI * 0.07f)
		{//�p�x�`�F�b�N
			if (fValue > fWK)
			{//�a�U��
				fValue = fWK;
				pPlayer->GetfFeedRot() = fRot;
				pPlayer->GetInduction() = CPlayer::INDUCTION_WIND;
			}
		}
	}
}
//=============================================================================
// ���[�V����
//=============================================================================
void CCharcter::UpdateMotion(void)
{
	//���[�V����
	CPlayer::KEY *pKey, *pNextKey;
	float fRateMotion;
	float fDiffMotion;
	D3DXVECTOR3 rotmotion;
	D3DXVECTOR3 posmotion;

	//�L�[���ő吔������Ȃ��悤��
	if (m_aMotionInfo[m_nMotionType].nNumKey <= m_nKey)
	{
		m_nKey = 0;
	}

	//���[�V�����X�V
	for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
	{
		if (m_apModel[nCntParts] != NULL)
		{
			//���݂̃L�[���擾
			pKey = &m_pKeyInfo[m_nMotionType][m_nKey].aKey[nCntParts];
			//���̃L�[���擾
			pNextKey = &m_pKeyInfo[m_nMotionType][(m_nKey + 1) % m_aMotionInfo[m_nMotionType].nNumKey].aKey[nCntParts];

			//���݂̃L�[���玟�̃L�[�ւ̍Đ��t���[�����ɂ����郂�[�V�����J�E���^�[�̑��Βl���Z�o
			fRateMotion = (float)m_nCountFlame / (float)m_pKeyInfo[m_nMotionType][m_nKey].nFrame;

			//ROT
			//���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍������Z�o
			fDiffMotion = pNextKey->frotX - pKey->frotX;
			//���Βl���������g���Ċe�v�f�̒l���Z�o
			rotmotion.x = pKey->frotX + (fDiffMotion * fRateMotion);

			//POS
			//���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍������Z�o
			fDiffMotion = pNextKey->fposX - pKey->fposX;
			//���Βl���������g���Ċe�v�f�̒l���Z�o
			posmotion.x = pKey->fposX + (fDiffMotion * fRateMotion);


			//���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍������Z�o
			fDiffMotion = pNextKey->frotY - pKey->frotY;
			//���Βl���������g���Ċe�v�f�̒l���Z�o
			rotmotion.y = pKey->frotY + (fDiffMotion * fRateMotion);
			//POS
			//���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍������Z�o
			fDiffMotion = pNextKey->fposY - pKey->fposY;
			//���Βl���������g���Ċe�v�f�̒l���Z�o
			posmotion.y = pKey->fposY + (fDiffMotion * fRateMotion);


			//���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍������Z�o
			fDiffMotion = pNextKey->frotZ - pKey->frotZ;
			//���Βl���������g���Ċe�v�f�̒l���Z�o
			rotmotion.z = pKey->frotZ + (fDiffMotion * fRateMotion);
			//POS
			//���݂̃L�[�Ǝ��̃L�[�̊e�v�f�̍������Z�o
			fDiffMotion = pNextKey->fposZ - pKey->fposZ;
			//���Βl���������g���Ċe�v�f�̒l���Z�o
			posmotion.z = pKey->fposZ + (fDiffMotion * fRateMotion);


			//�p�[�c�𓮂���
			m_apModel[nCntParts]->SetRot(rotmotion);

			//POS
			m_apModel[nCntParts]->SetPos(D3DXVECTOR3(m_aKayOffset[nCntParts].fposX + posmotion.x,
				m_aKayOffset[nCntParts].fposY + posmotion.y,
				m_aKayOffset[nCntParts].fposZ + posmotion.z));
		}
	}

	//���[�v�̔���
	switch (m_aMotionInfo[m_nMotionType].bLoop)
	{
	case true:
		//���[�v����
		//�t���[����i�߂�
		m_nCountFlame++;
		//�L�[�̍X�V
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
		//���[�v���Ȃ�
		if (m_aMotionInfo[m_nMotionType].nNumKey - 1 > m_nKey)
		{//�t���[����i�߂�
			m_nCountFlame++;
		}
		else if (m_aMotionInfo[m_nMotionType].nNumKey - 1 == m_nKey)
		{
			//if (m_nAnimnow == PLAYERANIM_ATTACK)
			//{//�U�����[�V����
			//	m_nAttackDelay++;
			//	if (m_nAttackDelay > 20)
			//	{
			//		m_bAttack = false;
			//		m_nAttackDelay = 0;
			//	}
			//}
			m_bMotionEnd = true;
		}
		//�L�[�̍X�V
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
	/*CDebugProc::Print(" Num�L�[  : (%d)\n", m_nKey);
	CDebugProc::Print(" m_nCountFlame  : (%d)\n", m_nCountFlame);*/

#endif

}

//=============================================================================
// �t�@�C���ǂݍ���
//=============================================================================
void CCharcter::FileLoad(void)
{
	//�f�o�C�X���擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//�t�@�C���ǂݍ��ݗp�ϐ�
	FILE *pFile;		//�t�@�C���|�C���^
	char *pStrcur;		//���݂̐擪�̕�����
	char aLine[256];	//������
	char aStr[256];		//�ꎞ�ۑ�������
	int nIndex = 0;		//���݂̃C���f�b�N�X
	int nWord = 0;		//�|�b�v�ŕԂ��ꂽ�l��ێ�

#if 1
						//�t�@�C�����J�� �ǂݍ���
	pFile = fopen(FILE_NAME_WOOD, "r");
	//NULL�`�F�b�N
	if (pFile != NULL)
	{
		for (int nCntMotion = 0; nCntMotion < MAX_MOTION; )
		{
			//������̐擪��ݒ�
			pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);
			//����������o��
			strcpy(aStr, pStrcur);

			//������̃f�[�^ ��r���镶���� ��r���镶����
			if (memcmp(pStrcur, "NUM_MODEL = ", strlen("NUM_MODEL = ")) == 0)
			{
				//���o��
				pStrcur += strlen("NUM_MODEL = ");
				//������̐擪��ݒ�
				strcpy(aStr, pStrcur);
				//�����񔲂��o��
				m_nNumModel = atoi(pStrcur);

				for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
				{
					int nNumber = nCntModel + CModel::PARTS_WOOD_LEG;
					LPD3DXBUFFER &m_pBuffMat = CModel::GetpBuffMat(nNumber);
					DWORD &m_nNumMat = CModel::GetnNumMat(nNumber);
					LPD3DXMESH &m_pMesh = CModel::GetpMesh(nNumber);

					//������̐擪��ݒ�
					pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);
					//����������߂�
					strcpy(aStr, pStrcur);
					if (memcmp(pStrcur, "MODEL_FILENAME = ", strlen("MODEL_FILENAME = ")) == 0)
					{
						//���o��
						pStrcur += strlen("MODEL_FILENAME = ");

						//�������ݒ�
						strcpy(aStr, pStrcur);

						//�K�v�ȕ�����̍Ō�̕����܂ł̕������𐔂���
						int nNullNum = CPlayer::PopString(pStrcur, &aStr[0]);

						//����������߂�
						strcpy(aStr, pStrcur);

						//�Ō�̕�����NULL����������
						aStr[nNullNum - 1] = '\0';

						//�Ώۂ̕����񂩂甲���o��
						strcpy(&m_aFileNameModel[nCntModel][0], aStr);

						//X�t�@�C���̓ǂݍ���
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
				//������̐擪��ݒ�
				pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);
				//����������o��
				strcpy(aStr, pStrcur);
			}


			//������̃f�[�^ ��r���镶���� ��r���镶����
			if (memcmp(pStrcur, "CHARACTERSET", 12) == 0)
			{
				while (1)
				{
					//������̐擪��ݒ�
					pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);
					//����������o��
					strcpy(aStr, pStrcur);
					if (memcmp(pStrcur, "NUM_PARTS = ", strlen("NUM_PARTS = ")) == 0)
					{
						//���o��
						pStrcur += strlen("NUM_PARTS = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_nNumParts = atoi(pStrcur);
					}
					if (memcmp(pStrcur, "PARTSSET", strlen("PARTSSET")) == 0)
					{
						while (1)
						{
							//������̐擪��ݒ�
							pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);

							//INDEX��ǂݍ���
							if (memcmp(pStrcur, "INDEX = ", strlen("INDEX = ")) == 0)
							{
								//���o��
								pStrcur += strlen("INDEX = ");
								//������̐擪��ݒ�
								strcpy(aStr, pStrcur);
								//�����񔲂��o��
								nIndex = atoi(pStrcur);
							}
							//PARENT��ǂݍ���
							if (memcmp(pStrcur, "PARENT = ", strlen("PARENT = ")) == 0)
							{
								//���o��
								pStrcur += strlen("PARENT = ");
								//������̐擪��ݒ�
								strcpy(aStr, pStrcur);
								//�����񔲂��o��
								m_aIndexParent[nIndex] = atoi(pStrcur);
							}
							//POS��ǂݍ���
							if (memcmp(pStrcur, "POS = ", strlen("POS = ")) == 0)
							{
								//���o��
								pStrcur += strlen("POS = ");
								//������̐擪��ݒ�
								strcpy(aStr, pStrcur);

								//��������Ԃ��Ă��炤
								nWord = CPlayer::PopString(pStrcur, &aStr[0]);
								//������ϊ�
								m_aKayOffset[nIndex].fposX = (float)atof(pStrcur);
								//���������i�߂�
								pStrcur += nWord;

								//��������Ԃ��Ă��炤
								nWord = CPlayer::PopString(pStrcur, &aStr[0]);
								//������ϊ�
								m_aKayOffset[nIndex].fposY = (float)atof(pStrcur);
								//���������i�߂�
								pStrcur += nWord;

								//��������Ԃ��Ă��炤
								nWord = CPlayer::PopString(pStrcur, &aStr[0]);
								//������ϊ�
								m_aKayOffset[nIndex].fposZ = (float)atof(pStrcur);

							}
							//ROT��ǂݍ���
							if (memcmp(pStrcur, "ROT = ", strlen("ROT = ")) == 0)
							{
								//���o��
								pStrcur += strlen("ROT = ");
								//������̐擪��ݒ�
								strcpy(aStr, pStrcur);

								//��������Ԃ��Ă��炤
								nWord = CPlayer::PopString(pStrcur, &aStr[0]);
								//������ϊ�
								m_aKayOffset[nIndex].frotX = (float)atof(pStrcur);

								//���������i�߂�
								pStrcur += nWord;
								//��������Ԃ��Ă��炤
								nWord = CPlayer::PopString(pStrcur, &aStr[0]);
								//������ϊ�
								m_aKayOffset[nIndex].frotY = (float)atof(pStrcur);

								//���������i�߂�
								pStrcur += nWord;
								//��������Ԃ��Ă��炤
								nWord = CPlayer::PopString(pStrcur, &aStr[0]);
								//������ϊ�
								m_aKayOffset[nIndex].frotZ = (float)atof(pStrcur);
							}
							//�p�[�c�Z�b�g�I��
							else if (memcmp(pStrcur, "END_PARTSSET", strlen("END_PARTSSET")) == 0)
							{
								break;
							}
						}
					}
					//�L�����N�^�[�Z�b�g�I��
					else if (memcmp(pStrcur, "END_CHARACTERSET", strlen("END_CHARACTERSET")) == 0)
					{
						break;
					}
				}
				//������̐擪��ݒ�
				pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);
				//����������o��
				strcpy(aStr, pStrcur);
			}

			//���[�V�����ǂݍ���
			if (memcmp(pStrcur, "MOTIONSET", strlen("MOTIONSET")) == 0)
			{
				//���o��
				pStrcur += strlen("MOTIONSET");

				while (1)
				{
					//������̐擪��ݒ�
					pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);
					//����������o��
					strcpy(aStr, pStrcur);

					if (memcmp(pStrcur, "LOOP = ", strlen("LOOP = ")) == 0)
					{
						//���o��
						pStrcur += strlen("LOOP = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);

						switch (atoi(pStrcur))
						{
						case 0:
							//�����񔲂��o��
							m_aMotionInfo[nCntMotion].bLoop = false;
							break;
						case 1:
							//�����񔲂��o��
							m_aMotionInfo[nCntMotion].bLoop = true;
							break;
						}
						//������̐擪��ݒ�
						pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);
					}
					if (memcmp(pStrcur, "NUM_KEY = ", strlen("NUM_KEY = ")) == 0)
					{
						//���o��
						pStrcur += strlen("NUM_KEY = ");
						//������̐擪��ݒ�
						strcpy(aStr, pStrcur);
						//�����񔲂��o��
						m_aMotionInfo[nCntMotion].nNumKey = atoi(pStrcur);

						//������̐擪��ݒ�
						pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);
						//����������o��
						strcpy(aStr, pStrcur);
					}

					//�L�[�̐ݒ�
					for (int nCntKey = 0; nCntKey < m_aMotionInfo[nCntMotion].nNumKey;)
					{
						if (memcmp(pStrcur, "KEYSET", strlen("KEYSET")) == 0)
						{
							//���o��
							pStrcur += strlen("KEYSET");
							//������̐擪��ݒ�
							strcpy(aStr, pStrcur);
							//������̐擪��ݒ�
							pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);

							if (memcmp(pStrcur, "FRAME = ", strlen("FRAME = ")) == 0)
							{
								//���o��
								pStrcur += strlen("FRAME = ");

								m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].nFrame = atoi(pStrcur);

								//������̐擪��ݒ�
								strcpy(aStr, pStrcur);
								//������̐擪��ݒ�
								pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);
							}

							//�p�[�c����
							for (int nCntParts = 0; nCntParts < m_nNumParts;)
							{
								if (memcmp(pStrcur, "KEY", strlen("KEY")) == 0)
								{
									//������̐擪��ݒ�
									pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);

									if (memcmp(pStrcur, "POS = ", strlen("POS = ")) == 0)
									{
										//���o��
										pStrcur += strlen("POS = ");
										//������̐擪��ݒ�
										strcpy(aStr, pStrcur);

										//��������Ԃ��Ă��炤
										nWord = CPlayer::PopString(pStrcur, &aStr[0]);
										//POS.X���
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].fposX = (float)atof(pStrcur);
										//���������i�߂�
										pStrcur += nWord;

										//��������Ԃ��Ă��炤
										nWord = CPlayer::PopString(pStrcur, &aStr[0]);
										//POS.Y���
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].fposY = (float)atof(pStrcur);
										//���������i�߂�
										pStrcur += nWord;

										//��������Ԃ��Ă��炤
										nWord = CPlayer::PopString(pStrcur, &aStr[0]);
										//POS.Z���
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].fposZ = (float)atof(pStrcur);
										//������̐擪��ݒ�
										pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);
									}
									//ROT��ǂݍ���
									if (memcmp(pStrcur, "ROT = ", strlen("ROT = ")) == 0)
									{
										//���o��
										pStrcur += strlen("ROT = ");
										//������̐擪��ݒ�
										strcpy(aStr, pStrcur);

										//��������Ԃ��Ă��炤
										nWord = CPlayer::PopString(pStrcur, &aStr[0]);
										//RotX
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].frotX = (float)atof(pStrcur);
										//���������i�߂�
										pStrcur += nWord;

										//��������Ԃ��Ă��炤
										nWord = CPlayer::PopString(pStrcur, &aStr[0]);
										//RotY
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].frotY = (float)atof(pStrcur);
										//���������i�߂�
										pStrcur += nWord;

										//��������Ԃ��Ă��炤
										nWord = CPlayer::PopString(pStrcur, &aStr[0]);
										//RotZ
										m_aMotionInfo[nCntMotion].aKayInfo[nCntKey].aKey[nCntParts].frotZ = (float)atof(pStrcur);

										//������̐擪��ݒ�
										pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);
									}
									if (memcmp(pStrcur, "END_KEY", strlen("END_KEY")) == 0)
									{
										//���o��
										pStrcur += strlen("END_KEY");
										//������̐擪��ݒ�
										strcpy(aStr, pStrcur);
										//������̐擪��ݒ�
										pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);
										//�p�[�c�̃J�E���g��i�߂�
										nCntParts++;
									}
								}
								else
								{
									//������̐擪��ݒ�
									pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);
								}
							}
							if (memcmp(pStrcur, "END_KEYSET", strlen("END_KEYSET")) == 0)
							{
								//������̐擪��ݒ�
								pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);
								//�J�E���g��i�߂�
								nCntKey++;
							}
						}
						else
						{
							//������̐擪��ݒ�
							pStrcur = CPlayer::ReadLine(pFile, &aLine[0]);
						}

					}
					if (memcmp(pStrcur, "END_MOTIONSET", strlen("END_MOTIONSET")) == 0)
					{
						//���[�V�����̏����Z�b�g
						m_pKeyInfo[nCntMotion] = &m_aMotionInfo[nCntMotion].aKayInfo[0];
						nCntMotion++;
						break;
					}
				}
			}
			//�X�N���v�g�̏I���
			if (memcmp(pStrcur, "END_SCRIPT	", strlen("END_SCRIPT")) == 0)
			{
				break;
			}
		}
	}
	else
	{	//�t�@�C�����J���Ȃ�����
		printf("�t�@�C�����J���܂���ł���\n");
	}
	//�t�@�C�������
	fclose(pFile);
#endif

}
