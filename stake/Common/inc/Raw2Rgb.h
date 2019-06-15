/************************************************************************/
/*                                                                      */
/*  Copyright (C) 2007 by DaHeng Image Vision Company.                  */
/*  All rights reserved. Property of DaHeng Image Vision Company.       */
/*  Restricted rights to use, duplicate or disclose this code are       */
/*  granted through contract.                                           */
/*                                                                      */
/*----------------------------------------------------------------------*/
/*                                                                      */
/*  Name:               Raw2Rgb.h                                           */
/*  Description:        This file defines the interface of color library    */
/*  Author:             Dh-Image
/*  Version:            1.3.0.0
/*  Date:               2007-11-07                                          */
/*                                                                      */
/************************************************************************/


#ifndef __RAW2RGB_H
#define __RAW2RGB_H

//////////////////////////////////////////////////////////////////////////

#include "HVDef.h"

//////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif
   	

typedef enum tagHV_BAYER_CONVERT_TYPE
{
	BAYER2RGB_ZHC,  
	BAYER2RGB_NEIGHBOUR,  
	BAYER2RGB_BILINER,    
	BAYER2RGB_PATTERN,       
	BAYER2RGB_EDGESENSING,  
	BAYER2RGB_RF,          
	BAYER2RGB_PIXELGROUPING,
	BAYER2RGB_NEIGHBOUR1,	
	BAYER2RGB_RESERVERED1,	 
	BAYER2RGB_RESERVERED2,	 
	BAYER2RGB_RESERVERED3	
	
}HV_BAYER_CONVERT_TYPE;


typedef enum tagHV_BAYER_LAYOUT
{
	BAYER_GB = 1,
	BAYER_GR = 2,
	BAYER_BG = 3,
	BAYER_RG = 4	
}HV_BAYER_LAYOUT;


typedef enum tagHV_ILLUMINATE
{
	DAYLIGHT,
	FLUORESCENT,
	INCANDESCENT,
}HV_ILLUMINANT;

//////////////////////////////////////////////////////////////////////////


void __stdcall ConvertBayer2Rgb(BYTE *pDest,BYTE *pSrce, int nWid,int nHei,HV_BAYER_CONVERT_TYPE cvtype,BYTE* pLutR,BYTE* pLutG, BYTE* pLutB,bool bFlip,HV_BAYER_LAYOUT Layout);
void __stdcall GetWhiteBalanceRatio(BYTE *pSrce,int nWid,int nHei,double* dRatioR,double* dRatioG,double* dRatioB);
void __stdcall SetGammaLut(BYTE pLutGamma[256],double dGammaRatio);
void __stdcall DetectDeadPixel(BYTE * pRawImgBuf,BYTE * pBadPixelPosBuf,int nImgWid,int nImgHei);
void __stdcall EraseDeadPixel(BYTE * pRawImgBuf,BYTE * pBadPixelPosBuf,int nXPos,int nYPos,int nImgWid,int nImgHei,int nBadPixelBufWid,int nBadPixelBufHei);
void __stdcall GetPatternNoise(BYTE * pRawImgBuf,int * pPatternBuf,int nPatternWid,int nPatternHei);
void __stdcall AvgPatternNoise(int * pPatternBuf,int nPatternWid,int nPatternHei,int nAvgTimes);
void __stdcall FixPatternNoise(BYTE * pRawImgBuf,int * pPatternBuf,int nXPos,int nYPos,int nImgWid,int nImgHei,int nPatternWid,int nPatternHei);

//////////////////////////////////////////////////////////////////////////


#ifdef __cplusplus
}
#endif

#endif


