//-----------------------------------------------------------------------------
//  (c) 2002 by Basler Vision Technologies
//  Section:  Vision Components
//  Project:  BCAM
//  $Header: ErrorBox.cpp, 3, 10/2/02 2:31:26 PM, Nebelung, H.$
//-----------------------------------------------------------------------------
/**
\file     ErrorBox.cpp
*
* \brief Implementation of the CErrorBox class
*
*/
//-----------------------------------------------------------------------------

// ErrorBox.cpp: implementation of the CErrorBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "ErrorBox.h"


LRESULT CErrorBox::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());
	return TRUE;
}

LRESULT CErrorBox::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DestroyWindow();
	return 0;
}



void CErrorBox::ReportError(HVBaseException &e)
{
	SetActiveWindow();
	MessageBeep(0);
	CString msg;
	CString description = e.Description();
	description.TrimRight();
	
#ifdef _DEBUG
	msg.Format("%s\n%s (0x%08x).", e.Context(), description, e.Error());
#else
	msg.Format("%s (0x%08x).", description, e.Error());
#endif 
	
	::SetWindowText(GetDlgItem(IDC_STATICMESSAGE), msg);
	UpdateWindow();
}