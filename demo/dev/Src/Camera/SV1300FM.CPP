// SV1300FC.cpp: implementation of the CSV1300FM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SV1300FM.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSV1300FM::CSV1300FM(int nNumber, int nIndex, HWND hWndNotify, CMainFrame& MainFrame) :
CDevice(nNumber, nIndex, hWndNotify, MainFrame)
{
	CreateFeatures();
}


CSV1300FM::~CSV1300FM()
{
	DestoryFeatures();
}




void CSV1300FM::CreateFeatures()
{
	assert(m_pInfo == NULL);
	m_pInfo = new CSV1300FMInfo((CDevice *)this);
	assert(m_pGain == NULL);
	m_pGain = new CSV1300FMGain(296, (CDevice *)this);
	assert(m_pShutter == NULL);
	m_pShutter = new CSV1300FMShutter(133, (CDevice *)this);
	assert(m_pSnapMode == NULL);
	m_pSnapMode = new CSV1300FMSnapMode(0, (CDevice *)this);
	assert(m_pTriggerPolarity == NULL);
	m_pTriggerPolarity = new CSV1300FMTriggerPolarity(1, (CDevice *)this);
	assert(m_pStrobePolarity == NULL);
	m_pStrobePolarity = new CSV1300FMStrobePolarity(1, (CDevice *)this);
	assert(m_pADCLevel == NULL);
	m_pADCLevel = new CSV1300FMADCLevel(ADC_LEVEL2, (CDevice *)this);
	assert(m_pBlackLevel == NULL);
	m_pBlackLevel = new CSV1300FMBlackLevel(0, (CDevice *)this);

	assert(m_pBlackLevelEnable == NULL);
	m_pBlackLevelEnable = new CSV1300FMBlackLevelEnable(0, (CDevice *)this);
	assert(m_pBlank == NULL);
	m_pBlank = new CSV1300FMBlank(0, 0, (CDevice *)this);

	
	m_PacketSize=1600;
	m_Brightness=16;
}


void CSV1300FM::DestoryFeatures()
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
