//-----------------------------------------------------------------------------
//  (c) 2002 by Basler Vision Technologies
//  Section:  Vision Components
//  Project:  BCAM
//  $Header: ChildFrm.h, 6, 10/31/02 3:02:26 PM, Albrecht M.$
//-----------------------------------------------------------------------------
/**
  \file     ChildFrm.h
  \brief Interface of the CChildFrame class
 *
 * CChildFrame represents a MDI child windows.
 *
 * Each child frame has a child window, the image view. The image view is responsible
 * for displaying the bitmaps grabbed by the associated camera objects.
 *
 */
//-----------------------------------------------------------------------------


#if !defined(AFX_CHILDFRM_H__8C4B8AD3_7C21_11D5_920C_0090278E5E96__INCLUDED_)
#define AFX_CHILDFRM_H__8C4B8AD3_7C21_11D5_920C_0090278E5E96__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "resource.h"
#include "ImageView.h"
#include "PluginManager.h"
#include "stdafx.h"
//------------------------------------------------------------------------------
// class CChildFrame
// Author: 
// Date: 20.09.2002
//------------------------------------------------------------------------------
/**
 * \brief  CChildFrame represents a MDI child windows.
 *
 * Each MDI child window is associated with a certain camera object. 
 * Each child frame has a child window, the image view. The image view is responsible
 * for displaying the bitmaps grabbed by the associated camera object.
 */
//------------------------------------------------------------------------------
  
class CChildFrame : public CMDIChildWindowImpl<CChildFrame>
{
public:
  DECLARE_FRAME_WND_CLASS(NULL, IDR_MDICHILD)

  CImageView		m_View;
  CString			m_WinName;
  CPluginManager	m_PluginManager;

  CChildFrame(CDevice *pDevice, CDeviceManager& DeviceManager, CMainFrame& MainFrame) : 
    m_pDevice(pDevice),
	m_DeviceManager(DeviceManager),
	m_MainFrame(MainFrame),
    m_View(pDevice, MainFrame),
	m_PluginManager(pDevice, *this, MainFrame)
  { }

  virtual void OnFinalMessage(HWND /*hWnd*/)
  {
	if (m_hMenu != NULL)
		::DestroyMenu(m_hMenu);
    delete this;
  }

  BEGIN_MSG_MAP(CChildFrame)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
	MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
    MESSAGE_HANDLER(WM_CHILDACTIVATE, OnChildActivate)
    MESSAGE_HANDLER(WM_FORWARDMSG, OnForwardMsg)
    MESSAGE_HANDLER(WM_CLOSE, OnClose)
    MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    MESSAGE_HANDLER(WM_MDIACTIVATE, OnActivate)
	MESSAGE_HANDLER(WM_DH_TEST_1, On_Dh_Test_1) 
	MESSAGE_HANDLER(WM_DH_TEST_2, On_Dh_Test_2) 
    CHAIN_MSG_MAP(CMDIChildWindowImpl<CChildFrame>)
  END_MSG_MAP()

  LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
  LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
  LRESULT OnShowWindow(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
  LRESULT OnForwardMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
  LRESULT OnChildActivate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
  LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled);
  LRESULT OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT On_Dh_Test_1(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
  LRESULT On_Dh_Test_2(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);

public:
	// process requests from mainframe
	void OnPluginManager();				// manage all plugins states
	/// Report error
	void ReportError(HVBaseException& e);

private:
	
	friend class CDeviceManager;
	friend class CPluginManager;

private:
	void Set_AOI(CRect rcAOI);
	CDevice		*m_pDevice; 
	CMainFrame& m_MainFrame;
	CDeviceManager& m_DeviceManager;
	CErrorBox               m_ErrorBox;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__8C4B8AD3_7C21_11D5_920C_0090278E5E96__INCLUDED_)
