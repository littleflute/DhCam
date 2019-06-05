#if !defined(AFX_TABPAGE1_H__B3F57D64_0173_4157_A07E_FEB4B02030E2__INCLUDED_)
#define AFX_TABPAGE1_H__B3F57D64_0173_4157_A07E_FEB4B02030E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabPage1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabPage1 dialog

class CTabPage1 : public CDialog
{
// Construction
public:
	CTabPage1(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTabPage1)
	enum { IDD = IDD_TAB_PAGE_1 };
	CString	m_strDllName;
	CString	m_strFunName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabPage1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabPage1)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABPAGE1_H__B3F57D64_0173_4157_A07E_FEB4B02030E2__INCLUDED_)
