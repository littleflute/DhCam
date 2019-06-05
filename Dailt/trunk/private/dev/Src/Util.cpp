#include "stdafx.h"


/*
	Function:
		HVStartSnapInternal
	Input:
		HHV hhv						Handle of card device
	Output:
		HVSTATUS					external error code
	Description:
		Start usb snap 
 */
extern "C" HVSTATUS __stdcall HVStartSnapInternal(HHV hhv)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	HVOBJ *pObj = NULL;
	FARPROC pFunc = NULL;
	int nRVal = 1;
	
	pObj = (HVOBJ *)GlobalLock( hhv );
	VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);
	
	//if usb is not idle, return device status
	if (pObj->HvStatus != HW_NONE){
		status = STATUS_IN_WORK;
	}
	else{
//		status = DevGetFuncAddr(pObj->type, "StartSnap", &pFunc, true, true);
//		if (HV_SUCCESS(status)){
//			nRVal = (* (FUNC_HVStartSnap *)pFunc)(pObj->hDevice);
//			if (0 != nRVal){
//				status = STATUS_INTERNAL_ERROR;
//			}
//			else{
//				// Set flag for snapping internal
//				pObj->HvStatus = HW_SNAP_INT;	 
//			}
//		}	
	}

	GlobalUnlock(hhv);

	return status;	
}



/*
	Function:
		HVStopSnapInternal
	Input:
		HHV hhv						Handle of card device
		
	Output:
		HVSTATUS					external error code
	Description:
	
 */
extern "C" HVSTATUS __stdcall HVStopSnapInternal(HHV hhv)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	HVOBJ *pObj = NULL;
	FARPROC pFunc = NULL;
	int nRVal = 1;
	
	pObj = (HVOBJ *)GlobalLock( hhv );
	VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);

	//if usb is not working, return device status
	if (pObj->HvStatus != HW_SNAP_INT) {
		status = STATUS_NOT_START_SNAP_INT;
	}
	else{
//		status = DevGetFuncAddr(pObj->type, "StopSnap", &pFunc, true, true);
//		if (HV_SUCCESS(status)){
//			nRVal = (* (FUNC_HVStopSnap *)pFunc)(pObj->hDevice);
//			if (0 != nRVal){
//				status = STATUS_INTERNAL_ERROR;
//			}
//			else{
//				pObj->HvStatus = HW_NONE;	//Clear flag
//			}
//		}
	}

	GlobalUnlock(hhv);

	return status;	
}



/*
	Function:
		HVTransfer
	Input:
		HHV hhv						Handle of card device
		BYTE *pBuffer				Pointer to user buffer 
		DWORD *pLength				Pointer to size of data tranfer
	Output:
		HVSTATUS					external error code
	Description:
		if start snap and then call HVTransfer to transfer data 
		form driver to user buffer
 */
extern "C" HVSTATUS __stdcall HVTransfer(HHV hhv, BYTE *pBuffer, DWORD *pBytesReturned)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	HVOBJ *pObj = NULL;
	FARPROC pFunc = NULL;
	int nRVal = 1;
	
	VERIFY_POINTER(pBuffer, STATUS_PARAMETER_INVALID);
	VERIFY_POINTER(pBytesReturned, STATUS_PARAMETER_INVALID);

	pObj = (HVOBJ *)GlobalLock( hhv );
	VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);

	//if usb is not working, do not transfer data
	if (pObj->HvStatus != HW_SNAP_INT) {
		status = STATUS_NOT_START_SNAP_INT;
	}
	else{
//		status = DevGetFuncAddr(pObj->type, "Transfer", &pFunc, true, true);
//		if (HV_SUCCESS(status)){
//			nRVal = (* (FUNC_HVTransfer *)pFunc)(pObj->hDevice, pBuffer, pBytesReturned);
//			if (0 != nRVal){
//				status = STATUS_INTERNAL_ERROR;
//			}
//		}
	}

	GlobalUnlock(hhv);

	return status;	
}



/*
	Function:
		HVVerifyPassword
	Input:
		HHV hhv						Handle of card device
		BOOL *pStatus
	Output:
		HVSTATUS					external error code
	Description:
		
 */
extern "C" HVSTATUS __stdcall HVVerifyPassword(HHV hhv, BOOL *pStatus)
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
	
//	status = DevGetFuncAddr(pObj->type, "VerifyPassword", &pFunc,  true, true);
//	if (HV_SUCCESS(status)){
//		nRVal = (* (FUNC_HVVerifyPassword *)pFunc)(pObj->hDevice, pStatus);
//		if (0 != nRVal){
//			status = STATUS_INTERNAL_ERROR;
//		}
//	}
	
	GlobalUnlock(hhv);
	return status;
}



/*
	Function:
		HVGetI2CParam
	Input:
		HHV hhv						Handle of card device
		WORD wStart
		WORD *pBuffer
		WORD wEntries
	Output:
		HVSTATUS					external error code
	Description:
		
 */
extern "C" HVSTATUS __stdcall HVGetI2CParam(HHV hhv, WORD wStart, WORD *pBuffer, WORD wEntries)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;  

	GlobalUnlock(hhv);
	return status;
}



/*
	Function:
		HVSetI2CParam
	Input:
		HHV hhv						Handle of card device
		WORD wStart
		WORD *pBuffer
		WORD wEntries
	Output:
		HVSTATUS					external error code
	Description:
		
 */
extern "C" HVSTATUS __stdcall HVSetI2CParam(HHV hhv, WORD wStart, WORD *pBuffer, WORD wEntries)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;  
	return status;
}



/*
	Function:
		HVBulkRead
	Input:
		HHV hhv						Handle of card device
		BYTE *pBuffer
		DWORD dwLength
	Output:
		HVSTATUS					external error code
	Description:
		
 */
extern "C" HVSTATUS __stdcall HVBulkRead(HHV hhv, BYTE *pBuffer, DWORD dwLength)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;  
	return status;
}



/*
	Function:
		HVBulkWrite
	Input:
		HHV hhv						Handle of card device
		BYTE *pBuffer
		DWORD dwLength
	Output:
		HVSTATUS					external error code
	Description:
		
 */
extern "C" HVSTATUS __stdcall HVBulkWrite(HHV hhv, BYTE *pBuffer, DWORD dwLength)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;  
	return status;
}


extern "C" HVSTATUS __stdcall HVGetTypeFromIndex(int nDevice, HVTYPE *pType, int *pIndex)
{
  HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
  
  if ( ! GetTypeFromIndex(nDevice, pType, pIndex) )
    status = STATUS_NO_DEVICE_FOUND;
  else  
	  status = STATUS_OK;

  return status;
}

extern "C" HVSTATUS __stdcall HVGetDevInterfaceFromIndex(int nDevice, HV_DEVICE_INTERFACE *pDeviceInterface)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;

	if (nDevice <= 0)  return  STATUS_PARAMETER_INVALID;
	
	if ( ! GetDevInterfaceFromIndex(nDevice, pDeviceInterface) )
		status = STATUS_NO_DEVICE_FOUND;
	else  
		status = STATUS_OK;
	
	return status;
}

extern "C" HVSTATUS __stdcall HVGetDeviceGUID(GUID *pGUID, DWORD cb, DWORD *cbNeeded)
{
	if ( cbNeeded == NULL )
		return STATUS_PARAMETER_INVALID;
	
	return GetDeviceGUID(pGUID, cb, cbNeeded);
}

/*
	Function:
		HV8051Run
	Input:
		HHV hhv						Handle of device
	Output:
		HVSTATUS					external error code
		Description:
		
 */
extern "C" HVSTATUS __stdcall HV8051Run(HHV hhv)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	HVOBJ *pObj = NULL;
	FARPROC pFunc = NULL;
	int nRVal = 0;
	
	pObj = (HVOBJ *)GlobalLock( hhv );
	VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);
	
	//Abort snap process
//	status = DevGetFuncAddr(pObj->type, "8051Run", &pFunc, true, true);
//	if (HV_SUCCESS(status)){
//		nRVal = (* (FUNC_HV8051Run *)pFunc)(pObj->hDevice);  
//		if (0 != nRVal){
//			status = (HVSTATUS)nRVal;
//		}
//	}
		
	GlobalUnlock(hhv);
	return status;
}



/*
	Function:
		HV8051Hold
	Input:
		HHV hhv						Handle of device
	Output:
		HVSTATUS					external error code
	Description:
	
 */
extern "C" HVSTATUS __stdcall HV8051Hold(HHV hhv)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	HVOBJ *pObj = NULL;
	FARPROC pFunc = NULL;
	int nRVal = 0;
	
	pObj = (HVOBJ *)GlobalLock( hhv );
	VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);
	
	//Abort snap process
//	status = DevGetFuncAddr(pObj->type, "8051Hold", &pFunc, true, true);
//	if (HV_SUCCESS(status)){
//		nRVal = (* (FUNC_HV8051Hold *)pFunc)(pObj->hDevice);  
//		if (0 != nRVal){
//			status = (HVSTATUS)nRVal;
//		}
//	}
		
	GlobalUnlock(hhv);
	return status;
}



/*
	Function:
		HVAnchorDownload
	Input:
		HHV hhv						Handle of device
	Output:
		HVSTATUS					external error code
	Description:
	
 */
extern "C" HVSTATUS __stdcall HVAnchorDownload(HHV hhv, BYTE *pBuffer, DWORD Length)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	HVOBJ *pObj = NULL;
	FARPROC pFunc = NULL;
	int nRVal = 0;
	
	//Check range of parameters
	VERIFY_POINTER(pBuffer, STATUS_PARAMETER_INVALID);

	pObj = (HVOBJ *)GlobalLock( hhv );
	VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);

//	status = DevGetFuncAddr(pObj->type, "AnchorDownload", &pFunc, true, true);
//	if (HV_SUCCESS(status)){
//		nRVal = (* (FUNC_HVAnchorDownload *)pFunc)(pObj->hDevice, pBuffer, Length);  
//		if (0 != nRVal){
//			status = (HVSTATUS)nRVal;
//		}
//	}
		
	GlobalUnlock(hhv);
	return status;
}



/*
	Function:
		HVLoadEEPROM
	Input:
		HHV hhv						Handle of device
	Output:
		HVSTATUS					external error code
	Description:
	
 */
extern "C" HVSTATUS __stdcall HVLoadEEPROM(HHV hhv, BYTE *pBuffer, DWORD Length)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	HVOBJ *pObj = NULL;
	FARPROC pFunc = NULL;
	int nRVal = 0;
	
	//Check range of parameters
	VERIFY_POINTER(pBuffer, STATUS_PARAMETER_INVALID);

	pObj = (HVOBJ *)GlobalLock( hhv );
	VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);
	
//	status = DevGetFuncAddr(pObj->type, "LoadEEPROM", &pFunc, true, true);
//	if (HV_SUCCESS(status)){
//		nRVal = (* (FUNC_HVLoadEEPROM *)pFunc)(pObj->hDevice, pBuffer, Length);  
//		if (0 != nRVal){
//			status = (HVSTATUS)nRVal;
//		}
//	}
		
	GlobalUnlock(hhv);
	return status;
}

extern "C" HVSTATUS __stdcall HVDeviceControl(HHV hhv, HV_CONTROL_CODE ControlCode, void *pContext, int *pLength)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE; 
	return status;
}

extern "C" HVSTATUS __stdcall HVCommand(HHV hhv, HV_COMMAND_CODE CommandCode, void *pContext, int *pLength)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	HVOBJ *pObj = NULL;
	FARPROC pFunc = NULL;
	int nRVal = 0;
	
	pObj = (HVOBJ *)GlobalLock( hhv );
	VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);       
	 
	switch(CommandCode) {		
	case CMD_HVAPI_CONTROL:
		{
			HVAPI_CONTROL_PARAMETER *pCtrlParam = (HVAPI_CONTROL_PARAMETER*)pContext;
			
			if( ORD_QUERY_STRING_DESCRIPTOR == pCtrlParam->code)
			{		
				HV_ARG_QUERY_STRING_DESCRIPTOR *pInBuf = (HV_ARG_QUERY_STRING_DESCRIPTOR*)pCtrlParam->pInBuf;
				if(pInBuf->StringId == STRING_ID_HVDAILT_VERSION)
				{
					HV_RES_QUERY_STRING_DESCRIPTOR *pOutBuf = (HV_RES_QUERY_STRING_DESCRIPTOR*)pCtrlParam->pOutBuf;
					pOutBuf->dwTotalByteLength = 7;
					status = STATUS_OK;
					return status;
				}
			}
			else if( ORD_GET_STRING == pCtrlParam->code)
			{
				HV_ARG_GET_STRING *pInBuf = (HV_ARG_GET_STRING*)pCtrlParam->pInBuf;
				if(pInBuf->StringId == STRING_ID_HVDAILT_VERSION)
				{
					HV_RES_GET_STRING *pOutBuf = (HV_RES_GET_STRING*)pCtrlParam->pOutBuf;

					VERSION version = GetLibraryVersion("HVDAILT.dll");
					sprintf(pOutBuf->pBuffer,"%d.%d.%d.%d",version.wMajor,version.wMinor,version.wRevision,version.wBuild);					
					status = STATUS_OK;
					return status;
				}
			}
			
			status = HVControl(pObj->hDevice,
				pCtrlParam->code,
				pCtrlParam->pInBuf,
				pCtrlParam->dwInBufSize,
				pCtrlParam->pOutBuf,
				pCtrlParam->dwOutBufSize,
				pCtrlParam->pBytesRet);
		}
		break;
	case CMD_SPEC_FUN_INTERFACE1:  
		{ 
			CCamera camIF1;
			status = camIF1.Fun_Interface1(pObj->hDevice,pContext); 
		}
		break;
    case CMD_SET_STROBE_ON: 
		{			
			CCamera camSetStrobeOn;
			status = camSetStrobeOn.Set_Strobe_On_Off(pObj->hDevice,1); 
		}
        break;
	case CMD_SET_STROBE_OFF: 
		{
			CCamera camSetStrobeOff;
			status = camSetStrobeOff.Set_Strobe_On_Off(pObj->hDevice,0); 
		}
		break;
	case CMD_FRAME_SHOT:
		{
			CCamera camFrameShot;
			status = camFrameShot.Set_Freeze_Frame_On_Off(pObj->hDevice,1);
		}
		break;
	case CMD_FRAME_SHOT_FINISH:		
		{
			CCamera camFrameShotFinish;
			status = camFrameShotFinish.Set_Freeze_Frame_On_Off(pObj->hDevice,0); 
		}
		break;
	case CMD_GET_LUT_DWENTRIES:// 2021
	//	pLut=(HV_CMD_LUT_DWENTRIES *)pContext;		
	//	HV_ARG_LUT argLut_Get;
	//	argLut_Get.nLong=pLut->dwEntries;
	//	argLut_Get.ptr=pLut->pBuffer;
	//	argLut_Get.nOffset=pLut->dwOffset;
	//	status = HVControl(pObj->hDevice,ORD_GET_LUT,&argLut_Get,0,0,0,0);

		break;
	case CMD_SET_LUT_DWENTRIES: 
		{
			CCamera camSetLut;
			status = camSetLut.Set_LUT(pObj->hDevice,pContext); 
		} 
		break;
	case CMD_GET_BYTE_PER_PACKET:
		break;
	case CMD_SET_BYTE_PER_PACKET: 
		{
			CCamera camSetPacketSize;
			status = camSetPacketSize.Set_Packet_Size(pObj->hDevice,pContext); 
		} 
		break;

	case CMD_1394_READ_QUADLET:// 2021 
		break;
	case CMD_1394_WRITE_QUADLET:// 2021 
		break;
	case CMD_1394_READ_BLOCK:// 2021
		break;
	case CMD_1394_WRITE_BLOCK:// 2021

		break;
 
	default:
		break;
	} 

	GlobalUnlock(hhv);
	return status;
}




/*
	Function:
		HVPeriDeviceControl
	Input:
		HHV hhv						Handle of device
		BYTE byParam
		long lValue
	Output:
		HVSTATUS					external error code
	Description:
		
 */
extern "C" HVSTATUS __stdcall HVPeriDeviceControl(HHV hhv, BYTE byParam, long lValue)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE; 
	HVOBJ *pObj = NULL;
	int nRVal = 1;
	FARPROC pFunc = NULL;
		
	//Check range of parameters
	if (byParam < PERIDEV_BICOLOR_LAMP1 || byParam > PERIDEV_ARRIVE_CLEAR) {
		return STATUS_PARAMETER_OUT_OF_BOUND;
	}

	pObj = (HVOBJ *)GlobalLock( hhv );
	VERIFY_POINTER(pObj, STATUS_DEVICE_HANDLE_INVALID);
	
//	status = DevGetFuncAddr(pObj->type, "PeriDeviceControl", &pFunc,  true, true);
//	if (HV_SUCCESS(status)){
//		nRVal = (* (FUNC_HVPeriDeviceControl *)pFunc)(pObj->hDevice, byParam, lValue);
//		if (0 != nRVal){
//			status = (HVSTATUS)nRVal;
//		}
//	}
	
	GlobalUnlock(hhv);
	return status;
}



/*
	Function:
		HVGetArriveStatus
	Input:
		HHV hhv						Handle of card device
		BYTE *pStatus
	Output:
		HVSTATUS					external error code
	Description:
		
 */
extern "C" HVSTATUS __stdcall HVGetArriveStatus(HHV hhv, BYTE *pStatus)
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
	
//	status = DevGetFuncAddr(pObj->type, "GetArriveStatus", &pFunc,  true, true);
//	if (HV_SUCCESS(status)){
//		nRVal = (* (FUNC_HVGetArriveStatus *)pFunc)(pObj->hDevice, pStatus);
//		if (0 != nRVal){
//			status = STATUS_INTERNAL_ERROR;
//		}
//	}
	
	GlobalUnlock(hhv);
	return status;
}



