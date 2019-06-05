// HV3100FC.cpp: implementation of the CHV3100FC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HV3100FC.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHV3100FC::CHV3100FC(int nNumber, int nIndex, HWND hWndNotify, CMainFrame& MainFrame) :
CDevice(nNumber, nIndex, hWndNotify, MainFrame)
{
	CreateFeatures();
}


CHV3100FC::~CHV3100FC()
{
	DestoryFeatures();
}




void CHV3100FC::CreateFeatures()
{
	assert(m_pInfo == NULL);
	m_pInfo = new CHV3100FCInfo((CDevice *)this);
	assert(m_pGain == NULL);
	m_pGain = new CHV3100FCGain(8, (CDevice *)this);
	assert(m_pShutter == NULL);
	m_pShutter = new CHV3100FCShutter(152, (CDevice *)this);
	assert(m_pSnapMode == NULL);
	m_pSnapMode = new CHV3100FCSnapMode(0, (CDevice *)this);
	assert(m_pTriggerPolarity == NULL);
	m_pTriggerPolarity = new CHV3100FCTriggerPolarity(1, (CDevice *)this);
	assert(m_pStrobePolarity == NULL);
	m_pStrobePolarity = new CHV3100FCStrobePolarity(1, (CDevice *)this);
	assert(m_pADCLevel == NULL);
	m_pADCLevel = new CHV3100FCADCLevel(ADC_LEVEL3, (CDevice *)this);
	assert(m_pBlackLevelEnable == NULL);
	m_pBlackLevelEnable = new CHV3100FCBlackLevelEnable(1, (CDevice *)this);
	assert(m_pBlackLevel == NULL);
	m_pBlackLevel = new CHV3100FCBlackLevel(-20, (CDevice *)this);

	assert(m_pBlank == NULL);
	m_pBlank = new CHV3100FCBlank(0, 0, (CDevice *)this);
}


void CHV3100FC::DestoryFeatures()
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
