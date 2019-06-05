/*
 *-----------------------------------------------------------------------------
 *       Name				HVUnknown.h
 *       Purpose			
 *       Development Kit	Microsoft Win32 SDK, Visual C++ 6.00 
 *       Author				Zhou zhong ya
 *       Date          		2004/04/16
 *		 Version			1.01.0002
 *
 *       Description
 *
 *       
 *-----------------------------------------------------------------------------
 */

#if !defined(HV_HVUNKNOWN)         //
#define HV_HVUNKNOWN


/* By C++ language compiler */
#ifdef __cplusplus
extern "C" {
#endif


//typedef enum tagHV_CONTROL_CODE {   //delete by xupx 2008/05/27
//	DEVCTRL_SECRETKEY_LEVEL			= 0,
//	DEVCTRL_SECRETKEY_LENGTH		= 1,
//	DEVCTRL_READ_SECRETKEY			= 2,
//	DEVCTRL_STATUS_CHECK_SECRETKEY	= 3,
//	DEVCTRL_CLEAR_SECRETKEY			= 4
//} HV_CONTROL_CODE;
//

HVSTATUS __stdcall HVStartSnapInternal(HHV hhv);
HVSTATUS __stdcall HVStopSnapInternal(HHV hhv);
HVSTATUS __stdcall HVTransfer(HHV hhv, BYTE *pBuffer, DWORD *pLength);

HVSTATUS __stdcall HVVerifyPassword(HHV hhv, BOOL *pStatus);
HVSTATUS __stdcall HVGetI2CParam(HHV hhv, WORD wStart, WORD *pBuffer, WORD wEntries);
HVSTATUS __stdcall HVSetI2CParam(HHV hhv, WORD wStart, WORD *pBuffer, WORD wEntries);
HVSTATUS __stdcall HVBulkRead(HHV hhv, BYTE *pBuffer, DWORD dwLength);
HVSTATUS __stdcall HVBulkWrite(HHV hhv, BYTE *pBuffer, DWORD dwLength);

HVSTATUS __stdcall HVReset(HHV hhv);

HVSTATUS __stdcall HVGetTypeFromIndex(int nDevice, HVTYPE *pType, int *pIndex);
	
HVSTATUS __stdcall HV8051Run(HHV hhv);
HVSTATUS __stdcall HV8051Hold(HHV hhv);
HVSTATUS __stdcall HVAnchorDownload(HHV hhv, BYTE *pBuffer, DWORD Length);
HVSTATUS __stdcall HVLoadEEPROM(HHV hhv, BYTE *pBuffer, DWORD Length);

HVSTATUS __stdcall HVDeviceControl(HHV hhv, HV_CONTROL_CODE ControlCode, void *pContext, int *pLength);

/* extren "C" { */
#ifdef __cplusplus
}
#endif

#endif