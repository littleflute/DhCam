//-----------------------------------------------------------------------------
//  (c) 2002 by Basler Vision Technologies
//  Section:  Vision Components
//  Project:  BCAM
//  $Header: zoomdlg.h, 4, 10/2/02 2:31:32 PM, Nebelung, H.$
//-----------------------------------------------------------------------------
/**
  \file     zoomdlg.h
 *
 * \brief Dialog to enter user defined zoom scale
 *
 */
//-----------------------------------------------------------------------------

#pragma once


//------------------------------------------------------------------------------
// class CZoomDlg
// Author: 
//------------------------------------------------------------------------------
/**
 * \brief   Dialog to enter user defined zoom scale
 */
//------------------------------------------------------------------------------
class CZoomDlg : public CDialogImpl<CZoomDlg>
{
public:
  enum { IDD = IDD_ZOOM };

  BEGIN_MSG_MAP(CZoomDlg)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_ID_HANDLER(IDOK, OnOK)
    COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
  END_MSG_MAP()

  LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
  {
    CenterWindow(GetParent());
    CString buffer;
    buffer.Format("%ld", (int) (m_dFactor * 100) ); 
    ::SetWindowText(GetDlgItem(IDC_EDIT1), buffer);
    ::SetFocus(GetDlgItem(IDC_EDIT1));
    return TRUE;
  }

  LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
  {
    char buffer[256 * sizeof(TCHAR)];
    ::GetWindowText(GetDlgItem(IDC_EDIT1), buffer, 256);
    m_dFactor = _ttoi(buffer) / 100.0;
    EndDialog(wID);
    return 0;
  }

  LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
  {
    EndDialog(wID);
    return 0;
  }

  void SetFactor(double factor) { m_dFactor = factor; }

  double GetFactor() { return m_dFactor ; }

private:
  double m_dFactor;

};