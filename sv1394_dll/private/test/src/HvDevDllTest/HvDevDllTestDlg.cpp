// HvDevDllTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HvDevDllTest.h"
#include "HvDevDllTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHvDevDllTestDlg dialog

CHvDevDllTestDlg::CHvDevDllTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHvDevDllTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHvDevDllTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHvDevDllTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHvDevDllTestDlg)
	DDX_Control(pDX, IDC_TAB_MAIN, m_ctrlTab);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CHvDevDllTestDlg, CDialog)
	//{{AFX_MSG_MAP(CHvDevDllTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, OnSelchangeTabMain)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHvDevDllTestDlg message handlers

BOOL CHvDevDllTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	v_Init_UI();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CHvDevDllTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHvDevDllTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHvDevDllTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CHvDevDllTestDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CHvDevDllTestDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CHvDevDllTestDlg::v_Init_UI()
{
	TC_ITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_TEXT;
	TabCtrlItem.pszText = "配置";
	m_ctrlTab.InsertItem( 0, &TabCtrlItem );
	TabCtrlItem.pszText = "测试";
	m_ctrlTab.InsertItem( 1, &TabCtrlItem );
	TabCtrlItem.pszText = "Reserved";
	m_ctrlTab.InsertItem( 2, &TabCtrlItem );
	CRect rc;
	this->GetWindowRect(rc);
	if(m_ctrlTab.m_hWnd)	m_ctrlTab.MoveWindow(0,0,rc.Width(),rc.Height(),1);

	CRect tabRect, itemRect;//计算标签控件的位置和区域
	int nX, nY, nXc, nYc;
	m_ctrlTab.GetClientRect(&tabRect);
	m_ctrlTab.GetItemRect(0, &itemRect);
	nX=itemRect.left;
	nY=itemRect.bottom+15;
	nXc=tabRect.right-itemRect.left-1;
	nYc=tabRect.bottom-nY-1;

	m_Page1.Create(IDD_TAB_PAGE_1,this);  //创建标签页窗口 
	m_Page1.SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);//移动到指定位置显示
 
	m_Page2.Create(IDD_TAB_PAGE_2,this);  //创建标签页窗口 
	m_Page2.SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);//移动到指定位置显示

 
	this->SendMessage(WM_SIZE,0,MAKEWPARAM(rc.Width()-10,rc.Height()-35));

//	::PostMessage(m_hWnd,WM_SYSCOMMAND,SC_MAXIMIZE,0); 
	ShowWindow(SW_SHOWMAXIMIZED); //最大化主窗口
}

void CHvDevDllTestDlg::OnSelchangeTabMain(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nIdx = m_ctrlTab.GetCurSel();

	switch(nIdx)
	{
	case 0:
		TRACE("000");
   		m_Page1.ShowWindow(SW_SHOW);
 		m_Page2.ShowWindow(SW_HIDE);
//		m_Page3.ShowWindow(SW_HIDE);
		break;
	case 1:
		m_Page1.UpdateData(); 
		m_Page2._Set_Init(m_Page1.m_strDllName,m_Page1.m_strFunName);
   		m_Page1.ShowWindow(SW_HIDE);
 		m_Page2.ShowWindow(SW_SHOW);
//		m_Page3.ShowWindow(SW_HIDE);
		break;
	case 2:
   		m_Page1.ShowWindow(SW_HIDE);
		m_Page2.ShowWindow(SW_HIDE);
//		m_Page3.ShowWindow(SW_SHOW);
		break;
	}
	*pResult = 0;
}

void CHvDevDllTestDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	if(m_ctrlTab.m_hWnd)	
	{
		CRect tabRect, itemRect;//计算标签控件的位置和区域
		int nX, nY;
		m_ctrlTab.GetItemRect(0, &itemRect);
		m_ctrlTab.MoveWindow(0,0,cx,cy,1); 

		m_ctrlTab.GetClientRect(&tabRect);
		
		nX=itemRect.left;
		nY=itemRect.bottom+itemRect.Height()/2; 

		m_Page1.MoveWindow(nX,nY,tabRect.Width()-10,tabRect.Height()-itemRect.Height()*2,1); 
		m_Page2.MoveWindow(nX,nY,tabRect.Width()-10,tabRect.Height()-itemRect.Height()*2,1); 
//		m_Page3.MoveWindow(nX,nY,tabRect.Width()-10,tabRect.Height()-itemRect.Height()*2,1); 
	}
}
