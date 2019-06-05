// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__EAF55A74_001E_4A24_95AC_DD5DC64CFF61__INCLUDED_)
#define AFX_STDAFX_H__EAF55A74_001E_4A24_95AC_DD5DC64CFF61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <assert.h>
#include <stdio.h>

#include <objbase.h>
#include <initguid.h>

#include <crtdbg.h>

#pragma warning ( disable : 4018 )

// TODO: reference additional headers your program requires here
#include "..\inc\HVAPI.h"
#include "..\inc\hvdef.h"
#include "..\inc\HVInter.h"

#include "camera.h"
#pragma comment(lib, "..\\lib\\HVAPI.lib")

BOOL IsHVFSerial(HHV hhv);



typedef enum tagMAX_VALUE{ 
		
	MAX_STRING      	= 256,
		
} MAX_VALUE;

extern char g_chErrorString[][MAX_STRING];



#define DEVICE_TYPE_NUMBER	(RESERVED_001 - HV1300UCTYPE + 1)

#define VERIFY_POINTER(p, status)	\
		if ((p) == NULL){	return (status); }

typedef enum tagHV_STATUS{ 
	
	HW_NONE			= 0, 
	HW_SNAP_INT		= 1, 
	HW_SNAP			= 2,
	HW_OTHER		= 3
		
} HV_STATUS;
 

//the handle structure       
typedef struct tagHVOBJ {
	
	HANDLE					hDevice;
	HVTYPE					type;
	int						nDevice;	
	HV_STATUS				HvStatus;
	HV_VIDEO_MODE			video_mode;	
	HV_SNAP_PROC			pSnapProc;
	void					*pParam;	
	BOOL					fSuspendGrab;	
	BOOL bLock;
	
} HVOBJ;


BOOL GetTypeFromIndex(int nDevice, HVTYPE  *pType, int *pIndex);
BOOL GetDevInterfaceFromIndex(int nDevice, HV_DEVICE_INTERFACE *pDeviceInterface);
HVSTATUS GetDeviceGUID(GUID *, DWORD, DWORD *);

extern int glcount;
//extern HV_DEVICE_INTERFACE* glDevInterface;

typedef struct tagHV_QTAB {
	int      nIndex;
	GUID     guid;
	HVTYPE   nType;
} HVQTAB;

/*
typedef enum tagHV_BEGIN_TYPE{ 
	FROM_INDEX         = 0,
	FROM_SERIESNUM     = 1,
	FROM_MAC           = 2,
	FROM_IP            = 3

}HV_BEGIN_TYPE;*/


#pragma warning(disable:4786)
#include <map>
using namespace std;


typedef struct tagHV_DEVICE_GRAPH{

	HV_DEVICE_INTERFACE DevInterface;
	char szSeriesnum[256];
	char szMac[256];
	char szIP[256];
	int  nIndex;

}HV_DEVICE_GRAPH;

typedef map<int, HV_DEVICE_GRAPH> DevIndex2DevGraph;

extern  DevIndex2DevGraph   g_DevIndex2DevGraph;
extern  HV_DEVICE_GRAPH*    glDevGraph;	

HVSTATUS CreateGlobalDeviceGraph();
HVSTATUS CreateIndex2GrapMap(char *pInfo,HHV *pHandle);
HVSTATUS BeginDeviceFromIndex(int nDevice, HHV *pHandle);
HVSTATUS BeginDeviceFromOthers(char *p, HHV *pHandle);
HANDLE   Packet_Handle_Struct(HANDLE hDevice,int nDevice,HHV *pHandle);
BOOL     FindGrap(char *p,HV_DEVICE_GRAPH& iGrap);

//////////////////////////////////////////////////////////////////////////
// 2009.04.23 lvxiaofeng
typedef struct tagVERSION {
    WORD wMajor;
    WORD wMinor;
    WORD wRevision;
    WORD wBuild;
} VERSION, *PVERSION;
/*
\brief GetLibraryVersion, 获取资源文件中的版本信息
\param hModule, exe，DLL 句柄
\return 返回
*/
// 从版本资源中获取 dll version
VERSION GetLibraryVersion(PCHAR LibraryName);
//////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__EAF55A74_001E_4A24_95AC_DD5DC64CFF61__INCLUDED_)
