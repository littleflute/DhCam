// HVPerf.cpp : main source file for HVPerf.exe
//

#include "stdafx.h"

#include "resource.h"

#include "ImageView.h"
#include "aboutdlg.h"
#include "ChildFrm.h"
#include "MainFrm.h"

CAppModule _Module;

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	// First check, if another instance of our application is already running
	// To do this check a named kernel object is created 
	// If the creation fails we know that our application is already running
	
	HANDLE hExclusion = NULL;   // used to detect another running instance
	hExclusion = CreateMutex(NULL, FALSE, "{8FA6E763-1394-4db0-AFC3-FA53ED373513}");
	
	if ( GetLastError() == ERROR_ALREADY_EXISTS )
	{
		// The mutex already exists, bring the running instance to top
		
		HWND hWndPrev, hWndChild;
		
		// Determine if another window with your class name exists...
		if (hWndPrev = ::FindWindow("Daheng Imavision HV Performance", NULL))
		{
			// If so, does it have any popups?
			hWndChild = ::GetLastActivePopup(hWndPrev);
			
			// If iconic, restore the main window
			if (::IsIconic(hWndPrev))
				::ShowWindow(hWndPrev, SW_RESTORE);
			
			// Bring the main window or its popup to
			// the foreground
			::SetForegroundWindow(hWndChild);
			
			// now the previous instance is activated 
			return 0;
		}
    }
		
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainFrame wndMain;

	if(wndMain.CreateEx() == NULL)
	{
		ATLTRACE(_T("Main window creation failed!\n"));
		return 0;
	}
	
	wndMain.ShowWindow(nCmdShow | SW_SHOWNORMAL);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	if ( hExclusion != NULL )
		CloseHandle(hExclusion);

	return nRet;
}



int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));


	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
