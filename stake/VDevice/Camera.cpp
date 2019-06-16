// Camera.cpp: implementation of the CCamera class.
//
//////////////////////////////////////////////////////////////////////
#include "CamObj.h"
#include "Camera.h"

//global


extern BOOL g_bDeviceRemove;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCamera::CCamera()
{
	memset(m_szDevicePath,0,128);

	m_bStopDisplay=0;

	m_T1=m_T2=0;
	
	m_Width=1392;//1628;  // SV2000FC(1392*1040)
	m_Height=1040;//1236;

	m_Left=0;
	m_Top =0;

	m_nRGain = m_nBGain = m_nGain = 0;
	m_nShutter = 0;
    m_dwTriggerOnOff = 0;
	m_hSoftTrigger = CreateEvent(NULL, FALSE , FALSE, NULL);
    m_hDisplayOver = CreateEvent(NULL, FALSE , FALSE, NULL);
	m_pImageBuffer = NULL;
	
	m_pImageBuffer = new BYTE[nMaxWidth * nMaxHeight * 3];    
	
	if (m_pImageBuffer) {
		FillMemory(m_pImageBuffer, nMaxWidth * nMaxHeight  * 3, 0xff);
	}

	//m_pBmpInfo即指向m_chBmpBuf缓冲区，用户可以自己分配BTIMAPINFO缓冲区	
	m_pBmpInfo								= (BITMAPINFO *)m_chBmpBuf;
	m_pBmpInfo->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	m_pBmpInfo->bmiHeader.biWidth			= 0;
	m_pBmpInfo->bmiHeader.biHeight			= 0;
	m_pBmpInfo->bmiHeader.biPlanes			= 1;
	m_pBmpInfo->bmiHeader.biBitCount		= 24;
	m_pBmpInfo->bmiHeader.biCompression		= BI_RGB;
	m_pBmpInfo->bmiHeader.biSizeImage		= 0;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter	= 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter	= 0;
	m_pBmpInfo->bmiHeader.biClrUsed			= 0;
	m_pBmpInfo->bmiHeader.biClrImportant	= 0;
	
	for (int i = 0; i < 256; i++){
		m_pBmpInfo->bmiColors[i].rgbBlue		= (BYTE)i;
		m_pBmpInfo->bmiColors[i].rgbGreen		= (BYTE)i;
		m_pBmpInfo->bmiColors[i].rgbRed			= (BYTE)i;
		m_pBmpInfo->bmiColors[i].rgbReserved	= 0;	
	}
    
	m_bDeviceRemove = FALSE;  
	
    m_nPicCount = 0;
  
    local_FindMyFile("*jpg",m_hvFile,&m_nPicCount);

}

CCamera::~CCamera()
{
	CloseHandle(m_hSoftTrigger);
	CloseHandle(m_hDisplayOver);
	if (m_pImageBuffer)
	{
		delete[]m_pImageBuffer;
		m_pImageBuffer = NULL;
	}

}

HANDLE CCamera::Get_Device_HANDLE (TCHAR* szDeviceName)
{
    HANDLE  hDevice;
	
	hDevice = CreateFile( szDeviceName,
		GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_OVERLAPPED,
		NULL
		);
    

    return(hDevice);
}

HVSTATUS CCamera::local_OpenSnap(void *pInBuffer)
{
	HV_ARG_OPEN_SNAP * pArgOpenSnap = (HV_ARG_OPEN_SNAP *)pInBuffer;

    m_lpSnapProc = pArgOpenSnap->pSnapFunc;
    m_pSnapParam = pArgOpenSnap->pParam;
	
    return HVSTATUS(0);
}

HVSTATUS CCamera::local_StartSnap(void *pInBuffer)
{
	HV_ARG_START_SNAP * pArgStartSnap = (HV_ARG_START_SNAP *)pInBuffer;
	
    if (pArgStartSnap->nSum < 1 || pArgStartSnap->nSum > 255) {
        return STATUS_PARAMETER_OUT_OF_BOUND;
    }

    for (int i=0; i<pArgStartSnap->nSum; i++) {
        if ((DWORD)pArgStartSnap->ppBuffer[i] < 0x0000FFFF)
            return STATUS_PARAMETER_INVALID;
    }
	
    
    m_BufferNum = pArgStartSnap->nSum;
    m_ppSnapBuffer = new BYTE*[pArgStartSnap->nSum];
    if (m_ppSnapBuffer == NULL) {
        return STATUS_NOT_ENOUGH_SYSTEM_MEMORY;
    } else {
        memcpy(m_ppSnapBuffer, pArgStartSnap->ppBuffer, sizeof(BYTE*) * pArgStartSnap->nSum);
    }
	
    DWORD m_ID;
    	 
	HANDLE hDisplayThread = ::CreateThread(NULL, 0, 
		local_Stub_Display_Fun, (void*)this,
		CREATE_SUSPENDED, &m_ID);
     
    m_bStopDisplay = 0;
	m_hDisplayThread = hDisplayThread;
    SetThreadPriority(hDisplayThread, THREAD_PRIORITY_NORMAL);
    ResumeThread(hDisplayThread);
	while(m_bStopDisplay!=1)
	{
		Sleep(1);	
	}
	
    return HVSTATUS(0);
}

HVSTATUS CCamera::Control(HV_CONTROL_CODE code, 
			 void *pInBuffer, DWORD nInBufferSize, 
			 void *pOutBuffer, DWORD nOutBufferSize)
{
	HVSTATUS status =  STATUS_NOT_SUPPORT_INTERFACE;
	 
	switch(code) {
	case ORD_INIT_CAMERA:
		status=STATUS_OK;
		break;
	case ORD_OPEN_SNAP:
		status = local_OpenSnap(pInBuffer);
		break;
	case ORD_START_SNAP:
		status = local_StartSnap(pInBuffer);
	    break;
	case ORD_STOP_SNAP:
		m_bStopDisplay = 0;
		if (m_dwTriggerOnOff)
			SetEvent(m_hSoftTrigger);
		WaitForSingleObject(m_hDisplayOver, NEARLYINFINITE);
		status=STATUS_OK;
		break;
	case ORD_SNAP_SHOT:
		status = local_SnapShot(pInBuffer);
		break;
	case ORD_SET_FMT7_MODE:
		{
			HV_ARG_SET_FMT7_MODE *pArgSetAOI =(HV_ARG_SET_FMT7_MODE *)pInBuffer;
		  //m_Width = (USHORT)pArgSetAOI->Fmt7Mode.AOI.Width;
		  //m_Height = (USHORT)pArgSetAOI->Fmt7Mode.AOI.Height;
		  //m_Left = (USHORT)pArgSetAOI->Fmt7Mode.AOI.Left;
		  //m_Top = (USHORT)pArgSetAOI->Fmt7Mode.AOI.Top;
		 
		
			status = STATUS_OK;
		}
		break;
		
	case ORD_GET_FEATURE:
		HV_ARG_GET_FEATURE *argGetFeature;
		argGetFeature = (HV_ARG_GET_FEATURE *)pInBuffer;
		HV_RES_GET_FEATURE *resGetFeature;
		resGetFeature = (HV_RES_GET_FEATURE *)pOutBuffer;
	 
		
		switch(argGetFeature->FeatureId) 
		{
		case FEATURE_ID_GAIN:
			resGetFeature->Scalar.Value = m_nGain;
			break;
		case FEATURE_ID_SHUTTER:
			resGetFeature->Scalar.Value = m_nShutter;
			break;
		case FEATURE_ID_BRIGHTNESS:
			resGetFeature->Scalar.Value = m_nBrightness;
			break;
		case FEATURE_ID_WHITE_BALANCE:
			resGetFeature->WhiteBalance.UB = m_nBGain;
			resGetFeature->WhiteBalance.VR = m_nRGain;
			break;
		default:
			break;
		}
		
		status = STATUS_OK;
		break;
	case ORD_SET_FEATURE:
		status = F_SET_FEATURE(pInBuffer);
		break;
	case ORD_CLOSE_SNAP:
	
		status = STATUS_OK;
		break;
	case ORD_QUERY_FMT7_MODE_DESCRIPTOR:
		{
			HV_ARG_QUERY_FMT7_MODE_DESCRIPTOR *pInBuf = (HV_ARG_QUERY_FMT7_MODE_DESCRIPTOR*)pInBuffer;
			HV_RES_QUERY_FMT7_MODE_DESCRIPTOR *pOutBuf = (HV_RES_QUERY_FMT7_MODE_DESCRIPTOR*)pOutBuffer; 
			switch(pInBuf->FuncId) {
			case FMT7_MODE_FUNC_ID_AOI:
				{
					int nHCMax = W1;
					int nVCMax = H1;
					
					switch(pInBuf->VideoMode)
					{
					case VIDEO_MODE0: 
						status = STATUS_OK;
						break;
					case VIDEO_MODE1:
						nHCMax /= 2;
						nVCMax /= 2;
						status = STATUS_OK;
						break;
					
					default:
						status = STATUS_PARAMETER_INVALID;
						break;
					} 
					pOutBuf->AOI.HorizontalMaxInq = nHCMax;//W1; //1392
					pOutBuf->AOI.VerticalMaxInq = nVCMax;//H1;   //1040
					pOutBuf->AOI.VerticalUnitInq = 0;
					pOutBuf->AOI.HorizontalUnitInq = 0;
				
				}
				break;
			case FMT7_MODE_FUNC_ID_COLOR_FILTER:
				 pOutBuf->ColorFilterMode.Filter = COLOR_FILTER_RG_GB;//RGGB
				 status = STATUS_OK;
				break;
			default:
				break;
			}
		}
		break;
	case ORD_QUERY_VIDEO_MODE_PRESENCE: 
		{
			HV_ARG_QUERY_VIDEO_MODE_PRESENCE *pInBuf = (HV_ARG_QUERY_VIDEO_MODE_PRESENCE *)pInBuffer;
			HV_RES_QUERY_VIDEO_MODE_PRESENCE *pOutBufMode = (HV_RES_QUERY_VIDEO_MODE_PRESENCE *)pOutBuffer;
			pOutBufMode->Mode0 = 1;
			pOutBufMode->Mode1 = 1;
			pOutBufMode->Mode2 = 0;
			pOutBufMode->Mode3 = 0;
			pOutBufMode->Mode4 = 0;
			pOutBufMode->Mode5 = 0;
			pOutBufMode->Mode6 = 0;
			status = STATUS_OK;
			
		}
		break;
	case ORD_SET_VIDEO_FORMAT_MODE_FRAME_RATE://设置分辨率
		status = STATUS_OK;
		break;
	case ORD_GET_VIDEO_FORMAT_MODE_FRAME_RATE:
		status = STATUS_OK;
		break;
	case  ORD_QUERY_STRING_DESCRIPTOR:
		_RPT0(_CRT_WARN, "ORD_QUERY_STRING_DESCRIPTORd");
		status = F_ORD_QUERY_STRING_DESCRIPTOR(pInBuffer,pOutBuffer);
		break;
	case  ORD_GET_STRING:
		_RPT0(_CRT_WARN, "ORD_GET_STRING");
		status = F_ORD_GET_STRING(pInBuffer,pOutBuffer);
		break;
    case ORD_SET_STROBE:
		status = STATUS_OK;
		break;
	case ORD_TRIGGER_SHOT:
		if (m_dwTriggerOnOff)
			SetEvent(m_hSoftTrigger);
		status = STATUS_OK;
		break;
	default:
		break;
	}
	
	return STATUS_OK;//status;
}


HVSTATUS CCamera::local_Get_SN(void *pContext, int *pSize)
{
	
	HVSTATUS status = STATUS_PARAMETER_INVALID;
	
	if(pContext && *pSize == 15)
	{
		sprintf((char*)pContext,m_szSN);
	
		status = STATUS_OK;
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

//DESC_DEVICE_FPGA_VERSION
HVSTATUS CCamera::local_Get_FPGA_Ver(void *pContext,int *pSize)
{
	HVSTATUS status = STATUS_PARAMETER_INVALID;
	if(pContext && *pSize == sizeof(DWORD))
	{
		//DWORD dwFPGAVer = 0x11;
		//*((DWORD*)pContext) = (DWORD)dwFPGAVer;
		sprintf((char*)pContext,"2.0.0.8");
		status = STATUS_OK;
		
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
			*(DWORD *)pContext = 4;	 
			status = STATUS_OK;			
		}
	}
	return status;	
}
DWORD CCamera::DisplayThreadProc( )
{
    m_bStopDisplay = 1;
	HV_SNAP_INFO snapInfo;
    snapInfo.hhv = NULL;
    snapInfo.nIndex = 0;
    snapInfo.pParam = m_pSnapParam;
	
    DWORD BufferSize =   W1 * H1; 
    DWORD index = 0;
	
	m_pData = new BYTE[BufferSize];
	BYTE*pTem=new BYTE[m_Width*m_Height*3];

	static int nC=0;
    ResetEvent(m_hSoftTrigger);

    int idex = 0;
	if (m_nPicCount>0)
	{
		FillMemory(m_pImageBuffer, nMaxWidth * nMaxHeight  * 3, 0xff);
		HVLoadJPEG(m_hvFile[idex].szfile, m_pImageBuffer, (int *)&(m_pBmpInfo->bmiHeader.biWidth), 				(int *)&(m_pBmpInfo->bmiHeader.biHeight), (int *)&(m_pBmpInfo->bmiHeader.biBitCount), FALSE);
		
		int nW = m_pBmpInfo->bmiHeader.biWidth;
		int nH = m_pBmpInfo->bmiHeader.biHeight;
		
		BYTE*p = m_pImageBuffer;
		p+= nW*3*m_Top+m_Left*3;
		
		for (int k=0;k<m_Height;k++)
		{
			::CopyMemory(pTem+k*m_Width*3,p,m_Width*3);
			p+=nW*3;			
		}
		local_UnBayerTest(m_Width,m_Height,pTem);
	}
	else
	{				
		if (m_pData) 
		{
			BYTE *pData=m_pData;
			for (int j=0;j<H1;j++)
			{
				for (int i=0;i<W1;i++)
				{
					pData[j*W1+i]=(BYTE)(i+nC & 0xFF);
					
				}			
			}
		}
		nC++;
		//OpenRawFile("image.raw",m_pData);
		
		BYTE*p=m_pData;
		p+=W1*m_Top+m_Left;
		
		for (int k=0;k<m_Height;k++)
		{
			::CopyMemory(pTem+k*m_Width,p,m_Width);
			p+=W1;
		}
		
	}
	
	while (m_bStopDisplay) 
	{
	
		if (g_bDeviceRemove)
		{ 
			g_bDeviceRemove=FALSE;
			break;
		}
		
		if (m_dwTriggerOnOff)
		{
			WaitForSingleObject(m_hSoftTrigger, INFINITE);
		}		
	
		Sleep(1);

		{
			BufferSize =  m_Width*m_Height;
			memcpy(m_ppSnapBuffer[index], pTem, BufferSize);
			snapInfo.nIndex=index;
			index++;
			if (index >= m_BufferNum) {
				index = 0;
			}
			
			(*m_lpSnapProc)(&snapInfo);
			
			m_T1=m_T2=::GetTickCount();
		}

		
	}
    
    delete[]m_pData;
	delete[]pTem;
	SetEvent(m_hDisplayOver);
    _RPT0(_CRT_WARN, "【Display Thread Over】");
    return 0;

}

DWORD WINAPI CCamera::local_Stub_Display_Fun(LPVOID lpParam)
{
    CCamera* pCam = (CCamera*)(lpParam);	
    pCam->DisplayThreadProc();
	return 0;
}

HVSTATUS CCamera::local_SnapShot(void *pInBuffer)
{
	HVSTATUS status = STATUS_OK;
	
	HV_ARG_SNAP_SHOT *pArgSnapShot = (HV_ARG_SNAP_SHOT *)pInBuffer;
    
    if (pArgSnapShot->nSum < 1) {
        return STATUS_PARAMETER_INVALID;
    }	
	
	int nTotalNum = pArgSnapShot->nSum;
	BYTE**ppBuffer=pArgSnapShot->ppBuffer;
	
	
	DWORD BufferSize =  W1 * H1;
	
	m_pData = new BYTE[BufferSize];
	if (m_pData) 
	{
		BYTE *pData=m_pData;
		for (int j=0;j<H1;j++)
		{
			for (int i=0;i<W1;i++)
			{
				pData[j*W1+i]=(BYTE)(i & 0xFF);				
			}			
		}		
	}


//	OpenRawFile("image.raw",m_pData);

	BYTE*pTem=new BYTE[m_Width*m_Height];

    BYTE*p=m_pData;
	p+=W1*m_Top+m_Left;

	for (int k=0;k<m_Height;k++)
	{
	   ::CopyMemory(pTem+k*m_Width,p,m_Width);
	   p+=W1;
	   
	}
	
	if (nTotalNum==1)
	{
		memcpy(ppBuffer[0], pTem, m_Width*m_Height);
	}
	else{                   
		int num=nTotalNum;
		while (num--)
		{
			int nIndex=nTotalNum - num - 1;
			_RPT1(_CRT_WARN, "Buf:%d",nIndex);
			memcpy(ppBuffer[nIndex], pTem, BufferSize);
		}		
	}
	
	
    delete[]m_pData;

	delete[]pTem;
    
	
	return status;

}



BOOL CCamera::OpenRawFile(LPCSTR lpFileName, BYTE *pRawBuffer)
{
	BOOL bRVal = TRUE;
	DWORD dwBytesRead = 0;
	int   w,h;
	
	HANDLE hFile = ::CreateFile(lpFileName,
								GENERIC_READ,
								FILE_SHARE_READ,
								NULL,
								OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL,
								NULL
								);
	if (hFile == INVALID_HANDLE_VALUE) {
		bRVal = FALSE;
	}
	else{
		::ReadFile(hFile, &(w), sizeof(DWORD), &dwBytesRead, NULL);
		::ReadFile(hFile, &(h), sizeof(DWORD), &dwBytesRead, NULL);
		
		::ReadFile(hFile, pRawBuffer, w * h, &dwBytesRead, NULL);
		
		CloseHandle(hFile);
	}
	
	return bRVal;

}

HVSTATUS CCamera::ORD_Fun_Interface_1(void *pInBuffer)
{
	HVSTATUS status=STATUS_NOT_SUPPORT_INTERFACE;
	HV_INTERFACE1_CONTEXT *pInfcectx=(HV_INTERFACE1_CONTEXT*)(pInBuffer);
	
	switch(pInfcectx->ID) {
	
	case COLOR_MODE:
		
		break;

	default:
		break;
	}
	return status;

}

HVSTATUS CCamera::F_ORD_GET_STRING(void *pInPut, void *pOut)
{
	HVSTATUS status=STATUS_NOT_SUPPORT_INTERFACE;
	HV_ARG_GET_STRING* InBuff = (HV_ARG_GET_STRING*)pInPut;
	HV_RES_GET_STRING* OutBuff = (	HV_RES_GET_STRING*)pOut;
	
	char*pBuf = OutBuff->pBuffer;
	int nSize = InBuff->nLength;
	HV_STRING_ID ID = InBuff->StringId;

	switch(ID) {
	case STRING_ID_DEVICE_SERIESNUM:
        status = local_Get_SN(pBuf,&nSize);
		break;
	case STRING_ID_FPGA_VERSION:
		status = local_Get_FPGA_Ver(pBuf,&nSize);
		break;
	case STRING_ID_DEVICE_DRIVER_VERSION:
		{
			status = STATUS_PARAMETER_INVALID;
			int nLen = strlen("1.0.0.1-VD");
			if(pBuf && nSize == nLen )
			{
				sprintf((char*)pBuf,"1.0.0.1-VD");//本模块版本
				status = STATUS_OK;
				
			}
		}
		break;
	case STRING_ID_FIRMWARE_VERSION:
		{
			status = STATUS_PARAMETER_INVALID;
			if(pBuf && nSize == sizeof(DWORD))
			{
				sprintf((char*)pBuf,"2.0.0.8");
				status = STATUS_OK;
				
			}
		}
		break;
	case STRING_ID_DEVICE_TYPE:
		{
			status = STATUS_PARAMETER_INVALID;
			if(pBuf && nSize == sizeof(HVTYPE))
			{
				*(HVTYPE *)pBuf = PD1420ECTYPE; 
				status = STATUS_OK;
			}
		}
		break;
	default:
		break;
	}
    return status;

}

HVSTATUS CCamera::F_ORD_QUERY_STRING_DESCRIPTOR(void *pInPut, void *pOut)
{
	HVSTATUS status=STATUS_NOT_SUPPORT_INTERFACE;
    
	HV_ARG_QUERY_STRING_DESCRIPTOR* InQuery = (HV_ARG_QUERY_STRING_DESCRIPTOR*)pInPut;
	HV_RES_QUERY_STRING_DESCRIPTOR* OutQuery = (HV_RES_QUERY_STRING_DESCRIPTOR*)pOut;
	HV_STRING_ID StrID = InQuery->StringId;
	
	switch(StrID) {
	case STRING_ID_DEVICE_SERIESNUM:
		OutQuery->dwTotalByteLength = 15;
		status = STATUS_OK;
		break;
	case STRING_ID_FPGA_VERSION:
		OutQuery->dwTotalByteLength = sizeof(DWORD);
		status = STATUS_OK;
		break;
	case STRING_ID_DEVICE_DRIVER_VERSION:
		OutQuery->dwTotalByteLength = strlen("1.0.0.1-VD");
		status = STATUS_OK;
		break;
	case STRING_ID_FIRMWARE_VERSION:
		OutQuery->dwTotalByteLength = sizeof(DWORD);
		status = STATUS_OK;
		break;
	case STRING_ID_DEVICE_TYPE:
		OutQuery->dwTotalByteLength = sizeof(HVTYPE);
		status = STATUS_OK;
		break;
	default:
		break;
	}
    return status;

}

HVSTATUS CCamera::F_SET_FEATURE(void *pInBuffer)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	
	
	HV_ARG_SET_FEATURE *setFeature;
	setFeature = (HV_ARG_SET_FEATURE *)pInBuffer;
	
	switch(setFeature->FeatureId) 
	{
	case FEATURE_ID_GAIN:
		m_nGain = setFeature->Feature.Scalar.Value;
		_RPT1(_CRT_WARN, "Gain : %d",m_nGain);
		status = STATUS_OK;
		break;
	case FEATURE_ID_SHUTTER:
		m_nShutter = setFeature->Feature.Scalar.Value;
		_RPT1(_CRT_WARN, "Shutter : %d",m_nShutter);
		status = STATUS_OK;
		break;
	case FEATURE_ID_BRIGHTNESS:
		m_nBrightness = setFeature->Feature.Scalar.Value;
		_RPT1(_CRT_WARN, "Brightness : %d",m_nBrightness);
		status = STATUS_OK;
		break;
	case FEATURE_ID_WHITE_BALANCE:
		{
			switch(setFeature->FeatureItemId) {
			case FEATURE_ITEM_ID_WHITE_BALANCE_UB:
				m_nBGain = setFeature->Feature.WhiteBalance.UB;
				break;
			case FEATURE_ITEM_ID_WHITE_BALANCE_VR:
				m_nRGain = setFeature->Feature.WhiteBalance.VR;
				break;
			default:
				break;
			}
		}
		_RPT2(_CRT_WARN, "WB : (UB:%d VR:%d)",m_nBGain,m_nRGain);
		status = STATUS_OK;
		break;
	case FEATURE_ID_TRIGGER:
		{
			switch(setFeature->FeatureItemId)
			{
			case FEATURE_ITEM_ID_TRIGGER_ON_OFF:
				m_dwTriggerOnOff = setFeature->Feature.Trigger.OnOff;
				if (m_dwTriggerOnOff==0)
				   SetEvent(m_hSoftTrigger);
			
			case FEATURE_ITEM_ID_TRIGGER_MODE:
			case FEATURE_ITEM_ID_TRIGGER_POLARITY:
				break;
			default:
				break;
			}
			
		}
		status = STATUS_OK;
		break;
	default:
		break;
	}
	
	return status;

}

void CCamera::local_UnBayerTest(int w, int h, BYTE *pRGBBuf)
{
	if (pRGBBuf == NULL)  return;

	BYTE *pBuf = pRGBBuf;

	int Width = w;
	int Height = h;
	BYTE *pRawBuf = new BYTE[w*h];
	
	BYTE *p = NULL, * q = NULL;	
	
	//以RGGB解析在demo中不对,故以BGGR解析
	for (int i = 0; i < Height; i++)
	{                           
		p = pBuf + i * Width * 3;
		for(int j = 0; j < Width; j++)
		{                     
			q = p + j * 3;
			if (i%2==0)//偶数行
			{
				if (j%2==0)//偶数列
				{
					pRawBuf[i*Width+j] = *(q+2);//*(q+0);
					
				}
				else//奇数列
				{
					pRawBuf[i*Width+j] = *(q+1);
					
				}
				
			}
			else if (i%2==1)//奇数行
			{
				if (j%2==0)
				{
					pRawBuf[i*Width+j] = *(q+1);
					
				}
				else
				{
					pRawBuf[i*Width+j] = *(q+0);//*(q+2);
				}
			}
		}	
	}


	FillMemory(pRGBBuf, Width * Height * 3 , 0xCC);
	::CopyMemory(pRGBBuf,pRawBuf,w*h);
	delete[]pRawBuf;
	

}

void CCamera::local_FindMyFile(char *pfileType, HV_FILE *pHvfile, int *pCount)
{
	if (pfileType == NULL || pHvfile == NULL || pCount == NULL)   
	{
		return;
	}

	char szFilePath[296];
	char szFileName[296];
	memset(szFilePath,0,296);
	memset(szFileName,0,296);
	::GetModuleFileName(NULL,szFilePath,296);
	PathRemoveFileSpec(szFilePath);
	PathRemoveBackslash(szFilePath);
	PathAddBackslash(szFilePath);
    strcat(szFileName,szFilePath);
	strcat(szFileName,pfileType);
	
	int id=0;
    char *szfile = szFileName;
	WIN32_FIND_DATA fd;
	
	HANDLE hd=::FindFirstFile((LPCTSTR)szfile,&fd);
	if(hd==INVALID_HANDLE_VALUE) {return;}
	
    sprintf(pHvfile[id++].szfile,"%s%s",szFilePath,fd.cFileName);
	while(FindNextFile(hd,&fd))
	{
		sprintf(pHvfile[id++].szfile,"%s%s",szFilePath,fd.cFileName);
	};
	
	if (pCount)
		*pCount = id;

	FindClose(hd); 

}


void CCamera::_Set_Device_Path(char *pszPath)
{
	strcpy(m_szDevicePath,pszPath);
	
	//Set SN...
	int nPos = 0;
	while('-'!=pszPath[nPos])
	{
		nPos++;
	}
	strcpy(m_szSN,&pszPath[nPos]); 

}
