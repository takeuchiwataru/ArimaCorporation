//=============================================================================
//
// �Ђ悱�̏��� [chick.cpp]
// Author : ���R���
//
//=============================================================================
#include "scene.h"
#include "chick.h"
#include "manager.h"
#include "object.h"
#include "meshfield.h"
#include "player.h"
#include "fade.h"
#include "shadow.h"
#include "tutorial.h"
#include "enemy.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CHICK_NAME	"data\\MODEL\\Weapon\\chick.x"			// �ǂݍ��ރ��f���t�@�C��

#define MODEL_SPEED				(5.0f)
#define PLAYER_DEPTH			(50)		// �v���C���[�̕������p
#define PLAYER_HEIGHT			(100.0f)	// �v���C���[�̔w�̍���
#define OBJCT_ANGLE_REVISION	(0.2f)		// �p�x�␳
#define EFFECT_HIGHT			(250.0f)	// �G�~�b�^�[�̍���
#define FOUNTAIN_UP				(20.0f)		// �����̏㏸������l

#define CHICK_SPEED				(25.0f)		// �Ђ悱�����ł��X�s�[�h

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
LPD3DXMESH CChick::m_pMeshModel = NULL;						//���b�V�����ւ̃|�C���^
LPD3DXBUFFER CChick::m_pBuffMatModel = NULL;					//�}�e���A���̏��ւ̃|�C���^
DWORD CChick::m_nNumMatModel = NULL;							//�}�e���A���̏��
LPDIRECT3DTEXTURE9 CChick::m_pMeshTextures = NULL;
D3DXVECTOR3 CChick::m_VtxMaxModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 CChick::m_VtxMinModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

//===============================================================================
//�@�f�t�H���g�R���X�g���N�^
//===============================================================================
CChick::CChick() : CModel3D(EGG_PRIOTITY, CScene::OBJTYPE_CHICK)
{
	m_scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �傫��
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fDestAngle = 0.0f;
	m_fDiffAngle = 0.0f;
	m_fHeight = 0.0f;
	m_nRank = 0;
	m_nNumPlayer = 0;
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CChick::~CChick() {}

//===============================================================================
//�@����
//===============================================================================
CChick * CChick::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type, BULLETTYPE bulletType, int nNumPlayer)
{
	CChick *pChick = NULL;

	// NULL�`�F�b�N
	if (pChick == NULL)
	{// �������̓��I�m��

		pChick = new CChick;

		if (pChick != NULL)
		{
			// ��ނ̐ݒ�
			pChick->BindModel(m_pMeshModel, m_pBuffMatModel, m_nNumMatModel, m_pMeshTextures,
				m_VtxMaxModel, m_VtxMinModel);
			// �T�C�Y����
			pChick->m_scale = scale;
			// �T�C�Y��e�N���X�ɑ��
			pChick->SetScale(scale);
			// ���̎�ނ���
			pChick->m_type = type;
			// �e�̎�ނ���
			pChick->m_bulletType = bulletType;
			// �I�u�W�F�N�g�N���X�̐���
			pChick->Init();
			// �ʒu����
			pChick->SetPosition(pos);
			// ��]�𔽉f
			pChick->SetRot(rot);
			// ���ʂ̗���
			pChick->m_nNumPlayer = nNumPlayer;
		}
	}

	return pChick;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CChick::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//3D���f����Init
	CModel3D::Init();

	// �ʒu�̏�����
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�ʒu�̑��
	CModel3D::SetPosition(pos);

	// �e����̑��
	CModel3D::SetScale(m_scale);

	//�ϐ��̏�����
	m_pObjBill = NULL;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fHeight = 0.0f;
	m_fDestAngle = 0.0f;
	m_fDiffAngle = 0.0f;
	m_bJump = false;
	m_state = STATE_NORMAL;
	m_nRank = 0;
	m_nNumPlayer = 0;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CChick::Uninit(void)
{
	//�I�u�W�F�N�g�r���{�[�h
	m_pObjBill = NULL;

	//3D���f����Uninit
	CModel3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CChick::Update(void)
{
	D3DXVECTOR3 pos = CModel3D::GetPosition();

	if (m_state == STATE_BULLET)
	{
		float fHeight = 0.0f;

		if (m_type == TYPE_ATTACK)
		{
			fHeight = 30.0f;
		}
		m_fHeight = SetHeight() + fHeight;
		Bullet();
	}
	else
	{
		m_move.y -= cosf(0) * 0.4f;
		m_fHeight += m_move.y;
	}

	pos.x += m_move.x;
	pos.z += m_move.z;

	CModel3D::SetMove(m_move);
	CModel3D::SetPosition(D3DXVECTOR3(pos.x, m_fHeight + 15.0f, pos.z));

	CDebugProc::Print("%.1f\n", m_fHeight);

	//�����̎擾
	float fLength = CModel3D::GetLength();

	if (CModel3D::GetDelete() == true) { Uninit(); }
}
//=============================================================================
// �`�揈��
//=============================================================================
void CChick::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�ʒu�̏�����
	D3DXVECTOR3 Modelpos = CModel3D::GetPosition();
	//�Q�[���̏��
	CManager::MODE pMode = CManager::GetMode();

	//���_�@���̎������K��
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	// �F�ύX
	if (m_type == TYPE_ATTACK)
	{
		CModel3D::Setcol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else if (m_type == TYPE_ANNOY)
	{
		CModel3D::Setcol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	}
	else if (m_type == TYPE_SPEED)
	{
		CModel3D::Setcol(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	}

	//�`�揈��
	CModel3D::Draw();

	//���_�@���̎������K��
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}

//===============================================================================
// X�t�@�C���̓ǂݍ���
//===============================================================================
HRESULT CChick::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL *pMat;

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(CHICK_NAME, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel, NULL, &m_nNumMatModel, &m_pMeshModel);

	//�}�e���A����񂩂�e�N�X�`���̎擾
	pMat = (D3DXMATERIAL*)m_pBuffMatModel->GetBufferPointer();

	int nNumVtx;		//���_��
	DWORD sizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

						//���f���̍ő�l�E�ŏ��l���擾����
	m_VtxMaxModel = D3DXVECTOR3(-10000, -10000, -10000);	//�ő�l
	m_VtxMinModel = D3DXVECTOR3(10000, 10000, 10000);	//�ŏ��l

														//���_�����擾
	nNumVtx = m_pMeshModel->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(m_pMeshModel->GetFVF());

	//���_�o�b�t�@�̃��b�N
	m_pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		//���_���W�̑��

														//�ő�l
		if (vtx.x > m_VtxMaxModel.x)
		{
			m_VtxMaxModel.x = vtx.x;
		}
		if (vtx.y > m_VtxMaxModel.y)
		{
			m_VtxMaxModel.y = vtx.y;
		}
		if (vtx.z > m_VtxMaxModel.z)
		{
			m_VtxMaxModel.z = vtx.z;
		}
		//�ŏ��l
		if (vtx.x < m_VtxMinModel.x)
		{
			m_VtxMinModel.x = vtx.x;
		}
		if (vtx.y < m_VtxMinModel.y)
		{
			m_VtxMinModel.y = vtx.y;
		}
		if (vtx.z < m_VtxMinModel.z)
		{
			m_VtxMinModel.z = vtx.z;
		}

		//�T�C�Y���̃|�C���^��i�߂�
		pVtxBuff += sizeFVF;
	}

	//���_�o�b�t�@�̃A�����b�N
	m_pMeshModel->UnlockVertexBuffer();

	//�g���Ă���e�N�X�`��
	//D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_1, &m_pMeshTextures[0]);

	return S_OK;
}

//===============================================================================
// �e�N�X�`���̔j��
//===============================================================================
void CChick::UnLoad(void)
{
	// ���b�V���̊J��
	if (m_pMeshModel != NULL)
	{
		m_pMeshModel->Release();
		m_pMeshModel = NULL;
	}
	// �}�e���A���̊J��
	if (m_pBuffMatModel != NULL)
	{
		m_pBuffMatModel->Release();
		m_pBuffMatModel = NULL;
	}

	//�e�N�X�`��
	if (m_pMeshTextures != NULL)
	{
		m_pMeshTextures->Release();
		m_pMeshTextures = NULL;
	}
}


//===============================================================================
// �����蔻��
//===============================================================================
bool CChick::CollisionChick(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld)
{
	//���͏��
	CInputKeyBoard *pCInputKeyBoard = CManager::GetInput();

	//�������Ă��邩�������ĂȂ���
	bool bHit = false;

	// �e����̎擾
	D3DXVECTOR3 ModelPos = CModel3D::GetPosition();		// �ʒu
	D3DXVECTOR3 VtxMax = CModel3D::GetVtxMax();			// ���f���̍ő�l
	D3DXVECTOR3 VtxMin = CModel3D::GetVtxMin();			// ���f���̍ŏ��l
	D3DXVECTOR3 rot = CModel3D::GetRot();

	D3DXVECTOR3 ModelMax = CModel3D::GetPosition() + CModel3D::GetVtxMax();	// �ʒu���݂̍ő�l
	D3DXVECTOR3 ModelMin = CModel3D::GetPosition() + CModel3D::GetVtxMin();	// �ʒu���݂̍ŏ��l

	if (pPos->x >= ModelMin.x - PLAYER_DEPTH && pPos->x <= ModelMax.x + PLAYER_DEPTH)
	{// Z�͈͓̔��ɂ���
		if (pPos->z >= ModelMin.z - PLAYER_DEPTH && pPos->z <= ModelMax.z + PLAYER_DEPTH)
		{// X�͈͓̔��ɂ���
			if (pPosOld->y >= ModelMax.y && pPos->y <= ModelMax.y)
			{// �I�u�W�F�N�g�̏ォ�瓖����ꍇ
				bHit = true;
			}
			else if (pPosOld->y + PLAYER_HEIGHT <= ModelMin.y && pPos->y + PLAYER_HEIGHT >= ModelMin.y)
			{// �I�u�W�F�N�g�̉����瓖����ꍇ
				bHit = true;
			}

			if (!(pPos->y >= ModelMax.y) && !(pPos->y + PLAYER_HEIGHT <= ModelMin.y))
			{// �I�u�W�F�N�g���Ƃ̓����蔻��
				bHit = true;
			}
		}
	}

	// �ʒu�̑��
	CModel3D::SetPosition(ModelPos);

	return bHit;
}

//=============================================================================
// ��������
//=============================================================================
float CChick::SetHeight(void)
{
	float fHeight = 0.0f;

	CScene *pScene = CScene::GetTop(MESH_PRIOTITY);

	D3DXVECTOR3 pos = CModel3D::GetPosition();

	//NULL�`�F�b�N
	while (pScene != NULL)
	{
		//Update��Uninit����Ă��܂��ꍇ�@Next��������\�������邩��Next�Ƀf�[�^���c���Ă���
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetObjType() == CScene::OBJTYPE_GROUND)
		{//�^�C�v���n�ʂ�������
			CMeshField *pField = (CMeshField*)pScene;

			if (pField->OnField(pos, 0.0f))
			{// �X�΂̌v�Z
				fHeight = pField->GetHeightMesh(CModel3D::GetPosition());

				if (m_bJump == false || (m_bJump == true && m_fHeight < fHeight))
				{
					m_fHeight = fHeight;					//�n�ʂ̍������擾
					m_move.y = 0.0f;					//�ړ��ʂ�����������

														//�W�����v�̏�Ԑݒ�
					m_bJump = false;

					CModel3D::SetMove(m_move);
					CModel3D::SetPosition(D3DXVECTOR3(pos.x, fHeight, pos.z));

					break;
				}
			}
		}
		//Next�Ɏ���Scene������
		pScene = pSceneNext;
	}

	return fHeight;
}

//=============================================================================
// �W�����v
//=============================================================================
void CChick::Jump(void)
{
	// �W�����v
	if (m_bJump == false)
	{// �W�����v���Ă��Ȃ�
		m_bJump = true;
		m_move.y += 5.5f;
	}

	CModel3D::SetMove(m_move);
}

//=============================================================================
// ���ł�
//=============================================================================
void CChick::Bullet(void)
{
	if (m_bulletType == BULLETTYPE_PLAYER)
	{
		if (m_type == TYPE_ATTACK)
		{//�^�C�v���G��������
		 // 1�O�̃v���C���[�ɔ��ł���
			CPlayer **pPlayer = CGame::GetPlayer();
			D3DXVECTOR3 pos = CModel3D::GetPosition();

			int nRank = m_nRank - 1;
			int nCntChar = 0;

			if (nRank >= 0)
			{
				for (nCntChar = 0; nCntChar < MAX_PLAYER; nCntChar++)
				{// �ЂƂO�̏��ʂ̂��������
					int nData = CGame::GetRanking(nCntChar);

					if (nRank == nData)
					{
						break;
					}
				}

				// �ړI�̊p�x
				m_fDestAngle = atan2f(pPlayer[nCntChar]->GetPos().x - pos.x, pPlayer[nCntChar]->GetPos().z - pos.z);

				// ����
				m_fDiffAngle = m_fDestAngle - m_rot.y;

				if (m_fDiffAngle > D3DX_PI)
				{
					m_fDiffAngle -= D3DX_PI * 2.0f;
				}
				if (m_fDiffAngle < -D3DX_PI)
				{
					m_fDiffAngle += D3DX_PI * 2.0f;
				}

				m_rot.y += m_fDiffAngle * 0.5f;

				if (m_rot.y > D3DX_PI)
				{
					m_rot.y -= D3DX_PI * 2.0f;
				}
				if (m_rot.y < -D3DX_PI)
				{
					m_rot.y += D3DX_PI * 2.0f;
				}
			}


			//���f���̈ړ�	���f���̈ړ�����p�x(�J�����̌��� + �p�x) * �ړ���
			m_move.x = sinf(m_rot.y) * CHICK_SPEED;
			m_move.z = cosf(m_rot.y) * CHICK_SPEED;

			m_rot.x = D3DX_PI * 0.5f;

			CModel3D::SetRot(m_rot);
		}
	}
	else if (m_bulletType == BULLETTYPE_ENEMY)
	{
		CScene *pScene = CScene::GetTop(3);

		D3DXVECTOR3 pos = CModel3D::GetPosition();
		m_rot = CModel3D::GetRot();

		//NULL�`�F�b�N
		while (pScene != NULL)
		{
			//Update��Uninit����Ă��܂��ꍇ�@Next��������\�������邩��Next�Ƀf�[�^���c���Ă���
			CScene *pSceneNext = pScene->GetNext();

			if (pScene->GetObjType() == CScene::OBJTYPE_PLAYER && m_type == TYPE_ATTACK)
			{//�^�C�v���G��������
				CPlayer *pPlayer = (CPlayer*)pScene;

				// �ړI�̊p�x
				m_fDestAngle = atan2f(pPlayer->GetPos().x - pos.x, pPlayer->GetPos().z - pos.z);

				// ����
				m_fDiffAngle = m_fDestAngle - m_rot.y;

				if (m_fDiffAngle > D3DX_PI)
				{
					m_fDiffAngle -= D3DX_PI * 2.0f;
				}
				if (m_fDiffAngle < -D3DX_PI)
				{
					m_fDiffAngle += D3DX_PI * 2.0f;
				}

				m_rot.y += m_fDiffAngle * 0.05f;

				if (m_rot.y > D3DX_PI)
				{
					m_rot.y -= D3DX_PI * 2.0f;
				}
				if (m_rot.y < -D3DX_PI)
				{
					m_rot.y += D3DX_PI * 2.0f;
				}

				//���f���̈ړ�	���f���̈ړ�����p�x(�J�����̌��� + �p�x) * �ړ���
				m_move.x = sinf(m_rot.y) * CHICK_SPEED;
				m_move.z = cosf(m_rot.y) * CHICK_SPEED;

				m_rot.x = D3DX_PI * 0.5f;
			}
			//Next�Ɏ���Scene������
			pScene = pSceneNext;
		}
		CModel3D::SetRot(m_rot);
	}
}