// mdi.cpp : Defines the class behaviors for the application.
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "mdi.h"

#include "mainfrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define UNIQE_NAME "{D6F43C09-2353-40d0-A60F-7E6D0F2FF7AD}"
/////////////////////////////////////////////////////////////////////////////
// CMdiApp

BEGIN_MESSAGE_MAP(CMdiApp, CWinApp)
	//{{AFX_MSG_MAP(CMdiApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMdiApp construction
// Place all significant initialization in InitInstance

CMdiApp::CMdiApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMdiApp object

CMdiApp NEAR theApp;

/////////////////////////////////////////////////////////////////////////////
// CMdiApp initialization

BOOL CMdiApp::InitInstance()
{
	Enable3dControls();


	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	m_hOneInstance = ::CreateMutex(NULL, FALSE, UNIQE_NAME); 
	if(GetLastError() == ERROR_ALREADY_EXISTS) 
	{
		AfxMessageBox("The program is running!");
		return FALSE;
	}

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();
	m_pMainWnd = pMainFrame;

	// load the two static menus
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMdiApp commands

void CMdiApp::OnAppAbout()
{
	CDialog(IDD_ABOUTBOX).DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// other globals


/////////////////////////////////////////////////////////////////////////////
