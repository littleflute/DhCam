// PropertyView.cpp: implementation of the CPropertyView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DeviceManager.h"
#include "PropertyView.h"
#include "device.h"
#include "mainfrm.h"
#include "utility.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// CPropertyView::CPropertyView(CDeviceManager& DeviceManager, CMainFrame& MainFrame) 
//------------------------------------------------------------------------------
/**
* Constructs a new bus viewer window
*
* \param     DeviceManager  Reference to the camera manager 
* \param     MainFrame Reference to the main frame
*/
//------------------------------------------------------------------------------


CPropertyView::CPropertyView(CDeviceManager& DeviceManager, CMainFrame& MainFrame) : 
m_DeviceManager(DeviceManager), 
m_MainFrame(MainFrame)
{
	m_IS_OldITS = FALSE;
	m_IS_OldGC	= FALSE;
	m_IS_OldGM	= FALSE;

	m_CurrentMode = (HV_VIDEO_MODE)0;

}



LRESULT CPropertyView::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	m_ListView.SubclassWindow(GetDlgItem(IDC_PROPERTYLIST));
	m_ListView.SetExtendedListStyle(PLS_EX_CATEGORIZED | PLS_EX_XPLOOK);
	
	return 0;
}




LRESULT CPropertyView::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}




LRESULT CPropertyView::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	RECT rcClient;
	GetWindowRect(&rcClient);
	
	HWND hwnd;
	RECT rc;
	hwnd = GetDlgItem(IDC_PROPERTYLIST);
	::GetWindowRect(hwnd, &rc);
	::SetWindowPos(hwnd, HWND_TOP, 
		0, 0, 
		rcClient.right - rc.left, rcClient.bottom - rc.top,
		SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
	
	return 0;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Protected / private member functions
//

//------------------------------------------------------------------------------
// void CPropertyView::ReportError(BcamException& e) 
// Author: 
//------------------------------------------------------------------------------
/**
* Shows an error message
*
* \param     e Reference to a BcamException
* \return    void
*
*/
//------------------------------------------------------------------------------
void CPropertyView::ReportError(HVBaseException& e) 
{
	m_MainFrame.ReportError(e);
}



//------------------------------------------------------------------------------
// void CPropertyView::Update_PRP(CDevice* pDevice)
// Author: 
//------------------------------------------------------------------------------
/**
* To be called if another device is to be selected (e.g. the user actiates an image 
* child window associated with a camera device which isn't selected in the tree view
*
* \param     pDevice Pointer to the camera device
* \return    void
*
*/
//------------------------------------------------------------------------------
void CPropertyView::Update_PRP(CDevice* pDevice)
{
	m_ListView.ResetContent();
				
	if ( ! pDevice ){
		return;
	}
	HVTYPE CamType	= pDevice->m_pInfo->DeviceType();
	m_hhv			= pDevice->GetHandle();

	if(!pDevice->m_bInit)
	{
		pDevice->m_HVTYPE		= pDevice->m_pInfo->DeviceHVTYPE();	
		pDevice->m_HardwareID	= pDevice->m_pInfo->HardwareVersionID();
	}

	m_IS_OldITS	= FALSE;
	m_IS_OldGC	= FALSE;
	m_IS_OldGM	= FALSE;

	if(pDevice->m_HardwareID < 110)
	{
		if(IS_OLD_ITS(pDevice->m_HVTYPE))
			m_IS_OldITS = TRUE;
	}
	if((pDevice->m_HardwareID < 200))
	{
		if(IS_OLD_GC(pDevice->m_HVTYPE))
			m_IS_OldGC = TRUE;
		if(IS_OLD_GM(pDevice->m_HVTYPE))
			m_IS_OldGM = TRUE;
	}


	if(IS_READABLE_CAM(pDevice->m_pInfo->DeviceType()))
	{
		// Spec_Fun_Presence 要在此处调用，因为旧款相机会根据需要改写一些查询标志位。
		// 旧款相机不支持 Spec_Fun_Presence

		pDevice->m_flagWB	= Spec_Fun_Presence(pDevice, SCALAR_RED_GAIN);

		if(m_IS_OldITS || m_IS_OldGC || m_IS_OldGM)
		{
			pDevice->m_flagAutoShutter	= 1;
			pDevice->m_flagAutoStrobe	= 0;
			pDevice->m_flagPassive		= 0;
			pDevice->m_flagMultiTrigger = 0;
			pDevice->m_flagDigitalGain	= 0;
			pDevice->m_flagLUT			= 1;
			pDevice->m_flagAutoGain		= 1;
			pDevice->m_flagFilterTime	= 0;	// ???

			if(m_IS_OldITS || m_IS_OldGC)
			{
				pDevice->m_flagSeparateGain = 1;
			}	
			if(m_IS_OldGM)
			{
				pDevice->m_flagSeparateGain = 0;
			}
			pDevice->m_flagTriggerDelay		= 0;
			pDevice->m_flagAntiFlicker		= 0;
			pDevice->m_flagACPhaseAdjust	= 0;
			pDevice->m_flagTriggerDelayUnit	= 0;


			if(m_IS_OldITS)
			{
				pDevice->resPIOPresence.Port0 = 1;
				pDevice->resPIOPresence.Port1 = 1;
				pDevice->resPIOPresence.Port2 = 0;
			}
			
		}
		else
		{
			pDevice->m_flagAutoShutter	 = Spec_Fun_Presence(pDevice, ITEM_SHUTTER_AUTO);
			pDevice->m_flagAutoStrobe	 = Spec_Fun_Presence(pDevice, ITEM_STROBE_AUTO);	
			pDevice->m_flagPassive		 = Spec_Fun_Presence(pDevice, ITEM_PASSIVE_TRANSMIT);
			pDevice->m_flagMultiTrigger	 = Spec_Fun_Presence(pDevice, SCALAR_TRIGGER_TIME);
			pDevice->m_flagDigitalGain	 = Spec_Fun_Presence(pDevice, ITEM_DIGITAL_GAIN);
			pDevice->m_flagLUT			 = Spec_Fun_Presence(pDevice, ITEM_ADCLEVEL);
			pDevice->m_flagAutoGain		 = Spec_Fun_Presence(pDevice, ITEM_GAIN_AUTO);
			pDevice->m_flagFilterTime	 = Spec_Fun_Presence(pDevice, SCALAR_FILTER_TIME);	

			pDevice->m_flagSeparateGain	 = Spec_Fun_Presence(pDevice, SCALAR_GREEN_GAIN);			

			pDevice->m_flagTriggerDelay	 = Spec_Fun_Presence(pDevice, ITEM_TRIGGERDELAY_STATE);
			pDevice->m_flagAntiFlicker	 = Spec_Fun_Presence(pDevice, ITEM_ANTI_FLICKER);
			pDevice->m_flagACPhaseAdjust = Spec_Fun_Presence(pDevice, SCALAR_AC_PHASE_ADJUST); 
			pDevice->m_flagTriggerDelayUnit = Spec_Fun_Presence(pDevice, ITEM_TRIGGERDELAY_UNIT); 
			pDevice->m_flagGlobalBlk	 = pDevice->resAdvPresence.GlobalBlackLevel;		//黑电平允许、黑电平调节

			if(!IS_OLD_GIGE(pDevice->m_HVTYPE))		// add by xupx 2009.09.17
			{
				pDevice->m_flagTrigger = Spec_Fun_Presence(pDevice, ITEM_INPUTIOSET);
			}

		}

	}

	/**************************/
	
	if(HV1302UCTYPE == CamType)
	{
		ListAddGain(pDevice);
		return;
	} 


	if(IS_GIGE(CamType) || IS_GM_CAM(CamType))
	{
		ListAddInfoProperty_NetCam(pDevice);
		ListAddSnapMode_NetCam(pDevice);
		ListAddTriggerDelay_NetCam(pDevice);	//延迟曝光功能
		ListAddAntiFilter(pDevice);
		ListAddPassiveTransmit(pDevice);
		ListAddVideoFormat_Readable(pDevice);

		ListAddShutter_NetCam(pDevice);
		
		ListAddGain_NetCam(pDevice);
		ListAddWB(pDevice);
		ListAddADCControlNetCam(pDevice);

		if(!IS_OLD_GIGE(pDevice->m_HVTYPE) )//IS_ITS_GC(pDevice->m_HVTYPE))
		{
			ListAddOutPutIoSet_NetCam(pDevice);	//输出IO功能设置
			ListAddOutPutIoControl_NetCam(pDevice);
			
			ListAddInPutIoSet(pDevice);		//输入IO功能设置
		}
		ListAddSaveParam(pDevice);
		
		pDevice->m_bInit = TRUE;
		return;
	}
		
	
	if(IS_PD_CAM(CamType))
	{

		ListAddInfoProperty_NetCam(pDevice);
		ListAddSnapMode_NetCam(pDevice);
		ListAddVideoFormat_Readable(pDevice);
		
		ListAddAntiFilter(pDevice);
		ListAddShutter_NetCam(pDevice);
		ListAddTriggerDelay_NetCam(pDevice);	//延迟曝光功能
		ListAddGain_NetCam(pDevice);
		ListAddWB(pDevice);
		
		ListAddADCControlNetCam(pDevice);
		ListAddTestImage(pDevice); 
		ListAddOutPutIoSet_NetCam(pDevice);	//输出IO功能设置
		ListAddOutPutIoControl_NetCam(pDevice);
		ListAddInPutIoSet(pDevice);		//输入IO功能设置
		ListAddSaveParam(pDevice);

		pDevice->m_bInit = TRUE;
		
		return;
		
	}

	if(CamType == DRVIF1394TYPE)
	{
		ListAddInfoProperty_NetCam(pDevice);
		ListAddVideoFormat_Readable(pDevice);

		ListAddSnapMode_NetCam(pDevice);
		ListAddShutter_NetCam(pDevice);
		ListAddTriggerDelay_NetCam(pDevice);	//延迟曝光功能
		ListAddAntiFilter(pDevice);
		ListAddGain_NetCam(pDevice);
		ListAddWB(pDevice);
		ListAddADCControlNetCam(pDevice);
		
		ListAddTestImage(pDevice); 
		ListAddOutPutIoSet_NetCam(pDevice);	//输出IO功能设置
		ListAddOutPutIoControl_NetCam(pDevice);
		ListAddInPutIoSet(pDevice);		//输入IO功能设置
		ListAddFilterTime_Readable(pDevice);	
		ListAddSaveParam(pDevice);
		if (IS_HV_CAM(pDevice->m_HVTYPE) && pDevice->m_bInit != TRUE)
		{
			HV_VIDEO_MODE resolution = pDevice->GetVideoMode();
			CRect nAOI;
			HV_RES_GET_FMT7_MODE res;
			pDevice->GetFMT7Mode(resolution, FMT7_MODE_FUNC_ID_AOI, &res);

			nAOI.BottomRight() = CPoint(res.AOI.Left + res.AOI.Width, res.AOI.Height + res.AOI.Top);
			nAOI.TopLeft() = CPoint( res.AOI.Left, res.AOI.Top); 
			pDevice->SetAOI(nAOI);
			pDevice->m_bInit = TRUE;
		}
		
		return;
	}

	ListAddInfoProperty(pDevice);
    ListAddVideoFormat(pDevice);
	ListAddSnapMode(pDevice);
	ListAddShutter(pDevice);
	ListAddGain(pDevice);
	ListAddADCControl(pDevice);

	if (IS_SV1311(pDevice->m_pInfo->DeviceType())||
		IS_SV1400(pDevice->m_pInfo->DeviceType())||
		IS_SV1410(pDevice->m_pInfo->DeviceType())||
		IS_SV1420(pDevice->m_pInfo->DeviceType())||
		IS_SV2000(pDevice->m_pInfo->DeviceType())||
		IS_SV400(pDevice->m_pInfo->DeviceType())) 
	{
		if(pDevice->m_nOutPutIO_0_POL==1)
		{
			Spec_Fun_Interface_1(pDevice,OUTPUT_IO_0,2);
			Spec_Fun_Interface_1(pDevice,OUTPUT_IO_0_POL,pDevice->m_nOutPutIO_0_POL);
		}
		if(pDevice->m_nOutPutIO_1_POL==1)
		{
			Spec_Fun_Interface_1(pDevice,OUTPUT_IO_1,2);
			Spec_Fun_Interface_1(pDevice,OUTPUT_IO_1_POL,pDevice->m_nOutPutIO_1_POL);
		}
		if(pDevice->m_nOutPutIO_2_POL==1)
		{
			Spec_Fun_Interface_1(pDevice,OUTPUT_IO_2,2);
			Spec_Fun_Interface_1(pDevice,OUTPUT_IO_2_POL,pDevice->m_nOutPutIO_2_POL);
		}

		 ListAddTestImage(pDevice);   
         ListAddTriggerDelay(pDevice);	
		 ListAdd8or12BitMode(pDevice); //		 ListAddVideoFormat_Readable(pDevice);	// 8/12位数据
		 ListAddOutPutIoSet(pDevice);  
		 ListAddOutPutIoControl(pDevice);
		 ListAddInPutIoSet(pDevice);
         ListAddTransfersDelay(pDevice);
		 if (IS_SV1400(pDevice->m_pInfo->DeviceType())||
			 IS_SV1410(pDevice->m_pInfo->DeviceType())||
			 IS_SV1420(pDevice->m_pInfo->DeviceType())||
			 IS_SV2000(pDevice->m_pInfo->DeviceType())||
			 IS_SV400(pDevice->m_pInfo->DeviceType())) 
		 {
			 ListAddFilterTime(pDevice);	
		 }
		 
		 Spec_Fun_Interface_1(pDevice,OUTPUT_IO_0,pDevice->m_nOutPutIO_0);
		 Spec_Fun_Interface_1(pDevice,OUTPUT_IO_1,pDevice->m_nOutPutIO_1);
		 Spec_Fun_Interface_1(pDevice,OUTPUT_IO_2,pDevice->m_nOutPutIO_2);
		 Spec_Fun_Interface_1(pDevice,OUTPUT_IO_0_POL,pDevice->m_nOutPutIO_0_POL);
		 Spec_Fun_Interface_1(pDevice,OUTPUT_IO_1_POL,pDevice->m_nOutPutIO_1_POL);
		 Spec_Fun_Interface_1(pDevice,OUTPUT_IO_2_POL,pDevice->m_nOutPutIO_2_POL);
	}

	CString str;
	str.LoadString(IDS_INFOPROPERTY);	
	HPROPERTY prp=m_ListView.FindProperty(str);
	prp->Activate(PACT_EXPAND,0);

}
void CPropertyView::Set_Param(CDevice * pDevice)
{
	Spec_Fun_Interface_1(pDevice,STROBE_ON_OFF,pDevice->m_StrobeOnOff);
	
}

LRESULT CPropertyView::OnItemChanged(int idCtrl, LPNMHDR pnmh, BOOL& /*bHandled*/)
{
	LPNMPROPERTYITEM pnpi = (LPNMPROPERTYITEM) pnmh;

	CDevice *pDevice = m_DeviceManager.GetCurrentDevice();

	if (pDevice == NULL){
		return 0;
	}

	HVTYPE type = pDevice->m_pInfo->DeviceType();
	
	CComVariant vValue;
	pnpi->prop->GetValue(&vValue);
	vValue.ChangeType(VT_UINT);
	int index = vValue.uiVal;
	int id = m_ListView.GetItemData(pnpi->prop);//m_ListView.GetCurSel();

	// settings have changed and reparametrize camera

	switch(id) {
	//视频模式，分辨率选择
	case ITEM_VIDEOMODE:	
		if(IS_READABLE_CAM(type))		// modified by xupx 2009.10.10
			OnVideoModeChanged1420(pDevice, index);
		else
			OnVideoModeChanged(pDevice, index);
		break; 
	case ITEM_ANTI_FLICKER:
		OnAntiFlicker(pDevice, index);
		break;
	case ITEM_ANTI_FLICKER_SOURCE:
		OnAntiFlickerSource(pDevice, index);
		break;
	//采集方式，连续、采单帧
	case ITEM_SNAPMODE:
		OnSnapModeChanged(pDevice, index);
		break;
	//外触发新号极性
	case ITEM_TRIGGERPOLARITY:	
		OnTriggerPolarityChanged(pDevice, index);
		break;
	//闪光灯新号极性
	case ITEM_STROBEPOLARITY:
		OnStrobePolarityChanged(pDevice, index);
		break;
	case ITEM_ADCLEVEL:
//			if(IS_PD_CAM(type))
//				OnLutNetCam(pDevice, index);
//			else	//SV GC 
			OnADCLevelChanged(pDevice, index);
		break;
	case ITEM_BLACKLEVELENABLE:
		OnBlackLevelEnableChanged(pDevice, index);
		break;
		//速度单位	
	case ITEM_SHUTTERUNIT:
		if(IS_READABLE_CAM(type))
			OnShutterUnitChanged_NetCam(pDevice, index);
		else
			OnShutterUnitChanged(pDevice, index);
		break;
	case ITEM_FILTER_TIME_UNIT:
		OnFilterTimeUnitChanged(pDevice, index);
		break;
	case ITEM_FRAMEFROZEN:
		OnFrameFrozenChanged(pDevice, index);
		break;
		
		//闪光灯开关
	case ITEM_STROBE_ON_OFF:
		OnStrobeOnOffChanged(pDevice,index); 
		break;
	case ITEM_TEST_IMAGE:
		OnTestImageChanged(pDevice,index);
		break;
	// 触发延迟
	case ITEM_TRIGGERDELAY_STATE:	
		OnTriggerDelayChanged(pDevice,index);
		break;
	case ITEM_TRIGGERDELAY_UNIT:
		OnTriggerDelayUnitChange(pDevice, index);
		break;
	//自动曝光	
	case ITEM_SHUTTER_AUTO:
		OnShutterAuto(pDevice, index);
		break;
	//自动增益
	case ITEM_GAIN_AUTO:
		OnGainAuto(pDevice, index);
		break;
	//参数组
	case ITEM_BEGIN_SAVE:
		OnBeginSave1410GC(pDevice, index);
		break;
	case ITEM_SAVE_PARAM:
		OnSaveParam1410GC(pDevice, index);
		break;
	case ITEM_LOAD_PARAM:
		OnLoadParam1410GC(pDevice, index);
		break;
		
	case ITEM_OUTPUTIO_0:
		OnOutPutIO_0Changed(pDevice,index);   
		break;
	case ITEM_OUTPUTIO_1:
		OnOutPutIO_1Changed(pDevice,index);  
		break;
	case ITEM_OUTPUTIO_2:
		OnOutPutIO_2Changed(pDevice,index);  
		break;
	case ITEM_OUTPUTIO_0_CONTROL:       
		OnOutPutIO_ControlChanged(pDevice, OUTPUT_IO_0_POL, index);
		break;
	case ITEM_OUTPUTIO_1_CONTROL:       
		OnOutPutIO_ControlChanged(pDevice, OUTPUT_IO_1_POL, index);
		break;
	case ITEM_OUTPUTIO_2_CONTROL:       
		OnOutPutIO_ControlChanged(pDevice, OUTPUT_IO_2_POL, index);
		break;
	case ITEM_INPUTIOSET:
		OnInPutIOSetChanged(pDevice,index);
		break;
	case ITEM_8or12BitMode:
		On8or12BitModeChanged(pDevice,index);
		break;
	case ITEM_LE_BE_MODE:
		OnLeBeModeChanged(pDevice,index);
		break;
	case ITEM_CONNECT_SPEED:
		OnNetWorkSetting(pDevice, ITEM_CONNECT_SPEED, index);
		break;
	case ITEM_PASSIVE_TRANSMIT:
		OnAdvFeatureSetting(pDevice, ITEM_PASSIVE_TRANSMIT, index);
		break;
	case ITEM_DIGITAL_GAIN:
		Set_Adv_Feature(pDevice, ITEM_DIGITAL_GAIN, index);
		break;
	case ITEM_STROBE_AUTO:
		OnStrobeAuto(pDevice, index);
		break;
	// 白平衡 WB
	case ITEM_WB_ON_OFF:
		OnWBCtrl(pDevice, ITEM_WB_ON_OFF, index);
		UpdateWB(pDevice);	
		UpdateRGBChannel(pDevice);		
		break;
	case ITEM_WB_AUTO_MANU:
		OnWBCtrl(pDevice, ITEM_WB_AUTO_MANU, index);
		UpdateWB(pDevice);
		UpdateRGBChannel(pDevice);
		break;
	case ITEM_WB_ONE_PUSH:
		OnWBCtrl(pDevice, ITEM_WB_ONE_PUSH, 1);
		UpdateWB(pDevice);
		break;
		
	default:
		break;
	}
		
		return 0;
}
	
LRESULT CPropertyView::OnScalarChanged(int idCtrl, LPNMHDR pnmh, BOOL& /*bHandled*/)
{
	LPNMPROPERTYSCALAR pnpi = (LPNMPROPERTYSCALAR) pnmh;

	CDevice *pDevice = m_DeviceManager.GetCurrentDevice();
	if (pDevice == NULL){
		return 0;
	}

	int value = pnpi->value;
	int id = m_ListView.GetItemData(pnpi->prop);

	// settings have changed and reparametrize camera

	switch(id) {
	case SCALAR_SHUTTERSPEED:
		OnShutterSpeedChanged(pDevice, value);
		break;
	case SCALAR_BLACKLEVEL:
		OnBlackLevelChanged(pDevice, value);
		break;
	case SCALAR_GAINLEVEL:
		OnGainLevelChanged(pDevice, value);
		break;
    case SCALAR_RED_GAIN:
        OnWhiteBalanceVChanged(pDevice, value);	//add 1410
        break;
    case SCALAR_BLUE_GAIN:
        OnWhiteBalanceUChanged(pDevice, value);	//add 1410
        break;
	case SCALAR_GREEN_GAIN:
		OnWhiteBalanceGChanged(pDevice, value);	//add by xupx 1410
		break;
	case SCALAR_BLANKHOR:
		OnBlankHorChanged(pDevice, value);
		break;
	case SCALAR_BLANKVERT:
		OnBlankVertChanged(pDevice, value);
		break;
    case SCALAR_PACKET:							//包长调节
        m_MainFrame.OnPacketSizeChanged(pDevice, value);		
        break;
    case SCALAR_BRIGHTNESS:
        OnBrightnessChanged(pDevice, value);
        break;
	case SCALAR_FILTER_TIME:
		OnFilterTimeChanged(pDevice, value);
		break;
	case SCALAR_TRIGGER_NUM:
		OnMultiTriggerNumChanged(pDevice, value);
		break;
	case SCALAR_TRIGGER_TIME:
		OnMultiTriggerTimeChanged(pDevice, value);
		break;
	//1410GC
	case SCALAR_TRIGGERDELAY_VALUE:
		OnTriggerDelayValueChanged(pDevice, value);
		break;
	case SCALAR_TRANSDELAY_VALUE:
		OnTransfersDelayValueChanged(pDevice, value);
		break;
	case SCALAR_PACKET_DELAY:
		OnNetWorkSetting(pDevice, SCALAR_PACKET_DELAY, value);
		break;
	case SCALAR_EXPECT_DATA_RATE:
		OnNetWorkSetting(pDevice, SCALAR_EXPECT_DATA_RATE, value);
		break;
	case SCALAR_AC_PHASE_ADJUST:
		OnACPhaseAdjust(pDevice, value);
		break;

	// 白平衡 WB 控制
	case SCALAR_R_VALUE:
		OnWBCtrl(pDevice, SCALAR_R_VALUE, value);	//add by xupx 2009.07.02
		break;
	case SCALAR_B_VALUE:
		OnWBCtrl(pDevice, SCALAR_B_VALUE, value);	//add by xupx
		break;
	default:
		break;
	}

	return 0;
}

//------------------------------------------------------------------------------
// void CPropertyView::ListAddInfoProperty(CDevice* pDevice) 
// Author: 
//------------------------------------------------------------------------------
/**
* Insert a information property item into list view
*
* \param     pDevice point to a camera object
* \return    void
*
*/
//------------------------------------------------------------------------------
CString CPropertyView::DoOEM(CString strOEM, CString strModeName)
{
	int n, nEnd, len;
	CString str;
	n		= strOEM.Find(strModeName);
	if(n != -1)
	{
		len		= n + strModeName.GetLength() + 1;
		nEnd	= strOEM.Find("\n", len);
		str		= strOEM.Mid(len, (nEnd - len));
		strModeName.Format(str);
	}	

	return strModeName;
}

void CPropertyView::ListAddInfoProperty_NetCam(CDevice *pDevice)
{
	if(!pDevice->m_bInit)
	{
		pDevice->m_CamType		= pDevice->m_pInfo->DeviceType();
		pDevice->m_ModeName		= pDevice->m_pInfo->ModelName();
		pDevice->m_strNodeID	= pDevice->m_pInfo->NodeId();
		pDevice->m_VendorName	= pDevice->m_pInfo->VendorName();
		pDevice->m_strFPGAVer	= pDevice->m_pInfo->FPGAVersion1410GC(); 
		pDevice->m_strHWVer		= pDevice->m_pInfo->HardwareVersion1410GC();
		if(pDevice->m_pInfo->DeviceType() == DRVIF1394TYPE)
		{
			pDevice->m_strSWVer		= Spec_Fun_Get_SV(pDevice, STRING_ID_DEVICE_DRIVER_VERSION);
			pDevice->m_strSWVer		+= ";" + Spec_Fun_Get_SV(pDevice, STRING_ID_SYS_DRIVER_VERSION);
		}
		else
			pDevice->m_strSWVer		= Spec_Fun_Get_SV(pDevice, STRING_ID_DEVICE_DRIVER_VERSION);

		pDevice->m_strAPIVer	= Spec_Fun_Get_APIV(pDevice);
		pDevice->m_strDailtVer	= Spec_Fun_Get_DailtV(pDevice);
		pDevice->m_strColorMode	= pDevice->m_pInfo->ColorMode();
		if(IS_GIGE(pDevice->m_CamType) || IS_PD_CAM(pDevice->m_CamType))
		{
			pDevice->m_strIP			= Spec_Fun_Address_Get(pDevice, IDS_IP_ADDRESS);
			pDevice->m_strMAC			= Spec_Fun_Address_Get(pDevice, IDS_MAC_ADDRESS);
		}

		if(pDevice->m_pInfo->DeviceType() == DRVIFGIGETYPE)
		{
			pDevice->m_strHostIP		= Spec_Fun_Address_Get(pDevice, IDS_NETWORK_CARD_IP);
			pDevice->m_strHostMAC		= Spec_Fun_Address_Get(pDevice, IDS_NETWORK_CARD_MAC);
		}

		/***************** for OEM ********************/
		CString strOEM ;
		strOEM.LoadString(IDS_OEM_STRING);
		
		pDevice->m_ModeName = DoOEM(strOEM, pDevice->m_ModeName);
	}
	
/**********************/
//	Device Name
	CString str;
	str.LoadString(IDS_INFOPROPERTY);	
	HPROPERTY hName0=m_ListView.AddItem( PropCreateCategory(str) );
	
	
	str.LoadString(IDS_MODEL);
	HPROPERTY hName = m_ListView.AddItem( PropCreateReadOnlyItem(str, pDevice->m_ModeName) );
	if (hName){
		m_ListView.SetItemEnabled(hName, false);
	}
	
	str.LoadString(IDS_VENDOR);
	hName = m_ListView.AddItem( PropCreateReadOnlyItem(str, pDevice->m_VendorName) );
	if (hName){
		m_ListView.SetItemEnabled(hName, false);
	}

//	add serial number

	str.LoadString(IDS_NODEID);
	hName = m_ListView.AddItem( PropCreateReadOnlyItem(str, pDevice->m_strNodeID ) );
	if (hName){
		m_ListView.SetItemEnabled(hName, false);
	}
	
//	FPGA   xupx			
		str.LoadString(IDS_FPGA_VERSION);
		hName = m_ListView.AddItem( PropCreateReadOnlyItem(str, pDevice->m_strFPGAVer ) );
		if (hName){
			m_ListView.SetItemEnabled(hName, false);
		}
	
//	固件版本		
		str.LoadString(IDS_HARD_WARE_VERSION);
	
		hName = m_ListView.AddItem( PropCreateReadOnlyItem(str, pDevice->m_strHWVer ) );
		if (hName){
			m_ListView.SetItemEnabled(hName, false);
		}	
	
//	驱动版本;
	str.LoadString(IDS_SOFT_WARE_VER);	
	hName = m_ListView.AddItem( PropCreateReadOnlyItem(str, pDevice->m_strSWVer));
	if (hName){
		m_ListView.SetItemEnabled(hName, false);
	}

//----HVAPI版本
	{
		str.LoadString(IDS_HVAPI_VERSION);
		
		hName = m_ListView.AddItem( PropCreateReadOnlyItem(str, pDevice->m_strAPIVer));
		if (hName){
			m_ListView.SetItemEnabled(hName, false);
		}
	}
	
//----HVDAILT版本
	{
		str.LoadString(IDS_HVDAILT_VERSION);
		
		hName = m_ListView.AddItem( PropCreateReadOnlyItem(str, pDevice->m_strDailtVer));
		if (hName){
			m_ListView.SetItemEnabled(hName, false);
		}
	}

//----颜色格式
	str.LoadString(IDS_RGB_FORMAT);
	
	hName = m_ListView.AddItem( PropCreateReadOnlyItem(str, pDevice->m_strColorMode));  
	if (hName){
		m_ListView.SetItemEnabled(hName, false);
	}	

	if(IS_GIGE(pDevice->m_CamType) || IS_PD_CAM(pDevice->m_CamType))
	{
		//----IP 地址
		str.LoadString(IDS_IP_ADDRESS);
		hName = m_ListView.AddItem( PropCreateReadOnlyItem(str, pDevice->m_strIP));  
		if (hName){
			m_ListView.SetItemEnabled(hName, false);
		}
		//----MAC 地址
		str.LoadString(IDS_MAC_ADDRESS);
		hName = m_ListView.AddItem( PropCreateReadOnlyItem(str, pDevice->m_strMAC));  
		if (hName){
			m_ListView.SetItemEnabled(hName, false);
		}
	}


//----网卡IP地址
	if( pDevice->m_CamType == DRVIFGIGETYPE)
	{
		str.LoadString(IDS_NETWORK_CARD_IP);
		hName = m_ListView.AddItem( PropCreateReadOnlyItem(str, pDevice->m_strHostIP) );  
		if (hName){
			m_ListView.SetItemEnabled(hName, false);
		}

//----网卡MAC地址
		str.LoadString(IDS_NETWORK_CARD_MAC);
		hName = m_ListView.AddItem( PropCreateReadOnlyItem(str, pDevice->m_strHostMAC) );  
		if (hName){
			m_ListView.SetItemEnabled(hName, false);
		}
	}
}


void CPropertyView::ListAddInfoProperty(CDevice* pDevice)
{
	CString str;
	HVTYPE CamType = pDevice->m_pInfo->DeviceType();

	str.LoadString(IDS_INFOPROPERTY);
	
	HPROPERTY hName0=m_ListView.AddItem( PropCreateCategory(str) );
	
	
	str.LoadString(IDS_MODEL);
	HPROPERTY hName = m_ListView.AddItem( PropCreateReadOnlyItem(str, pDevice->m_pInfo->ModelName()) );
	if (hName){
		m_ListView.SetItemEnabled(hName, false);
	}
	
	str.LoadString(IDS_VENDOR);
	hName = m_ListView.AddItem( PropCreateReadOnlyItem(str, pDevice->m_pInfo->VendorName()) );
	if (hName){
		m_ListView.SetItemEnabled(hName, false);
	}
	
	str.LoadString(IDS_NODEID);
	CString strNodeId = pDevice->m_pInfo->NodeId();
	hName = m_ListView.AddItem( PropCreateReadOnlyItem(str, strNodeId ) );
	if (hName){
		m_ListView.SetItemEnabled(hName, false);
	}
	
	str.LoadString(IDS_HARD_WARE_VERSION);
	CString strHWVer = pDevice->m_pInfo->HardwareVersion(); 

    hName = m_ListView.AddItem( PropCreateReadOnlyItem(str, strHWVer ) );
	if (hName){
		m_ListView.SetItemEnabled(hName, false);
	}
	
//	str.Format("驱动版本");
	str.LoadString(IDS_SOFT_WARE_VER);
	
	BYTE buffer[256] = {0};

	pDevice->GetDeviceInfo(DESC_DEVICE_SOFTWARE_VERSION, buffer, 0); 
	
	CString strSWVer;
	strSWVer.Format("%s",buffer);
	hName = m_ListView.AddItem( PropCreateReadOnlyItem(str, strSWVer));
	if (hName){
		m_ListView.SetItemEnabled(hName, false);
	}
	
	str.LoadString(IDS_DEVICENAME);
	hName = m_ListView.AddItem( PropCreateReadOnlyItem(str, pDevice->m_pInfo->DeviceName()) );  
	if (hName){
		m_ListView.SetItemEnabled(hName, false);
	}
}

void CPropertyView::ListAddVideoFormat_Readable(CDevice* pDevice)
{
	CString str;
	
	//Add category item
	str.LoadString(IDS_VIDEOFORMAT);
	m_ListView.AddItem( PropCreateCategory(str) );

	m_CurrentUnit			= pDevice->m_pShutter->m_Unit.Value();
	m_CurrentShutterSpeed	= pDevice->m_pShutter->Value();
	
	if(IS_PD_CAM(pDevice->m_CamType) || pDevice->m_CamType == DRVIF1394TYPE)
	{
		if(!pDevice->m_bInit)
		{
			pDevice->m_CurrentModeIdx = pDevice->GetVideoMode();
			
			m_CurrentModeIdx = pDevice->m_CurrentModeIdx;
			m_CurrentMode = (HV_VIDEO_MODE)m_CurrentModeIdx;

			pDevice->UpdateImageSize(m_CurrentMode);	

		}
		
		//Make video format description

		LPCTSTR list[256] = { NULL }; 
		int i = 0;
		std::list<CString> strlist = pDevice->DeviceModeList();
		std::list<CString>::iterator pString;
		for ( i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
		{
			list[i++] = (*pString);
		}
		
		str.LoadString(IDS_VIDEOMODE);
		m_Idx2Mode.clear();
		m_Idx2Mode = pDevice->DeviceModeMap();

		for ( Idx2Mode_t::iterator it = m_Idx2Mode.begin(); it != m_Idx2Mode.end(); ++ it )
		{
			if (m_CurrentMode == it->second)
			{
				m_CurrentModeIdx = it->first;
				break;
			}
		}
		if ( m_CurrentModeIdx == -1  ) { m_CurrentModeIdx = 0; }
		m_CurrentMode = m_Idx2Mode[m_CurrentModeIdx];
		HPROPERTY hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_CurrentModeIdx) ); 
		if (hName){
			m_ListView.SetItemData(hName, ITEM_VIDEOMODE);
		}
	}
	
	if(pDevice->m_pInfo->DeviceType() == DRVIF1394TYPE)
	{
		ListAddPacketSize(pDevice);
//		return;				// by xupx 2009.10.13 所有相机都会加上数据格式选项
	}
	ListAdd8or12BitMode(pDevice); 
}



void CPropertyView::ListAddVideoFormat(CDevice* pDevice)
{
	if (IS_CCD_CAMERA(pDevice->m_pInfo->DeviceType()))
	{
		ListAddPacketSize(pDevice);
	}
	else
	{
		CString str;
		
		//Add category item
		str.LoadString(IDS_VIDEOFORMAT);
		m_ListView.AddItem( PropCreateCategory(str) );
		
		//Make video format description
		LPCTSTR list[256] = { NULL }; 
		int i = 0;
		std::list<CString> strlist = pDevice->DeviceModeList();
		std::list<CString>::iterator pString;
		for ( i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
		{
			list[i++] = (*pString);
		}
		
		str.LoadString(IDS_VIDEOMODE);
		m_Idx2Mode.clear();
		m_Idx2Mode = pDevice->DeviceModeMap();
		m_CurrentMode = pDevice->Format.Resolution();
		for ( Idx2Mode_t::iterator it = m_Idx2Mode.begin(); it != m_Idx2Mode.end(); ++ it )
		{
			if (m_CurrentMode == it->second)
			{
				m_CurrentModeIdx = it->first;
				break;
			}
		}
		if ( m_CurrentModeIdx == -1  ) { m_CurrentModeIdx = 0; }
		m_CurrentMode = m_Idx2Mode[m_CurrentModeIdx];
		HPROPERTY hName = m_ListView.AddItem( PropCreateList(str, list, m_CurrentModeIdx) ); 
		if (hName){
			m_ListView.SetItemData(hName, ITEM_VIDEOMODE);
		}
		
	}
}

void CPropertyView::ListAddShutter_NetCam(CDevice* pDevice)
{
	CString str;

	str.LoadString(IDS_SHUTTER);
	m_ListView.AddItem( PropCreateCategory(str) );

	if( !pDevice->m_bInit )
	{
		/***************读取快门单位、速度*******************/
		pDevice->m_ShutterRange.maxV = Spec_Fun_Get_Descript(pDevice, SCALAR_SHUTTERSPEED, 1);
		pDevice->m_ShutterRange.minV = Spec_Fun_Get_Descript(pDevice, SCALAR_SHUTTERSPEED, 0);
		pDevice->m_ShutterUnit		 = Spec_Fun_ADV_Get(pDevice, ITEM_SHUTTERUNIT);	//从相机读取“速度单位”
		pDevice->m_ShutterSpeed		 = Spec_Fun_Get(pDevice, SCALAR_SHUTTERSPEED);	//从相机读取“快门速度” m_ShutterSpeed
		/*******************读取快门自动功能********************/
		if(pDevice->m_flagAutoShutter == 1)
		{
			pDevice->m_bShutterAutoInq = Spec_Fun_Get_Descript(pDevice, ITEM_SHUTTER_AUTO, 0);
			pDevice->m_bShutterAuto		 = Spec_Fun_Get(pDevice, ITEM_SHUTTER_AUTO);
		}
		/******************设置相机快门单位、速度*********************/
		pDevice->m_pShutter->m_Unit.Set((TUnit)pDevice->m_ShutterUnit);
		pDevice->m_pShutter->Set(pDevice->m_ShutterSpeed);		//设置的过程中进行了值传递。
	}

	{		
	// 速度单位
		std::list<CString> strlist;
		strlist.clear();
		strlist.push_back("us");
		strlist.push_back("ms");
		
		int i = 0;
		LPCTSTR list[256] = { NULL }; 
		std::list<CString>::iterator pString = NULL;
		for (i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
		{
			list[i++] = (*pString);
		}
		list[i] = NULL;
		
		str.LoadString(IDS_TIME_UNIT);
		HPROPERTY hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_ShutterUnit) );
		if (hName){
			m_ListView.SetItemData(hName, ITEM_SHUTTERUNIT);
			if(IS_GM_CAM(pDevice->m_pInfo->DeviceType()))
				m_ListView.SetItemEnabled(hName, 1);
			else
				m_ListView.SetItemEnabled(hName, !pDevice->m_bShutterAuto);		
		}
		
	// 快门速度	
		TRACKBARINFO info;
		info.RangeMax = pDevice->m_ShutterRange.maxV;
		info.RangeMin = pDevice->m_ShutterRange.minV;
		info.LineSize = 1; 
		
		str.LoadString(IDS_SHUTTERSPEED);
		hName = m_ListView.AddItem( PropCreateTrack(str, pDevice->m_ShutterSpeed, info ));
		if (hName){
			m_ListView.SetItemData(hName, SCALAR_SHUTTERSPEED);
			if(IS_GM_CAM(pDevice->m_pInfo->DeviceType()))
				m_ListView.SetItemEnabled(hName, 1);
			else
			{
				m_ListView.SetItemEnabled(hName, !pDevice->m_bShutterAuto);		
			}
		}
	}
	// 自动模式
	if(pDevice->m_flagAutoShutter)
	{
			std::list<CString> strlist;
			strlist.clear();
			strlist.push_back("Disable");
			strlist.push_back("Enable");
			
			LPCTSTR list[256] = { NULL }; 
			std::list<CString>::iterator pString = NULL;
			int i =0;
			
			for ( i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
			{
				list[i++] = (*pString);
			}
			list[i] = NULL;
			
			str.LoadString(IDS_SHUTTER_AUTO);
			HPROPERTY hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_bShutterAuto) );
			if (hName){
				m_ListView.SetItemData(hName, ITEM_SHUTTER_AUTO);
				m_ListView.SetItemEnabled(hName, pDevice->m_bShutterAutoInq);	// modified by xupx 2009.04.24
			}
	}
	UpdateShutter(pDevice);
}


void CPropertyView::ListAddShutter(CDevice* pDevice)
{
	if(pDevice->m_pInfo->DeviceType() == DRVIF1394TYPE)
	{
		ListAddShutter_NetCam(pDevice);
		return;
	}

	CString str;

	str.LoadString(IDS_SHUTTER);
	m_ListView.AddItem( PropCreateCategory(str) );
	
	std::list<CString> strlist;
	strlist.push_back("us");
	strlist.push_back("ms");

	if (!IS_CMOS_1394_CAM((pDevice->m_pInfo->DeviceType()))
		&&!(IS_CCD_CAMERA(pDevice->m_pInfo->DeviceType()))) {
        strlist.push_back("s");
    }
	
	int i = 0;
	LPCTSTR list[256] = { NULL }; 
	std::list<CString>::iterator pString = NULL;
	for (i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
	{
		list[i++] = (*pString);
	}
	list[i] = NULL;
	m_CurrentUnit = pDevice->m_pShutter->m_Unit.Value();
	if (IS_CCD_CAMERA(pDevice->m_pInfo->DeviceType()))
	{
		pDevice->SetShutterUnit(m_CurrentUnit);
	}

	str.LoadString(IDS_TIME_UNIT);
	HPROPERTY hName = m_ListView.AddItem( PropCreateList(str, list, m_CurrentUnit) );
	if (hName){
		m_ListView.SetItemData(hName, ITEM_SHUTTERUNIT);
		m_ListView.SetItemEnabled(hName, pDevice->m_pShutter->IsSupported());
	}

	TRACKBARINFO info;
	info.RangeMin = pDevice->m_pShutter->Min();
	info.RangeMax = pDevice->m_pShutter->Max();
	info.LineSize = 1; 
	m_CurrentShutterSpeed = pDevice->m_pShutter->Value();

	str.LoadString(IDS_SHUTTERSPEED);
	hName = m_ListView.AddItem( PropCreateTrack(str, m_CurrentShutterSpeed, info ));
	if (hName){
		m_ListView.SetItemData(hName, SCALAR_SHUTTERSPEED);
		bool b = pDevice->m_pShutter->IsSupported();
		m_ListView.SetItemEnabled(hName, b);
	}

}

void CPropertyView::ListAdvTrigger(CDevice *pDevice)
{
	if(pDevice->m_flagMultiTrigger == 0)
		return;
	
	CString str;
	TRACKBARINFO info;
	{
		if(!pDevice->m_bInit)
		{
			pDevice->m_TriggerTime					= Spec_Fun_ADV_Get(pDevice, SCALAR_TRIGGER_TIME);
			pDevice->m_TriggerNum					= Spec_Fun_ADV_Get(pDevice, SCALAR_TRIGGER_NUM);
			
			pDevice->m_MultiTriggerNumRange.maxV	= Spec_Fun_Get_AdvDescript(pDevice, SCALAR_TRIGGER_NUM, 1);
			pDevice->m_MultiTriggerNumRange.minV	= Spec_Fun_Get_AdvDescript(pDevice, SCALAR_TRIGGER_NUM, 0);
			pDevice->m_MultiTriggerTimeRange.maxV	= Spec_Fun_Get_AdvDescript(pDevice, SCALAR_TRIGGER_TIME, 1);
			pDevice->m_MultiTriggerTimeRange.minV	= Spec_Fun_Get_AdvDescript(pDevice, SCALAR_TRIGGER_TIME, 0);
			pDevice->m_MultiTriggerOnOffInq			= Spec_Fun_Get_AdvDescript(pDevice, ITEM_MULTI_TRIGGER_ONOFF, 0); // 2009.06.25
		}
		
		int nV = pDevice->m_TriggerNum;
		info.RangeMin = pDevice->m_MultiTriggerNumRange.minV;
		info.RangeMax = pDevice->m_MultiTriggerNumRange.maxV;
		info.LineSize = 1; 
		
		str.LoadString(IDS_SNAP_TRIGGER_NUM);
		HPROPERTY hName = m_ListView.AddItem( PropCreateTrack(str, nV, info ));	
		if (hName){
			m_ListView.SetItemData(hName, SCALAR_TRIGGER_NUM);		
			m_ListView.SetItemEnabled(hName, pDevice->m_MultiTriggerOnOffInq);	// 2009.06.25
		}
	}
	//
	{
		info.RangeMin = pDevice->m_MultiTriggerTimeRange.minV;
		info.RangeMax = pDevice->m_MultiTriggerTimeRange.maxV;
		info.LineSize = 1; 
		
		int nV = pDevice->m_TriggerTime;	
		str.LoadString(IDS_TRIGGER_TIME);
		HPROPERTY hName = m_ListView.AddItem( PropCreateTrack(str, nV, info ));	
		if (hName){
			m_ListView.SetItemData(hName, SCALAR_TRIGGER_TIME);	//TEST	
			m_ListView.SetItemEnabled(hName, pDevice->m_pGain->IsSupported());
			
		}		
	}
}



void CPropertyView::ListAddSnapMode_NetCam(CDevice *pDevice)
{
	if(!pDevice->m_bInit)
	{
		pDevice->m_CurrentSnapMode			= pDevice->GetParameter(IDS_SNAPMODE, 0);//Spec_Fun_Get(pDevice, Get_SnapMode);
		pDevice->m_CurrentTriggerPolarity	= pDevice->GetParameter(IDS_TRIGGERPOLARITY, 0);
		//读取闪光灯OnOff
		pDevice->m_StrobeOnOff		= pDevice->GetParameter(IDS_STROBEPOLARITY, 0);
		//读取闪光灯极性
		pDevice->m_CurrentStrobePolarity		= pDevice->GetParameter(IDS_STROBEPOLARITY, 1);
		Spec_Fun_Get_Descript(pDevice, ITEM_SNAPMODE, 1);	// 查询所支持的Trigger模式。 2009.10.10

		/************自动闪光灯功能*************/	
		if(pDevice->m_flagAutoStrobe == 1)
		{
			pDevice->m_bStrobeAutoInq = Spec_Fun_AutoDescript(pDevice, ITEM_STROBE_AUTO,0);
			pDevice->m_bStrobeAuto	  = Spec_Fun_Get_AutoFunc(pDevice, ITEM_STROBE_AUTO);
		}	
	}

	CString str;
	str.LoadString(IDS_SNAPMODE);
	m_ListView.AddItem( PropCreateCategory(str) );
	std::list<CString> strlist;
	strlist.push_back("Continuation");
	strlist.push_back("Trigger");
	if (IS_HV_CAM(pDevice->m_HVTYPE))
	{
		if(pDevice->m_resFeatureDes.Trigger.Mode1Inq)
			strlist.push_back("Trigger Edge");
		if(pDevice->m_resFeatureDes.Trigger.Mode2Inq)
			strlist.push_back("Trigger Level");
	}
    
	int i = 0;
	LPCTSTR list[256] = { NULL }; 
	std::list<CString>::iterator pString = NULL;
	for (i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
	{
		list[i++] = (*pString);
	}
	list[i] = NULL;

	HPROPERTY hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_CurrentSnapMode) );
	if (hName){
		m_ListView.SetItemData(hName, ITEM_SNAPMODE);
		m_ListView.SetItemEnabled(hName, pDevice->m_pSnapMode->IsSupported());
	}
//---外触发信号极性
	strlist.clear();
	strlist.push_back("Low");
	strlist.push_back("High");
	for ( i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
	{
		list[i++] = (*pString);
	}
	list[i] = NULL;
	str.LoadString(IDS_TRIGGERPOLARITY);
	hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_CurrentTriggerPolarity) );
	if (hName){
		m_ListView.SetItemData(hName, ITEM_TRIGGERPOLARITY);
		m_ListView.SetItemEnabled(hName, 1);
	}
//---闪光灯信号极性
	str.LoadString(IDS_STROBEPOLARITY);
	hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_CurrentStrobePolarity) );
	if (hName){
		m_ListView.SetItemData(hName, ITEM_STROBEPOLARITY);
		m_ListView.SetItemEnabled(hName, 1);
	}
////////////闪光灯开关////////////////////////
	{
		strlist.clear();
		strlist.push_back("off");
		strlist.push_back("on");
		
		for ( i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
		{
			list[i++] = (*pString);
		}
		list[i] = NULL;
		str.LoadString(IDS_STROBE_SWITCH);
		
		hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_StrobeOnOff) );
		if (hName){
			m_ListView.SetItemData(hName, ITEM_STROBE_ON_OFF);
			//在Trigger状态，且，打开自动闪光灯时，此项为灰色不可操作
			if((pDevice->m_CurrentSnapMode != 0 ) && (pDevice->m_bStrobeAuto))
				m_ListView.SetItemEnabled(hName, 0);
			else
				m_ListView.SetItemEnabled(hName, 1);
		}	
	}
//////////////// 闪光灯自动功能 ////////////////////////////////////////////
	if(pDevice->m_flagAutoStrobe == 1)
	{
		strlist.clear();
		strlist.push_back("Disable");
		strlist.push_back("Enable");
		
		for ( i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
		{
			list[i++] = (*pString);
		}
		list[i] = NULL;
		str.LoadString(IDS_STROBE_AUTO);
		
		hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_bStrobeAuto) );
		if (hName){
			m_ListView.SetItemData(hName, ITEM_STROBE_AUTO);
			m_ListView.SetItemEnabled(hName, pDevice->m_bStrobeAutoInq);
		}	
	}
	
//////////////////////////////////////////////////////////////////////////

	if(IS_READABLE_CAM(pDevice->m_CamType))		// 2009.10.16
	{
		ListAdvTrigger(pDevice);
	}

}


void CPropertyView::ListAddPassiveTransmit(CDevice *pDevice)
{
	if(!pDevice->m_bInit)
	{
		//查询是否有此功能
		if (pDevice->m_flagPassive == 0)
			return;
		
		pDevice->m_PassiveTransOnOff = Spec_Fun_ADV_Get(pDevice, ITEM_PASSIVE_TRANSMIT);
		if(pDevice->m_PassiveTransOnOff)
		{
			pDevice->m_PassiveTransBufNum = Spec_Fun_ADV_Get(pDevice, ID_DO_PASSIVE_TRANS);
		}
	}
	if (pDevice->m_flagPassive == 0)
		return;
		
	CString str;
	str.LoadString(IDS_PASSIVE_TRANSMIT);
	m_ListView.AddItem( PropCreateCategory(str) );
	std::list<CString> strlist;
	strlist.push_back("Disable");
	strlist.push_back("Enable");

	int i = 0;
	LPCTSTR list[256] = { NULL }; 
	std::list<CString>::iterator pString = NULL;
	for (i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
	{
		list[i++] = (*pString);
	}
	list[i] = NULL;
	
	str.LoadString(IDS_PASSIVE_TRANSMIT_ENABLE);
	HPROPERTY hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_PassiveTransOnOff) );
	if (hName){
		m_ListView.SetItemData(hName, ITEM_PASSIVE_TRANSMIT);
		m_ListView.SetItemEnabled(hName, pDevice->m_pSnapMode->Value()!=0);
	}
	
}


void CPropertyView::ListAddSnapMode(CDevice* pDevice)
{
	CString str;
	str.LoadString(IDS_SNAPMODE);
	m_ListView.AddItem( PropCreateCategory(str) );
	std::list<CString> strlist;
	strlist.push_back("Continuation");
	strlist.push_back("Trigger");
  	
    if (!(IS_HV1300(pDevice->m_pInfo->DeviceType()))
		&&!(IS_HV2000(pDevice->m_pInfo->DeviceType()))
		&&!(IS_CCD_CAMERA(pDevice->m_pInfo->DeviceType()))) 
	{
	    strlist.push_back("Trigger Edge");
    }
    	
    if (!(IS_HV1300(pDevice->m_pInfo->DeviceType()))
		&&!(IS_HV2000(pDevice->m_pInfo->DeviceType()))
		&& !(IS_ITS1394(pDevice->m_pInfo->DeviceType())) )
	{
	    strlist.push_back("Trigger Level");
    }
    
	int i = 0;
	LPCTSTR list[256] = { NULL }; 
	std::list<CString>::iterator pString = NULL;
	for (i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
	{
		list[i++] = (*pString);
	}
	list[i] = NULL;
	m_CurrentSnapMode = pDevice->m_pSnapMode->Value();


    if (IS_CCD_CAMERA(pDevice->m_pInfo->DeviceType())) 
	{
        if (m_CurrentSnapMode == 3) {
            m_CurrentSnapMode--;
        }
    }

	HPROPERTY hName = m_ListView.AddItem( PropCreateList(str, list, m_CurrentSnapMode) );
	if (hName){
		m_ListView.SetItemData(hName, ITEM_SNAPMODE);
		m_ListView.SetItemEnabled(hName, pDevice->m_pSnapMode->IsSupported());
	}

	strlist.clear();
	strlist.push_back("Low");
	strlist.push_back("High");
	for ( i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
	{
		list[i++] = (*pString);
	}
	list[i] = NULL;
	m_CurrentTriggerPolarity = pDevice->m_pTriggerPolarity->Value();
	str.LoadString(IDS_TRIGGERPOLARITY);
	hName = m_ListView.AddItem( PropCreateList(str, list, m_CurrentTriggerPolarity) );
	if (hName){
		m_ListView.SetItemData(hName, ITEM_TRIGGERPOLARITY);
		m_ListView.SetItemEnabled(hName, 1);
	}

	m_CurrentStrobePolarity = pDevice->m_pStrobePolarity->Value();
	str.LoadString(IDS_STROBEPOLARITY);
	hName = m_ListView.AddItem( PropCreateList(str, list, m_CurrentStrobePolarity) );
	if (hName){
		m_ListView.SetItemData(hName, ITEM_STROBEPOLARITY);
		m_ListView.SetItemEnabled(hName, 1);
	}
/////////////////////////////////////////
	if (IS_SV1311(pDevice->m_pInfo->DeviceType())||
		IS_SV1420(pDevice->m_pInfo->DeviceType())||
		IS_SV2000(pDevice->m_pInfo->DeviceType())||
		IS_SV1400(pDevice->m_pInfo->DeviceType())||
		IS_SV1410(pDevice->m_pInfo->DeviceType())||
		IS_SV1310(pDevice->m_pInfo->DeviceType())||
		IS_SV1300(pDevice->m_pInfo->DeviceType())||
		IS_SV400(pDevice->m_pInfo->DeviceType()))
	{
        Spec_Fun_Interface_1(pDevice,STROBE_ON_OFF,pDevice->m_StrobeOnOff);
		strlist.clear();
		strlist.push_back("off");
		strlist.push_back("on");
		
		for ( i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
		{
			list[i++] = (*pString);
		}
		list[i] = NULL;
		str.LoadString(IDS_STROBE_SWITCH);
		
		hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_StrobeOnOff) );
		if (hName){
			m_ListView.SetItemData(hName, ITEM_STROBE_ON_OFF);
			m_ListView.SetItemEnabled(hName, 1);
		}
	
	}
	
///////////////////////////////////////

    if (IS_CCD_CAMERA(pDevice->m_pInfo->DeviceType())){
		
		std::list<CString> strlist;
		strlist.push_back("off");
		strlist.push_back("on");
				
		int i = 0;
		LPCTSTR list[256] = { NULL }; 
		std::list<CString>::iterator pString = NULL;
		for (i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
		{
			list[i++] = (*pString);
		}
		list[i] = NULL;
		
		str.LoadString(IDS_FRAME_ROZEN);
		HPROPERTY hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_FrameFrozen) );
		if (hName){
			m_ListView.SetItemData(hName, ITEM_FRAMEFROZEN);
			m_ListView.SetItemEnabled(hName, pDevice->m_pShutter->IsSupported());
		}
    }
}


void CPropertyView::ListAddADCControlNetCam(CDevice *pDevice)
{
    CString str;
	HPROPERTY hName;
    str.LoadString(IDS_ADCONTROL);

	if(pDevice->m_flagDigitalGain == 1 && !IS_HV_CAM(pDevice->m_HVTYPE)) //支持数值增益功能 // by xupx 2009.10.10
	{
		m_ListView.AddItem( PropCreateCategory(str) );
	//数字增益
		if(!pDevice->m_bInit)
		{
			int nGain = Spec_Fun_ADV_Get(pDevice, ITEM_DIGITAL_GAIN);
			pDevice->m_nDigitalGainItem = DigitalGain2Item[nGain];
			///当有此功能时，查询此功能是否可用。
			pDevice->m_bDigitalGainInq  = Spec_Fun_Get_AdvDescript(pDevice, ITEM_DIGITAL_GAIN,0);
		}

        std::list<CString> strlist;
		strlist.clear();
		if(pDevice->m_bDigitalGainInq == 1)
		{
			strlist.push_back("4 -- 11");
			strlist.push_back("3 -- 10");
			strlist.push_back("2 --  9");
			strlist.push_back("1 --  8");
			strlist.push_back("0 --  7");
		}
		else		//pDevice->m_ColorCode == COLOR_RAW12 || pDevice->m_ColorCode == COLOR_RAW12_PACKED)
		{
			strlist.push_back("0 -- 11");
		}
		
        int i = 0;
        LPCTSTR list[256] = { NULL }; 
        std::list<CString>::iterator pString = NULL;
        for ( i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
        {
            list[i++] = (*pString);
        }
		list[i] = NULL;
		
		
		str.LoadString(IDS_DIGITAL_GAIN);
		if(pDevice->m_bDigitalGainInq == 1 )
		{
			hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_nDigitalGainItem) );
		}
		else
		{
			hName = m_ListView.AddItem( PropCreateList(str, list, 0) );
		}
			if (hName){
				m_ListView.SetItemData(hName, ITEM_DIGITAL_GAIN);
				/// modified by xupx 2009.04.15
				m_ListView.SetItemEnabled(hName, pDevice->m_bDigitalGainInq);
			}
	}
	if(pDevice->m_flagLUT == 1)
	{
		m_ListView.AddItem( PropCreateCategory(str) );
		//* LUT 文件方式
		str.LoadString(IDS_LUTPATH);
		hName = m_ListView.AddItem( PropCreateFileName(str, pDevice->m_LutPath, 0));
		if (hName){
			m_ListView.SetItemData(hName, ITEM_ADCLEVEL);
			m_ListView.SetItemEnabled(hName, TRUE);
		}
	}

//亮度调节		
	if(IS_PD_CAM(pDevice->m_pInfo->DeviceType()))
	{
		m_ListView.AddItem( PropCreateCategory(str) );

		if(!pDevice->m_bInit)
		{
			if(!pDevice->m_bInit)
			{
				pDevice->m_BrightnessRange.maxV = Spec_Fun_Get_Descript(pDevice, SCALAR_BRIGHTNESS, 1);
				pDevice->m_BrightnessRange.minV = Spec_Fun_Get_Descript(pDevice, SCALAR_BRIGHTNESS, 0);
			}

			pDevice->m_Brightness = Spec_Fun_Get(pDevice, SCALAR_BRIGHTNESS);
		}
		
		TRACKBARINFO info;
		info.RangeMax = pDevice->m_BrightnessRange.maxV;
		info.RangeMin = pDevice->m_BrightnessRange.minV;
		info.LineSize = 1; 
				
		str.LoadString(IDS_BRIGHTNESS_VAL);
		hName = m_ListView.AddItem( PropCreateTrack(str, pDevice->m_Brightness, info ));	
		if (hName){
			m_ListView.SetItemData(hName, SCALAR_BRIGHTNESS);
			m_ListView.SetItemEnabled(hName, 1);
		}    
	}

	if(pDevice->m_flagDigitalGain == 1 && IS_HV_CAM(pDevice->m_HVTYPE))
	{
		m_ListView.AddItem( PropCreateCategory(str) );
		//数字增益
		if(!pDevice->m_bInit)
		{
			int nGain = Spec_Fun_ADV_Get(pDevice, ITEM_DIGITAL_GAIN);
			pDevice->m_nDigitalGainItem = DigitalGain2Item[nGain];
			///当有此功能时，查询此功能是否可用。
			pDevice->m_bDigitalGainInq  = Spec_Fun_Get_AdvDescript(pDevice, ITEM_DIGITAL_GAIN,0);
		}

        std::list<CString> strlist;
        strlist.clear();
        strlist.push_back("ADC Level 0");
        strlist.push_back("ADC Level 1");
        strlist.push_back("ADC Level 2");
        strlist.push_back("ADC Level 3");
        
        int i = 0;
        LPCTSTR list[256] = { NULL }; 
        std::list<CString>::iterator pString = NULL;
        for ( i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
        {
            list[i++] = (*pString);
        }
        list[i] = NULL;
        str.LoadString(IDS_ADCLEVEL);
        HPROPERTY hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_nDigitalGainItem));
        if (hName){
            m_ListView.SetItemData(hName, ITEM_DIGITAL_GAIN);
            m_ListView.SetItemEnabled(hName, pDevice->m_bDigitalGainInq);
        }
		
		// 添加AD控制中的黑电平功能  add by xupx 2009.10.10
		if (pDevice->m_flagGlobalBlk)			// add by xupx 2009.10.16
		{
			strlist.clear();
			strlist.push_back("Disable");
			strlist.push_back("Enable");
			for ( i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
			{
				list[i++] = (*pString);
			}
			list[i] = NULL;
			m_CurrentBlackLevelEnable = Spec_Fun_ADV_Get(pDevice, ITEM_BLACKLEVELENABLE);//pDevice->m_pBlackLevelEnable->Value();
			str.LoadString(IDS_BLACKLEVELENABLE);
			hName = m_ListView.AddItem( PropCreateList(str, list, m_CurrentBlackLevelEnable) );
			if (hName){
				m_ListView.SetItemData(hName, ITEM_BLACKLEVELENABLE);
				m_ListView.SetItemEnabled(hName, pDevice->m_pBlackLevelEnable->IsSupported());
			}
			
			TRACKBARINFO info;
			info.RangeMin = Spec_Fun_Get_AdvDescript(pDevice, SCALAR_BLACKLEVEL, 0) - 255;//pDevice->m_pBlackLevel->Min();
			info.RangeMax = Spec_Fun_Get_AdvDescript(pDevice, SCALAR_BLACKLEVEL, 1) - 256;//pDevice->m_pBlackLevel->Max();
			info.LineSize = 1; //added by HYL 2007.02.07
			m_CurrentBlackLevel = Spec_Fun_ADV_Get(pDevice, SCALAR_BLACKLEVEL) - 255; //pDevice->m_pBlackLevel->Value();
			long temp = m_CurrentBlackLevel;
			str.LoadString(IDS_BLACKLEVEL);
			hName = m_ListView.AddItem( PropCreateTrack(str, temp, info ));	
			if (hName){
				m_ListView.SetItemData(hName, SCALAR_BLACKLEVEL);
				m_ListView.SetItemEnabled(hName, m_CurrentBlackLevelEnable);
			}
		}
	}
// end
}


void CPropertyView::ListAddADCControl(CDevice* pDevice)
{
    if (IS_CCD_CAMERA(pDevice->m_pInfo->DeviceType())) {
				
        CString str;
        str.LoadString(IDS_ADCONTROL);
        m_ListView.AddItem( PropCreateCategory(str) );
		
		//-----------写历史记录------
		DWORD Readed = 0;
		HANDLE hFile = ::CreateFile(pDevice->m_LutPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		ReadFile(hFile, pDevice->m_dwLut, 16*1024, &Readed, NULL);
		CloseHandle(hFile);
		if (Readed == 16*1024) {
			pDevice->LoadLut(pDevice->m_dwLut, 16*1024,0);
		}
		
        //----------------------------
		
		//* LUT 文件方式
        str.LoadString(IDS_LUTPATH);
        HPROPERTY hName = m_ListView.AddItem( PropCreateFileName(str, pDevice->m_LutPath, 0));
        if (hName){
            m_ListView.SetItemData(hName, ITEM_ADCLEVEL);
            m_ListView.SetItemEnabled(hName, IS_CCD_CAMERA(pDevice->m_pInfo->DeviceType()));
        }
        //PropCreateFileName
		//*/
		
		TRACKBARINFO info;
		info.RangeMax = 255;
		info.RangeMin = 0;
		info.LineSize = 1; 
		
		str.LoadString(IDS_BRIGHTNESS_VAL);
		hName = m_ListView.AddItem( PropCreateTrack(str, pDevice->m_Brightness, info ));	
		if (hName){
			m_ListView.SetItemData(hName, SCALAR_BRIGHTNESS);
			m_ListView.SetItemEnabled(hName, IS_CCD_CAMERA(pDevice->m_pInfo->DeviceType()));
		}
    } 
	else 
	{
        CString str;
        
        str.LoadString(IDS_ADCONTROL);
        m_ListView.AddItem( PropCreateCategory(str) );

        std::list<CString> strlist;
        strlist.clear();
        strlist.push_back("ADC Level 0");
        strlist.push_back("ADC Level 1");
        strlist.push_back("ADC Level 2");
        strlist.push_back("ADC Level 3");
        
        int i = 0;
        LPCTSTR list[256] = { NULL }; 
        std::list<CString>::iterator pString = NULL;
        for ( i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
        {
            list[i++] = (*pString);
        }
        list[i] = NULL;
        m_CurrentADLevel = pDevice->m_pADCLevel->Value();
        str.LoadString(IDS_ADCLEVEL);
        HPROPERTY hName = m_ListView.AddItem( PropCreateList(str, list, m_CurrentADLevel) );
        if (hName){
            m_ListView.SetItemData(hName, ITEM_ADCLEVEL);
            m_ListView.SetItemEnabled(hName, pDevice->m_pADCLevel->IsSupported());
        }

	    strlist.clear();
	    strlist.push_back("Disable");
	    strlist.push_back("Enable");
	    for ( i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
	    {
		    list[i++] = (*pString);
	    }
	    list[i] = NULL;
	    m_CurrentBlackLevelEnable = pDevice->m_pBlackLevelEnable->Value();
	    str.LoadString(IDS_BLACKLEVELENABLE);
	    hName = m_ListView.AddItem( PropCreateList(str, list, m_CurrentBlackLevelEnable) );
	    if (hName){
		    m_ListView.SetItemData(hName, ITEM_BLACKLEVELENABLE);
		    m_ListView.SetItemEnabled(hName, pDevice->m_pBlackLevelEnable->IsSupported());
	    }

	    TRACKBARINFO info;
	    info.RangeMin = pDevice->m_pBlackLevel->Min();
	    info.RangeMax = pDevice->m_pBlackLevel->Max();
		info.LineSize = 1; //added by HYL 2007.02.07
	    m_CurrentBlackLevel = pDevice->m_pBlackLevel->Value();
	    str.LoadString(IDS_BLACKLEVEL);
	    hName = m_ListView.AddItem( PropCreateTrack(str, m_CurrentBlackLevel, info ));	
	    if (hName){
		    m_ListView.SetItemData(hName, SCALAR_BLACKLEVEL);
		    m_ListView.SetItemEnabled(hName, m_CurrentBlackLevelEnable);
	    }
    }
}


void CPropertyView::ListAddGain_NetCam(CDevice* pDevice)
{
	CString str;
	TRACKBARINFO info;
	
	str.LoadString(IDS_GAIN);
	m_ListView.AddItem( PropCreateCategory(str) );
	
	if(!pDevice->m_bInit)
	{
		
		/*****************添加增益列表**********************/
		pDevice->m_GainRange.maxV = Spec_Fun_Get_Descript(pDevice, SCALAR_GAINLEVEL, 1);
		pDevice->m_GainRange.minV = Spec_Fun_Get_Descript(pDevice, SCALAR_GAINLEVEL, 0);
		pDevice->m_Gain = Spec_Fun_Get(pDevice, SCALAR_GAINLEVEL);
		/****************自动功能列表***********************/
		if(pDevice->m_flagAutoGain == 1)
		{
			pDevice->m_bGainAuto = Spec_Fun_Get(pDevice, ITEM_GAIN_AUTO);		
		}
		/***************红、绿、蓝通道分量******************/
		if(	pDevice->m_flagSeparateGain == 1)
		{
			pDevice->m_SeperateGain_Range.maxV = Spec_Fun_Get_AdvDescript(pDevice, SCALAR_GREEN_GAIN, 1);
			pDevice->m_SeperateGain_Range.minV = Spec_Fun_Get_AdvDescript(pDevice, SCALAR_GREEN_GAIN, 0);
		
			pDevice->m_GreenGain = Spec_Fun_ADV_Get(pDevice, SCALAR_GREEN_GAIN);
			pDevice->m_RedGain	 = Spec_Fun_ADV_Get(pDevice, SCALAR_RED_GAIN);
			pDevice->m_BlueGain	 = Spec_Fun_ADV_Get(pDevice, SCALAR_BLUE_GAIN);
		}
	}

	
	//添加界面
	/*************增益的自动模式*******************/
	if(pDevice->m_flagAutoGain == 1)
	{
		if(!pDevice->m_bInit){
			pDevice->m_bGainAutoInq = Spec_Fun_Get_Descript(pDevice, ITEM_GAIN_AUTO, 0);
		}
		
		std::list<CString> strlist;
		strlist.clear();
		strlist.push_back("Disable");
		strlist.push_back("Enable");
		
		LPCTSTR list[256] = { NULL }; 
		std::list<CString>::iterator pString = NULL;
		int i =0;
		
		for ( i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
		{
			list[i++] = (*pString);
		}
		list[i] = NULL;
		
		str.LoadString(IDS_GAIN_AUTO);
		HPROPERTY hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_bGainAuto) );
		if (hName){
			m_ListView.SetItemData(hName, ITEM_GAIN_AUTO);
			m_ListView.SetItemEnabled(hName, pDevice->m_bGainAutoInq);
		}
	}
	/************添加Gain栏**************/
	info.RangeMax = pDevice->m_GainRange.maxV;		//xupx 0513
	info.RangeMin = pDevice->m_GainRange.minV;
	info.LineSize = 1; //added by HYL 2007.02.07
	
	str.LoadString(IDS_GAINLEVEL);
	HPROPERTY hName = m_ListView.AddItem( PropCreateTrack(str, pDevice->m_Gain, info ));	
	if (hName){
		m_ListView.SetItemData(hName, SCALAR_GAINLEVEL);
		if(IS_GM_CAM(pDevice->m_pInfo->DeviceType()))
			m_ListView.SetItemEnabled(hName, 1);		
		else
		{
			m_ListView.SetItemEnabled(hName, !pDevice->m_bGainAuto);		
		}
	}	
	/************添加红蓝、绿通道选项**************/
	if(pDevice->m_flagWB)
	{
		pDevice->m_WB_OnOffInq	= Spec_Fun_Get_Descript(pDevice, ITEM_WB_ON_OFF, 0);
		if(pDevice->m_WB_OnOffInq)
			pDevice->m_WB_OnOff		= Spec_Fun_Get(pDevice, ITEM_WB_ON_OFF);
	}

	if(pDevice->m_flagSeparateGain == 1)
	{
		info.RangeMin = pDevice->m_SeperateGain_Range.minV;
		info.RangeMax = pDevice->m_SeperateGain_Range.maxV;
		
		str.LoadString(IDS_WHITEBALANCE_V);
		HPROPERTY hNameV = m_ListView.AddItem( PropCreateTrack(str, pDevice->m_RedGain, info ));	
		if (hNameV){
			m_ListView.SetItemData(hNameV, SCALAR_RED_GAIN);
			m_ListView.SetItemEnabled(hNameV, !pDevice->m_WB_OnOff);
		}	
		
		str.LoadString(IDS_WHITEBALANCE_U);
		hName = m_ListView.AddItem( PropCreateTrack(str, pDevice->m_BlueGain, info ));	
		if (hName){
			m_ListView.SetItemData(hName, SCALAR_BLUE_GAIN);			
			m_ListView.SetItemEnabled(hName, !pDevice->m_WB_OnOff);
		}
	
	//modified by xupx 2008.09.09
		str.LoadString(IDS_WHITEBALANCE_G);
		hName = m_ListView.AddItem( PropCreateTrack(str, pDevice->m_GreenGain, info ));	
		if (hName){
			m_ListView.SetItemData(hName, SCALAR_GREEN_GAIN);			
			m_ListView.SetItemEnabled(hName, !pDevice->m_WB_OnOff);
		}
	}	
	else if(IS_HV_CAM(pDevice->m_HVTYPE))
	{
		if (pDevice->resAdvPresence.PixelsSeparateGianRB)
		{
			if(!pDevice->m_bInit)
			{
				pDevice->m_RedGain	 = Spec_Fun_ADV_Get(pDevice, SCALAR_RED_GAIN);
				pDevice->m_BlueGain	 = Spec_Fun_ADV_Get(pDevice, SCALAR_BLUE_GAIN);
				pDevice->m_RBRange.maxV = Spec_Fun_Get_AdvDescript(pDevice, SCALAR_RB_GAIN_EX, 1);
				pDevice->m_RBRange.minV = Spec_Fun_Get_AdvDescript(pDevice, SCALAR_RB_GAIN_EX, 0);
			}
			info.RangeMin = pDevice->m_RBRange.minV;
			info.RangeMax = pDevice->m_RBRange.maxV;
			
			str.LoadString(IDS_WHITEBALANCE_V);
			HPROPERTY hNameV = m_ListView.AddItem( PropCreateTrack(str, pDevice->m_RedGain, info ));	
			if (hNameV){
				m_ListView.SetItemData(hNameV, SCALAR_RED_GAIN);
				m_ListView.SetItemEnabled(hNameV, !pDevice->m_WB_OnOff);
			}	
			
			str.LoadString(IDS_WHITEBALANCE_U);
			hName = m_ListView.AddItem( PropCreateTrack(str, pDevice->m_BlueGain, info ));	
			if (hName){
				m_ListView.SetItemData(hName, SCALAR_BLUE_GAIN);			
				m_ListView.SetItemEnabled(hName, !pDevice->m_WB_OnOff);
			}
		}
		if (pDevice->resAdvPresence.PixelsSeparateGainG1G2)
		{
			if(!pDevice->m_bInit)
			{
				pDevice->m_G1G2Range.maxV = Spec_Fun_Get_AdvDescript(pDevice, SCALAR_GREEN_GAIN_EX, 1);
				pDevice->m_G1G2Range.minV = Spec_Fun_Get_AdvDescript(pDevice, SCALAR_GREEN_GAIN_EX, 0);
				pDevice->m_GreenGain = Spec_Fun_ADV_Get(pDevice, SCALAR_GREEN_GAIN);
			}
			info.RangeMin = pDevice->m_G1G2Range.minV;
			info.RangeMax = pDevice->m_G1G2Range.maxV;

			str.LoadString(IDS_WHITEBALANCE_G);
			hName = m_ListView.AddItem( PropCreateTrack(str, pDevice->m_GreenGain, info ));	
			if (hName){
				m_ListView.SetItemData(hName, SCALAR_GREEN_GAIN);			
				m_ListView.SetItemEnabled(hName, !pDevice->m_WB_OnOff);
			}
		}
	}
	UpdateGain(pDevice);
}

void CPropertyView::ListAddWB(CDevice* pDevice)
{
	if(pDevice->m_flagWB == 0)
		return;		
	
	CString str;
	TRACKBARINFO info;
	HPROPERTY hName;
	
	str.LoadString(IDS_WHITE_BALANCE);
	m_ListView.AddItem( PropCreateCategory(str) );
	
	if(!pDevice->m_bInit)
	{		
		/***************红、蓝、绿通道分量******************/
		pDevice->m_WB_ValueRange.minV = Spec_Fun_Get_Descript(pDevice, SCALAR_R_VALUE, 0);
		pDevice->m_WB_ValueRange.maxV = Spec_Fun_Get_Descript(pDevice, SCALAR_R_VALUE, 1);
		
		pDevice->m_WB_OnOffInq	= Spec_Fun_Get_Descript(pDevice, ITEM_WB_ON_OFF, 0);
		pDevice->m_WB_AutoInq	= Spec_Fun_Get_Descript(pDevice, ITEM_WB_AUTO_MANU, 0);
		pDevice->m_WB_OnePushInq	= Spec_Fun_Get_Descript(pDevice, ITEM_WB_ONE_PUSH, 0);

		pDevice->m_R_Value		= Spec_Fun_Get(pDevice, SCALAR_R_VALUE);
		pDevice->m_B_Value		= Spec_Fun_Get(pDevice, SCALAR_B_VALUE);

		if(pDevice->m_WB_OnOffInq)
			pDevice->m_WB_OnOff		= Spec_Fun_Get(pDevice, ITEM_WB_ON_OFF);
		if(pDevice->m_WB_AutoInq)
			pDevice->m_WB_AutoManual= Spec_Fun_Get(pDevice, ITEM_WB_AUTO_MANU);
		if(pDevice->m_WB_OnePushInq)
			pDevice->m_WB_OnePush	= Spec_Fun_Get(pDevice, ITEM_WB_ONE_PUSH);
	}

	/************* 界面 *************/
	if(pDevice->m_WB_OnOffInq == 1)
	{
		std::list<CString> strlist;
		strlist.clear();
		strlist.push_back("Off");
		strlist.push_back("On");
		
		LPCTSTR list[256] = { NULL }; 
		std::list<CString>::iterator pString = NULL;
		int i =0;
		
		for ( i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
		{
			list[i++] = (*pString);
		}
		list[i] = NULL;
		
		str.LoadString(IDS_WB_ON_OFF);
		hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_WB_OnOff) );
		if (hName){
			m_ListView.SetItemData(hName, ITEM_WB_ON_OFF);
			m_ListView.SetItemEnabled(hName, 1);
		}
	}

	if(pDevice->m_WB_AutoInq == 1)
	{
		std::list<CString> strlist;
		strlist.clear();
		strlist.push_back("Manual");
		strlist.push_back("Auto");
		
		LPCTSTR list[256] = { NULL }; 
		std::list<CString>::iterator pString = NULL;
		int i =0;
		
		for ( i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
		{
			list[i++] = (*pString);
		}
		list[i] = NULL;
		
		str.LoadString(IDS_WB_AUTO);
		hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_WB_AutoManual) );
		if (hName){
			m_ListView.SetItemData(hName, ITEM_WB_AUTO_MANU);
			m_ListView.SetItemEnabled(hName, pDevice->m_WB_OnOff);
		}
		
	}
	
	{
		info.RangeMin = pDevice->m_WB_ValueRange.minV;
		info.RangeMax = pDevice->m_WB_ValueRange.maxV;
		
		str.LoadString(IDS_R_VALUE);
		HPROPERTY hNameV = m_ListView.AddItem( PropCreateTrack(str, pDevice->m_R_Value, info ));	
		if (hNameV){
			m_ListView.SetItemData(hNameV, SCALAR_R_VALUE);
			m_ListView.SetItemEnabled(hNameV, pDevice->m_WB_OnOff && !pDevice->m_WB_AutoManual);
		}	
		
		str.LoadString(IDS_B_VALUE);
		hName = m_ListView.AddItem( PropCreateTrack(str, pDevice->m_B_Value, info ));	
		if (hName){
			m_ListView.SetItemData(hName, SCALAR_B_VALUE);			
			m_ListView.SetItemEnabled(hName, pDevice->m_WB_OnOff && !pDevice->m_WB_AutoManual);
		}

	}
	
	if(pDevice->m_WB_OnePushInq == 1)
	{
		str.LoadString(IDS_WB_ONEPUSH_BUTTON);
		hName = m_ListView.AddItem( PropCreateEditButton(str, "Push",1) );
		
		m_ListView.SetItemData(hName, ITEM_WB_ONE_PUSH);
		m_ListView.SetItemEnabled(hName, pDevice->m_WB_OnOff);
	}
}

void CPropertyView::ListAddSaveParam(CDevice *pDevice)
{
	if(!pDevice->m_bInit)
	{
		pDevice->m_nBeginParam	= Spec_Fun_Get_Basic(pDevice, ITEM_BEGIN_SAVE);
		pDevice->m_nLoadParam	= Spec_Fun_Get_Basic(pDevice, ITEM_LOAD_PARAM);
		pDevice->m_nSaveParam	= Spec_Fun_Get_Basic(pDevice, ITEM_SAVE_PARAM) - 1;
	}
	CString str;
	str.LoadString(IDS_SAVE_PARAM_GROUP);
	m_ListView.AddItem( PropCreateCategory(str));
	//----启动参数组
	std::list<CString> strlist;
	strlist.clear();
	strlist.push_back(" Manufacturer");
	strlist.push_back(" UserSet1 ");
	strlist.push_back(" UserSet2");
	strlist.push_back(" UserSet3");	
	
	int i = 0;
	LPCTSTR list[256] = { NULL }; 
	std::list<CString>::iterator pString = NULL;
	
	for (i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
	{
		list[i++] = (*pString);
	}
	list[i] = NULL;

	str.LoadString(IDS_BEGIN_PARAM);
	HPROPERTY hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_nBeginParam) );
	if (hName){
		m_ListView.SetItemData(hName, ITEM_BEGIN_SAVE);
		m_ListView.SetItemEnabled(hName, 1);
	}
	
	//----保存参数
	LPCTSTR list1[256] = { NULL }; 
	std::list<CString> strlist1;
	strlist1.clear();
	strlist1.push_back(" UserSet1");
	strlist1.push_back(" UserSet2");
	strlist1.push_back(" UserSet3");	
	
	std::list<CString>::iterator pString1 = NULL;
	for(i = 0, pString1 =strlist1.begin(); pString1 != strlist1.end(); ++pString1)
	{
		list1[i++] = (*pString1);
	}
	
	str.LoadString(IDS_SAVE_PARAM);
	hName = m_ListView.AddItem( PropCreateList(str, list1, pDevice->m_nSaveParam));
	if (hName){
		m_ListView.SetItemData(hName, ITEM_SAVE_PARAM);
		m_ListView.SetItemEnabled(hName, 1);
	}

	//-----加载参数
	for (i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
	{
		list[i++] = (*pString);
	}
	list[i] = NULL;
	
	str.LoadString(IDS_LOAD_PARAM);
	hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_nLoadParam) );
	if (hName){
		m_ListView.SetItemData(hName, ITEM_LOAD_PARAM);
		m_ListView.SetItemEnabled(hName, 1);
	}
}

void CPropertyView::ListAddGain(CDevice* pDevice)
{
	CString str;
	str.LoadString(IDS_GAIN);
	m_ListView.AddItem( PropCreateCategory(str) );

	TRACKBARINFO info;
	info.RangeMin = pDevice->m_pGain->Min();
	info.RangeMax = pDevice->m_pGain->Max();
	info.LineSize = 1; //added by HYL 2007.02.07
	int nV = pDevice->m_pGain->Value();
	str.LoadString(IDS_GAINLEVEL);
	HPROPERTY hName = m_ListView.AddItem( PropCreateTrack(str, nV, info ));	
	if (hName){
		m_ListView.SetItemData(hName, SCALAR_GAINLEVEL);
		m_ListView.SetItemEnabled(hName, pDevice->m_pGain->IsSupported());
		
	}

	if (IS_CCD_CAMERA(pDevice->m_pInfo->DeviceType())) 
	{
		if(!IS_NOT_SV_COLOR(pDevice->m_pInfo->DeviceType()))
		{
			info.RangeMax = 63;		//xupx 0513
			info.RangeMin = 0;
			info.LineSize = 1; //added by HYL 2007.02.07
		
			str.LoadString(IDS_WHITEBALANCE_V);
			HPROPERTY hNameV = m_ListView.AddItem( PropCreateTrack(str, pDevice->m_RedGain, info ));	
			if (hNameV){
				m_ListView.SetItemData(hNameV, SCALAR_RED_GAIN);
				m_ListView.SetItemEnabled(hNameV, IS_CCD_CAMERA(pDevice->m_pInfo->DeviceType()));
			}				

			str.LoadString(IDS_WHITEBALANCE_U);
			hName = m_ListView.AddItem( PropCreateTrack(str, pDevice->m_BlueGain, info ));	
			if (hName){
				m_ListView.SetItemData(hName, SCALAR_BLUE_GAIN);
		 
				m_ListView.SetItemEnabled(hName, IS_CCD_CAMERA(pDevice->m_pInfo->DeviceType()));
			}
				
		}
	}

}


void CPropertyView::ListAddBlank(CDevice* pDevice)
{
	CString str;

	str.LoadString(IDS_BLANK);
	m_ListView.AddItem( PropCreateCategory(str) );

	TRACKBARINFO info;
	info.RangeMin		= pDevice->m_pBlank->HorMin();
	info.RangeMax		= pDevice->m_pBlank->HorMax();
	info.LineSize = 1; //added by HYL 2007.02.07
	m_CurrentBlankHor	= pDevice->m_pBlank->Hor();
	str.LoadString(IDS_BLANKHOR);
	HPROPERTY hName = m_ListView.AddItem( PropCreateTrack(str, m_CurrentBlankHor, info ));
	if (hName){
		m_ListView.SetItemData(hName, SCALAR_BLANKHOR);
		m_ListView.SetItemEnabled(hName, pDevice->m_pBlank->IsSupported());
	}

	info.RangeMin		= pDevice->m_pBlank->VertMin();
	info.RangeMax		= pDevice->m_pBlank->VertMax();
	info.LineSize = 1; //added by HYL 2007.02.07

	m_CurrentBlankVert	= pDevice->m_pBlank->Vert();
	str.LoadString(IDS_BLANKVERT);
	hName = m_ListView.AddItem( PropCreateTrack(str, m_CurrentBlankVert, info ));
	if (hName){
		m_ListView.SetItemData(hName, SCALAR_BLANKVERT);
		m_ListView.SetItemEnabled(hName, pDevice->m_pBlank->IsSupported());
	}
}


void CPropertyView::ListAddPacketSize(CDevice* pDevice)
{
    CString str;    
    TRACKBARINFO info;
	/****参数可读相机 2008.12.08 by xupx******/
	if(IS_READABLE_CAM(pDevice->m_pInfo->DeviceType()))	 // 2009.10.16
	{
		info.LineSize			 = 4;
		info.RangeMax			 = Spec_Fun_Get_FMT7_DESCRIPTOR(pDevice, SCALAR_PACKET, 1);
		info.RangeMin			 = Spec_Fun_Get_FMT7_DESCRIPTOR(pDevice, SCALAR_PACKET, 0);
		pDevice->m_PacketSize	 = Spec_Fun_Get(pDevice, SCALAR_PACKET);

		str.LoadString(IDS_PACKETSIZE);
		HPROPERTY hName = m_ListView.AddItem( PropCreateTrack(str, pDevice->m_PacketSize, info ));	
		if (hName){
			m_ListView.SetItemData(hName, SCALAR_PACKET);
			m_ListView.SetItemEnabled(hName, TRUE);
			hName->SetCustomData(4);
		}
			
		return;
	}
	/***************** End *****************/
    str.LoadString(IDS_VIDEOFORMAT);//(IDS_PACKET);
    m_ListView.AddItem( PropCreateCategory(str) );

    info.RangeMax = 4096;
    info.RangeMin = 1600;
	info.LineSize = 4;
	if (IS_SV1310(pDevice->m_pInfo->DeviceType())) 
	{
		info.RangeMin = 2500;
	}
	if (IS_SV1311(pDevice->m_pInfo->DeviceType())||
		IS_SV1400(pDevice->m_pInfo->DeviceType())||
		IS_SV1410(pDevice->m_pInfo->DeviceType())||
		IS_SV1420(pDevice->m_pInfo->DeviceType())) 
	{	
		info.RangeMin = 356;		
	}
	if (IS_SV400(pDevice->m_pInfo->DeviceType())) 
	{	
		info.RangeMin = 112;		
	}
	if (IS_SV2000(pDevice->m_pInfo->DeviceType())) 
	{
		info.RangeMin = 492;
	}
	//add by xupx 2008.12.03 当选择退出时删除历史记录，此属性与界面不同步	
	pDevice->SetPacketSize(pDevice->m_PacketSize);	
	// end by xupx
	
	//包长调节
    str.LoadString(IDS_PACKETSIZE);
    HPROPERTY hName = m_ListView.AddItem( PropCreateTrack(str, pDevice->m_PacketSize, info ));	
    if (hName){
        m_ListView.SetItemData(hName, SCALAR_PACKET);
		bool b = IS_CCD_CAMERA(pDevice->m_pInfo->DeviceType());
        m_ListView.SetItemEnabled(hName, b);
        hName->SetCustomData(4);
    }
}
void CPropertyView::ListAddTestImage(CDevice* pDevice)
{
	if (IS_HV_CAM(pDevice->m_HVTYPE) && pDevice->resAdvPresence.TestImage == 0)
		return;
	
    CString str;
	str.LoadString(IDS_TEST_IMAGE);
	m_ListView.AddItem( PropCreateCategory(str) );
    std::list<CString> strlist;

	if(IS_READABLE_CAM(pDevice->m_pInfo->DeviceType()))
	{
		if(!pDevice->m_bInit)
			pDevice->m_nTestImage = Spec_Fun_ADV_Get(pDevice, ITEM_TEST_IMAGE);
	}
	else
		Spec_Fun_Interface_1(pDevice,TEST_IMAGE,pDevice->m_nTestImage);
	
	strlist.clear();
	strlist.push_back(" Disabled");
	strlist.push_back(" Testimage 1");
	strlist.push_back(" Testimage 2");
	strlist.push_back(" Testimage 3");

	int i = 0;
	LPCTSTR list[256] = { NULL }; 
	std::list<CString>::iterator pString = NULL;
	for ( i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
	{
		list[i++] = (*pString);
	}
	list[i] = NULL;
    str.LoadString(IDS_TEST_IMAGE_ITEM);
	
	HPROPERTY hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_nTestImage) );
	if (hName){
		m_ListView.SetItemData(hName, ITEM_TEST_IMAGE);
		m_ListView.SetItemEnabled(hName, 1);
	}	
}


void CPropertyView::ListAddTriggerDelay(CDevice* pDevice)
{
	CString str;
	str.LoadString(IDS_TRIGGER_DELAY_FUN);
	m_ListView.AddItem( PropCreateCategory(str) );
	Spec_Fun_Interface_1(pDevice,TRIGGER_DELAY_ON_OFF,pDevice->m_bTriggerDelayOnOff);
	std::list<CString> strlist;
	strlist.clear();
	strlist.push_back("off");
    strlist.push_back("on");
	int i = 0;
	LPCTSTR list[256] = { NULL }; 
	std::list<CString>::iterator pString = NULL;
	for ( i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
	{
		list[i++] = (*pString);
	}
	list[i] = NULL;
	m_CurrentADLevel = pDevice->m_pADCLevel->Value();
	str.LoadString(IDS_TRIGGER_DELAY_SWITCH);
	HPROPERTY hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_bTriggerDelayOnOff) );
	if (hName){
		m_ListView.SetItemData(hName, ITEM_TRIGGERDELAY_STATE);
		m_ListView.SetItemEnabled(hName, pDevice->m_pADCLevel->IsSupported());
	}	
	
    TRACKBARINFO info;
	info.RangeMax = 4095;
	info.RangeMin = 1;
	info.LineSize = 1;     
	
    Spec_Fun_Interface_1(pDevice,TRIGGER_DELAY_VALUE,pDevice->m_nTriggerDelayValue);
	str.LoadString(IDS_TRIGGER_DELAY_VALUE);
	hName = m_ListView.AddItem( PropCreateTrack(str, pDevice->m_nTriggerDelayValue, info ));	
	if (hName){
		m_ListView.SetItemData(hName, SCALAR_TRIGGERDELAY_VALUE);
		m_ListView.SetItemEnabled(hName, 1);
	}
}

void CPropertyView::ListAddTriggerDelay_NetCam(CDevice* pDevice)
{
	if(pDevice->m_flagTriggerDelay == 0)
		return;

	if(!pDevice->m_bInit)
	{
		pDevice->m_TriggerDelayRange.maxV = Spec_Fun_Get_Descript(pDevice, SCALAR_TRIGGERDELAY_VALUE, 1);
		pDevice->m_TriggerDelayRange.minV = Spec_Fun_Get_Descript(pDevice, SCALAR_TRIGGERDELAY_VALUE, 0);
		
		pDevice->m_TriggerDelayStateDescrip = Spec_Fun_Get_Descript(pDevice, ITEM_TRIGGERDELAY_STATE, 0);
		pDevice->m_nTriggerDelayValue	  = Spec_Fun_Get(pDevice, SCALAR_TRIGGERDELAY_VALUE);
		pDevice->m_bTriggerDelayOnOff	  = Spec_Fun_Get(pDevice, ITEM_TRIGGERDELAY_STATE);

		if(pDevice->m_flagTriggerDelayUnit != 0)
			pDevice->m_TriggerDelayUnit		  = Spec_Fun_ADV_Get(pDevice, ITEM_TRIGGERDELAY_UNIT);
	}
	
	CString str;
	str.LoadString(IDS_TRIGGER_DELAY_FUN);
	m_ListView.AddItem( PropCreateCategory(str) );
	std::list<CString> strlist;
	strlist.clear();
	strlist.push_back("off");
    strlist.push_back("on");
	int i = 0;
	LPCTSTR list[256] = { NULL }; 
	std::list<CString>::iterator pString = NULL;
	for ( i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
	{
		list[i++] = (*pString);
	}
	list[i] = NULL;
	m_CurrentADLevel = pDevice->m_pADCLevel->Value();
	str.LoadString(IDS_TRIGGER_DELAY_SWITCH);
	HPROPERTY hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_bTriggerDelayOnOff) );
	if (hName){
		m_ListView.SetItemData(hName, ITEM_TRIGGERDELAY_STATE);
		m_ListView.SetItemEnabled(hName,  pDevice->m_TriggerDelayStateDescrip);
	}
	/****************************************/
	if(pDevice->m_flagTriggerDelayUnit != 0)
	{
		strlist.clear();
		strlist.push_back("us");
		strlist.push_back("ms");
		LPCTSTR listUnit[256] = { NULL }; 
		std::list<CString>::iterator pStringUnit = NULL;
		for ( i = 0, pStringUnit = strlist.begin(); pStringUnit != strlist.end(); ++ pStringUnit )
		{
			listUnit[i++] = (*pStringUnit);
		}
		listUnit[i] = NULL;
		
		str.LoadString(IDS_TRIGGERDELAY_UNIT);
		hName = m_ListView.AddItem( PropCreateList(str, listUnit, pDevice->m_TriggerDelayUnit) );
		if (hName){
			m_ListView.SetItemData(hName, ITEM_TRIGGERDELAY_UNIT);
			m_ListView.SetItemEnabled(hName,  pDevice->m_bTriggerDelayOnOff && pDevice->m_TriggerDelayStateDescrip);
		}
	}
		
	/****************************************/
    TRACKBARINFO info;
	info.RangeMax = pDevice->m_TriggerDelayRange.maxV;
	info.RangeMin = pDevice->m_TriggerDelayRange.minV;
	info.LineSize = 1;     
	
	str.LoadString(IDS_TRIGGER_DELAY_VALUE);
	hName = m_ListView.AddItem( PropCreateTrack(str, pDevice->m_nTriggerDelayValue, info ));	
	if (hName){
		m_ListView.SetItemData(hName, SCALAR_TRIGGERDELAY_VALUE);
		m_ListView.SetItemEnabled(hName, pDevice->m_bTriggerDelayOnOff && pDevice->m_TriggerDelayStateDescrip);
	}
}
void CPropertyView::ListAddTransfersDelay(CDevice* pDevice)
{
	CString str;
	str.LoadString(IDS_TRANSFERSDELAY);
	m_ListView.AddItem( PropCreateCategory(str) );

    TRACKBARINFO info;
	/************可读参数相机 2008.12.08 by xupx ***************/
	if(IS_ITS1394(pDevice->m_HVTYPE))
	{
		info.RangeMax = Spec_Fun_Get_AdvDescript(pDevice, SCALAR_TRANSDELAY_VALUE, 1);
		info.RangeMin = Spec_Fun_Get_AdvDescript(pDevice, SCALAR_TRANSDELAY_VALUE, 0);
		info.LineSize = 1;     

		pDevice->m_nTransfersDelayValue = Spec_Fun_ADV_Get(pDevice, SCALAR_TRANSDELAY_VALUE);
		str.LoadString(IDS_TRANSDELAY_VALUE);
		HPROPERTY  hName = m_ListView.AddItem( PropCreateTrack(str, pDevice->m_nTransfersDelayValue, info ));	
		if (hName){
			m_ListView.SetItemData(hName, SCALAR_TRANSDELAY_VALUE);
			m_ListView.SetItemEnabled(hName, 1);
		}
		return;
	}
	/*************** End ***********************/
	info.RangeMax = 3000;
	info.RangeMin = 0;
	info.LineSize = 1;     
	
    Spec_Fun_Interface_1(pDevice,TRANSFERS_DELAY_VALUE,pDevice->m_nTransfersDelayValue);
	str.LoadString(IDS_TRANSDELAY_VALUE);
	HPROPERTY  hName = m_ListView.AddItem( PropCreateTrack(str, pDevice->m_nTransfersDelayValue, info ));	
	if (hName){
		m_ListView.SetItemData(hName, SCALAR_TRANSDELAY_VALUE);
		m_ListView.SetItemEnabled(hName, 1);
	}
}

void CPropertyView::ListAddFilterTime_Readable(CDevice* pDevice)
{
	TRACKBARINFO info;
	
	if(!pDevice->m_bInit)
	{
//		pDevice->m_flagFilterUnit = Spec_Fun_Presence(pDevice, ITEM_FILTER_TIME_UNIT);//固件不让读取
		if(pDevice->m_flagFilterTime == 1)
		{
			pDevice->m_nFilterTimeUnit	= Spec_Fun_ADV_Get(pDevice, ITEM_FILTER_TIME_UNIT);
			pDevice->m_FilterUnitInq	= Spec_Fun_Get_AdvDescript(pDevice, ITEM_FILTER_TIME_UNIT,0);
			pDevice->m_nFilterTime		= Spec_Fun_ADV_Get(pDevice, SCALAR_FILTER_TIME);
			pDevice->m_FilterTimeInq	= Spec_Fun_Get_AdvDescript(pDevice, SCALAR_FILTER_TIME, 2);

			info.RangeMax = Spec_Fun_Get_AdvDescript(pDevice, SCALAR_FILTER_TIME, 1);
			info.RangeMin = Spec_Fun_Get_AdvDescript(pDevice, SCALAR_FILTER_TIME, 0);
			info.LineSize = 1;     
		}
	}
	if( pDevice->m_flagFilterTime == 0)	//pDevice->m_flagFilterUnit == 0 &&
	{
		return;
	}

	CString str;
	HPROPERTY hName;
	str.LoadString(IDS_FILTER_TIME_FUN);
	m_ListView.AddItem( PropCreateCategory(str));
	
	std::list<CString> strlist;
	strlist.push_back("us");
	strlist.push_back("ns");  
	int i = 0;
	LPCTSTR list[256] = { NULL }; 
	std::list<CString>::iterator pString = NULL;
	for (i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
	{
		list[i++] = (*pString);
	}
	list[i] = NULL; 
	
//	if(pDevice->m_flagFilterUnit == 1)
	{
		str.LoadString(IDS_TIME_UNIT);
		hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_nFilterTimeUnit) );
		if (hName){
			m_ListView.SetItemData(hName, ITEM_FILTER_TIME_UNIT);
			m_ListView.SetItemEnabled(hName, pDevice->m_FilterUnitInq);
		}
	}
	//------------- TIME ---------
	
//	if(pDevice->m_flagFilterTime == 1)
	{
		str.LoadString(IDS_FILTER_TIME);
		hName = m_ListView.AddItem( PropCreateTrack(str, pDevice->m_nFilterTime, info ));	
		if (hName){
			m_ListView.SetItemData(hName, SCALAR_FILTER_TIME);
			m_ListView.SetItemEnabled(hName, pDevice->m_FilterTimeInq);
		}
	}
}

void CPropertyView::ListAddFilterTime(CDevice* pDevice)
{
	CString str;
	str.LoadString(IDS_FILTER_TIME_FUN);
	m_ListView.AddItem( PropCreateCategory(str) );

	//-------------- UNIT --------		
	Spec_Fun_Interface_1(pDevice,FILTER_TIME_UNIT,pDevice->m_nFilterTimeUnit);	

	std::list<CString> strlist;
	strlist.push_back("us");
	strlist.push_back("ns");  

	int i = 0;
	LPCTSTR list[256] = { NULL }; 
	std::list<CString>::iterator pString = NULL;
	for (i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
	{
		list[i++] = (*pString);
	}
	list[i] = NULL; 

	str.LoadString(IDS_TIME_UNIT);
	HPROPERTY hName = m_ListView.AddItem( PropCreateList(str, list, 
		pDevice->m_nFilterTimeUnit) );
	if (hName){
		m_ListView.SetItemData(hName, ITEM_FILTER_TIME_UNIT);
		m_ListView.SetItemEnabled(hName, 1);
	}

	//------------- TIME ---------
    TRACKBARINFO info;
	info.RangeMax = 2000;
	info.RangeMin = 0;
	info.LineSize = 1;     
	
    Spec_Fun_Interface_1(pDevice,FILTER_TIME,pDevice->m_nFilterTime);
	str.LoadString(IDS_FILTER_TIME);
	hName = m_ListView.AddItem( PropCreateTrack(str, 
		pDevice->m_nFilterTime, info ));	
	if (hName){
		m_ListView.SetItemData(hName, SCALAR_FILTER_TIME);
		m_ListView.SetItemEnabled(hName, 1);
	}
	
}

//输出IO功能设置
void CPropertyView::ListAddOutPutIoSet_NetCam(CDevice* pDevice)
{
	if(pDevice->resPIOPresence.Port0 == 0)
		return;
	
//	Spec_Fun_Get_AdvDescript(pDevice, IDS_OUTPUTIOSET, 0); //可以得到m_res_Output_Descriptor
	CString str;
	str.LoadString(IDS_OUTPUTIOSET);
	m_ListView.AddItem( PropCreateCategory(str));

	std::list<CString> strlist;
	strlist.clear();
	strlist.push_back(" Integration signal");
	strlist.push_back(" Trigger ready  ");
	strlist.push_back(" User set");
	strlist.push_back(" Strobe");
	strlist.push_back(" Stroboscope ");

	HPROPERTY hName;
    int i = 0;
	LPCTSTR list[256] = { NULL }; 
	std::list<CString>::iterator pString = NULL;

	///////
 	if(pDevice->resPIOPresence.Port0)
	{
		if(!pDevice->m_bInit)
		{
			pDevice->m_nOutPutIO_0 = Spec_Fun_ADV_Get(pDevice, Get_OUTPUT_IO_0);
		}
		str.LoadString(IDS_OUTPUTIOSET_0);	
		
		for ( i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
		{
			list[i++] = (*pString);
		}
		Spec_Fun_Get_AdvDescript(pDevice, IDS_OUTPUTIOSET, 0); //可以得到m_res_Output_Descriptor
		if (pDevice->m_OutputIO_Desc.Channel0.SourceSelect.StroboscopeInq == 1) {
			list[i] = NULL;
		}
		else
			list[i - 1] = NULL;
		hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_nOutPutIO_0) );//3.21
		if (hName){
			m_ListView.SetItemData(hName, ITEM_OUTPUTIO_0);
			m_ListView.SetItemEnabled(hName, 1);
		}
	}
	
	///////
	if(pDevice->resPIOPresence.Port1)
	{
		if(!pDevice->m_bInit)
			pDevice->m_nOutPutIO_1 = Spec_Fun_ADV_Get(pDevice, Get_OUTPUT_IO_1);
		
		str.LoadString(IDS_OUTPUTIOSET_1);
		for ( i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
		{
			list[i++] = (*pString);
		}
		Spec_Fun_Get_AdvDescript(pDevice, IDS_OUTPUTIOSET, 1); //可以得到m_res_Output_Descriptor
		if (pDevice->m_OutputIO_Desc.Channel1.SourceSelect.StroboscopeInq == 1) {
			list[i] = NULL;
		}
		else
			list[i - 1] = NULL;
		hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_nOutPutIO_1) );
		if (hName){
			m_ListView.SetItemData(hName, ITEM_OUTPUTIO_1);
			m_ListView.SetItemEnabled(hName, 1);
		}
	}
	if(pDevice->resPIOPresence.Port2)
	{
		if(!pDevice->m_bInit)
			pDevice->m_nOutPutIO_2 = Spec_Fun_ADV_Get(pDevice, Get_OUTPUT_IO_2);
		
		str.LoadString(IDS_OUTPUTIOSET_2);
		for ( i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
		{
			list[i++] = (*pString);
		}
		Spec_Fun_Get_AdvDescript(pDevice, IDS_OUTPUTIOSET, 2); //可以得到m_res_Output_Descriptor
		if (pDevice->m_OutputIO_Desc.Channel2.SourceSelect.StroboscopeInq == 1) {
			list[i] = NULL;
		}
		else
			list[i - 1] = NULL;
		hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_nOutPutIO_2) );
		if (hName){
			m_ListView.SetItemData(hName, ITEM_OUTPUTIO_2);
			m_ListView.SetItemEnabled(hName, 1);
		}
	}
}


void CPropertyView::ListAddOutPutIoSet(CDevice* pDevice)
{
	CString str;
	str.LoadString(IDS_OUTPUTIOSET);
	m_ListView.AddItem( PropCreateCategory(str));
	std::list<CString> strlist;
	strlist.clear();
	strlist.push_back(" Integration signal");
	strlist.push_back(" Trigger ready  ");
	strlist.push_back(" User set");
	strlist.push_back(" Strobe");
	
    int i = 0;
	LPCTSTR list[256] = { NULL }; 
	std::list<CString>::iterator pString = NULL;
	for ( i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
	{
		list[i++] = (*pString);
	}
	list[i] = NULL;
    str.LoadString(IDS_OUTPUTIOSET_0);
	
	HPROPERTY hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_nOutPutIO_0) );//3.21
	if (hName){
		m_ListView.SetItemData(hName, ITEM_OUTPUTIO_0);
		m_ListView.SetItemEnabled(hName, 1);
	}
	///////
	str.LoadString(IDS_OUTPUTIOSET_1);
    hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_nOutPutIO_1) );
	if (hName){
		m_ListView.SetItemData(hName, ITEM_OUTPUTIO_1);
		m_ListView.SetItemEnabled(hName, 1);
	}
	//////
	str.LoadString(IDS_OUTPUTIOSET_2);
    hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_nOutPutIO_2) );
	if (hName){
		m_ListView.SetItemData(hName, ITEM_OUTPUTIO_2);
		m_ListView.SetItemEnabled(hName, 1);
	}
	
}

void CPropertyView::ListAddOutPutIoControl(CDevice* pDevice)
{
	CString str;
	str.LoadString(IDS_OUTPUTIOCONTROL);
	m_ListView.AddItem( PropCreateCategory(str));
	std::list<CString> strlist;
	strlist.clear();
	strlist.push_back(" Low");
	strlist.push_back(" High");
	
	int i = 0;
	LPCTSTR list[256] = { NULL }; 
	std::list<CString>::iterator pString = NULL;
	for ( i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
	{
		list[i++] = (*pString);
	}
	list[i] = NULL;
    str.LoadString(IDS_OUTPUTIOCONTROL_0);
	
	HPROPERTY hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_nOutPutIO_0_POL) );
	if (hName){
		m_ListView.SetItemData(hName, ITEM_OUTPUTIO_0_CONTROL);
		if(pDevice->m_nOutPutIO_0==2)
		{
			m_ListView.SetItemEnabled(hName, 1);
		}
		else
		{
			m_ListView.SetItemEnabled(hName, 0); //2007.3.1
		}
		
	}
	
	///
	str.LoadString(IDS_OUTPUTIOCONTROL_1);
    hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_nOutPutIO_1_POL) );
	if (hName){
		m_ListView.SetItemData(hName, ITEM_OUTPUTIO_1_CONTROL);
		if(pDevice->m_nOutPutIO_1==2)
		{
			m_ListView.SetItemEnabled(hName, 1);
		}
		else
		{
			m_ListView.SetItemEnabled(hName, 0); //2007.3.1
		}
		
	}
	///
	str.LoadString(IDS_OUTPUTIOCONTROL_2);
    hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_nOutPutIO_2_POL) );
	if (hName){
		m_ListView.SetItemData(hName, ITEM_OUTPUTIO_2_CONTROL);

		if(pDevice->m_nOutPutIO_2==2)
			m_ListView.SetItemEnabled(hName, 1);
		else
			m_ListView.SetItemEnabled(hName, 0); //2007.3.1
    }
	
}

void CPropertyView::ListAddOutPutIoControl_NetCam(CDevice* pDevice)
{
	CString str;
	str.LoadString(IDS_OUTPUTIOCONTROL);
	m_ListView.AddItem( PropCreateCategory(str));
	std::list<CString> strlist;
	strlist.clear();
	strlist.push_back(" Low");
	strlist.push_back(" High");
	
	int i = 0;
	LPCTSTR list[256] = { NULL }; 
	std::list<CString>::iterator pString = NULL;
	for ( i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
	{
		list[i++] = (*pString);
	}
	list[i] = NULL;

    str.LoadString(IDS_OUTPUTIOCONTROL_0);

	HPROPERTY hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_nOutPutIO_0_POL) );
	m_ListView.SetItemData(hName, ITEM_OUTPUTIO_0_CONTROL);
	if(hName && pDevice->resPIOPresence.Port0)	
	{
		if(!pDevice->m_bInit)
		{
			pDevice->m_nOutPutIO_0_POL = Spec_Fun_ADV_Get(pDevice, ITEM_OUTPUTIO_0_CONTROL);
		}
		if(pDevice->m_nOutPutIO_0==2)
		{
			m_ListView.SetItemEnabled(hName, 1);
			str.Format("%s", list[pDevice->m_nOutPutIO_0_POL]);
			CComVariant v(str);
			m_ListView.SetItemValue(hName, &v);
		}
		else
		{
			str.Format("%s", list[pDevice->m_nOutPutIO_0_POL]);
			CComVariant v(str);
			m_ListView.SetItemValue(hName, &v);
			m_ListView.SetItemEnabled(hName, 0); //2007.3.1
		}	
	}
	if(hName && pDevice->resPIOPresence.Port1)	
	{
		///
		str.LoadString(IDS_OUTPUTIOCONTROL_1);
		hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_nOutPutIO_1_POL));
		m_ListView.SetItemData(hName, ITEM_OUTPUTIO_1_CONTROL);
			
		if(!pDevice->m_bInit)
		{
			pDevice->m_nOutPutIO_1_POL = Spec_Fun_ADV_Get(pDevice, ITEM_OUTPUTIO_1_CONTROL);
		}
		if(pDevice->m_nOutPutIO_1==2)
		{
			m_ListView.SetItemEnabled(hName, 1);
			str.Format("%s", list[pDevice->m_nOutPutIO_1_POL]);
			CComVariant v(str);
			m_ListView.SetItemValue(hName, &v);
		}
	    else
		{
			m_ListView.SetItemEnabled(hName, 0); //2007.3.1
			str.Format("%s", list[pDevice->m_nOutPutIO_1_POL]);
			CComVariant v(str);
			m_ListView.SetItemValue(hName, &v);
		}	
	}
	
	if(hName && pDevice->resPIOPresence.Port2)
	{
		str.LoadString(IDS_OUTPUTIOCONTROL_2);
		hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_nOutPutIO_2_POL));
		m_ListView.SetItemData(hName, ITEM_OUTPUTIO_2_CONTROL);
		if(!pDevice->m_bInit)
		{
			pDevice->m_nOutPutIO_2_POL = Spec_Fun_ADV_Get(pDevice, ITEM_OUTPUTIO_2_CONTROL);
		}			
		if(pDevice->m_nOutPutIO_2==2)
		{
			m_ListView.SetItemEnabled(hName, 1);
			str.Format("%s", list[pDevice->m_nOutPutIO_2_POL]);
			CComVariant v(str);
			m_ListView.SetItemValue(hName, &v);
		}
		else
		{
			m_ListView.SetItemEnabled(hName, 0); //2007.3.1
			str.Format("%s", list[pDevice->m_nOutPutIO_2_POL]);
			CComVariant v(str);
			m_ListView.SetItemValue(hName, &v);
		}
	}
}


void CPropertyView::ListAddInPutIoSet(CDevice* pDevice) 
{
	if(pDevice->m_flagTrigger == 0)		// add by xupx 2009.09.17
		return;

	CString str;
	str.LoadString(IDS_INPUTIOSET);
	m_ListView.AddItem( PropCreateCategory(str));

	if(IS_READABLE_CAM(pDevice->m_pInfo->DeviceType())) //2008.12.30
	{
		if(!pDevice->m_bInit)
		{
			pDevice->m_nTriggerSource = Spec_Fun_Get(pDevice, ITEM_INPUTIOSET);
			Spec_Fun_Get_Descript(pDevice, ITEM_INPUTIOSET, 0);
		}
	}
	else
		Spec_Fun_Interface_1(pDevice,TRIGGER_SOURCE_IO,pDevice->m_nTriggerSource);
	
	
	std::list<CString> strlist;
	strlist.clear();
//	if(IS_ITS1394(pDevice->m_pInfo->DeviceType()))
	if(IS_READABLE_CAM(pDevice->m_pInfo->DeviceType())) //2009.07.16		
	{
		if(pDevice->m_TriggerSourceInq.Source0Inq == 1)
			strlist.push_back(" Input0");
		if(pDevice->m_TriggerSourceInq.Source1Inq == 1)
			strlist.push_back(" Input1 ");
		if(pDevice->m_TriggerSourceInq.Source2Inq == 1)
			strlist.push_back(" Input2 ");
		if(pDevice->m_TriggerSourceInq.Source3Inq == 1)
			strlist.push_back(" Input3 ");
	}
	else{
		strlist.push_back(" Input0");
		strlist.push_back(" Input1 ");
		strlist.push_back(" Input2");
	}

    int i = 0;
	LPCTSTR list[256] = { NULL }; 
	std::list<CString>::iterator pString = NULL;
	for ( i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
	{
		list[i++] = (*pString);
	}
	list[i] = NULL;
    str.LoadString(IDC_TRIGGER_SOURCE);
	
	HPROPERTY hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_nTriggerSource) );
	if (hName){
		m_ListView.SetItemData(hName, ITEM_INPUTIOSET);
		m_ListView.SetItemEnabled(hName, 1);
	}
	
}

const CString COLORCODE[171] = {
	"Mono 8","YUV8_4_1_1","YUV8_4_2_2", "YUV8_4_4_4",	"RGB8", "Mono 16", "RGB 16","Signed Mono16",
	"Signed RGB16","Raw 8", "Raw 16", " ",				" ", " ", " ", " ",
	" ", " ", " ", " ",	" ", " ", " ", " ", 
	" ", " ", " ", " ",	" ", " ", " ", " ", 
	" ", " ", " ", " ",	" ", " ", " ", " ", 
	" ", " ", " ", " ",	" ", " ", " ", " ", 
	" ", " ", " ", " ",	" ", " ", " ", " ", 
	" ", " ", " ", " ",	" ", " ", " ", " ", 
	" ", " ", " ", " ",	" ", " ", " ", " ", 
	" ", " ", " ", " ",	" ", " ", " ", " ", 
	" ", " ", " ", " ",	" ", " ", " ", " ", 
	" ", " ", " ", " ",	" ", " ", " ", " ", 
	" ", " ", " ", " ",	" ", " ", " ", " ", 
	" ", " ", " ", " ",	" ", " ", " ", " ", 
	" ", " ", " ", " ",	" ", " ", " ", " ", 
	" ", " ", " ", " 123",	" ", " ", " ", " ", 
	"Mono 10", "Mono10 Normalized", "Mono10 Packed", "Raw 10", 	"Raw10 Normalized", "Raw10 Packed", "Mono 12", "Mono12 Normalized",
	"Mono12 Packed", "Raw 12", "Raw12 Normalized", "Raw12 Packed", " 140", " ", " ", " ", 
	" ", " ", " ", " ",											" ", " ", " 150", "151 ",
	" ", " ", " ", " ",									" ", " ", " ", " ", 
	"DYUV 422", " ", " ", " ",							" ", " ", " "," ",
	"","","JPEG"
};


void CPropertyView::ListAdd8or12BitMode(CDevice* pDevice)
{
	CString str;
	if(!IS_READABLE_CAM(pDevice->m_pInfo->DeviceType()))
	{
		str.LoadString(IDS_RAW_TYPE);
		m_ListView.AddItem( PropCreateCategory(str) );
	}
	std::list<CString> strlist;
	int n;
	HVSTATUS stauts = STATUS_OK;
	
	if(IS_READABLE_CAM(pDevice->m_pInfo->DeviceType()))
	{
		if(!pDevice->m_bInit)
		{
			// modified 2009.06.30
			HV_VIDEO_MODE mode = pDevice->GetVideoMode();			
			pDevice->m_ColorCode = (HV_COLOR_CODE)pDevice->GetFMT7Mode(mode, FMT7_MODE_FUNC_ID_COLOR_CODING, NULL); 
			Spec_Fun_Get_FMT7_DESCRIPTOR(pDevice, ColorModeInq, 0);
		}

		int index = 0;
		strlist.clear();
		if(pDevice->m_resFMT7Descriptor.ColorMode.Raw8Inq)
		{
			strlist.push_back(COLORCODE[COLOR_RAW8]);
			pDevice->m_ColorCodeMap.insert(pair<CString, int>(COLORCODE[COLOR_RAW8], index));
			pDevice->m_ColorCodeIndex.insert(pair<HV_COLOR_CODE, int>(COLOR_RAW8, index));
			index++;
		}

		if(!m_IS_OldITS && !m_IS_OldGC && !m_IS_OldGM)
		{
			if(pDevice->m_resFMT7Descriptor.ColorMode.Mono8Inq)
			{
				strlist.push_back(COLORCODE[COLOR_MONO8]);
				pDevice->m_ColorCodeMap.insert(pair<CString, int>(COLORCODE[COLOR_MONO8], index));
				pDevice->m_ColorCodeIndex.insert(pair<HV_COLOR_CODE, int>(COLOR_MONO8, index++));
			}
			if(pDevice->m_resFMT7Descriptor.ColorMode.RGB8Inq)
			{
				strlist.push_back(COLORCODE[COLOR_RGB8]);
				pDevice->m_ColorCodeMap.insert(pair<CString, int>( COLORCODE[COLOR_RGB8], index));
				pDevice->m_ColorCodeIndex.insert(pair<HV_COLOR_CODE, int>(COLOR_RGB8, index++));
			}
			if(pDevice->m_resFMT7Descriptor.ColorMode.Mono10Inq)
			{
				strlist.push_back(COLORCODE[COLOR_MONO10]);
				pDevice->m_ColorCodeMap.insert(pair<CString, int>(COLORCODE[COLOR_MONO10], index));
				pDevice->m_ColorCodeIndex.insert(pair<HV_COLOR_CODE, int>(COLOR_MONO10, index++));
			}
			if(pDevice->m_resFMT7Descriptor.ColorMode.Mono10NormalizedInq)
			{
				strlist.push_back(COLORCODE[COLOR_MONO10_NORMALIZED]);
				pDevice->m_ColorCodeMap.insert(pair<CString, int>(COLORCODE[COLOR_MONO10_NORMALIZED], index));
				pDevice->m_ColorCodeIndex.insert(pair<HV_COLOR_CODE, int>(COLOR_MONO10_NORMALIZED, index++));
			}
			if(pDevice->m_resFMT7Descriptor.ColorMode.Mono10PackedInq)
			{
				strlist.push_back(COLORCODE[COLOR_MONO10_PACKED]);
				pDevice->m_ColorCodeMap.insert(pair<CString, int>(COLORCODE[COLOR_MONO10_PACKED], index));
				pDevice->m_ColorCodeIndex.insert(pair<HV_COLOR_CODE, int>(COLOR_MONO10_PACKED, index++));
			}
			if(pDevice->m_resFMT7Descriptor.ColorMode.Raw10Inq)
			{
				strlist.push_back(COLORCODE[COLOR_RAW10]);
				pDevice->m_ColorCodeMap.insert(pair<CString, int>(COLORCODE[COLOR_RAW10], index));
				pDevice->m_ColorCodeIndex.insert(pair<HV_COLOR_CODE, int>(COLOR_RAW10, index++));
			}
			if(pDevice->m_resFMT7Descriptor.ColorMode.Raw10NormalizedInq)
			{
				strlist.push_back(COLORCODE[COLOR_RAW10_NORMALIZED]);
				pDevice->m_ColorCodeMap.insert(pair<CString, int>(COLORCODE[COLOR_RAW10_NORMALIZED], index));
				pDevice->m_ColorCodeIndex.insert(pair<HV_COLOR_CODE, int>(COLOR_RAW10_NORMALIZED, index++));
			}
			if(pDevice->m_resFMT7Descriptor.ColorMode.Raw10PackedInq)
			{
				strlist.push_back(COLORCODE[COLOR_RAW10_PACKED]);
				pDevice->m_ColorCodeMap.insert(pair<CString, int>(COLORCODE[COLOR_RAW10_PACKED], index));
				pDevice->m_ColorCodeIndex.insert(pair<HV_COLOR_CODE, int>(COLOR_RAW10_PACKED, index++));
			}
			if(pDevice->m_resFMT7Descriptor.ColorMode.Mono12Inq)
			{
				strlist.push_back(COLORCODE[COLOR_MONO12]);
				pDevice->m_ColorCodeMap.insert(pair<CString, int>(COLORCODE[COLOR_MONO12], index));
				pDevice->m_ColorCodeIndex.insert(pair<HV_COLOR_CODE, int>(COLOR_MONO12, index++));
			}
			if(pDevice->m_resFMT7Descriptor.ColorMode.Mono12NormalizedInq)
			{
				strlist.push_back(COLORCODE[COLOR_MONO12_NORMALIZED]);
				pDevice->m_ColorCodeMap.insert(pair<CString, int>(COLORCODE[COLOR_MONO12_NORMALIZED], index));
				pDevice->m_ColorCodeIndex.insert(pair<HV_COLOR_CODE, int>(COLOR_MONO12_NORMALIZED, index++));
			}
			if(pDevice->m_resFMT7Descriptor.ColorMode.Mono12PackedInq)
			{
				strlist.push_back(COLORCODE[COLOR_MONO12_PACKED]);
				pDevice->m_ColorCodeMap.insert(pair<CString, int>(COLORCODE[COLOR_MONO12_PACKED], index));
				pDevice->m_ColorCodeIndex.insert(pair<HV_COLOR_CODE, int>(COLOR_MONO12_PACKED, index++));
			}
			if(pDevice->m_resFMT7Descriptor.ColorMode.Raw12Inq)	
			{
				strlist.push_back(COLORCODE[COLOR_RAW12]);
				pDevice->m_ColorCodeMap.insert(pair<CString, int>(COLORCODE[COLOR_RAW12], index));
				pDevice->m_ColorCodeIndex.insert(pair<HV_COLOR_CODE, int>(COLOR_RAW12, index++));
			}
			if(pDevice->m_resFMT7Descriptor.ColorMode.Raw12NormalizedInq)
			{
				strlist.push_back(COLORCODE[COLOR_RAW12_NORMALIZED]);
				pDevice->m_ColorCodeIndex.insert(pair<HV_COLOR_CODE, int>(COLOR_MONO8, index));
				pDevice->m_ColorCodeMap.insert(pair<CString, int>( COLORCODE[COLOR_MONO8], index++));
			}
			if(pDevice->m_resFMT7Descriptor.ColorMode.Raw12PackedInq)
			{
				strlist.push_back(COLORCODE[COLOR_RAW12_PACKED]);
				pDevice->m_ColorCodeMap.insert(pair<CString, int>( COLORCODE[COLOR_RAW12_PACKED], index));
				pDevice->m_ColorCodeIndex.insert(pair<HV_COLOR_CODE, int>(COLOR_RAW12_PACKED, index++));
			}
			if(pDevice->m_resFMT7Descriptor.ColorMode.Mono16Inq)
			{
				strlist.push_back(COLORCODE[COLOR_MONO16]);
				pDevice->m_ColorCodeMap.insert(pair<CString, int>(COLORCODE[COLOR_MONO16], index));
				pDevice->m_ColorCodeIndex.insert(pair<HV_COLOR_CODE, int>(COLOR_MONO16, index++));
			}
			if(pDevice->m_resFMT7Descriptor.ColorMode.SignedMono16Inq)
			{
				strlist.push_back(COLORCODE[COLOR_SIGNED_MONO16]);
				pDevice->m_ColorCodeMap.insert(pair<CString, int>( COLORCODE[COLOR_SIGNED_MONO16], index));
				pDevice->m_ColorCodeIndex.insert(pair<HV_COLOR_CODE, int>(COLOR_SIGNED_MONO16, index));
				index++;
			}
			if(pDevice->m_resFMT7Descriptor.ColorMode.Raw16Inq)
			{
				strlist.push_back(COLORCODE[COLOR_RAW16]);
				pDevice->m_ColorCodeMap.insert(pair<CString, int>( COLORCODE[COLOR_RAW16], index));
				pDevice->m_ColorCodeIndex.insert(pair<HV_COLOR_CODE, int>(COLOR_RAW16, index));
				index++;
			}
			if(pDevice->m_resFMT7Descriptor.ColorMode.RGB16Inq)
			{
				strlist.push_back(COLORCODE[COLOR_RGB16]);
				pDevice->m_ColorCodeMap.insert(pair<CString, int>( COLORCODE[COLOR_RGB16], index));
				pDevice->m_ColorCodeIndex.insert(pair<HV_COLOR_CODE, int>(COLOR_RGB16, index++));
			}
			if(pDevice->m_resFMT7Descriptor.ColorMode.SingnedRGB16Inq)
			{
				strlist.push_back(COLORCODE[COLOR_SIGNED_RGB16]);
				pDevice->m_ColorCodeMap.insert(pair<CString, int>( COLORCODE[COLOR_SIGNED_RGB16], index));
				pDevice->m_ColorCodeIndex.insert(pair<HV_COLOR_CODE, int>(COLOR_SIGNED_RGB16, index));
				index++;
			}
			if(pDevice->m_resFMT7Descriptor.ColorMode.JpegInq)
			{
				strlist.push_back(COLORCODE[COLOR_JPEG]);
				pDevice->m_ColorCodeMap.insert(pair<CString, int>(COLORCODE[COLOR_JPEG], index));
				pDevice->m_ColorCodeIndex.insert(pair<HV_COLOR_CODE, int>(COLOR_JPEG, index++));
			}
			if(pDevice->m_resFMT7Descriptor.ColorMode.Dyuv422Inq)
			{
				strlist.push_back(COLORCODE[COLOR_DYUV422]);
				pDevice->m_ColorCodeMap.insert(pair<CString, int>(COLORCODE[COLOR_DYUV422], index));
				pDevice->m_ColorCodeIndex.insert(pair<HV_COLOR_CODE, int>(COLOR_DYUV422, index++));
			}
			if(pDevice->m_resFMT7Descriptor.ColorMode.YUV8_4_1_1Inq)
			{
				strlist.push_back(COLORCODE[COLOR_YUV8_4_1_1]);
				pDevice->m_ColorCodeMap.insert(pair<CString, int>( COLORCODE[COLOR_YUV8_4_1_1], index));
				pDevice->m_ColorCodeIndex.insert(pair<HV_COLOR_CODE, int>(COLOR_YUV8_4_1_1, index++));
			}
			if(pDevice->m_resFMT7Descriptor.ColorMode.YUV8_4_2_2Inq)
			{
				strlist.push_back(COLORCODE[COLOR_YUV8_4_2_2]);
				pDevice->m_ColorCodeMap.insert(pair<CString, int>( COLORCODE[COLOR_YUV8_4_2_2], index));
				pDevice->m_ColorCodeIndex.insert(pair<HV_COLOR_CODE, int>(COLOR_YUV8_4_2_2, index++));
			}
			if(pDevice->m_resFMT7Descriptor.ColorMode.YUV8_4_4_4Inq)
			{
				strlist.push_back(COLORCODE[COLOR_YUV8_4_4_4]);
				pDevice->m_ColorCodeMap.insert(pair<CString, int>( COLORCODE[COLOR_YUV8_4_4_4], index));
				pDevice->m_ColorCodeIndex.insert(pair<HV_COLOR_CODE, int>(COLOR_YUV8_4_4_4, index++));
			}
		}
		
		map <CString, int>::iterator iter;
		iter = pDevice->m_ColorCodeMap.begin();
		for(int id =0; id < index; id++ )
		{
			if(iter->first == COLORCODE[pDevice->m_ColorCode])
			{
				n  = iter->second;
			}
			iter ++;
		}
		int i = 0;
		LPCTSTR list[256] = { NULL }; 
		std::list<CString>::iterator pString = NULL;
		for ( i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
		{
			list[i++] = (*pString);
		}
		list[i] = NULL;
		
		str.LoadString(IDS_RAW_TYPE_ITEM);
		HPROPERTY hName = m_ListView.AddItem( PropCreateList(str, list, n) );
		if (hName){
			m_ListView.SetItemData(hName, ITEM_8or12BitMode);
			m_ListView.SetItemEnabled(hName, 1);
		}
		
		return;
	}
	else
	{
		Spec_Fun_Interface_1(pDevice,COLOR_MODE,pDevice->m_bRaw8or12);//
	}

	strlist.clear();
	strlist.push_back("Raw8");
    strlist.push_back("Raw16");
	int i = 0;
	LPCTSTR list[256] = { NULL }; 
	std::list<CString>::iterator pString = NULL;
	for ( i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
	{
		list[i++] = (*pString);
	}
	list[i] = NULL;
	
	str.LoadString(IDS_RAW_TYPE_ITEM);
	HPROPERTY hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_bRaw8or12) );
	if (hName){
		m_ListView.SetItemData(hName, ITEM_8or12BitMode);
		m_ListView.SetItemEnabled(hName, 1);
	}
	
	//-------------------ITEM: LE_BE_MODE----------------------------------------------
//	Spec_Fun_Interface_1(pDevice,LE_BE_MODE,pDevice->m_bLeBe);
	
	HV_INTERFACE1_CONTEXT pInfcectx;
	pInfcectx.ID = LE_BE_MODE;
	pInfcectx.dwVal=(DWORD)pDevice->m_bLeBe;
	HVSTATUS status = HVCommand(m_hhv,CMD_SPEC_FUN_INTERFACE1,&pInfcectx,0);

	strlist.clear();
	strlist.push_back("BE");
    strlist.push_back("LE");
	i = 0;
  
	for ( i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
	{
		list[i++] = (*pString);
	}
	list[i] = NULL;
	
	str.LoadString(IDS_LE_BE_MODE);
	hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_bLeBe) );
	if (hName){
		m_ListView.SetItemData(hName, ITEM_LE_BE_MODE);
		m_ListView.SetItemEnabled(hName, pDevice->m_bRaw8or12);
	}
}

void CPropertyView::ListAddBandWidthOrg(CDevice *pDevice){
	
	CString str;
	str.LoadString(IDS_BANDWIDTH_ORG);
	m_ListView.AddItem(PropCreateCategory(str));
	
	if(!pDevice->m_bInit)
	{
		pDevice->m_PacketDelayRange.minV = Spec_NetWork_Get_Range(pDevice, SCALAR_PACKET_DELAY, 0);
		pDevice->m_PacketDelayRange.maxV = 32767;//15位 Spec_NetWork_Get_Range(pDevice, SCALAR_PACKET_DELAY, 1);

		//此项目前用户不可操作
		pDevice->m_ExpectDataRateRange.minV = Spec_NetWork_Get_Range(pDevice, SCALAR_EXPECT_DATA_RATE, 0);
		pDevice->m_ExpectDataRateRange.maxV = Spec_NetWork_Get_Range(pDevice, SCALAR_EXPECT_DATA_RATE, 1);

		pDevice->m_PacketDelay		= Spec_NetWork_Get(pDevice, SCALAR_PACKET_DELAY);
		pDevice->m_ExpectDataRate	= Spec_NetWork_Get(pDevice, SCALAR_EXPECT_DATA_RATE);
		pDevice->m_nConnectSpeed	= Spec_NetWork_Get(pDevice, ITEM_CONNECT_SPEED);
		pDevice->m_DelayResult		= Spec_NetWork_Get(pDevice, IDS_PACKET_DELAY_RESULT);

		if(pDevice->m_PacketDelay > 32767)
		{
			pDevice->m_PacketDelay = 32767;
			Spec_NetWork_Set(pDevice, SCALAR_PACKET_DELAY, 32767);
		}
	}
		
	//-----包间延迟
	TRACKBARINFO info;
	info.RangeMax = pDevice->m_PacketDelayRange.maxV;
	info.RangeMin = pDevice->m_PacketDelayRange.minV;
	info.LineSize = 1; 
	
	str.LoadString(IDS_PACKET_DELAY);
	HPROPERTY hName = m_ListView.AddItem( PropCreateTrack(str, pDevice->m_PacketDelay, info ));
	if (hName){
		m_ListView.SetItemData(hName, SCALAR_PACKET_DELAY);
		m_ListView.SetItemEnabled(hName, 1);
	}

	//-----预期数据率
	info.RangeMax = pDevice->m_ExpectDataRateRange.maxV;
	info.RangeMin = pDevice->m_ExpectDataRateRange.minV;
	info.LineSize = 1; 
	
	str.LoadString(IDS_DATA_RATE);
	hName = m_ListView.AddItem( PropCreateTrack(str, pDevice->m_ExpectDataRate, info ));
	if (hName){
		m_ListView.SetItemData(hName, SCALAR_EXPECT_DATA_RATE);
		m_ListView.SetItemEnabled(hName, 0);
	}
	
	//-----链接速度
	str.LoadString(IDS_CONNECT_SPEED);	
    std::list<CString> strlist;
	strlist.clear();
	strlist.push_back("10 MBits/seconds");
	strlist.push_back("100 MBits/seconds");
	strlist.push_back("1000 MBits/seconds");
	
	int i = 0;
	LPCTSTR list[256] = { NULL }; 
	std::list<CString>::iterator pString = NULL;
	for ( i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
	{
		list[i++] = (*pString);
	}
	list[i] = NULL;
	hName = m_ListView.AddItem(PropCreateList(str, list, pDevice->m_nConnectSpeed));
	if (hName){
		m_ListView.SetItemData(hName, ITEM_CONNECT_SPEED);
		m_ListView.SetItemEnabled(hName, 1);
	}
	//-----包间延迟结果
	str.LoadString(IDS_PACKET_DELAY_RESULT);
	hName = m_ListView.AddItem( PropCreateTrack(str, pDevice->m_DelayResult, info ));
	if (hName){		
		m_ListView.SetItemEnabled(hName, 0);
	}

}

void CPropertyView::ListAddAntiFilter(CDevice* pDevice)
{
	CString str;
	if(pDevice->m_flagAntiFlicker == 0)
		return;

	if(!pDevice->m_bInit)
	{
//		pDevice->m_AntiFilterInq = Spec_Fun_Get_AdvDescript(pDevice, ITEM_ANTI_FLICKER, 0);
		pDevice->m_AntiFilterState	= Spec_Fun_ADV_Get(pDevice, ITEM_ANTI_FLICKER);
		pDevice->m_AntiFilterSource = Spec_Fun_ADV_Get(pDevice, ITEM_ANTI_FLICKER_SOURCE);
	}
	str.LoadString(IDS_ANTI_FLICKER);
	m_ListView.AddItem( PropCreateCategory(str) );
	std::list<CString> strlist;
	strlist.push_back("off");
	strlist.push_back("on");

	int i = 0;
	LPCTSTR list[256] = { NULL }; 
	std::list<CString>::iterator pString = NULL;
	for (i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
	{
		list[i++] = (*pString);
	}
	list[i] = NULL;
	
	str.LoadString(IDS_ANTI_FLICKER_ONOFF);
	HPROPERTY hName = m_ListView.AddItem( PropCreateList(str, list, pDevice->m_AntiFilterState) );
	if (hName){
		m_ListView.SetItemData(hName, ITEM_ANTI_FLICKER);
		m_ListView.SetItemEnabled(hName, 1);
	}

	//触发源
	strlist.clear();
	strlist.push_back("Input0");
	strlist.push_back("Input1");
	strlist.push_back("Input2");
	
	i = 0;
	LPCTSTR list1[256] = { NULL }; 
	for (i = 0, pString = strlist.begin(); pString != strlist.end(); ++ pString )
	{
		list1[i++] = (*pString);
	}
	list1[i] = NULL;
	str.LoadString(IDS_ANTI_TRIGGERSOURCE);
	
	 hName = m_ListView.AddItem( PropCreateList(str, list1, pDevice->m_AntiFilterSource) );
	if (hName){
		m_ListView.SetItemData(hName, ITEM_ANTI_FLICKER_SOURCE);
		m_ListView.SetItemEnabled(hName, 1);
	}
	
	ListAddACPhaseAdjust(pDevice);
}

void CPropertyView::ListAddACPhaseAdjust(CDevice* pDevice)
{
	CString str;
	if(pDevice->m_flagACPhaseAdjust == 0)
		return;

	if(!pDevice->m_bInit)
	{
		pDevice->m_ACPhaseAdjust	= Spec_Fun_ADV_Get(pDevice, SCALAR_AC_PHASE_ADJUST);
		pDevice->m_ACPhaseAdjustRange.minV = Spec_Fun_Get_AdvDescript(pDevice,SCALAR_AC_PHASE_ADJUST, 0);
		pDevice->m_ACPhaseAdjustRange.maxV = Spec_Fun_Get_AdvDescript(pDevice,SCALAR_AC_PHASE_ADJUST, 1);
	}		

	TRACKBARINFO info;
	info.RangeMax = pDevice->m_ACPhaseAdjustRange.maxV;
	info.RangeMin = pDevice->m_ACPhaseAdjustRange.minV;
	info.LineSize = 1; 
	
	str.LoadString(IDS_AC_PHASE_ADJUST);
	HPROPERTY hName = m_ListView.AddItem( PropCreateTrack(str, pDevice->m_ACPhaseAdjust, info));
	if (hName){
		m_ListView.SetItemData(hName, SCALAR_AC_PHASE_ADJUST);
		m_ListView.SetItemEnabled(hName, 1);
	}
	
}

/***********************************************************************************/
//模式选择
void CPropertyView::OnVideoModeChanged1420(CDevice *pDevice, int index)
{
	try
	{	
		if ( pDevice->m_CurrentModeIdx != index )
		{
			pDevice->m_CurrentModeIdx = index;
			m_CurrentModeIdx = pDevice->m_CurrentModeIdx;
			m_CurrentMode = m_Idx2Mode[index];
			bool b=FALSE;
			if (pDevice->IsContinuousGrabActive()) {
				b = TRUE;
			}
			if (b) {
				pDevice->GrabCancel();
			}
			pDevice->SetVideoMode(m_CurrentMode);
			UpdateVideoMode(pDevice);

			if (IS_HV_CAM(pDevice->m_HVTYPE))
			{
//				pDevice->MaximizeAOI();		// by xupx 2009.11.05

				UpdatePacketSize(pDevice);
				UpdateColorMode(pDevice);
			}

			if (b){
				pDevice->GrabContinuous();
			}
		}
	}
	CATCH_REPORT();
}


void CPropertyView::OnVideoModeChanged(CDevice *pDevice, int index)
{
	try
	{	
		if ( m_CurrentModeIdx != index )
		{
			m_CurrentModeIdx = index;
			m_CurrentMode = m_Idx2Mode[index];
			pDevice->SetVideoMode(m_CurrentMode);

		}
	}
	CATCH_REPORT();
}

 



void CPropertyView::OnShutterSpeedChanged(CDevice *pDevice, int value)
{
	try
	{	
//		delete by xupx 2008.07.02.
//		reason: when the speedUnit is changed but the speed isn't changed,the value should be set.
//		if ( m_CurrentShutterSpeed != value )		
		m_CurrentShutterSpeed = value;

		pDevice->m_ShutterSpeed = value;
//		m_CurrentUnit = Spec_Fun_ADV_Get(pDevice, ITEM_SHUTTERUNIT);	//从相机读取“速度单位”

		pDevice->m_pShutter->Set(value);

		
		/*************查询一下 帧间隔*************/ 
		UpdateMultiTriggerTime(pDevice);	
		if (IS_HV_CAM(pDevice->m_HVTYPE))
		{
			bool b=FALSE;
			if (pDevice->IsContinuousGrabActive()) {
				b = TRUE;
			}
			if (b) 
				pDevice->GrabCancel();

			UpdatePacketSize(pDevice);
			if (b)
				pDevice->GrabContinuous();

		}
		
	}
	CATCH_REPORT();
}



void CPropertyView::OnBlackLevelChanged(CDevice *pDevice, int value)
{
	try
	{	
		if ( m_CurrentBlackLevel != value )
		{
			m_CurrentBlackLevel = value;
			pDevice->m_pBlackLevel->Set(m_CurrentBlackLevel);
		}
	}
	CATCH_REPORT();
}



void CPropertyView::OnGainLevelChanged(CDevice *pDevice, int value)
{
	try
	{	
		pDevice->m_pGain->Set(value);		
		pDevice->m_Gain = value;

		if (IS_HV_CAM(pDevice->m_HVTYPE))
			UpdateRGBChannel(pDevice);		// add by xupx  2009.10.15
	}
	CATCH_REPORT();
}
 

void CPropertyView::OnSnapModeChanged(CDevice *pDevice, int index)
{
	if ( m_CurrentSnapMode != index || (pDevice->m_CurrentSnapMode != index) )
	{
		m_CurrentSnapMode = index;
		pDevice->m_CurrentSnapMode = index;
		
		if (IS_CCD_CAMERA(pDevice->m_pInfo->DeviceType()))
		{
			if (m_CurrentSnapMode == 2) {
				m_CurrentSnapMode++;
			}
			
			pDevice->m_pSnapMode->Set(m_CurrentSnapMode); 
			return;
		}
	}

	try
	{	
			pDevice->m_pSnapMode->Set(m_CurrentSnapMode); 

			/// 更新 “自动闪光灯” 状态,是否可用
			if(pDevice->m_flagAutoStrobe == 1)
			{
				CString str;
				str.LoadString(IDS_STROBE_AUTO);
				HPROPERTY hName = m_ListView.FindProperty(str);				
				if (hName){
					pDevice->m_bStrobeAutoInq = Spec_Fun_AutoDescript(pDevice,ITEM_STROBE_AUTO, 2);
					m_ListView.SetItemEnabled(hName, pDevice->m_bStrobeAutoInq);
				}

				/// 不可用时，显示Disable；可用时，读取相机值。
				if(pDevice->m_bStrobeAutoInq == 1)
					pDevice->m_bStrobeAuto = Spec_Fun_Get_AutoFunc(pDevice,ITEM_STROBE_AUTO);
				else
					pDevice->m_bStrobeAuto = 0;
				

				switch(pDevice->m_bStrobeAuto) {
				case 0:
					str.Format("%s", "Disable");
					break;
				case 1:
					str.Format("%s", "Enable");
					break;
				}
				CComVariant v(str);
				m_ListView.SetItemValue(hName, &v);
				m_ListView.SetItemEnabled(hName,  pDevice->m_bStrobeAutoInq);

				/// 在连续采集状态时，将 "闪光灯开关"设为可以操作
				str.LoadString(IDS_STROBE_SWITCH);
				hName = m_ListView.FindProperty(str);				
				if(pDevice->m_CurrentSnapMode == 0)
				{
					m_ListView.SetItemEnabled(hName,  1);
				}
				else
				{
					m_ListView.SetItemEnabled(hName, !pDevice->m_bStrobeAuto);
				}
					
			}
			//////////////////////////////////////////////////////////////////////////
			
			//更新2A状态
			UpdateGain(pDevice);
			UpdateShutter(pDevice);
			if (IS_HV_CAM(pDevice->m_HVTYPE))
			{
				bool b=FALSE;
				if (pDevice->IsContinuousGrabActive()) {
					b = TRUE;
				}
				if (b) {
					pDevice->GrabCancel();
				}
				UpdatePacketSize(pDevice);
				if (b){
					pDevice->GrabContinuous();
				}

			}
			/// 只有1421需要更新自动模式
			CString str;
			HPROPERTY hName;

			if(pDevice->m_pInfo->DeviceType() == DRVIFGIGETYPE)
			{
				
				//当连续状态时，被动传输不可操作
				if(pDevice->m_flagPassive == 1)
				{
					str.LoadString(IDS_PASSIVE_TRANSMIT_ENABLE);
					hName = m_ListView.FindProperty(str);	
					int flag = 0;
					if (hName){
						m_ListView.SetItemEnabled(hName, flag = Spec_Fun_Get_AdvDescript(pDevice,ITEM_PASSIVE_TRANSMIT, 0));
					}
					if( flag == 1)
					{
						pDevice->m_PassiveTransOnOff = Spec_Fun_ADV_Get(pDevice, ITEM_PASSIVE_TRANSMIT);
						
						/*** 更新属性栏显示***/
						str.LoadString(IDS_PASSIVE_TRANSMIT_ENABLE);
						hName = m_ListView.FindProperty(str);
						str.Format("%d", pDevice->m_PassiveTransOnOff);
						CComVariant v1(str);
						m_ListView.SetItemValue(hName, &v1);
						/********************/

						pDevice->m_PassiveTransBufNum = Spec_Fun_ADV_Get(pDevice, ID_DO_PASSIVE_TRANS);
					}
				}
			}
			
			/*************查询一下 帧间隔*************/ 
			UpdateMultiTriggerTime(pDevice);
			if(IS_GIGE(pDevice->m_pInfo->DeviceType()) || IS_PD_CAM(pDevice->m_pInfo->DeviceType()))
				UpdateTriggerDelay(pDevice);
			
	}
	CATCH_REPORT();
}

void CPropertyView::OnTriggerPolarityChanged(CDevice *pDevice, int index)
{
	try
	{	
		if ( (m_CurrentTriggerPolarity != index) || (pDevice->m_CurrentTriggerPolarity != index))
		{
			m_CurrentTriggerPolarity = index;
			pDevice->m_CurrentTriggerPolarity = index;
			pDevice->m_pTriggerPolarity->Set(m_CurrentTriggerPolarity);
		}
	}
	CATCH_REPORT();
}

void CPropertyView::OnStrobePolarityChanged(CDevice *pDevice, int index)
{
	try
	{	
		if ( m_CurrentStrobePolarity != index || (pDevice->m_CurrentStrobePolarity != index))
		{
			m_CurrentStrobePolarity = index;
			pDevice->m_CurrentStrobePolarity = index;
			pDevice->m_pStrobePolarity->Set(m_CurrentStrobePolarity);
		}
	}
	CATCH_REPORT();
}


void CPropertyView::OnADCLevelChanged(CDevice *pDevice, int index)
{
	HVTYPE type = pDevice->m_pInfo->DeviceType();
	try
	{	
        if (IS_CCD_CAMERA(type) || IS_READABLE_CAM(type)) 
		{
			///* LUT 文件方式
            CString str;
            str.LoadString(IDS_LUTPATH);
            HPROPERTY hName = m_ListView.FindProperty(str);
            char buf[1024];
            hName->GetDisplayValue(buf, -1);
			
			pDevice->m_LutPath.Format("%s",buf);
		
			
       //     DWORD* p = new DWORD[4*1024];
            DWORD Readed = 0;
            HANDLE hFile = ::CreateFile(buf, GENERIC_READ,
				FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			BOOL b;
			b = ReadFile(hFile, pDevice->m_dwLut, 16*1024, &Readed, NULL);

            CloseHandle(hFile);
            if (Readed == 16*1024) {
                pDevice->LoadLut(pDevice->m_dwLut, 16*1024,0);
            }

         //   delete p;
			
        } else {
            if ( m_CurrentADLevel != index )
            {
                m_CurrentADLevel = index;
                pDevice->m_pADCLevel->Set(m_CurrentADLevel);
            }
        }
	}
	CATCH_REPORT();
}

void CPropertyView::OnBlackLevelEnableChanged(CDevice *pDevice, int index)
{
	try
	{	
		if ( m_CurrentBlackLevelEnable != index )
		{
			m_CurrentBlackLevelEnable = index;
			pDevice->m_pBlackLevelEnable->Set(m_CurrentBlackLevelEnable);
			CString str;
			str.LoadString(IDS_BLACKLEVEL);
			HPROPERTY hName = m_ListView.FindProperty(str);
			if (hName){
				m_ListView.SetItemEnabled(hName, index);
			}
		}
	}
	CATCH_REPORT();
}



void CPropertyView::OnBlankHorChanged(CDevice *pDevice, int value)
{
	try
	{	
		if ( m_CurrentBlankHor != value )
		{
			m_CurrentBlankHor = value;
			pDevice->m_pBlank->SetHor(m_CurrentBlankHor);
		}
	}
	CATCH_REPORT();
}


void CPropertyView::OnBlankVertChanged(CDevice *pDevice, int value)
{
	try
	{	
		if ( m_CurrentBlankVert != value )
		{
			m_CurrentBlankVert = value;
			pDevice->m_pBlank->SetVert(m_CurrentBlankVert);
		}
	}
	CATCH_REPORT();
}

void CPropertyView::OnTriggerDelayUnitChange(CDevice* pDevice, int value)
{
	Set_Adv_Feature(pDevice, ITEM_TRIGGERDELAY_UNIT, value);
	pDevice->m_TriggerDelayUnit		= value;
	pDevice->m_nTriggerDelayValue	= Spec_Fun_Get(pDevice, SCALAR_TRIGGERDELAY_VALUE);	
	pDevice->m_TriggerDelayRange.maxV = Spec_Fun_Get_Descript(pDevice, SCALAR_TRIGGERDELAY_VALUE, 1);
	pDevice->m_TriggerDelayRange.minV = Spec_Fun_Get_Descript(pDevice, SCALAR_TRIGGERDELAY_VALUE, 0);

	CString strVal;
	strVal.LoadString(IDS_TRIGGER_DELAY_VALUE);
	HPROPERTY hName = m_ListView.FindProperty(strVal);

	if(hName){
		m_ListView.SetItemRangeValue(hName, pDevice->m_TriggerDelayRange.minV, pDevice->m_TriggerDelayRange.maxV);
		strVal.Format("%d", pDevice->m_nTriggerDelayValue);
		CComVariant v(strVal);
		m_ListView.SetItemValue(hName, &v);
	}
	
}

void CPropertyView::OnShutterUnitChanged_NetCam(CDevice *pDevice, int index)
{
	try
	{	
		if ( pDevice->m_ShutterUnit != index )
		{
			pDevice->m_ShutterUnit = index;
			pDevice->m_pShutter->m_Unit.Set((TUnit)index);
            pDevice->SetShutterUnit(index);		// xupx test 0514
			
			CString str;
			str.LoadString(IDS_SHUTTERRAW);
			HPROPERTY hRaw = m_ListView.FindProperty(str);
			
			if (hRaw){
				str.Format("%d", pDevice->m_pShutter->Raw());
				CComVariant v(str);
				m_ListView.SetItemValue(hRaw, &v);
			}
			
			str.LoadString(IDS_SHUTTERSPEED);
			HPROPERTY hName = m_ListView.FindProperty(str);
			//读取 shutter的取值范围 by xupx 2008.09.10
			pDevice->m_ShutterRange.minV = Spec_Fun_Get_Descript(pDevice, SCALAR_SHUTTERSPEED, 0);
			pDevice->m_ShutterRange.maxV = Spec_Fun_Get_Descript(pDevice, SCALAR_SHUTTERSPEED, 1);
			pDevice->m_ShutterSpeed = Spec_Fun_Get(pDevice, SCALAR_SHUTTERSPEED);
			
			if (hName){

				m_ListView.SetItemRangeValue(hName, pDevice->m_ShutterRange.minV, pDevice->m_ShutterRange.maxV);
				OnShutterSpeedChanged(pDevice,pDevice->m_ShutterSpeed);
				
				str.Format("%d", pDevice->m_pShutter->Value());
				CComVariant v(str);
				m_ListView.SetItemValue(hName, &v);
			}
		}
		/*************查询一下 帧间隔*************/ 
		UpdateMultiTriggerTime(pDevice);		
		if (IS_HV_CAM(pDevice->m_HVTYPE))
		{
			bool b=FALSE;
			if (pDevice->IsContinuousGrabActive()) {
				b = TRUE;
			}
			if (b) {
				pDevice->GrabCancel();
			}
			UpdatePacketSize(pDevice);
			if (b){
				pDevice->GrabContinuous();
			}
		}

		
	}
	CATCH_REPORT();	
}

void CPropertyView::OnShutterUnitChanged(CDevice *pDevice, int index)
{
	try
	{	
		if ( m_CurrentUnit != index )
		{
			m_CurrentUnit = index;
			pDevice->m_pShutter->m_Unit.Set((TUnit)index);

            if (IS_CCD_CAMERA(pDevice->m_pInfo->DeviceType())) {
                pDevice->SetShutterUnit(index);
            } else {
                pDevice->m_pShutter->Refresh();
            }
                
			
			CString str;
			str.LoadString(IDS_SHUTTERRAW);
			HPROPERTY hRaw = m_ListView.FindProperty(str);
			if (hRaw){
				str.Format("%d", pDevice->m_pShutter->Raw());
				CComVariant v(str);
				m_ListView.SetItemValue(hRaw, &v);
			}
 
			str.LoadString(IDS_SHUTTERSPEED);
			HPROPERTY hName = m_ListView.FindProperty(str);
			if (hName){
				switch(index)
				{
				case 0:
					m_ListView.SetItemRangeValue(hName, 20,1000);
					break;
				case 1:
					m_ListView.SetItemRangeValue(hName, 0,3000);
					break;
				}
				OnShutterSpeedChanged(pDevice,60);
				
				str.Format("%d", pDevice->m_pShutter->Value());
				CComVariant v(str);
				m_ListView.SetItemValue(hName, &v);
			}
		}
		
	}
	CATCH_REPORT();
}



void CPropertyView::OnFilterTimeUnitChanged(CDevice *pDevice, int index)
{
	
	pDevice->m_nFilterTimeUnit = index;  
    
    try { 
		Spec_Fun_Interface_1(pDevice,FILTER_TIME_UNIT,index);	

		// UPDATE MaxV ...
		CString str;
		str.LoadString(IDS_FILTER_TIME);
		HPROPERTY hName = m_ListView.FindProperty(str);
		if (hName){
			switch(index)
			{
			case 0: // us
				m_ListView.SetItemRangeValue(hName, 0,2000);
				break;
			case 1: // ns
				m_ListView.SetItemRangeValue(hName, 0,1000);
				break;
			} 
			OnFilterTimeChanged(pDevice, 10);
			str.Format("%d", pDevice->m_nFilterTime);
			CComVariant v(str);
			m_ListView.SetItemValue(hName, &v);
		}	         
    }
    CATCH_REPORT();
 
}



void CPropertyView::OnBrightnessChanged(CDevice* pDevice, int value)
{
    int nVal = (value & 0xff);

    try {
        pDevice->m_Brightness = nVal;
        pDevice->Set_Brightness();
    }
    CATCH_REPORT();
}

void CPropertyView::OnWhiteBalanceUChanged(CDevice* pDevice, int value)
{
	//****用于1410GC
	if(IS_READABLE_CAM(pDevice->m_pInfo->DeviceType()))
	{
		int nVal = (value & 0x3f);
		
		try {
			pDevice->m_BlueGain = nVal;
			pDevice->Set_WB_Blue(nVal);
			
		}
		CATCH_REPORT();
		return;
	}
	//****End 1410GC

    int nVal = (value & 0x1f);

    try {
        pDevice->m_BlueGain = nVal;
		pDevice->Set_WB_Blue(nVal);
		
    }
    CATCH_REPORT();
}

void CPropertyView::OnWhiteBalanceGChanged(CDevice* pDevice, int value)
{
	//****用于1410GC
	if(IS_READABLE_CAM(pDevice->m_pInfo->DeviceType()))
	{
		int nVal = value;
		if (!IS_HV_CAM(pDevice->m_HVTYPE))
		{
			nVal = (value & 0x3f);
		}
		
		try {
			pDevice->m_GreenGain = nVal;
			pDevice->Set_WB_Green(nVal);
			if (IS_HV_CAM(pDevice->m_HVTYPE))
			{
				UpdateGainLevel(pDevice, 1);		// add by xupx 2009.10.15
			}
			
		}
		CATCH_REPORT();
		return;
	}
}

void CPropertyView::OnWhiteBalanceVChanged(CDevice* pDevice, int value)
{
	//****用于1410GC
	if(IS_READABLE_CAM(pDevice->m_pInfo->DeviceType()))
	{
		int nVal = (value & 0x3f);
		
		try {
			pDevice->m_RedGain = nVal;
			pDevice->Set_WB_Red(nVal);
			
		}
		CATCH_REPORT();
		return;
	}
	//****End 1410GC

    int nVal = (value & 0x1f);
    
    try {
        pDevice->m_RedGain = nVal;
		pDevice->Set_WB_Red(nVal);		
    }
    CATCH_REPORT();
}

void CPropertyView::OnWBCtrl(CDevice* pDevice, int FuncID, int value)
{
	HVAPI_CONTROL_PARAMETER  param;
	
	HV_ARG_SET_FEATURE ArgFeature;
	ArgFeature.FeatureId	= FEATURE_ID_WHITE_BALANCE;

	param.code		 = ORD_SET_FEATURE;
	param.pInBuf	 = &ArgFeature;
	param.dwInBufSize= sizeof(ArgFeature);	
	param.pOutBuf	 = NULL;
	param.dwOutBufSize = 0;
	param.pBytesRet = NULL;
	switch(FuncID) {
	case ITEM_WB_ON_OFF:
		ArgFeature.FeatureItemId= FEATURE_ITEM_ID_WHITE_BALANCE_ON_OFF;
		ArgFeature.Feature.WhiteBalance.OnOff = value;
		pDevice->m_WB_OnOff = value;
		break;
	case ITEM_WB_AUTO_MANU:
		ArgFeature.FeatureItemId= FEATURE_ITEM_ID_WHITE_BALANCE_AUTO_MANUAL_MODE;
		ArgFeature.Feature.WhiteBalance.AutoManualMode = value;
		pDevice->m_WB_AutoManual = value;
		break;
	case ITEM_WB_ONE_PUSH:
		ArgFeature.FeatureItemId= FEATURE_ITEM_ID_WHITE_BALANCE_ONE_PUSH;
		ArgFeature.Feature.WhiteBalance.OnePush = value;
		pDevice->m_WB_OnePush = value;
		break;
	case SCALAR_R_VALUE:
		ArgFeature.FeatureItemId= FEATURE_ITEM_ID_WHITE_BALANCE_VR;
		ArgFeature.Feature.WhiteBalance.VR = value;
		pDevice->m_R_Value = value;
		break;
	case SCALAR_B_VALUE:
		ArgFeature.FeatureItemId= FEATURE_ITEM_ID_WHITE_BALANCE_UB;
		ArgFeature.Feature.WhiteBalance.UB = value;
		pDevice->m_B_Value = value;
		break;
		
		
	default:
		break;
	}

	HVSTATUS status = HVCommand(m_hhv, CMD_HVAPI_CONTROL, &param, 0);
	
	if ( ! HV_SUCCESS(status))
		throw HVBaseException(status, "HVDAILT.dll : HVCommand (OnWBCtrl)");

}

void CPropertyView::OnFrameFrozenChanged(CDevice* pDevice, int index)
{
    try
    {	
            pDevice->Set_Frame_Frozen(index);
    }
    CATCH_REPORT();
}

void CPropertyView::OnStrobeOnOffChanged(CDevice*pDevice,int value)
{
	pDevice->m_StrobeOnOff = value;
    Spec_Fun_Interface_1(pDevice,STROBE_ON_OFF,value);

}
void CPropertyView::OnTestImageChanged(CDevice *pDevice, int value)
{
	pDevice->m_nTestImage=value;
    Spec_Fun_Interface_1(pDevice,TEST_IMAGE,value);
	
}
void CPropertyView::OnTriggerDelayChanged(CDevice*pDevice,int value)
{
	
	pDevice->m_bTriggerDelayOnOff=value;
	Spec_Fun_Interface_1(pDevice,TRIGGER_DELAY_ON_OFF,value);
	UpdateTriggerDelay(pDevice);	
	
}

void CPropertyView::OnTriggerDelayValueChanged(CDevice*pDevice,int value)
{
	if(pDevice->m_nTriggerDelayValue!=value)
	{
		pDevice->m_nTriggerDelayValue=value;
		Spec_Fun_Interface_1(pDevice,TRIGGER_DELAY_VALUE,value);		
	}	
}

void CPropertyView::OnTransfersDelayValueChanged(CDevice*pDevice,int value)
{
	if(pDevice->m_nTransfersDelayValue!=value)
	{
		pDevice->m_nTransfersDelayValue=value;
		Spec_Fun_Interface_1(pDevice,TRANSFERS_DELAY_VALUE,value);
		
	}
	
}

void CPropertyView::OnFilterTimeChanged(CDevice*pDevice,int value)
{
	ATLTRACE("CPropertyView::OnFilterTimeChanged");
	if(pDevice->m_nFilterTime!=value)
	{
		pDevice->m_nFilterTime=value;
		Spec_Fun_Interface_1(pDevice,FILTER_TIME,value);
	}
}


void CPropertyView::On8or12BitModeChanged(CDevice*pDevice,int value)
{
	BOOL bRestart = FALSE;
	HVSTATUS status = STATUS_OK;
	if (pDevice->IsContinuousGrabActive()) {
		bRestart = TRUE;
	}

	try {
		if (bRestart) {
			pDevice->GrabCancel();
		}

		// GM Cam add by xupx 2008.12.25
		HVTYPE type = pDevice->m_pInfo->DeviceType();
		if(  IS_READABLE_CAM(type))		// modified by xupx 2009.10.13  type == DRVIFGIGETYPE || IS_PD_CAM(type) ||
		{
			map <HV_COLOR_CODE, int>::iterator iter;
			iter = pDevice->m_ColorCodeIndex.begin();
			for(int id =0; id < pDevice->m_ColorCodeIndex.size(); id++ )
			{
				if(iter->second == value)
				{
					pDevice->m_ColorCode  = iter->first;
					break;
				}
				iter ++;
			}
			HV_VIDEO_MODE mode;
			mode = pDevice->GetVideoMode();
			Spec_Fun_SetColorCode(pDevice, pDevice->m_ColorCode, mode);

			/********更新 分辨率尺寸 2009.06.30***********/
			if(IS_PD_CAM(type))
			{
				UpdateVideoMode(pDevice);
				UpdateShutter(pDevice);
				UpdateGain(pDevice);
			}			
			if (IS_HV_CAM(pDevice->m_HVTYPE))
			{
				UpdatePacketSize(pDevice);
			}
			/***************** ***************************/
			
			pDevice->ConfigurationChanged(); // notify the plugin add by xupx 2009.02.09
			
			/// 更新数字增益是否可用 by xupx 2009.04.15
			if(!m_IS_OldITS && !m_IS_OldGC && !m_IS_OldGM && pDevice->m_flagDigitalGain == 1)
			{
				UpdateDigitalGain(pDevice);
			}
			///end 更新界面
			
			if (bRestart) {
				pDevice->GrabContinuous();
			}
			return;
		}
		// end GM Cam
		else
		{
			pDevice->m_bRaw8or12=value;//存记录
			Spec_Fun_Interface_1(pDevice,COLOR_MODE, value);//
		}

		pDevice->ConfigurationChanged(); // notify the plugin add by xupx 2009.02.09
		
		if (bRestart) {
			pDevice->GrabContinuous();
		}
		
		//  only Raw12 mode can set LE_BE_MODE        	
		CString str;
		str.LoadString(IDS_LE_BE_MODE);
		HPROPERTY hName = m_ListView.FindProperty(str);
		if (hName)
		{
			if(1 == pDevice->m_bRaw8or12){m_ListView.SetItemEnabled(hName, 1);}
			else{m_ListView.SetItemEnabled(hName, 0);}
		}    
	}
	CATCH_REPORT();	
}

void CPropertyView::OnLeBeModeChanged(CDevice*pDevice,int value)
{
	pDevice->m_bLeBe = value;  
    
    try { 
		Spec_Fun_Interface_1(pDevice,LE_BE_MODE,value);		         
    }
    CATCH_REPORT();
	
}


void CPropertyView::OnOutPutIO_0Changed(CDevice*pDevice,int value)
{
	pDevice->m_nOutPutIO_0=value;
	Spec_Fun_Interface_1(pDevice,OUTPUT_IO_0,value);
	
	if(IS_READABLE_CAM(pDevice->m_pInfo->DeviceType()))
	{
		UpdateOutputIoControl(pDevice, ITEM_OUTPUTIO_0_CONTROL, IDS_OUTPUTIOCONTROL_0);
		return;
	}
	//  only user set mode can set low high         	
	CString str;
	str.LoadString(IDS_OUTPUTIOCONTROL_0);
	HPROPERTY hName = m_ListView.FindProperty(str);
	if (hName)
	{
		if(value==2){m_ListView.SetItemEnabled(hName, 1);}
		else{m_ListView.SetItemEnabled(hName, 0);}
	}
}


void CPropertyView::OnOutPutIO_1Changed(CDevice*pDevice,int value)
{
	pDevice->m_nOutPutIO_1=value;
	Spec_Fun_Interface_1(pDevice,OUTPUT_IO_1,value);
	
	if(IS_READABLE_CAM(pDevice->m_pInfo->DeviceType()))
	{
		UpdateOutputIoControl(pDevice, ITEM_OUTPUTIO_1_CONTROL, IDS_OUTPUTIOCONTROL_1);
		return;
	}
	//  only user_set_mode can set low high polarity   	
	CString str;
	str.LoadString(IDS_OUTPUTIOCONTROL_1);
	HPROPERTY hName = m_ListView.FindProperty(str);
	if (hName)
	{
		if(value==2){m_ListView.SetItemEnabled(hName, 1);}
		else{m_ListView.SetItemEnabled(hName, 0);}
	}
}

void CPropertyView::OnOutPutIO_2Changed(CDevice*pDevice,int value)
{
	pDevice->m_nOutPutIO_2=value;
    Spec_Fun_Interface_1(pDevice,OUTPUT_IO_2,value);

	if(IS_READABLE_CAM(pDevice->m_pInfo->DeviceType()))
	{
		UpdateOutputIoControl(pDevice, ITEM_OUTPUTIO_2_CONTROL, IDS_OUTPUTIOCONTROL_2);
		return;
	}
	//  only user set mode can set low high   	
	CString str;
	str.LoadString(IDS_OUTPUTIOCONTROL_2);
	HPROPERTY hName = m_ListView.FindProperty(str);
	if (hName)
	{
		if(value==2){m_ListView.SetItemEnabled(hName, 1);}
		else{m_ListView.SetItemEnabled(hName, 0);}
	}
	//
	
}

void CPropertyView::OnOutPutIO_ControlChanged(CDevice*pDevice,HV_INTERFACE1_ID FuncID, int value)
{
	switch(FuncID) {
	case OUTPUT_IO_0_POL:
		pDevice->m_nOutPutIO_0_POL=value;
		break;
	case OUTPUT_IO_1_POL:
		pDevice->m_nOutPutIO_1_POL=value;
		break;
	case OUTPUT_IO_2_POL:
		pDevice->m_nOutPutIO_2_POL=value;
		break;
	default:
		break;
	}
	Spec_Fun_Interface_1(pDevice, FuncID, value);

}

void CPropertyView::OnInPutIOSetChanged(CDevice*pDevice,int value) 
{
	pDevice->m_nTriggerSource=value;
	Spec_Fun_Interface_1(pDevice,TRIGGER_SOURCE_IO,value);
	
}


void CPropertyView::OnShutterAuto(CDevice *pDevice, int index)
{
	//更新属性界面的使能状态
	CString str;
	str.LoadString(IDS_SHUTTERSPEED);
	HPROPERTY hName = m_ListView.FindProperty(str);		//快门速度

	CString str1;
	str1.LoadString(IDS_TIME_UNIT);
	HPROPERTY hName1 = m_ListView.FindProperty(str1);	//速度单位

	if(pDevice->m_bShutterAuto != index)
	{
		pDevice->m_bShutterAuto = index;
		Set_Shutter_Auto(pDevice, index);
		UpdateShutter(pDevice);
		/*************查询一下 帧间隔*************/ 
		UpdateMultiTriggerTime(pDevice);			
		/*************2009.04.30 End***************/
	}   

}

void CPropertyView::OnGainAuto(CDevice *pDevice, int index)
{
//修改属性栏的使能状态，在设为手动时读取增益参数
	CString str;
	str.LoadString(IDS_GAINLEVEL);
	HPROPERTY hName = m_ListView.FindProperty(str);
	

	if(pDevice->m_bGainAuto != index)		
	{
		pDevice->m_bGainAuto = index;
		Set_Gain_Auto(pDevice, index);
		UpdateGain(pDevice);

		/*************查询一下 帧间隔*************/ 
		UpdateMultiTriggerTime(pDevice);			
		/*************2009.04.30 End***************/
	}    
	
}

void CPropertyView::OnStrobeAuto(CDevice *pDevice, int index)
{
	if(pDevice->m_bStrobeAuto != index)		
	{
		pDevice->m_bStrobeAuto = index;
		Set_AutoFunc(pDevice, ITEM_STROBE_AUTO, pDevice->m_bStrobeAuto);
		
		/*************查询一下 帧间隔*************/ 
		//读取 闪光灯开关 状态
		pDevice->m_StrobeOnOff	= pDevice->GetParameter(IDS_STROBEPOLARITY, 0);
	
		CString str;
		str.LoadString(IDS_STROBE_SWITCH);
		HPROPERTY hName = m_ListView.FindProperty(str);		
		
		switch(pDevice->m_StrobeOnOff) {
		case 0:
			str.Format("%s", "off");
			break;
		case 1:
			str.Format("%s", "on");
			break;
		}
		CComVariant v(str);
		m_ListView.SetItemValue(hName, &v);
		m_ListView.SetItemEnabled(hName,  !index);
		
		UpdateShutter(pDevice);
		UpdateGain(pDevice);
		UpdateMultiTriggerTime(pDevice);
		/*************2009.04.30 End***************/
	}
}

void CPropertyView::OnBeginSave1410GC(CDevice *pDevice, int value)
{
	pDevice->m_nBeginParam = value;
	Spec_Fun_StartParam(pDevice);
}

void CPropertyView::OnSaveParam1410GC(CDevice *pDevice, int value)
{
	pDevice->m_nSaveParam = value;
	Spec_Fun_SaveParam(pDevice);
}

void CPropertyView::OnLoadParam1410GC(CDevice *pDevice, int value)
{
	bool b=FALSE;
	if (pDevice->IsContinuousGrabActive()) {
		b = TRUE;
	}
	if (b) {
		pDevice->GrabCancel();
	}
	pDevice->m_nLoadParam = value;
	Spec_Fun_LoadParam(pDevice);
	//---读取一下采集方式---
	m_CurrentSnapMode = pDevice->GetParameter(IDS_SNAPMODE, 0);	
	pDevice->m_pSnapMode->Set(m_CurrentSnapMode); 
	//---end
	pDevice->m_bInit = FALSE;
	Update_PRP(pDevice);

	pDevice->ConfigurationChanged();	//add by xupx 2009.04.23
	
	if (b){
		pDevice->GrabContinuous();
	}
}

void CPropertyView::OnNetWorkSetting(CDevice *pDevice, int FunID, int nValue)
{
	switch(FunID) {
	case SCALAR_PACKET_DELAY:
		if(pDevice->m_PacketDelay == nValue)
			return;
		break;
	case SCALAR_EXPECT_DATA_RATE:
		if(pDevice->m_ExpectDataRate == nValue)
			return;
		break;
	case ITEM_CONNECT_SPEED:
		if(pDevice->m_nConnectSpeed == nValue)
			return;
		break;
	default:
		break;
	}

	//----------------------
	bool b=FALSE;
	if (pDevice->IsContinuousGrabActive()) {
		b = TRUE;
	}
	if (b) {
		pDevice->GrabCancel();
	}

	Spec_NetWork_Set(pDevice, FunID, nValue);
	pDevice->m_DelayResult = Spec_NetWork_Get(pDevice, IDS_PACKET_DELAY_RESULT);
	RefreshDelayResult(pDevice->m_DelayResult);

	if (b){
		pDevice->GrabContinuous();
	}

}

void CPropertyView::OnAntiFlicker(CDevice* pDevice, int value)
{
	Set_Adv_Feature(pDevice, ITEM_ANTI_FLICKER, value);
	pDevice->m_AntiFilterState = Spec_Fun_ADV_Get(pDevice, ITEM_ANTI_FLICKER);
	
	/********************************************/
	HPROPERTY hNameOnOff;
	CString str;
	
	str.LoadString(IDS_ANTI_FLICKER_ONOFF);
	hNameOnOff = m_ListView.FindProperty(str);	
	

	switch(pDevice->m_AntiFilterState) {
	case 0:
		str.Format("%s", "off");
		break;
	case 1:
		str.Format("%s", "on");
		break;
	default:
		break;
	}
	CComVariant v(str);
	m_ListView.SetItemValue(hNameOnOff, &v);
	m_ListView.SetItemEnabled(hNameOnOff, 1);
	
}

void CPropertyView::OnAntiFlickerSource(CDevice* pDevice, int value)
{
	Set_Adv_Feature(pDevice, ITEM_ANTI_FLICKER_SOURCE, value);
//	pDevice->m_AntiFilterState = Spec_Fun_ADV_Get(pDevice, ITEM_ANTI_FLICKER);
	
	/*****************************************
	HPROPERTY hNameOnOff;
	CString str;
	
	str.LoadString(IDS_ANTI_FLICKER_ONOFF);
	hNameOnOff = m_ListView.FindProperty(str);	
	
	
	switch(pDevice->m_AntiFilterState) {
	case 0:
		str.Format("%s", "off");
		break;
	case 1:
		str.Format("%s", "on");
		break;
	default:
		break;
	}
	CComVariant v(str);
	m_ListView.SetItemValue(hNameOnOff, &v);
	m_ListView.SetItemEnabled(hNameOnOff, 1);***/
	
}

void CPropertyView::OnACPhaseAdjust(CDevice* pDevice, int value)
{
	Set_Adv_Feature(pDevice, SCALAR_AC_PHASE_ADJUST, value);	
}

void CPropertyView::Spec_Fun_Interface_1(CDevice*pDevice, HV_INTERFACE1_ID Interface_ID, int value)
{
	HV_INTERFACE1_CONTEXT pInfcectx;
	pInfcectx.ID = Interface_ID;
	pInfcectx.dwVal=(DWORD)value;
	HVSTATUS status = HVCommand(m_hhv,CMD_SPEC_FUN_INTERFACE1,&pInfcectx,0);
	if ( ! HV_SUCCESS(status))
	   throw HVBaseException(status, "HVDAILT.dll : HVCommand");
}


CString CPropertyView::Spec_Fun_Address_Get(CDevice*pDevice, int FuncID)
{
	HVAPI_CONTROL_PARAMETER  p;
	HV_ARG_QUERY_NET_FUNC_DESCRIPTOR ArgNetFuncDesc;
	HV_RES_QUERY_NET_FUNC_DESCRIPTOR ResNetFuncDesc;
	HV_ARG_GET_NET_FUNC  ArgGetNetFunc;
	HV_RES_GET_NET_FUNC  ResGetNetFunc;
	
	HVSTATUS status;
	
	CString str;
	char* mStr;
	char *pAdress;
	
	switch(FuncID) {
	case IDS_NETWORK_CARD_IP:
		ArgNetFuncDesc.NetFuncId = NET_FUNC_ID_HOST_IP;
		ArgGetNetFunc.NetFuncId	 = NET_FUNC_ID_HOST_IP;
		break;
	case IDS_IP_ADDRESS:
		ArgNetFuncDesc.NetFuncId = NET_FUNC_ID_DEVICE_IP;	
		ArgGetNetFunc.NetFuncId	 = NET_FUNC_ID_DEVICE_IP;
		break;
	case IDS_MAC_ADDRESS:
		ArgNetFuncDesc.NetFuncId = NET_FUNC_ID_DEVICE_MAC;		
		ArgGetNetFunc.NetFuncId  = NET_FUNC_ID_DEVICE_MAC;
		break;
	case IDS_NETWORK_CARD_MAC:
		ArgNetFuncDesc.NetFuncId = NET_FUNC_ID_HOST_MAC;
		ArgGetNetFunc.NetFuncId  = NET_FUNC_ID_HOST_MAC;
		break;
	default:
		break;
	}
	p.code			 = ORD_QUERY_NET_FUNC_DESCRIPTOR;
	p.pOutBuf		 = &ResNetFuncDesc;
	p.dwOutBufSize	 = sizeof(ResNetFuncDesc);
	p.pInBuf		 = &ArgNetFuncDesc;
	p.dwInBufSize	 = sizeof(ArgNetFuncDesc);
	p.pBytesRet		 = NULL;
	
	status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
	
	//--------------------
	p.code = ORD_GET_NET_FUNC;
	pAdress = new char[ResNetFuncDesc.Address.dwTotalByteLength+1];
	ResGetNetFunc.Address.pBuffer =  pAdress;
	
	p.pInBuf		 = &ArgGetNetFunc;
	p.dwInBufSize	 = sizeof(ArgGetNetFunc);
	p.pOutBuf		 = &ResGetNetFunc;
	p.dwOutBufSize	 = sizeof(ResGetNetFunc);
	
	status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
	
	mStr = ResGetNetFunc.Address.pBuffer;
	
	str.Format("%s",mStr);
	if ( ! HV_SUCCESS(status))
		throw HVBaseException(status, "HVDAILT.dll : HVCommand (Spec_Fun_Address_Get)");
	
	delete []pAdress;
	
	return str;
}

int CPropertyView::Spec_NetWork_Get(CDevice *pDevice, int readFunID)
{
	HVAPI_CONTROL_PARAMETER  p;
	HV_ARG_GET_NET_FUNC  ArgGetNetFunc;
	HV_RES_GET_NET_FUNC  ResGetNetFunc;
	
	HVSTATUS status;
	int nValue;
	
	p.pInBuf		= &ArgGetNetFunc;
	p.dwInBufSize	= sizeof(ArgGetNetFunc);
	p.pOutBuf		= &ResGetNetFunc;
	p.dwOutBufSize	= sizeof(ResGetNetFunc);
	p.code			= ORD_GET_NET_FUNC;
	p.pBytesRet		= NULL;

	switch(readFunID) {
	case SCALAR_PACKET_DELAY:
		ArgGetNetFunc.NetFuncId = NET_FUNC_ID_INTER_PACKET_DELAY;
		status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
		nValue = ResGetNetFunc.Scalar.dwValue;
		break;
	case SCALAR_EXPECT_DATA_RATE:
		ArgGetNetFunc.NetFuncId = NET_FUNC_ID_EXPECT_DATA_RATE;
		status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
		nValue = ResGetNetFunc.Scalar.dwValue;
		break;
	case ITEM_CONNECT_SPEED:
		ArgGetNetFunc.NetFuncId = NET_FUNC_ID_LINK_SPEED;
		status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
		nValue = ResGetNetFunc.LinkSpead.dwValue;
		break;			
	case IDS_PACKET_DELAY_RESULT:
		ArgGetNetFunc.NetFuncId = NET_FUNC_ID_CALCULATED_RESULT;
		status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
		nValue = ResGetNetFunc.Scalar.dwValue;
		break;			
	default:
		break;
	}
	
	if ( ! HV_SUCCESS(status))
		throw HVBaseException(status, "HVCommand (Spec_NetWork_Get)");

	return nValue;

}

void CPropertyView::Spec_NetWork_Set(CDevice *pDevice, int readFunID, int nValue)
{
	HVAPI_CONTROL_PARAMETER  p;
	HV_ARG_SET_NET_FUNC  ArgSetNetFunc;
	HVSTATUS status;
	
	switch(readFunID) {
	case SCALAR_PACKET_DELAY:
		ArgSetNetFunc.NetFuncId				 = NET_FUNC_ID_INTER_PACKET_DELAY;
		ArgSetNetFunc.NetFunc.Scalar.dwValue = nValue;
		pDevice->m_PacketDelay				 = nValue;
		break;
	case SCALAR_EXPECT_DATA_RATE:
		ArgSetNetFunc.NetFuncId				 = NET_FUNC_ID_EXPECT_DATA_RATE;
		ArgSetNetFunc.NetFunc.Scalar.dwValue = nValue;
		pDevice->m_ExpectDataRate			 = nValue;
		break;
	case ITEM_CONNECT_SPEED:
		ArgSetNetFunc.NetFuncId					= NET_FUNC_ID_LINK_SPEED;
		ArgSetNetFunc.NetFunc.LinkSpead.dwValue = nValue;
		pDevice->m_nConnectSpeed				= nValue;
		break;
	default:
		break;
	}

	p.code = ORD_SET_NET_FUNC;
	p.pInBuf = &ArgSetNetFunc;
	p.dwInBufSize = sizeof(ArgSetNetFunc);
	p.pBytesRet = NULL;

	status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);

	if ( ! HV_SUCCESS(status))
		throw HVBaseException(status, "HVCommand (Spec_NetWork_Set)");
	
}


void CPropertyView::Set_Adv_Feature(CDevice *pDevice, int readFunID, int nValue)
{
	HVAPI_CONTROL_PARAMETER  p;
	HV_ARG_SET_ADV_FEATURE  ArgSetFeature;

	HVSTATUS status;
	
	switch(readFunID) {
	case ITEM_PASSIVE_TRANSMIT:
		ArgSetFeature.AdvFeatureItemId	= ADV_FEATURE_ITEM_ID_IMAGE_PASSIVE_TRANS_HOLD_IMAGE;
		ArgSetFeature.AdvFeatureId		= ADV_FEATURE_ID_IMAGE_PASSIVE_TRANSMIT;
		ArgSetFeature.AdvFeature.ImagePassiveTransmit.HoldImg = nValue;
		pDevice->m_PassiveTransOnOff	= nValue;
		if(nValue)
			pDevice->m_PassiveTransBufNum = Spec_Fun_ADV_Get(pDevice, ID_DO_PASSIVE_TRANS);
		break;
	case ID_DO_PASSIVE_TRANS:
		ArgSetFeature.AdvFeatureItemId	= ADV_FEATURE_ITEM_ID_IMAGE_PASSIVE_TRANS_SEND_IMAGE;
		ArgSetFeature.AdvFeatureId		= ADV_FEATURE_ID_IMAGE_PASSIVE_TRANSMIT;
		ArgSetFeature.AdvFeature.ImagePassiveTransmit.SendImg = nValue; //nValue =1
		break;
	case ITEM_DIGITAL_GAIN:
		ArgSetFeature.AdvFeatureItemId	= ADV_FEATURE_ITEM_ID_DIGITAL_GAIN_GAIN_NUM;
		ArgSetFeature.AdvFeatureId		= ADV_FEATURE_ID_DIGITAL_GAIN;
		ArgSetFeature.AdvFeature.DigitalGain.GainNum = Item2DigitalGain[nValue]; 
		pDevice->m_nDigitalGainItem = nValue;
		break;
	case ITEM_ANTI_FLICKER:
		ArgSetFeature.AdvFeatureItemId	= ADV_FEATURE_ITEM_ID_ANTI_FLICKER_ONOFF;
		ArgSetFeature.AdvFeatureId		= ADV_FEATURE_ID_ANTI_FLICKER;
		ArgSetFeature.AdvFeature.AntiFlicker.OnOff = nValue; 
		pDevice->m_AntiFilterState = nValue;
		break;
	case ITEM_ANTI_FLICKER_SOURCE:
		ArgSetFeature.AdvFeatureItemId	= ADV_FEATURE_ITEM_ID_ANTI_FLICKER_TRIGGERSOURCE;
		ArgSetFeature.AdvFeatureId		= ADV_FEATURE_ID_ANTI_FLICKER;
		ArgSetFeature.AdvFeature.AntiFlicker.TriggerSource = nValue; 
		pDevice->m_AntiFilterSource = nValue;
		break;
	case SCALAR_AC_PHASE_ADJUST:
		ArgSetFeature.AdvFeatureItemId	= ADV_FEATURE_ITEM_ID_SCALAREX_VALUE;
		ArgSetFeature.AdvFeatureId		= ADV_FEATURE_ID_AC_PHASE_ADJUST;
		ArgSetFeature.AdvFeature.ScalarEx.Value = nValue; 
		pDevice->m_ACPhaseAdjust = nValue;
		break;		
	case ITEM_TRIGGERDELAY_UNIT:
		ArgSetFeature.AdvFeatureId		= ADV_FEATURE_ID_TRIGGER_DELAY_UNIT;
		ArgSetFeature.AdvFeatureItemId	= ADV_FEATURE_ITEM_ID_SELECTOR_MODE;
		ArgSetFeature.AdvFeature.Selector.Mode = nValue; 
		pDevice->m_TriggerDelayUnit = nValue;
		break;		
	default:
		break;
	}
	p.code = ORD_SET_ADV_FEATURE;
	p.pInBuf = &ArgSetFeature;
	p.dwInBufSize = sizeof(ArgSetFeature);
	p.pBytesRet = NULL;
	
	status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
	
	if ( ! HV_SUCCESS(status))
		throw HVBaseException(status, "HVCommand (Set_Adv_Feature)");

}


void CPropertyView::RefreshDelayResult(int nValue)
{
	CString str;
	str.LoadString(IDS_PACKET_DELAY_RESULT);
	HPROPERTY hName = m_ListView.FindProperty(str);

	if (hName){
		
		str.Format("%d", nValue);
		CComVariant v(str);
		m_ListView.SetItemValue(hName, &v);
	}
				
}

CString CPropertyView::Spec_Fun_Get_SV(CDevice *pDevice, HV_STRING_ID stringID )
{
	HVAPI_CONTROL_PARAMETER  p;
	HV_ARG_QUERY_STRING_DESCRIPTOR ArgStrDesc;
	HV_RES_QUERY_STRING_DESCRIPTOR ResStrDesc;
	HV_ARG_GET_STRING ArgGetStr;
	HV_RES_GET_STRING ResGetStr;

	CString str;
	char* mSV;

	ArgStrDesc.StringId = stringID;
	p.code = ORD_QUERY_STRING_DESCRIPTOR;
	p.pOutBuf = &ResStrDesc;
	p.dwOutBufSize = sizeof(ResStrDesc);
	p.pInBuf = &ArgStrDesc;
	p.dwInBufSize = sizeof(ArgStrDesc);
	p.pBytesRet = NULL;

	HVSTATUS status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
	
//--------------------
	p.code = ORD_GET_STRING;
	
	char *pDriverVer = new char[ResStrDesc.dwTotalByteLength+1]; 
	ArgGetStr.StringId = stringID;
	ArgGetStr.nLength  = ResStrDesc.dwTotalByteLength;
	ResGetStr.pBuffer = pDriverVer;

	p.pInBuf = &ArgGetStr;
	p.dwInBufSize = sizeof(ArgGetStr);
	p.pOutBuf = &ResGetStr;
	p.dwOutBufSize = sizeof(ResGetStr);
	
	status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);

	mSV = ResGetStr.pBuffer;
	
	str.Format("%s",mSV);
	if ( ! HV_SUCCESS(status))
		throw HVBaseException(status, "HVDAILT.dll : HVCommand (Spec_Fun_Get_SV)");
	
	delete []pDriverVer;
	return str;

}

CString CPropertyView::Spec_Fun_Get_APIV(CDevice *pDevice)
{
	HVAPI_CONTROL_PARAMETER  p;
	HV_ARG_QUERY_STRING_DESCRIPTOR ArgStrDesc;
	HV_RES_QUERY_STRING_DESCRIPTOR ResStrDesc;
	HV_ARG_GET_STRING ArgGetStr;
	HV_RES_GET_STRING ResGetStr;

	CString str;
	char* mSV;

	ArgStrDesc.StringId = STRING_ID_HVAPI_VERSION;
	p.code = ORD_QUERY_STRING_DESCRIPTOR;
	p.pOutBuf = &ResStrDesc;
	p.dwOutBufSize = sizeof(ResStrDesc);
	p.pInBuf = &ArgStrDesc;
	p.dwInBufSize = sizeof(ArgStrDesc);
	p.pBytesRet = NULL;

	HVSTATUS status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
	
//--------------------
	p.code = ORD_GET_STRING;
	
	char *pDriverVer = new char[ResStrDesc.dwTotalByteLength+1]; 
	ArgGetStr.StringId = STRING_ID_HVAPI_VERSION;
	ArgGetStr.nLength  = ResStrDesc.dwTotalByteLength;
	ResGetStr.pBuffer = pDriverVer;

	p.pInBuf = &ArgGetStr;
	p.dwInBufSize = sizeof(ArgGetStr);
	p.pOutBuf = &ResGetStr;
	p.dwOutBufSize = sizeof(ResGetStr);
	
	status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);

	mSV = ResGetStr.pBuffer;

	str.Format("%s",mSV);
	if ( ! HV_SUCCESS(status))
		throw HVBaseException(status, "HVDAILT.dll : HVCommand (Spec_Fun_Get_APIV)");

	delete []pDriverVer;

	return str;

}

CString CPropertyView::Spec_Fun_Get_DailtV(CDevice *pDevice)
{
	HVAPI_CONTROL_PARAMETER  p;
	HV_ARG_QUERY_STRING_DESCRIPTOR ArgStrDesc;
	HV_RES_QUERY_STRING_DESCRIPTOR ResStrDesc;
	HV_ARG_GET_STRING ArgGetStr;
	HV_RES_GET_STRING ResGetStr;

	CString str;
	char* mSV;

	ArgStrDesc.StringId = STRING_ID_HVDAILT_VERSION;
	p.code = ORD_QUERY_STRING_DESCRIPTOR;
	p.pOutBuf = &ResStrDesc;
	p.dwOutBufSize = sizeof(ResStrDesc);
	p.pInBuf = &ArgStrDesc;
	p.dwInBufSize = sizeof(ArgStrDesc);
	p.pBytesRet = NULL;

	HVSTATUS status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
	
	//--------------------
	p.code = ORD_GET_STRING;
	
	char *pDriverVer = new char[ResStrDesc.dwTotalByteLength+1]; 
	ArgGetStr.StringId = STRING_ID_HVDAILT_VERSION;
	ArgGetStr.nLength  = ResStrDesc.dwTotalByteLength;
	ResGetStr.pBuffer = pDriverVer;

	p.pInBuf = &ArgGetStr;
	p.dwInBufSize = sizeof(ArgGetStr);
	p.pOutBuf = &ResGetStr;
	p.dwOutBufSize = sizeof(ResGetStr);
	
	status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);

	mSV = ResGetStr.pBuffer;
	
	str.Format("%s",mSV);
	if ( ! HV_SUCCESS(status))
		throw HVBaseException(status, "HVDAILT.dll : HVCommand (Spec_Fun_Get_DailtV)");

	delete []pDriverVer;
	return str;
}

void CPropertyView::Set_AutoFunc(CDevice *pDevice, int FunID, int nValue)
{
	HVSTATUS status;
	
	HVAPI_CONTROL_PARAMETER  p;	
	HV_ARG_SET_AUTO_FUNC setFeature;
	setFeature.AutoFuncId = AUTO_FUNC_ID_AUTO_STROBE_CTRL;
	setFeature.AutoFuncItemId = AUTO_FUNC_ITEM_ID_AUTO_STROBE_SCALAR_AUTO_MANUAL_MODE;
	setFeature.AutoFunc.AutoStrobeScalar.AutoManualMode = nValue;

	p.code		 = ORD_SET_AUTO_FUNC;	
	p.pInBuf	 = &setFeature;
	p.dwInBufSize= sizeof(setFeature);
	
	pDevice->m_bStrobeAuto = nValue;
	status = HVCommand(m_hhv, CMD_HVAPI_CONTROL, &p, 0);

	if ( ! HV_SUCCESS(status))
		throw HVBaseException(status, "HVDAILT.dll : HVCommand (Set_AutoFunc)");
}

void CPropertyView::Set_Shutter_Auto(CDevice *pDevice, int index)
{
	HVSTATUS status;
	
	HVAPI_CONTROL_PARAMETER  p;	
	HV_ARG_SET_FEATURE setFeature;
	setFeature.FeatureId = FEATURE_ID_SHUTTER;
	setFeature.FeatureItemId = FEATURE_ITEM_ID_SCALAR_AUTO_MANUAL_MODE;
	p.code = ORD_SET_FEATURE;
	
	switch(index)
	{
	case 0:
        setFeature.Feature.Scalar.AutoManualMode = 0;
		break;
	case 1:
		setFeature.Feature.Scalar.AutoManualMode = 1;
		break;
	default:
		break;
	}
	p.pInBuf = &setFeature;
	p.dwInBufSize = sizeof(setFeature);
	
	pDevice->m_bShutterAuto = index;
	status = HVCommand(m_hhv, CMD_HVAPI_CONTROL, &p, 0);

	if ( ! HV_SUCCESS(status))
		throw HVBaseException(status, "HVDAILT.dll : HVCommand (Set_Shutter_Auto)");
	
}

void CPropertyView::Set_Gain_Auto(CDevice *pDevice, int index)
{
	HVSTATUS status;
	
	HVAPI_CONTROL_PARAMETER  p;	
	HV_ARG_SET_FEATURE setFeature;
	setFeature.FeatureId = FEATURE_ID_GAIN;
	setFeature.FeatureItemId = FEATURE_ITEM_ID_SCALAR_AUTO_MANUAL_MODE;
	switch(index)
	{
	case 0:
		setFeature.Feature.Scalar.AutoManualMode = 0;
		break;
	case 1:
		setFeature.Feature.Scalar.AutoManualMode = 1;
		break;
	default:
		break;
	}
	p.pInBuf = &setFeature;
	p.dwInBufSize = sizeof(setFeature);
	p.code = ORD_SET_FEATURE;
	
		pDevice->m_bGainAuto = index;	
		status = HVCommand(m_hhv, CMD_HVAPI_CONTROL, &p, 0);

	if ( ! HV_SUCCESS(status))
		throw HVBaseException(status, "HVDAILT.dll : HVCommand (Set_Gain_Auto)");
		
}

void CPropertyView::SendPassiveTrans(CDevice *pDevice, int commandID)
{
	switch(commandID) {
	case ID_DO_PASSIVE_TRANS:
		Set_Adv_Feature(pDevice, ID_DO_PASSIVE_TRANS, 1);
		break;
	default:
		break;
	}
}


void CPropertyView::Spec_Fun_SaveParam(CDevice *pDevice)
{
	HVAPI_CONTROL_PARAMETER  p;
	HV_ARG_SET_BASIC_FUNC ArgSetBasicFunc;
	ArgSetBasicFunc.FuncId = BASIC_FUNC_ID_MEM_CH_SAVE;
	ArgSetBasicFunc.ItemId = BASIC_FUNC_ITEM_ID_MEM_CH_SAVE_NUM;
	ArgSetBasicFunc.Func.MemChSave.Number = pDevice->m_nSaveParam + 1;//m_ComBoxSaveCh.GetCurSel()+1;
    
	p.code = ORD_SET_BASIC_FUNC;
	p.pInBuf = &ArgSetBasicFunc;
	p.dwInBufSize = sizeof(ArgSetBasicFunc);
	p.pOutBuf = NULL;
	p.dwOutBufSize = 0;
	p.pBytesRet = NULL;
	HVSTATUS status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
}


void CPropertyView::Spec_Fun_LoadParam(CDevice *pDevice)
{
	HVAPI_CONTROL_PARAMETER  p;
	HV_ARG_SET_BASIC_FUNC ArgSetBasicFunc;
	ArgSetBasicFunc.FuncId = BASIC_FUNC_ID_MEM_CH_CUR;
	ArgSetBasicFunc.ItemId = BASIC_FUNC_ITEM_ID_MEM_CH_CUR_NUM;
	ArgSetBasicFunc.Func.MemChSave.Number = pDevice->m_nLoadParam;//m_ComBoxCurCh.GetCurSel();
    
	p.code = ORD_SET_BASIC_FUNC;
	p.pInBuf = &ArgSetBasicFunc;
	p.dwInBufSize = sizeof(ArgSetBasicFunc);
	p.pOutBuf = NULL;
	p.dwOutBufSize = 0;
	p.pBytesRet = NULL;
	HVSTATUS status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
	if ( ! HV_SUCCESS(status))
		throw HVBaseException(status, "HVDAILT.dll : HVCommand (Spec_Fun_LoadParam)");
	
}


void CPropertyView::Spec_Fun_StartParam(CDevice *pDevice)
{
	HVAPI_CONTROL_PARAMETER  p;
	HV_ARG_SET_ADV_FEATURE ArgSetAdvFeature;
	ArgSetAdvFeature.AdvFeatureId = ADV_FEATURE_ID_STARTUP_MEM_CH;
    ArgSetAdvFeature.AdvFeatureItemId = ADV_FEATURE_ITEM_ID_STARTUP_MEM_CH_NUMBER;
    ArgSetAdvFeature.AdvFeature.StartupMemCh.Number = pDevice->m_nBeginParam;//m_ComBoxStartupCh.GetCurSel();
    
	p.code = ORD_SET_ADV_FEATURE;
	p.pInBuf = &ArgSetAdvFeature;
	p.dwInBufSize = sizeof(ArgSetAdvFeature);
	p.pOutBuf = NULL;
	p.dwOutBufSize = 0;
	p.pBytesRet = NULL;
	HVSTATUS status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
}


int CPropertyView::Spec_Fun_Get_Basic(CDevice *pDevice, int Id)
{
	int nValue;
	
	HVAPI_CONTROL_PARAMETER  p;
	HVSTATUS status;
	//获取参数保存、参数加载状态
	switch(Id) {
	case ITEM_SAVE_PARAM:	//保存参数
		{
			HV_ARG_GET_BASIC_FUNC ArgGetBasicFunc;
			HV_RES_GET_BASIC_FUNC ResGetBasicFunc;
			ArgGetBasicFunc.FuncId = BASIC_FUNC_ID_MEM_CH_SAVE;
			p.code = ORD_GET_BASIC_FUNC;
			p.pInBuf = &ArgGetBasicFunc;
			p.dwInBufSize = sizeof(ArgGetBasicFunc);
			p.pOutBuf = &ResGetBasicFunc;
			p.dwOutBufSize = sizeof(ResGetBasicFunc);
			p.pBytesRet = NULL;
			
				
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			nValue = ResGetBasicFunc.MemChSave.Number; 
		}
		break;

	case ITEM_LOAD_PARAM:	//加载参数
		{
			HV_ARG_GET_BASIC_FUNC ArgGetBasicFunc;
			HV_RES_GET_BASIC_FUNC ResGetBasicFunc;
			ArgGetBasicFunc.FuncId = BASIC_FUNC_ID_MEM_CH_CUR;
			p.code = ORD_GET_BASIC_FUNC;
			p.pInBuf = &ArgGetBasicFunc;
			p.dwInBufSize = sizeof(ArgGetBasicFunc);
			p.pOutBuf = &ResGetBasicFunc;
			p.dwOutBufSize = sizeof(ResGetBasicFunc);
			p.pBytesRet = NULL;
			
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			nValue = ResGetBasicFunc.MemChCur.Number;
		}
		break;

	//获取启动参数组状态
	case ITEM_BEGIN_SAVE:	//启动参数组
		{
			HV_ARG_GET_ADV_FEATURE   ArgGetAdvFeature;
			HV_RES_GET_ADV_FEATURE   ResGetAdvFeature;
			ArgGetAdvFeature.AdvFeatureId = ADV_FEATURE_ID_STARTUP_MEM_CH;
			
			p.code = ORD_GET_ADV_FEATURE;
			p.pInBuf = &ArgGetAdvFeature;
			p.dwInBufSize = sizeof(ArgGetAdvFeature);
			p.pOutBuf = &ResGetAdvFeature;
			p.dwOutBufSize = sizeof(ResGetAdvFeature);
			p.pBytesRet = NULL;
			
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			nValue = ResGetAdvFeature.StartupMemCh.Number;
		}
		break;
	default:
		break;
	}
	return nValue;	
}

//读取相机中的参数
int CPropertyView::Spec_Fun_Get(CDevice *pDevice, int Id)
{
	HVAPI_CONTROL_PARAMETER  p;
	HVSTATUS status;
	CString str;
	int	nValue;
	
	HV_ARG_GET_FEATURE GetFeature;
	HV_RES_GET_FEATURE ResFeature;
	p.pInBuf = &GetFeature;
	p.dwInBufSize = sizeof(GetFeature);	
	p.pOutBuf = &ResFeature;
	p.dwOutBufSize = sizeof(ResFeature);
	p.pBytesRet = NULL;		
	p.code = ORD_GET_FEATURE;
				
	switch(Id) {
	case SCALAR_PACKET:
		{
			HV_ARG_GET_FMT7_MODE GetFMT7Feature;
			HV_RES_GET_FMT7_MODE ResFMT7Feature;
			p.pInBuf = &GetFMT7Feature;
			p.dwInBufSize = sizeof(GetFMT7Feature);	
			p.pOutBuf = &ResFMT7Feature;
			p.dwOutBufSize = sizeof(ResFMT7Feature);
			p.pBytesRet = NULL;		
			p.code = ORD_GET_FMT7_MODE;
			
			GetFMT7Feature.VideoMode = (HV_VIDEO_MODE)pDevice->GetVideoMode();
			GetFMT7Feature.FuncId	 = FMT7_MODE_FUNC_ID_BAND_WIDTH;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			nValue = ResFMT7Feature.BandWidth.PacketSize;
		}
		break;
	case SCALAR_SHUTTERSPEED:	//读取快门速度
		{
			GetFeature.FeatureId = FEATURE_ID_SHUTTER;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			nValue = ResFeature.Scalar.Value;
			break;
		}
	case SCALAR_BRIGHTNESS:		//读取亮度
		{
			GetFeature.FeatureId = FEATURE_ID_BRIGHTNESS;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);	
			nValue = ResFeature.Scalar.Value;
		}	
		break;
		
		
	case SCALAR_GAINLEVEL:			//读取增益调节
		{
			GetFeature.FeatureId = FEATURE_ID_GAIN;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);	
			nValue = ResFeature.Scalar.Value;
		}	
		break;
	// 白平衡功能
	case ITEM_WB_ON_OFF:
		{
			GetFeature.FeatureId = FEATURE_ID_WHITE_BALANCE;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);	
			nValue = ResFeature.WhiteBalance.OnOff;
		}
		break;
	case ITEM_WB_AUTO_MANU:
		{
			GetFeature.FeatureId = FEATURE_ID_WHITE_BALANCE;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);	
			nValue = ResFeature.WhiteBalance.AutoManualMode;
		}
		break;
	case ITEM_WB_ONE_PUSH:
		{
			GetFeature.FeatureId = FEATURE_ID_WHITE_BALANCE;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);	
			nValue = ResFeature.WhiteBalance.OnePush;
		}
		break;
	case SCALAR_R_VALUE:
		{
			GetFeature.FeatureId = FEATURE_ID_WHITE_BALANCE;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);	
			nValue = ResFeature.WhiteBalance.VR;
		}
		break;
	case SCALAR_B_VALUE:
		{
			GetFeature.FeatureId = FEATURE_ID_WHITE_BALANCE;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);	
			nValue = ResFeature.WhiteBalance.UB;
		}
		break;
		

	////////
	case ITEM_GAIN_AUTO:			//读取增益状态
		{
			GetFeature.FeatureId = FEATURE_ID_GAIN;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);	
			nValue = ResFeature.Scalar.AutoManualMode;
		}	
		break;
	case ITEM_SHUTTER_AUTO:	//读取快门状态
		{
			GetFeature.FeatureId = FEATURE_ID_SHUTTER;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			nValue = ResFeature.Scalar.AutoManualMode;
			break;
		}
	case SCALAR_TRIGGERDELAY_VALUE:
		{
			GetFeature.FeatureId = FEATURE_ID_TRIGGER_DELAY;	//读取触发延迟时间
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			nValue = ResFeature.Scalar.Value; //.Trigger.Parameter;
			break;
		}
	case ITEM_TRIGGERDELAY_STATE:
		{
			GetFeature.FeatureId = FEATURE_ID_TRIGGER_DELAY;	//读取触发延迟开关
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			nValue = ResFeature.Scalar.OnOff;
			break;
		}
	case ITEM_INPUTIOSET:
		{
			GetFeature.FeatureId = FEATURE_ID_TRIGGER;	
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			nValue = ResFeature.Trigger.Source;			
		}
		break;
		
	default:
		break;
	}
	if ( ! HV_SUCCESS(status))
		throw HVBaseException(status, "HVDAILT.dll : HVCommand (Spec_Fun_Get)");
	
	return nValue;
}


int CPropertyView::Spec_Fun_Get_AutoFunc(CDevice *pDevice, int Id)
{
	HVAPI_CONTROL_PARAMETER  p;
	HVSTATUS status;
	int	nValue = 0;

	HV_ARG_GET_AUTO_FUNC argAutoFunc;
	HV_RES_GET_AUTO_FUNC resAutoFunc;
	
	p.pInBuf		= &argAutoFunc;
	p.dwInBufSize	= sizeof(argAutoFunc);
	p.pOutBuf		= &resAutoFunc;
	p.dwOutBufSize	= sizeof(resAutoFunc);
	p.pBytesRet		= NULL;
	p.code			= ORD_GET_AUTO_FUNC;

	switch(Id) {
	case ITEM_STROBE_AUTO:
		argAutoFunc.AutoFuncId = AUTO_FUNC_ID_AUTO_STROBE_CTRL;
		status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
		if(status == STATUS_OK)
		{
			nValue = resAutoFunc.AutoStrobeScalar.AutoManualMode;
		}
		break;
	default:
		break;
	}

	return nValue;
}

//读取相机中ADV的参数
int CPropertyView::Spec_Fun_ADV_Get(CDevice *pDevice, int readID)
{
	HVAPI_CONTROL_PARAMETER  p;
	HVSTATUS status;
	HPROPERTY hName;
	CString str;
	int	nValue = 0;

	HV_ARG_GET_ADV_FEATURE ArgGetAdvFeature;
	HV_RES_GET_ADV_FEATURE ResGetAdvFeature;

	p.pInBuf = &ArgGetAdvFeature;
	p.dwInBufSize = sizeof(ArgGetAdvFeature);
	p.pOutBuf = &ResGetAdvFeature;
	p.dwOutBufSize = sizeof(ResGetAdvFeature);
	p.pBytesRet = NULL;
	p.code = ORD_GET_ADV_FEATURE;
	
	switch(readID) {
	case ITEM_BLACKLEVELENABLE:
		{
			ArgGetAdvFeature.AdvFeatureId = ADV_FEATURE_ID_GLOBAL_BLACK_LEVEL;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if(status == STATUS_OK)
				nValue = ResGetAdvFeature.Scalar.OnOff;
		}
		break;
	case SCALAR_BLACKLEVEL:
		{
			ArgGetAdvFeature.AdvFeatureId = ADV_FEATURE_ID_GLOBAL_BLACK_LEVEL;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if(status == STATUS_OK)
				nValue = ResGetAdvFeature.Scalar.Value;
		}
		break;
	case SCALAR_AC_PHASE_ADJUST:
		{
			ArgGetAdvFeature.AdvFeatureId = ADV_FEATURE_ID_AC_PHASE_ADJUST;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if(status == STATUS_OK)
				nValue = ResGetAdvFeature.ScalarEx.Value;
		}
		break;
	case ITEM_ANTI_FLICKER:
		{
			ArgGetAdvFeature.AdvFeatureId = ADV_FEATURE_ID_ANTI_FLICKER;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if(status == STATUS_OK)
				nValue = ResGetAdvFeature.AntiFlicker.OnOff;
		}
		break;
	case ITEM_ANTI_FLICKER_SOURCE:
		{
			ArgGetAdvFeature.AdvFeatureId = ADV_FEATURE_ID_ANTI_FLICKER;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if(status == STATUS_OK)
				nValue = ResGetAdvFeature.AntiFlicker.TriggerSource;
		}
		break;
	case ITEM_FILTER_TIME_UNIT:
		{
			ArgGetAdvFeature.AdvFeatureId = ADV_FEATURE_ID_FILTER_TIME_UNIT;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if(status == STATUS_OK)
				nValue = ResGetAdvFeature.Selector.Mode;
		}
		break;
	case SCALAR_FILTER_TIME:
		{
			ArgGetAdvFeature.AdvFeatureId = ADV_FEATURE_ID_FILTER_TIME;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if(status == STATUS_OK)
				nValue = ResGetAdvFeature.FilterTime.Value;
		}
		break;
		

	/// 延迟传输
	case SCALAR_TRANSDELAY_VALUE:
		{
			ArgGetAdvFeature.AdvFeatureId = ADV_FEATURE_ID_DELAY_TRANSMIT;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if(status == STATUS_OK)
				nValue = ResGetAdvFeature.DelayTransmit.DelayTime;
		}
		break;
	case SCALAR_TRIGGER_TIME:
		{
			ArgGetAdvFeature.AdvFeatureId = ADV_FEATURE_ID_MULTI_TRIGGER_TIME;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if(status == STATUS_OK)
				nValue = ResGetAdvFeature.Scalar.Value;
		}
		break;
	case SCALAR_TRIGGER_NUM:
		{
			ArgGetAdvFeature.AdvFeatureId = ADV_FEATURE_ID_MULTI_TRIGGER_NUM;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if(status == STATUS_OK)
				nValue = ResGetAdvFeature.Scalar.Value;
		}
		break;
	case Get_OUTPUT_IO_0: //读取输出IO功能设置0
		{
			HV_ARG_GET_PIO_OUTPUT_PORT ArgGetIOFeature;
			HV_RES_GET_PIO_OUTPUT_PORT ResGetIOFeature;
			ArgGetIOFeature.PortId = PIO_OUTPUT_PORT0;
			ArgGetIOFeature.PortFuncId = PIO_OUTPUT_PORT_FUNC_ID_SOURCE_SELECT;

			p.pInBuf = &ArgGetIOFeature;
			p.dwInBufSize = sizeof(ArgGetIOFeature);
			p.pOutBuf = &ResGetIOFeature;
			p.dwOutBufSize = sizeof(ResGetIOFeature);
			p.code = ORD_GET_PIO_OUTPUT_PORT;

			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if(status == STATUS_OK)
				nValue = ResGetIOFeature.SourceSelect.Value;
		}
		break;
	case Get_OUTPUT_IO_1: //读取输出IO功能设置1
		{
			HV_ARG_GET_PIO_OUTPUT_PORT ArgGetIOFeature;
			HV_RES_GET_PIO_OUTPUT_PORT ResGetIOFeature;
			p.pInBuf = &ArgGetIOFeature;
			p.dwInBufSize = sizeof(ArgGetIOFeature);
			p.pOutBuf = &ResGetIOFeature;
			p.dwOutBufSize = sizeof(ResGetIOFeature);

			ArgGetIOFeature.PortId = PIO_OUTPUT_PORT1;
			ArgGetIOFeature.PortFuncId = PIO_OUTPUT_PORT_FUNC_ID_SOURCE_SELECT;
			p.code = ORD_GET_PIO_OUTPUT_PORT;
			
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if(status == STATUS_OK)
				nValue = ResGetIOFeature.SourceSelect.Value;
		}
		break;
	case Get_OUTPUT_IO_2: //读取输出IO功能设置2
		{
			HV_ARG_GET_PIO_OUTPUT_PORT ArgGetIOFeature;
			HV_RES_GET_PIO_OUTPUT_PORT ResGetIOFeature;
			p.pInBuf = &ArgGetIOFeature;
			p.dwInBufSize = sizeof(ArgGetIOFeature);
			p.pOutBuf = &ResGetIOFeature;
			p.dwOutBufSize = sizeof(ResGetIOFeature);
			p.code = ORD_GET_PIO_OUTPUT_PORT;
			
			ArgGetIOFeature.PortId = PIO_OUTPUT_PORT2;
			ArgGetIOFeature.PortFuncId = PIO_OUTPUT_PORT_FUNC_ID_SOURCE_SELECT;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if(status == STATUS_OK)
				nValue = ResGetIOFeature.SourceSelect.Value;
		}
		break;
	case ITEM_OUTPUTIO_0_CONTROL: //读取输出IO设置 high 、low
		{
			HV_ARG_GET_PIO_OUTPUT_PORT ArgGetIOFeature;
			HV_RES_GET_PIO_OUTPUT_PORT ResGetIOFeature;
			p.pInBuf = &ArgGetIOFeature;
			p.dwInBufSize = sizeof(ArgGetIOFeature);
			p.pOutBuf = &ResGetIOFeature;
			p.dwOutBufSize = sizeof(ResGetIOFeature);
			p.code = ORD_GET_PIO_OUTPUT_PORT;
			
			ArgGetIOFeature.PortId = PIO_OUTPUT_PORT0;
			ArgGetIOFeature.PortFuncId = PIO_OUTPUT_PORT_FUNC_ID_USER_SET;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if(status == STATUS_OK)
				nValue = ResGetIOFeature.UserSet.Value;
		}
		break;
	case ITEM_OUTPUTIO_1_CONTROL: //读取输出IO设置 high 、low
		{
			HV_ARG_GET_PIO_OUTPUT_PORT ArgGetIOFeature;
			HV_RES_GET_PIO_OUTPUT_PORT ResGetIOFeature;
			p.pInBuf = &ArgGetIOFeature;
			p.dwInBufSize = sizeof(ArgGetIOFeature);
			p.pOutBuf = &ResGetIOFeature;
			p.dwOutBufSize = sizeof(ResGetIOFeature);
			p.code = ORD_GET_PIO_OUTPUT_PORT;
			
			ArgGetIOFeature.PortId = PIO_OUTPUT_PORT1;
			ArgGetIOFeature.PortFuncId = PIO_OUTPUT_PORT_FUNC_ID_USER_SET;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if(status == STATUS_OK)
				nValue = ResGetIOFeature.UserSet.Value;
		}
		break;
	case ITEM_OUTPUTIO_2_CONTROL: //读取输出IO设置 high 、low
		{
			HV_ARG_GET_PIO_OUTPUT_PORT ArgGetIOFeature;
			HV_RES_GET_PIO_OUTPUT_PORT ResGetIOFeature;
			p.pInBuf = &ArgGetIOFeature;
			p.dwInBufSize = sizeof(ArgGetIOFeature);
			p.pOutBuf = &ResGetIOFeature;
			p.dwOutBufSize = sizeof(ResGetIOFeature);
			p.code = ORD_GET_PIO_OUTPUT_PORT;
			
			ArgGetIOFeature.PortId = PIO_OUTPUT_PORT2;
			ArgGetIOFeature.PortFuncId = PIO_OUTPUT_PORT_FUNC_ID_USER_SET;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if(status == STATUS_OK)
				nValue = ResGetIOFeature.UserSet.Value;
		}
		break;

	case ITEM_TEST_IMAGE:	//读取测试图状态	
		{
			ArgGetAdvFeature.AdvFeatureId = ADV_FEATURE_ID_TEST_IMAGE;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if(status == STATUS_OK)
				nValue = ResGetAdvFeature.Selector.Mode;
		}
		break;
	case ITEM_SHUTTERUNIT:	//读取速度单位	
		{
			ArgGetAdvFeature.AdvFeatureId = ADV_FEATURE_ID_SHUTTER_UNIT;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			nValue = ResGetAdvFeature.Selector.Mode;

			str.LoadString(IDS_TIME_UNIT);
			hName = m_ListView.FindProperty(str);
			if(status == STATUS_OK)
				pDevice->m_ShutterUnit= ResGetAdvFeature.Selector.Mode;
			
		}
		break;
	case SCALAR_RED_GAIN:		//读取R增益 
		{
			if (pDevice->resAdvPresence.PixelsSeparateGianRB)
			{
				ArgGetAdvFeature.AdvFeatureId = ADV_FEATURE_ID_PIXEL_SEPARATE_GAIN_RB;
				status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);	
				if(status == STATUS_OK)
					nValue = ResGetAdvFeature.PixelSeparateEx.R_G1_Value;
			}
			else
			{
				ArgGetAdvFeature.AdvFeatureId = ADV_FEATURE_ID_PIXEL_SEPARATE_GAIN;
				status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);	
				if(status == STATUS_OK)
					nValue = ResGetAdvFeature.PixelSeparate.RValue;
			}
		}
		break;
	case SCALAR_BLUE_GAIN:		//读取B增益 
		{
			if (pDevice->resAdvPresence.PixelsSeparateGianRB)
			{
				ArgGetAdvFeature.AdvFeatureId = ADV_FEATURE_ID_PIXEL_SEPARATE_GAIN_RB;
				status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);	
				if(status == STATUS_OK)
					nValue = ResGetAdvFeature.PixelSeparateEx.B_G2_Value;
			}
			else
			{
				ArgGetAdvFeature.AdvFeatureId = ADV_FEATURE_ID_PIXEL_SEPARATE_GAIN;
				status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);	
				if(status == STATUS_OK)
					nValue = ResGetAdvFeature.PixelSeparate.BValue;
			}
		}
		break;
	case SCALAR_GREEN_GAIN:		//读取G增益 
		{
			if (pDevice->resAdvPresence.PixelsSeparateGainG1G2)
			{
				ArgGetAdvFeature.AdvFeatureId = ADV_FEATURE_ID_PIXEL_SEPARATE_GAIN_G1G2;
				status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);	
				if(status == STATUS_OK)
					nValue = ResGetAdvFeature.PixelSeparateEx.R_G1_Value;
			}
			else
			{
				ArgGetAdvFeature.AdvFeatureId = ADV_FEATURE_ID_PIXEL_SEPARATE_GAIN;
				status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);	
				if(status == STATUS_OK)
					nValue = ResGetAdvFeature.PixelSeparate.GrValue;
			}
		}
		break;
	case ITEM_PASSIVE_TRANSMIT:		//被动传输 是否 使能状态
		{
			ArgGetAdvFeature.AdvFeatureId = ADV_FEATURE_ID_IMAGE_PASSIVE_TRANSMIT;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);	
			if(status == STATUS_OK)
				nValue = ResGetAdvFeature.ImagePassiveTransmit.HoldImg;
		}
		break;
	case ID_DO_PASSIVE_TRANS:	//被动传输，是否有图
		{
			ArgGetAdvFeature.AdvFeatureId = ADV_FEATURE_ID_IMAGE_PASSIVE_TRANSMIT;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);	
			if(status == STATUS_OK)
				nValue = ResGetAdvFeature.ImagePassiveTransmit.BufferNum;
		}
		break;
	case ITEM_DIGITAL_GAIN:	//数字增益
		{
			ArgGetAdvFeature.AdvFeatureId = ADV_FEATURE_ID_DIGITAL_GAIN;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);	
			if(status == STATUS_OK)
				nValue = ResGetAdvFeature.DigitalGain.GainNum;
		}
		break;
	case ITEM_TRIGGERDELAY_UNIT:	//读取TriggerDelay单位	
		{
			ArgGetAdvFeature.AdvFeatureId = ADV_FEATURE_ID_TRIGGER_DELAY_UNIT;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if(status == STATUS_OK)
				nValue = ResGetAdvFeature.Selector.Mode;
			pDevice->m_TriggerDelayUnit = ResGetAdvFeature.Selector.Mode;
		}
		break;
		
	default:
		break;
	}

// Deleted by xupx 2009.04.20
// 在Timer中会不断查询被动传输，当status连续抛错误时，Demo会崩溃。
	if ( ! HV_SUCCESS(status))
		{
			if(readID != ID_DO_PASSIVE_TRANS)
				throw HVBaseException(status, "HVDAILT.dll : HVCommand (Spec_Fun_ADV_Get)");
		}

	return nValue;
	
}


void CPropertyView::Spec_Fun_Set_AdvTrigger(CDevice *pDevice, ReadFunID Id, int value)
{
	HVAPI_CONTROL_PARAMETER  p;
	HVSTATUS status;
	CString str;
	
	HV_ARG_SET_ADV_FEATURE ArgSetAdvFeature;

	p.code = ORD_SET_ADV_FEATURE;
	p.pInBuf = &ArgSetAdvFeature;
	p.dwInBufSize = sizeof(ArgSetAdvFeature);
	p.pOutBuf = NULL;
	p.dwOutBufSize = 0;
	p.pBytesRet = NULL;

	switch(Id)
	{
	case SCALAR_TRIGGER_NUM:
		{
			ArgSetAdvFeature.AdvFeatureId = ADV_FEATURE_ID_MULTI_TRIGGER_NUM;
			ArgSetAdvFeature.AdvFeature.Scalar.Value = value;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);	
		}
		break;
	case SCALAR_TRIGGER_TIME:
		{
			ArgSetAdvFeature.AdvFeatureId = ADV_FEATURE_ID_MULTI_TRIGGER_TIME;
			ArgSetAdvFeature.AdvFeature.Scalar.Value = value;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);	
		}
		break;
	default:
		break;
	}

	if ( ! HV_SUCCESS(status))
		throw HVBaseException(status, "HVDAILT.dll : HVCommand (Spec_Fun_Set_AdvTrigger)");
}


void CPropertyView::Spec_Fun_SetColorCode(CDevice* pDevice, HV_COLOR_CODE value, HV_VIDEO_MODE mode)
{
	HVAPI_CONTROL_PARAMETER  p;
	HVSTATUS status;

	HV_ARG_SET_FMT7_MODE		ArgSetFMT7Mode;
	ArgSetFMT7Mode.VideoMode	 = mode;
	ArgSetFMT7Mode.FuncId		 = FMT7_MODE_FUNC_ID_COLOR_CODING;
	ArgSetFMT7Mode.ItemId		 = FMT7_MODE_ITEM_ID_COLOR_MODE_COLOR_CODE;
	ArgSetFMT7Mode.Fmt7Mode.ColorMode.Code = (HV_COLOR_CODE)value;

	p.code			 = ORD_SET_FMT7_MODE;
	p.pInBuf		 = &ArgSetFMT7Mode;
	p.dwInBufSize	 = sizeof(ArgSetFMT7Mode);
	p.pOutBuf		 = NULL;
	p.dwOutBufSize	 = 0;
	p.pBytesRet		 = NULL;

	status = HVCommand(m_hhv, CMD_HVAPI_CONTROL, &p, 0);
	ATLTRACE("Spec_Fun_SetColorCode status is %d", status);

	if ( ! HV_SUCCESS(status))
		throw HVBaseException(status, "HVDAILT.dll : HVCommand (Spec_Fun_SetColorCode)");
	
}


int CPropertyView::Spec_Fun_Get_Descript(CDevice *pDevice, int readFunID, int n)
{
	HVAPI_CONTROL_PARAMETER  p;
	HVSTATUS status;
	CString str;
	int	nValue;
	
	HV_RES_QUERY_FEATURE_DESCRIPTOR ResFeature;
	HV_ARG_QUERY_FEATURE_DESCRIPTOR ArgFeature;
	
	p.pInBuf = &ArgFeature;
	p.dwInBufSize = sizeof(ArgFeature);
	
	p.pOutBuf = &ResFeature;
	p.dwOutBufSize = sizeof(ResFeature);
	p.pBytesRet = NULL;		
	p.code = ORD_QUERY_FEATURE_DESCRIPTOR;
	
	switch(readFunID) {
	case ITEM_INPUTIOSET:
		{
			ArgFeature.FeatureId = FEATURE_ID_TRIGGER;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			pDevice->m_TriggerSourceInq.Source0Inq = ResFeature.Trigger.Source0Inq;
			pDevice->m_TriggerSourceInq.Source1Inq = ResFeature.Trigger.Source1Inq;
			pDevice->m_TriggerSourceInq.Source2Inq = ResFeature.Trigger.Source2Inq;
			pDevice->m_TriggerSourceInq.Source3Inq = ResFeature.Trigger.Source3Inq;
			break;
		}
	case ITEM_TRIGGERDELAY_STATE:
		{
			ArgFeature.FeatureId = FEATURE_ID_TRIGGER_DELAY;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			nValue = ResFeature.Scalar.OnOffInq;
			break;
		}
	case ITEM_SHUTTER_AUTO:
		{
			ArgFeature.FeatureId = FEATURE_ID_SHUTTER;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			nValue = ResFeature.Scalar.AutoInq;
			break;
		}
	case ITEM_GAIN_AUTO:
		{
			ArgFeature.FeatureId = FEATURE_ID_GAIN;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			nValue = ResFeature.Scalar.AutoInq;
			break;
		}
	case ITEM_SNAPMODE:	// Trigger Mode
		{
			ArgFeature.FeatureId = FEATURE_ID_TRIGGER;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			pDevice->m_resFeatureDes = ResFeature;
			break;
		}
	// 白平衡
	case ITEM_WB_ON_OFF:
		{
			ArgFeature.FeatureId = FEATURE_ID_WHITE_BALANCE;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			nValue = ResFeature.Scalar.OnOffInq;			
			break;
		}
	case ITEM_WB_AUTO_MANU:
		{
			ArgFeature.FeatureId = FEATURE_ID_WHITE_BALANCE;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			nValue = ResFeature.Scalar.AutoInq;			
			break;
		}
	case ITEM_WB_ONE_PUSH:
		{
			ArgFeature.FeatureId = FEATURE_ID_WHITE_BALANCE;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			nValue = ResFeature.Scalar.OnePushInq;			
			break;
		}
	case SCALAR_R_VALUE:
		{
			ArgFeature.FeatureId = FEATURE_ID_WHITE_BALANCE;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if (n == 0) 
				nValue = ResFeature.Scalar.MinValue;
			else
				nValue = ResFeature.Scalar.MaxValue;			
			break;
		}

	case SCALAR_TRIGGERDELAY_VALUE:
		{
			ArgFeature.FeatureId = FEATURE_ID_TRIGGER_DELAY;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if (n == 0) 
				nValue = ResFeature.Scalar.MinValue;			
			else
				nValue = ResFeature.Scalar.MaxValue;
			break;
		}		
	case SCALAR_SHUTTERSPEED:	//读取快门速度范围
		{
			ArgFeature.FeatureId = FEATURE_ID_SHUTTER;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if(n == 0)
				nValue = ResFeature.Scalar.MinValue;			
			else
				nValue = ResFeature.Scalar.MaxValue;
			break;
		}
	case SCALAR_GAINLEVEL:
		{
			ArgFeature.FeatureId = FEATURE_ID_GAIN;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if(n == 0)
				nValue = ResFeature.Scalar.MinValue;	
			else
				nValue = ResFeature.Scalar.MaxValue;			
			break;
		}
	case SCALAR_RED_GAIN:
		{
			ArgFeature.FeatureId = FEATURE_ID_WHITE_BALANCE;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if(n == 0)
				nValue = ResFeature.Scalar.MinValue;			
			else
				nValue = ResFeature.Scalar.MaxValue;			
			break;
		}
	case SCALAR_BRIGHTNESS:
		{
			ArgFeature.FeatureId = FEATURE_ID_BRIGHTNESS;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if(n == 0)
				nValue = ResFeature.Scalar.MinValue;			
			else
				nValue = ResFeature.Scalar.MaxValue;			
			break;
		}
	}
	ATLTRACE(" Spec_Fun_Get_Descript readID(%d), status is %d !!!!",readFunID,status);
	return nValue;
}

int CPropertyView::Spec_NetWork_Get_Range(CDevice *pDevice, int readFunID, int n)
{
	HVAPI_CONTROL_PARAMETER  p;
	HVSTATUS status;
	CString str;
	int	nValue;
	
	HV_ARG_QUERY_NET_FUNC_DESCRIPTOR argDescriptor;
	HV_RES_QUERY_NET_FUNC_DESCRIPTOR resDescriptor;
	
	p.pInBuf		= &argDescriptor;
	p.dwInBufSize	= sizeof(argDescriptor);
	
	p.pOutBuf		= &resDescriptor;
	p.dwOutBufSize	= sizeof(resDescriptor);
	p.pBytesRet		= NULL;
	p.code			= ORD_QUERY_NET_FUNC_DESCRIPTOR;
	
	switch(readFunID) {
	case SCALAR_EXPECT_DATA_RATE:
		{
			argDescriptor.NetFuncId = NET_FUNC_ID_EXPECT_DATA_RATE;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if(n== 1)
			{
				nValue = resDescriptor.Scalar.dwMaxValue;
			}
			else
			{
				nValue = resDescriptor.Scalar.dwMinValue;
			}
		}
		break;
	case SCALAR_PACKET_DELAY:
		{
			argDescriptor.NetFuncId = NET_FUNC_ID_INTER_PACKET_DELAY;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if(n== 1)
			{
				nValue = resDescriptor.Scalar.dwMaxValue;
			}
			else
			{
				nValue = resDescriptor.Scalar.dwMinValue;
			}
		}
		break;
	}
	ATLTRACE(" Spec_Fun_Get readID(%d), status is %d !!!!",readFunID,status);
	if ( ! HV_SUCCESS(status))
		throw HVBaseException(status, "HVCommand (Spec_NetWork_Get_Range)");
	
	return nValue;
}

int CPropertyView::Spec_Fun_AutoDescript(CDevice *pDevice, int readFunID, int n)
{

	HVAPI_CONTROL_PARAMETER  p;
	HVSTATUS status;
	CString str;
	int	nValue;
	
	HV_ARG_QUERY_AUTO_FUNC_DESCRIPTOR argAutoFunc;
	HV_RES_QUERY_AUTO_FUNC_DESCRIPTOR resAutoFunc;

	p.pInBuf		= &argAutoFunc;
	p.dwInBufSize	= sizeof(argAutoFunc);
	p.pOutBuf		= &resAutoFunc;
	p.dwOutBufSize	= sizeof(resAutoFunc);

	p.pBytesRet		= NULL;
	p.code			= ORD_QUERY_AUTO_FUNC_DESCRIPTOR;
	
	switch(readFunID) {
	case ITEM_STROBE_AUTO:
		argAutoFunc.AutoFuncId = AUTO_FUNC_ID_AUTO_STROBE_CTRL;
		status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
		if ( ! HV_SUCCESS(status))
			throw HVBaseException(status, "HVCommand (Spec_Fun_AutoDescript)");
		nValue = resAutoFunc.Scalar.AutoInq;
		break;
	default:
		break;
	}

	return nValue;
}


int CPropertyView::Spec_Fun_Get_AdvDescript(CDevice *pDevice, int readFunID, int n)
{
	HVAPI_CONTROL_PARAMETER  p;
	HVSTATUS status;
	CString str;
	int	nValue;

	HV_ARG_QUERY_ADV_FEATURE_DESCRIPTOR ArgAdvFeature;
	HV_RES_QUERY_ADV_FEATURE_DESCRIPTOR ResAdvFeature;
	p.pInBuf = &ArgAdvFeature;
	p.dwInBufSize = sizeof(ArgAdvFeature);
	
	p.pOutBuf = &ResAdvFeature;
	p.dwOutBufSize = sizeof(ResAdvFeature);
	p.pBytesRet = NULL;		
	p.code = ORD_QUERY_ADV_FEATURE_DESCRIPTOR;//ORD_QUERY_FEATURE_DESCRIPTOR;

	switch(readFunID) {

	case IDS_OUTPUTIOSET:
		{
			HV_ARG_QUERY_PIO_OUTPUT_PORT_DESCRIPTOR argPort;

			argPort.PortId = (HV_PIO_OUTPUT_PORT_ID)n; 
			p.pInBuf = &argPort;
			p.dwInBufSize = sizeof(HV_ARG_QUERY_PIO_OUTPUT_PORT_DESCRIPTOR);

			switch(n) {
			case 0:
				p.pOutBuf = &pDevice->m_OutputIO_Desc.Channel0;
				break;
			case 1:
				p.pOutBuf = &pDevice->m_OutputIO_Desc.Channel1;
				break;
			case 2:
				p.pOutBuf = &pDevice->m_OutputIO_Desc.Channel2;
				break;
			}
			p.dwOutBufSize = sizeof(HV_RES_QUERY_PIO_OUTPUT_PORT_DESCRIPTOR);
			p.pBytesRet = NULL;	
			p.code = ORD_QUERY_PIO_OUTPUT_PORT_DESCRIPTOR;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			break;
		}
	case SCALAR_BLACKLEVEL:
		{
			ArgAdvFeature.AdvFeatureId = ADV_FEATURE_ID_GLOBAL_BLACK_LEVEL;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if(n == 0)
				nValue = ResAdvFeature.Scalar.MinValue;					
			else
				nValue = ResAdvFeature.Scalar.MaxValue;			
		}
		break;		
	case SCALAR_AC_PHASE_ADJUST:
		{
			ArgAdvFeature.AdvFeatureId = ADV_FEATURE_ID_AC_PHASE_ADJUST;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if(n == 0)
				nValue = ResAdvFeature.ScalarEx.MinValue;					
			else
				nValue = ResAdvFeature.ScalarEx.MaxValue;			
		}
		break;
	case ITEM_ANTI_FLICKER:
		{
			ArgAdvFeature.AdvFeatureId = ADV_FEATURE_ID_ANTI_FLICKER;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			nValue = ResAdvFeature.Selector.OnOffInq;			
		}
		break;
	case ITEM_FILTER_TIME_UNIT:
		{
			ArgAdvFeature.AdvFeatureId = ADV_FEATURE_ID_FILTER_TIME_UNIT;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			nValue = ResAdvFeature.Selector.OnOffInq;			
		}
		break;
	case SCALAR_FILTER_TIME:
		{
			ArgAdvFeature.AdvFeatureId = ADV_FEATURE_ID_FILTER_TIME;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			switch(n) {
			case 0:
				nValue = ResAdvFeature.FilterTime.MinValue;	
				break;
			case 1:
				nValue = ResAdvFeature.FilterTime.MaxValue;			
				break;
			default:
				nValue = ResAdvFeature.FilterTime.OnOffInq;	
				break;
			}
		}
		break;
		
	case ITEM_PASSIVE_TRANSMIT:		// 查询此功能是否可用。即，是否变灰
		{
			ArgAdvFeature.AdvFeatureId = ADV_FEATURE_ID_IMAGE_PASSIVE_TRANSMIT;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			nValue = ResAdvFeature.ImagePassiveTransmitInq.OnOffInq;			
		}
		break;

	case ITEM_DIGITAL_GAIN:
		{
			ArgAdvFeature.AdvFeatureId = ADV_FEATURE_ID_DIGITAL_GAIN;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			nValue = ResAdvFeature.Scalar.OnOffInq;			
		}
		break;
	///延迟传输
	case SCALAR_TRANSDELAY_VALUE:
		{
			ArgAdvFeature.AdvFeatureId = ADV_FEATURE_ID_DELAY_TRANSMIT;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if(n == 0)
				nValue = ResAdvFeature.Scalar.MinValue;			
			else
				nValue = ResAdvFeature.Scalar.MaxValue;			
		}
		break;
	case SCALAR_RB_GAIN_EX:
		{
			ArgAdvFeature.AdvFeatureId = ADV_FEATURE_ID_PIXEL_SEPARATE_GAIN_RB;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if( n == 0)
				nValue = ResAdvFeature.Scalar.MinValue;			
			else
			{
				nValue = ResAdvFeature.Scalar.MaxValue;			
			}
		}
		break;
	case SCALAR_GREEN_GAIN_EX:
		{
			ArgAdvFeature.AdvFeatureId = ADV_FEATURE_ID_PIXEL_SEPARATE_GAIN_G1G2;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if( n == 0)
				nValue = ResAdvFeature.Scalar.MinValue;			
			else
			{
				nValue = ResAdvFeature.Scalar.MaxValue;			
			}
		}
		break;
	case SCALAR_GREEN_GAIN:		//绿通道范围
		{
			ArgAdvFeature.AdvFeatureId = ADV_FEATURE_ID_PIXEL_SEPARATE_GAIN;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if( n == 0)
				nValue = ResAdvFeature.Scalar.MinValue;			
			else
			{
				nValue = ResAdvFeature.Scalar.MaxValue;			
			}
		}
		break;
	case SCALAR_TRIGGER_NUM:
		{
			ArgAdvFeature.AdvFeatureId = ADV_FEATURE_ID_MULTI_TRIGGER_NUM;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if(n == 0)
			{
				nValue = ResAdvFeature.Scalar.MinValue;			
			}
			else
			{
				nValue = ResAdvFeature.Scalar.MaxValue;			
			}
			break;
		}
	case SCALAR_TRIGGER_TIME:
		{
			ArgAdvFeature.AdvFeatureId = ADV_FEATURE_ID_MULTI_TRIGGER_TIME;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if(n == 0)
			{
				nValue = ResAdvFeature.Scalar.MinValue;			
			}
			else
			{
				nValue = ResAdvFeature.Scalar.MaxValue;					
			}
			break;
		}
	case ITEM_MULTI_TRIGGER_ONOFF:
		{
			ArgAdvFeature.AdvFeatureId = ADV_FEATURE_ID_MULTI_TRIGGER_NUM;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			nValue = ResAdvFeature.Scalar.OnOffInq;
		}
		break;
	case SCALAR_PACKET_DELAY:
		{
			ArgAdvFeature.AdvFeatureId = ADV_FEATURE_ID_INTER_PACKET_DELAY;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if(n == 0)
			{
				nValue = ResAdvFeature.Scalar.MinValue;	
			}
			else
			{
				nValue = ResAdvFeature.Scalar.MaxValue;
			}
			break;
		}		
	}
	ATLTRACE(" Spec_Fun_Get readID(%d), status is %d !!!!",readFunID,status);
				
	return nValue;
}

int CPropertyView::Spec_Fun_Get_FMT7_DESCRIPTOR(CDevice *pDevice, int Id, int n)
{
	HVAPI_CONTROL_PARAMETER  p;
	HVSTATUS status;
	CString str;
	int	nValue;
	
	HV_RES_QUERY_FMT7_MODE_DESCRIPTOR	ResFMT7Feature;
	HV_ARG_QUERY_FMT7_MODE_DESCRIPTOR	ArgFMT7Feature;
	p.pInBuf = &ArgFMT7Feature;
	p.dwInBufSize = sizeof(ArgFMT7Feature);			
	p.pOutBuf = &ResFMT7Feature;
	p.dwOutBufSize = sizeof(ResFMT7Feature);
	p.pBytesRet = NULL;		
	p.code = ORD_QUERY_FMT7_MODE_DESCRIPTOR;

	ArgFMT7Feature.VideoMode = (HV_VIDEO_MODE)pDevice->GetVideoMode();
	
	switch(Id) {
	case SCALAR_PACKET:
		{
			ArgFMT7Feature.FuncId	= FMT7_MODE_FUNC_ID_BAND_WIDTH;
//			ArgFMT7Feature.VideoMode = VIDEO_MODE0;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
			if(n == 0)
				nValue = ResFMT7Feature.BandWidth.UnitBytePerPacketInq;
			else
				nValue = ResFMT7Feature.BandWidth.MaxBytePerPacketInq;
		}
		break;
	case ColorModeInq:
		{
			ArgFMT7Feature.FuncId	= FMT7_MODE_FUNC_ID_COLOR_CODING;
//			ArgFMT7Feature.VideoMode = VIDEO_MODE0;
			status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
//			nValue = ResFMT7Feature.ColorMode.Raw8Inq;
			pDevice->m_resFMT7Descriptor = ResFMT7Feature;
		}
		break;
	default:
		break;
	}
	ATLTRACE(" Spec_Fun_Get_FMT7_DESCRIPTOR readID(%d), status is %d !!!!",Id,status);
	
	return nValue;
}


int CPropertyView::Spec_Fun_GetLUT(CDevice* pDevice)
{
	HVAPI_CONTROL_PARAMETER  p;
	HVSTATUS status;
	CString str;
//	int	nValue;
	
	HV_ARG_GET_ADV_LUT GetFeature;
	HV_RES_GET_ADV_LUT ResFeature;
	GetFeature.nLength = 8;
	p.pInBuf = &GetFeature;
	p.dwInBufSize = sizeof(GetFeature);
	p.pOutBuf = &ResFeature;
	p.dwOutBufSize = sizeof(ResFeature);
	p.pBytesRet = NULL;		
	p.code = ORD_GET_ADV_LUT;
	
	status = HVCommand(m_hhv,CMD_HVAPI_CONTROL,&p,0);
	ATLTRACE(" Spec_Fun_GetLUT  status is %d !!!!",status);
	return 0;
}

//查询 被动传输是否可用；
int CPropertyView::Spec_Fun_Presence(CDevice*pDevice, int FunID)
{
	if(m_IS_OldITS || m_IS_OldGC || m_IS_OldGM)
		return 0;
	
	HVAPI_CONTROL_PARAMETER  p;
	HVAPI_CONTROL_PARAMETER  pAuto;
	HVSTATUS status = STATUS_OK;
	int nVal = 0;


	if((!pDevice->m_bInit) && (!pDevice->m_bGetPresence))
	{
		// 查询标准功能
		p.code = ORD_QUERY_FEATURE_PRESENCE;
		DWORD dwVal= 0;
		p.pOutBuf = &pDevice->resPresence;
		p.dwOutBufSize = sizeof(pDevice->resPresence);
		p.pBytesRet = NULL;
		status = HVCommand(m_hhv, CMD_HVAPI_CONTROL, &p,	0);	
		if ( ! HV_SUCCESS(status))
			throw HVBaseException(status, "Spec_Fun_Presence");

		// 查询adv功能
		p.code = ORD_QUERY_ADV_FEATURE_PRESENCE;
		p.pOutBuf = &pDevice->resAdvPresence;
		p.dwOutBufSize = sizeof(pDevice->resAdvPresence);
		p.pBytesRet = NULL;
		status = HVCommand(m_hhv, CMD_HVAPI_CONTROL, &p,	0);
		if ( ! HV_SUCCESS(status))
			throw HVBaseException(status, "Spec_Fun_Presence");
		
		// 查询自动功能
		pAuto.code = ORD_QUERY_AUTO_FUNC_PRESENCE;
		pAuto.pOutBuf = &pDevice->resAutoPresence;
		pAuto.dwOutBufSize = sizeof(pDevice->resAutoPresence);
		pAuto.pBytesRet = NULL;	
		status = HVCommand(m_hhv, CMD_HVAPI_CONTROL, &pAuto,0);
		if ( ! HV_SUCCESS(status))
			throw HVBaseException(status, "Spec_Fun_Presence");

		// 查询AdvFunc功能
		p.code = ORD_QUERY_ADV_OPT_FUNC_PRESENCE;
		p.pOutBuf = &pDevice->resAdvFunPresence;
		p.dwOutBufSize = sizeof(pDevice->resAdvFunPresence);
		p.pBytesRet = NULL;	
		status = HVCommand(m_hhv, CMD_HVAPI_CONTROL, &p,0);
		if ( ! HV_SUCCESS(status))
			throw HVBaseException(status, "Spec_Fun_Presence");

		// 查询PIO功能
		p.code = ORD_QUERY_PIO_OUTPUT_PORT_PRESENCE;
		p.pOutBuf = &pDevice->resPIOPresence;
		p.dwOutBufSize = sizeof(pDevice->resPIOPresence);
		p.pBytesRet = NULL;
		status = HVCommand(m_hhv, CMD_HVAPI_CONTROL, &p,0);
		if ( ! HV_SUCCESS(status))
			throw HVBaseException(status, "Spec_Fun_Presence");
		
		pDevice->m_bGetPresence = TRUE;
	}
	
	switch(FunID) {
	case SCALAR_RED_GAIN:
		nVal = pDevice->resPresence.WhiteBalance;
		break;
	case SCALAR_BLUE_GAIN:
		nVal = pDevice->resPresence.WhiteBalance;
		break;
	case ITEM_INPUTIOSET:	// 输入IO
		nVal = pDevice->resPresence.Trigger;
		break;

	case ITEM_TRIGGERDELAY_STATE:
		nVal = pDevice->resPresence.TriggerDelay;
		break;
		
	case ITEM_TRIGGERDELAY_UNIT:
		nVal = pDevice->resAdvPresence.TriggerDelayUnit;
		break;
	case ITEM_PASSIVE_TRANSMIT:
		nVal = pDevice->resAdvPresence.ImagePassiveTransmit;
		break;
	case ITEM_DIGITAL_GAIN:
		nVal = pDevice->resAdvPresence.DigitalGain;
		break;
	case SCALAR_GREEN_GAIN:	//查分量增益 是否支持
		nVal = pDevice->resAdvPresence.PixelsSeparateGain;
		break;
	case ITEM_FILTER_TIME_UNIT:
		nVal = pDevice->resAdvPresence.FilterTimeUnit;
		break;
	case SCALAR_FILTER_TIME:
		nVal = pDevice->resAdvPresence.FilterTime;
		break;
	case SCALAR_TRIGGER_TIME:
		nVal = pDevice->resAdvPresence.MultiTriggerTime;
		break;
	case ITEM_ANTI_FLICKER:
		nVal = pDevice->resAdvPresence.AntiFlicker;
		break;
	case SCALAR_AC_PHASE_ADJUST:
		nVal = pDevice->resAdvPresence.ACPhaseAdjust;
		break;
	/// 自动功能查询
	case ITEM_GAIN_AUTO:
		{
			nVal = pDevice->resAutoPresence.AutoGainCtrl;
		}
		break;
	case ITEM_SHUTTER_AUTO:
		{
			nVal = pDevice->resAutoPresence.AutoShutterCtrl;
		}
		break;
	case ITEM_STROBE_AUTO:
		{
			nVal = pDevice->resAutoPresence.AutoStrobeCtl;
		}
		break;
	case ITEM_ADCLEVEL:	
		nVal = pDevice->resAdvFunPresence.Lut;
		break;
	default:
		break;
	}
	ATLTRACE(" Spec_Fun_Presence readID(%d), status is %d !!!!",FunID,status);
	
	return nVal;

}

void CPropertyView::OnLutNetCam(CDevice *pDevice, int index)
{
		CString str;
		str.LoadString(IDS_LUTPATH);
		HPROPERTY hName = m_ListView.FindProperty(str);
		char buf[1024];
		hName->GetDisplayValue(buf, -1);
		
		pDevice->m_LutPath.Format("%s",buf);
		
		
		DWORD Readed = 0;
		HANDLE hFile = ::CreateFile(buf, GENERIC_READ,
			FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		BOOL b;
		b = ReadFile(hFile, pDevice->m_dwLut, 16*1024, &Readed, NULL);
		CloseHandle(hFile);
		if(!b)
			return;		//没有读到lut文件，则不操作直接返回。
		/************ 加载 LUT ***************/
		HVSTATUS status;	
		HVAPI_CONTROL_PARAMETER  p;	
		HV_ARG_SET_ADV_LUT setFeature;
		setFeature.dwOffset = 0;
		setFeature.nLength = 16*1024;
		setFeature.pBuffer = pDevice->m_dwLut;
		
		p.pInBuf = &setFeature;
		p.dwInBufSize = sizeof(setFeature);
		p.code = ORD_SET_ADV_LUT;		
		
		if (Readed == 16*1024) {
			status = HVCommand(m_hhv, CMD_HVAPI_CONTROL, &p, 0);
		}	

	if ( ! HV_SUCCESS(status))
		throw HVBaseException(status, "HVDAILT.dll : HVCommand (OnLutNetCam)");
}


void CPropertyView::OnMultiTriggerNumChanged(CDevice *pDevice, int index)
{
	if(pDevice->m_flagMultiTrigger == 0)
		return;
	
	pDevice->m_TriggerNum = index;

	HV_ARG_SET_ADV_FEATURE ArgAdvFeature;
	ArgAdvFeature.AdvFeatureId = ADV_FEATURE_ID_MULTI_TRIGGER_NUM;
	ArgAdvFeature.AdvFeatureItemId = ADV_FEATURE_ITEM_ID_SCALAR_VALUE;
	ArgAdvFeature.AdvFeature.Scalar.Value = index;
	
	HVAPI_CONTROL_PARAMETER  param;
	param.code = ORD_SET_ADV_FEATURE;
	param.pInBuf = &ArgAdvFeature;
	param.dwInBufSize = sizeof(ArgAdvFeature);	
	param.pOutBuf = NULL;
	param.dwOutBufSize = 0;
	param.pBytesRet = NULL;
	
	HVSTATUS status = HVCommand(m_hhv, CMD_HVAPI_CONTROL, &param, 0);
	
	if ( ! HV_SUCCESS(status))
		throw HVBaseException(status, "HVDAILT.dll : HVCommand (OnTriggerNumChanged)");
	
}


void CPropertyView::OnMultiTriggerTimeChanged(CDevice *pDevice, int index)
{
	if(pDevice->m_flagMultiTrigger == 0)
		return;
	
	pDevice->m_TriggerTime = index;
	
	HV_ARG_SET_ADV_FEATURE ArgAdvFeature;
	ArgAdvFeature.AdvFeatureId = ADV_FEATURE_ID_MULTI_TRIGGER_TIME;
	ArgAdvFeature.AdvFeatureItemId = ADV_FEATURE_ITEM_ID_SCALAR_VALUE;
	ArgAdvFeature.AdvFeature.Scalar.Value = index;
	
	HVAPI_CONTROL_PARAMETER  param;
	param.code = ORD_SET_ADV_FEATURE;
	param.pInBuf = &ArgAdvFeature;
	param.dwInBufSize = sizeof(ArgAdvFeature);	
	param.pOutBuf = NULL;
	param.dwOutBufSize = 0;
	param.pBytesRet = NULL;
	
	HVSTATUS status = HVCommand(m_hhv, CMD_HVAPI_CONTROL, &param, 0);
	
	if ( ! HV_SUCCESS(status))
		throw HVBaseException(status, "HVDAILT.dll : HVCommand (OnTriggerNumChanged)");
	
}


void CPropertyView::OnAdvFeatureSetting(CDevice *pDevice, int FunID, int nValue)
{
	switch(FunID) {
	case ITEM_PASSIVE_TRANSMIT:
		if(pDevice->m_PassiveTransOnOff == nValue)
			return;
		else
			Set_Adv_Feature(pDevice, ITEM_PASSIVE_TRANSMIT, nValue);
		break;
	default:
		break;
	}		
}

