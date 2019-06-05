// SV2000FC.cpp: implementation of the CSV2000FC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SV2000FC.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSV2000FC::CSV2000FC(int nNumber, int nIndex, HWND hWndNotify, CMainFrame& MainFrame) :
CDevice(nNumber, nIndex, hWndNotify, MainFrame)
{
	CreateFeatures();
}


CSV2000FC::~CSV2000FC()
{
	DestoryFeatures();
}




void CSV2000FC::CreateFeatures()
{
	assert(m_pInfo == NULL);
	m_pInfo = new CSV2000FCInfo((CDevice *)this);
	assert(m_pGain == NULL);
	m_pGain = new CSV2000FCGain(296, (CDevice *)this);
	assert(m_pShutter == NULL);
	m_pShutter = new CSV2000FCShutter(70, (CDevice *)this);
	assert(m_pSnapMode == NULL);
	m_pSnapMode = new CSV2000FCSnapMode(0, (CDevice *)this);
	assert(m_pTriggerPolarity == NULL);
	m_pTriggerPolarity = new CSV2000FCTriggerPolarity(1, (CDevice *)this);
	assert(m_pStrobePolarity == NULL);
	m_pStrobePolarity = new CSV2000FCStrobePolarity(1, (CDevice *)this);
	assert(m_pADCLevel == NULL);
	m_pADCLevel = new CSV2000FCADCLevel(ADC_LEVEL2, (CDevice *)this);
	assert(m_pBlackLevel == NULL);
	m_pBlackLevel = new CSV2000FCBlackLevel(0, (CDevice *)this);

	assert(m_pBlackLevelEnable == NULL);
	m_pBlackLevelEnable = new CSV2000FCBlackLevelEnable(0, (CDevice *)this);
	assert(m_pBlank == NULL);
	m_pBlank = new CSV2000FCBlank(0, 0, (CDevice *)this);

	
	
	m_PacketSize = 3600;
	m_Brightness = 16;
	m_BlueGain = 28;
	m_RedGain = 14;
	m_StrobeOnOff = 1;   
}


void CSV2000FC::DestoryFeatures()
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
