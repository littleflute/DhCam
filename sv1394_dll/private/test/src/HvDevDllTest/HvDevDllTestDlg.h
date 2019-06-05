// HvDevDllTestDlg.h : header file
//

#if !defined(AFX_HVDEVDLLTESTDLG_H__08B91BC0_3819_4843_949C_234C42C7753F__INCLUDED_)
#define AFX_HVDEVDLLTESTDLG_H__08B91BC0_3819_4843_949C_234C42C7753F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TabPage1.h"
#include "TabPage2.h"
/////////////////////////////////////////////////////////////////////////////
// CHvDevDllTestDlg dialog

class CHvDevDllTestDlg : public CDialog
{
// Construction
public:
	CHvDevDllTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CHvDevDllTestDlg)
	enum { IDD = IDD_HVDEVDLLTEST_DIALOG };
	CTabCtrl	m_ctrlTab;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHvDevDllTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CHvDevDllTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelchangeTabMain(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void v_Init_UI();
	CTabPage1		m_Page1;
	CTabPage2		m_Page2;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HVDEVDLLTESTDLG_H__08B91BC0_3819_4843_949C_234C42C7753F__INCLUDED_)
