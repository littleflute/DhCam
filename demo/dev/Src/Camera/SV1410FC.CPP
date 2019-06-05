// SV1410FC.cpp: implementation of the CSV1410FC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SV1410FC.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSV1410FC::CSV1410FC(int nNumber, int nIndex, HWND hWndNotify, CMainFrame& MainFrame) :
CDevice(nNumber, nIndex, hWndNotify, MainFrame)
{
	CreateFeatures();
}


CSV1410FC::~CSV1410FC()
{
	DestoryFeatures();
}




void CSV1410FC::CreateFeatures()
{
	assert(m_pInfo == NULL);
	m_pInfo = new CSV1410FCInfo((CDevice *)this);
	assert(m_pGain == NULL);
	m_pGain = new CSV1410FCGain(296, (CDevice *)this);
	assert(m_pShutter == NULL);
	m_pShutter = new CSV1410FCShutter(60, (CDevice *)this);
	assert(m_pSnapMode == NULL);
	m_pSnapMode = new CSV1410FCSnapMode(0, (CDevice *)this);
	assert(m_pTriggerPolarity == NULL);
	m_pTriggerPolarity = new CSV1410FCTriggerPolarity(1, (CDevice *)this);
	assert(m_pStrobePolarity == NULL);
	m_pStrobePolarity = new CSV1410FCStrobePolarity(1, (CDevice *)this);
	assert(m_pADCLevel == NULL);
	m_pADCLevel = new CSV1410FCADCLevel(ADC_LEVEL2, (CDevice *)this);
	assert(m_pBlackLevel == NULL);
	m_pBlackLevel = new CSV1410FCBlackLevel(0, (CDevice *)this);

	assert(m_pBlackLevelEnable == NULL);
	m_pBlackLevelEnable = new CSV1410FCBlackLevelEnable(0, (CDevice *)this);
	assert(m_pBlank == NULL);
	m_pBlank = new CSV1410FCBlank(0, 0, (CDevice *)this);

	
	
	m_PacketSize=2800;
	m_Brightness=16;
	m_BlueGain=28;
	m_RedGain=14;
	m_StrobeOnOff=1;  

}


void CSV1410FC::DestoryFeatures()
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
