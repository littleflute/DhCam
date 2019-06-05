// HV1300FM.cpp: implementation of the CHV1300FM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HV1300FM.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHV1300FM::CHV1300FM(int nNumber, int nIndex, HWND hWndNotify, CMainFrame& MainFrame) :
CDevice(nNumber, nIndex, hWndNotify, MainFrame)
{
	CreateFeatures();
}


CHV1300FM::~CHV1300FM()
{
	DestoryFeatures();
}




void CHV1300FM::CreateFeatures()
{
	assert(m_pInfo == NULL);
	m_pInfo = new CHV1300FMInfo((CDevice *)this);
	assert(m_pGain == NULL);
	m_pGain = new CHV1300FMGain(8, (CDevice *)this);
	assert(m_pShutter == NULL);
	m_pShutter = new CHV1300FMShutter(60, (CDevice *)this);
	assert(m_pSnapMode == NULL);
	m_pSnapMode = new CHV1300FMSnapMode(0, (CDevice *)this);
	assert(m_pTriggerPolarity == NULL);
	m_pTriggerPolarity = new CHV1300FMTriggerPolarity(1, (CDevice *)this);
	assert(m_pStrobePolarity == NULL);
	m_pStrobePolarity = new CHV1300FMStrobePolarity(1, (CDevice *)this);
	assert(m_pADCLevel == NULL);
	m_pADCLevel = new CHV1300FMADCLevel(ADC_LEVEL3, (CDevice *)this);
	assert(m_pBlackLevelEnable == NULL);
	m_pBlackLevelEnable = new CHV1300FMBlackLevelEnable(1, (CDevice *)this);
	assert(m_pBlackLevel == NULL);
	m_pBlackLevel = new CHV1300FMBlackLevel(-20, (CDevice *)this);

	assert(m_pBlank == NULL);
	m_pBlank = new CHV1300FMBlank(0, 0, (CDevice *)this);
}


void CHV1300FM::DestoryFeatures()
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
