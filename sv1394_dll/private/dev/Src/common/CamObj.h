
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
 
#include <objbase.h>
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

//
// Global link import library, move here for
// convenience.
//
#pragma comment(lib, "setupapi.lib")
//#pragma comment(lib, "version.lib")
//#pragma comment(lib, "shlwapi.lib")
 
#include "..\..\inc\hvdef.h"
#include "..\..\inc\HVInter.h"
  


#include "MIGRATE.H"
#include "offset.h"
#include "rtl.h"
#include "format7.h"
#include "trigger.h"
#include "feature.h"
#include "camera.h"
#include "dev.h"

#ifdef STATUS_TIMEOUT
#undef STATUS_TIMEOUT
#endif
#ifdef STATUS_NO_MEMORY
#undef STATUS_NO_MEMORY
#endif

#include "DHSTATUS.H" 

//
// Global Macros.
//
#define MAX_IMG_ERR				2
#define UNKNOW_CAMERA           0
#define CMOS_CAMERA             1
#define CCD_CAMERA              2

// Define snap buffer num
#define BUFFER_NUM               4

#define NEARLYINFINITE			1000
#define TIMEOUT_1000_MS			1000
#define TIMEOUT_10_S			10000

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




//////////////////////////////////////////////////////


