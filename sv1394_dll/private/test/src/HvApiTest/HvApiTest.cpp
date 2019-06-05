// HvApiTest.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "HvApiTest.h"
#include "HvApiTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDhTestApp

BEGIN_MESSAGE_MAP(CDhTestApp, CWinApp)
	//{{AFX_MSG_MAP(CDhTestApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDhTestApp construction

CDhTestApp::CDhTestApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDhTestApp object

CDhTestApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDhTestApp initialization

BOOL CDhTestApp::InitInstance()
{
	HANDLE hExclusion = NULL;   // used to detect another running instance
	hExclusion = CreateMutex(NULL, FALSE, "{8FA6E763-4480-0000-AFC3-FA53ED373513}");
	
	if ( GetLastError() == ERROR_ALREADY_EXISTS )
	{
		// The mutex already exists, bring the running instance to top
		
		HWND hWndPrev, hWndChild;
		
		// Determine if another window with your class name exists...
		if (hWndPrev = ::FindWindow(NULL, "DhTest"))
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
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CDhTestDlg dlg;
	m_pMainWnd = &dlg;
	m_bShowUI = TRUE;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	::CloseHandle(hExclusion);

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

BOOL CDhTestApp::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	/*
	if(pMsg->message == WM_HOTKEY)
	{
		if(LOWORD(pMsg->lParam
				m_bShowUI = TRUE;
				m_pMainWnd->ShowWindow(SW_SHOW);
			}
		}) == MOD_CONTROL && HIWORD(pMsg->lParam)=='T') 
		{
			if(m_bShowUI){
				m_bShowUI = FALSE;	
				m_pMainWnd->ShowWindow(SW_HIDE); 
			}
			else{
	}
	*/
	return CWinApp::PreTranslateMessage(pMsg);
}
