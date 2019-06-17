/************************************************************************/
/*                                                                      */
/*  Copyright (C) 2005 by DaHeng Image Vision Company.                  */
/*  All rights reserved. Property of DaHeng Image Vision Company.       */
/*  Restricted rights to use, duplicate or disclose this code are       */
/*  granted through contract.                                           */
/*                                                                      */
/*----------------------------------------------------------------------*/
/*                                                                      */
/*  Name:			Parameter.h                                         */
/*  Description:	This file defines the parameter class               */
/*  Author:         Michael Chang				                        */
/*  Date:			2005-04-20                                          */
/*                                                                      */
/************************************************************************/

#ifndef _PARAMETER_H
#define _PARAMETER_H

//////////////////////////////////////////////////////////////////////////

#include "..\inc\HVDef.h"
#include "..\inc\Raw2Rgb.h"
#include "PluginModule.h"
#include "PropertyBag.h"


//////////////////////////////////////////////////////////////////////////

class CParameter  
{
public:
	CParameter(HVTYPE CameraType = HV1300UCTYPE);
	virtual ~CParameter();
	
public:
	typedef enum tagPROCESS_TYPE
	{
		ORIGIN = 0,
		COLOR  = 1,
		GRAY   = 2
	}PROCESS_TYPE;

public:
	HVTYPE                m_CameraType;
	HV_BAYER_CONVERT_TYPE m_ConvertType;
	HV_BAYER_LAYOUT       m_BayerLayout;
	HV_ILLUMINANT         m_Illuminant;

	PROCESS_TYPE          m_ProcessType;
	BOOL                  m_bAutoLight;

    //
    // [xuyibo] new added
    //
	CImageInfo			  m_info;
	CImageData			  m_data;
    BOOL                  m_bDoColor;
    //

	BOOL                  m_bCorrect;
	BOOL                  m_bGray;
	double                m_RatioRed;
	double                m_RatioBlue;
	double                m_RatioGama;

	BYTE                  m_LutR[256];
	BYTE                  m_LutG[256];
	BYTE                  m_LutB[256];

public:
	void UpdateLutRed();
	void UpdateLutBlue();
	void UpdateLutGama();
	void ReadParam(CString);
	void SaveParam(CString);
	CString GetString();
};

#endif
