/************************************************************************/
/*                                                                      */
/*  Copyright (C) 2005 by DaHeng Image Vision Company.                  */
/*  All rights reserved. Property of DaHeng Image Vision Company.       */
/*  Restricted rights to use, duplicate or disclose this code are       */
/*  granted through contract.                                           */
/*                                                                      */
/*----------------------------------------------------------------------*/
/*                                                                      */
/*  Name:			ColorRender.cpp                                     */
/*  Description:	This file implement the class of color render       */
/*  Author:         Michael Chang				                        */
/*  Date:			2005-04-08                                          */
/*                                                                      */
/************************************************************************/

#include "stdafx.h"
#include "ColorRender.h"
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <assert.h>
#include "..\inc\raw2rgb.h"


//////////////////////////////////////////////////////////////////////////

/*
	功能：	得到白平衡的调整系数
	参数：	pSource：rgb数据
*/

BOOL CColorRender::PlxGetWBalanceRatio(BYTE *pSrce,int nWid,int nHei,double* dRatioR,double* dRatioG,double* dRatioB) 
{
	GetWhiteBalanceRatio(pSrce, nWid, nHei, dRatioR, dRatioG, dRatioB);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

/*
	功能：	MySetGammaLut  根据Gamma校正系数修改Gamma查值表
	参数：	dGammaRatio: Gamma校正系数
			pLutGamma: 传入的查找表
	注意：	dGammaRatio的取值范围为0.1-10
*/

void CColorRender::PlxSetGammaLut(BYTE pLutGamma[256],double dGammaRatio)
{
	SetGammaLut(pLutGamma, dGammaRatio);
}

//////////////////////////////////////////////////////////////////////////

/*
	功能：	将以BG开始的Bayer 图像数据　转化成　RGB24 格式
	参数：	pDest：目标图像数据区，RGB24 格式
			pSrce：原始图像数据，RAW格式
			sz:图像大小
			cvtype：插值类型
			dataType:原始数据类型
*/

BOOL CColorRender::PlxConvertBayer2Rgb(BYTE *pDest,BYTE *pSrce, int nWid,int nHei,HV_BAYER_CONVERT_TYPE cvtype,
									   BYTE pLutR[LUT_LENGTH], BYTE pLutG[LUT_LENGTH], BYTE pLutB[LUT_LENGTH], bool bIsFlip, HV_BAYER_LAYOUT Layout)
{
	if(pSrce == NULL || pDest==NULL) return FALSE;

	ConvertBayer2Rgb(pDest, pSrce, nWid, nHei, cvtype, pLutR, pLutG, pLutB, bIsFlip, Layout);

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////

/*
	功能：	得到区域RGB均值
	参数：	pSource：rgb数据
*/

BOOL CColorRender::GetRectRGBValue(BYTE *pSrce,int nWid,int nHei,int step,int* R,int* G,int* B)
{

	if(pSrce == NULL ) return FALSE;
	
	if ( nWid <= 4 || nHei <=4) return FALSE;
	
	int nWidth = nWid;
	int	nHeight = nHei;

	DWORD dwRedSum=0;
	DWORD dwGreenSum=0;
	DWORD dwBlueSum=0;
	
	DWORD dwRedNum=0;
	DWORD dwGreenNum=0;
	DWORD dwBlueSNum=0;
	
	DWORD dwPos =0;
	
	for(int y=0;y<nHeight;y++)
	{
		dwPos = y*step;
		for(int x=0;x<nWidth;x++)
		{
			//blue
			dwBlueSum+=(pSrce[dwPos]);
			dwBlueSNum++;
			dwPos++;
			//green
			dwGreenSum+=(pSrce[dwPos]);
			dwGreenNum++;
			dwPos++;
			//red
			dwRedSum+=(pSrce[dwPos]);
			dwRedNum++;
			dwPos++;
		}
	}

	//彩色均值
	*R = dwRedSum/dwRedNum;
	*G = dwGreenSum/dwGreenNum;
	*B = dwBlueSum/dwBlueSNum;

	return TRUE;
}

/*
	功能：	得到区域Gray均值
	参数：	pSource：Gray数据
*/

BOOL CColorRender::GetRectGrayValue(BYTE *pSrce,int nWid,int nHei,int step,int* Gray)
{
	
	if(pSrce == NULL ) return FALSE;
	
	if ( nWid <= 4 || nHei <=4) return FALSE;
	
	int nWidth = nWid;
	int	nHeight = nHei;
	
	DWORD dwGraySum=0;	
	DWORD dwGrayNum=0;
	
	
	DWORD dwPos =0;
	
	for(int y=0;y<nHeight;y++)
	{
		dwPos = y*step;
		for(int x=0;x<nWidth;x++)
		{
			//blue
			dwGraySum+=(pSrce[dwPos]);
			dwGrayNum++;
			dwPos++;			
		}
	}

	//均值
	*Gray = dwGraySum/dwGrayNum;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

/*
	功能：	将以RGB格式的彩色图像转化为灰度图像
	参数：	pDst：目标图像数据区，8bit 格式
			pSrc：原始图像数据区，RGB24 格式
			nWid：图像宽
			nHei：图像高
*/

void CColorRender::Color2Gray(BYTE * pDst, BYTE * pSrc, int nWid, int nHei)
{
	unsigned int pos  = 0;
	unsigned int gray = 0;

	for (int i = 0; i < nHei * nWid; ++i)
	{
		gray = (306 * pSrc[pos+2] + 601 * pSrc[pos+1] + 117 * pSrc[pos]) >> 10;
		pDst[i] = (BYTE)gray;
		pos += 3;
	}
}

