// MainFrm.cpp : implementation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChildFrm.h"
#include "MainFrm.h"
#include "utility.h"
#include "aboutdlg.h"
#include "zoomdlg.h"
#include "cameradlg.h"
#include "Device.h"
#include "AutoTest.h"

#include <htmlhelp.h>

CMainFrame::CMainFrame() : 
m_DeviceManager(*this),
m_BusView(m_DeviceManager, *this),
m_PropertyView(m_DeviceManager, *this),
m_fPropertyPageVisible(false),
m_fBroadCastFps(false),
m_nSaveTimes(0),
m_nDeviceNum(0)
{
	m_bClearReg = FALSE;
	m_bToClose = FALSE;
}

CMainFrame::~CMainFrame()
{
}



BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if(CMDIFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
		return TRUE;
	
	HWND hWnd = MDIGetActive();
	if(hWnd != NULL)
		return (BOOL)::SendMessage(hWnd, WM_FORWARDMSG, 0, (LPARAM)pMsg);
	
	return FALSE;
}

BOOL CMainFrame::OnIdle()
{
	m_Dock.OnIdle();
	
	UpdateUI();
	return FALSE;
}




LRESULT CMainFrame::OnFileSaveConfiguration(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CDevice* pDevice = m_DeviceManager.GetCurrentDevice();
	
	if ( pDevice == NULL )
		return 0;
	try
	{
//		CFileDialog dlg(FALSE, _T("cfg"), pDevice->m_pInfo->ModelName()+".cfg", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Bcam Configuration Files (*.cfg)\0*.cfg\0"), m_hWnd);
		CFileDialog dlg(FALSE, _T("cfg"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Bcam Configuration Files (*.cfg)\0*.cfg\0"), m_hWnd);
		if ( dlg.DoModal() == IDOK)
		{
			pDevice->SaveConfiguration(CString(dlg.m_szFileName));
		}
	} CATCH_REPORT();
	return 0;
}


LRESULT CMainFrame::OnFileRestoreConfiguration(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CDevice* pDevice = m_DeviceManager.GetCurrentDevice();
	if ( pDevice == NULL )
		return 0;
	try
	{
		CFileDialog dlg(TRUE, _T("cfg"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Bcam Configuration Files (*.cfg)\0*.cfg\0"), m_hWnd);
//		CFileDialog dlg(TRUE, _T("cfg"), pDevice->m_pInfo->ModelName()+".cfg", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Bcam Configuration Files (*.cfg)\0*.cfg\0"), m_hWnd);
		if ( dlg.DoModal() == IDOK)
		{
			////-----07.4.3-------------
			BOOL bRestart = FALSE;
			if (pDevice->IsContinuousGrabActive()) {
				bRestart = TRUE;
			}
			
			try {
				if (bRestart) {
					pDevice->GrabCancel();
				}
				pDevice->RestoreConfiguration(CString(dlg.m_szFileName));
				m_PropertyView.Update_PRP(pDevice); 
				if (bRestart) {
					pDevice->GrabContinuous();
				}
			}
			CATCH_REPORT();
	        /////-----------------
//			pDevice->RestoreConfiguration(CString(dlg.m_szFileName));
//			m_PropertyView.CurrentDeviceChanged(pDevice); 
		}
	} CATCH_REPORT();
	return 0;
}



LRESULT CMainFrame::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	try
	{
		m_bToClose = TRUE;
		m_DeviceManager.AppExit(m_bClearReg);
	} CATCH_REPORT();
	
	try
	{
		CPropertyBagPtr pDockViewBag = CRegistryPropertyBag::Create(CString(APPLICATION_KEY) + "DockView");
		
		RECT rc = { 0 };
		int iDockState = 0;
		m_Dock.GetWindowState(m_BusView.m_hWnd, iDockState, rc);
		int cy=rc.bottom-rc.top;
		pDockViewBag->WriteLong("BusSize", cy);
		m_Dock.GetWindowState(m_PropertyView.m_hWnd, iDockState, rc);
		pDockViewBag->WriteLong("PropertySize", rc.bottom - rc.top);

		CPropertyBagPtr pMainFramBag = CRegistryPropertyBag::Create(CString(APPLICATION_KEY) + "MainFrame");
		WINDOWPLACEMENT p;
		p.length = sizeof(p);
		GetWindowPlacement(&p);
		pMainFramBag->WriteLong("left", p.rcNormalPosition.left);
		pMainFramBag->WriteLong("right", p.rcNormalPosition.right);
		pMainFramBag->WriteLong("bottom", p.rcNormalPosition.bottom);
		pMainFramBag->WriteLong("top", p.rcNormalPosition.top);
		pMainFramBag->WriteLong("showCmd", p.showCmd);
	
	}
	CATCH_REPORT();

	bHandled = false;
	return 0;
}

LRESULT CMainFrame::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	try
	{
		if (m_hMenu != NULL)
			::DestroyMenu(m_hMenu);
		m_hMenu = NULL;
		if (m_BusView.IsWindow()) m_BusView.DestroyWindow();
		if (m_PropertyView.IsWindow()) m_PropertyView.DestroyWindow();
		if (m_CmdBar.IsWindow()) m_CmdBar.DestroyWindow();
		
	}
	CATCH_REPORT();
	
	bHandled = false;
	return 0;
}



LRESULT CMainFrame::OnCommand(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = false;
		
	if (wParam >= PLUGIN_MENU_MIN){
		CChildFrame* pChildFrame = m_DeviceManager.GetCurrentChild();
		if ( pChildFrame != NULL )
		{
			try
			{
				bHandled = pChildFrame->m_PluginManager.OnUserCommand(wParam);
			} CATCH_REPORT();
		}
	}
	
	return 0;
}


void CMainFrame::UpdateUI()
{
	// update UI elements
	UISetCheck(ID_VIEW_BUSVIEW, ::IsWindowVisible(m_BusView) );
	UISetCheck(ID_VIEW_PROPERTYVIEW, ::IsWindowVisible(m_PropertyView) );
	UISetCheck(ID_FILE_CLEAR_REG,m_bClearReg);
	
	bool MDIOpen = false;
	bool GrabActive = false;
	bool SingleGrabActive = false;
	bool WhiteBalanceActive = false;
	bool Scalable = false;
	int snapmode=0;
	
	CDevice* pDevice = m_DeviceManager.GetCurrentDevice();
	if ( pDevice != NULL ){
		MDIOpen =  m_DeviceManager.ExistMDIChild();
		GrabActive = m_DeviceManager.IsGrabActive();
		SingleGrabActive = pDevice->IsSingleGrabActive();
		Scalable = pDevice->IsScalable();

		
		if ( pDevice->m_pSnapMode->IsSupported() )
		{
			snapmode=pDevice->m_pSnapMode->Value();
		}
	}
	
	//modified by xupx 2008.06.20
	//此处m_mono8将在 CDevice::CreateNewBuffers中，根据插件版本来决定是否激活为可选项。
	bool mono8 = m_mono8;	
	bool yuv422 = ( pDevice != NULL && pDevice->GetColorCode() == COLOR_YUV8_4_2_2);
	

	UIEnable(ID_FILE_NEW, (pDevice != NULL ) && ! MDIOpen);
	UIEnable(ID_FILE_OPEN, (pDevice != NULL ) && ! GrabActive );
	UIEnable(ID_FILE_SAVE, (pDevice != NULL ) && ! GrabActive && pDevice->GetBitmap() != NULL );
	UIEnable(ID_EDIT_COPY, (pDevice != NULL ) && ( pDevice->GetBitmap() != NULL )) ;
	UIEnable(ID_CAMERA_MAXIMIZE, (pDevice != NULL ) && MDIOpen && pDevice->IsScalable() && ! SingleGrabActive );
	UIEnable(ID_GRAB_CONTINUOUS, (pDevice != NULL ) && ( ! GrabActive ));
	UIEnable(ID_TOTAL_CONTINUOUS, !IsTotalGrabActive());	//判定相机数量不为0，并且有相机没有开采 2009.02.27
	UIEnable(ID_TOTAL_CANCEL, !IsTotalGrabCancel());
	UIEnable(ID_DO_PASSIVE_TRANS, (pDevice != NULL ) && (pDevice->m_PassiveTransBufNum !=0) 
								&& (snapmode != 0) && (pDevice->m_PassiveTransOnOff == 1)
								&& (GrabActive));
	
	UIEnable(ID_GRAB_SINGLE, (pDevice != NULL ) && ( ! GrabActive )&&(snapmode==0) );
	UIEnable(ID_GRAB_CANCEL, (pDevice != NULL ) && GrabActive );
    UIEnable(ID_SOFTTRIGGER, (pDevice != NULL ) && (snapmode!=CONTINUATION) && (GrabActive));
	UIEnable(ID_PLUGINMANAGER, ( m_DeviceManager.GetCurrentChild() != NULL) );
	UIEnable(ID_CENTERCURSOR, ( m_DeviceManager.GetCurrentChild() != NULL));
	UIEnable(ID_CAMERAINFO, (pDevice != NULL) && ! SingleGrabActive );
	UIEnable(ID_DEFT_VAL, (pDevice != NULL) && ! SingleGrabActive && 
			(pDevice->m_pInfo->DeviceType() != DRVIFGIGETYPE) &&
			(pDevice->m_pInfo->DeviceType() != DRVIFENETTYPE) &&
			(!IS_ITS1394(pDevice->m_pInfo->DeviceType()))
			);
	UIEnable(ID_VIEW_ZOOM_BEST, (pDevice != NULL ) && MDIOpen && ! m_DeviceManager.GetCurrentChild()->m_View.IsZoomedToFit());
	UIEnable(ID_VIEW_NO_CONVERSION, mono8); 
	UIEnable(ID_VIEW_CONVERSION, mono8); 
	
	UIEnable(ID_WINDOW_CASCADE, MDIOpen);
	UIEnable(ID_WINDOW_TILE_HORZ, MDIOpen);
	UIEnable(ID_WINDOW_TILE_VERT, MDIOpen);
	UIEnable(ID_WINDOW_ARRANGE, MDIOpen);
	
	if ( ! GrabActive )
	{
		// delete fps panes
		m_Sbar.SetPaneText(ID_FPS_ACQUIRED_PANE, CString("") );
		m_Sbar.SetPaneText(ID_FPS_DISPLAYED_PANE, CString("") );
	}
	
	for (int i = 0; i < 2; i ++ )
	{
		UISetCheck(ID_VIEW_NO_CONVERSION + i, 
				(pDevice != NULL) && mono8 				//modified by xupx 2008.06.20
				&& pDevice->GetDisplayMode() == i );
	}

	UIUpdateToolBar();
}



LRESULT CMainFrame::OnTimer(UINT, WPARAM wParam, LPARAM, BOOL&)
{
	assert(wParam == TIMERID);
	
	// Show FPS 
	m_DeviceManager.XD_Show_All_FPS();
	
	CDevice *pCurrentDevice = m_DeviceManager.GetCurrentDevice();
	if ( pCurrentDevice != NULL )
	{
		double fps_acquired = 0;
		double fps_displayed = 0;
		m_DeviceManager.GetFrameRate(fps_displayed);
		CString out;
		if ( fps_acquired != -1 )
		{
			out.Format("FPS - acquired: %3.1f", fps_acquired);
			if ( m_fBroadCastFps )
			{
				::PostMessage(HWND_BROADCAST, m_uFpsBroadCastMessage, 0,  (long) ( fps_acquired * 10 + 0.5 )  );
			}
			
		}
		else
		{
			out = "";
			if ( m_fBroadCastFps )
			{
				::PostMessage(HWND_BROADCAST, m_uFpsBroadCastMessage, 0, 0 );
			}
		}
		m_Sbar.SetPaneText(ID_FPS_ACQUIRED_PANE, out);
		if ( fps_displayed != -1 ){
			out.Format("FPS - displayed: %3.1f", fps_displayed);
			
		}
		else
			out = "";
		m_Sbar.SetPaneText(ID_FPS_DISPLAYED_PANE, out);
		CChildFrame* pChildFrame = m_DeviceManager.GetCurrentChild();
		if(pChildFrame){
			TCHAR szCaption[128] = { 0 };    // max text length is 127 for floating caption
			::GetWindowText(pChildFrame->m_hWnd, szCaption, (sizeof(szCaption) / sizeof(TCHAR)) - 1);
		}
		m_DeviceManager.ShowPixelValue(); 
		
		if(pCurrentDevice->m_pInfo->DeviceType() == DRVIFGIGETYPE ||pCurrentDevice->m_pInfo->DeviceType() == DRVIFENETTYPE)
		{
			bool GrabActive			 = m_DeviceManager.IsGrabActive();
			int  snapmode = pCurrentDevice->m_pSnapMode->Value();

			if((snapmode != 0) && (pCurrentDevice->m_PassiveTransOnOff == 1) && (GrabActive))
				m_PropertyView.Update_DefaultValue(pCurrentDevice, 0);	
		}
	}
	
	return 0;
}



LRESULT CMainFrame::OnSetCursor(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	bHandled = FALSE;
	m_DeviceManager.ShowPixelValue();
	
	return 0;
}



void CMainFrame::CurrentDeviceChanged(CDevice* pDevice)
{
	if ( pDevice == NULL)	// 2009.09.02 add by xupx 
		return;
	try
	{
		// refresh the feature views
		Refresh(pDevice);  
		m_BusView.CurrentDeviceChanged(pDevice);
		
		if(pDevice->IsOpen())		// add by xupx 2009.07.28 (如果没有正常打开相机，则不需加载属性栏)
			m_PropertyView.Update_PRP(pDevice);
		/*	
		// check if the current device supports the shading feature
		m_fCurrentDeviceSupportsShading = false;
		if ( pDevice != NULL )
		{
		try
		{
		m_fCurrentDeviceSupportsShading = pDevice->ShadingCorrection.IsSupported();
		}
		CATCH_REPORT();
		}
		*/
	}
	CATCH_REPORT()
}



void CMainFrame::Refresh(CDevice* pDevice)
{
	if ( m_DeviceManager.GetCurrentDevice() != pDevice )
		return; // nothing to be done
	// handle feature windows
	
}





void CMainFrame::ConfigurationChanged(CDevice* pDevice, HV_VIDEO_MODE mode, HV_COLOR_CODE code)
{
	
	assert (pDevice != NULL );
	
}




LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// create command bar window
	HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
	// attach menu
	m_CmdBar.AttachMenu(GetMenu());
	// load command bar images
	m_CmdBar.LoadImages(IDR_MAINFRAME);
	// remove old menu
	SetMenu(NULL);
	
	HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);
	
	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndCmdBar);
	AddSimpleReBarBand(hWndToolBar, NULL, TRUE);
	
	CreateSimpleStatusBar("");
	
	m_Sbar.SubclassWindow(m_hWndStatusBar);
	int arrParts[] =
	{
		    ID_DEFAULT_PANE,
			ID_AOI_PANE,
			ID_POS_PANE,
			ID_VALUE_PANE,
			//		ID_FPS_ACQUIRED_PANE,
			ID_FPS_DISPLAYED_PANE
	};
	m_Sbar.SetPanes(arrParts, sizeof(arrParts) / sizeof(int), false);
	
	CreateMDIClient();
	//
	m_MenuControl.Install(m_CmdBar, m_hWndMDIClient);
	m_CmdBar.SetMDIClient(m_hWndMDIClient);
	
	UIAddToolBar(hWndToolBar);
	UISetCheck(ID_VIEW_TOOLBAR, 1);
	UISetCheck(ID_VIEW_STATUS_BAR, 1);
	
	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);
	
	m_Dock.m_hwndClient = NULL;
	HWND hwndDock = m_Dock.Create(m_hWnd, rcDefault);
	m_Dock.SetExtendedDockStyle(DCK_EX_REMEMBERSIZE);
	
	::HVGetDeviceTotal(&m_nDeviceNum);

	int iBusCy = 0, iPropertyCy = 0;
	try
	{
		CPropertyBagPtr ptrRegistry = CRegistryPropertyBag::Open(CString(APPLICATION_KEY) + "DockView");
		iBusCy = ptrRegistry->ReadLong("BusSize");
		iPropertyCy = ptrRegistry->ReadLong("PropertySize");
	}
	catch ( HVBaseException& )
	{
	}

	iBusCy = 80 + 40 * GetDeviceNum();

	m_Dock.SetPaneSize(DOCK_LEFT, 250);
//	m_Dock.SetPaneSize(DOCK_RIGHT, 220);
//	m_Dock.SetPaneSize(DOCK_TOP, 150);

	// Create bus viewer window
	m_BusView.Create(hwndDock, rcDefault);
	CString str;
	str.LoadString(IDS_BUS_VIEW);
	m_BusView.SetWindowText(str);
	m_Dock.AddWindow(m_BusView);
	m_Dock.DockWindow(m_BusView, DOCK_LEFT, iBusCy); 
	UISetCheck(ID_VIEW_BUSVIEW, TRUE);
	
	// Create property viewer window
	m_PropertyView.Create(hwndDock, rcDefault); 
	str.LoadString(IDS_PROPERTY_VIEW);
	m_PropertyView.SetWindowText(str);	
	m_Dock.AddWindow(m_PropertyView);
	m_Dock.DockWindow(m_PropertyView, DOCK_LEFT, iPropertyCy);
	UISetCheck(ID_VIEW_PROPERTYVIEW, TRUE);


	// Create docked views
	m_Dock.SetClient(m_hWndMDIClient);

	m_hWndClient = hwndDock;

	// Restore the image size and position
	WINDOWPLACEMENT p;
	ZeroMemory(&p, sizeof(WINDOWPLACEMENT) );
	p.rcNormalPosition.left = 24;
	p.rcNormalPosition.right = 1000;
	p.rcNormalPosition.top = 30;
	p.rcNormalPosition.bottom = 734 ;
	p.showCmd = SW_SHOWNORMAL;
	int showCmd = SW_MAXIMIZE;
	p.length = sizeof(WINDOWPLACEMENT);
	try
	{
		CPropertyBagPtr ptrRegistry = CRegistryPropertyBag::Open(CString(APPLICATION_KEY) + "MainFrame");
		p.rcNormalPosition.left = ptrRegistry->ReadLong("left");
		p.rcNormalPosition.right = ptrRegistry->ReadLong("right");
		p.rcNormalPosition.bottom = ptrRegistry->ReadLong("bottom");
		p.rcNormalPosition.top = ptrRegistry->ReadLong("top");
		showCmd = ptrRegistry->ReadLong("showCmd");;
	}
	catch ( HVBaseException& )
	{
	}
	SetWindowPlacement(&p);
	
	if ( showCmd == SW_MAXIMIZE )
	{
		PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	}
	
	// register fps broadcast message 
	m_uFpsBroadCastMessage = RegisterWindowMessage(BROADCAST_FPS_MSG);
	// check if we are asked to broadcast the frame rate
	CRegKey regKey;
	if ( regKey.Open(HKEY_CURRENT_USER, APPLICATION_KEY) == ERROR_SUCCESS )
	{
		DWORD val;
		if ( regKey.QueryValue(val, ENABLE_BROADCAST_KEY) != ERROR_SUCCESS )
		{
			regKey.SetValue((DWORD) 0, ENABLE_BROADCAST_KEY);  // if value entry doesn't exist, create it for convenience, but disable frame rate broadcast
		}
		else
		{
			m_fBroadCastFps = val != 0;
		}
	}
	
//	PostMessage(WM_TEST, TEST_GRAB_CONTINUOUS, 0);

	return 0;
}





// An error occured in the thread procedure of an image acquisition thread. The thread sends
// a user defined message to the main frame before it terminates. Inform the according camera object that 
// its image acquisition thread got an error. We have chosen this complicated procedure to avoid to run a local message 
// pump to allow both, waiting for the terminating thread and dispatching incoming windows messages. 
LRESULT CMainFrame::OnError(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	CDevice* pDevice = (CDevice*) lParam;
	
	if ( pDevice != NULL && m_DeviceManager.IsDeviceValid(pDevice) )
	{
		try
		{
			pDevice->OnContinuousGrabError(wParam);
		} CATCH_REPORT();
	}
	
	return 0;
}

LRESULT CMainFrame::OnTest(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{	
	CAutoTest test(*this);
	
	try{
		test.Dispatch(wParam, (void *)lParam);
	}CATCH_REPORT();
	
	return 0;
}



LRESULT CMainFrame::OnFileClearReg(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
//	CString keyName = APPLICATION_KEY;
//	SHDeleteKey(HKEY_CURRENT_USER,keyName);// _T("SoftwareMicrosoftInternet ExplorerIntelliForms"));
	m_bClearReg = !m_bClearReg;
	return 0;
}



LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PostMessage(WM_CLOSE);
	return 0;
}



LRESULT CMainFrame::OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: add code to initialize document
	if (m_DeviceManager.GetCurrentDevice() != NULL && \
		!m_DeviceManager.ExistMDIChild())
	{
		m_DeviceManager.AddMDIChild();
	}
	
	return 0;
}



LRESULT CMainFrame::OnEditCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if ( m_DeviceManager.GetCurrentDevice() != NULL && m_DeviceManager.GetCurrentDevice()->GetBitmap() != NULL )
	{
		try
		{
			bool res = m_DeviceManager.GetCurrentDevice()->GetBitmap()->CopyToClipboard(m_hWnd);
			if ( ! res )
				throw HVBaseException(::GetLastError(), "Copy to clipboard");
		}
		CATCH_REPORT();
	}
	
	return 0;
}



LRESULT CMainFrame::OnFileOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if ( m_DeviceManager.GetCurrentDevice() != NULL && ! m_DeviceManager.IsGrabActive() )
	{
		
		CFileDialog dlg(TRUE, _T("bmp"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Bitmap Files (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0"), m_hWnd);
		if(dlg.DoModal() == IDOK)
		{
			try
			{
				m_DeviceManager.GrabFromFile(dlg.m_szFileName);
			}
			CATCH_REPORT();
		}
	}
	
	return 0;
}



LRESULT CMainFrame::OnFileSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	
	if ( m_DeviceManager.GetCurrentDevice() != NULL && m_DeviceManager.GetCurrentDevice()->GetBitmap() != NULL )
	{
		
		try
		{
			CString strFilename;
			strFilename.Format("%s%03d","Image",m_nSaveTimes);

			//CFileDialog dlg(FALSE, _T("bmp"), _T("Temp"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Bitmap Files (*.bmp)\0*.bmp\0JPEG Files (*.jpg)\0*.jpg\0All Files (*.*)\0*.*\0"), m_hWnd);			
			CFileDialog dlg(FALSE, _T("bmp"), strFilename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Bitmap Files (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0"), m_hWnd);			
			if(dlg.DoModal() == IDOK)
			{
				m_nSaveTimes++;
				CHVBitmap *pBitmap = m_DeviceManager.GetCurrentDevice()->GetBitmap();
				int nConvertMono8ToRGB = m_DeviceManager.GetCurrentDevice()->GetDisplayMode();
			
				CString strExt;
				strExt = CString(dlg.m_szFileName).Right(3);
				strExt.MakeLower();
				if (strExt == "bmp")
				{
					bool res = pBitmap->Save(dlg.m_szFileName);
					if ( ! res )
						throw HVBaseException(::GetLastError(), "CImage::Save()");
				}
				else
					throw HVBaseException(STATUS_FILE_INVALID, "CImage::Save()"); 
			}
		} CATCH_REPORT();
	}
	return 0;
	
}



LRESULT CMainFrame::OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	static BOOL bVisible = TRUE;	// initially visible
	bVisible = !bVisible;
	CReBarCtrl rebar = m_hWndToolBar;
	int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
	rebar.ShowBand(nBandIndex, bVisible);
	UISetCheck(ID_VIEW_TOOLBAR, bVisible);
	UpdateLayout();
	return 0;
}



LRESULT CMainFrame::OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
	::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
	UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
	UpdateLayout();
	return 0;
}



LRESULT CMainFrame::OnViewBusView(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL visible;
	if( visible = ::IsWindowVisible(m_BusView) ) 
		m_Dock.HideWindow(m_BusView); 
	else 
		m_Dock.DockWindow(m_BusView, DOCK_LASTKNOWN);
	UISetCheck(ID_VIEW_BUSVIEW, ! visible);
	UpdateLayout();
	return 0;
}



LRESULT CMainFrame::OnViewPropertyView(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL visible;
	if( visible = ::IsWindowVisible(m_PropertyView) ) 
		m_Dock.HideWindow(m_PropertyView); 
	else 
		m_Dock.DockWindow(m_PropertyView, DOCK_LASTKNOWN);
	UISetCheck(ID_VIEW_PROPERTYVIEW, ! visible);
	UpdateLayout();
	m_PropertyView.SetFocus();
	return 0;
}




LRESULT CMainFrame::OnCenterCursor(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CChildFrame* pChildFrame = m_DeviceManager.GetCurrentChild();
	if ( pChildFrame != NULL )
	{
		try
		{
			pChildFrame->m_View.CenterCursor();
		} CATCH_REPORT();
	}
	return 0;
}



LRESULT CMainFrame::OnViewZoom(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	try
	{
		CChildFrame* pChildFrame = m_DeviceManager.GetCurrentChild();
		if ( pChildFrame != NULL )
		{
			double zoom = 1.0;
			switch ( wID )
			{
			case ID_VIEW_ZOOM_25:
				zoom = 0.25;
				break;
			case ID_VIEW_ZOOM_50:
				zoom = 0.5;
				break;
			case ID_VIEW_ZOOM_100:
				zoom = 1.0;
				break;
			case ID_VIEW_ZOOM_200:
				zoom = 2.0;
				break;
			case ID_VIEW_ZOOM_400:
				zoom = 4.0;
				break;
			case ID_VIEW_ZOOM_USER:
				{
					CZoomDlg dlg;
					zoom = pChildFrame->m_View.GetZoomLevel();
					dlg.SetFactor(zoom);
					if ( dlg.DoModal() == IDOK )
					{
						zoom = dlg.GetFactor();
					}
					if ( zoom < 1.0 / 32.0 )
					{
						zoom = 1.0 / 32.0; 
						MessageBeep(0);
					}
					else if ( zoom > 32 )
					{
						MessageBeep(0);
						zoom = 32;
					}
				}
				break;
			default:
				assert(false);
			}
			
			pChildFrame->m_View.SetZoomLevel(zoom);
			pChildFrame->m_View.ZoomIn(NULL, 0);
		}
	} CATCH_REPORT();
	
	return 0;
}

LRESULT CMainFrame::OnViewZoomBest(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CChildFrame* pChildFrame = m_DeviceManager.GetCurrentChild();
	if ( pChildFrame != NULL )
	{
		try
		{
			pChildFrame->m_View.ZoomToFit();
		} CATCH_REPORT();
	}
	
	return 0;
}


LRESULT CMainFrame::OnViewConversion(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CDevice *pDevice = m_DeviceManager.GetCurrentDevice();
	if(pDevice->m_pChildFrame->m_PluginManager.dwPluginVersion == 0x00010000)
	{
		if ( pDevice  != NULL && (pDevice->GetColorCode() == COLOR_MONO8))
		{
			try
			{
				pDevice->SetDisplayMode(wID - ID_VIEW_NO_CONVERSION);
			} CATCH_REPORT();
		}
	}
	else			//V1.1 		
	{
		//modified by xupx 2009.02.19
		if ( pDevice  != NULL && (pDevice->GetColorCode() == COLOR_MONO8 ||pDevice->GetColorCode() == COLOR_RAW8 || pDevice->GetColorCode() == COLOR_MONO16 ))// modified by xupx 2008.06.20
		{
			try
			{
				pDevice->SetDisplayMode(wID - ID_VIEW_NO_CONVERSION);
			} 
			CATCH_REPORT();
		}
	}
	
	return 0;
}




LRESULT CMainFrame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}


LRESULT CMainFrame::OnAppHelp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	HtmlHelp(m_hWnd, "hvperf-help.chm::/index.htm", HH_DISPLAY_TOPIC, NULL);
	return 0;
}


LRESULT CMainFrame::OnWindowCascade(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	MDICascade();
	return 0;
}

LRESULT CMainFrame::OnWindowTileHorz(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	MDITile();
	return 0;
}



LRESULT CMainFrame::OnWindowTileVert(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	MDITile(MDITILE_VERTICAL);
	return 0;
}



LRESULT CMainFrame::OnWindowArrangeIcons(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	MDIIconArrange();
	return 0;
}



LRESULT CMainFrame::OnGrabSingle(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if ( m_DeviceManager.GetCurrentDevice() != NULL )
	{
		try
		{
			m_DeviceManager.GrabSingle();
		}
		CATCH_REPORT();
	}
	return 0;
}

LRESULT CMainFrame::OnTotalContinuous(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	for(int i = 1; i < m_nDeviceNum + 1; i++)
	{
		m_DeviceManager.CurrentDeviceChanged(i);
		OnGrabContinuous(wNotifyCode, wID, hWndCtl, bHandled);
		
	}
	return 0;
}


LRESULT CMainFrame::OnTotalCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	for(int i = 1; i < m_nDeviceNum + 1; i++)
	{
		m_DeviceManager.CurrentDeviceChanged(i);
		OnGrabCancel(wNotifyCode, wID, hWndCtl, bHandled);
		
	}
	return 0;
}


LRESULT CMainFrame::OnDoPassiveTrans(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CDevice* pDevice = m_DeviceManager.GetCurrentDevice();
	if ( pDevice != NULL )
	{
		try
		{
			m_PropertyView.SendPassiveTrans(pDevice, ID_DO_PASSIVE_TRANS);
			
//			m_DeviceManager.GrabContinuous();
		}
		CATCH_REPORT();
	}
	
	return 0;
}


LRESULT CMainFrame::OnGrabContinuous(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if ( m_DeviceManager.GetCurrentDevice() != NULL )
	{
		try
		{
			m_DeviceManager.GrabContinuous();
		}
		CATCH_REPORT();
	}
	
	return 0;
}


LRESULT CMainFrame::OnSetDefaultValue(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CDevice *pDevice = m_DeviceManager.GetCurrentDevice();

	if(IS_READABLE_CAM(pDevice->m_pInfo->DeviceType()) )
		return 0;
	
	bool b=FALSE;
	if (pDevice->IsContinuousGrabActive()) {
		b = TRUE;
	}
	if (b) {
		pDevice->GrabCancel();
	}
	pDevice->m_bInit = FALSE;
	SetDefaultValue();
    CurrentDeviceChanged(pDevice);
	SetDefaultValue();
    CurrentDeviceChanged(pDevice);
	if (b){
		pDevice->GrabContinuous();
	}
	return 0;
}




LRESULT CMainFrame::OnGrabCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if ( m_DeviceManager.GetCurrentDevice() != NULL )
	{
		try
		{
			
				m_DeviceManager.GrabCancel();		
			
		}
		CATCH_REPORT();
	}
	
	return 0;
}


LRESULT CMainFrame::OnSoftTrigger(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if ( m_DeviceManager.GetCurrentDevice() != NULL )
    {
        try
        {
            m_DeviceManager.OnSoftTrigger();
        }
        CATCH_REPORT();
    }
    
    return 0;
}



LRESULT CMainFrame::OnDeviceMaximize(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CDevice *pDevice;
	pDevice = m_DeviceManager.GetCurrentDevice();

	//add by xupx for SV2000GC 2008.08.06
	if(IS_READABLE_CAM(pDevice->m_pInfo->DeviceType()) && !IS_HV_CAM(pDevice->m_HVTYPE))
		return 0;
	//end
	
	if ( m_DeviceManager.GetCurrentDevice() != NULL )
	{
		try
		{
			m_DeviceManager.GetCurrentDevice()->MaximizeAOI();
		}
		CATCH_REPORT();
	}
	
	return 0;
}



LRESULT CMainFrame::OnCameraInfo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if ( m_DeviceManager.GetCurrentDevice() != NULL )
	{
		try
		{
			CCameraDlg dlg(m_DeviceManager.GetCurrentDevice());
			dlg.DoModal();
		}
		CATCH_REPORT();
	}

	return 0;
}



LRESULT CMainFrame::OnPluginManager(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CChildFrame* pChildFrame = m_DeviceManager.GetCurrentChild();
	if ( pChildFrame != NULL )
	{
		try
		{
			pChildFrame->OnPluginManager();
		} CATCH_REPORT();
	}

	return 0;
}


void CMainFrame::ReportError(HVBaseException& e)
{
	if ( ! ::IsWindow(m_ErrorBox) )
	{
		m_ErrorBox.Create(m_hWnd);
		m_ErrorBox.ShowWindow(SW_SHOW);
		ATLTRACE("void CMainFrame::ReportError(HVBaseException& e)");
	}
	m_ErrorBox.ReportError(e);
}


//包长调节
void CMainFrame::OnPacketSizeChanged(CDevice *pDevice, int value)
{
    int nVal = ((value + 3) & 0xFFFFFFFC);
    BOOL bRestart = FALSE;

    if (pDevice->IsContinuousGrabActive()) {
        bRestart = TRUE;
    }
    
    try {
        if (pDevice->m_PacketSize != nVal) {
            if (bRestart) {
                pDevice->GrabCancel();
            }
            pDevice->SetPacketSize(nVal);
            pDevice->m_PacketSize = nVal;
			/*************查询一下 帧间隔*************/ 
			m_PropertyView.UpdateMultiTriggerTime(pDevice);	
			/*************2009.04.30 End***************/

            if (bRestart) {
                pDevice->GrabContinuous();
            }			
        }
    }
    CATCH_REPORT();

}


void CMainFrame::Set_Def_AOI(CRect &rc)
{
	if ( m_DeviceManager.GetCurrentDevice() != NULL )
	{
		try
		{
		
			m_DeviceManager.GetCurrentDevice()->SetAOI(rc);
			m_DeviceManager.GetCurrentDevice()->Refresh();
		}
		CATCH_REPORT();
	}
	

}

void CMainFrame::SetDefaultValue()
{
	CDevice *pDevice = m_DeviceManager.GetCurrentDevice();

    pDevice->m_pShutter->m_Unit.Set((TUnit)1);

 
    pDevice->m_pSnapMode->Set(0);
    pDevice->m_pTriggerPolarity->Set(1);
	pDevice->m_pStrobePolarity->Set(1);
	
	bool b=FALSE;
	if (pDevice->IsContinuousGrabActive()) {
		b = TRUE;
	}
	if (b) {
		pDevice->GrabCancel();
	}
    CurrentDeviceChanged(pDevice);
	if (b){
		pDevice->GrabContinuous();
	}
}

int CMainFrame::GetDeviceNum()
{
	return m_nDeviceNum;
}





















