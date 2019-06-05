// DeviceManager.cpp: implementation of the CDeviceManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DeviceManager.h"
#include "local.h"

extern char g_strPrefix[128][MAX_STRING];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDeviceManager::CDeviceManager()
{ 
}

CDeviceManager::~CDeviceManager()
{

} 
bool CDeviceManager::Do_Enumerate(HV_DEVICE_INTERFACE *pDevIF, 
								DWORD dwBytes, 
								DWORD *pdwBytes)
{
	bool bRet = false;
	HV_DEVICE_INTERFACE *pCurrent = pDevIF;
	*pdwBytes = 0;
	int n = CAMERA_KINDS;
	for (int i = 0; i < n ; ++i )
	{
		HMODULE hMod = v_Load_Dll(i);
		if ( hMod == NULL ) continue;
 
		{
			DWORD dwNeeded = 0;

			char szFunName[256];
			sprintf(szFunName, "%s", "HVEnumerate");
			FUNC_Device_Enumerate pFunc = (FUNC_Device_Enumerate)::GetProcAddress(hMod,szFunName); 
			if(NULL==pFunc){ 
				continue;
			}
			pFunc(NULL,0,&dwNeeded);//查总个数
			
			*pdwBytes += dwNeeded;
			if(NULL==pDevIF){
				continue;
			}
			
			int n = dwNeeded/sizeof(HV_DEVICE_INTERFACE);
			if(n>0)//有设备存在
			{
				HV_DEVICE_INTERFACE *pHvDevIF = new HV_DEVICE_INTERFACE[n];
				pFunc(pHvDevIF,dwNeeded,&dwNeeded);//取出所有设备的列表				 
				
				for(int i = 0; i < n; i++)
				{    
					if ( pCurrent && ( dwBytes >= *pdwBytes) )
					{
						CopyMemory(pCurrent->szDevicePath, pHvDevIF[i].szDevicePath, 128);
					//	CopyMemory(pCurrent->ClassGUID,pHvDevIF[i].ClassGUID,sizeof(GUID));
						pCurrent->ClassGUID = pHvDevIF[i].ClassGUID;
						pCurrent->Reserved =  pHvDevIF[i].Reserved;
						++pCurrent;
					}		
				}		
				delete []pHvDevIF;		
			}
			DriverRelease(hMod);
			continue;
		}
		 
	}
	bRet = true;
	return bRet;
}

HMODULE CDeviceManager::v_Load_Dll(int n)
{ 
	assert(n >= 0 && n < CAMERA_KINDS);
	char filename[MAX_STRING];
	sprintf(filename, "%s%s", g_strPrefix[n], ".dll"); 
	return LoadLibrary(filename);
}

void CDeviceManager::_Get_Device_Type_List(HMODULE hModule)
{
	TCHAR szList[128*32];
	LoadString(hModule, IDS_STRING_DEVICE_TYPE, szList, sizeof(szList));	
	
	int h=0,l=0;
    CAMERA_KINDS = 0;
	
	for(int i = 0; ; i++)
	{
		if (szList[i] != '\n')
		{
			g_strPrefix[h][l] = szList[i];
			l++;
		}
		if(szList[i] == '\n' && szList[i+1] != NULL)
		{
			h++;
			l = 0;
			CAMERA_KINDS++;
		}
		if(szList[i] == NULL || (szList[i] == '\n' && szList[i+1] == NULL))
			break;
	}
	CAMERA_KINDS+=1;

}
