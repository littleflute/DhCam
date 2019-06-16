//File: CAMOBJ.cpp

#include "CAMOBJ.h"

  DEFINE_GUID(VDEVICE_GUID, 
0x00000000, 0x0000, 0x0000, 0x99, 0x5f, 0xd3, 0x52, 0x6c, 0x83, 0xb1, 0x36);
#define	STR_DEVICE	"VDevic"

//global
char sz_Sim[128][128];//SN for device

BOOL g_bDeviceRemove = FALSE;
char g_szModuleFileName[128];
 

BOOL APIENTRY DllMain( HMODULE hModule, 
                       DWORD  dwReason, 
                       LPVOID lpReserved
					 )
{
	char szMP[1280];
	memset(szMP,0,128);
	memset(g_szModuleFileName,0,1280);
	DWORD dwLen = ::GetModuleFileName(hModule,szMP,128);
	
	//del [\\]
	for(DWORD offset=dwLen; offset>=0; offset--)
	{
		if(szMP[offset] == '\\')
		{
			szMP[offset] = 0;
			break;
		}
	}
	
	//del [.dll]
	char szFN[128];
	strcpy(szFN,&szMP[offset+1]);
	dwLen = strlen(szFN);
    for(  offset=dwLen; offset>=0; offset--)
	{
		if(szFN[offset] == '.')
		{
			szFN[offset] = 0;
			break;
		}
	}
	strcpy(g_szModuleFileName,szFN);
	
    return TRUE;
}


//---------------------------------------------------------------
 
#define Device_Enumerate	EXPORT_(Enumerate)
#define Device_Open			EXPORT_(Open)
#define Device_Guid			EXPORT_(Guid) 
#define Device_Close		EXPORT_(Close)
#define Device_Control		EXPORT_(Control)
//---------------------------------------------------------------
 
HVTYPE vv()
{
	HVTYPE t = UNKNOWN_TYPE;
	
	if(0==strcmp("SV1420FC",g_szModuleFileName))
	{
		t = SV1420FCTYPE;
	}
	else if(0==strcmp("SV1410FC",g_szModuleFileName))
	{
		t = SV1410FCTYPE;
	}
	else if(0==strcmp("HV1300FC",g_szModuleFileName))
	{
		t = HV1300FCTYPE;
	}
	else if(0==strcmp("SV1410GC",g_szModuleFileName))
	{
		t = SV1410GCTYPE;
	}
	else if(0==strcmp("SV1420GC",g_szModuleFileName))
	{
		t = SV1420GCTYPE;
	}
	else if(0==strcmp("PD1420EM",g_szModuleFileName))
	{
		t = PD1420EMTYPE;
	}
	else if(0==strcmp("PD1420EC",g_szModuleFileName))
	{
		t = PD1420ECTYPE;
	}
	return t;
}

extern "C" BOOL __stdcall Device_Enumerate(HV_DEVICE_INTERFACE *pDeviceInterface, 
										   DWORD cb,
										   DWORD *cbNeeded)
{ 
	bool bRet = false;	
	
	//获得摄像机列表
	char ch;
	char sz_Name[100][64];
	char sz_SleepTime[100][20];
	int  CounterName = 0;
	int  CounterSim = 0;
	int  CounterSleep = 0;
	
	int  Num = 0;//nummber of device
	memset(sz_Name,0,sizeof(sz_Name));
	memset(sz_Sim,0,sizeof(sz_Sim));
	memset(sz_SleepTime,0,sizeof(sz_SleepTime));


	char szFileNames[260]; 
	DWORD	dwLen = GetModuleFileName(NULL, szFileNames, sizeof(szFileNames));
	for(DWORD offset=dwLen; offset>=0; offset--)
	{
		if(szFileNames[offset] == '\\')
		{
			szFileNames[offset] = 0;
			break;
		}
	}
	char szFullFileNames[1024];
	sprintf(szFullFileNames,"%s\\%s.txt",szFileNames,g_szModuleFileName);
	
	
	fstream fin;
	fin.open(szFullFileNames,ios::in); 
	int n_C = 0; //number of '_'   Add by Jiapeng

	while (!fin.eof()) 
	{ 
		ch = fin.get(); 
		if (ch != '_'&& ch!='\n' && n_C == 0) {			 
			sz_Name[Num][CounterName]=ch;
			CounterName++;
		}
		else if (ch != '_' && ch!='\n' && n_C == 1) {			
			sz_Sim[Num-1][CounterSim]=ch;
			CounterSim++; 
		}
		else if (ch != '_' && ch!='\n' && n_C == 2) {
			sz_SleepTime[Num-1][CounterSleep] = ch;
			CounterSleep++;
		}

		if (ch == '_') {
			n_C++;
			if (n_C > 1) {
				continue;
			}
			Num++;
		}
		
		if (ch=='\n'){

			CounterName=0;
			CounterSim=0;
			CounterSleep=0;
			n_C = 0;
		}
	}
	fin.close();
		
	if(0==Num){		Num = 1;	}   //xdTest1:
	
	*cbNeeded = Num * sizeof (HV_DEVICE_INTERFACE);
	if(NULL == pDeviceInterface)
	{
		return true;
	}
	int nSleepTotal = 0;
	if(Num>0)//有设备存在
	{ 
		//取出所有设备的列表				 
		HV_DEVICE_INTERFACE *pCurrent = pDeviceInterface;		
		for(int i = 0; i < Num; i++)
		{    
			if ( pCurrent && ( cb >= *cbNeeded) )
			{
				strcpy(pCurrent->szDevicePath, sz_Name[i]);
				strcat(pCurrent->szDevicePath,"-");
				strcat(pCurrent->szDevicePath,sz_Sim[i]);
				pCurrent->ClassGUID = VDEVICE_GUID;
				CopyMemory(pCurrent->szDevicePath, pCurrent->szDevicePath, 128);
				
				nSleepTotal += atoi(sz_SleepTime[i]);

				pCurrent->ClassGUID = pCurrent->ClassGUID;
				pCurrent->Reserved = vv();
				++pCurrent;
			}		
		}		 		
	} 
	
	bRet = TRUE;
	
	Sleep(nSleepTotal);

	
	return bRet;
	
}
extern "C" void __stdcall Device_Guid(GUID *pGuid)
{
	*pGuid = VDEVICE_GUID;
	
}

extern "C" HVSTATUS __stdcall Device_Open(char *DevicePath, 
										   GUID *pGuid, 
										   HANDLE *phDev)
{
	HVSTATUS status = STATUS_OK;
	
	if ( *pGuid != VDEVICE_GUID )
		return STATUS_PARAMETER_INVALID;
	
	*phDev= GlobalAlloc(GHND,sizeof(PCAMOBJ));
	PCAMOBJ pCamObj = (PCAMOBJ) GlobalLock(*phDev);	
 
	 
	
	CCamera* pCam = new CCamera;
	pCam->_Set_Device_Path(DevicePath);

	pCamObj->pMyCam = (HANDLE)(pCam);	
	
	GlobalUnlock(phDev);
	
	return status;
}

extern "C" HVSTATUS __stdcall Device_Close(HANDLE hDev)
{
	HVSTATUS status = STATUS_OK;
	
	PCAMOBJ pCamObj = (PCAMOBJ) GlobalLock(hDev);
 
	CCamera* pCam = (CCamera *)(pCamObj->pMyCam);
	
	if(pCam) 
	{
		delete pCam;
		pCam = NULL;
	}
	
    GlobalUnlock(hDev);	
	GlobalFree(hDev);
	
	return status;
	
}

extern "C" HVSTATUS __stdcall Device_Control(
				HANDLE hDev, HV_CONTROL_CODE code, 
				void *pInBuffer, DWORD nInBufferSize, 
				void *pOutBuffer, DWORD nOutBufferSize,
				DWORD *pBytesReturned)

{
	HVSTATUS status = STATUS_OK;
	PCAMOBJ pCamObj = (PCAMOBJ) GlobalLock(hDev);
	CCamera* pCam = (CCamera*)(pCamObj->pMyCam);

	status = pCam->Control(code,pInBuffer, nInBufferSize, pOutBuffer, nOutBufferSize);
	
	GlobalUnlock(pCamObj);
	return status;
	
}
