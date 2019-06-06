../../../../../demo/bin/SV400FC.dll
..\..\..\..\..\demo\bin\hvperf_cnD.exe

[ReadMe.txt]
--2009.3.23 Mon --hyl--
1. SV2000FM,SV2000FC　
2. SV1420FM,SV1420FC
3. SV1410FM,SV1410FC
4. SV1400FM,SV1400FC
5. SV1311FM,SV1311FC
6. SV1310FM,SV1310FC
7. SV1300FM,SV1300FC
8. SV400FM,SV400FC
9. M: HVEnumerate
	pCurrent->Reserved = SV2000FMTYPE; //需要修改
10. V: strcpy(szDeviceDriverVer,"2.0.0.1");//本层模块版本  
===================================================================================

--2009.3.20 Fri --hyl--
1. V: strcpy(szDeviceDriverVer,"2.0.0.0");//本层模块版本  
2. I:
	HVEnumerate
	HVGuid
	HVOpen
	HVClose
	HVControl

#define EXPORT_(b)		HV ## b

#define Device_Enumerate	EXPORT_(Enumerate)
#define Device_Open			EXPORT_(Open)
#define Device_Guid			EXPORT_(Guid) 
#define Device_Close		EXPORT_(Close)
#define Device_Control		EXPORT_(Control)

extern "C" BOOL __stdcall HVEnumerate(HV_DEVICE_INTERFACE *pDeviceInterface, 
									  DWORD cb,
									  DWORD *cbNeeded)
===================================================================================
