
//-----------------------------------------------------------------------------
//  Company:   Daheng Imavision
//  Section:   Software Department
//  Project:   HVAPI.h
//
//  $Archive:
//  $Author:   Software Department
//  $Revision: 2.0.0.0$
//  $Date:     2008.6.2 2:00pm$
//------------------------------------------------------------------------------
/**
*  \file     HVAPI.h
*  \brief    The head file of the HVAPI interface.
*
*
*  The file defines the HVAPI interface
*/
//-----------------------------------------------------------------------------

#if !defined(HVAPI_H)
#define HVAPI_H

#include "HVDef.h"

/* By C++ language compiler */
#ifdef __cplusplus
extern "C"
{
#endif


//////////////////////////////////////////////////////////////////////////
/// The function prototype of the HVAPI.dll
    BOOL
    __stdcall
    HVEnumerate(
        HV_DEVICE_INTERFACE *pDeviceInterface,
        DWORD cb,
        DWORD *cbNeeded
    );

    HVSTATUS
    __stdcall
    HVOpen(char *pDevicePath,
           GUID *pGUID,
           HANDLE *phDevice
          );

    HVSTATUS
    __stdcall HVClose(
        HANDLE hDevice
    );

    HVSTATUS
    __stdcall
    HVControl(
        HANDLE hDevice,
        HV_CONTROL_CODE code,
        void *pInBuffer,
        DWORD nInBufferSize,
        void *pOutBuffer,
        DWORD nOutBufferSize,
        DWORD *pBytesReturned
    );

    HVSTATUS
    __stdcall
    HVGetGUID(
        GUID *pGUID,
        DWORD cb,
        DWORD *cbNeeded
    );


//////////////////////////////////////////////////////////////////////////
    /* extern "C" { */
#ifdef __cplusplus
}
#endif

#endif

