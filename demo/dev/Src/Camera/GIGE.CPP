// GIGE.cpp: implementation of the CGIGE class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GIGE.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGIGE::CGIGE(int nNumber, int nIndex, HWND hWndNotify, CMainFrame& MainFrame) :
CDevice(nNumber, nIndex, hWndNotify, MainFrame)
{
	CreateFeatures();
}


CGIGE::~CGIGE()
{
	DestoryFeatures();
}




void CGIGE::CreateFeatures()
{
	assert(m_pInfo == NULL);
	m_pInfo = new CGIGEInfo((CDevice *)this);
	assert(m_pGain == NULL);
	m_pGain = new CGIGEGain(296, (CDevice *)this);
	assert(m_pShutter == NULL);
	m_pShutter = new CGIGEShutter(60, (CDevice *)this);
	assert(m_pSnapMode == NULL);
	m_pSnapMode = new CGIGESnapMode(0, (CDevice *)this);
	assert(m_pTriggerPolarity == NULL);
	m_pTriggerPolarity = new CGIGETriggerPolarity(1, (CDevice *)this);
	assert(m_pStrobePolarity == NULL);
	m_pStrobePolarity = new CGIGEStrobePolarity(1, (CDevice *)this);
	assert(m_pADCLevel == NULL);
	m_pADCLevel = new CGIGEADCLevel(ADC_LEVEL2, (CDevice *)this);
	assert(m_pBlackLevel == NULL);
	m_pBlackLevel = new CGIGEBlackLevel(0, (CDevice *)this);

	assert(m_pBlackLevelEnable == NULL);
	m_pBlackLevelEnable = new CGIGEBlackLevelEnable(0, (CDevice *)this);
	assert(m_pBlank == NULL);
	m_pBlank = new CGIGEBlank(0, 0, (CDevice *)this);

	
	
	m_PacketSize = 2800;
	m_Brightness = 16;
	m_BlueGain = 60;
	m_RedGain = 46;
	m_StrobeOnOff = 1;  

}


void CGIGE::DestoryFeatures()
{
	assert(m_pInfo);
	delete m_pInfo;
	assert(m_pGain);
	delete m_pGain;
	assert(m_pShutter);
	delete m_pShutter;
	assert(m_pSnapMode);
	delete m_pSnapMode;
	assert(m_pTriggerPolarity);
	delete m_pTriggerPolarity;
	assert(m_pStrobePolarity);
	delete m_pStrobePolarity;
	assert(m_pADCLevel);
	delete m_pADCLevel;
	assert(m_pBlackLevel);
	delete m_pBlackLevel;

	assert(m_pBlackLevelEnable);
	delete m_pBlackLevelEnable;
	assert(m_pBlank);
	delete m_pBlank;
}
