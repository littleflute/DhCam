// SV400FM.cpp: implementation of the SV400FM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SV400FM.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSV400FM::CSV400FM(int nNumber, int nIndex, HWND hWndNotify, CMainFrame& MainFrame) :
CDevice(nNumber, nIndex, hWndNotify, MainFrame)
{
	CreateFeatures();
}


CSV400FM::~CSV400FM()
{
	DestoryFeatures();
}




void CSV400FM::CreateFeatures()
{
	assert(m_pInfo == NULL);
	m_pInfo = new CSV400FMInfo((CDevice *)this);
	assert(m_pGain == NULL);
	m_pGain = new CSV400FMGain(296, (CDevice *)this);
	assert(m_pShutter == NULL);
	m_pShutter = new CSV400FMShutter(20, (CDevice *)this);
	assert(m_pSnapMode == NULL);
	m_pSnapMode = new CSV400FMSnapMode(0, (CDevice *)this);
	assert(m_pTriggerPolarity == NULL);
	m_pTriggerPolarity = new CSV400FMTriggerPolarity(1, (CDevice *)this);
	assert(m_pStrobePolarity == NULL);
	m_pStrobePolarity = new CSV400FMStrobePolarity(1, (CDevice *)this);
	assert(m_pADCLevel == NULL);
	m_pADCLevel = new CSV400FMADCLevel(ADC_LEVEL2, (CDevice *)this);
	assert(m_pBlackLevel == NULL);
	m_pBlackLevel = new CSV400FMBlackLevel(0, (CDevice *)this);

	assert(m_pBlackLevelEnable == NULL);
	m_pBlackLevelEnable = new CSV400FMBlackLevelEnable(0, (CDevice *)this);
	assert(m_pBlank == NULL);
	m_pBlank = new CSV400FMBlank(0, 0, (CDevice *)this);

	
	
	m_PacketSize=2800;
	m_Brightness=16;
	m_BlueGain=28;
	m_RedGain=14;
}


void CSV400FM::DestoryFeatures()
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
