// HVColor.h : main header file for the HVCOLOR DLL
//

#if !defined(AFX_HVCOLOR_H__CD75486A_7431_4AB5_9809_7D388725B514__INCLUDED_)
#define AFX_HVCOLOR_H__CD75486A_7431_4AB5_9809_7D388725B514__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "ColorRender_Ex.h"

//////////////////////////////////////////////////////////////////////////
// User message 

#define WM_SETPARAMDLG_UPDATE	(WM_USER + 101)

//////////////////////////////////////////////////////////////////////////
// Color render object

extern CColorRender_Ex ColorRender;

//////////////////////////////////////////////////////////////////////////
// User message parameter

typedef struct tagIMAGE{
	void * pSrc;
	void * pDst;
	int    nWid;
	int    nHei;
} IMAGE;

/////////////////////////////////////////////////////////////////////////////
// CHVColorApp
// See HVColor.cpp for the implementation of this class
//

class CHVColorApp : public CWinApp
{
public:
	CHVColorApp();

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHVColorApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CHVColorApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HVCOLOR_H__CD75486A_7431_4AB5_9809_7D388725B514__INCLUDED_)
