// HV2000FC.cpp: implementation of the CHV2000FC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HV2000FC.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHV2000FC::CHV2000FC(int nNumber, int nIndex, HWND hWndNotify, CMainFrame& MainFrame) :
CDevice(nNumber, nIndex, hWndNotify, MainFrame)
{
	CreateFeatures();
}

CHV2000FC::~CHV2000FC()
{
	DestoryFeatures();
}



void CHV2000FC::CreateFeatures()
{
	assert(m_pInfo == NULL);
	m_pInfo = new CHV2000FCInfo((CDevice *)this);
	assert(m_pGain == NULL);
	m_pGain = new CHV2000FCGain(8, (CDevice *)this);
	assert(m_pShutter == NULL);
	m_pShutter = new CHV2000FCShutter(100, (CDevice *)this);
	assert(m_pSnapMode == NULL);
	m_pSnapMode = new CHV2000FCSnapMode(0, (CDevice *)this);
	assert(m_pTriggerPolarity == NULL);
	m_pTriggerPolarity = new CHV2000FCTriggerPolarity(1, (CDevice *)this);
	assert(m_pStrobePolarity == NULL);
	m_pStrobePolarity = new CHV2000FCStrobePolarity(1, (CDevice *)this);
	assert(m_pADCLevel == NULL);
	m_pADCLevel = new CHV2000FCADCLevel(ADC_LEVEL3, (CDevice *)this);
	assert(m_pBlackLevelEnable == NULL);
	m_pBlackLevelEnable = new CHV2000FCBlackLevelEnable(1, (CDevice *)this);
	assert(m_pBlackLevel == NULL);
	m_pBlackLevel = new CHV2000FCBlackLevel(-20, (CDevice *)this);
 
	assert(m_pBlank == NULL);
	m_pBlank = new CHV2000FCBlank(0, 0, (CDevice *)this);
}


void CHV2000FC::DestoryFeatures()
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
