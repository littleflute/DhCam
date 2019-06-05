//File: rtl.cpp

#include "CAMOBJ.h"

#define DEFINEGUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
        EXTERN_C const GUID name \
                = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

DEFINEGUID(GUID_1394CMDR, 0x4007067, 0x789b, 0x40aa, 0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca);

//////////////////////////////////////////////////////////////////////////////////////////


DWORD GetCameraInfo
(TCHAR* szDeviceName, PCAMERA_INFO pCell)
{
    TCHAR cellTag = TEXT('#');
    TCHAR VendorTag = TEXT('&');
    TCHAR* pBeg;
    TCHAR* p;
    TCHAR* pNew;
    TCHAR pDeviceName[256];

    //
    // Copy Device name and empty pCell buffer.
    lstrcpy(pDeviceName, szDeviceName);
    pBeg = &pDeviceName[4];
    memset(pCell, 0, sizeof(CAMERA_INFO));

    //
    // Retrieve camera type, as "1394".
    p = _tcschr(pDeviceName, cellTag);
    if (p != NULL) {
        pNew = p;
        *p = 0;
    } else {
        return STATUS_INVALID_PARAMETER;
    }

    lstrcpy(pCell->BusName, pBeg);

    //
    // Move to the next cell, and retrieve camera
    // vendor name.
    pBeg = ++pNew;
    p = _tcschr(pBeg, VendorTag);
    if (p != NULL) {
        pNew = p;
        *p = 0;
    } else {
        return STATUS_INVALID_PARAMETER;
    }

    lstrcpy(pCell->VendorName, pBeg);

    //
    // Move to the next cell, retrieve camera 
    // model name.
    pBeg = ++pNew;
    p = _tcschr(pBeg, cellTag);
    if (p != NULL) {
        pNew = p;
        *p = 0;
    } else {
        return STATUS_INVALID_PARAMETER;
    }
    
    lstrcpy(pCell->ModelName, pBeg);

    //
    // Move to the next cell, retrieve camera 
    // unqueue ID.
    pBeg = ++pNew;
    p = _tcschr(pBeg, cellTag);
    if (p != NULL) {
        pNew = p;
        *p = 0;
    } else {
        return STATUS_INVALID_PARAMETER;
    }
    
    lstrcpy(pCell->UniqueID, pBeg);

    return STATUS_OK;
}

/*
 * ReadBlock	用块读取方式,从给定地址读取指定长度的数据块
 * 
 * szDeviceName	设备名
 * ulOffset		读取数据的偏移地址
 * ulLength		读取数据的长度,长度必须为4的倍数(本函数没有判断)
 * pBuf			数据存放地址
 *
 * 返回值		成功：0
				错误：－1(0xFFFFFFFF)，
  */
DWORD
ReadBlock(
	TCHAR* szDeviceName,
	DWORD ulOffset,
	DWORD ulLength,//读取数据的长度
	PUCHAR pBuf//数据存放地址
	)
{
	HANDLE hDevice;
	DWORD dwRet=0,bytesRet;
	ASYNC_READ asyncRead;
	DWORD i = 0;

	//开辟传给驱动的存储空间
	DWORD           ulBufferSize;
	PASYNC_READ     pAsyncRead = NULL;

	if(pBuf == NULL)	//pBuf不能为空
		return -1;

	asyncRead.bRawMode = FALSE;
	asyncRead.bGetGeneration = TRUE;			
	asyncRead.ulGeneration = 0;
	//地址
	asyncRead.DestinationAddress.IA_Destination_Offset.Off_High = 0;
	asyncRead.DestinationAddress.IA_Destination_Offset.Off_Low = ulOffset;
	//读取的字节数
	asyncRead.nNumberOfBytesToRead = ulLength;
	asyncRead.nBlockSize = 0;
	asyncRead.fulFlags = 0;


	ulBufferSize = sizeof(ASYNC_READ) + asyncRead.nNumberOfBytesToRead;//字节为单位
	pAsyncRead = (PASYNC_READ)LocalAlloc(LPTR, ulBufferSize);
	if(pAsyncRead != NULL)
	{
		FillMemory(pAsyncRead, ulBufferSize, 0);  //开辟的每一个字节都为零
		memcpy(pAsyncRead,&asyncRead,sizeof(ASYNC_READ));
		
		hDevice = OpenDevice(szDeviceName,FALSE);
		if(hDevice != INVALID_HANDLE_VALUE)
		{
			dwRet = DeviceIoControl( hDevice,
				IOCTL_ASYNC_READ,
				pAsyncRead,
				ulBufferSize,
				pAsyncRead,
				ulBufferSize,
				&bytesRet,
				NULL
				);
			if(!dwRet)
			{
				dwRet = GetLastError();
			} 
			else 
			{
//				memcpy(pBuf,pAsyncRead->Data,ulLength);
				//把读取的数据从BE方式转化为LE的方式
				for(i=0;i<ulLength;i+=4)
				{
					pBuf[i] = pAsyncRead->Data[i+3];
					pBuf[i+1] = pAsyncRead->Data[i+2];
					pBuf[i+2] = pAsyncRead->Data[i+1];
					pBuf[i+3] = pAsyncRead->Data[i];
				}
				dwRet = 0;

			}
			CloseHandle(hDevice);
		} 
		else 
		{
			dwRet = -1;
		}
		LocalFree(pAsyncRead);
	}

	return dwRet;
}

/*
 * WriteBlock	用块读取方式,向设备的指定的地址写指定长度的数据块
 * 
 * szDeviceName	设备名
 * ulOffset		写数据的偏移地址
 * ulLength		写数据的长度,长度必须为4的倍数(本函数没有判断)
 * pBuf			数据存放地址
 *
 * 返回值		成功：0
				错误：－1(0xFFFFFFFF)，
  */
DWORD
WriteBlock(
	TCHAR* szDeviceName,
	DWORD ulOffset,
	DWORD ulLength,
	PUCHAR pBuf
	)
{
	HANDLE hDevice;
	DWORD dwRet=0,bytesRet;
	ASYNC_WRITE asyncWrite;

	DWORD           ulBufferSize;
	PASYNC_WRITE    pAsyncWrite = NULL;

	if (pBuf == NULL)
		return -1;

	asyncWrite.bRawMode = FALSE;
	asyncWrite.bGetGeneration = TRUE;			
	asyncWrite.ulGeneration = 0;
	asyncWrite.DestinationAddress.IA_Destination_Offset.Off_High = 0;
	asyncWrite.DestinationAddress.IA_Destination_Offset.Off_Low = ulOffset;
	asyncWrite.nNumberOfBytesToWrite = ulLength;
	asyncWrite.nBlockSize = 0;
	asyncWrite.fulFlags = 0;

	ulBufferSize = sizeof(ASYNC_WRITE) + asyncWrite.nNumberOfBytesToWrite;//字节为单位
	pAsyncWrite = (PASYNC_WRITE)LocalAlloc(LPTR, ulBufferSize);
	if (pAsyncWrite != NULL) {
		FillMemory(pAsyncWrite, ulBufferSize, 0);  //开辟的每一个字节都为零
		memcpy(pAsyncWrite,&asyncWrite,sizeof(ASYNC_WRITE));
		memcpy(pAsyncWrite->Data,pBuf,ulLength);//将数据拷贝给pAsyncWrite->Data
		
		hDevice = OpenDevice(szDeviceName,FALSE);
		if (hDevice != INVALID_HANDLE_VALUE) {
			dwRet = DeviceIoControl( hDevice,
				IOCTL_ASYNC_WRITE,
				pAsyncWrite,
				ulBufferSize,
				pAsyncWrite,
				ulBufferSize,
				&bytesRet,
				NULL
				);
			if (!dwRet) {
				dwRet = GetLastError();
			} else {
				// WriteBlock: IoCtl Succeeded
				dwRet = 0;
			}
			CloseHandle(hDevice);
		} else {
			dwRet = -1;
		}
        
		LocalFree(pAsyncWrite);
	}

	return dwRet;
}

/*++

Routine Description:

    Convert a multiply resolution to final public resolution
    format.

    The final public resolution format is:

    --------------------------------------------------------
    |                           |                          |
    |          width            |            heigth        |                   
    |                           |                          |
    --------------------------------------------------------

Return Value:

    STATUS_OK if success;
    STATUS_INVALID_PARAMETER if failure.

--*/


__inline VOID ToLowerCase(TCHAR* p)
{
    if (*p >= 0x41 && *p <= 0x5A) {
        *p += 32;
    }
}

DWORD ModelName2Type
(TCHAR* lpModelName)
{
    DWORD retval;
    TCHAR ModelName[64];
    TCHAR temp[8];

    lstrcpy(ModelName, lpModelName);

    for(int i=0; i<64; i++) {
        ToLowerCase(&ModelName[i]);
    }
    
    //
    // If model name does not have 'dh' header, return 
    // NULL immediately.
    if (ModelName[0] != TEXT('d') && 
        ModelName[1] != TEXT('h'))
    {
        return NULL;
    }

    //
    // Retrieve 400from "dh-hv400fc".
    lstrcpyn(temp, &ModelName[5], 5);
    retval = _ttoi(temp);

    retval = ToCameraType(retval, ModelName[9], ModelName[10]);

    return retval;
}

/*
 * ReadRegister
 *
 * Reads a quad-word from the camera CSR regiser at ulOffset
 * places the result in "bytes"
 *
 * Note: the offset is not an absolute offset, it is the relative offset
 * from the first CSR register, which is maintained now in the device extension
 */

DWORD
ReadRegister
(TCHAR* szDeviceName, DWORD ulOffset, PUCHAR bytes)
{
	HANDLE hDevice;
	DWORD dwRet,bytesRet;
	REGISTER_IOBUF regbuf;

	regbuf.ulOffset = ulOffset;
	memset(regbuf.data, 0, 4);
	hDevice = OpenDevice(szDeviceName,FALSE);
	if (hDevice != INVALID_HANDLE_VALUE) {
        dwRet = DeviceIoControl( hDevice,
                                 IOCTL_READ_REGISTER,
                                 &regbuf,
                                 sizeof(REGISTER_IOBUF),
                                 &regbuf,
                                 sizeof(REGISTER_IOBUF),
                                 &bytesRet,
                                 NULL
                                 );
		if (!dwRet) {
			dwRet = GetLastError();
        } else {
			dwRet = 0;
		}
		CloseHandle(hDevice);
	} else {
		dwRet = -1;
	}

	memcpy(bytes,regbuf.data,4);

	return dwRet;
}

/*
 * ReadRegisterUL
 *
 * Same as ReadRegister, except the data is passed in as a pointer to an 
 * unsigned long, with the first "byte" as the most significant byte.
 */

DWORD
ReadRegisterUL
(TCHAR* szDeviceName, DWORD ulOffset, DWORD* pData)
{
    DWORD retval;
    DWORD value;

    retval = ReadRegister(szDeviceName, ulOffset, (PUCHAR)&value);

    //
    // bswap optimize asm code.
    //
    __asm {
        PUSH EDX
        MOV EDX, value
        BSWAP EDX
        MOV value, EDX
        POP EDX
    }

    *pData = value;

    return retval;
}

/*
 * WriteRegister
 *
 * Writes a quad-word to the camera CSR regiser at ulOffset
 * "bytes" represents the quad-word to write
 *
 * Note: the offset is not an absolute offset, it is the relative offset
 * from the first CSR register, which is maintained now in the device extension
 */

DWORD
WriteRegister
(TCHAR* szDeviceName, DWORD ulOffset, PUCHAR bytes)
{
	HANDLE hDevice;
	DWORD dwRet,bytesRet;
	REGISTER_IOBUF regbuf;

	regbuf.ulOffset = ulOffset;
	memcpy(regbuf.data, bytes, 4);

	hDevice = OpenDevice(szDeviceName,FALSE);
	if (hDevice != INVALID_HANDLE_VALUE) {
        dwRet = DeviceIoControl( hDevice,
                                 IOCTL_WRITE_REGISTER,
                                 &regbuf,
                                 sizeof(REGISTER_IOBUF),
                                 &regbuf,
                                 sizeof(REGISTER_IOBUF),
                                 &bytesRet,
                                 NULL
                                 );
		if(!dwRet) {
			dwRet = GetLastError();
		} else {
			dwRet = 0;
		}
		CloseHandle(hDevice);
    } else {
		dwRet = -1;
	}

	return dwRet;
}
/*
 * WriteRegisterUL
 *
 * Same as writeRegister, except the data is passed in as an unsigned long,
 * with the first "byte" as the most significant byte.
 */

DWORD
WriteRegisterUL
(TCHAR* szDeviceName, DWORD ulOffset, DWORD data)
{
    DWORD value = data;

    //
    // bswap optimize asm code.
    //
    __asm {
        PUSH EDX
        MOV EDX, value
        BSWAP EDX
        MOV value, EDX
        POP EDX
    }

	return WriteRegister(szDeviceName, ulOffset, PUCHAR(&value));
}
/*
 * GetDeviceList
 *
 * Routine Description:
 *
 * Checks the GUID_1394CAMERA interface for all 1394 Digital cameras in the system
 * and returns their device names in DeviceData
 *
 * Arguments:
 *
 *   DeviceData: a pointer to an externally allocated DEVICE_DATA structure;
 *
 * Return Value:
 *
 *   the number of cameras discovered, or
 *   -1 on error
 */

DWORD WINAPI GetDeviceList(PCAMERA_1394_LIST DeviceData)
{

	HDEVINFO hDevInfo;
    DWORD i = 0;
    SP_DEVICE_INTERFACE_DATA deviceInterfaceData;
    PSP_DEVICE_INTERFACE_DETAIL_DATA DeviceInterfaceDetailData;
    DWORD requiredSize;
    GUID t1394CmdrGUID;
    DWORD dwError;
    BOOL bReturn;

    t1394CmdrGUID = GUID_1394CMDR;
	memset(DeviceData, 0, sizeof(CAMERA_1394_LIST));

    // Get 1394 class handle.
    hDevInfo = SetupDiGetClassDevs( &t1394CmdrGUID,
                                    NULL,
                                    NULL,
                                    (DIGCF_PRESENT | DIGCF_INTERFACEDEVICE)
                                    );

    if (!hDevInfo) {
        dwError = GetLastError();
		return -1;
    }

    //
	// Then we're going to loop and find all of the
    // 1394 camera interfaces available.
    //
    while (TRUE) {
        deviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
        if (!SetupDiEnumDeviceInterfaces(hDevInfo, NULL, &t1394CmdrGUID, 
            i, &deviceInterfaceData))
		{
            dwError = GetLastError();
			if (dwError != ERROR_NO_MORE_ITEMS) {
				return -1;
			} else {
				break;
			}
        }

		// figure out the size...
		bReturn = SetupDiGetDeviceInterfaceDetail( hDevInfo,
												   &deviceInterfaceData,
												   NULL,
												   0,
												   &requiredSize,
												   NULL
												   );

		if (!bReturn) {
			dwError = GetLastError();
			if (dwError != ERROR_INSUFFICIENT_BUFFER) {
				SetupDiDestroyDeviceInfoList(hDevInfo);
				return -1;
			}
		} else {
			SetupDiDestroyDeviceInfoList(hDevInfo);
			return -1;
		}

		DeviceInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)LocalAlloc(LPTR, requiredSize);
		if (DeviceInterfaceDetailData == NULL) {
			SetupDiDestroyDeviceInfoList(hDevInfo);
			return -1;
		}
		
		memset(DeviceInterfaceDetailData, 0, requiredSize);
		DeviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
		
		bReturn = SetupDiGetDeviceInterfaceDetail( hDevInfo,
			&deviceInterfaceData,
			DeviceInterfaceDetailData,
			requiredSize,
			NULL,
			NULL
			);

		if (!bReturn) {
			dwError = GetLastError();

			SetupDiDestroyDeviceInfoList(hDevInfo);
			LocalFree(DeviceInterfaceDetailData);

			return -1;
		} else {
			// Copy Device name to our output buffer.
			DeviceData->CameraNum++;
			lstrcpy(DeviceData->CameraList[i].DeviceName, DeviceInterfaceDetailData->DevicePath);
			LocalFree(DeviceInterfaceDetailData);
		}

        i++;
    } // while

	SetupDiDestroyDeviceInfoList(hDevInfo);
	
	return i;
}

DWORD GetMaxSpeedBetweenDevices
(TCHAR* szDeviceName, PGET_MAX_SPEED_BETWEEN_DEVICES GetMaxSpeedBetweenDevices)
{
    HANDLE hDevice;
    DWORD dwRet, dwBytesRet;

    hDevice = OpenDevice(szDeviceName, FALSE);
    if (hDevice != INVALID_HANDLE_VALUE) {
		
        dwRet = DeviceIoControl( hDevice,
			IOCTL_GET_MAX_SPEED_BETWEEN_DEVICES,
			GetMaxSpeedBetweenDevices,
                                 sizeof(GET_MAX_SPEED_BETWEEN_DEVICES),
                                 GetMaxSpeedBetweenDevices,
                                 sizeof(GET_MAX_SPEED_BETWEEN_DEVICES),
                                 &dwBytesRet,
                                 NULL
                                 );

        if (dwRet) {
            dwRet = ERROR_SUCCESS;
            _RPT1(_CRT_WARN, "fulSpeed = 0x%x\r\n", GetMaxSpeedBetweenDevices->fulSpeed);
        } else {
            dwRet = GetLastError();
        }

        CloseHandle(hDevice);
    } else {
        dwRet = GetLastError();
    }
    
    return(dwRet);
}


/*
 * ResetCameraState
 *
 * Causes the device driver to clear out any resources previously allocated
 * and move the camera into a passive state
 *
 */

void
ResetCameraState
(TCHAR* szDeviceName)
{
	HANDLE hDevice;
	DWORD dwRet,bytesRet;

	hDevice = OpenDevice(szDeviceName, FALSE);
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
		_RPT1(_CRT_WARN, "ResetCameraState: Couldn't open handle to %s\n",szDeviceName);
        return;
	}

}


WORD CSR_CRC16(unsigned long *data, int length)
{
    int check=0, i;
    int shift, sum, next=0;
    int temp;
    
    if(data == NULL)
        return 0;
    
    for (i = length; i; i--) 
    {
        for (next = check, shift = 28; shift >= 0; shift -= 4 )
        {
            temp = (BE2LE(*data) >> shift);
            sum = ((next >> 12) ^ temp) & 0xf;
            next = (next << 4) ^ (sum << 12) ^ (sum << 5) ^ (sum);
        }
        check = next & 0xffff;
        data++;
    }
    return (WORD)check;
}

HANDLE OpenDevice
(TCHAR* szDeviceName, BOOL bOverLapped)
{
    HANDLE  hDevice;

    if (bOverLapped) {

        hDevice = CreateFile( szDeviceName,
                              GENERIC_WRITE | GENERIC_READ,
                              FILE_SHARE_WRITE | FILE_SHARE_READ,
                              NULL,
                              OPEN_EXISTING,
                              FILE_FLAG_OVERLAPPED,
                              NULL
                              );
    } else {
    
        hDevice = CreateFile( szDeviceName,
                              GENERIC_WRITE | GENERIC_READ,
                              FILE_SHARE_WRITE | FILE_SHARE_READ,
                              NULL,
                              OPEN_EXISTING,
                              0,
                              NULL
                              );
    }

	if (hDevice == INVALID_HANDLE_VALUE) {
        return NULL;
    }

    return(hDevice);
}

HVSTATUS All_Open(char *DevicePath,HANDLE *phCamera,GUID *pGuid,GUID Guid)
{
	HVSTATUS status = STATUS_OK;

	if ( *pGuid != Guid )
		return STATUS_PARAMETER_INVALID;
	
	
	*phCamera= GlobalAlloc(GHND,sizeof(SVOBJ));
	PCAMOBJ pCamObj = (PCAMOBJ) GlobalLock(*phCamera);
	
	pCamObj->phCamera=OpenDevice(DevicePath,TRUE);
	
	if( pCamObj->phCamera == NULL)
		return STATUS_NO_DEVICE_FOUND;
	
	CCamera* pCam =new CCamera;
	pCamObj->pCam=ToHandle(pCam); 
	
    if (!pCam->OpenCamera(DevicePath)){
		delete pCam;
		pCam = NULL;
		return STATUS_HW_INIT_ERROR;
    }
	
//	pCam->StartBusResetThread();
	GlobalUnlock(pCamObj);
	
	return status;
}

HVSTATUS All_Close(HANDLE hCamera)
{
	HVSTATUS status = STATUS_OK;

    PCAMOBJ pCamObj = (PCAMOBJ) GlobalLock(hCamera);
	
	if ( !CloseHandle(pCamObj->phCamera) )
		return STATUS_DEVICE_HANDLE_INVALID;
	
	CCamera* pCam = (CCamera *)(pCamObj->pCam); 
//	pCam->StopBusResetThread();

	if(pCam) 
	{
	  delete pCam;
	  pCam = NULL;
	}
	
	
    GlobalUnlock(pCamObj);	
	GlobalFree(hCamera);

	return status;
}
bool All_Enumerate(int DevIndex, HV_DEVICE_INTERFACE *pDeviceInterface,GUID Guid_1394)
{
	bool Res = false;
	HDEVINFO hDevInfo = NULL;
	hDevInfo = SetupDiGetClassDevs((struct _GUID *)&Guid_1394, 
								NULL, 
								NULL, 
								(DIGCF_PRESENT | 
								DIGCF_INTERFACEDEVICE)); 
	if (hDevInfo == NULL)
		return false;

	SP_DEVICE_INTERFACE_DATA DevInterfaceData;
	DevInterfaceData.cbSize = sizeof DevInterfaceData;
	if (::SetupDiEnumDeviceInterfaces( hDevInfo, NULL, (struct _GUID *)&Guid_1394,
	  DevIndex, &DevInterfaceData ))
	{
		DWORD reqSize;
		::SetupDiGetDeviceInterfaceDetail( hDevInfo, &DevInterfaceData, NULL, 0, &reqSize, NULL );

		char * const pBuffer = (char*) ::_alloca( reqSize );
		PSP_DEVICE_INTERFACE_DETAIL_DATA pDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA) pBuffer;
		pDetail->cbSize = sizeof *pDetail;
		if (::SetupDiGetDeviceInterfaceDetail( hDevInfo, &DevInterfaceData, pDetail, reqSize, &reqSize, NULL ))
		{
		  strcpy(pDeviceInterface->szDevicePath, pDetail->DevicePath);
		  pDeviceInterface->ClassGUID = Guid_1394;
		  Res = true;
		}
	}
	
	SetupDiDestroyDeviceInfoList(hDevInfo);
	
	return Res;
}