// stdafx.cpp : source file that includes just the standard includes
//	HVDAILT.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

int glcount = 0;
//HV_DEVICE_INTERFACE* glDevInterface = NULL;
HV_DEVICE_GRAPH*     glDevGraph = NULL;
DevIndex2DevGraph    g_DevIndex2DevGraph;

#include <Winver.h>
#include <tchar.h>
#pragma comment(lib,"Version.lib")
VERSION GetLibraryVersion(PCHAR LibraryName)
{
	VERSION Version={0};
	HMODULE hModule = LoadLibrary(LibraryName);
	if (hModule!=NULL) {
		TCHAR szFullPath[MAX_PATH];
		DWORD dwVerInfoSize = 0;
		DWORD dwVerHnd;
		VS_FIXEDFILEINFO *pFileInfo = NULL;    
		GetModuleFileName(hModule, szFullPath, sizeof(szFullPath));
		dwVerInfoSize = GetFileVersionInfoSize(szFullPath, &dwVerHnd);
		if (dwVerInfoSize)
		{
			// If we were able to get the information, process it:
			HANDLE  hMem;
			LPVOID  lpvMem;
			unsigned int uInfoSize = 0;
			hMem = GlobalAlloc(GMEM_MOVEABLE, dwVerInfoSize);
			lpvMem = GlobalLock(hMem);
			GetFileVersionInfo(szFullPath, dwVerHnd, dwVerInfoSize, lpvMem);
			::VerQueryValue(lpvMem, (LPTSTR)_T("\\"), (void**)&pFileInfo, &uInfoSize);        
			// Product version from the FILEVERSION of the version info resource     
			Version.wMajor    = HIWORD(pFileInfo->dwProductVersionMS); 
			Version.wMinor    = LOWORD(pFileInfo->dwProductVersionMS);
			Version.wRevision = HIWORD(pFileInfo->dwProductVersionLS);
			Version.wBuild      = LOWORD(pFileInfo->dwProductVersionLS);     
			GlobalUnlock(hMem);
			GlobalFree(hMem);
		}
		FreeLibrary(hModule);
	}    
    return Version;
}
	

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
