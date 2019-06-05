//-----------------------------------------------------------------------------
//  (c) 2002 by Daheng image Technologies
//  Section:  Vision Components
//  Project:  hvperf
//  $Header: cameradlg.h, 4, 10/2/02 2:31:32 PM, Nebelung, H.$
//-----------------------------------------------------------------------------
/**
  \file     cameradlg.h
 *
 * \brief Dialog to enter user defined camera scale
 *
 */
//-----------------------------------------------------------------------------

#pragma once


//------------------------------------------------------------------------------
// class CCameraDlg
// Author: 
//------------------------------------------------------------------------------
/**
 * \brief   Dialog to enter user defined camera scale
 */
//------------------------------------------------------------------------------
class CCameraDlg : public CDialogImpl<CCameraDlg>
{
public:
  enum { IDD = IDD_CAMERAINFO };

  CCameraDlg(CDevice *pDevice) : m_pDevice(pDevice)
  {
  }

  BEGIN_MSG_MAP(CCameraDlg)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
  END_MSG_MAP()

  LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
  {
    CenterWindow(GetParent());
    ATLASSERT (m_pDevice != NULL);

	InitShutterInfo();
    
    return TRUE;
  }

  LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
  {
    EndDialog(wID);
    return 0;
  }

  LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
  {
    EndDialog(wID);
    return 0;
  }

  void InitShutterInfo()
  {
	CString buffer;
	int unit = m_pDevice->m_pShutter->m_Unit.Value();
	switch(unit) {
	case UNIT_MICROSECOND:
		buffer.Format("%s", "us"); 
		break;
	case UNIT_MILLISECOND:
		buffer.Format("%s", "ms"); 
		break;
	case UNIT_SECOND:
		buffer.Format("%s", "s"); 
		break;
	default:
		break;
	}
    ::SetWindowText(GetDlgItem(IDC_SHUTTERUNIT), buffer);

	buffer.Format("%d", m_pDevice->m_pShutter->Value());
	::SetWindowText(GetDlgItem(IDC_SHUTTERSPEED), buffer);
    
	buffer.Format("%d", m_pDevice->m_pShutter->Raw());
	::SetWindowText(GetDlgItem(IDC_SHUTTERRATIO), buffer);
  }

private:
  CCameraDlg();

private:
	CDevice *m_pDevice;
};