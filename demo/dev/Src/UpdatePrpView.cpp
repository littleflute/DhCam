
#include "stdafx.h"
#include "DeviceManager.h"
#include "PropertyView.h"
#include "device.h"
#include "mainfrm.h"
#include "utility.h"


void CPropertyView::Update_DefaultValue(CDevice* pDevice, BOOL b)
{
	HVTYPE type = pDevice->m_pInfo->DeviceType();
	
	if(type == DRVIFGIGETYPE)
	{
		if(pDevice->m_flagPassive == 1 && pDevice->m_PassiveTransOnOff == 1)
			pDevice->m_PassiveTransBufNum = Spec_Fun_ADV_Get(pDevice, ID_DO_PASSIVE_TRANS);
		return;
	}
	
	OnShutterAuto(pDevice, b);
	OnGainAuto(pDevice, b);
	
	if(IS_PD_CAM(type))
	{
		pDevice->m_nTestImage = 0;
		//延迟曝光功能
		pDevice->m_bTriggerDelayOnOff=0;
		pDevice->m_nTriggerDelayValue=1;
		
		Spec_Fun_Interface_1(pDevice,TEST_IMAGE,pDevice->m_nTestImage);
		Spec_Fun_Interface_1(pDevice,TRIGGER_DELAY_ON_OFF, pDevice->m_bTriggerDelayOnOff);
		Spec_Fun_Interface_1(pDevice,TRIGGER_DELAY_VALUE, pDevice->m_nTriggerDelayValue);
		
		OnMultiTriggerNumChanged(pDevice, 1);
		OnMultiTriggerTimeChanged(pDevice, 0);
		
		pDevice->m_nOutPutIO_0=3;
		pDevice->m_nOutPutIO_1=1;
		pDevice->m_nOutPutIO_2=0;
		pDevice->m_nOutPutIO_0_POL=0;
		pDevice->m_nOutPutIO_1_POL=0;
		pDevice->m_nOutPutIO_2_POL=0;
		
		Spec_Fun_Interface_1(pDevice,OUTPUT_IO_0, pDevice->m_nOutPutIO_0);
		Spec_Fun_Interface_1(pDevice,OUTPUT_IO_1, pDevice->m_nOutPutIO_1);
		Spec_Fun_Interface_1(pDevice,OUTPUT_IO_2, pDevice->m_nOutPutIO_2);
		
		Spec_Fun_Interface_1(pDevice,OUTPUT_IO_0_POL, pDevice->m_nOutPutIO_0_POL);		
		Spec_Fun_Interface_1(pDevice,OUTPUT_IO_1_POL, pDevice->m_nOutPutIO_1_POL);		
		Spec_Fun_Interface_1(pDevice,OUTPUT_IO_2_POL, pDevice->m_nOutPutIO_2_POL);		
		
		pDevice->m_nTriggerSource=0;		
		Spec_Fun_Interface_1(pDevice,TRIGGER_SOURCE_IO,pDevice->m_nTriggerSource);		
	}	
}


void CPropertyView::UpdateGain(CDevice * pDevice)
{
	if(pDevice->m_flagAutoGain == 0)
		return;
	
	CString str,  strAuto;
	HPROPERTY hName, hNameAuto;
	
	str.LoadString(IDS_GAINLEVEL);
	hName = m_ListView.FindProperty(str);		
	
	strAuto.LoadString(IDS_GAIN_AUTO);
	hNameAuto = m_ListView.FindProperty(strAuto);
	pDevice->m_bGainAuto		 = Spec_Fun_Get(pDevice, ITEM_GAIN_AUTO);
	pDevice->m_bGainAutoInq		 = Spec_Fun_Get_Descript(pDevice, ITEM_GAIN_AUTO, 0);
	
	switch(pDevice->m_bGainAuto) {
	case 0:
		strAuto.Format("%s", "Disable");
		break;
	case 1:
		strAuto.Format("%s", "Enable");
		break;
	}
	CComVariant vAtuo(strAuto);
	m_ListView.SetItemValue(hNameAuto, &vAtuo);
	/*****************************/
	if(pDevice->m_bGainAuto == 0)
	{
		pDevice->m_Gain = Spec_Fun_Get(pDevice, SCALAR_GAINLEVEL);
		str.LoadString(IDS_GAINLEVEL);
		str.Format("%d", pDevice->m_Gain);
		CComVariant v(str);
		m_ListView.SetItemValue(hName, &v);
	}
	if(pDevice->m_flagAutoStrobe == 0)
	{
		m_ListView.SetItemEnabled(hName,	!pDevice->m_bGainAuto);
		m_ListView.SetItemEnabled(hNameAuto, pDevice->m_bGainAutoInq);
		return;
	}
	
	pDevice->m_bStrobeAutoInq	= Spec_Fun_AutoDescript(pDevice, ITEM_STROBE_AUTO,0);
	pDevice->m_bStrobeAuto		= Spec_Fun_Get_AutoFunc(pDevice, ITEM_STROBE_AUTO);
	
	m_ListView.SetItemEnabled(hNameAuto, pDevice->m_bGainAutoInq);
	
	if (pDevice->m_bStrobeAutoInq && pDevice->m_bStrobeAuto) 
		m_ListView.SetItemEnabled(hName,  0);
	else
		m_ListView.SetItemEnabled(hName,  !pDevice->m_bGainAuto);
}

void CPropertyView::UpdateGainLevel(CDevice *pDevice, int flag)
{
	CString str;
	HPROPERTY hName;
	str.LoadString(IDS_GAINLEVEL);
	hName = m_ListView.FindProperty(str);		
	pDevice->m_Gain = Spec_Fun_Get(pDevice, SCALAR_GAINLEVEL);
	str.Format("%d", pDevice->m_Gain);
	CComVariant v(str);
	m_ListView.SetItemValue(hName, &v);
	m_ListView.SetItemEnabled(hName, flag);

}

void CPropertyView::UpdateShutterHV(CDevice* pDevice)
{
	CString str, str1, strAuto;
	HPROPERTY hName, hName1;
	
	str.LoadString(IDS_SHUTTERSPEED);
	hName = m_ListView.FindProperty(str);		//快门速度
	
	str1.LoadString(IDS_TIME_UNIT);
	hName1 = m_ListView.FindProperty(str1);	//速度单位

	pDevice->m_ShutterUnit	= Spec_Fun_ADV_Get(pDevice, ITEM_SHUTTERUNIT);
	pDevice->m_ShutterSpeed	= Spec_Fun_Get(pDevice, SCALAR_SHUTTERSPEED);

	pDevice->m_ShutterRange.maxV = Spec_Fun_Get_Descript(pDevice, SCALAR_SHUTTERSPEED, 1);
	pDevice->m_ShutterRange.minV = Spec_Fun_Get_Descript(pDevice, SCALAR_SHUTTERSPEED, 0);		
	m_ListView.SetItemRangeValue(hName, pDevice->m_ShutterRange.minV, pDevice->m_ShutterRange.maxV);	
	pDevice->m_pShutter->m_Unit.Set((TUnit)pDevice->m_ShutterUnit);		
	
	str.Format("%d", pDevice->m_ShutterSpeed);
	CComVariant v(str);
	m_ListView.SetItemValue(hName, &v);
	
	switch(pDevice->m_ShutterUnit) {
	case 0:
		str1.Format("%s", "us");
		break;
	case 1:
		str1.Format("%s", "ms");
		break;
	}
	CComVariant v1(str1);
	m_ListView.SetItemValue(hName1, &v1);

}


void CPropertyView::UpdateShutter(CDevice* pDevice)
{
	if(!pDevice->m_flagAutoShutter)			//旧款相机已经被置为1
		return;
	
	CString str, str1, strAuto;
	HPROPERTY hName, hName1, hNameAuto;

	str.LoadString(IDS_SHUTTERSPEED);
	hName = m_ListView.FindProperty(str);		//快门速度
	
	str1.LoadString(IDS_TIME_UNIT);
	hName1 = m_ListView.FindProperty(str1);	//速度单位

	strAuto.LoadString(IDS_SHUTTER_AUTO);
	hNameAuto = m_ListView.FindProperty(strAuto);			
	pDevice->m_bShutterAuto		 = Spec_Fun_Get(pDevice, ITEM_SHUTTER_AUTO);
	{
		switch(pDevice->m_bShutterAuto) {
		case 0:
			strAuto.Format("%s", "Disable");
			break;
		case 1:
			strAuto.Format("%s", "Enable");
			break;
		}
		CComVariant vAtuo(strAuto);
		m_ListView.SetItemValue(hNameAuto, &vAtuo);
	}


	if(0 == pDevice->m_bShutterAuto)
	{
		pDevice->m_ShutterUnit	= Spec_Fun_ADV_Get(pDevice, ITEM_SHUTTERUNIT);
		pDevice->m_ShutterSpeed	= Spec_Fun_Get(pDevice, SCALAR_SHUTTERSPEED);
		
		pDevice->m_ShutterRange.maxV = Spec_Fun_Get_Descript(pDevice, SCALAR_SHUTTERSPEED, 1);
		pDevice->m_ShutterRange.minV = Spec_Fun_Get_Descript(pDevice, SCALAR_SHUTTERSPEED, 0);		
		m_ListView.SetItemRangeValue(hName, pDevice->m_ShutterRange.minV, pDevice->m_ShutterRange.maxV);	
		pDevice->m_pShutter->m_Unit.Set((TUnit)pDevice->m_ShutterUnit);		
		
		str.Format("%d", pDevice->m_ShutterSpeed);
		CComVariant v(str);
		m_ListView.SetItemValue(hName, &v);
		
		switch(pDevice->m_ShutterUnit) {
		case 0:
			str1.Format("%s", "us");
			break;
		case 1:
			str1.Format("%s", "ms");
			break;
		}
		CComVariant v1(str1);
		m_ListView.SetItemValue(hName1, &v1);
	}
	
	if(m_IS_OldITS || m_IS_OldGC || m_IS_OldGM)
	{
		m_ListView.SetItemEnabled(hName,  !pDevice->m_bShutterAuto);
		m_ListView.SetItemEnabled(hName1, !pDevice->m_bShutterAuto);
		return;
	}

	pDevice->m_bShutterAutoInq	= Spec_Fun_Get_Descript(pDevice,ITEM_SHUTTER_AUTO, 0);

	if (pDevice->m_flagAutoStrobe == 0) {
		m_ListView.SetItemEnabled(hName,  !pDevice->m_bShutterAuto);
		m_ListView.SetItemEnabled(hName1, !pDevice->m_bShutterAuto);
		m_ListView.SetItemEnabled(hNameAuto, pDevice->m_bShutterAutoInq);
	}
	else
	{
		pDevice->m_bStrobeAutoInq	= Spec_Fun_AutoDescript(pDevice, ITEM_STROBE_AUTO,0);
		pDevice->m_bStrobeAuto		= Spec_Fun_Get_AutoFunc(pDevice, ITEM_STROBE_AUTO);
		
		if (pDevice->m_bStrobeAutoInq && pDevice->m_bStrobeAuto) {
			m_ListView.SetItemEnabled(hName,  0);
			m_ListView.SetItemEnabled(hName1, 0);
			m_ListView.SetItemEnabled(hNameAuto, pDevice->m_bShutterAutoInq);
		}
		else
		{
			m_ListView.SetItemEnabled(hName,  !pDevice->m_bShutterAuto);
			m_ListView.SetItemEnabled(hName1, !pDevice->m_bShutterAuto);		
			m_ListView.SetItemEnabled(hNameAuto, pDevice->m_bShutterAutoInq);
		}
	}
}

void CPropertyView::UpdateMultiTriggerTime(CDevice* pDevice)
{
	if(pDevice->m_flagMultiTrigger == 0)
		return;

	pDevice->m_MultiTriggerTimeRange.maxV	= Spec_Fun_Get_AdvDescript(pDevice, SCALAR_TRIGGER_TIME, 1);
	pDevice->m_MultiTriggerTimeRange.minV	= Spec_Fun_Get_AdvDescript(pDevice, SCALAR_TRIGGER_TIME, 0);
	pDevice->m_TriggerTime					= Spec_Fun_ADV_Get(pDevice, SCALAR_TRIGGER_TIME);
	
	pDevice->m_MultiTriggerOnOffInq			= Spec_Fun_Get_AdvDescript(pDevice, ITEM_MULTI_TRIGGER_ONOFF, 0);// 2009.06.25
	pDevice->m_MultiTriggerNumRange.maxV	= Spec_Fun_Get_AdvDescript(pDevice, SCALAR_TRIGGER_NUM, 1);
	pDevice->m_MultiTriggerNumRange.minV	= Spec_Fun_Get_AdvDescript(pDevice, SCALAR_TRIGGER_NUM, 0);
	pDevice->m_TriggerNum					= Spec_Fun_ADV_Get(pDevice, SCALAR_TRIGGER_NUM);
	

	CString str;
	str.LoadString(IDS_TRIGGER_TIME);
	HPROPERTY hName = m_ListView.FindProperty(str);
	
	if (hName){
		
		m_ListView.SetItemRangeValue(hName, pDevice->m_MultiTriggerTimeRange.minV, 
			pDevice->m_MultiTriggerTimeRange.maxV);
		
		str.Format("%d", pDevice->m_TriggerTime);
		CComVariant v(str);
		m_ListView.SetItemValue(hName, &v);
	}
	//-------------2009.06.25 add-------
	str.LoadString(IDS_SNAP_TRIGGER_NUM);
	hName = m_ListView.FindProperty(str);
	if(hName)
	{
		str.Format("%d", pDevice->m_TriggerNum);
		CComVariant v(str);
		m_ListView.SetItemValue(hName, &v);
		m_ListView.SetItemEnabled(hName, pDevice->m_MultiTriggerOnOffInq);
	}
}


void CPropertyView::UpdateTriggerDelay(CDevice *pDevice)
{
	if(pDevice->m_flagTriggerDelay == 0)
		return;
	
	HPROPERTY hNameOnOff, hNameVal, hNameUnit;
	CString str, strVal, strUnit;
	
	str.LoadString(IDS_TRIGGER_DELAY_SWITCH);
	hNameOnOff = m_ListView.FindProperty(str);	
	
	strVal.LoadString(IDS_TRIGGER_DELAY_VALUE);
	hNameVal = m_ListView.FindProperty(strVal);

	pDevice->m_TriggerDelayStateDescrip = Spec_Fun_Get_Descript(pDevice, ITEM_TRIGGERDELAY_STATE, 0);
	pDevice->m_bTriggerDelayOnOff	  = Spec_Fun_Get(pDevice, ITEM_TRIGGERDELAY_STATE);
	pDevice->m_nTriggerDelayValue	  = Spec_Fun_Get(pDevice, SCALAR_TRIGGERDELAY_VALUE);

	if(pDevice->m_flagTriggerDelayUnit != 0)
	{	
		strUnit.LoadString(IDS_TRIGGERDELAY_UNIT);
		hNameUnit = m_ListView.FindProperty(strUnit);
		pDevice->m_TriggerDelayUnit		  = Spec_Fun_ADV_Get(pDevice, ITEM_TRIGGERDELAY_UNIT);
	}
	
	if(pDevice->m_TriggerDelayStateDescrip != 0)
	{
		switch(pDevice->m_bTriggerDelayOnOff) {
		case 0:
			str.Format("off");		
			break;
		case 1:
			str.Format("on");		
			break;
		default:
			break;
		}
		CComVariant v(str);
		m_ListView.SetItemValue(hNameOnOff, &v);
		m_ListView.SetItemEnabled(hNameOnOff, pDevice->m_TriggerDelayStateDescrip);

		if(pDevice->m_flagTriggerDelayUnit != 0)
		{
			switch(pDevice->m_TriggerDelayUnit) {
			case 0:
				strUnit.Format("us");		
				break;
			case 1:
				strUnit.Format("ms");		
				break;
			default:
				break;
			}
			CComVariant vUnit(strUnit);
			m_ListView.SetItemValue(hNameUnit, &vUnit);
			m_ListView.SetItemEnabled(hNameUnit, pDevice->m_bTriggerDelayOnOff);
		}
		strVal.Format("%d", pDevice->m_nTriggerDelayValue);		
		CComVariant v1(strVal);
		m_ListView.SetItemValue(hNameVal, &v1);
		m_ListView.SetItemEnabled(hNameVal, pDevice->m_bTriggerDelayOnOff);
	}
	else
	{
		CComVariant v(str);
		//		pDevice->m_bTriggerDelayOnOff = 0;
		str.Format("%d", pDevice->m_bTriggerDelayOnOff);		
		m_ListView.SetItemValue(hNameOnOff, &v);
		m_ListView.SetItemEnabled(hNameOnOff,  0);

		if(pDevice->m_flagTriggerDelayUnit != 0)
		{
			CComVariant vUnit(str);
			str.Format("%d", pDevice->m_TriggerDelayUnit);		
			m_ListView.SetItemValue(hNameUnit, &vUnit);
			m_ListView.SetItemEnabled(hNameUnit,  0);		
		}
		
		CComVariant v1(strVal);
		strVal.Format("%d", pDevice->m_nTriggerDelayValue);		
		m_ListView.SetItemValue(hNameVal, &v1);
		m_ListView.SetItemEnabled(hNameVal, 0);
	}
}


void CPropertyView::UpdateWB(CDevice* pDevice)
{
	if(pDevice->m_flagWB == 0)
		return;
	
	pDevice->m_R_Value		= Spec_Fun_Get(pDevice, SCALAR_R_VALUE);
	pDevice->m_B_Value		= Spec_Fun_Get(pDevice, SCALAR_B_VALUE);
	
	if(pDevice->m_WB_AutoInq)
		pDevice->m_WB_AutoManual= Spec_Fun_Get(pDevice, ITEM_WB_AUTO_MANU);
	if(pDevice->m_WB_OnePushInq)
		pDevice->m_WB_OnePush	= Spec_Fun_Get(pDevice, ITEM_WB_ONE_PUSH);
	
				
	HPROPERTY hName;
	CString str, strVal, strUnit;
	
	str.LoadString(IDS_R_VALUE);
	hName = m_ListView.FindProperty(str);	
	str.Format("%d", pDevice->m_R_Value);
	CComVariant v(str);
	m_ListView.SetItemValue(hName, &v);
	m_ListView.SetItemEnabled(hName, pDevice->m_WB_OnOff && !pDevice->m_WB_AutoManual);
	
	
	str.LoadString(IDS_B_VALUE);
	hName = m_ListView.FindProperty(str);
	str.Format("%d", pDevice->m_B_Value);
	CComVariant v1(str);
	m_ListView.SetItemValue(hName, &v1);
	m_ListView.SetItemEnabled(hName, pDevice->m_WB_OnOff && !pDevice->m_WB_AutoManual);
	
	
	if(pDevice->m_WB_AutoInq)
	{
		str.LoadString(IDS_WB_AUTO);
		hName = m_ListView.FindProperty(str);
		switch(pDevice->m_WB_AutoManual) {
		case 0:
			str.Format("Manual");
			break;
		case 1:
			str.Format("Auto");
			break;
		default:
			break;
		}
		CComVariant vAuto(str);
		m_ListView.SetItemValue(hName, &vAuto);
		m_ListView.SetItemEnabled(hName, pDevice->m_WB_OnOff);
	}
	
	if(pDevice->m_WB_OnePushInq)
	{
		str.LoadString(IDS_WB_ONEPUSH_BUTTON);
		hName = m_ListView.FindProperty(str);
		m_ListView.SetItemEnabled(hName, pDevice->m_WB_OnOff);
	}
	
}

void CPropertyView::UpdateRGBChannel(CDevice* pDevice)		// modified by xupx 2009.10.26 for HV_Cam
{
// 	if(pDevice->m_flagSeparateGain == 0)
// 		return;
	HPROPERTY hName;
	CString str;

	if(pDevice->resAdvPresence.PixelsSeparateGianRB || pDevice->m_flagSeparateGain == 1)
	{
		pDevice->m_RedGain	 = Spec_Fun_ADV_Get(pDevice, SCALAR_RED_GAIN);
		pDevice->m_BlueGain	 = Spec_Fun_ADV_Get(pDevice, SCALAR_BLUE_GAIN);

		{
			str.LoadString(IDS_WHITEBALANCE_V);
			hName = m_ListView.FindProperty(str);		
			str.Format("%d", pDevice->m_RedGain);
			CComVariant v(str);
			m_ListView.SetItemValue(hName, &v);
			m_ListView.SetItemEnabled(hName,  !pDevice->m_WB_OnOff);
		}
		{
			str.LoadString(IDS_WHITEBALANCE_U);
			hName = m_ListView.FindProperty(str);		
			str.Format("%d", pDevice->m_BlueGain);
			CComVariant v(str);
			m_ListView.SetItemValue(hName, &v);
			m_ListView.SetItemEnabled(hName,  !pDevice->m_WB_OnOff);
		}
	}

	if (pDevice->resAdvPresence.PixelsSeparateGainG1G2|| pDevice->m_flagSeparateGain == 1)
	{
		pDevice->m_GreenGain = Spec_Fun_ADV_Get(pDevice, SCALAR_GREEN_GAIN);

		str.LoadString(IDS_WHITEBALANCE_G);
		hName = m_ListView.FindProperty(str);		
		str.Format("%d", pDevice->m_GreenGain);
		CComVariant v(str);
		m_ListView.SetItemValue(hName, &v);
		m_ListView.SetItemEnabled(hName,  !pDevice->m_WB_OnOff);
	}
		
}

void CPropertyView::UpdateOutputIoControl(CDevice* pDevice, int nID, int nIDS)
{
	HPROPERTY hName;
	CString str;
	int nVal = 0;
	int nEnable = 0;

	str.LoadString(nIDS);
	hName = m_ListView.FindProperty(str);
	nVal = Spec_Fun_ADV_Get(pDevice, nID);

	switch(nID) {
	case ITEM_OUTPUTIO_0_CONTROL:
		pDevice->m_nOutPutIO_0_POL = nVal;
		nEnable = pDevice->m_nOutPutIO_0==2 ? 1:0;
		break;
	case ITEM_OUTPUTIO_1_CONTROL:
		pDevice->m_nOutPutIO_1_POL = nVal;
		nEnable = pDevice->m_nOutPutIO_1==2 ? 1:0;
		break;
	case ITEM_OUTPUTIO_2_CONTROL:
		pDevice->m_nOutPutIO_2_POL = nVal;
		nEnable = pDevice->m_nOutPutIO_2==2 ? 1:0;
		break;
	default:
		break;
	}
				
	switch(nVal) {
	case 0:
		str.Format(" Low");
		break;
	case 1:
		str.Format(" High");
		break;
	default:
		break;
	}
	CComVariant v(str);
	m_ListView.SetItemValue(hName, &v);
	m_ListView.SetItemEnabled(hName, nEnable);
	
}

void CPropertyView::UpdateVideoMode(CDevice* pDevice)
{
	pDevice->m_CurrentModeIdx = pDevice->GetVideoMode();
	pDevice->UpdateImageSize((HV_VIDEO_MODE)pDevice->m_CurrentModeIdx);	//更新图像尺寸，从而更新缓冲区大小。
	
	CString strVideo;
	strVideo.LoadString(IDS_VIDEOMODE);
	HPROPERTY hName = m_ListView.FindProperty(strVideo);	
	CPropertyListItem * prop = (CPropertyListItem *)hName;	
	
	LPCTSTR list[256] = { NULL }; 
	int i = 0;
	std::list<CString> strlist = pDevice->DeviceModeList();
	std::list<CString>::iterator pString;
	for ( i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
	{
		list[i++] = (*pString);
	}
	list[i] = NULL;
	prop->SetList(list);					
	//-----------------------------
	strVideo.Format(list[pDevice->m_CurrentModeIdx]);
	CComVariant v(strVideo);
	m_ListView.SetItemValue(hName, &v);
	m_ListView.SetItemEnabled(hName,  1);
	
}

void CPropertyView::UpdateDigitalGain(CDevice *pDevice)
{
	if (IS_HV_CAM(pDevice->m_HVTYPE) )
	{
		if(pDevice->m_flagDigitalGain == 0)
			return;

		CString str;
        str.LoadString(IDS_ADCLEVEL);
		HPROPERTY hName = m_ListView.FindProperty(str);		//
        if (hName){
            m_ListView.SetItemData(hName, ITEM_DIGITAL_GAIN);
            m_ListView.SetItemEnabled(hName, pDevice->m_bDigitalGainInq);
        }

		int nGain = Spec_Fun_ADV_Get(pDevice, ITEM_DIGITAL_GAIN);
		pDevice->m_bDigitalGainInq = Spec_Fun_Get_AdvDescript(pDevice, ITEM_DIGITAL_GAIN,0);
		pDevice->m_nDigitalGainItem = DigitalGain2Item[nGain];					
		
        std::list<CString> strlist;
		switch(pDevice->m_nDigitalGainItem) {
		case 0:
			str.Format("%s", "ADC Level 0");
			break;
		case 1:
			str.Format("%s", "ADC Level 1");
			break;
		case 2:
			str.Format("%s", "ADC Level 2");
			break;
		case 3:
			str.Format("%s", "ADC Level 3");
			break;
		}
		CComVariant v(str);
		m_ListView.SetItemValue(hName, &v);
		
		m_ListView.SetItemEnabled(hName,  pDevice->m_bDigitalGainInq);
		
		
		return;
	}
	/******************************************************/
	CString str;
	str.LoadString(IDS_DIGITAL_GAIN);
	HPROPERTY hName = m_ListView.FindProperty(str);		//
	
	CPropertyListItem * prop = (CPropertyListItem *)hName;	


	if(pDevice->m_bDigitalGainInq == 1 )
	{	
		int nGain = Spec_Fun_ADV_Get(pDevice, ITEM_DIGITAL_GAIN);
		std::list<CString> strlist;
		strlist.clear();
		strlist.push_back("4 -- 11");
		strlist.push_back("3 -- 10");
		strlist.push_back("2 --  9");
		strlist.push_back("1 --  8");
		strlist.push_back("0 --  7");
		int i = 0;
		LPCTSTR list[256] = { NULL }; 
		std::list<CString>::iterator pString = NULL;
		for ( i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
		{
			list[i++] = (*pString);
		}
		list[i] = NULL;
		prop->SetList(list);					
		
		pDevice->m_nDigitalGainItem = DigitalGain2Item[nGain];					
		
		switch(pDevice->m_nDigitalGainItem) {
		case 0:
			str.Format("%s", "4 -- 11");
			break;
		case 1:
			str.Format("%s", "3 -- 10");
			break;
		case 2:
			str.Format("%s", "2 --  9");
			break;
		case 3:
			str.Format("%s", "1 --  8");
			break;
		case 4:
			str.Format("%s", "0 --  7");
			break;
		}
		CComVariant v(str);
		m_ListView.SetItemValue(hName, &v);
		
		m_ListView.SetItemEnabled(hName,  1);
		
	}
	else
	{
		std::list<CString> strlist;
		strlist.clear();
		strlist.push_back("0 -- 11");
		int i = 0;
		LPCTSTR list[256] = { NULL }; 
		std::list<CString>::iterator pString = NULL;
		for ( i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
		{
			list[i++] = (*pString);
		}
		list[i] = NULL;
		prop->SetList(list);					
		str.Format("%s", "0 --  11");
		
		CComVariant v(str);
		m_ListView.SetItemValue(hName, &v);
		m_ListView.SetItemEnabled(hName,  0);
	}
	///end 更新界面
	
}

void CPropertyView::UpdateColorMode(CDevice *pDevice)
{
	CString str;
	str.LoadString(IDS_RAW_TYPE_ITEM);
	HPROPERTY hName = m_ListView.FindProperty(str);
	
	CPropertyListItem * prop = (CPropertyListItem *)hName;	
	std::list<CString> strlist;
	strlist.clear();
	
	int n = 0;
	Spec_Fun_Get_FMT7_DESCRIPTOR(pDevice, ColorModeInq, 0);
	
	if(IS_READABLE_CAM(pDevice->m_pInfo->DeviceType())) 
	{
		if(pDevice->m_resFMT7Descriptor.ColorMode.Raw8Inq)
			strlist.push_back("Raw8");
		if(pDevice->m_resFMT7Descriptor.ColorMode.Mono8Inq)
		{
			if(!m_IS_OldITS && !m_IS_OldGC && !m_IS_OldGM )
				strlist.push_back("Mono 8");
		}
		if(pDevice->m_resFMT7Descriptor.ColorMode.RGB8Inq)
		{
			if(!m_IS_OldITS && !m_IS_OldGC && !m_IS_OldGM )
				strlist.push_back("RGB8");
		}
		if(pDevice->m_resFMT7Descriptor.ColorMode.Mono10Inq)
		{
			if(!m_IS_OldITS && !m_IS_OldGC && !m_IS_OldGM )
				strlist.push_back("Mono 10");
		}
		if(pDevice->m_resFMT7Descriptor.ColorMode.Mono10NormalizedInq)
		{
			if(!m_IS_OldITS && !m_IS_OldGC && !m_IS_OldGM )
				strlist.push_back("Mono10 Normalized");
		}
		if(pDevice->m_resFMT7Descriptor.ColorMode.Mono10PackedInq)
		{
			if(!m_IS_OldITS && !m_IS_OldGC && !m_IS_OldGM )
				strlist.push_back("Mono10 Packed");
		}
		if(pDevice->m_resFMT7Descriptor.ColorMode.Raw10Inq)
		{
			if(!m_IS_OldITS && !m_IS_OldGC && !m_IS_OldGM )
				strlist.push_back("Raw 10");
		}
		if(pDevice->m_resFMT7Descriptor.ColorMode.Raw10NormalizedInq)
		{
			if(!m_IS_OldITS && !m_IS_OldGC && !m_IS_OldGM )
				strlist.push_back("Raw10 Normalized");
		}
		if(pDevice->m_resFMT7Descriptor.ColorMode.Raw10PackedInq)
		{
			if(!m_IS_OldITS && !m_IS_OldGC && !m_IS_OldGM )
				strlist.push_back("Raw10 Packed");
		}
		if(pDevice->m_resFMT7Descriptor.ColorMode.Mono12Inq)
		{
			if(!m_IS_OldITS && !m_IS_OldGC && !m_IS_OldGM )
				strlist.push_back("Mono 12");
		}
		if(pDevice->m_resFMT7Descriptor.ColorMode.Mono12NormalizedInq)
		{
			if(!m_IS_OldITS && !m_IS_OldGC && !m_IS_OldGM )
				strlist.push_back("Mono12 Normalized");
		}
		if(pDevice->m_resFMT7Descriptor.ColorMode.Mono12PackedInq)
		{
			if(!m_IS_OldITS && !m_IS_OldGC && !m_IS_OldGM )
				strlist.push_back("Mono12 Packed");
		}
		if(pDevice->m_resFMT7Descriptor.ColorMode.Raw12Inq)
		{
			if(!m_IS_OldITS && !m_IS_OldGC && !m_IS_OldGM)
				strlist.push_back("Raw12");
		}
		if(pDevice->m_resFMT7Descriptor.ColorMode.Raw12NormalizedInq)
		{
			if(!m_IS_OldITS && !m_IS_OldGC && !m_IS_OldGM)
				strlist.push_back("Raw12 Normalized");
		}
		if(pDevice->m_resFMT7Descriptor.ColorMode.Raw12PackedInq)
		{
			if(!m_IS_OldITS && !m_IS_OldGC && !m_IS_OldGM)
				strlist.push_back("Raw12 Packed");
		}
		if(pDevice->m_resFMT7Descriptor.ColorMode.Mono16Inq)
		{
			if(!m_IS_OldITS && !m_IS_OldGC && !m_IS_OldGM )
				strlist.push_back("Mono 16");
		}
		if(pDevice->m_resFMT7Descriptor.ColorMode.SignedMono16Inq)
		{
			if(!m_IS_OldITS && !m_IS_OldGC && !m_IS_OldGM )
				strlist.push_back("Signed Mono16");
		}
		if(pDevice->m_resFMT7Descriptor.ColorMode.Raw16Inq)
		{
			if(!m_IS_OldITS && !m_IS_OldGC && !m_IS_OldGM )
				strlist.push_back("Raw 16");
		}
		if(pDevice->m_resFMT7Descriptor.ColorMode.RGB16Inq)
		{
			if(!m_IS_OldITS && !m_IS_OldGC && !m_IS_OldGM )
				strlist.push_back("RGB 16");
		}
		if(pDevice->m_resFMT7Descriptor.ColorMode.SingnedRGB16Inq)
		{
			if(!m_IS_OldITS && !m_IS_OldGC && !m_IS_OldGM )
				strlist.push_back("Signed RGB16");
		}
		if(pDevice->m_resFMT7Descriptor.ColorMode.JpegInq)
		{
			if(!m_IS_OldITS && !m_IS_OldGC && !m_IS_OldGM )
				strlist.push_back("JPEG");
		}
		if(pDevice->m_resFMT7Descriptor.ColorMode.Dyuv422Inq)
		{
			if(!m_IS_OldITS && !m_IS_OldGC && !m_IS_OldGM )
				strlist.push_back("DYUV 422");
		}
		if(pDevice->m_resFMT7Descriptor.ColorMode.YUV8_4_1_1Inq)
		{
			if(!m_IS_OldITS && !m_IS_OldGC && !m_IS_OldGM )
				strlist.push_back("YUV8_4_1_1");
		}
		if(pDevice->m_resFMT7Descriptor.ColorMode.YUV8_4_2_2Inq)
		{
			if(!m_IS_OldITS && !m_IS_OldGC && !m_IS_OldGM )
				strlist.push_back("YUV8_4_2_2");
		}
		if(pDevice->m_resFMT7Descriptor.ColorMode.YUV8_4_4_4Inq)
		{
			if(!m_IS_OldITS && !m_IS_OldGC && !m_IS_OldGM )
				strlist.push_back("YUV8_4_4_4");
		}
		/////////////////////////////////
		HV_VIDEO_MODE videoMode;
		videoMode = pDevice->GetVideoMode();
		pDevice->m_ColorCode = (HV_COLOR_CODE)pDevice->GetFMT7Mode(videoMode, FMT7_MODE_FUNC_ID_COLOR_CODING, NULL); 

		map <HV_COLOR_CODE, int>::iterator iter;
		iter = pDevice->m_ColorCodeIndex.begin();
		for(int id =0; id < pDevice->m_ColorCodeIndex.size(); id++ )
		{
			if(iter->first == pDevice->m_ColorCode)
			{
				n  = iter->second;
			}
			iter ++;
		}

		// ---------
		int i = 0;
		LPCTSTR list[256] = { NULL }; 
		std::list<CString>::iterator pString = NULL;
		for ( i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
		{
			list[i++] = (*pString);
		}
		list[i] = NULL;
		prop->SetList(list);					
		str.Format(list[n]);
		
		CComVariant v(str);
		m_ListView.SetItemValue(hName, &v);
		m_ListView.SetItemEnabled(hName,  1);
		
	}
}


void CPropertyView::UpdatePacketSize(CDevice *pDevice)
{
	if (!IS_READABLE_CAM(pDevice->m_CamType))
		return;


	CString str;    
	str.LoadString(IDS_PACKETSIZE);
    TRACKBARINFO info;

	info.LineSize			 = 4;
	info.RangeMax			 = Spec_Fun_Get_FMT7_DESCRIPTOR(pDevice, SCALAR_PACKET, 1);
	info.RangeMin			 = Spec_Fun_Get_FMT7_DESCRIPTOR(pDevice, SCALAR_PACKET, 0);
	pDevice->m_PacketSize	 = Spec_Fun_Get(pDevice, SCALAR_PACKET);

	HPROPERTY hName = m_ListView.FindProperty(str);	
	if (hName)
	{
		m_ListView.SetItemRangeValue(hName, info.RangeMin, info.RangeMax);
		str.Format("%d", pDevice->m_PacketSize);
		CComVariant v(str);
		m_ListView.SetItemValue(hName, &v);
	}
	UpdateShutter(pDevice);
}
