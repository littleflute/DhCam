#include "stdafx.h"
#include "local.h"



extern int CAMERA_KINDS;        //2008.8.28   --guolin--

extern char g_strPrefix[128][MAX_STRING];          //2008.8.28   --guolin--

//////////////////////////////////////////////////////////////////////////
// 2009.04.23 lvxiaofeng
typedef struct tagVERSION {
    WORD wMajor;
    WORD wMinor;
    WORD wRevision;
    WORD wBuild;
} VERSION, *PVERSION;
/*
\brief GetLibraryVersion, 获取资源文件中的版本信息
\param hModule, exe，DLL 句柄
\return 返回
*/
// 从版本资源中获取 dll version
#include <Winver.h>
#include <tchar.h>
#pragma comment(lib,"Version.lib")
VERSION GetLibraryVersion(HMODULE hModule)
{
    VERSION Version;
    TCHAR szFullPath[MAX_PATH];
    DWORD dwVerInfoSize = 0;
    DWORD dwVerHnd;
    VS_FIXEDFILEINFO *pFileInfo = NULL;    
    GetModuleFileName(hModule, szFullPath, sizeof(szFullPath));
    dwVerInfoSize = GetFileVersionInfoSize(szFullPath, &dwVerHnd);
    if (dwVerInfoSize)
    {
        // If we were able to get the information, process it:
        HANDLE  hMem;
        LPVOID  lpvMem;
        unsigned int uInfoSize = 0;
        hMem = GlobalAlloc(GMEM_MOVEABLE, dwVerInfoSize);
        lpvMem = GlobalLock(hMem);
        GetFileVersionInfo(szFullPath, dwVerHnd, dwVerInfoSize, lpvMem);
        ::VerQueryValue(lpvMem, (LPTSTR)_T("\\"), (void**)&pFileInfo, &uInfoSize);        
        // Product version from the FILEVERSION of the version info resource     
        Version.wMajor    = HIWORD(pFileInfo->dwProductVersionMS); 
        Version.wMinor    = LOWORD(pFileInfo->dwProductVersionMS);
        Version.wRevision = HIWORD(pFileInfo->dwProductVersionLS);
        Version.wBuild      = LOWORD(pFileInfo->dwProductVersionLS);     
        GlobalUnlock(hMem);
        GlobalFree(hMem);
    }
    return Version;
}
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------
//  HMODULE DriverAddRef(int type)
//  Author:
//  Date: 02/17/2006
//-----------------------------------------------------
/**
 * load driver by camera type
 *
 * \param    type
 * \return   HMODULE
 *
 */
//-----------------------------------------------------
 
HMODULE DriverAddRef(int type)
{	 
	assert(type >= 0 && type < CAMERA_KINDS);
	char filename[MAX_STRING];
	sprintf(filename, "%s%s", g_strPrefix[type], ".dll");
	 
	return LoadLibrary(filename);
}


//-----------------------------------------------------
//  HMODULE DriverAddRef(GUID *pGuid, int *pType)
//  Author:
//  Date: 02/20/2006
//-----------------------------------------------------
/**
 * load driver by camera GUID and return camera type
 *
 * \param    pGuid
 * \param    pType		
 * \return   HMODULE
 *
 */
//-----------------------------------------------------
HMODULE DriverAddRef(GUID *pGuid, int *pType)
{
	assert( (pGuid != NULL) && (pType != NULL) );
	HMODULE hMod = NULL;
	
	for (int type = 0; type < CAMERA_KINDS; ++type)
	{
		if ( hMod = DriverAddRef(type) ) 
		{ 
			char szFunName[256];
			sprintf(szFunName, "%s", "HVGuid");
			FUNC_Device_GUID pFunc = (FUNC_Device_GUID)::GetProcAddress(hMod,szFunName); 
			if(NULL==pFunc){ 
				continue;
			}
			GUID guid;
			pFunc(&guid);
			if ( !memcmp(&guid, pGuid, sizeof(GUID)))// check guid  
			{
				*pType = type;
				break;
			}
			
			
			DriverRelease(hMod);
		}
	}
	
	return ( type >= CAMERA_KINDS ? NULL : hMod );
}



//-----------------------------------------------------
//  BOOL DriverRelease(HMODULE hMod)
//  Author:
//  Date: 02/20/2006
//-----------------------------------------------------
/**
 * release driver by module handle
 *
 * \param    hMod
 * \return   BOOL
 *
 */
//-----------------------------------------------------
BOOL DriverRelease(HMODULE hMod)
{
	return FreeLibrary(hMod);
}



//-----------------------------------------------------
//  HVSTATUS GetFuncAddr(HMODULE hMod, int type, char *funcname, FARPROC * ppFunc)
//  Author:
//  Date: 02/20/2006
//-----------------------------------------------------
/**
 * get pointer of function from driver
 *
 * \param    hMod
 * \param    type
 * \param    funcname
 * \param    ppFunc
 * \return   HVSTATUS
 *
 */
//-----------------------------------------------------
HVSTATUS GetFuncAddr(HMODULE hMod, int type, char *funcname, FARPROC * ppFunc)
{	
	assert(type >= 0 && type < CAMERA_KINDS);
	char buffer[MAX_STRING];
	HVSTATUS status = STATUS_OK;
	
	sprintf(buffer, "%s%s", g_strPrefix[type], funcname);
	*ppFunc = GetProcAddress(hMod, buffer);
	if ( *ppFunc == NULL )
		status = STATUS_NOT_SUPPORT_INTERFACE;
	
	return status;
}

 



//----------------------------------------------------------------
//  HVSTATUS Open(char *DevicePath, GUID *pGuid, HANDLE *phCamera)
//  Author:
//  Date: 02/20/2006
//----------------------------------------------------------------
/**
 * open camera 
 *
 * \param    DevicePath
 * \param    pGuid
 * \param    phCamera
 * \return   HVSTATUS
 *
 */
//-----------------------------------------------------
HVSTATUS Open(char *DevicePath, GUID *pGuid, HANDLE *phCamera)
{
	HVSTATUS status = STATUS_OK;
	
	int type = 0;  
	/// load driver to user space
	HMODULE hMod = DriverAddRef(pGuid, &type);   
	if ( hMod == NULL)
		return STATUS_OPEN_DRIVER_FAILED;
	 
	char szFunName[256];
	sprintf(szFunName, "%s", "HVOpen"); 
	
	FUNC_Device_Open pFunc = (FUNC_Device_Open)::GetProcAddress(hMod,szFunName); 
	if(pFunc)
	{
		HANDLE hDevice = NULL;
		/// call function of driver to open camera
		if ( HV_SUCCESS(status = pFunc(DevicePath, pGuid, &hDevice)))
		{
			/// allocate global memory to store information of camera
			HANDLE hObj = GlobalAlloc(GHND, sizeof(HVOBJ));
			if ( hObj == NULL )
				status = STATUS_NOT_ENOUGH_SYSTEM_MEMORY;
			else
			{
				/// lock memory to user space
				HVOBJ *pOBJ = (HVOBJ *)GlobalLock(hObj); 
				assert( pOBJ != NULL );
				
				/// store camera information 
				pOBJ->hDevice = hDevice;
				pOBJ->hMod = hMod;
				pOBJ->type = type;
				
				/// return camera handle for user
				*phCamera = hObj;
				
				GlobalUnlock(hObj);
			}
		}
	}
	
	/// don't release driver if all successful
	if ( HV_FAILED(status) )
		DriverRelease(hMod);
	
	return status;
}



//-------------------------------------------------
//  HVSTATUS Close(HANDLE hCamera)
//  Author:
//  Date: 02/21/2006
//-------------------------------------------------
/**
 * close camera
 *
 * \param    hCamera
 * \return   HVSTATUS
 *
 */
//-------------------------------------------------
HVSTATUS Close(HANDLE hCamera)
{
	HVSTATUS status = STATUS_OK;
	
	HVOBJ *pOBJ = (HVOBJ *)GlobalLock(hCamera);  
	if ( pOBJ == NULL)
		return STATUS_DEVICE_HANDLE_INVALID;
	 
	
	char szFunName[256];
	sprintf(szFunName, "%s", "HVClose"); 
	
	FUNC_Device_Close pFunc = (FUNC_Device_Close)::GetProcAddress(pOBJ->hMod,szFunName); 
	if(pFunc)
	{
		if ( HV_SUCCESS(status = pFunc(pOBJ->hDevice)))
		{
			
		}
	}
	
	
	
	GlobalUnlock(hCamera);
	
	if ( HV_SUCCESS(status) )
		GlobalFree(hCamera);  /// free global heap if all successful
	
	return status;
}



//-------------------------------------------------------------
// HVSTATUS Control(HANDLE hCamera, HV_CONTROL_CODE code, 
//		void *pInBuffer, DWORD nInBufferSize, void *pOutBuffer, DWORD nOutBufferSize)
// Author:
// Date: 02/21/2006
//-------------------------------------------------------------
/**
 * send command code to driver and control camera
 *
 * \param    hCamera
 * \param    code
 * \param    pInBuffer
 * \param    nInBufferSize
 * \param    pOutBuffer
 * \param    nOutBufferSize
 * \return   HVSTATUS
 *
 */
//-------------------------------------------------------------
HVSTATUS Control(HANDLE hCamera, HV_CONTROL_CODE code, 
		void *pInBuffer, DWORD nInBufferSize, void *pOutBuffer, DWORD nOutBufferSize, DWORD *pBytesReturned)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;//STATUS_OK;
	
	HVOBJ *pOBJ = (HVOBJ *)GlobalLock(hCamera);  
	if ( pOBJ == NULL)
		return STATUS_DEVICE_HANDLE_INVALID;
	
	if( ORD_QUERY_STRING_DESCRIPTOR == code)
	{		
		HV_ARG_QUERY_STRING_DESCRIPTOR *pInBuf = (HV_ARG_QUERY_STRING_DESCRIPTOR*)pInBuffer;
		if(pInBuf->StringId == STRING_ID_HVAPI_VERSION)
		{
			HV_RES_QUERY_STRING_DESCRIPTOR *pOutBuf = (HV_RES_QUERY_STRING_DESCRIPTOR*)pOutBuffer;
			pOutBuf->dwTotalByteLength = 7;
			status = STATUS_OK;
			return status;
		}
	}
	else if( ORD_GET_STRING == code)
	{
		HV_ARG_GET_STRING *pInBuf = (HV_ARG_GET_STRING*)pInBuffer;
		if(pInBuf->StringId == STRING_ID_HVAPI_VERSION)
		{
			HV_RES_GET_STRING *pOutBuf = (HV_RES_GET_STRING*)pOutBuffer;
			
			//strcpy(pOutBuf->pBuffer,"2.0.0.5");//本模块(HVAPI.dll) 版本号设置

			HMODULE hMod = LoadLibrary("HVAPI.dll");
			if (hMod!=NULL)
			{
				VERSION CurrVersion = GetLibraryVersion(hMod);
				sprintf(pOutBuf->pBuffer,"%d.%d.%d.%d",
					CurrVersion.wMajor,
					CurrVersion.wMinor,
					CurrVersion.wRevision,
					CurrVersion.wBuild);
			}
			else
			{
				strcpy(pOutBuf->pBuffer,"0.0.0.0");//Invalid Version
			}


			status = STATUS_OK;
			return status;
		}
	}
	
	char szFunName[256];
	sprintf(szFunName, "%s", "HVControl"); 
	
	FUNC_Device_Control pFunc = (FUNC_Device_Control)::GetProcAddress(pOBJ->hMod,szFunName); 
	if(pFunc)
	{
		if ( HV_SUCCESS(status = pFunc(pOBJ->hDevice, code, 
			pInBuffer, nInBufferSize,
			pOutBuffer, nOutBufferSize, pBytesReturned)))
		{
			
		}
	}
	
	GlobalUnlock(hCamera);
	
	return status;
}



//------------------------------------------------------------
// HVSTATUS GetGUID(GUID *pguid, int *pSize)
// Author: 
// Date: 03/10/2006
//------------------------------------------------------------
/*
 * get guid of all supported cameras  
 *
 * \param   pguid   
 * \param   cb
 * \param   cbNeeded
 * \return  HVSTATUS
 *
 */
//------------------------------------------------------------
HVSTATUS GetGUID(GUID *pguid, DWORD cb, DWORD *cbNeeded)
{
	static GUID CameraGUIDs[] = {
		/// GUID_INTERFACE_DHHV1302UC
		{ 0x989938e0, 0xb8da, 0x11d9, {0xa7, 0xdb, 0x0, 0x2, 0x2a, 0xb0, 0x49, 0x72} },
			/// GUID_INTERFACE_DHHV2002UC
		{ 0xf17f23e0, 0xb8ab, 0x11d9, {0xa7, 0xdb, 0x0, 0x2, 0x2a, 0xb0, 0x49, 0x71} }
	};
	
	/// get size of guid array 
	if ( pguid == NULL)
	{
		*cbNeeded = sizeof(CameraGUIDs) ;
		return STATUS_OK;
	}
	
	int count = cb / sizeof(GUID);
	if ( count > (sizeof(CameraGUIDs) / sizeof(GUID)) )
		count = sizeof(CameraGUIDs) / sizeof(GUID);
	
	*cbNeeded = count * sizeof(GUID) ;
	CopyMemory(pguid, CameraGUIDs, *cbNeeded);
	
	return STATUS_OK;
}