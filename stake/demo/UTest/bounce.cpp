// bounce.cpp : Defines the class behaviors for the Bounce child window.


#include "stdafx.h"
#include "mdi.h"
#include "bounce.h"

#include "mainfrm.h"
#include "WBDlg.h"

#define ABS(x) ((x) < 0? -(x) : (x) > 0? (x) : 0)


/////////////////////////////////////////////////////////////////////////////
// CBounceWnd

BEGIN_MESSAGE_MAP(CBounceWnd, CMDIChildWnd)
	//{{AFX_MSG_MAP(CBounceWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_COMMAND(IDM_CUSTOM, OnOpenDlgPrp)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_CLOSE()
	ON_COMMAND(ID_OUTPUTWIN_SCALE50, OnOutputwinScale50)
	ON_UPDATE_COMMAND_UI(ID_OUTPUTWIN_SCALE50, OnUpdateOutputwinScale50)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_COMMAND(ID_AOI_MACHING_BEST, OnAoiMachingBest)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_OUTPUTWIN_SCALE100, OnOutputwinScale100)
	ON_UPDATE_COMMAND_UI(ID_OUTPUTWIN_SCALE100, OnUpdateOutputwinScale100)
	ON_COMMAND(ID_OUTPUTWIN_SCALE150, OnOutputwinScale150)
	ON_UPDATE_COMMAND_UI(ID_OUTPUTWIN_SCALE150, OnUpdateOutputwinScale150)
	ON_COMMAND(ID_OUTPUTWIN_SCALE200, OnOutputwinScale200)
	ON_UPDATE_COMMAND_UI(ID_OUTPUTWIN_SCALE200, OnUpdateOutputwinScale200)
	ON_WM_SETFOCUS()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_GRAB_CONTINUOUS, OnGrabContinuous)
	ON_COMMAND(ID_GRAB_CANCEL, OnGrabCancel)
	ON_UPDATE_COMMAND_UI(ID_GRAB_CONTINUOUS, OnUpdateGrabContinuous)
	ON_UPDATE_COMMAND_UI(ID_GRAB_CANCEL, OnUpdateGrabCancel)
	ON_COMMAND(ID_GRAB_SINGLE, OnGrabSingle)
	ON_UPDATE_COMMAND_UI(ID_GRAB_SINGLE, OnUpdateGrabSingle)
	ON_COMMAND(ID_IMAGEMODE, OnImagemode)
	ON_UPDATE_COMMAND_UI(ID_IMAGEMODE, OnUpdateImagemode)
	ON_COMMAND(ID_WHITE_BALANCE, OnWhiteBalance)
	ON_UPDATE_COMMAND_UI(ID_WHITE_BALANCE, OnUpdateWhiteBalance) 
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_GET_RATIO, OnGetRatioMessage)
END_MESSAGE_MAP()

CMenu NEAR CBounceWnd::menu;        // menu for all BOUNCE windows

/////////////////////////////////////////////////////////////////////////////
// CBounceWnd creation

BOOL CBounceWnd::Create(LPCTSTR szTitle, LONG style /* = 0 */,
	const RECT& rect /* = rectDefault */,
	CMDIFrameWnd* parent /* = NULL */)
{
	// Setup the shared menu
	if (menu.m_hMenu == NULL)
		menu.LoadMenu(IDR_BOUNCE);
	m_hMenuShared = menu.m_hMenu;

	// Register a custom WndClass and create a window.
	// This must be done because CBounceWnd has a custom cursor, and
	//  no icon.
	LPCTSTR lpszBounceClass =
		AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,
			LoadCursor(NULL, IDC_SIZE/*IDC_UPARROW*/),
			(HBRUSH)(COLOR_WINDOW+1),
			NULL);

	return CMDIChildWnd::Create(lpszBounceClass, szTitle, style, rect, parent);
}

CBounceWnd::CBounceWnd()
{
	
	m_hhv=NULL;
	m_pDlgPrp=NULL;
	m_pRawBuffer=NULL;
	m_pRGBBuffer=NULL;
	m_nCamerNumber=0;
	m_nListNum=0;
	m_strCamerName ="HVDevice";
	
	m_bBegin	 = FALSE;
	m_bOpen		 = FALSE;
	m_bStart     = FALSE;
	m_Layout	 = BAYER_GR;
    m_Resolution = RES_MODE0;
	m_ImageMode	 = HV_COLOR;
	m_bTrigger   =FALSE;
	
	m_dRatioR		= 1.0;
	m_dRatioG		= 1.0;
	m_dRatioB		= 1.0;
	
	m_T1=m_T2=0;
	m_strFPS=" ";
	m_C=0;
	
	m_nXStart=0;
	m_nYStart=0;
	m_nWidth=1280;
	m_nHeight=1024;
	m_nMaxWidth=1280;
	m_nMaxHeight=1024;
	m_rcOutputWindows.SetRect(4,4,m_nMaxWidth*m_fOptwScale+4,m_nMaxHeight*m_fOptwScale+4);
	
	m_rcAOI.SetRect(0,0,0,0);
	m_rcShowView.SetRect(0,0,0,0);
	m_rcTestOutput.SetRect(0,0,0,0);
	
	m_bAllowInvert = FALSE;	
    m_fOptwScale=1.00000;
    m_tracker.m_nHandleSize=8;
	m_bShowAOI=TRUE;
	m_IsLButtonDown=FALSE;
	ZeroMemory(m_szDevicePath,sizeof(m_szDevicePath));
	
}
CBounceWnd::~CBounceWnd()
{
	if (m_pDlgPrp)
	{
		delete[]m_pDlgPrp;
		m_pDlgPrp=NULL;
	}
	if(m_hhv)
	{
		HVSTATUS status = EndHVDevice(m_hhv);
		HV_VERIFY(status);
		m_hhv=NULL;
	}
	
	RELEASEBUFFER(m_pRawBuffer);
	RELEASEBUFFER(m_pRGBBuffer);
	
	TRACE("===============CBounceWnd::~CBounceWnd=================");

	
}

int CBounceWnd::OnCreate(LPCREATESTRUCT)
{
	if (!SetTimer(1, 100 , NULL))
	{
		MessageBox(_T("Not enough timers available for this window."),
				_T("MDI:Bounce"), MB_ICONEXCLAMATION | MB_OK);
		
		return -1;
	}

    Init(m_nCamerNumber); ///English:    ///Chinese: 初始化摄像机
	TestSetDefaultValue();///English:    ///Chinese: 向摄像机中写一遍默认值
    
	
	return 0;
}


void CBounceWnd::OnSize(UINT nType, int cx, int cy)
{
	SetScrollInfoX();

	CMDIChildWnd::OnSize(nType, cx, cy);
}

void CBounceWnd::OnOpenDlgPrp()
{  
    
    ///English:    ///Chinese:  根据摄像机的类型弹出对应的属性页参数调节对话框	
     
		 if (m_pDlgPrp==NULL)
        {
			 m_pDlgPrp=new CDlgPrp(m_hhv,this);
			 
			 m_pDlgPrp->Create(IDD_DLG_PRP,NULL);///English:    ///Chinese: 创建对话框
			 m_pDlgPrp->ShowWindow(SW_SHOW);    ///English:    ///Chinese: 显示对话框

        }
        else
		{
			m_pDlgPrp->ShowWindow(SW_SHOW);    ///English:    ///Chinese: 显示对话框
		}
	

}


void CBounceWnd::OnTimer(UINT /* wParam */)
{
	if (!m_bStart)
	{
		CString strfps="";
		CString strTitle="";
        strfps.Format(" %f(fps)  ",0.0);
//		strTitle.Format("%s%d",m_strCamerName,m_nListNum);
		strTitle.Format("%s",m_strCamerDispName);
		strTitle+=strfps;
		SetWindowText(strTitle);		
	}
	else
		Calculatefps();
}

void CBounceWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CRect rctem;
	rctem.left=m_rcOutputWindows.left-4;
	rctem.top =m_rcOutputWindows.top-4;
	rctem.right=m_rcOutputWindows.right+8;
	rctem.bottom=m_rcOutputWindows.bottom+8;
	if (!rctem.PtInRect(point))
		return;
	
	if (m_bShowAOI) {
		
		m_rcRemberTrackRect=m_tracker.m_rect;
		
		CRect rectSave;
		m_tracker.GetTrueRect(rectSave);
		
		if (m_tracker.HitTest(point) < 0||(m_tracker.m_rect.PtInRect(point)==TRUE))
		{
			m_IsLButtonDown=TRUE;
			CRectTracker tracker;
			if (tracker.TrackRubberBand(this, point, m_bAllowInvert))
			{
				MessageBeep(0); 
				m_tracker.m_rect=tracker.m_rect;
				///English:    ///Chinese: 不能超出输出窗口边框
				if (m_tracker.m_rect.left<m_rcOutputWindows.left) {
					m_tracker.m_rect.left=m_rcOutputWindows.left;
				}
				
				if (m_tracker.m_rect.top<m_rcOutputWindows.top) {
					m_tracker.m_rect.top=m_rcOutputWindows.top;
				}
				
				if (m_tracker.m_rect.right>m_rcOutputWindows.right) {
					m_tracker.m_rect.right=m_rcOutputWindows.right;
					
				}
				
				if (m_tracker.m_rect.bottom>m_rcOutputWindows.bottom) {
					m_tracker.m_rect.bottom=m_rcOutputWindows.bottom;
					
				}
				
				m_tracker.m_rect.NormalizeRect();
				
			}		
		}	
		else if (m_tracker.Track(this, point, m_bAllowInvert))
		{
			m_IsLButtonDown=TRUE;
			///English:    ///Chinese: AOI发生改变重新计算输出窗口
			int iWidth, iHeight;
			iWidth=m_tracker.m_rect.Width();
			iHeight=m_tracker.m_rect.Height();
			
			///English:    ///Chinese: 不能超出输出窗口边框
			if (m_tracker.m_rect.left<m_rcOutputWindows.left) {
				m_tracker.m_rect.left=m_rcOutputWindows.left;
				m_tracker.m_rect.right=m_tracker.m_rect.left+iWidth;
			}
			
			if (m_tracker.m_rect.top<m_rcOutputWindows.top) {
				m_tracker.m_rect.top=m_rcOutputWindows.top;
				m_tracker.m_rect.bottom=m_tracker.m_rect.top+iHeight;
			}
			
			if (m_tracker.m_rect.right>m_rcOutputWindows.right) {
				m_tracker.m_rect.right=m_rcOutputWindows.right;				
				m_tracker.m_rect.left=m_tracker.m_rect.right-iWidth;
				if (m_tracker.m_rect.left<m_rcOutputWindows.left) {
					m_tracker.m_rect.left=m_rcOutputWindows.left;
				}
				
			}
			
			if (m_tracker.m_rect.bottom>m_rcOutputWindows.bottom) {
				m_tracker.m_rect.bottom=m_rcOutputWindows.bottom;
				if (m_tracker.m_rect.top<m_rcOutputWindows.top) {
					m_tracker.m_rect.top=m_rcOutputWindows.top;
				}
				
			}
			
			m_tracker.m_rect.NormalizeRect();			
			
			
		}	
		SendMessage(WM_LBUTTONUP,0,0);
	}
	
	CMDIChildWnd::OnLButtonDown(nFlags, point);
}
static void SetNormalRect(CRect& rect, int left, int top, int width, int height)
{
	
	rect.left = left;
	rect.top = top;
	rect.right = left + width;
	rect.bottom = top + height;
	
	int nTemp;
	if (rect.left > rect.right)
	{
		nTemp = rect.left;
		rect.left = rect.right;
		rect.right = nTemp;
	}
	if (rect.top > rect.bottom)
	{
		nTemp = rect.top;
		rect.top = rect.bottom;
		rect.bottom = nTemp;
	}
}
void CBounceWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	

	// TODO: Add your message handler code here
	
	CBrush* pOldBrush = NULL;
	CBrush OptBrush;
	OptBrush.CreateSolidBrush(RGB(146, 152, 140));
	pOldBrush = dc.SelectObject(&OptBrush);
	
	///English:    ///Chinese: 绘制最大输出窗口矩形区域
	dc.Rectangle(m_rcOutputWindows);
    
	if (m_bShowAOI) {
		
		///English:    ///Chinese: 绘制AOI
		TRY
		{
			CBrush brush;
			CRect rect;			
			
			int nWidth = m_tracker.m_rect.Width();
			int nHeight = m_tracker.m_rect.Height();			
			
			///English:    ///Chinese: 计算图像显示参数
			int nAoix,nAoiy,nAoiwid,nAoihei;
			nAoix=(m_tracker.m_rect.left-m_rcOutputWindows.left)*(1.000000/m_fOptwScale);		
			nAoiy=(m_tracker.m_rect.top-m_rcOutputWindows.top)*(1.000000/m_fOptwScale);			
			nAoiwid=m_tracker.m_rect.Width()*(1.000000/m_fOptwScale);			
			nAoihei=m_tracker.m_rect.Height()*(1.000000/m_fOptwScale);			
			
			m_tracker.m_nStyle=CRectTracker::hatchedBorder|CRectTracker::resizeOutside;			
			brush.CreateSolidBrush(RGB(255, 255, 255));
			pOldBrush = dc.SelectObject(&brush);
			SetNormalRect(rect, m_tracker.m_rect.left,
				m_tracker.m_rect.top, nWidth, nHeight);
			dc.PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
			
			///English:    ///Chinese: 计算显示起点
			m_rcShowView.left=    m_tracker.m_rect.left;
			m_rcShowView.top=     m_tracker.m_rect.top;
			
			///English:    ///Chinese: 计算显示宽度
			m_rcShowView.right=m_tracker.m_rect.left+m_tracker.m_rect.Width();
			m_rcShowView.bottom= m_tracker.m_rect.top+m_tracker.m_rect.Height();
			
		    ///English:    ///Chinese: 计算图像显示参数
	
		    nAoix=(m_tracker.m_rect.left-m_rcOutputWindows.left)*(1.000000/m_fOptwScale);
			m_rcAOI.left=nAoix;
			
			nAoiy=(m_tracker.m_rect.top-m_rcOutputWindows.top)*(1.000000/m_fOptwScale);
			m_rcAOI.top=nAoiy;
			
			nAoiwid=m_tracker.m_rect.Width()*(1.000000/m_fOptwScale);
			m_rcAOI.right=m_rcAOI.left+nAoiwid;
			
			nAoihei=m_tracker.m_rect.Height()*(1.000000/m_fOptwScale);
			m_rcAOI.bottom=m_rcAOI.top+nAoihei;	
	        
            if (pOldBrush != NULL)
				dc.SelectObject(pOldBrush);
			brush.DeleteObject();
			
			m_tracker.Draw(&dc);
			
		}
		CATCH_ALL(e)
		{
			if (pOldBrush != NULL)
				dc.SelectObject(pOldBrush);
		}
		END_CATCH_ALL
	}


    if (!m_bStart)
    {
		ShowImage();
    }

	
	// Do not call CMDIChildWnd::OnPaint() for painting messages
}

void CBounceWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	HVSTATUS status=STATUS_OK;
	if (m_bShowAOI&&m_IsLButtonDown==TRUE) {
		
		m_IsLButtonDown=FALSE;
		CRect rectSave;
		m_tracker.GetTrueRect(rectSave);
		
		if (!(m_rcRemberTrackRect.left==m_tracker.m_rect.left) ||!(m_rcRemberTrackRect.top==m_tracker.m_rect.top)
			||!(m_rcRemberTrackRect.right==m_tracker.m_rect.right)||!(m_rcRemberTrackRect.bottom==m_tracker.m_rect.bottom))
		{
			
			///English:    ///Chinese: 计算输出窗口参数
			m_nXStart=(m_tracker.m_rect.left-m_rcOutputWindows.left)*(1.000000/m_fOptwScale);
			if (m_nXStart%4) {
				m_nXStart-=m_nXStart%4;
				if (m_nXStart<=0) {
					m_nXStart=0;
				}
				m_tracker.m_rect.left=m_rcOutputWindows.left+(1.000000*m_nXStart*m_fOptwScale);
			}
			m_nYStart=(m_tracker.m_rect.top-m_rcOutputWindows.top)*(1.000000/m_fOptwScale);
			if (m_nYStart%4/*2*/) {
				m_nYStart-=m_nYStart%4/*2*/;
				if (m_nYStart<=0) {
					m_nYStart=0;
				}
				m_tracker.m_rect.top=m_rcOutputWindows.top+(1.000000*m_nYStart*m_fOptwScale);
			}
			
			m_nWidth=(m_tracker.m_rect.Width())*(1.000000/m_fOptwScale);
			if (m_nWidth%4||m_nWidth<16) {
				m_nWidth-=m_nWidth%4;
				if (m_nWidth<16)
				{
					m_nWidth=16;
				}
				if (m_nWidth>=m_nMaxWidth-m_nXStart) {
					m_nWidth=m_nMaxWidth-m_nXStart;
				}				
				m_tracker.m_rect.right=m_tracker.m_rect.left+(1.000000*m_nWidth*m_fOptwScale);
				if (m_tracker.m_rect.right>=m_rcOutputWindows.right) {
					
					m_tracker.m_rect.right=m_rcOutputWindows.right;
					
					m_nWidth=m_tracker.m_rect.Width()*(1.000000/m_fOptwScale);
				}
				
			}
			
			m_nHeight=(m_tracker.m_rect.Height())*(1.000000/m_fOptwScale);
			if (m_nHeight%4||m_nHeight<16) {
				m_nHeight-=m_nHeight%4;
				if (m_nHeight>=m_nMaxHeight-m_nYStart) {
					m_nHeight=m_nMaxHeight-m_nYStart;				
				}
				if (m_nHeight<16)
				{
					m_nHeight=16;
				}
				m_tracker.m_rect.bottom=m_tracker.m_rect.top+(1.000000*m_nHeight*m_fOptwScale);
				if (m_tracker.m_rect.bottom>=m_rcOutputWindows.bottom) {
					
					m_tracker.m_rect.bottom=m_rcOutputWindows.bottom;
					
					m_nHeight=m_tracker.m_rect.Height()*(1.000000/m_fOptwScale);
				}
			}
			
			m_tracker.m_rect.NormalizeRect();
			
			
			
			if (m_bStart)           ///English:    ///Chinese: 在设置AOI前停止采集
			{
				HVStopSnap(m_hhv);
			}
			int l=m_nXStart;
			int t=m_nYStart;
			
			switch(m_Resolution) {     ///English:    ///Chinese: 修正CMOS摄像机小分辨率小窗口的Bug
			case RES_MODE1:
				l=l*2;
				t=t*2;
				break;
			case RES_MODE2:
				l=l*4;
				t=t*4;
				break;
				
			default:
				break;
			}
			
			
			status=HVSetOutputWindow(m_hhv, l,t, m_nWidth,m_nHeight);///English:    ///Chinese: 设置输出窗口
			HV_VERIFY(status);	
            AdjustExposureTime();    ///English:    ///Chinese: 调整CMOS摄像机的曝光时间
			
            if (m_bStart)            ///English:    ///Chinese: 在设置AOI后开始采集
			{
				BYTE * ppBuffer[1];
				ppBuffer[0] = m_pRawBuffer;
				HVStartSnap(m_hhv,ppBuffer,1);	
			}
			
			
		}
		
		Invalidate();
	}

	
	CMDIChildWnd::OnLButtonUp(nFlags, point);
}

HVSTATUS CBounceWnd::Open()
{
	HVSTATUS status = STATUS_OK;
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	ASSERT(pFrame);	
	/**************add by lm 081020******************/
	if (pFrame->m_nDevTotal==0) {
		MessageBox(TEXT("No Device Found!"), 
            TEXT("Error"), MB_OK|MB_ICONERROR);
		return status;
	}
	BOOL b_Flag=FALSE;
	for(int i=0;i<pFrame->m_nDevTotal;i++){
		CString m_strName = pFrame->m_wndTree.GetItemText(pFrame->m_MIDhti[i]);
		CString strName;
		int num;
		sscanf(m_strName.GetBuffer(m_strName.GetLength()),"%d %s",&num,strName);
		if (strcmp(strName,m_strCamerDispName)==0) {
			b_Flag=TRUE;
			DeviceNode *pCurrentNode = NULL;
			if(pCurrentNode = pFrame->f_GetDeviceNodeFromIndex(pFrame->phead,num))
			{
				if (pCurrentNode->pDeviceWnd == NULL)
				{
					pCurrentNode->pDeviceWnd = this;
					pCurrentNode->pDeviceWnd->m_nCamerNumber = pCurrentNode->nbegin;				
					pCurrentNode->pDeviceWnd->m_nListNum=num;//add 081013_lm
					sscanf(m_strName.GetBuffer(m_strName.GetLength()),"%d%[^ ]",&num,strName);
					pCurrentNode->pDeviceWnd->m_strCamerName.Format("%s",strName);
					sprintf(pCurrentNode->pDeviceWnd->m_szDevicePath,"%s",pCurrentNode->szDevicePath);
				}
			}
		}
	}
			
	if (b_Flag) {
		if (!m_bBegin) 
		{
			status = BeginHVDevice(m_nCamerNumber, &m_hhv);  ///English:    ///Chinese: 打开数字摄像机  
			HV_VERIFY(status);
			if (status == STATUS_OK) {
				m_bBegin=TRUE;
			}		
		}
	}else{
		MessageBox(TEXT("Device does not exist!"), 
            TEXT("Morning"), MB_OK|MB_ICONWARNING);

		return status;
	}

	/**********************End**************************/
	
	ASSERT(m_hhv);
	///English:    ///Chinese: 初始化摄像机采集图像到内存的控制,指定回调函数和用户定义的参数,分配资源
	status = HVOpenSnap(m_hhv, Snap_Callback, this);					
	HV_VERIFY(status);
	if (HV_SUCCESS(status)) {
		m_bOpen = TRUE;		///English:    ///Chinese: 标志已经打开采集环境
	}
	return status;

}

int CALLBACK CBounceWnd::Snap_Callback(HV_SNAP_INFO *pInfo)
{
    CBounceWnd*pB=(CBounceWnd*)(pInfo->pParam);
//	pB->Calculatefps();
	pB->ShowImage();
	pB->m_C++;	
	return 1;
}

void CBounceWnd::Init(int n)
{
	
    HVSTATUS status = STATUS_OK;
	status = BeginHVDevice(n, &m_hhv);  //打开数字摄像机  
	HV_VERIFY(status);
	if (status == STATUS_OK) {
		m_bBegin=TRUE;
	}

    ///English:    ///Chinese: m_pBmpInfo即指向m_chBmpBuf缓冲区，用户可以自己分配BTIMAPINFO缓冲区	
	m_pBmpInfo								= (BITMAPINFO *)m_chBmpBuf;
	///English:    ///Chinese: 初始化BITMAPINFO 结构，此结构在保存bmp文件、显示采集图像时使用
	m_pBmpInfo->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	///English:    ///Chinese: 图像宽度，一般为输出窗口宽度
	m_pBmpInfo->bmiHeader.biWidth			= m_nWidth;
	///English:    ///Chinese: 图像宽度，一般为输出窗口高度
	m_pBmpInfo->bmiHeader.biHeight			= m_nHeight;
	
	
	m_pBmpInfo->bmiHeader.biPlanes			= 1;
	m_pBmpInfo->bmiHeader.biBitCount		= 24;
	m_pBmpInfo->bmiHeader.biCompression		= BI_RGB;
	m_pBmpInfo->bmiHeader.biSizeImage		= 0;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter	= 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter	= 0;
	m_pBmpInfo->bmiHeader.biClrUsed			= 0;
	m_pBmpInfo->bmiHeader.biClrImportant	= 0;

    ///English:    ///Chinese: 设置位图调色板
	for (int i = 0; i < 256; i++){
		m_pBmpInfo->bmiColors[i].rgbBlue		= (BYTE)i;
		m_pBmpInfo->bmiColors[i].rgbGreen		= (BYTE)i;
		m_pBmpInfo->bmiColors[i].rgbRed			= (BYTE)i;
		m_pBmpInfo->bmiColors[i].rgbReserved	= 0;	
	}
	
	for(i=0;i<256;i++)
	{
		m_pLutR[i] = i;
		m_pLutG[i] = i;
		m_pLutB[i] = i;
	}

   
	int nBuffSize = 0;
    status = HVGetDeviceInfo(m_hhv, DESC_RESOLUTION, NULL,&nBuffSize);	
	if( !HV_SUCCESS(status))
		return;
	
	BYTE *pbContext = new BYTE[nBuffSize];
	DWORD *pdContext = (DWORD *)(pbContext);
	status = HVGetDeviceInfo(m_hhv, DESC_RESOLUTION, pdContext,&nBuffSize);	///English:    ///Chinese: 获取摄像机分辨率

    m_nMaxWidth  = *(pdContext + 2*RES_MODE0);
	m_nMaxHeight = *(pdContext + 2*RES_MODE0 +1);
	delete []pbContext;

    m_rcOutputWindows.SetRect(4,4,m_nMaxWidth*m_fOptwScale+4,m_nMaxHeight*m_fOptwScale+4);///English:    ///Chinese: 初始化最大输出窗口区域
    m_nWidth=m_nMaxWidth;
	m_nHeight=m_nMaxHeight;
	
	int nSize = sizeof(HV_BAYER_LAYOUT);          ///English:    ///Chinese: 获取Bayer数据类型
	status = HVGetDeviceInfo(m_hhv, DESC_DEVICE_BAYER_LAYOUT, &m_Layout,&nSize);	

    SetLutTable(m_dRatioR,m_dRatioG,m_dRatioB);   ///English:    ///Chinese: 初始化颜色查找表	
}

HVSTATUS CBounceWnd::Start()
{
	HVSTATUS status = STATUS_OK;
	ASSERT(m_hhv);
	if(m_pRawBuffer==NULL)                       ///English:    ///Chinese: 分配原始图像缓冲区，一般用来存储采集到的原始图像数据
	{
		m_pRawBuffer = new BYTE[m_nMaxWidth * m_nMaxHeight];
		ASSERT(m_pRawBuffer);
	}
	
	if(m_pRGBBuffer==NULL)                       ///English:    ///Chinese: 分配Bayer转换后图像数据缓冲
	{
		m_pRGBBuffer = new BYTE[m_nMaxWidth * m_nMaxHeight * 3];
		ASSERT(m_pRGBBuffer);
	}
	
                                                 ///English:    ///Chinese: 设置输出窗口
	status =HVSetOutputWindow(m_hhv, m_nXStart,m_nYStart, m_nWidth,m_nHeight);

	BYTE *ppBuf[1];
	ppBuf[0] = m_pRawBuffer;
	status = HVStartSnap(m_hhv, ppBuf,1);        ///English:    ///Chinese: 启动摄像机采集图像到内存
	HV_VERIFY(status);
	if (HV_SUCCESS(status)) {
		m_bStart = TRUE;
		m_T1=m_T2=GetTickCount();
		m_C=0;
	}
	return status;

}


HVSTATUS CBounceWnd::Stop()
{
	HVSTATUS status =STATUS_OK;
	ASSERT(m_hhv);
	status = HVStopSnap(m_hhv);  ///English:    ///Chinese: 停止摄像机采集图像到内存
	HV_VERIFY(status);
	if (HV_SUCCESS(status)) {
		m_bStart = FALSE;
	}
/*
	RELEASEBUFFER(m_pRawBuffer);
	RELEASEBUFFER(m_pRGBBuffer);
*/
	return status;

}

HVSTATUS CBounceWnd::Close()
{
	HVSTATUS status = STATUS_OK;
	if(m_bStart){
		Stop();
	}
	status = HVCloseSnap(m_hhv); ///English:    ///Chinese: 关闭采集图像到内存的控制，释放申请的资源
	HV_VERIFY(status);
	
	if (HV_SUCCESS(status)) {
		m_bOpen		= FALSE;
		m_bStart	= FALSE;
	}
	return status;

}

void CBounceWnd::ShowImage()
{
    
	CClientDC dc(this);

    int w=m_nWidth;
	int h=m_nHeight;

	m_pBmpInfo->bmiHeader.biWidth	= w;
	m_pBmpInfo->bmiHeader.biHeight	= h;

	if (m_ImageMode	== HV_COLOR) {
		m_pBmpInfo->bmiHeader.biBitCount = 24;
		///English:    ///Chinese: 将原始图像数据进行Bayer转换,转换后为24位,同时将原始数据进行上下翻转
        ConvertBayer2Rgb(m_pRGBBuffer,m_pRawBuffer,m_nWidth,m_nHeight,BAYER2RGB_NEIGHBOUR,m_pLutR,m_pLutG,m_pLutB,true,m_Layout);
	}
    else{
		m_pBmpInfo->bmiHeader.biBitCount = 8;
		BMPBufBottomToUp(m_pRawBuffer,m_pRGBBuffer,m_nWidth,m_nHeight);///English:    ///Chinese: 翻转原始图像数据
		
	}
		
	SetStretchBltMode(dc.GetSafeHdc(),COLORONCOLOR);
	
	
	m_rcTestOutput.left=m_rcOutputWindows.left+m_nXStart*m_fOptwScale;
	m_rcTestOutput.top=m_rcOutputWindows.top+m_nYStart*m_fOptwScale;
	m_rcTestOutput.right=m_rcTestOutput.left+m_nWidth*m_fOptwScale;
	m_rcTestOutput.bottom=m_rcTestOutput.top+m_nHeight*m_fOptwScale;
	
	m_pBmpInfo->bmiHeader.biWidth=m_nWidth;
	m_pBmpInfo->bmiHeader.biHeight=m_nHeight;

	
    
	StretchDIBits(dc.GetSafeHdc(),
		m_rcTestOutput.left,
		m_rcTestOutput.top,
		m_rcTestOutput.Width(),	  ///English:    ///Chinese: 显示窗口宽度
		m_rcTestOutput.Height(),  ///English:    ///Chinese: 显示窗口高度
		0,
		0,
		m_nWidth,				  ///English:    ///Chinese: 图像宽度
		m_nHeight,				  ///English:    ///Chinese: 图像高度
		m_pRGBBuffer,			  ///English:    ///Chinese: 图像缓冲区
		m_pBmpInfo,				  ///English:    ///Chinese: BMP图像描述信息
		DIB_RGB_COLORS,
		SRCCOPY
		);
	/*
	TRACE("m_rcTestOutput.left=%d ",m_rcTestOutput.left);
	
	TRACE("m_rcTestOutput.top=%d \n",m_rcTestOutput.top);
				TRACE("m_rcTestOutput.Width()=%d ",m_rcTestOutput.Width());
				TRACE("m_rcTestOutput.Height()=%d ",m_rcTestOutput.Height());*/

		
	
	if (m_hOldPal && (m_pBmpInfo->bmiHeader.biBitCount == 8)) {
		::SelectPalette(dc.GetSafeHdc(), m_hOldPal, FALSE);
		::RealizePalette(dc.GetSafeHdc());
	}

}

void CBounceWnd::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	ASSERT(pFrame);
    
    Close();   
	
	DeviceNode *pNode = NULL;
	if (pNode = pFrame->f_GetDeviceNodeFromPath(pFrame->phead,m_szDevicePath))
	{
		pNode->pDeviceWnd = NULL;
		TRACE("CBounceWnd::OnClose pNode = 0x%x",pNode);
	}	
	CMDIChildWnd::OnClose();
}

void CBounceWnd::OnOutputwinScale50() 
{
	// TODO: Add your command handler code here
	
	int ntrakleft,ntraktop, ntrakwidth,ntrakheight, ndiswidth,ndisheight;
	ntrakleft=m_tracker.m_rect.left;
	ntraktop=m_tracker.m_rect.top;
	ntrakwidth=m_tracker.m_rect.Width();
	ntrakheight=m_tracker.m_rect.Height();
	
	ndiswidth=ntrakleft-m_rcOutputWindows.left;///English:    ///Chinese: AOI与输出窗口边界宽
	ndisheight=ntraktop-m_rcOutputWindows.top;///English:    ///Chinese: AOI与输出窗口边界高
	
	float nbfscale=m_fOptwScale;///English:    ///Chinese: 保存原来缩放倍数
	m_fOptwScale=0.5000000;
	
	m_rcOutputWindows.SetRect(4,4,m_nMaxWidth*m_fOptwScale+4,m_nMaxHeight*m_fOptwScale+4);///English:    ///Chinese: 最大输出窗口区域
	
	SetScrollInfoX();
	
    m_tracker.m_rect.left=m_rcOutputWindows.left+(ndiswidth)*(m_fOptwScale/nbfscale);
	m_tracker.m_rect.top=m_rcOutputWindows.top+(ndisheight)*(m_fOptwScale/nbfscale);
	
	m_tracker.m_rect.right=m_tracker.m_rect.left+ntrakwidth*(m_fOptwScale/nbfscale);
	m_tracker.m_rect.bottom=m_tracker.m_rect.top+ntrakheight*(m_fOptwScale/nbfscale);
	
	this->Invalidate(TRUE);
	
}

void CBounceWnd::OnUpdateOutputwinScale50(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (m_fOptwScale==0.500000) {
		pCmdUI->SetCheck(TRUE);
	}else
		pCmdUI->SetCheck(FALSE);
}

void CBounceWnd::SetScrollInfoX()
{
	
	CRect rcViewClient;	
	SCROLLINFO HSI, VSI;
	
	GetClientRect(rcViewClient);
	ClientToScreen(rcViewClient);
	
	HSI.nMax	= m_nMaxWidth*m_fOptwScale-rcViewClient.Width()+8;
	HSI.cbSize	= sizeof(SCROLLINFO);
	HSI.fMask	= SIF_RANGE | SIF_PAGE;
	HSI.nMin    = 0;	
	HSI.nPage	= 4;
	
	if (HSI.nMax <= 0) {
		::EnableScrollBar(GetSafeHwnd(), SB_HORZ, ESB_DISABLE_BOTH);
	}
	else{
		::EnableScrollBar(GetSafeHwnd(), SB_HORZ, ESB_ENABLE_BOTH);
		::SetScrollInfo(GetSafeHwnd(), SB_HORZ, &HSI, TRUE );
	}
	
	VSI.nMax	=  m_nMaxHeight*m_fOptwScale-rcViewClient.Height()+8;
	VSI.cbSize	= sizeof(SCROLLINFO);
	VSI.fMask	= SIF_RANGE | SIF_PAGE;
	VSI.nMin    = 0;	
	VSI.nPage	= 4;
	
	if (VSI.nMax <= 0) {
		::EnableScrollBar(GetSafeHwnd(), SB_VERT, ESB_DISABLE_BOTH);
	}
	else{
		::EnableScrollBar(GetSafeHwnd(), SB_VERT, ESB_ENABLE_BOTH);
		::SetScrollInfo(GetSafeHwnd(), SB_VERT, &VSI, TRUE );
	}

}

void CBounceWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
    SCROLLINFO si;
	int nWidth;
	int iHoriPos;///English:    ///Chinese: 保存滚动条原始位置
	
    si.cbSize	= sizeof(SCROLLINFO);
	si.fMask	= SIF_ALL;
	GetScrollInfo(SB_HORZ, &si, SIF_ALL);

	iHoriPos=si.nPos;///English:    ///Chinese: 记录滚动条原位置	
	this->GetClientRect(m_rcClientRect);
	nWidth=m_rcOutputWindows.Width();///English:    ///Chinese: 记录原窗口宽度

	int nDiswid=m_tracker.m_rect.left-m_rcOutputWindows.left;///English:    ///Chinese: 记录TRACKER矩形上边界与输出窗口矩形上边界宽
	int nTrawidth=m_tracker.m_rect.Width();///English:    ///Chinese: 记录TRACKER矩形宽
	
	switch(nSBCode) {
	case SB_LINELEFT:
		///English:    ///Chinese: 主窗口移动
		m_rcOutputWindows.left +=1;
		m_rcOutputWindows.right=m_rcOutputWindows.left+nWidth;
		if (m_rcOutputWindows.left>=4) {
			m_rcOutputWindows.left=4;
			m_rcOutputWindows.right=m_rcOutputWindows.left+nWidth;
		}
		///English:    ///Chinese: 滚动条移动
		si.nPos-=1;
		if (si.nPos<si.nMin) {
			si.nPos=si.nMin;
		}		
		break;
	case SB_LINERIGHT:		
		m_rcOutputWindows.left-=1;		
		m_rcOutputWindows.right=m_rcOutputWindows.left+nWidth;		
		if (m_rcOutputWindows.right<=m_rcClientRect.right-4) {
			m_rcOutputWindows.right=m_rcClientRect.right-4;
			m_rcOutputWindows.left=m_rcOutputWindows.right-nWidth;
		}
		si.nPos+=1;
		if (si.nPos>si.nMax) {
			si.nPos=si.nMax;
		}		
		break;
	case SB_PAGELEFT:		
		m_rcOutputWindows.left += 4;
		m_rcOutputWindows.right=m_rcOutputWindows.left+nWidth;
		if (m_rcOutputWindows.left>=4) {
			m_rcOutputWindows.left=4;
			m_rcOutputWindows.right=m_rcOutputWindows.left+nWidth;       
		}
		si.nPos-=si.nPage;
		if (si.nPos<si.nMin) {
			si.nPos=si.nMin;
		}		
		break;
	case SB_PAGERIGHT:
	    m_rcOutputWindows.left-=4;
		m_rcOutputWindows.right=m_rcOutputWindows.left+nWidth;
		if (m_rcOutputWindows.right<=m_rcClientRect.right-4) {
			m_rcOutputWindows.right=m_rcClientRect.right-4;
			m_rcOutputWindows.left=m_rcOutputWindows.right-nWidth;
		}		
		si.nPos+=si.nPage;
		if (si.nPos>si.nMax) {
			si.nPos=si.nMax;
		}
		break;
	case SB_LEFT:
		m_rcOutputWindows.left=4;
		m_rcOutputWindows.right=m_rcOutputWindows.left+nWidth;	
		si.nPos = si.nMin;
		break;
	case SB_RIGHT:		
		m_rcOutputWindows.right=m_rcClientRect.right-4;
		m_rcOutputWindows.left=m_rcOutputWindows.right-nWidth;
		si.nPos=si.nMax;
		break;
	case SB_THUMBPOSITION:		
	case SB_THUMBTRACK:		
		m_rcOutputWindows.left = 4-si.nTrackPos;
		m_rcOutputWindows.right=m_rcOutputWindows.left+nWidth;
		si.nPos=si.nTrackPos;
		break;
	default:
		break;
	}

	 m_tracker.m_rect.left=m_rcOutputWindows.left+nDiswid;
	 m_tracker.m_rect.right=m_tracker.m_rect.left+nTrawidth;	

	SetScrollInfo(SB_HORZ, &si, TRUE);
	GetScrollInfo(SB_HORZ,&si,SIF_ALL);

	if (si.nPos != iHoriPos)
	{                    
		ScrollWindow(iHoriPos - si.nPos, 0, NULL,NULL);
	
	}	
	

	CMDIChildWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CBounceWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	SCROLLINFO si;
	int nHeight = 0;
	int iVertPos;
	
	
	si.cbSize	= sizeof(SCROLLINFO);
	si.fMask	= SIF_ALL;
	GetScrollInfo(SB_VERT, &si, SIF_ALL);
	iVertPos=si.nPos;///English:    ///Chinese: 保存滚动条原始位置
	
	this->GetClientRect(m_rcClientRect);

	nHeight=m_rcOutputWindows.Height();

	int nDishei=m_tracker.m_rect.top-m_rcOutputWindows.top;
	int nTraheight=m_tracker.m_rect.Height();
	
	switch(nSBCode) {
	case SB_LINEUP:		
		m_rcOutputWindows.top +=1;
		m_rcOutputWindows.bottom=m_rcOutputWindows.top+nHeight;
		if (m_rcOutputWindows.top>=4) {
			m_rcOutputWindows.top=4;
			m_rcOutputWindows.bottom=m_rcOutputWindows.top+nHeight;
		}
		si.nPos-=1;
		if (si.nPos<si.nMin) {
			si.nPos=si.nMin;
		}		
		break;
	case SB_LINEDOWN:		
		m_rcOutputWindows.top-=1;
		m_rcOutputWindows.bottom=m_rcOutputWindows.top+nHeight;
		if (m_rcOutputWindows.bottom<=m_rcClientRect.bottom-4) {
			m_rcOutputWindows.bottom=m_rcClientRect.bottom-4;
			m_rcOutputWindows.top=m_rcOutputWindows.bottom-nHeight;
		}		
		si.nPos+=1;
		if (si.nPos>si.nMax) {
			si.nPos=si.nMax;
		}		
		break;
	case SB_PAGEUP:		
		m_rcOutputWindows.top += 4;
		m_rcOutputWindows.bottom=m_rcOutputWindows.top+nHeight;
		if (m_rcOutputWindows.top>=4) {
			m_rcOutputWindows.top=4;
			m_rcOutputWindows.bottom=m_rcOutputWindows.top+nHeight;
		}
		si.nPos-=si.nPage;
		if (si.nPos<si.nMin) {
			si.nPos=si.nMin;
		}		
		break;
	case SB_PAGEDOWN:
		m_rcOutputWindows.top-=4;
		m_rcOutputWindows.bottom=m_rcOutputWindows.top+nHeight;
		if (m_rcOutputWindows.bottom<=m_rcClientRect.bottom-4) {
			m_rcOutputWindows.bottom=m_rcClientRect.bottom-4;
			m_rcOutputWindows.top=m_rcOutputWindows.bottom-nHeight;
		}		
		si.nPos+=si.nPage;
		if (si.nPos>si.nMax) {
			si.nPos=si.nMax;
		}
		break;
	case SB_TOP:		
		m_rcOutputWindows.top=4;
		m_rcOutputWindows.bottom=m_rcOutputWindows.top+nHeight;		
		si.nPos = si.nMin;		
		break;
	case SB_BOTTOM:		
		m_rcOutputWindows.bottom=m_rcClientRect.bottom-4;
		m_rcOutputWindows.top=m_rcOutputWindows.bottom-nHeight;		
		si.nPos=si.nMax;		
		break;
	case SB_THUMBPOSITION:		
	case SB_THUMBTRACK:		
		m_rcOutputWindows.top = 4-si.nTrackPos;
		m_rcOutputWindows.bottom=m_rcOutputWindows.top+nHeight;
		si.nPos=si.nTrackPos;		
		break;
	default:
		break;
	}
	
	m_tracker.m_rect.top=m_rcOutputWindows.top+nDishei;
	m_tracker.m_rect.bottom=m_tracker.m_rect.top+nTraheight;	

	SetScrollInfo(SB_VERT, &si, TRUE);
	GetScrollInfo(SB_VERT,&si,SIF_ALL);

	if (si.nPos!=iVertPos) {		
		ScrollWindow( 0,iVertPos - si.nPos ,NULL,NULL);
	}

	CMDIChildWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CBounceWnd::OnAoiMachingBest() 
{
	// TODO: Add your command handler code here

   HVSTATUS status=STATUS_OK;

	m_tracker.m_rect.left=m_rcOutputWindows.left;
	m_tracker.m_rect.top=m_rcOutputWindows.top;
	
	m_tracker.m_rect.right=m_tracker.m_rect.left+m_rcOutputWindows.Width();
	m_tracker.m_rect.bottom=m_tracker.m_rect.top+m_rcOutputWindows.Height();

	m_nXStart=0;
	m_nYStart=0;
	m_nWidth=m_nMaxWidth;
	m_nHeight=m_nMaxHeight;


	if (m_bStart)            ///English:    ///Chinese: 在设置AOI前停止采集
	{
		HVStopSnap(m_hhv);
	}

	status=HVSetOutputWindow(m_hhv, m_nXStart,m_nYStart, m_nWidth,m_nHeight);
		HV_VERIFY(status);	
	AdjustExposureTime();   ///English:    ///Chinese: 调整CMOS摄像机的曝光时间

	if (m_bStart)           ///English:    ///Chinese: 在设置AOI后开始采集
	{
	    BYTE * ppBuffer[1];
		ppBuffer[0] = m_pRawBuffer;
		HVStartSnap(m_hhv,ppBuffer,1);	
	}


	Invalidate();
	
	
}

void CBounceWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	if (m_tracker.HitTest(point)<0)
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
	
	}
	else
	{
		if ((m_tracker.m_rect.PtInRect(point)==TRUE))
		{
			
			SetCursor(LoadCursor(NULL, IDC_CROSS));
		}
		else{
			
			SetCursor(LoadCursor(NULL, IDC_SIZEALL));
		}
		
	}

	CMainFrame* pFrame;
	pFrame=(CMainFrame*)AfxGetMainWnd();
	
	CString strOptwParam,strAOI,strMousepos;
	
	int nXStart,nYStart,nWidth,nHeight;
	nXStart=m_nXStart;
	nYStart=m_nYStart;
	nWidth=m_nWidth;
	nHeight=m_nHeight;
	strOptwParam.Format("HVSetOutPutWindow:(%d,%d,%d*%d)",nXStart,nYStart,nWidth,nHeight);
	strOptwParam="";

	
	///English:    ///Chinese: 计算输出图像输出窗口参数
	int nAoix,nAoiy,nAoir,nAoib,nAoiwid,nAoihei;
	nAoix=m_nXStart;
	nAoiy=m_nYStart;
	nAoir=m_nXStart+m_nWidth;
	nAoib=m_nYStart+m_nHeight;
	nAoiwid=m_nWidth;
	nAoihei=m_nHeight;
	if (m_bShowAOI) {
		strAOI.Format("AOI:(%d,%d),%dX%d)",nAoix,nAoiy,nAoiwid,nAoihei);	
	}else{
		strAOI="";
	}
	pFrame->m_wndStatusBar.SetPaneText(2,strAOI,TRUE);
	
	
	
	CPoint inPoint;CRect nClientRect;
	GetClientRect(nClientRect);
	inPoint.x=1.000000*(point.x-m_rcOutputWindows.left)/m_rcOutputWindows.Width()*m_nMaxWidth;
	inPoint.y=1.000000*(point.y-m_rcOutputWindows.top)/m_rcOutputWindows.Height()*m_nMaxHeight;
	
	BOOL bShowInfo=TRUE;
	
	if (point.x<m_rcOutputWindows.left) {
		inPoint.x=0;
		bShowInfo=FALSE;
	}
	if (point.y<m_rcOutputWindows.top) {
		inPoint.y=0;bShowInfo=FALSE;
	}
	
	if (point.x>m_rcOutputWindows.right) {
		inPoint.x=m_nMaxWidth;
		bShowInfo=FALSE;
	}
	if (point.y>m_rcOutputWindows.bottom) {
		inPoint.y=m_nMaxHeight;
		bShowInfo=FALSE;
	}
	
	
	if (bShowInfo){
		strMousepos.Format("X:%d,Y:%d",inPoint.x,inPoint.y);
		pFrame->m_wndStatusBar.SetPaneText(3,strMousepos,TRUE);
	}
	else{
		///English:    ///Chinese: strMousepos.Format("客户区坐标为(X:%d,Y:%d)",point.x,point.y);
		strMousepos="";
		pFrame->m_wndStatusBar.SetPaneText(3,strMousepos,TRUE);
	}
	
	
	CMDIChildWnd::OnMouseMove(nFlags, point);
}

void CBounceWnd::OnOutputwinScale100() 
{
	// TODO: Add your command handler code here
	CMainFrame* pMain=(CMainFrame*)AfxGetApp()->m_pMainWnd;
   
    int ntrakleft,ntraktop, ntrakwidth,ntrakheight, ndiswidth,ndisheight;
	ntrakleft=m_tracker.m_rect.left;
	ntraktop=m_tracker.m_rect.top;
	ntrakwidth=m_tracker.m_rect.Width();
	ntrakheight=m_tracker.m_rect.Height();
	
	ndiswidth=ntrakleft-m_rcOutputWindows.left;///English:    ///Chinese: AOI与输出窗口边界宽
	ndisheight=ntraktop-m_rcOutputWindows.top;///English:    ///Chinese: AOI与输出窗口边界高
	
	float nbfscale=m_fOptwScale;///English:    ///Chinese: 保存原来缩放倍数
	m_fOptwScale=1.0000000;
	
	m_rcOutputWindows.SetRect(4,4,m_nMaxWidth*m_fOptwScale+4,m_nMaxHeight*m_fOptwScale+4);///English:    ///Chinese: 最大输出窗口区域
	
	SetScrollInfoX();
	m_tracker.m_rect.left=m_rcOutputWindows.left+(ndiswidth)*(m_fOptwScale/nbfscale);
	m_tracker.m_rect.top=m_rcOutputWindows.top+(ndisheight)*(m_fOptwScale/nbfscale);
	
	m_tracker.m_rect.right=m_tracker.m_rect.left+ntrakwidth*(m_fOptwScale/nbfscale);
	m_tracker.m_rect.bottom=m_tracker.m_rect.top+ntrakheight*(m_fOptwScale/nbfscale);
	
	this->Invalidate(TRUE);
	
}

void CBounceWnd::OnUpdateOutputwinScale100(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (m_fOptwScale==1.000000) {
		pCmdUI->SetCheck(TRUE);
	}else
		pCmdUI->SetCheck(FALSE);
}

void CBounceWnd::OnOutputwinScale150() 
{
	// TODO: Add your command handler code here
	
	int ntrakleft,ntraktop, ntrakwidth,ntrakheight, ndiswidth,ndisheight;
	ntrakleft=m_tracker.m_rect.left;
	ntraktop=m_tracker.m_rect.top;
	ntrakwidth=m_tracker.m_rect.Width();
	ntrakheight=m_tracker.m_rect.Height();
	
	ndiswidth=ntrakleft-m_rcOutputWindows.left;///English:    ///Chinese: AOI与输出窗口边界宽
	ndisheight=ntraktop-m_rcOutputWindows.top;///English:    ///Chinese: AOI与输出窗口边界高
	
	float nbfscale=m_fOptwScale;///English:    ///Chinese: 保存原来缩放倍数
	m_fOptwScale=1.5000000;
	
	m_rcOutputWindows.SetRect(4,4,4+m_nMaxWidth*m_fOptwScale,m_nMaxHeight*m_fOptwScale+4);///English:    ///Chinese: 最大输出窗口区域
	
	SetScrollInfoX();
	
    m_tracker.m_rect.left=m_rcOutputWindows.left+(ndiswidth)*(m_fOptwScale/nbfscale);
	m_tracker.m_rect.top=m_rcOutputWindows.top+(ndisheight)*(m_fOptwScale/nbfscale);
	
	m_tracker.m_rect.right=m_tracker.m_rect.left+ntrakwidth*(m_fOptwScale/nbfscale);
	m_tracker.m_rect.bottom=m_tracker.m_rect.top+ntrakheight*(m_fOptwScale/nbfscale);
	this->Invalidate(TRUE);
	
}

void CBounceWnd::OnUpdateOutputwinScale150(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (m_fOptwScale==1.500000) {
		pCmdUI->SetCheck(TRUE);
	}else
		pCmdUI->SetCheck(FALSE);
	
}

void CBounceWnd::OnOutputwinScale200() 
{
	// TODO: Add your command handler code here
	
    int ntrakleft,ntraktop, ntrakwidth,ntrakheight, ndiswidth,ndisheight;
	ntrakleft=m_tracker.m_rect.left;
	ntraktop=m_tracker.m_rect.top;
	ntrakwidth=m_tracker.m_rect.Width();
	ntrakheight=m_tracker.m_rect.Height();
	
	ndiswidth=ntrakleft-m_rcOutputWindows.left;///English:    ///Chinese: AOI与输出窗口边界宽
	ndisheight=ntraktop-m_rcOutputWindows.top;///English:    ///Chinese: AOI与输出窗口边界高
	
	float nbfscale=m_fOptwScale;///English:    ///Chinese: 保存原来缩放倍数
	m_fOptwScale=2.0000000;
	
	m_rcOutputWindows.SetRect(4,4,m_nMaxWidth*m_fOptwScale+4,m_nMaxHeight*m_fOptwScale+4);///English:    ///Chinese: 最大输出窗口区域
	
	SetScrollInfoX();
	
    m_tracker.m_rect.left=m_rcOutputWindows.left+(ndiswidth)*(m_fOptwScale/nbfscale);
	m_tracker.m_rect.top=m_rcOutputWindows.top+(ndisheight)*(m_fOptwScale/nbfscale);
	
	m_tracker.m_rect.right=m_tracker.m_rect.left+ntrakwidth*(m_fOptwScale/nbfscale);
	m_tracker.m_rect.bottom=m_tracker.m_rect.top+ntrakheight*(m_fOptwScale/nbfscale);
	
	this->Invalidate(TRUE);
	
}

void CBounceWnd::OnUpdateOutputwinScale200(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (m_fOptwScale==2.000000) {
		pCmdUI->SetCheck(TRUE);
	}else
		pCmdUI->SetCheck(FALSE);

}

void CBounceWnd::OnSetFocus(CWnd* pOldWnd) 
{
	CMDIChildWnd::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	ASSERT(pFrame);
	pFrame->m_wndTree.SelectItem(pFrame->m_MIDhti[m_nListNum-1]);///English:    ///Chinese: 升级需要改			
}

void CBounceWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CMenu Menu;
	Menu.LoadMenu(IDR_BOUNCE); 
	CMenu *pMenu = (CMenu *)Menu.GetSubMenu(2);
    CPoint pt;
	GetCursorPos(&pt);
	pMenu->TrackPopupMenu(TPM_RIGHTBUTTON,pt.x,pt.y,this,0);
	CMDIChildWnd::OnRButtonUp(nFlags, point);
}


void CBounceWnd::AdjustExposureTime()
{
	if (m_pDlgPrp!=NULL)
	{
		CDlgPrp *plg=(CDlgPrp*)m_pDlgPrp;
		
		if (m_strCamerName=="HV2000FC")
		{
			plg->SetExposureTime_HV2000FC(m_hhv,m_nWidth,plg->m_nShutterUnit,plg->m_nShutterSpeed);
		}
		else if (m_strCamerName=="HV3100FC")
		{
			plg->SetExposureTime_HV3100FC(m_hhv,m_nWidth,plg->m_nShutterUnit,plg->m_nShutterSpeed);
			
		}
		else if (m_strCamerName=="HV1300FC"||m_strCamerName=="HV1300FM")
		{
			plg->SetExposureTime_HV1300(m_hhv,m_nWidth,plg->m_nShutterUnit,plg->m_nShutterSpeed);
		}
	}
}

void CBounceWnd::OnGrabContinuous() 
{
	// TODO: Add your command handler code here
	
	if (!m_bOpen) {		
		Open();
	}
	if (m_bOpen) {
		if (!m_bStart) {
			Start();
		}
		
	}

}

void CBounceWnd::OnGrabCancel() 
{
	// TODO: Add your command handler code here
	if (m_bOpen) {
		Stop();
		if (!m_bStart) {
			Close();
		}
	}
}

void CBounceWnd::OnUpdateGrabContinuous(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_bStart);
}

void CBounceWnd::OnUpdateGrabCancel(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bStart);
}

void CBounceWnd::SetCMOSVideoMode0()
{
	HVSTATUS status = STATUS_OK;
	
	m_Resolution = RES_MODE0;
	
	int nBuffSize;
	HVGetDeviceInfo(m_hhv, DESC_RESOLUTION, NULL,&nBuffSize);	
	
	BYTE *pbContext = new BYTE[nBuffSize];
	DWORD *pdContext =(DWORD *)(pbContext);
	HVGetDeviceInfo(m_hhv, DESC_RESOLUTION, pdContext,&nBuffSize);	///English:    ///Chinese: 获取摄像机分辨率
	
	m_nWidth =m_nMaxWidth = *(pdContext + 2*RES_MODE0);
	m_nHeight=m_nMaxHeight = *(pdContext + 2*RES_MODE0 + 1);
	
	delete []pbContext;
	
    
    BOOL bRestart = FALSE;  ///English:    ///Chinese: 在设置输出窗口前,停止采集
	if (m_bStart)
	{
		bRestart=TRUE;
	}
	if (bRestart)
	{
		Stop();
		
	}
	
	status = HVSetResolution(m_hhv, RES_MODE0);   ///English:    ///Chinese: 设置摄像机分辨率
	
    m_rcOutputWindows.SetRect(4,4,m_nWidth*m_fOptwScale+4,m_nHeight*m_fOptwScale+4);///English:    ///Chinese: 最大输出窗口区域

	status=HVSetOutputWindow(m_hhv, 0,0, m_nWidth,m_nHeight);  ///English:    ///Chinese: 设置摄像机输出窗口
	HV_VERIFY(status);

	OnAoiMachingBest();
	
	if (bRestart)         ///English:    ///Chinese: 在设置输出窗口后,开始采集
	{
		Start();
	
	}
	Invalidate();

}

void CBounceWnd::SetCMOSVideoMode1()
{
	HVSTATUS status = STATUS_OK;
	m_Resolution = RES_MODE1;
	
	int nBuffSize;
	HVGetDeviceInfo(m_hhv, DESC_RESOLUTION, NULL,&nBuffSize);	
	
	BYTE *pbContext = new BYTE[nBuffSize];
	DWORD *pdContext =(DWORD *)(pbContext);
	HVGetDeviceInfo(m_hhv, DESC_RESOLUTION, pdContext,&nBuffSize);///English:    ///Chinese: 获取摄像机分辨率	
	
	m_nWidth=m_nMaxWidth = *(pdContext + 2*RES_MODE1);
	m_nHeight=m_nMaxHeight = *(pdContext + 2*RES_MODE1 + 1);
	
	delete []pbContext;
	
    
	BOOL bRestart = FALSE;  ///English:    ///Chinese: 在设置输出窗口前，停止采集
	if(m_bStart)
		bRestart=TRUE;
	if (bRestart)
	{
		Stop();
	}
	
	status = HVSetResolution(m_hhv, RES_MODE1);   ///English:    ///Chinese: 设置摄像机分辨率
	
	m_rcOutputWindows.SetRect(4,4,m_nWidth*m_fOptwScale+4,m_nHeight*m_fOptwScale+4);///English:    ///Chinese: 最大输出窗口区域
	status=HVSetOutputWindow(m_hhv, 0,0, m_nWidth,m_nHeight); ///English:    ///Chinese: 设置摄像机输出窗口
	OnAoiMachingBest();

	
	if (bRestart)          ///English:    ///Chinese: 在设置输出窗口后,开始采集
	{
		Start();
	}
	Invalidate();

}

void CBounceWnd::SetCMOSVideoMode2()
{
	HVSTATUS status = STATUS_OK;
	m_Resolution = RES_MODE2;
    
	int nBuffSize;
	HVGetDeviceInfo(m_hhv, DESC_RESOLUTION, NULL,&nBuffSize);	
	
	BYTE *pbContext = new BYTE[nBuffSize];
	DWORD *pdContext =(DWORD *)(pbContext);
	HVGetDeviceInfo(m_hhv, DESC_RESOLUTION, pdContext,&nBuffSize); ///English:    ///Chinese: 获取摄像机分辨率	
	
	m_nWidth= m_nMaxWidth = *(pdContext + 2*RES_MODE2);
	m_nHeight=m_nMaxHeight = *(pdContext + 2*RES_MODE2 + 1);
	delete []pbContext;	
    
	
	BOOL bRestart = FALSE;    ///English:    ///Chinese: 在设置输出窗口前，停止采集
	if (m_bStart)
	{
		bRestart=TRUE;
	}
	if (bRestart)
	{
		Stop();
		
	}
	
	status = HVSetResolution(m_hhv, RES_MODE2);  ///English:    ///Chinese: 设置摄像机分辨率
	
    m_rcOutputWindows.SetRect(4,4,m_nWidth*m_fOptwScale+4,m_nHeight*m_fOptwScale+4);///English:    ///Chinese: 最大输出窗口区域
	status=HVSetOutputWindow(m_hhv, 0,0, m_nWidth,m_nHeight);  ///English:    ///Chinese: 设置摄像机输出窗口
	OnAoiMachingBest();
	
	if (bRestart)             ///English:    ///Chinese: 在设置输出窗口后，开始采集
	{
		Start();
	
	}
	Invalidate();

}

void CBounceWnd::OnGrabSingle() 
{
	// TODO: Add your command handler code here

	if(m_pRawBuffer==NULL)///English:    ///Chinese: 分配原始图像缓冲区，用来存储采集到的原始图像数据
	{
		m_pRawBuffer = new BYTE[m_nMaxWidth * m_nMaxHeight];
		ASSERT(m_pRawBuffer);
	}
	
	if(m_pRGBBuffer==NULL)///English:    ///Chinese: 分配Bayer转换后图像数据缓冲
	{
		m_pRGBBuffer = new BYTE[m_nMaxWidth * m_nMaxHeight * 3];
		ASSERT(m_pRGBBuffer);
	}
	
	HVSetOutputWindow(m_hhv, m_nXStart,m_nYStart, m_nWidth,m_nHeight);///English:    ///Chinese: 设置输出窗口

	BYTE *ppBuff[1] ;///English:    ///Chinese: 只定义一个缓冲区来保存图像数据
	ppBuff[0] = m_pRawBuffer;
	HVSTATUS status= HVSnapShot(m_hhv, ppBuff, 1);///English:    ///Chinese: 采集一帧图像数据到内存缓冲区中
	HV_MESSAGE(status);                           
	if (HV_SUCCESS(status)) {
		ShowImage();
	}
}

void CBounceWnd::OnUpdateGrabSingle(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_bStart);
}



void CBounceWnd::TestSetDefaultValue()
{
	HVTYPE TYPE;
	HVSTATUS status=STATUS_OK;
	int nSize = sizeof(HVTYPE);
	status = HVGetDeviceInfo(m_hhv, DESC_DEVICE_TYPE, &TYPE,&nSize);///English:    ///Chinese: 获取摄像机类型

	TRACE("=======================TestSetDefaultValue=======================");
    
    ///English:    ///Chinese: CCD摄像机
	if (IS_CCD_CAMERA(TYPE))	
	{
		HVAGCControl(m_hhv,AGC_GAIN,296);           ///English:    ///Chinese: 增益
		HVAECControl(m_hhv, AEC_SHUTTER_UNIT,1);    ///English:    ///Chinese: 速度单位
		Spec_Fun_Interface(m_hhv,STROBE_ON_OFF,1);  ///English:    ///Chinese: 闪光灯开关
		HVADCControl(m_hhv,ADC_BRIGHTNESS,16);      ///English:    ///Chinese: 亮度
		HVCommand(m_hhv,CMD_FRAME_SHOT_FINISH,0,0); ///English:    ///Chinese: 帧冻结 
		HVADCControl(m_hhv,ADC_BITS, (HV_ADC_LEVEL)4);
		if (!IS_NOT_SV_COLOR(TYPE))
		{
			HVAGCControl(m_hhv,AGC_GAIN_BLUE_CHANNEL,28); ///English:    ///Chinese: 蓝通道
		    HVAGCControl(m_hhv,AGC_GAIN_RED_CHANNEL,14);  ///English:    ///Chinese: 红通道
		}
		if (IS_SV1311(TYPE)||IS_SV400(TYPE)||IS_SV1420(TYPE)||IS_SV2000(TYPE)||\
			IS_SV1410(TYPE)||IS_SV1400(TYPE))
		{
			Spec_Fun_Interface(m_hhv,TEST_IMAGE,0);           ///English:    ///Chinese: 测试图像
			Spec_Fun_Interface(m_hhv,TRIGGER_DELAY_ON_OFF,0); ///English:    ///Chinese: 延迟曝光
			Spec_Fun_Interface(m_hhv,TRIGGER_DELAY_VALUE,1);  ///English:    ///Chinese: 延迟时间值
			Spec_Fun_Interface(m_hhv,TRIGGER_SOURCE_IO,0);    ///English:    ///Chinese: 输入IO
			Spec_Fun_Interface(m_hhv,TRANSFERS_DELAY_VALUE,0);///English:    ///Chinese: 延迟传输
			Spec_Fun_Interface(m_hhv,COLOR_MODE,0);           ///English:    ///Chinese: Raw数据格式
			
		}
		
		HV_CMD_PACKET_SIZE sz;
		if (IS_SV400(TYPE))
		{
		    sz.dwSize=2800;
			HVCommand(m_hhv,CMD_SET_BYTE_PER_PACKET,&sz,0);///English:    ///Chinese: 包长
			HVAECControl(m_hhv, AEC_SHUTTER_SPEED,20);     ///English:    ///Chinese: 快门速度
		}
		else if (IS_SV1300(TYPE))
		{
			sz.dwSize=1600;
			HVCommand(m_hhv,CMD_SET_BYTE_PER_PACKET,&sz,0);
			HVAECControl(m_hhv, AEC_SHUTTER_SPEED,133);

		}
		else if (IS_SV1410(TYPE)||IS_SV1420(TYPE))
		{
			sz.dwSize=2800;
			HVCommand(m_hhv,CMD_SET_BYTE_PER_PACKET,&sz,0);
			HVAECControl(m_hhv, AEC_SHUTTER_SPEED,60);

		}
		else if (IS_SV2000(TYPE))
		{
			sz.dwSize=3600;
			HVCommand(m_hhv,CMD_SET_BYTE_PER_PACKET,&sz,0);
			HVAECControl(m_hhv, AEC_SHUTTER_SPEED,70);
		}
		else if (IS_SV1400(TYPE))
		{
			sz.dwSize=1400;
			HVCommand(m_hhv,CMD_SET_BYTE_PER_PACKET,&sz,0);
			HVAECControl(m_hhv, AEC_SHUTTER_SPEED,120);
		}
		else
		{
			sz.dwSize=2500;
			HVCommand(m_hhv,CMD_SET_BYTE_PER_PACKET,&sz,0);
			HVAECControl(m_hhv, AEC_SHUTTER_SPEED,60);
		}
		OnAoiMachingBest();
		
	}//ccd end
	
	///English:    ///Chinese: CMOS摄像机
    if (IS_CMOS_1394_CAM(TYPE))
	{
		HVAGCControl(m_hhv,AGC_GAIN,8);
		HVADCControl(m_hhv,ADC_BITS,ADC_LEVEL3);
		HVADCControl(m_hhv,ADC_BLKLEVEL_CAL,1);
		for (int Channel = ADC_BLKLEVEL_CAL_REDCHANNEL; Channel <= ADC_BLKLEVEL_CAL_BLUECHANNEL; ++Channel )
		{
			HVADCControl(m_hhv,Channel,-20);
		}
        SetCMOSVideoMode0();
		m_Resolution=(HV_RESOLUTION)0;
		
		
        if (IS_HV1300(TYPE))
		{ 
		   status = HVAECControl(m_hhv, AEC_EXPOSURE_TIME,945);//945
		   //SetExposureTime_HV1300(m_hhv,1280,1000,60);	
		}
		if (IS_HV2000(TYPE))
		{
			status = HVAECControl(m_hhv, AEC_EXPOSURE_TIME, 1227);//1227
			//SetExposureTime_HV2000FC(m_hhvCam,1600,1000,100);
		}
		if (IS_HV3100(TYPE))
		{
			status = HVAECControl(m_hhv, AEC_EXPOSURE_TIME, 1426);//1426
			//SetExposureTime_HV2000FC(m_hhvCam,2048,1000,152);
		}
		
    }
	HVSetSnapMode(m_hhv,CONTINUATION);///English:    ///Chinese: 采集模式
    HVSetTriggerPolarity(m_hhv,HIGH);///English:    ///Chinese: 外触发极性
    HVSetStrobePolarity(m_hhv,HIGH);///English:    ///Chinese: 闪光灯极性

}

void CBounceWnd::Spec_Fun_Interface(HHV hhv, HV_INTERFACE1_ID Interface_ID, int value)
{
	HV_INTERFACE1_CONTEXT pInfcectx;
	pInfcectx.ID = Interface_ID;
	pInfcectx.dwVal=(DWORD)value;
	HVSTATUS status = HVCommand(hhv,CMD_SPEC_FUN_INTERFACE1,&pInfcectx,0);

}

void CBounceWnd::Calculatefps()
{
	CString strfps="";
	CString strTitle;
	float fps;

	m_T2=::GetTickCount();
	
    if(m_T2-m_T1>1000){
		fps=1000.0*m_C/(m_T2-m_T1);
		if (fps==0) {
			m_bBegin=FALSE;///English:    ///Chinese: 当前工作的摄像机被拔出
		}
		strfps.Format(" %f(fps)  ",fps);
		m_strFPS=strfps;
		m_C=0;
		m_T1=m_T2=::GetTickCount();
	}
	
//  strTitle.Format("%s%d",m_strCamerName,m_nListNum);
	strTitle.Format("%s",m_strCamerDispName);
	strTitle+=m_strFPS;
	SetWindowText(strTitle);

}

void CBounceWnd::OnImagemode() 
{
	// TODO: Add your command handler code here
	m_ImageMode=(HV_IMAGE_MODE)(!m_ImageMode);
	
}

void CBounceWnd::OnUpdateImagemode(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_ImageMode == HV_COLOR);

}

BOOL CBounceWnd::BMPBufBottomToUp(BYTE *pInBuffer, BYTE *pOutBuffer, int nWidth, int nHeight)
{
	int i = 0; 
	int j = 0;
	BYTE temp=0;
	int ImgWidth = nWidth;
	int ImgHeight= nHeight;
	
	BYTE *pTempBit8 = new BYTE[ImgWidth*ImgHeight];
	if(pTempBit8 == NULL||pInBuffer==NULL)
		return -1 ;
	
	memcpy(pTempBit8,pInBuffer,ImgWidth*ImgHeight);
	
	for(i = 0; i < ImgHeight /2; ++i)
	{
		for(j = 0; j <ImgWidth; ++j )
		{
			temp = pTempBit8[i*ImgWidth + j];
			pTempBit8[i*ImgWidth + j] = pTempBit8[(ImgHeight-1-i) * ImgWidth +j];
			pTempBit8[(ImgHeight-1-i) * ImgWidth +j]  = temp; 
		}
	}
	
	memcpy(pOutBuffer,pTempBit8,ImgWidth*ImgHeight);
    if(pTempBit8)
	{
		delete []pTempBit8;
		pTempBit8 = NULL;
	}
	
	return 1;

}
LRESULT CBounceWnd::OnGetRatioMessage(WPARAM wParam, LPARAM lParam)
{
	if (m_pRGBBuffer) 
	{
		if (m_ImageMode	== HV_COLOR)
		{
            SetLutTable(1.0,1.0,1.0);
			m_pBmpInfo->bmiHeader.biBitCount = 24;
			CSize Size;
			Size.cx  = m_pBmpInfo->bmiHeader.biWidth;
			Size.cy = m_pBmpInfo->bmiHeader.biHeight;
			
			///English:    ///Chinese: 将原始图像数据进行Bayer转换,转换后为24位,同时将原始数据进行上下翻转
			ConvertBayer2Rgb(m_pRGBBuffer, m_pRawBuffer,Size.cx,Size.cy, BAYER2RGB_NEIGHBOUR,m_pLutR,m_pLutG,m_pLutB,true,m_Layout);
			
			///English:    ///Chinese: 获取RGB24图像的白平衡系数
			GetWhiteBalanceRatio(m_pRGBBuffer, 
				(int)m_pBmpInfo->bmiHeader.biWidth, (int)m_pBmpInfo->bmiHeader.biHeight, 
				&m_dRatioR, &m_dRatioG, &m_dRatioB);
			
		} 
		else 
		{
			return 1;
		}
		
	} 
	
	return 1;
}

void CBounceWnd::OnWhiteBalance() 
{
	// TODO: Add your command handler code here
	CWBDlg dlg(this);

	if (dlg.DoModal() == IDOK) 
	{
	} 
}

void CBounceWnd::OnUpdateWhiteBalance(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_ImageMode == HV_COLOR);
}

BOOL CBounceWnd::SetLutTable(double dRatioR, double dRatioG, double dRatioB)
{
	if(dRatioR <=0)
		return FALSE;
	if(dRatioG <=0)
		return FALSE;
	if(dRatioB <=0)
		return FALSE;

	for(int i=0;i<256;i++)
	{
		
		m_pLutR[i] = min((int)(i*dRatioR),255);
		m_pLutG[i] = min((int)(i*dRatioG),255);
		m_pLutB[i] = min((int)(i*dRatioB),255);
		
	}

	return TRUE;

}


 
