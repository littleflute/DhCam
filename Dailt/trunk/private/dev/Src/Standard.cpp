#include "stdafx.h"
#include "math.h"

extern "C" HVSTATUS __stdcall BeginHVDeviceEx(void* p,HV_BEGIN_TYPE type, HHV *pHandle)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
    VERIFY_POINTER(pHandle, STATUS_PARAMETER_INVALID);
    *pHandle = NULL;

    switch (type)
    {
    case FROM_INDEX:
    {
        int *pIndex = (int*)p;
        if (pIndex == NULL || *pIndex <= 0)
        {
            return STATUS_PARAMETER_INVALID;
        }
        status = BeginDeviceFromIndex(*pIndex,pHandle);
    }
    break;
    case FROM_SERIESNUM:
    case FROM_MAC:
    case FROM_IP:
    {
        char* pInBuf = (char*)p;
        if (pInBuf == NULL || pInBuf == (char*)0xcccccccc)
        {
            return STATUS_PARAMETER_INVALID;
        }
        status = BeginDeviceFromOthers(pInBuf, pHandle);

    }
    break;
    default:
        return STATUS_PARAMETER_INVALID;
        break;
    }

    return  status;

}

extern "C" HVSTATUS __stdcall BeginHVDevice(int nDevice, HHV *pHandle)
{
    return BeginDeviceFromIndex(nDevice,pHandle);
}

extern "C" HVSTATUS __stdcall EndHVDevice(HHV hhv)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
    HVOBJ *pObj = NULL;

    pObj = (HVOBJ *)GlobalLock( hhv );
    VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);

    //if the device is working, nothing to do
    if (pObj->HvStatus != HW_NONE)
    {
        GlobalUnlock(hhv);
        return STATUS_IN_WORK;
    }

    // change by lvxf, 2009-07-02
    // make sure unlock the handle
    status = HVClose(pObj->hDevice);
    GlobalUnlock(hhv);
    if (HV_SUCCESS(status))
    {
        GlobalFree(hhv);
    }
    return status;
}

extern "C" HVSTATUS __stdcall HVSetResolution(HHV hhv, HV_RESOLUTION resolution)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
    HVOBJ *pObj = NULL;
    int nRVal = 1;
    FARPROC pFuncInfo = NULL, pFuncSet = NULL;

    pObj = (HVOBJ *)GlobalLock( hhv );
    VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);

    //if the device is working, nothing to do
    if (pObj->HvStatus == HW_SNAP && (! pObj->fSuspendGrab))
    {
        // GlobalLock(hhv);
        GlobalUnlock(hhv); // change by lvxf, 2009-07-02
        return STATUS_IN_WORK;
    }
    //pObj->video_mode = (HV_VIDEO_MODE)resolution;

    CCamera camVideoMode;
    //status = camVideoMode.Set_Set_Video_Mode(pObj->hDevice,pObj->video_mode);
	status = camVideoMode.Set_Set_Video_Mode(pObj->hDevice,(HV_VIDEO_MODE)resolution);

    GlobalUnlock(hhv);
    return status;
}

/*
	Function:
		HVSetOutputWindow
	Input:
		HHV hhv						Handle of device
		int nXStart
		int nYStart
		int nWidth
		int nHeight
	Output:
		HVSTATUS					external error code
	Description:

 */
extern "C" HVSTATUS __stdcall HVSetOutputWindow(HHV hhv, int nXStart, int nYStart, int nWidth, int nHeight)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
    HVOBJ *pObj = NULL;
    int nRVal = 1;
    FARPROC pFunc = NULL;
    int nExtend = 0;


    if (nXStart < 0 || nYStart < 0 || nWidth < 4 || nHeight < 2 )
    {
        _RPT1(_CRT_WARN, "\n[HVDAILT->HVSetOutputWindow]:: --------nXStart=%d", nXStart);
        _RPT1(_CRT_WARN, "[HVDAILT->HVSetOutputWindow]:: --------nYStart=%d", nYStart);
        _RPT1(_CRT_WARN, "[HVDAILT->HVSetOutputWindow]:: --------nWidth=%d", nWidth);
        _RPT1(_CRT_WARN, "[HVDAILT->HVSetOutputWindow]:: --------nHeight=%d\n", nHeight);
        return STATUS_PARAMETER_INVALID;
    }


    _RPT2(_CRT_WARN, "[HVDAILT->HVSetOutputWindow]:: --------nWidth=%d nHeight=%d--------", nWidth,nHeight);

    pObj = (HVOBJ *)GlobalLock( hhv );
    VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);

    //if the device is working, nothing to do
    if (pObj->HvStatus == HW_SNAP && (! pObj->fSuspendGrab ))
    {
        //GlobalLock(hhv);
        GlobalUnlock(hhv);// change by lvxf 2009-07-02
        return STATUS_IN_WORK;
    }

	HV_RES_GET_VIDEO_FORMAT_MODE_FRAME_RATE resVMode;
	resVMode.VideoMode = VIDEO_MODE0;//默认
	status = HVControl(pObj->hDevice,
		ORD_GET_VIDEO_FORMAT_MODE_FRAME_RATE,
		NULL,0,
		&resVMode,sizeof(resVMode),		
		NULL);
    CCamera camSetAoi;
    status = camSetAoi.Set_AOI(pObj->hDevice,resVMode.VideoMode,nXStart,nYStart,nWidth,nHeight);

    GlobalUnlock(hhv);

    return status;
}


/*
	Function:
		HVSetDelay
	Input:
		HHV hhv						Handle of device
		int nXDelay					X delay value
		int nYDelay					X delay value
	Output:
		HVSTATUS					external error code
	Description:

 */
extern "C" HVSTATUS __stdcall HVSetBlanking(HHV hhv, int nHor, int nVert)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
    HVOBJ *pObj = NULL;
    int nRVal = 1;
    FARPROC pFunc = NULL;

    pObj = (HVOBJ *)GlobalLock( hhv );
    VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);

//	status = DevGetFuncAddr(pObj->type, "SetDelay", &pFunc,  true, true);
//	if (HV_SUCCESS(status)){
//		nRVal = (* (FUNC_HVSetDelay *)pFunc)(pObj->hDevice, nHor, nVert);
//		if (0 != nRVal){
//			status = (HVSTATUS)nRVal;
//		}
//	}

    GlobalUnlock(hhv);
    return status;
}



/*
	Function:
		HVSetSnapMode
	Input:
		HHV hhv						Handle of device
		HV_SNAP_MODE mode
	Output:
		HVSTATUS					external error code
	Description:

 */
extern "C" HVSTATUS __stdcall HVSetSnapMode(HHV hhv, HV_SNAP_MODE mode)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
    HVOBJ *pObj = NULL;
    int nRVal = 1;
    FARPROC pFunc = NULL;

    //Check range of parameters
    if (mode < CONTINUATION || mode > TRIGGER_LEVEL)
    {
        return STATUS_PARAMETER_OUT_OF_BOUND;
    }

    pObj = (HVOBJ *)GlobalLock( hhv );
    VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);

    CCamera camSetSnapMode;
    status = camSetSnapMode.Set_Snap_Mode(pObj->hDevice,mode);

    GlobalUnlock(hhv);
    return status;
}



/*
	Function:
		HVSetTriggerPolarity
	Input:
		HHV hhv						Handle of device
		HV_POLARITY polarity
	Output:
		HVSTATUS					external error code
	Description:

 */
extern "C" HVSTATUS __stdcall HVSetTriggerPolarity(HHV hhv, HV_POLARITY polarity)
{
    HVSTATUS status = STATUS_OK;
    HVOBJ *pObj = NULL;
    int nRVal = 1;
    FARPROC pFunc = NULL;

    //Check range of parameters
    if (polarity < LOW || polarity > HIGH)
    {
        return STATUS_PARAMETER_OUT_OF_BOUND;
    }

    pObj = (HVOBJ *)GlobalLock( hhv );
    VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);

    CCamera camSetTrgPol;
    status = camSetTrgPol.Set_Trigger_Polarity(pObj->hDevice,polarity);

    GlobalUnlock(hhv);
    return status;
}



/*
	Function:
		HVSetStrobePolarity
	Input:
		HHV hhv						Handle of device
		HV_POLARITY polarity
	Output:
		HVSTATUS					external error code
	Description:

 */
extern "C" HVSTATUS __stdcall HVSetStrobePolarity(HHV hhv, HV_POLARITY polarity)
{
    HVSTATUS status = STATUS_OK;
    HVOBJ *pObj = NULL;
    int nRVal = 1;
    FARPROC pFunc = NULL;

    //Check range of parameters
    if (polarity < LOW || polarity > HIGH)
    {
        return STATUS_PARAMETER_OUT_OF_BOUND;
    }

    pObj = (HVOBJ *)GlobalLock( hhv );
    VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);

    CCamera camSetStrobePol;
    status = camSetStrobePol.Set_Strobe_Polarity(pObj->hDevice,polarity);

    GlobalUnlock(hhv);
    return status;
}



/*
	Function:
		HVSetSnapSpeed
	Input:
		HHV hhv						Handle of device
		HV_SNAP_SPEED speed
	Output:
		HVSTATUS					external error code
	Description:

 */
extern "C" HVSTATUS __stdcall HVSetSnapSpeed(HHV hhv, HV_SNAP_SPEED speed)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
    HVOBJ *pObj = NULL;
    int nRVal = 1;
    FARPROC pFunc = NULL;

    //Check range of parameters
    if (speed > HIGH_SPEED || speed < NORMAL_SPEED)
    {
        return STATUS_PARAMETER_OUT_OF_BOUND;
    }

    pObj = (HVOBJ *)GlobalLock( hhv );
    VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);

//	status = DevGetFuncAddr(pObj->type, "SetSnapSpeed", &pFunc,  true, true);
//	if (HV_SUCCESS(status)){
//		nRVal = (* (FUNC_HVSetSnapSpeed *)pFunc)(pObj->hDevice, (BYTE)speed);
//		if (0 != nRVal){
//			status = (HVSTATUS)nRVal;
//		}
//	}

    GlobalUnlock(hhv);
    return status;
}



/*
	Function:
		HVEnableVideoMirror
	Input:
		HHV hhv						Handle of device
		MIRROR_DIRECTION dir
		BOOL bEnable
	Output:
		HVSTATUS					external error code
	Description:

 */
extern "C" HVSTATUS __stdcall HVEnableVideoMirror(HHV hhv, HV_MIRROR_DIRECTION dir, BOOL bEnable)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
    HVOBJ *pObj = NULL;
    int nRVal = 1;
    FARPROC pFunc = NULL;

    //Check range of parameters
    if (dir < VERT_DIR || dir > VERT_DIR)
    {
        return STATUS_PARAMETER_OUT_OF_BOUND;
    }

    pObj = (HVOBJ *)GlobalLock( hhv );
    VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);

//	status = DevGetFuncAddr(pObj->type, "EnableVideoMirror", &pFunc,  true, true);
//	if (HV_SUCCESS(status)){
//		nRVal = (* (FUNC_HVEnableVideoMirror *)pFunc)(pObj->hDevice, dir, bEnable);
//		if (0 != nRVal){
//			status = (HVSTATUS)nRVal;
//		}
//	}

    GlobalUnlock(hhv);
    return status;
}



/*
	Function:
		HVADCControl
	Input:
		HHV hhv						Handle of device
		BYTE byParam
		long lValue
	Output:
		HVSTATUS					external error code
	Description:

 */

extern "C" HVSTATUS __stdcall HVADCControl(HHV hhv, BYTE byParam, long lValue)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;

    HVOBJ *pObj = NULL;
    int nRVal = 1;
    FARPROC pFunc = NULL;

    pObj = (HVOBJ *)GlobalLock( hhv );
    VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);

    // 添加类型判断，HVxx00Fx系列使用原有寄存器，其余使用新寄存器
    BOOL bHVFserial = IsHVFSerial(pObj->hDevice);
    if (bHVFserial)
    {
        // HVxx00Fx系列
        switch (byParam)
        {
        case ADC_BITS:
        {
            CCamera cam1028Mode;
            status = cam1028Mode.Set_10_2_8_Mode(pObj->hDevice,lValue);
        }
        break;
        case ADC_BLKLEVEL_CAL:
        {
            CCamera camBlaceLevelOnOff;
            status = camBlaceLevelOnOff.Set_Black_Level_On_Off(pObj->hDevice,lValue);
        }
        break;
        case ADC_BLKLEVEL_CAL_REDCHANNEL:
        case ADC_BLKLEVEL_CAL_GREENCHANNEL1:
        case ADC_BLKLEVEL_CAL_GREENCHANNEL2:
        case ADC_BLKLEVEL_CAL_BLUECHANNEL:
        {
            CCamera camBrightness;
            HV_ADC_CONTROL id = (HV_ADC_CONTROL)byParam;
            status = camBrightness.Set_Brightness(pObj->hDevice,id,lValue);
        }
        break;
        case ADC_BRIGHTNESS:
            HV_ARG_SET_FEATURE InBuf;
            InBuf.FeatureId = FEATURE_ID_BRIGHTNESS;
            InBuf.FeatureItemId = FEATURE_ITEM_ID_SCALAR_VALUE;
            InBuf.Feature.Scalar.Value = lValue;
            status = HVControl(pObj->hDevice,
                               ORD_SET_FEATURE,
                               &InBuf,sizeof(HV_ARG_SET_FEATURE),
                               0,
                               0,
                               0);
            break;
        default:
            break;
        }
    }
    else
    {
        HV_ARG_SET_ADV_FEATURE argSetAdvFeature;
        memset(&argSetAdvFeature,0,sizeof(argSetAdvFeature));

        // 其它系列
        switch (byParam)
        {
		case ADC_BITS:
			argSetAdvFeature.AdvFeatureId     = ADV_FEATURE_ID_DIGITAL_GAIN;
            argSetAdvFeature.AdvFeatureItemId = ADV_FEATURE_ITEM_ID_DIGITAL_GAIN_GAIN_NUM;
            argSetAdvFeature.AdvFeature.DigitalGain.GainNum = (0x1<<lValue);
            status = HVControl(pObj->hDevice,
				ORD_SET_ADV_FEATURE,
				&argSetAdvFeature,
				sizeof(argSetAdvFeature),
				0,
				0,
                0);
			break;
        case ADC_BLKLEVEL_CAL:
            argSetAdvFeature.AdvFeatureId     = ADV_FEATURE_ID_GLOBAL_BLACK_LEVEL;
            argSetAdvFeature.AdvFeatureItemId = ADV_FEATURE_ITEM_ID_SCALAR_ON_OFF;
            argSetAdvFeature.AdvFeature.Scalar.OnOff = lValue;
            status = HVControl(pObj->hDevice,
                               ORD_SET_ADV_FEATURE,
                               &argSetAdvFeature,
                               sizeof(argSetAdvFeature),
                               0,
                               0,
                               0);
            break;
        case ADC_BLKLEVEL_CAL_REDCHANNEL://= 0x10,
            argSetAdvFeature.AdvFeatureId     = ADV_FEATURE_ID_PIXEL_SEPARATE_BLACK_LEVEL_RB;
            argSetAdvFeature.AdvFeatureItemId = ADV_FEATURE_ITEM_ID_PIXEL_SEPARATE_R_G1_VALUE;
            argSetAdvFeature.AdvFeature.PixelSeparateEx.R_G1_Value = lValue+255;
            status = HVControl(pObj->hDevice,
                               ORD_SET_ADV_FEATURE,
                               &argSetAdvFeature,
                               sizeof(argSetAdvFeature),
                               0,
                               0,
                               0);
            break;
        case ADC_BLKLEVEL_CAL_GREENCHANNEL1://= 0x11,
            argSetAdvFeature.AdvFeatureId     = ADV_FEATURE_ID_PIXEL_SEPARATE_BLACK_LEVEL_G1G2;
            argSetAdvFeature.AdvFeatureItemId = ADV_FEATURE_ITEM_ID_PIXEL_SEPARATE_R_G1_VALUE;
            argSetAdvFeature.AdvFeature.PixelSeparateEx.R_G1_Value = lValue+255;
            status = HVControl(pObj->hDevice,
                               ORD_SET_ADV_FEATURE,
                               &argSetAdvFeature,
                               sizeof(argSetAdvFeature),
                               0,
                               0,
                               0);
            break;
        case ADC_BLKLEVEL_CAL_GREENCHANNEL2://= 0x12,
            argSetAdvFeature.AdvFeatureId     = ADV_FEATURE_ID_PIXEL_SEPARATE_BLACK_LEVEL_G1G2;
            argSetAdvFeature.AdvFeatureItemId = ADV_FEATURE_ITEM_ID_PIXEL_SEPARATE_B_G2_VALUE;
            argSetAdvFeature.AdvFeature.PixelSeparateEx.B_G2_Value = lValue+255;
            status = HVControl(pObj->hDevice,
                               ORD_SET_ADV_FEATURE,
                               &argSetAdvFeature,
                               sizeof(argSetAdvFeature),
                               0,
                               0,
                               0);
            break;
        case ADC_BLKLEVEL_CAL_BLUECHANNEL://= 0x13,
            argSetAdvFeature.AdvFeatureId     = ADV_FEATURE_ID_PIXEL_SEPARATE_BLACK_LEVEL_RB;
            argSetAdvFeature.AdvFeatureItemId = ADV_FEATURE_ITEM_ID_PIXEL_SEPARATE_B_G2_VALUE;
            argSetAdvFeature.AdvFeature.PixelSeparateEx.B_G2_Value = lValue+255;
            status = HVControl(pObj->hDevice,
                               ORD_SET_ADV_FEATURE,
                               &argSetAdvFeature,
                               sizeof(argSetAdvFeature),
                               0,
                               0,
                               0);
            break;
        default:
            status = STATUS_PARAMETER_INVALID;
            break;
        }
    }
    GlobalUnlock(hhv);
    return status;
}

/*
	Function:
		HVAGCControl
	Input:
		HHV hhv						Handle of device
		BYTE byParam
		long lValue
	Output:
		HVSTATUS					external error code
	Description:

 */
extern "C" HVSTATUS __stdcall HVAGCControl(HHV hhv, BYTE byParam, long lValue)
{
    HVSTATUS status =  STATUS_NOT_SUPPORT_INTERFACE;
    HVOBJ *pObj = NULL;
    int nRVal = 1;
    FARPROC pFunc = NULL;

    // 将判断放在GlobalLock前，避免GlobalLock嵌套
    BOOL bHVFserial = IsHVFSerial(hhv);

    pObj = (HVOBJ *)GlobalLock( hhv );
    VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);

    HV_ARG_SET_FEATURE argSetFeature;
    HV_ARG_SET_ADV_FEATURE argSetAdvFeature;
    HV_RES_QUERY_ADV_FEATURE_PRESENCE resQueryAdvFeaturePresence = {0};

    switch (byParam)
    {
    case AGC_GAIN:
        argSetFeature.FeatureId = FEATURE_ID_GAIN;
        argSetFeature.FeatureItemId = FEATURE_ITEM_ID_SCALAR_VALUE;
        argSetFeature.Feature.Scalar.Value = lValue;
        status = HVControl(pObj->hDevice,
                           ORD_SET_FEATURE,
                           &argSetFeature,
                           sizeof(argSetFeature),
                           0,
                           0,
                           0);
        break;
    case AGC_GAIN_RED_CHANNEL:// modify by lvxf 2009-07-02
    {
        // HVF系列不支持读版本，使用相机类型判断
        if (bHVFserial)
        {
            argSetFeature.FeatureId     = FEATURE_ID_WHITE_BALANCE;
            argSetFeature.FeatureItemId = FEATURE_ITEM_ID_WHITE_BALANCE_VR;
            argSetFeature.Feature.WhiteBalance.VR = lValue;
            status = HVControl(pObj->hDevice,
                               ORD_SET_FEATURE,
                               &argSetFeature,
                               sizeof(argSetFeature),
                               0,
                               0,
                               0);
        }
        else
        {
            // 其他相机，通过判断协议版本,判断是设置到白平衡还是分量增益
            DWORD dwBytes=0;
            HV_ARG_READ_QUADLET argReadQuadlet = {0};
            HV_RES_READ_QUADLET resReadQuadlet = {0};
            argReadQuadlet.dwAddress = DHCAM_VERSION_ADDRESS;//
            status = HVControl(pObj->hDevice,
                               (HV_CONTROL_CODE)ORD_READ_QUADLET,
                               &argReadQuadlet,
                               sizeof(HV_ARG_READ_QUADLET),
                               &resReadQuadlet,
                               sizeof(resReadQuadlet),
                               &dwBytes);
            if (!HV_SUCCESS(status)||
                    resReadQuadlet.dwData<DHCAM_MIN_VERSION_SUPPORT_WHITE_BALANCE
               )
            {
                // 部分摄像机设置增益时，设置成了白平衡，这里做兼容处理。这类相机不支持白平衡
                // 摄像机不支持白平衡，有两种情况：
                //    1 没有该功能，返回错误状态；
                //    2 有该功能，但协议版本比较旧，还没有实现白平衡
                argSetFeature.FeatureId = FEATURE_ID_WHITE_BALANCE;
                argSetFeature.FeatureItemId = FEATURE_ITEM_ID_WHITE_BALANCE_VR;
                argSetFeature.Feature.WhiteBalance.VR = lValue;
                status = HVControl(pObj->hDevice,
                                   ORD_SET_FEATURE,
                                   &argSetFeature,
                                   sizeof(argSetFeature),
                                   0,
                                   0,
                                   0);
            }
            else
            {
                // 设置分量增益
                memset(&resQueryAdvFeaturePresence,0,sizeof(resQueryAdvFeaturePresence));
                status = HVControl(pObj->hDevice,
                                   ORD_QUERY_ADV_FEATURE_PRESENCE,
                                   NULL,
                                   0,
                                   &resQueryAdvFeaturePresence,
                                   sizeof(resQueryAdvFeaturePresence),
                                   NULL);
                if (1==resQueryAdvFeaturePresence.PixelsSeparateGianRB)
                {
                    // 设置到扩展后的分量增益，增益范围[0,127]
                    argSetAdvFeature.AdvFeatureId     = ADV_FEATURE_ID_PIXEL_SEPARATE_GAIN_RB;
                    argSetAdvFeature.AdvFeatureItemId = ADV_FEATURE_ITEM_ID_PIXEL_SEPARATE_R_G1_VALUE;
                    argSetAdvFeature.AdvFeature.PixelSeparateEx.R_G1_Value = lValue;
                }
                else
                {
                    // 设置到扩展前的分量增益，增益范围[0,63]
                    argSetAdvFeature.AdvFeatureId     = ADV_FEATURE_ID_PIXEL_SEPARATE_GAIN;
                    argSetAdvFeature.AdvFeatureItemId = ADV_FEATURE_ITEM_ID_PIXEL_SEPERARATE_R_VALUE;
                    argSetAdvFeature.AdvFeature.PixelSeparate.RValue = lValue;
                }
                status = HVControl(pObj->hDevice,
                                   ORD_SET_ADV_FEATURE,
                                   &argSetAdvFeature,
                                   sizeof(argSetAdvFeature),
                                   0,
                                   0,
                                   0);
            }
        }
    }
    break;
    case AGC_GAIN_BLUE_CHANNEL:
    {
        if (bHVFserial)
        {
            argSetFeature.FeatureId     = FEATURE_ID_WHITE_BALANCE;
            argSetFeature.FeatureItemId = FEATURE_ITEM_ID_WHITE_BALANCE_UB;
            argSetFeature.Feature.WhiteBalance.UB = lValue;
            status = HVControl(pObj->hDevice,
                               ORD_SET_FEATURE,
                               &argSetFeature,
                               sizeof(argSetFeature),
                               0,
                               0,
                               0);
        }
        else
        {
            DWORD dwBytes=0;
            HV_ARG_READ_QUADLET argReadQuadlet = {0};
            HV_RES_READ_QUADLET resReadQuadlet = {0};
            argReadQuadlet.dwAddress = DHCAM_VERSION_ADDRESS;//
            status = HVControl(pObj->hDevice,
                               (HV_CONTROL_CODE)ORD_READ_QUADLET,
                               &argReadQuadlet,
                               sizeof(HV_ARG_READ_QUADLET),
                               &resReadQuadlet,
                               sizeof(resReadQuadlet),
                               &dwBytes);
            if (!HV_SUCCESS(status)||
                    resReadQuadlet.dwData<DHCAM_MIN_VERSION_SUPPORT_WHITE_BALANCE
               )
            {
                // 摄像机不支持白平衡功能，沿用先前的方法，设置到Feature.WhiteBalance
                // 摄像机不支持白平衡，有两种情况：
                //    1 没有该功能，返回错误状态；
                //    2 有该功能，但协议版本比较旧，还没有实现白平衡
                argSetFeature.FeatureId = FEATURE_ID_WHITE_BALANCE;
                argSetFeature.FeatureItemId = FEATURE_ITEM_ID_WHITE_BALANCE_UB;
                argSetFeature.Feature.WhiteBalance.UB = lValue;
                status = HVControl(pObj->hDevice,
                                   ORD_SET_FEATURE,
                                   &argSetFeature,
                                   sizeof(argSetFeature),
                                   0,
                                   0,
                                   0);
            }
            else
            {
                // 设置分量增益
                memset(&resQueryAdvFeaturePresence,0,sizeof(resQueryAdvFeaturePresence));
                status = HVControl(pObj->hDevice,
                                   ORD_QUERY_ADV_FEATURE_PRESENCE,
                                   NULL,
                                   0,
                                   &resQueryAdvFeaturePresence,
                                   sizeof(resQueryAdvFeaturePresence),
                                   NULL);
                if (1==resQueryAdvFeaturePresence.PixelsSeparateGianRB)
                {
                    // 设置到扩展后的分量增益，增益范围[0,127]
                    argSetAdvFeature.AdvFeatureId     = ADV_FEATURE_ID_PIXEL_SEPARATE_GAIN_RB;
                    argSetAdvFeature.AdvFeatureItemId = ADV_FEATURE_ITEM_ID_PIXEL_SEPARATE_B_G2_VALUE;
                    argSetAdvFeature.AdvFeature.PixelSeparateEx.B_G2_Value = lValue;
                }
                else
                {
                    // 设置到扩展前的分量增益，增益范围[0,63]
                    argSetAdvFeature.AdvFeatureId = ADV_FEATURE_ID_PIXEL_SEPARATE_GAIN;
                    argSetAdvFeature.AdvFeatureItemId = ADV_FEATURE_ITEM_ID_PIXEL_SEPERARATE_B_VALUE;
                    argSetAdvFeature.AdvFeature.PixelSeparate.BValue = lValue;
                }
                status = HVControl(pObj->hDevice,
                                   ORD_SET_ADV_FEATURE,
                                   &argSetAdvFeature,
                                   sizeof(argSetAdvFeature),
                                   0,
                                   0,
                                   0);
            }
        }
    }
    break;

    case AGC_GAIN_GREEN_CHANNEL1:
        // 设置分量增益
        memset(&resQueryAdvFeaturePresence,0,sizeof(resQueryAdvFeaturePresence));
        status = HVControl(pObj->hDevice,
                           ORD_QUERY_ADV_FEATURE_PRESENCE,
                           NULL,
                           0,
                           &resQueryAdvFeaturePresence,
                           sizeof(resQueryAdvFeaturePresence),
                           NULL);
        if (1==resQueryAdvFeaturePresence.PixelsSeparateGainG1G2)
        {
            // 设置到扩展后的分量增益，增益范围[0,127]
            argSetAdvFeature.AdvFeatureId     = ADV_FEATURE_ID_PIXEL_SEPARATE_GAIN_G1G2;
            argSetAdvFeature.AdvFeatureItemId = ADV_FEATURE_ITEM_ID_PIXEL_SEPARATE_R_G1_VALUE;
            argSetAdvFeature.AdvFeature.PixelSeparateEx.R_G1_Value = lValue;
        }
        else
        {
            // 设置到扩展前的分量增益，增益范围[0,63]
            argSetAdvFeature.AdvFeatureId = ADV_FEATURE_ID_PIXEL_SEPARATE_GAIN;
            argSetAdvFeature.AdvFeatureItemId = ADV_FEATURE_ITEM_ID_PIXEL_SEPERARATE_GR_VALUE;
            argSetAdvFeature.AdvFeature.PixelSeparate.GrValue = lValue;
        }
        status = HVControl(pObj->hDevice,
                           ORD_SET_ADV_FEATURE,
                           &argSetAdvFeature,
                           sizeof(argSetAdvFeature),
                           0,
                           0,
                           0);
        break;
    case AGC_GAIN_GREEN_CHANNEL2:
        // 设置分量增益
        memset(&resQueryAdvFeaturePresence,0,sizeof(resQueryAdvFeaturePresence));
        status = HVControl(pObj->hDevice,
                           ORD_QUERY_ADV_FEATURE_PRESENCE,
                           NULL,
                           0,
                           &resQueryAdvFeaturePresence,
                           sizeof(resQueryAdvFeaturePresence),
                           NULL);
        if (1==resQueryAdvFeaturePresence.PixelsSeparateGainG1G2)
        {
            // 设置到扩展后的分量增益，增益范围[0,127]
            argSetAdvFeature.AdvFeatureId     = ADV_FEATURE_ID_PIXEL_SEPARATE_GAIN_G1G2;
            argSetAdvFeature.AdvFeatureItemId = ADV_FEATURE_ITEM_ID_PIXEL_SEPARATE_B_G2_VALUE;
            argSetAdvFeature.AdvFeature.PixelSeparateEx.B_G2_Value = lValue;
        }
        else
        {
            // 设置到扩展前的分量增益，增益范围[0,63]
            argSetAdvFeature.AdvFeatureId = ADV_FEATURE_ID_PIXEL_SEPARATE_GAIN;
            argSetAdvFeature.AdvFeatureItemId = ADV_FEATURE_ITEM_ID_PIXEL_SEPERARATE_GB_VALUE;
            argSetAdvFeature.AdvFeature.PixelSeparate.GbValue = lValue;
        }
        status = HVControl(pObj->hDevice,
                           ORD_SET_ADV_FEATURE,
                           &argSetAdvFeature,sizeof(argSetAdvFeature),
                           0,
                           0,
                           0);
        break;

    default:
        status = STATUS_PARAMETER_INVALID;
        break;
    }

    GlobalUnlock(hhv);
    return status;
}



/*
	Function:
		HVAECControl
	Input:
		HHV hhv						Handle of device
		BYTE byParam
		long lValue
	Output:
		HVSTATUS					external error code
	Description:

 */
extern "C" HVSTATUS __stdcall HVAECControl(HHV hhv, BYTE byParam, long lValue)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
    HVOBJ *pObj = NULL;
    int nRVal = 1;
    FARPROC pFunc = NULL;

    //Check range of parameters
    if (byParam < AEC_EXPOSURE_TIME || byParam > AEC_SHUTTER_UNIT/*AEC_SHUTTER_DELAY*/)
    {
        return STATUS_PARAMETER_OUT_OF_BOUND;
    }

    pObj = (HVOBJ *)GlobalLock( hhv );
    VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);

    switch (byParam)
    {
    case AEC_EXPOSURE_TIME:
    {
        CCamera camSetExTime;
        status = camSetExTime.Set_Exposure_Time(pObj->hDevice,lValue);
    }
    break;
    case AEC_SHUTTER_SPEED:
    {
        HV_ARG_SET_FEATURE InBuf;
        InBuf.FeatureId = FEATURE_ID_SHUTTER;
        InBuf.FeatureItemId = FEATURE_ITEM_ID_SCALAR_VALUE;
        InBuf.Feature.Scalar.Value = lValue;
        status = HVControl(pObj->hDevice,
                           ORD_SET_FEATURE,
                           &InBuf,sizeof(HV_ARG_SET_FEATURE),
                           0,0,
                           0);
    }
    break;
    case AEC_SHUTTER_UNIT:
        HV_ARG_SET_ADV_FEATURE InBuf;
        InBuf.AdvFeatureId = ADV_FEATURE_ID_SHUTTER_UNIT;
        InBuf.AdvFeatureItemId = ADV_FEATURE_ITEM_ID_SELECTOR_MODE;
        InBuf.AdvFeature.Selector.Mode = lValue;
        status = HVControl(pObj->hDevice,
                           ORD_SET_ADV_FEATURE,
                           &InBuf,sizeof(HV_ARG_SET_ADV_FEATURE),
                           0,0,
                           0);
        break;
    default:
        break;
    }

    GlobalUnlock(hhv);
    return status;
}




extern "C" HVSTATUS __stdcall HVSnapShot(HHV hhv, BYTE **ppBuffer, int nSum)
{
    HVSTATUS status = STATUS_OK;
    HVOBJ *pObj = NULL;

    //Check parameters
    VERIFY_POINTER(ppBuffer, STATUS_PARAMETER_INVALID);
    if (nSum < 0)
    {
        return STATUS_PARAMETER_INVALID;
    }
    else if (nSum == 0)
    {
        return STATUS_OK;
    }

    pObj = (HVOBJ *)GlobalLock( hhv );
    VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);

//	if ( pObj->bLock ){
//		GlobalUnlock(hhv);
//		return STATUS_OK;
//	}

    if (pObj->HvStatus != HW_NONE)  	//Is Device Idle
    {
        status = STATUS_IN_WORK;
    }
    else
    {
        HV_ARG_SNAP_SHOT ArgSnapShot;
        ArgSnapShot.ppBuffer  = ppBuffer;
        ArgSnapShot.nSum      = nSum;
        status = HVControl(pObj->hDevice, ORD_SNAP_SHOT,
                           &ArgSnapShot, sizeof(HV_ARG_SNAP_SHOT), NULL, 0, NULL);
    }

    GlobalUnlock(hhv);

    return status;
}



int CALLBACK SnapCallbackFunc(HV_SNAP_INFO *pInfo)
{
    HVOBJ *pObj = NULL;

    pObj = (HVOBJ *)GlobalLock(pInfo->pParam/*pInfo->hDevice*/);
    if ( pObj )
    {
        HV_SNAP_INFO SnapInfo = {0};
        SnapInfo.hhv     = pInfo->hhv;
        SnapInfo.nIndex  = pInfo->nIndex;

        SnapInfo.pParam  = pObj->pParam;
        SnapInfo.nDevice = pObj->nDevice;

//		_RPT1(_CRT_WARN, "HVAILT TR:  SnapInfoExt.nDevice=%d", SnapInfoExt.nDevice);

        /// call user function
        (pObj->pSnapProc)(&SnapInfo);

        GlobalUnlock(pInfo->pParam);
    }

    return 0;
}


extern "C" HVSTATUS __stdcall HVOpenSnap(HHV hhv, HV_SNAP_PROC lpSnapFunc, void *pParam)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
    HVOBJ *pObj = NULL;

    //Check parameters
    VERIFY_POINTER(lpSnapFunc, STATUS_PARAMETER_INVALID);

    pObj = (HVOBJ *)GlobalLock( hhv );
    VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);

    if (pObj->HvStatus != HW_NONE)  		//Is Device Idle
    {
        status = STATUS_IN_WORK;
    }
    else
    {
        HV_ARG_OPEN_SNAP ArgOpenSnap;
        ArgOpenSnap.pSnapFunc   = SnapCallbackFunc;
        ArgOpenSnap.pParam      = hhv;//NULL;
        status = HVControl(pObj->hDevice, ORD_OPEN_SNAP,
                           &ArgOpenSnap, sizeof(HV_ARG_OPEN_SNAP), NULL, 0, NULL);
        if ( HV_SUCCESS(status) )
        {
            pObj->pSnapProc     = lpSnapFunc;
            pObj->pParam		= pParam;
            pObj->fSuspendGrab  = TRUE;
            pObj->HvStatus		 = HW_SNAP;
        }
    }

    GlobalUnlock(hhv);

    return status;
}


extern "C" HVSTATUS __stdcall HVCloseSnap(HHV hhv)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
    HVOBJ *pObj = NULL;

    pObj = (HVOBJ *)GlobalLock( hhv );
    VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);


    status = HVControl(pObj->hDevice, ORD_CLOSE_SNAP, NULL, 0, NULL, 0, NULL);
    if ( HV_SUCCESS(status) )
    {
        pObj->fSuspendGrab	= TRUE;
        pObj->HvStatus = HW_NONE;
    }


    GlobalUnlock(hhv);

    return status;
}



extern "C" HVSTATUS __stdcall HVStartSnap(HHV hhv, BYTE **ppBuffer, int nSum)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
    HVOBJ *pObj = NULL;

    VERIFY_POINTER(ppBuffer, STATUS_PARAMETER_INVALID);
    pObj = (HVOBJ *)GlobalLock( hhv );
    VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);

//	if ( pObj->bLock ){
//		GlobalUnlock(hhv);
//		return STATUS_OK;
//	}

    if (pObj->HvStatus != HW_SNAP)  		//Is open snap process
    {
        status = STATUS_NOT_OPEN_SNAP;
    }
    else
    {
        if (  !pObj->fSuspendGrab )
        {
            status = STATUS_IN_WORK;
        }
        else 							//Not start snapping
        {
            HV_ARG_START_SNAP ArgStartSnap;
            ArgStartSnap.ppBuffer = ppBuffer;
            ArgStartSnap.nSum     = nSum;
            status = HVControl(pObj->hDevice, ORD_START_SNAP,
                               &ArgStartSnap, sizeof(HV_ARG_START_SNAP), NULL, 0, NULL);
            if ( HV_SUCCESS(status) )
                pObj->fSuspendGrab	= FALSE;  //to next
        }
    }

    GlobalUnlock(hhv);

    return status;
}




extern "C" HVSTATUS __stdcall HVStopSnap(HHV hhv)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
    HVOBJ *pObj = NULL;

    pObj = (HVOBJ *)GlobalLock( hhv );
    VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);


    status = HVControl(pObj->hDevice, ORD_STOP_SNAP, 0, NULL, 0, 0, NULL);
    if ( HV_SUCCESS(status) )
    {
        pObj->fSuspendGrab	= TRUE;
    }

    GlobalUnlock(hhv);

    return status;
}



extern "C" HVSTATUS __stdcall HVTriggerShot(HHV hhv)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
    HVOBJ *pObj = NULL;
    FARPROC pFunc = NULL;
    int nRVal = 1;

    pObj = (HVOBJ *)GlobalLock( hhv );
    VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);

    if ( pObj->fSuspendGrab)  		//Is Device Idle
    {
        status = STATUS_NOT_START_SNAP;
    }
    else
    {

        status = HVControl(pObj->hDevice,ORD_TRIGGER_SHOT,NULL,0,NULL,0,NULL);

    }

    GlobalUnlock(hhv);

    return status;
}



/*
	Function:
		HVAbortSnap
	Input:
		HHV hhv						Handle of device
	Output:
		HVSTATUS					external error code
	Description:

 */
extern "C" HVSTATUS __stdcall HVAbortSnap(HHV hhv)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
    HVOBJ *pObj = NULL;
    FARPROC pFunc = NULL;
    int nRVal = 0;

    pObj = (HVOBJ *)GlobalLock( hhv );
    VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);

    //Abort snap process
//	status = DevGetFuncAddr(pObj->type, "AbortSnap", &pFunc, true, true);
//	if (HV_SUCCESS(status)){
//		nRVal = (* (FUNC_HVAbortSnap *)pFunc)(pObj->hDevice);  //Stop
//		if (0 != nRVal){
//			status = (HVSTATUS)nRVal;
//		}
//	}


    GlobalUnlock(hhv);
    return status;
}



/*
	Function:
		HVReset
	Input:
		HHV hhv						Handle of device
	Output:
		HVSTATUS					external error code
	Description:

 */
extern "C" HVSTATUS __stdcall HVReset(HHV hhv)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
    HVOBJ *pObj = NULL;
    FARPROC pFunc = NULL;
    int nRVal = 0;

    pObj = (HVOBJ *)GlobalLock( hhv );
    VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);

    if (pObj->HvStatus != HW_NONE)  	//Is Device Idle
    {
        status = STATUS_IN_WORK;
    }
    else
    {

        //Stop snap process
//		status = DevGetFuncAddr(pObj->type, "StopSnap", &pFunc, true, true);
//		if (HV_SUCCESS(status)){
//			nRVal = (* (FUNC_HVStopSnap *)pFunc)(pObj->hDevice);  //Stop
//			if (0 != nRVal){
//				status = (HVSTATUS)nRVal;
//			}
//		}
    }

    GlobalUnlock(hhv);
    return status;
}



/*
	Function:
		HVWaitSync
	Input:
		HHV hhv						Handle of device
	Output:
		HVSTATUS					external error code
	Description:

 */
extern "C" HVSTATUS __stdcall HVWaitSync(HHV hhv)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
    HVOBJ *pObj = NULL;
    int nRVal = 1;
    FARPROC pFunc = NULL;

    pObj = (HVOBJ *)GlobalLock( hhv );
    VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);

    if ( pObj->fSuspendGrab)  	//Is Device work
    {
        status = STATUS_NOT_START_SNAP;
    }
    else
    {
//		if (pObj->bISOTransfer == FALSE)  //Bulk transfer
        {
//			status = DevGetFuncAddr(pObj->type, "WaitSync", &pFunc, true, true);
//			if (HV_SUCCESS(status))
//			{
//				nRVal = (* (FUNC_HVWaitSync *)pFunc)(pObj->hDevice);
//				status = (HVSTATUS)nRVal;
//			}
        }
        //	else
        {
//			status = DevGetFuncAddr(pObj->type, "Transfer", &pFunc, true, true);
//			if (HV_SUCCESS(status)) {
//				ISO_TRANSFER_CONTROL *pIsoControl = &(pObj->IsoControl);
//				DWORD dwBytesToRead = pIsoControl->PacketCount * (pIsoControl->PacketSize + sizeof(USBD_ISO_PACKET_DESCRIPTOR));
//
//				BYTE *pTempBuff = new BYTE[dwBytesToRead];
//				if (NULL == pTempBuff) {
//					status = STATUS_NOT_ENOUGH_SYSTEM_MEMORY;
//				}
//				else{
//					DWORD dwBytes = 0, dwPacketsRead = 0, dwDataRead = 0, dwImageSize = 0;
//					USBD_ISO_PACKET_DESCRIPTOR *pIsoDesc = NULL;
//					BYTE *pSrcBuffer = NULL;
//					BOOL bActive = FALSE;
//					DWORD dwCount = 0;
//					BOOL bSync = FALSE;
//
//					while( ! bActive){	//loop util transfer complete
//						nRVal = (* (FUNC_HVTransfer *)pFunc)(pObj->hDevice, pTempBuff, &dwBytes);
//						if (0 != nRVal){ //Transfer faild and then break
//							pObj->dwDataRead = 0;
//						}
//						else{
//							//Compute the number of packet read
//							dwPacketsRead = dwBytes / (pIsoControl->PacketSize + sizeof(USBD_ISO_PACKET_DESCRIPTOR));
//							//Get source buffer
//							pSrcBuffer = pTempBuff;
//							//Get the address of iso packet descriptor
//							pIsoDesc = (USBD_ISO_PACKET_DESCRIPTOR *) (pTempBuff + (dwPacketsRead * pIsoControl->PacketSize));
//
//							for (DWORD i = 0; i < dwPacketsRead; ++i){
//								if ((pIsoDesc[i].Length == 0) && (pIsoDesc[i].Status == (USBD_STATUS)(pObj->dwSyncSign))){  //0xC0000004L
//									bSync = TRUE;
//									++dwCount;
//								}
//								else{
//									if (bSync) {
//										bActive = TRUE;
//										break;
//									}
//								}
//								pSrcBuffer += pIsoControl->PacketSize;
//							}
//						}
//					}
//					//free temporary buffer
//					SAFE_DELETE_ARRAY(pTempBuff);
//				}
//			}
        }
    }

    GlobalUnlock(hhv);

    return status;
}



/*
Function:
	HVGetErrorString
Input:
	HHV hhv						Handle of device
Output:
	HVSTATUS					external error code
Description:
*/
extern "C" char * __stdcall HVGetErrorString(HVSTATUS status)
{
    if (STATUS_OK < status  || status < STATUS_INTERNAL_ERROR)
    {
        return "STATUS_NOT_DEFINED";
    }
    return g_chErrorString[abs(status)];
}

extern "C" HVSTATUS __stdcall HVGetDeviceTotal(int *pNumber)
{
    VERIFY_POINTER(pNumber, STATUS_PARAMETER_INVALID);

    if (glDevGraph == NULL)
    {
        if (CreateGlobalDeviceGraph() != STATUS_OK)
        {
            return STATUS_NO_DEVICE_FOUND;
        }
        else  *pNumber = glcount;
    }
    else
    {
        delete []glDevGraph;
        glDevGraph = NULL;

        if (CreateGlobalDeviceGraph() != STATUS_OK)
        {
            return STATUS_NO_DEVICE_FOUND;
        }
        else  *pNumber = glcount;
    }

    return STATUS_OK;
}

/*
	Function:
		HVGetDeviceInfo
	Input:
		HHV hhv						Handle of device
		HV_DEVICE_INFO Param
		char *pInfo					Pointer to store buffer
	Output:
		HVSTATUS					external error code
	Description:

 */
extern "C" HVSTATUS __stdcall HVGetDeviceInfo(HHV hhv, HV_DEVICE_INFO param,
        BYTE *pContext, int *pSize)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;

    HVOBJ *pObj = NULL;

    pObj = (HVOBJ *)GlobalLock( hhv );
    VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);

    CCamera camInfo;
    switch (param)
    {
    case DESC_DEVICE_TYPE:
        status = camInfo.DevInfo_Get_Device_Type(pObj->hDevice,pContext,pSize);
        break;
    case DESC_RESOLUTION:
        status = camInfo.DevInfo_Get_Resolution(pObj->hDevice,pContext,pSize);
        break;
    case DESC_DEVICE_SERIESNUM:
        status = camInfo.DevInfo_Get_SN(pObj->hDevice,pContext,pSize);
        break;
    case DESC_DEVICE_CHIPID:
        status = camInfo.DevInfo_Get_Sensor_ID(pObj->hDevice,pContext,pSize);
        break;
    case DESC_DEVICE_HARDWARE_VERSION:
        status = camInfo.DevInfo_Get_Hardware_Ver(pObj->hDevice,pContext,pSize);
        break;
    case DESC_DEVICE_NAME:
        status = camInfo.DevInfo_Get_Device_Name(pObj->hDevice,pContext,pSize);
        break;
    case DESC_DEVICE_BAYER_LAYOUT:
        status = camInfo.DevInfo_Get_Bayer_Layout(pObj->hDevice,pContext,pSize);
        break;
    case DESC_DEVICE_SOFTWARE_VERSION:
        status = camInfo.DevInfo_Get_SoftWare_Ver(pObj->hDevice,pContext,pSize);
        break;
    case DESC_DEVICE_FPGA_VERSION:
        status = camInfo.DevInfo_Get_FPGA_Ver(pObj->hDevice,pContext,pSize);
        break;
    case DESC_DEVICE_VENDOR_NAME:
        status = camInfo.DevInfo_Get_Vender_Name(pObj->hDevice,pContext,pSize);
        break;
    default:
        status = STATUS_PARAMETER_INVALID;
        break;

    }

    GlobalUnlock(pObj);
    return status;
}



/*
	Function:
		HVCheckDevice
	Input:
		HHV hhv						Handle of device
		HV_CHECK_PARAM Param		Check item
		BOOL *pStatus				Pointer to check status
	Output:
		HVSTATUS					external error code
	Description:

 */
extern "C" HVSTATUS __stdcall HVCheckDevice(HHV hhv, HV_CHECK_PARAM Param, BOOL *pStatus)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
    HVOBJ *pObj = NULL;
    int nRVal = 1;
    FARPROC pFunc = NULL;

    //Check range of parameters
    VERIFY_POINTER(pStatus, STATUS_PARAMETER_INVALID);
    *pStatus = FALSE;

    pObj = (HVOBJ *)GlobalLock( hhv );
    VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);

//	status = DevGetFuncAddr(pObj->type, "CheckDevice", &pFunc,  true, true);
//	if (HV_SUCCESS(status)){
//		nRVal = (* (FUNC_HVCheckDevice *)pFunc)(pObj->hDevice,
//				Param.byParam1, Param.byParam2, Param.byParam3, Param.byParam4, pStatus);
//		if (0 != nRVal){
//			status = (HVSTATUS)nRVal;
//		}
//	}

    GlobalUnlock(hhv);
    return status;
}



/*
	Function:
		HVDeviceMemRead
	Input:
		HHV hhv						Handle of card device
		DWORD dwOffset
		BYTE *pBuffer
		DWORD dwLength
	Output:
		HVSTATUS					external error code
	Description:

 */
extern "C" HVSTATUS __stdcall HVDeviceMemRead(HHV hhv, DWORD dwOffset, BYTE *pBuffer, DWORD dwLength)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
    HVOBJ *pObj = NULL;
    int nRVal = 1;
    FARPROC pFunc = NULL;

    //Check range of parameters
    VERIFY_POINTER(pBuffer, STATUS_PARAMETER_INVALID);

    pObj = (HVOBJ *)GlobalLock( hhv );
    VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);

    /*
    //2021 to do...
    HV_ARG_USER_MEM argMem_Get;
    argMem_Get.nLong=dwLength;
    argMem_Get.ptr=(DWORD*)pBuffer;
    argMem_Get.nOffset=dwOffset;
    status = HVControl(pObj->hDevice,ORD_GET_USER_MEM,&argMem_Get,0,0,0,0);
    */

    GlobalUnlock(hhv);
    return status;
}



/*
	Function:
		HVDeviceMemWrite
	Input:
		HHV hhv						Handle of card device
		DWORD dwOffset
		BYTE *pBuffer
		DWORD dwLength
	Output:
		HVSTATUS					external error code
	Description:

 */
extern "C" HVSTATUS __stdcall HVDeviceMemWrite(HHV hhv, DWORD dwOffset, BYTE *pBuffer, DWORD dwLength)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
    HVOBJ *pObj = NULL;
    int nRVal = 1;
    FARPROC pFunc = NULL;

    //Check range of parameters
    VERIFY_POINTER(pBuffer, STATUS_PARAMETER_INVALID);

    pObj = (HVOBJ *)GlobalLock( hhv );
    VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);

//	status = DevGetFuncAddr(pObj->type, "DeviceMemWrite", &pFunc,  true, true);
//	if (HV_SUCCESS(status)){
//		nRVal = (* (FUNC_HVDeviceMemWrite *)pFunc)(pObj->hDevice, dwOffset, pBuffer, dwLength);
//		if (0 != nRVal){
//			status = STATUS_INTERNAL_ERROR;
//		}
//	}

    GlobalUnlock(hhv);
    return status;
}


//向下兼容接口
extern "C" HVSTATUS __stdcall HVSetShutterUnit(HHV hhv, SHUTTER_UNIT_VALUE unit)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
    HVOBJ *pObj = NULL;
    int nRVal = 1;
    FARPROC pFunc = NULL;
    pObj = (HVOBJ *)GlobalLock( hhv );
    VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);

    HV_ARG_SET_ADV_FEATURE InBuf;
    InBuf.AdvFeatureId = ADV_FEATURE_ID_SHUTTER_UNIT;
    InBuf.AdvFeatureItemId = ADV_FEATURE_ITEM_ID_SELECTOR_MODE;
    InBuf.AdvFeature.Selector.Mode = unit;
    status = HVControl(pObj->hDevice,
                       ORD_SET_ADV_FEATURE,
                       &InBuf,sizeof(HV_ARG_SET_ADV_FEATURE),
                       NULL,0,
                       0);
    GlobalUnlock(hhv);// add by lvxf 2009-07-02
    return status;
}
extern "C" HVSTATUS __stdcall HV1394SetCameraBrightness(HHV hhv, unsigned long value)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
    return status;
}
extern "C" HVSTATUS __stdcall HV1394SetGain(HHV hhv, long dwValue)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;


    return status;
}
extern "C" HVSTATUS __stdcall HV1394FreezeFrame(HHV hCamera, int nFreeze)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;

    return status;
}
extern "C"HVSTATUS __stdcall HV1394SetBytesPerPacket(HHV hhv, int *nLen)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;

    return status;
}
extern "C"HVSTATUS __stdcall HV1394SetWhiteBalance(HHV hhv, int u_b, int v_r)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;

    return status;

}
extern "C"HVSTATUS __stdcall HV1394LoadLUT(HHV hhv, DWORD* ptr,int nLong)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;

    return status;
}
extern "C"HVSTATUS __stdcall HV1394ReadBlock(HHV hhv,unsigned long ulOffset,unsigned long ulLength,unsigned long* pBuf)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
    HVOBJ *pObj = NULL;
    pObj = (HVOBJ *)GlobalLock( hhv );
    VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);

    HV_ARG_READ_BLOCK argReadBlock;
    argReadBlock.dwAddress = (DWORD)ulOffset;
    argReadBlock.nLength = ulLength;
    HV_RES_READ_BLOCK resReadBlock;
    resReadBlock.pData = (BYTE*)pBuf;

    status =HVControl(pObj->hDevice,(HV_CONTROL_CODE)ORD_READ_BLOCK,
                      &argReadBlock,sizeof(HV_ARG_READ_BLOCK),
                      &resReadBlock,sizeof(HV_RES_READ_BLOCK),0);
    GlobalUnlock(hhv);// add by lvxf 2009-07-02
    return status;
}
extern "C"HVSTATUS __stdcall HV1394WriteBlock(HHV hhv,unsigned long ulOffset,unsigned long ulLength,unsigned long* pBuf)
{
    HVSTATUS status = STATUS_OK;
    HVOBJ *pObj = NULL;
    pObj = (HVOBJ *)GlobalLock( hhv );
    VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);

    HV_ARG_WRITE_BLOCK argWriteBlock;
    argWriteBlock.dwAddress = (DWORD)ulOffset;
    argWriteBlock.pData = (BYTE*)pBuf;
    argWriteBlock.nLength = ulLength;
    status = HVControl(pObj->hDevice,(HV_CONTROL_CODE)ORD_WRITE_BLOCK,
                       &argWriteBlock,0,0,0,0);
    GlobalUnlock(hhv);// add by lvxf 2009-07-02
    return status;
}
extern "C"HVSTATUS __stdcall HV1394ReadQuadlet(HHV hhv, unsigned long address, unsigned long *pData)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
    HVOBJ *pObj = NULL;
    pObj = (HVOBJ *)GlobalLock( hhv );
    VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);

    HV_ARG_READ_QUADLET argReadQualdet;
    argReadQualdet.dwAddress=(DWORD)address;
    HV_RES_READ_QUADLET resReadQualdet;
    status =HVControl(pObj->hDevice,(HV_CONTROL_CODE)ORD_READ_QUADLET,&argReadQualdet,0,&resReadQualdet,0,0);

    GlobalUnlock(hhv);// add by lvxf 2009-07-02
    *pData = resReadQualdet.dwData;

    return status;
}
extern "C"HVSTATUS __stdcall HV1394WriteQuadlet(HHV hhv, unsigned long address, unsigned long data)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
    HVOBJ *pObj = NULL;
    pObj = (HVOBJ *)GlobalLock( hhv );
    VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);

    HV_ARG_WRITE_QUADLET argWQt;
    argWQt.dwAddress=(DWORD)address;
    argWQt.dwData=(DWORD)data;
    status =HVControl(pObj->hDevice,(HV_CONTROL_CODE)ORD_WRITE_QUADLET,&argWQt,0,0,0,0);

    GlobalUnlock(hhv);// add by lvxf 2009-07-02
    return status;
}
extern "C" HVSTATUS __stdcall HVSetColorCode(HHV hhv,COLOR_CODE code)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;

    return status;
}
extern "C"HVSTATUS __stdcall HV1394SetUserData(HHV hhv, BYTE* pData,DWORD dwLen)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;

    return status;
}
extern "C"HVSTATUS __stdcall HV1394GetUserData(HHV hhv, BYTE* pData,DWORD dwLen)
{
    HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;

    return status;
}
extern "C" BOOL __stdcall HV1394CheckUserPassWD(HHV hhv, char* pData)
{
    BOOL bState = FALSE;
    return bState;

}


BOOL IsHVFSerial(HHV hhv)
{
    BOOL bHVFserial = FALSE;
    HVTYPE type;
	int  size  = 0;
	CCamera  camInfo;
	camInfo.DevInfo_Get_Device_Type(hhv,NULL,&size);
    camInfo.DevInfo_Get_Device_Type(hhv,&type,&size);
    switch (type)
    {
    case HV1300FCTYPE:
    case HV1300FMTYPE:
    case HV2000FCTYPE:
    case HV3100FCTYPE:
        bHVFserial = TRUE;
        break;
    default:
        bHVFserial = FALSE;
        break;
    }
    return bHVFserial;
}




