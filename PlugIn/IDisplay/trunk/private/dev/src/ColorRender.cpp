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
	���ܣ�	�õ���ƽ��ĵ���ϵ��
	������	pSource��rgb����
*/

BOOL CColorRender::PlxGetWBalanceRatio(BYTE *pSrce,int nWid,int nHei,double* dRatioR,double* dRatioG,double* dRatioB) 
{
	GetWhiteBalanceRatio(pSrce, nWid, nHei, dRatioR, dRatioG, dRatioB);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

/*
	���ܣ�	MySetGammaLut  ����GammaУ��ϵ���޸�Gamma��ֵ��
	������	dGammaRatio: GammaУ��ϵ��
			pLutGamma: ����Ĳ��ұ�
	ע�⣺	dGammaRatio��ȡֵ��ΧΪ0.1-10
*/

void CColorRender::PlxSetGammaLut(BYTE pLutGamma[256],double dGammaRatio)
{
	SetGammaLut(pLutGamma, dGammaRatio);
}

//////////////////////////////////////////////////////////////////////////

/*
	���ܣ�	����BG��ʼ��Bayer ͼ�����ݡ�ת���ɡ�RGB24 ��ʽ
	������	pDest��Ŀ��ͼ����������RGB24 ��ʽ
			pSrce��ԭʼͼ�����ݣ�RAW��ʽ
			sz:ͼ���С
			cvtype����ֵ����
			dataType:ԭʼ��������
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
	���ܣ�	�õ�����RGB��ֵ
	������	pSource��rgb����
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

	//��ɫ��ֵ
	*R = dwRedSum/dwRedNum;
	*G = dwGreenSum/dwGreenNum;
	*B = dwBlueSum/dwBlueSNum;

	return TRUE;
}

/*
	���ܣ�	�õ�����Gray��ֵ
	������	pSource��Gray����
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

	//��ֵ
	*Gray = dwGraySum/dwGrayNum;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

/*
	���ܣ�	����RGB��ʽ�Ĳ�ɫͼ��ת��Ϊ�Ҷ�ͼ��
	������	pDst��Ŀ��ͼ����������8bit ��ʽ
			pSrc��ԭʼͼ����������RGB24 ��ʽ
			nWid��ͼ���
			nHei��ͼ���
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

