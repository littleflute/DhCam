//File: CamObj.h
#if !defined(__CamObj_H__)
#define __CamObj_H__

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include "objbase.h"
#include <initguid.h>
#include <windows.h>
#include <windowsx.h>
#include <setupapi.h>
#include <malloc.h>
#include <winver.h>
#include <shlwapi.h>
#include <crtdbg.h>
#include <dbt.h>
#include <time.h>
#include <ASSERT.H>
#include <TCHAR.H>
#include <stdio.h>
#include <fstream.h>
#include <iostream.h>


//
// Global link import library, move here for
// convenience.
//
#pragma comment(lib, "setupapi.lib")
//#pragma comment(lib, "version.lib")
//#pragma comment(lib, "shlwapi.lib")

#include "..\common\inc\hvapi.h"
#include "..\common\inc\hvdef.h"
#include "local.h"
#include "camera.h"
#include "..\common\inc\HVUtil.h"
#pragma comment(lib,"..\\common\\lib\\hvutil.lib")
#include "shlobj.h"
#include "Shlwapi.h"
#pragma comment(lib,"Shlwapi.lib ") 

#define EXPORT_(b)		HV ## b

#ifdef STATUS_TIMEOUT
#undef STATUS_TIMEOUT
#endif
#ifdef STATUS_NO_MEMORY
#undef STATUS_NO_MEMORY
#endif




//
// Global Macros.
//

#define MAX_IMG_ERR				2
#define UNKNOW_CAMERA           0
#define CMOS_CAMERA             1
#define CCD_CAMERA              2

#define W1					    1392         //最大分辨率
#define H1						1040

// Define snap buffer num
#define BUFFER_NUM               4

#define NEARLYINFINITE       6000

#define RETRY_TIME              20
#define SLEEP_TIME             ((rand()%20)+10)

#ifdef _DEBUG
//#ifdef _RPT5
//#undef _RPT5
#define _RPT5(rptno, msg, arg1, arg2, arg3, arg4, arg5) \
    do { if ((1 == _CrtDbgReport(rptno, NULL, 0, NULL, msg, arg1, arg2, arg3, arg4, arg5))) \
_CrtDbgBreak(); } while (0)
#else
#define _RPT5(rptno, msg, arg1, arg2, arg3, arg4, arg5)
#endif

 


#endif //__CamObj_H__