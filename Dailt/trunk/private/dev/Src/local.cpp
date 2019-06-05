#include "stdafx.h"

/*
	Global variable 
 */
char g_chErrorString[][MAX_STRING] = {
	"STATUS_OK",
	"STATUS_NO_DEVICE_FOUND",				//Hardware error
	"STATUS_DEVICE_HANDLE_INVALID",
	"STATUS_HW_DEVICE_TYPE_ERROR",
	"STATUS_HW_INIT_ERROR",
	"STATUS_HW_RESET_ERROR",
	"STATUS_NOT_ENOUGH_SYSTEM_MEMORY",
	"STATUS_HW_IO_ERROR",
	"STATUS_HW_IO_TIMEOUT",
	"STATUS_HW_ACCESS_ERROR",
	
	"STATUS_OPEN_DRIVER_FAILED",			//Software error
	"STATUS_NOT_SUPPORT_INTERFACE",
	"STATUS_PARAMETER_INVALID",
	"STATUS_PARAMETER_OUT_OF_BOUND",
	"STATUS_IN_WORK",
	"STATUS_NOT_OPEN_SNAP",
	"STATUS_NOT_START_SNAP",
	"STATUS_FILE_CREATE_ERROR",
	"STATUS_FILE_INVALID",
	"STATUS_NOT_START_SNAP_INT",
	"STATUS_INTERNAL_ERROR"					//Internal error
};


DEFINE_GUID(GUID_SV1410GC, 
			0x14107167, 0x8386, 0x40aa, 0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca);
			//1410G C     S V
DEFINE_GUID(GUID_SV2000GC, 
			0x20007167, 0x8386, 0x40aa, 0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca);

DEFINE_GUID(PD1420EM_GUID, 
            0x40d68d0a, 0xa510, 0x4e27, 0x9c, 0x9f, 0xc6, 0xf8, 0x2b, 0x4d, 0xe1, 0x15);

DEFINE_GUID(PD1420EC_GUID, 
            0x11673b1e, 0xe24e, 0x468a, 0x99, 0x5f, 0xd3, 0x52, 0x6c, 0x83, 0xb1, 0x36);

DEFINE_GUID(GUID_ITS2000GC, 
            0x20007167, 0x7384, 0x83aa, 0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca);

DEFINE_GUID(GUID_DH1394Base, 
		    0x54a5816d, 0xd3f4, 0x4f07, 0x94, 0xd3, 0x1d, 0x5b, 0x1d, 0xaa, 0x5f, 0x76);


//2000G C      I T     S

static HVQTAB Tab[] = {
	{1, { 0x31007067, 0x7286, 0x40aa, {0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, HV3100FCTYPE },
	{2, { 0x20007067, 0x7286, 0x40aa, {0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, HV2000FCTYPE },
	{3, { 0x13007067, 0x7286, 0x40aa, {0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, HV1300FCTYPE },
	{4, { 0x13007077, 0x7286, 0x40aa, {0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, HV1300FMTYPE },
	{5, { 0x13007067, 0x789b, 0x40aa, {0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, SV1300FCTYPE },
	{6, { 0x13007077, 0x789b, 0x40aa, {0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, SV1300FMTYPE },
	{7, { 0x13107067, 0x789b, 0x40aa, {0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, SV1310FCTYPE },
	{8, { 0x13107077, 0x789b, 0x40aa, {0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, SV1310FMTYPE },
	{9, { 0x13117067, 0x789b, 0x40aa, {0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, SV1311FCTYPE },
	{10,{ 0x13117077, 0x789b, 0x40aa, {0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, SV1311FMTYPE },
	{11,{ 0x4007067,  0x789b, 0x40aa, {0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, SV400FCTYPE  },
	{12,{ 0x4007077,  0x789b, 0x40aa, {0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, SV400FMTYPE  },
	{13,{ 0x14007067, 0x789b, 0x40aa, {0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, SV1400FCTYPE },
	{14,{ 0x14007077, 0x789b, 0x40aa, {0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, SV1400FMTYPE },
	{15,{ 0x14107067, 0x789b, 0x40aa, {0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, SV1410FCTYPE },
	{16,{ 0x14107077, 0x789b, 0x40aa, {0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, SV1410FMTYPE },
	{17,{ 0x14207067, 0x789b, 0x40aa, {0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, SV1420FCTYPE },
	{18,{ 0x14207077, 0x789b, 0x40aa, {0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, SV1420FMTYPE },
	{19,{ 0x20007067, 0x789b, 0x40aa, {0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, SV2000FCTYPE },
	{20,{ 0x20007077, 0x789b, 0x40aa, {0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, SV2000FMTYPE },
	{21,{ 0x14107067, 0x789b, 0x40aa, {0x41, 0x76, 0x44, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, RESERVED_001 },
	{22,{ 0x14107167, 0x8386, 0x40aa, {0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, SV1410GCTYPE },
	{23,{ 0x20007167, 0x8386, 0x40aa, {0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, SV2000GCTYPE },
	{24,{ 0x11673b1e, 0xe24e, 0x468a, {0x99, 0x5f, 0xd3, 0x52, 0x6c, 0x83, 0xb1, 0x36}}, PD1420ECTYPE },
	{25,{ 0x40d68d0a, 0xa510, 0x4e27, {0x9c, 0x9f, 0xc6, 0xf8, 0x2b, 0x4d, 0xe1, 0x15}}, PD1420EMTYPE },
	{26,{ 0x5a790715, 0xbdba, 0x4d5d, {0x9a, 0xc1, 0x12, 0xeb, 0x86, 0x8d, 0x4f, 0x5d}}, PD2000ECTYPE },
	{27,{ 0xbeba75d1, 0x2767, 0x4401, {0xb4, 0x65, 0xa8, 0xa4, 0x34, 0x46, 0x9, 0x1a}},  PD2000EMTYPE },
	{28,{ 0x14107177, 0x8386, 0x40aa, {0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, SV1410GMTYPE },
	{29,{ 0x20007177, 0x8386, 0x40aa, {0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, SV2000GMTYPE },
	{30,{ 0x14107067, 0x789b, 0x40aa, {0x49, 0x54, 0x53, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, ITS1410FCTYPE },
	{31,{ 0x14207167, 0x8386, 0x40aa, {0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, SV1420GCTYPE },
	{32,{ 0x14207177, 0x8386, 0x40aa, {0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, SV1420GMTYPE },
	{33,{ 0x20007167, 0x7384, 0x83aa, {0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, ITS2000GCTYPE },
	{34,{ 0x20007177, 0x7384, 0x83aa, {0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, ITS2000GMTYPE },
	{35,{ 0x14207167, 0x7384, 0x83aa, {0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, ITS1420GCTYPE },
	{36,{ 0x14207177, 0x7384, 0x83aa, {0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, ITS1420GMTYPE },
	{37,{ 0x14107167, 0x7384, 0x83aa, {0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, ITS1410GCTYPE },
	{38,{ 0x14107177, 0x7384, 0x83aa, {0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, ITS1410GMTYPE },

	{39,{ 0x7b100fc7, 0x647a, 0x47b2, {0x89, 0x15, 0xe2, 0xcc, 0xda, 0xbb, 0x9c, 0x61}}, DRVIFENETTYPE },
	{40,{ 0x31bf8fcf,  0x9f8f,0x4f24, {0xb6, 0xc5, 0xd8, 0xae, 0x1f, 0x72, 0xa2, 0x8d}}, DRVIFGIGETYPE },
	{41,{ 0x20007067, 0x789b, 0x40aa, {0x49, 0x54, 0x53, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, ITS2000FCTYPE },
	{42,{ 0x14207067, 0x789b, 0x40aa, {0x49, 0x54, 0x53, 0x5e, 0x21, 0x2f, 0x13, 0xca}}, ITS1420FCTYPE },
	{43,{ 0x54a5816d, 0xd3f4, 0x4f07, {0x94, 0xd3, 0x1d, 0x5b, 0x1d, 0xaa, 0x5f, 0x76}}, DRVIF1394TYPE },
};
  
    


#define CAMERA_NUM	sizeof(Tab)/sizeof(HVQTAB)

BOOL GetTypeFromIndex(int nDevice, HVTYPE *pType, int *pIndex)
{ 
  
	if (CreateGlobalDeviceGraph() != STATUS_OK || glcount < nDevice)
	{
		return FALSE;
	}
	
	if (pType)
		*pType = UNKNOWN_TYPE;
	
	GUID guid = glDevGraph[nDevice-1].DevInterface.ClassGUID;;
	int nCam = CAMERA_NUM;

	for (int j = 0; j < nCam; j++)
	{
		if (Tab[j].guid == guid)
		{
			if (pType)
				*pType = Tab[j].nType;
			break;
		}
	}
	
	int ntemp = 0;
	int i = 0;
	while (i++<nDevice)
	{
		if (glDevGraph[i-1].DevInterface.ClassGUID == guid)
		{
			ntemp++;
		}
		
	}
	if (pIndex)
		*pIndex = ntemp;
	
	return TRUE;

}

BOOL GetDevInterfaceFromIndex(int nDevice, HV_DEVICE_INTERFACE *pDeviceInterface)
{
	if (CreateGlobalDeviceGraph() != STATUS_OK || glcount < nDevice)
	{
		return FALSE;
	}
	
	memcpy(pDeviceInterface,&glDevGraph[nDevice-1].DevInterface,sizeof(HV_DEVICE_INTERFACE));
	
	return TRUE;
}

HVSTATUS GetDeviceGUID(GUID *pguid, DWORD cb, DWORD *cbNeeded)
{
	/// get size of guid array 
	if ( pguid == NULL)
	{
		*cbNeeded = CAMERA_NUM * sizeof(GUID) ;
		return STATUS_OK;
	}
	
	int count = cb / sizeof(GUID);
	GUID CameraGUIDs[CAMERA_NUM];
	for (int i = 0; i < CAMERA_NUM; i++)
	{
		CameraGUIDs[i]=Tab[i].guid;
	}
	if ( count > (sizeof(CameraGUIDs) / sizeof(GUID)) )
		count = sizeof(CameraGUIDs) / sizeof(GUID);
	
	*cbNeeded = count * sizeof(GUID) ;
	CopyMemory(pguid, CameraGUIDs, *cbNeeded);	
	return STATUS_OK;	
}

HVSTATUS CreateGlobalDeviceGraph()
{
	if (glDevGraph == NULL)
	{
		DWORD cbNeeded = 0;         
		HVEnumerate(NULL, 0, &cbNeeded);
		int count = 0;
		glcount= count = cbNeeded / sizeof(HV_DEVICE_INTERFACE);
		
		if ( count <= 0 ) 
			return STATUS_NO_DEVICE_FOUND;
		
		HV_DEVICE_INTERFACE*  pDevInterface = NULL; 
		pDevInterface = new HV_DEVICE_INTERFACE[count+2];
		assert(pDevInterface);
		
		if ( ! HVEnumerate(pDevInterface, cbNeeded, &cbNeeded) )
		{
			delete []pDevInterface;
			pDevInterface = NULL;
			return STATUS_NO_DEVICE_FOUND;
		}
		
		glDevGraph = new HV_DEVICE_GRAPH[count+2];
		
		for (int i=1 ;i<=count;i++)
		{
			ZeroMemory(&glDevGraph[i-1],sizeof(HV_DEVICE_GRAPH));
			glDevGraph[i-1].DevInterface = pDevInterface[i-1];
			glDevGraph[i-1].nIndex = i;
			//...
		}
		
		delete []pDevInterface;
	}
	else 
	{
		if ( glcount <= 0) 
			return STATUS_NO_DEVICE_FOUND;
	}
	
	return STATUS_OK;

}
HVSTATUS CreateIndex2GrapMap(char *pInfo,HHV *pHandle)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	
	if (pInfo == NULL || pHandle == NULL || pInfo == (char*)0xcccccccc)
	{
		return STATUS_PARAMETER_INVALID;
	}
	if (CreateGlobalDeviceGraph() != STATUS_OK)
	{
		return STATUS_NO_DEVICE_FOUND; 
	}
	
	*pHandle = NULL;
	HVOBJ *pObj = NULL;
	
	for (int i=1; i<=glcount; i++)
	{
		HANDLE hDevice = NULL;
		if ( HV_SUCCESS(status = HVOpen(glDevGraph[i - 1].DevInterface.szDevicePath, 
			&(glDevGraph[i-1].DevInterface.ClassGUID), &hDevice)) )
		{
			status = HVControl(hDevice, ORD_INIT_CAMERA, NULL, 0, NULL, 0, NULL);
			if (!HV_SUCCESS(status) ) 
			{
				HVClose(hDevice);
				if (pObj != NULL)
				{
					HVClose(pObj->hDevice);
					GlobalFree(pObj);
					*pHandle = NULL;
				}
				return status;
			}
			//..........................
			CCamera camInfo;
			char szSeriesInfo[256] = {0};
			char szMacInfo[256] = {0};
			char szIPInfo[256] = {0};
			int  nSize = 0;
			HVSTATUS st;
			st = camInfo.DevInfo_Get_SN(hDevice,NULL,&nSize);
			if (HV_SUCCESS(st))
			{
				st = camInfo.DevInfo_Get_SN(hDevice,szSeriesInfo,&nSize);
				if (HV_SUCCESS(st))
					sprintf(glDevGraph[i-1].szSeriesnum,"%s",szSeriesInfo);
			}
            st = camInfo.Get_MAC(hDevice,szMacInfo);
			if (HV_SUCCESS(st))
				sprintf(glDevGraph[i-1].szMac,"%s",szMacInfo);
			
			st = camInfo.Get_IP(hDevice,szIPInfo);
			if (HV_SUCCESS(st))
				sprintf(glDevGraph[i-1].szIP,"%s",szIPInfo);
			//..........................
			
			g_DevIndex2DevGraph[i] = glDevGraph[i-1]; //CreateMap
			
			if (!strcmp(szSeriesInfo,pInfo) || 
				!strcmp(szMacInfo,pInfo)|| 
				!strcmp(szIPInfo,pInfo))
			{
				pObj = (HVOBJ*)Packet_Handle_Struct(hDevice,i,pHandle);
			}
			else
			{
				HVClose(hDevice);
			}
			
		}
		else 
		{
			if (pObj != NULL)
			{
				HVClose(pObj->hDevice);
				GlobalFree(pObj);
				*pHandle = NULL;
			}
			return status;
		}
		
	}//end for
				
				
	return STATUS_OK;
				
}


HANDLE Packet_Handle_Struct(HANDLE hDevice,int nDevice,HHV *pHandle)
{
	HANDLE hObj = GlobalAlloc(GHND, sizeof(HVOBJ));
	if ( hObj == NULL )
		return  hObj;//STATUS_NOT_ENOUGH_SYSTEM_MEMORY;
	else
	{
		/// lock memory to user space
		HVOBJ *pOBJ = (HVOBJ *)GlobalLock(hObj); 
		assert( pOBJ != NULL );
		/// store camera information 
		pOBJ->nDevice = nDevice; //added by HYL. 2006.9.19 
		pOBJ->hDevice = hDevice;
		pOBJ->pSnapProc = NULL;
		pOBJ->pParam = NULL;
		pOBJ->bLock = TRUE;
//		pOBJ->video_mode = VIDEO_MODE0;
		/// 获取设备类型，此处类型用来返回给用户
		GetTypeFromIndex(nDevice, &(pOBJ->type), NULL);
		/// return camera handle for user
		*pHandle = hObj;
		GlobalUnlock(hObj);
	}

	return hObj;

}
HVSTATUS BeginDeviceFromIndex(int nDevice, HHV *pHandle)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	VERIFY_POINTER(pHandle, STATUS_PARAMETER_INVALID);
	*pHandle = NULL;

	if (CreateGlobalDeviceGraph() != STATUS_OK || glcount < nDevice)
	{
		return STATUS_NO_DEVICE_FOUND;
	}
	
	HANDLE hDevice = NULL;
	if ( HV_SUCCESS(status = HVOpen(glDevGraph[nDevice - 1].DevInterface.szDevicePath, 
		&(glDevGraph[nDevice - 1].DevInterface.ClassGUID), &hDevice)) )
	{
		HANDLE hObj = Packet_Handle_Struct(hDevice,nDevice,pHandle);
		
		status = HVControl(hDevice, ORD_INIT_CAMERA, NULL, 0, NULL, 0, NULL);
		if ( ! HV_SUCCESS(status) ) 
		{
			HVClose(hDevice);
			GlobalFree(hObj);
			*pHandle = NULL;
		}
		
	}  
	
	return status;
}
HVSTATUS BeginDeviceFromOthers(char *p, HHV *pHandle)
{
	HVSTATUS status = STATUS_NOT_SUPPORT_INTERFACE;
	VERIFY_POINTER(pHandle, STATUS_PARAMETER_INVALID);
	*pHandle = NULL;

	if (CreateGlobalDeviceGraph() != STATUS_OK)
	{
		return STATUS_NO_DEVICE_FOUND; 
	}
	
	if (g_DevIndex2DevGraph.empty())
	{
		if((status = CreateIndex2GrapMap(p,pHandle)) != STATUS_OK)
		{
			g_DevIndex2DevGraph.clear();
			return status;
		}
		else
		{
			if (*pHandle == NULL)
				return STATUS_NO_DEVICE_FOUND;
		}
	}
	else
	{
		HV_DEVICE_GRAPH  iGrap;
		if ( !FindGrap(p,iGrap) )
		{
			return STATUS_NO_DEVICE_FOUND;
		}
		
		HANDLE hDevice = NULL;
		if ( HV_SUCCESS(status = HVOpen(iGrap.DevInterface.szDevicePath,
			&(iGrap.DevInterface.ClassGUID), &hDevice)) )
		{
			HANDLE hObj = Packet_Handle_Struct(hDevice,iGrap.nIndex,pHandle);
			
			status = HVControl(hDevice, ORD_INIT_CAMERA, NULL, 0, NULL, 0, NULL);
			if ( ! HV_SUCCESS(status) ) 
			{
				HVClose(hDevice);
				GlobalFree(hObj);
				*pHandle = NULL;
			}
		}
	}  
	
	return status;
		
}
BOOL FindGrap(char *p,HV_DEVICE_GRAPH& iGrap)
{
	if (g_DevIndex2DevGraph.empty())  return FALSE;
	
	BOOL bIsfind = FALSE;

	DevIndex2DevGraph::iterator it = g_DevIndex2DevGraph.begin();
	while ( it != g_DevIndex2DevGraph.end() )
	{
		HV_DEVICE_GRAPH Graph = it->second;
		if (!strcmp(Graph.szSeriesnum,p) ||
			!strcmp(Graph.szMac,p) ||
			!strcmp(Graph.szIP,p))
		{
			iGrap = Graph;
			bIsfind = TRUE;
			break;
		}
		it++;
	}
	
	return bIsfind;
}