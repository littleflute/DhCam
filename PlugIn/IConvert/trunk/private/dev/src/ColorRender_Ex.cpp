/************************************************************************/
/*                                                                      */
/*  Copyright (C) 2005 by DaHeng Image Vision Company.                  */
/*  All rights reserved. Property of DaHeng Image Vision Company.       */
/*  Restricted rights to use, duplicate or disclose this code are       */
/*  granted through contract.                                           */
/*                                                                      */
/*----------------------------------------------------------------------*/
/*                                                                      */
/*  Name:			ColorRender_Ex.cpp                                  */
/*  Description:	This file implement class of color render extend    */
/*  Author:         Michael Chang				                        */
/*  Date:			2005-04-08                                          */
/*                                                                      */
/************************************************************************/

#include "Stdafx.h"
#include "ColorRender_Ex.h"

//////////////////////////////////////////////////////////////////////////

//
CColorRender_Ex::CColorRender_Ex()
{
	for (int i = 0; i < LUT_LENGTH; ++i)
	{
		m_Lut[i] = i;
	}
}

//
BOOL CColorRender_Ex::ConvertBayer2Rgb_Ex(BYTE *pDest,BYTE *pSrce, int nWid, int nHei, HV_BAYER_CONVERT_TYPE cvtype,
			BYTE pLutR[LUT_LENGTH], BYTE pLutG[LUT_LENGTH], BYTE pLutB[LUT_LENGTH], bool bIsFlip, HV_BAYER_LAYOUT Layout,
			HV_ILLUMINANT light, HVTYPE camtype)
{
	if(pSrce == NULL || pDest==NULL) return FALSE;

	PlxConvertBayer2Rgb(pDest, pSrce, nWid, nHei, cvtype, m_Lut, m_Lut, m_Lut, bIsFlip, Layout);
	
	ColorCorrect(pDest, pDest, nWid * nHei, m_CorrectMatrix.GetCorrectMatrix(camtype, light),
				pLutR, pLutG, pLutB);

	return TRUE;
}

//
void CColorRender_Ex::ColorCorrect(BYTE * pSrc, BYTE * pDst, int size, double * matrix,
			BYTE pLutR[LUT_LENGTH], BYTE pLutG[LUT_LENGTH], BYTE pLutB[LUT_LENGTH])
{
	//
	int	temp[9];
	int r, g, b;
	
	for (int i = 0; i < 9; ++i)
	{
		temp[i] = (int)(matrix[i] * 1024);
	}

	//
	for (i = 0; i < size; i++)
	{
		r = temp[0] * pSrc[2] + temp[1] * pSrc[1] + temp[2] * pSrc[0];
		g = temp[3] * pSrc[2] + temp[4] * pSrc[1] + temp[5] * pSrc[0];
		b = temp[6] * pSrc[2] + temp[7] * pSrc[1] + temp[8] * pSrc[0];

		pDst[0] = pLutB[b < 0 ? 0 : (b > 255*1024 ? 255 : b >> 10)];
		pDst[1] = pLutG[g < 0 ? 0 : (g > 255*1024 ? 255 : g >> 10)];
		pDst[2] = pLutR[r < 0 ? 0 : (r > 255*1024 ? 255 : r >> 10)];

		pSrc += 3;
		pDst += 3;
	}
}

