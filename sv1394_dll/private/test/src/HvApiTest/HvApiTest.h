// DhTest.h : main header file for the DHTEST application
//

#if !defined(AFX_DHTEST_H__50DB839D_1E2C_40EA_8051_3AECC76B0A30__INCLUDED_)
#define AFX_DHTEST_H__50DB839D_1E2C_40EA_8051_3AECC76B0A30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDhTestApp:
// See DhTest.cpp for the implementation of this class
//

class CDhTestApp : public CWinApp
{
private:
	
	BOOL		m_bShowUI;

public:
	CDhTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDhTestApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDhTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DHTEST_H__50DB839D_1E2C_40EA_8051_3AECC76B0A30__INCLUDED_)
