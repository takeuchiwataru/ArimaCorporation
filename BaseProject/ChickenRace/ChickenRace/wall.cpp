//=============================================================================
//
// �Ǐ��� [wall.cpp]
// Author : �L�n���u
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
// �}�N����`
//=============================================================================
#define WALL_TEX_1	"data\\TEXTURE\\wall\\wallRock.jpg"			// �e�N�X�`���̖��O
#define WALL_TEX_2	"data\\TEXTURE\\wall\\Black.jpg"			// �e�N�X�`���̖��O

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9 CWall::m_pTexture[MAX_WALL_TEX] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CWall::CWall() : CScene3D(WALL_PRIOTITY, CScene::OBJTYPE_WALL)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CWall::~CWall()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CWall::Init()
{
	//����������
	CScene3D::Init();

	//��ނ̐ݒ�
	SetTypeNumber(TYPE_WALL);

	if (m_nType == 9)
	{
		//CScene3D::SetNor(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CWall::Uninit(void)
{
	//for (int nCount = 0; nCount < MAX_WALL_TEX; nCount++)
	//{
	//	// �e�N�X�`���̔j��
	//	if (m_pTexture[nCount] != NULL)
	//	{
	//		m_pTexture[nCount]->Release();
	//		m_pTexture[nCount] = NULL;
	//	}
	//}

	CScene3D::Uninit();
}
//=============================================================================
// �X�V����
//=============================================================================
void CWall::Update(void)
{
	CScene3D::Update();
}
//=============================================================================
// �`�揈��
//=============================================================================
void CWall::Draw(void)
{
	//�Q�[���̏��
	CManager::MODE pMode = CManager::GetMode();

	CScene3D::Draw();
}
//=============================================================================
// ��������
//=============================================================================
CWall * CWall::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR3 rot, int nTexType)
{
	CWall *pWall = NULL;

	//NULL�`�F�b�N
	if (pWall == NULL)
	{//�������̓��I�m��

		pWall = new CWall;

		if (pWall != NULL)
		{
			//��ނ̐ݒ�
			pWall->m_nType = nTexType;
			//����������
			pWall->Init();
			//�ʒu�̐ݒ�
			pWall->m_pos = pos;
			//�����蔻��p
			pWall->m_size = size;
			//�e�N�X�`���̐ݒ�
			pWall->BindTexture(m_pTexture[nTexType]);
			//�ʒu�Ɖ�]�ƃT�C�Y
			pWall->SetWall(pos, rot, size);
		}
	}

	return pWall;
}
//=============================================================================
// �ǂ̓����蔻��
//=============================================================================
bool CWall::CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fDepth)
{
	bool bTouch = false;

	D3DXVECTOR3 WallPos = GetPosition();
	D3DXVECTOR3 WallRot = GetRotation();
	D3DXVECTOR2 WallSize = GetSize();

	if (WallRot.y == (0.0f * D3DX_PI))
	{// Z�����̕�
		if (pPos->x <= WallPos.x + WallSize.x + 18.5f
			&& pPos->x >= WallPos.x - WallSize.x - 18.5f)
		{// �ǂƓ���z���ɑ��݂��Ă���
			if (pPos->z <= WallPos.z + WallSize.x + 18.5f
				&& pPos->z >= WallPos.z - WallSize.x - 18.5f)
			{// �ǂƓ���x���ɑ��݂��Ă���
				if (pPos->y < WallPos.y + WallSize.y && pPos->y + 100.0f >= WallPos.y)
				{// �ǂ����Ⴂ
					if (pPos->z >= WallPos.z - fDepth
						&& pPosOld->z <= WallPos.z - fDepth)
					{// ��O����ǂ��ђʂ���
						pPos->z = WallPos.z - fDepth;
						bTouch = true;
					}
					else if (pPos->z <= WallPos.z + fDepth
						&& pPosOld->z >= WallPos.z + fDepth)
					{// ������ǂ��ђʂ���
						pPos->z = WallPos.z + fDepth;
						bTouch = true;
					}
				}
			}
		}
	}			
	else if (WallRot.y == (0.5f * D3DX_PI))
	{// X�����̕�
		if (pPos->x <= WallPos.x + WallSize.x + 18.5f
			&& pPos->x >= WallPos.x - WallSize.x - 18.5f)
		{// �ǂƓ���z���ɑ��݂��Ă���
			if (pPos->z <= WallPos.z + WallSize.x + 18.5f
				&& pPos->z >= WallPos.z - WallSize.x - 18.5f)
			{// �ǂƓ���x���ɑ��݂��Ă���
				if (pPos->y < WallPos.y + WallSize.y && pPos->y + 100.0f >= WallPos.y)
				{// �ǂ����Ⴂ
					if (pPos->x >= WallPos.x - fDepth
						&& pPosOld->x <= WallPos.x - fDepth)
					{// ��O����ǂ��ђʂ���
						pPos->x = WallPos.x - fDepth;
						bTouch = true;
					}
					else if (pPos->x <= WallPos.x + fDepth
						&& pPosOld->x >= WallPos.x + fDepth)
					{// ������ǂ��ђʂ���
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
//	�X�e�[�W�ړ����ɏ��������邽��
//=============================================================================
void CWall::DeleteWall(void)
{
	CScene *pScene;

	//�v���C�I���e�B�[�`�F�b�N
	pScene = CScene::GetTop(WALL_PRIOTITY);

	//NULL�`�F�b�N
	while (pScene != NULL)
	{
		//Update��Uninit����Ă��܂��ꍇ�@Next��������\�������邩��Next�Ƀf�[�^���c���Ă���
		CScene *pSceneNext = pScene->GetNext();

		if (pScene->GetDeath() == false)
		{//�^�C�v���ǂ�������
			if (pScene->GetObjType() == OBJTYPE_WALL)
			{
				pScene->Uninit();
			}
		}
		//Next�Ɏ���Scene������
		pScene = pSceneNext;
	}
}
//=============================================================================
// �ǂݍ��ݏ���
//=============================================================================
HRESULT CWall::Load(void)
{
	// �f�o�C�X�擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// �e�N�X�`������
	D3DXCreateTextureFromFile(pDevice, WALL_TEX_1, &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, WALL_TEX_2, &m_pTexture[1]);
	
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CWall::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_WALL_TEX; nCount++)
	{
		// �e�N�X�`���j��
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}