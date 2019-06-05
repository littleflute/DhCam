// DhTestDlg.h : header file
//

#if !defined(AFX_DHTESTDLG_H__860FBD30_10F9_40D8_8ED2_4DF0F358DE57__INCLUDED_)
#define AFX_DHTESTDLG_H__860FBD30_10F9_40D8_8ED2_4DF0F358DE57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define BTN_NUM			5
#include "ParametersDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CDhTestDlg dialog

class CDhTestDlg : public CDialog
{
// Construction
public:
	CDhTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDhTestDlg)
	enum { IDD = IDD_DHTEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDhTestDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON			m_hIcon;
	CToolBar		m_wndToolBar;
	CStatusBar  	m_wndStatusBar;
	CToolTipCtrl    m_pToolTipCtrl;
	

	// Generated message map functions
	//{{AFX_MSG(CDhTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnHelpAbout();
	afx_msg void OnFileExit();
	afx_msg void OnTestItem_1();
	afx_msg void OnTest_HVGetDeviceInfo();
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void xdToolbarCtrlBtn(UINT nID);
	afx_msg void OnHotKey(LPARAM lParam,WPARAM wParam); 
	afx_msg void On_UserTest1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void v_Start_AOI_Test();
	static void v_AOI_Test_Thread0(LPVOID lpConText);
	static void v_AOI_Test_Thread1(LPVOID lpConText);
	static void Xd_StarStop_Test_Thread(LPVOID lpConText);
	static void Xd_SoftTrigger_Test_Thread(LPVOID lpConText);
	
	void xd_InitToolbarCtrl();
	BOOL xdToolTipNotify( UINT id, NMHDR * pTTTStruct, LRESULT * pResult );

	int AOIFreq(int Range, int Precision);
	
	BOOL		m_bAoiTest;
	BOOL        m_bStartStopTest;
	BOOL        m_bTriggerTest;
	BOOL        m_bTriggerBtn;
	int			m_nSleepTime;
	int			m_nAoiTestMode;
	HWND        m_hTriggerBtn;
	CParametersDlg dlg;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DHTESTDLG_H__860FBD30_10F9_40D8_8ED2_4DF0F358DE57__INCLUDED_)
