// HVAPI.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "local.h"
#include "DeviceManager.h"

HMODULE g_hMyModule = NULL;
char szFileName[1280];
int CAMERA_KINDS;
char g_strPrefix[128][MAX_STRING];
CDeviceManager	g_DevMng;  

BOOL IsHVAPI()
{
	 BOOL b = false;
	 int nLen = strlen(szFileName);
	 char *pCh = szFileName;
	 char s[10];
	 strcpy(s,&pCh[nLen-9]);
    _RPT1(_CRT_WARN, " -----------IsHVAPI()---------------- nLen = %d\n",nLen);
    _RPT1(_CRT_WARN, "============ %s =========\n",s);

	if('H'!=toupper(s[0])) return false;
	if('V'!=toupper(s[1])) return false;
	if('A'!=toupper(s[2])) return false;
	if('P'!=toupper(s[3])) return false;
	if('I'!=toupper(s[4])) return false;
	if('.'!=toupper(s[5])) return false;
	if('D'!=toupper(s[6])) return false;
	if('L'!=toupper(s[7])) return false;
	if('L'!=toupper(s[8])) return false;

	b = true;
	return b;
}
  
BOOL APIENTRY DllMain( HMODULE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{ 
	
    _RPT1(_CRT_WARN, " ----HVAPI.dll----       DllMain:  hModule = 0x%x\n",hModule);

	memset(szFileName,0,1280);
   ::GetModuleFileName(hModule,szFileName,1280);
   
   _RPT1(_CRT_WARN, " ----HVAPI.dll----       DllMain:  szFileName = [ %s ]\n",szFileName);
	
	g_hMyModule = hModule;
	
	g_DevMng._Get_Device_Type_List(hModule);
	return TRUE;
}




extern "C" BOOL __stdcall HVEnumerate(HV_DEVICE_INTERFACE *pDeviceInterface, 
                                      DWORD cb, DWORD *cbNeeded)
{
  if ( cbNeeded == NULL )
		return false;

  ZeroMemory(pDeviceInterface, cb);

  return g_DevMng.Do_Enumerate(pDeviceInterface, cb, cbNeeded);
}




extern "C" HVSTATUS __stdcall HVOpen(char *DevicePath, GUID *pGuid, HANDLE *phCamera)
{
	if ( !(DevicePath && pGuid && phCamera) )
		return STATUS_PARAMETER_INVALID;
	
	*phCamera = NULL;
	
	return Open(DevicePath, pGuid, phCamera);
}



extern "C" HVSTATUS __stdcall HVClose(HANDLE hCamera)
{
	if ( (hCamera == NULL) || (hCamera == INVALID_HANDLE_VALUE) )
		return STATUS_PARAMETER_INVALID;
	
	return Close(hCamera);
}



extern "C" HVSTATUS __stdcall HVControl(HANDLE hCamera, HV_CONTROL_CODE code, 
										void *pInBuffer, DWORD nInBufferSize, 
										void *pOutBuffer, DWORD nOutBufferSize, 
										DWORD *pBytesReturned)
{
	HVSTATUS st = STATUS_NOT_SUPPORT_INTERFACE; 
    _RPT0(_CRT_WARN, " ----HVAPI.dll----       HVControl:  \n");
	/*
	if(!IsHVAPI()) 
	{
		_RPT0(_CRT_WARN, " ----HVAPI.dll----       HVControl:  STATUS_FILE_INVALID!!!\n");
		return STATUS_FILE_INVALID;
	}
	*/
	if ( (hCamera == NULL) || (hCamera == INVALID_HANDLE_VALUE) )
		return STATUS_PARAMETER_INVALID;
	
	st = Control( hCamera, 
		code,	/// operation control code			
		pInBuffer, nInBufferSize,	/// input data buffer and size
		pOutBuffer, nOutBufferSize, /// output data buffer and size
		pBytesReturned);	/// receives the size in bytes
	return st;
}




extern "C" HVSTATUS __stdcall HVGetGUID(GUID *pguid, DWORD cb, DWORD *cbNeeded)
{
	if ( cbNeeded == NULL )
		return STATUS_PARAMETER_INVALID;
	
	return GetGUID(pguid, cb, cbNeeded);
}