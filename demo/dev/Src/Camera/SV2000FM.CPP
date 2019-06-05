// SV2000FM.cpp: implementation of the CSV2000FM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SV2000FM.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSV2000FM::CSV2000FM(int nNumber, int nIndex, HWND hWndNotify, CMainFrame& MainFrame) :
CDevice(nNumber, nIndex, hWndNotify, MainFrame)
{
	CreateFeatures();
}


CSV2000FM::~CSV2000FM()
{
	DestoryFeatures();
}




void CSV2000FM::CreateFeatures()
{
	assert(m_pInfo == NULL);
	m_pInfo = new CSV2000FMInfo((CDevice *)this);
	assert(m_pGain == NULL);
	m_pGain = new CSV2000FMGain(296, (CDevice *)this);
	assert(m_pShutter == NULL);
	m_pShutter = new CSV2000FMShutter(70, (CDevice *)this);
	assert(m_pSnapMode == NULL);
	m_pSnapMode = new CSV2000FMSnapMode(0, (CDevice *)this);
	assert(m_pTriggerPolarity == NULL);
	m_pTriggerPolarity = new CSV2000FMTriggerPolarity(1, (CDevice *)this);
	assert(m_pStrobePolarity == NULL);
	m_pStrobePolarity = new CSV2000FMStrobePolarity(1, (CDevice *)this);
	assert(m_pADCLevel == NULL);
	m_pADCLevel = new CSV2000FMADCLevel(ADC_LEVEL2, (CDevice *)this);
	assert(m_pBlackLevel == NULL);
	m_pBlackLevel = new CSV2000FMBlackLevel(0, (CDevice *)this);

	assert(m_pBlackLevelEnable == NULL);
	m_pBlackLevelEnable = new CSV2000FMBlackLevelEnable(0, (CDevice *)this);
	assert(m_pBlank == NULL);
	m_pBlank = new CSV2000FMBlank(0, 0, (CDevice *)this);

	
	
	m_PacketSize = 3600;
	m_Brightness = 16;
	m_BlueGain = 28;
	m_RedGain = 14;
	m_StrobeOnOff = 1;   
}


void CSV2000FM::DestoryFeatures()
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
