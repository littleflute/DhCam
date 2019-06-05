// ParametersDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hvapitest.h"
#include "ParametersDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParametersDlg dialog


CParametersDlg::CParametersDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CParametersDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CParametersDlg)
	m_strAoiTestMode = _T("MODE1");
	m_nAoiTestMode = 0;
	m_AOI_Precision = 16;
	m_StartCoordinate = _T("0,0");
	m_EndCoordinate = _T("320,320");
	nSleepTime = 1000;
	m_strAoiLangeControl = _T("Unable");
	m_nAoiLangeControl = 1;
	//}}AFX_DATA_INIT
}


void CParametersDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CParametersDlg)
	DDX_Control(pDX, IDC_COMBO_AOI_TEST_LANGE, m_CtrlAoiLangeControl);
	DDX_Control(pDX, IDC_COMBO_AOI_TEST_MODE, m_CtrlAoiTestMode);
	DDX_CBString(pDX, IDC_COMBO_AOI_TEST_MODE, m_strAoiTestMode);
	DDX_Text(pDX, IDC_EDIT_PRECISION, m_AOI_Precision);
	DDX_Text(pDX, IDC_COORDINATE_START, m_StartCoordinate);
	DDX_Text(pDX, IDC_COORDINATE_END, m_EndCoordinate);
	DDX_CBString(pDX, IDC_COMBO_AOI_TEST_LANGE, m_strAoiLangeControl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParametersDlg, CDialog)
	//{{AFX_MSG_MAP(CParametersDlg)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_SLEEP_TIME, OnReleasedcaptureSliderSleepTime)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SLEEP_TIME, OnCustomdrawSliderSleepTime)
	ON_CBN_SELCHANGE(IDC_COMBO_AOI_TEST_MODE, OnSelchangeComboAoiTestMode)
	ON_CBN_SELENDOK(IDC_COMBO_AOI_TEST_MODE, OnSelendokComboAoiTestMode)
	ON_CBN_SELCHANGE(IDC_COMBO_AOI_TEST_LANGE, OnSelchangeComboAoiTestLange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParametersDlg message handlers

void CParametersDlg::OnReleasedcaptureSliderSleepTime(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CSliderCtrl *pSlider = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_SLEEP_TIME);
	int pos=pSlider->GetPos();
	
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_STATIC_SLEEP_TIME_VALUE);
	CString str;
	str.Format("%d", pos);
	pEdit->SetWindowText(str);
	nSleepTime=pos;	
	*pResult = 0;
}

void CParametersDlg::OnCustomdrawSliderSleepTime(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CSliderCtrl *pSlider = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_SLEEP_TIME);
	int pos=pSlider->GetPos();
	
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_STATIC_SLEEP_TIME_VALUE);
	CString str;
	str.Format("%d", pos);
	pEdit->SetWindowText(str);
	//	pFrame->m_nSleepTime=pos;
	*pResult = 0;
}

BOOL CParametersDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	CSliderCtrl *pSlider = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_SLEEP_TIME);	 //ÐÞ¸ÄÁÁ¶ÈÖµ·¶Î§
	pSlider->SetRange(0,5000,TRUE);
	pSlider->SetPos(nSleepTime);
	if (m_nAoiLangeControl) {
		GetDlgItem(IDC_COORDINATE_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_COORDINATE_END)->EnableWindow(FALSE);
	}else{
		GetDlgItem(IDC_COORDINATE_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_COORDINATE_END)->EnableWindow(TRUE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CParametersDlg::OnSelchangeComboAoiTestMode() 
{
	// TODO: Add your control notification handler code here
	m_nAoiTestMode = m_CtrlAoiTestMode.GetCurSel();
}

void CParametersDlg::OnSelendokComboAoiTestMode() 
{
	// TODO: Add your control notification handler code here
	
}



void CParametersDlg::OnSelchangeComboAoiTestLange() 
{
	// TODO: Add your control notification handler code here
	m_nAoiLangeControl = m_CtrlAoiLangeControl.GetCurSel();
	if (m_nAoiLangeControl) {
		GetDlgItem(IDC_COORDINATE_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_COORDINATE_END)->EnableWindow(FALSE);
	}else{
		GetDlgItem(IDC_COORDINATE_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_COORDINATE_END)->EnableWindow(TRUE);
	}
}
