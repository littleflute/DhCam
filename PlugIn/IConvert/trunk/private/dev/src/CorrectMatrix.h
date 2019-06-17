/************************************************************************/
/*                                                                      */
/*  Copyright (C) 2005 by DaHeng Image Vision Company.                  */
/*  All rights reserved. Property of DaHeng Image Vision Company.       */
/*  Restricted rights to use, duplicate or disclose this code are       */
/*  granted through contract.                                           */
/*                                                                      */
/*----------------------------------------------------------------------*/
/*                                                                      */
/*  Name:			CorrectMatrix.h                                     */
/*  Description:	This file defines the class of correct matrix       */
/*  Author:         Michael Chang				                        */
/*  Date:			2005-04-08                                          */
/*                                                                      */
/************************************************************************/

#ifndef _CORRECTMATRIX_H
#define _CORRECTMATRIX_H

//////////////////////////////////////////////////////////////////////////
#include "..\inc\HVDef.h"
#include "..\inc\Raw2Rgb.h"

//////////////////////////////////////////////////////////////////////////

#define SUPPORT_CAMERA			HV3102UCTYPE + 1
#define SUPPORT_ILLUMINANT		INCANDESCENT + 1		

//////////////////////////////////////////////////////////////////////////

class CCorrectMatrix
{
public:
	double * m_Matrix[SUPPORT_CAMERA][SUPPORT_ILLUMINANT];

protected:
	static double HV1300_6500K[9];
	static double HV2000_6500K[9];
	static double HV3000_6500K[9];
	static double HV1300_4000K[9];
	static double HV2000_4000K[9];
	static double HV3000_4000K[9];
	static double HV1300_2700K[9];
	static double HV2000_2700K[9];
	static double HV3000_2700K[9];

public:
	CCorrectMatrix();

public:
	double * GetCorrectMatrix(HVTYPE camera, HV_ILLUMINANT light);
};

#endif