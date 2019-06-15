// WBDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mdi.h"
#include "WBDlg.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWBDlg dialog


CWBDlg::CWBDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWBDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWBDlg)

	m_bounceWnd =(CBounceWnd*)pParent;
	m_dRatioR = 1.0;
	m_dRatioG = 1.0;
	m_dRatioB = 1.0;
	//}}AFX_DATA_INIT

	m_dRstRatioR = 1.0;
	m_dRstRatioG = 1.0;
	m_dRstRatioB = 1.0;


	m_bApply     = FALSE;
}


void CWBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWBDlg)
	DDX_Text(pDX, IDC_EDIT_RED_RATIO, m_dRatioR);
	DDX_Text(pDX, IDC_EDIT_GREEN_RATIO, m_dRatioG);
	DDX_Text(pDX, IDC_EDIT_BLUE_RATIO, m_dRatioB);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWBDlg, CDialog)
	//{{AFX_MSG_MAP(CWBDlg)
	ON_BN_CLICKED(ID_WB_RATIO, OnWbRatio)
	ON_BN_CLICKED(ID_WB_SET, OnWbSet)
	ON_EN_SETFOCUS(IDC_EDIT_RED_RATIO, OnSetfocusEditRedRatio)
	ON_EN_SETFOCUS(IDC_EDIT_BLUE_RATIO, OnSetfocusEditBlueRatio)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWBDlg message handlers


BOOL CWBDlg::OnInitDialog() 
{
	

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_dRatioR		= m_bounceWnd->m_dRatioR;
	m_dRatioG		= m_bounceWnd->m_dRatioG;
	m_dRatioB		= m_bounceWnd->m_dRatioB;

	m_dRstRatioR		= m_dRatioR;
	m_dRstRatioG		= m_dRatioG;
	m_dRstRatioB		= m_dRatioB;
    UpdateData(FALSE);
	GetDlgItem(ID_WB_SET)->EnableWindow(FALSE);

    if (m_bounceWnd)
	{
	  CString strTitle;
//	  strTitle.Format("White Balance-%s%d",m_bounceWnd->m_strCamerName,m_bounceWnd->m_nListNum);
	  strTitle.Format("White Balance-%s",m_bounceWnd->m_strCamerDispName);
	  SetWindowText(strTitle); 
	}
    
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void CWBDlg::ApplyRatio()
{
	m_bounceWnd->SetLutTable(m_dRatioR,m_dRatioG,m_dRatioB);
}



void CWBDlg::OnOK() 
{
	// TODO: Add extra validation here
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	ASSERT(pFrame);
	
	if (m_bApply) {
		ApplyRatio();
	}
	
	
	CDialog::OnOK();
}




void CWBDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	m_dRatioR	= m_dRstRatioR;
	m_bounceWnd->m_dRatioG	= m_dRstRatioG;
	m_bounceWnd->m_dRatioB	= m_dRstRatioB;

	m_bounceWnd->SetLutTable(m_dRstRatioR,m_dRstRatioG,m_dRstRatioB);


	CDialog::OnCancel();
}


void CWBDlg::OnWbRatio() 
{
	// TODO: Add your control notification handler code here
	
	
	m_bounceWnd->SendMessage(WM_GET_RATIO);
	
	m_dRatioR = m_bounceWnd->m_dRatioR;
	m_dRatioG = m_bounceWnd->m_dRatioG;
	m_dRatioB = m_bounceWnd->m_dRatioB;
	
	ApplyRatio();
	m_bApply  = TRUE;

	GetDlgItem(ID_WB_SET)->EnableWindow(FALSE);	
	UpdateData(FALSE);
	
}


void CWBDlg::OnWbSet() 
{

	
	UpdateData(TRUE);
	
	m_bounceWnd->m_dRatioR = m_dRatioR;
	m_bounceWnd->m_dRatioG= m_dRatioG;
	m_bounceWnd->m_dRatioB= m_dRatioB;	
	
	ApplyRatio();
	
	m_bApply  = TRUE;
	
}

void CWBDlg::OnSetfocusEditRedRatio() 
{
	GetDlgItem(ID_WB_SET)->EnableWindow(TRUE);
	
}

void CWBDlg::OnSetfocusEditBlueRatio() 
{
	GetDlgItem(ID_WB_SET)->EnableWindow(TRUE);
}


BOOL CWBDlg::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::Create(IDD, pParentWnd);
}
