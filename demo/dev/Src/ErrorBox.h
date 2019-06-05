//-----------------------------------------------------------------------------
//  (c) 2002 by Basler Vision Technologies
//  Section:  Vision Components
//  Project:  BCAM
//  $Header: ErrorBox.h, 4, 10/2/02 2:31:26 PM, Nebelung, H.$
//-----------------------------------------------------------------------------
/**
\file     ErrorBox.h
*
* \brief Interface for the CErrorBox class
*
*/
//-----------------------------------------------------------------------------


#if !defined(AFX_ERRORBOX_H__0D8F7C56_C6C9_11D5_9264_0090278E5E96__INCLUDED_)
#define AFX_ERRORBOX_H__0D8F7C56_C6C9_11D5_9264_0090278E5E96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//------------------------------------------------------------------------------
// class CErrorBox
// Author: 
// Date: 20.09.2002
//------------------------------------------------------------------------------
/**
* \brief   Shows the error message and error code of an BcamException object
*
*/
//------------------------------------------------------------------------------
class CErrorBox  : public CDialogImpl<CErrorBox>
{
public:
	enum {IDD=IDD_ERRORBOX};
	
	BEGIN_MSG_MAP(CErrorBox)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		END_MSG_MAP()
		
		CErrorBox()  {};
	
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	
	void ReportError(HVBaseException& e);
	
	
private:
};

#endif // !defined(AFX_ERRORBOX_H__0D8F7C56_C6C9_11D5_9264_0090278E5E96__INCLUDED_)
