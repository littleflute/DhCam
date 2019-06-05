#ifndef _1394ADD_H_
#define _1394ADD_H_

#include "HVDef.h"

#ifdef __cplusplus
extern "C" {
#endif
    
//QiaoJH,2005.6.21
typedef enum tagCOLOR_CODE { 
    MONO8    = 0,
    YUV8_411 = 1,
    YUV8_422 = 2,
    YUV8_444 = 3,
    RGB8     = 4,
    MONO16   = 5,
    RGB16    = 6
} COLOR_CODE;

typedef enum tagSHUTTER_UNIT { 
    SHUTTER_US    = 0,
    SHUTTER_MS = 1,
    SHUTTER_S = 2
} SHUTTER_UNIT_VALUE;

HVSTATUS __stdcall HVSetColorCode(HHV hhv,COLOR_CODE code);
//end:QiaoJH,2005.6.21


HVSTATUS __stdcall HV1394ReadBlock(HHV hhv,unsigned long ulOffset,unsigned long ulLength,unsigned long* pBuf);
HVSTATUS __stdcall HV1394WriteBlock(HHV hhv,unsigned long ulOffset,unsigned long ulLength,unsigned long* pBuf);

HVSTATUS __stdcall HV1394ReadQuadlet(HHV hhv, unsigned long address, unsigned long *pData);
HVSTATUS __stdcall HV1394WriteQuadlet(HHV hhv, unsigned long address, unsigned long data);

HVSTATUS __stdcall HVSetShutterUnit(HHV hhv, SHUTTER_UNIT_VALUE unit);

HVSTATUS __stdcall HV1394SetCameraBrightness(HHV hhv, unsigned long value);

HVSTATUS __stdcall HV1394SetUserData(HHV hhv, BYTE* pData,DWORD dwLen);

HVSTATUS __stdcall HV1394GetUserData(HHV hhv, BYTE* pData,DWORD dwLen);

BOOL __stdcall HV1394CheckUserPassWD(HHV hhv, char* pData);

HVSTATUS __stdcall HV1394LoadLUT(HHV hhv, DWORD* ptr,int nLong);

HVSTATUS __stdcall HV1394SetBytesPerPacket(HHV hhv, int *nLen);


HVSTATUS __stdcall HV1394SetWhiteBalance(HHV hhv, int u_b, int v_r);

HVSTATUS __stdcall HV1394SetGain(HHV hhv, long dwValue);

HVSTATUS __stdcall HV1394FreezeFrame(HHV hCamera, int nFreeze);

/* extren "C" { */
#ifdef __cplusplus
}
#endif

#endif //_1394ADD_H_
