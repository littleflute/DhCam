/************************************************************************/
/*                                                                      */
/*  Copyright (C) 2005 by DaHeng Image Vision Company.                  */
/*  All rights reserved. Property of DaHeng Image Vision Company.       */
/*  Restricted rights to use, duplicate or disclose this code are       */
/*  granted through contract.                                           */
/*                                                                      */
/*----------------------------------------------------------------------*/
/*                                                                      */
/*  Name:			CorrectMatrix.cpp                                   */
/*  Description:	This file implements the class of correct matrix    */
/*  Author:         Michael Chang				                        */
/*  Date:			2005-04-08                                          */
/*                                                                      */
/************************************************************************/

#include "Stdafx.h"
#include "CorrectMatrix.h"

//////////////////////////////////////////////////////////////////////////

double CCorrectMatrix::HV1300_6500K[9] = {
1.26901,  0.131059,  -0.118839,
-0.189833,  1.01478,  0.0308023,
-0.0796099,  -0.145937,  1.08817};

double CCorrectMatrix::HV1300_4000K[9] = {
1.03458,  0.12766,  -0.245212,
-0.116042,  1.01576,  -0.0686014,
0.0812626,  -0.143654,  1.31402};

double CCorrectMatrix::HV1300_2700K[9] = {
1.02764,  0.181739,  -0.806932,
-0.0687496,  1.15791,  -0.231579,
0.0406341,  -0.339703,  2.03916	};

double CCorrectMatrix::HV2000_6500K[9] = {
1.34068,  0.0791926,  -0.217606,
-0.274675,  1.21576,  -0.113774,
-0.0665264,  -0.294965,  1.33154};

double CCorrectMatrix::HV2000_4000K[9] = {
1.2249,  0.0833157,  -0.44144,
-0.198066,  1.24523,  -0.223702,
-0.0274982,  -0.328358,  1.6654};

double CCorrectMatrix::HV2000_2700K[9] = {
1.21192,  0.194012,  -1.12349,
-0.17837,  1.56788,  -0.672326,
-0.0341723,  -0.761879,  2.79661};

double CCorrectMatrix::HV3000_6500K[9] = {
1.50965,  -0.0244778,  -0.222598,
-0.381194,  1.39865,  -0.189214,
-0.129007,  -0.374201,  1.412};

double CCorrectMatrix::HV3000_4000K[9] = {
1.36623,  0.0140359,  -0.476867,
-0.27891,  1.43656,  -0.318029,
-0.0880923,  -0.45034,  1.79518};

double CCorrectMatrix::HV3000_2700K[9] = {
1.36504,  0.140637,  -1.11321,
-0.264625,  1.84297,  -0.847334,
-0.101138,  -0.983604,  2.96142};

//////////////////////////////////////////////////////////////////////////

CCorrectMatrix::CCorrectMatrix()
{
	m_Matrix[HV1300UCTYPE][DAYLIGHT]     = HV1300_6500K;
	m_Matrix[HV2000UCTYPE][DAYLIGHT]     = HV2000_6500K;
	m_Matrix[HV1301UCTYPE][DAYLIGHT]     = HV1300_6500K;
	m_Matrix[HV2001UCTYPE][DAYLIGHT]     = HV2000_6500K;
	m_Matrix[HV3000UCTYPE][DAYLIGHT]     = HV3000_6500K;
	m_Matrix[HV1300UMTYPE][DAYLIGHT]     = HV1300_6500K;
	m_Matrix[HV1302UCTYPE][DAYLIGHT]     = HV1300_6500K;
	m_Matrix[HV2002UCTYPE][DAYLIGHT]     = HV2000_6500K;
	m_Matrix[HV3102UCTYPE][DAYLIGHT]     = HV3000_6500K;

	m_Matrix[HV1300UCTYPE][FLUORESCENT]  = HV1300_4000K;
	m_Matrix[HV2000UCTYPE][FLUORESCENT]  = HV2000_4000K;
	m_Matrix[HV1301UCTYPE][FLUORESCENT]  = HV1300_4000K;
	m_Matrix[HV2001UCTYPE][FLUORESCENT]  = HV2000_4000K;
	m_Matrix[HV3000UCTYPE][FLUORESCENT]  = HV3000_4000K;
	m_Matrix[HV1300UMTYPE][FLUORESCENT]  = HV1300_4000K;
	m_Matrix[HV1302UCTYPE][FLUORESCENT]  = HV1300_4000K;
	m_Matrix[HV2002UCTYPE][FLUORESCENT]  = HV2000_4000K;
	m_Matrix[HV3102UCTYPE][FLUORESCENT]  = HV3000_4000K;

	m_Matrix[HV1300UCTYPE][INCANDESCENT] = HV1300_2700K;
	m_Matrix[HV2000UCTYPE][INCANDESCENT] = HV2000_2700K;
	m_Matrix[HV1301UCTYPE][INCANDESCENT] = HV1300_2700K;
	m_Matrix[HV2001UCTYPE][INCANDESCENT] = HV2000_2700K;
	m_Matrix[HV3000UCTYPE][INCANDESCENT] = HV3000_2700K;
	m_Matrix[HV1300UMTYPE][INCANDESCENT] = HV1300_2700K;
	m_Matrix[HV1302UCTYPE][INCANDESCENT] = HV1300_2700K;
	m_Matrix[HV2002UCTYPE][INCANDESCENT] = HV2000_2700K;
	m_Matrix[HV3102UCTYPE][INCANDESCENT] = HV3000_2700K;
}

double * CCorrectMatrix::GetCorrectMatrix(HVTYPE camera, HV_ILLUMINANT light)
{
	if (camera >= SUPPORT_CAMERA)
		camera = HV1300UCTYPE;

	if (light >= SUPPORT_ILLUMINANT)
		light = DAYLIGHT;

	return m_Matrix[camera][light];
}