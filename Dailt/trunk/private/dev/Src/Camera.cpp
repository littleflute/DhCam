// Camera.cpp: implementation of the CCamera class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Camera.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCamera::CCamera()
{

}

CCamera::~CCamera()
{

} 

HVSTATUS CCamera::Set_Set_Video_Mode(HANDLE hDevice,HV_VIDEO_MODE mode)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	
	HV_ARG_SET_VIDEO_FORMAT_MODE_FRAME_RATE InBuf;
	InBuf.VideoFormat = VIDEO_FORMAT7;
	InBuf.VideoMode = (HV_VIDEO_MODE)(VIDEO_MODE0 + mode);
	InBuf.FrameRate = FRAME_RATE_IGNORE;

	status = HVControl(hDevice,
				ORD_SET_VIDEO_FORMAT_MODE_FRAME_RATE,
				&InBuf,sizeof(HV_ARG_SET_VIDEO_FORMAT_MODE_FRAME_RATE),
				NULL,0,
				NULL); 

	return status;
}

HVSTATUS CCamera::Set_Snap_Mode(HANDLE hDevice,HV_SNAP_MODE mode)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	HV_ARG_SET_FEATURE InBuf; 
	switch(mode)
	{
	case CONTINUATION:		
		InBuf.FeatureId = FEATURE_ID_TRIGGER;
		InBuf.Feature.Trigger.OnOff = 0;
		InBuf.FeatureItemId = FEATURE_ITEM_ID_TRIGGER_ON_OFF; 
		status = HVControl(hDevice,ORD_SET_FEATURE,&InBuf,sizeof(HV_ARG_SET_FEATURE),NULL,0,NULL); 
		break;
	case TRIGGER:
		InBuf.FeatureId = FEATURE_ID_TRIGGER;
		InBuf.Feature.Trigger.OnOff = 1;
		InBuf.FeatureItemId = FEATURE_ITEM_ID_TRIGGER_ON_OFF;
		status = HVControl(hDevice,ORD_SET_FEATURE,&InBuf,sizeof(HV_ARG_SET_FEATURE),NULL,0,NULL); 	

		InBuf.FeatureId = FEATURE_ID_TRIGGER;
		InBuf.Feature.Trigger.Mode = 0;
		InBuf.FeatureItemId = FEATURE_ITEM_ID_TRIGGER_MODE;	
		status = HVControl(hDevice,ORD_SET_FEATURE,&InBuf,sizeof(HV_ARG_SET_FEATURE),NULL,0,NULL); 	
		break;
	case TRIGGER_EDGE: 
		InBuf.FeatureId = FEATURE_ID_TRIGGER;
		InBuf.Feature.Trigger.OnOff = 1;
		InBuf.FeatureItemId = FEATURE_ITEM_ID_TRIGGER_ON_OFF;
		status = HVControl(hDevice,ORD_SET_FEATURE,&InBuf,sizeof(HV_ARG_SET_FEATURE),NULL,0,NULL); 

		InBuf.FeatureId = FEATURE_ID_TRIGGER;
		InBuf.Feature.Trigger.Mode = IsHVFSerial(hDevice)?2:1;
		InBuf.FeatureItemId = FEATURE_ITEM_ID_TRIGGER_MODE;	
		status = HVControl(hDevice,ORD_SET_FEATURE,&InBuf,sizeof(HV_ARG_SET_FEATURE),NULL,0,NULL); 
		break;
	case TRIGGER_LEVEL:	
		InBuf.FeatureId = FEATURE_ID_TRIGGER;
		InBuf.Feature.Trigger.OnOff = 1;
		InBuf.FeatureItemId = FEATURE_ITEM_ID_TRIGGER_ON_OFF;
		status = HVControl(hDevice,ORD_SET_FEATURE,&InBuf,sizeof(HV_ARG_SET_FEATURE),NULL,0,NULL); 
		
		InBuf.FeatureId = FEATURE_ID_TRIGGER;
		InBuf.Feature.Trigger.Mode = IsHVFSerial(hDevice)?1:2;
		InBuf.FeatureItemId = FEATURE_ITEM_ID_TRIGGER_MODE;	
		status = HVControl(hDevice,ORD_SET_FEATURE,&InBuf,sizeof(HV_ARG_SET_FEATURE),NULL,0,NULL); 
		break;
	default:
		status = STATUS_PARAMETER_INVALID;
		break;
	}

	return status;
}
HVSTATUS CCamera::Set_10_2_8_Mode(HANDLE hDevice, long lVal)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;

	status = _Write_Quadlet(hDevice,NOISE_CONTROL,lVal);
	return status;
}

#define SHUTTERWIDTH_H 			0x08		
#define SHUTTERWIDTH_L 			0x09
HVSTATUS CCamera::Set_Exposure_Time(HANDLE hDevice, long lVal)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	
	ULONG ulV = lVal; 
 

	unsigned long dwVal = 0;
	DWORD Mask = 0xffff;
	
	//屏蔽掉高16位，并将低16位写入09寄存器
	dwVal = ulV;
	dwVal &= Mask;               
	if(_Write_Sensor_Registers(hDevice,&dwVal, SHUTTERWIDTH_L,1) != STATUS_OK)
		return STATUS_HW_IO_ERROR;
	
	//右移16位，通过这种方式将高16位写入08寄存器
	dwVal = ulV;
	dwVal = dwVal >> 16;         
	dwVal &= Mask;               //本行代码可不用	
	if(dwVal >0)
	{
		if(_Write_Sensor_Registers(hDevice,&dwVal, SHUTTERWIDTH_H,1) != STATUS_OK)
			return STATUS_HW_IO_ERROR;
	}
	
	status = STATUS_OK;

	return status;
}

HVSTATUS CCamera::Set_Trigger_Polarity(HANDLE hDevice, HV_POLARITY polarity)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	HV_ARG_SET_FEATURE InBuf; 
	InBuf.FeatureId = FEATURE_ID_TRIGGER;
	InBuf.FeatureItemId = FEATURE_ITEM_ID_TRIGGER_POLARITY; 
	switch(polarity)
	{
	case LOW:		
		InBuf.Feature.Trigger.Polarity = 0;
		break;
	case HIGH:
		InBuf.Feature.Trigger.Polarity = 1;	
		break; 
	default:
		status = STATUS_PARAMETER_INVALID;
		break;
	}
	status = HVControl(hDevice,
				ORD_SET_FEATURE,
				&InBuf,sizeof(HV_ARG_SET_FEATURE),
				NULL,0,NULL); 

	return status;
}
 
HVSTATUS CCamera::Set_Strobe_Polarity(HANDLE hDevice, HV_POLARITY polarity)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	HV_ARG_SET_STROBE InBuf; 
	InBuf.StrobeId = STROBE0;
	InBuf.StrobeItemId = STROBE_ITEM_ID_SIGNAL_POLARITY; 
	switch(polarity)
	{
	case LOW:		
		InBuf.Strobe.SignalPolarity = 0;
		break;
	case HIGH:
		InBuf.Strobe.SignalPolarity = 1;	
		break; 
	default:
		status = STATUS_PARAMETER_INVALID;
		break;
	}
	status = HVControl(hDevice,
				ORD_SET_STROBE,
				&InBuf,sizeof(HV_ARG_SET_STROBE),
				NULL,0,NULL); 

	return status;
}
 
HVSTATUS CCamera::Set_Strobe_On_Off(HANDLE hDevice, bool b)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	HV_ARG_SET_STROBE InBuf; 
	InBuf.StrobeId = STROBE0;
	InBuf.StrobeItemId = STROBE_ITEM_ID_ON_OFF; 
	InBuf.Strobe.OnOff = b; 
	status = HVControl(hDevice,
				ORD_SET_STROBE,
				&InBuf,sizeof(HV_ARG_SET_STROBE),
				NULL,0,NULL); 

	return status;
}
 

 
HVSTATUS CCamera::Set_Freeze_Frame_On_Off(HANDLE hDevice, bool b)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	HV_ARG_SET_ADV_BASIC_FUNC InBuf; 
	InBuf.FuncId = ADV_BASIC_FUNC_ID_FREEZE_FRAME;
	InBuf.FuncItemId = ADV_BASIC_FUNC_ITEM_ID_FREEZE_FRAME_START_FLAG; 
	InBuf.Func.FreezeFrame.StartFlag = b; 
	status = HVControl(hDevice,
				ORD_SET_ADV_BASIC_FUNC,
				&InBuf,sizeof(HV_ARG_SET_ADV_BASIC_FUNC),
				NULL,0,NULL); 

	return status;
}
 
HVSTATUS CCamera::Set_Brightness(HANDLE hDevice,HV_ADC_CONTROL id, long lVal)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;  
	long lTemp = lVal + 255; 	
	switch(id)
	{
	case ADC_BLKLEVEL_CAL_GREENCHANNEL1:
		status = _Write_Quadlet(hDevice,REG_CAMERA_GREEN1_BRIGHTNESS_CTL,lTemp); 
		break;
	case ADC_BLKLEVEL_CAL_GREENCHANNEL2:
		status = _Write_Quadlet(hDevice,REG_CAMERA_GREEN2_BRIGHTNESS_CTL,lTemp); 
		break;
	case ADC_BLKLEVEL_CAL_REDCHANNEL:
		status = _Write_Quadlet(hDevice,REG_CAMERA_RED_BRIGHTNESS_CTL,lTemp);  
		break;
	case ADC_BLKLEVEL_CAL_BLUECHANNEL:
		status = _Write_Quadlet(hDevice,REG_CAMERA_BLUE_BRIGHTNESS_CTL,lTemp); 
		break;
	default:
		break;
	} 
	return status;
}
 


#define CALCTRL 		0x62
HVSTATUS CCamera::Set_Black_Level_On_Off(HANDLE hDevice, long lVal)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE; 
	DWORD dwVal = 0;
	if(_Read_Sensor_Registers(hDevice,&dwVal, CALCTRL,1) != STATUS_OK)
		return STATUS_HW_IO_ERROR;

	if(lVal)//允许黑电平调整
	{
		dwVal |= 0x00000001L;
	}
	else//禁止黑电平调整
	{
		dwVal &= 0x0000FFFEL;
	} 

	if(_Write_Sensor_Registers(hDevice,&dwVal, CALCTRL,1) != STATUS_OK)
		return STATUS_HW_IO_ERROR;

	status = STATUS_OK;
	return status;
}
 

HVSTATUS CCamera::Set_LUT(HANDLE hDevice, void *pCtx)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	HV_CMD_LUT_DWENTRIES *pLutCtx=(HV_CMD_LUT_DWENTRIES*)(pCtx);

	HV_ARG_SET_ADV_LUT InBuf; 
	InBuf.dwOffset = pLutCtx->dwOffset;
	InBuf.nLength = pLutCtx->dwEntries;
	InBuf.pBuffer = pLutCtx->pBuffer; 
	status = HVControl(hDevice,
		ORD_SET_ADV_LUT,
		&InBuf,sizeof(HV_ARG_SET_ADV_LUT),
		NULL,0,NULL);  
	return status;
}
HVSTATUS CCamera::Set_Packet_Size(HANDLE hDevice, void *pCtx)
{	
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	HV_CMD_PACKET_SIZE *pPacketSizeCtx=(HV_CMD_PACKET_SIZE*)(pCtx);

	HV_ARG_SET_FMT7_MODE InBuf;
	BOOL bHVFserial =  IsHVFSerial(hDevice);
	if (bHVFserial)
	{
		InBuf.VideoMode = VIDEO_MODE0;
	}
	else
	{
		HV_RES_GET_VIDEO_FORMAT_MODE_FRAME_RATE resGetVMode;
		status = HVControl(hDevice,
			ORD_GET_VIDEO_FORMAT_MODE_FRAME_RATE,			
			NULL,
			0,
			&resGetVMode,
			sizeof(resGetVMode),
			NULL); 
		InBuf.VideoMode = resGetVMode.VideoMode;
	}
	
	InBuf.FuncId = FMT7_MODE_FUNC_ID_BAND_WIDTH;
	InBuf.ItemId = FMT7_MODE_ITEM_ID_BAND_WIDTH_PACKET_SIZE;
	InBuf.Fmt7Mode.BandWidth.PacketSize = pPacketSizeCtx->dwSize;	 
	status = HVControl(hDevice,
		ORD_SET_FMT7_MODE,
		&InBuf,sizeof(HV_ARG_SET_FMT7_MODE),
		NULL,0,NULL); 
	return status;
}

HVSTATUS CCamera::Fun_Interface1(HANDLE hDevice, void *pCtx)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	HV_INTERFACE1_CONTEXT *pIFCtx=(HV_INTERFACE1_CONTEXT*)(pCtx);
	 

	switch(pIFCtx->ID) {
	case OUTPUT_IO_0:
		{			
			HV_ARG_SET_PIO_OUTPUT_PORT InBuf; 
			InBuf.PortId = PIO_OUTPUT_PORT0;
			InBuf.PortFuncId = PIO_OUTPUT_PORT_FUNC_ID_SOURCE_SELECT; 
			InBuf.PortItemId = PIO_OUTPUT_PORT_ITEM_ID_SOURCE_SELECT_VALUE; 
			InBuf.OutputPort.SourceSelect.Value = pIFCtx->dwVal; 
			status = HVControl(hDevice,
				ORD_SET_PIO_OUTPUT_PORT,
				&InBuf,sizeof(HV_ARG_SET_PIO_OUTPUT_PORT),
				NULL,0,NULL);  
		} 
		break;
	case OUTPUT_IO_1:
		{			
			HV_ARG_SET_PIO_OUTPUT_PORT InBuf; 
			InBuf.PortId = PIO_OUTPUT_PORT1;
			InBuf.PortFuncId = PIO_OUTPUT_PORT_FUNC_ID_SOURCE_SELECT; 
			InBuf.PortItemId = PIO_OUTPUT_PORT_ITEM_ID_SOURCE_SELECT_VALUE; 
			InBuf.OutputPort.SourceSelect.Value = pIFCtx->dwVal; 
			status = HVControl(hDevice,
				ORD_SET_PIO_OUTPUT_PORT,
				&InBuf,sizeof(HV_ARG_SET_PIO_OUTPUT_PORT),
				NULL,0,NULL);  
		} 
		break;
	case OUTPUT_IO_2:
		{			
			HV_ARG_SET_PIO_OUTPUT_PORT InBuf; 
			InBuf.PortId = PIO_OUTPUT_PORT2;
			InBuf.PortFuncId = PIO_OUTPUT_PORT_FUNC_ID_SOURCE_SELECT; 
			InBuf.PortItemId = PIO_OUTPUT_PORT_ITEM_ID_SOURCE_SELECT_VALUE; 
			InBuf.OutputPort.SourceSelect.Value = pIFCtx->dwVal; 
			status = HVControl(hDevice,
				ORD_SET_PIO_OUTPUT_PORT,
				&InBuf,sizeof(HV_ARG_SET_PIO_OUTPUT_PORT),
				NULL,0,NULL);  
		} 
		break;
	case OUTPUT_IO_0_POL:
		{			
			HV_ARG_SET_PIO_OUTPUT_PORT InBuf; 
			InBuf.PortId = PIO_OUTPUT_PORT0;
			InBuf.PortFuncId = PIO_OUTPUT_PORT_FUNC_ID_USER_SET; 
			InBuf.PortItemId = PIO_OUTPUT_PORT_ITEM_ID_USER_SET_VALUE; 
			InBuf.OutputPort.UserSet.Value = pIFCtx->dwVal; 
			status = HVControl(hDevice,
				ORD_SET_PIO_OUTPUT_PORT,
				&InBuf,sizeof(HV_ARG_SET_PIO_OUTPUT_PORT),
				NULL,0,NULL);  
		} 
		break;
	case OUTPUT_IO_1_POL:
		{			
			HV_ARG_SET_PIO_OUTPUT_PORT InBuf; 
			InBuf.PortId = PIO_OUTPUT_PORT1;
			InBuf.PortFuncId = PIO_OUTPUT_PORT_FUNC_ID_USER_SET; 
			InBuf.PortItemId = PIO_OUTPUT_PORT_ITEM_ID_USER_SET_VALUE; 
			InBuf.OutputPort.UserSet.Value = pIFCtx->dwVal; 
			status = HVControl(hDevice,
				ORD_SET_PIO_OUTPUT_PORT,
				&InBuf,sizeof(HV_ARG_SET_PIO_OUTPUT_PORT),
				NULL,0,NULL);  
		} 
		break;
	case OUTPUT_IO_2_POL:
		{			
			HV_ARG_SET_PIO_OUTPUT_PORT InBuf; 
			InBuf.PortId = PIO_OUTPUT_PORT2;
			InBuf.PortFuncId = PIO_OUTPUT_PORT_FUNC_ID_USER_SET; 
			InBuf.PortItemId = PIO_OUTPUT_PORT_ITEM_ID_USER_SET_VALUE; 
			InBuf.OutputPort.UserSet.Value = pIFCtx->dwVal; 
			status = HVControl(hDevice,
				ORD_SET_PIO_OUTPUT_PORT,
				&InBuf,sizeof(HV_ARG_SET_PIO_OUTPUT_PORT),
				NULL,0,NULL);  
		}  
		break;
	case FILTER_TIME_UNIT:
		{			
			HV_ARG_SET_ADV_FEATURE InBuf; 
			InBuf.AdvFeatureId = ADV_FEATURE_ID_FILTER_TIME_UNIT;
			InBuf.AdvFeatureItemId = ADV_FEATURE_ITEM_ID_SELECTOR_MODE; 
			InBuf.AdvFeature.Selector.Mode = pIFCtx->dwVal; 
			status = HVControl(hDevice,
				ORD_SET_ADV_FEATURE,
				&InBuf,sizeof(HV_ARG_SET_ADV_FEATURE),
				NULL,0,NULL);  
		} 
		break;
	case FILTER_TIME:
		{			
			HV_ARG_SET_ADV_FEATURE InBuf; 
			InBuf.AdvFeatureId = ADV_FEATURE_ID_FILTER_TIME;
			InBuf.AdvFeatureItemId = ADV_FEATURE_ITEM_ID_FILTER_TIME_VALUE; 
			InBuf.AdvFeature.FilterTime.Value = pIFCtx->dwVal; 
			status = HVControl(hDevice,
				ORD_SET_ADV_FEATURE,
				&InBuf,sizeof(HV_ARG_SET_ADV_FEATURE),
				NULL,0,NULL);  
		} 
		break;
	case TRIGGER_SOURCE_IO:
		{ 
			HV_ARG_SET_FEATURE InBuf; 
			InBuf.FeatureId = FEATURE_ID_TRIGGER;
			InBuf.FeatureItemId = FEATURE_ITEM_ID_TRIGGER_SOURCE; 
			InBuf.Feature.Trigger.Source = pIFCtx->dwVal; 
			status = HVControl(hDevice,
				ORD_SET_FEATURE,
				&InBuf,sizeof(HV_ARG_SET_FEATURE),
				NULL,0,NULL); 
		}
		break;
	case LE_BE_MODE:
		{			
			HV_ARG_SET_ADV_FEATURE InBuf; 
			InBuf.AdvFeatureId = ADV_FEATURE_ID_PIXEL_ENDIANESS;
			InBuf.AdvFeatureItemId = ADV_FEATURE_ITEM_ID_SELECTOR_MODE; 
			InBuf.AdvFeature.Selector.Mode = pIFCtx->dwVal; 
			status = HVControl(hDevice,
				ORD_SET_ADV_FEATURE,
				&InBuf,sizeof(HV_ARG_SET_ADV_FEATURE),
				NULL,0,NULL);  
		} 
		break;
	case TRANSFERS_DELAY_VALUE: 
		{			
			HV_ARG_SET_ADV_FEATURE InBuf; 
			InBuf.AdvFeatureId = ADV_FEATURE_ID_DELAY_TRANSMIT;
			InBuf.AdvFeatureItemId = ADV_FEATURE_ITEM_ID_DELAY_TRANSMIT_DELAY_TIME; 
			InBuf.AdvFeature.DelayTransmit.DelayTime = pIFCtx->dwVal; 
			status = HVControl(hDevice,
				ORD_SET_ADV_FEATURE,
				&InBuf,sizeof(HV_ARG_SET_ADV_FEATURE),
				NULL,0,NULL);  
		}
		break;
	case STROBE_ON_OFF:
		{
			CCamera camSetStrobeOnff;
			status = camSetStrobeOnff.Set_Strobe_On_Off(hDevice, pIFCtx->dwVal?1:0); 
		}
		break; 
	case TRIGGER_DELAY_ON_OFF:
		{ 
			HV_ARG_SET_FEATURE InBuf; 
			InBuf.FeatureId = FEATURE_ID_TRIGGER_DELAY;
			InBuf.FeatureItemId = FEATURE_ITEM_ID_SCALAR_ON_OFF; 
			InBuf.Feature.Trigger.OnOff = pIFCtx->dwVal; 
			status = HVControl(hDevice,
				ORD_SET_FEATURE,
				&InBuf,sizeof(HV_ARG_SET_FEATURE),
				NULL,0,NULL); 
		}
		break;
	case TRIGGER_DELAY_VALUE:
		{ 
			HV_ARG_SET_FEATURE InBuf; 
			InBuf.FeatureId = FEATURE_ID_TRIGGER_DELAY;
			InBuf.FeatureItemId = FEATURE_ITEM_ID_SCALAR_VALUE; 
			InBuf.Feature.Scalar.Value = pIFCtx->dwVal; 
			status = HVControl(hDevice,
				ORD_SET_FEATURE,
				&InBuf,sizeof(HV_ARG_SET_FEATURE),
				NULL,0,NULL); 
		}
		break;
	case COLOR_MODE:
		{
			HV_ARG_SET_FMT7_MODE InBuf;
			InBuf.VideoMode = VIDEO_MODE0; //应该取当前值...
			InBuf.FuncId = FMT7_MODE_FUNC_ID_COLOR_CODING;
			InBuf.ItemId = FMT7_MODE_ITEM_ID_COLOR_MODE_COLOR_CODE;
			InBuf.Fmt7Mode.ColorMode.Code = (HV_COLOR_CODE)pIFCtx->dwVal;
 
			
			status = HVControl(hDevice,
				ORD_SET_FMT7_MODE,
				&InBuf,sizeof(HV_ARG_SET_FMT7_MODE),
				NULL,0,NULL); 
		}
		break;
	case TEST_IMAGE:
		{			
			HV_ARG_SET_ADV_FEATURE InBuf; 
			InBuf.AdvFeatureId = ADV_FEATURE_ID_TEST_IMAGE;
			InBuf.AdvFeatureItemId = ADV_FEATURE_ITEM_ID_SELECTOR_MODE; 
			InBuf.AdvFeature.Selector.Mode = pIFCtx->dwVal; 
			status = HVControl(hDevice,
				ORD_SET_ADV_FEATURE,
				&InBuf,sizeof(HV_ARG_SET_ADV_FEATURE),
				NULL,0,
				NULL);  
		}
		break;
	default:
		status = STATUS_PARAMETER_INVALID;
		break;
	}
	return status;
}
//Set AOI
HVSTATUS CCamera::Set_AOI(HANDLE hDevice,HV_VIDEO_MODE mode,int nXStart, int nYStart, int nWidth, int nHeight)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;

	 
	HV_ARG_SET_FMT7_MODE InBuf;
	InBuf.VideoMode = mode; //应该取当前值...
	InBuf.FuncId = FMT7_MODE_FUNC_ID_AOI;
	InBuf.ItemId = FMT7_MODE_ITEM_ID_AOI_ALL;
	InBuf.Fmt7Mode.AOI.Top = nYStart;
	InBuf.Fmt7Mode.AOI.Left = nXStart;
	InBuf.Fmt7Mode.AOI.Height = nHeight;
	InBuf.Fmt7Mode.AOI.Width = nWidth;
	 
	status = HVControl(hDevice,
		ORD_SET_FMT7_MODE,
		&InBuf,sizeof(HV_ARG_SET_FMT7_MODE),
		NULL,0,
		NULL); 

	return status;
}
HVSTATUS CCamera::DevInfo_Get_SoftWare_Ver(HANDLE hDevice, void *pContext,int *pSize)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	char szAll[256] = {0};
	char szSysV[256] = {0};
	char szHVAPI[256] = {0};
	char szHVDailt[256] = {0};
	char szDeviceDrv[256] = {0};

	//开始获取内核驱动版本号, 如SV1410FC.sys 的版本
	sprintf(szSysV,"0.0.0.0");
	int nSize = 0;
	status = DevInfo_Get_Sys_Ver(hDevice,NULL,&nSize);
	if(STATUS_OK == status){
		status = DevInfo_Get_Sys_Ver(hDevice,szSysV,&nSize);
	}

	//开始获取设备驱动版本号, 如SV1410FC.dll 的版本
	sprintf(szDeviceDrv,"0.0.0.0");
	status = DevInfo_Get_Device_Drv_Ver(hDevice,NULL,&nSize);
	if(STATUS_OK == status){
		status = DevInfo_Get_Device_Drv_Ver(hDevice,szDeviceDrv,&nSize);
	}



	//开始获取HVAPI.dll 版本号, 如SV1410FC.dll 的版本 
	sprintf(szHVAPI,"0.0.0.0");
	status = DevInfo_Get_HVAPI_Ver(hDevice,NULL,&nSize);
	if(STATUS_OK == status){
		status = DevInfo_Get_HVAPI_Ver(hDevice,szHVAPI,&nSize);
	}

	//sprintf(szHVDailt,"4.0.0.2"); //本模块(HVDAILT.dll) 版本号设置
	VERSION version = GetLibraryVersion("HVDAILT.dll");
	sprintf(szHVDailt,"%d.%d.%d.%d",version.wMajor,version.wMinor,version.wRevision,version.wBuild);	

	sprintf(szAll,"%s;%s;%s;%s",szHVDailt,szHVAPI,szDeviceDrv,szSysV);
 
	if(pContext == NULL && pSize != NULL)
	{
		*pSize = strlen(szAll);
		status = STATUS_OK;
	}
	else if(pContext)
	{
		sprintf((char*)pContext,"%s",szAll);
		status = STATUS_OK;	
	}
	
	return status;
}
  
HVSTATUS CCamera::DevInfo_Get_Bayer_Layout(HANDLE hDevice, void *pContext,int *pSize)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	
	if (pContext == NULL)
	{
		if(pSize != NULL)
		{
			*pSize = sizeof(DWORD);
			status = STATUS_OK;
		}
	}
	else
	{
		if (*pSize == sizeof(DWORD)) {			
			HV_ARG_QUERY_FMT7_MODE_DESCRIPTOR InBuf;
			InBuf.VideoMode = VIDEO_MODE0;
			InBuf.FuncId = FMT7_MODE_FUNC_ID_COLOR_FILTER;
			
			HV_RES_QUERY_FMT7_MODE_DESCRIPTOR OutBuf; 
			status = HVControl(hDevice,ORD_QUERY_FMT7_MODE_DESCRIPTOR,
				&InBuf,sizeof(HV_ARG_QUERY_FMT7_MODE_DESCRIPTOR),
				&OutBuf,sizeof(HV_RES_QUERY_FMT7_MODE_DESCRIPTOR),
				NULL);
			
			*(DWORD *)pContext = OutBuf.ColorFilterMode.Filter?OutBuf.ColorFilterMode.Filter:4;	//当dwV=0时,改为4; 1,2,3　不变
			
		}
	}
	return status;
}
HVSTATUS CCamera::DevInfo_Get_Device_Type(HANDLE hDevice, void *pContext,int *pSize)
{	
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	if(pContext && pSize)
	{		
		HV_ARG_GET_STRING arg_GString;
		arg_GString.StringId = STRING_ID_DEVICE_TYPE;
		arg_GString.nLength = *pSize;
		HV_RES_GET_STRING res_GSring;
		res_GSring.pBuffer = (char*)pContext;
        status = HVControl(hDevice,ORD_GET_STRING,
			&arg_GString, sizeof(HV_ARG_GET_STRING), 
			&res_GSring, sizeof(HV_RES_GET_STRING), 
			NULL);
	}
	else
	{
		if(pSize != NULL)
		{
			HV_ARG_QUERY_STRING_DESCRIPTOR arg_QSring;
			arg_QSring.StringId = STRING_ID_DEVICE_TYPE;
			HV_RES_QUERY_STRING_DESCRIPTOR res_QGetSize;
			
			status = HVControl(hDevice,ORD_QUERY_STRING_DESCRIPTOR,
				&arg_QSring,sizeof(HV_ARG_QUERY_STRING_DESCRIPTOR),
				&res_QGetSize,sizeof(HV_RES_QUERY_STRING_DESCRIPTOR),NULL);
			
			int nSize = res_QGetSize.dwTotalByteLength;
			*pSize = nSize;			
		}
	}

	return status;
}
HVSTATUS CCamera::DevInfo_Get_Sensor_ID(HANDLE hDevice, void *pContext,int *pSize)
{	
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	if(pContext && pSize)
	{		
		HV_ARG_GET_STRING arg_GString;
		arg_GString.StringId = STRING_ID_DEVICE_CHIPID;
		arg_GString.nLength = *pSize;
		HV_RES_GET_STRING res_GSring;
		res_GSring.pBuffer = (char*)pContext; 
		status = HVControl(hDevice,ORD_GET_STRING,
			&arg_GString, sizeof(HV_ARG_GET_STRING), 
			&res_GSring, sizeof(HV_RES_GET_STRING), 
			NULL);
	}
	else
	{
		if(pSize != NULL)
		{
			HV_ARG_QUERY_STRING_DESCRIPTOR arg_QSring;
			arg_QSring.StringId = STRING_ID_DEVICE_CHIPID;
			HV_RES_QUERY_STRING_DESCRIPTOR res_QGetSize;
			
			status = HVControl(hDevice,ORD_QUERY_STRING_DESCRIPTOR,
				&arg_QSring,sizeof(HV_ARG_QUERY_STRING_DESCRIPTOR),
				&res_QGetSize,sizeof(HV_RES_QUERY_STRING_DESCRIPTOR),0);
			
			int nSize = res_QGetSize.dwTotalByteLength;
			*pSize = nSize;			
		}
	}

	return status;
}
//DESC_RESOLUTION
HVSTATUS CCamera::DevInfo_Get_Resolution(HANDLE hDevice, void *pContext,int *pSize)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE; 
	
	int nMode = 0;
	HV_ARG_QUERY_VIDEO_MODE_PRESENCE InBuf;
	InBuf.VideoFormat = VIDEO_FORMAT7;
	HV_RES_QUERY_VIDEO_MODE_PRESENCE OutBufMode; 
	memset(&OutBufMode,0,sizeof(int));
	status = HVControl(hDevice,ORD_QUERY_VIDEO_MODE_PRESENCE,
		&InBuf,sizeof(HV_ARG_QUERY_VIDEO_MODE_PRESENCE),
		&OutBufMode,sizeof(HV_RES_QUERY_VIDEO_MODE_PRESENCE),
		NULL);
	if(OutBufMode.Mode0) nMode++;
	if(OutBufMode.Mode1) nMode++;
	if(OutBufMode.Mode2) nMode++;
	if(OutBufMode.Mode3) nMode++;
	if(OutBufMode.Mode4) nMode++;
	if(OutBufMode.Mode5) nMode++;
	if(OutBufMode.Mode6) nMode++; 
	
	
	if(pContext && *pSize == 2 * nMode * sizeof(DWORD))
	{	
		int nIdx = 0;
		int n = 0;
		while(nMode)
		{
			HV_ARG_QUERY_FMT7_MODE_DESCRIPTOR InBuf;
			InBuf.VideoMode = (HV_VIDEO_MODE)(VIDEO_MODE0 + nIdx);
			InBuf.FuncId = FMT7_MODE_FUNC_ID_AOI;
			
			HV_RES_QUERY_FMT7_MODE_DESCRIPTOR OutBuf; 
			status = HVControl(hDevice,ORD_QUERY_FMT7_MODE_DESCRIPTOR,
						&InBuf,sizeof(HV_ARG_QUERY_FMT7_MODE_DESCRIPTOR),
						&OutBuf,sizeof(HV_RES_QUERY_FMT7_MODE_DESCRIPTOR),
						NULL);
			
			if(STATUS_OK == status){
				int nVMax = OutBuf.AOI.VerticalMaxInq;
				int nHMax = OutBuf.AOI.HorizontalMaxInq;
				int nVUnit = OutBuf.AOI.VerticalUnitInq;
				int nHUnit = OutBuf.AOI.HorizontalUnitInq; 
				((DWORD *)pContext)[n] = nHMax; 
				n++;
				((DWORD *)pContext)[n] = nVMax; 
				n++; 
			} 
			else{
				return status;
			}
			nIdx++;
			nMode--;
		} 
	}
	else
	{
		if(pSize != NULL)
		{
			*pSize = 2 * nMode * sizeof(DWORD);  
		}
	}
	return status;	
}



HVSTATUS CCamera::DevInfo_Get_SN(HANDLE hDevice, void *pContext, int *pSize)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;

	if(pContext && pSize)
	{
		
		HV_ARG_GET_STRING arg_GString;
		arg_GString.StringId = STRING_ID_DEVICE_SERIESNUM;
		arg_GString.nLength = *pSize;
		HV_RES_GET_STRING res_GSring;
		res_GSring.pBuffer = (char*)pContext;
        status = HVControl(hDevice,ORD_GET_STRING,
			&arg_GString, sizeof(HV_ARG_GET_STRING), 
			&res_GSring, sizeof(HV_RES_GET_STRING), 
			NULL);
	}
	else
	{
		if(pSize != NULL)
		{
			HV_ARG_QUERY_STRING_DESCRIPTOR arg_QSring;
			arg_QSring.StringId = STRING_ID_DEVICE_SERIESNUM;
			HV_RES_QUERY_STRING_DESCRIPTOR res_QGetSize;
			
			status = HVControl(hDevice,ORD_QUERY_STRING_DESCRIPTOR,
				&arg_QSring,sizeof(HV_ARG_QUERY_STRING_DESCRIPTOR),
				&res_QGetSize,sizeof(HV_RES_QUERY_STRING_DESCRIPTOR),
				NULL);
			
			int nSize = res_QGetSize.dwTotalByteLength;
			*pSize = nSize;
			
		}
	}

	return status;

}

HVSTATUS CCamera::DevInfo_Get_FPGA_Ver(HANDLE hDevice, void *pContext, int *pSize)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	
	if(pContext && pSize)
	{
		
		HV_ARG_GET_STRING arg_GString;
		arg_GString.StringId = STRING_ID_FPGA_VERSION;
		arg_GString.nLength = *pSize;
		HV_RES_GET_STRING res_GSring;
		res_GSring.pBuffer = (char*)pContext;
        status = HVControl(hDevice,ORD_GET_STRING,
			&arg_GString, sizeof(HV_ARG_GET_STRING), 
			&res_GSring, sizeof(HV_RES_GET_STRING), 
			NULL);
		
	}
	else
	{
		if(pSize != NULL)
		{
			HV_ARG_QUERY_STRING_DESCRIPTOR arg_QSring;
			arg_QSring.StringId = STRING_ID_FPGA_VERSION;
			HV_RES_QUERY_STRING_DESCRIPTOR res_QGetSize;
			
			status = HVControl(hDevice,ORD_QUERY_STRING_DESCRIPTOR,
				&arg_QSring,sizeof(HV_ARG_QUERY_STRING_DESCRIPTOR),
				&res_QGetSize,sizeof(HV_RES_QUERY_STRING_DESCRIPTOR),
				NULL);
			
			int nSize = res_QGetSize.dwTotalByteLength;
			*pSize = nSize;
			
		}
	}
	
	return status;

}

HVSTATUS CCamera::DevInfo_Get_Hardware_Ver(HANDLE hDevice, void *pContext, int *pSize)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	
	if(pContext && pSize)
	{
		
		HV_ARG_GET_STRING arg_GString;
		arg_GString.StringId = STRING_ID_FIRMWARE_VERSION;
		arg_GString.nLength = *pSize;
		HV_RES_GET_STRING res_GSring;
		res_GSring.pBuffer = (char*)pContext;
        status = HVControl(hDevice,ORD_GET_STRING,
			&arg_GString, sizeof(HV_ARG_GET_STRING), 
			&res_GSring, sizeof(HV_RES_GET_STRING), 
			NULL);		
	}
	else
	{
		if(pSize != NULL)
		{
			HV_ARG_QUERY_STRING_DESCRIPTOR arg_QSring;
			arg_QSring.StringId = STRING_ID_FIRMWARE_VERSION;
			HV_RES_QUERY_STRING_DESCRIPTOR res_QGetSize;
			
			status = HVControl(hDevice,ORD_QUERY_STRING_DESCRIPTOR,
				&arg_QSring,sizeof(HV_ARG_QUERY_STRING_DESCRIPTOR),
				&res_QGetSize,sizeof(HV_RES_QUERY_STRING_DESCRIPTOR),
				NULL);
			
			int nSize = res_QGetSize.dwTotalByteLength;
			*pSize = nSize;
			
		}
	}
	
	return status;
}

HVSTATUS CCamera::DevInfo_Get_Device_Name(HANDLE hDevice, void *pContext, int *pSize)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	
	if(pContext && pSize)
	{
		
		HV_ARG_GET_STRING arg_GString;
		arg_GString.StringId = STRING_ID_DEVICE_NAME;
		arg_GString.nLength = *pSize;
		HV_RES_GET_STRING res_GSring;
		res_GSring.pBuffer = (char*)pContext;
        status = HVControl(hDevice,ORD_GET_STRING,
			&arg_GString, sizeof(HV_ARG_GET_STRING), 
			&res_GSring, sizeof(HV_RES_GET_STRING), 
			NULL);
		
	}
	else
	{
		if(pSize != NULL)
		{
			HV_ARG_QUERY_STRING_DESCRIPTOR arg_QSring;
			arg_QSring.StringId = STRING_ID_DEVICE_NAME;
			HV_RES_QUERY_STRING_DESCRIPTOR res_QGetSize;
			
			status = HVControl(hDevice,ORD_QUERY_STRING_DESCRIPTOR,
				&arg_QSring,sizeof(HV_ARG_QUERY_STRING_DESCRIPTOR),
				&res_QGetSize,sizeof(HV_RES_QUERY_STRING_DESCRIPTOR),
				NULL);
			
			int nSize = res_QGetSize.dwTotalByteLength;
			*pSize = nSize;
			
		}
	}
	
	return status;
}


HVSTATUS CCamera::DevInfo_Get_Vender_Name(HANDLE hDevice, void *pContext, int *pSize)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	
	if(pContext && pSize)
	{
		
		HV_ARG_GET_STRING arg_GString;
		arg_GString.StringId = STRING_ID_VENDOR_NAME;
		arg_GString.nLength = *pSize;
		HV_RES_GET_STRING res_GSring;
		res_GSring.pBuffer = (char*)pContext;
        status = HVControl(hDevice,ORD_GET_STRING,
			&arg_GString, sizeof(HV_ARG_GET_STRING), 
			&res_GSring, sizeof(HV_RES_GET_STRING), 
			NULL);
		
	}
	else
	{
		if(pSize != NULL)
		{
			HV_ARG_QUERY_STRING_DESCRIPTOR arg_QSring;
			arg_QSring.StringId = STRING_ID_VENDOR_NAME;
			HV_RES_QUERY_STRING_DESCRIPTOR res_QGetSize;
			
			status = HVControl(hDevice,ORD_QUERY_STRING_DESCRIPTOR,
				&arg_QSring,sizeof(HV_ARG_QUERY_STRING_DESCRIPTOR),
				&res_QGetSize,sizeof(HV_RES_QUERY_STRING_DESCRIPTOR),
				NULL);
			
			int nSize = res_QGetSize.dwTotalByteLength;
			*pSize = nSize;
			
		}
	}
	
	return status;
}

HVSTATUS CCamera::DevInfo_Get_Sys_Ver(HANDLE hDevice, void *pContext, int *pSize)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	
	if(pContext && pSize)
	{
		
		HV_ARG_GET_STRING arg_GString;
		arg_GString.StringId = STRING_ID_SYS_DRIVER_VERSION;
		arg_GString.nLength = *pSize;
		HV_RES_GET_STRING res_GSring;
		res_GSring.pBuffer = (char*)pContext;
        status = HVControl(hDevice,ORD_GET_STRING,
			&arg_GString, sizeof(HV_ARG_GET_STRING), 
			&res_GSring, sizeof(HV_RES_GET_STRING), 
			NULL);		
	}
	else
	{
		if(pSize != NULL)
		{
			HV_ARG_QUERY_STRING_DESCRIPTOR arg_QSring;
			arg_QSring.StringId = STRING_ID_SYS_DRIVER_VERSION;
			HV_RES_QUERY_STRING_DESCRIPTOR res_QGetSize;
			
			status = HVControl(hDevice,ORD_QUERY_STRING_DESCRIPTOR,
				&arg_QSring,sizeof(HV_ARG_QUERY_STRING_DESCRIPTOR),
				&res_QGetSize,sizeof(HV_RES_QUERY_STRING_DESCRIPTOR),
				NULL);
			
			int nSize = res_QGetSize.dwTotalByteLength;
			*pSize = nSize;
			
		}
	}
	
	return status;


}

HVSTATUS CCamera::DevInfo_Get_Device_Drv_Ver(HANDLE hDevice, void *pContext, int *pSize)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	
	if(pContext && pSize)
	{
		
		HV_ARG_GET_STRING arg_GString;
		arg_GString.StringId = STRING_ID_DEVICE_DRIVER_VERSION;
		arg_GString.nLength = *pSize;
		HV_RES_GET_STRING res_GSring;
		res_GSring.pBuffer = (char*)pContext;
        status = HVControl(hDevice,ORD_GET_STRING,
			&arg_GString, sizeof(HV_ARG_GET_STRING), 
			&res_GSring, sizeof(HV_RES_GET_STRING), 
			NULL);		
	}
	else
	{
		if(pSize != NULL)
		{
			HV_ARG_QUERY_STRING_DESCRIPTOR arg_QSring;
			arg_QSring.StringId = STRING_ID_DEVICE_DRIVER_VERSION;
			HV_RES_QUERY_STRING_DESCRIPTOR res_QGetSize;
			
			status = HVControl(hDevice,ORD_QUERY_STRING_DESCRIPTOR,
				&arg_QSring,sizeof(HV_ARG_QUERY_STRING_DESCRIPTOR),
				&res_QGetSize,sizeof(HV_RES_QUERY_STRING_DESCRIPTOR),
				NULL);
			
			int nSize = res_QGetSize.dwTotalByteLength;
			*pSize = nSize;
			
		}
	}
	
	return status;


}

HVSTATUS CCamera::DevInfo_Get_HVAPI_Ver(HANDLE hDevice, void *pContext, int *pSize)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	
	if(pContext && pSize)
	{
		
		HV_ARG_GET_STRING arg_GString;
		arg_GString.StringId = STRING_ID_HVAPI_VERSION;
		arg_GString.nLength = *pSize;
		HV_RES_GET_STRING res_GSring;
		res_GSring.pBuffer = (char*)pContext;
        status = HVControl(hDevice,ORD_GET_STRING,
			&arg_GString, sizeof(HV_ARG_GET_STRING), 
			&res_GSring, sizeof(HV_RES_GET_STRING), 
			NULL);
		
	}
	else
	{
		if(pSize != NULL)
		{
			HV_ARG_QUERY_STRING_DESCRIPTOR arg_QSring;
			arg_QSring.StringId = STRING_ID_HVAPI_VERSION;
			HV_RES_QUERY_STRING_DESCRIPTOR res_QGetSize;
			
			status = HVControl(hDevice,ORD_QUERY_STRING_DESCRIPTOR,
				&arg_QSring,sizeof(HV_ARG_QUERY_STRING_DESCRIPTOR),
				&res_QGetSize,sizeof(HV_RES_QUERY_STRING_DESCRIPTOR),
				NULL);
			
			int nSize = res_QGetSize.dwTotalByteLength;
			*pSize = nSize;
			
		}
	}
	
	return status;


}

HVSTATUS CCamera::_Write_Quadlet(HANDLE hDevice,DWORD address, DWORD dwData)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
  
	HV_ARG_WRITE_QUADLET argWriteQuadlet; 
	argWriteQuadlet.dwAddress = address;
	argWriteQuadlet.dwData = dwData; 
	status = HVControl(hDevice,(HV_CONTROL_CODE)ORD_WRITE_QUADLET,
		&argWriteQuadlet, sizeof(HV_ARG_WRITE_QUADLET),
		NULL,0,NULL);

	return status;
}
 				
HVSTATUS CCamera::_Read_Block(HANDLE hDevice,DWORD address, DWORD *pData,int nLength)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
  
	HV_ARG_READ_BLOCK argReadBlock;
	argReadBlock.dwAddress = address; 
	argReadBlock.nLength = nLength;
	HV_RES_READ_BLOCK resReadBlock;
	resReadBlock.pData = (BYTE*)pData;
	status = HVControl(hDevice,(HV_CONTROL_CODE)ORD_READ_BLOCK,
		&argReadBlock, sizeof(HV_ARG_READ_BLOCK),
		&resReadBlock, sizeof(HV_RES_READ_BLOCK),
		NULL);

	return status;
}	
HVSTATUS CCamera::_Write_Block(HANDLE hDevice,DWORD address, DWORD *pData,int nLength)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
  
	HV_ARG_WRITE_BLOCK argWriteBlock;
	argWriteBlock.dwAddress = address;
	argWriteBlock.pData = (BYTE *)pData;
	argWriteBlock.nLength = nLength;
	status = HVControl(hDevice,(HV_CONTROL_CODE)ORD_WRITE_BLOCK,
		&argWriteBlock, sizeof(HV_ARG_WRITE_BLOCK),
		NULL, 0, 
		NULL);

	return status;
}
HVSTATUS CCamera::_Read_Sensor_Registers(HANDLE hDevice,DWORD* pData, DWORD dwStart,DWORD dwLen)
{ 
    if(pData == NULL)
        return STATUS_PARAMETER_INVALID;
    
    if(dwStart > 255 || dwLen > 255)
        return STATUS_PARAMETER_INVALID;
    
    if(STATUS_OK != _Write_Quadlet(hDevice,SENSOR_START_REG,dwStart))
    {
        return STATUS_HW_IO_ERROR;
    }
    if(STATUS_OK != _Read_Block(hDevice,SENSOR_I2C_OPERATE,pData,dwLen*sizeof(unsigned long)))
    {
        return STATUS_HW_IO_ERROR;
    }
    
    return (HVSTATUS)STATUS_OK;
}
 				
HVSTATUS CCamera::_Write_Sensor_Registers(HANDLE hDevice,DWORD *pData, DWORD dwStart, DWORD dwLen)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	if(pData == NULL)
		return STATUS_PARAMETER_INVALID;
	
	if(dwStart > 255 || dwLen > 255)
		return STATUS_PARAMETER_INVALID;
	
	DWORD *pReg = NULL;
	DWORD i;
	
	pReg = new DWORD [dwLen];
	if(pReg == NULL)
		return STATUS_NOT_ENOUGH_SYSTEM_MEMORY;
	
	for(i=0;i<dwLen;i++)
	{
		pReg[i] = ( (dwStart+i)<<16 ) | ( pData[i] & 0xFFFFL);
	}
	
	if(_Write_Block(hDevice,SENSOR_I2C_OPERATE,pReg,dwLen*sizeof(unsigned long)) != STATUS_OK)
	{
		delete []pReg;
		return STATUS_HW_IO_ERROR;
	}
	
	delete []pReg;
	status = STATUS_OK;
	return status;

}

HVSTATUS CCamera::_Read_Quadlet(HANDLE hDevice, DWORD address, DWORD *pData)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	
    HV_ARG_READ_QUADLET argReadQualdet;
	argReadQualdet.dwAddress=(DWORD)address;
	HV_RES_READ_QUADLET resReadQualdet;
	status =HVControl(hDevice,(HV_CONTROL_CODE)ORD_READ_QUADLET,&argReadQualdet,0,&resReadQualdet,0,0);
	
	*pData = resReadQualdet.dwData;
	
	return status;

}

HVSTATUS CCamera::Get_MAC(HANDLE hDevice, char *pOut)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;

	HV_ARG_GET_NET_FUNC  InBuf;
	HV_RES_GET_NET_FUNC  OutBuf;
	
	InBuf.NetFuncId = NET_FUNC_ID_DEVICE_IP;
	OutBuf.Address.pBuffer = pOut;
	
	status = HVControl(hDevice,
		ORD_GET_NET_FUNC,
		&InBuf,
		sizeof(HV_ARG_GET_NET_FUNC),
		&OutBuf,
		sizeof(HV_RES_GET_NET_FUNC),
		0);

	return status;

}

HVSTATUS CCamera::Get_IP(HANDLE hDevice, char *pOut)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;

	HV_ARG_GET_NET_FUNC  InBuf;
	HV_RES_GET_NET_FUNC  OutBuf;
	
	InBuf.NetFuncId = NET_FUNC_ID_DEVICE_MAC;
	OutBuf.Address.pBuffer = pOut;
	
	status = HVControl(hDevice,
		ORD_GET_NET_FUNC,
		&InBuf,
		sizeof(HV_ARG_GET_NET_FUNC),
		&OutBuf,
		sizeof(HV_RES_GET_NET_FUNC),
		0);

	return status;

}
