
 output: ..\..\..\..\..\demo\bin\HVDAILT.dll

[HVDAILT.dll]
----2009.3.25 Wed --HYL----
1. 基于 HVDef.h (2.0.3.0)
2. V4.0.0.2  sprintf(szHVDailt,"4.0.0.2"); //本模块(HVDAILT.dll) 版本号设置
 

============================================
----2009.3.25 Wed --HYL----
1. 基于 HVDef.h (2.0.2.9)
2. V4.0.0.1  sprintf(szHVDailt,"4.0.0.1"); //本模块(HVDAILT.dll) 版本号设置
	 
3. A:
	{39,{ 0x7b100fc7, 0x647a, 0x47b2, {0x89, 0x15, 0xe2, 0xcc, 0xda, 0xbb, 0x9c, 0x61}}, DRVIFENETTYPE },
	{40,{ 0x31bf8fcf, 0x9f8f, 0x4f24, {0xb6, 0xc5, 0xd8, 0xae, 0x1f, 0x72, 0xa2, 0x8d}}, DRVIFGIGETYPE },
	// 
4. M:
BOOL GetTypeFromIndex(int nDevice, HVTYPE *pType, int *pIndex)

============================================

----2009.3.23 Mon --HYL----
1. 基于 HVDef.h (2.0.2.7)
2. V4.0.0.0

============================================

----2009.3.11 Wed --HYL----
1. V: 4.0.0.0	sprintf((char*)pContext,"4.0.0.0;%s",v);
	
2. M: *pType = (HVTYPE)glDevGraph[nDevice-1].DevInterface.Reserved;//Tab[j].nType;


==========================================
