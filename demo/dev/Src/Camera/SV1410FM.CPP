// SV1410FM.cpp: implementation of the CSV1410FM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SV1410FM.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSV1410FM::CSV1410FM(int nNumber, int nIndex, HWND hWndNotify, CMainFrame& MainFrame) :
CDevice(nNumber, nIndex, hWndNotify, MainFrame)
{
	CreateFeatures();
}


CSV1410FM::~CSV1410FM()
{
	DestoryFeatures();
}




void CSV1410FM::CreateFeatures()
{
	assert(m_pInfo == NULL);
	m_pInfo = new CSV1410FMInfo((CDevice *)this);
	assert(m_pGain == NULL);
	m_pGain = new CSV1410FMGain(296, (CDevice *)this);
	assert(m_pShutter == NULL);
	m_pShutter = new CSV1410FMShutter(60, (CDevice *)this);
	assert(m_pSnapMode == NULL);
	m_pSnapMode = new CSV1410FMSnapMode(0, (CDevice *)this);
	assert(m_pTriggerPolarity == NULL);
	m_pTriggerPolarity = new CSV1410FMTriggerPolarity(1, (CDevice *)this);
	assert(m_pStrobePolarity == NULL);
	m_pStrobePolarity = new CSV1410FMStrobePolarity(1, (CDevice *)this);
	assert(m_pADCLevel == NULL);
	m_pADCLevel = new CSV1410FMADCLevel(ADC_LEVEL2, (CDevice *)this);
	assert(m_pBlackLevel == NULL);
	m_pBlackLevel = new CSV1410FMBlackLevel(0, (CDevice *)this);

	assert(m_pBlackLevelEnable == NULL);
	m_pBlackLevelEnable = new CSV1410FMBlackLevelEnable(0, (CDevice *)this);
	assert(m_pBlank == NULL);
	m_pBlank = new CSV1410FMBlank(0, 0, (CDevice *)this);

	
	
	m_PacketSize=2800;
	m_Brightness=16;
    m_StrobeOnOff=1;  
}


void CSV1410FM::DestoryFeatures()
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
