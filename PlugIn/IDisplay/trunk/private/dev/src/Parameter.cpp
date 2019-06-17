/************************************************************************/
/*                                                                      */
/*  Copyright (C) 2005 by DaHeng Image Vision Company.                  */
/*  All rights reserved. Property of DaHeng Image Vision Company.       */
/*  Restricted rights to use, duplicate or disclose this code are       */
/*  granted through contract.                                           */
/*                                                                      */
/*----------------------------------------------------------------------*/
/*                                                                      */
/*  Name:			Parameter.cpp                                       */
/*  Description:	This file implement the parameter class             */
/*  Author:         Michael Chang				                        */
/*  Date:			2005-04-20                                          */
/*                                                                      */
/************************************************************************/

#include "stdafx.h"
#include "Parameter.h"
#include "ColorRender.h"
#include "Exception.h"
#include "..\inc\HVDAILT.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction

CParameter::CParameter(HVTYPE CameraType)
{
	// Set initial value
	m_ProcessType = COLOR;
	m_BayerLayout = BAYER_GR;
	m_ConvertType = BAYER2RGB_NEIGHBOUR1;
	m_Illuminant  = DAYLIGHT;
	
	m_RatioGama  = 1.0;
	m_RatioRed   = 1.0;
	m_RatioBlue  = 1.0;
	m_bAutoLight = FALSE;
	m_bCorrect   = FALSE;
	m_bGray      = FALSE;
    m_bDoColor   = TRUE;	

	UpdateLutGama();
}

CParameter::~CParameter()
{
}

//////////////////////////////////////////////////////////////////////////
// Implement

void CParameter::ReadParam(CString keyPath)
{
	try
	{
		CPropertyBagPtr ptrBag = CRegistryPropertyBag::Open(keyPath);
		m_bAutoLight  = ptrBag->ReadBool("AutoLight");
		m_bCorrect    = ptrBag->ReadBool("Correct");
		m_bGray       = ptrBag->ReadBool("Gray");
        m_bDoColor    = ptrBag->ReadBool("bDoColor");
		m_RatioRed    = ptrBag->ReadFloat("RatioRed");
		m_RatioBlue   = ptrBag->ReadFloat("RatioBlue");
		m_RatioGama   = ptrBag->ReadFloat("RatioGama");	
		m_ProcessType = (PROCESS_TYPE)ptrBag->ReadLong("ProcessType");
		m_BayerLayout = (HV_BAYER_LAYOUT)ptrBag->ReadLong("BayerLayout");
		m_ConvertType = (HV_BAYER_CONVERT_TYPE)ptrBag->ReadLong("ConvertType");
		m_Illuminant  = (HV_ILLUMINANT)ptrBag->ReadLong("Illuminant");
		m_info.ImageBitDepth = ptrBag->ReadLong("Depth");
		UpdateLutGama();
	}

	catch (CPropertyBagException) {
	}

}

void CParameter::SaveParam(CString keyPath)
{
	try
	{
		CPropertyBagPtr ptrBag = CRegistryPropertyBag::Create(keyPath);
		ptrBag->WriteLong("ProcessType", m_ProcessType);
		ptrBag->WriteLong("BayerLayout", m_BayerLayout);
		ptrBag->WriteLong("ConvertType", m_ConvertType);
		ptrBag->WriteLong("Illuminant",  m_Illuminant);
		ptrBag->WriteBool("AutoLight",   m_bAutoLight);
		ptrBag->WriteBool("Correct",     m_bCorrect);
		ptrBag->WriteBool("Gray",        m_bGray);
        ptrBag->WriteBool("bDoColor",    m_bDoColor);
		ptrBag->WriteFloat("RatioRed",  (float)m_RatioRed);
		ptrBag->WriteFloat("RatioBlue", (float)m_RatioBlue);
		ptrBag->WriteFloat("RatioGama", (float)m_RatioGama);
		ptrBag->WriteLong("Depth", m_info.ImageBitDepth);		
	}
	catch (CPropertyBagException) {
	}

}

CString CParameter::GetString()
{
	switch(m_CameraType)
	{
	case HV1300UCTYPE:
	case HV1301UCTYPE:
		return CString("HV1300UC");

	case HV2000UCTYPE:
	case HV2001UCTYPE:
		return CString("HV2000UC");

	case HV3000UCTYPE:
		return CString("HV3000UC");

	default:
		return CString("Unknown");
	}
}

//////////////////////////////////////////////////////////////////////////
// Operation

void CParameter::UpdateLutRed()
{
	for (int i = 0; i < 256; ++i)
	{
		m_LutR[i] = (BYTE)min(255, m_LutG[i] * m_RatioRed);
	}
}

void CParameter::UpdateLutBlue()
{
	for (int i = 0; i < 256; ++i)
	{
		m_LutB[i] = (BYTE)min(255, m_LutG[i] * m_RatioBlue);
	}
}

void CParameter::UpdateLutGama()
{
	for (int i = 0; i < 256; ++i)
	{
		m_LutG[i] = i;
	}

	CColorRender::PlxSetGammaLut(m_LutG, m_RatioGama);
	UpdateLutRed();
	UpdateLutBlue();
}

