// DhTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HvApiTest.h"
#include "HvApiTestDlg.h"
#include "HVAPI_Test.h"

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
	CString	m_strVersion;
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
	m_strVersion = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_STATIC_VERSION, m_strVersion);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT auIDStatusBar[] = 
{ 
	ID_SEPARATOR
};

/////////////////////////////////////////////////////////////////////////////
// CDhTestDlg dialog

CDhTestDlg::CDhTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDhTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDhTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDhTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDhTestDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDhTestDlg, CDialog)
	//{{AFX_MSG_MAP(CDhTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
	ON_COMMAND(ID_FILE_EXIT, OnFileExit)
	ON_COMMAND(ID_TEST_ITEM_1 , OnTestItem_1)
	ON_COMMAND(ID_TEST_HVGETDEVINFO , OnTest_HVGetDeviceInfo)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND_RANGE(ID_BUTTON_1,ID_BUTTON_4,xdToolbarCtrlBtn)
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, xdToolTipNotify )
	ON_MESSAGE(WM_HOTKEY,OnHotKey)
	ON_COMMAND(IDC_USER_TEST_1,On_UserTest1)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDhTestDlg message handlers

BOOL CDhTestDlg::OnInitDialog()
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
	if(!m_wndToolBar.CreateEx(this,
		WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_GRIPPER|
		CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC)
		||!m_wndToolBar.LoadToolBar(IDR_TOOLBAR1))

	{
		TRACE0("创建工具条失败\n");
		return -1;
	}

/*	
	//创建状态栏
	if(m_wndStatusBar.CreateEx(this,0,WS_CHILD|WS_VISIBLE|CBRS_BOTTOM,AFX_IDW_STATUS_BAR))
	{
		m_wndStatusBar.SetIndicators(auIDStatusBar, sizeof(auIDStatusBar) / sizeof(UINT));
		m_wndStatusBar.SetWindowText("HVdailt.dll Test");
	}
*/	
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);

	//////////////////////////////////////////////////////////////////
	EnableToolTips(TRUE);	
	m_pToolTipCtrl.Create(this,TTS_ALWAYSTIP);	    
	m_pToolTipCtrl.Activate(TRUE);	
	/////////////////////////////////////////////////////////////////////
	m_bAoiTest = FALSE;
	m_bStartStopTest = FALSE;
	m_bTriggerTest = FALSE;
	m_bTriggerBtn = FALSE;
	
	RegisterHotKey(m_hWnd,ID_BUTTON_1,MOD_CONTROL,'P');
	RegisterHotKey(m_hWnd,ID_BUTTON_2,MOD_CONTROL,'A');
	RegisterHotKey(m_hWnd,ID_BUTTON_3,MOD_CONTROL,'S');
	RegisterHotKey(m_hWnd,ID_BUTTON_4,MOD_CONTROL,'T');
	m_nSleepTime = dlg.nSleepTime;
	m_nAoiTestMode = dlg.m_nAoiTestMode;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL CDhTestDlg::xdToolTipNotify( UINT id, NMHDR *pNMHDR, LRESULT *pResult )
{
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
    UINT nID =pNMHDR->idFrom;
    if (pTTT->uFlags & TTF_IDISHWND)
    {
        // idFrom is actually the HWND of the tool
        nID = ::GetDlgCtrlID((HWND)nID);
        if(nID)
        {
           pTTT->lpszText = MAKEINTRESOURCE(nID);
           pTTT->hinst = AfxGetResourceHandle();
           return(TRUE);
        }
    }
    return(FALSE);
}
void CDhTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		OnHelpAbout();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDhTestDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	if (IsIconic())
	{

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
HCURSOR CDhTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDhTestDlg::OnHelpAbout() 
{
	// TODO: Add your command handler code here

	CAboutDlg dlg;

	CString szRet;

	CString strPath; 
	
	TCHAR szFull[_MAX_PATH]; 
	
	
	
	::GetModuleFileName(NULL, szFull, sizeof(szFull)/sizeof(TCHAR)); 
	/*
	    TCHAR szDrive[_MAX_DRIVE]; 
	
	    TCHAR szDir[_MAX_DIR]; 
	
		_tsplitpath(szFull, szDrive, szDir, NULL, NULL); //分解出盘符,和目录
		
		_tcscpy(szFull, szDrive); 
		
		_tcscat(szFull, szDir); 
		
		strPath = CString(szFull); 
		AfxMessageBox(strPath);
		*/
	
	CFileVersion fv(szFull);
//	szRet = fv.GetFullVersion();
//	AfxMessageBox(szRet);
	//...
	szRet = fv.GetFileVersion();
	
	dlg.m_strVersion="版本: "+szRet;
	dlg.DoModal();
}

void CDhTestDlg::OnFileExit() 
{
	// TODO: Add your command handler code here

	::PostQuitMessage(WM_QUIT);
}

void CDhTestDlg::OnTest_HVGetDeviceInfo() 
{
	HVSTATUS status;	
	HINSTANCE hHvdailtInst;
	hHvdailtInst = LoadLibrary("HVDAILT.dll");	

	BeginHVDeviceFunc BeginHVDevice = (BeginHVDeviceFunc)::GetProcAddress(hHvdailtInst,"BeginHVDevice");
	EndHVDeviceFunc EndHVDevice = (EndHVDeviceFunc)::GetProcAddress(hHvdailtInst,"EndHVDevice");
	HVCommandFunc HVCommand = (HVCommandFunc)::GetProcAddress(hHvdailtInst,"HVCommand");

	
	HHV hhv;
	CString str;
	status = BeginHVDevice(1, &hhv);
    if(STATUS_OK != status) 
	{ 
		str.Format("调用 BeginHVDevice 接口函数失败!\n status = %d 0x%x",status,status);
		AfxMessageBox(str,MB_ICONERROR);
		return;
	}
	else 
	{
		AfxMessageBox("调用 BeginHVDevice 接口函数成功!");

		HVGetDeviceInfo_Func HVGetDeviceInfo = (HVGetDeviceInfo_Func)::GetProcAddress(hHvdailtInst,"HVGetDeviceInfo");
		HVTYPE hvType; 
    	int nSize;
		BYTE szBuf[256] = {0};
		DWORD dwBuf;	
//*
		//DESC_DEVICE_TYPE
	    status= HVGetDeviceInfo(hhv, DESC_DEVICE_TYPE, NULL,&nSize);
		if(status==STATUS_OK){
			status= HVGetDeviceInfo(hhv, DESC_DEVICE_TYPE, &hvType,&nSize);
			if(status==STATUS_OK){
				str.Format("HVGetDeviceInfo - DESC_DEVICE_TYPE 调用成功:\n hvType = %d \n nSize = %d",hvType,nSize);
				AfxMessageBox(str);
			}
		}
		if(status!=STATUS_OK){
			str.Format("HVGetDeviceInfo - DESC_DEVICE_TYPE  调用失败:\n status = %d 0x%x",status,status);
			AfxMessageBox(str,MB_ICONERROR);
		}

		//DESC_RESOLUTION

		status= HVGetDeviceInfo(hhv, DESC_RESOLUTION, NULL,&nSize);
		if(status==STATUS_OK){
			status= HVGetDeviceInfo(hhv, DESC_RESOLUTION, szBuf,&nSize);
			if(status==STATUS_OK){
				str.Format("HVGetDeviceInfo - DESC_RESOLUTION 调用成功:\n");
				int count = nSize /  (2 * sizeof(DWORD));
				
				CString strDesc;
				for (int i = 0; i < count; ++i)
				{
					strDesc.Format("mode %d (%d * %d)", 
						i, *((DWORD *)szBuf + 2 * i), *((DWORD *)szBuf + 2 * i + 1));
					str += strDesc;
				}
				AfxMessageBox(str);
			}
		}
		if(status!=STATUS_OK){
			str.Format("HVGetDeviceInfo - DESC_RESOLUTION  调用失败:\n status = %d 0x%x",status,status);
			AfxMessageBox(str,MB_ICONERROR);
		}

		//DESC_DEVICE_MARK

		status= HVGetDeviceInfo(hhv, DESC_DEVICE_MARK, NULL,&nSize);
		if(status==STATUS_OK){
			status= HVGetDeviceInfo(hhv, DESC_DEVICE_MARK, szBuf,&nSize);
			if(status==STATUS_OK){
				str.Format("HVGetDeviceInfo - DESC_DEVICE_MARK 调用成功:\n ... ");				
				AfxMessageBox(str);
			}
		}
		if(status!=STATUS_OK){
			str.Format("HVGetDeviceInfo - DESC_DEVICE_MARK  调用失败:\n status = %d 0x%x",status,status);
			AfxMessageBox(str,MB_ICONERROR);
		}

		//DESC_DEVICE_SERIESNUM

		status= HVGetDeviceInfo(hhv, DESC_DEVICE_SERIESNUM, NULL,&nSize);
		if(status==STATUS_OK){
			status= HVGetDeviceInfo(hhv, DESC_DEVICE_SERIESNUM, szBuf,&nSize);
			if(status==STATUS_OK){
				str.Format("HVGetDeviceInfo - DESC_DEVICE_SERIESNUM 调用成功:\n SN: %s ",szBuf);
				
				AfxMessageBox(str);
			}
		}
		if(status!=STATUS_OK){
			str.Format("HVGetDeviceInfo - DESC_DEVICE_SERIESNUM  调用失败:\n status = %d 0x%x",status,status);
			AfxMessageBox(str,MB_ICONERROR);
		}

		//DESC_DEVICE_BLANKSIZE

		status= HVGetDeviceInfo(hhv, DESC_DEVICE_BLANKSIZE, NULL,&nSize);
		if(status==STATUS_OK){
			status= HVGetDeviceInfo(hhv, DESC_DEVICE_BLANKSIZE, szBuf,&nSize);
			if(status==STATUS_OK){
				str.Format("HVGetDeviceInfo - DESC_DEVICE_BLANKSIZE 调用成功:\n ... ");				
				AfxMessageBox(str);
			}
		}
		if(status!=STATUS_OK){
			str.Format("HVGetDeviceInfo - DESC_DEVICE_BLANKSIZE  调用失败:\n status = %d 0x%x",status,status);
			AfxMessageBox(str,MB_ICONERROR);
		}
		
		//DESC_DEVICE_CHIPID
		WORD wBuf;
		status= HVGetDeviceInfo(hhv, DESC_DEVICE_CHIPID, NULL,&nSize);
		if(status==STATUS_OK){
			status= HVGetDeviceInfo(hhv, DESC_DEVICE_CHIPID, &wBuf,&nSize);
			if(status==STATUS_OK){
				str.Format("HVGetDeviceInfo - DESC_DEVICE_CHIPID 调用成功:\n [Chip ID]: %d (0x%x) ",wBuf,wBuf);
				
				AfxMessageBox(str);
			}
		}
		if(status!=STATUS_OK){
			str.Format("HVGetDeviceInfo - DESC_DEVICE_CHIPID  调用失败:\n status = %d 0x%x",status,status);
			AfxMessageBox(str,MB_ICONERROR);
		}
		
		//DESC_DEVICE_HARDWARE_VERSION
		status= HVGetDeviceInfo(hhv, DESC_DEVICE_HARDWARE_VERSION, NULL,&nSize);
		if(status==STATUS_OK){
			status= HVGetDeviceInfo(hhv, DESC_DEVICE_HARDWARE_VERSION, &dwBuf,&nSize);
			if(status==STATUS_OK){
				str.Format("HVGetDeviceInfo - DESC_DEVICE_HARDWARE_VERSION 调用成功:\n [HardWare Ver]: %d (0x%x) ",dwBuf,dwBuf);
				
				AfxMessageBox(str);
			}
		}
		if(status!=STATUS_OK){
			str.Format("HVGetDeviceInfo - DESC_DEVICE_HARDWARE_VERSION  调用失败:\n status = %d 0x%x",status,status);
			AfxMessageBox(str,MB_ICONERROR);
		}
	
		//DESC_DEVICE_NAME
		status= HVGetDeviceInfo(hhv, DESC_DEVICE_NAME, NULL,&nSize);
		if(status==STATUS_OK){
			status= HVGetDeviceInfo(hhv, DESC_DEVICE_NAME, szBuf,&nSize);
			if(status==STATUS_OK){
				str.Format("HVGetDeviceInfo - DESC_DEVICE_NAME 调用成功:\n [Device Name]: %s \n nSize = %d",szBuf,nSize);
				
				AfxMessageBox(str);
			}
		}
		if(status!=STATUS_OK){
			str.Format("HVGetDeviceInfo - DESC_DEVICE_NAME  调用失败:\n status = %d 0x%x",status,status);
			AfxMessageBox(str,MB_ICONERROR);
		}

		//DESC_DEVICE_SOFTWARE_VERSION
		status= HVGetDeviceInfo(hhv, DESC_DEVICE_SOFTWARE_VERSION, NULL,&nSize);
		if(status==STATUS_OK){
			status= HVGetDeviceInfo(hhv, DESC_DEVICE_SOFTWARE_VERSION, szBuf,&nSize);
			if(status==STATUS_OK){
				str.Format("HVGetDeviceInfo - DESC_DEVICE_SOFTWARE_VERSION 调用成功:\n [Software V]: %s \n nSize = %d",szBuf,nSize);
				
				AfxMessageBox(str);
			}
		}
		if(status!=STATUS_OK){
			str.Format("HVGetDeviceInfo - DESC_DEVICE_SOFTWARE_VERSION  调用失败:\n status = %d 0x%x",status,status);
			AfxMessageBox(str,MB_ICONERROR);
		}

		//DESC_DEVICE_FPGA_VERSION
		status= HVGetDeviceInfo(hhv, DESC_DEVICE_FPGA_VERSION, NULL,&nSize);
		if(status==STATUS_OK){
			status= HVGetDeviceInfo(hhv, DESC_DEVICE_FPGA_VERSION, &dwBuf,&nSize);
			if(status==STATUS_OK){
				str.Format("HVGetDeviceInfo - DESC_DEVICE_FPGA_VERSION 调用成功:\n [FPGA V]: %d \n nSize = %d",dwBuf,nSize);
				
				AfxMessageBox(str);
			}
		}
		if(status!=STATUS_OK){
			str.Format("HVGetDeviceInfo - DESC_DEVICE_FPGA_VERSION  调用失败:\n status = %d 0x%x",status,status);
			AfxMessageBox(str,MB_ICONERROR);
		}

		//DESC_DEVICE_VENDOR_NAME
		status= HVGetDeviceInfo(hhv, DESC_DEVICE_VENDOR_NAME, NULL,&nSize);
		if(status==STATUS_OK){
			status= HVGetDeviceInfo(hhv, DESC_DEVICE_VENDOR_NAME, szBuf,&nSize);
			if(status==STATUS_OK){
				str.Format("HVGetDeviceInfo - DESC_DEVICE_VENDOR_NAME 调用成功:\n [Vendor Name]: %s \n nSize = %d",szBuf,nSize);
				
				AfxMessageBox(str);
			}
		}
		if(status!=STATUS_OK){
			str.Format("HVGetDeviceInfo - DESC_DEVICE_VENDOR_NAME  调用失败:\n status = %d 0x%x",status,status);
			AfxMessageBox(str,MB_ICONERROR);
		}
//*/
		//DESC_DEVICE_BAYER_LAYOUT
		status= HVGetDeviceInfo(hhv, DESC_DEVICE_BAYER_LAYOUT, NULL,&nSize);
		if(status==STATUS_OK){
			status= HVGetDeviceInfo(hhv, DESC_DEVICE_BAYER_LAYOUT, &dwBuf,&nSize);
			if(status==STATUS_OK){
				str.Format("HVGetDeviceInfo - DESC_DEVICE_BAYER_LAYOUT 调用成功:\n [Bayer Layout]: %d \n nSize = %d",dwBuf,nSize);
				
				AfxMessageBox(str);
			}
		}
		if(status!=STATUS_OK){
			str.Format("HVGetDeviceInfo - DESC_DEVICE_BAYER_LAYOUT  调用失败:\n status = %d 0x%x",status,status);
			AfxMessageBox(str,MB_ICONERROR);
		}
	}

	status = EndHVDevice(hhv);
	if(STATUS_OK != status) 
	{ 
		str.Format("调用 EndHVDevice 接口函数失败!\n status = %d 0x%x",status,status);
		AfxMessageBox(str,MB_ICONERROR);
	}
	else{
		AfxMessageBox("调用 EndHVDevice 接口函数成功!");
	}

}
void CDhTestDlg::OnTestItem_1() 
{
	// TODO: Add your command handler code here
	m_bTriggerBtn=!m_bTriggerBtn;
	CMenu* menu = this->GetMenu();
	CMenu* submenu = menu->GetSubMenu(1);

	if(m_bTriggerBtn){
		CRect rect1;
		CRect rect2;
		HWND ph=this->GetSafeHwnd();
		::GetClientRect(ph,&rect1);
		CWnd * wnd=GetDlgItem(IDC_BUTTONGROUP);
		wnd->GetClientRect(&rect2);
		
		m_hTriggerBtn= CreateWindowEx(0L,
						"BUTTON",
						(LPSTR)NULL,
						WS_CHILD,
						rect1.left+10,
						rect1.bottom - (rect2.bottom+25)/2,
						5*GetSystemMetrics(SM_CYVSCROLL),
						25,
						ph,
						(HMENU)NULL,
						0,
						(LPVOID)NULL
						);
		SetWindowLong(m_hTriggerBtn, GWL_ID,IDC_USER_TEST_1);
		::SetWindowText(m_hTriggerBtn,_T("UserTest1"));
		submenu->CheckMenuItem(ID_TEST_ITEM_1,MF_CHECKED);
	}
	else{
		submenu->CheckMenuItem(ID_TEST_ITEM_1,MF_UNCHECKED);
	}
	::ShowWindow(m_hTriggerBtn,m_bTriggerBtn);
}

int CDhTestDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	


	return 0;
}


void CDhTestDlg::xd_InitToolbarCtrl()
{
}


void CDhTestDlg::xdToolbarCtrlBtn(UINT nID)
{
	switch(nID)
	{
	case ID_BUTTON_1:
		if(dlg.DoModal()==IDOK)
		{
			m_nSleepTime = dlg.nSleepTime;
			m_nAoiTestMode = dlg.m_nAoiTestMode;
		}
		break;
	case ID_BUTTON_2:
		if(!m_bAoiTest)
		{	
			v_Start_AOI_Test();				
			m_bAoiTest=TRUE;
			ShowWindow(SW_HIDE);
		}	
		else
		{
			m_bAoiTest=FALSE; 
			ShowWindow(SW_SHOW);
		}
		break;
	case ID_BUTTON_3:		
		if(!m_bStartStopTest)
		{	
			AfxBeginThread((AFX_THREADPROC)Xd_StarStop_Test_Thread,this);				
			m_bStartStopTest=TRUE;
			ShowWindow(SW_HIDE);
		}	
		else
		{
			m_bStartStopTest=FALSE; 
			ShowWindow(SW_SHOW);
		}
		break;
	case ID_BUTTON_4:
		if(!m_bTriggerTest){
			AfxBeginThread((AFX_THREADPROC)Xd_SoftTrigger_Test_Thread,this);
			m_bTriggerTest = TRUE;
			ShowWindow(SW_HIDE);
		}
		else
		{
			m_bTriggerTest = FALSE; 
			ShowWindow(SW_SHOW);
		}
		break;		
}
}

void CDhTestDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default

	UnregisterHotKey(m_hWnd,ID_BUTTON_2);
	CDialog::OnClose();
}

void CDhTestDlg::v_AOI_Test_Thread1(LPVOID lpConText)
{
	CDhTestDlg *pMe=(CDhTestDlg*)lpConText;

	BOOL &b=pMe->m_bAoiTest;
	int	 &nTime = pMe->m_nSleepTime;
	 
	CString str;
	
	HWND hWnd =NULL;
	HWND hWndView=NULL;
	hWnd = ::FindWindow("Daheng Imavision HV Performance",NULL);
	if(hWnd)
	{
		::SetForegroundWindow(hWnd);
		hWndView =::FindWindowEx(hWnd,NULL,"MDIClient",NULL);//hWnd的子窗口
		::PostMessage(hWndView,WM_SETFOCUS,0,0);		
	}
	
	CPoint pt1(160,160),pt2(320,320);
	int i = 1;
	while(b)
	{ 
		pt1.x += 16 * i;
		pt1.y += 16 * i;
		pt2.x += 16 * 2 * i;
		pt2.y += 16 * 2 * i;
		
		i++;	
		if(i==6)
		{
			i=1;
			pt1 = CPoint (160,160);
			pt2 = CPoint (320,320);
		}
		str.Format("(%3d,%3d-%3d,%3d) - (%3d,%3d)",
			pt1.x,pt1.y,pt2.x,pt2.y,
			pt2.x-pt1.x,pt2.y-pt1.y);
		TRACE(str);

		HDC hdc = ::GetDC(hWndView);		
		::Rectangle(hdc,pt1.x,pt1.y,pt2.x,pt2.y);	
		::ReleaseDC(hWndView,hdc);

		//把程序的坐标映射成屏幕坐标
		::ClientToScreen(hWndView,&pt1);
		::ClientToScreen(hWndView,&pt2);			

		SetCursorPos(pt1.x, pt1.y);		
		mouse_event(MOUSEEVENTF_LEFTDOWN,0, 0,0,0);		
		Sleep(1); 

		SetCursorPos(pt2.x, pt2.y);
		Sleep(1);
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);

		Sleep(nTime);
	}
}
void CDhTestDlg::v_AOI_Test_Thread0(LPVOID lpConText)
{
	CDhTestDlg *pMe=(CDhTestDlg*)lpConText;

	BOOL &b=pMe->m_bAoiTest;
	int	 &nTime = pMe->m_nSleepTime;
	CPoint pt1,pt2;
	double x1, y1, x2, y2;
	srand( (unsigned)time( NULL ) );
	CString str;
	HWND hWnd =NULL;
	HWND hWndView=NULL;
	hWnd = ::FindWindow("Daheng Imavision HV Performance",NULL);
	if(hWnd)
	{
		::SetForegroundWindow(hWnd);
		hWndView =::FindWindowEx(hWnd,NULL,"MDIClient",NULL);//hWnd的子窗口
		::PostMessage(hWndView,WM_SETFOCUS,0,0);
		if (pMe->dlg.m_nAoiLangeControl) {
			CRect rect;
			::GetClientRect(hWndView,&rect);
			x1=rect.left;
			y1=rect.top;
			x2=rect.right-16;
			y2=rect.bottom-16;			
		}else{
			sscanf(pMe->dlg.m_StartCoordinate, "%[^,]", str);
			x1 = atoi(str);
			sscanf(pMe->dlg.m_StartCoordinate, "%*[^,],%s", str);
			y1 = atoi(str);
			sscanf(pMe->dlg.m_EndCoordinate, "%[^,]", str);
			x2 = atoi(str)-16;
			sscanf(pMe->dlg.m_EndCoordinate, "%*[^,],%s", str);
			y2 = atoi(str)-16;
		}
	}
	while(b)
	{ 
/*
		pt1.x=int(x1+(double)rand()/(RAND_MAX+1)*(x2-x1));
		pt1.y=int(y1+(double)rand()/(RAND_MAX+1)*(y2-y1));
		pt2.x=int(64+pt1.x+(double)rand()/(RAND_MAX+1)*(x2-pt1.x));
		pt2.y=int(64+pt1.y+(double)rand()/(RAND_MAX+1)*(y2-pt1.y));	
*/

		int dx1 = pMe->AOIFreq((int)(x2-x1),pMe->dlg.m_AOI_Precision);
		int dy1 = pMe->AOIFreq((int)(y2-y1),pMe->dlg.m_AOI_Precision);
		pt1.x=int(x1)+dx1*(pMe->dlg.m_AOI_Precision);
		pt1.y=int(y1)+dy1*(pMe->dlg.m_AOI_Precision);	
		
		int dx2 = pMe->AOIFreq((int)(x2-pt1.x),pMe->dlg.m_AOI_Precision);
		int dy2 = pMe->AOIFreq((int)(y2-pt1.y),pMe->dlg.m_AOI_Precision);		
		pt2.x=16+pt1.x+dx2*(pMe->dlg.m_AOI_Precision);
		pt2.y=16+pt1.y+dy2*(pMe->dlg.m_AOI_Precision);

		str.Format("(%3d,%3d-%3d,%3d) - (%3d,%3d)",
			pt1.x,pt1.y,pt2.x,pt2.y,
			pt2.x-pt1.x,pt2.y-pt1.y);
		TRACE(str);

		HDC hdc = ::GetDC(hWndView);		
		::Rectangle(hdc,pt1.x,pt1.y,pt2.x,pt2.y);	
		::ReleaseDC(hWndView,hdc);

		//把程序的坐标映射成屏幕坐标
		::ClientToScreen(hWndView,&pt1);
		::ClientToScreen(hWndView,&pt2);			

		SetCursorPos(pt1.x, pt1.y);		
		mouse_event(MOUSEEVENTF_LEFTDOWN,0, 0,0,0);		
		Sleep(1); 

		SetCursorPos(pt2.x, pt2.y);
		Sleep(1);
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);

		Sleep(nTime);
	}
}

void CDhTestDlg::Xd_StarStop_Test_Thread(LPVOID lpConText)
{
	CDhTestDlg *pMe=(CDhTestDlg*)lpConText;	
	BOOL &b=pMe->m_bStartStopTest;
	int	 &nTime = pMe->m_nSleepTime;
	CPoint pt1,pt2;
	
	HWND hWnd = NULL;
	HWND hWndToolbar = NULL;
	hWnd = ::FindWindow("Daheng Imavision HV Performance",NULL);
	if(hWnd){
		::SetForegroundWindow(hWnd);
		HWND hWnd1=::FindWindowEx(hWnd,NULL,"ReBarWindow32",NULL);
		hWndToolbar=::FindWindowEx(hWnd1,NULL,"ToolbarWindow32",NULL);
		CRect rect;
		::GetClientRect(hWndToolbar,&rect);		
		pt1.x=rect.left+int(7*447/17);
		pt1.y=rect.top+(rect.bottom-rect.top)/2;
		pt2.x=pt1.x+int(1.5*447/17);
		pt2.y=rect.top+(rect.bottom-rect.top)/2;
		::ClientToScreen(hWndToolbar,&pt1);
		::ClientToScreen(hWndToolbar,&pt2);
	}
	while(b)
	{
		SetCursorPos(pt1.x, pt1.y);
		mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0); 
		Sleep(1000);
		SetCursorPos(pt2.x, pt2.y);
		mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0); 
		Sleep(nTime);
	}	
}
void CDhTestDlg::Xd_SoftTrigger_Test_Thread(LPVOID lpConText)
{
	CDhTestDlg *pMe=(CDhTestDlg*)lpConText;	
	BOOL &b=pMe->m_bTriggerTest;
	int	 &nTime = pMe->m_nSleepTime;
	CPoint pt1;

	HWND hWnd = NULL;
	HWND hWndToolbar = NULL;
	hWnd = ::FindWindow("Daheng Imavision HV Performance",NULL);
	if(hWnd){
		::SetForegroundWindow(hWnd);
		HWND hWnd1=::FindWindowEx(hWnd,NULL,"ReBarWindow32",NULL);
		hWndToolbar=::FindWindowEx(hWnd1,NULL,"ToolbarWindow32",NULL);
		CRect rect;
		::GetClientRect(hWndToolbar,&rect);		
		pt1.x=rect.left+305;
		pt1.y=rect.top+(rect.bottom-rect.top)/2;
		::ClientToScreen(hWndToolbar,&pt1);
	}
	while(b)
	{		
		SetCursorPos(pt1.x, pt1.y);
		mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
		Sleep(nTime);
	}
	
}

void CDhTestDlg::OnHotKey(LPARAM lParam,WPARAM wParam)
{		
	switch(lParam){
	case ID_BUTTON_1:// [Ctrl + P]
		TRACE("ppp...");
		xdToolbarCtrlBtn(ID_BUTTON_1);
		break;
	case ID_BUTTON_2: // [Ctrl + A]
		TRACE("aaa...");
		xdToolbarCtrlBtn(ID_BUTTON_2);
		break;
	case ID_BUTTON_3: // [Ctrl + S]
		TRACE("sss...");
		xdToolbarCtrlBtn(ID_BUTTON_3);
		break;
	case ID_BUTTON_4: //[Ctrl + T]
		TRACE("ttt...");
		xdToolbarCtrlBtn(ID_BUTTON_4);
	}
}


BOOL CDhTestDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message== WM_LBUTTONDOWN ||		
		 pMsg->message== WM_LBUTTONUP ||		
		 pMsg->message== WM_MOUSEMOVE)
		
		m_pToolTipCtrl.RelayEvent(pMsg);
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CDhTestDlg::On_UserTest1()
{
	//待添加...
	MessageBox("等待添加新功能!");
}

void CDhTestDlg::v_Start_AOI_Test()
{
	switch(m_nAoiTestMode) {
	case 0:
		AfxBeginThread((AFX_THREADPROC)v_AOI_Test_Thread0,this);
		break;
	case 1:
		AfxBeginThread((AFX_THREADPROC)v_AOI_Test_Thread1,this);
		break;
	default:
		break;
	}

}

int CDhTestDlg::AOIFreq(int Range, int Precision)
{
	int d=rand();
	if (d>(Range/Precision)) 
	{			
		d=d%(Range/Precision);			
	}
	return d;
}
