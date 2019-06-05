//#include "..\common\CAMOBJ.h"
#include "..\\common\\CAMOBJ.h"
#include "LOCAL.h"
#include "math.h"

//ITS 0x49 0x54 0x53
//                                                  1420                                      I       T       S
DEFINE_GUID(GUID_ITS1420FC, 0x14207067, 0x789b, 0x40aa, 0x49, 0x54, 0x53, 0x5e, 0x21, 0x2f, 0x13, 0xca);

PCHAR GetLibraryName()
{
	const static PCHAR LibraryName = "ITS1420FC.dll";
	return LibraryName;
}


#define EXPORT_(b)		HV ## b

#define Device_Enumerate	EXPORT_(Enumerate)
#define Device_Open			EXPORT_(Open)
#define Device_Guid			EXPORT_(Guid) 
#define Device_Close		EXPORT_(Close)
#define Device_Control		EXPORT_(Control)


extern "C" 
BOOL 
__stdcall 
Device_Enumerate(
				 HV_DEVICE_INTERFACE *pDeviceInterface, 
				 DWORD cb,
				 DWORD *cbNeeded
				 )
{ 
	bool bRet = false;
	
	//获取设备总数
	int nDevice = 0;
	do{
		HV_DEVICE_INTERFACE devIF;
		if(All_Enumerate(nDevice,&devIF,GUID_ITS1420FC))
		{
			nDevice++;
		}
		else
		{
			break;
		}
	}while(1);
	
	*cbNeeded = nDevice * sizeof (HV_DEVICE_INTERFACE);
	
	if(NULL == pDeviceInterface)
	{
		return true;
	}
	
	if(nDevice>0)//有至少一个设备存在
	{ 
		//取出所有设备的列表				 
		HV_DEVICE_INTERFACE *pCurrent = pDeviceInterface;		
		for(int i = 0; i < nDevice; i++)
		{    
			if ( pCurrent && ( cb >= *cbNeeded) )
			{
				HV_DEVICE_INTERFACE devIF;
				if(All_Enumerate(i,&devIF,GUID_ITS1420FC)){
					strcpy(pCurrent->szDevicePath, devIF.szDevicePath); 
					pCurrent->ClassGUID = GUID_ITS1420FC;
					//pCurrent->Reserved = ITS1420FCTYPE;
					
					++pCurrent;
					bRet = TRUE;
				}
				else{
					bRet = FALSE;
					break;
				}  
			}		
		}		 		
	} 
	
	return bRet;
}

extern "C" 
void 
__stdcall 
Device_Guid(
			GUID *pGuid
			)
{
	*pGuid = GUID_ITS1420FC;
}


extern "C" 
HVSTATUS 
__stdcall 
Device_Open(
			char *DevicePath, 
			GUID *pGuid, 
			HANDLE *phCamera
			)
{
	HVSTATUS status = STATUS_OK;	
	status = All_Open(DevicePath,phCamera,pGuid,GUID_ITS1420FC);
	return status;
}

extern "C" 
HVSTATUS 
__stdcall 
Device_Close(
			 HANDLE hCamera
			 )
{
	HVSTATUS status = STATUS_OK;
	status = All_Close(hCamera);
	return status;
}

extern "C" 
HVSTATUS 
__stdcall 
Device_Control(
			   HANDLE hCamera,
			   HV_CONTROL_CODE code, 
			   void *pInBuffer, 
			   DWORD nInBufferSize, 
			   void *pOutBuffer, 
			   DWORD nOutBufferSize,
			   DWORD *pBytesReturned
			   )
{
	HVSTATUS status = STATUS_OK;
	PCAMOBJ pCamObj = (PCAMOBJ) GlobalLock(hCamera);	
	if(pCamObj->phCamera==NULL)
	{
		status=STATUS_DEVICE_HANDLE_INVALID;
		return status;
	} 	
	CCamera* pMyCam = ToClass(pCamObj->pCam);
	status = pMyCam->ALL_Ctrl(code,pInBuffer,nInBufferSize,pOutBuffer,nOutBufferSize,pBytesReturned);
	GlobalUnlock(pCamObj);
	return status;
}

 

