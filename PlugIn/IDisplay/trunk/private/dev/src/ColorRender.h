/************************************************************************/
/*                                                                      */
/*  Copyright (C) 2005 by DaHeng Image Vision Company.                  */
/*  All rights reserved. Property of DaHeng Image Vision Company.       */
/*  Restricted rights to use, duplicate or disclose this code are       */
/*  granted through contract.                                           */
/*                                                                      */
/*----------------------------------------------------------------------*/
/*                                                                      */
/*  Name:			ColorRender.h                                       */
/*  Description:	This file defines the class of color render         */
/*  Author:         Michael Chang				                        */
/*  Date:			2005-04-08                                          */
/*                                                                      */
/************************************************************************/

#ifndef _COLORRENDER_H
#define _COLORRENDER_H

//////////////////////////////////////////////////////////////////////////

#include "..\inc\Raw2Rgb.h"

//////////////////////////////////////////////////////////////////////////

#define LUT_LENGTH				256
#define MAX_WIDTH				2400
#define MAX_HEIGHT				1800
#define AVG_ZERO				0.000000001


typedef enum COLOR_CHANNEL		//ÑÕÉ«Í¨µÀ
{
	R_CHANNEL = 0,
	G_CHANNEL = 1,
	B_CHANNEL = 2
} COLOR_CHANNEL;

//////////////////////////////////////////////////////////////////////////

class CColorRender
{
public:
	//
	BOOL PlxConvertBayer2Rgb(BYTE *pDest,BYTE *pSrce, int nWid, int nHei, HV_BAYER_CONVERT_TYPE cvtype,
					BYTE pLutR[LUT_LENGTH], BYTE pLutG[LUT_LENGTH], BYTE pLutB[LUT_LENGTH], bool bIsFlip, HV_BAYER_LAYOUT Layout);
	
	//
	static void PlxSetGammaLut(BYTE pLutGamma[256],double dGammaRatio);

	//
	static BOOL PlxGetWBalanceRatio(BYTE *pSrce,int nWid,int nHei,double* dRatioR,double* dRatioG,double* dRatioB);	

	//
	static BOOL GetRectRGBValue(BYTE *pSrce,int nWid,int nHei,int step,int* R,int* G,int* B);

	//
	static BOOL GetRectGrayValue(BYTE *pSrce,int nWid,int nHei,int step,int* Gray);

	//
	static void Color2Gray(BYTE * pDst, BYTE * pSrc, int nWid, int nHei);
};
	
#endif