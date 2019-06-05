// HvDevDllTest.h : main header file for the HVDEVDLLTEST application
//

#if !defined(AFX_HVDEVDLLTEST_H__3E9E8490_BA6A_48FA_87DB_7815B45601F4__INCLUDED_)
#define AFX_HVDEVDLLTEST_H__3E9E8490_BA6A_48FA_87DB_7815B45601F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CHvDevDllTestApp:
// See HvDevDllTest.cpp for the implementation of this class
//

class CHvDevDllTestApp : public CWinApp
{
public:
	CHvDevDllTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHvDevDllTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CHvDevDllTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HVDEVDLLTEST_H__3E9E8490_BA6A_48FA_87DB_7815B45601F4__INCLUDED_)
