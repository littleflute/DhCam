// TabPage1.cpp : implementation file
//

#include "stdafx.h"
#include "HvDevDllTest.h"
#include "TabPage1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabPage1 dialog


CTabPage1::CTabPage1(CWnd* pParent /*=NULL*/)
	: CDialog(CTabPage1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabPage1)
	m_strDllName = _T("HVAPI");
	m_strFunName = _T("HVEnumerate");
	//}}AFX_DATA_INIT
}


void CTabPage1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabPage1)
	DDX_Text(pDX, IDC_EDIT_DLL_NAME, m_strDllName);
	DDX_Text(pDX, IDC_EDIT_FUNCTION_NAME, m_strFunName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabPage1, CDialog)
	//{{AFX_MSG_MAP(CTabPage1)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabPage1 message handlers

void CTabPage1::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	CStatic *pSt = (CStatic*)GetDlgItem(IDC_STATIC_BOX);
	if(pSt)
	pSt->MoveWindow(10,0,cx-10,cy,1);
	
}
