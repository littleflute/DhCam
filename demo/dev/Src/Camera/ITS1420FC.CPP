// ITS1420FC.cpp: implementation of the CITS1420FC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ITS1420FC.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CITS1420FC::CITS1420FC(int nNumber, int nIndex, HWND hWndNotify, CMainFrame& MainFrame) :
CDevice(nNumber, nIndex, hWndNotify, MainFrame)
{
	CreateFeatures();
}


CITS1420FC::~CITS1420FC()
{
	DestoryFeatures();
}




void CITS1420FC::CreateFeatures()
{
	assert(m_pInfo == NULL);
	m_pInfo = new CITS1420FCInfo((CDevice *)this);
	assert(m_pGain == NULL);
	m_pGain = new CITS1420FCGain(296, (CDevice *)this);
	assert(m_pShutter == NULL);
	m_pShutter = new CITS1420FCShutter(60, (CDevice *)this);
	assert(m_pSnapMode == NULL);
	m_pSnapMode = new CITS1420FCSnapMode(0, (CDevice *)this);
	assert(m_pTriggerPolarity == NULL);
	m_pTriggerPolarity = new CITS1420FCTriggerPolarity(1, (CDevice *)this);
	assert(m_pStrobePolarity == NULL);
	m_pStrobePolarity = new CITS1420FCStrobePolarity(1, (CDevice *)this);
	assert(m_pADCLevel == NULL);
	m_pADCLevel = new CITS1420FCADCLevel(ADC_LEVEL2, (CDevice *)this);
	assert(m_pBlackLevel == NULL);
	m_pBlackLevel = new CITS1420FCBlackLevel(0, (CDevice *)this);

	assert(m_pBlackLevelEnable == NULL);
	m_pBlackLevelEnable = new CITS1420FCBlackLevelEnable(0, (CDevice *)this);
	assert(m_pBlank == NULL);
	m_pBlank = new CITS1420FCBlank(0, 0, (CDevice *)this);

	
	
	m_PacketSize=2800;
	m_Brightness=16;
	m_BlueGain=28;
	m_RedGain=14;
	m_StrobeOnOff=1;  

}


void CITS1420FC::DestoryFeatures()
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
