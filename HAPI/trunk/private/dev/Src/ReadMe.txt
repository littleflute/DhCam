output: ..\..\..\..\..\demo\bin\HVAPI.dll
		..\..\..\..\..\demo\bin\hvperf_cnD.exe


g_strPrefix


 [HVAPI.dll]
-----2009.3.27 Fri --HYL--
1. M: HVDef.h      //  $Revision: 2.0.3.0$  
   A:
	SV2001GMTYPE = 126,
	SV2001GCTYPE = 127,
	SV1411GMTYPE = 128,
	SV1411GCTYPE = 129,
	SV1421GMTYPE = 130,
	SV1421GCTYPE = 131,
	SV401GMTYPE  = 132,
	SV401GCTYPE  = 133,

2. V: 2.0.0.5  strcpy(pOutBuf->pBuffer,"2.0.0.5");//本模块(HVAPI.dll) 版本号设置

=========================================================================

-----2009.3.25 Wed --HYL--
1. M: HVDef.h      //  $Revision: 2.0.2.9$  

	DRVIFENETTYPE= 8192,    //0x00002000
    DRVIFGIGETYPE= 12288,   //0x00003000

2. V: 2.0.0.3  strcpy(pOutBuf->pBuffer,"2.0.0.3");//本模块(HVAPI.dll) 版本号设置

=========================================================================

-----2009.3.23 Mon --HYL--
1. M: HVDef.h      //  $Revision: 2.0.2.7$ (李润峰 增加图像被动传输功)
2. M: IDS_STRING_DEVICE_TYPE
   A: \nDHEnetBase

3. V: 2.0.0.2  strcpy(pOutBuf->pBuffer,"2.0.0.2");//本模块(HVAPI.dll) 版本号设置

=========================================================================

-----2009.3.20 Fri --HYL--
1. M: HVDef.h      //  $Revision: 2.0.2.6$ (李润峰 增加图像被动传输功)
2. V: 2.0.0.1  strcpy(pOutBuf->pBuffer,"2.0.0.1");//本模块(HVAPI.dll) 版本号设置

=========================================================================

-----2009.3.16 Wed --HYL--
1. M:    HVDef.h      //  $Revision: 2.0.2.5$ 

 =========================================================================

-----2009.3.11 Wed --HYL--
1. M: pCurrent->Reserved =  pHvDevIF[i].Reserved;
2. V: 2.0.0.0  strcpy(pOutBuf->pBuffer,"2.0.0.0");//本模块(HVAPI.dll) 版本号设置

 =========================================================================
-----2009.3.9 Mon --HYL--
1. M: (HVDef.h 2.0.2.4)
2. V: 1.2.0.2  strcpy(pOutBuf->pBuffer,"1.2.0.2");//本模块(HVAPI.dll) 版本号设置

 =========================================================================
 
-----2009.3.6 Fri --HYL--
1. M: (HVDef.h 2.0.2.4)
2. V: 1.2.0.1  strcpy(pOutBuf->pBuffer,"1.2.0.1");//本模块(HVAPI.dll) 版本号设置

 =========================================================================
 

-----2009.3.4 Wed --HYL--
1. V: 1.2.0.0  strcpy(pOutBuf->pBuffer,"1.2.0.0");//本模块(HVAPI.dll) 版本号设置
	* 从本版本开始，以新的接口方式实现　HVEnumerate　接口。
	即要求下层的设备库也开放同样的 HVEnumerate 接口。

 (HVDef.h 2.0.2.3)
 
 






