//File camera.cpp


#include "CAMOBJ.h"

#define RESERVED_001 46
///////////////////////////////////////////////////////////////////////////////

CCamera::CCamera()
{
    memset(this, 0, sizeof(CCamera));
	m_lpNotifyFun = NULL; 

	m_hMyCam = NULL;
	m_BusResetThreadParams = NULL;
	m_nAttNumErr = 0;
	m_bDeviceRemove=FALSE;
	m_lLostNum=0;
	m_lAllNum=0;

	m_hBlockEvent=NULL;
	m_pBlockBuffer=NULL;
    m_n8or12=1;
	m_Trigger_Source=0;
    m_AttachNum=0;
	m_BroadNum= 0;
	
    //
    // Init feature m_pCamera and offset.
    //
    m_Brightness.Init(this, BRIGHTNESS_OFFSET);
    m_AutoExposure.Init(this, AUTO_EXPOSURE_OFFSET);
    m_Sharpness.Init(this, SHARPNESS_OFFSET);
    m_WhiteBalance.Init(this, WHITE_BALANCE_OFFSET);
    m_Hue.Init(this, HUE_OFFSET);
    m_Saturation.Init(this, SATURATION_OFFSET);
    m_Gamma.Init(this, GAMMA_OFFSET);
    m_Shutter.Init(this, SHUTTER_OFFSET);
    m_Gain.Init(this, GAIN_OFFSET);
    m_Iris.Init(this, IRIS_OFFSET);
    m_Focus.Init(this, FOCUS_OFFSET);
    m_Temperature.Init(this, TEMPERATURE_OFFSET);
    m_TriggerDly.Init(this, TRIGGER_DLY_OFFSET);
    m_WhiteShd.Init(this, WHITE_SHD_OFFSET);
    m_FrameRate.Init(this, FRAME_RATE_OFFSET);
    
    m_Zoom.Init(this, ZOOM_OFFSET);
    m_Pan.Init(this, PAN_OFFSET);
    m_Tilt.Init(this, TILT_OFFSET);
    m_OpticalFilter.Init(this, OPTICAL_FILTER_OFFSET);
    m_CaptureSize.Init(this, CAPTURE_SIZE_OFFSET);
    m_CaptureQuality.Init(this, CAPTURE_QUALITY_OFFSET);
	
	m_Trigger.m_pCamera = this;
	m_Format7.m_pCamera = this;

	m_Strobe.m_pCamStrobe = this;
 

    //
    // Create Events and section.
    // 
    m_hEvNewFrame = CreateEvent(NULL, FALSE, FALSE, NULL);
    m_hEvStopSnap = CreateEvent(NULL, FALSE, FALSE, NULL);
    m_hEvTerminate = CreateEvent(NULL, FALSE , FALSE, NULL);

    m_hEvDisplayOver = CreateEvent(NULL, FALSE , FALSE, NULL);
    m_hEvAcqOver = CreateEvent(NULL, FALSE , FALSE, NULL); 

    InitializeCriticalSection(&m_CritSect);
}


//
// If we are grabbing images, stop it.  
//

CCamera::~CCamera()
{
    if (m_CamStatus == ContinuousGrabing) {
        v_Stop_Snap();
    }
    
    CloseHandle(m_hEvNewFrame);
    CloseHandle(m_hEvStopSnap);
    CloseHandle(m_hEvTerminate);
    CloseHandle(m_hEvDisplayOver);
    CloseHandle(m_hEvAcqOver); 
    DeleteCriticalSection(&m_CritSect);
}


HVSTATUS CCamera::f_INIT_CAMERA(void *pInBuffer)
{
	HVSTATUS status = STATUS_OK;//STATUS_NOT_SUPPORT_INTERFACE;

	if (!CheckPassword()) 
	{
        status = STATUS_HW_INIT_ERROR;
    }
	GET_MAX_SPEED_BETWEEN_DEVICES maxSpeed;
	DWORD dwRet;
	ULONG maxSpeedNotLocal, maxSpeedLocal;

	if(!m_pName)
	{
		status = STATUS_PARAMETER_INVALID;
	}

	if(m_hMyCam)
	{
		status = STATUS_IN_WORK;
	}

	// reset state
	// this frees up any isoch resources that may be left behind
	// from a previous program that didn't clean up after itself
	// properly (i.e. crashed)

	// this should *probably* have a return code, but if things
	// go wrong with resetting the state, other problems will rear up

	v_Stop_Video_Stream();		 
	//ResetCameraState(m_pName);
	v_Reset_Camera_State();

	// determine max speed
	// this is used for allocating bandwidth and stuff
	maxSpeed.fulFlags = 0;
	maxSpeed.ulNumberOfDestinations = 0;
	if (dwRet = GetMaxSpeedBetweenDevices(m_pName, &maxSpeed))
	{
		status = STATUS_HW_IO_ERROR;
	}
	maxSpeedNotLocal = maxSpeed.fulSpeed;

	maxSpeed.fulFlags = 1;
	if (dwRet = GetMaxSpeedBetweenDevices(m_pName, &maxSpeed))
	{
		status = STATUS_HW_IO_ERROR;
	}
	maxSpeedLocal = maxSpeed.fulSpeed;

	// select the smaller of the two
	m_maxSpeed = (maxSpeedLocal < maxSpeedNotLocal ? maxSpeedLocal : maxSpeedNotLocal);

	// determine video formats/modes/rates
	// private functions return bools and do their own respective tracing
	if(!InquireVideoFormats())
	{
		status = STATUS_HW_IO_ERROR;
	}

	if(!InquireVideoModes())
	{
		status = STATUS_HW_IO_ERROR;
	}

	if(!InquireVideoRates())
	{
		status = STATUS_HW_IO_ERROR;
	}

    InquireControlRegisters();
    StatusControlRegisters();
    LoadProfileSettings();
    
	return status;
}
HVSTATUS CCamera::f_CLEAR_PASSWORD()
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE; 	
	status = WriteQuadlet(0xf6f80018,0xcccccccc);	 
	return status;
}

HVSTATUS CCamera::f_CHECK_PASSWORD(void *pInBuffer)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE; 
	
	HV_ARG_CMD_RESERVED1 *pInBuf = (HV_ARG_CMD_RESERVED1 *)pInBuffer;
  	
	//step 1
	status = WriteQuadlet(0xf6f80004,0xbbbbbbbb);
	
	//step 2
	DWORD *pContext = (DWORD*)pInBuf->pContext;	
	if(STATUS_OK==status){
		for(int i = 0; i < pInBuf->nLength; i++)
		{
			status = WriteQuadlet(0xf6f8000c,pContext[i]);
			if(STATUS_OK!=status) break;
		}
	}
	
	//step 3
	if(STATUS_OK==status){
		DWORD dwTmp = ((pInBuf->nLength<<16)*4)|0x0000aaaa;
		status = WriteQuadlet(0xf6f80008,dwTmp);
	}		   
	 
	return status;
}
HVSTATUS CCamera::f_SET_PASSWORD(void *pInBuffer)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE; 
	
	HV_ARG_CMD_RESERVED0 *pInBuf = (HV_ARG_CMD_RESERVED0 *)pInBuffer;
  	
	//step 1
	status = WriteQuadlet(0xf6f80004,0xbbbbbbbb);

	
	//step 2
	DWORD *pContext = (DWORD*)pInBuf->pContext;
	if(STATUS_OK==status){
		for(int i = 0; i < pInBuf->nLength; i++)
		{
			status = WriteQuadlet(0xf6f80014,pContext[i]);
			if(STATUS_OK!=status) break;
		}
	}
	
	//step 3
	if(STATUS_OK==status){
		DWORD dwTmp = ((pInBuf->nLength<<16)*4)|0x00005555;
		status = WriteQuadlet(0xf6f80010,dwTmp);
	}		   
	 
	return status;
}
HVSTATUS CCamera::f_NOTIFY_REGESTER(void *pInBuffer)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE; 
	
	HV_ARG_NOTIFY_REGESTER *pInBuf = (HV_ARG_NOTIFY_REGESTER *)pInBuffer;
 
    m_lpNotifyFun = pInBuf->ftn; 
	
	status = STATUS_OK;
	return status;
}
HVSTATUS CCamera::f_OPEN_SNAP(void *pInBuffer)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE; 
	HV_ARG_OPEN_SNAP * pArgOpenSnap = (HV_ARG_OPEN_SNAP *)pInBuffer;
	
    if ( m_CamStatus != idle) {
        return STATUS_IN_WORK;
    }
	
    m_lpSnapProc = pArgOpenSnap->pSnapFunc;
    m_pSnapParam = pArgOpenSnap->pParam;
	
	
	if(m_lpNotifyFun){		
		HV_NOTIFY_INFO ni;
		ni.nMsgType = 1;				
		(*m_lpNotifyFun)(&ni);
	}
	status = STATUS_OK;
	return status;
}

DWORD WINAPI stubGrabThread(LPVOID lpThreadParameter)
{ 
    CCamera* pCam = ToClass(lpThreadParameter);

    int i;
    pCam->_Acq_Thread_Fun(&i);
    return 0;
}

DWORD WINAPI stubDisplayThread(LPVOID lpThreadParameter)
{ 
    CCamera* pCam = ToClass(lpThreadParameter); 

    pCam->_Display_Thread_Fun(0);
    return 0;
}
HVSTATUS CCamera::v_Start_Snap()
{
	HVSTATUS stRet = STATUS_NOT_SUPPORT_INTERFACE;

    DWORD m_ID; 
    if (v_Init_Snap(BUFFER_NUM, 0))
    {
        //ErrorMessageBox(g_pCam->m_hTwainDlg, 60004);
        //MessageBox(NULL, TEXT("创建采图缓冲区失败！\n\n频繁的改变采图宽度或高度可能会导致这个问题。"),TEXT("错误"), MB_OK|MB_ICONERROR);
        return STATUS_INTERNAL_ERROR;
    }
    
    m_CamStatus = ContinuousGrabing;

	
    HANDLE hGrabThread = ::CreateThread(NULL, 0, stubGrabThread, (void*)this, CREATE_SUSPENDED, &m_ID);
    if (hGrabThread == NULL)
    { 
        return STATUS_NOT_ENOUGH_SYSTEM_MEMORY;
    }

    HANDLE hDisplayThread = ::CreateThread(NULL, 0, stubDisplayThread, (void*)this, CREATE_SUSPENDED, &m_ID);
    if (hDisplayThread == NULL)
    { 
        return STATUS_NOT_ENOUGH_SYSTEM_MEMORY;
    }
    
    m_bStop_Disp = 0;
    m_hDisplayThread = hDisplayThread;
    SetThreadPriority(hDisplayThread, THREAD_PRIORITY_NORMAL); // BELOW_
    ResumeThread(hDisplayThread);
	while(m_bStop_Disp!=1)
	{
		Sleep(1);	
	}
	m_hGrabThread = hGrabThread;
	SetThreadPriority(hGrabThread, THREAD_PRIORITY_HIGHEST);
	ResumeThread(hGrabThread);

    return stRet;
	
}
 
HVSTATUS CCamera::f_START_SNAP(void *pInBuffer)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	 
	HV_ARG_START_SNAP * pArgStartSnap = (HV_ARG_START_SNAP *)pInBuffer;
	
    if (pArgStartSnap->nSum < 1 || pArgStartSnap->nSum > 255) {
        return STATUS_PARAMETER_OUT_OF_BOUND;
    }
	
    if (m_CamStatus != idle) {
        return STATUS_IN_WORK;
    }
	
    //
    // In 2k and xp os, the 0~64k process address space are 
    // reserved.
    //
    for (int i=0; i<pArgStartSnap->nSum; i++) {
        if ((DWORD)pArgStartSnap->ppBuffer[i] < 0x0000FFFF)
            return STATUS_PARAMETER_INVALID;
    }
	
    //
    // NOTE! i only use the first buffer.
    //
    //pCam->m_pSnapBuffer = ppBuffer[0];
    //pCam->m_nBufferSize = pCam->m_Width*pCam->m_Height;
    m_BufferNum = pArgStartSnap->nSum;
    m_ppSnapBuffer = new BYTE*[pArgStartSnap->nSum];
    if (m_ppSnapBuffer == NULL) {
        return STATUS_NOT_ENOUGH_SYSTEM_MEMORY;
    } else {
        memcpy(m_ppSnapBuffer, pArgStartSnap->ppBuffer, sizeof(BYTE*) * pArgStartSnap->nSum);
    }
	
    v_Start_Snap();

	status = STATUS_OK;
	return status;
}
HVSTATUS CCamera::f_STOP_SNAP()
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE; 

    if (m_CamStatus == ContinuousGrabing) {
 
		v_Stop_Snap();

        delete[] m_ppSnapBuffer;
        m_ppSnapBuffer = NULL;
    } else if (m_CamStatus == SingleGrabing) {
        m_CamStatus = idle;
        WaitForSingleObject(m_hGrabThread, NEARLYINFINITE);
    }
	
    CloseHandle(m_hGrabThread);
    CloseHandle(m_hDisplayThread);
    m_hGrabThread = NULL;
    m_hDisplayThread = NULL;
	
	status = STATUS_OK;
	return status;
}
HVSTATUS CCamera::f_CLOSE_SNAP()
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE; 
    
    f_STOP_SNAP();
	
    m_lpSnapProc = NULL;
    m_pSnapParam = NULL;

	status = STATUS_OK;
	return status;
}

DWORD WINAPI stubSnapShot(LPVOID lpThreadParameter)
{
    SnapShotCell* pCell = (SnapShotCell*)lpThreadParameter;
    CCamera* pCam = pCell->pCamera;
	
    pCam->_Snap_Shot_Fun(pCell->ppBuffer, pCell->nSum);
	
    
    _RPT0(_CRT_WARN, "【stubSnapShot Over】");
    SetEvent(pCell->hEvent);
    CloseHandle(pCell->hThread);
    delete pCell;
    
    return 1;
}

HVSTATUS CCamera::f_SNAP_SHOT(void *pInBuffer)
{
		HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;

	HV_ARG_SNAP_SHOT *pArgSnapShot = (HV_ARG_SNAP_SHOT *)pInBuffer;
    
    if (pArgSnapShot->nSum < 1) {
        status = STATUS_PARAMETER_INVALID;
		return status;
    }
	
    if (m_CamStatus != idle) {
        status = STATUS_IN_WORK;
		return status;
    }
    
    DWORD m_ID;
    SnapShotCell* pCell = new SnapShotCell;     // Will be delete in stubSnapShot thread.
   
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    pCell->pCamera = this;
    pCell->ppBuffer = pArgSnapShot->ppBuffer;
    pCell->nSum = pArgSnapShot->nSum;
    pCell->hEvent = hEvent;
	
    
    if (v_Init_Snap(1, 0))
    {
        MessageBox(NULL, TEXT("创建采图缓冲区失败！\n\n频繁的改变采图宽度或高度可能会导致这个问题。"), 
            TEXT("错误"), MB_OK|MB_ICONERROR);
    }
    
    pCell->hThread = ::CreateThread(NULL, 0, stubSnapShot, (void*)pCell, CREATE_SUSPENDED, &m_ID);
    //
    // Use m_hGrabThread as single snap thread handle, used in
    // HVCloseSnap.
    //
    m_hGrabThread = pCell->hThread;
	
    SetThreadPriority(pCell->hThread, THREAD_PRIORITY_NORMAL); // BELOW_
    ResumeThread(pCell->hThread);
	
    WaitForSingleObject(hEvent, INFINITE);
    CloseHandle(hEvent);
    _RPT0(_CRT_WARN, "【Wait Grab Thread Over】");
    status = STATUS_OK;
	
	return status;
}


HVSTATUS CCamera::f_TRIGGER_SHOT()
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	status = TriggerShot();
    return status;
}


HVSTATUS CCamera::f_SET_STROBE(void *pInBuffer)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	HV_ARG_SET_STROBE *pInBuf = (HV_ARG_SET_STROBE *)pInBuffer;
	switch(pInBuf->StrobeId)
	{
	case STROBE0:
		switch(pInBuf->StrobeItemId)
		{
		case STROBE_ITEM_ID_ON_OFF:
			status = m_Strobe.Set_On_Off(pInBuf->Strobe.OnOff);
			break;
		case STROBE_ITEM_ID_SIGNAL_POLARITY:
			status = m_Strobe.Set_Polarity(pInBuf->Strobe.SignalPolarity);
			break;
		default:
			status = STATUS_PARAMETER_INVALID;
			break;
		}
		break;
	case STROBE1:
			break;
	case STROBE2:
			break;
	case STROBE3:
			break;
	default:
		status = STATUS_PARAMETER_INVALID;
		break;
	}
	return status;
}

HVSTATUS CCamera::f_SET_FEATURE(void *pInBuffer)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	HV_ARG_SET_FEATURE *pInBuf = (HV_ARG_SET_FEATURE *)pInBuffer;
	switch(pInBuf->FeatureId)
	{
	case FEATURE_ID_SHUTTER:
		status = m_Shutter.GetInquireRegister();
		VerifyRetVal(status);
		m_Shutter.m_value = pInBuf->Feature.Scalar.Value;
		status = m_Shutter.SetStatusRegister();
		break;
	case FEATURE_ID_WHITE_BALANCE:
		{
			switch(pInBuf->FeatureItemId)
			{
			case FEATURE_ITEM_ID_WHITE_BALANCE_VR:
				status = m_WhiteBalance.GetStatusRegister();
				VerifyRetVal(status);
				m_WhiteBalance.m_value = pInBuf->Feature.WhiteBalance.VR;
				status = m_WhiteBalance.SetStatusRegister();
				break; 
			case FEATURE_ITEM_ID_WHITE_BALANCE_UB:
				status = m_WhiteBalance.GetStatusRegister();
				VerifyRetVal(status);
				m_WhiteBalance.m_ubvalue = pInBuf->Feature.WhiteBalance.UB;
				status = m_WhiteBalance.SetStatusRegister();
				break; 
			}
		}
		break;
	case FEATURE_ID_GAIN:
		status = m_Gain.GetInquireRegister();
		VerifyRetVal(status);
		m_Gain.m_value = pInBuf->Feature.Scalar.Value;
		status = m_Gain.SetStatusRegister();
		break;
	case FEATURE_ID_BRIGHTNESS:
		status = m_Brightness.GetInquireRegister();
		VerifyRetVal(status);
		m_Brightness.m_value = pInBuf->Feature.Scalar.Value;
		status = m_Brightness.SetStatusRegister();
		break;

	case FEATURE_ID_TRIGGER:
		{
			switch(pInBuf->FeatureItemId)
			{
			case FEATURE_ITEM_ID_TRIGGER_ON_OFF:
				status = m_Trigger.Set_On_Off(pInBuf->Feature.Trigger.OnOff);
				break;
			case FEATURE_ITEM_ID_TRIGGER_POLARITY:
				status = m_Trigger.Set_Polarity(pInBuf->Feature.Trigger.Polarity);
				break;
			case FEATURE_ITEM_ID_TRIGGER_SOURCE:
			//	status = m_Trigger.Set_Source(pInBuf->Feature.Trigger.Source);
				HV_INTERFACE1_CONTEXT pInfcectx;
				pInfcectx.dwVal = pInBuf->Feature.Trigger.Source;
				pInfcectx.ID=TRIGGER_SOURCE_IO;
				status = SetTriggerSource(&pInfcectx); 
				break;
			case FEATURE_ITEM_ID_TRIGGER_MODE:
				status = m_Trigger.Set_Mode(pInBuf->Feature.Trigger.Mode);
				break;
			default:
				status = STATUS_PARAMETER_INVALID;
				break;
			}
			break;
		}
	case FEATURE_ID_TRIGGER_DELAY:
		{
			status = m_TriggerDly.GetStatusRegister();
			VerifyRetVal(status);
			switch(pInBuf->FeatureItemId)
			{
			case FEATURE_ITEM_ID_SCALAR_ON_OFF:
				m_TriggerDly.m_OnOff = pInBuf->Feature.Scalar.OnOff;
				break; 
			case FEATURE_ITEM_ID_SCALAR_VALUE:
				m_TriggerDly.m_value = pInBuf->Feature.Scalar.Value;
				break;
			default:
				status = STATUS_PARAMETER_INVALID;
				break;
			}
			status = m_TriggerDly.SetStatusRegister();
			break;
		}
		break;

	}
	
	return status;
}
HVSTATUS CCamera::f_SET_ADV_BASIC_FUNC(void *pInBuffer)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	HV_ARG_SET_ADV_BASIC_FUNC *pInBuf = (HV_ARG_SET_ADV_BASIC_FUNC *)pInBuffer;
	switch(pInBuf->FuncId)
	{
	case ADV_BASIC_FUNC_ID_FREEZE_FRAME: 
		if(pInBuf->Func.FreezeFrame.StartFlag == 0)	//不冻结
		{
			status = WriteQuadlet(0xF2F00600L ,0x00000000L);
		}  
		else if(pInBuf->Func.FreezeFrame.StartFlag == 1)	//SV1310 CCD相机，只能冻结1幅图像。
		{
			DWORD dwVal = 0x80000000L | 1;
			status = WriteQuadlet(0xF2F00600L,dwVal);
		}
		break;
	}
	return status;
}

#define LUT_LENGTH		4*4*1024
HVSTATUS CCamera::f_SET_ADV_LUT(void *pInBuffer)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	HV_ARG_SET_ADV_LUT *pInBuf = (HV_ARG_SET_ADV_LUT *)pInBuffer; 
    
	
    if(pInBuf->nLength != LUT_LENGTH || pInBuf->pBuffer == NULL)		//参数合法吗?
        return STATUS_PARAMETER_INVALID;
	
    int nWriteLen = 2*1024;
    for(int i = 0; i < LUT_LENGTH; i += nWriteLen)
    {
        if( AsyncBlockWrite(0xf5f00000 + i, pInBuf->pBuffer+(i/4), nWriteLen) != STATUS_OK)
        {
            return STATUS_HW_IO_ERROR;
        }
    }
	status = STATUS_OK;
	return status;
}
HVSTATUS CCamera::f_SET_ADV_FEATURE(void *pInBuffer)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	HV_ARG_SET_ADV_FEATURE *pInBuf = (HV_ARG_SET_ADV_FEATURE *)pInBuffer;
	switch(pInBuf->AdvFeatureId)
	{
	case ADV_FEATURE_ID_SHUTTER_UNIT:
		{
			DWORD dwVal =  pInBuf->AdvFeature.Selector.Mode;
			status = ShutterUnit(dwVal);
		}
		break;
	case ADV_FEATURE_ID_FILTER_TIME_UNIT:
		{
			DWORD dwVal =  pInBuf->AdvFeature.Selector.Mode;
			status = FilterTimeUnit(dwVal);
		}
		break;
	case ADV_FEATURE_ID_FILTER_TIME:
		{
			DWORD dwVal =  pInBuf->AdvFeature.FilterTime.Value;
			status = FilterTimeValue(dwVal);
		}
		break;
	case ADV_FEATURE_ID_PIXEL_ENDIANESS:
		{
			DWORD dwVal =  pInBuf->AdvFeature.Selector.Mode;
			status = LE_BE_Mode(dwVal);
		}
		break;
	case ADV_FEATURE_ID_DELAY_TRANSMIT:
		{
			DWORD dwVal =  pInBuf->AdvFeature.DelayTransmit.DelayTime;
			status = TransfersDelayValue(dwVal);
		}
		break;
	case ADV_FEATURE_ID_TEST_IMAGE:
		switch(pInBuf->AdvFeatureItemId)
		{
		case ADV_FEATURE_ITEM_ID_SELECTOR_MODE:  
			{
				DWORD dwWriteValue;
				switch(pInBuf->AdvFeature.Selector.Mode){
				case 0:
					dwWriteValue=0x00000000;
					break;
				case 1:
					dwWriteValue=0x02010000;
					break;
				case 2:
					dwWriteValue=0x02020000;
					break;
				case 3:
					dwWriteValue=0x02030000;
					break;
				case 4:
					dwWriteValue=0x02040000;
					break;
				case 5:
					dwWriteValue=0x02050000;
					break;
				case 6:
					dwWriteValue=0x02060000;
					break;
				case 7:
					dwWriteValue=0x02070000;
					break;
				default:
					return STATUS_PARAMETER_INVALID;
					break;
				}
				
				status = WriteQuadlet(CCD_TEST_IMG_ADDR,dwWriteValue);
			}
			break; 
		default:
			status = STATUS_PARAMETER_INVALID;
			break;
		}
		break;
	case ADV_FEATURE_ID_STARTUP_MEM_CH://上电参数组设定
		{
			switch(pInBuf->AdvFeature.StartupMemCh.Number) 
			{
			case 0://0-factory
			case 1:
			case 2:
			case 3:
				DWORD dwWriteValue;//[28...31] 0x80000001 ...
				dwWriteValue = pInBuf->AdvFeature.StartupMemCh.Number|0x80000000;
				if (WriteQuadlet(0xf2f00828,dwWriteValue) != STATUS_OK)
				{
					return STATUS_HW_IO_ERROR;
				}
				status = STATUS_OK ;
				break;
			default:
				status = STATUS_PARAMETER_INVALID;
				break;
			}
			
		}
		break;

	default:
	    break;
	}
	
	return status;
}

HVSTATUS CCamera::f_SET_PIO_OUTPUT_PORT(void *pInBuffer)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	HV_ARG_SET_PIO_OUTPUT_PORT *pInBuf =(HV_ARG_SET_PIO_OUTPUT_PORT *)pInBuffer;
	HV_INTERFACE1_CONTEXT ctx;	
	switch(pInBuf->PortId)
	{
	case PIO_OUTPUT_PORT0:
		{
			switch(pInBuf->PortFuncId)
			{
			case PIO_OUTPUT_PORT_FUNC_ID_SOURCE_SELECT:
				ctx.dwVal = pInBuf->OutputPort.SourceSelect.Value;
				status = SetOutPutIO_0(&ctx);
				break;
			case PIO_OUTPUT_PORT_FUNC_ID_USER_SET:
				ctx.dwVal = pInBuf->OutputPort.UserSet.Value;
				status = SetOutPutIO_0_POL(&ctx);
				break;
			default:
				status = STATUS_PARAMETER_INVALID;
				break;
			}
		} 
		break;
	case PIO_OUTPUT_PORT1:
		{
			switch(pInBuf->PortFuncId)
			{
			case PIO_OUTPUT_PORT_FUNC_ID_SOURCE_SELECT:
				ctx.dwVal = pInBuf->OutputPort.SourceSelect.Value;
				status = SetOutPutIO_1(&ctx);
				break;
			case PIO_OUTPUT_PORT_FUNC_ID_USER_SET:
				ctx.dwVal = pInBuf->OutputPort.UserSet.Value;
				status = SetOutPutIO_1_POL(&ctx);
				break;
			default:
				status = STATUS_PARAMETER_INVALID;
				break;
			}
		} 
		break;
	case PIO_OUTPUT_PORT2:
		{
			switch(pInBuf->PortFuncId)
			{
			case PIO_OUTPUT_PORT_FUNC_ID_SOURCE_SELECT:
				ctx.dwVal = pInBuf->OutputPort.SourceSelect.Value;
				status = SetOutPutIO_2(&ctx);
				break;
			case PIO_OUTPUT_PORT_FUNC_ID_USER_SET:
				ctx.dwVal = pInBuf->OutputPort.UserSet.Value;
				status = SetOutPutIO_2_POL(&ctx);
				break;
			default:
				status = STATUS_PARAMETER_INVALID;
				break;
			}
		} 
		break;  
	default:
		status = STATUS_PARAMETER_INVALID;
		break;
	}
	return status;
}
HVSTATUS CCamera::f_SET_VIDEO_FORMAT_MODE_FRAME_RATE(void *pInBuffer)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE; 
	HV_ARG_SET_VIDEO_FORMAT_MODE_FRAME_RATE *pInBuf =(HV_ARG_SET_VIDEO_FORMAT_MODE_FRAME_RATE *)pInBuffer;	
	if( VIDEO_FORMAT7 == pInBuf->VideoFormat  && VIDEO_MODE0 == pInBuf->VideoMode )
		status = STATUS_OK;
	else
		status = STATUS_PARAMETER_INVALID;

	return status;
}
HVSTATUS CCamera::f_SET_FMT7_MODE(void *pInBuffer)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE; 
	HV_ARG_SET_FMT7_MODE *pArgSetAOI =(HV_ARG_SET_FMT7_MODE *)pInBuffer;	
	switch(pArgSetAOI->FuncId)
	{
	case FMT7_MODE_FUNC_ID_BAND_WIDTH:
		m_Format7.SetBytesPerPacket(pArgSetAOI->Fmt7Mode.BandWidth.PacketSize);	 
		status = STATUS_OK;
		break;
	case FMT7_MODE_FUNC_ID_AOI:
		m_Format7.SetSize(pArgSetAOI->Fmt7Mode.AOI.Width, pArgSetAOI->Fmt7Mode.AOI.Height);
		m_Format7.SetPosition(pArgSetAOI->Fmt7Mode.AOI.Left, pArgSetAOI->Fmt7Mode.AOI.Top);
		status = STATUS_OK;
		break;
	case FMT7_MODE_FUNC_ID_COLOR_CODING:
		{
			HV_INTERFACE1_CONTEXT ctx;
			ctx.dwVal = pArgSetAOI->Fmt7Mode.ColorMode.Code;
			status = Raw8or12(&ctx); 
		}
		break;
	}  
	return status;
}
HVSTATUS CCamera::f_GET_STRING(void *pInBuffer,void *pOutBuffer)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	HV_ARG_GET_STRING* pInBuf = (HV_ARG_GET_STRING*)pInBuffer;
	HV_RES_GET_STRING* pOutBuf = (HV_RES_GET_STRING*)pOutBuffer;

	HV_STRING_ID ID = pInBuf->StringId;
	int nSize = pInBuf->nLength;
	char*pBuf = pOutBuf->pBuffer;
    int nTemp;
	
	switch(ID) {
	case STRING_ID_DEVICE_TYPE:
		status = local_Get_Device_Type(NULL,&nTemp);
		if (HV_SUCCESS(status))
		{
			if (nTemp == nSize)
			{
				status = local_Get_Device_Type(pBuf,&nSize);
			}
			else{
				status = STATUS_PARAMETER_INVALID;
			}
		}
		break;
	case STRING_ID_VENDOR_NAME:
		status = local_Get_Vender_Name(NULL,&nTemp);
		if (HV_SUCCESS(status))
		{
			if (nTemp == nSize)
			{
				status = local_Get_Vender_Name(pBuf,&nSize);
			}
			else{
				status = STATUS_PARAMETER_INVALID;
			}
		}
		break;
	case STRING_ID_DEVICE_SERIESNUM:
		status = local_Get_SN(NULL,&nTemp);
		if (HV_SUCCESS(status))
		{
			if (nTemp == nSize)
			{
				status = local_Get_SN(pBuf,&nSize);
			}
			else{
				status = STATUS_PARAMETER_INVALID;
			}
		}
		break;
	case STRING_ID_DEVICE_CHIPID:
		 status = local_Get_Sensor_ID(NULL,&nTemp);
		 if (HV_SUCCESS(status))
		 {
			 if (nTemp == nSize)
			 {
				 status = local_Get_Sensor_ID(pBuf,&nSize);
			 }
			 else{
				 status = STATUS_PARAMETER_INVALID;
			 }
		 }
		 
		break;
	case STRING_ID_DEVICE_NAME:
		status = local_Get_Device_Name(NULL,&nTemp);
		if (HV_SUCCESS(status))
		{
			if (nTemp == nSize)
			{
				status = local_Get_Device_Name(pBuf,&nSize);
			}
			else{
				status = STATUS_PARAMETER_INVALID;
			}
		}
		break;
	case STRING_ID_DEVICE_DRIVER_VERSION:
		status = local_Get_Device_Drv_Ver(NULL,&nTemp);
		if (HV_SUCCESS(status))
		{
			if (nTemp == nSize)
			{
				status = local_Get_Device_Drv_Ver(pBuf,&nSize);
			}
			else{
				status = STATUS_PARAMETER_INVALID;
			}
		}
		break;
	case STRING_ID_SYS_DRIVER_VERSION:
		status = local_Get_Sys_Ver(NULL,&nTemp);
		if (HV_SUCCESS(status))
		{
			if (nTemp == nSize)
			{
				status = local_Get_Sys_Ver(pBuf,&nSize);
			}
			else{
				status = STATUS_PARAMETER_INVALID;
			}
		}
		
		break;
	case STRING_ID_FIRMWARE_VERSION:
		status= local_Get_Hardware_Ver(NULL,&nTemp); 
		if (HV_SUCCESS(status))
		{
			if (nTemp == nSize)
			{
				status = local_Get_Hardware_Ver(pBuf,&nSize);
			}
			else{
				status = STATUS_PARAMETER_INVALID;
			}
		}
		break;
	case STRING_ID_FPGA_VERSION:
		status = local_Get_FPGA_Ver(NULL,&nTemp);
		if (HV_SUCCESS(status))
		{
			if (nTemp == nSize)
			{
				status = local_Get_FPGA_Ver(pBuf,&nSize);
			}
			else{
				status = STATUS_PARAMETER_INVALID;
			}
		}
		break;
	default:
		break;
	}
	
	return status;
}

HVSTATUS CCamera::f_GET_FEATURE(void *pInBuffer,void *pOutBuffer)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	
	return status;
}

HVSTATUS CCamera::f_GET_ADV_FEATURE(void *pInBuffer,void *pOutBuffer)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	
	return status;
}

HVSTATUS CCamera::Get_Unique_ID(TCHAR* szDeviceName, PLARGE_INTEGER pliUniqueID)
{
	HANDLE hDevice;
	DWORD dwRet,bytesRet;
	
	HVSTATUS status=STATUS_OK;
	hDevice = OpenDevice(szDeviceName,FALSE);
	if(hDevice != INVALID_HANDLE_VALUE) {
        dwRet = DeviceIoControl( hDevice,
								 IOCTL_GET_CAMERA_UNIQUE_ID,
								 NULL,
								 0,
                                 pliUniqueID,
                                 sizeof(LARGE_INTEGER),
                                 &bytesRet,
                                 NULL
                                 );
		if(!dwRet) {
			dwRet = GetLastError();
			status = STATUS_INTERNAL_ERROR;
		} else {
			dwRet = 0;
			status = STATUS_OK;
		}
		CloseHandle(hDevice);
	} else {
		dwRet = GetLastError();
		status = STATUS_NO_DEVICE_FOUND;
	}

	return status;
}

HVSTATUS CCamera::Get_SYS_Version(char* pSzDrvVer)
{
	HANDLE hDevice;
	DWORD bytesRet,dwRet;
    LARGE_INTEGER DV;
	
	HVSTATUS status=STATUS_OK;
	hDevice = OpenDevice(m_pName,FALSE);
	if(hDevice != INVALID_HANDLE_VALUE) {
        dwRet = DeviceIoControl( hDevice,
								 IOCTL_GET_DRIVE_VERSION,
								 NULL,
								 0,
                                 &DV,
                                 sizeof(LARGE_INTEGER),
                                 &bytesRet,
                                 NULL
                                 );
		if(!dwRet) {
			dwRet = GetLastError();
			status = STATUS_INTERNAL_ERROR;
		} else {
			dwRet = 0;
			DWORD dwH = DV.HighPart;
			DWORD dwL = DV.LowPart;
			DWORD w1 = (dwH>>16);
			WORD w2 = (WORD)dwH;
			DWORD w3 = dwL>>16;
			WORD w4 = (WORD)dwL;				
			sprintf(pSzDrvVer,"%d.%d.%d.%d",w1,w2,w3,w4);
			status = STATUS_OK;
		}
		CloseHandle(hDevice);
	} else {
		dwRet = GetLastError();
		status = STATUS_NO_DEVICE_FOUND;
	}

	return status;
}
 

BOOL CCamera::InquireVideoRates()
{
	ULONG format, mode, value;
	DWORD dwRet;

    //
	// NOTE! only modes 0-5 have a concept of framerate
	// 6 has a concept of revisions, which fits within the videoflags structure
	// *** the driver set currently has no support for the stored image format (6)
	// 7 apparently has no concept of framerates, and has quadlet offsets where
	// the framerates usually go... who knows.
    //

	for (format = 0; format < 7; format++)
	{
		if (IsFormatAvailable(format)) {
			for (mode=0; mode<8; mode++)
			{
				if (IsModeAvailable(format, mode)) {
					// Inquire mode
					if (dwRet = ReadQuadlet(0x200+format*32+mode*4,&value)) {
						return FALSE;
					}

					if (!(value & 0xff000000)) {
                        _RPT0(_CRT_WARN, "【ERROR】InquireVideoFormats()");
                        return FALSE;
					}

                    m_dwRateMask[format][mode] = value;
				}
			}
		}
	}

	return TRUE;
}

DWORD CCamera::_Snap_Shot_Fun(BYTE** ppBuffer, int num)
{
    DWORD dwRet = 0;
    DWORD dwBytesRet = 0;				
	
	PQBUFFER pBuf;
    int nTotalNum = num;
	
    assert(num != 0);
    m_CamStatus = SingleGrabing;
	
	pBuf = m_pCurBuf->pNextBuffer;
    
	if (nTotalNum == 1) {
        OVERLAPPED ListenOverlapped = { 0 };
        ISOCH_LISTEN isochListen = { 0 };
        BOOL retval;
		
        ListenOverlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        isochListen.hResource = m_hResource;
        isochListen.fulFlags = 0;
        isochListen.StartTime.CL_SecondCount = 0;
        isochListen.StartTime.CL_CycleCount = 0;
        isochListen.StartTime.CL_CycleOffset = 0;
        
        retval = DeviceIoControl( m_hMyCam,
            IOCTL_ISOCH_LISTEN,
            &isochListen,
            sizeof(ISOCH_LISTEN),
            NULL,
            0,
            &dwBytesRet,
            &ListenOverlapped
            );
        
        if ( !retval ) {
            dwRet = GetLastError();
            if(dwRet != ERROR_IO_PENDING) {
                return -1;
            } else {
                GetOverlappedResult(m_hMyCam,&ListenOverlapped,&dwBytesRet,TRUE);
            }
        }
        GetOverlappedResult(m_hMyCam,&ListenOverlapped,&dwBytesRet,TRUE);
        v_Start_Video_Stream();
        CloseHandle(ListenOverlapped.hEvent);
        GetOverlappedResult(m_hMyCam, 
            &pBuf->overLapped, 
            &dwBytesRet, 
            TRUE);
		
        memcpy(ppBuffer[0], pBuf->pFrameStart, m_Width*m_Height*m_n8or12);
    } else {
		v_Start_Video_Stream();
        while (num-- || m_CamStatus != SingleGrabing) {
			
			OVERLAPPED ListenOverlapped = { 0 };
			ISOCH_LISTEN isochListen = { 0 };
			BOOL retval;
			
			ListenOverlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			isochListen.hResource = m_hResource;
			isochListen.fulFlags = 0;
			isochListen.StartTime.CL_SecondCount = 0;
			isochListen.StartTime.CL_CycleCount = 0;
			isochListen.StartTime.CL_CycleOffset = 0;
			
			retval = DeviceIoControl( m_hMyCam,
				IOCTL_ISOCH_LISTEN,
				&isochListen,
				sizeof(ISOCH_LISTEN),
				NULL,
				0,
				&dwBytesRet,
				&ListenOverlapped
				);
			
			if ( !retval ) {
				dwRet = GetLastError();
				if(dwRet != ERROR_IO_PENDING) {
					return -1;
				} else {
					GetOverlappedResult(m_hMyCam,&ListenOverlapped,&dwBytesRet,TRUE);
				}
			}
			GetOverlappedResult(m_hMyCam,&ListenOverlapped,&dwBytesRet,TRUE);
			CloseHandle(ListenOverlapped.hEvent);
			
			GetOverlappedResult(m_hMyCam, 
				&pBuf->overLapped, 
				&dwBytesRet, 
				TRUE);
			int nIndex=nTotalNum - num - 1;
			_RPT1(_CRT_WARN, "Buf:%d",nIndex);
			memcpy(ppBuffer[nTotalNum - num - 1], pBuf->pFrameStart, m_Width*m_Height*m_n8or12);
			
			if (num > 0) {
				Enqueue(pBuf);
			}
			
		}
		
	}
	
    m_pCurBuf = pBuf;
    v_Stop_Snap_Shot();
	
    m_CamStatus = idle;
	
    _RPT0(_CRT_WARN, "【Grab Thread Over】");
    return 0;
}

int CCamera::v_Init_Snap(int nBuffers, int Flags)
{  
	m_nAttNumErr = 0;
	m_BroadNum=m_AttachNum; 
	PQBUFFER				pAcqBuffer;
	BOOL bRetVal;	
	DWORD							dwRet, dwBytesRet;
    ISOCH_LISTEN isochListen = { 0 };
    OVERLAPPED ListenOverlapped = { 0 };						
	int							i;
	int nRet = 0;

	if(nBuffers < 1)
	{
		return STATUS_INVALID_PARAMETER;
	}
    
	if(m_videoFormat != 7)
	{
		if(!IsRateAvailable(m_videoFormat, m_videoMode, m_videoFrameRate))
		{
			return STATUS_INVALID_COLORCODE;
		}
	}
	else
	{
		if(!IsFormatAvailable(7))
		{
			return STATUS_INVALID_FORMAT;
		}
		// Fishy to have to do this, but I think it's necessary
		_RPT0(_CRT_WARN, "[SVFX_DLL]:: -- Call: v_Update_Format7_Parameters ----------------------");
		v_Update_Format7_Parameters();
	}

	if(m_hMyCam != NULL)
	{
		return STATUS_BUSY;
	}

	if(!v_Init_Resources())
	{
		return STATUS_NO_RESOURCE;
	}


	//
	// allocate and set up the frame buffers
    //

	for(i=0; i<nBuffers; i++)
	{
		
        _RPT1(_CRT_WARN, "i = %d\n",i);

		// allocate the buffer header (stores data about a buffer)
		pAcqBuffer = (PQBUFFER) GlobalAlloc(LPTR,sizeof(QBUFFER));
		if(!pAcqBuffer)
		{
			nRet = ERROR_OUTOFMEMORY;
			goto EXIT;
		}

		// add it to our list of buffers
		if(i == 0)
		{
			m_pFirstBuffer = m_pCurBuf = pAcqBuffer;
		}
		else
		{
			m_pCurBuf->pNextBuffer = pAcqBuffer;
			m_pCurBuf = pAcqBuffer;
		}

		// allocate the actual frame buffer
		// the buffer passed to ATTACH_BUFFER must be aligned on a page boundary
		// thus, allocate an extra 4K and generate a pointer to the first page boundary
		pAcqBuffer->pDataBuf = (unsigned char *)GlobalAlloc(LPTR,m_maxBufferSize + 4096);
		if(!pAcqBuffer->pDataBuf)
		{
			nRet = ERROR_OUTOFMEMORY;
			goto EXIT;
		}

		// point pFrameStart at the first page boundary
		pAcqBuffer->pFrameStart = pAcqBuffer->pDataBuf + (4096 - (((int)(pAcqBuffer->pDataBuf)) & 0xfff));
        //pAcqBuffer->pFrameStart = pAcqBuffer->pDataBuf;
		// set the index (mostly for debugging purposes)
		pAcqBuffer->index = i;

		// give the overlapped structure an event

		ZeroMemory(&pAcqBuffer->overLapped, sizeof(pAcqBuffer->overLapped));
	
		pAcqBuffer->overLapped.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
		if(pAcqBuffer->overLapped.hEvent == INVALID_HANDLE_VALUE)
		{
			nRet = GetLastError();
			goto EXIT;
		}

		// allocate the AttachBuffers struct
		pAcqBuffer->pIsochAttachBuffers = (PISOCH_ATTACH_BUFFERS) GlobalAlloc(LPTR,sizeof(ISOCH_ATTACH_BUFFERS));
		if(!pAcqBuffer->pIsochAttachBuffers)
		{
			nRet = ERROR_OUTOFMEMORY;
			goto EXIT;
		}

        _RPT5(_CRT_WARN, "Link buffer: index:%d, size:%d, R3qpp:%d, R3length: %d, FrameStart:%08x\n",
            pAcqBuffer->index,
            pAcqBuffer->pIsochAttachBuffers->ulBufferSize,
            pAcqBuffer->pIsochAttachBuffers->R3_IsochDescriptor->nMaxBytesPerFrame,
            pAcqBuffer->pIsochAttachBuffers->R3_IsochDescriptor->ulLength,
            pAcqBuffer->pFrameStart);

		// fill in the AttachBuffers struct
		pAcqBuffer->pIsochAttachBuffers->hResource = m_hResource;
		pAcqBuffer->pIsochAttachBuffers->nNumberOfDescriptors = 1;
		pAcqBuffer->pIsochAttachBuffers->ulBufferSize = m_maxBufferSize;
		pAcqBuffer->pIsochAttachBuffers->R3_IsochDescriptor[0].fulFlags = DESCRIPTOR_SYNCH_ON_SY;
		pAcqBuffer->pIsochAttachBuffers->R3_IsochDescriptor[0].ulLength = m_maxBufferSize;
		pAcqBuffer->pIsochAttachBuffers->R3_IsochDescriptor[0].nMaxBytesPerFrame = m_maxBytes;
		pAcqBuffer->pIsochAttachBuffers->R3_IsochDescriptor[0].ulSynch = 1;
		pAcqBuffer->pIsochAttachBuffers->R3_IsochDescriptor[0].ulTag = 0;
		pAcqBuffer->pIsochAttachBuffers->R3_IsochDescriptor[0].CycleTime.CL_CycleOffset = 0;
		pAcqBuffer->pIsochAttachBuffers->R3_IsochDescriptor[0].CycleTime.CL_CycleCount = 0;
		pAcqBuffer->pIsochAttachBuffers->R3_IsochDescriptor[0].CycleTime.CL_SecondCount = 0;
		pAcqBuffer->pIsochAttachBuffers->R3_IsochDescriptor[0].bUseCallback = TRUE;
		pAcqBuffer->pIsochAttachBuffers->R3_IsochDescriptor[0].bAutoDetach = TRUE;
	}

	// all done making buffers
	// after here, we reference the buffers *exclusively* in thier linked-list form
	// point our last buffer around to the first
	m_pCurBuf->pNextBuffer = m_pFirstBuffer;

    /*
     *    |-> 0    ->   1    -> 2    -> 3    -> 5	->|
     *       Fir.                             Cur. & pAcqBuffer
     */

    //
	// Open camera for grab.
    //

	m_hMyCam = OpenDevice(m_pName, TRUE);

	if (m_hMyCam == INVALID_HANDLE_VALUE) {
		m_hMyCam = NULL;
		nRet = GetLastError();
		goto EXIT;
	}

    // Attach all buffers.
    pAcqBuffer = m_pFirstBuffer;

    do {
        _RPT5(_CRT_WARN, "Attaching buffer: index:%d, size:%d, R3qpp:%d, R3length: %d, FrameStart:%08x\n",
            pAcqBuffer->index,
            pAcqBuffer->pIsochAttachBuffers->ulBufferSize,
            pAcqBuffer->pIsochAttachBuffers->R3_IsochDescriptor->nMaxBytesPerFrame,
            pAcqBuffer->pIsochAttachBuffers->R3_IsochDescriptor->ulLength,
            pAcqBuffer->pFrameStart);

        bRetVal = DeviceIoControl(m_hMyCam, IOCTL_ATTACH_BUFFER,
                                  pAcqBuffer->pIsochAttachBuffers, 
                                  sizeof(ISOCH_ATTACH_BUFFERS),
                                  pAcqBuffer->pFrameStart,
                                  pAcqBuffer->pIsochAttachBuffers->ulBufferSize,
                                  &dwBytesRet, 
                                  &pAcqBuffer->overLapped
                                  );
		pAcqBuffer->indexAtch=m_AttachNum++;//ydTest...

        dwRet = GetLastError();
        if ((dwRet != ERROR_IO_PENDING) && 
            (dwRet != ERROR_SUCCESS)) {
            return -1;
        }

        pAcqBuffer = pAcqBuffer->pNextBuffer;

    } while (pAcqBuffer != m_pFirstBuffer);

    if (nBuffers > 1) {
        ListenOverlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

        isochListen.hResource = m_hResource;
        isochListen.fulFlags = 0;
        isochListen.StartTime.CL_SecondCount = 0;
        isochListen.StartTime.CL_CycleCount = 0;
        isochListen.StartTime.CL_CycleOffset = 0;

        bRetVal = DeviceIoControl(m_hMyCam,
                                  IOCTL_ISOCH_LISTEN,
                                  &isochListen, sizeof(ISOCH_LISTEN),
                                  NULL, 0,
                                  &dwBytesRet, &ListenOverlapped
                                  );

        if (!bRetVal) {
            dwRet = GetLastError();
            if (dwRet != ERROR_IO_PENDING) {
                return -1;
            } else {
                GetOverlappedResult(m_hMyCam, &ListenOverlapped, &dwBytesRet, TRUE);
            }
        }

        GetOverlappedResult(m_hMyCam, &ListenOverlapped, &dwBytesRet, TRUE);
        v_Start_Video_Stream();
        
        CloseHandle(ListenOverlapped.hEvent);
    }

	return 0;

EXIT:
    //
    // Some error occured, so deallocate all buffers.
    //

    m_pCurBuf = pAcqBuffer = m_pFirstBuffer;

    while (m_pCurBuf) {
        if (pAcqBuffer->overLapped.hEvent)
            CloseHandle(pAcqBuffer->overLapped.hEvent);

        if (pAcqBuffer->pDataBuf)
            GlobalFree(pAcqBuffer->pDataBuf);

        if (pAcqBuffer->pIsochAttachBuffers)
            GlobalFree(pAcqBuffer->pIsochAttachBuffers);

        m_pCurBuf = pAcqBuffer->pNextBuffer;
        GlobalFree(pAcqBuffer);
        pAcqBuffer = m_pCurBuf;

        if (m_pCurBuf == m_pFirstBuffer)
            m_pCurBuf = NULL;
    }
    
    v_Stop_Video_Stream();
    v_Free_Resources();

    m_pCurBuf = m_pFirstBuffer = NULL;
    
    _RPT1(_CRT_WARN, "[]:: CCamera.v_Init_Snap : ****Some error occured**** : nRet = %d\n",nRet);
    return nRet;
}


int CCamera::v_Stop_Snap_Shot()
{
    PQBUFFER pQStart;
    PQBUFFER pBuf;
    ISOCH_STOP isochStop;
    OVERLAPPED StopOverlapped = { 0 };
    BOOL bRetVal;
    DWORD dwRetVal = 0;
    DWORD dwBytesRet = 0;

    _RPT0(_CRT_WARN, "CCamera::StopImageAcquisition()");
	v_Stop_Video_Stream();

    pQStart = pBuf = m_pCurBuf->pNextBuffer;

    StopOverlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    isochStop.hResource = m_hResource;
    isochStop.fulFlags = 0;
    
    bRetVal = DeviceIoControl(m_hMyCam,
        IOCTL_ISOCH_STOP,
        &isochStop,
        sizeof(ISOCH_STOP),
        NULL,
        0,
        &dwBytesRet,
        &StopOverlapped
        );
    
    _RPT0(_CRT_WARN, "CCamera::StopImageAcquisition() - 01");

    if (!GetOverlappedResult(m_hMyCam,&StopOverlapped,&dwBytesRet,TRUE)) 
	{
        _ASSERT(0);
    }
    
    CloseHandle(StopOverlapped.hEvent);
    
    CloseHandle(m_hMyCam);
    m_hMyCam = NULL;
    
    _RPT0(_CRT_WARN, "CCamera::StopImageAcquisition() - 02");
    //
    // Free all allocate buffers.
    //
    pQStart = pBuf;
    do {
        // Free event.
        CloseHandle(pBuf->overLapped.hEvent);
        // Free frame buffer.
        GlobalFree(pBuf->pDataBuf);
        // Free Attach Buffers struct.
        GlobalFree(pBuf->pIsochAttachBuffers);
        // Save next buffer.
        m_pNextBuffer = pBuf->pNextBuffer;
        // Free the actual buffer.
        _RPT1(_CRT_WARN, "Free Buffer : %d", pBuf->index);
        GlobalFree(pBuf);
        
        pBuf = m_pNextBuffer;
    } while (pBuf != pQStart);
    
    _RPT0(_CRT_WARN, "CCamera::StopImageAcquisition() - 03");
    //
    // Free resources.
    //
    v_Free_Resources();
    _RPT0(_CRT_WARN, "CCamera::StopImageAcquisition() - 04");
    
    m_pNextBuffer = m_pCurBuf = m_pFirstBuffer = NULL;
    
    return TRUE;
}

int CCamera::Enqueue(PQBUFFER pQBuffer) 
{
    BOOL retval;
    DWORD dwBytesRet = 0;
    DWORD error = 0;


    ResetEvent(pQBuffer->overLapped.hEvent);
    retval = DeviceIoControl(
        m_hMyCam,
        IOCTL_ATTACH_BUFFER,
        pQBuffer->pIsochAttachBuffers,
        sizeof(ISOCH_ATTACH_BUFFERS),
        pQBuffer->pFrameStart,
        m_maxBufferSize,
        &dwBytesRet,
        &pQBuffer->overLapped
        );
    pQBuffer->indexAtch=m_AttachNum++;
	
    if (!retval) {
		error = GetLastError();
		if ((error != ERROR_IO_PENDING) && (error != ERROR_SUCCESS)) 
		{			
			m_nAttNumErr ++;
			_RPT3(_CRT_WARN, "IOCTL_ATTACH_BUFFER ** Error**: Buf->index= %d error=0x%x m_nNumErr=%d\n", 
				pQBuffer->index,error,m_nAttNumErr);
		}
		else
		{
			m_nAttNumErr = 0;	
		}
    }
	return error;
}


// these video stream functions should probably leave
// they are more one-liners that would pop up error boxes

void CCamera::v_Start_Video_Stream()
{
	if(WriteQuadlet(0x614,0x80000000) < 0)
	{
		OutputDebugString(TEXT("【ERROR】StartVideoStream()"));
	}
}

// Allocates the Isochronous resources necessary to start an isochronous
// streaming operation.
BOOL CCamera::v_Init_Resources()
{
    ISOCH_ALLOCATE_BANDWIDTH AllocBandwidth = { 0 };
	ISOCH_ALLOCATE_CHANNEL AllocChannel = { 0 };
	ISOCH_ALLOCATE_RESOURCES AllocResources = { 0 };
	ULONG ulRet,ulData;

	if (m_hMyCam) {
        // Camera is busy, stop image acquisition first
		return FALSE;
	} 
	m_hBandwidth = m_hResource = NULL;
	m_lChannel = -1;

    // Allocate Bandwidth
	AllocBandwidth.nMaxBytesPerFrameRequested = m_maxBytes;
	AllocBandwidth.fulSpeed = m_maxSpeed;
    ulRet = IsochAllocateBandwidth(m_pName, &AllocBandwidth);
    if (ulRet) {
        goto EXIT;
    }
	m_hBandwidth = AllocBandwidth.hBandwidth;

    // Allocate Channel
	AllocChannel.nRequestedChannel = -1;
	ulRet = IsochAllocateChannel(m_pName, &AllocChannel);
	if(ulRet) {
		goto EXIT;
	}
	m_lChannel = AllocChannel.Channel;

    // Allocate Resources
	AllocResources.fulSpeed = m_maxSpeed;
	// BUGCHECK!
	// OCHI compliant controllers are required RESOURCE_STRIP, check future? 
	AllocResources.fulFlags = // Listen Mode
        RESOURCE_USED_IN_LISTENING | RESOURCE_STRIP_ADDITIONAL_QUADLETS;
	AllocResources.nChannel = m_lChannel;
	AllocResources.nMaxBytesPerFrame = m_maxBytes;
	AllocResources.nNumberOfBuffers = 4;
	AllocResources.nMaxBufferSize = m_maxBufferSize;
	AllocResources.nQuadletsToStrip = 1;
	ulRet = IsochAllocateResources(m_pName, &AllocResources);
	if (ulRet) {
		goto EXIT;
	}
	m_hResource = AllocResources.hResource;

	// Set Channel Speed.
	ulData = (ULONG) m_lChannel;
	ulData <<= 4;
	ulData += (m_maxSpeed/2);
	ulData <<= 24;
	ulRet = WriteQuadlet(0x60c, ulData);
	if (ulRet) {
		goto EXIT;
	}

	return TRUE;

EXIT:
	// this is the common exit point for failed resource allocation
	// free them in reverse order

	if (m_hResource) {
		IsochFreeResources(m_pName,m_hResource);
		m_hResource = NULL;
	}

	if (m_lChannel != -1) {
		IsochFreeChannel(m_pName,m_lChannel);
		m_lChannel = -1;
	}

	if (m_hBandwidth) {
		IsochFreeBandwidth(m_pName,m_hBandwidth);
		m_hBandwidth = NULL;
	}

	return FALSE;
}

DWORD CCamera::_Display_Thread_Fun(void* pParameter)
{ 
    DWORD result;
    HANDLE hEvArray[] = {m_hEvNewFrame, m_hEvStopSnap, m_hEvTerminate};
    m_bStop_Disp = 1;

    HV_SNAP_INFO snapInfo;
    snapInfo.hhv = ToHandle(this);
    snapInfo.nIndex = 0;
    //snapInfo.nDevice = 1;
    snapInfo.pParam = m_pSnapParam;

    DWORD BufferSize = m_Width * m_Height;
    DWORD index = 0;    

    
    //
    // If doesn't ResetEvent, after restarting grab, the
    // DisplayThread will be close immediately.
    //
    ResetEvent(hEvArray[0]);
    ResetEvent(hEvArray[1]);
    ResetEvent(hEvArray[2]);
    
    while (m_bStop_Disp) {

        result = WaitForMultipleObjects(3, hEvArray, FALSE, INFINITE);
        switch (result) {
            case WAIT_OBJECT_0: // m_hEvNewFrame signals.
                EnterCriticalSection(&m_CritSect);
                assert(m_pNextBuffer != NULL);
                m_pCurBuf = m_pNextBuffer;
                m_pNextBuffer = NULL;
                LeaveCriticalSection(&m_CritSect);
                
                //
                // Here m_pNextBuffer = NULL, m_pCurrentBuffer != NULL.
                //

                m_pData = m_pCurBuf->pFrameStart;
                 
                //
                // Invoke user callback.
                //
				BufferSize = m_Width * m_Height*m_n8or12;// xdTEST.... 
		//		_RPT2(_CRT_WARN, ": %x    :   %x", m_ppSnapBuffer[index],m_pData);
                memcpy(m_ppSnapBuffer[index], m_pData, BufferSize);
				FillMemory(m_pData,BufferSize,0xff);//added by hyl 2006.12.26
				
				snapInfo.nIndex=index;//HYL 2006.9.1
					
                index++;
                if (index >= m_BufferNum) {
                    index = 0;
                }

                (*m_lpSnapProc)(&snapInfo);

                //EnterCriticalSection(&m_CritSect);
              				
				int error;
				error=Enqueue(m_pCurBuf);
				
                //LeaveCriticalSection(&m_CritSect);
                break;

            case WAIT_OBJECT_0 + 1:   // m_hEvStopSnap
            case WAIT_OBJECT_0 + 2:   // m_hEvTerminate
                goto ENDFLAG;

            default:
                DWORD retval;
                retval = GetLastError();
                assert(0);
                break;
        }
    }
    
ENDFLAG:
    SetEvent(m_hEvDisplayOver);
    _RPT0(_CRT_WARN, "_Display_Thread_Fun:: Exit.");
    return 0;
}

DWORD CCamera::_Acq_Thread_Fun(void* pParameter)
{
    DWORD dwRet = 0;
    DWORD dwBytesRet = 0;				
	BOOL bRetVal;
	PQBUFFER pBufWait;

	pBufWait = m_pCurBuf->pNextBuffer;
    m_pNextBuffer = NULL;

    while (m_CamStatus != idle) {

		if(m_nAttNumErr >= MAX_IMG_ERR)
		{
			_RPT0(_CRT_WARN, "****************  Acquisition_Thread_Proc ::  m_nNumErr >= MAX_IMG_ERR...\n\n");
			
		    m_bDeviceRemove=TRUE;
			break;
		}
		for (int i=0;i<BUFFER_NUM;i++)
		{
			if (pBufWait->indexAtch==m_BroadNum)
			{
				break;
			}
			else
			{
				pBufWait=pBufWait->pNextBuffer;
			}
			
		}
		m_BroadNum++;
		
		m_bBlockWait=TRUE;
		m_pBlockBuffer=pBufWait;  //to be waited buffer  2007.1.15 ls

		bRetVal = GetOverlappedResult(m_hMyCam, 
                                      &pBufWait->overLapped, 
                                      &dwBytesRet, 
                                      TRUE);
//		_RPT2(_CRT_WARN, "Wait Buffer %d %d", pBufWait->index,pBufWait->indexAtch);
		m_lAllNum++; 
		m_bBlockWait=FALSE;
		if(m_hBlockEvent) SetEvent(m_hBlockEvent);
		
//		_RPT0(_CRT_WARN, "xdtest  Acquisition_Thread_Proc::  GetOverlappedResult -- Get a buf.\n");
        if (m_CamStatus == idle)
		{		
			_RPT0(_CRT_WARN, "xdtest  ::  BREAK.\n");
            break;
		}
        if (!bRetVal) {
			_RPT0(_CRT_WARN, "xdtest  Acquisition_Thread_Proc ::  Some errors occured.\n");

            //
            // Some errors occured.
            //
            dwRet = GetLastError();
            if (dwRet == ERROR_SEM_TIMEOUT) {
                
				_RPT1(_CRT_WARN, "xdtest  Acquisition_Thread_Proc::  reattach pBufWait->index=%d.\n",pBufWait->index);
                //
                // If timeout, we reattach this buffer and process the
                // next buffer.
                //
                dwRet = 0;

                DeviceIoControl(
                    m_hMyCam,
                    IOCTL_ATTACH_BUFFER,
                    pBufWait->pIsochAttachBuffers,
                    sizeof(ISOCH_ATTACH_BUFFERS),
                    pBufWait->pFrameStart,
                    m_maxBufferSize,
                    &dwBytesRet,
                    &pBufWait->overLapped
                    );
                
                //pBuf = pBuf->pNextBuffer;
                continue;

            } else {
                //
                // If not timeout errors, we exit this thread.
                //
                
                DWORD error;
                error = GetLastError();
                _RPT1(_CRT_WARN, "_Acq_Thread_Fun :: error = 0x%x \n",error);
    
                return dwRet;

            }
        } 
		else 
		{
            EnterCriticalSection(&m_CritSect);
            if (m_pNextBuffer != NULL) {

				int error=Enqueue(m_pNextBuffer);
				m_lLostNum++;

                m_pNextBuffer = pBufWait;
                
            } 
			else 
			{
                //
                // Update m_pNextBuffer.
                //
                m_pNextBuffer = pBufWait;
			
				SetEvent(m_hEvNewFrame);
            } 

           LeaveCriticalSection(&m_CritSect);   
        }
    }
	
    SetEvent(m_hEvAcqOver);
    _RPT0(_CRT_WARN, "_Acq_Thread_Fun:: Exit.");
	return 0;
}

BOOL CCamera::InquireVideoModes()
{
	ULONG value, format;
	DWORD retval;

	for (format=0; format<8; format++)
	{
		if (IsFormatAvailable(format)) {
			// Inquire video mode for current format
			if (retval = ReadQuadlet(0x180+format*4, &value))
			{
				return FALSE;
			}

			if (!(value & 0xff000000)) {
                _RPT0(_CRT_WARN, "【ERROR】InquireVideoFormats()");
			}

            m_dwModeMask[format] = value;
		}
	}

	return TRUE;
}

/*
AsyncBlockRead：异步块读请求
address ：读取数据的偏移地址
pData   : 存取数据的缓冲区
nLength : 读取的长度单位为字节，必须为4的倍数

返回值：  成功：STATUS_OK
		  错误：STATUS_INVALID_CAMERA
				STATUS_CAMERA_IO_ERROR
*/
DWORD CCamera::AsyncBlockRead(DWORD address, DWORD *pData, int nLength)
{
	int nretries = 4;
	DWORD dwRet;

	if(!m_pName)
	{
		return STATUS_INVALID_CAMERA;
	}

	if(!pData)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return STATUS_CAMERA_IO_ERROR;
	}

	// we're gonna try this nretries times, looking for
	// ERROR_SEM_TIMEOUT, which maps to STATUS_IO_TIMEOUT
	// meaning that the camera can't keep up.
	while((dwRet = ReadBlock(m_pName,address,nLength,(unsigned char*)pData)) != 0 && nretries > 0)
	{
		if(dwRet != ERROR_SEM_TIMEOUT)
			// some other error, break out
			break;
		// Sleep for 10 ms
		Sleep(10);
		nretries--;
	}

	if(dwRet != 0)
	{
		return STATUS_CAMERA_IO_ERROR;
	}

	return STATUS_OK;
}

/*
AsyncBlockWrite：异步块写请求
address ：写数据的偏移地址
pData   : 存取数据的缓冲区
nLength : 写数据的长度，单位为字节，必须为4的倍数

返回值：  成功：STATUS_OK
		  错误：STATUS_INVALID_CAMERA
				STATUS_CAMERA_IO_ERROR
*/
DWORD CCamera::AsyncBlockWrite(DWORD address, DWORD *pData, int nLength)
{
	int nretries = 4;
	DWORD dwRet;
	int i = 0;

	if(!m_pName)
	{
		return STATUS_INVALID_CAMERA;
	}

	if(!pData)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return STATUS_CAMERA_IO_ERROR;
	}

	//Convert LE data format to LE data format
	for(i=0;i<nLength/4;i++)
	{
		pData[i] = BE2LE(pData[i]);
	}
	// we're gonna try this nretries times, looking for
	// ERROR_SEM_TIMEOUT, which maps to STATUS_IO_TIMEOUT
	// meaning that the camera can't keep up.
	while((dwRet = WriteBlock(m_pName,address,nLength,(unsigned char*)pData)) != 0 && nretries > 0)
	{
		if(dwRet != ERROR_SEM_TIMEOUT)
			// some other error, break out
			break;
		// Sleep for 10 ms
		Sleep(10);
		nretries--;
	}

	if(dwRet != 0)
	{
		return STATUS_CAMERA_IO_ERROR;
	}

	return STATUS_OK;
}

BOOL CCamera::IsFormatAvailable(int nFormat) {
    DWORD mask = 0x80000000;
    BOOL  retval = FALSE;

    mask >>= nFormat;
    retval = ((m_dwFormatMask & mask) != 0);

    return retval;
}
BOOL CCamera::IsModeAvailable(int nFormat, int nMode)
{
    DWORD mask = 0x80000000;
    BOOL  retval = FALSE;
    
    mask >>= nMode;
    retval = ((m_dwModeMask[nFormat] & mask) != 0);
    
    return retval;
}

BOOL CCamera::InquireVideoFormats()
{
    DWORD retval;
    
    // Inquire video formats
    if (retval = ReadQuadlet(0x100, &m_dwFormatMask)) {
        return FALSE;
    }
    
    if (!(m_dwFormatMask & 0xff000000)) {
        _RPT0(_CRT_WARN, "【ERROR】InquireVideoFormats()");
    }
    
    return TRUE;
}

/*
 * ReadQuadlet
 *
 * Public
 *
 * Essentially a wrapper for ReadRegisterUL
 *
 * Arguments:
 *  - address: the offset into the camera register space to read from
 *      Addresses leading with "f" as in 0xf0000344 will be treated as absolute addresses.
 *      Those without a leading "f" will be treated as offsets into the 
 *        CSR space, so 0x00000344 will be read at CSR + 0x344, usually 0xf0f00344
 *
 *  - pData: the place to put the data read in from the register.  The data will be in
 *      machine order, so the most significant bit would be 0x80000000
 *
 * Returns:
 *  - STATUS_OK
 *  - STATUS_CAMERA_IO_ERROR: something bad happened down in the bowels of the OS, use GetLastError() to find out.
 *  - STATUS_INVALID_CAMERA: no camera has been selected
 *
 * Comments:
 *  ReadQuadlet catches ERROR_SEM_TIMEOUT, which means the camera was too busy to process the request.
 *  It will retry the request for the initial value of nretries times, by default this is 4, but
 *  it may become a registry variable.
 */

HVSTATUS CCamera::ReadQuadlet(DWORD address, DWORD *pData)
{
	int nretries = RETRY_TIME;
	DWORD dwRet;
	
    assert(m_pName[0]);
    assert(pData);
	
	while((dwRet = ReadRegisterUL(m_pName,address,pData)) != 0 && nretries > 0)
	{
		if(dwRet != ERROR_SEM_TIMEOUT)
			break;
		
		Sleep(SLEEP_TIME);
		nretries--;
	}
	
	if(dwRet != 0) {
		return STATUS_HW_IO_ERROR;
	}
	
	return STATUS_OK;
}




/*
 * WriteQuadlet
 *
 * Public
 *
 * Essentially a wrapper for WriteRegisterUL
 *
 * Arguments:
 *  - address: the offset into the camera register space to write to
 *      Addresses leading with "f" as in 0xf0000344 will be treated as absolute addresses.
 *      Those without a leading "f" will be treated as offsets into the 
 *        CSR space, so 0x00000344 will be written at CSR + 0x344, usually 0xf0f00344
 *
 *  - data: the data write to the register
 *
 * Returns:
 *  - STATUS_OK
 *  - STATUS_CAMERA_IO_ERROR: something bad happened down in the bowels of the OS, use GetLastError() to find out.
 *  - STATUS_INVALID_CAMERA: no camera has been selected
 *
 * Comments:
 *  WriteQuadlet catches ERROR_SEM_TIMEOUT, which means the camera was too busy to process the request.
 *  It will retry the request for the initial value of nretries times, by default this is 4, but
 *  it may become a registry variable.
 */

HVSTATUS CCamera::WriteQuadlet(DWORD address, DWORD data)
{
	int retry = RETRY_TIME;
	DWORD dwRet;
    
    assert(m_pName[0]);

	while((dwRet = WriteRegisterUL(m_pName,address,data)) != 0 && retry > 0)
	{
		if(dwRet != ERROR_SEM_TIMEOUT)
			break;

		Sleep(SLEEP_TIME);
		retry--;
	}

	if(dwRet != 0) {
		return STATUS_HW_IO_ERROR;
	}

	return STATUS_OK;
}


/*
 * v_Free_Resources
 *
 * Private
 *
 * Frees the isochronous bandwidth, channel, and resource handle
 * that are allocated by InitResources
 *
 * Called by:
 *   StopImageCapture
 *   StopImageAcquisition
 */

BOOL CCamera::v_Free_Resources()
{
	ULONG ulRet;

    _RPT0(_CRT_WARN, "FreeResources->");
	if (ulRet = IsochFreeResources(m_pName, m_hResource)) {
		m_hResource = NULL;
		return FALSE;
	}
    _RPT0(_CRT_WARN, "IsochFreeResources");

	if (ulRet = IsochFreeChannel(m_pName, m_lChannel)) {
		m_lChannel = -1;
        return FALSE;
	}
    _RPT0(_CRT_WARN, "IsochFreeChannel");

	if (IsochFreeBandwidth(m_pName, m_hBandwidth)) {
		m_hBandwidth = NULL;
        return FALSE;
	}
    _RPT0(_CRT_WARN, "IsochFreeBandwidth");

	return TRUE;
}


void CCamera::v_Stop_Video_Stream()
{
	if (WriteQuadlet(0x614,0) != STATUS_OK) {
        _RPT0(_CRT_WARN, "StopVideoStream()");
	}
}

BOOL CCamera::OpenCamera(TCHAR* lpFullCameraName)
{
    CAMERA_INFO cell = { 0 };

    TCHAR oldCamera[256];

    if (m_pName != NULL) {
        lstrcpy(oldCamera, m_pName);
    }

    lstrcpy(m_pName, lpFullCameraName);
    _RPT0(_CRT_WARN, lpFullCameraName);

    GetCameraInfo(lpFullCameraName, &cell);
    m_dwCameraType = ModelName2Type(cell.ModelName);

	TCHAR *pCh = cell.ModelName;
	int t = UNKNOW_CAMERA;

	if(('d' == pCh[0] || 'D' == pCh[0]) && ('h' == pCh[1] || 'H' == pCh[1]))
	{//是大恒的标识
		if (cell.ModelName[3] == 'h' || cell.ModelName[3] == 'H') 
		{
			t = CMOS_CAMERA;
		} 
		else if (cell.ModelName[3] == 's' || cell.ModelName[3] == 'S')
		{
			t = CCD_CAMERA;
		}
	}
	else if(('a' == pCh[0] || 'A' == pCh[0]) 
		&& ('l' == pCh[1] || 'L' == pCh[1])
	//	&& ('d' == pCh[2] || 'D' == pCh[2])
		)
	{//是ALD的标识
		if (cell.ModelName[3] == 'h' || cell.ModelName[3] == 'H') 
		{
			t = CMOS_CAMERA;
		} 
		else if (cell.ModelName[3] == 's' || cell.ModelName[3] == 'S')
		{
			t = CCD_CAMERA;
		}
	}

	m_CameraType = t;
    return TRUE;
}

void CCamera::v_Reset_Camera_State()
{
	HANDLE hDevice;
	DWORD dwRet,bytesRet;

			
	_RPT0(_CRT_WARN, "--------------------v_Reset_Camera_State:  ...\n");
	hDevice = OpenDevice(m_pName, FALSE);
	if (hDevice != INVALID_HANDLE_VALUE) {
        dwRet = DeviceIoControl( hDevice,
                                 IOCTL_RESET_CMDR_STATE,
                                 NULL,
                                 0,
                                 NULL,
                                 0,
                                 &bytesRet,
                                 NULL
                                 );
		if (!dwRet) {
			_RPT1(_CRT_WARN, "ResetCameraState: IoCtl Failed: %d\n",dwRet);
            return;
		}

		CloseHandle(hDevice);
	} else {
		_RPT1(_CRT_WARN, "ResetCameraState: Couldn't open handle to %s\n",m_pName);
        return;
	}
}
 

DWORD CCamera::v_Stop_Snap() 
{
    _RPT0(_CRT_WARN, "\n\n----v_Stop_Snap:: -----------------------\n");

	v_Stop_Video_Stream();
    //
    // Close Display_Thread_Proc thread.
    //

    m_bStop_Disp = 0;
    SetEvent(m_hEvStopSnap);  
    DWORD dwRetVal = WaitForSingleObject(m_hEvDisplayOver, NEARLYINFINITE);
    if (dwRetVal != WAIT_OBJECT_0) {
        DWORD dwExitCode = 0; 
        GetExitCodeThread(m_hDisplayThread, &dwExitCode);
        TerminateThread(m_hDisplayThread, dwExitCode);
        _RPT1(_CRT_WARN, "v_Stop_Snap::【 Wrong - m_hEvDisplayOver 】Error Code = %d\n", GetLastError());
    } 
    _RPT0(_CRT_WARN, "--v_Stop_Snap--Display_Thread_Proc exit OK----");

    //
    // Close _Acq_Thread_Fun thread.
    //
    _RPT0(_CRT_WARN, " to Close _Acq_Thread_Fun thread ...");
    m_CamStatus = idle;
	v_Reset_Camera_State();

    dwRetVal = WaitForSingleObject(m_hEvAcqOver, NEARLYINFINITE);
    if (dwRetVal != WAIT_OBJECT_0)
    {
        DWORD dwExitCode = 0;
        GetExitCodeThread(m_hGrabThread, &dwExitCode);
        TerminateThread(m_hGrabThread, dwExitCode);
        _RPT1(_CRT_WARN, "v_Stop_Snap::  m_hEvAcqOver Error Code = %d\n", GetLastError());
    }
    
    _RPT0(_CRT_WARN, "v_Stop_Snap:: _Acq_Thread_Fun exit OK.=============\n");
	
	CloseHandle(m_hMyCam);
	m_hMyCam = NULL;
		

	//
	// Free all allocate buffers.
	//
    PQBUFFER pQStart;
    PQBUFFER pBuf;
	pQStart = pBuf = m_pCurBuf->pNextBuffer;
	do {
		// Free event.
		CloseHandle(pBuf->overLapped.hEvent);
		// Free frame buffer.
		GlobalFree(pBuf->pDataBuf);
		// Free Attach Buffers struct.
		GlobalFree(pBuf->pIsochAttachBuffers);
		// Save next buffer.
		m_pNextBuffer = pBuf->pNextBuffer;
		// Free the actual buffer.
		_RPT1(_CRT_WARN, "Free Buffer : %d", pBuf->index);
		GlobalFree(pBuf);
		
		pBuf = m_pNextBuffer;
	} while (pBuf != pQStart);
	
    _RPT0(_CRT_WARN, "=================v_Stop_Snap::=============\n");
	return 0;
}


BOOL CCamera::CheckPassword()
{
    if (m_CameraType == CCD_CAMERA) {
        time_t a_time;
        DWORD dwTemp = 0;
        WORD wCrc16 = 0;
        DWORD dwToWrite[2];
        int i = 0,j = 0;
        int nretries = 5;
        
        time(&a_time);
        
        dwToWrite[0] = 0x514B5800L;
        dwToWrite[0] |=	((DWORD)a_time) & 0x000000FFL;		//QiaoJH,2005.11.9
        wCrc16 = CSR_CRC16(dwToWrite,1);
        
        //QiaoJH,2005.11.9
        //dwToWrite[1] = (DWORD)MakePass(wCrc16);
        DWORD CRC16 = (DWORD)MAKEPASS(wCrc16);
        DWORD dwTimeOut = 0;
        dwToWrite[1] = (CRC16<<16) | 0x0000F000L | dwTimeOut;
        //end //QiaoJH,2005.11.9
        
        for(j=0; j<nretries; j++)//2005.9.16,有时函数AsyncBlockWrite()会失败	=>循环5次
        {
            if(AsyncBlockWrite(PRODUCT_ENCRYPT,dwToWrite,8) == STATUS_OK)
            {
                Sleep(100);
                for(i=0;i<nretries;i++)//2005.9.16,有时函数ReadQuadlet()会失败	=>循环5次
                {
                    //if(lpHV->pCam->ReadQuadlet(PRODUCT_ENCRYPT_STATE,&dwTemp) == CAM_SUCCESS)
                    if(ReadQuadlet(PRODUCT_ENCRYPT,&dwTemp) == STATUS_OK)
                    {
                        if(dwTemp == (0xFFFFFFFFL))		//LE: should be:0x100
                            return FALSE;
                        else
                            return TRUE;
                    }
                    
                    Sleep(20);	//if error on ReadQuadlet(),then retry again after 20ms.
                }
            }
            
            Sleep(20);	//if error on AsyncBlockWrite(),then retry again after 20ms.
        }
    } 
    return FALSE;
}

int CCamera::LoadProfileSettings()
{
	LoadFormat7Settings();

    return 1;
}

/*
 * StatusControlRegisters
 *
 * No Args, No Return Value
 *
 * Probes the Feature_Hi_Inq and Feature_Lo_Inq Registers
 * to find out what features are available, then probes the
 * Status registers of each control that is available
 */
void CCamera::StatusControlRegisters()
{
	DWORD foo;

	// Feature_Hi_Inq
	ReadQuadlet(0x404, &foo);

	if(foo & 0x80000000)
		m_Brightness.GetStatusRegister();
	if(foo & 0x40000000)
		m_AutoExposure.GetStatusRegister();
	if(foo & 0x20000000)
		m_Sharpness.GetStatusRegister();
	if(foo & 0x10000000)
		m_WhiteBalance.GetStatusRegister();
	if(foo & 0x08000000)
		m_Hue.GetStatusRegister();
	if(foo & 0x04000000)
		m_Saturation.GetStatusRegister();
	if(foo & 0x02000000)
		m_Gamma.GetStatusRegister();
	if(foo & 0x01000000)
		m_Shutter.GetStatusRegister();
	if(foo & 0x00800000)
		m_Gain.GetStatusRegister();
	if(foo & 0x00400000)
		m_Iris.GetStatusRegister();
	if(foo & 0x00200000)
		m_Focus.GetStatusRegister();
	if(foo & 0x00080000)
		m_Trigger.Get_Status();

	// Feature_Lo_Inq
	ReadQuadlet(0x408, &foo);
	
	if(foo & 0x80000000)
		m_Zoom.GetStatusRegister();

	
	// Strobe Inq
	ReadQuadlet(STROBE_BASE_ADDR, &foo);
	if(foo & 0x80000000)
		m_Strobe.Get_Status_Reg();

}


/*
 * InquireControlRegisters
 *
 * No Args, No Return Value
 *
 * Probes the Feature_Hi_Inq and Feature_Lo_Inq Registers
 * to find out what features are available, then probes the
 * Inquiry registers of each control that is available
 */
void CCamera::InquireControlRegisters()
{
	DWORD foo;

	// Feature_Hi_Inq
	ReadQuadlet(0x404, &foo);

	if(foo & 0x80000000)
		m_Brightness.GetInquireRegister();
	if(foo & 0x40000000)
		m_AutoExposure.GetInquireRegister();
	if(foo & 0x20000000)
		m_Sharpness.GetInquireRegister();
	if(foo & 0x10000000)
		m_WhiteBalance.GetInquireRegister();
	if(foo & 0x08000000)
		m_Hue.GetInquireRegister();
	if(foo & 0x04000000)
		m_Saturation.GetInquireRegister();
	if(foo & 0x02000000)
		m_Gamma.GetInquireRegister();
	if(foo & 0x01000000)
		m_Shutter.GetInquireRegister();
	if(foo & 0x00800000)
		m_Gain.GetInquireRegister();
	if(foo & 0x00400000)
		m_Iris.GetInquireRegister();
	if(foo & 0x00200000)
		m_Focus.GetInquireRegister();
	if(foo & 0x00080000)
		m_Trigger.Inquire();

	// Feature_Lo_Inq
	ReadQuadlet(0x408, &foo);
	
	if(foo & 0x80000000)
		m_Zoom.GetInquireRegister();

	ReadQuadlet(STROBE_BASE_ADDR, &foo);
	if(foo & 0x80000000)
		m_Strobe.Get_Inquire_Reg();
}



//
// Load Format7 Dialog Setting
//

int CCamera::LoadFormat7Settings()
{
	for (int format=0; format<3; format++) 
    {
        for (int mode=0; mode<8; mode++) 
        {
            for (int rate=0; rate<6; rate++) 
            {
                if (IsRateAvailable(format, mode, rate))
                {
                    SetVideoFormat(format);
                    SetVideoMode(mode);
				    SetVideoFrameRate(rate);
                }
            }
        }
    }

    if (IsFormatAvailable(7))
    {
        int j;
        m_Format7.Inquire();
        m_Format7.Status();
        SetVideoFormat(7);
        
        for (j=0; j<8; j++)
        {
            if (m_Format7.ModeSupported(j))
            {
                SetVideoMode(j);
                break;
            }
        }
    }

    m_Format7.SetSize(m_Format7.m_Width, m_Format7.m_Height);
    m_Format7.SetPosition(m_Format7.m_Left, m_Format7.m_Top);

	return 1;
}



void CCamera::SetGain(UINT value)
{
	if (m_Gain.m_value == value)
		return;
	m_Gain.SetValue(value, 0);
}

 

void CCamera::SetShutter(UINT value)
{
	if (m_Shutter.m_value == value)
		return;
	m_Shutter.SetValue(value, 0);
	m_uiShutterVal = value;
}

void CCamera::SetWhiteBalance(UINT u, UINT v)
{
	if ((m_WhiteBalance.m_ubvalue == u) && (m_WhiteBalance.m_value == v))
		return;
	m_WhiteBalance.SetValue(v, u);
}
/*
 * UpdateParameters
 *
 * Private
 *
 * Helper function that tweaks m_width, m_height, m_maxBytes, m_maxBufferSize
 * whenever the video settings change.
 *
 * Doing this right away seems just as shady as writing the registers right away
 * but we will leave it for now
 *
 * Called by SetVideo*
 */

void CCamera::v_Update_Format7_Parameters()
{
	if (m_videoFormat != 7)
	{
	} else {
		m_Format7.Status();
		m_maxBytes = m_Format7.m_BytePerPacket;
		m_Width = m_Format7.m_Width;
		m_Height = m_Format7.m_Height;
		m_maxBufferSize = m_Format7.m_BytePerPacket * m_Format7.m_PacketPerFrame;
	}
}

int CCamera::SetVideoMode(DWORD mode)
{
	DWORD dwRet;

	if (!m_pName)
	{
		return STATUS_INVALID_CAMERA;
	}

	if(m_hMyCam)
	{
		return STATUS_BUSY;
	}
		
	if (mode<=7)
	{
        if (!IsModeAvailable(m_videoFormat, mode))
		{
			return STATUS_INVALID_MODE;
		}

		if(dwRet = WriteQuadlet(0x604, mode << 29))
		{
			return STATUS_CAMERA_IO_ERROR;
		}

		m_videoMode = mode;
		v_Update_Format7_Parameters();
	} 
	else 
	{
		return STATUS_INVALID_PARAMETER;
	}

	return STATUS_OK;
}

int CCamera::SetVideoFormat(DWORD format)
{
	DWORD dwRet;

	_RPT1(_CRT_WARN, "ENTER SetVideoFormat (%d)\n",format);

	if (!m_pName)
	{
		return STATUS_INVALID_CAMERA;
	}

	if(m_hMyCam)
	{
        return STATUS_BUSY;
	}
	
	if (format <= 7)
	{
		if(!IsFormatAvailable(format))
		{
			// the desired format is not supported
			_RPT1(_CRT_WARN,"【SetVideoFormat】: Format %d not supported\n",format);
			return STATUS_INVALID_FORMAT;
		}

		// shift it over into the most significant bits
		if(dwRet = WriteQuadlet(0x608, format << 29))
		{
			_RPT1(_CRT_WARN,"【SetVideoFormat】: error %08x on WriteRegister\n",dwRet);
			return STATUS_CAMERA_IO_ERROR;
		}

		m_videoFormat = format;

		v_Update_Format7_Parameters();
	} 
	else 
	{
		_RPT1(_CRT_WARN,"【SetVideoFormat】: format %d out of range\n",format);
		return STATUS_INVALID_PARAMETER;
	}

	if (format == 7)
		m_Format7.Status();

	_RPT1(_CRT_WARN,"EXIT SetVideoFormat (%d)\n",format);
	return STATUS_OK;
}

int CCamera::SetVideoFrameRate(DWORD rate)
{
	DWORD dwRet;
	
	if (!m_pName)
	{
		return STATUS_INVALID_CAMERA;
	}

	if(m_hMyCam)
	{
		return STATUS_BUSY;
	}

	if(m_videoMode != 7)
	{
		if (rate <= 5)
		{
			if(!IsRateAvailable(m_videoFormat, m_videoMode, rate))
			{
				return STATUS_INVALID_FRAMERATE;
			}

			if(dwRet = WriteQuadlet(0x600, rate << 29))
			{
				return STATUS_CAMERA_IO_ERROR;
			}

			m_videoFrameRate = rate;

			v_Update_Format7_Parameters();					
		} 
		else 
		{
			return STATUS_INVALID_PARAMETER;
		}
	} 

	return STATUS_OK;

}
BOOL CCamera::IsRateAvailable(int nFormat, int nMode, int nRate)
{
    DWORD mask = 0x80000000;
    BOOL  retval = FALSE;

    mask >>= nRate;
    retval = ((m_dwRateMask[nFormat][nMode] & mask) != 0);

    return retval;
}
HVSTATUS CCamera::TriggerShot()
{
	VerifyRetVal(m_Trigger.SetSource(1));
	
	if (WriteQuadlet(CCD_SOFT_TRIGGER, 0x80000000L) != STATUS_OK)
		return STATUS_HW_IO_ERROR;
	
	Sleep(2);
//	VerifyRetVal(m_Trigger.SetSource(0));  
//------------------------------------------------
    HV_INTERFACE1_CONTEXT pInfcectx;
	pInfcectx.dwVal=m_Trigger_Source;
	pInfcectx.ID=TRIGGER_SOURCE_IO;
	SetTriggerSource(&pInfcectx);     //写回软触发前的触发源
//------------------------------------------------	
	return STATUS_OK;

}

void CCamera::StartBusResetThread()
{
    // make sure we don't already have a thread...
    if (m_BusResetThreadParams) {
        _RPT0(_CRT_WARN, "bus reset thread already exists\r\n");
        return;
    }

    // allocate our bus reset thread params...
    m_BusResetThreadParams = (PBUS_RESET_THREAD_PARAMS)LocalAlloc(LPTR, sizeof(BUS_RESET_THREAD_PARAMS));
    ZeroMemory(m_BusResetThreadParams, sizeof(BUS_RESET_THREAD_PARAMS));

    _RPT1(_CRT_WARN, "BusResetThreadParams = 0x%x\r\n", m_BusResetThreadParams);

    _RPT0(_CRT_WARN, "Creating BusResetThread...\r\n");

    // sure do. create a bus reset thread...
    m_BusResetThreadParams->szDeviceName = m_pName;
    m_BusResetThreadParams->bKill = FALSE;
	m_BusResetThreadParams->pMyCam = this;

    m_BusResetThreadParams->hThread = CreateThread( NULL,
                                                  0,
                                                  BusResetThread,
                                                  (LPVOID)m_BusResetThreadParams,
                                                  0,
                                                  &m_BusResetThreadParams->ThreadId
                                                  );
}

DWORD CCamera::BusResetThread(LPVOID lpParameter)
{
	PBUS_RESET_THREAD_PARAMS    pThreadParams;
    HANDLE                      hDevice;
    DWORD                       dwRet, dwBytesRet;
    OVERLAPPED                  overLapped;

    pThreadParams = (PBUS_RESET_THREAD_PARAMS)lpParameter;

    _RPT0(_CRT_WARN,, "Enter BusResetThread\r\n");

    // try to open the device
    hDevice = OpenDevice(pThreadParams->szDeviceName, TRUE);

    _RPT1(_CRT_WARN, "hDevice = 0x%x\r\n", hDevice);

    // device opened, so let's do loopback
    if (hDevice != INVALID_HANDLE_VALUE) {

        overLapped.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );

        pThreadParams->hEvent = overLapped.hEvent;
		CCamera *pThis = pThreadParams->pMyCam;

        while (TRUE) {

            ResetEvent(overLapped.hEvent);

            DeviceIoControl( hDevice,
                             IOCTL_BUS_RESET_NOTIFY,
                             NULL,
                             0,
                             NULL,
                             0,
                             &dwBytesRet,
                             &overLapped
                             );

            dwRet = GetLastError();

			_RPT1(_CRT_WARN, "dwRet = GetLastError() = 0x%x\r\n", dwRet);
            // we should always return pending, if not, something's wrong...
            if (dwRet == ERROR_IO_PENDING) {

                dwRet = GetOverlappedResult(hDevice, &overLapped, &dwBytesRet, TRUE);
                        
				_RPT0(_CRT_WARN,"BusThreadProc: dwRet = GetOverlappedResult\r\n");

                if (!pThreadParams->bKill) {

                    if (dwRet) {

                        // bus reset!
                        _RPT0(_CRT_WARN,"BusThreadProc: BUS RESET!!!\r\n");

						//NotifyClients(NULL, NOTIFY_BUS_RESET); 
						if(pThis->m_lpNotifyFun){
							HV_NOTIFY_INFO ni;
							ni.nMsgType = 0;
							(*pThis->m_lpNotifyFun)(&ni);
						}
                    }
                    else {

                        dwRet = GetLastError();
                        _RPT1(_CRT_WARN,, "BusThreadProc: dwRet = GetLastError() = 0x%x\r\n",dwRet);
                        break;
                    }
                }
                else {

                    _RPT0(_CRT_WARN,"BusThreadProc: Cancelling thread...\r\n");

					CancelIo(hDevice);

                    break;
                }
            }
            else {
                _RPT0(_CRT_WARN,"BusThreadProc: IOCTL didn't return PENDING\r\n");
                break;
            }
        } // while

        // free up all resources
        CloseHandle(hDevice);
        CloseHandle(overLapped.hEvent);
    }

    _RPT0(_CRT_WARN,"Exit BusResetThread\r\n");

    // that's it, shut this thread down
    ExitThread(0);
    return(0);

}

void CCamera::StopBusResetThread()
{
    _RPT0(_CRT_WARN, "Enter StopBusResetThread\r\n");

    // see if the thread's running
    if (!m_BusResetThreadParams) {
        _RPT0(_CRT_WARN, "no thread to stop...\r\n");
        return;
    }

    // kill the bus reset thread
    m_BusResetThreadParams->bKill = TRUE;
    SetEvent(m_BusResetThreadParams->hEvent);

    // wait for the thread to complete
    WaitForSingleObject(m_BusResetThreadParams->hThread, INFINITE);

    LocalFree(m_BusResetThreadParams);
    m_BusResetThreadParams = NULL;

    _RPT0(_CRT_WARN, "Exit StopBusResetThread\r\n");
} 
HVSTATUS CCamera::Raw8or12(void *pInBuffer)
{
	DWORD dwWriteValue;
	HV_INTERFACE1_CONTEXT *pInfcectx=(HV_INTERFACE1_CONTEXT*)(pInBuffer);
    switch(pInfcectx->dwVal) {
	case 0:
		dwWriteValue=0x09000000;  //raw8       
		m_n8or12=1;
		break;
	case 1:
        dwWriteValue=0x0a000000;  //raw12
	    m_n8or12=2;
		break;
	default:
		break;
	}
	if(WriteQuadlet(CCD_8or12BitMode,dwWriteValue) != 0)
    {
        return STATUS_HW_IO_ERROR;
    } 
	
	return STATUS_OK;
}
HVSTATUS CCamera::SetOutPutIO_0(void *pInBuffer)
{
	DWORD dwWriteValue;
	HV_INTERFACE1_CONTEXT *pInfcectx=(HV_INTERFACE1_CONTEXT*)(pInBuffer);
	dwWriteValue=pInfcectx->dwVal;
	if(WriteQuadlet(CCD_OUTPUTIO_0,dwWriteValue) != 0)
    {
        return STATUS_HW_IO_ERROR;
    }
	return STATUS_OK;
}
HVSTATUS CCamera::SetOutPutIO_1(void *pInBuffer)
{
	DWORD dwWriteValue;
	HV_INTERFACE1_CONTEXT *pInfcectx=(HV_INTERFACE1_CONTEXT*)(pInBuffer);
	dwWriteValue=pInfcectx->dwVal;
	if(WriteQuadlet(CCD_OUTPUTIO_1,dwWriteValue) != 0)
    {
        return STATUS_HW_IO_ERROR;
    }
	return STATUS_OK;
}
HVSTATUS CCamera::SetOutPutIO_2(void *pInBuffer)
{
	DWORD dwWriteValue;
	HV_INTERFACE1_CONTEXT *pInfcectx=(HV_INTERFACE1_CONTEXT*)(pInBuffer);
	dwWriteValue=pInfcectx->dwVal;
	
	if(WriteQuadlet(CCD_OUTPUTIO_2,dwWriteValue) != 0)
    {
        return STATUS_HW_IO_ERROR;
    }
	return STATUS_OK;
}
HVSTATUS CCamera::SetOutPutIO_0_POL(void *pInBuffer)
{
	DWORD dwWriteValue;
	HV_INTERFACE1_CONTEXT *pInfcectx=(HV_INTERFACE1_CONTEXT*)(pInBuffer);
	
	switch(pInfcectx->dwVal) {
	case 0:
		dwWriteValue=0x00000010;  //低      
		break;
	case 1:
        dwWriteValue=0x00000011;  //高
		break;
	default:
		break;
	}
	
    if(WriteQuadlet(CCD_OUTPUTIO_CONTROL,dwWriteValue) != 0)
    {
        return STATUS_HW_IO_ERROR;
    }
	
  return STATUS_OK;
}
HVSTATUS CCamera::SetOutPutIO_1_POL(void *pInBuffer)
{
	DWORD dwWriteValue;
	HV_INTERFACE1_CONTEXT *pInfcectx=(HV_INTERFACE1_CONTEXT*)(pInBuffer);
	
	switch(pInfcectx->dwVal){
	case 0:
		dwWriteValue=0x00000020;  //低      
		break;
	case 1:
        dwWriteValue=0x00000022;  //高
		break;
	default:
		break;
	}
	
	if(WriteQuadlet(CCD_OUTPUTIO_CONTROL,dwWriteValue) != 0)
    {
        return STATUS_HW_IO_ERROR;
    }
	return STATUS_OK;
}
HVSTATUS CCamera::SetOutPutIO_2_POL(void *pInBuffer)
{
	DWORD dwWriteValue;
	HV_INTERFACE1_CONTEXT *pInfcectx=(HV_INTERFACE1_CONTEXT*)(pInBuffer);
	switch(pInfcectx->dwVal){
	case 0:
		dwWriteValue=0x00000040;  //低      2007.1.9
		break;
	case 1:
        dwWriteValue=0x00000044;  //高
		break;
	default:
		break;
	}
	
	if(WriteQuadlet(CCD_OUTPUTIO_CONTROL,dwWriteValue) != 0)
    {
        return STATUS_HW_IO_ERROR;
    }
	return STATUS_OK;
}
HVSTATUS CCamera::SetTriggerSource(void *pInBuffer)
{
	HV_INTERFACE1_CONTEXT *pInfcectx=(HV_INTERFACE1_CONTEXT*)(pInBuffer);
	DWORD dwReadValue,dwWriteValue;
	if (ReadQuadlet(TRIGGER_MODE, &dwReadValue) != STATUS_OK) {
        return STATUS_HW_IO_ERROR;
    }
	
	switch(pInfcectx->dwVal){
	case 0:
		dwWriteValue=(dwReadValue&0xFF0FFFFF);
		m_Trigger_Source=0;
		break;
	case 1:
        dwWriteValue=(dwReadValue&0xFF0FFFFF)|0x00200000;
		m_Trigger_Source=1;
		break;
	case 2:
		dwWriteValue=(dwReadValue&0xFF4FFFFF)|0x00400000;
		m_Trigger_Source=2;
		break;
		
	default:
		break;
	}
	
	if(WriteQuadlet(TRIGGER_MODE,dwWriteValue) != 0)
    {
        return STATUS_HW_IO_ERROR;
    }
	return STATUS_OK;

}
HVSTATUS CCamera::TriggerDelayOnOff(void *pInBuffer)
{
	HV_INTERFACE1_CONTEXT *pInfcectx=(HV_INTERFACE1_CONTEXT*)(pInBuffer);
	DWORD dwWriteValue,dwReadValue;
	if (ReadQuadlet(CCD_DELAY_TIME_ADDR, &dwReadValue) != STATUS_OK) {
		return STATUS_HW_IO_ERROR;
		}

	switch(pInfcectx->dwVal){
	case 0:
		dwWriteValue=0x00000000|(dwReadValue&0xFDFFFFFF);
		break;
	case 1:
		dwWriteValue=0x02000000|(dwReadValue&0xFDFFFFFF);
		break;
		
	default:
		break;
	}
	
	if(WriteQuadlet(CCD_DELAY_TIME_ADDR,dwWriteValue) != 0)
    {
        return STATUS_HW_IO_ERROR;
    }
	return STATUS_OK;
}
HVSTATUS CCamera::TriggerDelayValue(void *pInBuffer)
{
	HV_INTERFACE1_CONTEXT *pInfcectx=(HV_INTERFACE1_CONTEXT*)(pInBuffer);
	DWORD dwReadValue,dwWriteValue=pInfcectx->dwVal;

	if (ReadQuadlet(CCD_DELAY_TIME_ADDR, &dwReadValue) != STATUS_OK) {
		   return STATUS_HW_IO_ERROR;
		}

    dwWriteValue=(dwReadValue&0xFFFFF000)|dwWriteValue; 
//	dwWriteValue=dwWriteValue|0x02000000;
	
	if(WriteQuadlet(CCD_DELAY_TIME_ADDR,dwWriteValue) != 0)   
    {
        return STATUS_HW_IO_ERROR;
    }
	return STATUS_OK;
}
HVSTATUS CCamera::TestImage(void *pInBuffer)
{
	DWORD dwWriteValue;
	HV_INTERFACE1_CONTEXT *pInfcectx=(HV_INTERFACE1_CONTEXT*)(pInBuffer);
	switch(pInfcectx->dwVal){
	case 0:
		dwWriteValue=0x00000000;
		break;
	case 1:
		dwWriteValue=0x02010000;
		break;
	case 2:
		dwWriteValue=0x02020000;
		break;
	case 3:
		dwWriteValue=0x02030000;
		break;
	case 4:
		dwWriteValue=0x02040000;
		break;
	case 5:
		dwWriteValue=0x02050000;
		break;
	case 6:
		dwWriteValue=0x02060000;
		break;
	case 7:
		dwWriteValue=0x02070000;
		break;
	default:
		break;
	}
	
	if(WriteQuadlet(CCD_TEST_IMG_ADDR,dwWriteValue) != 0)
    {
        return STATUS_HW_IO_ERROR;
    }
	return STATUS_OK;
}

HVSTATUS CCamera::TransfersDelayValue(DWORD dwV)
{
	DWORD dwWriteValue=dwV<<16;
	
	if(WriteQuadlet(CCD_TRANSFERSDELAY_ADDR,dwWriteValue) != 0)   
    {
        return STATUS_HW_IO_ERROR;
    }
	
	return STATUS_OK;
}
 

HVSTATUS CCamera::ShutterUnit(DWORD dwV)
{	 
	DWORD reg;
    ReadQuadlet(0xf2f00818,&reg);
	reg = reg & 0xFFF0FFFF;
	
	DWORD unit;	
	switch(dwV) {
	case SHUTTER_US:
		unit = 0;
		break;
	case SHUTTER_MS:
		unit = 1;
		break;
	case SHUTTER_S:
		unit = 2;
		break;
	default:
		break;
	}
	
	unit <<= 16;
	reg |= unit;
	WriteQuadlet(0xf2f00818,reg);
	
	return STATUS_OK;
}
HVSTATUS CCamera::FilterTimeUnit(DWORD dwV)
{
	DWORD dwInside;
	if(ReadQuadlet(CCD_FILTER_TIME_ADDR,&dwInside) != 0)   
    {
        return STATUS_HW_IO_ERROR;
    }
	DWORD dwT1 = dwInside & 0xFFFF0000;
	DWORD dwT2 = dwV?0x00008000:0x00000000;
	dwInside = dwT1 | dwT2;
	if(WriteQuadlet(CCD_FILTER_TIME_ADDR,dwInside) != 0)   
    {
        return STATUS_HW_IO_ERROR;
    }
	
	return STATUS_OK;
}
HVSTATUS CCamera::FilterTimeValue(DWORD dwV)
{
	DWORD dwWriteValue=dwV<<16;
	
	DWORD dwInside;
	if(ReadQuadlet(CCD_FILTER_TIME_ADDR,&dwInside) != 0)   
    {
        return STATUS_HW_IO_ERROR;
    }
	DWORD dwT1 = dwWriteValue & 0xFFFF0000;
	DWORD dwT2 = dwInside & 0x00008000;
	dwInside = dwT1 | dwT2;

	if(WriteQuadlet(CCD_FILTER_TIME_ADDR,dwInside) != 0)   
    {
        return STATUS_HW_IO_ERROR;
    }
	
	return STATUS_OK;
}
HVSTATUS CCamera::LE_BE_Mode(DWORD dwV)
{
	DWORD dwWriteValue = dwV?0x00000000:0x80000000;
	
	if(WriteQuadlet(CCD_LE_BE_MODE_ADDR,dwWriteValue) != 0)   
    {
        return STATUS_HW_IO_ERROR;
    }
	
	return STATUS_OK;
}  
 

//DESC_RESOLUTION
HVSTATUS CCamera::local_Get_Resolution(void *pContext,int *pSize)
{
	HVSTATUS status = STATUS_PARAMETER_INVALID;
	if(pContext && *pSize == 2*sizeof(DWORD))
	{
		DWORD dwAoi;
		if (ReadQuadlet(0xf1f00000, &dwAoi) != STATUS_OK) {
			return STATUS_HW_IO_ERROR;
		}
		
		((DWORD *)pContext)[0] = dwAoi>>16&0x0000ffff;
		((DWORD *)pContext)[1] = dwAoi&0x0000ffff;
		status = STATUS_OK;
	}
	else
	{
		if(pSize != NULL)
		{
			*pSize = 2*sizeof(DWORD);
			status = STATUS_OK;
		}
	}
	return status;	
}

//DESC_DEVICE_SERIESNUM
HVSTATUS CCamera::local_Get_SN(void *pContext,int *pSize)
{
	HVSTATUS status = STATUS_PARAMETER_INVALID;
	if(pContext && *pSize == 15)
	{
		LARGE_INTEGER UniqueID;
		
		status = Get_Unique_ID(m_pName, &UniqueID);
		DWORD dH = BE2LE(UniqueID.HighPart)&0xFF;
		DWORD dL = BE2LE(UniqueID.LowPart);
		
		LARGE_INTEGER qq;
		qq.HighPart = dH;
		qq.LowPart = dL;
		sprintf((char*)pContext,"%014I64o",qq);
	}
	else
	{
		
		if(pSize != NULL)
		{
			*pSize = 15;
			status = STATUS_OK;
		}

	}
	return status;	
}

//DESC_DEVICE_CHIPID
HVSTATUS CCamera::local_Get_Sensor_ID(void *pContext,int *pSize)
{
	HVSTATUS status = STATUS_PARAMETER_INVALID;
	if(pContext && *pSize == sizeof(WORD))
	{
		DWORD dwChipID;
		if(ReadQuadlet(CCD_SENSOR_ID,&dwChipID) != STATUS_OK)
		{
			status = STATUS_HW_IO_ERROR;
		}
		else
		{
			*((WORD*)pContext) = (WORD)dwChipID;
			status = STATUS_OK;
		}
	}
	else
	{
		if(pSize != NULL)
		{
			*pSize = sizeof(WORD);
			status = STATUS_OK;
		}
	}
	return status;	
}

//DESC_DEVICE_HARDWARE_VERSION
HVSTATUS CCamera::local_Get_Hardware_Ver(void *pContext,int *pSize)
{
	HVSTATUS status = STATUS_PARAMETER_INVALID;
	if(pContext && *pSize == sizeof(DWORD))
	{
		DWORD dwChipID;
		if(ReadQuadlet(CCD_FIRM_VERSION,&dwChipID) != STATUS_OK)
		{
			status = STATUS_HW_IO_ERROR;
		}
		else
		{
			*((DWORD*)pContext) = (DWORD)dwChipID;
			status = STATUS_OK;
		}
	}
	else
	{   
		if(pSize != NULL)
		{
			*pSize = sizeof(DWORD);
			status = STATUS_OK;
		}
	}
	return status;	
}

//DESC_DEVICE_NAME
HVSTATUS CCamera::local_Get_Device_Name(void *pContext,int *pSize)
{
	HVSTATUS status = STATUS_PARAMETER_INVALID;
	CAMERA_INFO cell = { 0 };
	GetCameraInfo(m_pName, &cell);
	int length = strlen(cell.ModelName);

	if(pContext && *pSize == length)
	{
		sprintf((char*)pContext,"%s",cell.ModelName);
		status = STATUS_OK;		
	}
	else if(pSize != NULL)
	{
		*pSize = length;
		status = STATUS_OK;
	}
	return status;	
}
 

HVSTATUS CCamera::local_Get_Sys_Ver(void *pContext,int *pSize)
{
	HVSTATUS status = STATUS_PARAMETER_INVALID;
	char szSysV[256] = {0}; 
	status = Get_SYS_Version(szSysV);
	if(status != STATUS_OK){
		strcpy(szSysV,"1.1.1.1"); //.sys version
		status = STATUS_OK;
	} 

	status = STATUS_PARAMETER_INVALID;
	if(pContext == NULL && pSize != NULL)
	{
		*pSize = strlen(szSysV);
		status = STATUS_OK;
	}
	else if(pContext)
	{
		sprintf((char*)pContext,"%s",szSysV);
		status = STATUS_OK;	
	}
	
	return status;	
}
HVSTATUS CCamera::local_Get_Device_Drv_Ver(void *pContext,int *pSize)
{
	HVSTATUS status = STATUS_PARAMETER_INVALID;
	char szDeviceDriverVer[256] = {0};  
	strcpy(szDeviceDriverVer,"2.0.0.1");//本层模块版本  

	status = STATUS_PARAMETER_INVALID;
	if(pContext == NULL && pSize != NULL)
	{
		*pSize = strlen(szDeviceDriverVer);
		status = STATUS_OK;
	}
	else if(pContext)
	{
		sprintf((char*)pContext,"%s",szDeviceDriverVer);
		status = STATUS_OK;	
	}
	
	return status;	
}
//DESC_DEVICE_FPGA_VERSION
HVSTATUS CCamera::local_Get_FPGA_Ver(void *pContext,int *pSize)
{
	HVSTATUS status = STATUS_PARAMETER_INVALID;
	if(pContext && *pSize == sizeof(DWORD))
	{
		DWORD dwFPGAVer;
		if(ReadQuadlet(CCD_FPGA_VERSION,&dwFPGAVer) != STATUS_OK)
		{
			status = STATUS_HW_IO_ERROR;
		}
		else
		{
			*((DWORD*)pContext) = (DWORD)dwFPGAVer;
			status = STATUS_OK;
		}
	}
	else
	{   
		if(pSize != NULL)
		{
			*pSize = sizeof(DWORD);
			status = STATUS_OK;
		}
	}
	return status;	
}

//DESC_DEVICE_VENDOR_NAME
HVSTATUS CCamera::local_Get_Vender_Name(void *pContext,int *pSize)
{
	HVSTATUS status = STATUS_PARAMETER_INVALID;
	CAMERA_INFO cell = { 0 };
	GetCameraInfo(m_pName, &cell);
	int length = strlen(cell.VendorName);

	if(pContext && *pSize == length)
	{
		sprintf((char*)pContext,"%s",cell.VendorName);
		status = STATUS_OK;		
	}
	else if(pSize != NULL)
	{
		*pSize = length;
		status = STATUS_OK;
	}
	return status;	
}

HVSTATUS CCamera::local_Get_Device_Type(void *pContext,int *pSize)
{
	HVSTATUS status = STATUS_PARAMETER_INVALID;
	if (pContext == NULL)
	{
		if(pSize != NULL)
		{
			*pSize = sizeof(HVTYPE);
			status = STATUS_OK;
		}
	}
	else
	{
		if (*pSize == sizeof(HVTYPE)) {
			*(HVTYPE *)pContext = _GetMyType(); 
			status = STATUS_OK;
		}
	}
	return status;	
}
 
//DESC_DEVICE_BAYER_LAYOUT
HVSTATUS CCamera::local_Get_Bayer_Layout(void *pContext,int *pSize)
{
	HVSTATUS status = STATUS_PARAMETER_INVALID;
	
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
			UCHAR Bayer[4];
			status = ReadQuadlet(FORMAT7_DCAM_ADDR+0x58,(DWORD*)Bayer);
			
			if(Bayer[3]<0||Bayer[3]>3) 
			{
				MessageBox(NULL, TEXT("RGB primary color filter unexpected!\n"), 
					TEXT("Error"), MB_OK|MB_ICONERROR);
				status = STATUS_INTERNAL_ERROR;
			}
			*(DWORD *)pContext = Bayer[3]?Bayer[3]:4;	//当dwV=0时,改为4; 1,2,3　不变				
		}
	}
	return status;	
}

HVSTATUS CCamera::f_QUERY_VIDEO_MODE_PRESENCE(HV_CONTROL_CODE code, 
											  void *pInBuffer, 
											  DWORD nInBufferSize, 
											  void *pOutBuffer, 
											  DWORD nOutBufferSize,
											  DWORD *pBytesReturned)
{
	HVSTATUS st = STATUS_NOT_SUPPORT_INTERFACE; 
	HV_ARG_QUERY_VIDEO_MODE_PRESENCE *pInBuf = (HV_ARG_QUERY_VIDEO_MODE_PRESENCE *)pInBuffer;
	 
	HV_RES_QUERY_VIDEO_MODE_PRESENCE *pOutBufMode = (HV_RES_QUERY_VIDEO_MODE_PRESENCE *)pOutBuffer;

	memcpy(pOutBufMode,&m_dwModeMask[pInBuf->VideoFormat],sizeof(DWORD));
	st = STATUS_OK;
	return st;
}

HVSTATUS CCamera::f_GET_FMT7_MODE( 
								  void *pInBuffer, 
								  DWORD nInBufferSize, 
								  void *pOutBuffer, 
								  DWORD nOutBufferSize,
								  DWORD *pBytesReturned)
{
	HVSTATUS st = STATUS_NOT_SUPPORT_INTERFACE;  
	HV_ARG_GET_FMT7_MODE *pInBuf = (HV_ARG_GET_FMT7_MODE*)pInBuffer;
	HV_RES_GET_FMT7_MODE *pOutBuf = (HV_RES_GET_FMT7_MODE*)pOutBuffer;
	switch( pInBuf->VideoMode)
	{
	case VIDEO_MODE0:
		{
			switch( pInBuf->FuncId){
			case FMT7_MODE_FUNC_ID_AOI:
				{
					DWORD dwReadSize,dwReadPos;
					if (ReadQuadlet(0xf1f0000c, &dwReadSize) != STATUS_OK) 
						return STATUS_HW_IO_ERROR;
					
					if (ReadQuadlet(0xf1f00008, &dwReadPos) != STATUS_OK) 
						return STATUS_HW_IO_ERROR;

					pOutBuf->AOI.Width = dwReadSize>>16&0x0000ffff;
					pOutBuf->AOI.Height = dwReadSize&0x0000ffff;
					pOutBuf->AOI.Left = dwReadPos>>16&0x0000ffff;
					pOutBuf->AOI.Top = dwReadPos&0x0000ffff;

					st = STATUS_OK;
                    
				}
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}

	 
	return st;
}

HVSTATUS CCamera::f_QUERY_FMT7_MODE_DESCRIPTOR( 
											  void *pInBuffer, 
											  DWORD nInBufferSize, 
											  void *pOutBuffer, 
											  DWORD nOutBufferSize,
											  DWORD *pBytesReturned)
{
	HVSTATUS st = STATUS_NOT_SUPPORT_INTERFACE;  
	HV_ARG_QUERY_FMT7_MODE_DESCRIPTOR *pInBuf = (HV_ARG_QUERY_FMT7_MODE_DESCRIPTOR*)pInBuffer;
	HV_RES_QUERY_FMT7_MODE_DESCRIPTOR *pOutBuf = (HV_RES_QUERY_FMT7_MODE_DESCRIPTOR*)pOutBuffer; 
		
	if (!m_Format7.ModeSupported(pInBuf->VideoMode))
	{
		st = STATUS_PARAMETER_INVALID;
		return st;
	}

	switch(pInBuf->FuncId)
	{
	case FMT7_MODE_FUNC_ID_COLOR_FILTER:
		{	
			UCHAR Bayer[4];
			st = ReadQuadlet(FORMAT7_DCAM_ADDR+0x58,(DWORD*)Bayer);
			
			if(Bayer[3]<0||Bayer[3]>3) 
			{
				MessageBox(NULL, TEXT("RGB primary color filter unexpected!\n"), 
					TEXT("Error"), MB_OK|MB_ICONERROR);
				st = STATUS_INTERNAL_ERROR;
			}
		//	*(DWORD *)pContext = Bayer[3]?Bayer[3]:4;	//当dwV=0时,改为4; 1,2,3　不变
			pOutBuf->ColorFilterMode.Filter = (HV_COLOR_FILTER)Bayer[3];
		}
		break;
	case  FMT7_MODE_FUNC_ID_AOI:
		{
			DWORD dwOffset = 0x00;
			DWORD dwStatus = 0x00;
			dwOffset = 4 * pInBuf->VideoMode;
			st = ReadQuadlet(IIDC_FORMAT7_OFFSET + dwOffset, &dwStatus);
			VerifyRetVal(st);
			
			dwOffset = 4*dwStatus + 0xf0000000;
			
			
			// Maximum dimensions at 0x00
			st =  ReadQuadlet(dwOffset + 0x00, &dwStatus);
			VerifyRetVal(st);
			
			int nVmax = (USHORT)(dwStatus & 0xffff);
			int nHmax = (USHORT)(dwStatus >> 16);
			
			
			// Size unit at 0x04
			st = ReadQuadlet(dwOffset + 0x04, &dwStatus);
			VerifyRetVal(st);
			
			int nVUnit = (USHORT)(dwStatus & 0xffff);
			int nHUnit = (USHORT)(dwStatus >> 16);

			pOutBuf->AOI.VerticalMaxInq = nVmax;
			pOutBuf->AOI.HorizontalMaxInq = nHmax;
			pOutBuf->AOI.VerticalUnitInq = nVUnit;
			pOutBuf->AOI.HorizontalUnitInq = nHUnit;
		}
		break;
	default:
		break;
	}	
	 

	return st;
}
HVSTATUS CCamera::ALL_Ctrl(HV_CONTROL_CODE code, 
				void *pInBuffer, DWORD nInBufferSize, 
				void *pOutBuffer, DWORD nOutBufferSize,
				DWORD *pBytesReturned)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	switch(code) {
	case ORD_CMD_RESERVED0:
		status = f_SET_PASSWORD(pInBuffer);
		break;
	case ORD_CMD_RESERVED1:
		status = f_CHECK_PASSWORD(pInBuffer);
		break;
	case ORD_CMD_RESERVED2:
		status = f_CLEAR_PASSWORD();
		break;
	case ORD_NOTIFY_REGESTER:
		status = f_NOTIFY_REGESTER(pInBuffer);
		break;
	case ORD_READ_QUADLET:
		{
			HV_ARG_READ_QUADLET *pInBuf = (HV_ARG_READ_QUADLET *)pInBuffer;
			HV_RES_READ_QUADLET *pOutBuf = (HV_RES_READ_QUADLET *)pOutBuffer;
			DWORD dwAddress = pInBuf->dwAddress|0xf0000000;//需求
			status = ReadQuadlet(dwAddress,&pOutBuf->dwData);
		}
		break;
	case ORD_WRITE_QUADLET:
		{
			HV_ARG_WRITE_QUADLET *pInBuf = (HV_ARG_WRITE_QUADLET *)pInBuffer;
			DWORD dwAddress = pInBuf->dwAddress|0xf0000000;//需求
			status = WriteQuadlet(dwAddress,pInBuf->dwData);
		}			 
		break;
	case ORD_READ_BLOCK:
		{
			HV_ARG_READ_BLOCK *pInBuf = (HV_ARG_READ_BLOCK *)pInBuffer; 
			HV_RES_READ_BLOCK *pOutBuf = (HV_RES_READ_BLOCK *)pOutBuffer;
			DWORD dwAddress = pInBuf->dwAddress|0xf0000000;//需求
			if(STATUS_OK != AsyncBlockRead(dwAddress,(DWORD *)pOutBuf->pData,pInBuf->nLength))
			{
				status = STATUS_HW_IO_ERROR;
			}
			else
				status = STATUS_OK;

		}
		break;
	case ORD_WRITE_BLOCK: 
		{
			HV_ARG_WRITE_BLOCK *argWriteBlock = (HV_ARG_WRITE_BLOCK *)pInBuffer;
			DWORD dwAddress = argWriteBlock->dwAddress|0xf0000000;//需求
			if(STATUS_OK != AsyncBlockWrite(dwAddress,(DWORD *)argWriteBlock->pData,argWriteBlock->nLength))
			{
				status = STATUS_HW_IO_ERROR;
			}
			else
				status = STATUS_OK;

		}
		break;
	case ORD_INIT_CAMERA:
		status = f_INIT_CAMERA(pInBuffer);
		break;
 
	case ORD_OPEN_SNAP:
		status = f_OPEN_SNAP(pInBuffer);
		break;
	case ORD_START_SNAP:
		status = f_START_SNAP(pInBuffer);
		break;
	case ORD_STOP_SNAP:
		status = f_STOP_SNAP();
		break;
	case ORD_CLOSE_SNAP:
		status = f_CLOSE_SNAP();
		break;
	case ORD_SNAP_SHOT:
		status = f_SNAP_SHOT(pInBuffer);
		break;
	
	case ORD_TRIGGER_SHOT:
		status = f_TRIGGER_SHOT();
		break;

	case ORD_GET_STRING:
		status = f_GET_STRING(pInBuffer,pOutBuffer);
		break;

	case ORD_SET_STROBE:
		status = f_SET_STROBE(pInBuffer);
		break;
      
	case ORD_SET_FEATURE:
		status = f_SET_FEATURE(pInBuffer);
		break;
		
	case ORD_GET_FEATURE:
		status = f_GET_FEATURE(pInBuffer,pOutBuffer);
		break;
	case ORD_SET_BASIC_FUNC:
		status = f_SET_BASIC_FUNC(pInBuffer);
		break;
		      
	case ORD_SET_ADV_FEATURE:
		status = f_SET_ADV_FEATURE(pInBuffer);
		break;
		
	case ORD_GET_ADV_FEATURE:
		status = f_GET_ADV_FEATURE(pInBuffer,pOutBuffer);
		break;
		      
	case ORD_SET_PIO_OUTPUT_PORT:
		status = f_SET_PIO_OUTPUT_PORT(pInBuffer);
		break;
	case ORD_SET_ADV_BASIC_FUNC:
		status = f_SET_ADV_BASIC_FUNC(pInBuffer);
		break;
	case ORD_SET_ADV_LUT:
		status = f_SET_ADV_LUT(pInBuffer);
		break;
	case ORD_QUERY_VIDEO_MODE_PRESENCE: 
		status = f_QUERY_VIDEO_MODE_PRESENCE(code,pInBuffer,
			nInBufferSize,pOutBuffer,
			nOutBufferSize,
			pBytesReturned);
		break;
	case ORD_QUERY_FMT7_MODE_DESCRIPTOR:
		status = f_QUERY_FMT7_MODE_DESCRIPTOR(
			pInBuffer,
			nInBufferSize,pOutBuffer,
			nOutBufferSize,
			pBytesReturned);
		break;
	case ORD_GET_FMT7_MODE:
		status = f_GET_FMT7_MODE(pInBuffer,
			nInBufferSize,pOutBuffer,
			nOutBufferSize,
			pBytesReturned);
		break;
	case ORD_SET_FMT7_MODE:
		status = f_SET_FMT7_MODE(pInBuffer);
		break;

	case ORD_SET_VIDEO_FORMAT_MODE_FRAME_RATE:
		status = f_SET_VIDEO_FORMAT_MODE_FRAME_RATE(pInBuffer);
		break;

	case ORD_QUERY_STRING_DESCRIPTOR:
		status = f_QUERY_STRING_DESCRIPTOR(code,pInBuffer,
			nInBufferSize,pOutBuffer,
			nOutBufferSize,
			pBytesReturned);
		break;

		 

	case ORD_QUERY_ADV_USER_CUSTOM_DESCRIPTOR:
		status = f_QUERY_ADV_USER_CUSTOM_DESCRIPTOR(code,pInBuffer,
			nInBufferSize,pOutBuffer,
			nOutBufferSize,
			pBytesReturned);
		break;
	case ORD_GET_ADV_USER_CUSTOM:
		status = f_GET_ADV_USER_CUSTOM(pInBuffer,nInBufferSize,pOutBuffer,nOutBufferSize);
		break;
	case ORD_SET_ADV_USER_CUSTOM:
		status = f_SET_ADV_USER_CUSTOM(pInBuffer,nInBufferSize);
		break;

	default:
		break;
	}
	return status;
}

HVSTATUS CCamera::f_QUERY_STRING_DESCRIPTOR(HV_CONTROL_CODE code, void *pInBuffer, DWORD nInBufferSize, void *pOutBuffer, DWORD nOutBufferSize, DWORD *pBytesReturned)
{
	HVSTATUS st = STATUS_NOT_SUPPORT_INTERFACE;
	
	HV_ARG_QUERY_STRING_DESCRIPTOR *pInBuf = (HV_ARG_QUERY_STRING_DESCRIPTOR*)pInBuffer;

	HV_RES_QUERY_STRING_DESCRIPTOR *pOutBuf = (HV_RES_QUERY_STRING_DESCRIPTOR*)pOutBuffer;

	int nTemp = 0;
	switch(pInBuf->StringId) {
	case STRING_ID_DEVICE_TYPE:
		st = local_Get_Device_Type(NULL,&nTemp); 
		break;
	case STRING_ID_VENDOR_NAME:
		st = local_Get_Vender_Name(NULL,&nTemp); 
		break;
	case STRING_ID_DEVICE_SERIESNUM:		
		st = local_Get_SN(NULL,&nTemp); 
		break;
	case STRING_ID_DEVICE_CHIPID:
		st = local_Get_Sensor_ID(NULL,&nTemp); 
		break;
	case STRING_ID_DEVICE_NAME:
		st = local_Get_Device_Name(NULL,&nTemp); 
		break;
	case STRING_ID_DEVICE_DRIVER_VERSION:
		st = local_Get_Device_Drv_Ver(NULL,&nTemp);
		break;
	case STRING_ID_SYS_DRIVER_VERSION:
		st = local_Get_Sys_Ver(NULL,&nTemp); 
		break;
	case STRING_ID_FIRMWARE_VERSION:
		st = local_Get_Hardware_Ver(NULL,&nTemp); 
		break;
	case STRING_ID_FPGA_VERSION:
		st = local_Get_FPGA_Ver(NULL,&nTemp); 
		break;
	default:
		break;
	}
	
	if(st == STATUS_OK) pOutBuf->dwTotalByteLength = nTemp;
	
	return st;

}

HVSTATUS CCamera::f_QUERY_ADV_USER_CUSTOM_DESCRIPTOR(HV_CONTROL_CODE code, void *pInBuffer, DWORD nInBufferSize, void *pOutBuffer, DWORD nOutBufferSize, DWORD *pBytesReturned)
{
	HVSTATUS st = STATUS_NOT_SUPPORT_INTERFACE;
	 

	HV_RES_QUERY_ADV_USER_CUSTOM_DESCRIPTOR *pOutBuf = (HV_RES_QUERY_ADV_USER_CUSTOM_DESCRIPTOR*)pOutBuffer;
 
	if(sizeof(HV_RES_QUERY_ADV_USER_CUSTOM_DESCRIPTOR)==nOutBufferSize)
	{
		
	//	st = ReadQuadlet(FORMAT7_DCAM_ADDR+0x58,(DWORD*)Bayer);

		st = STATUS_OK;
	}
	
	return st;

}

HVSTATUS CCamera::f_GET_ADV_USER_CUSTOM(void *pInBuffer, DWORD nInBufferSize, void *pOutBuffer, DWORD nOutBufferSize)
{
	HVSTATUS st = STATUS_NOT_SUPPORT_INTERFACE;
	if( (NULL==pInBuffer) ||
		(NULL==pOutBuffer) ||
		(sizeof(HV_ARG_GET_ADV_USER_CUSTOM)!=nInBufferSize) ||
		(sizeof(HV_RES_GET_ADV_USER_CUSTOM)!=nOutBufferSize))
	{
		st = STATUS_PARAMETER_INVALID;
	}
	else{
		HV_ARG_GET_ADV_USER_CUSTOM *pArg = (HV_ARG_GET_ADV_USER_CUSTOM*)pInBuffer;
		HV_RES_GET_ADV_USER_CUSTOM *pRes = (HV_RES_GET_ADV_USER_CUSTOM*)pOutBuffer;
		
		if(AsyncBlockRead(0xf4f00000 + pArg->dwOffset, pRes->pBuffer, pArg->nLength) != STATUS_OK)
		{
			return STATUS_HW_IO_ERROR;
		} 
		st = STATUS_OK;
	}
	
	return st;

}
HVSTATUS CCamera::f_SET_ADV_USER_CUSTOM(void *pInBuffer, DWORD nInBufferSize)
{
	HVSTATUS st = STATUS_NOT_SUPPORT_INTERFACE;
	if( (NULL==pInBuffer) || 
		(sizeof(HV_ARG_SET_ADV_USER_CUSTOM)!=nInBufferSize))
	{
		st = STATUS_PARAMETER_INVALID;
	}
	else{
		HV_ARG_SET_ADV_USER_CUSTOM *pArg = (HV_ARG_SET_ADV_USER_CUSTOM*)pInBuffer; 
		
		if(AsyncBlockWrite(0xf4f00000 + pArg->dwOffset, pArg->pBuffer, pArg->nLength) != STATUS_OK)
		{
			return STATUS_HW_IO_ERROR;
		} 
		st = STATUS_OK;
	}
	
	return st;

}

HVTYPE CCamera::_GetMyType()
{
	HVTYPE MyType = (HVTYPE)-1;
	CAMERA_INFO cell = { 0 };
	GetCameraInfo(m_pName, &cell);

	if(0 == strcmp(cell.ModelName,"dh-sv1300fc"))
	{
		MyType = SV1300FCTYPE;
	}
	else if(0 == strcmp(cell.ModelName,"dh-sv1300fm"))
	{
		MyType = SV1300FMTYPE;
	}
	else if(0 == strcmp(cell.ModelName,"dh-sv1310fc"))
	{
		MyType = SV1310FCTYPE;
	}
	else if(0 == strcmp(cell.ModelName,"dh-sv1310fm"))
	{
		MyType = SV1310FMTYPE;
	}
	else if(0 == strcmp(cell.ModelName,"dh-sv1311fc"))
	{
		MyType = SV1311FCTYPE;
	}
	else if(0 == strcmp(cell.ModelName,"dh-sv1311fm"))
	{
		MyType = SV1311FMTYPE;
	}
	else if(0 == strcmp(cell.ModelName,"dh-sv1400fc"))
	{
		MyType = SV1400FCTYPE;
	}
	else if(0 == strcmp(cell.ModelName,"dh-sv1400fm"))
	{
		MyType = SV1400FMTYPE;
	}
	else if(0 == strcmp(cell.ModelName,"dh-sv1410fc"))
	{
		MyType = SV1410FCTYPE;
	}
	else if(0 == strcmp(cell.ModelName,"dh-sv1410fm"))
	{
		MyType = SV1410FMTYPE;
	}
	else if(0 == strcmp(cell.ModelName,"dh-sv1420fc"))
	{
		MyType = SV1420FCTYPE;
	}
	else if(0 == strcmp(cell.ModelName,"dh-sv1420fm"))
	{
		MyType = SV1420FMTYPE;
	}
	else if(0 == strcmp(cell.ModelName,"dh-sv2000fc"))
	{
		MyType = SV2000FCTYPE;
	}
	else if(0 == strcmp(cell.ModelName,"dh-sv2000fm"))
	{
		MyType = SV2000FMTYPE;
	}
	else if(0 == strcmp(cell.ModelName,"dh-sv400fc"))
	{
		MyType = SV400FCTYPE;
	}
	else if(0 == strcmp(cell.ModelName,"dh-sv400fm"))
	{
		MyType = SV400FMTYPE;
	}
	else if(0 == strcmp(cell.ModelName,"al-sv1410fc"))
	{
		MyType = (HVTYPE)RESERVED_001;
	}

	return MyType;
}

HVSTATUS CCamera::f_SET_BASIC_FUNC(void *pInBuffer)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	HV_ARG_SET_BASIC_FUNC *pInBuf = (HV_ARG_SET_BASIC_FUNC *)pInBuffer;

	switch(pInBuf->FuncId) {
	case BASIC_FUNC_ID_MEM_CH_SAVE://保存用户参数
		{
			switch(pInBuf->Func.MemChSave.Number) {
			case 1:
			case 2:
			case 3:
				DWORD dwWriteValue;
				dwWriteValue= pInBuf->Func.MemChSave.Number<<28;//[0..3] 0x10000000 ...
				if (WriteQuadlet(0xf0f00620,dwWriteValue) != STATUS_OK)
				{
					return STATUS_HW_IO_ERROR;
				}
				if (WriteQuadlet(0xf0f00618,0x80000000) != STATUS_OK)
				{
					return STATUS_HW_IO_ERROR;
				}
				status = STATUS_OK ;
				break;
			default:
				status = STATUS_PARAMETER_INVALID;
				break;
			}
			
		}
		break;
	case BASIC_FUNC_ID_MEM_CH_CUR://加载用户参数
		{
			switch(pInBuf->Func.MemChCur.Number) {
			case 0://0-factory
			case 1:
			case 2:
			case 3:
				DWORD dwWriteValue;
				dwWriteValue= pInBuf->Func.MemChCur.Number<<28;//[0..3] 0x10000000 ...
				if (WriteQuadlet(0x624,dwWriteValue) != STATUS_OK)
				{
					return STATUS_HW_IO_ERROR;
				}
				status = STATUS_OK ;
				break;
			default:
				status = STATUS_PARAMETER_INVALID;
				break;
			}

		}
		break;
	default:
		break;
	}

	return status;

}
