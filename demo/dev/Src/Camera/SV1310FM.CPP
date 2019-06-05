// SV1310FM.cpp: implementation of the CSV1310FM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SV1310FM.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSV1310FM::CSV1310FM(int nNumber, int nIndex, HWND hWndNotify, CMainFrame& MainFrame) :
CDevice(nNumber, nIndex, hWndNotify, MainFrame)
{
	CreateFeatures();
}


CSV1310FM::~CSV1310FM()
{
	DestoryFeatures();
}




void CSV1310FM::CreateFeatures()
{
	assert(m_pInfo == NULL);
	m_pInfo = new CSV1310FMInfo((CDevice *)this);
	assert(m_pGain == NULL);
	m_pGain = new CSV1310FMGain(296, (CDevice *)this);
	assert(m_pShutter == NULL);
	m_pShutter = new CSV1310FMShutter(60, (CDevice *)this);
	assert(m_pSnapMode == NULL);
	m_pSnapMode = new CSV1310FMSnapMode(0, (CDevice *)this);
	assert(m_pTriggerPolarity == NULL);
	m_pTriggerPolarity = new CSV1310FMTriggerPolarity(1, (CDevice *)this);
	assert(m_pStrobePolarity == NULL);
	m_pStrobePolarity = new CSV1310FMStrobePolarity(1, (CDevice *)this);
	assert(m_pADCLevel == NULL);
	m_pADCLevel = new CSV1310FMADCLevel(ADC_LEVEL2, (CDevice *)this);
	assert(m_pBlackLevel == NULL);
	m_pBlackLevel = new CSV1310FMBlackLevel(0, (CDevice *)this);

	assert(m_pBlackLevelEnable == NULL);
	m_pBlackLevelEnable = new CSV1310FMBlackLevelEnable(0, (CDevice *)this);
	assert(m_pBlank == NULL);
	m_pBlank = new CSV1310FMBlank(0, 0, (CDevice *)this);

	
	m_PacketSize=2500;
	m_Brightness=16;
}


void CSV1310FM::DestoryFeatures()
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
