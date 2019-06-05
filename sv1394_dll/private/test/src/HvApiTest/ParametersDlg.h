#if !defined(AFX_PARAMETERSDLG_H__90DD695E_FFB7_4246_9DAC_AF54C08C2DA5__INCLUDED_)
#define AFX_PARAMETERSDLG_H__90DD695E_FFB7_4246_9DAC_AF54C08C2DA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParametersDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CParametersDlg dialog

class CParametersDlg : public CDialog
{
// Construction
public:
	CParametersDlg(CWnd* pParent = NULL);   // standard constructor
	int nSleepTime;
	int	m_nAoiTestMode;
	int m_nAoiLangeControl;
// Dialog Data
	//{{AFX_DATA(CParametersDlg)
	enum { IDD = IDD_DIALOG_PARAMETERS };
	CComboBox	m_CtrlAoiLangeControl;
	CString	m_strAoiLangeControl;
	CComboBox	m_CtrlAoiTestMode;
	CString	m_strAoiTestMode;
	CString	m_StartCoordinate;
	CString	m_EndCoordinate;
	int		m_AOI_Precision;
	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParametersDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CParametersDlg)
	afx_msg void OnReleasedcaptureSliderSleepTime(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawSliderSleepTime(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboAoiTestMode();
	afx_msg void OnSelendokComboAoiTestMode();
	afx_msg void OnSelchangeComboAoiTestLange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAMETERSDLG_H__90DD695E_FFB7_4246_9DAC_AF54C08C2DA5__INCLUDED_)
