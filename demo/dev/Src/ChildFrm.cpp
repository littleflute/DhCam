// ChildFrm.cpp : implementation of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "ChildFrm.h"
#include "pluginmanagerdlg.h"


LRESULT CChildFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    m_hWndClient = m_View.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);
	
    bHandled = FALSE;
    return 1;
}



LRESULT CChildFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	if (m_hWndClient != NULL && ::IsWindow(m_hWndClient))
		  m_View.DestroyWindow();	  
	
    bHandled = FALSE;
    return 1;
}



LRESULT CChildFrame::OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
    BOOL fShow = (BOOL)wParam;

    bHandled = FALSE;
    return 1;
}



  
LRESULT CChildFrame::OnForwardMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    LPMSG pMsg = (LPMSG)lParam;

    if(CMDIChildWindowImpl<CChildFrame>::PreTranslateMessage(pMsg))
      return TRUE;

    return m_View.PreTranslateMessage(pMsg);
}


 
LRESULT CChildFrame::OnChildActivate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    bHandled = FALSE;
    if ( GetFocus() != m_View )
      m_View.SetFocus();

    return 0;
}

  

LRESULT CChildFrame::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
    bHandled = FALSE;
    m_View.SaveLayout();
    m_DeviceManager.MDIChildClosed(this);


    return 0;
}


  
LRESULT CChildFrame::OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if ( (HWND) lParam == m_hWnd )
    {
      m_DeviceManager.MDIChildActivated(this); 
      m_View.OnActivate( true );
    }
    if ( ( HWND ) wParam == m_hWnd  ){
      m_View.OnActivate( false );
    }

    bHandled = false;
    return 0;
}


void CChildFrame::OnPluginManager()
{
	CPluginManagerDlg dlg(m_PluginManager);
	dlg.DoModal();
}





LRESULT CChildFrame::On_Dh_Test_1(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{	
	int ID=wParam;
	int nVal=lParam;
	
	m_MainFrame.OnPacketSizeChanged(m_pDevice, nVal);

	m_MainFrame.CurrentDeviceChanged(m_DeviceManager.GetCurrentDevice());
	return 0;
}



LRESULT CChildFrame::On_Dh_Test_2(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{	
	WORD ID = wParam;

	CRect rc;
	switch(ID)
	{
	case WM_DH_TEST_2_ID_SET_AOI:
		RECT *pRect;
		pRect = (RECT*)lParam;
		rc.SetRect(pRect->left,pRect->top,pRect->right,pRect->bottom);
		Set_AOI(rc);
		break;
	case WM_DH_TEST_2_ID_GET_SHUTTER_SPEED:
		//...
		int *pV;
		pV = (int*)lParam;
		*pV = 100;
		ATLTRACE("WM_DH_TEST_2_ID_GET_SHUTTER_SPEED Come ...\n");
		break;

	default:
		break;
	}
	return 0;
}

void CChildFrame::Set_AOI(CRect rcAOI)
{
	if ( m_DeviceManager.GetCurrentDevice() != NULL )
	{
		try
		{
		
			m_DeviceManager.GetCurrentDevice()->SetAOI(
				CRect(CPoint(rcAOI.left,rcAOI.top), 
				CSize(rcAOI.right,rcAOI.bottom))
				);
			m_DeviceManager.GetCurrentDevice()->Refresh();
		}
		CATCH_REPORT();
	}

}

void CChildFrame::ReportError(HVBaseException& e)
{
	if ( ! ::IsWindow(m_ErrorBox) )
	{
		m_ErrorBox.Create(m_hWnd);
		m_ErrorBox.ShowWindow(SW_SHOW);
		ATLTRACE(" CChildFrame::ReportError(HVBaseException& e)");
	}
	m_ErrorBox.ReportError(e);
}

