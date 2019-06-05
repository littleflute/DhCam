// HV1300FC.cpp: implementation of the CHV1300FC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HV1300FC.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHV1300FC::CHV1300FC(int nNumber, int nIndex, HWND hWndNotify, CMainFrame& MainFrame) :
CDevice(nNumber, nIndex, hWndNotify, MainFrame)
{
	CreateFeatures();
}


CHV1300FC::~CHV1300FC()
{
	DestoryFeatures();
}




void CHV1300FC::CreateFeatures()
{
	assert(m_pInfo == NULL);
	m_pInfo = new CHV1300FCInfo((CDevice *)this);
	assert(m_pGain == NULL);
	m_pGain = new CHV1300FCGain(8, (CDevice *)this);
	assert(m_pShutter == NULL);
	m_pShutter = new CHV1300FCShutter(60, (CDevice *)this);
	assert(m_pSnapMode == NULL);
	m_pSnapMode = new CHV1300FCSnapMode(0, (CDevice *)this);
	assert(m_pTriggerPolarity == NULL);
	m_pTriggerPolarity = new CHV1300FCTriggerPolarity(1, (CDevice *)this);
	assert(m_pStrobePolarity == NULL);
	m_pStrobePolarity = new CHV1300FCStrobePolarity(1, (CDevice *)this);
	assert(m_pADCLevel == NULL);
	m_pADCLevel = new CHV1300FCADCLevel(ADC_LEVEL3, (CDevice *)this);
	assert(m_pBlackLevelEnable == NULL);
	m_pBlackLevelEnable = new CHV1300FCBlackLevelEnable(1, (CDevice *)this);
	assert(m_pBlackLevel == NULL);
	m_pBlackLevel = new CHV1300FCBlackLevel(-20, (CDevice *)this);

	assert(m_pBlank == NULL);
	m_pBlank = new CHV1300FCBlank(0, 0, (CDevice *)this);
}


void CHV1300FC::DestoryFeatures()
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
