//=============================================================================
//
// テキストの読み込み処理 [LoadText.h]
// Author : 佐藤安純 Sato_Asumi
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
// テキスト読み込みクラス
//=============================================================================
class CLoadText
{
public:
	//テクスチャの情報
	typedef struct
	{
		int		nMaxTex;			//テクスチャの最大数
		char ** ppFileName;			//ファイルを格納する
	}TEX_INFO;

	CLoadText();
	~CLoadText();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	static CLoadText * Create(void);
	TEX_INFO GetTexInfo(void) { return m_TexInfo; };									//テクスチャ情報
	void SetLoadTextFileName(char * pFileName) { m_pLoadTextFileName = pFileName; };	//読み込むテキストファイル目の保管

protected:
	char * ReadLine(FILE * pFile, char * pDest);
	char * AddChar(char * pStrCur, char * aStrCur, int nAddChar);
	int PopString(char * pStrCur, char * aStrCur);
	void AddNull(char * pLine);
	char * m_pStrCur;					//文字列の先頭へのポインタ
	char  m_aLine[256];				//文字列読み込み用（一行分）
	char  m_aStrCur[256];				//文字列抜き出し用
	TEX_INFO m_TexInfo;				//テクスチャ情報
	char * m_pLoadTextFileName;		//テキストファイル名

};

//=============================================================================
// モーション情報の読み込みクラス
//=============================================================================
class CLoadTextMotion : public CLoadText
{
public:
	typedef struct
	{
		int	nMaxModel;				//読み込むモデルの最大数
		int	nMaxParts;				//読み込むパーツの最大数
		char ** paFileName;			//ファイル名の格納
		D3DXVECTOR3 * pOfSetPos;	//位置
		D3DXVECTOR3 * pOfsetRot;	//向き
		int         * pParent;		//親情報
	}MODEL_INFO;

	CLoadTextMotion();
	~CLoadTextMotion();
	HRESULT Init(void);
	void Uninit(void);
	static CLoadTextMotion * Create(char * pFileName);
	CMotion::MOTION_INFO * GetMotionInfo(void) { return m_pMotionInfo; };			//モーション情報の取得
	MODEL_INFO GetModelInfo(void) { return m_ModelInfo; };
	int GetMaxMotion(void) { return m_nMaxMotion; };

private:
	CMotion::MOTION_INFO * m_pMotionInfo;
	int m_nMaxMotion;
	MODEL_INFO m_ModelInfo;
};

//=============================================================================
// プレイヤーの情報
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