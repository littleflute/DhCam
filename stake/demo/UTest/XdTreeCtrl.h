#if !defined(AFX_XDTREECTRL_H__2FD780E2_80BD_45AF_8594_813EABFB2D25__INCLUDED_)
#define AFX_XDTREECTRL_H__2FD780E2_80BD_45AF_8594_813EABFB2D25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XdTreeCtrl.h : header file
//
#include "afxcmn.h"
/////////////////////////////////////////////////////////////////////////////
// CXdTreeCtrl window

class CXdTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CXdTreeCtrl();

// Attributes
public:
   CString m_strName;///English:    ///Chinese: 记录树型控件中的名字
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXdTreeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CXdTreeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CXdTreeCtrl)
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XDTREECTRL_H__2FD780E2_80BD_45AF_8594_813EABFB2D25__INCLUDED_)
