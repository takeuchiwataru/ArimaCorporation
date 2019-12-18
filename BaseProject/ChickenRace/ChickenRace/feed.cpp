//=============================================================================
//
// �a�̏��� [feed.cpp]
// Author : ���R���
//
//=============================================================================
#include "scene.h"
#include "feed.h"
#include "manager.h"
#include "object.h"
#include "meshfield.h"
#include "player.h"
#include "fade.h"
#include "shadow.h"
#include "tutorial.h"
#include "particle.h"
#include "scene3D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_SPEED				(5.0f)
#define OBJCT_ANGLE_REVISION	(0.2f)		// �p�x�␳
#define EFFECT_HIGHT			(250.0f)	// �G�~�b�^�[�̍���
#define FOUNTAIN_UP				(20.0f)		// �����̏㏸������l
#define GET_TIME				(50)		// �a�̏����Ă��鎞��
#define EFFECT_TIME				(30)		// �G�t�F�N�g���o�鎞��

//�X�V�͈�
#define FOUNTAIN_LENGTH			(15000)		//�����̍X�V�͈�
#define LEAF_LENGTH				(10500)		//�t�̍X�V����
#define WOOD_LENGTH				(8000)		//�؂��r���{�[�h�ɓ���ւ��鋗��
#define FLOWER_LENGTH			(5500)		//�Ԓd�̕`��͈�

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
D3DXVECTOR3 CFeed::m_VtxMaxModel[FEEDTYPE_MAX] = {};
D3DXVECTOR3 CFeed::m_VtxMinModel[FEEDTYPE_MAX] = {};

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CFeed::CFeed() : CModel3D(FEED_PRIOTITY, CScene::OBJTYPE_FEED)
{
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_feedType = FEEDTYPE::FEEDTYPE_ATTACK;
	m_nCntGetTimer = 0;
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CFeed::~CFeed() {}

//===============================================================================
//�@����
//===============================================================================
CFeed * CFeed::Create(D3DXVECTOR3 pos, int nZone, int nType)
{
	CFeed *pFeed = NULL;
	D3DXCOLOR col;

	// NULL�`�F�b�N
	if (pFeed == NULL)
	{// �������̓��I�m��

		pFeed = new CFeed;

		if (pFeed != NULL)
		{
			// ���f���̐ݒ�
			pFeed->SetModelType(nType + MODEL_TYPE_FEED_K);
			// �e�N�X�`���̐ݒ�
			pFeed->SetTextureType(nType + TEXTURE_TYPE_FEED_K);
			//��ނ̑��
			pFeed->m_feedType = (FEEDTYPE)nType;
			// �I�u�W�F�N�g�N���X�̐���
			pFeed->Init();
			// �ʒu����
			pFeed->SetPosition(pos);

			pFeed->m_pPin = C3DPolygon::Create(C3DPolygon::TYPE_Pin, pos, INIT_VECTOR, 5);
			pFeed->m_pLight = C3DAnim::Create(C3DPolygon::TYPE_Light, pos, INIT_VECTOR, 5);
			switch (nType)
			{
			case FEEDTYPE_ATTACK:	col = D3DXCOLOR(1.0f, 0.1f, 0.03f, 1.0f);	break;
			case FEEDTYPE_ANNOY:	col = D3DXCOLOR(0.0f, 0.368f, 1.0f, 1.0f);	break;
			case FEEDTYPE_SPEED:	col = D3DXCOLOR(1.0f, 0.98f, 0.02f, 1.0f);	break;
			}
			if (pFeed->m_pPin != NULL) { pFeed->m_pPin->SetColor(col); }
			if (pFeed->m_pLight != NULL) { pFeed->m_pLight->SetColor(col); }
		}
	}

	return pFeed;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CFeed::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//3D���f����Init
	CModel3D::Init();

	// �ʒu�̏�����
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	switch (m_feedType)
	{
	case FEEDTYPE::FEEDTYPE_ATTACK:	m_rot.x = 0.7f;	break;
	case FEEDTYPE::FEEDTYPE_ANNOY:	break;
	case FEEDTYPE::FEEDTYPE_SPEED:	m_rot.x = -0.7f; break;
	}

	//�ʒu�̑��
	CModel3D::SetPosition(pos);

	//�ϐ��̏�����
	m_bGet = true;
	m_bEffect = false;
	m_nCntGetTimer = 0;
	m_nCntEffectTimer = 0;
	m_pPin = NULL;
	m_pLight = NULL;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CFeed::Uninit(void)
{
	//if (m_pPin != NULL) { m_pPin->Uninit(); m_pPin = NULL; }
	//if (m_pLight != NULL) { m_pLight->Uninit(); m_pLight = NULL; }

	//3D���f����Uninit
	CModel3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CFeed::Update(void)
{
	D3DXVECTOR3 pos = CModel3D::GetPosition();

	CModel3D::Update();

	m_rot.y += 0.007f;

	if (m_bGet == false)
	{// ���Ȃ��Ƃ�
		m_nCntGetTimer++;

		if (m_nCntGetTimer > GET_TIME)
		{
			if (m_pLight != NULL) { m_pLight->GetDrawType() = C2D::DRAW_TYPE_ADD; }
			m_bGet = true;
			m_nCntGetTimer = 0;
		}
		if (m_nCntGetTimer > GET_TIME - 15)
		{
			if (m_bEffect == false)
			{
				m_bEffect = true;
			}
		}
	}

	if (m_bEffect == true)
	{// �G�t�F�N�g���o��
		if (m_nCntEffectTimer < EFFECT_TIME)
		{
			if (m_nCntEffectTimer % 5 == 0)
			{
				D3DXVECTOR2 fSize;

				fSize.x = 15.0f + (float)(CServer::Rand() % 3);
				fSize.y = 15.0f + (float)(CServer::Rand() % 3);

				CParticle::Create(D3DXVECTOR3(pos.x, pos.y - 10.0f, pos.z),
					D3DXVECTOR3(sinf((CServer::Rand() % 628) / 100.0f) * ((CServer::Rand() % 2 + 1)), 0.0f, cosf((CServer::Rand() % 628) / 100.0f) * ((CServer::Rand() % 2 + 1))),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
					fSize,
					20,
					CParticle::TEXTURE_SMOKE,
					CParticle::TYPE_TURN,
					0);
			}
			m_nCntEffectTimer++;
		}
		else if (m_nCntEffectTimer >= EFFECT_TIME)
		{
			m_nCntEffectTimer = 0;
			m_bEffect = false;
		}

	}


	CModel3D::SetPosition(pos);
	CModel3D::SetRot(m_rot);
}
//=============================================================================
// �`�揈��
//=============================================================================
void CFeed::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�ʒu�̏�����
	D3DXVECTOR3 Modelpos = CModel3D::GetPosition();
	//�Q�[���̏��
	CManager::MODE pMode = CManager::GetMode();

	//���_�@���̎������K��
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	if (m_bGet == true)
	{
		//�`�揈��
		CModel3D::Draw();
	}


	//���_�@���̎������K��
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}

//===============================================================================
// X�t�@�C���̓ǂݍ���
//===============================================================================
HRESULT CFeed::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	int			nNumVtx = 0;						//���_��
	DWORD		sizeFVF;							//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE		*pVtxBuff;							//���_�o�b�t�@�ւ̃|�C���^

													//���f���̍ő�l�E�ŏ��l���擾����
	for (int nCntModel = 0; nCntModel < FEEDTYPE_MAX; nCntModel++)
	{
		m_VtxMaxModel[nCntModel] = D3DXVECTOR3(-10000, -10000, -10000);	//�ő�l
		m_VtxMinModel[nCntModel] = D3DXVECTOR3(10000, 10000, 10000);	//�ŏ��l

		CModel3D::MODEL_TYPE type;
		switch (nCntModel)
		{
		case FEEDTYPE_ATTACK:	type = MODEL_TYPE_FEED_K;	break;
		case FEEDTYPE_ANNOY:	type = MODEL_TYPE_FEED_B;	break;
		case FEEDTYPE_SPEED:	type = MODEL_TYPE_FEED_S;	break;
		}

		LPD3DXMESH &Mesh = MeshLoad(type);

		if (Mesh != NULL)
		{
			//���_�����擾
			nNumVtx = Mesh->GetNumVertices();

			//���_�t�H�[�}�b�g�̃T�C�Y���擾
			sizeFVF = D3DXGetFVFVertexSize(Mesh->GetFVF());

			//���_�o�b�t�@�̃��b�N
			Mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		//���_���W�̑��

																//�ő�l
				if (vtx.x > m_VtxMaxModel[nCntModel].x)
				{
					m_VtxMaxModel[nCntModel].x = vtx.x;
				}
				if (vtx.y > m_VtxMaxModel[nCntModel].y)
				{
					m_VtxMaxModel[nCntModel].y = vtx.y;
				}
				if (vtx.z > m_VtxMaxModel[nCntModel].z)
				{
					m_VtxMaxModel[nCntModel].z = vtx.z;
				}
				//�ŏ��l
				if (vtx.x < m_VtxMinModel[nCntModel].x)
				{
					m_VtxMinModel[nCntModel].x = vtx.x;
				}
				if (vtx.y < m_VtxMinModel[nCntModel].y)
				{
					m_VtxMinModel[nCntModel].y = vtx.y;
				}
				if (vtx.z < m_VtxMinModel[nCntModel].z)
				{
					m_VtxMinModel[nCntModel].z = vtx.z;
				}

				//�T�C�Y���̃|�C���^��i�߂�
				pVtxBuff += sizeFVF;
			}

			//���_�o�b�t�@�̃A�����b�N
			Mesh->UnlockVertexBuffer();
		}
	}

	return S_OK;
}

//===============================================================================
// �e�N�X�`���̔j��
//===============================================================================
void CFeed::UnLoad(void)
{

}


//===============================================================================
// �����蔻��
//===============================================================================
bool CFeed::CollisionFeed(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld)
{
	//�������Ă��邩�������ĂȂ���
	bool bHit = false;

	if (m_bGet == false) { return bHit; }

	// �e����̎擾
	D3DXVECTOR3 ModelPos = CModel3D::GetPosition();		// �ʒu
	D3DXVECTOR3 VtxMax = m_VtxMaxModel[m_feedType];			// ���f���̍ő�l
	D3DXVECTOR3 VtxMin = m_VtxMinModel[m_feedType];			// ���f���̍ŏ��l
	D3DXVECTOR3 rot = CModel3D::GetRot();

	D3DXVECTOR3 ModelMax = ModelPos + VtxMax;	// �ʒu���݂̍ő�l
	D3DXVECTOR3 ModelMin = ModelPos + VtxMin;	// �ʒu���݂̍ŏ��l

	if (pPos->x >= ModelMin.x - PLAYER_DEPTH && pPos->x <= ModelMax.x + PLAYER_DEPTH)
	{// Z�͈͓̔��ɂ���
		if (pPos->z >= ModelMin.z - PLAYER_DEPTH && pPos->z <= ModelMax.z + PLAYER_DEPTH)
		{// X�͈͓̔��ɂ���
			if (pPosOld->y >= ModelMax.y && pPos->y <= ModelMax.y)
			{// �I�u�W�F�N�g�̏ォ�瓖����ꍇ
				bHit = true;
				m_bGet = false;
				if (m_pLight != NULL) { m_pLight->GetDrawType() = C2D::DRAW_TYPE_NO; }
			}
			else if (pPosOld->y + PLAYER_HEIGHT <= ModelMin.y && pPos->y + PLAYER_HEIGHT >= ModelMin.y)
			{// �I�u�W�F�N�g�̉����瓖����ꍇ
				bHit = true;
				m_bGet = false;
				if (m_pLight != NULL) { m_pLight->GetDrawType() = C2D::DRAW_TYPE_NO; }
			}

			if (!(pPos->y >= ModelMax.y) && !(pPos->y + PLAYER_HEIGHT <= ModelMin.y))
			{// �I�u�W�F�N�g���Ƃ̓����蔻��
				bHit = true;
				m_bGet = false;
				if (m_pLight != NULL) { m_pLight->GetDrawType() = C2D::DRAW_TYPE_NO; }
			}
		}
	}

	// �ʒu�̑��
	CModel3D::SetPosition(ModelPos);

	return bHit;
}
//===============================================================================
// �����蔻��
//===============================================================================
bool CFeed::TargetFeed(CPlayer *pPlayer, float &fValue)
{
	if (m_bGet == false) { return false; }

	D3DXVECTOR3 &m_pos = GetposR();
	D3DXVECTOR3 &pos = pPlayer->Getpos();

	float fDis = sqrtf(powf(m_pos.x - pos.x, 2) + powf(m_pos.z - pos.z, 2));
	if (fDis < 500.0f)
	{//�����`�F�b�N
		if (pPlayer->GetItemNum() < MAX_EGG)
		{//�A�C�e���̓����蔻��
			if (CollisionFeed(&pos, &pPlayer->Getposold())) { return true; }
		}

		if (pPlayer->GetInduction() < CPlayer::INDUCTION_ITEM || fDis <= 100.0f) { return false; }
		float fRot = atan2f(m_pos.x - pos.x, m_pos.z - pos.z);
		float &fRotY = pPlayer->Getrot().y;
		float fWK;
		fWK = fRot - fRotY;
		if (fWK < 0.0f) { fWK *= -1.0f; }

		if (fWK < D3DX_PI * 0.05f)
		{//�p�x�`�F�b�N
			if (fValue > fWK && pPlayer->GetFeedType() != m_feedType)
			{//�a�U��
				fValue = fWK;
				pPlayer->GetfFeedRot() = fRot;
				pPlayer->GetInduction() = CPlayer::INDUCTION_ITEM;
			}
		}
	}
	return false;
}