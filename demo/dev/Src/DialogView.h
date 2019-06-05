//------------------------------------------------------------------------------
// template class CDialogView
// Author: ilovedrv
// Date: 2004.03.12
//------------------------------------------------------------------------------
/**
* \brief  implementation of class CDialogView
*
* 
* 
 */

#if !defined(AFX_DIALOGVIEW_H__23F84F91_DF15_4715_AC28_5F3A1820CD2D__INCLUDED_)
#define AFX_DIALOGVIEW_H__23F84F91_DF15_4715_AC28_5F3A1820CD2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template<class T, class TBase = CWindow>
class CDialogView : public CDialogImpl<T, TBase>  
{
public:
	HWND m_hWndFocus;

	BEGIN_MSG_MAP(CDialogView)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus)
		MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
	END_MSG_MAP()

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		ATLASSERT( GetStyle() & WS_CHILD);
		ModifyStyle(WS_BORDER | WS_CAPTION, 0);
		ShowWindow(SW_NORMAL);
		bHandled = FALSE;
		return 0;
	}

	LRESULT OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		HWND hWndFocus = ::GetFocus();
		if ((hWndFocus != NULL) && (IsChild(m_hWndFocus))) m_hWndFocus = hWndFocus;
		return 0;
	}

	LRESULT OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		// Set focus to last known control
		if (!::IsWindow(m_hWndFocus) || !IsChild(m_hWndFocus)) {
			bHandled = FALSE;
			return 0;
		}
		::SetFocus(m_hWndFocus);
		return 0;
	}

	virtual BOOL PreTanslateMessage(MSG *pMsg)
	{
		HWND hWndFocus = ::GetFocus();
		if ((hWndFocus == NULL) || (!IsChild(m_hWndFocus))) return FALSE;
		if (GetWindow(GW_CHILD)) return FALSE;
		if (IsDialogMessage(pMsg)) return TRUE;
		return FALSE;
	}

};

#endif // !defined(AFX_DIALOGVIEW_H__23F84F91_DF15_4715_AC28_5F3A1820CD2D__INCLUDED_)
