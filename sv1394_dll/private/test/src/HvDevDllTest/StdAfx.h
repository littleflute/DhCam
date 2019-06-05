// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__E90B8562_57EE_4054_9D9F_7084160D5CE5__INCLUDED_)
#define AFX_STDAFX_H__E90B8562_57EE_4054_9D9F_7084160D5CE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "..\\..\\..\\dev\\inc\\hvdef.h"

/** declare prototype of function in driver*/
//typedef bool ( __stdcall FUNC_Enumerate )(int , HV_DEVICE_INTERFACE *);
typedef HVSTATUS (__stdcall *FUNC_Enumerate)(HV_DEVICE_INTERFACE *pDeviceInterface, 
                                      DWORD cb, DWORD *cbNeeded);
typedef void ( __stdcall *FUNC_Guid )(GUID *);
typedef HVSTATUS ( __stdcall FUNC_Open )(char *, GUID *, HANDLE *);
typedef HVSTATUS ( __stdcall FUNC_Close )(HANDLE);
typedef HVSTATUS ( __stdcall FUNC_Control )(HANDLE , HV_CONTROL_CODE , void *, DWORD , void *, DWORD, DWORD *);


//------------------------------------------------------------------------------
// class CStopWatch
// Author: 
// Date: 
//------------------------------------------------------------------------------
/**
 * \brief   Measurement of time intervalls
 *
 */
//------------------------------------------------------------------------------


class CStopWatch
{
public:
  //////////////////////////////////////////////////////////////////////////////////////////
  // cconstructor, starts time measurement
  CStopWatch()
  {
    Start();
  }

  //////////////////////////////////////////////////////////////////////////////////////////
  // Start the stop watch 
  void Start()
  {
     QueryPerformanceCounter(&m_StartTime);
  }

  //////////////////////////////////////////////////////////////////////////////////////////
  // Stop. The elapsed time is returned. The stop watch may be started againn neuen Zeitpunkt
  double Stop(bool StartAgain)
  {
    QueryPerformanceCounter(&m_StopTime);
    double theElapsedTime = ElapsedTime();
    if(StartAgain)
      m_StartTime = m_StopTime; 
    return theElapsedTime;
  }

  //////////////////////////////////////////////////////////////////////////////////////////
  // Return the elapsed time in seconds between start() and stop()
  double ElapsedTime()
  {
    LARGE_INTEGER timerFrequency;
    QueryPerformanceFrequency(&timerFrequency);
  
    __int64 oldTicks = ((__int64)m_StartTime.HighPart << 32) + (__int64)m_StartTime.LowPart;
    __int64 newTicks = ((__int64)m_StopTime.HighPart << 32) + (__int64)m_StopTime.LowPart;
    long double timeDifference = (long double) (newTicks - oldTicks);

    long double ticksPerSecond = (long double) (((__int64)timerFrequency.HighPart << 32) 
				  + (__int64)timerFrequency.LowPart);

    return (double)(timeDifference / ticksPerSecond);
  }

protected:
  // zero-point for time measurment
  LARGE_INTEGER m_StartTime;

  // last time stamp
  LARGE_INTEGER m_StopTime;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__E90B8562_57EE_4054_9D9F_7084160D5CE5__INCLUDED_)
