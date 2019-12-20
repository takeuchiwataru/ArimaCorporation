//=============================================================================
//
// ���̃|�C���g���� [RoadPointer.cpp]
// Author : Ryo Sugimoto
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS	//Fire Wall�˔j
#include <stdio.h>				//�C���N���h�t�@�C��
#include "main.h"
#include "manager.h"
#include "player.h"
#include "RoadPointer.h"
#include "ColMesh.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define IGNOR_DIS	(200.0f)
#define BENT_DIS	(650.0f)
#define BENTX_DIS	(500.0f)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CRoad_Manager	*CRoad_Manager::m_pRoadManager = NULL;

//==================================================================================================//
//    * ����t�����b�V���̐����֐� *
//==================================================================================================//
bool	CRoad_Pointer::Create(CRoad_Pointer *&pRoadPointer)
{
	pRoadPointer = NULL;
	pRoadPointer = new CRoad_Pointer;
	if (pRoadPointer != NULL) { pRoadPointer->Init(); return true; }

	return false;
}
//==================================================================================================//
//    * ���̃|�C���^�������֐� *
//==================================================================================================//
HRESULT CRoad_Pointer::Init(void)
{
	CRoad_Manager *pManager = CRoad_Manager::GetManager();
	m_nNumber = pManager->GetnNumAll();
	m_nNumRoad = m_nNumber;
	pManager->GetnNumAll()++;

	if (m_nNumber == 0 && !pManager->GetbWKData()) { pManager->SetTop(this); }
	pManager->SetCur(this);
	m_pos = INIT_VECTOR;
	for (int nCount = 0; nCount < POINT_MAX; nCount++)
	{
		m_Point[nCount] = INIT_VECTOR;
		m_Trails[nCount] = TRAILS_NONE;
	}
	m_pNextPointer[0] = NULL;
	m_pNextPointer[1] = NULL;
	m_pRoadPointer[0] = NULL;
	m_pRoadPointer[1] = NULL;
	return S_OK;
}
//==================================================================================================//
//    * ���̃|�C���^�I���֐� *
//==================================================================================================//
void	CRoad_Pointer::Uninit(void)
{
	for (int nCount = 0; nCount < POINT_MAX; nCount++)
	{//���ɂ��m�点
		if (m_pRoadPointer[nCount] == NULL) { continue; }

		for (int nNumber = 0; nNumber < POINT_MAX; nNumber++)
		{
			if (m_pRoadPointer[nCount]->m_pNextPointer[nNumber] == this)
			{//���̎������������炻����NULL��
				m_pRoadPointer[nCount]->m_pNextPointer[nNumber] = NULL;
			}
		}
	}

	for (int nCount = 0; nCount < POINT_MAX; nCount++)
	{
		if (m_pNextPointer[nCount] == NULL) { continue; }
		m_pNextPointer[nCount]->Uninit();
		m_pNextPointer[nCount] = NULL;
	}
	if (m_nNumber == 0)
	{
		CRoad_Manager *pManager = CRoad_Manager::GetManager();
		pManager->SetTop(NULL); pManager->GetnNumAll() = 0;
	}
	delete this;
}
//==================================================================================================//
//    * ����t�����b�V���̐����֐� *
//==================================================================================================//
void	CRoad_Pointer::RankPoint(CPlayer *pPlayer, bool &bGoal)
{
	D3DXVECTOR3 pos = pPlayer->GetPos();
	CRoad_Pointer *&pPoint = pPlayer->GetpPoint();
	int &nMap = pPlayer->GetnMap();
	int &nNumber = pPlayer->GetnPlayerNum();
	bool &bDivided = pPlayer->GetbDivided();
	float fDistance;

	BeyondPoint(pPlayer, true, bGoal);
	BeyondPoint(pPlayer, false, bGoal);

	int nNumPoint = 999999;
	if (pPoint == NULL) { return; }
	if (pPoint != NULL) { nNumPoint = pPoint->m_nNumber + (nMap * 1000); }
	//���������ʂɔ��f
	fDistance = -sqrtf(powf(pos.x - pPoint->m_pos.x, 2) + powf(pos.z - pPoint->m_pos.z, 2));
	if (bDivided)
	{//�����ꓹ�̓r���Ȃ�
		CRoad_Pointer *pPlus = pPoint;
		//�����n�_�܂ł̋����𑪂���Z����
		while (pPlus->m_pRoadPointer[1] == NULL)
		{
			pPlus = pPlus->m_pNextPointer[0];
			fDistance += pPlus->m_fNextDistance[0];
		}
		nNumPoint = pPlus->m_nNumRoad + (nMap * 1000);
	}
	CRoad_Manager::GetManager()->SetDistance(nNumber, nNumPoint, fDistance, bGoal);
}
//==================================================================================================//
//    * ���̃|�C���^�̏��ʕt���֐� *
//==================================================================================================//
bool	CRoad_Pointer::BeyondPoint(CPlayer *&pPlayer, bool bRank, bool &bGoal)
{
	CRoad_Pointer *pPoint;

	if (bRank) { pPoint = pPlayer->GetpPoint(); }
	else { pPoint = pPlayer->GetpEnmPoint(); }

	if (pPoint == NULL) { return false; }
	for (int nCount = 0; nCount < ROAD_MAX; nCount++)
	{
		if (Beyond(pPlayer, pPoint, nCount, bRank, bGoal))
		{//�z�����Ȃ�
			if (bRank)
			{
				if (pPoint->m_nNumber == 0) { pPlayer->GetnMap()++; }
				pPlayer->GetpPoint() = pPoint;
				pPlayer->ChangeRoad();
			}
			else { pPlayer->GetpEnmPoint() = pPoint; }
		}
		if (!bRank) { continue; }
		if (Reverse(pPlayer, pPoint, nCount))
		{
			if (bRank) { pPlayer->GetpPoint() = pPoint; }
			else { pPlayer->GetpEnmPoint() = pPoint; }
		}
	}
	return false;
}
//==================================================================================================//
//    * ���̃|�C���^�ۑ��֐� *
//==================================================================================================//
float	CRoad_Pointer::NextRot(D3DXVECTOR3 &pos, CRoad_Pointer *&pmyPoint, float &fRoad, int &nMap, int nNumber, CCOL_MESH::EFFECT &effect)
{
	float fDistance, fRot;
	D3DXVECTOR3 Point;

	CRoad_Pointer *pPoint = pmyPoint->GetNext(nNumber, nMap, 1);
	if (pPoint == NULL) { return 0.0f; }

	Point = pPoint->m_pos;
	//1�ڂ̃|�C���g�Ƃ̋����A�p�x���v�Z
	fDistance = sqrtf(powf(pos.x - Point.x, 2) + powf(pos.z - Point.z, 2));

	if (fRoad > 0.0f)
	{//IN������
		Point += (pPoint->m_Point[0] - Point) * (0.01f * fRoad);
	}
	else
	{//�A�E�g������
		Point += (pPoint->m_Point[1] - Point) * (0.01f * (-fRoad));
	}
	fRot = atan2f(Point.x - pos.x, Point.z - pos.z);

	if (effect != CCOL_MESH::EFFECT_GRASS)
	{//���͂���������
		fRot = BentRot(pos, pPoint, fRot, fDistance, fRoad);
	}
	return fRot;
}
//==================================================================================================//
//    * ���̃|�C���^�ۑ��֐� *
//==================================================================================================//
float	CRoad_Pointer::BentRot(D3DXVECTOR3 &pos, CRoad_Pointer *&pPoint, float &fRot, float &fDistance, float &fRoad)
{
	float fPercent, fAdd;
	float fARot;
	D3DXVECTOR3 Point;
	//2�ڂ̃|�C���g�Ƃ̋����A�p�x���v�Z
	if (pPoint->m_pNextPointer[0] != NULL)
	{//����NULL�łȂ��Ȃ�
		Point = pPoint->m_pNextPointer[0]->m_pos;
		if (fRoad > 0.0f)
		{//IN������
			Point += (pPoint->m_Point[0] - Point) * (0.01f * fRoad);
		}
		else
		{//�A�E�g������
			Point += (pPoint->m_Point[1] - Point) * (0.01f * (-fRoad));
		}

		fDistance += pPoint->m_fNextDistance[0];
		fARot = atan2f(Point.x - pos.x, Point.z - pos.z);

		fAdd = fARot - fRot;
		if (fAdd > D3DX_PI) { fAdd -= D3DX_PI * 2.0f; }
		if (fAdd < -D3DX_PI) { fAdd += D3DX_PI * 2.0f; }

		//%���␳
		fPercent = 1.0f - ((fDistance - IGNOR_DIS) / BENT_DIS);
		if (fPercent <= 0.0f) { return fRot; }

		fRot += fAdd * fPercent;
		if (fRot > D3DX_PI) { fRot -= D3DX_PI * 2.0f; }
		if (fRot < -D3DX_PI) { fRot += D3DX_PI * 2.0f; }

		BentRot(pos, pPoint->m_pNextPointer[0], fRot, fDistance, fRoad);
	}
	return fRot;
}
//==================================================================================================//
//    * ���̃|�C���^X�p�x�擾�֐� *
//==================================================================================================//
float	CRoad_Pointer::BentRotX(D3DXVECTOR3 &pos, CRoad_Pointer *&pPoint, float &fRot, float &fDistance)
{
	float fPercent, fAdd;
	float fARot, fADistance;
	D3DXVECTOR3 Point, m_pos;
	bool bStrat = false;
	//2�ڂ̃|�C���g�Ƃ̋����A�p�x���v�Z
	if (pPoint->m_pNextPointer[0] != NULL)
	{//����NULL�łȂ��Ȃ�
		Point = pPoint->m_pNextPointer[0]->m_pos;
		m_pos = pPoint->m_pos;

		if (fDistance == 0.0f) { bStrat = true; }
		fDistance += pPoint->m_fNextDistance[0];
		fADistance = (Point.y - m_pos.y) * -1.5f;
		if (fADistance < 0.0f) { fADistance *= 0.5f; }

		fARot = atan2f(sqrtf(powf(Point.x - m_pos.x, 2) + powf(Point.z - m_pos.z, 2)), fADistance) - D3DX_PI * 0.55f;
		if (fARot > D3DX_PI) { fARot -= D3DX_PI * 2.0f; }
		if (fARot < -D3DX_PI) { fARot += D3DX_PI * 2.0f; }

		if (bStrat) { fRot = fARot; }
		else
		{
			fAdd = fARot - fRot;
			if (fAdd > D3DX_PI) { fAdd -= D3DX_PI * 2.0f; }
			if (fAdd < -D3DX_PI) { fAdd += D3DX_PI * 2.0f; }

			//%���␳
			fPercent = 1.0f - (fDistance / BENTX_DIS);
			if (fPercent <= 0.0f) { return fRot; }

			fRot += fAdd * fPercent;
			if (fRot > D3DX_PI) { fRot -= D3DX_PI * 2.0f; }
			if (fRot < -D3DX_PI) { fRot += D3DX_PI * 2.0f; }
		}

		BentRotX(pos, pPoint->m_pNextPointer[0], fRot, fDistance);
	}
	return fRot;
}
//==================================================================================================//
//    * ���̃|�C���^�ۑ��֐� *
//==================================================================================================//
void	CRoad_Pointer::Save(FILE *pFile)
{
	fprintf(pFile, "%f %f %f\n", m_pos.x, m_pos.y, m_pos.z);
	for (int nCount = 0; nCount < POINT_MAX; nCount++)
	{
		fprintf(pFile, "%f %f %f\n", m_Point[nCount].x, m_Point[nCount].y, m_Point[nCount].z);
		fprintf(pFile, "%d\n", (int)m_Trails);
	}
	for (int nCount = 0; nCount < ROAD_MAX; nCount++)
	{//���X�g�\���ɂ�莟�̃|�C���g���Z�[�u
		if (m_pNextPointer[nCount] != NULL)
		{
			fprintf(pFile, "%d\n", nCount);	//�J��Ԃ��w��
			if (m_pNextPointer[nCount]->m_pRoadPointer[1] != NULL && m_pNextPointer[nCount]->m_pRoadPointer[0] != this)
			{//���̃|�C���g�ւ̓�����ȏ゠�� && �����n�_��0�Ԃ������łȂ��Ȃ� �����n�_
				fprintf(pFile, "%d\n", nCount);	//�����n�_�w��
				fprintf(pFile, "%d\n", m_pNextPointer[nCount]->m_nNumber);	//�����n�_�ԍ�
				for (int nNumber = 0; nNumber < ROAD_MAX; nNumber++)
				{
					if (m_pNextPointer[nCount]->m_pRoadPointer[nNumber] == this)
					{
						fprintf(pFile, "%d\n", nNumber);	//�����n�_�̑O����ԍ��w��
					}
				}
			}
			else
			{//�����n�_����Ȃ��Ȃ�
				fprintf(pFile, "%d\n", -1);	//�����n�_�łȂ��w��
				fprintf(pFile, "%d\n", nCount);	//Next�ԍ��w��
				m_pNextPointer[nCount]->Save(pFile);
			}
		}
	}
	fprintf(pFile, "%d\n", -1);
}
//==================================================================================================//
//    * ���̃|�C���^�ǂݍ��݊֐� *
//==================================================================================================//
void	CRoad_Pointer::Load(FILE *pFile, CRoad_Pointer **&pPoint)
{
	int nWKData0, nWKData1;

	fscanf(pFile, "%f %f %f\n", &m_pos.x, &m_pos.y, &m_pos.z);
	for (int nCount = 0; nCount < POINT_MAX; nCount++)
	{
		fscanf(pFile, "%f %f %f\n", &m_Point[nCount].x, &m_Point[nCount].y, &m_Point[nCount].z);
		fscanf(pFile, "%d\n", &m_Trails);
	}

	while (1)
	{//�I���܂ŌJ��Ԃ�
		fscanf(pFile, "%d\n", &nWKData0);
		if (nWKData0 == -1) { break; }

		fscanf(pFile, "%d\n", &nWKData0);
		if (nWKData0 != -1)
		{//���������n�_�Ȃ�|�C���g�������t���邾��
			fscanf(pFile, "%d\n", &nWKData1);
			m_pNextPointer[nWKData0] = pPoint[nWKData1];
			m_fNextDistance[nWKData0] = sqrtf(powf(m_pos.x - pPoint[nWKData1]->m_pos.x, 2) + powf(m_pos.z - pPoint[nWKData1]->m_pos.z, 2));
			fscanf(pFile, "%d\n", &nWKData0);
			pPoint[nWKData1]->m_pRoadPointer[nWKData0] = this;
		}
		else
		{//���������n�_�łȂ��Ȃ�V�����������ēǂݍ���
			fscanf(pFile, "%d\n", &nWKData0);
			Create(m_pNextPointer[nWKData0]);
			m_pNextPointer[nWKData0]->m_pRoadPointer[0] = this;
			m_pNextPointer[nWKData0]->Load(pFile, pPoint);
			m_fNextDistance[nWKData0] = sqrtf(powf(m_pos.x - m_pNextPointer[nWKData0]->m_pos.x, 2) + powf(m_pos.z - m_pNextPointer[nWKData0]->m_pos.z, 2));
		}
	}
	for (int nCount = 0; nCount < POINT_MAX; nCount++)
	{
		if (m_pNextPointer[nCount] == NULL)
		{
			D3DXVECTOR3 &pPos = m_pos;
			m_fNextDistance[nCount] = sqrtf(powf(m_pos.x - pPos.x, 2) + powf(m_pos.z - pPos.z, 2));
			break;
		}
	}
	pPoint[m_nNumber] = this;
}
//==================================================================================================//
//    * ���̃|�C���^�����ݒ�֐� *
//==================================================================================================//
void	CRoad_Pointer::SetRoad(int &nNumRoad, bool &bRoad)
{
	m_nNumRoad = nNumRoad;
	if (m_pNextPointer[1] != NULL) { bRoad = true; }	//�����ꓹ
	if (m_pRoadPointer[1] != NULL) { bRoad = false; }	//�����n�_
	if (!bRoad) { nNumRoad++; }

	for (int nCount = 0; nCount < ROAD_MAX; nCount++)
	{//���X�g�\���ɂ�莟�̃|�C���g��ݒ�
		if (m_pNextPointer[nCount] != NULL)
		{//���ݒ�Ȃ�ݒ�
			if (m_pNextPointer[nCount]->GetnNumRoad() != -1) { continue; }
			if (nCount > 0)
			{
				nNumRoad = m_nNumber;
				bRoad = true;
			}
			m_pNextPointer[nCount]->SetRoad(nNumRoad, bRoad);
		}
	}
}
//==================================================================================================//
//    * �L���[�̃|�C���g�ݒ�֐� *
//==================================================================================================//
CRoad_Pointer	*&CRoad_Pointer::SetKiller(D3DXVECTOR3 &pos, int &nMap)
{
	float fDis = 99999999.9f;
	CRoad_Pointer *pTop = CRoad_Manager::GetManager()->GetTop(1, nMap);
	CRoad_Pointer *pPoint = NULL;
	//���݂̃}�b�v�ň�ԋ߂��̂�T���i���ȏ�߂��͖̂�������j
	return pTop->SearchKiller(pos, fDis, pPoint);
}
//==================================================================================================//
//    * ���̃|�C���^�ڑ��֐� *
//==================================================================================================//
CRoad_Pointer	*&CRoad_Pointer::SearchKiller(D3DXVECTOR3 &pos, float &fDistance, CRoad_Pointer *&pPoint)
{
	float fDis = sqrtf(powf(pos.x - m_pos.x, 2) + powf(pos.z - m_pos.z, 2));

	if (fDis < fDistance && m_pNextPointer[0] != NULL)
	{//�߂��Ȃ�
		fDistance = fDis;
		pPoint = this;
	}

	for (int nCnt = 0; nCnt < ROAD_MAX; nCnt++)
	{
		if (m_pNextPointer[nCnt] != NULL) { m_pNextPointer[nCnt]->SearchKiller(pos, fDistance, pPoint); }
	}

	return pPoint;
}
//==================================================================================================//
//    * ���̃|�C���^�ڑ��֐� *
//==================================================================================================//
void	CRoad_Pointer::Connect(CRoad_Pointer *pPoint)
{
	for (int nCount = 0; nCount < ROAD_MAX; nCount++)
	{//���̃|�C���g�̋󂫂Ɍq��
		if (m_pNextPointer[nCount] != NULL) { continue; }
		m_pNextPointer[nCount] = pPoint;
		for (int nCount = 0; nCount < ROAD_MAX; nCount++)
		{
			if (pPoint->m_pRoadPointer[nCount] != NULL) { continue; }
			pPoint->m_pRoadPointer[nCount] = this;	//�Ȃ񂩖�����肾����2����܂ł����󂯕t���Ȃ���I
			break;
		}
		break;
	}
}
//==================================================================================================//
//    * ���̃|�C���^�z�������֐� *
//==================================================================================================//
bool	CRoad_Pointer::Beyond(CPlayer *&pPlayer, CRoad_Pointer *&pmyPoint, int &nNumber, bool &bRank, bool &bGoal)
{
	D3DXVECTOR3 pos = pPlayer->GetPos();
	int &nMap = pPlayer->GetnMap();

	CRoad_Pointer *pPoint = pmyPoint->GetNext(nNumber, nMap, (bRank ? 0 : 1));
	if (pPoint == NULL)
	{
		if (bRank) { bGoal = true; }
		return false;
	}

	//�z������
	CRoad_Pointer *pWKPoint = NULL;
	D3DXVECTOR3 PointPos0 = pPoint->m_Point[0];
	D3DXVECTOR3 PointPos1 = pPoint->m_Point[1];

	float fRot;
	bool  bBeyond = false;

	float fAngle[2];
	fAngle[0] = atan2f(pPoint->m_Point[1].x - pPoint->m_Point[0].x, pPoint->m_Point[1].z - pPoint->m_Point[0].z);
	fAngle[1] = atan2f(pos.x - pPoint->m_Point[0].x, pos.z - pPoint->m_Point[0].z);

	if (CCOL_MESH::AngleCheck(fAngle[0], fAngle[1]) <= 0)
	{//�z����
		pmyPoint = pPoint;
		bBeyond = true;
	}
	else
	{
		//������⏕
		if (pPoint->m_pNextPointer[0] != NULL || nMap != CRoad_Manager::MAP_MAX - 1)
		{//�����S�[���łȂ��Ȃ�
			if (!bRank)
			{//�G && �����L���O�łȂ��Ȃ�
				fRot = atan2f(pos.x - pPoint->m_pos.x, pos.z - pPoint->m_pos.z);
				pos -= D3DXVECTOR3(sinf(fRot), 0.0f, cosf(fRot)) * IGNOR_DIS;
			}
		}
		fAngle[0] = atan2f(pPoint->m_Point[1].x - pPoint->m_Point[0].x, pPoint->m_Point[1].z - pPoint->m_Point[0].z);
		fAngle[1] = atan2f(pos.x - pPoint->m_Point[0].x, pos.z - pPoint->m_Point[0].z);

		if (CCOL_MESH::AngleCheck(fAngle[0], fAngle[1]) <= 0)
		{//�z����
			pmyPoint = pPoint;
			bBeyond = true;
		}
	}

	//�J��Ԃ�
	return bBeyond;
}
//==================================================================================================//
//    * ���̃|�C���^�t���֐� *
//==================================================================================================//
bool	CRoad_Pointer::Reverse(CPlayer *&pPlayer, CRoad_Pointer *&pmyPoint, int &nNumber)
{
	D3DXVECTOR3 pos = pPlayer->GetPos();
	D3DXVECTOR3 posold = pPlayer->GetOldPos();
	int &nMap = pPlayer->GetnMap();

	CRoad_Pointer *pPoint = pmyPoint;

	//�z������
	bool  bBeyond = false;

	float fAngle[2];
	fAngle[0] = atan2f(pPoint->m_Point[0].x - pPoint->m_Point[1].x, pPoint->m_Point[0].z - pPoint->m_Point[1].z);
	fAngle[1] = atan2f(pos.x - pPoint->m_Point[1].x, pos.z - pPoint->m_Point[1].z);

	//if (CCOL_MESH::CrossCheck(pPoint->m_Point[1], pPoint->m_Point[0], pos, posold))
	if (CCOL_MESH::AngleCheck(fAngle[0], fAngle[1]) <= 0)
	{//�߂��� �����`�F�b�N�B����������Ȃ��ƃR�[�X�ɂ���Ă͌듮�삪�����邽��
		pPoint = pPoint->GetPrev(0, nMap);
		if (pmyPoint->m_nNumber == 0) { pPlayer->GetnMap()--; }
		pmyPoint = pPoint;
		bBeyond = true;
	}

	//�J��Ԃ�
	return bBeyond;
}
//==================================================================================================//
//    * ���̃|�C���^����֐� *
//==================================================================================================//
CRoad_Pointer	*CRoad_Pointer::Release(void)
{
	CRoad_Pointer* pPoint = NULL;

	pPoint = m_pRoadPointer[0];
	for (int nCount = 0; nCount < ROAD_MAX; nCount++)
	{//���ƑO���Ȃ�
		if (m_pNextPointer[nCount] == NULL) { continue; }
		for (int nNumber = 0; nNumber < ROAD_MAX; nNumber++)
		{//���g�̕�����NULL��
			if (m_pNextPointer[nCount]->m_pRoadPointer[nNumber] != this) { continue; }
			m_pNextPointer[nCount]->m_pRoadPointer[nNumber] = NULL;
		}
		for (int nNumber = 0; nNumber < ROAD_MAX; nNumber++)
		{//NULL�������ē���ւ�
			if (m_pNextPointer[nCount]->m_pRoadPointer[nNumber] != NULL) { continue; }
			for (int nRoad = 0; nRoad < ROAD_MAX; nRoad++)
			{//���݂���O�������t����
				if (m_pRoadPointer[nRoad] == NULL) { continue; }
				m_pNextPointer[nCount]->m_pRoadPointer[nNumber] = m_pRoadPointer[nRoad];
				m_pRoadPointer[nRoad] = NULL;
				break;
			}
		}
	}
	for (int nCount = 0; nCount < ROAD_MAX; nCount++)
	{//�O�Ǝ����Ȃ�
		if (m_pRoadPointer[nCount] == NULL) { continue; }
		for (int nNumber = 0; nNumber < ROAD_MAX; nNumber++)
		{//���g�̕�����NULL��
			if (m_pRoadPointer[nCount]->m_pNextPointer[nNumber] != this) { continue; }
			m_pRoadPointer[nCount]->m_pNextPointer[nNumber] = NULL;
		}
		for (int nNumber = 0; nNumber < ROAD_MAX; nNumber++)
		{//NULL�������ē���ւ�
			if (m_pRoadPointer[nCount]->m_pNextPointer[nNumber] != NULL) { continue; }
			for (int nRoad = 0; nRoad < ROAD_MAX; nRoad++)
			{//���݂���O�������t����
				if (m_pNextPointer[nRoad] == NULL) { continue; }
				m_pRoadPointer[nCount]->m_pNextPointer[nNumber] = m_pNextPointer[nRoad];
				m_pNextPointer[nRoad] = NULL;
				break;
			}
		}
	}

	Uninit();
	return pPoint;
}
//==================================================================================================//
//    * ���̃|�C���^�ݒu�֐� *
//==================================================================================================//
void	CRoad_Pointer::Scale(POINT point, float fValue)
{
	float fRot, fDistance;

	//���S����̊p�x���v�Z
	fRot = atan2f(m_Point[point].x - m_pos.x, m_Point[point].z - m_pos.z);

	//���S����̋������v�Z
	fDistance = sqrtf(powf(m_Point[point].x - m_pos.x, 2) + powf(m_Point[point].z - m_pos.z, 2));

	//�����ɔ{���������A���S���v�Z���Ȃ���
	fDistance *= fValue;

	m_Point[point] = m_pos + D3DXVECTOR3(sinf(fRot), 0.0f, cosf(fRot)) * fDistance;

	m_pos = (m_Point[POINT_LEFT] + m_Point[POINT_RIGHT]) / 2.0f;
}
//==================================================================================================//
//    * ���̃|�C���^�ݒu�֐� *
//==================================================================================================//
float	CRoad_Pointer::GetfRotY(void)
{
	float fRot = atan2f(m_Point[1].x - m_Point[0].x, m_Point[1].z - m_Point[0].z) - (D3DX_PI * 0.5f);
	float fDistance = 0.0f;
	float fRoad = 0.0f;
	CRoad_Pointer *pPoint = this;
	if (fRot < D3DX_PI) { fRot += D3DX_PI; }
	fRot = BentRot(m_pos - D3DXVECTOR3(sinf(fRot), 0.0f, cosf(fRot)) * 0.1f, pPoint, fRot, fDistance, fRoad);
	return fRot;
}
//==================================================================================================//
//    * ���̃|�C���^�ݒu�֐� *
//==================================================================================================//
CRoad_Pointer	*CRoad_Pointer::GetNext(int nNumber, int &nMap, int nRank)
{
	CRoad_Pointer *pPoint = NULL;
	if (m_pNextPointer[nNumber] != NULL) { return m_pNextPointer[nNumber]; }
	if (m_pNextPointer[0] != NULL) { return m_pNextPointer[0]; }
	do
	{
		CRoad_Manager *pManager = CRoad_Manager::GetManager();
		if (nMap >= CRoad_Manager::MAP_MAX - 1)
		{
			break;
		}
		else
		{
			pPoint = pManager->GetTop(nRank, nMap + 1);
		}
	} while (pPoint == NULL);
	return pPoint;
}
//==================================================================================================//
//    * ���̃|�C���^�O�l���֐� *
//==================================================================================================//
CRoad_Pointer	*CRoad_Pointer::GetPrev(int nNumber, int &nMap)
{
	CRoad_Pointer *pPoint = NULL;
	int				nWKMap = nMap - 1;
	if (m_pRoadPointer[nNumber] != NULL) { return m_pRoadPointer[nNumber]; }

	do
	{
		CRoad_Manager *pManager = CRoad_Manager::GetManager();
		if (nWKMap < 0)
		{
			break;
		}
		else
		{
			pPoint = pManager->GetCur(nWKMap);
		}
	} while (pPoint == NULL);
	return pPoint;
}
//==================================================================================================//
//    * ���̃|�C���^�ݒu�֐� *
//==================================================================================================//
CRoad_Pointer	*CRoad_Pointer::Plus(CRoad_Pointer *pPointer, D3DXVECTOR3 pos, D3DXVECTOR3 pos2)
{
	CRoad_Pointer *m_pPointer = NULL;
	if (!Create(m_pPointer)) { return NULL; }

	float fDistance, fRot;
	D3DXVECTOR3 &m_pos = m_pPointer->m_pos;

	//����, ����, �p�x���v�Z	�[�̒��_���o��
	m_pos = (pos + pos2) / 2.0f;

	fDistance = sqrtf(powf(m_pos.x - pos.x, 2) + powf(m_pos.z - pos.z, 2));
	fRot = atan2f(pos.x - m_pos.x, pos.z - m_pos.z);
	m_pPointer->m_Point[0] = m_pos + D3DXVECTOR3(sinf(fRot), 0.0f, cosf(fRot)) * (fDistance - 30.0f);

	fDistance = sqrtf(powf(m_pos.x - pos2.x, 2) + powf(m_pos.z - pos2.z, 2));
	fRot = atan2f(pos2.x - m_pos.x, pos2.z - m_pos.z);
	m_pPointer->m_Point[1] = m_pos + D3DXVECTOR3(sinf(fRot), 0.0f, cosf(fRot)) * (fDistance - 30.0f);

	if (pPointer != NULL)
	{
		for (int nCount = 0; nCount < POINT_MAX; nCount++)
		{
			if (pPointer->m_pNextPointer[nCount] == NULL)
			{
				D3DXVECTOR3 &pPos = pPointer->m_pos;
				pPointer->m_pNextPointer[nCount] = m_pPointer;
				pPointer->m_fNextDistance[nCount] = sqrtf(powf(m_pos.x - pPos.x, 2) + powf(m_pos.z - pPos.z, 2));
				break;
			}
		}
	}


	//�O�̃|�C���g�������ĕԂ�
	m_pPointer->m_pRoadPointer[0] = pPointer;
	return m_pPointer;
}

//==================================================================================================//
//    * ����t�����b�V���̐����֐� *
//==================================================================================================//
bool	CRoad_Manager::Create(CRoad_Manager *&pRoadManager)
{
	pRoadManager = NULL;
	pRoadManager = new CRoad_Manager;
	if (pRoadManager != NULL) { pRoadManager->Init(); return true; }

	return false;
}
//==================================================================================================//
//    * ���̃|�C���^(�Ǘ�)�֐� *
//==================================================================================================//
HRESULT CRoad_Manager::Init(void)
{
	m_pRoadManager = this;
	for (int nCount = 0; nCount < MAX_RACER; nCount++)
	{
		m_fDistance[nCount] = 99999999.9f;
		m_nNumPoint[nCount] = 0;
		m_nRanking[nCount] = nCount + 1;
	}

	for (int nCount = 0; nCount < MAX_TOP; nCount++)
	{
		for (int nRank = 0; nRank < MAX_RPOINT; nRank++)
		{
			m_pTop[nRank][nCount] = NULL;
			m_nNumAll[nRank][nCount] = 0;
		}
	}

	m_nWKRank = -1;
	m_nWKData = -1;
	m_pWKTop = NULL;

	return S_OK;
}
//==================================================================================================//
//    * ���̃|�C���^(�Ǘ�)�֐� *
//==================================================================================================//
void	CRoad_Manager::Uninit(void)
{
	for (int nCount = 0; nCount < MAX_TOP; nCount++)
	{
		for (int nRank = 0; nRank < MAX_RPOINT; nRank++)
		{
			m_nWKData = nCount;
			m_nWKRank = nRank;
			if (m_pTop[nRank][nCount] != NULL)
			{//�|�C���g�̍폜
				m_pTop[nRank][nCount]->Uninit();
				m_pTop[nRank][nCount] = NULL;
			}
		}
	}
	m_nWKData = -1;
	if (m_pWKTop != NULL)
	{//�|�C���g�̍폜
		m_pWKTop->Uninit();
		m_pWKTop = NULL;
	}
	m_pRoadManager = NULL;

	delete this;
}
//==================================================================================================//
//    * ���̃|�C���^(�Ǘ�)�֐� *
//==================================================================================================//
void	CRoad_Manager::Save(void)
{
	if (m_pTop[0][0] == NULL) { return; }
	FILE *pFile = NULL;
	pFile = fopen("data/TEXT/ROAD_POINT/Save.txt", "w");

	if (pFile != NULL)
	{//Save�@����
		fprintf(pFile, "%d\n", m_nNumAll[0][0]);
		m_pTop[0][0]->Save(pFile);
		fclose(pFile);
	}
}
//==================================================================================================//
//    * ���̃|�C���^(�Ǘ�)�}�b�v�ǂݍ��݊֐� *
//==================================================================================================//
void	CRoad_Manager::LoadMap(void)
{
	for (int nCount = 0; nCount < MAP_MAX; nCount++)
	{//�}�b�v�̍ő�܂œǂݍ���
		Load(nCount, (MAP)nCount);
	}
}
//==================================================================================================//
//    * ���̃|�C���^(�Ǘ�)�}�b�v�X�̓ǂݍ��݊֐� *
//==================================================================================================//
void	CRoad_Manager::Load(int &nNumber, MAP map)
{
	FILE			*pFile = NULL;
	CRoad_Pointer	**pPoint = NULL;
	int				nNumAll;
	char			aStr[64];

	m_nWKData = nNumber;
	for (int nRank = 0; nRank < MAX_RPOINT; nRank++)
	{
		CRoad_Pointer	*&pTop = m_pTop[nRank][nNumber];
		m_nWKRank = nRank;
		strcpy(aStr, "data/TEXT/ROAD_POINT/");

		if (nRank == 1)
		{
			switch (map)
			{
			case MAP_Half0:		strcat(aStr, "Half0.txt"); break;
			case MAP_Bridge:	strcat(aStr, "Bridge.txt"); break;
			case MAP_Half1:		strcat(aStr, "Half1.txt"); break;
			}
		}
		else
		{
			switch (map)
			{
			case MAP_Half0:		strcat(aStr, "RHalf0.txt"); break;
			case MAP_Bridge:	strcat(aStr, "RBridge.txt"); break;
			case MAP_Half1:		strcat(aStr, "RHalf1.txt"); break;
			}
		}

		if (pTop != NULL) { pTop->Uninit(); }
		if (pTop == NULL) { CRoad_Pointer::Create(pTop); }

		pFile = fopen(aStr, "r");

		if (pFile != NULL)
		{//Load�@����
			fscanf(pFile, "%d\n", &nNumAll);
			pPoint = new CRoad_Pointer*[nNumAll];
			pTop->Load(pFile, pPoint);
			delete[] pPoint;
			fclose(pFile);

			//CPlayer **pPlayer = CGame::GetPlayer();
			//int nMaxPlayer = CGame::GetMaxPlayer();
			//for (int nCount = 0;nCount < nMaxPlayer;nCount++)
			//{
			//	if (pPlayer[nCount] == NULL) { continue; }
			//	pPlayer[nCount]->GetpPoint() = m_pTop[0];
			//}
		}
	}
	m_nWKData = -1;
}
//==================================================================================================//
//    * ���̃|�C���^(�Ǘ�)�֐� *
//==================================================================================================//
void	CRoad_Manager::SetDistance(int &nNumber, int &nNumPoint, float fDistance, bool &bGoal)
{
	if (bGoal == true)
		m_fDistance[nNumber] = (float)m_nRanking[nNumber];
	else
		m_fDistance[nNumber] = fDistance;

	m_nNumPoint[nNumber] = nNumPoint;
	SetRank();
}
//==================================================================================================//
//    * ���̃|�C���^(�Ǘ�)�֐� *
//==================================================================================================//
void	CRoad_Manager::SetRank(void)
{
	int		nWKData;

	for (int nCount = 0; nCount < MAX_RACER; nCount++)
	{//������
		for (int nNumber = 0; nNumber < MAX_RACER; nNumber++)
		{
			if (nCount == nNumber) { continue; }
			if (m_nRanking[nNumber] < m_nRanking[nCount])
			{//���ʂŔ�r
				if (m_nNumPoint[nNumber] < m_nNumPoint[nCount]
					|| (m_nNumPoint[nNumber] == m_nNumPoint[nCount] && m_fDistance[nNumber] > m_fDistance[nCount]))
				{//�|�C���g���傫�� || (�|�C���g������ && �������Z���Ȃ�)
					nWKData = m_nRanking[nCount];
					m_nRanking[nCount] = m_nRanking[nNumber];
					m_nRanking[nNumber] = nWKData;
				}
			}
		}
	}
}
//==================================================================================================//
//    * ���̃|�C���^(�Ǘ�)�֐� *
//==================================================================================================//
void	CRoad_Manager::SetRoad(void)
{
	for (int nCount = 0; nCount < MAX_TOP; nCount++)
	{
		if (m_pTop[nCount] == NULL) { return; }

		int nNumber = 0;
		bool bRoad = false;

		m_pTop[0][nCount]->SetRoad(nNumber, bRoad);
	}
}