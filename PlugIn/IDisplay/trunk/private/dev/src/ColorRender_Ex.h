/************************************************************************/
/*                                                                      */
/*  Copyright (C) 2005 by DaHeng Image Vision Company.                  */
/*  All rights reserved. Property of DaHeng Image Vision Company.       */
/*  Restricted rights to use, duplicate or disclose this code are       */
/*  granted through contract.                                           */
/*                                                                      */
/*----------------------------------------------------------------------*/
/*                                                                      */
/*  Name:			ColorRender_Ex.h                                    */
/*  Description:	This file defines the class of color render extend  */
/*  Author:         Michael Chang				                        */
/*  Date:			2005-04-08                                          */
/*                                                                      */
/************************************************************************/

#ifndef _COLORRENDER_EX_H
#define _COLORRENDER_EX_H

//////////////////////////////////////////////////////////////////////////

#include "..\inc\Raw2Rgb.h"
#include "ColorRender.h"
#include "CorrectMatrix.h"

//////////////////////////////////////////////////////////////////////////

class CColorRender_Ex : public CColorRender
{
protected:
	CCorrectMatrix m_CorrectMatrix;
	BYTE           m_Lut[LUT_LENGTH];

public:
	CColorRender_Ex();

public:		 
	BOOL ConvertBayer2Rgb_Ex(BYTE *pDest,BYTE *pSrce, int nWid, int nHei, HV_BAYER_CONVERT_TYPE cvtype,
			BYTE pLutR[LUT_LENGTH], BYTE pLutG[LUT_LENGTH], BYTE pLutB[LUT_LENGTH], bool bIsFlip, HV_BAYER_LAYOUT Layout,
			HV_ILLUMINANT light, HVTYPE camtype);
	void ColorCorrect(BYTE * pSrc, BYTE * pDst, int size, double * matrix, BYTE pLutR[LUT_LENGTH], 
			BYTE pLutG[LUT_LENGTH], BYTE pLutB[LUT_LENGTH]);
};

#endif