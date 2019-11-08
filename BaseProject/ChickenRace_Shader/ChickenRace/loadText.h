//=============================================================================
//
// �e�L�X�g�̓ǂݍ��ݏ��� [LoadText.h]
// Author : �������� Sato_Asumi
//
//=============================================================================
#ifndef _LOADTEXT_H_
#define _LOADTEXT_H_

#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include "motion.h"
#include "player.h"

//=============================================================================
// �e�L�X�g�ǂݍ��݃N���X
//=============================================================================
class CLoadText
{
public:
	//�e�N�X�`���̏��
	typedef struct
	{
		int		nMaxTex;			//�e�N�X�`���̍ő吔
		char ** ppFileName;			//�t�@�C�����i�[����
	}TEX_INFO;

	CLoadText();
	~CLoadText();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	static CLoadText * Create(void);
	TEX_INFO GetTexInfo(void) { return m_TexInfo; };									//�e�N�X�`�����
	void SetLoadTextFileName(char * pFileName) { m_pLoadTextFileName = pFileName; };	//�ǂݍ��ރe�L�X�g�t�@�C���ڂ̕ۊ�

protected:
	char * ReadLine(FILE * pFile, char * pDest);
	char * AddChar(char * pStrCur, char * aStrCur, int nAddChar);
	int PopString(char * pStrCur, char * aStrCur);
	void AddNull(char * pLine);
	char * m_pStrCur;					//������̐擪�ւ̃|�C���^
	char  m_aLine[256];				//������ǂݍ��ݗp�i��s���j
	char  m_aStrCur[256];				//�����񔲂��o���p
	TEX_INFO m_TexInfo;				//�e�N�X�`�����
	char * m_pLoadTextFileName;		//�e�L�X�g�t�@�C����

};

//=============================================================================
// ���[�V�������̓ǂݍ��݃N���X
//=============================================================================
class CLoadTextMotion : public CLoadText
{
public:
	typedef struct
	{
		int	nMaxModel;				//�ǂݍ��ރ��f���̍ő吔
		int	nMaxParts;				//�ǂݍ��ރp�[�c�̍ő吔
		char ** paFileName;			//�t�@�C�����̊i�[
		D3DXVECTOR3 * pOfSetPos;	//�ʒu
		D3DXVECTOR3 * pOfsetRot;	//����
		int         * pParent;		//�e���
	}MODEL_INFO;

	CLoadTextMotion();
	~CLoadTextMotion();
	HRESULT Init(void);
	void Uninit(void);
	static CLoadTextMotion * Create(char * pFileName);
	CMotion::MOTION_INFO * GetMotionInfo(void) { return m_pMotionInfo; };			//���[�V�������̎擾
	MODEL_INFO GetModelInfo(void) { return m_ModelInfo; };
	int GetMaxMotion(void) { return m_nMaxMotion; };

private:
	CMotion::MOTION_INFO * m_pMotionInfo;
	int m_nMaxMotion;
	MODEL_INFO m_ModelInfo;
};

//=============================================================================
// �v���C���[�̏��
//=============================================================================
class CLoadTextPlayer : public CLoadText
{
public:
	CLoadTextPlayer();
	~CLoadTextPlayer();
	HRESULT Init(void);
	void Uninit(void);
	static CLoadTextPlayer * Create(char * pFileName);
	CPlayer::PLAYER_INFO GetPlayerInfo(void) { return m_PlayerInfo; }

private:
	CPlayer::PLAYER_INFO m_PlayerInfo;
};
#endif