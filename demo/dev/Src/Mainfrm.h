// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__4E614EAB_75B9_4B7A_87FB_DDFFF6B86D50__INCLUDED_)
#define AFX_MAINFRM_H__4E614EAB_75B9_4B7A_87FB_DDFFF6B86D50__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#define APPLICATION_KEY _T("Software\\Daheng\\HVDevice\\")
#define BROADCAST_FPS_MSG "BcamViewerFpsMsg"
#define ENABLE_BROADCAST_KEY "EnableFpsBroadcast"

#define PLUGIN_MENU_MIN			50000

#define	WM_TEST					( WM_USER + 100 )


#include "DeviceManager.h"
#include "BusView.h"
#include "PropertyView.h"
#include "MDIMenuControl.h"
#include "ErrorBox.h"


class CMainFrame : public CMDIFrameWindowImpl<CMainFrame>, public CUpdateUI<CMainFrame>,
public CMessageFilter, public CIdleHandler
{
public:
	int GetDeviceNum();
	bool m_mono8;		//add by xupx  0620 根据插件版本，判断是否激活显示格式的菜单。V1.0版本的不激活。
	void OnPacketSizeChanged(CDevice* pDevice, int value);
	DECLARE_FRAME_WND_CLASS("Daheng Imavision HV Performance", IDR_MAINFRAME)
		
	CMainFrame();
	~CMainFrame();
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();
	
	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_FILE_NEW, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_FILE_OPEN, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_FILE_SAVE, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_EDIT_COPY, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_FILE_CLEAR_REG, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_BUSVIEW,UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_PROPERTYVIEW,UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_CAMERAINFO, UPDUI_MENUPOPUP | UPDUI_TOOLBAR )
		UPDATE_ELEMENT(ID_DEFT_VAL, UPDUI_MENUPOPUP | UPDUI_TOOLBAR )
		UPDATE_ELEMENT(ID_CENTERCURSOR, UPDUI_MENUPOPUP | UPDUI_TOOLBAR )
		UPDATE_ELEMENT(ID_PLUGINMANAGER, UPDUI_MENUPOPUP | UPDUI_TOOLBAR )
		UPDATE_ELEMENT(ID_VIEW_ZOOM_25, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_ZOOM_50, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_ZOOM_100, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_ZOOM_200, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_ZOOM_400, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_ZOOM_USER, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_ZOOM_BEST, UPDUI_MENUPOPUP | UPDUI_TOOLBAR )
		UPDATE_ELEMENT(ID_GRAB_CONTINUOUS, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_TOTAL_CONTINUOUS, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)	//
		UPDATE_ELEMENT(ID_TOTAL_CANCEL, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)	//
		UPDATE_ELEMENT(ID_DO_PASSIVE_TRANS, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_GRAB_SINGLE, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_GRAB_CANCEL, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
        UPDATE_ELEMENT(ID_SOFTTRIGGER, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_CAMERA_MAXIMIZE, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)	
		UPDATE_ELEMENT(ID_VIEW_NO_CONVERSION, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_CONVERSION, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_WINDOW_CASCADE, UPDUI_MENUPOPUP | UPDUI_TOOLBAR )
		UPDATE_ELEMENT(ID_WINDOW_TILE_HORZ, UPDUI_MENUPOPUP | UPDUI_TOOLBAR )
		UPDATE_ELEMENT(ID_WINDOW_TILE_VERT, UPDUI_MENUPOPUP | UPDUI_TOOLBAR )

		
		END_UPDATE_UI_MAP()
		
	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		MESSAGE_HANDLER(WM_ERROR, OnError)   // error when continuously grabbing images

		MESSAGE_HANDLER(WM_TEST, OnTest)   // 

		COMMAND_ID_HANDLER(ID_FILE_CLEAR_REG, OnFileClearReg)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
		COMMAND_ID_HANDLER(ID_EDIT_COPY, OnEditCopy)
		COMMAND_ID_HANDLER(ID_FILE_OPEN, OnFileOpen)
		COMMAND_ID_HANDLER(ID_FILE_SAVE, OnFileSave)
		COMMAND_ID_HANDLER(ID_FILE_SAVE_CONFIGURATION, OnFileSaveConfiguration)
		COMMAND_ID_HANDLER(ID_FILE_RESTORE_CONFIGURATION, OnFileRestoreConfiguration)
		COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
		COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
		COMMAND_ID_HANDLER(ID_VIEW_BUSVIEW, OnViewBusView)
		COMMAND_ID_HANDLER(ID_VIEW_PROPERTYVIEW, OnViewPropertyView)
		COMMAND_ID_HANDLER(ID_CENTERCURSOR, OnCenterCursor)
		
		COMMAND_ID_HANDLER(ID_VIEW_ZOOM_BEST, OnViewZoomBest)
		COMMAND_RANGE_HANDLER(ID_VIEW_NO_CONVERSION, ID_VIEW_CONVERSION, OnViewConversion);
		COMMAND_RANGE_HANDLER(ID_VIEW_ZOOM_50, ID_VIEW_ZOOM_USER, OnViewZoom);
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(ID_APP_HELP, OnAppHelp)
		COMMAND_ID_HANDLER(ID_WINDOW_CASCADE, OnWindowCascade)
		COMMAND_ID_HANDLER(ID_WINDOW_TILE_HORZ, OnWindowTileHorz)
		COMMAND_ID_HANDLER(ID_WINDOW_TILE_VERT, OnWindowTileVert)
		COMMAND_ID_HANDLER(ID_WINDOW_ARRANGE, OnWindowArrangeIcons)
		COMMAND_ID_HANDLER(ID_GRAB_SINGLE, OnGrabSingle)
		COMMAND_ID_HANDLER(ID_GRAB_CONTINUOUS, OnGrabContinuous)
		COMMAND_ID_HANDLER(ID_GRAB_CANCEL, OnGrabCancel)
		COMMAND_ID_HANDLER(ID_TOTAL_CONTINUOUS, OnTotalContinuous)
		COMMAND_ID_HANDLER(ID_TOTAL_CANCEL, OnTotalCancel)
		COMMAND_ID_HANDLER(ID_DO_PASSIVE_TRANS, OnDoPassiveTrans)
		COMMAND_ID_HANDLER(ID_CAMERA_MAXIMIZE, OnDeviceMaximize)
		COMMAND_ID_HANDLER(ID_CAMERAINFO, OnCameraInfo)
		COMMAND_ID_HANDLER(ID_PLUGINMANAGER, OnPluginManager)
        COMMAND_ID_HANDLER(ID_DEFT_VAL, OnSetDefaultValue)
        // CCD camera.
        COMMAND_ID_HANDLER(ID_SOFTTRIGGER, OnSoftTrigger)
		
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CMDIFrameWindowImpl<CMainFrame>)
		CHAIN_DOCK_CHILD_COMMANDS(m_BusView)
		CHAIN_DOCK_CHILD_COMMANDS(m_PropertyView)
	END_MSG_MAP()
		
		// Handler prototypes (uncomment arguments if needed):
		//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
		//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
		//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
		
		
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCommand(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnError(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);

	LRESULT OnTest(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);

	LRESULT OnFileClearReg(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileSaveConfiguration(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileRestoreConfiguration(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  
	LRESULT OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewBusView(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewPropertyView(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	
	LRESULT OnCenterCursor(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAppHelp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	
	LRESULT OnViewConversion(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewZoom(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewZoomBest(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	
	LRESULT OnWindowCascade(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnWindowTileHorz(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnWindowTileVert(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnWindowArrangeIcons(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	
	LRESULT OnGrabSingle(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnGrabContinuous(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnTotalContinuous(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnGrabCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnTotalCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnDoPassiveTrans(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSoftTrigger(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnDeviceMaximize(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCameraInfo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);	
	LRESULT OnPluginManager(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnTimer(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnSetCursor(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnSetDefaultValue(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	/// Report error
	void ReportError(HVBaseException& e);
	/// Calback (change of the current device)
	void CurrentDeviceChanged(CDevice* pDevice);
	
	/// Callback (current device has been reconfigured)
	void ConfigurationChanged(CDevice* pDevice, HV_VIDEO_MODE mode, HV_COLOR_CODE code);
	
	/// Refresh the controls for a given camera object
	void Refresh(CDevice* pDevice);
	
	void UpdateUI();
	
	BOOL KillTimer(UINT nIDEvent)
	{
		ATLASSERT(::IsWindow(m_hWnd));
        if ( m_fBroadCastFps )
            ::PostMessage(HWND_BROADCAST, m_uFpsBroadCastMessage, 0, 0);
		return ::KillTimer(m_hWnd, nIDEvent);
	}

	HMENU GetPluginSubMenu() 
	{ 
		CMenuHandle hMenu =  m_CmdBar.GetMenu();
		return hMenu.GetSubMenu(PLUGIN_MENU_POSITION);
	}
	
	BOOL IsTotalGrabActive()
	{
		CDevice *pDevice;
		BOOL	b = TRUE;

		for(int i = 1; i < m_nDeviceNum + 1; i++)
		{
			pDevice = m_DeviceManager.GetDevice(i);
			if(pDevice!= NULL && !pDevice->IsGrabActive())	// 2009.09.02
			{
				return b = false;
			}
		}	
		return b;
	}

	BOOL IsTotalGrabCancel()
	{
		CDevice *pDevice;
		BOOL	b = TRUE;
		
		for(int i = 1; i < m_nDeviceNum + 1; i++)
		{
			pDevice = m_DeviceManager.GetDevice(i);
			if(pDevice!= NULL&& pDevice->IsGrabActive())	// 2009.09.02
			{
				return b = false;
			}
		}	
		return b;
	}
	
private:
	void SetDefaultValue();
	void Set_Def_AOI(CRect &rc);
	
	CMDICommandBarCtrl		m_CmdBar;
	CMDIMenuControl			m_MenuControl;
	
	CDeviceManager          m_DeviceManager;
	int 					m_nDeviceNum;

	CErrorBox               m_ErrorBox;
	
	// GUI elements
	CMultiPaneStatusBarCtrl m_Sbar;
//	CFlatDockingWindow      m_Dock;
	CDotNetDockingWindow	m_Dock;
	CBusView                m_BusView;
	CPropertyView           m_PropertyView;
	
	bool                    m_fPropertyPageVisible;
	bool					m_bClearReg;
	
	UINT                    m_uFpsBroadCastMessage;  // message ID for fps broadcasts
	bool                    m_fBroadCastFps;         // if true, the fps value will be broadcasted
	long					m_nSaveTimes;            // the number of times to save file
	
	bool					m_bToClose;

	friend class CDevice;   // Device object needs access to the white balance sliders
	friend class CBusView;  // Busview needs access to our command bar to display context menus
	friend class CPropertyView;  // 
	friend class CImageView;// Needs access to command and status bar
	friend class CDeviceManager; // Device manager needs access to the main frame's status bar
	friend class CAutoTest;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__4E614EAB_75B9_4B7A_87FB_DDFFF6B86D50__INCLUDED_)
