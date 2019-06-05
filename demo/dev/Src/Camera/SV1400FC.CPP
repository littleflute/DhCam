// SV1400FC.cpp: implementation of the CSV1400FC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SV1400FC.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSV1400FC::CSV1400FC(int nNumber, int nIndex, HWND hWndNotify, CMainFrame& MainFrame) :
CDevice(nNumber, nIndex, hWndNotify, MainFrame)
{
	CreateFeatures();
}


CSV1400FC::~CSV1400FC()
{
	DestoryFeatures();
}




void CSV1400FC::CreateFeatures()
{
	assert(m_pInfo == NULL);
	m_pInfo = new CSV1400FCInfo((CDevice *)this);
	assert(m_pGain == NULL);
	m_pGain = new CSV1400FCGain(296, (CDevice *)this);
	assert(m_pShutter == NULL);
	m_pShutter = new CSV1400FCShutter(120, (CDevice *)this);
	assert(m_pSnapMode == NULL);
	m_pSnapMode = new CSV1400FCSnapMode(0, (CDevice *)this);
	assert(m_pTriggerPolarity == NULL);
	m_pTriggerPolarity = new CSV1400FCTriggerPolarity(1, (CDevice *)this);
	assert(m_pStrobePolarity == NULL);
	m_pStrobePolarity = new CSV1400FCStrobePolarity(1, (CDevice *)this);
	assert(m_pADCLevel == NULL);
	m_pADCLevel = new CSV1400FCADCLevel(ADC_LEVEL2, (CDevice *)this);
	assert(m_pBlackLevel == NULL);
	m_pBlackLevel = new CSV1400FCBlackLevel(0, (CDevice *)this);

	assert(m_pBlackLevelEnable == NULL);
	m_pBlackLevelEnable = new CSV1400FCBlackLevelEnable(0, (CDevice *)this);
	assert(m_pBlank == NULL);
	m_pBlank = new CSV1400FCBlank(0, 0, (CDevice *)this);

	
	
	m_PacketSize=1400;
	m_Brightness=16;
	m_BlueGain=28;
	m_RedGain=14;
	m_StrobeOnOff=1;  

}


void CSV1400FC::DestoryFeatures()
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
