//---------------------------------------------------------
//   Company:  Daheng Imavision 
//   Project:  HVAPI
//   $Author:  zhou zhong ya$
//   $Date:  02/16/06 14:27$
//---------------------------------------------------------
/**
  *  \file     local.h
  *
  *  \brief  Interface for kernel mode driver
  *
  *  This file defines macros and function pointers for 
  *  Daheng Imavison Digital Camera driver.
  */
//---------------------------------------------------------
 
#if !defined(LOCAL_H)
#define LOCAL_H

#define  MAX_STRING	   64

typedef struct tagHVOBJ {
  HANDLE hDevice;
  HMODULE hMod;
  int type;
} HVOBJ;


/** declare prototype of function in driver*/
typedef bool ( __stdcall FUNC_Enumerate )(int , HV_DEVICE_INTERFACE *);
typedef void ( __stdcall FUNC_Guid )(GUID *);
typedef HVSTATUS ( __stdcall FUNC_Open )(char *, GUID *, HANDLE *);
typedef HVSTATUS ( __stdcall FUNC_Close )(HANDLE);
typedef HVSTATUS ( __stdcall FUNC_Control )(HANDLE , HV_CONTROL_CODE , void *, DWORD , void *, DWORD, DWORD *);

typedef HVSTATUS (__stdcall *FUNC_Device_Enumerate)(HV_DEVICE_INTERFACE *pDeviceInterface, 
                                      DWORD cb, DWORD *cbNeeded);
typedef HVSTATUS (__stdcall *FUNC_Device_GUID)(GUID *pGUID);
typedef HVSTATUS ( __stdcall *FUNC_Device_Open )(char *, GUID *, HANDLE *);
typedef HVSTATUS ( __stdcall *FUNC_Device_Control )(HANDLE , HV_CONTROL_CODE , void *, DWORD , void *, DWORD, DWORD *);

typedef HVSTATUS ( __stdcall *FUNC_Device_Close )(HANDLE);

/** declare prototype of function in main.cpp */
HMODULE DriverAddRef(int);
HMODULE DriverAddRef(GUID *, int *);
BOOL DriverRelease(HMODULE );
HVSTATUS GetFuncAddr(HMODULE, int, char *, FARPROC * );

HVSTATUS Open(char *, GUID *, HANDLE *);
HVSTATUS Close(HANDLE );
HVSTATUS Control(HANDLE , HV_CONTROL_CODE , void *, DWORD , void *, DWORD, DWORD *);
HVSTATUS GetGUID(GUID *, DWORD, DWORD *);


#endif 