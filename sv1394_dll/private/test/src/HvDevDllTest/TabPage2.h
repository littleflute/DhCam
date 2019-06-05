#if !defined(AFX_TABPAGE2_H__4BD01BBD_7764_4723_8F19_F56C7FC9EBBE__INCLUDED_)
#define AFX_TABPAGE2_H__4BD01BBD_7764_4723_8F19_F56C7FC9EBBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabPage2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabPage2 dialog

class CTabPage2 : public CDialog
{
// Construction
public:
	void _Set_Init(CString strDllName, CString strFunName);
	CTabPage2(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTabPage2)
	enum { IDD = IDD_TAB_PAGE_2 };
	CListCtrl	m_ctrlDeviceList;
	CString	m_strDllName;
	CString	m_strFunName; 
	CString	m_strTimeStart;
	CString	m_strTimeStop;
	CString	m_strTimeElapse;
	CString	m_strGUID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabPage2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabPage2)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBtnTest();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListEnumDevice(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void v_UpdateUI(bool b);
	void v_Test_Enumerate();
	void v_Test_HVGuid();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABPAGE2_H__4BD01BBD_7764_4723_8F19_F56C7FC9EBBE__INCLUDED_)
