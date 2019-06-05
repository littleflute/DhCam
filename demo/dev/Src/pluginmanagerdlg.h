//-----------------------------------------------------------------------------
//  (c) 2002 by Basler Vision Technologies
//  Section:  Vision Components
//  Project:  BCAM
//  $Header: pluginmanagerdlg.h, 4, 10/2/02 2:31:32 PM, Nebelung, H.$
//-----------------------------------------------------------------------------
/**
  \file     pluginmanagerdlg.h
 *
 * \brief Dialog to enter user defined pluginmanager scale
 *
 */
//-----------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// class CPluginManagerDlg
// Author: 
//------------------------------------------------------------------------------
/**
 * \brief   Dialog to enter user defined pluginmanager scale
 */
//------------------------------------------------------------------------------
class CPluginManagerDlg : public CDialogImpl<CPluginManagerDlg>
{
public:
  enum { IDD = IDD_PLUGINMANAGER };

  BEGIN_MSG_MAP(CPluginManagerDlg)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_ID_HANDLER(IDOK, OnOK)
    COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_ID_HANDLER(IDC_APPLY, OnApply)
	COMMAND_ID_HANDLER(IDC_ITEMUP, OnItemUp)
	COMMAND_ID_HANDLER(IDC_ITEMDOWN, OnItemDown)
	NOTIFY_CODE_HANDLER(LVN_ITEMCHANGED, OnListItemchanged)
	NOTIFY_CODE_HANDLER(NM_CLICK, OnClick)
  END_MSG_MAP()

  CPluginManagerDlg(CPluginManager &PluginManager) : 
	m_PluginManager(PluginManager), 
	m_SelectedIndex(0),
	m_bModified(false)
  {
	m_PluginSet = m_PluginManager.PluginSet();
  }

  LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
  {
    CenterWindow(GetParent());
    
	m_List.SubclassWindow(GetDlgItem(IDC_PLUGINLIST));

	m_List.InsertColumn ( 0, _T("Index"), LVCFMT_LEFT, 100, 0 );
	m_List.InsertColumn ( 1, _T("Name"), LVCFMT_LEFT, 100, 0 );
	
	int index = 0;
	CString buffer;
	CPlugin *pPlugin = NULL;
	for ( std::vector<CPlugin *>::iterator it = m_PluginSet.begin(); it != m_PluginSet.end(); ++ it){
		if ( pPlugin = *it ) {
			buffer.Format("%d", index);
			m_List.InsertItem(index, buffer);
			m_List.SetCheckState(index, pPlugin->IsActive());
			m_List.SetItemText(index, 1, pPlugin->GetName());
			++index;
		}
	}

	m_SelectedIndex = 0;
	m_List.SelectItem(m_SelectedIndex);
	if (m_PluginSet.size() > 0){
		::SetWindowText(GetDlgItem(IDC_PLUGINDESC), m_PluginSet[0]->GetInfo());
	}
	
	m_List.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_List.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );

	CHeaderCtrl hdr = m_List.GetHeader();
	hdr.ModifyStyle(HDS_BUTTONS, 0);

	::EnableWindow(GetDlgItem(IDC_APPLY), false);

    return TRUE;
  }

  LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
  {
	if (m_bModified){
		UpdatePluginState();
	}
    EndDialog(wID);
    return 0;
  }

  LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
  {
    EndDialog(wID);
    return 0;
  }

  LRESULT OnApply(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
  {
	UpdatePluginState();
    m_bModified = false;
	::EnableWindow(GetDlgItem(IDC_APPLY), false);

    return 0;
  }

  LRESULT OnItemUp(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
  {
	if (m_SelectedIndex <= 0)
		return 0;

	CPlugin *pPlugin = NULL;
	pPlugin								= m_PluginSet[ m_SelectedIndex - 1];
	m_PluginSet[m_SelectedIndex - 1]	= m_PluginSet[m_SelectedIndex];
	m_PluginSet[m_SelectedIndex]		= pPlugin;

	m_List.SetItemText(m_SelectedIndex - 1, 1, m_PluginSet[m_SelectedIndex - 1]->GetName());
	m_List.SetItemText(m_SelectedIndex, 1, m_PluginSet[m_SelectedIndex]->GetName());
	
	BOOL flag = m_List.GetCheckState(m_SelectedIndex - 1);
	m_List.SetCheckState(m_SelectedIndex - 1, m_List.GetCheckState(m_SelectedIndex));
	m_List.SetCheckState(m_SelectedIndex, flag);

	m_List.SelectItem(m_SelectedIndex - 1);

	m_bModified = true;  
	::EnableWindow(GetDlgItem(IDC_APPLY), true);
	
	return 0;
  }


  LRESULT OnItemDown(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
  {  
	if ((m_PluginSet.size() <= 0) || (m_SelectedIndex >= (m_PluginSet.size() - 1)))
		return 0;

	CPlugin *pPlugin = NULL;
	pPlugin								= m_PluginSet[ m_SelectedIndex + 1];
	m_PluginSet[m_SelectedIndex + 1]	= m_PluginSet[m_SelectedIndex];
	m_PluginSet[m_SelectedIndex]		= pPlugin;

	m_List.SetItemText(m_SelectedIndex + 1, 1, m_PluginSet[m_SelectedIndex + 1]->GetName());
	m_List.SetItemText(m_SelectedIndex, 1, m_PluginSet[m_SelectedIndex]->GetName());
	
	BOOL flag = m_List.GetCheckState(m_SelectedIndex);
	m_List.SetCheckState(m_SelectedIndex , m_List.GetCheckState(m_SelectedIndex + 1));
	m_List.SetCheckState(m_SelectedIndex + 1, flag);

	m_List.SelectItem(m_SelectedIndex + 1);
	
	m_bModified = true; 
	::EnableWindow(GetDlgItem(IDC_APPLY), true);
	
	return 0;
  }
	
  LRESULT OnListItemchanged(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL/*& bHandled*/)
   {
	  int index = m_List.GetSelectedIndex();

	  if (index >= 0 && index < m_PluginSet.size()){
		::SetWindowText(GetDlgItem(IDC_PLUGINDESC), m_PluginSet[index]->GetInfo());
		m_SelectedIndex = index;
	  }

      return 0;
   }

  LRESULT OnClick(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL/*& bHandled*/)
   {
	  if (m_bModified){
		  return 0;
	  }
	  
	  POINT ptMsg = {0};
	  ::GetCursorPos(&ptMsg);
	  ::ScreenToClient(m_List.m_hWnd, &ptMsg);
	  
	  LVHITTESTINFO lvh;
	  lvh.pt = ptMsg;
	  if(m_List.HitTest(&lvh) != -1 && lvh.flags == LVHT_ONITEMSTATEICON && ::GetKeyState(VK_CONTROL) >= 0){
		m_bModified = true;
		::EnableWindow(GetDlgItem(IDC_APPLY), true);
	  }
	
	  return 0;
   }

private:
	
	void UpdatePluginState()
	{
		int index = 0;
		CPlugin *pPlugin = NULL;
		for ( std::vector<CPlugin *>::iterator it = m_PluginSet.begin(); it != m_PluginSet.end(); ++ it){
			if ( pPlugin = *it ) {
				if (m_List.GetCheckState(index)){
					m_PluginManager.PluginActivate(pPlugin);
				}
				else{
					m_PluginManager.PluginDeactivate(pPlugin);
				}
				++index;
			}
		}
		m_PluginManager.RebuildPluginSet(m_PluginSet);
	}

	CPluginManager	&m_PluginManager;

	CCheckListViewCtrl	m_List;
	std::vector<CPlugin *> m_PluginSet;
	int m_SelectedIndex;
	bool m_bModified;

};